# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = Listas de bloqueo
    .style = width: 60em
blocklist-desc = Puede elegir qué lista usará { -brand-short-name } para bloquear elementos web que puedan rastrear su actividad de navegación.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = Lista
blocklist-button-cancel =
    .label = Cancelar
    .accesskey = C
blocklist-button-ok =
    .label = Guardar cambios
    .accesskey = G
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Protección básica Disconnect.me (recomendada).
blocklist-item-moz-std-desc = Permitir algunos rastreadores para que los sitios web funcionen bien.
blocklist-item-moz-full-name = Protección estricta Disconnect.me.
blocklist-item-moz-full-desc = Bloquea rastreadores conocidos. Algunos sitios web pueden no funcionar correctamente.
