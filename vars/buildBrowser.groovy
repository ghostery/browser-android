#!/usr/bin/env groovy

def buildBrowser(String target, String channel, String flavor) {
    withEnv([
        "ANDROID_TARGET=${target}",
        "BRAND=${flavor}",
        "CLIQZ_CHANNEL=${channel}"
        ]){
        sh "ls && bash +xe ../build-system/magic.sh"
    }
}

return this