# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

### This file is not in a locales directory to prevent it from
### being translated as the feature is still in heavy development
### and strings are likely to change often.

# Variables:
#   $count (Number) - Number of tracking events blocked.
graph-week-summary =
  { $count ->
     [one] { -brand-short-name } ha bloccato { $count } tracciante nell’ultima settimana
    *[other] { -brand-short-name } ha bloccato { $count } traccianti nell’ultima settimana
  }

# Variables:
#   $count (Number) - Number of tracking events blocked.
#   $earliestDate (Number) - Unix timestamp in ms, representing a date. The
# earliest date recorded in the database.
graph-total-summary =
  { $count ->
     [one] { $count } tracciante bloccato dal { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
    *[other] { $count } traccianti bloccati dal { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
  }

# The terminology used to refer to categories of Content Blocking is also used in chrome/browser/browser.properties and should be translated consistently.
# "Standard" in this case is an adjective, meaning "default" or "normal".
# The category name in the <b> tag will be bold.
protection-header-details-standard = Il livello di protezione è impostato su <b>Normale</b>
protection-header-details-strict = Il livello di protezione è impostato su <b>Restrittivo</b>
protection-header-details-custom = Il livello di protezione è impostato su <b>Personalizzato</b>
protection-report-page-title = Protezioni per la privacy
protection-report-content-title = Protezioni per la privacy

etp-card-title = Protezione antitracciamento avanzata
etp-card-content = Gli elementi traccianti ti seguono online al fine di raccogliere informazioni sui tuoi interessi e le tue abitudini di navigazione. { -brand-short-name } blocca molti di questi traccianti e altri script dannosi.

# This string is used to label the X axis of a graph. Other days of the week are generated via Intl.DateTimeFormat,
# capitalization for this string should match the output for your locale.
graph-today = oggi

# This string is used to describe the graph for screenreader users.
graph-legend-description = Grafico contenente il numero totale di traccianti bloccati questa settimana, suddiviso per tipologia.

social-tab-title = Traccianti dei social media
social-tab-contant = I social network impostano elementi traccianti in altri siti per scoprire cosa fai, leggi e guardi quando sei online. In questo modo sono in grado di raccogliere molte più informazioni rispetto a quello che condividi nei tuoi profili online. <a data-l10n-name="learn-more-link">Ulteriori informazioni</a>

cookie-tab-title = Cookie traccianti intersito
cookie-tab-content = Questi cookie ti seguono da un sito all’altro per raccogliere informazioni su ciò che fai online. Sono impostati da terze parti come agenzie pubblicitarie e di analisi dati. Il blocco di questi cookie riduce il numero di pubblicità personalizzate che ti seguono attraverso tutto il Web. <a data-l10n-name="learn-more-link">Ulteriori informazioni</a>

tracker-tab-title = Contenuti traccianti
tracker-tab-content = I siti web possono caricare pubblicità, video e altri contenuti da fonti esterne che includono elementi traccianti. Il blocco degli elementi traccianti può velocizzare il caricamento dei siti, ma può causare il malfunzionamento di pulsanti, moduli e campi di accesso. <a data-l10n-name="learn-more-link">Ulteriori informazioni</a>

fingerprinter-tab-title = Fingerprinter
fingerprinter-tab-content = I fingerprinter raccolgono informazioni sulle impostazioni del browser e del computer al fine di creare un tuo profilo. Utilizzando questa “impronta digitale” sono in grado di seguirti attraverso siti diversi. <a data-l10n-name="learn-more-link">Ulteriori informazioni</a>

cryptominer-tab-title = Cryptominer
cryptominer-tab-content = I cryptominer utilizzano le risorse del sistema per effettuare il “mining” di valute digitali. Questi script consumano la batteria, rallentano il computer e possono aumentare il costo della bolletta elettrica. <a data-l10n-name="learn-more-link">Ulteriori informazioni</a>

lockwise-title = Non dimenticare più le tue password
lockwise-title-logged-in = { -lockwise-brand-name }
lockwise-header-content = { -lockwise-brand-name } salva le tue password in modo sicuro direttamente nel browser.
lockwise-header-content-logged-in = Salva le password in modo sicuro e sincronizzale su tutti i tuoi dispositivi.
open-about-logins-button = Apri in { -brand-short-name }
lockwise-no-logins-content = Installa l’app <a data-l10n-name="lockwise-inline-link">{ -lockwise-brand-name }</a> e porta le tue password sempre con te.

# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
lockwise-passwords-stored = Password salvate in modo sicuro <a data-l10n-name="lockwise-how-it-works">Come funziona</a>

turn-on-sync = Attiva { -sync-brand-short-name }…
  .title = Apri le impostazioni di sincronizzazione

manage-devices = Gestisci dispositivi

# Variables:
#   $count (Number) - Number of devices connected with sync.
lockwise-sync-status =
  { $count ->
     [one] Sincronizzato con un altro dispositivo
    *[other] Sincronizzato con { $count } altri dispositivi
  }
lockwise-sync-not-syncing = Non sincronizzato con altri dispositivi.

monitor-title = Tieni sotto controllo le violazioni di dati
monitor-link = Come funziona
monitor-header-content = Controlla { -monitor-brand-name } per verificare se sei stato coinvolto in una violazione di dati e ricevere avvisi per nuove violazioni.
monitor-header-content-logged-in = { -monitor-brand-name } ti avvisa se le tue informazioni compaiono in una violazione di dati conosciuta.
monitor-sign-up = Iscriviti per ricevere avvisi sulle violazioni
auto-scan = Controllato automaticamente oggi

# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-monitored-addresses =
  { $count ->
     [one] Indirizzo email monitorato.
    *[other] Indirizzi email monitorati.
  }

# This string is displayed after a large numeral that indicates the total number
# of known data breaches. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-known-breaches =
  { $count ->
     [one] Violazione di dati conosciuta che ha esposto le tue informazioni.
    *[other] Violazioni di dati conosciute che hanno esposto le tue informazioni.
  }

# This string is displayed after a large numeral that indicates the total number
# of exposed passwords. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-exposed-passwords = Password esposte da tutte le violazioni

full-report-link = Visualizza il rapporto completo in <a data-l10n-name="monitor-inline-link">{ -monitor-brand-name }</a>

# This string is displayed after a large numeral that indicates the total number
# of saved logins which may have been exposed. Don’t add $count to
# your localization, because it would result in the number showing twice.
password-warning =
  { $count ->
     [one] Credenziale salvata potrebbe essere stata esposta in una violazione di dati. Cambia la password per garantire la tua sicurezza online. <a data-l10n-name="lockwise-link">Visualizza le credenziali salvate</a>
    *[other] Credenziali salvata potrebbero essere state esposte in una violazione di dati. Cambia queste password per garantire la tua sicurezza online. <a data-l10n-name="lockwise-link">Visualizza le credenziali salvate</a>
  }

# This is the title attribute describing the graph report's link to about:settings#privacy
go-to-privacy-settings = Vai alle impostazioni relative alla privacy

# This is the title attribute describing the Lockwise card's link to about:logins
go-to-saved-logins = Vai alle credenziali salvate

## The title attribute is used to display the type of protection.
## The aria-label is spoken by screen readers to make the visual graph accessible to blind users.
##
## Variables:
##   $count (Number) - Number of specific trackers
##   $percentage (Number) - Percentage this type of tracker contributes to the whole graph

bar-tooltip-social =
  .title = Traccianti dei social media
  .aria-label =
    { $count ->
       [one] { $count } tracciante dei social media ({ $percentage }%)
      *[other] { $count } traccianti dei social media ({ $percentage }%)
    }
bar-tooltip-cookie =
  .title = Cookie traccianti intersito
  .aria-label =
    { $count ->
       [one] { $count } cookie tracciante intersito ({ $percentage }%)
      *[other] { $count } cookie traccianti intersito ({ $percentage }%)
    }
bar-tooltip-tracker =
  .title = Contenuti traccianti
  .aria-label =
    { $count ->
       [one] { $count } contenuto tracciante ({ $percentage }%)
      *[other] { $count } contenuti traccianti ({ $percentage }%)
    }
bar-tooltip-fingerprinter =
  .title = Fingerprinter
  .aria-label = { $count } fingerprinter ({ $percentage }%)
bar-tooltip-cryptominer =
  .title = Cryptominer
  .aria-label = { $count } cryptominer ({ $percentage }%)
