#!/bin/env groovy
@Library('cliqz-shared-library@v1.2') _
def matrix = [
    'cliqz':[
        'bundleid': 'com.cliqz.browser',
        'target': 'i386-linux-android',
        'test': false,
    ],
    'ghostery':[
        'bundleid': 'com.ghostery.android.ghostery',
        'target': 'i386-linux-android',
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
            setupTestInstance(
                test,
                "ami-07457b491395bb595",
                "1",
                "t2.medium",
                "android_ci_genymotion",
                "sg-0e66fea9f33f2a661",
                "subnet-0d577755",
                "us-east-1"
            ) {
                stage('Checkout') {
                    checkout scm
                    dockerTag = readFile('mozilla-release/browser/config/version_display.txt').trim()
                }
                def baseImageName = "browser-f/android:${dockerTag}"
                docker.withRegistry('https://092783680059.dkr.ecr.us-east-1.amazonaws.com') {
                    docker.image("${baseImageName}").inside {
                        stage('Download cache') {
                            withCredentials([[
                                    $class: 'AmazonWebServicesCredentialsBinding',
                                    accessKeyVariable: 'AWS_ACCESS_KEY_ID',
                                    credentialsId: 'd7e38c4a-37eb-490b-b4da-2f53cc14ab1b',
                                    secretKeyVariable: 'AWS_SECRET_ACCESS_KEY']]) {
                                def s3Path = "s3://repository.ghostery.net/dist/android/cache"
                                def cachePath = ".gradle/caches"
                                sh """#!/bin/bash -l 
                                    pip install awscli --upgrade --user
                                    cd
                                    aws s3 sync --acl public-read --acl bucket-owner-full-control ${s3Path} ${cachePath}
                                """
                            }
                        }
                        stage("Build APK: ${flavorname}") {
                            withEnv([
                                "ANDROID_TARGET=${androidtarget}",
                                "BRAND=${flavorname}",
                                "CLIQZ_CHANNEL=MA99"
                                ]) {
                                sh '''#!/bin/bash -l
                                    set -x
                                    set -e
                                    cp mozconfigs/jenkins.mozconfig mozilla-release/mozconfig
                                    cd mozilla-release
                                    ./mach clobber
                                    ./mach build
                                    for lang in `ls ../l10n/`; do
                                        ./mach build chrome-$lang
                                    done
                                    ./mach package
                                '''
                            }
                            apk = sh(returnStdout: true,
                                script: """cd mozilla-release/objdir-frontend-android/${flavorname}/dist && \
                                find *.apk -not -name 'robo*' -not -name '*-unsigned-*'""").trim()
                        }
                        stage("Upload APK: ${flavorname}") {
                            archiveArtifacts allowEmptyArchive: true, artifacts: "mozilla-release/objdir-frontend-android/${flavorname}/dist/${apk}"
                        }
                    }
                    if (test == true){
                        try {
                            stage('Checkout Autobots') {
                                dir('autobots'){
                                    git branch:'version2.0',
                                    credentialsId: 'cliqz-ci',
                                    url: 'https://github.com/cliqz/autobots.git'
                                }
                            }
                            timeout(10) {
                                stage('Genymotion Status') {
                                    def status = sh(returnStdout: true, script: """
                                            aws ec2 describe-instance-status \
                                            --region=$REGION \
                                            --instance-id $INSTANCE_ID \
                                            --query 'InstanceStatuses[].InstanceStatus[].Details[].Status' \
                                            --output text
                                        """).trim()
                                    while (status != 'passed') {
                                        println "Waiting for the instance to fully Boot up...."
                                        sleep(15)
                                        status = sh(returnStdout: true, script: """
                                            aws ec2 describe-instance-status \
                                            --region=$REGION \
                                            --instance-id $INSTANCE_ID \
                                            --query 'InstanceStatuses[].InstanceStatus[].Details[].Status' \
                                            --output text
                                        """).trim()
                                        println "Instance Status: ${status}"
                                    }
                                }
                            }
                            docker.image("${baseImageName}").inside {
                                withEnv([
                                    "APP=${apk}",
                                    "FLAVOR=${flavorname}",
                                    "platformName=android",
                                    "deviceName=127.0.0.1:5556",
                                    "MODULE=testCompleteSuite",
                                    "TEST=CompleteSuite",
                                    "appPackage=${bundleid}",
                                    "appActivity=org.mozilla.gecko.LauncherActivity",
                                    "TEST_TYPE=smoke"
                                    ]) {
                                    withCredentials([file(credentialsId: 'da5f91e6-e1ca-4aac-94ea-352b6769228b', variable: 'FILE' )]) {
                                        stage('Genymotion ADB Connect') {
                                            sh'''#!/bin/bash -l
                                                set -x
                                                set -e
                                                chmod 400 $FILE
                                                ssh -v -o StrictHostKeyChecking=no -i $FILE shell@$IP "setprop persist.sys.usb.config adb"
                                                ssh -v -o StrictHostKeyChecking=no -i $FILE -NL 5556:127.0.0.1:5555 shell@$IP &
                                                $ANDROID_HOME/platform-tools/adb connect 127.0.0.1:5556
                                                $ANDROID_HOME/platform-tools/adb wait-for-device
                                            '''
                                        }
                                    }
                                    stage("Run Tests: ${flavorname}") {
                                        timeout(60) {
                                            sh'''#!/bin/bash -l
                                                set -x
                                                set -e
                                                export app=$PWD/mozilla-release/objdir-frontend-android/$FLAVOR/dist/$APP
                                                $ANDROID_HOME/platform-tools/adb install $app
                                                cd autobots
                                                appium --log $FLAVOR-appium.log &
                                                echo $! > appium.pid
                                                sleep 10
                                                virtualenv ~/venv
                                                source ~/venv/bin/activate
                                                chmod 0755 requirements.txt
                                                pip install -r requirements.txt
                                                $ANDROID_HOME/platform-tools/adb forward tcp:6000 localfilesystem:/data/data/${appPackage}/firefox-debugger-socket
                                                $ANDROID_HOME/platform-tools/adb forward --list
                                                python testRunner.py || true
                                                $ANDROID_HOME/platform-tools/adb uninstall ${appPackage}
                                                sleep 10
                                                $ANDROID_HOME/platform-tools/adb forward --remove-all
                                                kill `cat appium.pid` || true
                                                rm -f appium.pid
                                            '''
                                       }
                                    }
                                }
                                stage("Upload Results: ${flavorname}") {
                                    archiveTestResults("${flavorname}")
                                }
                            }
                        }catch(e) {
                          print e
                        }
                    }
                }
            }
            stage('Clean Up') {
                sh '''#!/bin/bash
                    rm -f mozilla-release/mozconfig
                    rm -rf mozilla-release/objdir-frontend-android
                    rm -rf autobots
                    rm -f screenshots.zip
                '''
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
        withGenymotion(
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

def withGenymotion(
        String instanceImage,
        String count,
        String instanceType,
        String keyName,
        String securityGroup,
        String subnetId,
        String region,
        Closure body
    ) {
    def instance_id = ""
    try {
        instance_id = sh(returnStdout: true, script: """
            aws ec2 run-instances \
                --image-id ${instanceImage} \
                --count ${count} \
                --instance-type ${instanceType} \
                --key-name ${keyName} \
                --security-group-ids ${securityGroup} \
                --subnet-id ${subnetId} \
                --region=${region} \
                --query "Instances[].InstanceId" \
                --output text
            """).trim()
        def ip = sh(returnStdout: true, script: """
            aws ec2 describe-instances \
                --instance-ids ${instance_id} \
                --region=${region} \
                --query 'Reservations[].Instances[].PrivateIpAddress' \
                --output text
            """).trim()
        sh "aws ec2 create-tags \
                --resources ${instance_id} \
                --region=${region} \
                --tag Key=Name,Value='Appium-Genymotion'"
        withEnv([
                "INSTANCE_ID=${instance_id}",
                "IP=${ip}",
                "REGION=${region}"
            ]){
                body()
        }
    } finally {
        sh """#!/bin/bash -l
            set -x
            set -e
            aws ec2 terminate-instances --instance-ids ${instance_id} --region=${region}
        """
    }
}

def cloneRepoViaSSH(String repoLink, String args) {
    sh """#!/bin/bash -l
        set -x
        set -e
        mkdir -p ~/.ssh
        cp $SSH_KEY ~/.ssh/id_rsa
        chmod 600 ~/.ssh/id_rsa
        ssh-keyscan -t rsa github.com >> ~/.ssh/known_hosts
        git clone ${args} ${repoLink}
    """
}

def archiveTestResults(String flavorName) {
    try {
        archiveArtifacts allowEmptyArchive: true, artifacts: 'autobots/*.log'
        junit 'autobots/test-reports/*.xml'
        zip archive: true, dir: 'autobots/screenshots', glob: '', zipFile: "${flavorName}-screenshots.zip"
    } catch(e) {
        print e
    }
}
