# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = 傳送 “Do Not Track” 訊號，告訴網站您不想被追蹤
do-not-track-learn-more = 了解更多
do-not-track-option-default =
    .label = 只在使用追蹤保護功能時
do-not-track-option-default-content-blocking =
    .label = 僅在 { -brand-short-name } 設定封鎖偵測到的追蹤器時
do-not-track-option-always =
    .label = 總是
pref-page =
    .title =
        { PLATFORM() ->
            [windows] 選項
           *[other] 偏好設定
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
            [windows] 在選項中尋找
           *[other] 在偏好設定中尋找
        }
policies-notice =
    { PLATFORM() ->
        [windows] 您的組織已鎖定某些選項。
       *[other] 您的組織已鎖定某些偏好設定。
    }
pane-general-title = 一般
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = 首頁
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = 搜尋
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = 隱私權與安全性
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Firefox 帳號
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = { -brand-short-name } 技術支援
focus-search =
    .key = f
close-button =
    .aria-label = 關閉

## Browser Restart Dialog

feature-enable-requires-restart = 必須重新啟動 { -brand-short-name } 才能啟用此功能。
feature-disable-requires-restart = 必須重新啟動 { -brand-short-name } 才能停用此功能。
should-restart-title = 重新啟動 { -brand-short-name }
should-restart-ok = 立刻重新啟動 { -brand-short-name }
cancel-no-restart-button = 取消
restart-later = 稍後再重新啟動

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
extension-controlled-homepage-override = 擴充套件「<img data-l10n-name="icon"/> { $name }」正在控制您的首頁內容。
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = 擴充套件「<img data-l10n-name="icon"/> { $name }」正在控制您的新分頁內容。
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = 擴充套件「<img data-l10n-name="icon"/> { $name }」修改了您的預設搜尋引擎。
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = 擴充套件「<img data-l10n-name="icon"/> { $name }」需要開啟容器分頁功能才能使用。
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = 擴充套件「<img data-l10n-name="icon"/> { $name }」正在控制您的追蹤保護設定。
# This string is shown to notify the user that their content blocking "All Detected Trackers"
# preferences are being controlled by an extension.
extension-controlled-websites-content-blocking-all-trackers = 擴充套件<img data-l10n-name="icon"/> { $name } 正在控制此設定。
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = 擴充套件「<img data-l10n-name="icon"/> { $name }」正在控制您的 { -brand-short-name } 如何連線至網際網路。
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = 要啟用這套擴充套件，請到 <img data-l10n-name="menu-icon"/> 選單中的 <img data-l10n-name="addons-icon"/> 附加元件。

## Preferences UI Search Results

search-results-header = 搜尋結果
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] 抱歉！沒有「<span data-l10n-name="query"></span>」的選項搜尋結果。
       *[other] 抱歉！沒有「<span data-l10n-name="query"></span>」的偏好設定搜尋結果。
    }
search-results-help-link = 需要幫忙嗎？請到 <a data-l10n-name="url">{ -brand-short-name } 技術支援</a>

## General Section

startup-header = 啟動
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = 允許 { -brand-short-name } 與 Firefox 同時執行
use-firefox-sync = 小秘訣: 這樣會使用不同設定檔，可使用 { -sync-brand-short-name } 在兩套瀏覽器間同步資料。
get-started-not-logged-in = 登入 { -sync-brand-short-name }…
get-started-configured = 開啟 { -sync-brand-short-name } 偏好設定
always-check-default =
    .label = 總是檢查 { -brand-short-name } 是否為您的預設瀏覽器
    .accesskey = w
is-default = { -brand-short-name } 是您目前的預設瀏覽器
is-not-default = { -brand-short-name } 不是您的預設瀏覽器
set-as-my-default-browser =
    .label = 設為預設瀏覽器…
    .accesskey = D
startup-restore-previous-session =
    .label = 回復先前的瀏覽狀態
    .accesskey = s
disable-extension =
    .label = 停用擴充套件
tabs-group-header = 分頁
ctrl-tab-recently-used-order =
    .label = 按下 Ctrl+Tab 時，依照最近使用分頁的順序切換分頁標籤
    .accesskey = T
