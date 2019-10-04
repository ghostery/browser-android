# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### UI strings for the simplified onboarding modal / about:welcome
### Various strings use a non-breaking space to avoid a single dangling /
### widowed word, so test on various window sizes if you also want this.


## These button action text can be split onto multiple lines, so use explicit
## newlines in translations to control where the line break appears (e.g., to
## avoid breaking quoted text).

onboarding-button-label-learn-more = En savoir plus
onboarding-button-label-try-now = Essayer
onboarding-button-label-get-started = Pour commencer

## Welcome modal dialog strings

onboarding-welcome-header = Bienvenue dans { -brand-short-name }
onboarding-welcome-body = Vous avez le navigateur.<br/>Découvrez les autres ressources de { -brand-product-name }.
onboarding-welcome-learn-more = Découvrir tous les avantages.
onboarding-join-form-header = Rejoignez { -brand-product-name }
onboarding-join-form-body = Saisissez votre adresse électronique pour vous lancer.
onboarding-join-form-email =
    .placeholder = Adresse électronique
onboarding-join-form-email-error = Adresse électronique valide requise
onboarding-join-form-legal = En continuant, vous acceptez nos <a data-l10n-name="terms">Conditions d’utilisation</a> et notre <a data-l10n-name="privacy">Politique de confidentialité</a>.
onboarding-join-form-continue = Continuer
onboarding-start-browsing-button-label = Commencer la navigation
onboarding-cards-dismiss =
    .title = Retirer
    .aria-label = Retirer

## Firefox Sync modal dialog strings.

onboarding-sync-welcome-header = Emportez { -brand-product-name } avec vous
onboarding-sync-welcome-content = Accédez à vos marque-pages, votre historique, vos mots de passe et d’autres paramètres sur l’ensemble de vos appareils.
onboarding-sync-welcome-learn-more-link = En savoir plus sur les comptes Firefox
onboarding-sync-form-invalid-input = Adresse électronique valide requise
onboarding-sync-legal-notice = En continuant, vous acceptez les <a data-l10n-name="terms">Conditions d’utilisation</a> et la <a data-l10n-name="privacy">Politique de confidentialité</a>.
onboarding-sync-form-input =
    .placeholder = Adresse électronique
onboarding-sync-form-continue-button = Continuer
onboarding-sync-form-skip-login-button = Ignorer cette étape

## This is part of the line "Enter your email to continue to Firefox Sync"

onboarding-sync-form-header = Saisissez votre adresse électronique
onboarding-sync-form-sub-header = pour continuer avec { -sync-brand-name }.

## These are individual benefit messages shown with an image, title and
## description.

onboarding-benefit-products-title = Des logiciels à votre service
onboarding-benefit-products-text = Améliorez votre productivité grâce à un ensemble d’outils qui respectent votre vie privée sur tous vos appareils.
onboarding-benefit-knowledge-title = Des connaissances utiles
onboarding-benefit-knowledge-text = Apprenez tout ce qu’il faut savoir pour naviguer intelligemment et en sécurité.
onboarding-benefit-privacy-title = Le respect de la vie privée, pour de vrai
# "Personal Data Promise" is a concept that should be translated consistently
# across the product. It refers to a concept shown elsewhere to the user: "The
# Firefox Personal Data Promise is the way we honor your data in everything we
# make and do. We take less data. We keep it safe. And we make sure that we are
# transparent about how we use it."
onboarding-benefit-privacy-text = Tout ce que nous faisons respecte notre « Garantie en matière de données personnelles » : collecter moins de données, les protéger, ne rien cacher.

## These strings belong to the individual onboarding messages.


## Each message has a title and a description of what the browser feature is.
## Each message also has an associated button for the user to try the feature.
## The string for the button is found above, in the UI strings section

