# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# An old map warning, see https://en.wikipedia.org/wiki/Here_be_dragons
about-config-warning-title = 위험한 곳입니다!
about-config-warning-text = 이 고급 설정을 변경하면 이 프로그램의 안정성, 보안 및 성능에 해가 될 수 있습니다. 당신이 무엇을 하고 있는지 확신하는 경우에만 계속해야 합니다.
about-config-warning-checkbox = 다음에 이 경고 계속 표시하기
about-config-warning-button = 위험을 감수하겠습니다!
about-config-title = about:config
about-config2-title = 고급 설정
about-config-search-input =
    .placeholder = 검색
about-config-show-all = 모두 보기
about-config-pref-add = 추가
about-config-pref-toggle = 토글
about-config-pref-edit = 편집
about-config-pref-save = 저장
about-config-pref-reset = 초기화
about-config-pref-delete = 삭제

## Labels for the type selection radio buttons shown when adding preferences.

about-config-pref-add-type-boolean = 불린
about-config-pref-add-type-number = 숫자
about-config-pref-add-type-string = 문자열

## Preferences with a non-default value are differentiated visually, and at the
## same time the state is made accessible to screen readers using an aria-label
## that won't be visible or copied to the clipboard.
##
## Variables:
##   $value (String): The full value of the preference.

about-config-pref-accessible-value-default =
    .aria-label = { $value } (기본값)
about-config-pref-accessible-value-custom =
    .aria-label = { $value } (사용자 지정)
