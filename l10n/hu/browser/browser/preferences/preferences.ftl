# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = „Do Not Track” jelzés küldése a webhelyeknek, jelezve, hogy nem szeretné, hogy kövessék
do-not-track-learn-more = További információk
do-not-track-option-default =
    .label = Csak, ha követésvédelmet használ
do-not-track-option-always =
    .label = Mindig
pref-page =
    .title =
        { PLATFORM() ->
            [windows] Beállítások
           *[other] Beállítások
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
            [windows] Keresés a beállításokban
           *[other] Keresés a beállításokban
        }
policies-notice =
    { PLATFORM() ->
        [windows] A szervezete letiltotta egyes beállítások módosítását.
       *[other] A szervezete letiltotta egyes beállítások módosítását.
    }
pane-general-title = Általános
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = Kezdőlap
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = Keresés
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = Adatvédelem és biztonság
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Firefox fiók
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = { -brand-short-name } támogatás
focus-search =
    .key = f
close-button =
    .aria-label = Bezárás

## Browser Restart Dialog

feature-enable-requires-restart = A funkció bekapcsolásához a { -brand-short-name } újraindítása szükséges.
feature-disable-requires-restart = A funkció kikapcsolásához a { -brand-short-name } újraindítása szükséges.
should-restart-title = { -brand-short-name } újraindítása
should-restart-ok = { -brand-short-name } újraindítása most
cancel-no-restart-button = Mégse
restart-later = Újraindítás később

## Extension Control Notifications
##
## These strings are used to inform the user
## about changes made by extensions to browser settings.
##
## <img data-l10n-name="icon"/> is going to be replaced by the extension icon.
##
## Variables:
##   $name (String): name of the extension

# This string is shown to notify the user that their home page
# is being controlled by an extension.
extension-controlled-homepage-override = Egy kiterjesztés, a(z) <img data-l10n-name="icon"/> { $name }, vezérli a kezdőoldalt.
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = Egy kiterjesztés, a(z) <img data-l10n-name="icon"/> { $name }, vezérli az Új lap oldalt.
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = Egy kiegészítő, a(z) <img data-l10n-name="icon"/> { $name }, beállította az alapértelmezett keresőszolgáltatást.
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = A(z) <img data-l10n-name="icon"/> { $name } kiterjesztéshez szükségesek a konténerlapok.
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = A(z) <img data-l10n-name="icon"/> { $name } kiegészítő vezérli a követés elleni védelmet.
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = A(z) <img data-l10n-name="icon"/> { $name } kiegészítő vezérli, hogy a { -brand-short-name } hogy kapcsolódik az internethez
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = A kiegészítő engedélyezéséhez ugorjon a <img data-l10n-name="addons-icon"/> Kiegészítőkhöz a <img data-l10n-name="menu-icon"/> menüben.

## Preferences UI Search Results

search-results-header = Találatok
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] Elnézését, nincs találat a Beállítások közt erre: „<span data-l10n-name="query"></span>”.
       *[other] Elnézését, nincs találat a Beállítások közt erre: „<span data-l10n-name="query"></span>”.
    }
search-results-help-link = Segítségre van szüksége? Látogasson el ide: <a data-l10n-name="url">{ -brand-short-name } támogatás</a>

## General Section

startup-header = Indítás
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = A { -brand-short-name } és a Firefox futhat egyszerre
use-firefox-sync = Tipp: Ez külön profilokat használ. A { -sync-brand-short-name } segítségével adatokat oszthat meg közöttük.
get-started-not-logged-in = Bejelentkezés a { -sync-brand-short-name }be
get-started-configured = A { -sync-brand-short-name } beállításainak megnyitása
always-check-default =
    .label = Mindig ellenőrizze, hogy a { -brand-short-name }-e az alapértelmezett böngésző
    .accesskey = M
