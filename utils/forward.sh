#!/bin/bash

# Forward local port 6000 to the Firefox debug socket.
# You can specify which version (alpha, beta) as the first argument. No
# argument means production.

CWD=`dirname $0`

source "${CWD}/android.sh"

flavor=""
if [ -n "${1}" ]
then
    flavor=".${1}"
fi

$ABD forward tcp:6000 localfilesystem:/data/data/com.cliqz.browser${flavor}/firefox-debugger-socket
