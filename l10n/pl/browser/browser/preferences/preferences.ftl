# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description=Informuj witryny o preferencjach względem śledzenia (wysyłanie nagłówka „Do Not Track”):
do-not-track-learn-more=Więcej informacji
do-not-track-option-default=
  .label=tylko podczas używania ochrony przed śledzeniem
do-not-track-option-default-content-blocking=
  .label=gdy { -brand-short-name } blokuje wykryte elementy śledzące
do-not-track-option-always=
  .label=zawsze
pref-page=
  .title=
    { PLATFORM() ->
      [windows] Opcje
      *[other] Preferencje
    }
search-input-box=
  .style=width: 15.4em
  .placeholder=
    { PLATFORM() ->
      [windows] Szukaj w opcjach
      *[other] Szukaj w preferencjach
    }
policies-notice=
  { PLATFORM() ->
    [windows] Administrator zablokował możliwość zmiany niektórych opcji
    *[other] Administrator zablokował możliwość zmiany niektórych ustawień
  }
pane-general-title=Ogólne
category-general=
  .tooltiptext=Ogólne ustawienia
pane-home-title=Uruchamianie
category-home=
  .tooltiptext=Ustawienia powiązane z uruchamianiem programu
pane-search-title=Wyszukiwanie
category-search=
  .tooltiptext=Ustawienia dotyczące wyszukiwania
pane-privacy-title=Prywatność i bezpieczeństwo
category-privacy=
  .tooltiptext=Ustawienia dotyczące prywatności i bezpieczeństwa
pane-sync-title=Synchronizacja
category-sync=
  .tooltiptext=Konto programu Firefox
help-button-label=Wsparcie programu { -brand-short-name }
addons-button-label=Dodatki i motywy
focus-search=
  .key=f
close-button=
  .aria-label=Zamknij

feature-enable-requires-restart=Konieczne jest ponowne uruchomienie programu, aby włączyć tę funkcję.
feature-disable-requires-restart=Konieczne jest ponowne uruchomienie programu, aby wyłączyć tę funkcję.
should-restart-title=Wymagane ponowne uruchamianie programu
should-restart-ok=Uruchom ponownie
cancel-no-restart-button=Anuluj
restart-later=Później

extension-controlled-homepage-override=Rozszerzenie „{ $name }” <img data-l10n-name="icon"/> kontroluje stronę startową.
extension-controlled-new-tab-url=Rozszerzenie „{ $name }” <img data-l10n-name="icon"/> kontroluje stronę nowej karty.
extension-controlled-web-notifications=Rozszerzenie „{ $name }” <img data-l10n-name="icon"/> kontroluje to ustawienie.
extension-controlled-default-search=Rozszerzenie „{ $name }” <img data-l10n-name="icon"/> zmieniło domyślną wyszukiwarkę.
extension-controlled-privacy-containers=Rozszerzenie „{ $name }” <img data-l10n-name="icon"/> wymaga włączonych „Kart z kontekstem”.
extension-controlled-websites-tracking-protection-mode=Rozszerzenie „{ $name }” <img data-l10n-name="icon"/> kontroluje ochronę przed śledzeniem.
extension-controlled-websites-content-blocking-all-trackers=Rozszerzenie „{ $name }” <img data-l10n-name="icon"/> kontroluje to ustawienie.
extension-controlled-proxy-config=Rozszerzenie „{ $name }” <img data-l10n-name="icon"/> kontroluje, jak przeglądarka łączy się z Internetem.
extension-controlled-enable=Aby włączyć rozszerzenie, przejdź do sekcji dodatki <img data-l10n-name="addons-icon"/> w menu <img data-l10n-name="menu-icon"/>.
search-results-header=Wyniki wyszukiwania
search-results-empty-message=
  { PLATFORM() ->
    [windows] Niestety! W opcjach niczego nie odnaleziono dla wyszukiwania „<span data-l10n-name="query"></span>”.
    *[other] Niestety! W preferencjach niczego nie odnaleziono dla wyszukiwania „<span data-l10n-name="query"></span>”.
  }
search-results-help-link=Potrzebujesz pomocy? Odwiedź <a data-l10n-name="url">witrynę wsparcia</a>.

startup-header=Uruchamianie
separate-profile-mode=
  .label=Jednoczesne działanie programu { -brand-short-name } oraz programu Firefox
