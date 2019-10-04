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
        [windows] { -brand-short-name } 설정
       *[other] { -brand-short-name } 설정
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
detail-contributions-button = 공헌하기
    .title = 이 부가기능의 개발에 기여합니다
    .accesskey = C
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
# Used as a description for the option to allow or block an add-on in private windows.
detail-private-browsing-label = 사생활 보호 모드에서 실행
detail-private-browsing-description2 = 허용되면, 확장 기능은 사생활 보호 브라우징 중에 사용자 온라인 활동에 접근할 수 있습니다. <label data-l10n-name="detail-private-browsing-learn-more">더 알아보기</label>
# Some add-ons may elect to not run in private windows by setting incognito: not_allowed in the manifest.  This
# cannot be overridden by the user.
detail-private-disallowed-label = 사생활 보호 창에서 허용안됨
detail-private-disallowed-description = 이 확장기능은 사생활 보호 모드에서는 실행되지 않습니다. <label data-l10n-name="detail-private-browsing-learn-more">더 알아보기</label>
# Some special add-ons are privileged, run in private windows automatically, and this permission can't be revoked
detail-private-required-label = 사생활 보호 창에 대한 접근 필요
detail-private-required-description = 이 확장기능은 사생활 보호모드에서 온라인 활동에 접근 할 수 있습니다. <label data-l10n-name="detail-private-browsing-learn-more">더 알아보기</label>
detail-private-browsing-on =
    .label = 허용
    .tooltiptext = 사생활 보호 모드에서 사용
detail-private-browsing-off =
    .label = 허용하지 않음
    .tooltiptext = 사생활 보호 모드에서 사용 안 함
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
private-browsing-description2 =
    { -brand-short-name }가 사생활 보호 모드에서 확장 기능이 작동하는 방식을 바꾸고 있습니다.
    { -brand-short-name }에 추가되는 모든 새 확장 기능은 기본적으로 사생활 보호 모드에서 실행되지 않습니다. 만약 사용자가 설정에서 허용하지 않는다면, 확장 기능은 사생활 보호 모드에서 작동하지 않으며, 사용자 온라인 활동에 접근할 수 없습니다.
    사용자의 사생활 보호 모드를 비공개로 유지 하기 위해 이렇게 변경 했습니다.
    <label data-l10n-name="private-browsing-learn-more">확장 기능 설정 관리 방법 알아보기</label>
extensions-view-discopane =
    .name = 추천
    .tooltiptext = { extensions-view-discopane.name }
extensions-view-recent-updates =
    .name = 최근 업데이트
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
    .label = 최근 업데이트 보기
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
    .label = 모든 부가기능을 자동 업데이트로 재설정
    .accesskey = R
extensions-updates-reset-updates-to-manual =
    .label = 모든 부가기능을 수동 업데이트로 재설정
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
    .value = 업데이트 없음
extensions-updates-manual-updates-found =
    .label = 업데이트 가능 항목 보기
extensions-updates-update-selected =
    .label = 업데이트 설치
    .tooltiptext = 현재 목록 내 업데이트 설치하기

## Extension shortcut management

manage-extensions-shortcuts =
    .label = 확장기능 단축키 관리
    .accesskey = S
shortcuts-no-addons = 활성화된 확장기능이 없습니다.
shortcuts-no-commands = 다음 확장기능에는 단축키가 없습니다:
shortcuts-input =
    .placeholder = 단축키 입력
shortcuts-browserAction = 확장기능 활성화
shortcuts-pageAction = 페이지 동작 활성화
shortcuts-sidebarAction = 사이드바 열고 닫기
shortcuts-modifier-mac = Ctrl, Alt 또는 ⌘ 포함
shortcuts-modifier-other = Ctrl 또는 Alt 포함
shortcuts-invalid = 잘못된 조합
shortcuts-letter = 문자 입력
shortcuts-system = { -brand-short-name } 단축키를 재정의 할 수 없음
# String displayed in warning label when there is a duplicate shortcut
shortcuts-duplicate = 중복 단축키
# String displayed when a keyboard shortcut is already assigned to more than one add-on
# Variables:
#   $shortcut (string) - Shortcut string for the add-on
shortcuts-duplicate-warning-message = { $shortcut }이 하나 이상의 경우에 단축키로 사용되고 있습니다. 단축키가 중복되면 예상치 못한 동작이 발생할 수 있습니다.
# String displayed when a keyboard shortcut is already used by another add-on
# Variables:
#   $addon (string) - Name of the add-on
shortcuts-exists = 이미 { $addon }에서 사용 중입니다.
shortcuts-card-expand-button =
    { $numberToShow ->
       *[other] { $numberToShow }개 더 보기
    }
shortcuts-card-collapse-button = 간략히 보기
go-back-button =
    .tooltiptext = 뒤로 가기

## Recommended add-ons page

