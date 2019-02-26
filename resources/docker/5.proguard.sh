#!/bin/bash
mkdir -p $MOZBUILD_STATE_PATH/proguard/lib
mkdir -p $HOME/tmp
wget --output-document=$HOME/tmp/proguard.tgz --quiet "https://repository.cliqz.com/dist/android/artifacts/proguard/proguard5.3.3.tar.gz"
(cd $HOME/tmp; tar xf proguard.tgz && cp proguard5.3.3/lib/proguard.jar $HOME/.mozbuild/proguard/lib)
rm -rf $HOME/tmp/*
echo 'proguard - Done' >> status.log