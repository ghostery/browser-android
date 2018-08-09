# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = 拦截列表
    .style = width: 50em
blocklist-desc = 您可以选择一个列表，{ -brand-short-name } 将依此过滤可能会跟踪您网上活动的网页元素。
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = 列表
blocklist-button-cancel =
    .label = 取消
    .accesskey = C
blocklist-button-ok =
    .label = 保存更改
    .accesskey = S
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Disconnect.me 提供的基础保护列表（推荐）。
blocklist-item-moz-std-desc = 允许部分跟踪行为，同时让网站功能保持正常。
blocklist-item-moz-full-name = Disconnect.me 提供的严格列表。
blocklist-item-moz-full-desc = 拦截所有已知的跟踪器。某些网站可能会异常。