is-default = Jelenleg a { -brand-short-name } az alapértelmezett böngésző.
is-not-default = A { -brand-short-name } nem az alapértelmezett böngésző
set-as-my-default-browser =
    .label = Beállítás alapértelmezettként…
    .accesskey = a
startup-page = A { -brand-short-name } indításakor
    .accesskey = s
startup-user-homepage =
    .label = Kezdőlap megjelenítése
startup-blank-page =
    .label = Üres oldal megjelenítése
startup-prev-session =
    .label = A legutóbbi ablakok és lapok megjelenítése
startup-restore-previous-session =
    .label = Előző munkamenet helyreállítása
    .accesskey = h
disable-extension =
    .label = Kiterjesztés letiltása
home-page-header = Kezdőlap
tabs-group-header = Lapok
ctrl-tab-recently-used-order =
    .label = A Ctrl+Tab a legutóbbi használat sorrendjében lépked körbe a lapokon
    .accesskey = T
open-new-link-as-tabs =
    .label = Hivatkozások megnyitása új lapon, az új ablak helyett
    .accesskey = l
warn-on-close-multiple-tabs =
    .label = Figyelmeztetés több lap bezárása előtt
    .accesskey = t
warn-on-open-many-tabs =
    .label = Figyelmeztetés, hogy több lap megnyitása lelassíthatja a { -brand-short-name } programot
    .accesskey = F
switch-links-to-new-tabs =
    .label = Hivatkozás új lapon való megnyitásakor átváltás rá azonnal
    .accesskey = H
show-tabs-in-taskbar =
    .label = Lapok előnézetének megjelenítése a Windows tálcán
    .accesskey = L
browser-containers-enabled =
    .label = Konténer lapok engedélyezése
    .accesskey = n
browser-containers-learn-more = További tudnivalók
browser-containers-settings =
    .label = Beállítások…
    .accesskey = B
containers-disable-alert-title = Az összes konténerlap bezárása?
containers-disable-alert-desc =
    { $tabCount ->
        [one] Ha most letiltja a konténerlapokat, akkor { $tabCount } konténerlap bezáródik. Biztosan letiltja a konténerlapokat?
       *[other] Ha most letiltja a konténerlapokat, akkor { $tabCount } konténerlap bezáródik. Biztosan letiltja a konténerlapokat?
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] { $tabCount } konténerlap bezárása
       *[other] { $tabCount } konténerlap bezárása
    }
containers-disable-alert-cancel-button = Maradjon engedélyezve
containers-remove-alert-title = Eltávolítja ezt a konténert?
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg =
    { $count ->
        [one] Ha most eltávolítja ezt a konténerlapot, akkor { $count } konténerlap bezáródik. Biztosan eltávolítja ezt a konténert?
       *[other] Ha most eltávolítja ezt a konténerlapot, akkor { $count } konténerlap bezáródik. Biztosan eltávolítja ezt a konténert?
    }
containers-remove-ok-button = Konténer eltávolítása
containers-remove-cancel-button = Ne távolítsa el a konténert

## General Section - Language & Appearance

language-and-appearance-header = Nyelv és megjelenés
fonts-and-colors-header = Betűk és színek
default-font = Alapértelmezett betűkészlet
    .accesskey = A
default-font-size = Méret
    .accesskey = M
advanced-fonts =
    .label = Speciális…
    .accesskey = c
colors-settings =
    .label = Színek…
    .accesskey = z
language-header = Nyelv
choose-language-description = Az oldalak megjelenítésére előnyben részesített nyelv megadása
choose-button =
    .label = Tallózás…
    .accesskey = T
translate-web-pages =
    .label = Webtartalom fordítása
    .accesskey = f
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = Fordítás: <img data-l10n-name="logo"/>
translate-exceptions =
    .label = Kivételek…
    .accesskey = K
check-user-spelling =
    .label = Helyesírás-ellenőrzés beírás közben
    .accesskey = H

## General Section - Files and Applications

