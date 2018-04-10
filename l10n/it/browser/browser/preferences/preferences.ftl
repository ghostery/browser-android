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
    .title = { PLATFORM() ->
            [windows] Opzioni
           *[other] Preferenze
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input =
    .style = width: 16em
pane-general-title = Generale
category-general =
    .tooltiptext = { pane-general-title }
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