open-new-link-as-tabs =
    .label = 以分頁開啟鏈結，而不使用新視窗
    .accesskey = w
warn-on-close-multiple-tabs =
    .label = 關閉多個分頁前警告您
    .accesskey = m
warn-on-open-many-tabs =
    .label = 開啟多個分頁時，警告我可能會拖慢 { -brand-short-name }
    .accesskey = d
switch-links-to-new-tabs =
    .label = 用新分頁開啟鏈結後，自動切換至該分頁
    .accesskey = h
show-tabs-in-taskbar =
    .label = 在 Windows 工作列顯示分頁預覽圖
    .accesskey = k
browser-containers-enabled =
    .label = 開啟容器分頁
    .accesskey = n
browser-containers-learn-more = 詳細資訊
browser-containers-settings =
    .label = 設定…
    .accesskey = i
containers-disable-alert-title = 要關閉所有容器分頁嗎？
containers-disable-alert-desc = 若您現在停用容器分頁，將關閉 { $tabCount } 個分頁。您真的要停用容器分頁嗎？
containers-disable-alert-ok-button = 關閉 { $tabCount } 個容器分頁
containers-disable-alert-cancel-button = 保持開啟
containers-remove-alert-title = 要移除這個容器嗎？
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg = 若您現在移除此容器，將關閉 { $count } 個容器分頁。您確定要移除此容器嗎？
containers-remove-ok-button = 移除此容器
containers-remove-cancel-button = 不要移除此容器

## General Section - Language & Appearance

language-and-appearance-header = 語言與外觀
fonts-and-colors-header = 字型與色彩
default-font = 預設字型
    .accesskey = D
default-font-size = 大小
    .accesskey = S
advanced-fonts =
    .label = 進階…
    .accesskey = A
colors-settings =
    .label = 色彩…
    .accesskey = C
language-header = 語言
choose-language-description = 請選擇瀏覽支援多國語言的網頁時要優先顯示哪種語言
choose-button =
    .label = 選擇…
    .accesskey = o
choose-browser-language-description = 請選擇 { -brand-short-name } 顯示選單、訊息、通知時使用的語言。
manage-browser-languages-button =
    .label = 設定其他語言…
    .accesskey = l
confirm-browser-language-change-description = 重新啟動 { -brand-short-name } 來套用變更
confirm-browser-language-change-button = 套用並重新啟動
translate-web-pages =
    .label = 翻譯網頁內容
    .accesskey = T
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = 翻譯服務由 <img data-l10n-name="logo"/> 提供
translate-exceptions =
    .label = 例外網站…
    .accesskey = x
check-user-spelling =
    .label = 打字時即時檢查拼字
    .accesskey = t

## General Section - Files and Applications

files-and-applications-title = 檔案與應用程式
download-header = 下載
download-save-to =
    .label = 儲存檔案到
    .accesskey = v
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] 選擇…
           *[other] 瀏覽…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] e
           *[other] o
        }
download-always-ask-where =
    .label = 每次都問您要存到何處
    .accesskey = A
applications-header = 應用程式
applications-description = 選擇 { -brand-short-name } 要如何處理您上網時下載的檔案。
applications-filter =
    .placeholder = 搜尋檔案類型或應用程式
applications-type-column =
    .label = 內容類型
    .accesskey = t
applications-action-column =
    .label = 動作
    .accesskey = A
drm-content-header = 數位權利管理（DRM）內容
play-drm-content =
    .label = 播放 DRM 內容
    .accesskey = P
play-drm-content-learn-more = 詳細資訊
update-application-title = { -brand-short-name } 更新
update-application-description = 保持更新 { -brand-short-name }，獲得最佳效能、穩定度以及安全性。
update-application-version = { $version }版 <a data-l10n-name="learn-more">有什麼新鮮事</a>
update-history =
    .label = 顯示更新紀錄…
    .accesskey = p
update-application-allow-description = 讓 { -brand-short-name }:
update-application-auto =
    .label = 自動安裝更新（推薦）
    .accesskey = A
update-application-check-choose =
    .label = 自動檢查更新，但讓您選擇要不要安裝
    .accesskey = C
update-application-manual =
    .label = 不要檢查更新（不建議，可能會有安全性風險）
    .accesskey = N
