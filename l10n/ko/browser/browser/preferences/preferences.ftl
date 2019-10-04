# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = 웹사이트에 “방문자 추적 금지” 신호를 보내서 추적을 원하지 않는다고 알림
do-not-track-learn-more = 자세히 보기
do-not-track-option-default-content-blocking-known =
    .label = { -brand-short-name }가 알려진 추적기를 차단하도록 설정 됐을 때만
do-not-track-option-always =
    .label = 항상
pref-page =
    .title =
        { PLATFORM() ->
            [windows] 설정
           *[other] 환경 설정
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Please keep the placeholder string short to avoid truncation.
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input-box =
    .style = width: 15.4em
    .placeholder =
        { PLATFORM() ->
            [windows] 옵션에서 찾기
           *[other] 설정에서 찾기
        }
managed-notice = 속한 회사나 단체에서 브라우저를 관리하고 있습니다.
pane-general-title = 일반
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = 홈
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = 검색
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = 개인 정보 및 보안
category-privacy =
    .tooltiptext = { pane-privacy-title }
pane-sync-title2 = { -sync-brand-short-name }
category-sync2 =
    .tooltiptext = { pane-sync-title2 }
help-button-label = { -brand-short-name } 도움말
addons-button-label = 확장기능과 테마
focus-search =
    .key = f
close-button =
    .aria-label = 닫기

## Browser Restart Dialog

feature-enable-requires-restart = 이 기능을 켜려면 { -brand-short-name }를 반드시 재시작해야 합니다.
feature-disable-requires-restart = 이 기능을 끄려면 { -brand-short-name }를 반드시 재시작해야 힙니다.
should-restart-title = { -brand-short-name } 재시작
should-restart-ok = 지금 { -brand-short-name } 재시작
cancel-no-restart-button = 취소
restart-later = 나중에 다시 시작

## Extension Control Notifications
##
## These strings are used to inform the user
## about changes made by extensions to browser settings.
##
## <img data-l10n-name="icon"/> is going to be replaced by the extension icon.
##
## Variables:
##   $name (String): name of the extension

# This string is shown to notify the user that their home page
# is being controlled by an extension.
extension-controlled-homepage-override = <img data-l10n-name="icon"/> { $name } 확장기능이 홈페이지를 제어하고 있습니다.
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = <img data-l10n-name="icon"/> { $name } 확장기능이 새 탭 페이지를 제어하고 있습니다.
# This string is shown to notify the user that their notifications permission
# is being controlled by an extension.
extension-controlled-web-notifications = <img data-l10n-name="icon"/> { $name } 확장기능이 이 설정을 제어하고 있습니다.
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = <img data-l10n-name="icon"/> { $name } 확장기능이 기본 검색 엔진을 설정했습니다.
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = <img data-l10n-name="icon"/> { $name } 확장기능에는 컨테이너 탭이 필요합니다.
# This string is shown to notify the user that their content blocking "All Detected Trackers"
# preferences are being controlled by an extension.
extension-controlled-websites-content-blocking-all-trackers = <img data-l10n-name="icon"/> { $name } 확장기능이 이 설정을 제어하고 있습니다.
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = <img data-l10n-name="icon"/> { $name } 확장기능이 { -brand-short-name }가 인터넷에 접근하는 방법을 제어하고 있습니다.
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = 확장 프로그램을 사용하려면 <img data-l10n-name="menu-icon"/> 메뉴에서 <img data-l10n-name="addons-icon"/> 부가 기능으로 이동하십시오.

## Preferences UI Search Results

search-results-header = 검색 결과
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] 죄송합니다! “<span data-l10n-name="query"></span>”옵션에 대한 결과가 없습니다.
       *[other] 죄송합니다! “<span data-l10n-name="query"></span>”설정에 대한 결과가 없습니다.
    }
search-results-help-link = 도움이 필요하세요? <a data-l10n-name="url">{ -brand-short-name } 지원</a>에 방문하세요.

## General Section

startup-header = 시작 페이지
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = { -brand-short-name }과 Firefox 같이 돌리기
use-firefox-sync = 팁: 이렇게 하면 프로필을 따로 쓰게 됩니다. { -sync-brand-short-name }를 써서 데이터를 공유하십시오.
get-started-not-logged-in = { -sync-brand-short-name }에 로그인…
get-started-configured = { -sync-brand-short-name } 설정 열기
always-check-default =
    .label = { -brand-short-name }가 기본 브라우저인지 항상 확인
    .accesskey = w
