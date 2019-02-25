### This script builds the browser.
## The passed argument decides if a Multilocale is required.

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
echo "*** DONE ***"
echo ""
echo ""