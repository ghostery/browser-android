# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = Списки блокировки
    .style = width: 58em
blocklist-desc = Вы можете выбрать, какой список будет использовать { -brand-short-name } для блокировки веб-элементов, которые могут отслеживать ваше поведение в Интернете.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = Список
blocklist-button-cancel =
    .label = Отмена
    .accesskey = м
blocklist-button-ok =
    .label = Сохранить изменения
    .accesskey = х
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Базовая защита Disconnect.me (Рекомендуется).
blocklist-item-moz-std-desc = Разрешает некоторые трекеры для корректной работы веб-сайтов.
blocklist-item-moz-full-name = Строгая защита Disconnect.me.
blocklist-item-moz-full-desc = Блокирует известные трекеры. Некоторые веб-сайты могут некорректно работать.
