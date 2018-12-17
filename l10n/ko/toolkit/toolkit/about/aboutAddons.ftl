# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

addons-window =
    .title = 부가 기능 관리자
search-header =
    .placeholder = addons.mozilla.org 검색
    .searchbuttonlabel = 검색
search-header-shortcut =
    .key = f
loading-label =
    .value = 읽는 중…
list-empty-installed =
    .value = 설치한 부가 기능이 없음
list-empty-available-updates =
    .value = 업데이트 없음
list-empty-recent-updates =
    .value = 부가 기능에 대한 업데이트가 없습니다.
list-empty-find-updates =
    .label = 업데이트 확인
list-empty-button =
    .label = 부가 기능 자세히 보기
install-addon-from-file =
    .label = 파일에서 부가 기능 설치…
    .accesskey = I
help-button = 부가 기능 지원
preferences =
    { PLATFORM() ->
        [windows] { -brand-short-name } 옵션
       *[other] { -brand-short-name } 환경 설정
    }
tools-menu =
    .tooltiptext = 부가 기능 도구
show-unsigned-extensions-button =
    .label = 몇몇 확장기능은 확인을 할 수 없음
show-all-extensions-button =
    .label = 모든 확장기능 보기
debug-addons =
    .label = 부가 기능 디버그
    .accesskey = B
cmd-show-details =
    .label = 자세히 보기
    .accesskey = S
cmd-find-updates =
    .label = 업데이트 찾기
    .accesskey = F
