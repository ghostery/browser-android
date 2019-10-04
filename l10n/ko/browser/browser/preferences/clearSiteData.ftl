# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

clear-site-data-window =
    .title = 데이터 삭제
    .style = width: 35em
clear-site-data-description = { -brand-short-name }에 저장된 모든 쿠키와 사이트 데이터를 삭제하면 웹사이트에서 로그아웃 되거나 오프라인 웹 콘텐츠가 지워집니다. 캐시 데이터를 지우는 것은 로그인에 영향을 미치지 않습니다.
clear-site-data-close-key =
    .key = w
# The parameters in parentheses in this string describe disk usage
# in the format ($amount $unit), e.g. "Cookies and Site Data (24 KB)"
# Variables:
#   $amount (Number) - Amount of site data currently stored on disk
#   $unit (String) - Abbreviation of the unit that $amount is in, e.g. "MB"
clear-site-data-cookies-with-data =
    .label = 쿠키와 사이트 데이터({ $amount } { $unit })
    .accesskey = S
# This string is a placeholder for while the data used to fill
# clear-site-data-cookies-with-data is loading. This placeholder is usually
# only shown for a very short time (< 1s), so it should be very similar
# or the same as clear-site-data-cookies-with-data (except the amount and unit),
# to avoid flickering.
clear-site-data-cookies-empty =
    .label = 쿠키와 사이트 데이터
    .accesskey = S
clear-site-data-cookies-info = 삭제하면 웹사이트에서 로그아웃 됨
# The parameters in parentheses in this string describe disk usage
# in the format ($amount $unit), e.g. "Cached Web Content (24 KB)"
# Variables:
#   $amount (Number) - Amount of cache currently stored on disk
#   $unit (String) - Abbreviation of the unit that $amount is in, e.g. "MB"
clear-site-data-cache-with-data =
    .label = 캐시된 웹 콘텐츠({ $amount } { $unit })
    .accesskey = W
# This string is a placeholder for while the data used to fill
# clear-site-data-cache-with-data is loading. This placeholder is usually
# only shown for a very short time (< 1s), so it should be very similar
# or the same as clear-site-data-cache-with-data (except the amount and unit),
# to avoid flickering.
clear-site-data-cache-empty =
    .label = 캐시된 웹 콘텐츠
    .accesskey = W
clear-site-data-cache-info = 웹사이트가 이미지와 데이터를 새로 고침함
clear-site-data-cancel =
    .label = 취소
    .accesskey = C
clear-site-data-clear =
    .label = 삭제
    .accesskey = l
