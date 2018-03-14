#!/bin/env groovy

node('us-east-1 && ubuntu && docker && !gpu') {

    def hash = ""
    
    stage('Checkout') {
        checkout scm
        hash = sh(returnStdout: true, script: """git log refs/remotes/origin/upstream-releases --format="%H" -n 1""").trim()
    }
    sh "`aws ecr get-login --region=us-east-1 --no-include-email`"
    def dockerTag = "${hash}"
    def baseImageName = "browser-f/android:${dockerTag}"
    docker.image("141047255820.dkr.ecr.us-east-1.amazonaws.com/${baseImageName}").inside {
        try {
            stage('Build APK') {
                sh '''#!/bin/bash -l
                    echo $SHELL
                    cd mozilla-release
                    mv mozconfig.txt mozconfig
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
                    rm -rf mozilla-release/objdir-frontend-android
                '''
            }
        }
    }
}