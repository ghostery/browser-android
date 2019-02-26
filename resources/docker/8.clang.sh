#!/bin/bash
mkdir -p $HOME/clang
cd $HOME/clang
wget --output-document=clang.tar.xz --quiet "https://queue.taskcluster.net/v1/task/E-pAZ4gVQVCkX4gzYF3WfA/artifacts/public/build/clang.tar.xz"
tar xf clang.tar.xz
echo 'clang - Done' >> status.log