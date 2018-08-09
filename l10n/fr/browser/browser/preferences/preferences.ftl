# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = Envoyer aux sites web un signal « Ne pas me pister » indiquant que vous ne souhaitez pas être pisté
do-not-track-learn-more = En savoir plus
do-not-track-option-default =
    .label = Seulement lorsque la protection contre le pistage est utilisée
do-not-track-option-always =
    .label = Toujours
pref-page =
    .title =
        { PLATFORM() ->
            [windows] Options
           *[other] Préférences
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input =
    .style = width: 19em
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Please keep the placeholder string short to avoid truncation.
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input-box =
    .style = width: 19em
    .placeholder =
        { PLATFORM() ->
            [windows] Rechercher dans les options
           *[other] Rechercher dans les préférences
        }
policies-notice =
    { PLATFORM() ->
        [windows] Votre organisation a désactivé la possibilité de changer certaines options.
       *[other] Votre organisation a désactivé la possibilité de changer certaines préférences.
    }
pane-general-title = Général
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = Accueil
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = Recherche
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = Vie privée et sécurité
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Compte Firefox
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = Assistance de { -brand-short-name }
focus-search =
    .key = f
close-button =
    .aria-label = Fermer

## Browser Restart Dialog

feature-enable-requires-restart = { -brand-short-name } doit redémarrer pour activer cette fonctionnalité.
feature-disable-requires-restart = { -brand-short-name } doit redémarrer pour désactiver cette fonctionnalité.
should-restart-title = Redémarrer { -brand-short-name }
should-restart-ok = Redémarrer { -brand-short-name } maintenant
cancel-no-restart-button = Annuler
restart-later = Redémarrer plus tard

## Extension Control Notifications
##
## These strings are used to inform the user
## about changes made by extensions to browser settings.
##
## <img data-l10n-name="icon"/> is going to be replaced by the extension icon.
##
## Variables:
##   $name (String): name of the extension

# This string is shown to notify the user that their home page
# is being controlled by an extension.
extension-controlled-homepage-override = Une extension, <img data-l10n-name="icon"/> { $name }, contrôle votre page d’accueil.
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = Une extension, <img data-l10n-name="icon"/> { $name }, contrôle la page Nouvel onglet.
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = Une extension, <img data-l10n-name="icon"/> { $name }, a défini votre moteur de recherche par défaut.
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = Une extension, <img data-l10n-name="icon"/> { $name }, a besoin des onglets contextuels.
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = Une extension, <img data-l10n-name="icon"/> { $name }, contrôle la protection contre le pistage.
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = Une extension, <img data-l10n-name="icon"/> { $name }, contrôle la façon dont { -brand-short-name } se connecte à Internet.
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = Pour activer l’extension, sélectionnez <img data-l10n-name="addons-icon"/> Modules complémentaires dans le menu <img data-l10n-name="menu-icon"/>.

## Preferences UI Search Results

search-results-header = Résultats de la recherche
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] Désolé, il n’y a aucun résultat dans les options pour « <span data-l10n-name="query"></span> ».
       *[other] Désolé, il n’y a aucun résultat dans les préférences pour « <span data-l10n-name="query"></span> ».
    }
search-results-help-link = Besoin d’aide ? Consultez <a data-l10n-name="url">l’assistance de { -brand-short-name }</a>

## General Section

startup-header = Démarrage
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = Autoriser { -brand-short-name } et Firefox à s’exécuter en parallèle
use-firefox-sync = Astuce : Des profils distincts sont utilisés. Vous pouvez tirer parti de { -sync-brand-short-name } pour partager vos données entre eux.
get-started-not-logged-in = Se connecter à { -sync-brand-short-name }…
get-started-configured = Ouvrir les préférences de { -sync-brand-short-name }
always-check-default =
    .label = Toujours vérifier que { -brand-short-name } est votre navigateur par défaut
    .accesskey = v
is-default = { -brand-short-name } est votre navigateur par défaut
is-not-default = { -brand-short-name } n’est pas votre navigateur par défaut
set-as-my-default-browser =
    .label = Définir par défaut…
    .accesskey = D
startup-page = Au démarrage de { -brand-short-name }
    .accesskey = d
startup-user-homepage =
    .label = Afficher votre page d’accueil
startup-blank-page =
    .label = Afficher une page vide
startup-prev-session =
    .label = Afficher les derniers onglets et fenêtres utilisés
startup-restore-previous-session =
    .label = Restaurer la session précédente
    .accesskey = e
