### This script defines the Env Variables for running Appium Tests.

echo ""
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