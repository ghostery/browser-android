### This script runs Appium Tests.

echo "Run Appium Tests"
echo ""
echo "*** Define the required Variables ***"
echo ""
if [ "${platformName}" == "" ]; then
    export platformName="android"
fi
if [ "${deviceName}" == "" ]; then
    export deviceName="127.0.0.1:5556"
fi
if [ "${appActivity}" == "" ]; then
    export appActivity="org.mozilla.gecko.LauncherActivity"
fi
if [ "${MODULE}" == "" ]; then
    export MODULE="testCompleteSuite"
fi
if [ "${TEST}" == "" ]; then
    export appActivity="CompleteSuite"
fi
if [ "${TEST_TYPE}" == "" ]; then
    export TEST_TYPE="smoke"
fi
echo ""
echo "Starting Tests"
echo ""
export app=$PWD/build/$APP
$ANDROID_HOME/platform-tools/adb install $app
cd $TESTS_FOLDER
appium --log $FLAVOR-appium.log &
echo $! > appium.pid
sleep 10
virtualenv ~/venv
source ~/venv/bin/activate
chmod 0755 requirements.txt
pip install -r requirements.txt
python testRunner.py || true
$ANDROID_HOME/platform-tools/adb uninstall ${appPackage}
sleep 10
$ANDROID_HOME/platform-tools/adb forward --remove-all
kill `cat appium.pid` || true
rm -f appium.pid
echo ""
echo ""
echo " *** DONE *** "