onboarding-private-browsing-title = Navigation privée
onboarding-private-browsing-text = Naviguez à l’abri des regards. La navigation privée avec blocage de contenu arrête les traqueurs qui vous pistent partout sur le Web.
onboarding-screenshots-title = Captures d’écran
onboarding-screenshots-text = Prenez des captures d’écran, enregistrez-les et partagez-les sans quitter { -brand-short-name }. Capturez une zone de la page ou la page entière pendant que vous naviguez. Enregistrez ensuite votre capture en ligne pour y accéder facilement et la partager.
onboarding-addons-title = Modules complémentaires
onboarding-addons-text = Ajoutez encore plus de fonctionnalités pour que { -brand-short-name } en fasse davantage pour vous. Comparez les prix, consultez la météo ou affirmez-vous avec un thème personnalisé.
onboarding-ghostery-title = Ghostery
onboarding-ghostery-text = Naviguez plus vite, plus intelligemment ou avec plus de sécurité avec des extensions comme Ghostery qui vous permet de bloquer les publicités envahissantes.
# Note: "Sync" in this case is a generic verb, as in "to synchronize"
onboarding-fxa-title = Synchroniser
onboarding-fxa-text = Créez votre { -fxaccount-brand-name } et synchronisez vos marque-pages, vos mots de passe et onglets ouverts partout où vous utilisez { -brand-short-name }.
onboarding-tracking-protection-title2 = Protection contre le pistage
onboarding-tracking-protection-text2 = { -brand-short-name } empêche les sites web de vous pister pendant votre navigation, ce qui complique la tâche des publicités qui tentent de vous suivre sur le Web.
onboarding-tracking-protection-button2 = Principes de fonctionnement
onboarding-data-sync-title = Emportez vos paramètres avec vous
# "Sync" is short for synchronize.
onboarding-data-sync-text2 = Synchronisez marque-pages, mots de passe, etc., partout où vous utilisez { -brand-product-name }.
onboarding-data-sync-button2 = Se connecter à { -sync-brand-short-name }
onboarding-firefox-monitor-title = Suivez de près les fuites de données
onboarding-firefox-monitor-text = { -monitor-brand-name } vérifie si votre adresse électronique fait partie d’une fuite de données et vous alerte si elle apparaît dans une nouvelle fuite de données.
onboarding-firefox-monitor-button = S’abonner aux alertes
onboarding-browse-privately-title = Naviguez en toute confidentialité
onboarding-browse-privately-text = La navigation privée efface vos historiques de navigation et de recherches pour que les autres utilisateurs de votre ordinateur n’en sachent rien.
onboarding-browse-privately-button =
    Ouvrir une fenêtre de
    navigation privée
onboarding-firefox-send-title = Protégez les fichiers que vous partagez
onboarding-firefox-send-text2 = Envoyez vos fichiers sur { -send-brand-name } pour les partager avec un chiffrement de bout en bout et un lien qui expire automatiquement.
onboarding-firefox-send-button = Essayer { -send-brand-name }
onboarding-mobile-phone-title = Installez { -brand-product-name } sur votre téléphone
onboarding-mobile-phone-text = Téléchargez { -brand-product-name } pour iOS ou Android et synchronisez vos données entre vos appareils.
# "Mobile" is short for mobile/cellular phone, "Browser" is short for web
# browser.
onboarding-mobile-phone-button = Télécharger le navigateur mobile
onboarding-send-tabs-title = Envoyez-vous des onglets
# "Send Tabs" refers to "Send Tab to Device" feature that appears when opening a
# tab's context menu.
onboarding-send-tabs-text = L’envoi d’onglets partage instantanément des pages entre vos appareils sans avoir à copier, coller ou quitter le navigateur.
onboarding-send-tabs-button =
    Commencer à utiliser
    « Envoyer l’onglet »
onboarding-pocket-anywhere-title = Lisez et écoutez, où que vous soyez
onboarding-pocket-anywhere-text2 = Enregistrez vos contenus préférés hors connexion avec l’application { -pocket-brand-name } pour les lire, regarder ou écouter quand bon vous semble.
onboarding-pocket-anywhere-button = Essayer { -pocket-brand-name }
onboarding-lockwise-passwords-title = Emmenez vos mots de passe partout
onboarding-lockwise-passwords-text2 = Conservez en sécurité les mots de passe que vous enregistrez et connectez-vous en toute simplicité à vos comptes avec { -lockwise-brand-name }.
onboarding-lockwise-passwords-button2 = Obtenir l’application
onboarding-facebook-container-title = Fixez les limites avec Facebook
onboarding-facebook-container-text2 = { -facebook-container-brand-name } conserve votre profil séparé de tout le reste. Il est ainsi plus difficile pour Facebook de vous cibler avec des publicités.
onboarding-facebook-container-button = Installer l’extension

## Message strings belonging to the Return to AMO flow

return-to-amo-sub-header = Parfait, vous disposez de { -brand-short-name }
# <icon></icon> will be replaced with the icon belonging to the extension
#
# Variables:
#   $addon-name (String) - Name of the add-on
return-to-amo-addon-header = À présent, installons <icon></icon><b>{ $addon-name }.</b>
return-to-amo-extension-button = Ajouter l’extension
return-to-amo-get-started-button = Démarrer avec { -brand-short-name }
