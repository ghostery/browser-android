# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### UI strings for the simplified onboarding modal / about:welcome
### Various strings use a non-breaking space to avoid a single dangling /
### widowed word, so test on various window sizes if you also want this.


## These button action text can be split onto multiple lines, so use explicit
## newlines in translations to control where the line break appears (e.g., to
## avoid breaking quoted text).

onboarding-button-label-learn-more = További tudnivalók
onboarding-button-label-try-now = Próbálja ki most
onboarding-button-label-get-started = Kezdő lépések

## Welcome modal dialog strings

onboarding-welcome-header = Üdvözli a { -brand-short-name }
onboarding-welcome-body = Megvan a böngészője.<br/>Ismerkedjen meg a { -brand-product-name } család többi tagjával.
onboarding-welcome-learn-more = További tudnivalók az előnyökről.
onboarding-join-form-header = Csatlakozzon a { -brand-product-name }hoz
onboarding-join-form-body = Kezdésként adja meg az e-mail címét.
onboarding-join-form-email =
    .placeholder = Adja meg az e-mail címét
onboarding-join-form-email-error = Érvényes e-mail cím szükséges
onboarding-join-form-legal = A folytatással elfogadja a <a data-l10n-name="terms">Szolgáltatási feltételeket</a> és az <a data-l10n-name="privacy">Adatvédelmi nyilatkozatot</a>.
onboarding-join-form-continue = Folytatás
onboarding-start-browsing-button-label = Böngészés megkezdése
onboarding-cards-dismiss =
    .title = Elutasítás
    .aria-label = Elutasítás

## Firefox Sync modal dialog strings.

onboarding-sync-welcome-header = Vigye magával a { -brand-product-name }ot
onboarding-sync-welcome-content = Kapja meg a könyvjelzőit, előzményeit, jelszavait és egyéb beállításait az összes eszközén.
onboarding-sync-welcome-learn-more-link = Ismerje meg a Firefox fiókokat
onboarding-sync-form-invalid-input = Érvényes e-mail szükséges
onboarding-sync-legal-notice = A folytatással elfogadja a <a data-l10n-name="terms">Felhasználási feltételeket</a> és az <a data-l10n-name="privacy">Adatvédelmi nyilatkozatot</a>.
onboarding-sync-form-input =
    .placeholder = E-mail
onboarding-sync-form-continue-button = Folytatás
onboarding-sync-form-skip-login-button = Lépés kihagyása

## This is part of the line "Enter your email to continue to Firefox Sync"

onboarding-sync-form-header = Adja meg az e-mail címét
onboarding-sync-form-sub-header = és lépjen tovább a { -sync-brand-name }hez.

## These are individual benefit messages shown with an image, title and
## description.

onboarding-benefit-products-title = Hasznos termékek
onboarding-benefit-products-text = Végezze el a teendőit egy olyan eszközcsaláddal, amely tiszteletben tartja a magánszféráját az összes eszközén.
onboarding-benefit-knowledge-title = Gyakorlati tudás
onboarding-benefit-knowledge-text = Tudjon meg mindent, amit tudnia kell ahhoz, hogy okosabb legyen és nagyobb biztonságban legyen online.
onboarding-benefit-privacy-title = Valódi adatvédelem
# "Personal Data Promise" is a concept that should be translated consistently
# across the product. It refers to a concept shown elsewhere to the user: "The
# Firefox Personal Data Promise is the way we honor your data in everything we
# make and do. We take less data. We keep it safe. And we make sure that we are
# transparent about how we use it."
onboarding-benefit-privacy-text = Minden, amit teszünk, betartja a személyes adatokra vonatkozó ígéretünket: Gyűjts kevesebbet. Tartsd biztonságban. Nincsenek titkok.

## These strings belong to the individual onboarding messages.


## Each message has a title and a description of what the browser feature is.
## Each message also has an associated button for the user to try the feature.
## The string for the button is found above, in the UI strings section

