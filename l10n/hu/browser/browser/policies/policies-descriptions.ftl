# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Firefox installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-AppUpdateURL = Egyéni alkalmazás-frissítési URL megadása.
policy-Authentication = Integrált hitelesítés beállítása azokhoz a weboldalakhoz, melyek támogatják.
policy-BlockAboutAddons = Hozzáférés blokkolása a Kiegészítőkezelőhöz (about:addons).
policy-BlockAboutConfig = Hozzáférés blokkolása az about:config oldalhoz.
policy-BlockAboutProfiles = Hozzáférés blokkolása az about:profiles oldalhoz.
policy-BlockAboutSupport = Hozzáférés blokkolása az about:support oldalhoz.
policy-Bookmarks = Könyvjelzők létrehozása a Könyvjelzők eszköztáron, a Könyvjelző menüben vagy az abban megadott mappában.
policy-Certificates = Használja-e a beépített tanúsítványokat, vagy sem. Ez a házirend jelenleg csak Windowson működik.
policy-CertificatesDescription = Tanúsítványok hozzáadása vagy beépített tanúsítványok használata.
policy-Cookies = A weboldalak süti elhelyezésének engedélyezése vagy letiltása.
policy-DisableAppUpdate = A böngésző frissítésének megakadályozása.
policy-DisableBuiltinPDFViewer = A PDF.js-nek, a { -brand-short-name } beépített PDF-megjelenítőjének letiltása.
policy-DisableDeveloperTools = A fejlesztői eszközökhöz hozzáférés blokkolása.
policy-DisableFeedbackCommands = A visszajelzés küldési parancsok letiltása a Súgó menüben (Visszajelzés beküldése és Félrevezető oldal jelentése).
policy-DisableFirefoxAccounts = A { -fxaccount-brand-name } alapú szolgáltatások letiltása, beleértve a Syncet.
# Firefox Screenshots is the name of the feature, and should not be translated.
policy-DisableFirefoxScreenshots = A Firefox Screenshots funkció letiltása.
policy-DisableFirefoxStudies = Annak a megakadályozása, hogy a { -brand-short-name } tanulmányokat futtasson.
policy-DisableForgetButton = Az Elfelejtés gombhoz hozzáférés megakadályozása.
policy-DisableFormHistory = Ne jegyezze meg a keresőmezők és űrlapmezők előzményeit.
policy-DisableMasterPasswordCreation = Ha igaz, akkor nem hozható létre mesterjelszó.
policy-DisablePocket = A weboldalak Pocketbe mentését biztosító funkció letiltása.
policy-DisablePrivateBrowsing = Privát böngészés letiltása.
policy-DisableProfileImport = A más böngészőből történő adatimportálás parancsok letiltása.
policy-DisableProfileRefresh = A { -brand-short-name } felfrissítése gomb letiltása az about:support oldalon.
policy-DisableSafeMode = Az Újraindítás letiltott kiegészítőkkel funkció letiltása. Megjegyzés: a kiegészítők nélküli indításhoz használható Shift billentyű csak Windowson tiltható le csoportházirend segítségével.
policy-DisableSecurityBypass = Annak a megakadályozása, hogy a felhasználó átugorjon bizonyos biztonsági figyelmeztetéseket.
policy-DisableSetDesktopBackground = A Kép beállítása háttérképként menüparancs letiltása.
policy-DisableSetAsDesktopBackground = A Beállítás háttérképként menüparancs letiltása a képeknél.
policy-DisableSystemAddonUpdate = Annak a megakadályozása, hogy a böngésző rendszer-kiegészítőket telepítsen és frissítsen.
policy-DisableTelemetry = Telemetria kikapcsolása.
policy-DisplayBookmarksToolbar = A Könyvjelző eszköztár megjelenítése alapértelmezetten.
policy-DisplayMenuBar = A Menüsáv megjelenítése alapértelmezetten.
policy-DNSOverHTTPS = HTTPS feletti DNS beállítása.
policy-DontCheckDefaultBrowser = Az alapértelmezett böngésző ellenőrzés kikapcsolása indításkor.
# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = A Tartalomblokkolás engedélyezése vagy letiltása, és válaszható módon, annak zárolása.
# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#extensions-machine-only
policy-Extensions = Kiegészítők telepítése, eltávolítása vagy zárolása. A Telepítés lehetőség URL-t vagy útvonalat vár paraméterként. Az Eltávolítás és Zárolás kiegészítőazonosítót vár.
policy-FlashPlugin = A Flash bővítmény használatának engedélyezése vagy tiltása.
policy-HardwareAcceleration = Ha hamis, akkor kikapcsolja a hardveres gyorsítást.
# “lock” means that the user won’t be able to change this setting
policy-Homepage = A kezdőlap beállítása, és választható módon, annak zárolása.
policy-InstallAddonsPermission = Bizonyos weboldalak telepíthetnek kiegészítőket.
policy-NoDefaultBookmarks = A { -brand-short-name }szal szállított alapértelmezett könyvjelzők és okos könyvjelzők (Legtöbbet látogatott, Friss címkék) létrehozásának letiltása. Megjegyzés: ez a házirend csak a profil első futtatása előtt van érvényben.
policy-OfferToSaveLogins = A beállítás erőltetése, miszerint a { -brand-short-name } felajánlja a mentett bejelentkezések és jelszavak megjegyzését. Mind az igaz, mind a hamis érték elfogadott.
policy-OverrideFirstRunPage = Az első indítás oldal felülbírálása. Állítsa üres értékre ezt a házirendet, ha le akarja tiltani a az első indítás oldalt.
policy-OverridePostUpdatePage = A frissítés utáni „Újdonságok” oldal felülbírálása. Állítsa üres ezt a házirendet, ha azt szeretné, hogy ne legyen frissítés utáni oldal.
policy-Permissions = A kamera, mikrofon, helyadatok és értesítések jogosultságának beállítása.
policy-PopupBlocking = Bizonyos weboldalak alapértelmezetten jeleníthessenek meg felugró ablakokat.
policy-Proxy = Proxy-beállítások konfigurálása.
policy-RequestedLocales = Adja meg a kért területi beállításokat az alkalmazásnak, az Ön által előnyben részesített sorrendben.
policy-SanitizeOnShutdown = Összes navigációs adat törlése leállításkor.
policy-SearchBar = A keresősáv alapértelmezett helyének megadása. A felhasználó továbbra is testreszabhatja.
policy-SearchEngines = Keresőszolgáltatások beállításainak konfigurálása. Ez a házirend csak a kibővített támogatású kiadásban (ESR) érhető el.
# For more information, see https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/PKCS11/Module_Installation
policy-SecurityDevices = PKCS #11 modulok telepítése.
# “format” refers to the format used for the value of this policy. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#websitefilter-machine-only
policy-WebsiteFilter = Weboldalak felkeresésének blokkolása. Lásd a dokumentációt a formátum részleteiért.
