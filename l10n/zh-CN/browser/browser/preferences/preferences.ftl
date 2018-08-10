# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = 向网站发出“请勿跟踪”信号，示明您不想被跟踪
do-not-track-learn-more = 详细了解
do-not-track-option-default =
    .label = 仅在使用跟踪保护时
do-not-track-option-always =
    .label = 始终
pref-page =
    .title =
        { PLATFORM() ->
            [windows] 选项
           *[other] 首选项
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input =
    .style = width: 15.4em
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
            [windows] 在选项中查找
           *[other] 在首选项中查找
        }
policies-notice =
    { PLATFORM() ->
        [windows] 您的组织已禁用某些选项。
       *[other] 您的组织已禁用某些首选项。
    }
pane-general-title = 常规
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = 主页
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = 搜索
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = 隐私与安全
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Firefox 账户
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = { -brand-short-name } 帮助
focus-search =
    .key = f
close-button =
    .aria-label = 关闭

## Browser Restart Dialog

feature-enable-requires-restart = 必须重新打开 { -brand-short-name } 才能启用此功能。
feature-disable-requires-restart = 必须重新打开 { -brand-short-name } 才能禁用此功能。
should-restart-title = 重新打开 { -brand-short-name }
should-restart-ok = 立即重开 { -brand-short-name }
cancel-no-restart-button = 取消
restart-later = 稍后重开

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
extension-controlled-homepage-override = 扩展 <img data-l10n-name="icon"/> { $name } 正在控制您的主页。
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = 扩展 <img data-l10n-name="icon"/> { $name } 正在控制您的新标签页。
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = 您的默认搜索引擎由扩展 <img data-l10n-name="icon"/> { $name } 设置。
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = 扩展 <img data-l10n-name="icon"/> { $name } 需要启用身份标签页功能才能使用。
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = 跟踪保护功能由扩展 <img data-l10n-name="icon"/> { $name } 控制。
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = 扩展 <img data-l10n-name="icon"/> { $name } 正在控制 { -brand-short-name } 如何连接互联网。
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = 要启用该扩展，请从 <img data-l10n-name="menu-icon"/> 菜单打开 <img data-l10n-name="addons-icon"/> 附加组件页面。

## Preferences UI Search Results

search-results-header = 搜索结果
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] 很抱歉，没有找到有关“<span data-l10n-name="query"></span>”的选项。
       *[other] 很抱歉，没有找到有关“<span data-l10n-name="query"></span>”的首选项。
    }
search-results-help-link = 需要帮助？访问 <a data-l10n-name="url">{ -brand-short-name } 技术支持</a>

## General Section

startup-header = 启动
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = 允许 { -brand-short-name } 和 Firefox 同时运行
use-firefox-sync = 提示：这将使用单独的配置文件。使用“同步”在它们之间同步数据。
get-started-not-logged-in = 登录到 { -sync-brand-short-name }…
get-started-configured = 打开{ -sync-brand-short-name }服务首选项
always-check-default =
    .label = 始终检查 { -brand-short-name } 是否是您的默认浏览器
    .accesskey = w
is-default = { -brand-short-name } 目前是您的默认浏览器
is-not-default = { -brand-short-name } 目前不是您的默认浏览器
set-as-my-default-browser =
    .label = 设为默认…
    .accesskey = D
startup-page = 在 { -brand-short-name } 启动时
    .accesskey = s
startup-user-homepage =
    .label = 显示您的主页
startup-blank-page =
    .label = 显示空白页
startup-prev-session =
    .label = 显示您上次的窗口和标签页
startup-restore-previous-session =
    .label = 恢复先前的浏览状态
    .accesskey = s
disable-extension =
    .label = 禁用扩展
home-page-header = 主页
tabs-group-header = 标签页
ctrl-tab-recently-used-order =
    .label = 按下 Ctrl+Tab 时，依照最近使用顺序循环切换标签页
    .accesskey = T
open-new-link-as-tabs =
    .label = 在新标签页中打开链接而不是新窗口
    .accesskey = w
warn-on-close-multiple-tabs =
    .label = 提醒确认关闭多个标签页
    .accesskey = m
warn-on-open-many-tabs =
    .label = 打开多个标签页可能致使 { -brand-short-name } 缓慢时警告
    .accesskey = d
switch-links-to-new-tabs =
    .label = 在新标签页中打开链接时，立即切换过去
    .accesskey = h
show-tabs-in-taskbar =
    .label = 在 Windows 任务栏上显示标签页预览图
    .accesskey = k
browser-containers-enabled =
    .label = 启用身份标签页
    .accesskey = n
