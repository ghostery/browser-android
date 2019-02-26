#!/bin/bash
source 1.apt.sh
source 2.envs.sh
source 3.rust.sh
source 4.node.sh
source 5.proguard.sh
source 6.android-sdk.sh
source 7.android-ndk.sh
source 8.clang.sh
source 9.ruby-keys.sh
source 10.misc.sh
echo "bootstrap complete." >> status.log
echo ""
echo ""
echo "*** STATUS ***"
cat status.log
echo ""
echo "*** DONE ***"