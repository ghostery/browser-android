# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

choose-messenger-language-description = { -brand-short-name }가 메뉴와 메시지, 알림을 표시할 언어를 선택하세요.
manage-messenger-languages-button =
    .label = 대체 수단 설정…
    .accesskey = l
confirm-messenger-language-change-description = 변경사항 적용을 위해 { -brand-short-name }를 재시작
confirm-messenger-language-change-button = 적용하고 재시작
update-pref-write-failure-title = 저장 실패
# Variables:
#   $path (String) - Path to the configuration file
update-pref-write-failure-message = 설정을 저장할 수 없습니다. 파일에 저장할 수 없습니다: { $path }
update-setting-write-failure-title = 업데이트 설정 저장 중 오류 발생
# Variables:
#   $path (String) - Path to the configuration file
# The newlines between the main text and the line containing the path is
# intentional so the path is easier to identify.
update-setting-write-failure-message =
    오류가 발생하여 { -brand-short-name }가 이 변경 내용을 저장하지 않았습니다. 이 업데이트 환경 설정을 하려면 아래 파일에 쓰기 권한이 필요합니다. 사용자나 시스템 관리자가 사용자 그룹에 이 파일에 대한 모든 권한을 부여하여 오류를 해결할 수 있습니다.
    
    파일에 쓸 수 없음: { $path }
update-in-progress-title = 업데이트 진행중
update-in-progress-message = { -brand-short-name }가 이 업데이트를 계속하기를 원하시나요?
update-in-progress-ok-button = 취소
# Continue is the cancel button so pressing escape or using a platform standard
# method of closing the UI will not discard the update.
update-in-progress-cancel-button = 계속
