#!/bin/env groovy
@Library(['cliqz-shared-library@v1.2', 'browser-android@automation']) _
def matrix = [
    'cliqz':[
        'bundleid': 'com.cliqz.browser',
        'target': 'i686-linux-android',
        'test': false,
    ],
    'ghostery':[
        'bundleid': 'com.ghostery.android.ghostery',
        'target': 'i686-linux-android',
        'test': true,
    ],
    'cliqz-alpha':[
        'bundleid': 'com.cliqz.browser.alpha',
        'target': 'arm-linux-androideabi',
        'test': false,
    ],
    'ghostery-alpha':[
        'bundleid': 'com.ghostery.android.alpha',
        'target': 'arm-linux-androideabi',
        'test': false,
    ],
]

def build(Map m){
    def androidtarget = m.target
    def flavorname = m.name
    def bundleid = m.bundleid
    def nodeLabel = 'us-east-1 && ubuntu && docker && !gpu'
    def test = m.test
    return {
        node(nodeLabel){
            def dockerTag = ""
            def apk = ""
            def testsFolder = "cliqz-mobile-tests"
            setupTestInstance(
                test,
                "ami-6c24fc11",
                "1",
                "t2.medium",
                "android_ci_genymotion",
                "sg-5bbf173f",
                "subnet-341ff61f",
                "us-east-1"
            ) {
                try {
                    stage('Checkout') {
                        checkout scm
                        dockerTag = readFile('mozilla-release/browser/config/version.txt').trim()
                    }
                    def baseImageName = "browser-f/android:${dockerTag}"
                    docker.withRegistry('https://141047255820.dkr.ecr.us-east-1.amazonaws.com') {
                        def image = docker.image(baseImageName)
                        image.pull()
                        image.inside {
                            stage('Build Cliqz React Native') {
                                cliqz.buildCliqzReactNative("cliqz")
                            }
                            stage("Build APK: ${flavorname}") {
                                apk = cliqz.buildBrowser("${androidtarget}", "${flavorname}", "ci")
                                archiveArtifacts allowEmptyArchive: true, artifacts: "build/${apk}"
                            }
                        }
                        if (test == true){
                            stage('Checkout Mobile Tests') {
                                dir("${testsFolder}"){
                                    git branch:'master',
                                    credentialsId: 'cliqz-oss-ci',
                                    url: 'https://github.com/cliqz-oss/cliqz-mobile-tests.git'
                                }
                            }
                            stage("Check Genymotion Status") {
                                genymotion.genyInstanceStatus()
                            }
                            docker.image("${baseImageName}").inside {
                                withEnv([
                                    "FLAVOR=${flavorname}",
                                    "appPackage=${bundleid}"
                                ]) {
                                    stage('Genymotion ADB Connect') {
                                        genymotion.connectGenyInstance('da5f91e6-e1ca-4aac-94ea-352b6769228b')
                                    }
                                    stage("Run Tests & Upload Results: ${flavorname}") {
                                        timeout(60) {
                                            cliqz.runAppiumTests("${testsFolder}", "${apk}")
                                       }
                                    }
                                }
                            }
                        }
                    }
                } catch(e) {
                    print e
                    raise e
                } finally {
                    stage('Clean Up') {
                        utils.cleanUp("${testsFolder}")
                    }
                }
            }
        }
    }
}


def stepsForParallelBuilds = helpers.entries(matrix).collectEntries{
    [("Building ${it[0]}"):build(
        name: it[0],
        bundleid: it[1]['bundleid'],
        target:it[1]['target'],
        test:it[1]['test']
    )]
}

parallel stepsForParallelBuilds


def setupTestInstance(
        Boolean inTests,
        String instanceImage,
        String count,
        String instanceType,
        String keyName,
        String securityGroup,
        String subnetId,
        String region,
        Closure body
    ) {
    if (inTests == true) {
        genymotion.withGenymotion(
            instanceImage,
            count,
            instanceType,
            keyName,
            securityGroup,
            subnetId,
            region
        ) {
            body()
        }
    } else {
        body()
    }
}