files-and-applications-title = Fájlok és alkalmazások
download-header = Letöltések
download-save-to =
    .label = Fájlok mentése
    .accesskey = m
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] Tallózás…
           *[other] Tallózás…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] T
           *[other] T
        }
download-always-ask-where =
    .label = Mindig kérdezzen rá a fájlok letöltési helyére
    .accesskey = r
applications-header = Alkalmazások
applications-description = Válassza ki, hogy a { -brand-short-name } hogyan kezelje az internetről letöltött fájlokat vagy a böngészéskor használt alkalmazásokat.
applications-filter =
    .placeholder = Fájltípusok vagy alkalmazások keresése
applications-type-column =
    .label = Tartalomtípus
    .accesskey = T
applications-action-column =
    .label = Művelet
    .accesskey = M
drm-content-header = Digitális jogkezelést (DRM) használó tartalom
play-drm-content =
    .label = DRM-vezérelt tartalom lejátszása
    .accesskey = l
play-drm-content-learn-more = További tudnivalók
update-application-title = { -brand-short-name } frissítések
update-application-description = Tartsa naprakészen a { -brand-short-name }ot a legjobb teljesítmény, stabilitás és biztonság érdekében.
update-application-info = Verzió{ $version } <a>Újdonságok</a>
update-application-version = Verzió{ $version } <a data-l10n-name="learn-more">Újdonságok</a>
update-history =
    .label = Frissítési előzmények megjelenítése…
    .accesskey = z
update-application-allow-description = A következők engedélyezése a { -brand-short-name }nak:
update-application-auto =
    .label = Frissítések automatikus telepítése (ajánlott)
    .accesskey = A
update-application-check-choose =
    .label = Frissítések keresése, de a telepítés jóváhagyással történik
    .accesskey = k
update-application-manual =
    .label = Ne legyen frissítve (nem ajánlott)
    .accesskey = N
update-application-use-service =
    .label = Háttérben futó szolgáltatás intézze a frissítést
    .accesskey = H
update-enable-search-update =
    .label = Keresőszolgáltatások automatikus frissítése
    .accesskey = u

## General Section - Performance

performance-title = Teljesítmény
performance-use-recommended-settings-checkbox =
    .label = Javasolt teljesítménybeállítások használata
    .accesskey = h
performance-use-recommended-settings-desc = Ezek a beállítások a számítógép hardveréhez és operációs rendszeréhez lettek szabva.
performance-settings-learn-more = További tudnivalók
performance-allow-hw-accel =
    .label = Hardveres gyorsítás használata, ha lehetséges
    .accesskey = r
performance-limit-content-process-option = Tartalom folyamatok korlátja
    .accesskey = k
performance-limit-content-process-enabled-desc = A további tartalom folyamatok növelhetik a teljesítményt, ha több lapot használ, de több memóriát is használnak.
performance-limit-content-process-disabled-desc = A tartalom folyamatok számának módosítása csak többfolyamatos { -brand-short-name } esetén lehetséges. <a>Ismerje meg, hogyan lehet ellenőrizni, hogy a többfolyamatos működés engedélyezve van-e</a>
performance-limit-content-process-blocked-desc = A tartalom folyamatok számának módosítása csak többfolyamatos { -brand-short-name } esetén lehetséges. <a data-l10n-name="learn-more">Ismerje meg, hogyan lehet ellenőrizni, hogy a többfolyamatos működés engedélyezve van-e</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (alapértelmezett)

## General Section - Browsing

browsing-title = Böngészés
browsing-use-autoscroll =
    .label = Automatikus görgetés
    .accesskey = u
browsing-use-smooth-scrolling =
    .label = Finom görgetés
    .accesskey = F
browsing-use-onscreen-keyboard =
    .label = Érintőbillentyűzet megjelenítése, ha szükséges
    .accesskey = r
browsing-use-cursor-navigation =
    .label = Kurzorbillentyűk használata az oldalon belüli navigációhoz
    .accesskey = c
