# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### UI strings for the simplified onboarding modal / about:welcome
### Various strings use a non-breaking space to avoid a single dangling /
### widowed word, so test on various window sizes if you also want this.


## These button action text can be split onto multiple lines, so use explicit
## newlines in translations to control where the line break appears (e.g., to
## avoid breaking quoted text).

onboarding-button-label-learn-more = Ulteriori informazioni
onboarding-button-label-try-now = Prova subito
onboarding-button-label-get-started = Inizia

## Welcome modal dialog strings

onboarding-welcome-header = Benvenuto in { -brand-short-name }
onboarding-welcome-body = Adesso hai il browser.<br/>Esplora tutti gli altri prodotti della famiglia { -brand-product-name }.
onboarding-welcome-learn-more = Scopri tutti i vantaggi.
onboarding-join-form-header = Entra a far parte di { -brand-product-name }
onboarding-join-form-body = Inserisci il tuo indirizzo email per iniziare
onboarding-join-form-email =
    .placeholder = Inserisci email
onboarding-join-form-email-error = Inserisci un indirizzo email valido
onboarding-join-form-legal = Proseguendo, accetti le <a data-l10n-name="terms">condizioni di utilizzo del servizio</a> e l’<a data-l10n-name="privacy">informativa sulla privacy</a>.
onboarding-join-form-continue = Continua
onboarding-start-browsing-button-label = Inizia a navigare
onboarding-cards-dismiss =
    .title = Rimuovi
    .aria-label = Rimuovi

## Firefox Sync modal dialog strings.

onboarding-sync-welcome-header = Porta { -brand-product-name } con te
onboarding-sync-welcome-content = Ritrova segnalibri, cronologia, password e altre impostazioni su tutti i tuoi dispositivi.
onboarding-sync-welcome-learn-more-link = Ulteriori informazioni sull’account Firefox
onboarding-sync-form-invalid-input = Inserire un indirizzo email valido
onboarding-sync-legal-notice = Proseguendo, accetti le <a data-l10n-name="terms">condizioni di utilizzo del servizio</a> e l’<a data-l10n-name="privacy">informativa sulla privacy</a>.
onboarding-sync-form-input =
    .placeholder = Email
onboarding-sync-form-continue-button = Continua
onboarding-sync-form-skip-login-button = Ignora questo passaggio

## This is part of the line "Enter your email to continue to Firefox Sync"

onboarding-sync-form-header = Inserisci la tua email
onboarding-sync-form-sub-header = per utilizzare { -sync-brand-name }.

## These are individual benefit messages shown with an image, title and
## description.

onboarding-benefit-products-title = Prodotti al tuo servizio
onboarding-benefit-products-text = Una famiglia completa di strumenti che rispetta la tua privacy su tutti i tuoi dispositivi.
onboarding-benefit-knowledge-title = Conoscenze pratiche
onboarding-benefit-knowledge-text = Scopri tutto ciò che ti serve per muoverti online in modo più intelligente e sicuro.
onboarding-benefit-privacy-title = Vera privacy
onboarding-benefit-privacy-text = Ogni nostra azione avviene nel rispetto della nostra “Garanzia sui dati personali”: raccogli meno dati, mantienili al sicuro, nessun segreto.

## These strings belong to the individual onboarding messages.


## Each message has a title and a description of what the browser feature is.
## Each message also has an associated button for the user to try the feature.
## The string for the button is found above, in the UI strings section