is-default = 현재 { -brand-short-name }가 기본 브라우저입니다.
is-not-default = { -brand-short-name }가 기본 브라우저가 아닙니다.
set-as-my-default-browser =
    .label = 기본값으로…
    .accesskey = D
startup-restore-previous-session =
    .label = 이전 세션 복원
    .accesskey = s
startup-restore-warn-on-quit =
    .label = 브라우저 종료시 경고
disable-extension =
    .label = 확장기능 비활성화
tabs-group-header = 탭
ctrl-tab-recently-used-order =
    .label = Ctrl+Tab으로 최근 사용한 순서대로 탭 순환
    .accesskey = T
open-new-link-as-tabs =
    .label = 링크를 새 창 대신 새 탭에서 열기
    .accesskey = w
warn-on-close-multiple-tabs =
    .label = 여러 개의 탭을 동시에 닫으려고 할 때 알려주기
    .accesskey = m
warn-on-open-many-tabs =
    .label = 여러개의 탭을 열어서 { -brand-short-name }가 느려질 수 있으면 알려주기
    .accesskey = d
switch-links-to-new-tabs =
    .label = 링크를 새 탭으로 열면 해당 탭으로 전환
    .accesskey = h
show-tabs-in-taskbar =
    .label = Windows 작업표시줄에 탭 미리 보기 표시
    .accesskey = k
browser-containers-enabled =
    .label = 컨테이너 탭 활성화
    .accesskey = n
browser-containers-learn-more = 더 알아보기
browser-containers-settings =
    .label = 설정…
    .accesskey = i
containers-disable-alert-title = 모든 컨테이너 탭을 닫으시겠습니까?
containers-disable-alert-desc = 지금 컨테이너 탭을 비활성화 하면 { $tabCount }개의 컨테이너 탭이 닫히게 됩니다. 컨테이너 탭을 비활성화 하시겠습니까?
containers-disable-alert-ok-button = { $tabCount }개의 컨테이너 탭 닫기
containers-disable-alert-cancel-button = 활성화 하기
containers-remove-alert-title = 이 컨테이너를 삭제하시겠습니까?
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg = 이 컨테이너를 삭제하면 { $count } 컨테이너 탭이 닫힙니다. 이 컨테이너를 정말로 삭제하시겠습니까?
containers-remove-ok-button = 이 컨테이너 삭제
containers-remove-cancel-button = 이 컨테이너 삭제하지 않음

## General Section - Language & Appearance

language-and-appearance-header = 언어와 표현
fonts-and-colors-header = 글꼴과 색상
default-font = 기본 글꼴
    .accesskey = D
default-font-size = 크기
    .accesskey = S
advanced-fonts =
    .label = 고급…
    .accesskey = A
colors-settings =
    .label = 색상…
    .accesskey = C
language-header = 언어
choose-language-description = 웹 페이지를 표시할 선호 언어 선택
choose-button =
    .label = 선택…
    .accesskey = o
choose-browser-language-description = { -brand-short-name }가 메뉴나 메시지, 알림을 표시하는데 사용할 언어를 선택해 주세요.
manage-browser-languages-button =
    .label = 대체 설정…
    .accesskey = I
confirm-browser-language-change-description = 변경 내용 적용을 위해 { -brand-short-name } 재시작
confirm-browser-language-change-button = 적용하고 재시작
translate-web-pages =
    .label = 웹 콘텐츠 번역하기
    .accesskey = T
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = 번역:  <img data-l10n-name="logo"/>
translate-exceptions =
    .label = 예외…
    .accesskey = x
check-user-spelling =
    .label = 입력할 때 철자 확인
    .accesskey = t

## General Section - Files and Applications

files-and-applications-title = 파일과 애플리케이션
download-header = 다운로드
download-save-to =
    .label = 저장 위치
    .accesskey = v
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] 선택…
           *[other] 찾아보기…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] e
           *[other] o
        }
download-always-ask-where =
    .label = 파일 저장 위치 항상 묻기
    .accesskey = A
applications-header = 애플리케이션
applications-description = { -brand-short-name } 가 다운로드 받은 파일이나 브라우저에서 사용하고 있는 응용프로그램에 대해 어떻게 처리할 지 선택하십시오.
applications-filter =
    .placeholder = 파일 타입 혹은 애플리케이션 검색
applications-type-column =
    .label = 파일 형식
    .accesskey = T
applications-action-column =
    .label = 실행
    .accesskey = A
drm-content-header = 디지털 권리 관리(DRM) 콘텐츠
play-drm-content =
    .label = DRM 제어 콘텐츠 재생
    .accesskey = P
