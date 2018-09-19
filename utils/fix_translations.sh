#!/bin/bash

l10n=`dirname "$0"|xargs -I '{}' readlink -f "{}/../l10n"`

[ -d "$l10n" ] || (echo "$l10n doesn't exists"l; exit 1)
find "$l10n" -name "cliqz_strings.dtd" -print0|xargs -0 sed -i -r "s/\&amp;(\w+;)/\&\1/g;s/\b'\b/\\\'/g"
