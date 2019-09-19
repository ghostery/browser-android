#!/usr/bin/env groovy

def buildBrowser(
        String target="i686-linux-android",
        int minSdk=16,
        String brand="ghostery",
        String buildType="ci",
        String workspace="."
    ) {
    def arch = ""
    switch (target) {
        case ~/.*aarch64.*/:
            arch = "arm64"
            break
        case ~/.*arm.*/:
            arch = "arm"
            break
        case ~/.*x86_64.*/:
            arch = "x86_64"
            break
        case ~/.*i686.*/:
        default:
            arch = "x86"
            break
    }
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
        "ANDROID_MIN_SDK=${minSdk}",
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
            withEnv(["WORKSPACE=${workspace}", "BRAND=${brand}"]){
                sh '''#!/bin/bash -l
                    set -x
                    set -e
                    echo "*** Build Language Packs ***"
                    cd ${WORKSPACE}
                    export MOZ_CHROME_MULTILOCALE=`ls -1 l10n|paste -s -d " "`
                    cd mozilla-release
                    rm -f objdir-frontend-android/${BRAND}/dist/bin/defaults/pref/mobile-l10n.js
                    cp mobile/android/installer/mobile-l10n.js objdir-frontend-android/${BRAND}/dist/bin/defaults/pref/mobile-l10n.js
                    for AB_CD in $MOZ_CHROME_MULTILOCALE; do
                        ./mach build chrome-$AB_CD
                    done
                    export AB_CD=multi
                    ./mach package
                    echo "*** DONE ***"
                '''
            }
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
            script: """cd ${workspace}/mozilla-release/objdir-frontend-android/${brand}/dist && \
            find *.apk -not -name 'robo*' -not -name '*-unsigned-*'""").trim()
        apkFulllink = "${workspace}/mozilla-release/objdir-frontend-android/${brand}/dist/${apkName}"
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
                script: """cd ${workspace}/mozilla-release/objdir-frontend-android/${brand}/gradle/build/mobile/android/app/outputs/apk/withGeckoBinaries/release && \
                find *.apk -not -name 'robo*' -not -name '*-unsigned-*'""").trim()
            apkFulllink = "${workspace}/mozilla-release/objdir-frontend-android/${brand}/gradle/build/mobile/android/app/outputs/apk/withGeckoBinaries/release/${apkName}"
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

def rSyncRepo(String orig, String copy) {
    sh """#!/bin/bash -l
        set -e
        set -x
        rsync -r --exclude '.git' --exclude '.idea' --exclude '.tx' ${orig} ${copy}
    """
}

def buildCliqzReactNative(String workspace=".") {
    sh """#!/bin/bash -l
        set -e
        set -x
        cd ${workspace}
        rm -rf node_modules
        npm ci
        npm run build
    """
}

def runAppiumTests(String testsFolder, String brand, String apk) {
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
            $ANDROID_HOME/platform-tools/adb uninstall ${appPackage} || true
            $ANDROID_HOME/platform-tools/adb install "$app"
            cd $TESTS_FOLDER
            appium --log $FLAVOR-appium.log &
            APPIUM_PID=$!
            sleep 10
            virtualenv ~/venv
            source ~/venv/bin/activate
            chmod 0755 requirements.txt
            pip install -r requirements.txt
            python testRunner.py || true
            $ANDROID_HOME/platform-tools/adb uninstall ${appPackage}
            sleep 10
            $ANDROID_HOME/platform-tools/adb forward --remove-all
            kill $APPIUM_PID || true
            echo "*** DONE ***"
        '''
    }
    try {
        archiveArtifacts allowEmptyArchive: true, artifacts: "${testsFolder}/*.log"
        junit "${testsFolder}/test-reports/*.xml"
        zip archive: true, dir: "${testsFolder}/screenshots", glob: '', zipFile: "Appium-${brand}-screenshots.zip"
    } catch(e) {
        print e
    }
}

def runUITests(String brand){
    withEnv([
        "ANDROID_TARGET=i686-linux-android",
        "BRAND=${brand}",
        "CLIQZ_CHANNEL=MA99"
        ]){
        sh'''#!/bin/bash -l
            set -x
            echo "*** Starting UI Tests ***"
            cd mozilla-release
            $ANDROID_HOME/platform-tools/adb shell mkdir /sdcard/rec
            $ANDROID_HOME/platform-tools/adb shell """
                screenrecord --bit-rate 6000000 /sdcard/rec/1.mp4;
                screenrecord --bit-rate 6000000 /sdcard/rec/2.mp4;
                screenrecord --bit-rate 6000000 /sdcard/rec/3.mp4;
                screenrecord --bit-rate 6000000 /sdcard/rec/4.mp4;
                screenrecord --bit-rate 6000000 /sdcard/rec/5.mp4; """ &
            RECORDING_PID=$!
            $ANDROID_HOME/platform-tools/adb logcat -c
            $ANDROID_HOME/platform-tools/adb logcat > ../UIA-device.log &
            LOGCAT_PID=$!
            ./gradlew app:connectedWithGeckoBinariesDebugAndroidTest || true
            kill $RECORDING_PID
            sleep 3
            kill $LOGCAT_PID
            $ANDROID_HOME/platform-tools/adb logcat -c
            mkdir -p ../recording ../screenshots
            $ANDROID_HOME/platform-tools/adb pull /sdcard/rec ../recording/
            $ANDROID_HOME/platform-tools/adb pull /sdcard/test-screenshots ../screenshots/ || true
            $ANDROID_HOME/platform-tools/adb uninstall ${appPackage} || true
            echo "*** DONE ***"
        '''
        try {
            archiveArtifacts allowEmptyArchive: true, artifacts: "mozilla-release/objdir-frontend-android/${brand}/gradle/build/mobile/android/app/reports/androidTests/connected/flavors/WITHGECKOBINARIES/**/*"
            archiveArtifacts allowEmptyArchive: true, artifacts: "UIA-device.log"
            junit "mozilla-release/objdir-frontend-android/${brand}/gradle/build/mobile/android/app/outputs/androidTest-results/connected/flavors/WITHGECKOBINARIES/*.xml"
            zip archive: true, dir: "mozilla-release/objdir-frontend-android/${brand}/gradle/build/mobile/android/app/reports/androidTests/connected/flavors/WITHGECKOBINARIES/", glob: '', zipFile: "${brand}-reports.zip"
            zip archive: true, dir: "screenshots/", glob: '', zipFile: "${brand}-UIA-screenshots.zip"
            zip archive: true, dir: "recording/", glob: '', zipFile: "${brand}-UIA-videos.zip"
        } catch (e) {
            print e
        }
    }
}

return this
