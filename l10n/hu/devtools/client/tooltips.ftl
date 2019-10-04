# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### Localization for Developer Tools tooltips.

learn-more = <span data-l10n-name="link">További tudnivalók</span>

## In the Rule View when a CSS property cannot be successfully applied we display
## an icon. When this icon is hovered this message is displayed to explain why
## the property is not applied.
## Variables:
##   $property (string) - A CSS property name e.g. "color".


## In the Rule View when a CSS property cannot be successfully applied we display
## an icon. When this icon is hovered this message is displayed to explain why
## the property is not applied.
## Variables:
##   $property (string) - A CSS property name e.g. "color".
##   $display (string) - A CSS display value e.g. "inline-block".

inactive-css-not-grid-or-flex-container = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel nem flexibilis vagy rácstároló.
inactive-css-not-grid-or-flex-container-or-multicol-container = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel ez nem flex konténer, rács- vagy többoszlopos tároló.
inactive-css-not-grid-or-flex-item = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel nem rács vagy flexibilis elem.
inactive-css-not-grid-item = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel nem egy rácselem.
inactive-css-not-grid-container = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel nem rácstároló.
inactive-css-not-flex-item = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel nem flexibilis elem.
inactive-css-not-flex-container = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel nem flexibilis tároló.
inactive-css-not-inline-or-tablecell = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel nem „inline” vagy „table-cell” elem.
inactive-css-property-because-of-display = A(z) <strong>{ $property }</strong> nem befolyásolja ezt az elemet, mivel a „display” értéke <strong>{ $display }</strong>.

## In the Rule View when a CSS property cannot be successfully applied we display
## an icon. When this icon is hovered this message is displayed to explain how
## the problem can be solved.

inactive-css-not-grid-or-flex-container-fix = Próbálja meg ezeket hozzáadni: <strong>display:grid</strong> vagy <strong>display:flex</strong>. { learn-more }
inactive-css-not-grid-or-flex-container-or-multicol-container-fix = Próbálja meg ezeket hozzáadni: <strong>display:grid</strong>, <strong>display:flex</strong> vagy <strong>columns:2</strong>. { learn-more }
inactive-css-not-grid-or-flex-item-fix = Próbálja meg ezeket hozzáadni: <strong>display:grid</strong>, <strong>display:flex</strong>, <strong>display:inline-grid</strong> vagy <strong>display:inline-flex</strong>. { learn-more }
inactive-css-not-grid-item-fix = Próbálja meg ezeket hozzáadni az elem szülőjéhez: <strong>display:grid</strong> vagy <strong>display:inline-grid</strong>. { learn-more }
inactive-css-not-grid-container-fix = Próbálja meg ezeket hozzáadni: <strong>display:grid</strong> vagy <strong>display:inline-grid</strong>. { learn-more }
inactive-css-not-flex-item-fix = Próbálja meg ezeket hozzáadni az elem szülőjéhez: <strong>display:flex</strong> vagy <strong>display:inline-flex</strong>. { learn-more }
inactive-css-not-flex-container-fix = Próbálja meg ezeket hozzáadni: <strong>display:flex</strong> vagy <strong>display:inline-flex</strong>. { learn-more }
inactive-css-not-inline-or-tablecell-fix = Próbálja meg ezeket hozzáadni: <strong>display:inline</strong> vagy <strong>display:table-cell</strong>.{ learn-more }
inactive-css-non-replaced-inline-or-table-row-or-row-group-fix = Próbálja meg ezeket hozzáadni: <strong>display:inline-block</strong> vagy <strong>display:block</strong>. { learn-more }
inactive-css-non-replaced-inline-or-table-column-or-column-group-fix = Próbálja meg ezt hozzáadni: <strong>display:inline-block</strong>. { learn-more }
