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
    .title =
        { PLATFORM() ->
            [windows] Opcje
           *[other] Preferencje
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input =
    .style = width: 15.4em
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Please keep the placeholder string short to avoid truncation.
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input-box =
    .style = width: 15.4em
    .placeholder =
        { PLATFORM() ->
            [windows] Szukaj w opcjach
           *[other] Szukaj w preferencjach
        }
policies-notice =
    { PLATFORM() ->
        [windows] Administrator zablokował możliwość zmiany niektórych opcji
       *[other] Administrator zablokował możliwość zmiany niektórych ustawień
    }
pane-general-title = Ogólne
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = Uruchamianie
category-home =
    .tooltiptext = Ustawienia powiązane z uruchamianiem programu
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
cancel-no-restart-button = Anuluj
restart-later = Później

## Preferences UI Search Results

search-results-header = Wyniki wyszukiwania
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] Niestety! W opcjach niczego nie odnaleziono dla wyszukiwania „<span data-l10n-name="query"></span>”.
       *[other] Niestety! W preferencjach niczego nie odnaleziono dla wyszukiwania „<span data-l10n-name="query"></span>”.
    }
search-results-help-link = Potrzebujesz pomocy? Odwiedź witrynę <a data-l10n-name="url">wsparcia programu { -brand-short-name }</a>.

## General Section

startup-header = Uruchamianie
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = Jednoczesne działanie programu { -brand-short-name } oraz Firefoksa
use-firefox-sync = Podczas jednoczesnego działania wykorzystywane są oddzielne profile. Można wykorzystać synchronizację, aby dzielić dane między nimi.
get-started-not-logged-in = Zaloguj się, aby synchronizować…
get-started-configured = Otwórz ustawienia synchronizacji
always-check-default =
    .label = Sprawdzanie, czy { -brand-short-name } jest domyślną przeglądarką
    .accesskey = e
is-default = { -brand-short-name } jest obecnie domyślną przeglądarką
is-not-default = { -brand-short-name } nie jest obecnie domyślną przeglądarką
set-as-my-default-browser =
    .label = Ustaw jako domyślną…
    .accesskey = U
startup-page = Po uruchomieniu programu { -brand-short-name }:
    .accesskey = P
startup-user-homepage =
    .label = wyświetl stronę startową
startup-blank-page =
    .label = wyświetl pustą stronę
startup-prev-session =
    .label = wyświetl okna i karty z poprzedniej sesji
disable-extension =
    .label = Wyłącz rozszerzenie
home-page-header = Strona startowa:
tabs-group-header = Karty
ctrl-tab-recently-used-order =
    .label = Przełączanie kart za pomocą Ctrl+Tab w kolejności ostatnich wyświetleń
    .accesskey = T
open-new-link-as-tabs =
    .label = Otwieranie odnośników w kartach zamiast w nowych oknach
    .accesskey = O
warn-on-close-multiple-tabs =
    .label = Ostrzeganie przed zamknięciem wielu kart
    .accesskey = a
warn-on-open-many-tabs =
    .label = Ostrzeganie o otwarciu zbyt wielu kart mogących spowolnić program { -brand-short-name }
    .accesskey = m
switch-links-to-new-tabs =
    .label = Przechodzenie do nowych kart otwieranych poprzez odnośniki
    .accesskey = c
show-tabs-in-taskbar =
    .label = Podgląd kart na pasku zadań Windows
    .accesskey = W
browser-containers-enabled =
    .label = Karty z kontekstem
    .accesskey = K
browser-containers-learn-more = Więcej informacji
browser-containers-settings =
    .label = Dostosuj…
    .accesskey = D
containers-disable-alert-title = Czy zamknąć wszystkie karty z kontekstem?
containers-disable-alert-desc =
    { $tabCount ->
        [one] Jeśli wyłączysz funkcję kart z kontekstem, jedna taka karta zostanie zamknięta. Czy na pewno wyłączyć karty z kontekstem?
        [few] Jeśli wyłączysz funkcję kart z kontekstem, { $tabCount } takie karty zostaną zamknięte. Czy na pewno wyłączyć karty z kontekstem?
       *[many] Jeśli wyłączysz funkcję kart z kontekstem, { $tabCount } takich kart zostanie zamkniętych. Czy na pewno wyłączyć karty z kontekstem?
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] Zamknij kartę z kontekstem
        [few] Zamknij { $tabCount } karty z kontekstem
       *[many] Zamknij { $tabCount } kart z kontekstem
    }
