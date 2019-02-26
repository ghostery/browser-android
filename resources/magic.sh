### This script is MAGIC !!.
## The passed argument decides if a Multilocale is required.
echo ""
echo ""
echo "Starting Build..."
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
echo ""
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
echo ""
echo ""
echo ""
echo "*** Copy the required Mozconfig ***"
echo ""
if [ "${BUILD_TYPE}" == "nightly" ]; then
    export PACK_LOCALE="multi"
    cp ${SRC_DIR}/../mozconfigs/deploy.mozconfig ${SRC_DIR}/mozconfig
elif [ "${BUILD_TYPE}" == "release" ]; then
    export PACK_LOCALE="multi"
    cp ${SRC_DIR}/../mozconfigs/deploy.mozconfig ${SRC_DIR}/mozconfig
else
    cp ${SRC_DIR}/../mozconfigs/jenkins.mozconfig ${SRC_DIR}/mozconfig
fi
echo ""
echo "Copied the mozconfig file. Here is the contents of the file:"
cat ${SRC_DIR}/mozconfig
echo ""
echo "*** DONE ***"
echo ""
echo ""
echo ""
echo "*** Build and Package the Browser ***"
echo ""
cd ${SRC_DIR}
./mach clobber
./mach build
if [ "${PACK_LOCALE}" == "multi" ]; then
    rm -f ${OBJ_DIR}/dist/bin/defaults/pref/mobile-l10n.js
    cp ${SRC_DIR}/mobile/android/installer/mobile-l10n.js ${OBJ_DIR}/dist/bin/defaults/pref/mobile-l10n.js
    export MOZ_CHROME_MULTILOCALE=`ls -1 "${l10n}"|paste -s -d " "`
    for AB_CD in $MOZ_CHROME_MULTILOCALE; do
        ./mach build chrome-$AB_CD
    done
    export AB_CD=multi
fi
./mach package
mkdir -p ${SRC_DIR}/../build/
echo ""
if [ "${CERT_PATH}" != "" ] && [ "${CERT_PASS}" != "" ]; then
    echo "Building the Signed Build..."
    ./gradlew :app:assembleWithGeckoBinariesRelease
    cd ${OBJ_DIR}/gradle/build/mobile/android/app/outputs/apk/WithGeckoBinariesRelease/release
    FILE_NAME=`find *.apk -name '*-release*' -not -name '*-unsigned-*'`
    cp ${FILE_NAME} ${SRC_DIR}/../build/${TARGET_ARCH}.apk
else
    echo "No Certs Found."
    echo "Using the already Packaged APK. *** NOTE : BUILD WILL BE 'en-US' ONLY ***"
    cd ${OBJ_DIR}/dist/
    FILE_NAME=`find *.apk -not -name 'robo*' -not -name '*-unsigned-*'`
    cp ${OBJ_DIR}/dist/${FILE_NAME} ${SRC_DIR}/../build/${TARGET_ARCH}.apk
fi
echo ""
echo "*** DONE !!   Build: ${SRC_DIR}/../build/${TARGET_ARCH}.apk ***"
echo ""
echo ""