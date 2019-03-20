#!/usr/bin/env groovy

def lastSuccessfulBuild(passedBuilds, build) {
    if ((build != null) && (build.result != 'SUCCESS')) {
        echo "Finding last succesful build"
        passedBuilds.add(build)
        echo "Found a build"
        lastSuccessfulBuild(passedBuilds, build.getPreviousBuild())
    }
}

@NonCPS
def getChangeString(passedBuilds) {
    def changeString = ""
    echo "Gathering Changes Since Last Successful Build"
    for (int x = 0; x < passedBuilds.size(); x++) {
        def currentBuild = passedBuilds[x];
        def buildNumber = currentBuild.number
        echo "Changes for Build ${buildNumber}"
        def changeLogSets = currentBuild.rawBuild.changeSets
        for (int i = 0; i < changeLogSets.size(); i++) {
            def entries = changeLogSets[i].items
            for (int j = 0; j < entries.length; j++) {
                def entry = entries[j]
                changeString += "* ${entry.msg} \n"
            }
        }
    }
    if (!changeString) {
        changeString = " - No new changes"
    }
    echo changeString
    return changeString;
}

@NonCPS
def getIssueList(){
    def list = []
    def changes = getChangeString(passedBuilds)
    def re = /AB2-([0-9])+/
    def y = changes =~ re
    while (y){
        list.add(y.group().toString())
    }
    if (list.size() > 0) {
        echo 'Detected JIRA tickets'
        echo list.toString()
    } else {
       echo 'No JIRA tickets detected'
    }
    return list
}

def transitionIssues() {
    def transitionIssueList
    passedBuilds = []
    lastSuccessfulBuild(passedBuilds,currentBuild)
    def issueIds = getIssueList()
    def transitionInput = [
            transition: [
                    id: '121'
            ]
    ]
    for (element in issueIds){
        try{
            def issue = jiraGetIssue idOrKey: element
            if(issue.data.fields.status.name == "Resolved"){
                jiraTransitionIssue idOrKey: element, input: transitionInput
                echo "Transistioned ${element}"
            } else{
                echo """Element with id ${element} was not transitioned\nIts status is: """+issue.data.fields.status.name
            }
        }
        catch (Error e){
            echo e
        }
    }
    if (issueIds.size() > 0){
        transitionIssueList = issueIds.join(", ")
        echo transitionIssueList
    }
    return [passedBuilds, transitionIssueList]
}

return this