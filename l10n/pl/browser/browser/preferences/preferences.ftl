# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = Informuj witryny o preferencjach względem śledzenia (wysyłanie nagłówka „Do Not Track”):
do-not-track-learn-more = Więcej informacji
do-not-track-option-default =
    .label = Tylko podczas używania ochrony przed śledzeniem
do-not-track-option-always =
    .label = Zawsze
pref-page =
    .title = { PLATFORM() ->
            [windows] Opcje
           *[other] Preferencje
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Notice: The value of the `.style` attribute is a CSS string, and the `min-width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-field =
    .style = min-width: 15.4em
pane-general-title = Ogólne
category-general =
    .tooltiptext = { pane-general-title }
pane-search-title = Wyszukiwanie
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = Prywatność i bezpieczeństwo
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Konto Firefoksa
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = Wsparcie programu { -brand-short-name }
focus-search =
    .key = f
close-button =
    .aria-label = Zamknij

## Browser Restart Dialog

feature-enable-requires-restart = Konieczne jest ponowne uruchomienie programu { -brand-short-name }, aby włączyć tę funkcję.
feature-disable-requires-restart = Konieczne jest ponowne uruchomienie programu { -brand-short-name }, aby wyłączyć tę funkcję.
should-restart-title = Ponowne uruchamianie programu { -brand-short-name }
should-restart-ok = Uruchom ponownie
revert-no-restart-button = Anuluj zmiany
restart-later = Później