use-firefox-sync=Podczas jednoczesnego działania wykorzystywane są oddzielne profile. Można wykorzystać synchronizację, aby dzielić dane między nimi.
get-started-not-logged-in=Zaloguj się, aby synchronizować…
get-started-configured=Otwórz ustawienia synchronizacji
always-check-default=
  .label=Sprawdzanie, czy { -brand-short-name } jest domyślną przeglądarką
  .accesskey=e
is-default={ -brand-short-name } jest obecnie domyślną przeglądarką
is-not-default={ -brand-short-name } nie jest obecnie domyślną przeglądarką
set-as-my-default-browser=
  .label=Ustaw jako domyślną…
  .accesskey=U
startup-restore-previous-session=
  .label=Przywracanie poprzedniej sesji
  .accesskey=P
disable-extension=
  .label=Wyłącz rozszerzenie
tabs-group-header=Karty
ctrl-tab-recently-used-order=
  .label=Przełączanie kart za pomocą Ctrl+Tab w kolejności ostatnich wyświetleń
  .accesskey=T
open-new-link-as-tabs=
  .label=Otwieranie odnośników w kartach zamiast w nowych oknach
  .accesskey=O
warn-on-close-multiple-tabs=
  .label=Ostrzeganie przed zamknięciem wielu kart
  .accesskey=a
warn-on-quit-close-multiple-tabs=
  .label=Ostrzeganie przed zamknięciem wielu kart podczas zamykania programu
  .accesskey=O
warn-on-open-many-tabs=
  .label=Ostrzeganie o otwarciu zbyt wielu kart mogących spowolnić program
  .accesskey=m
switch-links-to-new-tabs=
  .label=Przechodzenie do nowych kart otwieranych poprzez odnośniki
  .accesskey=c
show-tabs-in-taskbar=
  .label=Podgląd kart na pasku zadań Windows
  .accesskey=W
browser-containers-enabled=
  .label=Karty z kontekstem.
  .accesskey=K
browser-containers-learn-more=Więcej informacji
browser-containers-settings=
  .label=Dostosuj…
  .accesskey=D
containers-disable-alert-title=Czy zamknąć wszystkie karty z kontekstem?
containers-disable-alert-desc=
  { $tabCount ->
    [one] Jeśli wyłączysz funkcję kart z kontekstem, jedna taka karta zostanie zamknięta. Czy na pewno wyłączyć karty z kontekstem?
    [few] Jeśli wyłączysz funkcję kart z kontekstem, { $tabCount } takie karty zostaną zamknięte. Czy na pewno wyłączyć karty z kontekstem?
    *[many] Jeśli wyłączysz funkcję kart z kontekstem, { $tabCount } takich kart zostanie zamkniętych. Czy na pewno wyłączyć karty z kontekstem?
  }
containers-disable-alert-ok-button=
  { $tabCount ->
    [one] Zamknij kartę z kontekstem
    [few] Zamknij { $tabCount } karty z kontekstem
    *[many] Zamknij { $tabCount } kart z kontekstem
  }
containers-disable-alert-cancel-button=Anuluj
containers-remove-alert-title=Usuwanie kontekstu
containers-remove-alert-msg=
  { $count ->
    [one] Jeśli usuniesz teraz ten kontekst, { $count } karta zostanie zamknięta. Czy na pewno usunąć ten kontekst?
    [few] Jeśli usuniesz teraz ten kontekst, { $count } karty zostaną zamknięte. Czy na pewno usunąć ten kontekst?
    *[many] Jeśli usuniesz teraz ten kontekst, { $count } kart zostanie zamkniętych. Czy na pewno usunąć ten kontekst?
  }
containers-remove-ok-button=Usuń
containers-remove-cancel-button=Nie usuwaj
language-and-appearance-header=Język i wygląd
fonts-and-colors-header=Czcionki i kolory
default-font=Domyślna czcionka:
  .accesskey=D
default-font-size=Rozmiar:
  .accesskey=R
advanced-fonts=
  .label=Zaawansowane…
  .accesskey=s
colors-settings=
  .label=Kolory…
  .accesskey=K
language-header=Język
choose-language-description=Wybierz preferowany język, w jakim mają być wyświetlane strony
choose-button=
  .label=Wybierz…
  .accesskey=e
choose-browser-language-description=Wybierz język używany do wyświetlania interfejsu użytkownika programu (menu, wiadomości, powiadomienia itp.).
manage-browser-languages-button=
  .label=Wybierz alternatywne…
  .accesskey=W