update-application-use-service =
    .label = 在背景服務當中安裝更新
    .accesskey = b
update-enable-search-update =
    .label = 自動更新搜尋引擎
    .accesskey = e

## General Section - Performance

performance-title = 效能
performance-use-recommended-settings-checkbox =
    .label = 使用推薦的效能設定
    .accesskey = U
performance-use-recommended-settings-desc = 這些設定是根據您電腦的硬體與作業系統所量身訂做。
performance-settings-learn-more = 了解更多
performance-allow-hw-accel =
    .label = 可用時開啟硬體加速
    .accesskey = r
performance-limit-content-process-option = 內容處理程序數量限制
    .accesskey = L
performance-limit-content-process-enabled-desc = 調高內容處理程序的數量，可改善開啟多個分頁時的效能，但也會使用更多記憶體。
performance-limit-content-process-blocked-desc = 僅能在多程序的 { -brand-short-name } 當中修改內容處理程序數量。 <a data-l10n-name="learn-more">了解如何確認多程序模式是否已開啟</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num }（預設）

## General Section - Browsing

browsing-title = 瀏覽
browsing-use-autoscroll =
    .label = 使用自動捲動
    .accesskey = a
browsing-use-smooth-scrolling =
    .label = 使用平滑捲動
    .accesskey = m
browsing-use-onscreen-keyboard =
    .label = 需要時顯示觸控鍵盤
    .accesskey = k
browsing-use-cursor-navigation =
    .label = 總是使用鍵盤方向鍵瀏覽網頁（鍵盤瀏覽）
    .accesskey = c
browsing-search-on-start-typing =
    .label = 打字時直接搜尋頁面文字（隨打即找）
    .accesskey = x

## General Section - Proxy

network-proxy-title = 網路代理伺服器
network-settings-title = 網路設定
network-proxy-connection-description = 設定 { -brand-short-name } 要如何連線至網際網路。
network-proxy-connection-learn-more = 了解更多
network-proxy-connection-settings =
    .label = 設定…
    .accesskey = e

## Home Section

home-new-windows-tabs-header = 新視窗與分頁
home-new-windows-tabs-description2 = 選擇開啟首頁、新視窗和新分頁時要看到的內容。

## Home Section - Home Page Customization

home-homepage-mode-label = 首頁與新視窗
home-newtabs-mode-label = 新分頁
home-restore-defaults =
    .label = 回復為預設值
    .accesskey = R
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Firefox 首頁（預設值）
home-mode-choice-custom =
    .label = 自訂網址…
home-mode-choice-blank =
    .label = 空白頁
home-homepage-custom-url =
    .placeholder = 貼上網址…
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] 使用目前頁面
           *[other] 使用目前所有頁面
        }
    .accesskey = C
choose-bookmark =
    .label = 使用書籤…
    .accesskey = B

## Search Section

search-bar-header = 搜尋列
search-bar-hidden =
    .label = 使用網址列搜尋與上網
search-bar-shown =
    .label = 在工具列加入搜尋列
search-engine-default-header = 預設搜尋引擎
search-engine-default-desc = 請選擇在網址列或搜尋列進行搜尋時，預設要使用的搜尋引擎。
search-suggestions-option =
    .label = 提供搜尋建議
    .accesskey = s
search-show-suggestions-url-bar-option =
    .label = 在網址列結果中顯示搜尋建議
    .accesskey = l
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = 在網址列顯示的結果中，將搜尋建議放在瀏覽紀錄前面
search-suggestions-cant-show = 由於您已經設定 { -brand-short-name } 不要記住瀏覽紀錄，網址列中將不會顯示建議搜尋結果。
search-one-click-header = 快捷搜尋引擎清單
search-one-click-desc = 請選擇當您在網址列或搜尋列輸入關鍵字時，可選用的其他搜尋引擎。
search-choose-engine-column =
    .label = 搜尋引擎
search-choose-keyword-column =
    .label = 關鍵字
search-restore-default =
    .label = 還原預設搜尋引擎
    .accesskey = d
search-remove-engine =
    .label = 移除
    .accesskey = r
