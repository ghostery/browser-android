# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Firefox installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-AppUpdateURL = Définir une URL de mise à jour personnalisée pour l’application.
policy-Authentication = Configurer l’authentification intégrée pour les sites web qui la proposent.
policy-BlockAboutAddons = Bloquer l’accès au gestionnaire de modules (about:addons).
policy-BlockAboutConfig = Bloquer l’accès à la page about:config.
policy-BlockAboutProfiles = Bloquer l’accès à la page about:profiles.
policy-BlockAboutSupport = Bloquer l’accès à la page about:support.
policy-Bookmarks = Créer des marque-pages dans la Barre personnelle, le menu des marque-pages ou un de leurs sous-dossiers.
policy-Certificates = Utiliser ou non des certificats intégrés. Cette politique est spécifique à Windows pour l’instant.
policy-Cookies = Permet ou interdit aux sites de définir des cookies.
policy-DisableAppUpdate = Empêche le navigateur de se mettre à jour.
policy-DisableBuiltinPDFViewer = Désactive PDF.js, la visionneuse PDF intégrée dans { -brand-short-name }.
policy-DisableDeveloperTools = Bloque l’accès aux outils de développement.
policy-DisableFeedbackCommands = Désactive les commandes permettant d’envoyer des commentaires dans le menu d’aide (Donner votre avis et Signaler un site trompeur).
policy-DisableFirefoxAccounts = Désactive les services basés sur { -fxaccount-brand-name }, y compris la synchronisation.
# Firefox Screenshots is the name of the feature, and should not be translated.
policy-DisableFirefoxScreenshots = Désactiver la fonctionnalité Firefox Screenshots.
policy-DisableFirefoxStudies = Empêche { -brand-short-name } d’exécuter des études.
policy-DisableForgetButton = Empêche l’accès au bouton d’effacement des données.
policy-DisableFormHistory = Ne pas conserver d’historique des recherches et des formulaires.
policy-DisableMasterPasswordCreation = Si activé, il sera impossible de créer un mot de passe principal.
policy-DisablePocket = Désactive la fonctionnalité d’enregistrement de pages web dans Pocket.
policy-DisablePrivateBrowsing = Désactive la navigation privée.
policy-DisableProfileImport = Désactive la commande de menu permettant d’importer des données à partir d’un autre navigateur.
policy-DisableProfileRefresh = Désactive le bouton Réparer { -brand-short-name } dans la page about:support.
policy-DisableSafeMode = Désactive le redémarrage en mode sans échec. Note : sous Windows, le passage en mode sans échec via la touche Majuscule ne peut être désactivé qu’à l’aide d’une stratégie de groupe.
policy-DisableSecurityBypass = Empêche l’utilisateur de contourner certains avertissements de sécurité.
policy-DisableSetDesktopBackground = Désactive l’option Choisir l’image comme fond d’écran.
policy-DisableSetAsDesktopBackground = Désactive la commande contextuelle Choisir l’image comme fond d’écran pour les images.
policy-DisableSystemAddonUpdate = Empêche le navigateur d’installer et de mettre à jour des modules complémentaires.
policy-DisableTelemetry = Désactive la télémétrie.
policy-DisplayBookmarksToolbar = Affiche la barre personnelle par défaut.
policy-DisplayMenuBar = Affiche la barre de menus par défaut.
policy-DontCheckDefaultBrowser = Désactive la vérification du navigateur par défaut au démarrage.
# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = Active ou désactive le blocage de contenu et permet de verrouiller ce choix.
# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#extensions-machine-only
policy-Extensions = Installe, désinstalle ou verrouille les extensions. L’option d’installation accepte des URL ou des chemins comme paramètres. Les options Désinstaller et Verrouiller utilisent les identifiants des extensions.
policy-FlashPlugin = Autorise ou non l’utilisation du plugin Flash.
policy-HardwareAcceleration = Si faux, désactive l’accélération matérielle.
# “lock” means that the user won’t be able to change this setting
policy-Homepage = Définit la page d’accueil et propose de la verrouiller.
policy-InstallAddonsPermission = Autorise certains sites web à installer des extensions.
policy-NoDefaultBookmarks = Désactive la création des marque-pages par défaut fournis avec { -brand-short-name } ainsi que les marque-pages intelligents (sites les plus visités, sites récents). Note : cette politique ne fonctionne que si elle est activée avant le premier lancement du profil.
policy-OfferToSaveLogins = Applique les paramètres qui permettent à { -brand-short-name } de se souvenir des identifiants et des mots de passe enregistrés. Les valeurs true et false sont toutes deux acceptées.
policy-OverrideFirstRunPage = Contrôle la page de premier démarrage. Laissez cette règle vide pour désactiver la page de premier démarrage.
policy-OverridePostUpdatePage = Contrôle la page « Nouveautés » après une mise à jour. Laissez cette règle vide pour désactiver la page après mise à jour.
policy-Permissions = Configure les permissions pour la caméra, le microphone, la localisation et les notifications.
policy-PopupBlocking = Autorise certains sites web à afficher des popups par défaut.
policy-Proxy = Configure les paramètres du proxy.
policy-SanitizeOnShutdown = Supprime toutes les données de navigation à la fermeture.
policy-SearchBar = Définit l’emplacement par défaut de la barre de recherche. L’utilisateur garde la possibilité de la personnaliser.
policy-SearchEngines = Configure les paramètres du moteur de recherche. Cette propriété n’est disponible que sur la version Extended Support Release (ESR).
# “format” refers to the format used for the value of this policy. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#websitefilter-machine-only
policy-WebsiteFilter = Bloque l’accès à des sites web. Voir la documentation pour plus de détails sur le format.
