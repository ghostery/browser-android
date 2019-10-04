# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

addons-window =
    .title = Gestione componenti aggiuntivi
search-header =
    .placeholder = Cerca in addons.mozilla.org
    .searchbuttonlabel = Cerca
search-header-shortcut =
    .key = f
loading-label =
    .value = Caricamento in corso…
list-empty-installed =
    .value = Non risulta installato alcun componente aggiuntivo di questo tipo
list-empty-available-updates =
    .value = Nessun aggiornamento disponibile
list-empty-recent-updates =
    .value = Nessun componente aggiuntivo è stato aggiornato di recente
list-empty-find-updates =
    .label = Controlla aggiornamenti
list-empty-button =
    .label = Scopri altre informazioni sui componenti aggiuntivi
install-addon-from-file =
    .label = Installa componente aggiuntivo da file…
    .accesskey = I
help-button = Supporto componenti aggiuntivi
preferences =
    { PLATFORM() ->
        [windows] Opzioni di { -brand-short-name }
       *[other] Preferenze di { -brand-short-name }
    }
tools-menu =
    .tooltiptext = Strumenti per tutti i componenti aggiuntivi
show-unsigned-extensions-button =
    .label = Non è stato possibile verificare alcune estensioni
show-all-extensions-button =
    .label = Visualizza tutte le estensioni
debug-addons =
    .label = Debug componenti aggiuntivi
    .accesskey = D
cmd-show-details =
    .label = Visualizza ulteriori informazioni
    .accesskey = V
cmd-find-updates =
    .label = Controlla aggiornamenti
    .accesskey = C
