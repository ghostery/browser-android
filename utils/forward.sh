#!/bin/bash

# Forward local port 6000 to the Firefox debug socket.
# You can specify which version (alpha, beta) as the first argument. No
# argument means production.

CWD=`dirname $0`

source "${CWD}/android.sh"

declare -a flavors=(  "Cliqz" "Cliqz Alpha" "Ghostery" "Ghostery Alpha" )
declare -A packages
packages=( \
    ["${flavors[0]}"]="com.cliqz.browser" \
    ["${flavors[1]}"]="com.cliqz.browser.alpha" \
    ["${flavors[2]}"]="com.ghostery.android" \
    ["${flavors[3]}"]="com.ghostery.android.alpha" )

counter=0
for f in "${flavors[@]}"
do
    ((counter++))
    echo "${counter}. $f"
done
echo -n "Please, pick one: "
while read n
do
    [ $n -gt 0 ] && [ $n -le $counter ] && break
    echo -n "Invalid input $n. Please, pick one: "
done

flavor="${flavors[$[ n - 1]]}"
package="${packages[$flavor]}"

$ADB forward tcp:6000 localfilesystem:/data/data/${package}/firefox-debugger-socket
