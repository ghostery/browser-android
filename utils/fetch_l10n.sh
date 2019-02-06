#!/bin/bash

readonly RELDIR="`dirname $0`"
readonly ABSDIR=`readlink -f "${RELDIR}"`
readonly L10NDIR=`readlink -f "${RELDIR}/../l10n"`
readonly TMPDIR=`readlink -f "${RELDIR}/../.l10n"`
readonly REPDIR=`readlink -f "${RELDIR}/../.l10n-rep"`
readonly LOCALE_JSON=`readlink -f "${ABSDIR}/../mozilla-release/mobile/locales/l10n-changesets.json"`
declare -rA LANGS=( \
    ["de"]="de" \
    ["es"]="es-ES" \
    ["fr"]="fr" \
    ["hu"]="hu" \
    ["it"]="it" \
    ["ja"]="ja" \
    ["ko"]="ko" \
    ["nl"]="nl" \
    ["pl"]="pl" \
    ["pt-BR"]="pt-BR" \
    ["ru"]="ru" \
    ["zh-CN"]="zh-CN" \
    ["zh-TW"]="zh-TW" \
)

function finish {
    rm -rf "${TMPDIR}" "${REPDIR}"
}

function fetch {
    revision=`cat "${LOCALE_JSON}"|jq ".\"${2}\".revision"`
    hg clone "https://hg.mozilla.org/l10n-central/$2" "$1"
    cd $1
    hg update "${revision}"
    rm -rf .hg .hgtags .hgignore
}

trap finish EXIT

# Making tmp dir
mkdir -p "${TMPDIR}"

for lang in ${!LANGS[@]}
do
    (cd "${TMPDIR}"; fetch "${lang}" "${LANGS[$lang]}")
done

mv "${L10NDIR}" "${REPDIR}"
mv "${TMPDIR}" "${L10NDIR}"

# Make sure to revert the Cliqz strings
(
    cd "${ABSDIR}/../"
    files="`git diff --name-only --diff-filter=D|grep cliqz`"
    for f in ${files}
    do
        git checkout -- "${f}"
    done
)

