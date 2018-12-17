#!/bin/bash

readonly RELDIR="`dirname $0`"
readonly ABSDIR=`readlink -f "${RELDIR}"`
readonly L10NDIR=`readlink -f "${RELDIR}/../l10n"`
readonly TMPDIR=`readlink -f "${RELDIR}/../.l10n"`
readonly REPDIR=`readlink -f "${RELDIR}/../.l10n-rep"`
readonly LANGS=`cd "${L10NDIR}"; ls -1d */`
readonly LOCALE_JSON=`readlink -f "${ABSDIR}/../mozilla-release/mobile/locales/l10n-changesets.json"`

function finish {
    rm -rf "${TMPDIR}" "${REPDIR}"
}

function fetch {
    revision=`cat "${LOCALE_JSON}"|jq ".\"${1}\".revision"`
    hg clone "https://hg.mozilla.org/l10n-central/$1" "$1"
    cd $1
    hg update "${revision}"
    rm -rf .hg .hgtags .hgignore
}

trap finish EXIT

# Making tmp dir
mkdir -p "${TMPDIR}"

for lang in ${LANGS}
do
    (cd "${TMPDIR}"; fetch "${lang%/}")
done

mv "${L10NDIR}" "${REPDIR}"
mv "${TMPDIR}" "${L10NDIR}"
