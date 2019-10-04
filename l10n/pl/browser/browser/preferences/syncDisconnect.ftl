# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

sync-disconnect-dialog =
    .title = Czy zaprzestać { -sync-brand-short-name(case: "gen", capitalization: "lower") }?
    .style = width: 36em; min-height: 35em;
sync-disconnect-heading = Czy usunąć dane przeglądarki z tego komputera podczas rozłączania? Zsynchronizowane dane pozostaną dostępne na koncie { -sync-brand-name(case: "gen", capitalization: "lower") }.
sync-disconnect-remove-sync-caption = Usuń dane { -sync-brand-name(case: "gen", capitalization: "lower") }
sync-disconnect-remove-sync-data = Zakładki, historia, hasła itp.
sync-disconnect-remove-other-caption = Usuń pozostałe dane prywatne
sync-disconnect-remove-other-data = Ciasteczka, pamięć podręczna, dane witryn trybu offline itp.
# Shown while the disconnect is in progress
sync-disconnect-disconnecting = Rozłączanie…
sync-disconnect-cancel =
    .label = Anuluj
    .accesskey = A

## Disconnect confirm Button
##
## The 2 labels which may be shown on the single "Disconnect" button, depending
## on the state of the checkboxes.

sync-disconnect-confirm-disconnect-delete =
    .label = Rozłącz i usuń
    .accesskey = R
sync-disconnect-confirm-disconnect =
    .label = Zwyczajnie rozłącz
    .accesskey = Z