browser-containers-learn-more = 详细了解
browser-containers-settings =
    .label = 设置…
    .accesskey = i
containers-disable-alert-title = 关闭所有身份标签页？
containers-disable-alert-desc =
    { $tabCount ->
        [one] 如果您现在禁用身份标签页，将有 { $tabCount } 个容器标签页被关闭。您确实要禁用身份标签页吗？
       *[other] 如果您现在禁用身份标签页，将有 { $tabCount } 个容器标签页被关闭。您确实要禁用身份标签页吗？
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] 关闭 { $tabCount } 个身份标签页
       *[other] 关闭 { $tabCount } 个身份标签页
    }
containers-disable-alert-cancel-button = 保持启用
containers-remove-alert-title = 移除此身份？
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg = 如果您现在移除此身份，{ $count } 个身份标签页将被关闭。您是否确定移除此身份？
containers-remove-ok-button = 移除此身份
containers-remove-cancel-button = 不移除此身份

## General Section - Language & Appearance

language-and-appearance-header = 语言与外观
fonts-and-colors-header = 字体和颜色
default-font = 默认字体
    .accesskey = D
default-font-size = 大小
    .accesskey = S
advanced-fonts =
    .label = 高级…
    .accesskey = A
colors-settings =
    .label = 颜色…
    .accesskey = C
language-header = 语言
choose-language-description = 选择您想要优先使用哪种语言显示页面
choose-button =
    .label = 选择…
    .accesskey = o
translate-web-pages =
    .label = 翻译网页内容
    .accesskey = T
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = 翻译由 <img data-l10n-name="logo"/> 提供
translate-exceptions =
    .label = 例外…
    .accesskey = x
check-user-spelling =
    .label = 在您输入时检查拼写
    .accesskey = t

## General Section - Files and Applications

files-and-applications-title = 文件与应用程序
download-header = 下载
download-save-to =
    .label = 保存文件至
    .accesskey = v
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] 选择…
           *[other] 浏览…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] e
           *[other] o
        }
download-always-ask-where =
    .label = 总是询问您保存文件的位置
    .accesskey = A
applications-header = 应用程序
applications-description = 选择 { -brand-short-name } 如何处理这些文件。
applications-filter =
    .placeholder = 搜索文件类型或应用程序
applications-type-column =
    .label = 内容类型
    .accesskey = T
applications-action-column =
    .label = 操作
    .accesskey = A
drm-content-header = 采用数字版权管理（DRM）的内容
play-drm-content =
    .label = 播放采用 DRM 的内容
    .accesskey = P
play-drm-content-learn-more = 详细了解
update-application-title = { -brand-short-name } 更新
update-application-description = 让 { -brand-short-name } 保持最新，持续拥有最强的性能、稳定性和安全性。
update-application-info = 版本: { $version } <a>新版变化</a>
update-application-version = 版本: { $version } <a data-l10n-name="learn-more">新版变化</a>
update-history =
    .label = 显示更新历史…
    .accesskey = p
update-application-allow-description = 允许 { -brand-short-name }：
update-application-auto =
    .label = 自动安装更新（推荐）
    .accesskey = A
update-application-check-choose =
    .label = 检查更新，但由您决定是否安装
    .accesskey = C
update-application-manual =
    .label = 不检查更新（不推荐）
    .accesskey = N
update-application-use-service =
    .label = 使用一项系统服务以静默安装更新
    .accesskey = b
update-enable-search-update =
    .label = 自动更新搜索引擎
    .accesskey = e

## General Section - Performance

performance-title = 性能
performance-use-recommended-settings-checkbox =
    .label = 使用推荐的性能设置
    .accesskey = U
performance-use-recommended-settings-desc = 自动选择适合此电脑配置的设置。
performance-settings-learn-more = 详细了解
performance-allow-hw-accel =
    .label = 自动启用硬件加速
    .accesskey = r
performance-limit-content-process-option = 内容进程限制
    .accesskey = L
performance-limit-content-process-enabled-desc = 更多内容进程可以改善使用多个标签页时的性能，但也将消耗更多内存。
performance-limit-content-process-disabled-desc = 仅在多进程 { -brand-short-name } 时可修改进程数量。 <a>了解如何检查多进程的启用状况</a>
performance-limit-content-process-blocked-desc = 仅在多进程 { -brand-short-name } 时可修改进程数量。 <a data-l10n-name="learn-more">了解如何检查多进程的启用状况</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (默认)

## General Section - Browsing

browsing-title = 浏览
browsing-use-autoscroll =
    .label = 使用自动滚屏
    .accesskey = a
