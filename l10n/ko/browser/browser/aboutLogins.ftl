# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

about-logins-page-title = 로그인과 비밀번호

# "Google Play" and "App Store" are both branding and should not be translated

login-app-promo-title = 어디에서나 비밀번호를 사용하십시오
login-app-promo-subtitle = 무료 { -lockwise-brand-name } 앱을 받으세요
login-app-promo-android =
    .alt = Google Play에서 받기
login-app-promo-apple =
    .alt = App Store에서 다운로드
login-filter =
    .placeholder = 로그인 검색
create-login-button = 새 로그인 만들기
# This string is used as alternative text for favicon images.
# Variables:
#   $title (String) - The title of the website associated with the favicon.
login-favicon =
    .alt = { $title }의 파비콘
fxaccounts-sign-in-text = 다른 기기에서 비밀번호 받기
fxaccounts-sign-in-button = { -sync-brand-short-name }에 로그인
fxaccounts-avatar-button =
    .title = 계정 관리

## The ⋯ menu that is in the top corner of the page

menu =
    .title = 메뉴 열기
# This menuitem is only visible on Windows
menu-menuitem-import = 비밀번호 가져오기…
menu-menuitem-preferences =
    { PLATFORM() ->
        [windows] 설정
       *[other] 설정
    }
menu-menuitem-feedback = 의견 보내기
menu-menuitem-faq = 자주 묻는 질문
menu-menuitem-android-app = Android 용 { -lockwise-brand-short-name }
menu-menuitem-iphone-app = iPhone과 iPad 용 { -lockwise-brand-short-name }

## Login List

login-list =
    .aria-label = 검색어와 일치하는 로그인
login-list-count =
    { $count ->
       *[other] { $count }회 로그인
    }
login-list-sort-label-text = 정렬 방식:
login-list-name-option = 이름 (A-Z)
login-list-breached-option = 유출된 웹 사이트
login-list-last-changed-option = 마지막 수정
login-list-last-used-option = 마지막 사용
login-list-intro-title = 로그인을 찾을 수 없음
login-list-intro-description = { -brand-product-name }에 비밀번호를 저장하면, 여기에 표시됩니다.
login-list-item-title-new-login = 새 로그인
login-list-item-subtitle-new-login = 로그인 자격 증명을 입력하십시오
login-list-item-subtitle-missing-username = (사용자 이름 없음)

## Introduction screen

login-intro-heading = 저장된 로그인을 찾으십니까? { -sync-brand-short-name }를 설정하십시오.
login-intro-description = 다른 기기에 { -brand-product-name }의 로그인을 저장한 경우, 다음 방법을 참조하시기 바랍니다:
login-intro-instruction-fxa = 로그인이 저장된 기기에서 { -fxaccount-brand-name }을 만들거나 로그인하십시오
login-intro-instruction-fxa-settings = { -sync-brand-short-name } 설정에서 로그인 확인란을 선택했는지 확인하십시오
login-intro-instruction-faq = 자세한 내용은 { -lockwise-brand-short-name } <a data-l10n-name="faq">자주 묻는 질문</a>을 참조하시기 바랍니다

## Login

login-item-new-login-title = 새 로그인 만들기
login-item-edit-button = 편집
login-item-delete-button = 삭제
login-item-origin-label = 웹 사이트 주소
login-item-origin =
    .placeholder = https://www.example.com
login-item-open-site-button = 실행
login-item-username-label = 사용자 이름
login-item-username =
    .placeholder = name@example.com
login-item-copy-username-button-text = 복사
login-item-copied-username-button-text = 복사됨!
login-item-password-label = 비밀번호
login-item-password-reveal-checkbox-show =
    .title = 비밀번호 표시
login-item-password-reveal-checkbox-hide =
    .title = 비밀번호 숨기기
login-item-copy-password-button-text = 복사
login-item-copied-password-button-text = 복사됨!
login-item-save-changes-button = 변경사항 저장
login-item-save-new-button = 저장
login-item-cancel-button = 취소
login-item-time-changed = 마지막 수정: { DATETIME($timeChanged, day: "numeric", month: "long", year: "numeric") }
login-item-time-created = 생성: { DATETIME($timeCreated, day: "numeric", month: "long", year: "numeric") }
login-item-time-used = 마지막 사용: { DATETIME($timeUsed, day: "numeric", month: "long", year: "numeric") }

## Master Password notification

master-password-notification-message = 저장된 로그인과 비밀번호를 보려면 기본 비밀번호를 입력하십시오
master-password-reload-button =
    .label = 로그인
    .accesskey = L

## Dialogs

confirmation-dialog-cancel-button = 취소
confirmation-dialog-dismiss-button =
    .title = 취소
enable-password-sync-notification-message =
    { PLATFORM() ->
        [windows] { -brand-product-name }를 사용하는 모든 곳에서 로그인을 원하십니까? { -sync-brand-short-name } 설정으로 이동하여 로그인 확인란을 선택하십시오.
       *[other] { -brand-product-name }를 사용하는 모든 곳에서 로그인을 원하십니까? { -sync-brand-short-name } 설정으로 이동하여 로그인 확인란을 선택하십시오.
    }
enable-password-sync-preferences-button =
    .label =
        { PLATFORM() ->
            [windows] { -sync-brand-short-name } 설정 방문
           *[other] { -sync-brand-short-name } 설정 방문
        }
    .accesskey = V
confirm-delete-dialog-title = 이 로그인을 삭제하시겠습니까?
confirm-delete-dialog-message = 이 작업은 취소할 수 없습니다.
confirm-delete-dialog-confirm-button = 삭제
confirm-discard-changes-dialog-title = 저장되지 않은 변경 사항을 버리시겠습니까?
confirm-discard-changes-dialog-message = 저장되지 않은 모든 변경 사항을 잃게 됩니다.
confirm-discard-changes-dialog-confirm-button = 버리기

## Breach Alert notification

breach-alert-text = 마지막 로그인 정보를 업데이트 한 후 이 웹 사이트에서 비밀번호가 유출되거나 도난당했습니다. 계정을 보호하기 위해 비밀번호를 변경하십시오.
breach-alert-link = 이 유출에 대해 자세히 알아보기.
breach-alert-dismiss =
    .title = 이 알림 닫기