browsing-search-on-start-typing =
    .label = Szöveg keresése a keresett szó beírásának elkezdésétől
    .accesskey = d

## General Section - Proxy

network-proxy-title = Hálózati proxy
network-proxy-connection-description = Állítsa be hogy a { -brand-short-name } hogyan kapcsolódik az internethez.
network-proxy-connection-learn-more = További tudnivalók
network-proxy-connection-settings =
    .label = Beállítások…
    .accesskey = B

## Home Section

home-new-windows-tabs-header = Új ablakok és lapok
home-new-windows-tabs-description2 = Válasszon hogy mit lásson, ha megnyitja a kezdőoldalt, vagy egy új ablakot, lapot.

## Home Section - Home Page Customization

home-homepage-mode-label = Kezdőlap és új ablakok
home-newtabs-mode-label = Új lapok
home-restore-defaults =
    .label = Alapértelmezések visszaállítása
    .accesskey = A
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Firefox kezdőlap (alapértelmezett)
home-mode-choice-custom =
    .label = Egyéni URL-ek…
home-mode-choice-blank =
    .label = Üres lap
home-homepage-custom-url =
    .placeholder = Illesszen be egy URL-t…
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] Aktuális oldal használata
           *[other] Aktuális oldalak használata
        }
    .accesskey = A
choose-bookmark =
    .label = Könyvjelző használata…
    .accesskey = n
restore-default =
    .label = Alapértelmezés
    .accesskey = p

## Search Section

search-bar-header = Keresősáv
search-bar-hidden =
    .label = Használja a címsávot a kereséshez és a navigációhoz
search-bar-shown =
    .label = Keresősáv hozzáadása az eszköztárhoz
search-engine-default-header = Alapértelmezett keresőszolgáltatás
search-engine-default-desc = Válassza ki a címsávban és a keresősávban alapértelmezetten használandó keresőszolgáltatást.
search-suggestions-option =
    .label = Keresési javaslatok
    .accesskey = K
search-show-suggestions-url-bar-option =
    .label = Keresési javaslatok megjelenítése a címsáv találataiban
    .accesskey = K
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = Keresési javaslatok megjelenítése a böngészési előzmények előtt a címsor találatai között
search-suggestions-cant-show = A keresési javaslatok nem jelennek meg a címsáv találatai között, mert a { -brand-short-name } nem jegyzi meg az előzményeket.
search-one-click-header = Egy kattintásos keresőszolgáltatások
search-one-click-desc = Válassza ki a címsáv alatt és a keresősávban gépeléskor megjelenő alternatív keresőszolgáltatatásokat.
search-choose-engine-column =
    .label = Keresőszolgáltatás
search-choose-keyword-column =
    .label = Kulcsszó
search-restore-default =
    .label = Alapértelmezett keresőszolgáltatások visszaállítása
    .accesskey = v
search-remove-engine =
    .label = Eltávolítás
    .accesskey = E
search-find-more-link = További keresőszolgáltatások felvétele
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = Duplikált kulcsszó
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = Olyan kulcsszót választott, amelyet jelenleg „{ $name }” használ. Válasszon másikat.
search-keyword-warning-bookmark = Olyan kulcsszót választott, amelyet jelenleg egy könyvjelző használ. Válasszon másikat.

## Containers Section

containers-back-link = « Vissza
containers-header = Konténer lapok
containers-add-button =
    .label = Új konténer hozzáadása
    .accesskey = a
containers-preferences-button =
    .label = Beállítások
containers-remove-button =
    .label = Eltávolítás

## Sync Section - Signed out

sync-signedout-caption = Vigye magával a webet
sync-signedout-description = Szinkronizálja könyvjelzőit, előzményeit, lapjait, jelszavait, kiegészítőit és beállításait minden eszközén.
sync-signedout-account-title = Kapcsolódjon egy { -fxaccount-brand-name }kal
sync-signedout-account-create = Nincs felhasználóneve? Kezdjen neki
    .accesskey = K
