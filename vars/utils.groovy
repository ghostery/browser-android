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

def cleanUp(String testsFolder='cliqz-mobile-tests') {
    sh """#!/bin/bash
        rm -f mozilla-release/mozconfig
        rm -rf mozilla-release/objdir-frontend-android
        rm -rf ${testsFolder}
    """
}