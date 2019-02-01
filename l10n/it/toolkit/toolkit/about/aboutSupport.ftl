# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

page-title = Informazioni per la risoluzione dei problemi
page-subtitle = Questa pagina contiene informazioni tecniche che potrebbero risultare utili per risolvere eventuali problemi. Per le risposte alle domande più comuni a proposito di { -brand-short-name }, consultare il <a data-l10n-name="support-link">sito web dedicato al supporto</a>.
crashes-title = Segnalazioni di arresto anomalo
crashes-id = ID segnalazione
crashes-send-date = Inviata
crashes-all-reports = Tutte le segnalazioni
crashes-no-config = Questa applicazione non è stata configurata per visualizzare le segnalazioni di arresto anomalo.
extensions-title = Estensioni
extensions-name = Nome
extensions-enabled = Attiva
extensions-version = Versione
extensions-id = ID
security-software-title = Software di sicurezza
security-software-type = Tipo
security-software-name = Nome
security-software-antivirus = Antivirus
security-software-antispyware = Antispyware
security-software-firewall = Firewall
features-title = Caratteristiche di { -brand-short-name }
features-name = Nome
features-version = Versione
features-id = ID
app-basics-title = Informazioni di base
app-basics-name = Nome
app-basics-version = Versione
app-basics-build-id = ID build
app-basics-update-channel = Canale di aggiornamento
app-basics-update-history = Cronologia aggiornamenti
app-basics-show-update-history = Mostra cronologia aggiornamenti
app-basics-profile-dir =
    { PLATFORM() ->
        [linux] Cartella del profilo
       *[other] Cartella del profilo
    }
app-basics-enabled-plugins = Plugin attivi
app-basics-launcher-process-status = Processo launcher
app-basics-build-config = Configurazione build
app-basics-user-agent = User agent
app-basics-os = SO
app-basics-memory-use = Utilizzo memoria
app-basics-performance = Prestazioni
app-basics-service-workers = Service worker registrati
app-basics-profiles = Profili
app-basics-multi-process-support = Finestre multiprocesso
app-basics-process-count = Processi contenuti web
app-basics-enterprise-policies = Criteri a livello aziendale
app-basics-key-google = Chiave Google
app-basics-key-mozilla = Chiave per georilevazione di Mozilla
app-basics-safe-mode = Modalità provvisoria
show-dir-label =
    { PLATFORM() ->
        [macos] Mostra nel Finder
        [windows] Apri cartella
       *[other] Apri percorso
    }
modified-key-prefs-title = Preferenze importanti modificate
modified-prefs-name = Nome
modified-prefs-value = Valore
user-js-title = Preferenze in user.js
user-js-description = Nella cartella del profilo è presente un <a data-l10n-name="user-js-link">file user.js</a> con preferenze che non sono state impostate da { -brand-short-name }.
locked-key-prefs-title = Preferenze importanti bloccate
locked-prefs-name = Nome
locked-prefs-value = Valore
graphics-title = Grafica
graphics-features-title = Caratteristiche
graphics-diagnostics-title = Diagnostica
graphics-failure-log-title = Registro errori
graphics-gpu1-title = GPU #1
graphics-gpu2-title = GPU #2
graphics-decision-log-title = Registro decisioni
graphics-crash-guards-title = Funzioni disattivate da Crash Guard
graphics-workarounds-title = Soluzioni alternative
place-database-title = Database Places
place-database-integrity = Integrità
place-database-verify-integrity = Verifica integrità
js-title = JavaScript
js-incremental-gc = GC incrementale
a11y-title = Accessibilità
a11y-activated = Attivato
a11y-force-disabled = Blocca servizi accessibilità
a11y-handler-used = Handler accessibilità utilizzato
a11y-instantiator = Istanziatore accessibilità
library-version-title = Versioni librerie
copy-text-to-clipboard-label = Copia il testo negli appunti
copy-raw-data-to-clipboard-label = Copia i dati non elaborati negli appunti
sandbox-title = Sandbox
sandbox-sys-call-log-title = Chiamate di sistema
sandbox-sys-call-index = #
sandbox-sys-call-age = Secondi fa
sandbox-sys-call-pid = PID
sandbox-sys-call-tid = TID
sandbox-sys-call-proc-type = Tipo processo
sandbox-sys-call-number = Syscall
sandbox-sys-call-args = Argomenti
safe-mode-title = Prova la modalità provvisoria
restart-in-safe-mode-label = Riavvia disattivando i componenti aggiuntivi…
media-title = Media
media-output-devices-title = Dispositivi di output
media-input-devices-title = Dispositivi di input
media-device-name = Nome
media-device-group = Gruppo
media-device-vendor = Produttore
media-device-state = Stato
media-device-preferred = Preferito
media-device-format = Formato
media-device-channels = Canali
media-device-rate = Frequenza
media-device-latency = Latenza
intl-title = Internazionalizzazione e localizzazione
intl-app-title = Impostazioni dell’applicazione
intl-locales-requested = Lingue richieste
intl-locales-available = Lingue disponibili
intl-locales-supported = Lingue applicazione
intl-locales-default = Lingua predefinita
intl-os-title = Sistema operativo
intl-os-prefs-system-locales = Lingue del sistema
intl-regional-prefs = Impostazioni internazionali
# Variables
# $days (Integer) - Number of days of crashes to log
report-crash-for-days =
    { $days ->
        [one] Segnalazioni di arresto anomalo dell’ultimo giorno
       *[other] Segnalazioni di arresto anomalo degli ultimi { $days } giorni
    }
