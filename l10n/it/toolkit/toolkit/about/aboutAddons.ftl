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
    .value = Caricamento…
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
legacy-extensions-description = Queste estensioni non soddisfano gli standard attualmente richiesti da { -brand-short-name } e sono state disattivate. <label data-l10n-name="legacy-learn-more">Ulteriori informazioni sui cambiamenti riguardanti le estensioni in &brandShortName;</label>
extensions-view-discover =
    .name = Esplora
    .tooltiptext = { extensions-view-discover.name }
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

shortcuts-manage =
    .label = Scorciatoie da tastiera
shortcuts-empty-message = Non sono presenti scorciatoie da tastiera per questa estensione.
shortcuts-no-addons = Non ci sono estensioni attive.
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
