# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

about-logins-page-title = Bejelentkezések és jelszavak

# "Google Play" and "App Store" are both branding and should not be translated

login-app-promo-title = Vigye magával a jelszavait bárhová
login-app-promo-subtitle = Szerezze be az ingyenes { -lockwise-brand-name } alkalmazást
login-app-promo-android =
    .alt = Szerezze be a Google Play-en
login-app-promo-apple =
    .alt = Töltse le az App Store-ból
login-filter =
    .placeholder = Bejelentkezések keresése
create-login-button = Új bejelentkezés létrehozása
# This string is used as alternative text for favicon images.
# Variables:
#   $title (String) - The title of the website associated with the favicon.
login-favicon =
    .alt = Favicon a következőhöz: { $title }
fxaccounts-sign-in-text = Érje el jelszavait a többi eszközén is
fxaccounts-sign-in-button = Jelentkezzen be a { -sync-brand-short-name }be
fxaccounts-avatar-button =
    .title = Fiók kezelése

## The ⋯ menu that is in the top corner of the page

menu =
    .title = Menü megnyitása
# This menuitem is only visible on Windows
menu-menuitem-import = Jelszavak importálása…
menu-menuitem-preferences =
    { PLATFORM() ->
        [windows] Beállítások
       *[other] Beállítások
    }
menu-menuitem-feedback = Visszajelzés küldése
menu-menuitem-faq = Gyakran Ismételt Kérdések
menu-menuitem-android-app = { -lockwise-brand-short-name } Androidra
menu-menuitem-iphone-app = { -lockwise-brand-short-name } iPhone és iPad készülékekhez

## Login List

login-list =
    .aria-label = A keresésnek megfelelő bejelentkezések
login-list-count =
    { $count ->
        [one] { $count } bejelentkezés
       *[other] { $count } bejelentkezés
    }
login-list-sort-label-text = Rendezés:
login-list-name-option = Név (A-Z)
login-list-breached-option = Feltört weboldalak
login-list-last-changed-option = Legutóbbi módosítás
login-list-last-used-option = Legutóbbi használat
login-list-intro-title = Nincsenek bejelentkezések
login-list-intro-description = Ha elment egy jelszót a { -brand-product-name }ban, akkor az itt fog megjelenni.
login-list-item-title-new-login = Új bejelentkezés
login-list-item-subtitle-new-login = Adja meg a bejelentkezési adatait
login-list-item-subtitle-missing-username = (nincs felhasználónév)

## Introduction screen

login-intro-heading = A mentett bejelentkezéseit keresi? Állítsa be a { -sync-brand-short-name }-t.
login-intro-description = Ha egy másik eszközön mentette a bejelentkezéseit a { -brand-product-name }ban, akkor így érheti el őket itt:
login-intro-instruction-fxa = Hozzon létre egyet, vagy jelentkezzen be a { -fxaccount-brand-name }jába azon az eszközön, amelyen a bejelentkezéseit menti
login-intro-instruction-fxa-settings = Győződjön meg róla, hogy bejelölte a Bejelentkezések választómezőt a { -sync-brand-short-name } beállításokban
login-intro-instruction-faq = További segítségért keresse fel a { -lockwise-brand-short-name }-zal kapcsolatos <a data-l10n-name="faq">gyakran ismétlődő kérdéseket</a>

## Login

login-item-new-login-title = Új bejelentkezés létrehozása
login-item-edit-button = Szerkesztés
login-item-delete-button = Törlés
login-item-origin-label = Honlap címe
login-item-origin =
    .placeholder = https://www.example.com
login-item-open-site-button = Indítás
login-item-username-label = Felhasználónév
login-item-username =
    .placeholder = email@example.com
login-item-copy-username-button-text = Másolás
login-item-copied-username-button-text = Másolva!
login-item-password-label = Jelszó
login-item-password-reveal-checkbox-show =
    .title = Jelszó megjelenítése
login-item-password-reveal-checkbox-hide =
    .title = Jelszó elrejtése
login-item-copy-password-button-text = Másolás
login-item-copied-password-button-text = Másolva!
login-item-save-changes-button = Változások mentése
login-item-save-new-button = Mentés
login-item-cancel-button = Mégse
login-item-time-changed = Legutóbb módosítva: { DATETIME($timeChanged, day: "numeric", month: "long", year: "numeric") }
login-item-time-created = Létrehozva: { DATETIME($timeCreated, day: "numeric", month: "long", year: "numeric") }
login-item-time-used = Legutóbb használva: { DATETIME($timeUsed, day: "numeric", month: "long", year: "numeric") }

## Master Password notification

master-password-notification-message = Adja meg a mesterjelszavát a mentett bejelentkezések és jelszavak megtekintéséhez
master-password-reload-button =
    .label = Bejelentkezés
    .accesskey = B

## Dialogs

confirmation-dialog-cancel-button = Mégse
confirmation-dialog-dismiss-button =
    .title = Mégse
enable-password-sync-notification-message =
    { PLATFORM() ->
        [windows] Mindenhol szeretne hozzáférni a bejelentkezéseihez, ahol a { -brand-product-name }ot használja? Ugorjon a { -sync-brand-short-name } beállításokhoz és jelölje be a Bejelentkezések választógombot.
       *[other] Mindenhol szeretne hozzáférni a bejelentkezéseihez, ahol a { -brand-product-name }ot használja? Ugorjon a { -sync-brand-short-name } beállításokhoz és jelölje be a Bejelentkezések választógombot.
    }
enable-password-sync-preferences-button =
    .label =
        { PLATFORM() ->
            [windows] { -sync-brand-short-name } beállítások megtekintése
           *[other] { -sync-brand-short-name } beállítások megtekintése
        }
    .accesskey = m
confirm-delete-dialog-title = Törli a bejelentkezést?
confirm-delete-dialog-message = Ez a művelet nem vonható vissza.
confirm-delete-dialog-confirm-button = Törlés
confirm-discard-changes-dialog-title = Elveti a mentetlen módosításokat?
confirm-discard-changes-dialog-message = Minden nem mentett változás elvész.
confirm-discard-changes-dialog-confirm-button = Elvetés

## Breach Alert notification

breach-alert-text = A jelszavai kiszivárogtak vagy ellopták őket a weboldalról a bejelentkezési adatai legutóbbi frissítése óta. A fiókja védelme érdekében cserélje le jelszavát.
breach-alert-link = Tudjon meg többet erről az adatsértésről.
breach-alert-dismiss =
    .title = Figyelmeztetés bezárása