disable-extension =
    .label = Désactiver l’extension
home-page-header = Page d’accueil
tabs-group-header = Onglets
ctrl-tab-recently-used-order =
    .label = Ctrl+Tab fait défiler vos onglets en les classant selon leur dernière utilisation
    .accesskey = T
open-new-link-as-tabs =
    .label = Ouvrir les liens dans des onglets au lieu de nouvelles fenêtres
    .accesskey = f
warn-on-close-multiple-tabs =
    .label = Avertir lors de la fermeture de plusieurs onglets
    .accesskey = A
warn-on-open-many-tabs =
    .label = Prévenir lors de l’ouverture de multiples onglets d’un ralentissement possible de { -brand-short-name }
    .accesskey = P
switch-links-to-new-tabs =
    .label = Lors de l’ouverture d’un lien dans un nouvel onglet, basculer vers celui-ci immédiatement
    .accesskey = L
show-tabs-in-taskbar =
    .label = Afficher les aperçus d’onglets dans la barre des tâches de Windows
    .accesskey = c
browser-containers-enabled =
    .label = Activer les onglets contextuels
    .accesskey = g
browser-containers-learn-more = En savoir plus
browser-containers-settings =
    .label = Paramètres…
    .accesskey = s
containers-disable-alert-title = Fermer tous les onglets contextuels ?
containers-disable-alert-desc =
    { $tabCount ->
        [one] Si vous désactivez les onglets contextuels maintenant, { $tabCount } onglet contextuel sera fermé. Voulez-vous vraiment désactiver les onglets contextuels ?
       *[other] Si vous désactivez les onglets contextuels maintenant, { $tabCount } onglets contextuels seront fermés. Voulez-vous vraiment désactiver les onglets contextuels ?
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] Fermer { $tabCount } onglet contextuel
       *[other] Fermer { $tabCount } onglets contextuels
    }
containers-disable-alert-cancel-button = Garder activé
containers-remove-alert-title = Supprimer ce contexte ?
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg =
    { $count ->
        [one] Si vous supprimez ce contexte maintenant, { $count } onglet contextuel sera fermé. Voulez-vous vraiment supprimer ce contexte ?
       *[other] Si vous supprimez ce contexte maintenant, { $count } onglets contextuels seront fermés. Voulez-vous vraiment supprimer ce contexte ?
    }
containers-remove-ok-button = Supprimer ce contexte
containers-remove-cancel-button = Ne pas supprimer ce contexte

## General Section - Language & Appearance

language-and-appearance-header = Langue et apparence
fonts-and-colors-header = Polices et couleurs
default-font = Police par défaut
    .accesskey = P
default-font-size = Taille
    .accesskey = T
advanced-fonts =
    .label = Avancé…
    .accesskey = v
colors-settings =
    .label = Couleurs…
    .accesskey = C
language-header = Langue
choose-language-description = Choix de la langue préférée pour l’affichage des pages
choose-button =
    .label = Choisir…
    .accesskey = o
translate-web-pages =
    .label = Traduire le contenu web
    .accesskey = w
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = Traductions fournies par <img data-l10n-name="logo"/>
translate-exceptions =
    .label = Exceptions…
    .accesskey = x
check-user-spelling =
    .label = Vérifier l’orthographe pendant la saisie
    .accesskey = V

## General Section - Files and Applications

files-and-applications-title = Fichiers et applications
download-header = Téléchargements
download-save-to =
    .label = Enregistrer les fichiers dans le dossier
    .accesskey = n
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] Choisir…
           *[other] Parcourir…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] C
           *[other] P
        }
download-always-ask-where =
    .label = Toujours demander où enregistrer les fichiers
    .accesskey = T
applications-header = Applications
applications-description = Définissez le comportement de { -brand-short-name } avec les fichiers que vous téléchargez et les applications que vous utilisez lorsque vous naviguez.
applications-filter =
    .placeholder = Rechercher des types de fichiers ou d’applications
applications-type-column =
    .label = Type de contenu
    .accesskey = T
applications-action-column =
    .label = Action
    .accesskey = A
drm-content-header = Contenu protégé par des composants de gestion des droits numériques (DRM)
play-drm-content =
    .label = Lire le contenu protégé par des DRM
    .accesskey = L
