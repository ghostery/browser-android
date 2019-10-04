# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# An old map warning, see https://en.wikipedia.org/wiki/Here_be_dragons
about-config-warning-title = Here be dragons!
about-config-warning-text = Het wijzigen van deze uitgebreide voorkeuren kan schadelijk zijn voor de stabiliteit, veiligheid en prestaties van deze toepassing. Ga alleen door als u weet wat u doet.
about-config-warning-checkbox = Blijf me tot last zijn!
about-config-warning-button = Ik aanvaard het risico

about-config-title = about:config

about-config2-title = Geavanceerde configuraties

about-config-search-input =
    .placeholder = Zoeken
about-config-show-all = Alles tonen

about-config-pref-add = Toevoegen
about-config-pref-toggle = Omschakelen
about-config-pref-edit = Bewerken
about-config-pref-save = Opslaan
about-config-pref-reset = Herinitialiseren
about-config-pref-delete = Verwijderen

## Labels for the type selection radio buttons shown when adding preferences.
about-config-pref-add-type-boolean = Boolean
about-config-pref-add-type-number = Getal
about-config-pref-add-type-string = String

## Preferences with a non-default value are differentiated visually, and at the
## same time the state is made accessible to screen readers using an aria-label
## that won't be visible or copied to the clipboard.
##
## Variables:
##   $value (String): The full value of the preference.
about-config-pref-accessible-value-default =
    .aria-label = { $value } (standaard)
about-config-pref-accessible-value-custom =
    .aria-label = { $value } (aangepast)
