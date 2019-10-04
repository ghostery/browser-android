# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### Localization for Developer Tools tooltips.

learn-more = <span data-l10n-name="link">En savoir plus </ span>

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

inactive-css-not-grid-or-flex-container = <strong>{ $property }</strong> n’a aucun effet sur cet élément, car il ne s’agit ni d’un conteneur flex, ni d’un conteneur de grille.
inactive-css-not-grid-or-flex-container-or-multicol-container = <strong>{ $property }</strong> n’a aucun effet sur cet élément, car ce n’est ni un conteneur flex, ni un conteneur de grille ni un conteneur de plusieurs colonnes.
inactive-css-not-grid-or-flex-item = <strong>{ $property }</strong> n’a aucun effet sur cet élément, car il ne s’agit ni d’un élément de grille ni d’un élément flexible.
inactive-css-not-grid-item = <strong>{ $property }</strong> n’a aucun effet sur cet élément, car il ne s’agit pas d’un élément de grille.
inactive-css-not-grid-container = <strong>{ $property }</strong> n’a aucun effet sur cet élément, car il ne s’agit pas d’un conteneur de grille.
inactive-css-not-flex-item = <strong>{ $property }</strong> n’a aucun effet sur cet élément, car il ne s’agit pas d’un élément flexible.
inactive-css-not-flex-container = <strong>{ $property }</strong> n’a aucun effet sur cet élément, car il ne s’agit pas d’un conteneur d’éléments flexibles.
inactive-css-not-inline-or-tablecell = <strong>{ $property }</strong> n’a aucun effet sur cet élément, car il ne s’agit ni d’un élément « inline » ni d’un élément « table-cell ».
inactive-css-property-because-of-display = <strong>{ $property }</strong> n’a aucun effet sur cet élément car la valeur de sa propriété « display » est <strong>{ $display }</strong>.

## In the Rule View when a CSS property cannot be successfully applied we display
## an icon. When this icon is hovered this message is displayed to explain how
## the problem can be solved.

inactive-css-not-grid-or-flex-container-fix = Essayez d’ajouter <strong>display: grid</strong> ou <strong>display: flex</strong>. { learn-more }
inactive-css-not-grid-or-flex-container-or-multicol-container-fix = Essayez d’ajouter <strong>display:grid</strong>, <strong>display:flex</strong>, ou <strong>columns:2</strong>. { learn-more }
inactive-css-not-grid-or-flex-item-fix = Essayez d’ajouter <strong>display: grid</strong>, <strong>display: flex</strong>, <strong>display: inline-grid</strong> ou <strong>display: inline-flex</strong>. { learn-more }
inactive-css-not-grid-item-fix = Essayez d’ajouter <strong>display: grid</strong> ou <strong>display:inline-grid</strong> à l’élément parent. { learn-more }
inactive-css-not-grid-container-fix = Essayez d’ajouter <strong>display: grid</strong> ou <strong>display:inline-grid</strong>. { learn-more }
inactive-css-not-flex-item-fix = Essayez d’ajouter <strong>display:flex</strong> or <strong>display:inline-flex</strong> à l’élément parent. { learn-more }
inactive-css-not-flex-container-fix = Essayez d’ajouter <strong>display:flex</strong> ou <strong>display:inline-flex</strong>. { learn-more }
inactive-css-not-inline-or-tablecell-fix = Essayez d’ajouter <strong>display:inline</strong> ou <strong>display:table-cell</strong>. { learn-more }
inactive-css-non-replaced-inline-or-table-row-or-row-group-fix = Essayez d’ajouter <strong>display:inline-block</strong> ou <strong>display:block</strong>. { learn-more }
inactive-css-non-replaced-inline-or-table-column-or-column-group-fix = Essayez d’ajouter <strong>display:inline-block</strong>. { learn-more }