play-drm-content-learn-more = 더 알아보기
update-application-title = { -brand-short-name } 업데이트
update-application-description = { -brand-short-name }가 최상의 성능, 안정성, 보안을 유지할 수 있도록 최신 버전으로 유지합니다.
update-application-version = 버전 { $version } <a data-l10n-name="learn-more">새로운 기능</a>
update-history =
    .label = 업데이트 기록 보기…
    .accesskey = p
update-application-allow-description = { -brand-short-name } 설치 방법
update-application-auto =
    .label = 자동으로 업데이트 설치(추천)
    .accesskey = A
update-application-check-choose =
    .label = 새로운 업데이트를 확인하지만 설치는 수동으로 진행
    .accesskey = C
update-application-manual =
    .label = 업데이트 확인 안함(권장하지 않음)
    .accesskey = N
update-application-warning-cross-user-setting = 이 설정은 이 { -brand-short-name } 설치를 사용하는 모든 윈도우즈 계정 및 { -brand-short-name } 프로필에 적용됩니다.
update-application-use-service =
    .label = 업데이트 설치 시 백그라운드 작업으로 하기
    .accesskey = b
update-enable-search-update =
    .label = 검색 엔진 자동 업데이트
    .accesskey = e
update-pref-write-failure-title = 쓰기 실패
# Variables:
#   $path (String) - Path to the configuration file
update-pref-write-failure-message = 환경 설정을 저장할 수 없습니다. 파일에 쓸 수 없습니다: { $path }
update-setting-write-failure-title = 업데이트 설정 저장 중 오류 발생
# Variables:
#   $path (String) - Path to the configuration file
# The newlines between the main text and the line containing the path is
# intentional so the path is easier to identify.
update-setting-write-failure-message =
    오류가 발생하여 { -brand-short-name }가 이 변경 내용을 저장하지 않았습니다. 이 업데이트 환경 설정을 하려면 아래 파일에 쓰기 권한이 필요합니다. 사용자나 시스템 관리자가 사용자 그룹에 이 파일에 대한 모든 권한을 부여하여 오류를 해결할 수 있습니다.
    
    파일에 쓸 수 없음: { $path }
update-in-progress-title = 업데이트 진행 중
update-in-progress-message = { -brand-short-name }가 이 업데이트를 계속하길 원하십니까?
update-in-progress-ok-button = 취소
# Continue is the cancel button so pressing escape or using a platform standard
# method of closing the UI will not discard the update.
update-in-progress-cancel-button = 계속

## General Section - Performance

performance-title = 성능
performance-use-recommended-settings-checkbox =
    .label = 권장 설정을 사용
    .accesskey = U
performance-use-recommended-settings-desc = 이 설정은 컴퓨터 하드웨어 및 운영체제에 맞게 조정됩니다.
performance-settings-learn-more = 더 알아보기
performance-allow-hw-accel =
    .label = 하드웨어 가속이 가능하면 사용
    .accesskey = r
performance-limit-content-process-option = 콘텐츠 프로세스 제한
    .accesskey = L
performance-limit-content-process-enabled-desc = 추가 콘텐츠 프로세스는 다중 탭을 사용 하는 경우 성능을 향상 시킬 수 있지만 더 많은 메모리를 사용 합니다.
performance-limit-content-process-blocked-desc = 콘텐츠 프로세스 갯수 변경은 멀티 프로세스 { -brand-short-name }에서만 가능합니다. <a data-l10n-name="learn-more">멀티 프로세스가 활성화 되었는지 확인하는 방법</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (기본값)

## General Section - Browsing

browsing-title = 보기 기능
browsing-use-autoscroll =
    .label = 자동 스크롤 기능 사용
    .accesskey = a
browsing-use-smooth-scrolling =
    .label = 부드러운 스크롤 기능 사용
    .accesskey = m
browsing-use-onscreen-keyboard =
    .label = 필요하면 터치 키보드 보여주기
    .accesskey = k
browsing-use-cursor-navigation =
    .label = 커서 키를 항상 페이지 내에서 사용
    .accesskey = c
browsing-search-on-start-typing =
    .label = 타이핑을 시작하면 검색
    .accesskey = x
browsing-cfr-recommendations =
    .label = 탐색시 확장 기능 추천
    .accesskey = R
browsing-cfr-features =
    .label = 탐색시 기능 추천
    .accesskey = f
browsing-cfr-recommendations-learn-more = 더 알아보기

## General Section - Proxy

