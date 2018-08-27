# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = Invia ai siti web un segnale “Do Not Track” per chiedere di non effettuare alcun tracciamento
do-not-track-learn-more = Ulteriori informazioni
do-not-track-option-default =
    .label = Solo quando è attiva la protezione antitracciamento
do-not-track-option-always =
    .label = Sempre
pref-page =
    .title =
        { PLATFORM() ->
            [windows] Opzioni
           *[other] Preferenze
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Please keep the placeholder string short to avoid truncation.
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input-box =
    .style = width: 16em
    .placeholder =
        { PLATFORM() ->
            [windows] Cerca nelle opzioni
           *[other] Cerca nelle preferenze
        }
policies-notice =
    { PLATFORM() ->
        [windows] La possibilità di modificare alcune opzioni è stata disattivata dalla propria azienda.
       *[other] La possibilità di modificare alcune preferenze è stata disattivata dalla propria azienda.
    }
pane-general-title = Generale
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = Pagina iniziale
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = Ricerca
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = Privacy e sicurezza
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Account Firefox
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = Supporto a { -brand-short-name }
focus-search =
    .key = f
close-button =
    .aria-label = Chiudi

## Browser Restart Dialog

feature-enable-requires-restart = È necessario riavviare { -brand-short-name } per attivare questa funzione.
feature-disable-requires-restart = È necessario riavviare { -brand-short-name } per disattivare questa funzione.
should-restart-title = Riavvia { -brand-short-name }
should-restart-ok = Riavvia { -brand-short-name } adesso
cancel-no-restart-button = Annulla
restart-later = Riavvia in seguito

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
extension-controlled-homepage-override = La pagina iniziale è attualmente gestita da un’estensione (<img data-l10n-name="icon"/> { $name }).
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = La pagina Nuova scheda è attualmente gestita da un’estensione (<img data-l10n-name="icon"/> { $name }).
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = Il motore di ricerca predefinito è attualmente impostato da un’estensione (<img data-l10n-name="icon"/> { $name }).
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = Le schede contenitore sono necessarie per il funzionamento di un’estensione (<img data-l10n-name="icon"/> { $name }).
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = La protezione antitracciamento è attualmente gestita da un’estensione (<img data-l10n-name="icon"/> { $name }).
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = Le impostazioni di <img data-l10n-name="icon"/> { $name } relative alla connessione a Internet sono attualmente gestite da un’estensione ({ -brand-short-name }).
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = Per attivare questa estensione aprire Componenti aggiuntivi <img data-l10n-name="addons-icon"/> nel menu <img data-l10n-name="menu-icon"/>.

## Preferences UI Search Results

search-results-header = Risultati della ricerca
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] Siamo spiacenti, nessun risultato trovato per “<span data-l10n-name="query"></span>” nelle opzioni.
       *[other] Siamo spiacenti, nessun risultato trovato per “<span data-l10n-name="query"></span>” nelle preferenze.
    }
search-results-help-link = Hai bisogno di aiuto? Visita <a data-l10n-name="url">il sito web di supporto per { -brand-short-name }</a>.

## General Section

startup-header = Avvio
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = Consenti l’esecuzione in contemporanea di { -brand-short-name } e Firefox
use-firefox-sync = Consiglio: questo comporta l’utilizzo di profili diversi. È possibile appoggiarsi a { -sync-brand-short-name } per la sincronizzazione dei dati.
get-started-not-logged-in = Accedi a { -sync-brand-short-name }…
get-started-configured = Apri preferenze di { -sync-brand-short-name }
always-check-default =
    .label = Controlla sempre se { -brand-short-name } è il browser predefinito
    .accesskey = t
is-default = { -brand-short-name } è attualmente il browser predefinito
is-not-default = { -brand-short-name } non è il browser predefinito
set-as-my-default-browser =
    .label = Imposta come browser predefinito…
    .accesskey = o
startup-restore-previous-session =
    .label = Ripristina la sessione precedente
    .accesskey = R
disable-extension =
    .label = Disattiva estensione
tabs-group-header = Schede
ctrl-tab-recently-used-order =
    .label = Scorri le schede con Ctrl+Tab ordinandole in base all’utilizzo più recente
    .accesskey = z
