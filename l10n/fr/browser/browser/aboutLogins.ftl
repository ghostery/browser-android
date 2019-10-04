# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

about-logins-page-title = Identifiants et mots de passe

# "Google Play" and "App Store" are both branding and should not be translated

login-app-promo-title = Emportez vos mots de passe partout
login-app-promo-subtitle = Téléchargez l’application gratuite { -lockwise-brand-name }
login-app-promo-android =
    .alt = Disponible sur Google Play
login-app-promo-apple =
    .alt = Télécharger dans l’App Store
login-filter =
    .placeholder = Rechercher des identifiants
create-login-button = Créer un nouvel identifiant
# This string is used as alternative text for favicon images.
# Variables:
#   $title (String) - The title of the website associated with the favicon.
login-favicon =
    .alt = Icône pour { $title }
fxaccounts-sign-in-text = Accédez à vos mots de passe sur vos autres appareils
fxaccounts-sign-in-button = Se connecter à { -sync-brand-short-name }
fxaccounts-avatar-button =
    .title = Gérer le compte

## The ⋯ menu that is in the top corner of the page

menu =
    .title = Ouvrir le menu
# This menuitem is only visible on Windows
menu-menuitem-import = Importer des mots de passe…
menu-menuitem-preferences =
    { PLATFORM() ->
        [windows] Options
       *[other] Préférences
    }
menu-menuitem-feedback = Donner mon avis
menu-menuitem-faq = Questions fréquentes
menu-menuitem-android-app = { -lockwise-brand-short-name } pour Android
menu-menuitem-iphone-app = { -lockwise-brand-short-name } pour iPhone et iPad

## Login List

login-list =
    .aria-label = Identifiants correspondants à la recherche
login-list-count =
    { $count ->
        [one] { $count } identifiant
       *[other] { $count } identifiants
    }
login-list-sort-label-text = Trier par :
login-list-name-option = Nom (A-Z)
login-list-breached-option = Fuites de données de sites
login-list-last-changed-option = Dernière modification
login-list-last-used-option = Dernière utilisation
login-list-intro-title = Aucun identifiant trouvé
login-list-intro-description = Lorsque vous enregistrez un mot de passe dans { -brand-product-name }, il apparaît ici.
login-list-item-title-new-login = Nouvel identifiant
login-list-item-subtitle-new-login = Saisissez vos informations de connexion
login-list-item-subtitle-missing-username = (aucun nom d’utilisateur)

## Introduction screen

login-intro-heading = Vous recherchez vos identifiants enregistrés ? Configurez { -sync-brand-short-name }.
login-intro-description = Si vous avez enregistré vos identifiants dans { -brand-product-name } sur un autre appareil, voici comment y accéder ici :
login-intro-instruction-fxa = Connectez-vous ou créez un { -fxaccount-brand-name } sur l’appareil où vos identifiants sont enregistrés
login-intro-instruction-fxa-settings = Assurez-vous d’avoir coché la case Identifiants dans les paramètres de { -sync-brand-short-name }
login-intro-instruction-faq = Consultez la <a data-l10n-name="faq">foire aux questions</a> de { -lockwise-brand-short-name } pour obtenir de l’aide supplémentaire

## Login

login-item-new-login-title = Créer un nouvel identifiant
login-item-edit-button = Modifier
login-item-delete-button = Supprimer
login-item-origin-label = Adresse web
login-item-origin =
    .placeholder = https://www.example.com
login-item-open-site-button = Ouvrir
login-item-username-label = Nom d’utilisateur
login-item-username =
    .placeholder = nom@example.com
login-item-copy-username-button-text = Copier
login-item-copied-username-button-text = Copié !
login-item-password-label = Mot de passe
login-item-password-reveal-checkbox-show =
    .title = Afficher le mot de passe
login-item-password-reveal-checkbox-hide =
    .title = Masquer le mot de passe
login-item-copy-password-button-text = Copier
login-item-copied-password-button-text = Copié !
login-item-save-changes-button = Enregistrer les modifications
login-item-save-new-button = Enregistrer
login-item-cancel-button = Annuler
login-item-time-changed = Dernière modification : { DATETIME($timeChanged, day: "numeric", month: "long", year: "numeric") }
login-item-time-created = Créé le : { DATETIME($timeCreated, day: "numeric", month: "long", year: "numeric") }
login-item-time-used = Dernière utilisation : { DATETIME($timeUsed, day: "numeric", month: "long", year: "numeric") }

## Master Password notification

master-password-notification-message = Veuillez saisir votre mot de passe principal pour afficher les identifiants et mots de passe enregistrés.
master-password-reload-button =
    .label = Connexion
    .accesskey = C

## Dialogs

confirmation-dialog-cancel-button = Annuler
confirmation-dialog-dismiss-button =
    .title = Annuler
enable-password-sync-notification-message =
    { PLATFORM() ->
        [windows] Vous souhaitez accéder à vos identifiants partout où vous utilisez { -brand-product-name } ? Rendez-vous dans les options de { -sync-brand-short-name } et cochez la case Identifiants.
       *[other] Vous souhaitez accéder à vos identifiants partout où vous utilisez { -brand-product-name } ? Rendez-vous dans les préférences de { -sync-brand-short-name } et cochez la case Identifiants.
    }
enable-password-sync-preferences-button =
    .label =
        { PLATFORM() ->
            [windows] Consulter les options de { -sync-brand-short-name }
           *[other] Consulter les préférences de { -sync-brand-short-name }
        }
    .accesskey = C
confirm-delete-dialog-title = Supprimer cet identifiant ?
confirm-delete-dialog-message = Cette action est irréversible.
confirm-delete-dialog-confirm-button = Supprimer
confirm-discard-changes-dialog-title = Ignorer les modifications non enregistrées ?
confirm-discard-changes-dialog-message = Toutes les modifications non enregistrées seront perdues.
confirm-discard-changes-dialog-confirm-button = Ignorer

## Breach Alert notification

breach-alert-text = Les mots de passe de ce site ont été divulgués ou volés après la dernière modification de vos informations de connexion. Changez votre mot de passe pour protéger votre compte.
breach-alert-link = En savoir plus sur cette fuite de données.
breach-alert-dismiss =
    .title = Fermer cette alerte
