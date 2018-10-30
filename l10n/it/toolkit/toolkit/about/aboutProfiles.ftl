# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

profiles-title = Informazioni sui profili
profiles-subtitle = Questa pagina permette di gestire i profili. Ogni profilo rappresenta una realtà separata, con cronologia, segnalibri, impostazioni e componenti aggiuntivi completamente indipendenti.
profiles-create = Crea un nuovo profilo
profiles-restart-title = Riavvia
profiles-restart-in-safe-mode = Riavvia disattivando i componenti aggiuntivi…
profiles-restart-normal = Riavvia normalmente…
# Variables:
#   $name (String) - Name of the profile
profiles-name = Profilo: { $name }
profiles-is-default = Profilo predefinito
profiles-rootdir = Cartella radice
# localDir is used to show the directory corresponding to
# the main profile directory that exists for the purpose of storing data on the
# local filesystem, including cache files or other data files that may not
# represent critical user data. (e.g., this directory may not be included as
# part of a backup scheme.)
# In case localDir and rootDir are equal, localDir is not shown.
profiles-localdir = Cartella locale
profiles-current-profile = Non è possibile eliminare il profilo attualmente in uso.
profiles-in-use-profile = Questo profilo è attualmente utilizzato da un’altra applicazione e non può essere eliminato.
profiles-rename = Rinomina
profiles-remove = Elimina
profiles-set-as-default = Imposta come profilo predefinito
profiles-launch-profile = Avvia un nuovo browser con questo profilo
profiles-yes = Sì
profiles-no = No
profiles-rename-profile-title = Rinomina il profilo
# Variables:
#   $name (String) - Name of the profile
profiles-rename-profile = Rinomina il profilo “{ $name }”
profiles-invalid-profile-name-title = Nome profilo non valido
# Variables:
#   $name (String) - Name of the profile
profiles-invalid-profile-name = Il nome “{ $name }” indicato per il profilo non è valido.
profiles-delete-profile-title = Elimina il profilo
# Variables:
#   $dir (String) - Path to be displayed
profiles-delete-profile-confirm =
    Quando si elimina un profilo questo verrà rimosso dall’elenco dei profili disponibili e non sarà possibile annullare l’operazione.
    È possibile scegliere di cancellare anche i file associati al profilo, inclusi impostazioni, certificati e altri dati dell’utente. Scegliendo questa opzione la cartella “{ $dir }” verrà eliminata e non sarà possibile ripristinarla.
    Eliminare i file associati al profilo?
profiles-delete-files = Elimina i file
profiles-dont-delete-files = Non eliminare i file
profiles-delete-profile-failed-title = Errore
profiles-delete-profile-failed-message = Si è verificato un errore durante il tentativo di rimuovere questo profilo.
profiles-opendir =
    { PLATFORM() ->
        [macos] Mostra nel Finder
        [windows] Apri cartella
       *[other] Apri percorso
    }