confirm-browser-language-change-description=Uruchom przeglądarkę ponownie, aby zastosować zmiany.
confirm-browser-language-change-button=Uruchom ponownie
translate-web-pages=
  .label=Tłumaczenie stron WWW
  .accesskey=T
translate-attribution=Tłumaczeń dostarcza <img data-l10n-name="logo"/>
translate-exceptions=
  .label=Wyjątki…
  .accesskey=i
check-user-spelling=
  .label=Sprawdzanie pisowni podczas wprowadzania tekstu
  .accesskey=S
files-and-applications-title=Pliki i aplikacje
download-header=Pobieranie
download-save-to=
  .label=Pobierane pliki zapisuj do:
  .accesskey=o
download-choose-folder=
  .label=
    { PLATFORM() ->
      [macos] Wybierz…
      *[other] Przeglądaj…
    }
  .accesskey=
    { PLATFORM() ->
      [macos] y
      *[other] g
    }
download-always-ask-where=
  .label=Pytaj, gdzie zapisać każdy plik
  .accesskey=t
applications-header=Aplikacje
applications-description=Wybierz, jak { -brand-short-name } będzie obsługiwać pobierane z sieci pliki i aplikacje używane podczas przeglądania.
applications-filter=
  .placeholder=Typ pliku lub nazwa aplikacji
applications-type-column=
  .label=Typ zawartości
  .accesskey=T
applications-action-column=
  .label=Czynność
  .accesskey=C
drm-content-header=Treści DRM (Digital Rights Management)
play-drm-content=
  .label=Odtwarzanie treści chronionych przez DRM.
  .accesskey=O
play-drm-content-learn-more=Więcej informacji

update-application-title=Aktualizacje
update-application-description=Utrzymuj program aktualnym dla najlepszej wydajności, stabilności i bezpieczeństwa.
update-application-version=Wersja: { $version }. <a data-l10n-name="learn-more">Informacje o wydaniu</a>.
update-history=
  .label=Wyświetl historię aktualizacji…
  .accesskey=W
update-application-allow-description=Zezwalaj na:
update-application-auto=
  .label=automatyczne instalowanie aktualizacji (zalecane)
  .accesskey=a
update-application-check-choose=
  .label=sprawdzanie dostępności aktualizacji i pytania o ich instalację
  .accesskey=s
update-application-manual=
  .label=niesprawdzanie dostępności aktualizacji (niezalecane)
  .accesskey=n
update-application-use-service=
  .label=Używaj usługi instalowania aktualizacji działającej w tle
  .accesskey=U
update-enable-search-update=
  .label=Automatyczne aktualizacje wyszukiwarek
  .accesskey=z
update-pref-write-failure-title=Błąd zapisu
update-pref-write-failure-message=Nie udało się zapisać ustawienia. Nie można było zapisać pliku: { $path }

performance-title=Wydajność
performance-use-recommended-settings-checkbox=
  .label=Zalecane ustawienia wydajności.
  .accesskey=w
performance-use-recommended-settings-desc=Ustawienia te są specjalnie dostosowane do specyfikacji tego komputera i systemu operacyjnego.
performance-settings-learn-more=Więcej informacji
performance-allow-hw-accel=
  .label=Korzystaj ze sprzętowego przyspieszania, jeśli jest dostępne
  .accesskey=s
performance-limit-content-process-option=Limit liczby procesów treści:
  .accesskey=o
performance-limit-content-process-enabled-desc=Więcej procesów treści może poprawić wydajność przy wielu otwartych kartach, ale zwiększy też zapotrzebowanie na pamięć.
performance-limit-content-process-blocked-desc=Zmiana liczby procesów jest możliwa, jeśli { -brand-short-name } został uruchomiony z obsługą wielu procesów. <a data-l10n-name="learn-more">Jak sprawdzić, czy obsługa wielu procesów jest włączona</a>?
performance-default-content-process-count=
  .label={ $num } (domyślnie)
browsing-title=Przeglądanie
browsing-use-autoscroll=
  .label=Używaj automatycznego przewijania
  .accesskey=y
browsing-use-smooth-scrolling=
  .label=Używaj płynnego przewijania
  .accesskey=n
browsing-use-onscreen-keyboard=
  .label=Wyświetlanie klawiatury ekranowej, gdy zachodzi taka potrzeba
  .accesskey=e
browsing-use-cursor-navigation=
  .label=Zawsze używaj klawiszy kursora do nawigacji po stronach
  .accesskey=g
browsing-search-on-start-typing=
  .label=Rozpoczynaj wyszukiwanie podczas wpisywania tekstu
  .accesskey=R
