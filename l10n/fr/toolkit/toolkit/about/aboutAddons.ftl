# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

addons-window =
    .title = Gestionnaire de modules complémentaires
search-header =
    .placeholder = Rechercher sur addons.mozilla.org
    .searchbuttonlabel = Rechercher
search-header-shortcut =
    .key = f
loading-label =
    .value = Chargement…
list-empty-installed =
    .value = Vous n’avez aucun module de ce type installé
list-empty-available-updates =
    .value = Aucune mise à jour trouvée
list-empty-recent-updates =
    .value = Vous n’avez pas mis à jour vos modules complémentaires depuis un certain temps
list-empty-find-updates =
    .label = Vérifier les mises à jour
list-empty-button =
    .label = En apprendre plus sur les modules complémentaires
install-addon-from-file =
    .label = Installer un module depuis un fichier…
    .accesskey = I
help-button = Assistance des modules
preferences =
    { PLATFORM() ->
        [windows] Options de { -brand-short-name }
       *[other] Préférences de { -brand-short-name }
    }
tools-menu =
    .tooltiptext = Outils pour tous les modules
show-unsigned-extensions-button =
    .label = Certaines extensions n’ont pas pu être vérifiées
show-all-extensions-button =
    .label = Afficher toutes les extensions
debug-addons =
    .label = Déboguer des modules
    .accesskey = b
cmd-show-details =
    .label = Afficher plus d’informations
    .accesskey = A
cmd-find-updates =
    .label = Rechercher des mises à jour
    .accesskey = R