# Variables
# $minutes (integer) - Number of minutes since crash
crashes-time-minutes =
    { $minutes ->
        [one] { $minutes } minuto fa
       *[other] { $minutes } minuti fa
    }
# Variables
# $hours (integer) - Number of hours since crash
crashes-time-hours =
    { $hours ->
        [one] { $hours } ora fa
       *[other] { $hours } ore fa
    }
# Variables
# $days (integer) - Number of days since crash
crashes-time-days =
    { $days ->
        [one] { $days } giorno fa
       *[other] { $days } giorni fa
    }
# Variables
# $reports (integer) - Number of pending reports
pending-reports =
    { $reports ->
        [one] Tutte le segnalazioni di arresto anomalo (inclusa { $reports } in attesa nell’intervallo di tempo indicato)
       *[other] Tutte le segnalazioni di arresto anomalo (incluse { $reports } in attesa nell’intervallo di tempo indicato)
    }
raw-data-copied = Dati non elaborati copiati negli appunti
text-copied = Testo copiato negli appunti

## The verb "blocked" here refers to a graphics feature such as "Direct2D" or "OpenGL layers".

blocked-driver = Bloccato a causa della versione dei driver della scheda grafica.
blocked-gfx-card = Bloccato in quanto i driver della scheda grafica presentano problemi irrisolti.
blocked-os-version = Bloccato a causa della versione del sistema operativo.
blocked-mismatched-version = Bloccato a causa della non corrispondenza della versione dei driver tra registro e DLL.
# Variables
# $driverVersion - The graphics driver version string
try-newer-driver = Bloccato a causa della versione dei driver della scheda grafica. Aggiornare i driver alla versione { $driverVersion } o successiva.
# "ClearType" is a proper noun and should not be translated. Feel free to leave English strings if
# there are no good translations, these are only used in about:support
clear-type-parameters = Parametri ClearType
compositing = Composizione
hardware-h264 = Decodifica hardware H264
main-thread-no-omtc = thread principale, no OMTC
yes = Sì
no = No
unknown = Sconosciuto
virtual-monitor-disp = Virtual Monitor Display

## The following strings indicate if an API key has been found.
## In some development versions, it's expected for some API keys that they are
## not found.