# Explanatory introduction to the list of recommended add-ons. The action word
# ("recommends") in the final sentence is a link to external documentation.
discopane-intro = 확장기능 및 테마는 브라우저 앱과 비슷하며, 비밀번호 보호, 동영상 다운로드, 거래 찾기, 성가신 광고 차단, 브라우저가 어떻게 보이는지 변경 등을 할 수 있도록 합니다. 이 작은 소프트웨어 프로그램은 보통 제3자에 의해 개발됩니다. 다음은 탁월한 보안, 성능 및 기능을 위해 { -brand-product-name }가 <a data-l10n-name="learn-more-trigger">추천하는</a> 목록입니다.
# Notice to make user aware that the recommendations are personalized.
discopane-notice-recommendations = 이러한 추천 중 일부는 개인화된 것입니다. 설치한 다른 확장기능, 프로필 설정 및 사용 통계를 기반으로 합니다.
discopane-notice-learn-more = 더 알아보기
privacy-policy = 개인 정보 보호 정책
# Refers to the author of an add-on, shown below the name of the add-on.
# Variables:
#   $author (string) - The name of the add-on developer.
created-by-author = 제작자: <a data-l10n-name="author">{ $author }</a>
# Shows the number of daily users of the add-on.
# Variables:
#   $dailyUsers (number) - The number of daily users.
user-count = 사용자: { $dailyUsers }
install-extension-button = { -brand-product-name }에 추가
install-theme-button = 테마 설치하기
# The label of the button that appears after installing an add-on. Upon click,
# the detailed add-on view is opened, from where the add-on can be managed.
manage-addon-button = 관리
find-more-addons = 부가기능 더 찾기

## Add-on actions

report-addon-button = 신고하기
remove-addon-button = 제거
disable-addon-button = 비활성화
enable-addon-button = 활성화
expand-addon-button = 옵션 더보기
preferences-addon-button =
    { PLATFORM() ->
        [windows] 설정
       *[other] 설정
    }
details-addon-button = 상세 정보
release-notes-addon-button = 출시 정보
permissions-addon-button = 권한
addons-enabled-heading = 활성화 됨
addons-disabled-heading = 비활성화 됨
extension-enabled-heading = 사용함
extension-disabled-heading = 사용 안 함
theme-enabled-heading = 사용함
theme-disabled-heading = 사용 안 함
plugin-enabled-heading = 사용함
plugin-disabled-heading = 사용 안 함
dictionary-enabled-heading = 사용함
dictionary-disabled-heading = 사용 안 함
locale-enabled-heading = 사용함
locale-disabled-heading = 사용 안 함
ask-to-activate-button = 활성화 요청
always-activate-button = 항상 사용
never-activate-button = 사용 안함
addon-detail-author-label = 제작자
addon-detail-version-label = 버전
addon-detail-last-updated-label = 최종 수정일
addon-detail-homepage-label = 홈페이지
addon-detail-rating-label = 평가
# The average rating that the add-on has received.
# Variables:
#   $rating (number) - A number between 0 and 5. The translation should show at most one digit after the comma.
five-star-rating =
    .title = 평점: { NUMBER($rating, maximumFractionDigits: 1) } / 5
# This string is used to show that an add-on is disabled.
# Variables:
#   $name (string) - The name of the add-on
addon-name-disabled = { $name } (사용 안 함)
# The number of reviews that an add-on has received on AMO.
# Variables:
#   $numberOfReviews (number) - The number of reviews received
addon-detail-reviews-link =
    { $numberOfReviews ->
       *[other] { $numberOfReviews }개의 리뷰
    }

## Pending uninstall message bar

# Variables:
#   $addon (string) - Name of the add-on
pending-uninstall-description = <span data-l10n-name="addon-name">{ $addon }</span>가 제거되었습니다.
pending-uninstall-undo-button = 실행 취소
addon-detail-updates-label = 자동 업데이트 허용
addon-detail-updates-radio-default = 기본값
addon-detail-updates-radio-on = 켜기
addon-detail-updates-radio-off = 끄기
addon-detail-update-check-label = 업데이트 확인
install-update-button = 업데이트
# This is the tooltip text for the private browsing badge in about:addons. The
# badge is the private browsing icon included next to the extension's name.
addon-badge-private-browsing-allowed =
    .title = 사생활 보호 창에서 허용됨
addon-detail-private-browsing-help = 허용된 경우, 확장기능은 사생활 보호 브라우징 중에 온라인 활동에 접근 할 수 있습니다. <a data-l10n-name="learn-more">더 알아보기</a>
addon-detail-private-browsing-allow = 허용
addon-detail-private-browsing-disallow = 허용하지 않음
# This is the tooltip text for the recommended badge for an extension in about:addons. The
# badge is a small icon displayed next to an extension when it is recommended on AMO.
addon-badge-recommended =
    .title = 추천
    .alt = 추천
available-updates-heading = 업데이트 가능 항목
recent-updates-heading = 최근 업데이트
release-notes-loading = 읽는 중…
release-notes-error = 죄송합니다. 출시 정보를 읽어오는 중에 오류가 발생했습니다.
addon-permissions-empty = 이 확장 기능에는 권한이 필요하지 않습니다.
recommended-extensions-heading = 추천 확장 기능
recommended-themes-heading = 추천 테마
# A recommendation for the Firefox Color theme shown at the bottom of the theme
# list view. The "Firefox Color" name itself should not be translated.
recommended-theme-1 = 창의적인 느낌이 떠오르시나요? <a data-l10n-name="link">Firefox Color로 나만의 테마를 만들어 보십시오.</a>
