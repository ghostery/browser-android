# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Thunderbird installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-3rdparty = Állítson be házirendeket, amelyeket a WebExtensionök a chrome.storage.managed segítségével érhetnek el.
policy-AppUpdateURL = Egyéni alkalmazásfrissítési URL beállítása.
policy-Authentication = Az integrált hitelesítés beállítása az azt támogató webhelyek számára.
policy-BlockAboutAddons = Hozzáférés blokkolása a kiegészítőkezelőhöz (about:addons).
policy-BlockAboutConfig = Hozzáférés blokkolása az about:config oldalhoz.
policy-BlockAboutProfiles = Hozzáférés blokkolása az about:profiles oldalhoz.
policy-BlockAboutSupport = Hozzáférés blokkolása az about:support oldalhoz.
policy-CaptivePortal = Beléptető portál engedélyezése vagy letiltása.
policy-CertificatesDescription = Tanúsítványok hozzáadása vagy a beépített tanúsítványok használata.
policy-Cookies = Engedélyezés vagy letiltás, hogy a weboldalak sütiket állítsanak be.
policy-DefaultDownloadDirectory = Az alapértelmezett letöltési könyvtár megadása.
policy-DisableAppUpdate = A { -brand-short-name } frissítésének megakadályozása.
policy-DisableDeveloperTools = Hozzáférés blokkolása a fejlesztői eszközökhöz.
policy-DisableFeedbackCommands = A visszajelzés küldési parancsok letiltása a Súgó menüben (Visszajelzés beküldése és Félrevezető oldal jelentése).
policy-DisableForgetButton = Az Elfelejtés gomb elérésének megakadályozása.
policy-DisableMasterPasswordCreation = Ha igaz, akkor nem hozható létre mesterjelszó.
policy-DisableProfileImport = Az Importálás egy másik böngészőből menüparancs letiltása.
policy-DisableSafeMode = Az Újraindítás letiltott kiegészítőkkel funkció letiltása. Megjegyzés: a kiegészítők nélküli indításhoz használható Shift billentyű csak Windowson tiltható le csoportházirend segítségével.
policy-DisableSecurityBypass = Megakadályozza, hogy a felhasználó megkerüljön bizonyos biztonsági figyelmeztetéseket.
policy-DisableSystemAddonUpdate = Megakadályozza, hogy a { -brand-short-name } telepítse és frissítse a rendszer-kiegészítőket.
policy-DisableTelemetry = Telemetria kikapcsolása.
policy-DisplayMenuBar = A menüsáv megjelenítése alapértelmezetten.
policy-DNSOverHTTPS = HTTPS feletti DNS beállítása.
policy-DontCheckDefaultClient = Az indításkori alapértelmezett kliensprogram ellenőrzés letiltása.
policy-DownloadDirectory = A letöltési könyvtár megadása és zárolása.
# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = A tartalomblokkolás engedélyezése vagy letiltása, valamint adott esetben zárolása.
# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs.
policy-Extensions = Kiegészítők telepítése, eltávolítása vagy zárolása. A Telepítés lehetőség URL-t vagy útvonalat vár paraméterként. Az Eltávolítás és Zárolás kiegészítőazonosítót vár.
policy-ExtensionSettings = Kezelje a kiegészítőtelepítés összes aspektusát.
policy-ExtensionUpdate = Az automatikus kiegészítőfrissítések engedélyezése vagy letiltása.
policy-HardwareAcceleration = Ha hamis, akkor kikapcsolja a hardveres gyorsítást.
policy-InstallAddonsPermission = Engedélyezi, hogy bizonyos weboldalak kiegészítőket telepítsenek.
policy-LocalFileLinks = Engedélyezi, hogy bizonyos weboldalak helyi fájlokra hivatkozzanak.
policy-NetworkPrediction = A hálózati előrejelzés engedélyezése vagy letiltása (előzetes DNS lekérdezés).
policy-OfferToSaveLogins = A beállítás erőltetése, miszerint a { -brand-short-name } felajánlja a mentett bejelentkezések és jelszavak megjegyzését. Mind az igaz, mind a hamis érték elfogadott.
policy-OverrideFirstRunPage = Az első indítás oldal felülbírálása. Állítsa üres értékre ezt a házirendet, ha le akarja tiltani a az első indítás oldalt.
policy-OverridePostUpdatePage = A frissítés utáni „Újdonságok” oldal felülbírálása. Állítsa üres ezt a házirendet, ha azt szeretné, hogy ne legyen frissítés utáni oldal.
policy-Preferences = A beállítások egy részének megadása és zárolása.
policy-PromptForDownloadLocation = Kérdezze meg, hogy hová mentse le a fájlokat letöltéskor.
policy-Proxy = Proxy beállítások konfigurálása.
policy-RequestedLocales = A kért területi beállítások megadása az alkalmazásnak, az előnyben részesített sorrendben.
policy-SanitizeOnShutdown2 = Navigációs adatok törlése leállításkor.
policy-SearchEngines = Keresőszolgáltatások beállításainak konfigurálása. Ez a házirend csak a kibővített támogatású kiadásban (ESR) érhető el.
# For more information, see https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/PKCS11/Module_Installation
policy-SecurityDevices = PKCS #11 modulok telepítése.
policy-SSLVersionMax = A maximális SSL-verzió megadása.
policy-SSLVersionMin = A minimális SSL-verzió megadása.
policy-SupportMenu = Egyéni támogatási menüelem hozzáadása a súgó menühöz.
# “format” refers to the format used for the value of this policy.
policy-WebsiteFilter = Weboldalak felkeresésének blokkolása. Lásd a dokumentációt a formátum részleteiért.
