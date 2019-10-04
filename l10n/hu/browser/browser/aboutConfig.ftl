# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# An old map warning, see https://en.wikipedia.org/wiki/Here_be_dragons
about-config-warning-title = Itt sárkányok vannak!
about-config-warning-text = Ezen speciális beállítások megváltoztatása káros hatással lehet az alkalmazás stabilitására, biztonságára vagy teljesítményére. Csak akkor folytassa, ha tisztában van azzal, hogy mit csinál.
about-config-warning-checkbox = Emlékeztessen továbbra is
about-config-warning-button = Elfogadom a kockázatot
about-config-title = about:config
about-config2-title = Speciális beállítások
about-config-search-input =
    .placeholder = Keresés
about-config-show-all = Összes megjelenítése
about-config-pref-add = Hozzáadás
about-config-pref-toggle = Ki/be
about-config-pref-edit = Szerkesztés
about-config-pref-save = Mentés
about-config-pref-reset = Visszaállítás
about-config-pref-delete = Törlés

## Labels for the type selection radio buttons shown when adding preferences.

about-config-pref-add-type-boolean = Logikai
about-config-pref-add-type-number = Szám
about-config-pref-add-type-string = Karakterlánc

## Preferences with a non-default value are differentiated visually, and at the
## same time the state is made accessible to screen readers using an aria-label
## that won't be visible or copied to the clipboard.
##
## Variables:
##   $value (String): The full value of the preference.

about-config-pref-accessible-value-default =
    .aria-label = { $value } (alapértelmezett)
about-config-pref-accessible-value-custom =
    .aria-label = { $value } (egyéni)
