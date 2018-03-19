#!/bin/env groovy

node('us-east-1 && ubuntu && docker && !gpu') {

    def hash = ""
    
    stage('Checkout') {
        checkout scm
        hash = sh(returnStdout: true, script: """git log refs/remotes/origin/upstream-releases --format="%H" -n 1""").trim()
    }
    def dockerTag = "${hash}"
    def baseImageName = "browser-f/android:${dockerTag}"
    docker.image("141047255820.dkr.ecr.us-east-1.amazonaws.com/${baseImageName}").inside {
        try {
            withEnv(["CPU_ARCH=x86", "GRE_MILESTONE=59.0"]) {
                stage('Build APK') {
                    sh '''#!/bin/bash -l
                        set -e
                        set -x
                        cd mozilla-release
                        mv mozconfig.txt mozconfig
                        ./mach build
                        ./mach package
                    '''
                }
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