play-drm-content-learn-more = En savoir plus
update-application-title = Mises à jour de { -brand-short-name }
update-application-description = Conservez { -brand-short-name } à jour pour bénéficier des dernières avancées en matière de performances, de stabilité et de sécurité.
update-application-info = Version { $version } <a>Notes de version</a>
update-application-version = Version { $version } <a data-l10n-name="learn-more">Notes de version</a>
update-history =
    .label = Afficher l’historique des mises à jour…
    .accesskey = h
update-application-allow-description = Autoriser { -brand-short-name } à
update-application-auto =
    .label = Installer les mises à jour automatiquement (recommandé)
    .accesskey = I
update-application-check-choose =
    .label = Vérifier l’existence de mises à jour, mais vous laisser décider de leur installation
    .accesskey = C
update-application-manual =
    .label = Ne jamais vérifier les mises à jour (déconseillé)
    .accesskey = N
update-application-use-service =
    .label = Utiliser un service en arrière-plan pour installer les mises à jour
    .accesskey = s
update-enable-search-update =
    .label = Mettre à jour automatiquement les moteurs de recherche
    .accesskey = r

## General Section - Performance

performance-title = Performances
performance-use-recommended-settings-checkbox =
    .label = Utiliser les paramètres de performance recommandés
    .accesskey = U
performance-use-recommended-settings-desc = Ces paramètres sont adaptés à la configuration matérielle de votre ordinateur et à votre système d’exploitation.
performance-settings-learn-more = En savoir plus
performance-allow-hw-accel =
    .label = Utiliser l’accélération graphique matérielle si disponible
    .accesskey = n
performance-limit-content-process-option = Nombre maximum de processus de contenu
    .accesskey = N
performance-limit-content-process-enabled-desc = Davantage de processus de contenu peut améliorer les performances lors de l’utilisation de plusieurs onglets, cependant la consommation de mémoire sera plus importante.
performance-limit-content-process-disabled-desc = Modifier le nombre de processus de contenu est possible uniquement avec la version multiprocessus de { -brand-short-name }. <a>Apprendre à vérifier si le mode multiprocessus est activé</a>
performance-limit-content-process-blocked-desc = Modifier le nombre de processus de contenu est possible uniquement avec la version multiprocessus de { -brand-short-name }. <a data-l10n-name="learn-more">Apprendre à vérifier si le mode multiprocessus est activé</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (par défaut)

## General Section - Browsing

browsing-title = Navigation
browsing-use-autoscroll =
    .label = Utiliser le défilement automatique
    .accesskey = U
browsing-use-smooth-scrolling =
    .label = Utiliser le défilement doux
    .accesskey = s
browsing-use-onscreen-keyboard =
    .label = Afficher un clavier virtuel lorsque nécessaire
    .accesskey = A
browsing-use-cursor-navigation =
    .label = Toujours utiliser les touches de navigation pour se déplacer à l’intérieur d’une page
    .accesskey = T
browsing-search-on-start-typing =
    .label = Lancer la recherche lors de la saisie de texte
    .accesskey = c

## General Section - Proxy

network-proxy-title = Proxy réseau
network-proxy-connection-description = Configurer la façon dont { -brand-short-name } se connecte à Internet.
network-proxy-connection-learn-more = En savoir plus
network-proxy-connection-settings =
    .label = Paramètres…
    .accesskey = P

## Home Section

home-new-windows-tabs-header = Nouvelles fenêtres et nouveaux onglets
home-new-windows-tabs-description2 = Choisissez ce qui est affiché lorsque vous ouvrez votre page d’accueil, de nouvelles fenêtres ou de nouveaux onglets.

## Home Section - Home Page Customization

home-homepage-mode-label = Page d’accueil et nouvelles fenêtres
home-newtabs-mode-label = Nouveaux onglets
home-restore-defaults =
    .label = Configuration par défaut
    .accesskey = C
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Page d’accueil de Firefox (par défaut)
home-mode-choice-custom =
    .label = Adresses web personnalisées…
home-mode-choice-blank =
    .label = Page vide
home-homepage-custom-url =
    .placeholder = Coller une adresse…
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] Page courante
           *[other] Pages courantes
        }
    .accesskey = U
choose-bookmark =
    .label = Marque-page…
    .accesskey = m
restore-default =
    .label = Restaurer la configuration par défaut
    .accesskey = R

## Search Section

search-bar-header = Barre de recherche
search-bar-hidden =
    .label = Utiliser la barre d’adresse pour naviguer et effectuer des recherches
search-bar-shown =
    .label = Ajouter la barre de recherche à la barre d’outils
