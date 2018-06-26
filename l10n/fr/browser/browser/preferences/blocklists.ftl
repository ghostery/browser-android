# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = Listes de blocage
    .style = width: 70em
blocklist-desc = Vous pouvez désormais choisir quelle liste { -brand-short-name } doit utiliser pour bloquer les éléments web susceptibles de pister votre navigation.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = Liste
blocklist-button-cancel =
    .label = Annuler
    .accesskey = A
blocklist-button-ok =
    .label = Enregistrer les modifications
    .accesskey = E
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Protection basique de Disconnect.me (Recommandée).
blocklist-item-moz-std-desc = Autorise certains éléments de pistage afin que des sites fonctionnent correctement.
blocklist-item-moz-full-name = Protection stricte de Disconnect.me.
blocklist-item-moz-full-desc = Bloque les éléments de pistage connus. Certains sites peuvent ne pas fonctionner correctement.
