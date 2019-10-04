# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## These messages are used as headings in the recommendation doorhanger

cfr-doorhanger-extension-heading = Extension recommandée
cfr-doorhanger-feature-heading = Fonctionnalité recommandée
cfr-doorhanger-pintab-heading = Essayez ceci : épingler un onglet



cfr-doorhanger-extension-sumo-link =
    .tooltiptext = Pourquoi ceci s’affiche-t-il ?
cfr-doorhanger-extension-cancel-button = Pas maintenant
    .accesskey = P
cfr-doorhanger-extension-ok-button = Ajouter maintenant
    .accesskey = A
cfr-doorhanger-pintab-ok-button = Épingler cet onglet
    .accesskey = i
cfr-doorhanger-extension-manage-settings-button = Gérer les paramètres de recommandation
    .accesskey = G
cfr-doorhanger-extension-never-show-recommendation = Ne pas montrer cette recommandation
    .accesskey = N
cfr-doorhanger-extension-learn-more-link = En savoir plus
# This string is used on a new line below the add-on name
# Variables:
#   $name (String) - Add-on author name
cfr-doorhanger-extension-author = par { $name }
# This is a notification displayed in the address bar.
# When clicked it opens a panel with a message for the user.
cfr-doorhanger-extension-notification = Recommandation

## Add-on statistics
## These strings are used to display the total number of
## users and rating for an add-on. They are shown next to each other.

# Variables:
#   $total (Number) - The rating of the add-on from 1 to 5
cfr-doorhanger-extension-rating =
    .tooltiptext =
        { $total ->
            [one] { $total } étoile
           *[other] { $total } étoiles
        }
# Variables:
#   $total (Number) - The total number of users using the add-on
cfr-doorhanger-extension-total-users =
    { $total ->
        [one] { $total } utilisateur
       *[other] { $total } utilisateurs
    }
cfr-doorhanger-pintab-description = Obtenez un accès facile à vos sites les plus utilisés. Gardez les sites ouverts dans un onglet (même lorsque vous redémarrez).

## These messages are steps on how to use the feature and are shown together.

cfr-doorhanger-pintab-step1 = <b>Effectuez un clic droit</b> sur l’onglet que vous souhaitez épingler.
cfr-doorhanger-pintab-step2 = Sélectionnez <b>Épingler cet onglet</b> dans le menu.
cfr-doorhanger-pintab-step3 = Si le site est mis à jour vous verrez un point bleu apparaître sur votre onglet épinglé.
cfr-doorhanger-pintab-animation-pause = Pause
cfr-doorhanger-pintab-animation-resume = Reprendre

## Firefox Accounts Message

cfr-doorhanger-bookmark-fxa-header = Synchronisez vos marque-pages partout.
cfr-doorhanger-bookmark-fxa-body = Vous avez déniché la perle rare ! Maintenant, retrouvez ce marque-page sur vos appareils mobiles. C’est le moment d’utiliser un { -fxaccount-brand-name }.
cfr-doorhanger-bookmark-fxa-link-text = Synchroniser les marque-pages maintenant…
cfr-doorhanger-bookmark-fxa-close-btn-tooltip =
    .aria-label = Bouton de fermeture
    .title = Fermer

## Protections panel

cfr-protections-panel-header = Naviguez sans être suivi·e
cfr-protections-panel-body = Gardez vos données pour vous. { -brand-short-name } vous protège de la plupart des traqueurs les plus courants qui suivent ce que vous faites en ligne.
cfr-protections-panel-link-text = En savoir plus

## What's New toolbar button and panel

cfr-whatsnew-button =
    .label = Nouveautés
    .tooltiptext = Nouveautés
cfr-whatsnew-panel-header = Nouveautés

## Bookmark Sync

cfr-doorhanger-sync-bookmarks-header = Accédez à ce marque-page sur votre téléphone
cfr-doorhanger-sync-bookmarks-body = Emportez vos marque-pages, vos mots de passe, votre historique et bien d’autres choses sur tous les appareils connectés à votre compte { -brand-product-name }.
cfr-doorhanger-sync-bookmarks-ok-button = Activer { -sync-brand-short-name }
    .accesskey = A

## Login Sync

cfr-doorhanger-sync-logins-header = Ne perdez plus jamais vos mots de passe
cfr-doorhanger-sync-logins-body = Enregistrez et synchronisez vos mots de passe sur tous vos appareils en toute sécurité.
cfr-doorhanger-sync-logins-ok-button = Activer { -sync-brand-short-name }
    .accesskey = A

## Send Tab

cfr-doorhanger-send-tab-header = Lisez ceci en déplacement
cfr-doorhanger-send-tab-recipe-header = Lisez cette recette dans la cuisine
cfr-doorhanger-send-tab-body = « Envoyer l’onglet » vous permet de transférer facilement ce lien à votre téléphone ou à tout autre appareil connecté à votre compte { -brand-product-name }.
cfr-doorhanger-send-tab-ok-button = Essayer l’envoi d’onglet
    .accesskey = E

## Firefox Send

cfr-doorhanger-firefox-send-header = Partagez ce PDF en toute sécurité
cfr-doorhanger-firefox-send-body = Protégez vos documents sensibles des regards indiscrets avec un chiffrement de bout en bout et un lien qui disparaît lorsque vous avez terminé.
cfr-doorhanger-firefox-send-ok-button = Essayer { -send-brand-name }
    .accesskey = E
