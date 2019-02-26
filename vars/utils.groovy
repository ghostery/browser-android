#!/usr/bin/env groovy

def archiveTestResults(String flavorName, String testsFolder='cliqz-mobile-tests') {
    try {
        archiveArtifacts allowEmptyArchive: true, artifacts: "${testsFolder}/*.log"
        junit "${testsFolder}/test-reports/*.xml"
        zip archive: true, dir: "${testsFolder}/screenshots", glob: '', zipFile: "${flavorName}-screenshots.zip"
    } catch(e) {
        print e
    }
}

def cleanUp(String testsFolder="") {
    sh """#!/bin/bash
        rm -f mozilla-release/mozconfig
        rm -rf mozilla-release/objdir-frontend-android
    """
    if (testsFolder != ""){
        sh """#!/bin/bash
            rm -rf ${testsFolder}
        """
    }
}

def s3copy(String apkFullPath, String s3Path, String fileName, String buildArch="") {
    if (buildArch=="x86"){
        sh """#!/bin/bash -l
            set -e
            set -x
            aws s3 cp --acl public-read --acl bucket-owner-full-control ${apkFullPath} ${s3Path}/${fileName}_${buildarch}.apk
            aws s3 cp --acl public-read --acl bucket-owner-full-control ${apkFullPath} ${s3Path}/latest_${buildarch}.apk
        """
    } else {
        sh """#!/bin/bash -l
            set -e
            set -x
            aws s3 cp --acl public-read --acl bucket-owner-full-control ${apkFullPath} ${s3Path}/${fileName}.apk
            aws s3 cp --acl public-read --acl bucket-owner-full-control ${apkFullPath} ${s3Path}/latest.apk
        """
    }
}

def s3rm(String s3FullPath) {
    sh """#!/bin/bash -l
        aws s3 rm ${s3FullPath}
    """
}