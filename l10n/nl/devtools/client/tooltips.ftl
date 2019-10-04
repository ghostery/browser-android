# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### Localization for Developer Tools tooltips.

learn-more = <span data-l10n-name="link">Meer informatie</span>

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

inactive-css-not-grid-or-flex-container = <strong>{ $property }</strong> heeft geen effect op dit element aangezien het geen flexcontainer of gridcontainer is.
inactive-css-not-grid-or-flex-container-or-multicol-container = <strong>{ $property }</strong> heeft geen effect op dit element, omdat het geen flex-container, grid-container of een container met meerdere kolommen is.
inactive-css-not-grid-or-flex-item = <strong>{ $property }</strong> heeft geen effect op dit element aangezien het geen grid- of flexitem is.
inactive-css-not-grid-item = <strong>{ $property }</strong> heeft geen effect op dit element aangezien het geen griditem is.
inactive-css-not-grid-container = <strong>{ $property }</strong> heeft geen effect op dit element aangezien het geen gridcontainer is.
inactive-css-not-flex-item = <strong>{ $property }</strong> heeft geen effect op dit element aangezien het geen flexitem is.
inactive-css-not-flex-container = <strong>{ $property }</strong> heeft geen effect op dit element aangezien het geen flexcontainer is.
inactive-css-not-inline-or-tablecell = <strong>{ $property }</strong> heeft geen effect op dit element, omdat het geen inline of table-cell-element is.
inactive-css-property-because-of-display = <strong>{ $property }</ strong> heeft geen effect op dit element, omdat het een weergave van <strong>{ $display }</ strong> heeft.

## In the Rule View when a CSS property cannot be successfully applied we display
## an icon. When this icon is hovered this message is displayed to explain how
## the problem can be solved.

inactive-css-not-grid-or-flex-container-fix = Probeer <strong>display:grid</strong> of <strong>display:flex</strong> toe te voegen. { learn-more }
inactive-css-not-grid-or-flex-container-or-multicol-container-fix = Probeer <strong>display:grid</strong>, <strong>display:flex</strong> of <strong>colums:2</strong> toe te voegen. { learn-more }
inactive-css-not-grid-or-flex-item-fix = Probeer <strong>display:grid</strong>, <strong>display:flex</strong>, <strong>display:inline-grid</strong> of <strong>display:inline-flex</strong> toe te voegen. { learn-more }
inactive-css-not-grid-item-fix = Probeer <strong>display:grid</strong> of <strong>display:inline-grid</strong> toe te voegen aan het bovenliggende element. { learn-more }
inactive-css-not-grid-container-fix = Probeer <strong>display:grid</strong> of <strong>display:inline-grid</strong> toe te voegen. { learn-more }
inactive-css-not-flex-item-fix = Probeer <strong>display:flex</strong> of <strong>display:inline-flex</strong> toe te voegen aan het bovenliggende element. { learn-more }
inactive-css-not-flex-container-fix = Probeer <strong>display:flex</strong> of <strong>display:inline-flex</strong> toe te voegen. { learn-more }
inactive-css-not-inline-or-tablecell-fix = Probeer <strong>display:inline</strong> of <strong>display:table-cell</strong> toe te voegen. { learn-more }
inactive-css-non-replaced-inline-or-table-row-or-row-group-fix = Probeer <strong>display:inline-block</ strong> of <strong>display:block</ strong> toe te voegen. { learn-more }
inactive-css-non-replaced-inline-or-table-column-or-column-group-fix = Probeer <strong>display:inline-block</ strong> toe te voegen. { learn-more }