browsing-use-smooth-scrolling =
    .label = 使用平滑滚动
    .accesskey = m
browsing-use-onscreen-keyboard =
    .label = 在需要时显示触摸键盘
    .accesskey = k
browsing-use-cursor-navigation =
    .label = 始终使用方向键在页面内导航
    .accesskey = c
browsing-search-on-start-typing =
    .label = 若在文本框外输入，则在页面中查找文本
    .accesskey = x

## General Section - Proxy

network-proxy-title = 网络代理
network-proxy-connection-description = 配置 { -brand-short-name } 如何连接互联网。
network-proxy-connection-learn-more = 详细了解
network-proxy-connection-settings =
    .label = 设置…
    .accesskey = e

## Home Section

home-new-windows-tabs-header = 新窗口和标签页
home-new-windows-tabs-description2 = 选择您打开主页、新窗口和新标签页时要看到的内容。

## Home Section - Home Page Customization

home-homepage-mode-label = 主页和新窗口
home-newtabs-mode-label = 新标签页
home-restore-defaults =
    .label = 恢复默认设置
    .accesskey = R
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Firefox 主页（默认）
home-mode-choice-custom =
    .label = 自定义网址…
home-mode-choice-blank =
    .label = 空白页
home-homepage-custom-url =
    .placeholder = 粘贴一个网址…
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] 使用当前页面
           *[other] 使用当前所有页面
        }
    .accesskey = C
choose-bookmark =
    .label = 使用书签…
    .accesskey = B
restore-default =
    .label = 恢复默认设置
    .accesskey = R

## Search Section

search-bar-header = 搜索栏
search-bar-hidden =
    .label = 使用地址栏完成搜索和访问
search-bar-shown =
    .label = 添加搜索栏到工具栏
search-engine-default-header = 默认搜索引擎
search-engine-default-desc = 选择在地址栏和搜索栏中搜索时默认使用的搜索引擎。
search-suggestions-option =
    .label = 提供搜索建议
    .accesskey = s
search-show-suggestions-url-bar-option =
    .label = 在地址栏结果中显示搜索建议
    .accesskey = l
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = 在地址栏显示的结果中，将搜索建议显示在浏览历史上方
search-suggestions-cant-show = 由于您已经设置 { -brand-short-name } 不要记住浏览历史，地址栏中将不会显示搜索建议。
search-one-click-header = 快捷搜索引擎列表
search-one-click-desc = 请选择在地址栏和搜索栏输入关键词时，您希望显示在下方的其他可选用的搜索引擎。
search-choose-engine-column =
    .label = 搜索引擎
search-choose-keyword-column =
    .label = 关键词
search-restore-default =
    .label = 恢复默认搜索引擎
    .accesskey = d
search-remove-engine =
    .label = 移除
    .accesskey = R
search-find-more-link = 寻找更多搜索引擎
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = 关键词重复
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = 您选择的关键词已用于“{ $name }”，请换一个。
search-keyword-warning-bookmark = 您选择的关键词已用于某个书签，请换一个。

## Containers Section

containers-back-link = « 返回
containers-header = 身份标签页
containers-add-button =
    .label = 添加新身份
    .accesskey = A
containers-preferences-button =
    .label = 首选项
containers-remove-button =
    .label = 移除

## Sync Section - Signed out

sync-signedout-caption = 让您个性化的网络体验随身相伴
sync-signedout-description = 在您的各种设备间同步您的书签、历史记录、标签页、密码、附加组件与首选项。
sync-signedout-account-title = 使用 { -fxaccount-brand-name }登录，开启同步功能
sync-signedout-account-create = 没有账户吗？注册一个吧
    .accesskey = C
sync-signedout-account-signin =
    .label = 登录…
    .accesskey = I
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = 在 <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> 或者 <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a> 上安装 Firefox，让您的个性体验随身相伴。

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = 更改头像
sync-disconnect =
    .label = 断开连接…
    .accesskey = D
sync-manage-account = 管理账户
    .accesskey = o
sync-signedin-unverified = { $email } 未验证。
sync-signedin-login-failure = 请登录以重新连接 { $email }
sync-resend-verification =
    .label = 重发验证邮件
    .accesskey = d
sync-remove-account =
    .label = 移除账号
    .accesskey = p
sync-sign-in =
    .label = 登录
    .accesskey = g
sync-signedin-settings-header = 同步设置
sync-signedin-settings-desc = 选择 { -brand-short-name } 要在您的设备间同步哪些数据。
sync-engine-bookmarks =
    .label = 书签
    .accesskey = m
