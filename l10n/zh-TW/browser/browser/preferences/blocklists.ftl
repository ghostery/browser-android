# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = 封鎖清單
    .style = width: 55em
blocklist-desc = 您可以選擇 { -brand-short-name } 要依照哪個清單來封鎖可能會追蹤您的線上行為的網頁元素。
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = 清單
blocklist-button-cancel =
    .label = 取消
    .accesskey = C
blocklist-button-ok =
    .label = 儲存變更
    .accesskey = S
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Disconnect.me 基礎保護（推薦）。
blocklist-item-moz-std-desc = 允許部分追蹤者，讓網站功能保持正常。
blocklist-item-moz-full-name = Disconnect.me 嚴格保護。
blocklist-item-moz-full-desc = 封鎖所有已知的追蹤者，某些網站可能無法正常使用。