containers-disable-alert-cancel-button = Anuluj
containers-remove-alert-title = Usuwanie kontekstu
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg =
    { $count ->
        [one] Jeśli usuniesz teraz ten kontekst, { $count } karta zostanie zamknięta. Czy na pewno usunąć ten kontekst?
        [few] Jeśli usuniesz teraz ten kontekst, { $count } karty zostaną zamknięte. Czy na pewno usunąć ten kontekst?
       *[many] Jeśli usuniesz teraz ten kontekst, { $count } kart zostanie zamkniętych. Czy na pewno usunąć ten kontekst?
    }
containers-remove-ok-button = Usuń
containers-remove-cancel-button = Nie usuwaj

## General Section - Language & Appearance

language-and-appearance-header = Język i wygląd
fonts-and-colors-header = Czcionki i kolory
default-font = Domyślna czcionka:
    .accesskey = D
default-font-size = Rozmiar:
    .accesskey = R
advanced-fonts =
    .label = Zaawansowane…
    .accesskey = s
colors-settings =
    .label = Kolory…
    .accesskey = K
language-header = Język
choose-language-description = Wybierz preferowany język, w jakim mają być wyświetlane strony
choose-button =
    .label = Wybierz…
    .accesskey = e
translate-web-pages =
    .label = Tłumaczenie stron WWW
    .accesskey = T
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = Tłumaczeń dostarcza <img data-l10n-name="logo"/>
translate-exceptions =
    .label = Wyjątki…
    .accesskey = i
check-user-spelling =
    .label = Sprawdzaj pisownię podczas wprowadzania tekstu
    .accesskey = i

## General Section - Files and Applications

files-and-applications-title = Pliki i aplikacje
download-header = Pobieranie
download-save-to =
    .label = Pobierane pliki zapisuj do:
    .accesskey = o
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] Wybierz…
           *[other] Przeglądaj…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] y
           *[other] g
        }
download-always-ask-where =
    .label = Pytaj, gdzie zapisać każdy plik
    .accesskey = t
applications-header = Aplikacje
applications-description = Wybierz, jak program { -brand-short-name } obsługuje pobierane z sieci pliki lub aplikacje używane podczas przeglądania.
applications-filter =
    .placeholder = Typ pliku lub nazwa aplikacji
applications-type-column =
    .label = Typ zawartości
    .accesskey = T
applications-action-column =
    .label = Czynność
    .accesskey = C
drm-content-header = Treści DRM (Digital Rights Management)
play-drm-content =
    .label = Odtwarzanie treści chronionych przez DRM.
    .accesskey = O
play-drm-content-learn-more = Więcej informacji
update-application-title = Aktualizacje programu { -brand-short-name }
update-application-description = Utrzymuj program { -brand-short-name } aktualnym dla najlepszej wydajności, stabilności i bezpieczeństwa.
update-application-info = Wersja: { $version } <a>Informacje o wydaniu</a>
update-application-version = Wersja: { $version } <a data-l10n-name="learn-more">Informacje o wydaniu</a>
update-history =
    .label = Wyświetl historię aktualizacji…
    .accesskey = W
update-application-allow-description = Zezwalaj programowi { -brand-short-name } na:
update-application-auto =
    .label = Automatyczne instalowanie aktualizacji (zalecane)
    .accesskey = i
update-application-check-choose =
    .label = Sprawdzanie dostępności aktualizacji i pytania o ich instalację
    .accesskey = S
update-application-manual =
    .label = Niesprawdzanie dostępności aktualizacji (niezalecane)
    .accesskey = N
update-application-use-service =
    .label = Używaj usługi instalowania aktualizacji działającej w tle
    .accesskey = U
