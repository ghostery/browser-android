# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# An old map warning, see https://en.wikipedia.org/wiki/Here_be_dragons
about-config-warning-title = Prudence, vous partez à l’aventure
about-config-warning-text = La modification de ces préférences avancées peut être dommageable pour la stabilité, la sécurité et les performances de cette application. Ne continuez que si vous savez ce que vous faites.
about-config-warning-checkbox = Me le rappeler la prochaine fois
about-config-warning-button = Je prends le risque
about-config-title = about:config
about-config2-title = Configurations avancées
about-config-search-input =
    .placeholder = Rechercher
about-config-show-all = Tout afficher
about-config-pref-add = Ajouter
about-config-pref-toggle = Inverser
about-config-pref-edit = Modifier
about-config-pref-save = Enregistrer
about-config-pref-reset = Réinitialiser
about-config-pref-delete = Supprimer

## Labels for the type selection radio buttons shown when adding preferences.

about-config-pref-add-type-boolean = Booléen
about-config-pref-add-type-number = Nombre
about-config-pref-add-type-string = Chaîne

## Preferences with a non-default value are differentiated visually, and at the
## same time the state is made accessible to screen readers using an aria-label
## that won't be visible or copied to the clipboard.
##
## Variables:
##   $value (String): The full value of the preference.

about-config-pref-accessible-value-default =
    .aria-label = { $value } (par défaut)
about-config-pref-accessible-value-custom =
    .aria-label = { $value } (personnalisée)
