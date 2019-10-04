# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## These messages are used as headings in the recommendation doorhanger

cfr-doorhanger-extension-heading = Polecane rozszerzenie
cfr-doorhanger-feature-heading = Polecana funkcja
cfr-doorhanger-pintab-heading = Wypróbuj przypinanie kart



cfr-doorhanger-extension-sumo-link =
    .tooltiptext = Dlaczego jest to wyświetlane?
cfr-doorhanger-extension-cancel-button = Nie teraz
    .accesskey = N
cfr-doorhanger-extension-ok-button = Dodaj
    .accesskey = D
cfr-doorhanger-pintab-ok-button = Przypnij tę kartę
    .accesskey = P
cfr-doorhanger-extension-manage-settings-button = Ustawienia polecania
    .accesskey = U
cfr-doorhanger-extension-never-show-recommendation = Nie pokazuj więcej polecenia tego rozszerzenia
    .accesskey = e
cfr-doorhanger-extension-learn-more-link = Więcej informacji
# This string is used on a new line below the add-on name
# Variables:
#   $name (String) - Add-on author name
cfr-doorhanger-extension-author = Autor: { $name }
# This is a notification displayed in the address bar.
# When clicked it opens a panel with a message for the user.
cfr-doorhanger-extension-notification = Polecenie

## Add-on statistics
## These strings are used to display the total number of
## users and rating for an add-on. They are shown next to each other.

# Variables:
#   $total (Number) - The rating of the add-on from 1 to 5
cfr-doorhanger-extension-rating =
    .tooltiptext =
        { $total ->
            [one] { $total } gwiazdka
            [few] { $total } gwiazdki
            [many] { $total } gwiazdek
           *[other] { $total } gwiazdki
        }
# Variables:
#   $total (Number) - The total number of users using the add-on
cfr-doorhanger-extension-total-users =
    { $total ->
        [one] { $total } użytkownik
        [few] { $total } użytkowników
        [many] { $total } użytkowników
       *[other] { $total } użytkowników
    }
cfr-doorhanger-pintab-description = Łatwy dostęp do najczęściej używanych stron dzięki kartom otwartym na stałe (nawet po ponownym uruchomieniu).

## These messages are steps on how to use the feature and are shown together.

cfr-doorhanger-pintab-step1 = <b>Kliknij prawym przyciskiem</b> kartę, którą chcesz przypiąć.
cfr-doorhanger-pintab-step2 = Wybierz <b>Przypnij kartę</b> z menu.
cfr-doorhanger-pintab-step3 = Strony z aktualizacjami mają niebieską kropkę na przypiętej karcie.
cfr-doorhanger-pintab-animation-pause = Wstrzymaj
cfr-doorhanger-pintab-animation-resume = Wznów

## Firefox Accounts Message

cfr-doorhanger-bookmark-fxa-header = Synchronizuj zakładki na każdym urządzeniu.
cfr-doorhanger-bookmark-fxa-body = Wspaniałe odkrycie! Fajnie byłoby mieć tę zakładkę także na telefonie, prawda? Zacznij korzystać z { -fxaccount-brand-name(case: "gen", capitalization: "lower") }.
cfr-doorhanger-bookmark-fxa-link-text = Synchronizuj zakładki…
cfr-doorhanger-bookmark-fxa-close-btn-tooltip =
    .aria-label = Przycisk zamknięcia
    .title = Zamknij

## Protections panel

cfr-protections-panel-header = Przeglądaj bez wścibskich oczu
cfr-protections-panel-body = Zachowaj prywatność swoich danych. { -brand-short-name } chroni Cię przed wieloma najpopularniejszymi elementami śledzącymi, które monitorują, co robisz w Internecie.
cfr-protections-panel-link-text = Więcej informacji

## What's New toolbar button and panel

cfr-whatsnew-button =
    .label = Co nowego
    .tooltiptext = Co nowego
cfr-whatsnew-panel-header = Co nowego

## Bookmark Sync

cfr-doorhanger-sync-bookmarks-header = Przenieś tę zakładkę na telefon
cfr-doorhanger-sync-bookmarks-body = Zabierz swoje zakładki, hasła, historię i nie tylko wszędzie, gdzie korzystasz z przeglądarki { -brand-product-name }.
cfr-doorhanger-sync-bookmarks-ok-button = Włącz { -sync-brand-short-name(case: "acc", capitalization: "lower") }
    .accesskey = W

## Login Sync

cfr-doorhanger-sync-logins-header = Już nigdy nie zgub żadnego hasła
cfr-doorhanger-sync-logins-body = Bezpiecznie przechowuj i synchronizuj hasła na wszystkich urządzeniach.
cfr-doorhanger-sync-logins-ok-button = Włącz { -sync-brand-short-name(case: "acc", capitalization: "lower") }
    .accesskey = W

## Send Tab

cfr-doorhanger-send-tab-header = Przeczytaj to w podróży
cfr-doorhanger-send-tab-recipe-header = Zabierz ten przepis do kuchni
cfr-doorhanger-send-tab-body = Funkcja przesyłania kart umożliwia łatwe wysłanie tego odnośnika na telefon lub wszędzie, gdzie korzystasz z przeglądarki { -brand-product-name }.
cfr-doorhanger-send-tab-ok-button = Wypróbuj wysyłanie kart
    .accesskey = W

## Firefox Send

cfr-doorhanger-firefox-send-header = Bezpiecznie udostępnij ten plik PDF
cfr-doorhanger-firefox-send-body = Chroń swoje poufne dokumenty przed wścibskimi oczami dzięki szyfrowaniu typu „end-to-end” i odnośnikowi, który znika po użyciu.
cfr-doorhanger-firefox-send-ok-button = Wypróbuj { -send-brand-name }
    .accesskey = W
