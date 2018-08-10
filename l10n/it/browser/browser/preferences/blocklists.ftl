# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = Elenchi per blocco elementi traccianti
    .style = width: 55em
blocklist-desc = Scegliere l’elenco da utilizzare in { -brand-short-name } per bloccare gli elementi in grado di tracciare la navigazione.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = Elenco
blocklist-button-cancel =
    .label = Annulla
    .accesskey = A
blocklist-button-ok =
    .label = Salva modifiche
    .accesskey = S
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Disconnect.me protezione base (consigliato).
blocklist-item-moz-std-desc = Consente alcuni elementi traccianti per garantire il corretto funzionamento dei siti.
blocklist-item-moz-full-name = Disconnect.me protezione massima.
blocklist-item-moz-full-desc = Blocca gli elementi traccianti conosciuti. Alcuni siti web potrebbero non funzionare correttamente.
