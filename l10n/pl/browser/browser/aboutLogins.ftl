# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

about-logins-page-title = Dane logowania i hasła

# "Google Play" and "App Store" are both branding and should not be translated

login-app-promo-title = Miej hasła zawsze przy sobie
login-app-promo-subtitle = Pobierz darmową aplikację { -lockwise-brand-name }
login-app-promo-android =
    .alt = Pobierz z Google Play
login-app-promo-apple =
    .alt = Pobierz z App Store
login-filter =
    .placeholder = Szukaj danych logowania
create-login-button = Utwórz nowe dane logowania
# This string is used as alternative text for favicon images.
# Variables:
#   $title (String) - The title of the website associated with the favicon.
login-favicon =
    .alt = Ikona witryny „{ $title }”
fxaccounts-sign-in-text = Korzystaj ze swoich haseł na innych urządzeniach
fxaccounts-sign-in-button = Zaloguj się do { -sync-brand-short-name(case: "gen", capitalization: "lower") }
fxaccounts-avatar-button =
    .title = Zarządzaj kontem

## The ⋯ menu that is in the top corner of the page

menu =
    .title = Otwórz menu
# This menuitem is only visible on Windows
menu-menuitem-import = Importuj hasła…
menu-menuitem-preferences =
    { PLATFORM() ->
        [windows] Opcje
       *[other] Preferencje
    }
menu-menuitem-feedback = Wyślij opinię
menu-menuitem-faq = Najczęściej zadawane pytania
menu-menuitem-android-app = { -lockwise-brand-short-name } na Androida
menu-menuitem-iphone-app = { -lockwise-brand-short-name } na iPhone’a i iPada

## Login List

login-list =
    .aria-label = Dane logowania pasujące do zapytania
login-list-count =
    { $count ->
        [one] { $count } dane logowania
        [few] { $count } dane logowania
       *[many] { $count } danych logowania
    }
login-list-sort-label-text = Sortuj wg:
login-list-name-option = Nazwa (A-Z)
login-list-breached-option = Witryny z wyciekami haseł
login-list-last-changed-option = Ostatnia modyfikacja
login-list-last-used-option = Ostatnie użycie
login-list-intro-title = Brak danych logowania
login-list-intro-description = Tutaj będą wyświetlane hasła zachowane w przeglądarce { -brand-product-name }.
login-list-item-title-new-login = Nowe dane logowania
login-list-item-subtitle-new-login = Wprowadź dane logowania
login-list-item-subtitle-missing-username = (bez nazwy użytkownika)

## Introduction screen

login-intro-heading = Szukasz swoich zachowanych haseł? Skonfiguruj { -sync-brand-short-name(case: "acc", capitalization: "lower") }.
login-intro-description = Aby przenieść tutaj dane logowania zachowane w przeglądarce { -brand-product-name } na innym urządzeniu:
login-intro-instruction-fxa = Zarejestruj się lub zaloguj na { -fxaccount-brand-name(case: "loc", capitalization: "lower") } na urządzeniu, w którym zachowano dane logowania
login-intro-instruction-fxa-settings = Upewnij się, że opcja „Dane logowania” jest zaznaczona w ustawieniach { -sync-brand-short-name(case: "gen", capitalization: "lower") }
login-intro-instruction-faq = <a data-l10n-name="faq">Najczęściej zadawane pytania</a> usługi { -lockwise-brand-short-name } zawierają więcej informacji

## Login

login-item-new-login-title = Nowe dane logowania
login-item-edit-button = Edytuj
login-item-delete-button = Usuń
login-item-origin-label = Adres witryny
login-item-origin =
    .placeholder = https://www.example.com
login-item-open-site-button = Otwórz
login-item-username-label = Nazwa użytkownika
login-item-username =
    .placeholder = e-mail@example.com
login-item-copy-username-button-text = Kopiuj
login-item-copied-username-button-text = Skopiowano
login-item-password-label = Hasło
login-item-password-reveal-checkbox-show =
    .title = Wyświetl hasło
login-item-password-reveal-checkbox-hide =
    .title = Ukryj hasło
login-item-copy-password-button-text = Kopiuj
login-item-copied-password-button-text = Skopiowano
login-item-save-changes-button = Zapisz zmiany
login-item-save-new-button = Zapisz
login-item-cancel-button = Anuluj
login-item-time-changed = Ostatnia modyfikacja: { DATETIME($timeChanged, day: "numeric", month: "long", year: "numeric") }
login-item-time-created = Utworzono: { DATETIME($timeCreated, day: "numeric", month: "long", year: "numeric") }
login-item-time-used = Ostatnie użycie: { DATETIME($timeUsed, day: "numeric", month: "long", year: "numeric") }

## Master Password notification

master-password-notification-message = Proszę podać hasło główne, aby wyświetlić zachowane dane logowania i hasła
master-password-reload-button =
    .label = Zaloguj się
    .accesskey = Z

## Dialogs

confirmation-dialog-cancel-button = Anuluj
confirmation-dialog-dismiss-button =
    .title = Anuluj
enable-password-sync-notification-message =
    { PLATFORM() ->
        [windows] Chcesz mieć swoje hasła wszędzie, gdzie korzystasz z przeglądarki { -brand-product-name }? Przejdź do opcji { -sync-brand-short-name(case: "gen", capitalization: "lower") } i zaznacz opcję „Dane logowania”.
       *[other] Chcesz mieć swoje hasła wszędzie, gdzie korzystasz z przeglądarki { -brand-product-name }? Przejdź do preferencji { -sync-brand-short-name(case: "gen", capitalization: "lower") } i zaznacz opcję „Dane logowania”.
    }
enable-password-sync-preferences-button =
    .label =
        { PLATFORM() ->
            [windows] Otwórz opcje { -sync-brand-short-name(case: "gen", capitalization: "lower") }
           *[other] Otwórz preferencje { -sync-brand-short-name(case: "gen", capitalization: "lower") }
        }
    .accesskey = O
confirm-delete-dialog-title = Usunąć te dane logowania?
confirm-delete-dialog-message = Tej czynności nie można cofnąć.
confirm-delete-dialog-confirm-button = Usuń
confirm-discard-changes-dialog-title = Odrzucić niezachowane zmiany?
confirm-discard-changes-dialog-message = Wszystkie niezachowane zmiany zostaną utracone.
confirm-discard-changes-dialog-confirm-button = Odrzuć

## Breach Alert notification

breach-alert-text = Od czasu ostatniej zmiany danych logowania hasła z tej witryny wyciekły lub zostały wykradzione. Zmień hasło, aby ochronić swoje konto.
breach-alert-link = Więcej informacji o tym wycieku.
breach-alert-dismiss =
    .title = Zamknij to powiadomienie