update-enable-search-update =
    .label = Automatyczne aktualizacje wyszukiwarek
    .accesskey = z

## General Section - Performance

performance-title = Wydajność
performance-use-recommended-settings-checkbox =
    .label = Zalecane ustawienia wydajności.
    .accesskey = w
performance-use-recommended-settings-desc = Ustawienia te są specjalnie dostosowane do specyfikacji tego komputera i systemu operacyjnego.
performance-settings-learn-more = Więcej informacji
performance-allow-hw-accel =
    .label = Korzystaj ze sprzętowego przyspieszania, jeśli jest dostępne
    .accesskey = s
performance-limit-content-process-option = Limit liczby procesów treści:
    .accesskey = o
performance-limit-content-process-enabled-desc = Więcej procesów treści może poprawić wydajność przy wielu otwartych kartach, ale zwiększy też zapotrzebowanie na pamięć.
performance-limit-content-process-disabled-desc = Zmiana liczby procesów jest możliwa, jeśli { -brand-short-name } został uruchomiony z obsługą wielu procesów. <a>Jak sprawdzić, czy obsługa wielu procesów jest włączona</a>?
performance-limit-content-process-blocked-desc = Zmiana liczby procesów jest możliwa, jeśli { -brand-short-name } został uruchomiony z obsługą wielu procesów. <a data-l10n-name="learn-more">Jak sprawdzić, czy obsługa wielu procesów jest włączona</a>?
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (domyślnie)

## General Section - Browsing

browsing-title = Przeglądanie
browsing-use-autoscroll =
    .label = Używaj automatycznego przewijania
    .accesskey = y
browsing-use-smooth-scrolling =
    .label = Używaj płynnego przewijania
    .accesskey = n
browsing-use-onscreen-keyboard =
    .label = Wyświetlanie klawiatury ekranowej, gdy zachodzi taka potrzeba
    .accesskey = e
browsing-use-cursor-navigation =
    .label = Zawsze używaj klawiszy kursora do nawigacji po stronach
    .accesskey = g
browsing-search-on-start-typing =
    .label = Rozpoczynaj wyszukiwanie podczas wpisywania tekstu
    .accesskey = R

## General Section - Proxy

network-proxy-title = Proxy sieciowe
network-proxy-connection-learn-more = Więcej informacji
network-proxy-connection-settings =
    .label = Ustawienia…
    .accesskey = U

## Home Section

home-new-windows-tabs-header = Nowe okna i karty
home-new-windows-tabs-description2 = Wybierz, co ma być wyświetlane przy otwieraniu strony startowej, nowych okien i kart.

## Home Section - Home Page Customization

home-homepage-mode-label = Strona startowa i nowe okna:
home-newtabs-mode-label = Nowa karta:
home-restore-defaults =
    .label = Przywróć domyślne
    .accesskey = P
home-mode-choice-default =
    .label = Strona domowa programu Firefox (domyślnie)
home-mode-choice-custom =
    .label = Inne strony
home-mode-choice-blank =
    .label = Pusta strona
home-homepage-custom-url =
    .placeholder = Adres URL
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] Użyj bieżącej strony
           *[other] Użyj bieżących stron
        }
    .accesskey = b
choose-bookmark =
    .label = Użyj zakładki…
    .accesskey = z
restore-default =
    .label = Przywróć domyślną
    .accesskey = d

## Search Section

search-bar-header = Pasek wyszukiwania
search-bar-hidden =
    .label = Pasek adresu z funkcjami wyszukiwania i nawigacji
search-bar-shown =
    .label = Osobny pasek wyszukiwania
search-engine-default-header = Domyślna wyszukiwarka
search-engine-default-desc = Wybierz domyślną wyszukiwarkę paska adresu i paska wyszukiwania.
search-suggestions-option =
    .label = Podpowiedzi wyszukiwania
    .accesskey = P
