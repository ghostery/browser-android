#!/bin/bash
mkdir -p $MOZBUILD_STATE_PATH/android-ndk-linux
cd $MOZBUILD_STATE_PATH/android-ndk-linux
wget --output-document=ndk.zip --quiet 'https://dl.google.com/android/repository/android-ndk-r16b-linux-x86_64.zip'
unzip ndk.zip
rm -f ndk.zip
echo 'android-ndk - Done' >> status.log