onboarding-private-browsing-title = Privát böngészés
onboarding-private-browsing-text = Böngésszen egyedül. A tartalomblokkolással ellátott privát böngészés blokkolja az online követőket, amelyek követik Önt a weben.
onboarding-screenshots-title = Képernyőképek
onboarding-screenshots-text = Készítsen, mentsen és osszon meg képernyőképeket – a { -brand-short-name } elhagyása nélkül. Rögzítsen egy területet vagy egy teljes lapot böngészés közben. Aztán mentse a webre a könnyű eléréshez és megosztáshoz.
onboarding-addons-title = Kiegészítők
onboarding-addons-text = A kiegészítők lehetővé teszik, hogy a { -brand-short-name } keményebben dolgozzon Önnek. Hasonlítson össze árakat, nézze meg az időjárást, vagy fejezze ki a személyiségét egy egyéni témával.
onboarding-ghostery-title = Ghostery
onboarding-ghostery-text = Böngésszen gyorsabban, okosabban vagy biztonságosabban az olyan kiegészítőkkel mint a Ghostery, amelyekkel blokkolhatja az idegesítő reklámokat.
# Note: "Sync" in this case is a generic verb, as in "to synchronize"
onboarding-fxa-title = Szinkronizálás
onboarding-fxa-text = Regisztráljon egy { -fxaccount-brand-name }ot, és szinkronizálja a könyvjelzőit, jelszavait és nyitott lapjait bárhol, ahol { -brand-short-name }ot használ.
onboarding-tracking-protection-title2 = Védelem a nyomon követés ellen
onboarding-tracking-protection-text2 = A { -brand-short-name } segít megakadályozni, hogy a webhelyek nyomon követhessék Önt online, így nehezebbé teszi, hogy a hirdetések kövessék a weben.
onboarding-tracking-protection-button2 = Hogyan működik
onboarding-data-sync-title = Vigye magával a beállításait
# "Sync" is short for synchronize.
onboarding-data-sync-text2 = Szinkronizálja a könyvjelzőket, a jelszavakat és még többet, bárhol is használja a { -brand-product-name }ot.
onboarding-data-sync-button2 = Jelentkezzen be a { -sync-brand-short-name }be
onboarding-firefox-monitor-title = Figyeljen az adatszegésekre
onboarding-firefox-monitor-text = A { -monitor-brand-name } figyeli, hogy az e-mail címe megjelent-e egy adatszegésben, és figyelmezteti Önt, ha az egy új adatszegésben jelenik meg.
onboarding-firefox-monitor-button = Iratkozzon fel a figyelmeztetésekre
onboarding-browse-privately-title = Böngésszen privát módon
onboarding-browse-privately-text = A privát böngészés törli a keresési és böngészési előzményeket, hogy titokban tartsa azokat azoktól, akik a számítógépét használják.
onboarding-browse-privately-button = Privát ablak megnyitása
onboarding-firefox-send-title = Tárolja bizalmasan a megosztott fájljait
onboarding-firefox-send-text2 = Töltse fel a fájljait a { -send-brand-name } segítségével, és ossza meg azokat végpontok közötti titkosítással és egy automatikusan lejáró hivatkozással.
onboarding-firefox-send-button = Próbálja ki a { -send-brand-name }et
onboarding-mobile-phone-title = Szerezze be a { -brand-product-name } alkalmazást a telefonján
onboarding-mobile-phone-text = Töltse le a { -brand-product-name } alkalmazást iOS-re vagy Androidra, és szinkronizálja az adatait az eszközei között.
# "Mobile" is short for mobile/cellular phone, "Browser" is short for web
# browser.
onboarding-mobile-phone-button = Mobilböngésző letöltése
onboarding-send-tabs-title = Küldjön lapokat magának azonnal
# "Send Tabs" refers to "Send Tab to Device" feature that appears when opening a
# tab's context menu.
onboarding-send-tabs-text = A Lapok küldése azonnal megosztja a lapokat az eszközei között, anélkül hogy másolni, beillesztenie kellene, vagy elhagynia a böngészőt.
onboarding-send-tabs-button = Kezdje el használni a lapok küldését
onboarding-pocket-anywhere-title = Olvasson és hallgasson bárhol
onboarding-pocket-anywhere-text2 = Mentse le a kedvenc tartalmait a { -pocket-brand-name } alkalmazással, és olvassa el, hallgassa meg vagy nézze meg, amikor az Ön számára kényelmes.
onboarding-pocket-anywhere-button = Próbálja ki a { -pocket-brand-name }et
onboarding-lockwise-passwords-title = Vigye el a jelszavait mindenhová
onboarding-lockwise-passwords-text2 = Tartsa biztonságban a mentett jelszavait és jelentkezzen be egyszerűen a fiókjaiba a { -lockwise-brand-name } segítségével.
onboarding-lockwise-passwords-button2 = Alkalmazás beszerzése
onboarding-facebook-container-title = Állítson be korlátokat a Facebookkal
onboarding-facebook-container-text2 = A { -facebook-container-brand-name } elkülöníti a profilját minden mástól, ami nehezebbé teszi, hogy a Facebook célzott hirdetéseket küldjön Önnek.
onboarding-facebook-container-button = A kiegészítő hozzáadása

## Message strings belonging to the Return to AMO flow

return-to-amo-sub-header = Nagyszerű, már van { -brand-short-name }a
# <icon></icon> will be replaced with the icon belonging to the extension
#
# Variables:
#   $addon-name (String) - Name of the add-on
return-to-amo-addon-header = Most pedig szerezze be a(z) <icon></icon><b>{ $addon-name } kiegészítőt.</b>
return-to-amo-extension-button = Kiegészítő hozzáadása
return-to-amo-get-started-button = Első lépések a { -brand-short-name }szal