open-new-link-as-tabs =
    .label = Apri link in schede invece di nuove finestre
    .accesskey = A
warn-on-close-multiple-tabs =
    .label = Avvisa quando si chiudono più schede
    .accesskey = d
warn-on-open-many-tabs =
    .label = Avvisa quando l’apertura contemporanea di più schede potrebbe rallentare { -brand-short-name }
    .accesskey = c
switch-links-to-new-tabs =
    .label = Porta in primo piano le nuove schede aperte da un link
    .accesskey = P
show-tabs-in-taskbar =
    .label = Visualizza un’anteprima delle schede nella barra delle applicazioni di Windows
    .accesskey = V
browser-containers-enabled =
    .label = Attiva schede contenitore
    .accesskey = h
browser-containers-learn-more = Ulteriori informazioni
browser-containers-settings =
    .label = Impostazioni…
    .accesskey = o
containers-disable-alert-title = Chiudere tutte le schede contenitore?
containers-disable-alert-desc =
    { $tabCount ->
        [one] Disattivando questa funzione verrà chiusa { $tabCount } scheda contenitore. Continuare?
       *[other] Disattivando questa funzione verranno chiuse { $tabCount } schede contenitore. Continuare?
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] Chiudi { $tabCount } scheda contenitore
       *[other] Chiudi { $tabCount } schede contenitore
    }
containers-disable-alert-cancel-button = Non disattivare
containers-remove-alert-title = Rimuovere questo contenitore?
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg =
    { $count ->
        [one] Rimuovendo questo contenitore verrà chiusa { $count } scheda contenitore. Continuare?
       *[other] Rimuovendo questo contenitore verranno chiuse { $count } schede contenitore. Continuare?
    }
containers-remove-ok-button = Rimuovi questo contenitore
containers-remove-cancel-button = Non rimuovere questo contenitore

## General Section - Language & Appearance

language-and-appearance-header = Lingua e aspetto
fonts-and-colors-header = Caratteri e colori
default-font = Carattere predefinito
    .accesskey = C
default-font-size = Dimensioni
    .accesskey = D
advanced-fonts =
    .label = Avanzate…
    .accesskey = n
colors-settings =
    .label = Colori…
    .accesskey = r
language-header = Lingua
choose-language-description = Scegli la lingua in cui visualizzare le pagine web
choose-button =
    .label = Scegli…
    .accesskey = g

choose-browser-language-description = Scegli la lingua in cui visualizzare menu, messaggi e notifiche di { -brand-short-name }.
confirm-browser-language-change-description = Riavviare { -brand-short-name } per applicare queste modifiche
confirm-browser-language-change-button = Applica e riavvia

translate-web-pages =
    .label = Traduci contenuti web
    .accesskey = T
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = Traduzioni a cura di <img data-l10n-name="logo"/>
translate-exceptions =
    .label = Eccezioni…
    .accesskey = z
check-user-spelling =
    .label = Controllo ortografico durante la digitazione
    .accesskey = C

## General Section - Files and Applications

files-and-applications-title = File e applicazioni
download-header = Download
download-save-to =
    .label = Salva i file in
    .accesskey = v
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] Scegli…
           *[other] Sfoglia…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] c
           *[other] f
        }
download-always-ask-where =
    .label = Chiedi dove salvare ogni file
    .accesskey = e
applications-header = Applicazioni
applications-description = Scegli come gestire in { -brand-short-name } i file scaricati da Internet e le applicazioni da utilizzare durante la navigazione.
applications-filter =
    .placeholder = Cerca tipo di contenuto o applicazione
applications-type-column =
    .label = Tipo di contenuto
    .accesskey = T
applications-action-column =
    .label = Azione
    .accesskey = A
drm-content-header = Contenuti con DRM (Digital Rights Management)
play-drm-content =
    .label = Riproduci contenuti protetti da DRM
    .accesskey = R
play-drm-content-learn-more = Ulteriori informazioni
update-application-title = Aggiornamenti di { -brand-short-name }
update-application-description = Mantieni { -brand-short-name } aggiornato per garantire il massimo in termini di prestazioni, stabilità e sicurezza.
update-application-version = Versione { $version } <a data-l10n-name="learn-more">Novità</a>
update-history =
    .label = Mostra cronologia aggiornamenti…
    .accesskey = o
