#!/usr/bin/env groovy

def buildBrowser(String target, String channel, String flavor) {
    withEnv([
        "ANDROID_TARGET=${target}",
        "BRAND=${flavor}",
        "CLIQZ_CHANNEL=${channel}"
        ]){
        def script = libraryResource 'build-system/magic.sh'
        sh "bash +xe ${script}"
    }
}

return this