# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## Settings

site-data-settings-window =
    .title = 쿠키와 사이트 데이터 관리
site-data-settings-description = 다음의 웹사이트가 컴퓨터에 쿠키와 사이트 데이터를 저장합니다. 사용자가 삭제하고 웹사이트에서 여유 공간이 필요해서 비영구 공간을 삭제할 때 까지 { -brand-short-name }가 웹사이트의 데이터를 영구적인 저장소로 유지합니다.
site-data-search-textbox =
    .placeholder = 웹사이트 검색
    .accesskey = S
site-data-column-host =
    .label = 사이트
site-data-column-cookies =
    .label = 쿠키
site-data-column-storage =
    .label = 저장소
site-data-column-last-used =
    .label = 마지막 사용
site-data-remove-selected =
    .label = 선택된 값 삭제
    .accesskey = r
site-data-button-cancel =
    .label = 취소
    .accesskey = C
site-data-button-save =
    .label = 변경 내용 저장
    .accesskey = a
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
site-usage-pattern = { $value } { $unit }
site-usage-persistent = { site-usage-pattern } (영구)
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
site-storage-usage =
    .value = { $value } { $unit }
site-storage-persistent =
    .value = { site-storage-usage.value } (영구)
site-data-remove-all =
    .label = 모두 삭제
    .accesskey = e
site-data-remove-shown =
    .label = 보여지는 값 모두 삭제
    .accesskey = e

## Removing

site-data-removing-dialog =
    .title = { site-data-removing-header }
    .buttonlabelaccept = 삭제
site-data-removing-header = 쿠키와 사이트 데이터 삭제
site-data-removing-desc = 쿠키와 사이트 데이터를 삭제하면 웹사이트에서 로그아웃 될 수 있습니다. 변경하시겠습니까?
site-data-removing-table = 다음 사이트의 쿠키와 사이트 데이터가 삭제됨
