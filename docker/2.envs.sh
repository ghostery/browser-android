#!/bin/bash
echo '''
export LANG="en_US.UTF-8"
export LANGUAGE="en_US:en"
export LC_ALL="en_US.UTF-8"
export MOZBUILD_STATE_PATH="$HOME/.mozbuild"
export JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64"
export RUSTUP_HOME="/usr/local/rustup"
export CARGO_HOME="/usr/local/cargo"
export ANDROID_HOME="MOZBUILD_STATE_PATH/android-sdk-linux"
export ANDROID_NDK_HOME="MOZBUILD_STATE_PATH/android-ndk-linux/android-ndk-r16b"
export NVM_DIR="$HOME/nvm"
export NODE_VERSION="8.11.4"
export CLANG_HOME="$HOME/clang"
export HOST_CC="$CLANG_HOME/bin/clang"
export HOST_CXX="$CLANG_HOME/bin/clang++"
export export LD_LIBRARY_PATH="$CLANG_HOME/lib:LD_LIBRARY_PATH"
export PATH="/usr/local/cargo/bin:$HOME/.local/bin:$HOME/node-v8.11.4-linux-x64/bin:$CLANG_HOME/bin:$PATH"
''' >> ~/.bashrc
echo 'envs - Done' >> status.log