browsing-cfr-recommendations=
  .label=Polecanie rozszerzeń podczas przeglądania
  .accesskey=P
browsing-cfr-recommendations-learn-more=Więcej informacji
network-settings-title=Sieć
network-proxy-connection-description=Konfiguruj, jak { -brand-short-name } ma się łączyć z Internetem.
network-proxy-connection-learn-more=Więcej informacji
network-proxy-connection-settings=
  .label=Ustawienia…
  .accesskey=U
home-new-windows-tabs-header=Nowe okna i karty
home-new-windows-tabs-description2=Wybierz, co ma być wyświetlane przy otwieraniu strony startowej, nowych okien i kart.
home-homepage-mode-label=Strona startowa i nowe okna:
home-newtabs-mode-label=Nowa karta:
home-restore-defaults=
  .label=Przywróć domyślne
  .accesskey=P
home-mode-choice-default=
  .label=strona startowa programu (domyślnie)
home-mode-choice-custom=
  .label=inne strony
home-mode-choice-blank=
  .label=pusta strona
home-homepage-custom-url=
  .placeholder=Adres URL
use-current-pages=
  .label=
    { $tabCount ->
      [1] Użyj bieżącej strony
      *[other] Użyj bieżących stron
    }
  .accesskey=b
choose-bookmark=
  .label=Użyj zakładki…
  .accesskey=z
search-bar-header=Pasek wyszukiwania
search-bar-hidden=
  .label=Pasek adresu z funkcjami wyszukiwania i nawigacji
search-bar-shown=
  .label=Osobny pasek wyszukiwania
search-engine-default-header=Domyślna wyszukiwarka
search-engine-default-desc=Wybierz domyślną wyszukiwarkę paska adresu i paska wyszukiwania.
search-suggestions-option=
  .label=Podpowiedzi wyszukiwania
  .accesskey=P
search-show-suggestions-url-bar-option=
  .label=Podpowiedzi wyszukiwania w podpowiedziach paska adresu
  .accesskey=e
search-show-suggestions-above-history-option=
  .label=Podpowiedzi wyszukiwania nad historią przeglądania w wynikach paska adresu
search-suggestions-cant-show=Podpowiedzi wyszukiwania nie będą wyświetlane w podpowiedziach paska adresu, ponieważ wyłączono zachowywanie historii przeglądania.
search-one-click-header=Dodatkowe wyszukiwarki
search-one-click-desc=Wybierz dodatkowe wyszukiwarki wyświetlane na dole wyników wyszukiwania w pasku adresu i pasku wyszukiwania.
search-choose-engine-column=
  .label=Nazwa
search-choose-keyword-column=
  .label=Słowo kluczowe
search-restore-default=
  .label=Przywróć domyślne
  .accesskey=d
search-remove-engine=
  .label=Usuń
  .accesskey=U
search-find-more-link=Więcej wyszukiwarek
search-keyword-warning-title=Słowo kluczowe już istnieje
search-keyword-warning-engine=Wybrano słowo kluczowe używane obecnie przez wyszukiwarkę { $name }. Należy wybrać inne.
search-keyword-warning-bookmark=Wybrano słowo kluczowe używane obecnie przez zakładkę. Należy wybrać inne.
containers-back-link=« Powrót
containers-header=Karty z kontekstem
containers-add-button=
  .label=Dodaj kontekst
  .accesskey=D
containers-preferences-button=
  .label=Preferencje
containers-remove-button=
  .label=Usuń
sync-signedout-caption=Zabierz swoją sieć ze sobą
sync-signedout-description=Synchronizuj zakładki, historię, karty, hasła, dodatki i ustawienia między wszystkimi swoimi urządzeniami.
sync-signedout-account-title={ -fxaccount-brand-name }
sync-signedout-account-create=Nie masz konta? Utwórz je…
  .accesskey=U
sync-signedout-account-signin=
  .label=Zaloguj się
  .accesskey=Z
sync-mobile-promo=Firefox na <a data-l10n-name="android-link">Androida</a> <img data-l10n-name="android-icon"/> i <a data-l10n-name="ios-link">iOSa</a> <img data-l10n-name="ios-icon"/> daje możliwość synchronizacji z urządzeniami przenośnymi.
sync-profile-picture=
    .tooltiptext=Zmień obraz przypisany do konta
sync-disconnect=
  .label=Rozłącz…
  .accesskey=R
sync-manage-account=Zarządzaj kontem
  .accesskey=Z
