### This script connects to the Genymotion Instance.

IP=@1
FILE=@2

chmod 400 $FILE
ssh -v -o StrictHostKeyChecking=no -i $FILE shell@$IP "setprop persist.sys.usb.config adb"
ssh -v -o StrictHostKeyChecking=no -i $FILE -NL 5556:127.0.0.1:5555 shell@$IP &
$ANDROID_HOME/platform-tools/adb connect 127.0.0.1:5556
$ANDROID_HOME/platform-tools/adb wait-for-device