# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

blocklist-window =
    .title = 차단 목록
    .style = width: 50em
blocklist-desc = { -brand-short-name }가 어느 목록을 사용해서 사용자의 브라우징 활동을 추적하는 웹 요소를 차단할지 선택할 수 있습니다.
blocklist-close-key =
    .key = w
blocklist-treehead-list =
    .label = 목록
blocklist-button-cancel =
    .label = 취소
    .accesskey = C
blocklist-button-ok =
    .label = 변경사항 저장
    .accesskey = S
# This template constructs the name of the block list in the block lists dialog.
# It combines the list name and description.
# e.g. "Standard (Recommended). This list does a pretty good job."
#
# Variables:
#   $listName {string, "Standard (Recommended)."} - List name.
#   $description {string, "This list does a pretty good job."} - Description of the list.
blocklist-item-list-template = { $listName } { $description }
blocklist-item-moz-std-name = Disconnect.me 기본 보호 (추천).
blocklist-item-moz-std-desc = 웹사이트가 잘 작동하도록 몇몇 추적을 허용합니다.
blocklist-item-moz-full-name = Disconnect.me 강한 보호.
blocklist-item-moz-full-desc = 알려진 추적을 차단합니다. 어떤 웹사이트는 제대로 작동하지 않을 수 있습니다.