cmd-preferences =
    .label =
        { PLATFORM() ->
            [windows] Opzioni
           *[other] Preferenze
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
cmd-enable-theme =
    .label = Utilizza questo tema
    .accesskey = t
cmd-disable-theme =
    .label = Abbandona questo tema
    .accesskey = t
cmd-install-addon =
    .label = Installa
    .accesskey = I
cmd-contribute =
    .label = Fai una donazione
    .accesskey = F
    .tooltiptext = Contribuisci allo sviluppo di questo componente aggiuntivo
discover-title = Che cosa sono i componenti aggiuntivi?
discover-description = I componenti aggiuntivi sono applicazioni che permettono di personalizzare { -brand-short-name } aggiungendo nuove caratteristiche o modificandone l’aspetto. Rendi { -brand-short-name } unico aggiungendo una nuova barra laterale per risparmiare tempo, visualizza le notifiche del meteo direttamente in { -brand-short-name } o cambiane l’aspetto con un nuovo tema.
discover-footer = Quando si è connessi a Internet in questo pannello vengono visualizzati alcuni dei componenti aggiuntivi più popolari e più interessanti.
detail-version =
    .label = Versione
detail-last-updated =
    .label = Ultimo aggiornamento
detail-contributions-description = Lo sviluppatore di questo componente aggiuntivo chiede agli utenti una piccola donazione per contribuire al suo sviluppo.
detail-contributions-button = Fai una donazione
    .title = Contribuisci allo sviluppo di questo componente aggiuntivo
    .accesskey = C
detail-update-type =
    .value = Aggiornamento automatico
detail-update-default =
    .label = Predefinito
    .tooltiptext = Installa automaticamente gli aggiornamenti se questa è l’impostazione predefinita
detail-update-automatic =
    .label = Attivo
    .tooltiptext = Installa automaticamente gli aggiornamenti
detail-update-manual =
    .label = Disattivato
    .tooltiptext = Non installare automaticamente gli aggiornamenti
detail-private-browsing-label = Funzionamento in finestre anonime
detail-private-browsing-description2 = Se viene garantito il permesso, l’estensione potrà accedere alle tue attività online nelle finestre anonime. <label data-l10n-name="detail-private-browsing-learn-more">Ulteriori informazioni</label>
detail-private-disallowed-label = Disattivata in finestre anonime
detail-private-disallowed-description = Questa estensione non funziona in navigazione anonima. <label data-l10n-name="detail-private-browsing-learn-more">Ulteriori informazioni</label>
detail-private-disallowed-description2 = Questa estensione non funziona in navigazione anonima. <<a data-l10n-name="learn-more">Ulteriori informazioni</a>
detail-private-required-label = Richiede accesso alle finestre anonime
detail-private-required-description = Questa estensione ha accesso alle tue attività online nelle finestre anonime. <label data-l10n-name="detail-private-browsing-learn-more">Ulteriori informazioni</label>
detail-private-required-description2 = Questa estensione ha accesso alle tue attività online nelle finestre anonime. <a data-l10n-name="learn-more">Ulteriori informazioni</a>
detail-private-browsing-on =
    .label = Consenti
    .tooltiptext = Attiva in Navigazione anonima
detail-private-browsing-off =
    .label = Non consentire
    .tooltiptext = Disattiva in Navigazione anonima
detail-home =
    .label = Sito web
detail-home-value =
    .value = { detail-home.label }
detail-repository =
    .label = Profilo del componente aggiuntivo
detail-repository-value =
    .value = { detail-repository.label }
detail-check-for-updates =
    .label = Controlla aggiornamenti
    .accesskey = e
    .tooltiptext = Controlla aggiornamenti per questo componente aggiuntivo
detail-show-preferences =
    .label =
        { PLATFORM() ->
            [windows] Opzioni
           *[other] Preferenze
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
    .tooltiptext =
        { PLATFORM() ->
            [windows] Modifica le opzioni di questo componente aggiuntivo
           *[other] Modifica le preferenze di questo componente aggiuntivo
        }
detail-rating =
    .value = Voto
addon-restart-now =
    .label = Riavvia adesso
disabled-unsigned-heading =
    .value = Alcuni componenti aggiuntivi sono stati disattivati
disabled-unsigned-description = I seguenti componenti non sono stati verificati per l’utilizzo in { -brand-short-name }. È possibile <label data-l10n-name="find-addons">cercare delle alternative</label> o chiedere allo sviluppatore di farli verificare.
disabled-unsigned-learn-more = Scopri ulteriori informazioni sul nostro impegno per garantire la sicurezza degli utenti online.
disabled-unsigned-devinfo = Gli sviluppatori interessati al processo di verifica dei componenti aggiuntivi possono consultare il seguente <label data-l10n-name="learn-more">manuale</label>.
plugin-deprecation-description = Manca qualcosa? Alcuni plugin non sono più supportati da { -brand-short-name }. <label data-l10n-name="learn-more">Ulteriori informazioni.</label>
legacy-warning-show-legacy = Mostra le estensioni obsolete
legacy-extensions =
    .value = Estensioni obsolete
legacy-extensions-description = Queste estensioni non soddisfano gli standard attualmente richiesti da { -brand-short-name } e sono state disattivate. <label data-l10n-name="legacy-learn-more">Ulteriori informazioni sui cambiamenti riguardanti le estensioni</label>
private-browsing-description2 =
    Il funzionamento delle estensioni in finestre anonime sta per cambiare.
    Qualunque estensione aggiunta a { -brand-short-name } non funzionerà in
    finestre anonime per impostazione predefinita. Un’estensione non funzionerà
    in finestre anonime e non avrà accesso alle attività online, a meno che il
    funzionamento sia stato consentito nelle impostazioni. Questa modifica è
    stata introdotta per garantire la riservatezza dei dati di navigazione
    quando si utilizzano finestre anonime.
    <label data-l10n-name="private-browsing-learn-more">Scopri come gestire le impostazioni delle estensioni</label>
extensions-view-discopane =
    .name = Consigli
    .tooltiptext = { extensions-view-discopane.name }
extensions-view-recent-updates =
    .name = Aggiornamenti recenti
    .tooltiptext = { extensions-view-recent-updates.name }
extensions-view-available-updates =
    .name = Aggiornamenti disponibili
    .tooltiptext = { extensions-view-available-updates.name }

## These are global warnings

extensions-warning-safe-mode-label =
    .value = Tutti i componenti aggiuntivi sono stati disattivati dalla modalità provvisoria.
extensions-warning-safe-mode-container =
    .tooltiptext = { extensions-warning-safe-mode-label.value }
extensions-warning-check-compatibility-label =
    .value = Il controllo di compatibilità dei componenti aggiuntivi è disattivato. Potrebbero essere presenti dei componenti aggiuntivi non compatibili.
extensions-warning-check-compatibility-container =
    .tooltiptext = { extensions-warning-check-compatibility-label.value }
extensions-warning-check-compatibility-enable =
    .label = Attiva
    .tooltiptext = Attiva il controllo di compatibilità dei componenti aggiuntivi
extensions-warning-update-security-label =
    .value = Il controllo sulla sicurezza degli aggiornamenti dei componenti aggiuntivi è disattivato. Il sistema potrebbe essere danneggiato da un aggiornamento.
extensions-warning-update-security-container =
    .tooltiptext = { extensions-warning-update-security-label.value }
extensions-warning-update-security-enable =
    .label = Attiva
    .tooltiptext = Attiva il controllo sulla sicurezza degli aggiornamenti dei componenti aggiuntivi

## Strings connected to add-on updates

extensions-updates-check-for-updates =
    .label = Controlla aggiornamenti
    .accesskey = C
extensions-updates-view-updates =
    .label = Visualizza aggiornamenti recenti
    .accesskey = V

# This menu item is a checkbox that toggles the default global behavior for
# add-on update checking.

extensions-updates-update-addons-automatically =
    .label = Aggiorna automaticamente i componenti aggiuntivi
    .accesskey = A

## Specific add-ons can have custom update checking behaviors ("Manually",
## "Automatically", "Use default global behavior"). These menu items reset the
## update checking behavior for all add-ons to the default global behavior
## (which itself is either "Automatically" or "Manually", controlled by the
## extensions-updates-update-addons-automatically.label menu item).

extensions-updates-reset-updates-to-automatic =
    .label = Ripristina l’aggiornamento automatico per tutti i componenti aggiuntivi
    .accesskey = R
extensions-updates-reset-updates-to-manual =
    .label = Ripristina l’aggiornamento manuale per tutti i componenti aggiuntivi
    .accesskey = R

## Status messages displayed when updating add-ons

extensions-updates-updating =
    .value = Aggiornamento dei componenti aggiuntivi in corso
extensions-updates-installed =
    .value = I componenti aggiuntivi sono stati aggiornati.
extensions-updates-downloaded =
    .value = Gli aggiornamenti sono stati scaricati.
extensions-updates-restart =
    .label = Riavvia adesso per completare l’installazione
extensions-updates-none-found =
    .value = Nessun aggiornamento disponibile
extensions-updates-manual-updates-found =
    .label = Visualizza aggiornamenti disponibili
extensions-updates-update-selected =
    .label = Installa aggiornamenti
    .tooltiptext = Installa gli aggiornamenti disponibili in questo elenco

## Extension shortcut management

manage-extensions-shortcuts =
    .label = Gestisci scorciatoie da tastiera
    .accesskey = G
shortcuts-no-addons = Non ci sono estensioni attive.
shortcuts-no-commands = Le seguenti estensioni non hanno scorciatoie da tastiera:
shortcuts-input =
  .placeholder = Inserisci una scorciatoia

shortcuts-browserAction = Attiva estensione
shortcuts-pageAction = Attiva azione pagina
shortcuts-sidebarAction = Attiva/disattiva barra laterale

shortcuts-modifier-mac = Includi Ctrl, Alt o ⌘
shortcuts-modifier-other = Includi Ctrl o Alt
shortcuts-invalid = Combinazione non valida
shortcuts-letter = Inserisci una lettera
shortcuts-system = Non è possibile sostituire una scorciatoia da tastiera di { -brand-short-name }

# String displayed in warning label when there is a duplicate shortcut
shortcuts-duplicate = Scorciatoia da tastiera duplicata

# String displayed when a keyboard shortcut is already assigned to more than one add-on
# Variables:
#   $shortcut (string) - Shortcut string for the add-on
shortcuts-duplicate-warning-message = La combinazione { $shortcut } è utilizzata come scorciatoia da tastiera per più comandi. Scorciatoie duplicate possono causare comportamenti imprevisti.

shortcuts-exists = Già utilizzata da { $addon }
shortcuts-card-expand-button = Visualizza altre { $numberToShow }
shortcuts-card-collapse-button = Mostra meno scorciatoie
go-back-button =
    .tooltiptext = Torna indietro

## Recommended add-ons page

# Explanatory introduction to the list of recommended add-ons. The action word
# ("recommends") in the final sentence is a link to external documentation.
discopane-intro =
    Estensioni e temi sono l’equivalente delle app per il tuo browser;
    permettono di proteggere password, scaricare video, risparmiare negli
    acquisti online, bloccare pubblicità fastidiose, cambiare l’aspetto del
    browser, e molto altro ancora. Questi software sono spesso sviluppati da
    terze parti. Ecco una selezione <a data-l10n-name="learn-more-trigger">
    consigliata</a> da { -brand-product-name }, con la garanzia di sicurezza,
    prestazioni e funzionalità al massimo livello.

discopane-notice-recommendations =
    Alcuni consigli in questa pagina sono personalizzati. Sono basati sulle
    estensioni già installate, le impostazioni del profilo e statistiche
    d’utilizzo.
discopane-notice-learn-more = Ulteriori informazioni

privacy-policy = Informativa sulla privacy

# Refers to the author of an add-on, shown below the name of the add-on.
# Variables:
#   $author (string) - The name of the add-on developer.
created-by-author = di <a data-l10n-name="author">{ $author }</a>
user-count = Utenti: { $dailyUsers }
install-extension-button = Aggiungi a { -brand-product-name }
install-theme-button = Installa tema
# The label of the button that appears after installing an add-on. Upon click,
# the detailed add-on view is opened, from where the add-on can be managed.
manage-addon-button = Gestisci
find-more-addons = Trova altri componenti aggiuntivi

## Add-on actions
report-addon-button = Segnala
remove-addon-button = Rimuovi
disable-addon-button = Disattiva
enable-addon-button = Attiva
expand-addon-button = Altre opzioni
preferences-addon-button =
    { PLATFORM() ->
        [windows] Opzioni
       *[other] Preferenze
    }
details-addon-button = Dettagli
release-notes-addon-button = Note di versione
permissions-addon-button = Permessi
addons-enabled-heading = Elementi attivi
addons-disabled-heading = Elementi disattivati

extension-enabled-heading = Attive
extension-disabled-heading = Disattivate

theme-enabled-heading = Attivi
theme-disabled-heading = Disattivati

plugin-enabled-heading = Attivi
plugin-disabled-heading = Disattivati

dictionary-enabled-heading = Attivi
dictionary-disabled-heading = Disattivati

locale-enabled-heading = Attive
locale-disabled-heading = Disattivate

ask-to-activate-button = Chiedi prima di attivare
always-activate-button = Attiva sempre
never-activate-button = Non attivare mai

addon-detail-author-label = Autore
addon-detail-version-label = Versione
addon-detail-last-updated-label = Ultimo aggiornamento
addon-detail-homepage-label = Sito web
addon-detail-rating-label = Voto

# The average rating that the add-on has received.
# Variables:
#   $rating (number) - A number between 0 and 5. The translation should show at most one digit after the comma.
five-star-rating =
  .title = Voto: { NUMBER($rating, maximumFractionDigits: 1) } su 5

addon-name-disabled = { $name } (disattivato)
addon-detail-reviews-link =
    { $numberOfReviews ->
        [one] { $numberOfReviews } recensione
       *[other] { $numberOfReviews } recensioni
}
pending-uninstall-description = <span data-l10n-name="addon-name">{ $addon }</span> è stato rimosso.
pending-uninstall-undo-button = Annulla
addon-detail-updates-label = Aggiornamento automatico
addon-detail-updates-radio-default = Predefinito
addon-detail-updates-radio-on = Attivo
addon-detail-updates-radio-off = Disattivato
addon-detail-update-check-label = Controlla aggiornamenti
install-update-button = Aggiorna
addon-badge-private-browsing-allowed =
    .title = Attiva in finestre anonime
addon-badge-private-browsing-allowed2 =
    .title = Attiva in finestre anonime
    .aria-label = { addon-badge-private-browsing-allowed2.title }
addon-detail-private-browsing-help = Se viene garantito il permesso, l’estensione potrà accedere alle tue attività online nelle finestre anonime. <a data-l10n-name="learn-more">Ulteriori informazioni</a>
addon-detail-private-browsing-allow = Consenti
addon-detail-private-browsing-disallow = Non consentire
addon-badge-recommended =
  .title = Consigliata
  .alt = Consigliata
addon-badge-recommended2 =
  .title = Vengono consigliate solo estensioni che soddisfano i requisiti di sicurezza e prestazioni di { -brand-product-name }
  .aria-label = { addon-badge-recommended2.title }
available-updates-heading = Aggiornamenti disponibili
recent-updates-heading = Aggiornamenti recenti
release-notes-loading = Caricamento in corso…
release-notes-error = Siamo spiacenti, si è verificato un errore nel caricamento delle note di versione.
addon-permissions-empty = Questa estensione non richiede alcun permesso
recommended-extensions-heading = Estensioni consigliate
recommended-themes-heading = Temi consigliati
recommended-theme-1 = Ti senti creativo? <a data-l10n-name="link">Disegna il tuo tema con Firefox Color</a>.
