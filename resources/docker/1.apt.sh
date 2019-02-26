#!/bin/bash
apt-get update
apt-get install -y software-properties-common
add-apt-repository universe
apt-get update
apt-get install -y \
    autoconf2.13 \
    build-essential \
    ccache \
    curl \
    git \
    language-pack-en \
    libncurses5:i386 \
    libstdc++6:i386 \
    openjdk-8-jdk \
    python-dev \
    python3-dev \
    python-pip \
    python-setuptools \
    python-virtualenv \
    unzip \
    uuid \
    wget \
    yasm \
    zip \
    automake \
    bison \
    gawk \
    gnupg2 \
    language-pack-en \
    libffi-dev \
    libgdbm-dev \
    libgmp-dev \
    libncurses5-dev \
    libreadline6-dev \
    libsqlite3-dev \
    libssl-dev \
    libtool \
    libyaml-dev \
    pkg-config \
    python-virtualenv \
    sqlite3 \
    zlib1g-deve
locale-gen en_US en_US.UTF-8
dpkg-reconfigure locales
echo 'apt - Done' >> status.log