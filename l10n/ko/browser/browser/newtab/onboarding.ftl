# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### UI strings for the simplified onboarding modal / about:welcome
### Various strings use a non-breaking space to avoid a single dangling /
### widowed word, so test on various window sizes if you also want this.


## These button action text can be split onto multiple lines, so use explicit
## newlines in translations to control where the line break appears (e.g., to
## avoid breaking quoted text).

onboarding-button-label-learn-more = 더 알아보기
onboarding-button-label-try-now = 지금 사용해보기
onboarding-button-label-get-started = 시작하기

## Welcome modal dialog strings

onboarding-welcome-header = { -brand-short-name }를 사용해 주셔서 감사합니다.
onboarding-welcome-body = 이제 브라우저가 있습니다.<br/>{ -brand-product-name }의 나머지를 만나 보세요.
onboarding-welcome-learn-more = 장점 더 알아보기
onboarding-join-form-header = { -brand-product-name }와 함께하기
onboarding-join-form-body = 시작하려면 이메일 주소를 입력하세요.
onboarding-join-form-email =
    .placeholder = 이메일 입력
onboarding-join-form-email-error = 유효한 이메일이 필요합니다
onboarding-join-form-legal = 계속 진행하면 <a data-l10n-name="terms">서비스 약관</a> 및 <a data-l10n-name="privacy">개인 정보 보호 정책</a>에 동의하게 됩니다.
onboarding-join-form-continue = 계속
onboarding-start-browsing-button-label = 브라우징 시작
onboarding-cards-dismiss =
    .title = 닫기
    .aria-label = 닫기

## Firefox Sync modal dialog strings.

onboarding-sync-welcome-header = { -brand-product-name }와 함께 하세요
onboarding-sync-welcome-content = 즐겨찾기와 방문기록, 비밀번호, 다른 설정을 모든 기기에서 사용해 보세요.
onboarding-sync-welcome-learn-more-link = Firefox 계정 자세히 알아보기
onboarding-sync-form-invalid-input = 유효한 이메일 필요함
onboarding-sync-legal-notice = 진행하면 <a data-l10n-name="terms">이용 약관</a>과 <a data-l10n-name="privacy">개인 정보 보호 정책</a>에 동의하게 됩니다.
onboarding-sync-form-input =
    .placeholder = 이메일
onboarding-sync-form-continue-button = 계속
onboarding-sync-form-skip-login-button = 단계 건너뛰기

## This is part of the line "Enter your email to continue to Firefox Sync"

onboarding-sync-form-header = 이메일을 입력
onboarding-sync-form-sub-header = 해서 { -sync-brand-name } 사용

## These are individual benefit messages shown with an image, title and
## description.

onboarding-benefit-products-title = 유용한 제품
onboarding-benefit-products-text = 내가 사용하는 모든 기기에서 개인 정보를 존중하는 도구를 사용합니다.
onboarding-benefit-knowledge-title = 실용 지식
onboarding-benefit-knowledge-text = 온라인에서 더 똑똑하고 안전하게 지내기 위해 알아야 할 모든 것을 배워 보세요.
onboarding-benefit-privacy-title = 진정한 개인 정보 보호 정책
# "Personal Data Promise" is a concept that should be translated consistently
# across the product. It refers to a concept shown elsewhere to the user: "The
# Firefox Personal Data Promise is the way we honor your data in everything we
# make and do. We take less data. We keep it safe. And we make sure that we are
# transparent about how we use it."
onboarding-benefit-privacy-text = 우리가 하는 모든 활동은 개인 정보 약속을 존중합니다: 적게 가집니다. 안전하게 보관합니다. 비밀은 없습니다.

## These strings belong to the individual onboarding messages.


## Each message has a title and a description of what the browser feature is.
## Each message also has an associated button for the user to try the feature.
## The string for the button is found above, in the UI strings section