sync-signedout-account-signin =
    .label = Bejelentkezés…
    .accesskey = B
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Töltse le a Firefox for <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> vagy <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a> appot a mobileszközével való szinkronizáláshoz.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = Profilkép módosítása
sync-disconnect =
    .label = A kapcsolat bontása…
    .accesskey = t
sync-manage-account = Fiók kezelése
    .accesskey = F
sync-signedin-unverified = A(z)  { $email } cím nincs ellenőrizve.
sync-signedin-login-failure = Jelentkezzen be  { $email } újracsatlakoztatásához
sync-resend-verification =
    .label = Ellenőrző e-mail újraküldése
    .accesskey = k
sync-remove-account =
    .label = Fiók eltávolítása
    .accesskey = t
sync-sign-in =
    .label = Bejelentkezés
    .accesskey = B
sync-signedin-settings-header = Sync beállítások
sync-signedin-settings-desc = Válassza ki, mit szeretne szinkronizálni eszközei között a { -brand-short-name } segítségével.
sync-engine-bookmarks =
    .label = Könyvjelzők
    .accesskey = K
sync-engine-history =
    .label = Előzmények
    .accesskey = E
sync-engine-tabs =
    .label = Nyitott lapok
    .tooltiptext = Lista arról, hogy mi van nyitva a szinkronizált eszközökön
    .accesskey = L
sync-engine-logins =
    .label = Bejelentkezések
    .tooltiptext = A mentett felhasználónevek és jelszavak
    .accesskey = B
sync-engine-addresses =
    .label = Címek
    .tooltiptext = Mentett postai címek (csak asztali gépen)
    .accesskey = e
sync-engine-creditcards =
    .label = Bankkártyák
    .tooltiptext = Nevek, számok és lejárati dátumok (csak asztali gépen)
    .accesskey = B
sync-engine-addons =
    .label = Kiegészítők
    .tooltiptext = Kiterjesztések és témák az asztali Firefoxhoz
    .accesskey = K
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] Beállítások
           *[other] Beállítások
        }
    .tooltiptext = Módosított általános, adatvédelmi és biztonsági beállítások
    .accesskey = B
sync-device-name-header = Eszköznév
sync-device-name-change =
    .label = Eszköznév módosítása…
    .accesskey = m
sync-device-name-cancel =
    .label = Mégse
    .accesskey = g
sync-device-name-save =
    .label = Mentés
    .accesskey = M
sync-mobilepromo-single = Másik eszköz csatlakoztatása
sync-mobilepromo-multi = Eszközök kezelése
sync-tos-link = A szolgáltatás feltételei
sync-fxa-privacy-notice = Adatvédelmi megjegyzés

## Privacy Section

privacy-header = Böngésző adatvédelme

## Privacy Section - Forms

forms-header = Űrlapok és jelszavak
forms-ask-to-save-logins =
    .label = Felhasználónevek és jelszavak megjegyzésének megkérdezése az oldalakhoz
    .accesskey = k
forms-exceptions =
    .label = Kivételek…
    .accesskey = v
forms-saved-logins =
    .label = Mentett bejelentkezések…
    .accesskey = t
forms-master-pw-use =
    .label = Mesterjelszó használata
    .accesskey = M
forms-master-pw-change =
    .label = Mesterjelszó megváltoztatása…
    .accesskey = z

## Privacy Section - History

history-header = Előzmények
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = A { -brand-short-name }
    .accesskey = A
history-remember-option-all =
    .label = Megjegyzi az előzményeket
history-remember-option-never =
    .label = Nem jegyzi meg az előzményeket
history-remember-option-custom =
    .label = Egyéni beállításokat használ az előzményekhez
history-remember-description = A { -brand-short-name } emlékezni fog a böngészési, letöltési, űrlap és keresési előzményekre.
history-dontremember-description = A { -brand-short-name } ugyanazokat a beállításokat fogja használni, mint a privát böngészés, és nem fogja megjegyezni az internethasználat előzményeit.
history-private-browsing-permanent =
    .label = Mindig a privát böngészési módot használja
    .accesskey = p