search-find-more-link = 尋找更多搜尋引擎
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = 關鍵字重複
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = 您選用的關鍵字目前正被「{ $name }」所使用，請另選一個。
search-keyword-warning-bookmark = 您選用的關鍵字目前正被書籤項目所使用，請另選一個。

## Containers Section

containers-back-link = « 返回
containers-header = 容器分頁
containers-add-button =
    .label = 新增容器
    .accesskey = A
containers-preferences-button =
    .label = 偏好設定
containers-remove-button =
    .label = 移除

## Sync Section - Signed out

sync-signedout-caption = 把 Web 隨身帶著走
sync-signedout-description = 在您所有裝置間同步書籤、歷史紀錄、分頁、密碼、附加元件與偏好設定。
sync-signedout-account-title = 連線到 { -fxaccount-brand-name }
sync-signedout-account-create = 沒有帳號嗎？開始使用
    .accesskey = C
sync-signedout-account-signin =
    .label = 登入…
    .accesskey = I
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = 下載 Firefox for<img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a>或<img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a>以與您的行動裝置同步。

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = 更改個人資料照片
sync-disconnect =
    .label = 斷線…
    .accesskey = D
sync-manage-account = 管理帳號
    .accesskey = o
sync-signedin-unverified = { $email } 未驗證。
sync-signedin-login-failure = 請登入以重新連線 { $email }
sync-resend-verification =
    .label = 重寄驗證信
    .accesskey = d
sync-remove-account =
    .label = 移除帳號
    .accesskey = p
sync-sign-in =
    .label = 登入
    .accesskey = g
sync-signedin-settings-header = 同步設定
sync-signedin-settings-desc = 請選擇要同步哪些資料到您其他使用 { -brand-short-name } 的裝置上。
sync-engine-bookmarks =
    .label = 書籤
    .accesskey = m
sync-engine-history =
    .label = 瀏覽紀錄
    .accesskey = r
sync-engine-tabs =
    .label = 開啟分頁
    .tooltiptext = 所有同步設備中，開啟的網頁清單
    .accesskey = T
sync-engine-logins =
    .label = 登入資訊
    .tooltiptext = 您儲存的使用者名稱與密碼
    .accesskey = L
sync-engine-addresses =
    .label = 地址
    .tooltiptext = 您儲存的郵政地址（僅桌機）
    .accesskey = e
sync-engine-creditcards =
    .label = 信用卡
    .tooltiptext = 姓名、卡號及到期日（僅桌機）
    .accesskey = C
sync-engine-addons =
    .label = 附加元件
    .tooltiptext = Firefox 桌機版的擴充套件與佈景主題
    .accesskey = A
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] 選項
           *[other] 偏好設定
        }
    .tooltiptext = 您調整過的一般、隱私權、安全性設定
    .accesskey = s
sync-device-name-header = 裝置名稱
sync-device-name-change =
    .label = 更改裝置名稱…
    .accesskey = h
sync-device-name-cancel =
    .label = 取消
    .accesskey = n
sync-device-name-save =
    .label = 儲存
    .accesskey = v
sync-mobilepromo-single = 連結其他裝置
sync-mobilepromo-multi = 管理裝置
sync-tos-link = 服務條款
sync-fxa-privacy-notice = 隱私權公告

## Privacy Section

privacy-header = 瀏覽器隱私權

## Privacy Section - Forms

forms-header = 表單與密碼
forms-ask-to-save-logins =
    .label = 向您詢問是否要記住網站的登入帳號與密碼
    .accesskey = r
forms-exceptions =
    .label = 例外網站…
    .accesskey = x
forms-saved-logins =
    .label = 已存登入資訊…
    .accesskey = L
forms-master-pw-use =
    .label = 使用主控密碼
    .accesskey = U
forms-master-pw-change =
    .label = 變更主控密碼
    .accesskey = M

## Privacy Section - History

history-header = 瀏覽紀錄
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = { -brand-short-name } 將
    .accesskey = w
history-remember-option-all =
    .label = 保留所有歷史記錄
history-remember-option-never =
    .label = 不保留歷史記錄
history-remember-option-custom =
    .label = 使用自訂的設定
