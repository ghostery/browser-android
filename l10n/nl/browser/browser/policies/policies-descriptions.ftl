# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Firefox installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-AppUpdateURL = Aangepaste app-update-URL instellen.

policy-Authentication = Geïntegreerde authenticatie configureren voor websites die dit ondersteunen.

policy-BlockAboutAddons = Toegang tot de Add-onbeheerder (about:addons) blokkeren.

policy-BlockAboutConfig = Toegang tot de about:config-pagina blokkeren.

policy-BlockAboutProfiles = Toegang tot de about:profiles-pagina blokkeren.

policy-BlockAboutSupport = Toegang tot de about:support-pagina blokkeren.

policy-Bookmarks = Bladwijzers maken in de Bladwijzerwerkbalk, het menu Bladwijzers, of een hierbinnen opgegeven map.

policy-Certificates = Of ingebouwde certificaten wel of niet worden gebruikt. Deze beleidsregel geldt op dit moment alleen voor Windows.

policy-CertificatesDescription = Certificaten toevoegen of ingebouwde certificaten gebruiken.

policy-Cookies = Toestaan of weigeren dat websites cookies instellen.

policy-DisableAppUpdate = Voorkomen dat de browser wordt bijgewerkt.

policy-DisableBuiltinPDFViewer = PDF.js uitschakelen, de ingebouwde PDF-viewer in { -brand-short-name }.

policy-DisableDeveloperTools = Toegang tot de ontwikkelaarshulpmiddelen blokkeren.

policy-DisableFeedbackCommands = Opdrachten voor het verzenden van feedback vanuit het menu Help uitschakelen (Feedback verzenden en Misleidende website rapporteren).

policy-DisableFirefoxAccounts = Op { -fxaccount-brand-name } gebaseerde services uitschakelen, waaronder Sync.

# Firefox Screenshots is the name of the feature, and should not be translated.
policy-DisableFirefoxScreenshots = De Firefox Screenshots-functie uitschakelen.

policy-DisableFirefoxStudies = Voorkomen dat { -brand-short-name } onderzoeken uitvoert.

policy-DisableForgetButton = Toegang tot de knop Vergeten voorkomen.

policy-DisableFormHistory = Geen zoek- en formuliergeschiedenis onthouden.

policy-DisableMasterPasswordCreation = Wanneer true, kan geen hoofdwachtwoord worden aangemaakt.

policy-DisablePocket = De functie voor het opslaan van webpagina’s naar Pocket uitschakelen.

policy-DisablePrivateBrowsing = Privénavigatie uitschakelen.

policy-DisableProfileImport = De menuopdracht voor het importeren van gegevens vanuit een andere browser uitschakelen.

policy-DisableProfileRefresh = De knop { -brand-short-name } opfrissen in de about:support-pagina uitschakelen.

policy-DisableSafeMode = De functie voor het herstarten in Veilige modus uitschakelen. Noot: de Shift-toets voor het betreden van de Veilige modus kan in Windows alleen worden uitgeschakeld via Groepsbeleid.

policy-DisableSecurityBypass = Voorkomen dat de gebruiker bepaalde beveiligingsinstellingen omzeilt.

policy-DisableSetDesktopBackground = De menuopdracht Als bureaubladachtergrond instellen uitschakelen.

policy-DisableSetAsDesktopBackground = De menuopdracht Als bureaubladachtergrond instellen voor afbeeldingen uitschakelen.

policy-DisableSystemAddonUpdate = Voorkomen dat de browser systeemadd-ons installeert en bijwerkt.

policy-DisableTelemetry = Telemetry uitschakelen.

policy-DisplayBookmarksToolbar = Standaard de Bladwijzerwerkbalk weergeven.

policy-DisplayMenuBar = Standaard de Menubalk weergeven.

policy-DNSOverHTTPS = DNS over HTTPS configureren.

policy-DontCheckDefaultBrowser = Controle op standaardbrowser bij opstarten uitschakelen.

# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = Inhoudsblokkering inschakelen of uitschakelen en optioneel vergrendelen.

# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs.
policy-Extensions = Extensies installeren, verwijderen of vergrendelen. De optie voor installeren gebruikt URL’s of paden als parameters. De opties voor verwijderen en vergrendelen gebruiken extensie-ID’s.

policy-FlashPlugin = Gebruik van de Flash-plug-in toestaan of weigeren.

policy-HardwareAcceleration = Wanneer false, hardwareversnelling uitschakelen.

# “lock” means that the user won’t be able to change this setting
policy-Homepage = De startpagina instellen en optioneel vergrendelen.

policy-InstallAddonsPermission = Toestaan dat bepaalde websites add-ons installeren.

policy-NoDefaultBookmarks = Aanmaken van de standaardbladwijzers die met { -brand-short-name } worden meegeleverd uitschakelen, evenals de Slimme bladwijzers (Meest bezocht, Recente labels). Noot: deze beleidsregel is alleen van kracht bij gebruik ervan voordat het profiel voor het eerst wordt uitgevoerd.

policy-OfferToSaveLogins = De instelling voor het toestaan dat { -brand-short-name } mag aanbieden opgeslagen aanmeldingen en wachtwoorden te onthouden afdwingen. Zowel de waarde true als false wordt geaccepteerd.

policy-OverrideFirstRunPage = De pagina die voor het eerst wordt geopend vervangen. Stel deze beleidsregel in op leeg als u de betreffende pagina wilt uitschakelen.

policy-OverridePostUpdatePage = De pagina ‘Wat is er nieuw’ na een update vervangen. Stel deze beleidsregel in op leeg als u de betreffende pagina wilt uitschakelen.

policy-Permissions = Toestemmingen voor camera, microfoon, locatie en notificaties configureren.

policy-PopupBlocking = Toestaan dat bepaalde websites standaard pop-ups weergeven.

policy-Proxy = Proxyinstellingen configureren.

policy-RequestedLocales = De lijst van gevraagde locales voor de toepassing instellen, op volgorde van voorkeur.

policy-SanitizeOnShutdown = Alle navigatiegegevens wissen bij afsluiten.

policy-SearchBar = De standaardlocatie van de zoekbalk instellen. De gebruiker mag deze nog steeds aanpassen.

policy-SearchEngines = Instellingen voor zoekmachines configureren. Deze beleidsregel is alleen beschikbaar in de Extended Support Release (ESR)-versie.

# For more information, see https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/PKCS11/Module_Installation
policy-SecurityDevices = PKCS #11-modules installeren.

# “format” refers to the format used for the value of this policy.
policy-WebsiteFilter = Het bezoeken van websites blokkeren. Zie de documentatie voor meer informatie over de notatie.