found = Disponibile
missing = Non presente
gpu-process-pid = GPUProcessPid
gpu-process = GPUProcess
gpu-description = Descrizione
gpu-vendor-id = ID produttore
gpu-device-id = ID dispositivo
gpu-subsys-id = ID sottosistema
gpu-drivers = Driver
gpu-ram = RAM
gpu-driver-version = Versione driver
gpu-driver-date = Data aggiornamento driver
gpu-active = Attivo
webgl1-wsiinfo = Informazioni driver WSI WebGL 1
webgl1-renderer = Renderer driver WebGL 1
webgl1-version = Versione driver WebGL 1
webgl1-driver-extensions = Estensioni driver WebGL 1
webgl1-extensions = Estensioni WebGL 1
webgl2-wsiinfo = Informazioni driver WSI WebGL 2
webgl2-renderer = Renderer driver WebGL 2
webgl2-version = Versione driver WebGL 2
webgl2-driver-extensions = Estensioni driver WebGL 2
webgl2-extensions = Estensioni WebGL 2
blocklisted-bug = Bloccato per problemi noti
# Variables
# $bugNumber (string) - String of bug number from Bugzilla
bug-link = bug { $bugNumber }
# Variables
# $failureCode (string) - String that can be searched in the source tree.
unknown-failure = Bloccato: codice non funzionante { $failureCode }
d3d11layers-crash-guard = D3D11 - Compositore
d3d11video-crash-guard = D3D11 - Decoder video
d3d9video-crash-buard = D3D9 - Decoder video
glcontext-crash-guard = OpenGL
reset-on-next-restart = Reimposta al prossimo avvio
gpu-process-kill-button = Termina processo GPU
gpu-device-reset = Ripristino dispositivo
gpu-device-reset-button = Avvia ripristino dispositivo
uses-tiling = Usa Tiling
content-uses-tiling = Usa Tiling (contenuti)
off-main-thread-paint-enabled = Off Main Thread Painting attivo
off-main-thread-paint-worker-count = Conteggio worker Off Main Thread Painting
low-end-machine = Rilevato hardware con scarse prestazioni
target-frame-rate = Obiettivo per frequenza fotogrammi
audio-backend = Back-end audio
max-audio-channels = Numero massimo di canali
channel-layout = Layout preferito dei canali
sample-rate = Frequenza di campionamento preferita
min-lib-versions = Versione minima prevista
loaded-lib-versions = Versione in uso
has-seccomp-bpf = Seccomp-BPF (filtro chiamate di sistema)
has-seccomp-tsync = Sincronizzazione thread seccomp
has-user-namespaces = Namespace dell’utente
has-privileged-user-namespaces = Namespace dell’utente per processi privilegiati
can-sandbox-content = Sandbox processo per i contenuti
can-sandbox-media = Sandbox plugin multimediali
content-sandbox-level = Livello sandbox content process
effective-content-sandbox-level = Livello effettivo sandbox content process
sandbox-proc-type-content = contenuti
sandbox-proc-type-file = contenuto file
sandbox-proc-type-media-plugin = plugin multimediale
launcher-process-status-0 = Attivo
launcher-process-status-1 = Disattivato a causa di errori
launcher-process-status-2 = Disattivato in modo forzato
launcher-process-status-unknown = Stato sconosciuto
# Variables
# $remoteWindows (integer) - Number of remote windows
# $totalWindows (integer) - Number of total windows
multi-process-windows = { $remoteWindows }/{ $totalWindows }
multi-process-status-0 = Attivato manualmente
multi-process-status-1 = Attivato automaticamente
multi-process-status-2 = Disattivato
multi-process-status-4 = Disattivato a causa degli strumenti di accessibilità
multi-process-status-6 = Disattivato per mancanza supporto inserimento testo
multi-process-status-7 = Disattivato da componenti aggiuntivi
multi-process-status-8 = Disattivato in modo forzato
multi-process-status-unknown = Stato sconosciuto
async-pan-zoom = Panoramica/zoom asincroni (APZ)
apz-none = nessuno
wheel-enabled = input rotella attivo
touch-enabled = input touch attivo
drag-enabled = trascinamento barre di scorrimento attivo
keyboard-enabled = tastiera attiva
autoscroll-enabled = scorrimento automatico attivo

## Variables
## $preferenceKey (string) - String ID of preference

wheel-warning = input rotella asincrono disattivato a causa di una preferenza non supportata: { $preferenceKey }
touch-warning = input touch asincrono disattivato a causa di una preferenza non supportata: { $preferenceKey }

## Strings representing the status of the Enterprise Policies engine.

policies-inactive = Disattivati
policies-active = Attivi
policies-error = Errore
