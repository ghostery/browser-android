#!/usr/bin/env groovy

def buildBrowser(
        String target,
        String brand,
        String buildType="ci",
        String workspace="."
    ) {
    def arch = target.contains('arm') ? "arm" : "x86"
    def packLocale = ""
    def abcd = ""
    def channel = "MA99"
    def mozconfigFile = ""
    def apkFulllink = ""
    def apkName = ""
    channel = brand.contains('cliqz') ? "MA0" : "MA5"
    switch (buildType) {
        case "release":
            channel += "0"
            packLocale = "multi"
            mozconfigFile = "deploy.mozconfig"
            break
        case "nightly":
            channel += "2"
            packLocale = "multi"
            mozconfigFile = "deploy.mozconfig"
            break
        case "artifact":
            channel = "MA99"
            mozconfigFile = "artifact.mozconfig"
        default:
            channel = "MA99"
            mozconfigFile = "jenkins.mozconfig"
    }
    channel = brand.contains('alpha') ? "MA99" : channel
    withEnv([
        "ANDROID_TARGET=${target}",
        "BRAND=${brand}",
        "CLIQZ_CHANNEL=${channel}"
        ]){
        sh """#!/bin/bash -l
            set -x
            set -e
            echo '*** Copy the required Mozconfig ***'
            cd ${workspace}
            cp mozconfigs/${mozconfigFile} mozilla-release/mozconfig
            cat mozilla-release/mozconfig
            echo '*** DONE ***'
        """
        sh """#!/bin/bash -l
            set -x
            set -e
            echo '*** Build and Package the Browser ***'
            cd ${workspace}
            cd mozilla-release
            rm -rf objdir-frontend-android
            ./mach clobber
            ./mach build
        """
        if(packLocale == "multi"){
            sh """#!/bin/bash -l
                set -x
                set -e
                echo '*** Build Language Packs ***'
                cd ${workspace}
                export MOZ_CHROME_MULTILOCALE=`ls -1 l10n|paste -s -d " "`
                cd mozilla-release
                rm -f objdir-frontend-android/${brand}/dist/bin/defaults/pref/mobile-l10n.js
                cp mobile/android/installer/mobile-l10n.js objdir-frontend-android/${brand}/dist/bin/defaults/pref/mobile-l10n.js
                for AB_CD in $MOZ_CHROME_MULTILOCALE; do
                    ./mach build chrome-$AB_CD
                done
                export AB_CD=multi
                ./mach package
                echo '*** DONE ***'
            """
        } else {
            sh """#!/bin/bash -l
                set -x
                set -e
                cd ${workspace}
                cd mozilla-release
                ./mach package
                echo '*** DONE ***'
            """
        }
        apkName = sh(returnStdout: true,
            script: """cd mozilla-release/objdir-frontend-android/${brand}/dist && \
            find *.apk -not -name 'robo*' -not -name '*-unsigned-*'""").trim()
        apkFulllink = "mozilla-release/objdir-frontend-android/${brand}/dist/${apkName}"
        if ((buildType == "nightly") || (buildType == "release")){
            sh """#!/bin/bash -l
                set -x
                set -e
                echo '*** Repackage using Gradle ***'
                cd ${workspace}
                cd mozilla-release
                ./gradlew :app:assembleWithGeckoBinariesRelease
                echo '*** DONE ***'
            """
            apkName = sh(returnStdout: true,
                script: """cd mozilla-release/objdir-frontend-android/${brand}/gradle/build/mobile/android/app/outputs/apk/WithGeckoBinariesRelease/release && \
                find *.apk -not -name 'robo*' -not -name '*-unsigned-*'""").trim()
            apkFulllink = "mozilla-release/objdir-frontend-android/${brand}/gradle/build/mobile/android/app/outputs/apk/WithGeckoBinariesRelease/release/${apkName}"
        }
        sh """#!/bin/bash -l
            set -x
            set -e
            echo '*** Copy APK File to Build folder ***'
            cd ${workspace}
            mkdir -p build
            cp ${apkFulllink} build/${brand}_${arch}.apk
            echo '*** DONE !! Build: build/${brand}_${arch}.apk ***'
        """
        return "${brand}_${arch}.apk"
    }
}

def buildFreshTab(String workspace=".") {
    sh """#!/bin/bash -l
        set -e
        set -x
        cd ${workspace}
        rm -rf node_modules
        npm ci
        npm run build
    """
}

def runAppiumTests(String testsFolder, String apk) {
    withEnv([
        "TESTS_FOLDER=${testsFolder}",
        "platformName=android",
        "deviceName=127.0.0.1:5556",
        "appActivity=org.mozilla.gecko.LauncherActivity",
        "MODULE=testCompleteSuite",
        "TEST=CompleteSuite",
        "TEST_TYPE=smoke",
        "APP=${apk}"
        ]){
        sh '''#!/bin/bash -l
            set -x
            set -e
            echo "*** Starting Tests ***"
            export app="`pwd`/build/$APP"
            $ANDROID_HOME/platform-tools/adb install "$app"
            cd $TESTS_FOLDER
            appium --log $FLAVOR-appium.log &
            echo $! > appium.pid
            sleep 10
            virtualenv ~/venv
            source ~/venv/bin/activate
            chmod 0755 requirements.txt
            pip install -r requirements.txt
            python testRunner.py || true
            $ANDROID_HOME/platform-tools/adb uninstall ${appPackage}
            sleep 10
            $ANDROID_HOME/platform-tools/adb forward --remove-all
            kill `cat appium.pid` || true
            rm -f appium.pid
            echo "*** DONE ***"
        '''
    }
}

return this