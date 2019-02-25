#!/usr/bin/env groovy

def buildBrowser(target, channel, flavor) {
    withEnv([
        "ANDROID_TARGET=${target}",
        "BRAND=${flavor}",
        "CLIQZ_CHANNEL=${channel}"
        ]){
        sh 'bash +xe ../build-system/magic.sh'
    }
}