cmd-preferences =
    .label =
        { PLATFORM() ->
            [windows] Options
           *[other] Préférences
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
cmd-enable-theme =
    .label = Appliquer le thème
    .accesskey = A
cmd-disable-theme =
    .label = Enlever le thème
    .accesskey = E
cmd-install-addon =
    .label = Installer
    .accesskey = I
cmd-contribute =
    .label = Contribuer
    .accesskey = C
    .tooltiptext = Contribuer au développement de ce module complémentaire
discover-title = Que sont les modules complémentaires ?
discover-description = Les modules complémentaires sont des applications permettant de personnaliser { -brand-short-name } avec des fonctionnalités ou des styles supplémentaires. Gagnez du temps avec un panneau latéral, surveillez la météo ou changez l’apparence de { -brand-short-name } à votre guise.
discover-footer =
    Lorsque votre connexion Internet sera active, ce panneau vous permettra de découvrir
    certains des meilleurs modules complémentaires ainsi que les plus populaires.
detail-version =
    .label = Version
detail-last-updated =
    .label = Dernière mise à jour
detail-contributions-description = Le développeur de ce module demande que vous l’aidiez à assurer la poursuite de son développement en lui versant une petite contribution.
detail-update-type =
    .value = Mises à jour automatiques
detail-update-default =
    .label = Par défaut
    .tooltiptext = N’installer les mises à jour automatiquement que s’il s’agit du réglage par défaut
detail-update-automatic =
    .label = Activées
    .tooltiptext = Installer automatiquement les mises à jour
detail-update-manual =
    .label = Désactivées
    .tooltiptext = Ne pas installer automatiquement les mises à jour
detail-home =
    .label = Page d’accueil
detail-home-value =
    .value = { detail-home.label }
detail-repository =
    .label = Profil du module
detail-repository-value =
    .value = { detail-repository.label }
detail-check-for-updates =
    .label = Vérifier si des mises à jour sont disponibles
    .accesskey = V
    .tooltiptext = Vérifier si des mises à jour sont disponibles pour ce module
detail-show-preferences =
    .label =
        { PLATFORM() ->
            [windows] Options
           *[other] Préférences
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
    .tooltiptext =
        { PLATFORM() ->
            [windows] Modifier les options de ce module
           *[other] Modifier les préférences de ce module
        }
detail-rating =
    .value = Appréciation
addon-restart-now =
    .label = Redémarrer maintenant
disabled-unsigned-heading =
    .value = Certains modules complémentaires ont été désactivés
disabled-unsigned-description = Les modules complémentaires suivants n’ont pas pu être vérifiés pour leur utilisation dans { -brand-short-name }. Vous pouvez <label data-l10n-name="find-addons">rechercher des modules équivalents</label> ou demander au développeur de les faire vérifier.
disabled-unsigned-learn-more = En savoir plus sur nos efforts pour vous apporter plus de sécurité en ligne.
disabled-unsigned-devinfo = Les développeurs souhaitant faire vérifier leurs modules complémentaires peuvent obtenir davantage d’informations en lisant notre <label data-l10n-name="learn-more">manuel</label>.
plugin-deprecation-description = Quelque chose semble manquer ? Certains plugins ne sont plus pris en charge par { -brand-short-name }. <label data-l10n-name="learn-more">En savoir plus.</label>
legacy-warning-show-legacy = Afficher les extensions obsolètes
legacy-extensions =
    .value = Extensions obsolètes
legacy-extensions-description = Ces extensions ne répondent pas aux exigences actuelles de { -brand-short-name }. Elles ont été désactivées. <label data-l10n-name="legacy-learn-more">En savoir plus sur les changements apportés aux modules</label>
extensions-view-discover =
    .name = Catalogue
    .tooltiptext = { extensions-view-discover.name }
extensions-view-recent-updates =
    .name = Mis à jour
    .tooltiptext = { extensions-view-recent-updates.name }
extensions-view-available-updates =
    .name = À mettre à jour
    .tooltiptext = { extensions-view-available-updates.name }

## These are global warnings

extensions-warning-safe-mode-label =
    .value = Tous les modules complémentaires ont été désactivés par le mode sans échec.
extensions-warning-safe-mode-container =
    .tooltiptext = { extensions-warning-safe-mode-label.value }
extensions-warning-check-compatibility-label =
    .value = La vérification de compatibilité des modules complémentaires est désactivée. Vous avez peut-être des modules complémentaires incompatibles.
extensions-warning-check-compatibility-container =
    .tooltiptext = { extensions-warning-check-compatibility-label.value }
extensions-warning-check-compatibility-enable =
    .label = Activer
    .tooltiptext = Activer la vérification de compatibilité
extensions-warning-update-security-label =
    .value = La vérification de mises à jour de sécurité pour les modules complémentaires est désactivée. Vous avez peut-être des modules complémentaires non sûrs.
extensions-warning-update-security-container =
    .tooltiptext = { extensions-warning-update-security-label.value }
extensions-warning-update-security-enable =
    .label = Activer
    .tooltiptext = Activer la vérification de mises à jour de sécurité pour les modules complémentaires

## Strings connected to add-on updates

extensions-updates-check-for-updates =
    .label = Rechercher des mises à jour
    .accesskey = R
extensions-updates-view-updates =
    .label = Afficher les mises à jour récentes
    .accesskey = A

# This menu item is a checkbox that toggles the default global behavior for
# add-on update checking.

extensions-updates-update-addons-automatically =
    .label = Mettre à jour les modules automatiquement
    .accesskey = M

## Specific add-ons can have custom update checking behaviors ("Manually",
## "Automatically", "Use default global behavior"). These menu items reset the
## update checking behavior for all add-ons to the default global behavior
## (which itself is either "Automatically" or "Manually", controlled by the
## extensions-updates-update-addons-automatically.label menu item).

extensions-updates-reset-updates-to-automatic =
    .label = Activer la mise à jour automatique pour tous les modules
    .accesskey = c
extensions-updates-reset-updates-to-manual =
    .label = Désactiver la mise à jour automatique pour tous les modules
    .accesskey = D

## Status messages displayed when updating add-ons

extensions-updates-updating =
    .value = Mise à jour des modules
extensions-updates-installed =
    .value = Vos modules ont été mis à jour.
extensions-updates-downloaded =
    .value = Les mises à jour de vos modules ont été téléchargées.
extensions-updates-restart =
    .label = Redémarrer maintenant pour terminer l’installation
extensions-updates-none-found =
    .value = Aucune mise à jour disponible
extensions-updates-manual-updates-found =
    .label = Afficher les mises à jour disponibles
extensions-updates-update-selected =
    .label = Installer les mises à jour
    .tooltiptext = Installer les mises à jour disponibles dans cette liste
