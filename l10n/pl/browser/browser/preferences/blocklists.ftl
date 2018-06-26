# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = Lista blokowanych elementów
    .style = width: 57em
blocklist-desc = Określ, którą listę { -brand-short-name } będzie wykorzystywał do blokowania elementów stron mogących śledzić Twoją aktywność.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = Lista
blocklist-button-cancel =
    .label = Anuluj
    .accesskey = A
blocklist-button-ok =
    .label = Zachowaj zmiany
    .accesskey = Z
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Disconnect.me, podstawowa (zalecana).
blocklist-item-moz-std-desc = Dopuszcza pewne śledzące elementy, aby strony działały prawidłowo.
blocklist-item-moz-full-name = Disconnect.me, ścisła.
blocklist-item-moz-full-desc = Blokuje znane śledzące elementy. Niektóre strony mogą nie działać prawidłowo.
