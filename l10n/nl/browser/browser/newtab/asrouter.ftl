# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## These messages are used as headings in the recommendation doorhanger

cfr-doorhanger-extension-heading = Aanbevolen extensie
cfr-doorhanger-feature-heading = Aanbevolen functie
cfr-doorhanger-pintab-heading = Probeer dit: Tabblad vastmaken



cfr-doorhanger-extension-sumo-link =
    .tooltiptext = Waarom zie ik dit
cfr-doorhanger-extension-cancel-button = Niet nu
    .accesskey = N
cfr-doorhanger-extension-ok-button = Nu toevoegen
    .accesskey = t
cfr-doorhanger-pintab-ok-button = Dit tabblad vastmaken
    .accesskey = v
cfr-doorhanger-extension-manage-settings-button = Instellingen voor aanbevelingen beheren
    .accesskey = I
cfr-doorhanger-extension-never-show-recommendation = Deze aanbeveling niet tonen
    .accesskey = D
cfr-doorhanger-extension-learn-more-link = Meer info
# This string is used on a new line below the add-on name
# Variables:
#   $name (String) - Add-on author name
cfr-doorhanger-extension-author = door { $name }
# This is a notification displayed in the address bar.
# When clicked it opens a panel with a message for the user.
cfr-doorhanger-extension-notification = Aanbeveling

## Add-on statistics
## These strings are used to display the total number of
## users and rating for an add-on. They are shown next to each other.

# Variables:
#   $total (Number) - The rating of the add-on from 1 to 5
cfr-doorhanger-extension-rating =
    .tooltiptext =
        { $total ->
            [one] { $total } ster
           *[other] { $total } sterren
        }
# Variables:
#   $total (Number) - The total number of users using the add-on
cfr-doorhanger-extension-total-users =
    { $total ->
        [one] { $total } gebruiker
       *[other] { $total } gebruikers
    }
cfr-doorhanger-pintab-description = Makkelijke toegang tot uw meestgebruikte websites. Houd websites open in een tabblad (zelfs wanneer u herstart).

## These messages are steps on how to use the feature and are shown together.

cfr-doorhanger-pintab-step1 = <b>Klik met de rechtermuisknop</b> op het tabblad dat u wilt vastmaken.
cfr-doorhanger-pintab-step2 = Selecteer <b>Tabblad vastmaken</b> vanuit het menu.
cfr-doorhanger-pintab-step3 = Als de website een update bevat, ziet u een blauwe stip op uw vastgemaakte tabblad.
cfr-doorhanger-pintab-animation-pause = Pauzeren
cfr-doorhanger-pintab-animation-resume = Hervatten

## Firefox Accounts Message

cfr-doorhanger-bookmark-fxa-header = Synchroniseer uw bladwijzers overal.
cfr-doorhanger-bookmark-fxa-body = Goed gevonden! Zorg er nu voor dat u niet zonder bladwijzers zit op uw mobiele apparaten. Ga van start met { -fxaccount-brand-name }.
cfr-doorhanger-bookmark-fxa-link-text = Bladwijzers nu synchroniseren…
cfr-doorhanger-bookmark-fxa-close-btn-tooltip =
    .aria-label = Knop Sluiten
    .title = Sluiten

## Protections panel

cfr-protections-panel-header = Surf zonder te worden gevolgd
cfr-protections-panel-body = Houd uw gegevens voor uzelf. { -brand-short-name } beschermt u tegen veel van de meest voorkomende trackers die volgen wat u online doet.
cfr-protections-panel-link-text = Meer info

## What's New toolbar button and panel

cfr-whatsnew-button =
    .label = Wat is er nieuw
    .tooltiptext = Wat is er nieuw
cfr-whatsnew-panel-header = Wat is er nieuw

## Bookmark Sync

cfr-doorhanger-sync-bookmarks-header = Deze bladwijzer op uw telefoon ontvangen
cfr-doorhanger-sync-bookmarks-body = Neem uw bladwijzers, wachtwoorden, geschiedenis en meer mee naar overal waar u bent aangemeld bij { -brand-product-name }.
cfr-doorhanger-sync-bookmarks-ok-button = { -sync-brand-short-name } inschakelen
    .accesskey = i

## Login Sync

cfr-doorhanger-sync-logins-header = Verlies nooit meer een wachtwoord
cfr-doorhanger-sync-logins-body = Bewaar en synchroniseer uw wachtwoorden veilig op al uw apparaten.
cfr-doorhanger-sync-logins-ok-button = { -sync-brand-short-name } inschakelen
    .accesskey = i

## Send Tab

cfr-doorhanger-send-tab-header = Dit onderweg lezen
cfr-doorhanger-send-tab-recipe-header = Dit recept meenemen naar de keuken
cfr-doorhanger-send-tab-body = Met Send Tab kunt u eenvoudig deze koppeling met uw telefoon delen, of waar u ook maar bent aangemeld bij { -brand-product-name }.
cfr-doorhanger-send-tab-ok-button = Send Tab proberen
    .accesskey = p

## Firefox Send

cfr-doorhanger-firefox-send-header = Dit pdf-document veilig delen
cfr-doorhanger-firefox-send-body = Houd uw gevoelige documenten weg bij nieuwsgierige blikken met end-to-end-versleuteling en een koppeling die verdwijnt als u klaar bent.
cfr-doorhanger-firefox-send-ok-button = { -send-brand-name } proberen
    .accesskey = p
