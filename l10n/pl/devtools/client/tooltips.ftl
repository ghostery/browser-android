# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### Localization for Developer Tools tooltips.

learn-more = <span data-l10n-name="link">Więcej informacji</span>

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

inactive-css-not-grid-or-flex-container = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ nie jest on kontenerem Flex ani Grid.
inactive-css-not-grid-or-flex-container-or-multicol-container = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ nie jest on kontenerem Flex, kontenerem Grid ani kontenerem wielokolumnowym.
inactive-css-not-grid-or-flex-item = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ nie jest on elementem Grid ani Flex.
inactive-css-not-grid-item = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ nie jest on elementem Grid.
inactive-css-not-grid-container = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ nie jest on kontenerem Grid.
inactive-css-not-flex-item = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ nie jest on elementem Flex.
inactive-css-not-flex-container = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ nie jest on kontenerem Flex.
inactive-css-not-inline-or-tablecell = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ nie jest on elementem liniowym lub komórki tabeli.
inactive-css-property-because-of-display = <strong>{ $property }</strong> nie ma wpływu na ten element, ponieważ wyświetla <strong>{ $display }</strong>.

## In the Rule View when a CSS property cannot be successfully applied we display
## an icon. When this icon is hovered this message is displayed to explain how
## the problem can be solved.

inactive-css-not-grid-or-flex-container-fix = Spróbuj dodać <strong>display:grid</strong> lub <strong>display:flex</strong>. { learn-more }
inactive-css-not-grid-or-flex-container-or-multicol-container-fix = Spróbuj dodać <strong>display:grid</strong>, <strong>display:flex</strong> lub <strong>columns:2</strong>. { learn-more }
inactive-css-not-grid-or-flex-item-fix = Spróbuj dodać <strong>display:grid</strong>, <strong>display:flex</strong>, <strong>display:inline-grid</strong> lub <strong>display:inline-flex</strong>. { learn-more }
inactive-css-not-grid-item-fix = Spróbuj dodać <strong>display:grid</strong> lub <strong>display:inline-grid</strong> do elementu nadrzędnego. { learn-more }
inactive-css-not-grid-container-fix = Spróbuj dodać <strong>display:grid</strong> lub <strong>display:inline-grid</strong>. { learn-more }
inactive-css-not-flex-item-fix = Spróbuj dodać <strong>display:flex</strong> lub <strong>display:inline-flex</strong> do elementu nadrzędnego. { learn-more }
inactive-css-not-flex-container-fix = Spróbuj dodać <strong>display:flex</strong> lub <strong>display:inline-flex</strong>. { learn-more }
inactive-css-not-inline-or-tablecell-fix = Spróbuj dodać <strong>display:inline</strong> lub <strong>display:table-cell</strong>. { learn-more }
inactive-css-non-replaced-inline-or-table-row-or-row-group-fix = Spróbuj dodać <strong>display:inline-block</strong> lub <strong>display:block</strong>. { learn-more }
inactive-css-non-replaced-inline-or-table-column-or-column-group-fix = Spróbuj dodać <strong>display:inline-block</strong>. { learn-more }