network-settings-title = 네트워크 설정
network-proxy-connection-description = { -brand-short-name }가 인터넷에 접근하는 방법을 설정하세요.
network-proxy-connection-learn-more = 자세히 보기
network-proxy-connection-settings =
    .label = 설정…
    .accesskey = e

## Home Section

home-new-windows-tabs-header = 새 창과 탭
home-new-windows-tabs-description2 = 홈페이지나 새창, 새탭을 열었을 때 나올 내용을 선택하세요.

## Home Section - Home Page Customization

home-homepage-mode-label = 홈페이지와 새 창
home-newtabs-mode-label = 새 탭
home-restore-defaults =
    .label = 기본값으로 복원
    .accesskey = R
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Firefox 홈페이지(기본값)
home-mode-choice-custom =
    .label = 사용자 지정 URL…
home-mode-choice-blank =
    .label = 빈 페이지
home-homepage-custom-url =
    .placeholder = URL 붙여넣기…
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] 현재 페이지
           *[other] 현재 탭
        }
    .accesskey = C
choose-bookmark =
    .label = 북마크 사용…
    .accesskey = B

## Home Section - Firefox Home Content Customization

home-prefs-content-header = Firefox 홈 콘텐츠
home-prefs-content-description = Firefox 홈 화면에 나올 콘텐츠를 선택하세요.
home-prefs-content-discovery-description = Firefox 홈의 콘텐츠 탐색 기능을 사용하면 웹에 있는 고품질의 관련 문서를 탐색할 수 있습니다.
home-prefs-search-header =
    .label = 웹 검색
home-prefs-topsites-header =
    .label = 상위 사이트
home-prefs-topsites-description = 가장 많이 방문한 사이트
# Variables:
#  $provider (String): Name of the corresponding content provider, e.g "Pocket".
home-prefs-recommended-by-header =
    .label = { $provider } 추천
home-prefs-recommended-by-description = 여러분에게 맞춰진 웹에서 제공되는 훌륭한 컨텐츠
home-prefs-recommended-by-learn-more = 사용 방법
home-prefs-recommended-by-option-sponsored-stories =
    .label = 후원된 스토리
home-prefs-highlights-header =
    .label = 하이라이트
home-prefs-highlights-description = 저장하거나 방문한 사이트 모음
home-prefs-highlights-option-visited-pages =
    .label = 방문한 페이지
home-prefs-highlights-options-bookmarks =
    .label = 즐겨찾기
home-prefs-highlights-option-most-recent-download =
    .label = 가장 최근 다운로드
home-prefs-highlights-option-saved-to-pocket =
    .label = { -pocket-brand-name }에 저장된 페이지
# For the "Snippets" feature traditionally on about:home.
# Alternative translation options: "Small Note" or something that
# expresses the idea of "a small message, shortened from something else,
# and non-essential but also not entirely trivial and useless.
home-prefs-snippets-header =
    .label = 짧은 요약
home-prefs-snippets-description = { -vendor-short-name }와 { -brand-product-name } 소식
home-prefs-sections-rows-option =
    .label = { $num } 행

## Search Section

search-bar-header = 검색 표시줄
search-bar-hidden =
    .label = 주소 표시줄을 사용하여 검색과 탐색
search-bar-shown =
    .label = 도구 모음에 검색 표시줄 추가
search-engine-default-header = 기본 검색 사이트
search-engine-default-desc = 주소바와 검색바에서 사용할 기본 검색 엔진을 선택하세요.
search-suggestions-option =
    .label = 검색어 자동 완성 사용
    .accesskey = s
search-show-suggestions-url-bar-option =
    .label = 주소창 결과에 검색 추천 보이기
    .accesskey = l
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = 주소창 결과의 방문 기록 앞에 검색 추천 보이기
search-suggestions-cant-show = { -brand-short-name }가 탐색 목록을 저장하지 않도록 설정했기 때문에 주소창 결과에 검색 추천이 나오지 않을 것입니다.
search-one-click-header = 원클릭 검색 사이트
search-one-click-desc = 단어 입력을 시작했을 때 주소바와 검색바 아래에 나타날 대체 검색 엔진을 선택하세요.
search-choose-engine-column =
    .label = 검색 사이트
search-choose-keyword-column =
    .label = 키워드
search-restore-default =
    .label = 기본 검색 사이트 되돌리기
    .accesskey = d
search-remove-engine =
    .label = 삭제
    .accesskey = R
search-find-more-link = 더 많은 검색 엔진 찾기
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = 키워드 복사
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = "{ $name }"에서 이미 사용 중인 키워드를 선택했습니다. 다른 것을 선택하십시오.
search-keyword-warning-bookmark = 북마크에서 이미 사용 중인 키워드를 선택했습니다. 다른 것을 선택하십시오.

