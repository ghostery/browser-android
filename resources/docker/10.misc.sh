#!/bin/bash
curl -sSL https://get.rvm.io | bash -s stable --ruby=2.5.1 --autolibs=read-fail
source /home/jenkins/.rvm/scripts/rvm
rvm reload
gem install fastlane --version 2.109.1
npm install --global yarn appium@1.9.1 wd@1.10.1
pip install awscli --upgrade --user
echo 'misc - Done' >> status.log