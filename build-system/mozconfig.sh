### This script copies the required mozconfig.
## The name/path of the mozconfig can passed as an argument for the script.

echo ""
echo ""
echo "*** Copy the required Mozconfig ***"
echo ""
if [ "${BUILD_TYPE}" == "nightly" ]; then
    cp ${SRC_DIR}/../mozconfigs/deploy.mozconfig ${SRC_DIR}/mozconfig
elif [ "${BUILD_TYPE}" == "release" ]; then
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