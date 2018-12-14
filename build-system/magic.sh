#!/bin/bash
### This script is MAGIC !!.
## The passed argument decides if a Multilocale is required.
echo "Starting Build..."
SCRIPT_PATH=`dirname $0`
for i in "$@"
do
case $i in
    -t=*|--type=*)
    export BUILD_TYPE="${i#*=}"
    shift
    ;;
    -a=*|--arch=*)
    export TARGET_ARCH="${i#*=}"
    shift
    ;;
    -b=*|--brand=*)
    export BRAND="${i#*=}"
    shift
    ;;
    -c=*|--channel=*)
    export CLIQZ_CHANNEL="${i#*=}"
    shift
    ;;
    -l=*|--locale=*)
    export PACK_LOCALE="${i#*=}"
    shift
    ;;
    *)
          # unknown option
    ;;
esac
done
. ${SCRIPT_PATH}/envs.sh
. ${SCRIPT_PATH}/mozconfig.sh
. ${SCRIPT_PATH}/build.sh