search-engine-default-header = Moteur de recherche par défaut
search-engine-default-desc = Sélectionnez le moteur de recherche à utiliser par défaut dans la barre d’adresse et la barre de recherche.
search-suggestions-option =
    .label = Afficher les suggestions de recherche
    .accesskey = A
search-show-suggestions-url-bar-option =
    .label = Afficher les suggestions de recherche parmi les résultats de la barre d’adresse
    .accesskey = c
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = Afficher les suggestions de recherche avant l’historique de navigation dans les résultats de la barre d’adresse
search-suggestions-cant-show = Les suggestions de recherche ne seront pas affichées parmi les résultats de la barre d’adresse car vous avez configuré { -brand-short-name } de façon à ce qu’il ne conserve jamais l’historique.
search-one-click-header = Moteurs de recherche accessibles en un clic
search-one-click-desc = Sélectionnez les moteurs de recherche alternatifs qui apparaissent sous la barre d’adresse et la barre de recherche lorsque vous commencez à saisir un mot-clé.
search-choose-engine-column =
    .label = Moteur de recherche
search-choose-keyword-column =
    .label = Mot-clé
search-restore-default =
    .label = Restaurer les moteurs de recherche par défaut
    .accesskey = R
search-remove-engine =
    .label = Supprimer
    .accesskey = S
search-find-more-link = Découvrir d’autres moteurs de recherche
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = Doublon de mot-clé
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = Vous avez choisi un mot-clé qui est déjà utilisé par « { $name } ». Veuillez en choisir un autre.
search-keyword-warning-bookmark = Vous avez choisi un mot-clé qui est déjà utilisé par un marque-page. Veuillez en choisir un autre.

## Containers Section

containers-back-link = « Retour
containers-header = Onglets contextuels
containers-add-button =
    .label = Ajouter un nouveau contexte
    .accesskey = A
containers-preferences-button =
    .label = Préférences
containers-remove-button =
    .label = Supprimer

## Sync Section - Signed out

sync-signedout-caption = Emportez votre Web partout avec vous
sync-signedout-description = Synchronisez marque-pages, historique, onglets, mots de passe, modules complémentaires et préférences entre tous vos appareils.
sync-signedout-account-title = Se connecter avec un { -fxaccount-brand-name[lowercase] }
sync-signedout-account-create = Vous n’avez pas de compte ? Inscrivez-vous
    .accesskey = V
sync-signedout-account-signin =
    .label = Se connecter…
    .accesskey = S
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Téléchargez Firefox pour <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> ou <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a> afin de synchroniser vos données avec votre appareil mobile.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = Changer la photo de profil
sync-disconnect =
    .label = Se déconnecter…
    .accesskey = d
sync-manage-account = Gérer le compte
    .accesskey = G
sync-signedin-unverified = { $email } n’est pas vérifié.
sync-signedin-login-failure = Veuillez vous identifier pour vous reconnecter via { $email }
sync-resend-verification =
    .label = Renvoyer la vérification
    .accesskey = f
sync-remove-account =
    .label = Supprimer le compte
    .accesskey = t
sync-sign-in =
    .label = Connexion
    .accesskey = x
sync-signedin-settings-header = Paramètres de synchronisation
sync-signedin-settings-desc = Sélectionnez les données à synchroniser entre vos appareils utilisant { -brand-short-name }.
sync-engine-bookmarks =
    .label = Marque-pages
    .accesskey = M
sync-engine-history =
    .label = Historique
    .accesskey = r
sync-engine-tabs =
    .label = Onglets ouverts
    .tooltiptext = Une liste de ce qui est actuellement ouvert sur tous vos appareils synchronisés
    .accesskey = O
sync-engine-logins =
    .label = Identifiants
    .tooltiptext = Les noms d’utilisateurs et les mots de passe que vous avez enregistrés
    .accesskey = I
sync-engine-addresses =
    .label = Adresses
    .tooltiptext = Les adresses postales que vous avez enregistrées (uniquement sur ordinateur)
    .accesskey = A
sync-engine-creditcards =
    .label = Cartes bancaires
    .tooltiptext = Noms, numéros et dates d’expiration (uniquement sur ordinateur)
    .accesskey = C
sync-engine-addons =
    .label = Modules complémentaires
    .tooltiptext = Extensions et thèmes pour Firefox sur ordinateur
    .accesskey = u
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] Options
           *[other] Préférences
        }
    .tooltiptext = Les paramètres que vous avez modifiés dans les sections Général, Vie privée et Sécurité
    .accesskey = s
