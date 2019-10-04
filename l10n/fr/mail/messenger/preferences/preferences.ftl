# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

choose-messenger-language-description = Choisissez les langues utilisées pour afficher les menus, messages et notifications de { -brand-short-name }.
manage-messenger-languages-button =
    .label = Choisir des alternatives…
    .accesskey = l
confirm-messenger-language-change-description = Redémarrez { -brand-short-name } pour appliquer ces modifications
confirm-messenger-language-change-button = Appliquer et redémarrer
update-pref-write-failure-title = Échec d’écriture
# Variables:
#   $path (String) - Path to the configuration file
update-pref-write-failure-message = Impossible d’enregistrer la préférence. Impossible d’écrire dans le fichier : { $path }
update-setting-write-failure-title = Erreur lors de l’enregistrement des préférences de mise à jour
# Variables:
#   $path (String) - Path to the configuration file
# The newlines between the main text and the line containing the path is
# intentional so the path is easier to identify.
update-setting-write-failure-message =
    { -brand-short-name } a rencontré une erreur et n’a pas enregistré cette modification. Notez que modifier cette préférence de mise à jour nécessite la permission d’écriture pour le fichier ci-dessous. Vous, ou un administrateur système, pouvez peut-être corriger l’erreur en accordant au groupe Users l’accès complet à ce fichier.
    
    Écriture impossible dans le fichier : { $path }
update-in-progress-title = Mise à jour en cours
update-in-progress-message = Voulez-vous que { -brand-short-name } continue cette mise à jour ?
update-in-progress-ok-button = &Abandonner
# Continue is the cancel button so pressing escape or using a platform standard
# method of closing the UI will not discard the update.
update-in-progress-cancel-button = &Continuer
