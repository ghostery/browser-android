#!/bin/bash
### This script defines the Variables to build the browser.

echo ""
echo ""
echo "*** Define the required Variables ***"
echo ""
if [ "${ANDROID_TARGET}" == "arm-linux-androideabi" ]; then
    export TARGET_ARCH="arm"
else
    export TARGET_ARCH="x86"
fi
if [ "${TARGET_ARCH}" == "" ]; then
    export TARGET_ARCH="x86"
fi
if [ "${BRAND}" == "" ]; then
    export BRAND="ghostery"
fi
if [ "${TARGET_ARCH}" == "x86" ]; then
    export ANDROID_TARGET="i686-linux-android"
elif [ "${TARGET_ARCH}" == "arm" ]; then
    export ANDROID_TARGET="arm-linux-androideabi"
fi
if [ "${CLIQZ_CHANNEL}" == "" ]; then
    export CLIQZ_CHANNEL="MA99"
fi
if [ "${PACK_LOCALE}" == "" ]; then
    export PACK_LOCALE="en-US"
fi
echo "Building ${BRAND} branded browser for ${TARGET_ARCH} with ${PACK_LOCALE} language pack."
export SRC_DIR="${PWD}/mozilla-release"
export OBJ_DIR="${SRC_DIR}/objdir-frontend-android/${BRAND}"
export l10n="${SRC_DIR}/l10n"