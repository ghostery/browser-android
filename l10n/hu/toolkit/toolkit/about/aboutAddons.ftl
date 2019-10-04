# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

addons-window =
    .title = Kiegészítőkezelő
search-header =
    .placeholder = Keresés itt: addons.mozilla.org
    .searchbuttonlabel = Keresés
search-header-shortcut =
    .key = f
loading-label =
    .value = Betöltés…
list-empty-installed =
    .value = Nincs ilyen típusú kiegészítője
list-empty-available-updates =
    .value = Nem találhatók frissítések
list-empty-recent-updates =
    .value = Mostanában nem frissített kiegészítőket
list-empty-find-updates =
    .label = Frissítések keresése
list-empty-button =
    .label = További tudnivalók a kiegészítőkről
install-addon-from-file =
    .label = Kiegészítő telepítése fájlból…
    .accesskey = f
help-button = Kiegészítő támogatás
preferences =
    { PLATFORM() ->
        [windows] { -brand-short-name } beállítások
       *[other] { -brand-short-name } beállítások
    }
tools-menu =
    .tooltiptext = Eszközök minden kiegészítőhöz
show-unsigned-extensions-button =
    .label = Néhány kiegészítő nem ellenőrizhető
show-all-extensions-button =
    .label = Minden kiegészítő megjelenítése
debug-addons =
    .label = Kiegészítők hibakeresése
    .accesskey = h
cmd-show-details =
    .label = További adatok megjelenítése
    .accesskey = T
cmd-find-updates =
    .label = Frissítések keresése
    .accesskey = F
cmd-preferences =
    .label =
        { PLATFORM() ->
            [windows] Beállítások
           *[other] Beállítások
        }
    .accesskey =
        { PLATFORM() ->
            [windows] B
           *[other] B
        }
cmd-enable-theme =
    .label = Téma alkalmazása
    .accesskey = T
cmd-disable-theme =
    .label = Téma levétele
    .accesskey = l
cmd-install-addon =
    .label = Telepítés
    .accesskey = T
cmd-contribute =
    .label = Támogatás
    .accesskey = T
    .tooltiptext = A kiegészítő fejlesztésének támogatása
discover-title = Mik azok a kiegészítők?
discover-description = A kiegészítők olyan alkalmazások, amelyekkel személyre szabható a { -brand-short-name } akár többletfunkciókkal, akár eltérő kinézettel. Próbáljon ki egy oldalsávot, amellyel gyorsabb a munka, nézze meg, milyen az időjárás, vagy öltöztesse a { -brand-short-name } programot olyan ruhába, amilyen tetszik.
discover-footer = Ha csatlakozva van az internetre, ezen a panelen megjelennek a legjobb és legnépszerűbb kiegészítők, amelyeket kipróbálhat.
detail-version =
    .label = Verzió
detail-last-updated =
    .label = Utoljára frissítve
detail-contributions-description = A kiegészítő fejlesztője azt kéri, hogy egy csekély összeggel támogassa a további fejlesztést.
detail-contributions-button = Közreműködés
    .title = Közreműködés ezen kiegészítő fejlesztésében
    .accesskey = K
detail-update-type =
    .value = Automatikus frissítések
detail-update-default =
    .label = Alapértelmezett
    .tooltiptext = Frissítések automatikus telepítése csak akkor, ha ez az alapbeállítás
detail-update-automatic =
    .label = Be
    .tooltiptext = Frissítések automatikus telepítése
detail-update-manual =
    .label = Ki
    .tooltiptext = Ne legyenek automatikus frissítések
# Used as a description for the option to allow or block an add-on in private windows.
detail-private-browsing-label = Futtatás privát ablakokban
detail-private-browsing-description2 = Ha engedélyezve van, akkor a kiegészítő privát böngészésben is hozzá fog férni az online tevékenységéhez. <label data-l10n-name="detail-private-browsing-learn-more">Tudjon meg többet</label>
# Some add-ons may elect to not run in private windows by setting incognito: not_allowed in the manifest.  This
# cannot be overridden by the user.
detail-private-disallowed-label = Privát ablakokban nem engedélyezett
detail-private-disallowed-description = Ez a kiegészítő nem fut privát böngészéskor. <label data-l10n-name="detail-private-browsing-learn-more">További információk</label>
# Some special add-ons are privileged, run in private windows automatically, and this permission can't be revoked
detail-private-required-label = Hozzáférés szükséges a privát ablakokhoz
detail-private-required-description = Ez a kiegészítő hozzáfér az online tevékenységéhez privát böngészés közben. <label data-l10n-name="detail-private-browsing-learn-more">További információ</label>
detail-private-browsing-on =
    .label = Engedélyezés
    .tooltiptext = Engedélyezés privát böngészésben
