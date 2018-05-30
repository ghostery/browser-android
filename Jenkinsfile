#!/bin/env groovy
@Library('cliqz-shared-library@v1.2') _
def matrix = [
        'cliqz':[
            'config':'mozconfigs/cliqz.mozconfig',
            'test': true, 
        ],
        'ghostery':[
            'config':'mozconfigs/ghostery.mozconfig',
            'test': false,
        ],
        'cliqz-alpha':[
            'config':'mozconfigs/cliqz-alpha.mozconfig',
            'test': false,
        ],
        'ghostery-alpha':[
            'config':'mozconfigs/ghostery-alpha.mozconfig',
            'test': false,
        ],
    ]

def build(Map m){
    def flavor = m.config
    def flavorname = m.name
    def nodeLabel = 'us-east-1 && ubuntu && docker && !gpu'
    def test =m.test
    return {
        node(nodeLabel){
            def dockerTag = ""
            def apk = ""
            stage('Checkout') {
                checkout scm
                dockerTag = readFile('mozilla-release/browser/config/version_display.txt').trim()
            }
            def baseImageName = "browser-f/android:${dockerTag}"
            docker.withRegistry('https://141047255820.dkr.ecr.us-east-1.amazonaws.com') {
                docker.image("${baseImageName}").inside {
                    stage('Download cache') {
                        withCredentials([[
                                $class: 'AmazonWebServicesCredentialsBinding',  
                                accessKeyVariable: 'AWS_ACCESS_KEY_ID', 
                                credentialsId: 'd7e38c4a-37eb-490b-b4da-2f53cc14ab1b', 
                                secretKeyVariable: 'AWS_SECRET_ACCESS_KEY']]) {
                            def s3Path = "s3://repository.cliqz.com/dist/android/cache"
                            def cachePath = ".gradle/caches"
                            sh """#!/bin/bash -l 
                                pip install awscli --upgrade --user
                                cd
                                aws s3 sync --acl public-read --acl bucket-owner-full-control ${s3Path} ${cachePath}
                            """                        
                        }
                    }
                    stage('Build APKS') {
                        sh """#!/bin/bash -l
                            set -e
                            set -x
                            cp ${flavor} mozilla-release/mozconfig
                        """
                        sh '''#!/bin/bash -l
                            set -x 
                            set -e 
                            cd mozilla-release
                            ./mach clobber
                            ./mach build
                            for language in `ls ../l10n/`; do
                                ./mach build chrome-$language
                            done
                            ./mach package
                        '''
                        apk = sh(returnStdout: true, 
                            script: """cd mozilla-release/objdir-frontend-android/${flavorname}/dist && \
                            find *.apk -name 'fennec*i386*' -not -name '*-unsigned-*'""").trim()
                    }
                    stage('Upload APK') {
                        archiveArtifacts allowEmptyArchive: true, artifacts: "mozilla-release/objdir-frontend-android/${flavorname}/dist/*.apk"
                    }
                }
                if (test == true){
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
                                    "MODULE=testSmoke",
                                    "TEST=SmokeTest",
                                    "appPackage=com.cliqz.browser",
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
                                                export app=$PWD/mozilla-release/objdir-frontend-android/$FLAVOR/dist/$APP
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
                                stage('Upload Results') {
                                    archiveTestResults()
                                }
                            }
                        }
                    }catch(e) {
                      print e  
                    }
                }     
            }
            stage('Clean Up') {
                sh '''#!/bin/bash
                    rm -f mozilla-release/mozconfig
                    rm -rf mozilla-release/objdir-frontend-android
                    rm -rf autobots
                '''
            }
        }
    }
}
 

def stepsForParallelBuilds = helpers.entries(matrix).collectEntries{
    [("Building ${it[0]}"):build(
        name: it[0],
        config:it[1]['config'],
        test:it[1]['test']
    )]
}

parallel stepsForParallelBuilds    
 

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

def archiveTestResults() {
    try {
        archiveArtifacts allowEmptyArchive: true, artifacts: 'autobots/*.log'
        junit 'autobots/test-reports/*.xml'
        zip archive: true, dir: 'autobots/screenshots', glob: '', zipFile: 'autobots/screenshots.zip'
    } catch(e) {
        print e
    }
}