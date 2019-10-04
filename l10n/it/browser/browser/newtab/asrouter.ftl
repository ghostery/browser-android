# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cfr-doorhanger-extension-heading = Estensione consigliata
cfr-doorhanger-feature-heading = Funzione consigliata
cfr-doorhanger-pintab-heading = Prova questo: appunta scheda

cfr-doorhanger-extension-sumo-link =
    .tooltiptext = Perché viene visualizzato questo messaggio

cfr-doorhanger-extension-cancel-button = Non adesso
    .accesskey = N

cfr-doorhanger-extension-manage-settings-button = Gestisci impostazioni suggerimenti
    .accesskey = G

cfr-doorhanger-extension-never-show-recommendation = Non visualizzare suggerimenti
    .accesskey = v

cfr-doorhanger-extension-ok-button = Aggiungi
    .accesskey = A
cfr-doorhanger-pintab-ok-button = Appunta questa scheda
    .accesskey = A

cfr-doorhanger-extension-learn-more-link = Ulteriori informazioni

# This string is used on a new line below the add-on name
# Variables:
#   $name (String) - Add-on author name
cfr-doorhanger-extension-author = di { $name }

# This is a notification displayed in the address bar.
# When clicked it opens a panel with a message for the user.
cfr-doorhanger-extension-notification = Suggerimento

## Add-on statistics
## These strings are used to display the total number of
## users and rating for an add-on. They are shown next to each other.

# Variables:
#   $total (Number) - The rating of the add-on from 1 to 5
cfr-doorhanger-extension-rating =
    .tooltiptext =
        { $total ->
              [one] { $total } stella
             *[other] { $total } stelle
        }
# Variables:
#   $total (Number) - The total number of users using the add-on
cfr-doorhanger-extension-total-users =
    { $total ->
        [one] { $total } utente
       *[other] { $total } utenti
    }

cfr-doorhanger-pintab-description = Accedi facilmente ai siti web che utilizzi più spesso. Mantieni i siti aperti in una scheda (anche quando riavvii).

## These messages are steps on how to use the feature and are shown together.
cfr-doorhanger-pintab-step1 = <b>Fai clic con il tasto destro</b> sulla scheda da appuntare.
cfr-doorhanger-pintab-step2 = Seleziona <b>Appunta scheda</b> nel menu.
cfr-doorhanger-pintab-step3 = Quando i contenuti del sito vengono aggiornati, un punto blu apparirà sulla scheda.

cfr-doorhanger-pintab-animation-pause = Metti in pausa
cfr-doorhanger-pintab-animation-resume = Riprendi

cfr-doorhanger-bookmark-fxa-header = Sincronizza ovunque i tuoi segnalibri
cfr-doorhanger-bookmark-fxa-body = Ottima scoperta. Assicurati di non restare senza questo segnalibro sul tuo dispositivo mobile. Crea un { -fxaccount-brand-name }.
cfr-doorhanger-bookmark-fxa-link-text = Sincronizza subito i segnalibri…
cfr-doorhanger-bookmark-fxa-close-btn-tooltip =
  .aria-label = Pulsante di chiusura
  .title = Chiudi

## What's New toolbar button and panel

cfr-whatsnew-button =
  .label = Novità
  .tooltiptext = Novità

cfr-whatsnew-panel-header = Novità

## Bookmark Sync

cfr-doorhanger-sync-bookmarks-header = Ritrova questo segnalibro sul tuo telefono
cfr-doorhanger-sync-bookmarks-body = Mantieni segnalibri, password e cronologia sempre a portata di mano su tutti i dispositivi connessi al tuo account { -brand-product-name }.
cfr-doorhanger-sync-bookmarks-ok-button = Attiva { -sync-brand-short-name }
  .accesskey = A

## Login Sync

cfr-doorhanger-sync-logins-header = Perché rischiare di dimenticare una password?
cfr-doorhanger-sync-logins-body = Salva le password in modo sicuro e sincronizzale su tutti i tuoi dispositivi.
cfr-doorhanger-sync-logins-ok-button = Attiva { -sync-brand-short-name }
  .accesskey = A

## Send Tab

cfr-doorhanger-send-tab-header = Leggilo ovunque ti trovi
cfr-doorhanger-send-tab-recipe-header = Porta questa ricetta in cucina
cfr-doorhanger-send-tab-body = “Invia scheda” ti permette di condividere facilmente questo link con il tuo telefono e altri dispositivi connessi al tuo account { -brand-product-name }.
cfr-doorhanger-send-tab-ok-button = Prova “Invia scheda”
  .accesskey = P

## Firefox Send

cfr-doorhanger-firefox-send-header = Condividi questo PDF in modo sicuro
cfr-doorhanger-firefox-send-body = Mantieni i tuoi documenti riservati al sicuro con crittografia end-to-end e un link che scompare automaticamente dopo l’utilizzo.
cfr-doorhanger-firefox-send-ok-button = Prova { -send-brand-name }
  .accesskey = P

## Protections panel

cfr-protections-panel-header = Naviga senza lasciarti seguire
cfr-protections-panel-body = Mantieni i tuoi dati al riparo da occhi indiscreti. { -brand-short-name } ti protegge dagli elementi traccianti più comuni che cercano di seguire le tue attività online.
cfr-protections-panel-link-text = Ulteriori informazioni
