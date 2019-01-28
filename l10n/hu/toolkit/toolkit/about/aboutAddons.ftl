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
    .value = Néhány kiterjesztés letiltásra került
disabled-unsigned-description = A következő kiegészítők nem lettek ellenőrizve a { -brand-short-name } böngészőben való használatra. Lehetősége van <label data-l10n-name="find-addons">helyettesítőket keresni</label> vagy megkérni a fejlesztőt az ellenőriztetésre.
disabled-unsigned-learn-more = Tudjon meg többet erőfeszítéseinkről az online biztonsága fenntartásáért.
disabled-unsigned-devinfo = A kiegészítőik ellenőriztetése iránt érdeklődő fejlesztők folytathatják a <label data-l10n-name="learn-more">kézikönyv</label> elolvasásával.
plugin-deprecation-description = Hiányzik valami? Néhány bővítményt már nem támogat a { -brand-short-name }. <label data-l10n-name="learn-more">További tudnivalók.</label>
legacy-warning-show-legacy = Hagyományos kiegészítők megjelenítése
legacy-extensions =
    .value = Hagyományos kiegészítők
legacy-extensions-description = Ezek a kiegészítők nem felelnek meg a { -brand-short-name } aktuális elvárásainak, emiatt le lettek tiltva. <label data-l10n-name="legacy-learn-more">További tudnivalók a kiegészítők módosulásáról</label>
extensions-view-discover =
    .name = Kiegészítők letöltése
    .tooltiptext = { extensions-view-discover.name }
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
