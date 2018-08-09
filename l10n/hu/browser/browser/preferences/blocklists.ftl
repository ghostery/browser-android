# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = Blokkolási listák
    .style = width: 55em
blocklist-desc = Kiválaszthatja, hogy mely lista használatával fogja a { -brand-short-name } blokkolni az Ön böngészési tevékenységét követő weboldalelemeket.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = Listázás
blocklist-button-cancel =
    .label = Mégse
    .accesskey = M
blocklist-button-ok =
    .label = Változtatások mentése
    .accesskey = V
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Disconnect.me alapszintű védelem (ajánlott).
blocklist-item-moz-std-desc = Néhány követő engedélyezése, hogy a weboldalak megfelelően működjenek.
blocklist-item-moz-full-name = Disconnect.me szigorú védelem.
blocklist-item-moz-full-desc = Ismert követők blokkolása. Néhány webhely lehet, hogy nem fog megfelelően működni.
