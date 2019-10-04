# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

### Localization for Developer Tools tooltips.

learn-more = <span data-l10n-name="link">Ulteriori informazioni</span>

## In the Rule View when a CSS property cannot be successfully applied we display
## an icon. When this icon is hovered this message is displayed to explain why
## the property is not applied.
## Variables:
##   $property (string) - A CSS property name e.g. "color".

inactive-css-not-grid-or-flex-container = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto non è un contenitore flex o griglia.
inactive-css-not-grid-or-flex-container-or-multicol-container = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto non è un contenitore flex, un contenitore griglia o un contenitore multicolonna.
inactive-css-not-grid-or-flex-item = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto non è un elemento flex o griglia.
inactive-css-not-grid-item = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto non è un elemento griglia.
inactive-css-not-grid-container = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto non è un contenitore griglia.
inactive-css-not-flex-item = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto non è un elemento flex.
inactive-css-not-flex-container = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto non è un contenitore flex.
inactive-css-not-inline-or-tablecell = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto non è “inline” o “table-cell”.
inactive-css-property-because-of-display = <strong>{ $property }</strong> non ha effetto su questo elemento in quanto il valore di “display” è <strong>{ $display }</strong>.

## In the Rule View when a CSS property cannot be successfully applied we display
## an icon. When this icon is hovered this message is displayed to explain how
## the problem can be solved.

inactive-css-not-grid-or-flex-container-fix = Provare ad aggiungere <strong>display:grid</strong> o <strong>display:flex</strong>. { learn-more }
inactive-css-not-grid-or-flex-container-or-multicol-container-fix = Provare ad aggiungere <strong>display:grid</strong>, <strong>display:flex</strong> o <strong>columns:2</strong>. { learn-more }
inactive-css-not-grid-or-flex-item-fix = Provare ad aggiungere <strong>display:grid</strong>, <strong>display:flex</strong>, <strong>display:inline-grid</strong> o <strong>display:inline-flex</strong>. { learn-more }
inactive-css-not-grid-item-fix =Provare ad aggiungere <strong>display:grid</strong> o <strong>display:inline-grid</strong> al genitore dell’elemento. { learn-more }
inactive-css-not-grid-container-fix = Provare ad aggiungere <strong>display:grid</strong> o <strong>display:inline-grid</strong>. { learn-more }
inactive-css-not-flex-item-fix = Provare ad aggiungere <strong>display:flex</strong> o <strong>display:inline-flex</strong> al genitore dell’elemento. { learn-more }
inactive-css-not-flex-container-fix = Provare ad aggiungere <strong>display:flex</strong> o <strong>display:inline-flex</strong>. { learn-more }
inactive-css-not-inline-or-tablecell-fix = Provare ad aggiungere <strong>display:inline</strong> o <strong>display:table-cell</strong>. { learn-more }
inactive-css-non-replaced-inline-or-table-row-or-row-group-fix = Provare ad aggiungere <strong>display:inline-block</strong> o <strong>display:block</strong>. { learn-more }
inactive-css-non-replaced-inline-or-table-column-or-column-group-fix = Provare ad aggiungere <strong>display:inline-block</strong>. { learn-more }
