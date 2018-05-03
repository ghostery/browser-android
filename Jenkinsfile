#!/bin/env groovy

node('us-east-1 && ubuntu && docker && !gpu') {

    def dockerTag = ""
    def apk = ""
    try {
        withGenymotion(
            "ami-13050368",
            "1",
            "t2.medium",
            "android_ci_genymotion",
            "sg-5bbf173f",
            "subnet-341ff61f",
            "us-east-1"
            ) {
            stage('Checkout') {
                checkout scm
                def delim = '"'
                dockerTag = sh(returnStdout: true,
                    script: """curl https://api.github.com/repos/cliqz-oss/cliqz-android/branches/upstream-releases | \
                    grep -m 1 sha | cut -d '${delim}' -f 4""").trim()
                withCredentials([file(credentialsId: 'ceb2d5e9-fc88-418f-aa65-ce0e0d2a7ea1', variable: 'SSH_KEY')]) {
                    cloneRepoViaSSH(
                        "git@github.com:cliqz/autobots.git",
                        "-b version2.0 --single-branch --depth=1"
                    )
                }
            }
            def baseImageName = "browser-f/android:${dockerTag}"
            docker.withRegistry('https://141047255820.dkr.ecr.us-east-1.amazonaws.com') {
                docker.image("${baseImageName}").inside {
                    stage('Build APK') {
                        sh '''#!/bin/bash -l
                            set -e
                            set -x
                            cp mozconfigs/cliqz.mozconfig mozilla-release/mozconfig
                            cd mozilla-release
                            ./mach build
                            for lang in `ls ../l10n/`; do
                                ./mach build chrome-$lang
                            done
                            ./mach package
                        '''
                        apk = sh(returnStdout: true, 
                            script: """cd mozilla-release/objdir-frontend-android/dist && \
                            find *.apk -name 'fennec*i386*' -not -name '*-unsigned-*'""").trim()
                    }
                }
                timeout(5) {
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
                        "platformName=android",
                        "deviceName=127.0.0.1:5556",
                        "MODULE=testSmoke",
                        "TEST=SmokeTest",
                        "appPackage=org.mozilla.fennec_",
                        "appActivity=org.mozilla.gecko.LauncherActivity"
                        ]) {
                        withCredentials([file(credentialsId: 'da5f91e6-e1ca-4aac-94ea-352b6769228b', variable: 'FILE' )]) {
                            stage('Genymotion ADB Connect') {
                                sh'''#!/bin/bash -l
                                    set -x
                                    set -e
                                    chmod 400 $FILE
                                    ssh -v -o StrictHostKeyChecking=no -i $FILE root@$IP "setprop persist.sys.usb.config adb"
                                    ssh -v -o StrictHostKeyChecking=no -i $FILE -NL 5556:127.0.0.1:5555 root@$IP &
                                    $ANDROID_HOME/platform-tools/adb connect 127.0.0.1:5556
                                    $ANDROID_HOME/platform-tools/adb wait-for-device
                                '''
                            }
                        }
                        stage('Run Tests') {
                            timeout(60) {
                                sh'''#!/bin/bash -l
                                    set -x
                                    set -e
                                    appium &
                                    sleep 10
                                    export app=$PWD/mozilla-release/objdir-frontend-android/dist/$APP
                                    cd autobots
                                    virtualenv ~/venv
                                    source ~/venv/bin/activate
                                    chmod 0755 requirements.txt
                                    pip install -r requirements.txt
                                    python testRunner.py || true
                               '''
                           }
                        }
                    }
                    stage('Upload APK') {
                        archiveArtifacts allowEmptyArchive: true, artifacts: 'mozilla-release/objdir-frontend-android/dist/*.apk'
                        archiveTestResults()
                    }
                }
            }
        }
    } finally {
        stage('Clean Up') {
            sh '''#!/bin/bash
                rm -f mozilla-release/mozconfig
                rm -rf mozilla-release/objdir-frontend-android
                rm -rf autobots
            '''
        }
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

@NonCPS
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

@NonCPS
def archiveTestResults() {
    try {
        archiveArtifacts allowEmptyArchive: true, artifacts: 'autobots/*.log'
        junit 'autobots/test-reports/*.xml'
        zip archive: true, dir: 'autobots/screenshots', glob: '', zipFile: 'autobots/screenshots.zip'
    } catch(e) {
        print e
    }
}
