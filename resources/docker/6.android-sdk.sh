#!/bin/bash
mkdir -p $ANDROID_HOME
cd $ANDROID_HOME
wget --output-document=sdktools.zip --quiet 'https://repository.cliqz.com/dist/android/artifacts/android-sdk/sdk-tools-linux-3859397.zip'
unzip sdktools.zip
rm -f sdktools.zip
echo 'android-sdk - Done' >> status.log
if [ "${1}" == "" ]; then
    echo "Android SDK Packages were not installed." >> status.log
else
    (while (true); do echo y; sleep 2; done) | \
      tools/bin/sdkmanager  --package_file=$1;
    echo 'android-sdk packages - Done' >> status.log
fi