# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# An old map warning, see https://en.wikipedia.org/wiki/Here_be_dragons
about-config-warning-title = Uwaga, niebezpieczeństwo!
about-config-warning-text = Modyfikacja tych ustawień może spowodować problemy, takie jak utrata stabilności i wydajności programu oraz zagrożenie bezpieczeństwa. Kontynuuj tylko wtedy, gdy masz pewność tego, co robisz.
about-config-warning-checkbox = Wyświetlanie tego ostrzeżenia za każdym razem
about-config-warning-button = Akceptuję ryzyko
about-config-title = about:config
about-config2-title = Konfiguracja zaawansowana
about-config-search-input =
    .placeholder = Szukaj
about-config-show-all = Wyświetl wszystko
about-config-pref-add = Dodaj
about-config-pref-toggle = Przełącz
about-config-pref-edit = Edytuj
about-config-pref-save = Zapisz
about-config-pref-reset = Resetuj
about-config-pref-delete = Usuń

## Labels for the type selection radio buttons shown when adding preferences.

about-config-pref-add-type-boolean = Wartość logiczna
about-config-pref-add-type-number = Liczba
about-config-pref-add-type-string = Łańcuch

## Preferences with a non-default value are differentiated visually, and at the
## same time the state is made accessible to screen readers using an aria-label
## that won't be visible or copied to the clipboard.
##
## Variables:
##   $value (String): The full value of the preference.

about-config-pref-accessible-value-default =
    .aria-label = { $value } (domyślne)
about-config-pref-accessible-value-custom =
    .aria-label = { $value } (zmienione)