sync-signedin-unverified=Konto { $email } nie zostało zweryfikowane.
sync-signedin-login-failure=Zaloguj się, aby ponownie połączyć konto { $email }
sync-resend-verification=
  .label=Wyślij nową wiadomość weryfikującą
  .accesskey=W
sync-remove-account=
  .label=Usuń konto
  .accesskey=U
sync-sign-in=
  .label=Zaloguj
  .accesskey=o
sync-signedin-settings-header=Ustawienia synchronizacji
sync-signedin-settings-desc=Wybierz, co synchronizować pomiędzy urządzeniami:
sync-engine-bookmarks=
  .label=zakładki
  .accesskey=z
sync-engine-history=
  .label=historia
  .accesskey=h
sync-engine-tabs=
  .label=karty
  .tooltiptext=Lista otwartych stron na wszystkich synchronizowanych urządzeniach
  .accesskey=k
sync-engine-logins=
  .label=dane logowania
  .tooltiptext=Zachowane nazwy użytkownika i hasła
  .accesskey=l
sync-engine-addresses=
  .label=adresy
  .tooltiptext=Zachowane adresy pocztowe (tylko na komputerach)
  .accesskey=a
sync-engine-creditcards=
  .label=dane kart płatniczych
  .tooltiptext=Nazwiska, numery i okresy ważności (tylko na komputerach)
  .accesskey=d
sync-engine-addons=
  .label=dodatki
  .tooltiptext=Rozszerzenia i motywy w wersji na komputery
  .accesskey=d
sync-engine-prefs=
  .label=
    { PLATFORM() ->
      [windows] Opcje
      *[other] Preferencje
    }
  .tooltiptext=Zmienione ustawienia ogólne, prywatności i bezpieczeństwa
  .accesskey=P
sync-device-name-header=Nazwa urządzenia
sync-device-name-change=
  .label=Zmień nazwę urządzenia…
  .accesskey=n
sync-device-name-cancel=
  .label=Anuluj
  .accesskey=A
sync-device-name-save=
  .label=Zachowaj
  .accesskey=Z
sync-mobilepromo-single=Połącz inne urządzenie
sync-mobilepromo-multi=Zarządzaj urządzeniami
sync-tos-link=Warunki korzystania z usługi
sync-fxa-privacy-notice=Uwagi dotyczące prywatności
privacy-header=Prywatność
forms-header=Formularze i hasła
logins-header=Dane logowania i hasła
forms-ask-to-save-logins=
  .label=Pytanie o zachowywanie danych logowania do witryn
  .accesskey=P
forms-exceptions=
  .label=Wyjątki…
  .accesskey=i
forms-saved-logins=
  .label=Zachowane dane logowania…
  .accesskey=d
forms-master-pw-use=
  .label=Hasło główne
  .accesskey=H
forms-master-pw-change=
  .label=Zmień hasło główne…
  .accesskey=Z
history-header=Historia
history-remember-label=Program { -brand-short-name }:
  .accesskey=m
history-remember-option-all=
  .label=będzie zachowywał historię
history-remember-option-never=
  .label=nie będzie zachowywał historii
history-remember-option-custom=
  .label=będzie używał ustawień użytkownika
history-remember-description={ -brand-short-name } zachowa historię przeglądania, wyszukiwania, pobieranych plików i danych formularzy.
history-dontremember-description={ -brand-short-name } będzie używał tych samych ustawień co w trybie prywatnym i nie będzie zachowywał historii podczas przeglądania witryn WWW.
history-private-browsing-permanent=
  .label=Zawsze aktywny tryb przeglądania prywatnego
  .accesskey=t
history-remember-browser-option=
  .label=Historia przeglądania i pobierania plików
  .accesskey=H
history-remember-search-option=
  .label=Historia pola wyszukiwania i formularzy
  .accesskey=H
history-clear-on-close-option=
  .label=Czyszczenie historii podczas zamykania programu
  .accesskey=z
history-clear-on-close-settings=
  .label=Ustawienia…
  .accesskey=U
history-clear-button=
  .label=Wyczyść historię…
  .accesskey=W

sitedata-header=Ciasteczka i dane stron
sitedata-total-size-calculating=Obliczanie rozmiaru danych i pamięci podręcznej stron…
sitedata-total-size=Przechowywane ciasteczka, dane i pamięć podręczna stron zajmują { $value } { $unit } na dysku.
sitedata-learn-more=Więcej informacji
sitedata-keep-until=Przechowywanie ciasteczek:
  .accesskey=P
