# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = Blokkeerlijsten
    .style = width: 55em
blocklist-desc = U kunt kiezen welke lijst { -brand-short-name } zal gebruiken voor het blokkeren van web-elementen die uw surfactiviteit kunnen volgen.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = Lijst
blocklist-button-cancel =
    .label = Annuleren
    .accesskey = A
blocklist-button-ok =
    .label = Wijzigingen opslaan
    .accesskey = W
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Disconnect.me - basisbescherming (Aanbevolen).
blocklist-item-moz-std-desc = Staat enkele volgers toe, zodat websites goed functioneren.
blocklist-item-moz-full-name = Disconnect.me - strenge bescherming.
blocklist-item-moz-full-desc = Blokkeert bekende volgers. Sommige websites werken mogelijk niet goed.
