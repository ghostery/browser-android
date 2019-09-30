<<<<<<< HEAD
#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/. */

set -o errexit
set -o nounset
set -o pipefail
set -o xtrace

test "$(whoami)" == 'root'

# Install stuff we need
apt-get -y update
apt-get install -y \
    cmake \
    curl \
    git \
    libfontconfig1-dev \
    libgl1-mesa-dev \
    libx11-dev \
    pkg-config \
    python \
    python-pip \
    software-properties-common

# Build freetype with subpixel rendering enabled
curl -sSfL -o ft.tar.bz2 \
    https://download.savannah.gnu.org/releases/freetype/freetype-2.8.1.tar.bz2
tar xjf ft.tar.bz2
cd freetype-2.8.1
# Need to respect 80-char line limit for servo-tidy, or this would be neater
SUBPIXEL_OPTION="FT_CONFIG_OPTION_SUBPIXEL_RENDERING"
sed --in-place="" \
    -e "s/.*${SUBPIXEL_OPTION}.*/#define ${SUBPIXEL_OPTION}/" \
    include/freetype/config/ftoption.h
./configure
make
make install

# Replace the system libfreetype with the one we just built
cd /usr/lib/x86_64-linux-gnu/
rm -f libfreetype.so.6
ln -s /usr/local/lib/libfreetype.so.6

# Other stuff we need
pip install mako voluptuous PyYAML servo-tidy
||||||| merged common ancestors
=======
#!/usr/bin/env bash

# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/. */

set -o errexit
set -o nounset
set -o pipefail
set -o xtrace

test "$(whoami)" == 'root'

# Install stuff we need
apt-get -y update
apt-get install -y \
    bzip2 \
    cmake \
    curl \
    gcc \
    git \
    g++ \
    libfontconfig1-dev \
    libgl1-mesa-dev \
    libx11-dev \
    openjdk-8-jdk \
    pkg-config \
    python \
    python-mako \
    python-pip \
    python-setuptools \
    python-voluptuous \
    python-yaml \
    software-properties-common

# Get freetype 2.8 with subpixel rendering enabled. The SNAPSHOT_ARCHIVE
# variable is just to work around servo-tidy's moronic 80-char width limit
# in shell scripts.
SNAPSHOT_ARCHIVE=http://snapshot.debian.org/archive/debian/20180213T153535Z
curl -sSfL -o libfreetype6.deb \
  "${SNAPSHOT_ARCHIVE}/pool/main/f/freetype/libfreetype6_2.8.1-2_amd64.deb"
curl -sSfL -o libfreetype6-dev.deb \
  "${SNAPSHOT_ARCHIVE}/pool/main/f/freetype/libfreetype6-dev_2.8.1-2_amd64.deb"
apt install -y ./libfreetype6.deb ./libfreetype6-dev.deb

# Other stuff we need
pip install servo-tidy==0.3.0
>>>>>>> upstream-releases