history-remember-option =
    .label = Böngészési és letöltési előzmények megőrzése
    .accesskey = l
history-remember-browser-option =
    .label = Böngészési és letöltési előzmények megőrzése
    .accesskey = b
history-remember-search-option =
    .label = Keresőmezők és űrlapmezők előzményeinek megőrzése
    .accesskey = K
history-clear-on-close-option =
    .label = Előzmények törlése a { -brand-short-name } bezárásakor
    .accesskey = E
history-clear-on-close-settings =
    .label = Beállítások…
    .accesskey = B
history-clear-button =
    .label = Előzmények törlése…
    .accesskey = l

## Privacy Section - Site Data

sitedata-header = Sütik és oldaladatok
sitedata-total-size-calculating = Az oldaladatok és a gyorsítótár méretének kiszámítása…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = A tárolt sütik, oldaladatok és a gyorsítótár jelenleg { $value } { $unit } területet foglal el a lemezen.
sitedata-learn-more = További tudnivalók
sitedata-accept-cookies-option =
    .label = Sütik és oldaladatok elfogadása a webhelyektől (ajánlott)
    .accesskey = e
sitedata-block-cookies-option =
    .label = Sütik és oldaladatok blokkolása (a weboldalak hibáját okozhatja)
    .accesskey = b
sitedata-keep-until = Sütik megtartása
    .accesskey = m
sitedata-keep-until-expire =
    .label = Lejárnak
sitedata-keep-until-closed =
    .label = A { -brand-short-name } zárolva van
sitedata-accept-third-party-desc = Harmadik féltől származó sütik és oldaladatok elfogadása
    .accesskey = f
sitedata-accept-third-party-always-option =
    .label = Mindig
sitedata-accept-third-party-visited-option =
    .label = Meglátogatottól
sitedata-accept-third-party-never-option =
    .label = Soha
sitedata-clear =
    .label = Adatok törlése…
    .accesskey = t
sitedata-settings =
    .label = Adatok kezelése…
    .accesskey = A
sitedata-cookies-exceptions =
    .label = Kivételek…
    .accesskey = K

## Privacy Section - Address Bar

addressbar-header = Címsáv
addressbar-suggest = A címsáv használatakor jelenjen meg
addressbar-locbar-history-option =
    .label = Böngészési előzmények
    .accesskey = e
addressbar-locbar-bookmarks-option =
    .label = Könyvjelzők
    .accesskey = K
addressbar-locbar-openpage-option =
    .label = Nyitott lapok
    .accesskey = N
addressbar-suggestions-settings = Keresőszolgáltatás-javaslatok beállításainak módosítása

## Privacy Section - Tracking

tracking-header = Követés elleni védelem
tracking-desc = A követés elleni védelem blokkolja az online nyomkövetőket, melyek a webhelyek közti böngészési adatait gyűjtik. <a data-l10n-name="learn-more">Tudjon meg többet a követés elleni védelemről és az adatvédelemről</a>
tracking-mode-label = Követés elleni védelem használata az ismert követők blokkolására
tracking-mode-always =
    .label = Mindig
    .accesskey = d
tracking-mode-private =
    .label = Csak privát ablakokban
    .accesskey = C
tracking-mode-never =
    .label = Soha
    .accesskey = o
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = Követésvédelem használata privát böngészésben, az ismert követők blokkolására
    .accesskey = v
tracking-exceptions =
    .label = Kivételek…
    .accesskey = K
tracking-change-block-list =
    .label = Blokkolási lista módosítása…
    .accesskey = m

## Privacy Section - Permissions

permissions-header = Engedélyek
permissions-location = Hely
permissions-location-settings =
    .label = Beállítások…
    .accesskey = H
permissions-camera = Kamera
permissions-camera-settings =
    .label = Beállítások…
    .accesskey = K
