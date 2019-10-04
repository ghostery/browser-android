# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Thunderbird installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-3rdparty = Imposta criteri a cui le estensioni basate su tecnologia WebExtension possono accedere via chrome.storage.managed.
policy-AppUpdateURL = Imposta URL personalizzato per aggiornamento applicazione.
policy-Authentication = Configura autenticazione integrata per i siti in cui è supportata.
policy-BlockAboutAddons = Blocca accesso al gestore componenti aggiuntivi (about:addons).
policy-BlockAboutConfig = Blocca accesso alla pagina about:config.
policy-BlockAboutProfiles = Blocca accesso alla pagina about:profiles.
policy-BlockAboutSupport = Blocca accesso alla pagina about:support.
policy-CaptivePortal = Attiva o disattiva supporto per captive portal.
policy-CertificatesDescription = Aggiungi certificati o utilizza i certificati predefiniti (built-in).
policy-Cookies = Consenti o nega ai siti web di impostare cookie.
policy-DefaultDownloadDirectory = Imposta la cartella predefinita per i download.
policy-DisableAppUpdate = Blocca l’aggiornamento di { -brand-short-name }.
policy-DisableDeveloperTools = Blocca accesso agli strumenti di sviluppo.
policy-DisableFeedbackCommands = Disattiva i comandi per inviare feedback dal menu Aiuto (“Invia feedback…” e “Segnala un sito ingannevole…”).
policy-DisableForgetButton = Impedisci accesso al pulsante “Dimentica”.
policy-DisableMasterPasswordCreation = Se impostato a “true” non è possibile impostare una password principale.
policy-DisableProfileImport = Disattiva il menu per importare dati da un’altra applicazione.
policy-DisableSafeMode = Disattiva la possibilità di riavviare in modalità provvisoria. Nota: l’utilizzo del pulsante Maiusc per avviare in modalità provvisoria può essere disattivato solo nei criteri di gruppo.
policy-DisableSecurityBypass = Impedisci all’utente di ignorare alcuni avvisi di sicurezza.
policy-DisableSystemAddonUpdate = Impedisci a { -brand-short-name } di installare e aggiornare componenti aggiuntivi di sistema.
policy-DisableTelemetry = Disattiva telemetria.
policy-DisplayMenuBar = Visualizza la barra dei menu per impostazione predefinita.
policy-DNSOverHTTPS = Configura DNS over HTTPS.
policy-DontCheckDefaultClient = Disattiva il controllo programma di posta predefinito all’avvio.
policy-DownloadDirectory = Imposta la cartella per i download e impedisci ulteriori modifiche.
# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = Attiva o disattiva il blocco contenuti ed eventualmente impedisci modifiche all’opzione.
# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs.
policy-Extensions = Installa, disinstalla o blocca estensioni. L’opzione per installare richiede URL o percorsi come parametri. L’opzione per disinstallare o bloccare richiede gli ID delle estensioni.
policy-ExtensionSettings = Gestisci tutti gli aspetti dell’installazione delle estensioni.
policy-ExtensionUpdate = Attiva o disattiva l’aggiornamento automatico delle estensioni.
policy-HardwareAcceleration = Se “false”, disattiva l’accelerazione hardware.
policy-InstallAddonsPermission = Consenti a determinati siti web di installare componenti aggiuntivi.
policy-LocalFileLinks = Consenti a determinati siti web di usare link a file locali.
policy-NetworkPrediction = Attiva o disattiva “network prediction” (prelettura DNS).
policy-OfferToSaveLogins = Gestisci la richiesta in { -brand-short-name } di salvare credenziali di accesso. Entrambi i valori “true” e “false” sono validi.
policy-OverrideFirstRunPage = Sostituisci la pagina visualizzata alla prima esecuzione. Impostare questo criterio con un valore vuoto per disattivarne la visualizzazione.
policy-OverridePostUpdatePage = Sostituisci la pagina “Novità” visualizzata dopo un aggiornamento. Impostare questo criterio con un valore vuoto per disattivarne la visualizzazione.
policy-Preferences = Imposta un gruppo di preferenze e bloccane il valore.
policy-PromptForDownloadLocation = Chiedi dove salvare i file scaricati.
policy-Proxy = Configura le impostazioni dei proxy.
policy-RequestedLocales = Configura, in ordine di preferenza, l’elenco delle lingue (“locale”) richieste per l’applicazione.
policy-SanitizeOnShutdown2 = Elimina dati di navigazione alla chiusura.
policy-SearchEngines = Configura le impostazioni relative ai motori di ricerca. Questo criterio è disponibile solo nella versione Extended Support Release (ESR).
# For more information, see https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/PKCS11/Module_Installation
policy-SecurityDevices = Installa moduli PKCS #11.
policy-SSLVersionMax = Imposta la versione massima di SSL.
policy-SSLVersionMin = Imposta la versione minima di SSL.
policy-SupportMenu = Aggiungi una voce di menu personalizzata nel menu Aiuto.
# “format” refers to the format used for the value of this policy.
policy-WebsiteFilter = Impedisci l’accesso a determinati siti web. Consulta la documentazione per ulteriori dettagli sul formato da utilizzare.
