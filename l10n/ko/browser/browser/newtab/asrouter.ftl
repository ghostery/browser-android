# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cfr-doorhanger-extension-heading = 추천 확장기능
cfr-doorhanger-extension-sumo-link =
    .tooltiptext = 왜 이게 나왔나요
cfr-doorhanger-extension-cancel-button = 나중에
    .accesskey = N
cfr-doorhanger-extension-ok-button = 지금 추가
    .accesskey = A
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
