# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


pane-general-title = Algemeen
category-general =
    .tooltiptext = { pane-general-title }

general-language-and-appearance-header = Taal & Vormgeving

general-incoming-mail-header = Inkomende berichten

general-files-and-attachment-header = Bestanden & Bijlagen

general-tags-header = Labels

general-reading-and-display-header = Lezen & Weergave

general-updates-header = Updates

general-network-and-diskspace-header = Netwerk & Schijfruimte

general-indexing-label = Indexering

composition-category-header = Compositie

composition-attachments-header = Bijlagen

composition-spelling-title = Spelling

compose-html-style-title = HTML-stijl

composition-addressing-header = Addressering

privacy-main-header = Privacy

privacy-passwords-header = Wachtwoorden

privacy-junk-header = Ongewenste berichten

privacy-data-collection-header = Gegevensverzameling en -gebruik

privacy-security-header = Beveiliging

privacy-scam-detection-title = Scamdetectie

privacy-anti-virus-title = Antivirus

privacy-certificates-title = Certificaten

chat-pane-header = Chat

chat-status-title = Status

chat-notifications-title = Notificaties

chat-pane-styling-header = Vormgeving

choose-messenger-language-description = Kies de talen die worden gebruikt voor het weergeven van menu’s, berichten en notificaties van { -brand-short-name }.
manage-messenger-languages-button =
  .label = Alternatieven instellen…
  .accesskey = l
confirm-messenger-language-change-description = Herstart { -brand-short-name } om deze wijzigingen toe te passen.
confirm-messenger-language-change-button = Toepassen en herstarten

update-pref-write-failure-title = Schrijffout

# Variables:
#   $path (String) - Path to the configuration file
update-pref-write-failure-message = Kan voorkeur niet opslaan. Kon niet schrijven naar bestand: { $path }

update-setting-write-failure-title = Fout bij opslaan updatevoorkeuren

# Variables:
#   $path (String) - Path to the configuration file
# The newlines between the main text and the line containing the path is
# intentional so the path is easier to identify.
update-setting-write-failure-message =
    { -brand-short-name } heeft een fout aangetroffen en heeft deze wijziging niet opgeslagen. Merk op dat voor het instellen van deze updatevoorkeur schrijfrechten voor onderstaand bestand benodigd zijn. U of uw systeembeheerder kan deze fout oplossen door de groep Gebruikers volledige toegang tot dit bestand te geven.

    Kon niet schrijven naar bestand: { $path }

update-in-progress-title = Update wordt uitgevoerd

update-in-progress-message = Wilt u dat { -brand-short-name } doorgaat met deze update?

update-in-progress-ok-button = &Verwerpen
# Continue is the cancel button so pressing escape or using a platform standard
# method of closing the UI will not discard the update.
update-in-progress-cancel-button = &Doorgaan
