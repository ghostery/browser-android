#!/bin/bash

# Start an emulator. If the emulator name is present, it start it directly.
# Otherwise, the script will ask the user to pick one from a list of previously
# created emulators.

CWD="`dirname ${0}`"
source "${CWD}/android.sh"

emuname="${1}"

if [ -z "${emuname}" ]
then
    emuFiles=`find "${HOME}/.android/avd" -maxdepth 1 -iname "*.ini"`
    if [ -z "${emuFiles}" ]
    then
        echo >&2 "No emulator found, please create one using Android Studio"
        exit 1
    fi
    emuNames=('none')
    counter=0
    for emu in ${emuFiles}
    do
        counter=$[counter + 1]
        emuName="`basename ${emu%%.ini}`"
        emuNames+=("${emuName}")
        echo "${counter}. ${emuName}"
    done
    echo -n "Please, pick one: "
    while read n
    do
        [ $n -gt 0 ] && [ $n -le $counter ] && break
        echo -n "Invalid input $n. Please, pick one: "
    done
    emuname=${emuNames[$n]}
fi

$EMULATOR "@${emuname}" &
