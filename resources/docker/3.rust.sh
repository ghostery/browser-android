#!/bin/bash
curl https://sh.rustup.rs -sSf | sh -s -- -y
chmod -R a+w $RUSTUP_HOME $CARGO_HOME
rustup --version
cargo --version
rustc --version
rustup self update
rustup toolchain install 1.29.2-x86_64-unknown-linux-gnu
rustup default 1.29.2-x86_64-unknown-linux-gnu
rustup target add i686-linux-android
rustup target add armv7-linux-androideabi
cargo install cbindgen --vers="0.6.6" --force
echo 'rust - Done' >> status.log