## Containers Section

containers-back-link = « 뒤로가기
containers-header = 컨테이너 탭
containers-add-button =
    .label = 새 컨테이너 추가
    .accesskey = A
containers-preferences-button =
    .label = 설정
containers-remove-button =
    .label = 삭제

## Sync Section - Signed out

sync-signedout-caption = 웹과 함께 하세요.
sync-signedout-description = 북마크, 방문 기록, 탭, 비밀번호, 부가기능, 설정을 모든 기기에 걸쳐 동기화 하세요.
sync-signedout-account-title = { -fxaccount-brand-name }과 연결
sync-signedout-account-create = 계정이 없으신가요? 시작하세요
    .accesskey = C
sync-signedout-account-signin =
    .label = 로그인…
    .accesskey = I
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = 모바일 기기와 동기화하기 위해서 Firefox <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> 또는 <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a>를 다운로드 합니다.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = 프로필 사진 변경
sync-disconnect =
    .label = 연결 끊기…
    .accesskey = D
sync-manage-account = 계정 관리
    .accesskey = o
sync-signedin-unverified = { $email } 은 아직 인증되지 않았습니다.
sync-signedin-login-failure = { $email } 으로 로그인을 해서 다시 연결해 주십시오
sync-resend-verification =
    .label = 확인 재전송
    .accesskey = d
sync-remove-account =
    .label = 계정 삭제
    .accesskey = R
sync-sign-in =
    .label = 로그인
    .accesskey = g
sync-signedin-settings-header = 동기화 설정
sync-signedin-settings-desc = { -brand-short-name }를 사용 중인 기기에서 동기화할 내용을 선택하세요.
sync-engine-bookmarks =
    .label = 북마크
    .accesskey = m
sync-engine-history =
    .label = 방문 기록
    .accesskey = r
sync-engine-tabs =
    .label = 열린 탭
    .tooltiptext = 모든 동기화된 장치에서 열린 항목의 목록
    .accesskey = T
sync-engine-logins =
    .label = 로그인
    .tooltiptext = 저장한 사용자 이름과 비밀번호
    .accesskey = L
sync-engine-addresses =
    .label = 주소
    .tooltiptext = 저장한 우편 주소(데스크탑)
    .accesskey = e
sync-engine-creditcards =
    .label = 신용카드
    .tooltiptext = 이름, 숫자 그리고 만료 날짜 (데스크톱만)
    .accesskey = C
sync-engine-addons =
    .label = 부가 기능
    .tooltiptext = 데스크톱용 Firefox를 위한 확장 프로그램과 테마
    .accesskey = A
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] 설정
           *[other] 환경 설정
        }
    .tooltiptext = 변경된 일반, 개인 정보 및 보안 설정
    .accesskey = S
sync-device-name-header = 기기 이름
sync-device-name-change =
    .label = 기기 이름 바꾸기…
    .accesskey = h
sync-device-name-cancel =
    .label = 취소
    .accesskey = n
sync-device-name-save =
    .label = 저장
    .accesskey = v
sync-connect-another-device = 다른 기기 연결
sync-manage-devices = 기기 관리
sync-fxa-begin-pairing = 기기 연결하기
sync-tos-link = 서비스 약관
sync-fxa-privacy-notice = 개인 정보 보호 정책

## Privacy Section

privacy-header = 브라우저 개인정보

## Privacy Section - Forms

logins-header = 로그인과 비밀번호
forms-ask-to-save-logins =
    .label = 웹사이트의 로그인과 비밀번호를 기억할지 묻기
    .accesskey = r
forms-exceptions =
    .label = 예외 목록…
    .accesskey = x
forms-generate-passwords =
    .label = 강력한 암호 제안 및 생성
    .accesskey = u
forms-fill-logins-and-passwords =
    .label = 로그인과 비밀번호 자동완성
    .accesskey = i
forms-saved-logins =
    .label = 저장된 로그인…
    .accesskey = L
forms-master-pw-use =
    .label = 정보 보호을 위해 기본 비밀번호 사용
    .accesskey = U
forms-master-pw-change =
    .label = 기본 비밀번호 변경…
    .accesskey = M

## Privacy Section - History

history-header = 방문 기록
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = { -brand-short-name }의 방문 기록을
    .accesskey = w
history-remember-option-all =
    .label = 방문 기록 저장
history-remember-option-never =
    .label = 방문 기록 저장 안 함
