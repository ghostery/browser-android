#!/bin/bash

set -e

CWD="`dirname $0`"
SRCROOT=$(cd "${CWD}/../"; pwd)
MOZ_SRC="${SRCROOT}/mozilla-release"
MOZ_CONFIG="${MOZ_SRC}/mozconfig"
MOZ_CONFIG_BAK="${MOZ_CONFIG}-${RANDOM}"

function usage() {
    cmd=`basename "$0"`
    >&2 cat << EOF
Build the a multi-locale version given a mozconfig file.

    ./${cmd} [brand] [arch]
EOF
}

function cleanup() {
    [ -f "${MOZ_CONFIG_BAK}" ] && \
        mv "${MOZ_CONFIG_BAK}" "${MOZ_CONFIG}"
}

trap cleanup 0



case "${1}" in
    "-h" | "--help")
        usage
        exit 0
        ;;
esac

CLIQZ_BRAND="${1:-cliqz}"
CLIQZ_BRAND_FOLDER="${MOZ_SRC}/mobile/android/branding/${CLIQZ_BRAND}"

if [ ! -d "${CLIQZ_BRAND_FOLDER}" ]; then
    >&2 echo "${CLIQZ_BRAND_FOLDER} does not exists"
    usage
    exit 1
fi

CLIQZ_ARCH="${2:-i686-linux-android}"
case "${CLIQZ_ARCH}" in
    "arm")
        CLIQZ_ARCH="arm-linux-androideabi"
        ;;
    "arm-linux-androideabi")
        ;;
    "i386" | "x86" | "i686")
        CLIQZ_ARCH="i686-linux-android"
        ;;
    "i686-linux-android")
        ;;
    *)
        >&2 echo "Unsopported arch ${CLIQZ_ARCH}"
        usage
        exit 1
        ;;
esac

[ -f "${MOZ_CONFIG}" ] && mv "${MOZ_CONFIG}" "${MOZ_CONFIG_BAK}"

ANDROID_HOME="${ANDROID_HOME:-$HOME/.mozbuild/android-sdk-linux}"

cat <<EOF > "${MOZ_CONFIG}"
# Build Firefox for Android:
ac_add_options --enable-application=mobile/android
ac_add_options --target="${CLIQZ_ARCH}"
# With the following Android SDK:
ac_add_options --with-android-sdk="${ANDROID_HOME}"
# Enable artifact building:
ac_add_options --enable-artifact-builds
# Write build artifacts to:
mk_add_options MOZ_OBJDIR="./objdir-${CLIQZ_BRAND}"
# Branding
ac_add_options --with-branding="mobile/android/branding/${CLIQZ_BRAND}"
# l10n
ac_add_options --with-l10n-base="\$topsrcdir/../l10n"
EOF

(
    cd "${MOZ_SRC}"
    ./mach clobber
    ./mach build
    for loc in `ls ../l10n/`; do
        ./mach build chrome-${loc}
    done
    AB_CD=multi ./mach package
)