update-application-allow-description = Consenti a { -brand-short-name } di
update-application-auto =
    .label = Installare automaticamente gli aggiornamenti (consigliato)
    .accesskey = s
update-application-check-choose =
    .label = Controllare la disponibilità di aggiornamenti ma permettere all’utente di scegliere se installarli
    .accesskey = C
update-application-manual =
    .label = Non controllare mai la disponibilità di aggiornamenti (non consigliato)
    .accesskey = N
update-application-use-service =
    .label = Utilizza un servizio di sistema per installare gli aggiornamenti
    .accesskey = U
update-enable-search-update =
    .label = Aggiorna automaticamente i motori di ricerca
    .accesskey = M

## General Section - Performance

performance-title = Prestazioni
performance-use-recommended-settings-checkbox =
    .label = Utilizza le impostazioni predefinite
    .accesskey = m
performance-use-recommended-settings-desc = Queste impostazioni sono determinate in base alle caratteristiche hardware del computer e al sistema operativo.
performance-settings-learn-more = Ulteriori informazioni
performance-allow-hw-accel =
    .label = Utilizza l’accelerazione hardware quando disponibile
    .accesskey = h
performance-limit-content-process-option = Numero massimo di processi per i contenuti
    .accesskey = o
performance-limit-content-process-enabled-desc = Un numero maggiore di processi per la gestione dei contenuti può migliorare le prestazioni quando si utilizzano molte schede, comportando però un maggiore utilizzo di memoria.
performance-limit-content-process-blocked-desc = È possibile modificare il numero di processi per i contenuti solo se è attiva la modalità multiprocesso di { -brand-short-name }. <a data-l10n-name="learn-more">Scopri come verificare se la modalità multiprocesso è attiva</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (predefinito)

## General Section - Browsing

browsing-title = Navigazione
browsing-use-autoscroll =
    .label = Utilizza lo scorrimento automatico
    .accesskey = s
browsing-use-smooth-scrolling =
    .label = Utilizza lo scorrimento continuo
    .accesskey = a
browsing-use-onscreen-keyboard =
    .label = Visualizza una tastiera touch quando necessario
    .accesskey = s
browsing-use-cursor-navigation =
    .label = Utilizza sempre i tasti direzione per navigare nelle pagine
    .accesskey = l
browsing-search-on-start-typing =
    .label = Cerca nel testo quando si digita qualcosa
    .accesskey = e

## General Section - Proxy

network-proxy-title = Server proxy
network-proxy-connection-description = Determina come { -brand-short-name } si collega a Internet.
network-proxy-connection-learn-more = Ulteriori informazioni
network-proxy-connection-settings =
    .label = Impostazioni…
    .accesskey = z

## Home Section

home-new-windows-tabs-header = Nuove finestre e schede
home-new-windows-tabs-description2 = Scegli cosa visualizzare quando vengono aperti la pagina iniziale, nuove finestre e nuove schede.

## Home Section - Home Page Customization

home-homepage-mode-label = Pagina iniziale e nuove finestre
home-newtabs-mode-label = Nuove schede
home-restore-defaults =
    .label = Ripristina predefiniti
    .accesskey = R
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Pagina iniziale di Firefox (predefinita)
home-mode-choice-custom =
    .label = Indirizzi personalizzati…
home-mode-choice-blank =
    .label = Pagina vuota
home-homepage-custom-url =
    .placeholder = Incolla un indirizzo…
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] Usa la pagina corrente
           *[other] Usa le pagine correnti
        }
    .accesskey = U
choose-bookmark =
    .label = Usa un segnalibro…
    .accesskey = b

## Search Section

search-bar-header = Barra di ricerca
search-bar-hidden =
    .label = Utilizza la barra degli indirizzi per ricerche e navigazione
search-bar-shown =
    .label = Aggiungi la barra di ricerca alla barra degli strumenti
