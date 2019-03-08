#!/usr/bin/env groovy

def fastlaneRelease(
    String certPathCredId,
    String certPassCredId,
    String playStoreCertCredId
    ) {
    withCredentials([
        file(credentialsId: certPathCredId, variable: 'CERT_PATH'),
        string(credentialsId: certPassCredId, variable: 'CERT_PASS'),
        file(credentialsId: playStoreCertCredId, variable: 'PLAY_STORE_CERT')]) {
        sh """#!/bin/bash -l
            set -e
            set -x
            cd mozilla-release/mobile/android
            mv fastlane/Appfile.${brand} fastlane/Appfile
            fastlane alpha
        """
    }
}

def cleanUp(String testsFolder="") {
    sh """#!/bin/bash
        rm -f mozilla-release/mozconfig
        rm -rf mozilla-release/objdir-frontend-android
        rm -rf build
        rm -rf *.zip
        rm -rf recording screenshots
        rm -rf *.log
    """
    if (testsFolder != ""){
        sh """#!/bin/bash
            rm -rf ${testsFolder}
        """
    }
}

def s3Copy(String apkFullPath, String s3Path, String fileName, String buildArch, Boolean latestLink) {
    if (buildArch != ""){
        buildArch = buildArch.equals("arm") ? "" : "_${buildArch}"
    }
    if (latestLink){
        sh """#!/bin/bash -l
            set -e
            set -x
            aws s3 cp --acl public-read --acl bucket-owner-full-control ${apkFullPath} ${s3Path}/${fileName}${buildArch}.apk
            aws s3 cp --acl public-read --acl bucket-owner-full-control ${apkFullPath} ${s3Path}/latest${buildArch}.apk
        """
    } else {
        sh """#!/bin/bash -l
            set -e
            set -x
            aws s3 cp --acl public-read --acl bucket-owner-full-control ${apkFullPath} ${s3Path}/${fileName}${buildArch}.apk
        """
    }
}

def s3rm(String s3FullPath) {
    sh """#!/bin/bash -l
        aws s3 rm ${s3FullPath}
    """
}