sync-device-name-header = Nom de l’appareil
sync-device-name-change =
    .label = Changer le nom de l’appareil…
    .accesskey = h
sync-device-name-cancel =
    .label = Annuler
    .accesskey = A
sync-device-name-save =
    .label = Enregistrer
    .accesskey = E
sync-mobilepromo-single = Connecter un autre appareil
sync-mobilepromo-multi = Gérer les appareils
sync-tos-link = Conditions d’utilisation
sync-fxa-privacy-notice = Politique de confidentialité

## Privacy Section

privacy-header = Vie privée

## Privacy Section - Forms

forms-header = Formulaires et mots de passe
forms-ask-to-save-logins =
    .label = Proposer d’enregistrer les identifiants et les mots de passe pour les sites web
    .accesskey = r
forms-exceptions =
    .label = Exceptions…
    .accesskey = x
forms-saved-logins =
    .label = Identifiants enregistrés…
    .accesskey = I
forms-master-pw-use =
    .label = Utiliser un mot de passe principal
    .accesskey = U
forms-master-pw-change =
    .label = Changer le mot de passe principal…
    .accesskey = C

## Privacy Section - History

history-header = Historique
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = Règles de conservation
    .accesskey = R
history-remember-option-all =
    .label = Conserver l’historique
history-remember-option-never =
    .label = Ne jamais conserver l’historique
history-remember-option-custom =
    .label = Utiliser les paramètres personnalisés pour l’historique
history-remember-description = { -brand-short-name } conservera les données de navigation, les téléchargements, les formulaires et l’historique de recherche.
history-dontremember-description = { -brand-short-name } utilisera les mêmes paramètres que pour la navigation privée et ne conservera aucun historique de votre navigation.
history-private-browsing-permanent =
    .label = Toujours utiliser le mode de navigation privée
    .accesskey = i
history-remember-option =
    .label = Conserver l’historique de navigation et des téléchargements
    .accesskey = C
history-remember-browser-option =
    .label = Conserver l’historique de navigation et des téléchargements
    .accesskey = C
history-remember-search-option =
    .label = Conserver l’historique des recherches et des formulaires
    .accesskey = n
history-clear-on-close-option =
    .label = Vider l’historique lors de la fermeture de { -brand-short-name }
    .accesskey = V
history-clear-on-close-settings =
    .label = Paramètres…
    .accesskey = P
history-clear-button =
    .label = Effacer l’historique…
    .accesskey = h

## Privacy Section - Site Data

sitedata-header = Cookies et données de sites
sitedata-total-size-calculating = Calcul du volume des données de sites et du cache…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = Le stockage des cookies, du cache et des données de sites utilise actuellement { $value } { $unit } d’espace disque.
sitedata-learn-more = En savoir plus
sitedata-accept-cookies-option =
    .label = Accepter les cookies et les données de sites (recommandé)
    .accesskey = A
sitedata-block-cookies-option =
    .label = Bloquer les cookies et les données de sites (peut empêcher certains sites de fonctionner)
    .accesskey = B
sitedata-keep-until = Les conserver jusqu’à
    .accesskey = L
sitedata-keep-until-expire =
    .label = Leur expiration
sitedata-keep-until-closed =
    .label = La fermeture de { -brand-short-name }
sitedata-accept-third-party-desc = Accepter les cookies tiers et les données de sites tiers
    .accesskey = k
sitedata-accept-third-party-always-option =
    .label = Toujours
sitedata-accept-third-party-visited-option =
    .label = Depuis les sites visités
sitedata-accept-third-party-never-option =
    .label = Jamais
sitedata-clear =
    .label = Effacer les données…
    .accesskey = E
sitedata-settings =
    .label = Gérer les données…
    .accesskey = G
sitedata-cookies-exceptions =
    .label = Exceptions…
    .accesskey = x

## Privacy Section - Address Bar

addressbar-header = Barre d’adresse
addressbar-suggest = Lors de l’utilisation de la barre d’adresse, afficher des suggestions depuis
addressbar-locbar-history-option =
    .label = L’historique de navigation
    .accesskey = H
addressbar-locbar-bookmarks-option =
    .label = Les marque-pages
    .accesskey = M
addressbar-locbar-openpage-option =
    .label = Les onglets ouverts
    .accesskey = O
addressbar-suggestions-settings = Modifier les préférences pour les suggestions de recherche

## Privacy Section - Tracking