history-remember-option-custom =
    .label = 방문 기록 사용자 지정 설정 사용
history-remember-description = { -brand-short-name }가 탐색과 다운로드, 서식, 검색 히스토리를 저장합니다.
history-dontremember-description = { -brand-short-name }는 사생활 보호 모드와 같은 설정을 가지며 웹 사이트 방문 중 어떤 기록도 저장하지 않습니다.
history-private-browsing-permanent =
    .label = 항상 사생활 보호 모드 사용
    .accesskey = p
history-remember-browser-option =
    .label = 방문 및 다운로드 기록 저장
    .accesskey = b
history-remember-search-option =
    .label = 검색 및 입력 기록 저장
    .accesskey = f
history-clear-on-close-option =
    .label = { -brand-short-name } 닫을 때 방문 기록 삭제
    .accesskey = r
history-clear-on-close-settings =
    .label = 설정…
    .accesskey = t
history-clear-button =
    .label = 방문 기록 삭제…
    .accesskey = s

## Privacy Section - Site Data

sitedata-header = 쿠키와 사이트 데이터
sitedata-total-size-calculating = 사이트 데이터와 캐시 크기 계산중…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = 현재 저장된 쿠키와 사이트 데이터, 캐시가 { $value } { $unit }의 디스크를 사용하고 있습니다.
sitedata-learn-more = 더 알아보기
sitedata-delete-on-close =
    .label = { -brand-short-name }가 닫힐 때 쿠키와 사이트 데이터를 삭제
    .accesskey = c
sitedata-delete-on-close-private-browsing = 영구 사생활 보호 모드에서는 { -brand-short-name }가 닫히면 쿠키와 사이트 데이터가 항상 지워집니다.
sitedata-allow-cookies-option =
    .label = 쿠키와 사이트 데이터 허용
    .accesskey = A
sitedata-disallow-cookies-option =
    .label = 쿠키와 사이트 데이터 차단
    .accesskey = B
# This label means 'type of content that is blocked', and is followed by a drop-down list with content types below.
# The list items are the strings named sitedata-block-*-option*.
sitedata-block-desc = 차단 유형
    .accesskey = T
sitedata-option-block-trackers =
    .label = 제3자 추적기
sitedata-option-block-cross-site-trackers =
    .label = 교차 사이트 추적기
sitedata-option-block-cross-site-and-social-media-trackers =
    .label = 교차 사이트 및 소셜 미디어 추적기
sitedata-option-block-unvisited =
    .label = 방문하지 않은 웹 사이트의 쿠키
sitedata-option-block-all-third-party =
    .label = 모든 제3자 쿠키(웹사이트가 깨질 수 있음)
sitedata-option-block-all =
    .label = 모든 쿠키(웹사이트가 깨질 수 있음)
sitedata-clear =
    .label = 데이터 삭제…
    .accesskey = l
sitedata-settings =
    .label = 데이터 관리…
    .accesskey = M
sitedata-cookies-permissions =
    .label = 권한 관리…
    .accesskey = P

## Privacy Section - Address Bar

addressbar-header = 주소 표시줄
addressbar-suggest = 주소 표시줄 사용시 제안할 항목
addressbar-locbar-history-option =
    .label = 방문 기록
    .accesskey = H
addressbar-locbar-bookmarks-option =
    .label = 북마크
    .accesskey = k
addressbar-locbar-openpage-option =
    .label = 열린 탭
    .accesskey = O
addressbar-suggestions-settings = 검색 엔진 추천 설정 변경

## Privacy Section - Content Blocking

content-blocking-header = 콘텐츠 차단
content-blocking-enhanced-tracking-protection = 향상된 추적 방지 기능
content-blocking-section-description = 탐색하는 동안 개인 정보를 보호하십시오. 사용자가 방문하는 사이트를 추적하고 개인을 식별하는 보이지 않는 콘텐츠를 차단합니다. 이 콘텐츠 중 일부를 차단하면 페이지가 더 빨리 로드 될 수 있습니다.
content-blocking-learn-more = 자세히 보기
# The terminology used to refer to categories of Content Blocking is also used in chrome/browser/browser.properties and should be translated consistently.
# "Standard" in this case is an adjective, meaning "default" or "normal".
content-blocking-setting-standard =
    .label = 표준
    .accesskey = d
content-blocking-setting-strict =
    .label = 엄격
    .accesskey = r
content-blocking-setting-custom =
    .label = 사용자 지정
    .accesskey = C