sync-engine-history =
    .label = 历史记录
    .accesskey = r
sync-engine-tabs =
    .label = 打开的标签页
    .tooltiptext = 已同步的所有设备目前打开什么
    .accesskey = T
sync-engine-logins =
    .label = 登录信息
    .tooltiptext = 您已保存的用户名和密码
    .accesskey = L
sync-engine-addresses =
    .label = 地址
    .tooltiptext = 您已保存的邮政地址（仅限桌面版）
    .accesskey = e
sync-engine-creditcards =
    .label = 信用卡
    .tooltiptext = 姓名、号码、有效期限（仅限桌面版）
    .accesskey = C
sync-engine-addons =
    .label = 附加组件
    .tooltiptext = 用于 Firefox 桌面版的扩展和主题
    .accesskey = A
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] 选项
           *[other] 首选项
        }
    .tooltiptext = 您已更改的常规、隐私和安全设置
    .accesskey = s
sync-device-name-header = 设备名称
sync-device-name-change =
    .label = 更改设备名称…
    .accesskey = h
sync-device-name-cancel =
    .label = 取消
    .accesskey = n
sync-device-name-save =
    .label = 保存
    .accesskey = v
sync-mobilepromo-single = 连接其他设备
sync-mobilepromo-multi = 管理设备
sync-tos-link = 服务条款
sync-fxa-privacy-notice = 隐私声明

## Privacy Section

privacy-header = 浏览器隐私

## Privacy Section - Forms

forms-header = 表单与密码
forms-ask-to-save-logins =
    .label = 向您询问是否要记住网站的登录账号和密码
    .accesskey = r
forms-exceptions =
    .label = 例外…
    .accesskey = x
forms-saved-logins =
    .label = 已保存的登录信息…
    .accesskey = L
forms-master-pw-use =
    .label = 使用主密码
    .accesskey = U
forms-master-pw-change =
    .label = 修改主密码…
    .accesskey = M

## Privacy Section - History

history-header = 历史记录
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = { -brand-short-name } 将
    .accesskey = w
history-remember-option-all =
    .label = 记录历史
history-remember-option-never =
    .label = 不记录历史
history-remember-option-custom =
    .label = 使用自定义设置
history-remember-description = { -brand-short-name } 将记住您的浏览、下载、表单和搜索记录。
history-dontremember-description = { -brand-short-name } 将采用与“隐私浏览模式”相同的设置，不会记录您浏览网络的历史。
history-private-browsing-permanent =
    .label = 始终使用隐私浏览模式
    .accesskey = p
history-remember-option =
    .label = 记住我的浏览和下载历史
    .accesskey = b
history-remember-browser-option =
    .label = 记住浏览和下载历史
    .accesskey = b
history-remember-search-option =
    .label = 记住搜索和表单历史
    .accesskey = f
history-clear-on-close-option =
    .label = 在 { -brand-short-name } 关闭时清除历史记录
    .accesskey = r
history-clear-on-close-settings =
    .label = 设置…
    .accesskey = t
history-clear-button =
    .label = 清除历史记录…
    .accesskey = s

## Privacy Section - Site Data

sitedata-header = Cookie 和网站数据
sitedata-total-size-calculating = 正在计算网站数据和缓存的大小…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = 您已存储的 Cookie、网站数据和缓存目前使用 { $value } { $unit } 磁盘空间。
sitedata-learn-more = 详细了解
sitedata-accept-cookies-option =
    .label = 接受来自网站的 Cookie 和网站数据（推荐）
    .accesskey = A
sitedata-block-cookies-option =
    .label = 屏蔽 Cookie 和网站数据（可能导致网站故障）
    .accesskey = B
sitedata-keep-until = 保存，直到
    .accesskey = u
sitedata-keep-until-expire =
    .label = 它们过期
sitedata-keep-until-closed =
    .label = { -brand-short-name } 关闭
sitedata-accept-third-party-desc = 接受第三方 Cookie 和网站数据
    .accesskey = y
sitedata-accept-third-party-always-option =
    .label = 始终
sitedata-accept-third-party-visited-option =
    .label = 来自访问过的网站
sitedata-accept-third-party-never-option =
    .label = 永不
sitedata-clear =
    .label = 清除数据…
    .accesskey = l
sitedata-settings =
    .label = 管理数据…
    .accesskey = M
sitedata-cookies-exceptions =
    .label = 例外…
    .accesskey = E

## Privacy Section - Address Bar

