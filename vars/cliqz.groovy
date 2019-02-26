#!/usr/bin/env groovy

def buildBrowser(String buildType, String target, String channel, String flavor) {
    withEnv([
        "BUILD_TYPE=${buildType}",
        "ANDROID_TARGET=${target}",
        "BRAND=${flavor}",
        "CLIQZ_CHANNEL=${channel}"
        ]){
        def script = libraryResource 'magic.sh'
        sh "bash +xe ${script}"
    }
}

def runAppiumTests(String testsFolder) {
    withEnv([
        "TESTS_FOLDER=${testsFolder}"
        ]){
        def script = libraryResource 'run-tests.sh'
        sh "bash +xe ${script}"
    }
}

return this