search-engine-default-header = Motore di ricerca predefinito
search-engine-default-desc = Seleziona il motore di ricerca predefinito da utilizzare nella barra degli indirizzi e nella barra di ricerca.
search-suggestions-option =
    .label = Visualizza suggerimenti di ricerca
    .accesskey = V
search-show-suggestions-url-bar-option =
    .label = Visualizza suggerimenti di ricerca tra i risultati della barra degli indirizzi
    .accesskey = i
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = Mostra i suggerimenti di ricerca prima della cronologia nei risultati della barra degli indirizzi
search-suggestions-cant-show = I suggerimenti di ricerca non verranno visualizzati tra i risultati della barra degli indirizzi in quanto { -brand-short-name } è configurato per non salvare la cronologia.
search-one-click-header = Motori di ricerca in un clic
search-one-click-desc = Scegli i motori di ricerca alternativi che appaiono nella barra degli indirizzi e nella barra di ricerca quando si inizia a digitare una parola chiave.
search-choose-engine-column =
    .label = Motore di ricerca
search-choose-keyword-column =
    .label = Parola chiave
search-restore-default =
    .label = Ripristina i motori di ricerca predefiniti
    .accesskey = m
search-remove-engine =
    .label = Rimuovi
    .accesskey = R
search-find-more-link = Trova altri motori di ricerca
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = Parola chiave duplicata
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = La parola chiave scelta è già utilizzata da “{ $name }”. Selezionarne una diversa.
search-keyword-warning-bookmark = La parola chiave scelta è già utilizzata da un segnalibro. Selezionarne una diversa.

## Containers Section

containers-back-link = « Torna indietro
containers-header = Schede contenitore
containers-add-button =
    .label = Aggiungi nuovo contenitore
    .accesskey = A
containers-preferences-button =
    .label = Preferenze
containers-remove-button =
    .label = Elimina

## Sync Section - Signed out

sync-signedout-caption = Il tuo Web, sempre con te
sync-signedout-description = Sincronizza segnalibri, cronologia, schede, password, componenti aggiuntivi e impostazioni attraverso tutti i tuoi dispositivi.
sync-signedout-account-title = Connetti il tuo { -fxaccount-brand-name[lowercase] }
sync-signedout-account-create = Non hai ancora un account? Scopri come crearne uno
    .accesskey = h
sync-signedout-account-signin =
    .label = Accedi…
    .accesskey = d
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Scarica Firefox per <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> o <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a> per sincronizzare con cellulari e tablet.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = Cambia l’immagine del profilo
sync-disconnect =
    .label = Disconnetti…
    .accesskey = t
sync-manage-account = Gestisci account
    .accesskey = G
sync-signedin-unverified = { $email } non è verificato.
sync-signedin-login-failure = Accedi per riattivare la connessione con { $email }
sync-resend-verification =
    .label = Invia di nuovo email di verifica
    .accesskey = n
sync-remove-account =
    .label = Elimina account
    .accesskey = n
sync-sign-in =
    .label = Accedi
    .accesskey = A
sync-signedin-settings-header = Impostazioni di sincronizzazione
sync-signedin-settings-desc = Scegli gli elementi da sincronizzare sui dispositivi con { -brand-short-name }.
sync-engine-bookmarks =
    .label = Segnalibri
    .accesskey = e
sync-engine-history =
    .label = Cronologia
    .accesskey = C
sync-engine-tabs =
    .label = Schede aperte
    .tooltiptext = Elementi aperti nei dispositivi sincronizzati
    .accesskey = h
sync-engine-logins =
    .label = Accessi
    .tooltiptext = Nomi utente e password salvati
    .accesskey = i
sync-engine-addresses =
    .label = Indirizzi
    .tooltiptext = Indirizzi salvati come via, città, ecc. (solo per desktop)
    .accesskey = I
sync-engine-creditcards =
    .label = Carte di credito
    .tooltiptext = Nome, numero e data di scadenza (solo per desktop)
    .accesskey = r
sync-engine-addons =
    .label = Componenti aggiuntivi
    .tooltiptext = Estensioni e temi per Firefox desktop
    .accesskey = o
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] Opzioni
           *[other] Preferenze
        }
    .tooltiptext = Impostazioni modificate nei pannelli “Generale” e “Privacy e sicurezza”
    .accesskey = z