addressbar-header = 地址栏
addressbar-suggest = 使用地址栏时，为我建议：
addressbar-locbar-history-option =
    .label = 浏览历史
    .accesskey = H
addressbar-locbar-bookmarks-option =
    .label = 书签
    .accesskey = k
addressbar-locbar-openpage-option =
    .label = 已打开的标签页
    .accesskey = O
addressbar-suggestions-settings = 更改搜索引擎建议的首选项

## Privacy Section - Tracking

tracking-header = 跟踪保护
tracking-desc = 跟踪保护功能可阻止跟踪者在不同网站间收集您的浏览数据。 <a data-l10n-name="learn-more">详细了解跟踪保护和您的隐私</a>
tracking-mode-label = 使用跟踪保护功能拦截已知的跟踪器
tracking-mode-always =
    .label = 始终
    .accesskey = y
tracking-mode-private =
    .label = 只在隐私窗口中
    .accesskey = l
tracking-mode-never =
    .label = 永不
    .accesskey = n
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = 在隐私浏览模式中使用跟踪保护拦截已知的跟踪器
    .accesskey = v
tracking-exceptions =
    .label = 例外…
    .accesskey = x
tracking-change-block-list =
    .label = 更换拦截列表…
    .accesskey = C

## Privacy Section - Permissions

permissions-header = 权限
permissions-location = 位置
permissions-location-settings =
    .label = 设置…
    .accesskey = l
permissions-camera = 摄像头
permissions-camera-settings =
    .label = 设置…
    .accesskey = c
permissions-microphone = 麦克风
permissions-microphone-settings =
    .label = 设置…
    .accesskey = m
permissions-notification = 通知
permissions-notification-settings =
    .label = 设置…
    .accesskey = n
permissions-notification-link = 详细了解
permissions-notification-pause =
    .label = 暂停通知直至下次打开 { -brand-short-name }
    .accesskey = n
permissions-block-autoplay-media =
    .label = 阻止网站自动播放有声媒体内容
    .accesskey = B
permissions-block-autoplay-media-exceptions =
    .label = 例外…
    .accesskey = E
permissions-block-popups =
    .label = 拦截弹出式窗口
    .accesskey = B
permissions-block-popups-exceptions =
    .label = 例外…
    .accesskey = E
permissions-addon-install-warning =
    .label = 当网站尝试安装附加组件时警告您
    .accesskey = W
permissions-addon-exceptions =
    .label = 例外…
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = 阻止无障碍服务接入您的浏览器
    .accesskey = a
permissions-a11y-privacy-link = 详细了解

## Privacy Section - Data Collection

collection-header = { -brand-short-name } 数据收集与使用
collection-description = 我们力图为您提供选择权，并保证只收集我们为众人提供和改进 { -brand-short-name } 所需的信息。我们仅在征得您的同意后接收个人信息。
collection-privacy-notice = 隐私声明
collection-health-report =
    .label = 允许 { -brand-short-name } 向 { -vendor-short-name } 发送技术信息及交互数据
    .accesskey = r
collection-health-report-link = 详细了解
collection-studies =
    .label = 允许 { -brand-short-name } 安装并运行一些实验项目
collection-studies-link = 查看 { -brand-short-name } 在进行的实验
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = 在此构建配置下数据反馈被禁用
collection-browser-errors =
    .label = 允许 { -brand-short-name } 发送浏览器错误报告（包含错误信息）到 { -vendor-short-name }
    .accesskey = b
collection-browser-errors-link = 详细了解
collection-backlogged-crash-reports =
    .label = 允许 { -brand-short-name } 代您发送积压的崩溃报告
    .accesskey = c
collection-backlogged-crash-reports-link = 详细了解

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = 安全
security-browsing-protection = 欺诈内容和危险软件防护
security-enable-safe-browsing =
    .label = 拦截危险与诈骗内容
    .accesskey = B
security-enable-safe-browsing-link = 详细了解
security-block-downloads =
    .label = 拦截危险的下载项
    .accesskey = D
security-block-uncommon-software =
    .label = 发现流氓软件或罕见软件时发出警告
    .accesskey = C

## Privacy Section - Certificates

certs-header = 证书
certs-personal-label = 当服务器请求您的个人证书时
certs-select-auto-option =
    .label = 自动选择一个
    .accesskey = S
certs-select-ask-option =
    .label = 每次均询问我
    .accesskey = A
certs-enable-ocsp =
    .label = 查询 OCSP 响应服务器，以确认证书当前是否有效
    .accesskey = Q
certs-view =
    .label = 查看证书…
    .accesskey = C
certs-devices =
    .label = 安全设备…
    .accesskey = D
