# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
#   $count (Number) - Number of tracking events blocked.
graph-week-summary =
    { $count ->
       *[other] { -brand-short-name }가 지난 주 동안 { $count }개의 추적기를 차단함
    }
# Variables:
#   $count (Number) - Number of tracking events blocked.
#   $earliestDate (Number) - Unix timestamp in ms, representing a date. The
# earliest date recorded in the database.
graph-total-summary =
    { $count ->
       *[other] { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") } 이후 { $count }개의 추적기가 차단됨
    }
# The terminology used to refer to categories of Content Blocking is also used in chrome/browser/browser.properties and should be translated consistently.
# "Standard" in this case is an adjective, meaning "default" or "normal".
# The category name in the <b> tag will be bold.
protection-header-details-standard = 보호 수준이 <b>표준</b>으로 설정되었습니다
protection-header-details-strict = 보호 수준이 <b>엄격</b>으로 설정되었습니다
protection-header-details-custom = 보호 수준이 <b>사용자 지정</b>으로 설정되었습니다
protection-report-page-title = 개인 정보 보호
protection-report-content-title = 개인 정보 보호
etp-card-title = 향상된 추적 방지 기능
etp-card-content = 추적기는 온라인에서 사용자를 따라다니며 브라우징 습관과 관심사에 대한 정보를 수집합니다. { -brand-short-name }는 이러한 많은 추적기 및 기타 악성 스크립트를 차단합니다.
# This string is used to label the X axis of a graph. Other days of the week are generated via Intl.DateTimeFormat,
# capitalization for this string should match the output for your locale.
graph-today = 오늘
# This string is used to describe the graph for screenreader users.
graph-legend-description = 이번 주에 차단된 각 유형의 추적기의 총 수를 포함하는 그래프입니다.
social-tab-title = 소셜 미디어 추적기
social-tab-contant = 소셜 네트워크는 다른 웹 사이트에 추적기를 배치하여 온라인에서 한 일이나 본 것, 재생한 것을 추적합니다. 이를 통해 소셜 미디어 회사들은 소셜 미디어 프로필에서 공유하는 것 이상의 자세한 정보를 얻을 수 있습니다. <a data-l10n-name="learn-more-link">더 알아보기</a>
cookie-tab-title = 교차 사이트 추적 쿠키
cookie-tab-content = 이러한 쿠키는 사용자가 온라인에서 수행하는 작업에 대한 데이터를 수집하기 위해 사이트 간에 사용자를 따라 다닙니다. 광고사나 분석 회사와 같은 제3자가 설정합니다. 교차 사이트 추적 쿠키를 차단하면 따라다니는 광고 수가 줄어듭니다. <a data-l10n-name="learn-more-link">더 알아보기</a>
tracker-tab-title = 추적 콘텐츠
tracker-tab-content = 웹 사이트에서 외부 광고, 동영상 및 기타 추적 코드가 포함된 콘텐츠를 로드 할 수 있습니다. 추적 콘텐츠를 차단하면 사이트를 더 빨리 로드 할 수 있지만 일부 버튼, 양식 및 로그인 필드가 작동하지 않을 수 있습니다. <a data-l10n-name="learn-more-link">더 알아보기</a>
fingerprinter-tab-title = 핑거프린터
fingerprinter-tab-content = 핑거프린터는 브라우저와 컴퓨터에서 설정을 수집하여 사용자의 프로필을 만듭니다. 이 디지털 지문을 사용하여 여러 웹 사이트에서 사용자를 추적할 수 있습니다. <a data-l10n-name="learn-more-link">더 알아보기</a>
cryptominer-tab-title = 크립토마이너
cryptominer-tab-content = 크립토마이너는 디지털 화폐를 채굴하기 위해 시스템의 연산 능력을 사용합니다. 암호 해독 스크립트는 배터리를 소모하고 컴퓨터 속도를 저하시키며 에너지 요금을 증가시킬 수 있습니다. <a data-l10n-name="learn-more-link">더 알아보기</a>
lockwise-title = 비밀번호를 다시 잊지 마세요
lockwise-title-logged-in = { -lockwise-brand-name }
lockwise-header-content = { -lockwise-brand-name }은 비밀번호를 브라우저에 안전하게 저장합니다.
lockwise-header-content-logged-in = 비밀번호를 모든 기기에 안전하게 저장하고 동기화합니다.
open-about-logins-button = { -brand-short-name }에서 열기
lockwise-no-logins-content = <a data-l10n-name="lockwise-inline-link">{ -lockwise-brand-name }</a> 앱을 통해 어디서나 비밀번호를 사용하십시오.
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
lockwise-passwords-stored =
    { $count ->
       *[other] 비밀번호가 안전하게 저장됨 <a data-l10n-name="lockwise-how-it-works">작동원리</a>
    }
turn-on-sync = { -sync-brand-short-name } 켜기…
    .title = Sync 설정으로 가기
manage-devices = 기기 관리
# Variables:
#   $count (Number) - Number of devices connected with sync.
lockwise-sync-status =
    { $count ->
       *[other] 다른 { $count }개의 기기에 동기화중
    }
lockwise-sync-not-syncing = 다른 기기와 동기화되지 않습니다.
monitor-title = 데이터 유출에 주의하십시오
monitor-link = 작동 원리
monitor-header-content = { -monitor-brand-name }를 확인하여 사용자가 데이터 유출 사건의 피해자인지 확인하고 새로운 유출에 대한 알림을 받습니다.
monitor-header-content-logged-in = 사용자의 정보가 알려진 데이터 유출 사건에 있으면 { -monitor-brand-name }가 경고합니다
monitor-sign-up = 유출 알림 가입하기
auto-scan = 오늘의 자동 스캔
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-monitored-addresses =
    { $count ->
       *[other] 모니터되는 이메일 주소입니다.
    }
# This string is displayed after a large numeral that indicates the total number
# of known data breaches. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-known-breaches =
    { $count ->
       *[other] 알려진 데이터 유출로 인해 정보가 노출되었습니다.
    }
# This string is displayed after a large numeral that indicates the total number
# of exposed passwords. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-exposed-passwords =
    { $count ->
       *[other] 모든 유출에 걸처 비밀번호가 노출되었습니다.
    }
full-report-link = <a data-l10n-name="monitor-inline-link">{ -monitor-brand-name }</a>에 대한 전체 보고서 보기
# This string is displayed after a large numeral that indicates the total number
# of saved logins which may have been exposed. Don’t add $count to
# your localization, because it would result in the number showing twice.
password-warning =
    { $count ->
       *[other] 저장된 로그인이 데이터 유출에 노출되었을 수 있습니다. 더 나은 온라인 보안을 위해 이 비밀번호를 변경하십시오. <a data-l10n-name="lockwise-link">저장된 로그인 보기</a>
    }
# This is the title attribute describing the graph report's link to about:settings#privacy
go-to-privacy-settings = 개인정보 보호 설정으로 이동
# This is the title attribute describing the Lockwise card's link to about:logins
go-to-saved-logins = 저장된 로그인으로 이동

## The title attribute is used to display the type of protection.
## The aria-label is spoken by screen readers to make the visual graph accessible to blind users.
##
## Variables:
##   $count (Number) - Number of specific trackers
##   $percentage (Number) - Percentage this type of tracker contributes to the whole graph

bar-tooltip-social =
    .title = 소셜 미디어 추적기
    .aria-label =
        { $count ->
           *[other] { $count } 소셜 미디어 추적기 ({ $percentage }%)
        }
bar-tooltip-cookie =
    .title = 교차 사이트 추적 쿠키
    .aria-label =
        { $count ->
           *[other] { $count } 교차 사이트 추적 쿠키 ({ $percentage }%)
        }
bar-tooltip-tracker =
    .title = 추적 콘텐츠
    .aria-label =
        { $count ->
           *[other] { $count } 추적 콘텐츠 ({ $percentage }%)
        }
bar-tooltip-fingerprinter =
    .title = 핑거프린터
    .aria-label =
        { $count ->
           *[other] { $count } 핑거프린터 ({ $percentage }%)
        }
bar-tooltip-cryptominer =
    .title = 크립토마이너
    .aria-label =
        { $count ->
           *[other] { $count } 크립토마이너 ({ $percentage }%)
        }
