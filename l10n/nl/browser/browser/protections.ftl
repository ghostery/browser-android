# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
#   $count (Number) - Number of tracking events blocked.
graph-week-summary =
    { $count ->
        [one] { -brand-short-name } heeft afgelopen week { $count } tracker geblokkeerd
       *[other] { -brand-short-name } heeft afgelopen week { $count } trackers geblokkeerd
    }
# Variables:
#   $count (Number) - Number of tracking events blocked.
#   $earliestDate (Number) - Unix timestamp in ms, representing a date. The
# earliest date recorded in the database.
graph-total-summary =
    { $count ->
        [one] { $count } tracker geblokkeerd sinds { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
       *[other] { $count } trackers geblokkeerd sinds { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
    }
# The terminology used to refer to categories of Content Blocking is also used in chrome/browser/browser.properties and should be translated consistently.
# "Standard" in this case is an adjective, meaning "default" or "normal".
# The category name in the <b> tag will be bold.
protection-header-details-standard = Beschermingsniveau is ingesteld op <b>Standaard</b>
protection-header-details-strict = Beschermingsniveau is ingesteld op <b>Streng</b>
protection-header-details-custom = Beschermingsniveau is ingesteld op <b>Aangepast</b>
protection-report-page-title = Privacybeschermingen
protection-report-content-title = Privacybeschermingen
etp-card-title = Verbeterde bescherming tegen volgen
etp-card-content = Trackers volgen u online om gegevens over uw surfgedrag en interesses te verzamelen. { -brand-short-name } blokkeert veel van deze trackers en andere kwaadwillende scripts.
# This string is used to label the X axis of a graph. Other days of the week are generated via Intl.DateTimeFormat,
# capitalization for this string should match the output for your locale.
graph-today = Vandaag
# This string is used to describe the graph for screenreader users.
graph-legend-description = Een grafiek van het totale aantal trackers per type die deze week zijn geblokkeerd.
social-tab-title = Sociale-mediatrackers
social-tab-contant = Sociale netwerken plaatsen trackers op andere websites om te volgen wat u online doet, ziet en bekijkt. Hierdoor kunnen sociale-mediabedrijven meer over u leren dan wat u deelt op uw sociale-mediaprofielen. <a data-l10n-name="learn-more-link">Meer info</a>
cookie-tab-title = Cross-site-trackingcookies
cookie-tab-content = Deze cookies volgen u op verschillende websites om gegevens te verzamelen over wat u online doet. Ze worden geplaatst door derden, zoals adverteerders en analysebedrijven. Door cross-sitetrackingcookies te blokkeren, vermindert het aantal advertenties dat u volgt. <a data-l10n-name="learn-more-link">Meer info</a>
tracker-tab-title = Volginhoud
tracker-tab-content = Websites kunnen externe advertenties, video’s en andere inhoud laden die volgcode bevat. Het blokkeren van volginhoud kan websites helpen sneller te laden, maar sommige knoppen, formulieren en aanmeldvelden werken mogelijk niet. <a data-l10n-name="learn-more-link">Meer info</a>
fingerprinter-tab-title = Fingerprinters
fingerprinter-tab-content = Fingerprinters verzamelen instellingen van uw browser en computer om een profiel van u te maken. Met behulp van deze digitale vingerafdruk kunnen ze u op verschillende websites volgen. <a data-l10n-name="learn-more-link">Meer info</a>
cryptominer-tab-title = Cryptominers
cryptominer-tab-content = Cryptominers gebruiken de rekenkracht van uw systeem om digitale valuta te genereren. Cryptominer-scripts trekken uw accu leeg, vertragen uw computer en kunnen uw energierekening omhoog jagen. <a data-l10n-name="learn-more-link">Meer info</a>
lockwise-title = Vergeet nooit meer een wachtwoord
lockwise-title-logged-in = { -lockwise-brand-name }
lockwise-header-content = { -lockwise-brand-name } slaat uw wachtwoorden veilig op in uw browser.
lockwise-header-content-logged-in = Bewaar en synchroniseer uw wachtwoorden veilig op al uw apparaten.
open-about-logins-button = Openen in { -brand-short-name }
lockwise-no-logins-content = Download de <a data-l10n-name="lockwise-inline-link">{ -lockwise-brand-name }</a>-app om uw wachtwoorden overal mee naartoe te nemen.
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
lockwise-passwords-stored =
    { $count ->
        [one] wachtwoord veilig opgeslagen <a data-l10n-name="lockwise-how-it-works">Hoe het werkt</a>
       *[other] wachtwoorden veilig opgeslagen <a data-l10n-name="lockwise-how-it-works">Hoe het werkt</a>
    }
turn-on-sync = { -sync-brand-short-name } inschakelen…
    .title = Naar synchronisatievoorkeuren
manage-devices = Apparaten beheren
# Variables:
#   $count (Number) - Number of devices connected with sync.
lockwise-sync-status =
    { $count ->
        [one] Synchroniseert met { $count } ander apparaat
       *[other] Synchroniseert met { $count } andere apparaten
    }
lockwise-sync-not-syncing = Synchroniseert niet met andere apparaten.
monitor-title = Let op datalekken
monitor-link = Hoe het werkt
monitor-header-content = Kijk op { -monitor-brand-name } om te zien of u getroffen bent door een datalek en ontvang waarschuwingen over nieuwe lekken.
monitor-header-content-logged-in = { -monitor-brand-name } waarschuwt u als uw gegevens voorkomen in een bekend datalek
monitor-sign-up = Inschrijven voor waarschuwingen over datalekken
auto-scan = Vandaag automatisch gescand
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-monitored-addresses =
    { $count ->
        [one] e-mailadres wordt gemonitord.
       *[other] e-mailadressen worden gemonitord.
    }
# This string is displayed after a large numeral that indicates the total number
# of known data breaches. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-known-breaches =
    { $count ->
        [one] bekend datalek heeft uw gegevens gelekt.
       *[other] bekende datalekken hebben uw gegevens gelekt.
    }
# This string is displayed after a large numeral that indicates the total number
# of exposed passwords. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-exposed-passwords =
    { $count ->
        [one] wachtwoord gelekt in alle lekken.
       *[other] wachtwoorden gelekt in alle lekken.
    }
full-report-link = Bekijk het volledige rapport op <a data-l10n-name="monitor-inline-link">{ -monitor-brand-name }</a>
# This string is displayed after a large numeral that indicates the total number
# of saved logins which may have been exposed. Don’t add $count to
# your localization, because it would result in the number showing twice.
password-warning =
    { $count ->
        [one] opgeslagen aanmelding is mogelijk gelekt in een datalek. Wijzig dit wachtwoord voor betere online beveiliging. <a data-l10n-name="lockwise-link">Opgeslagen aanmeldingen bekijken</a>
       *[other] opgeslagen aanmeldingen zijn mogelijk gelekt in een datalek. Wijzig deze wachtwoorden voor betere online beveiliging. <a data-l10n-name="lockwise-link">Opgeslagen aanmeldingen bekijken</a>
    }
# This is the title attribute describing the graph report's link to about:settings#privacy
go-to-privacy-settings = Naar privacyinstellingen
# This is the title attribute describing the Lockwise card's link to about:logins
go-to-saved-logins = Naar opgeslagen aanmeldingen

## The title attribute is used to display the type of protection.
## The aria-label is spoken by screen readers to make the visual graph accessible to blind users.
##
## Variables:
##   $count (Number) - Number of specific trackers
##   $percentage (Number) - Percentage this type of tracker contributes to the whole graph

bar-tooltip-social =
    .title = Sociale-mediatrackers
    .aria-label =
        { $count ->
            [one] { $count } sociale-mediatracker ({ $percentage }%)
           *[other] { $count } sociale-mediatrackers ({ $percentage }%)
        }
bar-tooltip-cookie =
    .title = Cross-site-trackingcookies
    .aria-label =
        { $count ->
            [one] { $count } cross-site-trackingcookie ({ $percentage }%)
           *[other] { $count } cross-site-trackingcookies ({ $percentage }%)
        }
bar-tooltip-tracker =
    .title = Volginhoud
    .aria-label =
        { $count ->
            [one] { $count } volginhouditem ({ $percentage }%)
           *[other] { $count } volginhouditems ({ $percentage }%)
        }
bar-tooltip-fingerprinter =
    .title = Fingerprinters
    .aria-label =
        { $count ->
            [one] { $count } fingerprinter ({ $percentage }%)
           *[other] { $count } fingerprinters ({ $percentage }%)
        }
bar-tooltip-cryptominer =
    .title = Cryptominers
    .aria-label =
        { $count ->
            [one] { $count } cryptominer ({ $percentage }%)
           *[other] { $count } cryptominers ({ $percentage }%)
        }