history-remember-description = { -brand-short-name } 將記住您的瀏覽、下載、表單填寫與搜尋紀錄。
history-dontremember-description = { -brand-short-name } 不會儲存任何瀏覽記錄，如同在隱私瀏覽狀態下執行。
history-private-browsing-permanent =
    .label = 總是使用隱私瀏覽模式
    .accesskey = p
history-remember-browser-option =
    .label = 保留瀏覽與下載記錄
    .accesskey = b
history-remember-search-option =
    .label = 保留我的搜尋列與表單輸入的內容
    .accesskey = f
history-clear-on-close-option =
    .label = 結束 { -brand-short-name } 時清除歷史記錄
    .accesskey = r
history-clear-on-close-settings =
    .label = 設定…
    .accesskey = t
history-clear-button =
    .label = 清除瀏覽記錄…
    .accesskey = s

## Privacy Section - Site Data

sitedata-header = Cookie 與網站資料
sitedata-total-size-calculating = 正在計算網站資料與快取大小…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = 您儲存的 Cookie、網站資料與快取目前使用了 { $value } { $unit } 磁碟空間
sitedata-learn-more = 了解更多
sitedata-accept-cookies-option =
    .label = 接受網站設定的 Cookie 與網站資料（建議）
    .accesskey = A
sitedata-block-cookies-option =
    .label = 封鎖 Cookie 與網站資料（可能會造成某些網站運作不正常）
    .accesskey = B
sitedata-keep-until = 保留 Cookie 直到
    .accesskey = u
sitedata-keep-until-expire =
    .label = Cookie 過期
sitedata-keep-until-closed =
    .label = 關閉 { -brand-short-name }
sitedata-accept-third-party-desc = 接受第三方 Cookie 與網站資料
    .accesskey = y
sitedata-accept-third-party-always-option =
    .label = 總是
sitedata-accept-third-party-visited-option =
    .label = 造訪過的網站
sitedata-accept-third-party-never-option =
    .label = 永不
sitedata-allow-cookies-option =
    .label = 允許網站設定 Cookie 與網站資料
    .accesskey = A
sitedata-disallow-cookies-option =
    .label = 封鎖 Cookie 與網站資料
    .accesskey = B
# This label means 'type of content that is blocked', and is followed by a drop-down list with content types below.
# The list items are the strings named sitedata-block-*-option*.
sitedata-block-desc = 要封鎖的類型
    .accesskey = T
sitedata-block-trackers-option-recommended =
    .label = 第三方追蹤器（推薦）
sitedata-block-trackers-option =
    .label = 第三方追蹤器
sitedata-block-unvisited-option =
    .label = 未造訪過的網站的 Cookie
sitedata-block-all-third-parties-option =
    .label = 所有第三方追蹤器
sitedata-block-always-option =
    .label = 所有 Cookie（可能會造成網站運作不正常）
sitedata-block-all-third-party-option =
    .label = 所有第三方 Cookie（可能會造成某些網站不正常）
sitedata-block-all-option =
    .label = 所有 Cookie（會造成大部分網站不正常）
sitedata-clear =
    .label = 清除資料…
    .accesskey = l
sitedata-settings =
    .label = 管理資料…
    .accesskey = M
sitedata-cookies-exceptions =
    .label = 例外網站…
    .accesskey = E
# This is a warning message shown next to a yellow warning icon when the Cookies and Site Data subsection
# in Preferences has been disabled due to Content Blocking being disabled. It is displayed next to the
# Cookies and Site Data section.
sitedata-warning-your-settings-prevent-changes = 您的內容封鎖設定不允許調整 Cookie 與網站資料設定。

## Privacy Section - Address Bar

addressbar-header = 網址列
addressbar-suggest = 在使用網址列時提供建議
addressbar-locbar-history-option =
    .label = 瀏覽紀錄
    .accesskey = H
addressbar-locbar-bookmarks-option =
    .label = 書籤
    .accesskey = k
addressbar-locbar-openpage-option =
    .label = 開啟分頁
    .accesskey = O
addressbar-suggestions-settings = 修改搜尋建議偏好設定

## Privacy Section - Content Blocking

content-blocking-header = 內容封鎖
content-blocking-desc = 封鎖會拖慢您上網速度並追蹤您的廣告或程式碼等第三方網頁內容。可調整相關設定以在保護與效能間取得最佳平衡。
content-blocking-learn-more = 了解更多
content-blocking-restore-defaults =
    .label = 回復為預設值
    .accesskey = R
