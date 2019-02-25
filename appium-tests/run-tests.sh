### This script runs Appium Tests.

SCRIPT_PATH=`dirname $0`

. ${SCRIPT_PATH}/appium-tests/envs.sh

export app=$PWD/build/$APP
$ANDROID_HOME/platform-tools/adb install $app
cd cliqz-mobile-tests
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