sitedata-keep-until-expire=
  .label=do ich wygaśnięcia
sitedata-keep-until-closed=
  .label=do zamknięcia programu
sitedata-delete-on-close=
  .label=Usuwanie ciasteczek i danych stron podczas zamykania programu
  .accesskey=U
sitedata-allow-cookies-option=
  .label=Akceptowanie ciasteczek i danych stron
  .accesskey=A
sitedata-disallow-cookies-option=
  .label=Blokowanie ciasteczek i danych stron
  .accesskey=B
sitedata-block-desc=Blokowanie:
  .accesskey=B
sitedata-block-trackers-option-recommended=
  .label=elementów śledzących zewnętrznych witryn (zalecane)
sitedata-block-trackers-option=
  .label=elementów śledzących
sitedata-block-unvisited-option=
  .label=ciasteczek nieodwiedzonych witryn
sitedata-block-all-third-party-option=
  .label=wszystkich ciasteczek zewnętrznych witryn (może powodować problemy)
sitedata-block-all-option=
  .label=wszystkich ciasteczek (powoduje problemy)
sitedata-clear=
  .label=Wyczyść dane
  .accesskey=a
sitedata-settings=
  .label=Zachowane dane…
  .accesskey=c
sitedata-cookies-exceptions=
  .label=Wyjątki…
  .accesskey=W
sitedata-cookies-permissions=
  .label=Wyjątki…
  .accesskey=W
sitedata-warning-your-settings-prevent-changes=Ustawienia blokowania treści uniemożliwiają zmiany w ustawieniach przechowywania ciasteczek i danych stron.
addressbar-header=Pasek adresu
addressbar-suggest=Podpowiedzi w pasku adresu opieraj na:
addressbar-locbar-history-option=
  .label=historii przeglądania
  .accesskey=h
addressbar-locbar-bookmarks-option=
  .label=zakładkach
  .accesskey=z
addressbar-locbar-openpage-option=
  .label=otwartych kartach
  .accesskey=k
addressbar-suggestions-settings=Zmień ustawienia podpowiedzi dostarczanych przez wyszukiwarki

content-blocking-header=Blokowanie treści
content-blocking-desc=Blokowanie elementów zewnętrznych witryn (jak reklamy i skrypty) mogących spowolnić przeglądanie i śledzić aktywność w sieci. Ustawienia poniżej ułatwiają optymalne dostrojenie poziomu ochrony i szybkości wczytywania witryn.
content-blocking-description=Blokowanie elementów zewnętrznych witryn, które śledzą aktywność użytkowników sieci. Zdecyduj, jak wiele informacji o aktywności ma być przechowywane i współdzielone przez witryny.
content-blocking-learn-more=Więcej informacji
content-blocking-setting-standard=
  .label=Standardowe
  .accesskey=S
content-blocking-setting-strict=
  .label=Ścisłe
  .accesskey=e
content-blocking-setting-custom=
  .label=Własne
  .accesskey=W
content-blocking-standard-desc=Wyważone pod kątem ochrony i szybkości wczytywania stron. Dopuszcza pewne elementy śledzące, aby strony działały prawidłowo.
content-blocking-strict-desc=Blokowanie wszystkich wykrytych elementów śledzących. Może powodować problemy ze stronami.
content-blocking-custom-desc=Wybierz, co blokować:
content-blocking-private-trackers=Blokowanie znanych elementów śledzących w oknach prywatnych.
content-blocking-third-party-cookies=Blokowanie ciasteczek zewnętrznych witryn używanych do śledzenia.
content-blocking-all-windows-trackers=Blokowanie znanych elementów śledzących we wszystkich oknach.
content-blocking-all-third-party-cookies=Blokowanie wszystkich ciasteczek zewnętrznych witryn.
content-blocking-warning-title=Ostrzeżenie
content-blocking-warning-desc=Blokowanie ciasteczek i elementów śledzących może powodować problemy ze stronami. Blokowanie można łatwo wyłączyć na stronach, którym ufasz.
content-blocking-learn-how=Więcej informacji
content-blocking-restore-defaults=
  .label=Przywróć domyślne
  .accesskey=P
content-blocking-toggle-on=
  .tooltiptext=Wyłącz blokowanie treści
content-blocking-toggle-off=
  .tooltiptext=Włącz blokowanie treści
content-blocking-toggle-label-on=Włączone
  .accesskey=W
