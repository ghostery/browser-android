# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## These messages are used as headings in the recommendation doorhanger

cfr-doorhanger-extension-heading = 추천 확장기능
cfr-doorhanger-feature-heading = 추천 기능
cfr-doorhanger-pintab-heading = 사용해보기: 탭 고정



cfr-doorhanger-extension-sumo-link =
    .tooltiptext = 왜 이게 나왔나요
cfr-doorhanger-extension-cancel-button = 나중에
    .accesskey = N
cfr-doorhanger-extension-ok-button = 지금 추가
    .accesskey = A
cfr-doorhanger-pintab-ok-button = 이 탭 고정
    .accesskey = P
cfr-doorhanger-extension-manage-settings-button = 추천 설정 관리
    .accesskey = M
cfr-doorhanger-extension-never-show-recommendation = 이 추천을 보여주지 않음
    .accesskey = S
cfr-doorhanger-extension-learn-more-link = 자세히 보기
# This string is used on a new line below the add-on name
# Variables:
#   $name (String) - Add-on author name
cfr-doorhanger-extension-author = { $name } 제작
# This is a notification displayed in the address bar.
# When clicked it opens a panel with a message for the user.
cfr-doorhanger-extension-notification = 추천

## Add-on statistics
## These strings are used to display the total number of
## users and rating for an add-on. They are shown next to each other.

# Variables:
#   $total (Number) - The rating of the add-on from 1 to 5
cfr-doorhanger-extension-rating =
    .tooltiptext =
        { $total ->
           *[other] { $total } 점
        }
# Variables:
#   $total (Number) - The total number of users using the add-on
cfr-doorhanger-extension-total-users =
    { $total ->
       *[other] { $total } 사용자
    }
cfr-doorhanger-pintab-description = 가장 많이 사용하는 사이트에 쉽게 접근하세요. 사이트를 탭으로 열어 둡니다(다시 시작할 때에도).

## These messages are steps on how to use the feature and are shown together.

cfr-doorhanger-pintab-step1 = 고정하려는 탭에서 <b>마우스 오른쪽 버튼을 클릭</b>하세요.
cfr-doorhanger-pintab-step2 = 메뉴에서 <b>탭 고정</b>을 선택하세요.
cfr-doorhanger-pintab-step3 = 사이트에 업데이트가 있으면 고정된 탭에 파란색 점이 표시됩니다.
cfr-doorhanger-pintab-animation-pause = 일시 중지
cfr-doorhanger-pintab-animation-resume = 계속

## Firefox Accounts Message

cfr-doorhanger-bookmark-fxa-header = 어디서나 북마크 동기화
cfr-doorhanger-bookmark-fxa-body = 멋진 발견! 이제 다른 휴대 기기에서도 이 북마크를 사용해 보세요. { -fxaccount-brand-name }로 시작해 보세요.
cfr-doorhanger-bookmark-fxa-link-text = 북마크 지금 동기화…
cfr-doorhanger-bookmark-fxa-close-btn-tooltip =
    .aria-label = 닫기 버튼
    .title = 닫기

## Protections panel

cfr-protections-panel-header = 추적되지 않는 탐색
cfr-protections-panel-body = 자신의 데이터를 보호하세요. { -brand-short-name }는 온라인에서 하는 일을 추적하는 가장 일반적인 많은 추적기로부터 사용자를 보호합니다.
cfr-protections-panel-link-text = 더 알아보기

## What's New toolbar button and panel

cfr-whatsnew-button =
    .label = 새로운 기능
    .tooltiptext = 새로운 기능
cfr-whatsnew-panel-header = 새로운 기능

## Bookmark Sync

cfr-doorhanger-sync-bookmarks-header = 이 북마크를 휴대폰에서 사용합니다
cfr-doorhanger-sync-bookmarks-body = { -brand-product-name }에 로그인한 모든 곳에서 북마크, 비밀번호, 방문 기록 등을 가져옵니다.
cfr-doorhanger-sync-bookmarks-ok-button = { -sync-brand-short-name } 켜기
    .accesskey = T

## Login Sync

cfr-doorhanger-sync-logins-header = 다시는 비밀번호를 잃어버리지 마십시오
cfr-doorhanger-sync-logins-body = 비밀번호를 모든 기기에 안전하게 저장하고 동기화합니다.
cfr-doorhanger-sync-logins-ok-button = { -sync-brand-short-name } 켜기
    .accesskey = T

## Send Tab

cfr-doorhanger-send-tab-header = 이동 중에 이것을 읽어보세요
cfr-doorhanger-send-tab-recipe-header = 이 요리법을 주방으로 가져가세요
cfr-doorhanger-send-tab-body = 탭 보내기를 사용하면 이 링크를 휴대폰 또는 { -brand-product-name }에 로그인 한 곳 어디에서나 쉽게 공유할 수 있습니다.
cfr-doorhanger-send-tab-ok-button = 탭 보내기 사용해보기
    .accesskey = T

## Firefox Send

cfr-doorhanger-firefox-send-header = 이 PDF를 안전하게 공유하세요
cfr-doorhanger-firefox-send-body = 종단 간 암호화와 완료시 사라지는 링크를 사용하여 중요한 문서를 안전하게 보관할 수 있습니다.
cfr-doorhanger-firefox-send-ok-button = { -send-brand-name } 사용해보기
    .accesskey = T