content-blocking-toggle-on =
    .tooltiptext = 關閉內容封鎖
content-blocking-toggle-off =
    .tooltiptext = 開啟內容封鎖
content-blocking-toggle-label-on = 開啟
    .accesskey = O
content-blocking-toggle-label-off = 關閉
    .accesskey = O
content-blocking-category-label = 選擇要封鎖的內容
# "Slow" in this instance means "slow to load on the network".
# FastBlock is a feature that blocks requests to tracking sites if they
# have not finished loading after a certain threshold of seconds.
content-blocking-fastblock-label = 很慢才載入的追蹤元素
    .accesskey = S
content-blocking-fastblock-description = 封鎖超過 5 秒仍未載入完成的第三方內容。
content-blocking-fastblock-option-enabled =
    .label = 總是封鎖
content-blocking-fastblock-option-disabled =
    .label = 永不封鎖
content-blocking-tracking-protection-label = 追蹤器
    .accesskey = T
content-blocking-tracking-protection-description = 封鎖所有已知的追蹤器（可能也會無法載入某些頁面）。
content-blocking-tracking-protection-option-enabled =
    .label = 總是封鎖
content-blocking-tracking-protection-option-pbm =
    .label = 僅在隱私瀏覽視窗封鎖
content-blocking-tracking-protection-option-disabled =
    .label = 永不封鎖
content-blocking-tracking-protection-change-blocklist = 變更封鎖清單…
# "Slow" in this instance means "slow to load on the network".
# FastBlock is a feature that blocks requests to tracking sites if they
# have not finished loading after a certain threshold of seconds.
content-blocking-fastblock-slow-loading-trackers-label =
    .label = 慢速載入的追蹤器
    .accesskey = S
content-blocking-fastblock-new-description = 只封鎖會影響頁面載入速度的追蹤器。
content-blocking-tracking-protection-all-detected-trackers-label =
    .label = 所有偵測到的追蹤器
    .accesskey = T
content-blocking-tracking-protection-new-description = 封鎖所有已知的追蹤器（可能會影響某些頁面載入。）
content-blocking-tracking-protection-option-always =
    .label = 總是
    .accesskey = A
content-blocking-tracking-protection-option-private =
    .label = 僅在隱私瀏覽視窗
    .accesskey = p
content-blocking-tracking-protection-change-block-list = 變更封鎖清單
content-blocking-third-party-cookies-label =
    .label = 第三方 Cookie
    .accesskey = C
content-blocking-reject-trackers-description = 封鎖所有第三方 Cookie，或只封鎖追蹤器設定的 Cookie
# This is a warning message shown next to a yellow warning icon when the Third-Party Cookies subsection
# of the Content Blocking UI in Preferences has been disabled due to the either the "All cookies" option
# or the "Cookies from unvisited websites" option being selected in the Cookies and Site Data section of
# the UI.
content-blocking-reject-trackers-warning-your-settings-prevent-changes = 您的 Cookie 與網站資料設定不允許調整第三方 Cookie 的設定。
content-blocking-change-cookie-settings =
    .label = 變更 Cookie 設定
    .accesskey = S
content-blocking-reject-trackers-block-trackers-option-recommended =
    .label = 追蹤器（建議）
    .accesskey = k
content-blocking-reject-trackers-block-trackers-option =
    .label = 追蹤器
    .accesskey = k
content-blocking-reject-trackers-all-third-parties-option =
    .label = 所有第三方 Cookie（可能會造成某些網站不正常）
    .accesskey = A

## Privacy Section - Tracking

tracking-header = 追蹤保護
tracking-desc = 追蹤保護功能會封鎖在不同網站間收集您的瀏覽資料的網路追蹤器。 <a data-l10n-name="learn-more">了解有關追蹤保護功能，以及您的隱私權的更多資訊</a>
tracking-mode-label = 使用追蹤保護功能，封鎖已知的追蹤器
tracking-mode-always =
    .label = 總是
    .accesskey = y
tracking-mode-private =
    .label = 僅在隱私瀏覽視窗
    .accesskey = l