sync-device-name-header = Nome dispositivo
sync-device-name-change =
    .label = Cambia nome dispositivo…
    .accesskey = d
sync-device-name-cancel =
    .label = Annulla
    .accesskey = n
sync-device-name-save =
    .label = Salva
    .accesskey = S
sync-mobilepromo-single = Connetti un altro dispositivo
sync-mobilepromo-multi = Gestisci dispositivi
sync-tos-link = Condizioni di utilizzo del servizio
sync-fxa-privacy-notice = Informativa sulla privacy

## Privacy Section

privacy-header = Privacy del browser

## Privacy Section - Forms

forms-header = Moduli e password
forms-ask-to-save-logins =
    .label = Chiedi se salvare le credenziali di accesso ai siti web
    .accesskey = C
forms-exceptions =
    .label = Eccezioni…
    .accesskey = z
forms-saved-logins =
    .label = Accessi salvati…
    .accesskey = s
forms-master-pw-use =
    .label = Utilizza una password principale
    .accesskey = U
forms-master-pw-change =
    .label = Cambia la password principale…
    .accesskey = w

## Privacy Section - History

history-header = Cronologia
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = Impostazioni cronologia:
    .accesskey = m
history-remember-option-all =
    .label = salva la cronologia
history-remember-option-never =
    .label = non salvare la cronologia
history-remember-option-custom =
    .label = utilizza impostazioni personalizzate
history-remember-description = Verranno salvate la cronologia di navigazione, i download, le informazioni inserite nei moduli o nei campi di ricerca.
history-dontremember-description = Verranno utilizzate le stesse impostazioni della Navigazione anonima: in questo modo non verrà salvata alcuna cronologia relativa alla navigazione.
history-private-browsing-permanent =
    .label = Utilizza sempre la modalità Navigazione anonima
    .accesskey = U
history-remember-option =
    .label = Conserva la cronologia di navigazione e dei download
    .accesskey = d
history-remember-browser-option =
    .label = Conserva la cronologia di navigazione e dei download
    .accesskey = d
history-remember-search-option =
    .label = Conserva la cronologia delle ricerche e dei moduli
    .accesskey = g
history-clear-on-close-option =
    .label = Cancella la cronologia alla chiusura di { -brand-short-name }
    .accesskey = C
history-clear-on-close-settings =
    .label = Impostazioni…
    .accesskey = I
history-clear-button =
    .label = Cancella cronologia…
    .accesskey = e

## Privacy Section - Site Data

sitedata-header = Cookie e dati dei siti web
sitedata-total-size-calculating = Calcolo dimensioni…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = I cookie, i dati salvati dai siti web e la cache stanno utilizzando { $value } { $unit } di spazio su disco
sitedata-learn-more = Ulteriori informazioni
sitedata-accept-cookies-option =
    .label = Accetta cookie e dati dai siti web (consigliato)
    .accesskey = k
sitedata-block-cookies-option =
    .label = Blocca cookie e dati dei siti web (potrebbe causare malfunzionamenti in alcuni siti)
    .accesskey = B
sitedata-keep-until = Conservali fino
    .accesskey = v
sitedata-keep-until-expire =
    .label = alla loro scadenza
sitedata-keep-until-closed =
    .label = alla chiusura di { -brand-short-name }
sitedata-accept-third-party-desc = Accetta cookie e dati da siti di terze parti
    .accesskey = t
sitedata-accept-third-party-always-option =
    .label = sempre
sitedata-accept-third-party-visited-option =
    .label = dai siti visitati
sitedata-accept-third-party-never-option =
    .label = mai

sitedata-allow-cookies-option =
    .label = Accetta cookie e dati dei siti web
    .accesskey = A

sitedata-disallow-cookies-option =
    .label = Blocca cookie e dati dei siti web
    .accesskey = B

# This label means 'type of content that is blocked', and is followed by a drop-down list with content types below.
# The list items are the strings named sitedata-block-*-option*.
sitedata-block-desc = Contenuti bloccati
    .accesskey = C

sitedata-block-trackers-option-recommended =
    .label = Elementi traccianti di terze parti (consigliato)
sitedata-block-trackers-option =
    .label = Elementi traccianti di terze parti