content-blocking-standard-desc = 보호와 성능 사이의 균형을 유지합니다. 웹 사이트가 제대로 작동하도록 일부 추적기를 허용합니다.
content-blocking-strict-description = 강한 보호 기능으로 인해 일부 사이트가 손상될 수 있습니다.
content-blocking-custom-desc = 차단할 항목을 선택하세요.
content-blocking-private-trackers = 사생활 보호 모드에서 알려진 추적기만
content-blocking-third-party-cookies = 제3자 추적 쿠키
content-blocking-etp-standard-desc = 보호와 성능사이의 균형이 잡혀 있습니다. 페이지가 정상적으로 로드됩니다.
content-blocking-etp-strict-desc = 더 강력한 보호 기능을 제공하지만, 일부 사이트나 콘텐츠가 손상될 수 있습니다.
content-blocking-etp-custom-desc = 차단할 추적기와 스크립트를 선택하십시오.
content-blocking-private-windows = 사생활 보호 창의 추적 콘텐츠
content-blocking-cross-site-tracking-cookies = 교차 사이트 추적 쿠키
content-blocking-social-media-trackers = 소셜 미디어 추적기
content-blocking-all-cookies = 모든 쿠키
content-blocking-unvisited-cookies = 방문하지 않은 사이트의 쿠키
content-blocking-all-windows-trackers = 알려진 추적기를 모든 창에서
content-blocking-all-windows-tracking-content = 모든 창의 추적 콘텐츠
content-blocking-all-third-party-cookies = 모든 제3자 쿠키
content-blocking-cryptominers = 크립토마이너
content-blocking-fingerprinters = 핑거프린터
content-blocking-warning-title = 조심하세요!
content-blocking-warning-description = 콘텐츠를 차단하면 일부 웹사이트가 깨질 수 있습니다. 신뢰할 수 있는 사이트에 대한 차단은 쉽게 해제 할 수 있습니다.
content-blocking-learn-how = 방법 알아보기
content-blocking-etp-warning-description = 추적기 차단은 일부 사이트의 기능에 영향을 줄 수 있습니다. 모든 콘텐츠를 로드하려면 추적기가 있는 페이지를 다시 로드하십시오.
content-blocking-warning-learn-how = 방법 알아보기
content-blocking-reload-description = 이러한 변경 사항을 적용하려면 탭을 다시 로드해야 합니다.
content-blocking-reload-tabs-button =
    .label = 모든 탭 다시 읽기
    .accesskey = R
content-blocking-trackers-label =
    .label = 추적기
    .accesskey = T
content-blocking-tracking-content-label =
    .label = 추적 콘텐츠
    .accesskey = T
content-blocking-tracking-protection-option-all-windows =
    .label = 모든 창에서
    .accesskey = A
content-blocking-option-private =
    .label = 사생활 보호 창에서만
    .accesskey = P
content-blocking-tracking-protection-change-block-list = 차단 목록 변경
content-blocking-cookies-label =
    .label = 쿠키
    .accesskey = C
content-blocking-expand-section =
    .tooltiptext = 더 알아보기
# Cryptomining refers to using scripts on websites that can use a computer’s resources to mine cryptocurrency without a user’s knowledge.
content-blocking-cryptominers-label =
    .label = 크립토마이너
    .accesskey = y
# Browser fingerprinting is a method of tracking users by the configuration and settings information (their "digital fingerprint")
# that is visible to websites they browse, rather than traditional tracking methods such as IP addresses and unique cookies.
content-blocking-fingerprinters-label =
    .label = 핑거프린터
    .accesskey = F

## Privacy Section - Tracking

tracking-manage-exceptions =
    .label = 예외 관리…
    .accesskey = x

## Privacy Section - Permissions

permissions-header = 권한
permissions-location = 위치
permissions-location-settings =
    .label = 설정…
    .accesskey = l
permissions-camera = 카메라
permissions-camera-settings =
    .label = 설정…
    .accesskey = c
permissions-microphone = 마이크
permissions-microphone-settings =
    .label = 설정…
    .accesskey = m
permissions-notification = 알림
permissions-notification-settings =
    .label = 설정…
    .accesskey = n
permissions-notification-link = 더 알아보기
permissions-notification-pause =
    .label = { -brand-short-name }를 재시작 할 때 까지 알림을 멈춤
    .accesskey = n
permissions-block-autoplay-media2 =
    .label = 웹사이트가 자동으로 소리를 재생하는 것 차단
    .accesskey = B
permissions-block-autoplay-media-exceptions =
    .label = 예외 목록…
    .accesskey = E
permissions-autoplay = 자동 재생
permissions-autoplay-settings =
    .label = 설정…
    .accesskey = t
