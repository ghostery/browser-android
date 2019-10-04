# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/. --

page-info-window =
    .style = width: 600px; min-height: 500px;
copy =
    .key = C
menu-copy =
    .label = 복사
    .accesskey = C
select-all =
    .key = A
menu-select-all =
    .label = 모두 선택
    .accesskey = A
close-window =
    .key = A
close-dialog =
    .key = w
general-tab =
    .label = 일반
    .accesskey = G
general-title =
    .value = 제목:
general-url =
    .value = 주소:
general-type =
    .value = 형식:
general-mode =
    .value = 렌더링 방식:
general-size =
    .value = 파일 용량:
general-referrer =
    .value = 참조 URL:
general-modified =
    .value = 수정:
general-encoding =
    .value = 글자 인코딩:
general-meta-name =
    .label = 이름
general-meta-content =
    .label = 내용
media-tab =
    .label = 미디어
    .accesskey = M
media-location =
    .value = 위치:
media-text =
    .value = 추가 내용:
media-alt-header =
    .label = 대체 텍스트
media-address =
    .label = 주소
media-type =
    .label = 유형
media-size =
    .label = 이미지 크기
media-count =
    .label = 카운트
media-dimension =
    .value = 사이즈:
media-long-desc =
    .value = 설명:
media-save-as =
    .label = 다른 이름으로 저장…
    .accesskey = A
media-save-image-as =
    .label = 다른 이름으로 저장…
    .accesskey = e
media-preview =
    .value = 미리 보기:
perm-tab =
    .label = 이용 권한
    .accesskey = P
permissions-for =
    .value = 이용 권한:
security-tab =
    .label = 보안
    .accesskey = S
security-view =
    .label = 인증서 보기
    .accesskey = V
security-view-unknown = 알 수 없음
    .value = 알 수 없음
security-view-identity =
    .value = 웹 사이트 정보
security-view-identity-owner =
    .value = 소유자:
security-view-identity-domain =
    .value = 사이트 정보:
security-view-identity-verifier =
    .value = 신원 확인자:
security-view-identity-validity =
    .value = 만료일:
security-view-privacy =
    .value = 개인 정보 및 방문 기록
security-view-privacy-history-value = 이전에 현재 웹 사이트 방문 여부
security-view-privacy-sitedata-value = 이 웹사이트가 내 컴퓨터에 데이터를 저장하나요?
security-view-privacy-clearsitedata =
    .label = 쿠키와 사이트 데이터 삭제
    .accesskey = C
security-view-privacy-passwords-value = 이 사이트에 저정된 비밀번호가 있나요?
security-view-privacy-viewpasswords =
    .label = 저장된 비밀번호 보기
    .accesskey = w
security-view-technical =
    .value = 세부 사항
help-button =
    .label = 도움말

## These strings are used to tell the user if the website is storing cookies
## and data on the users computer in the security tab of pageInfo
## Variables:
##   $value (number) - Amount of data being stored
##   $unit (string) - The unit of data being stored (Usually KB)

security-site-data-cookies = 네, 쿠키와 { $value } { $unit } 데이터
security-site-data-only = 네, { $value } { $unit } 데이터
security-site-data-cookies-only = 네, 쿠키
security-site-data-no = 아니오
image-size-unknown = 알 수 없음
not-set-verified-by = 설정되지 않음
not-set-alternative-text = 설정되지 않음
not-set-date = 설정되지 않음
media-img = 이미지
media-bg-img = 배경 이미지
media-border-img = 외곽 이미지
media-list-img = 목록 마커 이미지
media-cursor = 커서
media-object = 개체
media-embed = 임베드
media-link = 아이콘
media-input = 입력
media-video = 동영상
media-audio = 음성
saved-passwords-yes = 있음
saved-passwords-no = 없음
no-page-title =
    .value = 페이지 제목 없음:
general-quirks-mode =
    .value = 비표준 모드
general-strict-mode =
    .value = 표준 호환 모드
security-no-owner = 현재 웹 사이트는 소유자 정보를 제공하지 않고 있습니다.
media-select-folder = 이미지를 저장할 폴더 선택
media-unknown-not-cached =
    .value = 알 수 없음(캐싱 안 함)
permissions-use-default =
    .label = 기본 설정 사용
security-no-visits = 없음
# This string is used to display the number of meta tags
# in the General Tab
# Variables:
#   $tags (number) - The number of meta tags
general-meta-tags =
    .value =
        { $tags ->
           *[other] 메타 ({ $tags }개 태그)
        }
# This string is used to display the number of times
# the user has visited the website prior
# Variables:
#   $visits (number) - The number of previous visits
security-visits-number =
    { $visits ->
        [0] 아니오
       *[other] 예, { $visits }회
    }
# This string is used to display the size of a media file
# Variables:
#   $kb (number) - The size of an image in Kilobytes
#   $bytes (number) - The size of an image in Bytes
properties-general-size =
    .value =
        { $bytes ->
           *[other] { $kb } KB ({ $bytes } 바이트)
        }
# This string is used to display the type and number
# of frames of a animated image
# Variables:
#   $type (string) - The type of a animated image
#   $frames (number) - The number of frames in an animated image
media-animated-image-type =
    .value =
        { $frames ->
           *[other] { $type } 이미지 (애니메이션, { $frames } 프레임)
        }
# This string is used to display the type of
# an image
# Variables:
#   $type (string) - The type of an image
media-image-type =
    .value = { $type } 형식
# This string is used to display the size of a scaled image
# in both scaled and unscaled pixels
# Variables:
#   $dimx (number) - The horizontal size of an image
#   $dimy (number) - The vertical size of an image
#   $scaledx (number) - The scaled horizontal size of an image
#   $scaledy (number) - The scaled vertical size of an image
media-dimensions-scaled =
    .value = { $dimx }px × { $dimy }px (크기 변경 { $scaledx }px × { $scaledy }px)
# This string is used to display the size of an image in pixels
# Variables:
#   $dimx (number) - The horizontal size of an image
#   $dimy (number) - The vertical size of an image
media-dimensions =
    .value = { $dimx }px × { $dimy }px
# This string is used to display the size of a media
# file in kilobytes
# Variables:
#   $size (number) - The size of the media file in kilobytes
media-file-size = { $size } KB
# This string is used to display the website name next to the
# "Block Images" checkbox in the media tab
# Variables:
#   $website (string) - The website name
media-block-image =
    .label = { $website }에서 이미지 차단
    .accesskey = B
# This string is used to display the URL of the website on top of the
# pageInfo dialog box
# Variables:
#   $website (string) - The url of the website pageInfo is getting info for
page-info-page =
    .title = 페이지 정보 - { $website }
page-info-frame =
    .title = 프레임 정보 - { $website }