sitedata-block-unvisited-option =
    .label = Cookie da siti web non visitati
sitedata-block-all-third-parties-option =
    .label = Tutti i cookie di terze parti
sitedata-block-always-option =
    .label = Tutti i cookie (potrebbe causare malfunzionamenti in alcuni siti)

sitedata-clear =
    .label = Elimina dati…
    .accesskey = i
sitedata-settings =
    .label = Gestisci dati…
    .accesskey = d
sitedata-cookies-exceptions =
    .label = Eccezioni…
    .accesskey = z

## Privacy Section - Address Bar

addressbar-header = Barra degli indirizzi
addressbar-suggest = Nella barra degli indirizzi visualizza suggerimenti da
addressbar-locbar-history-option =
    .label = cronologia di navigazione
    .accesskey = n
addressbar-locbar-bookmarks-option =
    .label = segnalibri
    .accesskey = g
addressbar-locbar-openpage-option =
    .label = schede aperte
    .accesskey = d
addressbar-suggestions-settings = Modifica le impostazioni relative ai suggerimenti dei motori di ricerca

## Privacy Section - Content Blocking

content-blocking-header = Blocco contenuti

content-blocking-desc = Blocca contenuti di terze parti, come pubblicità o codice, in grado di rallentare la navigazione e tracciare i tuoi spostamenti sul Web. Personalizza le impostazioni per ottenere il perfetto equilibrio tra protezione e velocità.

content-blocking-learn-more = Ulteriori informazioni
content-blocking-restore-defaults =
  .label = Ripristina predefiniti
  .accesskey = R

content-blocking-toggle-on =
  .tooltiptext = Disattiva il blocco contenuti
content-blocking-toggle-off =
  .tooltiptext = Attiva il blocco contenuti

content-blocking-toggle-label-on = ATTIVO
  .accesskey = A
content-blocking-toggle-label-off = DISATTIVATO
  .accesskey = D

content-blocking-category-label = Scegli che cosa bloccare

# "Slow" in this instance means "slow to load on the network".
# FastBlock is a feature that blocks requests to tracking sites if they
# have not finished loading after a certain threshold of seconds.
content-blocking-fastblock-label = Elementi traccianti lenti
  .accesskey = m
content-blocking-fastblock-description = Blocca contenuti di terze parti che richiedono più di 5 secondi per essere caricati.
content-blocking-fastblock-option-enabled =
  .label = Blocca sempre
content-blocking-fastblock-option-disabled =
  .label = Non bloccare

content-blocking-tracking-protection-label = Elementi traccianti
  .accesskey = E
content-blocking-tracking-protection-description = Blocca tutti gli elementi traccianti conosciuti (nota: alcune pagine web potrebbero non caricarsi correttamente).
content-blocking-tracking-protection-option-enabled =
  .label = Blocca sempre
content-blocking-tracking-protection-option-pbm =
  .label = Blocca solo in navigazione anonima
content-blocking-tracking-protection-option-disabled =
  .label = Non bloccare
content-blocking-tracking-protection-change-blocklist = Cambia elenco per blocco contenuti…

## Privacy Section - Tracking

tracking-header = Protezione antitracciamento
tracking-desc = La protezione antitracciamento blocca elementi utilizzati per raccogliere dati sulla navigazione attraverso siti diversi. <a data-l10n-name="learn-more">Ulteriori informazioni sulla protezione antitracciamento e la privacy</a>
tracking-mode-label = Utilizza la protezione antitracciamento per bloccare i tracciatori conosciuti
tracking-mode-always =
    .label = Sempre
    .accesskey = p
tracking-mode-private =
    .label = Solo in finestre anonime
    .accesskey = f
tracking-mode-never =
    .label = Mai
    .accesskey = a
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = Attiva la protezione antitracciamento in modalità Navigazione anonima per bloccare i tracciatori conosciuti
    .accesskey = g
tracking-exceptions =
    .label = Eccezioni…
    .accesskey = E
tracking-change-block-list =
    .label = Seleziona elenco…
    .accesskey = l

## Privacy Section - Permissions

