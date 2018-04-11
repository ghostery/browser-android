#!/bin/env groovy

node('us-east-1 && ubuntu && docker && !gpu') {

    def hash = ""
    
    stage('Checkout') {
        checkout scm
        def delim = '"'
        hash = sh(returnStdout: true, script: """curl https://api.github.com/repos/cliqz-oss/cliqz-android/branches/upstream-releases | grep -m 1 sha | cut -d '${delim}' -f 4""").trim()
    }
    def dockerTag = "${hash}"
    def baseImageName = "browser-f/android:${dockerTag}"
    docker.image("141047255820.dkr.ecr.us-east-1.amazonaws.com/${baseImageName}").inside {
        try {
            stage('Build APK') {
                sh '''#!/bin/bash -l
                    set -e
                    set -x
                    cd mozilla-release
                    mv ../mozconfigs/cliqz.mozconfig mozconfig
                    ./mach build
                    ./mach package
                '''
            }
            stage('Upload APK') {
                archiveArtifacts allowEmptyArchive: true, artifacts: 'mozilla-release/objdir-frontend-android/dist/*.apk'
            }
        }
        finally {
            stage('Clean Up') {
                sh '''#!/bin/bash -l
                    rm -f mozilla-release/mozconfig
                    rm -rf mozilla-release/objdir-frontend-android
                '''
            }
        }
    }
}
