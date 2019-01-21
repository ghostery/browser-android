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
    /*'cliqz-alpha':[
        'bundleid': 'com.cliqz.browser.alpha',
        'target': 'arm-linux-androideabi',
        'test': false,
    ],*/
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
                test,                       // Boolean value for Running Tests
                "ami-6c24fc11",             // Amazon AWS AMI ID
                "1",                        // Count, Number of Instances
                "t2.medium",                // Instance Size
                "android_ci_genymotion",    // RSA Key
                "sg-5bbf173f",              // Secutiry Group ID of AWS
                "subnet-341ff61f",          // Subnet ID for the instance
                "us-east-1"                 // AWS Region
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
                                cliqz.buildCliqzReactNative("cliqz")            // Pass the Folder Name for the React Native SRC
                            }
                            stage("Build APK: ${flavorname}") {
                                apk = cliqz.buildBrowser(
                                    "${androidtarget}",     // Target for the build
                                    "${flavorname}",        // Name of the Flavor
                                    "ci"                    // Type of the Build (CI, Nightly or Release)
                                    )
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
                                    stage('Set Genymotion Resolution and Connect') {
                                        genymotion.genySetPhoneResolution('da5f91e6-e1ca-4aac-94ea-352b6769228b')           // Pass the Credentials ID for the AWS API Key
                                        genymotion.connectGenyInstance('da5f91e6-e1ca-4aac-94ea-352b6769228b')              // Pass the Credentials ID for the AWS API Key
                                    }
                                    stage("Run UIA2 Tests & Upload Results: ${flavorname}"){
                                        timeout(10){
                                            cliqz.runUITests("${flavorname}")           // Pass the Flavor Name for running the UIAutomator Tests
                                        }
                                    }
                                    stage("Run Appium Tests & Upload Results: ${flavorname}") {
                                        timeout(60) {
                                            cliqz.runAppiumTests(
                                                "${testsFolder}",           // Path to the Folder where the Tests are cloned.
                                                "${flavorname}",            // Flavor Name for running the UIAutomator Tests
                                                "${apk}"                    // Path to the APK used for testing.
                                                )
                                       }
                                    }
                                }
                            }
                        }
                    }
                } catch(e) {
                    print e
                    error 'Something Failed ! Check Logs above.'
                } finally {
                    stage('Clean Up') {
                        utils.cleanUp("${testsFolder}")     // Path to the Folder where the Tests are cloned. This will delete the Tests Folder.
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