permissions-header = Permessi
permissions-location = Posizione
permissions-location-settings =
    .label = Impostazioni…
    .accesskey = t
permissions-camera = Fotocamera
permissions-camera-settings =
    .label = Impostazioni…
    .accesskey = t
permissions-microphone = Microfono
permissions-microphone-settings =
    .label = Impostazioni…
    .accesskey = t
permissions-notification = Notifiche
permissions-notification-settings =
    .label = Impostazioni…
    .accesskey = t
permissions-notification-link = Ulteriori informazioni
permissions-notification-pause =
    .label = Sospendi notifiche fino al riavvio di { -brand-short-name }
    .accesskey = n
permissions-block-autoplay-media-menu = Per i siti web che cercano di riprodurre automaticamente contenuti audio:
permissions-block-autoplay-media =
    .label = Impedisci ai siti web di riprodurre automaticamente contenuti audio
    .accesskey = m
autoplay-option-ask =
    .label = chiedi sempre
autoplay-option-allow =
    .label = consenti riproduzione
autoplay-option-dont =
    .label = non riprodurre

permissions-autoplay-link = Ulteriori informazioni

permissions-block-autoplay-media-exceptions =
    .label = Eccezioni…
    .accesskey = E
permissions-block-popups =
    .label = Blocca le finestre pop-up
    .accesskey = B
permissions-block-popups-exceptions =
    .label = Eccezioni…
    .accesskey = o
permissions-addon-install-warning =
    .label = Avvisa se un sito web cerca di installare un componente aggiuntivo
    .accesskey = A
permissions-addon-exceptions =
    .label = Eccezioni…
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = Impedisci ai servizi di accessibilità di accedere al browser
    .accesskey = b
permissions-a11y-privacy-link = Ulteriori informazioni

## Privacy Section - Data Collection

collection-header = Raccolta e utilizzo dati di { -brand-short-name }
collection-description = Cerchiamo di garantire agli utenti la possibilità di scegliere, raccogliendo solo i dati necessari per realizzare e migliorare { -brand-short-name } per tutti. Chiediamo sempre l’autorizzazione prima di raccogliere dati personali.
collection-privacy-notice = Informativa sulla privacy
collection-health-report =
    .label = Consenti a { -brand-short-name } di inviare a { -vendor-short-name } dati tecnici e relativi all’interazione con il browser
    .accesskey = v
collection-health-report-link = Ulteriori informazioni
collection-studies =
    .label = Consenti a { -brand-short-name } di installare e condurre studi
collection-studies-link = Visualizza studi di { -brand-short-name }
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = L’invio dei dati è stato disattivato nella configurazione utilizzata per questa build
collection-browser-errors =
    .label = Consenti a { -brand-short-name } di inviare a { -vendor-short-name } le segnalazioni di errori nel browser (inclusi i messaggi di errore)
    .accesskey = b
collection-browser-errors-link = Ulteriori informazioni
collection-backlogged-crash-reports =
    .label = Consenti a { -brand-short-name } di inviare segnalazioni di arresto anomalo in sospeso
    .accesskey = C
collection-backlogged-crash-reports-link = Ulteriori informazioni

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = Sicurezza
security-browsing-protection = Protezione contro contenuti ingannevoli e software a rischio
security-enable-safe-browsing =
    .label = Blocca contenuti a rischio e ingannevoli
    .accesskey = B
security-enable-safe-browsing-link = Ulteriori informazioni
security-block-downloads =
    .label = Blocca download a rischio
    .accesskey = c
security-block-uncommon-software =
    .label = Avvisa in caso di software indesiderato e non scaricato abitualmente
    .accesskey = w

## Privacy Section - Certificates

certs-header = Certificati
certs-personal-label = Quando un sito web richiede il certificato personale
certs-select-auto-option =
    .label = Selezionane uno automaticamente
    .accesskey = S
certs-select-ask-option =
    .label = Chiedi ogni volta
    .accesskey = h
certs-enable-ocsp =
    .label = Interroga risponditori OCSP per confermare la validità attuale dei certificati
    .accesskey = P
certs-view =
    .label = Mostra certificati…
    .accesskey = M
certs-devices =
    .label = Dispositivi di sicurezza…
    .accesskey = D
