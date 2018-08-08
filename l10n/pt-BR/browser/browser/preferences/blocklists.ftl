# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = Listas de bloqueio
    .style = width: 50em
blocklist-desc = Escolha a lista que o { -brand-short-name } usará para bloquear elementos da Web que podem rastrear sua atividade de navegação.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = Lista
blocklist-button-cancel =
    .label = Cancelar
    .accesskey = C
blocklist-button-ok =
    .label = Salvar alterações
    .accesskey = S
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Proteção básica Disconnect.me (Recomendado).
blocklist-item-moz-std-desc = Permite alguns rastreadores, para que sites funcionem corretamente.
blocklist-item-moz-full-name = Proteção rigorosa Disconnect.me.
blocklist-item-moz-full-desc = Bloqueia rastreadores conhecidos. Alguns sites podem não funcionar corretamente.
