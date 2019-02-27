#!/usr/bin/env groovy

def buildBrowser(String target, String channel, String flavor, String buildType="ci") {
    withEnv([
        "BUILD_TYPE=${buildType}",
        "ANDROID_TARGET=${target}",
        "BRAND=${flavor}",
        "CLIQZ_CHANNEL=${channel}"
        ]){
        def script = libraryResource 'magic.sh'
        sh """${script}"""
    }
}

def runAppiumTests(String testsFolder) {
    withEnv([
        "TESTS_FOLDER=${testsFolder}"
        ]){
        def script = libraryResource 'run-tests.sh'
        sh """${script}"""
    }
}

return this