search-show-suggestions-url-bar-option =
    .label = Podpowiedzi wyszukiwania w podpowiedziach paska adresu
    .accesskey = e
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = Podpowiedzi wyszukiwania nad historią przeglądania w wynikach paska adresu
search-suggestions-cant-show = Podpowiedzi wyszukiwania nie będą wyświetlane w podpowiedziach paska adresu, ponieważ wyłączono zachowywanie historii przeglądania programu { -brand-short-name }.
search-one-click-header = Dodatkowe wyszukiwarki
search-one-click-desc = Wybierz dodatkowe wyszukiwarki wyświetlane na dole wyników wyszukiwania w pasku adresu i pasku wyszukiwania.
search-choose-engine-column =
    .label = Nazwa
search-choose-keyword-column =
    .label = Słowo kluczowe
search-restore-default =
    .label = Przywróć domyślne
    .accesskey = d
search-remove-engine =
    .label = Usuń
    .accesskey = U
search-find-more-link = Więcej wyszukiwarek
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = Słowo kluczowe już istnieje
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = Wybrano słowo kluczowe używane obecnie przez wyszukiwarkę { $name }. Należy wybrać inne.
search-keyword-warning-bookmark = Wybrano słowo kluczowe używane obecnie przez zakładkę. Należy wybrać inne.

## Containers Section

containers-back-link = « Powrót
containers-header = Karty z kontekstem
containers-add-button =
    .label = Dodaj kontekst
    .accesskey = D
containers-preferences-button =
    .label = Preferencje
containers-remove-button =
    .label = Usuń

## Sync Section - Signed out

sync-signedout-caption = Zabierz swoją sieć ze sobą
sync-signedout-description = Synchronizuj zakładki, historię, karty, hasła, dodatki i ustawienia między wszystkimi swoimi urządzeniami.
sync-signedout-account-title = { -fxaccount-brand-name }
sync-signedout-account-create = Nie masz konta? Utwórz je…
    .accesskey = U
sync-signedout-account-signin =
    .label = Zaloguj się
    .accesskey = Z
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Pobierz Firefoksa na <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Androida</a> lub <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOSa</a>, aby synchronizować ze swoim urządzeniem przenośnym.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = Zmień obraz przypisany do konta
sync-disconnect =
    .label = Rozłącz…
    .accesskey = R
sync-manage-account = Zarządzaj kontem
    .accesskey = Z
sync-signedin-unverified = Konto { $email } nie zostało zweryfikowane.
sync-signedin-login-failure = Zaloguj się, aby ponownie połączyć konto { $email }
sync-resend-verification =
    .label = Wyślij nową wiadomość weryfikującą
    .accesskey = W
sync-remove-account =
    .label = Usuń konto
    .accesskey = U
sync-sign-in =
    .label = Zaloguj
    .accesskey = o
sync-signedin-settings-header = Ustawienia synchronizacji
sync-signedin-settings-desc = Wybierz, co synchronizować pomiędzy urządzeniami używając programu { -brand-short-name }.
sync-engine-bookmarks =
    .label = Zakładki
    .accesskey = Z
sync-engine-history =
    .label = Historia
    .accesskey = H
sync-engine-tabs =
    .label = Karty
    .tooltiptext = Lista otwartych stron na wszystkich synchronizowanych urządzeniach
    .accesskey = K
sync-engine-logins =
    .label = Dane logowania
    .tooltiptext = Zachowane nazwy użytkownika i hasła
    .accesskey = a
sync-engine-addresses =
    .label = Adresy
    .tooltiptext = Zachowane adresy pocztowe (tylko na komputerach)
    .accesskey = d
sync-engine-creditcards =
    .label = Dane kart płatniczych
    .tooltiptext = Nazwiska, numery i okresy ważności (tylko na komputerach)
    .accesskey = k
sync-engine-addons =
    .label = Dodatki
    .tooltiptext = Rozszerzenia i motywy dla Firefoksa na komputery
    .accesskey = d
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] Opcje
           *[other] Preferencje
        }
    .tooltiptext = Zmienione ustawienia ogólne, prywatności i bezpieczeństwa
    .accesskey = P
sync-device-name-header = Nazwa urządzenia
sync-device-name-change =
    .label = Zmień nazwę urządzenia…
    .accesskey = n
sync-device-name-cancel =
    .label = Anuluj
    .accesskey = A
sync-device-name-save =
    .label = Zachowaj
    .accesskey = Z