detail-private-browsing-off =
    .label = Tiltás
    .tooltiptext = Tiltás privát böngészésben
detail-home =
    .label = Honlap
detail-home-value =
    .value = { detail-home.label }
detail-repository =
    .label = Kiegészítő profilja
detail-repository-value =
    .value = { detail-repository.label }
detail-check-for-updates =
    .label = Frissítések keresése
    .accesskey = F
    .tooltiptext = Frissítések keresése ehhez a kiegészítőhöz
detail-show-preferences =
    .label =
        { PLATFORM() ->
            [windows] Beállítások
           *[other] Beállítások
        }
    .accesskey =
        { PLATFORM() ->
            [windows] B
           *[other] B
        }
    .tooltiptext =
        { PLATFORM() ->
            [windows] A kiegészítő beállításainak módosítása
           *[other] A kiegészítő beállításainak módosítása
        }
detail-rating =
    .value = Értékelés
addon-restart-now =
    .label = Újraindítás most
disabled-unsigned-heading =
    .value = Néhány kiegészítő letiltásra került
disabled-unsigned-description = A következő kiegészítők nem lettek ellenőrizve a { -brand-short-name } böngészőben való használatra. Lehetősége van <label data-l10n-name="find-addons">helyettesítőket keresni</label> vagy megkérni a fejlesztőt az ellenőriztetésre.
disabled-unsigned-learn-more = Tudjon meg többet erőfeszítéseinkről az online biztonsága fenntartásáért.
disabled-unsigned-devinfo = A kiegészítőik ellenőriztetése iránt érdeklődő fejlesztők folytathatják a <label data-l10n-name="learn-more">kézikönyv</label> elolvasásával.
plugin-deprecation-description = Hiányzik valami? Néhány bővítményt már nem támogat a { -brand-short-name }. <label data-l10n-name="learn-more">További tudnivalók.</label>
legacy-warning-show-legacy = Hagyományos kiegészítők megjelenítése
legacy-extensions =
    .value = Hagyományos kiegészítők
legacy-extensions-description = Ezek a kiegészítők nem felelnek meg a { -brand-short-name } aktuális elvárásainak, emiatt le lettek tiltva. <label data-l10n-name="legacy-learn-more">További tudnivalók a kiegészítők módosulásáról</label>
private-browsing-description2 =
    A { -brand-short-name } megváltoztatja a kiegészítők működését privát böngészésben. A { -brand-short-name }hoz
    hozzáadott új kiegészítők alapértelmezetten nem futnak privát böngészésben. Ha nem engedélyezi a beállításokban,
    akkor a kiegészítő nem fog működni privát böngészésben, és ott nem fog hozzáférni az online tevékenységéhez.
    Ezt a változtatást azért hoztuk, hogy a privát böngészése tényleg privát legyen.
    <label data-l10n-name="private-browsing-learn-more">Tudjon meg többet a kiegészítőbeállítások kezeléséről.</label>
extensions-view-discopane =
    .name = Javaslatok
    .tooltiptext = { extensions-view-discopane.name }
extensions-view-recent-updates =
    .name = Legutóbbi frissítések
    .tooltiptext = { extensions-view-recent-updates.name }
extensions-view-available-updates =
    .name = Elérhető frissítések
    .tooltiptext = { extensions-view-available-updates.name }

## These are global warnings

extensions-warning-safe-mode-label =
    .value = Minden kiegészítő tiltva van a csökkentett mód miatt.
extensions-warning-safe-mode-container =
    .tooltiptext = { extensions-warning-safe-mode-label.value }
extensions-warning-check-compatibility-label =
    .value = A kiegészítők kompatibilitásának vizsgálata tiltva van. Előfordulhat, hogy nem kompatibilis kiegészítői vannak.
extensions-warning-check-compatibility-container =
    .tooltiptext = { extensions-warning-check-compatibility-label.value }
extensions-warning-check-compatibility-enable =
    .label = Engedélyezés
    .tooltiptext = Kiegészítők kompatibilitási ellenőrzésének engedélyezése
extensions-warning-update-security-label =
    .value = A kiegészítők biztonsági vizsgálata tiltva van. A frissítések biztonsági kockázatot hordoznak.
extensions-warning-update-security-container =
    .tooltiptext = { extensions-warning-update-security-label.value }
extensions-warning-update-security-enable =
    .label = Engedélyezés
    .tooltiptext = Kiegészítők frissítésekor a biztonsági ellenőrzés engedélyezése

## Strings connected to add-on updates

extensions-updates-check-for-updates =
    .label = Frissítések keresése
    .accesskey = F