onboarding-private-browsing-title = Navigazione anonima
onboarding-private-browsing-text = Naviga al riparo da occhi indiscreti. La Navigazione anonima con blocco contenuti mette uno stop a tutti gli elementi traccianti che cercano di seguirti sul Web.
onboarding-screenshots-title = Screenshot
onboarding-screenshots-text = Cattura, salva e condividi schermate senza uscire da { -brand-short-name }. Cattura una sezione della pagina, o la pagina intera, mentre navighi. Poi salvala sul web per poterla visualizzare e condividere ancora più facilmente.
onboarding-addons-title = Componenti aggiuntivi
onboarding-addons-text = Aggiungi nuove caratteristiche in grado di rendere { -brand-short-name } ancora più adatto alle tue esigenze. Confronta prezzi, controlla il meteo o esprimi la tua personalità con un tema su misura.
onboarding-ghostery-title = Ghostery
onboarding-ghostery-text = Naviga in modo più veloce, intelligente e sicuro grazie a estensioni come Ghostery, in grado di bloccare tutte quelle fastidiose pubblicità.
onboarding-fxa-title = Sincronizzazione
onboarding-fxa-text = Crea un { -fxaccount-brand-name } e sincronizza segnalibri, password e schede aperte su tutti i dispositivi in cui utilizzi { -brand-short-name }.
onboarding-tracking-protection-title2 = Protezione dal tracciamento
onboarding-tracking-protection-text2 = { -brand-short-name } impedisce ai siti web di tracciarti durante la navigazione, rendendo la vita difficile a quelle pubblicità che cercano di seguirti in tutto il Web.
onboarding-tracking-protection-button2 = Come funziona
onboarding-data-sync-title = Porta le tue personalizzazioni sempre con te
onboarding-data-sync-text2 = Sincronizza segnalibri, password e molto altro in tutti i dispositivi in cui usi { -brand-product-name }.
onboarding-data-sync-button2 = Accedi a { -sync-brand-short-name }
onboarding-firefox-monitor-title = Tieni sotto controllo le violazioni di dati
onboarding-firefox-monitor-text = { -monitor-brand-name } verifica se la tua email è stata coinvolta in una violazione di dati e ti avvisa se appare in una nuova violazione.
onboarding-firefox-monitor-button = Iscriviti agli avvisi
onboarding-browse-privately-title = Naviga in modo riservato
onboarding-browse-privately-text = La navigazione anonima non conserva la cronologia di ricerca e navigazione, mantenendo le tue abitudini di navigazione al sicuro da chiunque altro utilizzi il tuo computer.
onboarding-browse-privately-button = Apri una finestra anonima
onboarding-firefox-send-title = Mantieni al sicuro i file che condividi
onboarding-firefox-send-text2 = Carica i tuoi file su { -send-brand-name } e condividili con crittografia end-to-end e un link che scade automaticamente.
onboarding-firefox-send-button = Prova { -send-brand-name }
onboarding-mobile-phone-title = Installa { -brand-product-name } sul tuo telefono
onboarding-mobile-phone-text = Scarica { -brand-product-name } per iOS e Android e sincronizza i tuoi dati con tutti i tuoi dispositivi.
onboarding-mobile-phone-button = Scarica il browser mobile
onboarding-send-tabs-title = Inviati una scheda
onboarding-send-tabs-text = “Invia scheda” permette di condividere immediatamente pagine con altri dispositivi senza bisogno di usare copia e incolla, o lasciare il browser.
onboarding-send-tabs-button = Inizia a utilizzare “Invia scheda”
onboarding-pocket-anywhere-title = Leggi e ascolta ovunque ti trovi
onboarding-pocket-anywhere-text2 = Salva i tuoi contenuti preferiti nell’app di { -pocket-brand-name }. Leggili, ascoltali o guardali quando ti fa più comodo, anche senza connessione a Internet.
onboarding-pocket-anywhere-button = Prova { -pocket-brand-name }
onboarding-lockwise-passwords-title = Porta le tue password sempre con te
onboarding-lockwise-passwords-text2 = Conserva al sicuro le password salvate e accedi facilmente ai tuoi account con { -lockwise-brand-name }.
onboarding-lockwise-passwords-button2 = Scarica l’app
onboarding-facebook-container-title = Metti un limite a Facebook
onboarding-facebook-container-text2 = { -facebook-container-brand-name } mantiene il tuo profilo separato da tutto il resto. In questo modo per Facebook diventerà più difficile mostrarti pubblicità mirate.
onboarding-facebook-container-button = Aggiungi l’estensione

## Message strings belonging to the Return to AMO flow

return-to-amo-sub-header = Ottimo, ora hai installato { -brand-short-name }
return-to-amo-addon-header = Perché adesso non provi <icon></icon><b>{ $addon-name }</b>?
return-to-amo-extension-button = Aggiungi l’estensione
return-to-amo-get-started-button = Inizia a usare { -brand-short-name }
