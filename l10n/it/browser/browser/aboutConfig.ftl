# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# An old map warning, see https://en.wikipedia.org/wiki/Here_be_dragons
about-config-warning-title = Qui ci sono i draghi!
about-config-warning-text = La modifica di queste impostazioni avanzate può compromettere la stabilità, la sicurezza e le prestazioni del browser. Si consiglia di proseguire solo se consapevoli delle proprie azioni.
about-config-warning-checkbox = Ricordamelo la prossima volta, grazie.
about-config-warning-button = Accetto i rischi

about-config2-title = Configurazioni avanzate

about-config-search-input =
    .placeholder = Cerca
about-config-show-all = Visualizza tutto

about-config-pref-add = Aggiungi
about-config-pref-toggle = Cambia
about-config-pref-edit = Modifica
about-config-pref-save = Salva
about-config-pref-reset = Ripristina
about-config-pref-delete = Elimina

## Labels for the type selection radio buttons shown when adding preferences.
about-config-pref-add-type-boolean = Booleano
about-config-pref-add-type-number = Numero
about-config-pref-add-type-string = Stringa

## Preferences with a non-default value are differentiated visually, and at the
## same time the state is made accessible to screen readers using an aria-label
## that won't be visible or copied to the clipboard.
##
## Variables:
##   $value (String): The full value of the preference.
about-config-pref-accessible-value-default =
    .aria-label = { $value } (predefinito)
about-config-pref-accessible-value-custom =
    .aria-label = { $value } (personalizzato)