cmd-preferences =
    .label =
        { PLATFORM() ->
            [windows] 설정
           *[other] 환경 설정
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
cmd-enable-theme =
    .label = 테마 적용
    .accesskey = W
cmd-disable-theme =
    .label = 테마 적용 중지
    .accesskey = W
cmd-install-addon =
    .label = 설치
    .accesskey = I
cmd-contribute =
    .label = 공헌
    .accesskey = C
    .tooltiptext = 부가 기능 개발 참여하기
discover-title = 부가 기능 소개
discover-description = 부가 기능은 { -brand-short-name }를 여러분의 취향에 따라 맞춤 기능을 만들어 주는 애플리케이션 혹은 테마입니다. 메일 혹은 날씨 알림, 원하는 스타일 등 다양한 기능을 제공합니다.
discover-footer = 인터넷에 연결할 때 최신 추천 부가 기능을 소개해 드립니다.
detail-version =
    .label = 버전
detail-last-updated =
    .label = 최종 수정일
detail-contributions-description = 이 부가 기능 개발자에게 작은 도움을 통해 개발이 지속될 수 있도록 지원할 수 있습니다.
detail-update-type =
    .value = 자동 업데이트
detail-update-default =
    .label = 기본 설정
    .tooltiptext = 기본으로 업데이트를 자동으로 설치
detail-update-automatic =
    .label = 사용
    .tooltiptext = 업데이트 자동 설치 설정
detail-update-manual =
    .label = 중단
    .tooltiptext = 업데이트 자동 설치 중단
detail-home =
    .label = 홈페이지
detail-home-value =
    .value = { detail-home.label }
detail-repository =
    .label = 부가 기능 프로필
detail-repository-value =
    .value = { detail-repository.label }
detail-check-for-updates =
    .label = 업데이트 확인
    .accesskey = F
    .tooltiptext = 현재 부가 기능 업데이트 확인
detail-show-preferences =
    .label =
        { PLATFORM() ->
            [windows] 설정
           *[other] 환경 설정
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
    .tooltiptext =
        { PLATFORM() ->
            [windows] 부가 기능 설정 변경
           *[other] 부가 기능 환경 설정 변경
        }
detail-rating =
    .value = 평가
addon-restart-now =
    .label = 다시 시작
disabled-unsigned-heading =
    .value = 몇몇 부가 기능이 비활성화 됨
disabled-unsigned-description = 다음 부가기능은 { -brand-short-name }에서의 사용이 확인되지 않았습니다. <label data-l10n-name="find-addons">대체제를 검색</label>하거나 개발자가 확인을 받도록 요청할 수 있습니다.
disabled-unsigned-learn-more = 사용자가 온라인에서 안전할 수 있게 노력하는 내용에 대해서 더 알아보세요.
disabled-unsigned-devinfo = 부가기능을 확인하는데 관심이 있는 개발자는 <label data-l10n-name="learn-more">메뉴얼</label>을 읽어보세요.
plugin-deprecation-description = 빠진게 있나요? 어떤 플러그인은 { -brand-short-name }에서 더이상 지원하지 않습니다. <label data-l10n-name="learn-more">더 알아보기.</label>
legacy-warning-show-legacy = 오래된 확장 기능 보기
legacy-extensions =
    .value = 오래된 확장 기능
legacy-extensions-description = 이 확장 기능들은 현재 { -brand-short-name } 표준에 맞지 않으므로 비활성화되었습니다. <label data-l10n-name="legacy-learn-more">부가 기능의 변경 내용 알아보기</label>
extensions-view-discover =
    .name = 추천
    .tooltiptext = { extensions-view-discover.name }
extensions-view-recent-updates =
    .name = 최근 항목 보기
    .tooltiptext = { extensions-view-recent-updates.name }
extensions-view-available-updates =
    .name = 업데이트 가능 항목
    .tooltiptext = { extensions-view-available-updates.name }

## These are global warnings

extensions-warning-safe-mode-label =
    .value = 안전 모드에서 부가 기능 전체를 사용할 수 없습니다.
extensions-warning-safe-mode-container =
    .tooltiptext = { extensions-warning-safe-mode-label.value }
extensions-warning-check-compatibility-label =
    .value = 부가 기능 호환성 확인 기능을 중단 합니다. 호환성이 맞지 않을 수 있습니다.
extensions-warning-check-compatibility-container =
    .tooltiptext = { extensions-warning-check-compatibility-label.value }
extensions-warning-check-compatibility-enable =
    .label = 활성화
    .tooltiptext = 부가 기능 호환성 확인 기능 사용
extensions-warning-update-security-label =
    .value = 부가 기능 업데이트 보안 확인 기능을 중단 합니다. 업데이트가 보안 문제를 일으킬 수 있습니다.
extensions-warning-update-security-container =
    .tooltiptext = { extensions-warning-update-security-label.value }
extensions-warning-update-security-enable =
    .label = 취소
    .tooltiptext = 부가 기능 업데이트 보안 확인 기능 사용

## Strings connected to add-on updates

extensions-updates-check-for-updates =
    .label = 업데이트 확인하기
    .accesskey = C
extensions-updates-view-updates =
    .label = 최신 업데이트 보기
    .accesskey = V

# This menu item is a checkbox that toggles the default global behavior for
# add-on update checking.

extensions-updates-update-addons-automatically =
    .label = 자동 업데이트 하기
    .accesskey = A

## Specific add-ons can have custom update checking behaviors ("Manually",
## "Automatically", "Use default global behavior"). These menu items reset the
## update checking behavior for all add-ons to the default global behavior
## (which itself is either "Automatically" or "Manually", controlled by the
## extensions-updates-update-addons-automatically.label menu item).

extensions-updates-reset-updates-to-automatic =
    .label = 자동 업데이트 설정
    .accesskey = R
extensions-updates-reset-updates-to-manual =
    .label = 자동 업데이트 설정 해제
    .accesskey = R

## Status messages displayed when updating add-ons

extensions-updates-updating =
    .value = 업데이트 확인 중
extensions-updates-installed =
    .value = 업데이트가 완료되었습니다.
extensions-updates-downloaded =
    .value = 부가 기능 업데이트를 다운로드 완료 했습니다.
extensions-updates-restart =
    .label = 설치를 완료하려면 다시 시작 하십시오.
extensions-updates-none-found =
    .value = 최신 업데이트 없음
extensions-updates-manual-updates-found =
    .label = 업데이트 가능 항목 보기
extensions-updates-update-selected =
    .label = 업데이트 설치
    .tooltiptext = 현재 목록 내 업데이트 설치하기