content-blocking-toggle-label-off=Wyłączone
  .accesskey=W
content-blocking-category-label=Blokowane elementy
content-blocking-fastblock-slow-loading-trackers-label=
  .label=Wolno wczytywane elementy śledzące
  .accesskey=W
content-blocking-fastblock-new-description=Blokowanie elementów śledzących uniemożliwiających szybkie wczytywanie stron.
content-blocking-tracking-protection-trackers-label=
  .label=Elementy śledzące
  .accesskey=E
content-blocking-tracking-protection-all-detected-trackers-label=
  .label=Wszystkie wykryte elementy śledzące
  .accesskey=W
content-blocking-tracking-protection-new-description=Blokowanie wszystkich znanych elementów śledzących (niektóre strony mogą nie działać prawidłowo).
content-blocking-tracking-protection-option-always=
  .label=Obligatoryjne blokowanie
  .accesskey=e
content-blocking-tracking-protection-option-all-windows=
  .label=Obligatoryjne blokowanie
  .accesskey=O
content-blocking-tracking-protection-option-private=
  .label=Blokowanie w oknach prywatnych
  .accesskey=B
content-blocking-tracking-protection-change-block-list=Zmień listę blokowanych
content-blocking-cookies-label=
  .label=ciasteczka
  .accesskey=c
content-blocking-third-party-cookies-label=
  .label=Ciasteczka zewnętrznych witryn
  .accesskey=C
content-blocking-reject-trackers-description=Blokowanie wszystkich ciasteczek zewnętrznych witryn lub tylko ustawianych przez elementy śledzące.
content-blocking-reject-trackers-warning-your-settings-prevent-changes=Ustawienia przechowywania ciasteczek i danych stron uniemożliwiają zmiany w ustawieniach ciasteczek zewnętrznych witryn.
content-blocking-change-cookie-settings=
  .label=Ustawienia dotyczące ciasteczek i danych stron
  .accesskey=Z
content-blocking-reject-trackers-block-trackers-option-recommended=
  .label=Elementy śledzące (zalecane)
  .accesskey=E
content-blocking-reject-trackers-block-trackers-option=
  .label=Elementy śledzące
  .accesskey=E
content-blocking-reject-trackers-all-third-parties-option=
  .label=Wszystkie ciasteczka zewnętrznych witryn (może powodować problemy)
  .accesskey=W
tracking-header=Ochrona przed śledzeniem
tracking-desc=Ochrona przed śledzeniem blokuje elementy, które zbierają informacje o przeglądaniu na wielu różnych stronach. <a data-l10n-name="learn-more">Więcej informacji o ochronie przed śledzeniem i prywatności</a>.
tracking-mode-label=Chroń przed śledzeniem przez znane elementy śledzące:
tracking-mode-always=
  .label=zawsze
  .accesskey=z
tracking-mode-private=
  .label=w oknach prywatnych
  .accesskey=w
tracking-mode-never=
  .label=nigdy
  .accesskey=n
tracking-exceptions=
  .label=Wyjątki…
  .accesskey=W
tracking-manage-exceptions=
  .label=Wyjątki…
  .accesskey=W
tracking-change-block-list=
  .label=Zmień listę blokowanych…
  .accesskey=Z
permissions-header=Uprawnienia
permissions-location=Położenie
permissions-location-settings=
  .label=Ustawienia…
  .accesskey=t
permissions-camera=Kamera
permissions-camera-settings=
  .label=Ustawienia…
  .accesskey=a
permissions-microphone=Mikrofon
permissions-microphone-settings=
  .label=Ustawienia…
  .accesskey=w
permissions-notification=Powiadomienia.
permissions-notification-settings=
  .label=Ustawienia…
  .accesskey=s
permissions-notification-link=Więcej informacji
permissions-notification-pause=
  .label=Wstrzymaj powiadomienia do czasu ponownego uruchomienia programu
  .accesskey=W
permissions-block-autoplay-media=
  .label=Blokowanie automatycznego odtwarzania treści przez strony
  .accesskey=B
permissions-block-autoplay-media-menu=Gdy strony chcą automatycznie odtwarzać treści:
autoplay-option-ask=
  .label=pytaj
autoplay-option-allow=
  .label=odtwarzaj
autoplay-option-dont=
  .label=nie odtwarzaj
permissions-autoplay-link=Więcej informacji 
permissions-block-autoplay-media-exceptions=
  .label=Wyjątki…
  .accesskey=W
permissions-block-popups=
  .label=Blokowanie wyskakujących okien
  .accesskey=B