tracking-mode-never =
    .label = 永不
    .accesskey = n
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = 在隱私瀏覽模式使用追蹤保護功能，封鎖已知的追蹤器
    .accesskey = v
tracking-exceptions =
    .label = 例外網站…
    .accesskey = x
tracking-change-block-list =
    .label = 變更封鎖清單…
    .accesskey = C

## Privacy Section - Permissions

permissions-header = 權限
permissions-location = 位置
permissions-location-settings =
    .label = 設定…
    .accesskey = l
permissions-camera = 攝影機
permissions-camera-settings =
    .label = 設定…
    .accesskey = c
permissions-microphone = 麥克風
permissions-microphone-settings =
    .label = 設定…
    .accesskey = m
permissions-notification = 通知
permissions-notification-settings =
    .label = 設定…
    .accesskey = n
permissions-notification-link = 了解更多
permissions-notification-pause =
    .label = 暫停通知到 { -brand-short-name } 重新啟動後
    .accesskey = n
permissions-block-autoplay-media =
    .label = 防止網站自動播放有聲音的媒體內容
    .accesskey = B
permissions-block-autoplay-media-menu = 針對會自動播放音效的網站
permissions-block-autoplay-media-exceptions =
    .label = 例外網站…
    .accesskey = E
autoplay-option-ask =
    .label = 總是詢問是否要自動播放
autoplay-option-allow =
    .label = 允許自動播放
autoplay-option-dont =
    .label = 不要自動播放
permissions-autoplay-link = 了解更多
permissions-block-popups =
    .label = 封鎖彈出型視窗
    .accesskey = B
permissions-block-popups-exceptions =
    .label = 例外網站…
    .accesskey = E
permissions-addon-install-warning =
    .label = 當網站嘗試安裝附加元件時警告您
    .accesskey = W
permissions-addon-exceptions =
    .label = 例外網站…
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = 防止無障礙輔助功能存取您的瀏覽器
    .accesskey = a
permissions-a11y-privacy-link = 了解更多

## Privacy Section - Data Collection

collection-header = { -brand-short-name } 資料收集與使用
collection-description = 我們致力於提供您選擇，也只會收集我們在提供與改善 { -brand-short-name } 時所必需的資料。我們也一定會經過您的同意才收集您的個人資訊。
collection-privacy-notice = 隱私權公告
collection-health-report =
    .label = 允許 { -brand-short-name } 傳送技術與互動資料給 { -vendor-short-name }
    .accesskey = r
collection-health-report-link = 更多資訊
collection-studies =
    .label = 允許 { -brand-short-name } 安裝並進行研究
collection-studies-link = 檢視 { -brand-short-name } 所進行的研究
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = 進行編譯設定時，已停用了資料回報功能
collection-browser-errors =
    .label = 允許 { -brand-short-name } 向 { -vendor-short-name } 發送瀏覽器錯誤報告（包括錯誤訊息）
    .accesskey = b
collection-browser-errors-link = 了解更多
collection-backlogged-crash-reports =
    .label = 允許 { -brand-short-name } 為您傳送先前紀錄下的錯誤報告
    .accesskey = c
collection-backlogged-crash-reports-link = 更多資訊

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = 安全性
security-browsing-protection = 詐欺內容與危險網站保護
security-enable-safe-browsing =
    .label = 封鎖危險及詐騙內容
    .accesskey = B
security-enable-safe-browsing-link = 了解更多
security-block-downloads =
    .label = 封鎖危險的下載項目
    .accesskey = D
security-block-uncommon-software =
    .label = 下載不安全或不常見的軟體時警告您
    .accesskey = C

## Privacy Section - Certificates

certs-header = 憑證
certs-personal-label = 當伺服器要求您的個人憑證時
certs-select-auto-option =
    .label = 自動選擇一組憑證
    .accesskey = S
certs-select-ask-option =
    .label = 每次詢問您
    .accesskey = A
certs-enable-ocsp =
    .label = 向 OCSP 回應伺服器查詢，以確認憑證有效性
    .accesskey = Q
certs-view =
    .label = 檢視憑證…
    .accesskey = C
certs-devices =
    .label = 安全性裝置…
    .accesskey = D
