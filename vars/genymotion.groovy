#!/usr/bin/env groovy

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

def genyInstanceStatus() {
    timeout(10) {
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

def connectGenyInstance(String credentialsId) {
    withCredentials([file(credentialsId: credentialsId, variable: 'FILE' )]) {
        sh '''#!/bin/bash -l
            set -x
            set -e
            echo "*** Connecting to Genymotion Instance ***"
            chmod 400 $FILE
            ssh -v -o StrictHostKeyChecking=no -i $FILE shell@$IP "setprop persist.sys.usb.config adb"
            ssh -v -o StrictHostKeyChecking=no -i $FILE -NL 5556:127.0.0.1:5555 shell@$IP &
            $ANDROID_HOME/platform-tools/adb connect 127.0.0.1:5556
            $ANDROID_HOME/platform-tools/adb wait-for-device
            echo "*** DONE ***"
        '''
    }
}

return this