tracking-header = Protection contre le pistage
tracking-desc = La protection contre le pistage bloque les traqueurs en ligne qui collectent vos données de navigation depuis plusieurs sites web. <a data-l10n-name="learn-more">En savoir plus sur la protection contre le pistage et sur la protection de votre vie privée</a>
tracking-mode-label = Utiliser la protection contre le pistage pour bloquer les traqueurs connus
tracking-mode-always =
    .label = Toujours
    .accesskey = T
tracking-mode-private =
    .label = Uniquement dans les fenêtres privées
    .accesskey = U
tracking-mode-never =
    .label = Jamais
    .accesskey = J
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = Utiliser la protection contre le pistage en navigation privée pour bloquer les traqueurs connus
    .accesskey = i
tracking-exceptions =
    .label = Exceptions…
    .accesskey = x
tracking-change-block-list =
    .label = Modifier les listes de blocage…
    .accesskey = b

## Privacy Section - Permissions

permissions-header = Permissions
permissions-location = Localisation
permissions-location-settings =
    .label = Paramètres…
    .accesskey = a
permissions-camera = Caméra
permissions-camera-settings =
    .label = Paramètres…
    .accesskey = a
permissions-microphone = Microphone
permissions-microphone-settings =
    .label = Paramètres…
    .accesskey = a
permissions-notification = Notifications
permissions-notification-settings =
    .label = Paramètres…
    .accesskey = a
permissions-notification-link = En savoir plus
permissions-notification-pause =
    .label = Arrêter les notifications jusqu’au redémarrage de { -brand-short-name }
    .accesskey = n
permissions-block-popups =
    .label = Bloquer les fenêtres popup
    .accesskey = B
permissions-block-popups-exceptions =
    .label = Exceptions…
    .accesskey = E
permissions-addon-install-warning =
    .label = Prévenir lorsque les sites essaient d’installer des modules complémentaires
    .accesskey = P
permissions-addon-exceptions =
    .label = Exceptions…
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = Empêcher les services d’accessibilité d’avoir accès à votre navigateur
    .accesskey = a
permissions-a11y-privacy-link = En savoir plus

## Privacy Section - Data Collection

collection-header = Collecte de données par { -brand-short-name } et utilisation
collection-description = Nous nous efforçons de vous laisser le choix et de recueillir uniquement les informations dont nous avons besoin pour proposer { -brand-short-name } et l’améliorer pour tout le monde. Nous demandons toujours votre permission avant de recevoir des données personnelles.
collection-privacy-notice = Politique de confidentialité
collection-health-report =
    .label = Autoriser { -brand-short-name } à envoyer des données techniques et des données d’interaction à { -vendor-short-name }
    .accesskey = A
collection-health-report-link = En savoir plus
collection-studies =
    .label = Autoriser { -brand-short-name } à installer et à lancer des études
collection-studies-link = Consulter les études de { -brand-short-name }
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = L’envoi de données est désactivé pour cette configuration de compilation
collection-browser-errors =
    .label = Autoriser { -brand-short-name } à envoyer les rapports d’erreur du navigateur (comprenant les messages d’erreur) à { -vendor-short-name }
    .accesskey = u
collection-browser-errors-link = En savoir plus
collection-backlogged-crash-reports =
    .label = Autoriser { -brand-short-name } à envoyer pour vous les rapports de plantage en attente
    .accesskey = t
collection-backlogged-crash-reports-link = En savoir plus

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = Sécurité
security-browsing-protection = Protection contre les contenus trompeurs et les logiciels dangereux
security-enable-safe-browsing =
    .label = Bloquer les contenus dangereux ou trompeurs
    .accesskey = B
security-enable-safe-browsing-link = En savoir plus
security-block-downloads =
    .label = Bloquer les téléchargements dangereux
    .accesskey = D
security-block-uncommon-software =
    .label = Signaler la présence de logiciels indésirables ou peu communs
    .accesskey = n

## Privacy Section - Certificates

certs-header = Certificats
certs-personal-label = Lorsqu’un serveur demande votre certificat personnel
certs-select-auto-option =
    .label = En sélectionner un automatiquement
    .accesskey = E
certs-select-ask-option =
    .label = Vous demander à chaque fois
    .accesskey = V
certs-enable-ocsp =
    .label = Interroger le répondeur OCSP pour confirmer la validité de vos certificats
    .accesskey = I
certs-view =
    .label = Afficher les certificats…
    .accesskey = A
certs-devices =
    .label = Périphériques de sécurité…
    .accesskey = P