extensions-updates-view-updates =
    .label = A legutóbbi frissítések megtekintése
    .accesskey = A

# This menu item is a checkbox that toggles the default global behavior for
# add-on update checking.

extensions-updates-update-addons-automatically =
    .label = Kiegészítők automatikus frissítése
    .accesskey = K

## Specific add-ons can have custom update checking behaviors ("Manually",
## "Automatically", "Use default global behavior"). These menu items reset the
## update checking behavior for all add-ons to the default global behavior
## (which itself is either "Automatically" or "Manually", controlled by the
## extensions-updates-update-addons-automatically.label menu item).

extensions-updates-reset-updates-to-automatic =
    .label = Minden kiegészítő automatikus frissítése
    .accesskey = M
extensions-updates-reset-updates-to-manual =
    .label = Minden kiegészítő kézi frissítése
    .accesskey = k

## Status messages displayed when updating add-ons

extensions-updates-updating =
    .value = Kiegészítők frissítése
extensions-updates-installed =
    .value = A kiegészítői frissítve lettek.
extensions-updates-downloaded =
    .value = A kiegészítőfrissítések letöltődtek.
extensions-updates-restart =
    .label = A telepítés befejezéséhez újra kell indítani
extensions-updates-none-found =
    .value = Nem találhatók frissítések
extensions-updates-manual-updates-found =
    .label = Az elérhető frissítések megtekintése
extensions-updates-update-selected =
    .label = Frissítések telepítése
    .tooltiptext = A listán szereplő frissítések telepítése

## Extension shortcut management

manage-extensions-shortcuts =
    .label = Kiegészítő-gyorsbillentyűk kezelése
    .accesskey = o
shortcuts-no-addons = Egyetlen kiegészítő sincs engedélyezve.
shortcuts-no-commands = A következő kiegészítők nem rendelkeznek gyorsbillentyűvel:
shortcuts-input =
    .placeholder = Írjon be egy gyorsbillentyűt
shortcuts-browserAction = Kiegészítő aktiválása
shortcuts-pageAction = Lapművelet aktiválása
shortcuts-sidebarAction = Oldalsáv be/ki
shortcuts-modifier-mac = Ctrl, Alt vagy ⌘ gombot tartalmaz
shortcuts-modifier-other = Ctrl vagy Alt gombot tartalmaz
shortcuts-invalid = Érvénytelen kombináció
shortcuts-letter = Írjon be egy betűt
shortcuts-system = Nem írhat felül egy { -brand-short-name } gyorsbillentyűt
# String displayed in warning label when there is a duplicate shortcut
shortcuts-duplicate = Ismétlődő parancsikon
# String displayed when a keyboard shortcut is already assigned to more than one add-on
# Variables:
#   $shortcut (string) - Shortcut string for the add-on
shortcuts-duplicate-warning-message = A { $shortcut } gyorsbillentyű több mint egy esetben van használva. Az ismétlődő gyorsbillentyűk váratlan viselkedést okozhatnak.
# String displayed when a keyboard shortcut is already used by another add-on
# Variables:
#   $addon (string) - Name of the add-on
shortcuts-exists = Már használja: { $addon }
shortcuts-card-expand-button =
    { $numberToShow ->
        [one] Még { $numberToShow } megjelenítése
       *[other] Még { $numberToShow } megjelenítése
    }
shortcuts-card-collapse-button = Kevesebb megjelenítése
go-back-button =
    .tooltiptext = Ugrás vissza

## Recommended add-ons page

# Explanatory introduction to the list of recommended add-ons. The action word
# ("recommends") in the final sentence is a link to external documentation.
discopane-intro =
    A kiegészítők és témák olyanok, mint az alkalmazások a böngészője számára,
    és segítségükkel megvédheti a jelszavait, videókat tölthet le, leárazásokat
    találhat, blokkolhatja a zavaró hirdetéseket, módosíthatja a böngésző
    kinézetét, és még sok mást is tehet. Ezek a kis programokat általában
    harmadik felek készítik. Itt vagy egy válogatás, amelyet a { -brand-product-name }
    a kivételes biztonságuk, teljesítményük és funkcionalitásuk miatt 
    <a data-l10n-name="learn-more-trigger">javasol</a>.
# Notice to make user aware that the recommendations are personalized.
discopane-notice-recommendations =
    Ezen javaslatok egy része személyre szabott. Ennek alapja a telepített kiegészítői,
    a profilbeállításai és a használati statisztikái.