sync-mobilepromo-single = Połącz inne urządzenie
sync-mobilepromo-multi = Zarządzaj urządzeniami
sync-tos-link = Warunki korzystania z usługi
sync-fxa-privacy-notice = Uwagi dotyczące prywatności

## Privacy Section

privacy-header = Prywatność

## Privacy Section - Forms

forms-header = Formularze i hasła
forms-exceptions =
    .label = Wyjątki…
    .accesskey = i
forms-saved-logins =
    .label = Zachowane dane logowania…
    .accesskey = d
forms-master-pw-use =
    .label = Hasło główne
    .accesskey = H
forms-master-pw-change =
    .label = Zmień hasło główne
    .accesskey = Z

## Privacy Section - History

history-header = Historia
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = Program { -brand-short-name }:
    .accesskey = m
history-remember-option-all =
    .label = będzie zachowywał historię
history-remember-option-never =
    .label = nie będzie zachowywał historii
history-remember-option-custom =
    .label = będzie używał ustawień użytkownika
history-remember-description = { -brand-short-name } zachowa historię przeglądania, wyszukiwania, pobieranych plików i danych formularzy.
history-dontremember-description = { -brand-short-name } będzie używał tych samych ustawień co w trybie prywatnym i nie będzie zachowywał historii podczas przeglądania witryn WWW.
history-private-browsing-permanent =
    .label = Zawsze aktywny tryb przeglądania prywatnego
    .accesskey = t
history-remember-option =
    .label = Historia przeglądania i pobierania plików
    .accesskey = s
history-remember-search-option =
    .label = Historia wyszukiwania i formularzy
    .accesskey = f
history-clear-on-close-option =
    .label = Czyszczenie historii podczas zamykania programu { -brand-short-name }
    .accesskey = z
history-clear-on-close-settings =
    .label = Ustawienia…
    .accesskey = U
history-clear-button =
    .label = Wyczyść historię…
    .accesskey = W

## Privacy Section - Site Data

sitedata-header = Ciasteczka i dane stron
sitedata-learn-more = Więcej informacji
sitedata-accept-cookies-option =
    .label = Akceptowanie ciasteczek i danych stron (zalecane)
    .accesskey = k
sitedata-block-cookies-option =
    .label = Blokowanie ciasteczek i danych stron (może powodować problemy)
    .accesskey = B
sitedata-keep-until = Przechowywanie ciasteczek:
    .accesskey = h
sitedata-accept-third-party-desc = Akceptowanie ciasteczek i danych stron witryn zewnętrznych podmiotów:
    .accesskey = c
sitedata-accept-third-party-always-option =
    .label = zawsze
sitedata-accept-third-party-visited-option =
    .label = z odwiedzonych
sitedata-accept-third-party-never-option =
    .label = nigdy
sitedata-clear =
    .label = Wyczyść dane
    .accesskey = a
sitedata-settings =
    .label = Zachowane dane…
    .accesskey = c
sitedata-cookies-exceptions =
    .label = Wyjątki…
    .accesskey = W

## Privacy Section - Address Bar

addressbar-header = Pasek adresu
addressbar-suggest = Podpowiedzi w pasku adresu opieraj na:
addressbar-locbar-history-option =
    .label = historii przeglądania
    .accesskey = h
addressbar-locbar-bookmarks-option =
    .label = zakładkach
    .accesskey = z
addressbar-locbar-openpage-option =
    .label = otwartych kartach
    .accesskey = k
addressbar-suggestions-settings = Zmień ustawienia podpowiedzi dostarczanych przez wyszukiwarki

## Privacy Section - Tracking

tracking-header = Ochrona przed śledzeniem
tracking-desc = Ochrona przed śledzeniem blokuje elementy, które zbierają informacje o przeglądaniu na wielu różnych stronach. <a data-l10n-name="learn-more">Więcej informacji o ochronie przed śledzeniem i prywatności</a>.
tracking-mode-label = Chroń przed śledzeniem przez znane elementy śledzące:
tracking-mode-always =
    .label = Zawsze
    .accesskey = Z