permissions-block-popups-exceptions=
  .label=Wyjątki…
  .accesskey=t
permissions-addon-install-warning=
  .label=Ostrzeganie, gdy witryny próbują instalować dodatki
  .accesskey=O
permissions-addon-exceptions=
  .label=Wyjątki…
  .accesskey=W
permissions-a11y-privacy-checkbox=
  .label=Blokowanie dostępu do przeglądarki usługom ułatwień dostępu.
  .accesskey=u
permissions-a11y-privacy-link=Więcej informacji
collection-header=Dane zbierane przez program { -brand-short-name }
collection-description=Dążymy do zapewnienia odpowiedniego wyboru i zbierania wyłącznie niezbędnych danych, aby dostarczać i doskonalić program { -brand-short-name } dla nas wszystkich. Zawsze prosimy o pozwolenie przed przesłaniem danych osobistych.
collection-privacy-notice=Uwagi dotyczące prywatności
collection-health-report=
  .label=Przesyłanie do Mozilli danych technicznych i o interakcjach z programem.
  .accesskey=z
collection-health-report-link=Więcej informacji
collection-studies=
  .label=Instalowanie i przeprowadzanie badań.
collection-studies-link=Wyświetl badania
collection-health-report-disabled=Przesyłanie danych jest wyłączone przy tej konfiguracji programu
collection-browser-errors=
  .label=Przesyłanie raportów z błędów przeglądarki (razem z treścią błędów) do Mozilli.
  .accesskey=z
collection-browser-errors-link=Więcej informacji
collection-backlogged-crash-reports=
  .label=Przesyłanie zgromadzonych raportów z awarii.
  .accesskey=o
collection-backlogged-crash-reports-link=Więcej informacji
security-header=Bezpieczeństwo
security-browsing-protection=Ochrona przed oszustwami i niebezpiecznym oprogramowaniem
security-enable-safe-browsing=
  .label=Blokowanie niebezpiecznych i podejrzanych treści.
  .accesskey=B
security-enable-safe-browsing-link=Więcej informacji
security-block-downloads=
  .label=Blokowanie możliwości pobierania niebezpiecznych plików
  .accesskey=e
security-block-uncommon-software=
  .label=Ostrzeganie przed niepożądanym i nietypowym oprogramowaniem
  .accesskey=n
certs-header=Certyfikaty
certs-personal-label=Kiedy serwer żąda osobistego certyfikatu użytkownika:
certs-select-auto-option=
  .label=wybierz certyfikat automatycznie
  .accesskey=a
certs-select-ask-option=
  .label=pytaj za każdym razem
  .accesskey=r
certs-enable-ocsp=
  .label=Odpytywanie serwerów OCSP w celu potwierdzenia wiarygodności certyfikatów
  .accesskey=O
certs-view=
  .label=Wyświetl certyfikaty…
  .accesskey=W
certs-devices=
  .label=Urządzenia zabezpieczające…
  .accesskey=U
space-alert-learn-more-button=
  .label=Więcej informacji
  .accesskey=W
space-alert-over-5gb-pref-button=
  .label=
    { PLATFORM() ->
      [windows] Otwórz opcje
      *[other] Otwórz preferencje
    }
  .accesskey=
    { PLATFORM() ->
      [windows] O
      *[other] O
    }
space-alert-over-5gb-message=
  { PLATFORM() ->
    [windows] Zaczyna brakować miejsca na dysku. Zawartość stron może być wyświetlana niepoprawnie. Przechowywane dane może wyczyścić w Opcje → Prywatność i bezpieczeństwo → Ciasteczka i dane stron.
    *[other] Zaczyna brakować miejsca na dysku. Zawartość stron może być wyświetlana niepoprawnie. Przechowywane dane może wyczyścić w Preferencje → Prywatność i bezpieczeństwo → Ciasteczka i dane stron.
  }
space-alert-under-5gb-ok-button=
  .label=OK
  .accesskey=O
space-alert-under-5gb-message=Zaczyna brakować miejsca na dysku. Zawartość stron może być wyświetlana niepoprawnie. Skorzystaj z odnośnika „Więcej informacji”, aby zoptymalizować użycie dysku dla lepszego przeglądania.
desktop-folder-name=Pulpit
downloads-folder-name=Pobrane
choose-download-folder-title=Wybór folderu dla pobieranych plików

save-files-to-cloud-storage=
  .label=Wysyłanie plików do usługi „{ $service-name }”