onboarding-private-browsing-title = 사생활 보호 모드
onboarding-private-browsing-text = 개인적으로 탐색하세요. 컨텐츠 차단과 사생활 보호 탐색은 웹에서 사용자를 추적하는 온라인 추적기들을 막아줍니다.
onboarding-screenshots-title = 스크린샷
onboarding-screenshots-text = 스크린샷을 찍고 저장하고 공유하세요 - { -brand-short-name }를 떠날 필요조차 없습니다. 탐색하면서 일부분 또는 전체 페이지를 캡처하세요. 그리고 웹에 저장해서 쉽게 접근하고 공유하세요.
onboarding-addons-title = 부가기능
onboarding-addons-text = { -brand-short-name }가 여러분을 위해 더 열심히 일하도록 더욱 많은 기능들을 추가하세요. 가격을 비교하거나 날씨를 확인하거나 사용자 지정 테마로 여러분의 개성을 표현해 보세요.
onboarding-ghostery-title = Ghostery
onboarding-ghostery-text = 귀찮은 광고들로부터 여러분을 막아주는 Ghostery 같은 확장기능으로 더욱 빠르고 똑똑하고 안전하게 탐색하세요.
# Note: "Sync" in this case is a generic verb, as in "to synchronize"
onboarding-fxa-title = Sync
onboarding-fxa-text = { -fxaccount-brand-name }에 가입해 { -brand-short-name }를 사용하는 모든 곳에서 북마크, 비밀번호, 열린 탭들을 동기화하세요.
onboarding-tracking-protection-title2 = 추적으로부터의 보호
onboarding-tracking-protection-text2 = { -brand-short-name }는 웹 사이트가 온라인에서 사용자를 추적하는 것을 막아, 광고가 웹에서 사용자를 따라 다니는 것을 어렵게 만듭니다.
onboarding-tracking-protection-button2 = 작동 원리
onboarding-data-sync-title = 설정을 항상 유지하기
# "Sync" is short for synchronize.
onboarding-data-sync-text2 = { -brand-product-name }를 사용하는 모든 곳에서 북마크, 비밀번호 등을 동기화하십시오.
onboarding-data-sync-button2 = { -sync-brand-short-name }에 로그인하기
onboarding-firefox-monitor-title = 데이터 유출 알림
onboarding-firefox-monitor-text = { -monitor-brand-name }는 데이터 유출 사건에 사용자의 이메일이 있는지를 모니터링하고 새 데이터 유출 사건에 있으면 사용자에게 알려 줍니다.
onboarding-firefox-monitor-button = 알림 가입 신청
onboarding-browse-privately-title = 사생활 보호 모드
onboarding-browse-privately-text = 사생활 보호 모드가 검색 및 방문 기록을 지우기 때문에 컴퓨터를 사용하는 모든 사람들이 개인 정보를 유지할 수 있습니다.
onboarding-browse-privately-button = 사생활 보호 창 열기
onboarding-firefox-send-title = 공유 파일 보호
onboarding-firefox-send-text2 = 파일을 { -send-brand-name }에 업로드하여 종단 간 암호화와 자동으로 만료되는 링크로 공유하십시오.
onboarding-firefox-send-button = { -send-brand-name }를 사용해보세요
onboarding-mobile-phone-title = 휴대폰에서 { -brand-product-name } 받기
onboarding-mobile-phone-text = iOS 또는 Android 용 { -brand-product-name }를 다운로드하고 기기간에 데이터를 동기화합니다.
# "Mobile" is short for mobile/cellular phone, "Browser" is short for web
# browser.
onboarding-mobile-phone-button = 모바일 브라우저 다운로드
onboarding-send-tabs-title = 즉시 탭 보내기
# "Send Tabs" refers to "Send Tab to Device" feature that appears when opening a
# tab's context menu.
onboarding-send-tabs-text = 탭 보내기는 복사나 붙여넣기, 브라우저를 나가지 않고도 내 기기들로 페이지를 즉시 공유합니다.
onboarding-send-tabs-button = 탭 보내기 사용 시작
onboarding-pocket-anywhere-title = 어디서나 읽고 듣기
onboarding-pocket-anywhere-text2 = { -pocket-brand-name } 앱으로 좋아하는 콘텐츠를 오프라인에 저장하고 편리할 때마다 읽고, 듣고, 시청할 수 있습니다.
onboarding-pocket-anywhere-button = { -pocket-brand-name }을 사용해보세요
onboarding-lockwise-passwords-title = 어디서나 비밀번호를 사용
onboarding-lockwise-passwords-text2 = { -lockwise-brand-name }로 저장한 비밀번호를 안전하게 유지하고 계정에 쉽게 로그인하십시오.
onboarding-lockwise-passwords-button2 = 앱 다운로드
onboarding-facebook-container-title = Facebook 경계 설정
onboarding-facebook-container-text2 = { -facebook-container-brand-name }는 프로필을 다른 것과 구분하여 유지하므로 Facebook이 사용자를 구분하여 광고하는 것을 어렵게 만듭니다.
onboarding-facebook-container-button = 확장 기능 추가

## Message strings belonging to the Return to AMO flow

return-to-amo-sub-header = 좋습니다. { -brand-short-name }를 설치 했네요.
# <icon></icon> will be replaced with the icon belonging to the extension
#
# Variables:
#   $addon-name (String) - Name of the add-on
return-to-amo-addon-header = 이제 <icon></icon><b>{ $addon-name } 부가기능</b>을 설치하겠습니다.
return-to-amo-extension-button = 확장 기능 추가
return-to-amo-get-started-button = { -brand-short-name } 시작하기