tracking-mode-private =
    .label = W oknach prywatnych
    .accesskey = o
tracking-mode-never =
    .label = Nigdy
    .accesskey = N
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = Ochrona przed śledzeniem przez znane elementy śledzące w oknach prywatnych.
    .accesskey = r
tracking-exceptions =
    .label = Wyjątki…
    .accesskey = W
tracking-change-block-list =
    .label = Zmień listę blokowanych…
    .accesskey = b

## Privacy Section - Permissions

permissions-header = Uprawnienia
permissions-location = Położenie
permissions-location-settings =
    .label = Ustawienia…
    .accesskey = t
permissions-camera = Kamera
permissions-camera-settings =
    .label = Ustawienia…
    .accesskey = a
permissions-microphone = Mikrofon
permissions-microphone-settings =
    .label = Ustawienia…
    .accesskey = w
permissions-notification = Powiadomienia.
permissions-notification-settings =
    .label = Ustawienia…
    .accesskey = s
permissions-notification-link = Więcej informacji
permissions-notification-pause =
    .label = Wstrzymaj powiadomienia do czasu ponownego uruchomienia Firefoksa
    .accesskey = W
permissions-block-popups =
    .label = Blokowanie wyskakujących okien
    .accesskey = B
permissions-block-popups-exceptions =
    .label = Wyjątki…
    .accesskey = t
permissions-addon-install-warning =
    .label = Ostrzeganie, gdy witryny próbują instalować dodatki
    .accesskey = O
permissions-addon-exceptions =
    .label = Wyjątki…
    .accesskey = W
permissions-a11y-privacy-checkbox =
    .label = Blokowanie dostępu do przeglądarki usługom ułatwień dostępu.
    .accesskey = u
permissions-a11y-privacy-link = Więcej informacji

## Privacy Section - Data Collection

collection-header = Dane zbierane przez program { -brand-short-name }
collection-description = Dążymy do zapewnienia odpowiedniego wyboru i zbierania wyłącznie niezbędnych danych, aby dostarczać i doskonalić program { -brand-short-name } dla nas wszystkich. Zawsze prosimy o pozwolenie przed przesłaniem danych osobistych.
collection-privacy-notice = Uwagi dotyczące prywatności
collection-health-report =
    .label = Przesyłanie do Mozilli danych technicznych i o interakcjach z programem.
    .accesskey = z
collection-health-report-link = Więcej informacji
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = Przesyłanie danych jest wyłączone przy tej konfiguracji programu
collection-browser-errors =
    .label = Przesyłanie raportów z błędów przeglądarki (razem z treścią błędów) do Mozilli.
    .accesskey = z
collection-browser-errors-link = Więcej informacji
collection-backlogged-crash-reports =
    .label = Przesyłanie zgromadzonych raportów z awarii.
    .accesskey = o
collection-backlogged-crash-reports-link = Więcej informacji

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = Bezpieczeństwo
security-browsing-protection = Ochrona przed oszustwami i niebezpiecznym oprogramowaniem
security-enable-safe-browsing =
    .label = Blokowanie niebezpiecznych i podejrzanych treści.
    .accesskey = B
security-enable-safe-browsing-link = Więcej informacji
security-block-downloads =
    .label = Blokowanie możliwości pobierania niebezpiecznych plików
    .accesskey = e
security-block-uncommon-software =
    .label = Ostrzeganie przed niepożądanym i nietypowym oprogramowaniem
    .accesskey = n

## Privacy Section - Certificates

certs-header = Certyfikaty
certs-personal-label = Kiedy serwer żąda osobistego certyfikatu użytkownika:
certs-select-auto-option =
    .label = wybierz certyfikat automatycznie
    .accesskey = a
certs-select-ask-option =
    .label = pytaj za każdym razem
    .accesskey = r
certs-enable-ocsp =
    .label = Odpytywanie serwerów OCSP w celu potwierdzenia wiarygodności certyfikatów
    .accesskey = O
certs-view =
    .label = Wyświetl certyfikaty…
    .accesskey = W
certs-devices =
    .label = Urządzenia zabezpieczające…
    .accesskey = U