permissions-microphone = Mikrofon
permissions-microphone-settings =
    .label = Beállítások…
    .accesskey = m
permissions-notification = Értesítések
permissions-notification-settings =
    .label = Beállítások…
    .accesskey = e
permissions-notification-link = További tudnivalók
permissions-notification-pause =
    .label = Értesítések kikapcsolása a { -brand-short-name } újraindulásáig
    .accesskey = e
permissions-block-autoplay-media =
    .label = Webhelyek blokkolása, hogy automatikusan ne játszhassanak le médiát hanggal.
    .accesskey = b
permissions-block-autoplay-media-exceptions =
    .label = Kivételek…
    .accesskey = K
permissions-block-popups =
    .label = Felugró ablakok tiltása
    .accesskey = F
permissions-block-popups-exceptions =
    .label = Kivételek…
    .accesskey = K
permissions-addon-install-warning =
    .label = Figyelmeztetés kiegészítők telepítése előtt
    .accesskey = F
permissions-addon-exceptions =
    .label = Kivételek…
    .accesskey = K
permissions-a11y-privacy-checkbox =
    .label = Az akadálymentesítési szolgáltatások ne férjenek hozzá a böngészőhöz
    .accesskey = a
permissions-a11y-privacy-link = További tudnivalók

## Privacy Section - Data Collection

collection-header = { -brand-short-name } adatgyűjtés és felhasználás
collection-description = Arra törekszünk, hogy választást biztosítsunk, és csak azt gyűjtsük, amire szükségünk a van a { -brand-short-name } fejlesztéséhez, mindenki számára. Mindig engedélyt kérünk, mielőtt személyes információkat fogadunk.
collection-privacy-notice = Adatvédelmi nyilatkozat
collection-health-report =
    .label = Engedélyezés, hogy a { -brand-short-name } műszaki és interakciós adatokat küldjön a Mozillának
    .accesskey = E
collection-health-report-link = További tudnivalók
collection-studies =
    .label = Engedélyezés, hogy a { -brand-short-name } tanulmányokat telepítsen és futasson
collection-studies-link = { -brand-short-name } tanulmányok megtekintése
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = Az adatjelentést letiltották ehhez a binárishoz
collection-browser-errors =
    .label = Engedélyezi a { -brand-short-name } számára, hogy böngésző hibajelentéseket (köztük hibaüzeneteket) küldjön a { -vendor-short-name } számára
    .accesskey = b
collection-browser-errors-link = További tudnivalók
collection-backlogged-crash-reports =
    .label = A { -brand-short-name } a háttérben küldhet összeomlási jelentéseket az Ön nevében
    .accesskey = j
collection-backlogged-crash-reports-link = További tudnivalók

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = Biztonság
security-browsing-protection = Félrevezető tartalom és veszélyes szoftver elleni védelem
security-enable-safe-browsing =
    .label = Veszélyes és félrevezető tartalom blokkolása
    .accesskey = V
security-enable-safe-browsing-link = További tudnivalók
security-block-downloads =
    .label = Veszélyes letöltések blokkolása
    .accesskey = b
security-block-uncommon-software =
    .label = Figyelmeztetés a nem kívánatos és szokatlan szoftverekre
    .accesskey = F

## Privacy Section - Certificates

certs-header = Tanúsítványok
certs-personal-label = Ha a kiszolgáló elkéri a személyes tanúsítványt
certs-select-auto-option =
    .label = Automatikus választás
    .accesskey = A
certs-select-ask-option =
    .label = Megerősítés minden alkalommal
    .accesskey = M
certs-enable-ocsp =
    .label = Az OCSP válaszoló kiszolgálók lekérdezése a tanúsítványok érvényességének megerősítéséhez
    .accesskey = C
certs-view =
    .label = Tanúsítványok megtekintése…
    .accesskey = T
certs-devices =
    .label = Adatvédelmi eszközök…
    .accesskey = e