permissions-block-popups =
    .label = 팝업 창 차단
    .accesskey = B
permissions-block-popups-exceptions =
    .label = 예외 목록…
    .accesskey = E
permissions-addon-install-warning =
    .label = 웹사이트가 부가 기능을 설치하려 할 때 경고
    .accesskey = W
permissions-addon-exceptions =
    .label = 예외 목록…
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = 접근성 서비스가 브라우저에 접근하지 못하게 함
    .accesskey = a
permissions-a11y-privacy-link = 더 알아보기

## Privacy Section - Data Collection

collection-header = { -brand-short-name } 데이터 수집과 사용
collection-description = { -brand-short-name }를 모두를 위해 제공하고 개선하기 위해서 필요한 것만 수집하고 선택권을 제공하기 위해 노력합니다. 개인 정보를 전송하기 전에 항상 허가여부를 묻습니다.
collection-privacy-notice = 개인정보 안내
collection-health-report =
    .label = { -brand-short-name }가 기술과 상호 작용 정보를 { -vendor-short-name }에 전송하도록 허용
    .accesskey = r
collection-health-report-link = 더 알아보기
collection-studies =
    .label = { -brand-short-name }가 연구를 설치하고 실행하도록 허용
collection-studies-link = { -brand-short-name } 연구 보기
addon-recommendations =
    .label = { -brand-short-name }가 개인화된 확장 기능 추천을 하도록 허용
addon-recommendations-link = 자세히 보기
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = 이 빌드 설정에서는 데이터 보고가 비활성화 되어 있음
collection-backlogged-crash-reports =
    .label = { -brand-short-name }가 사용자를 대신해서 백로그 충돌 보고서를 보낼 수 있게 함
    .accesskey = c
collection-backlogged-crash-reports-link = 더 알아보기

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = 보안
security-browsing-protection = 사기성 콘텐츠 및 위험한 소프트웨어 보호
security-enable-safe-browsing =
    .label = 위험하고 사기성 콘텐츠 차단
    .accesskey = B
security-enable-safe-browsing-link = 더 알아보기
security-block-downloads =
    .label = 위험한 다운로드 차단
    .accesskey = D
security-block-uncommon-software =
    .label = 원치 않거나 일반적이지 않은 소프트웨어에 대해 알림
    .accesskey = C

## Privacy Section - Certificates

certs-header = 인증서
certs-personal-label = 서버가 인증 정보를 요구할 때
certs-select-auto-option =
    .label = 자동으로 하나를 선택
    .accesskey = S
certs-select-ask-option =
    .label = 항상 물어보기
    .accesskey = A
certs-enable-ocsp =
    .label = OCSP서버를 통해 인증서 유효성 실시간 확인
    .accesskey = Q
certs-view =
    .label = 인증서 보기…
    .accesskey = C
certs-devices =
    .label = 보안 기기…
    .accesskey = D
space-alert-learn-more-button =
    .label = 더 알아보기
    .accesskey = L
space-alert-over-5gb-pref-button =
    .label =
        { PLATFORM() ->
            [windows] 옵션 열기
           *[other] 설정 열기
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] O
        }
space-alert-over-5gb-message =
    { PLATFORM() ->
        [windows] { -brand-short-name } 디스크 용량이 부족합니다. 웹사이트 내용이 제대로 표시되지 않을 수 있습니다. 옵션 > 개인정보와 보안 > 쿠키와 사이트 데이터에서 저장된 데이터를 지울 수 있습니다.
       *[other] { -brand-short-name } 디스크 용량이 부족합니다. 웹사이트 내용이 제대로 표시되지 않을 수 있습니다. 설정 > 개인정보와 보안 > 쿠키와 사이트 데이터에서 저장된 데이터를 지울 수 있습니다.
    }
space-alert-under-5gb-ok-button =
    .label = 알겠습니다
    .accesskey = K
space-alert-under-5gb-message = { -brand-short-name } 디스크 용량이 부족합니다. 웹사이트 내용이 제대로 표시되지 않을 수 있습니다. 더 나은 인터넷 경험을 위해 디스크 용량을 최적화하는 방법을 알아보려면 “더 알아보기”를 방문하세요.

## The following strings are used in the Download section of settings

desktop-folder-name = 바탕 화면
downloads-folder-name = 다운로드
choose-download-folder-title = 다운로드 폴더 선택:
# Variables:
#   $service-name (String) - Name of a cloud storage provider like Dropbox, Google Drive, etc...
save-files-to-cloud-storage =
    .label = { $service-name }에 파일 저장