discopane-notice-learn-more = További tudnivalók
privacy-policy = Adatvédelmi irányelvek
# Refers to the author of an add-on, shown below the name of the add-on.
# Variables:
#   $author (string) - The name of the add-on developer.
created-by-author = szerző: <a data-l10n-name="author">{ $author }</a>
# Shows the number of daily users of the add-on.
# Variables:
#   $dailyUsers (number) - The number of daily users.
user-count = Felhasználók: { $dailyUsers }
install-extension-button = Hozzáadás a { -brand-product-name }hoz
install-theme-button = Téma telepítése
# The label of the button that appears after installing an add-on. Upon click,
# the detailed add-on view is opened, from where the add-on can be managed.
manage-addon-button = Kezelés
find-more-addons = Több kiegészítő keresése

## Add-on actions

report-addon-button = Jelentés
remove-addon-button = Eltávolítás
disable-addon-button = Letiltás
enable-addon-button = Engedélyezés
expand-addon-button = További beállítások
preferences-addon-button =
    { PLATFORM() ->
        [windows] Beállítások
       *[other] Beállítások
    }
details-addon-button = Részletek
release-notes-addon-button = Kiadási megjegyzések
permissions-addon-button = Engedélyek
addons-enabled-heading = Engedélyezve
addons-disabled-heading = Tiltva
extension-enabled-heading = Engedélyezve
extension-disabled-heading = Tiltva
theme-enabled-heading = Engedélyezve
theme-disabled-heading = Tiltva
plugin-enabled-heading = Engedélyezve
plugin-disabled-heading = Tiltva
dictionary-enabled-heading = Engedélyezve
dictionary-disabled-heading = Tiltva
locale-enabled-heading = Engedélyezve
locale-disabled-heading = Tiltva
ask-to-activate-button = Aktiválás kérésre
always-activate-button = Mindig aktiválja
never-activate-button = Soha ne aktiválja
addon-detail-author-label = Szerző
addon-detail-version-label = Verzió
addon-detail-last-updated-label = Utoljára frissítve
addon-detail-homepage-label = Honlap
addon-detail-rating-label = Értékelés
# The average rating that the add-on has received.
# Variables:
#   $rating (number) - A number between 0 and 5. The translation should show at most one digit after the comma.
five-star-rating =
    .title = Értékelés: { NUMBER($rating, maximumFractionDigits: 1) } az 5-ből
# This string is used to show that an add-on is disabled.
# Variables:
#   $name (string) - The name of the add-on
addon-name-disabled = { $name } (letiltva)
# The number of reviews that an add-on has received on AMO.
# Variables:
#   $numberOfReviews (number) - The number of reviews received
addon-detail-reviews-link =
    { $numberOfReviews ->
        [one] { $numberOfReviews } értékelés
       *[other] { $numberOfReviews } értékelés
    }

## Pending uninstall message bar

# Variables:
#   $addon (string) - Name of the add-on
pending-uninstall-description = <span data-l10n-name="addon-name">{ $addon }</span> has been removed.
pending-uninstall-undo-button = Visszavonás
addon-detail-updates-label = Automatikus frissítések engedélyezése
addon-detail-updates-radio-default = Alapértelmezett
addon-detail-updates-radio-on = Be
addon-detail-updates-radio-off = Ki
addon-detail-update-check-label = Frissítések keresése
install-update-button = Frissítés
# This is the tooltip text for the private browsing badge in about:addons. The
# badge is the private browsing icon included next to the extension's name.
addon-badge-private-browsing-allowed =
    .title = Engedélyezett privát ablakokban
addon-detail-private-browsing-help = Ha engedélyezve van, a kiegészítő hozzá fog férni az online tevékenységekhez privát böngészés közben. <a data-l10n-name="learn-more">További tudnivalók</a>
addon-detail-private-browsing-allow = Engedélyezés
addon-detail-private-browsing-disallow = Tiltás
# This is the tooltip text for the recommended badge for an extension in about:addons. The
# badge is a small icon displayed next to an extension when it is recommended on AMO.
addon-badge-recommended =
    .title = Ajánlott
    .alt = Ajánlott
available-updates-heading = Elérhető frissítések
recent-updates-heading = Legutóbbi frissítések
release-notes-loading = Betöltés…
release-notes-error = Sajnáljuk, de hiba történt a kiadási megjegyzések betöltésekor.
addon-permissions-empty = Ez a kiegészítő nem igényel semmilyen engedélyt
recommended-extensions-heading = Ajánlott kiegészítők
recommended-themes-heading = Ajánlott témák
# A recommendation for the Firefox Color theme shown at the bottom of the theme
# list view. The "Firefox Color" name itself should not be translated.
recommended-theme-1 = Kreatívnak érzi magát? <a data-l10n-name="link">Állítsa össze a saját témáját a Firefox Color használatával.</a>
