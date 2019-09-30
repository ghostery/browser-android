#!/bin/bash
set -x -e -v

WORKSPACE=$HOME/workspace
HOME_DIR=$WORKSPACE/build
INSTALL_DIR=$WORKSPACE/wine
UPLOAD_DIR=$HOME/artifacts

mkdir -p $INSTALL_DIR

cd $HOME_DIR

# --------------
<<<<<<< HEAD
$GPG --import $data_dir/DA23579A74D4AD9AF9D3F945CEFAC8EAAF17519D.key
download_and_check http://dl.winehq.org/wine/source/3.0/ wine-3.0.3.tar.xz.sign
tar xaf $TMPDIR/wine-3.0.3.tar.xz
cd wine-3.0.3
||||||| merged common ancestors
$GPG --import $data_dir/DA23579A74D4AD9AF9D3F945CEFAC8EAAF17519D.key

download_and_check http://dl.winehq.org/wine/source/2.0/ wine-2.0.1.tar.xz.sign
tar xaf $TMPDIR/wine-2.0.1.tar.xz
cd wine-2.0.1
=======
cd wine-3.0.3
>>>>>>> upstream-releases
./configure --prefix=$INSTALL_DIR/
make -j$(nproc)
make install

# --------------

cd $WORKSPACE/
tar caf wine.tar.xz wine

mkdir -p $UPLOAD_DIR
cp wine.tar.* $UPLOAD_DIR
