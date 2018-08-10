# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = ウェブサイトに “Do Not Track” 信号を送り、追跡されたくないことを知らせます
do-not-track-learn-more = 詳細情報
do-not-track-option-default =
    .label = トラッキング防止の使用時のみ
do-not-track-option-always =
    .label = 常に送る
pref-page =
    .title =
        { PLATFORM() ->
            [windows] オプション
           *[other] 設定
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
            [windows] オプションを検索
           *[other] 設定を検索
        }
policies-notice =
    { PLATFORM() ->
        [windows] あなたの所属組織が一部のオプションの変更を制限しています。
       *[other] あなたの所属組織が一部の設定の変更を制限しています。
    }
pane-general-title = 一般
category-general =
    .tooltiptext = { pane-general-title }
pane-search-title = 検索
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = プライバシーとセキュリティ
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Firefox アカウント
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = { -brand-short-name } サポート
focus-search =
    .key = f
close-button =
    .aria-label = 閉じる

## Browser Restart Dialog

feature-enable-requires-restart = この機能を有効にするには、{ -brand-short-name } の再起動が必要です
feature-disable-requires-restart = この機能を無効にするには、{ -brand-short-name } の再起動が必要です
should-restart-title = { -brand-short-name } を再起動
should-restart-ok = { -brand-short-name } を今すぐ再起動
cancel-no-restart-button = キャンセル
restart-later = 後で再起動

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
extension-controlled-homepage-override = 拡張機能 <img data-l10n-name="icon"/> { $name } によりホームページ設定が変更されています。
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = 拡張機能 <img data-l10n-name="icon"/> { $name } により新しいタブページが変更されています。
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = 拡張機能 <img data-l10n-name="icon"/> { $name } により既定の検索エンジンが変更されています。
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = 拡張機能 <img data-l10n-name="icon"/> { $name } がコンテナータブを必要としています。
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = 拡張機能 <img data-l10n-name="icon"/> { $name } がトラッキング防止の設定を変更しています。
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = 拡張機能 <img data-l10n-name="icon"/> { $name } が { -brand-short-name } のインターネット接続方法の設定を変更しています。
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = 拡張機能を有効にするには、<img data-l10n-name="menu-icon"/> メニューから <img data-l10n-name="addons-icon"/> アドオンを開きます。

## Preferences UI Search Results

search-results-header = 検索結果
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] “<span data-l10n-name="query"></span>” オプションについての検索結果はありません。
       *[other] “<span data-l10n-name="query"></span>” 設定についての検索結果はありません。
    }
search-results-help-link = 助けが必要な方は、<a data-l10n-name="url">{ -brand-short-name } サポート</a> をご利用ください

## General Section

startup-header = 起動
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = { -brand-short-name } と Firefox の同時起動を許可する
use-firefox-sync = ヒント: これは別のプロファイルを使用します。データを共有するには { -sync-brand-short-name } を使用してください。
get-started-not-logged-in = { -sync-brand-short-name } にログインする...
get-started-configured = { -sync-brand-short-name } 設定を開く
always-check-default =
    .label = { -brand-short-name } が既定のブラウザーか確認する
    .accesskey = y
is-default = { -brand-short-name } は既定のブラウザーに設定されています
is-not-default = { -brand-short-name } は既定のブラウザーに設定されていません
set-as-my-default-browser =
    .label = 既定のブラウザーにする...
    .accesskey = D
startup-page = { -brand-short-name } を起動するとき
    .accesskey = s
startup-user-homepage =
    .label = ホームページを表示する
startup-blank-page =
    .label = 空白ページを表示する
startup-prev-session =
    .label = 前回終了時のウィンドウとタブを表示する
disable-extension =
    .label = 拡張機能を無効化
home-page-header = ホームページ
tabs-group-header = タブグループ
ctrl-tab-recently-used-order =
    .label = Ctrl+Tab で最近使用した順にタブを切り替える
    .accesskey = T
open-new-link-as-tabs =
    .label = 新しいウィンドウではなく新しいタブに開く
    .accesskey = w
warn-on-close-multiple-tabs =
    .label = 同時に複数のタブを閉じるときは確認する
    .accesskey = m
warn-on-open-many-tabs =
    .label = 同時に複数のタブを開いて { -brand-short-name } の動作が遅くなるときは確認する
    .accesskey = d
switch-links-to-new-tabs =
    .label = リンクを新しいタブで開いたとき、すぐにそのタブに切り替える
    .accesskey = h
show-tabs-in-taskbar =
    .label = タスクバーにタブのプレビューを表示する
    .accesskey = k
browser-containers-enabled =
    .label = コンテナータブを有効にする
    .accesskey = n
browser-containers-learn-more = 詳細情報
browser-containers-settings =
    .label = 設定...
    .accesskey = i
containers-disable-alert-title = すべてのコンテナータブを閉じますか？
containers-disable-alert-desc = コンテナータブを無効にすると、{ $tabCount } 個のコンテナータブが閉じられます。コンテナータブを無効にしますか？
containers-disable-alert-ok-button = { $tabCount } 個のコンテナータブを閉じる
containers-disable-alert-cancel-button = このままにする
containers-remove-alert-title = このコンテナーを削除しますか？
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg = このコンテナーを削除すると { $count } 個のコンテナータブが閉じられます。このコンテナーを削除してもよろしいですか？
containers-remove-ok-button = このコンテナーを削除する
containers-remove-cancel-button = 削除しない

## General Section - Language & Appearance

language-and-appearance-header = 言語と外観
fonts-and-colors-header = フォントと配色
default-font = 既定のフォント
    .accesskey = D
default-font-size = サイズ
    .accesskey = S
advanced-fonts =
    .label = 詳細設定...
    .accesskey = A
colors-settings =
    .label = 配色設定...
    .accesskey = C
language-header = 言語
choose-language-description = ウェブページの表示に使用する言語の優先順位を設定できます。
choose-button =
    .label = 言語設定...
    .accesskey = o
translate-web-pages =
    .label = ウェブページを翻訳
    .accesskey = T
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = 翻訳:  <img data-l10n-name="logo"/>
translate-exceptions =
    .label = 例外...
    .accesskey = x
check-user-spelling =
    .label = 自動スペルチェック機能を使用する
    .accesskey = t

## General Section - Files and Applications

files-and-applications-title = ファイルとプログラム
download-header = ダウンロード
download-save-to =
    .label = 次のフォルダーに保存する
    .accesskey = v
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] 選択...
           *[other] 参照...
        }
    .accesskey =
        { PLATFORM() ->
            [macos] e
           *[other] o
        }
download-always-ask-where =
    .label = ファイルごとに保存先を指定する
    .accesskey = A
applications-header = プログラム
applications-description = ウェブからダウンロードしたファイルを { -brand-short-name } が取り扱う方法とブラウズ中に使用するプログラムを選んでください。
applications-filter =
    .placeholder = ファイルの種類とプログラムを検索
applications-type-column =
    .label = ファイルの種類 (Content Type)
    .accesskey = T
applications-action-column =
    .label = 取り扱い方法
    .accesskey = A
drm-content-header = デジタル著作権管理 (DRM) コンテンツ
play-drm-content =
    .label = DRM 制御のコンテンツを再生
    .accesskey = P
play-drm-content-learn-more = 詳細情報
update-application-title = { -brand-short-name } の更新
update-application-description = 最高のパフォーマンスと安定性、セキュリティを提供するため { -brand-short-name } を最新の状態に保ちます。
update-application-info = バージョン { $version } <a>更新情報</a>
update-application-version = バージョン { $version } <a data-l10n-name="learn-more">更新情報</a>
update-history =
    .label = 更新履歴を表示...
    .accesskey = p
update-application-allow-description = { -brand-short-name } の更新動作
update-application-auto =
    .label = 更新を自動的にインストールする (推奨)
    .accesskey = A
update-application-check-choose =
    .label = 更新の確認は行うが、インストールするかを選択する
    .accesskey = C
update-application-manual =
    .label = 更新の確認は行わない (非推奨)
    .accesskey = N
update-application-use-service =
    .label = 更新のインストールにバックグラウンドサービスを使用する
    .accesskey = b
update-enable-search-update =
    .label = 検索エンジンを自動的に更新する
    .accesskey = e

## General Section - Performance

performance-title = パフォーマンス
performance-use-recommended-settings-checkbox =
    .label = 推奨のパフォーマンス設定を使用する
    .accesskey = U
performance-use-recommended-settings-desc = これらの設定はコンピューターのハードウェアとオペレーティングシステムに合わせて調整されます。
performance-settings-learn-more = 詳細情報
performance-allow-hw-accel =
    .label = ハードウェアアクセラレーション機能を使用する (可能な場合)
    .accesskey = r
performance-limit-content-process-option = コンテンツプロセス数の制限
    .accesskey = L
performance-limit-content-process-enabled-desc = コンテンツプロセスを増やすと、複数タブの使用時にパフォーマンスが向上しますが、メモリーを多く消費します。
performance-limit-content-process-disabled-desc = コンテンツプロセスの数はマルチプロセスの { -brand-short-name } でしか変更できません。 <a>マルチプロセスが有効になっているか確認してください</a>
performance-limit-content-process-blocked-desc = コンテンツプロセスの数はマルチプロセスの { -brand-short-name } でしか変更できません。 <a data-l10n-name="learn-more">マルチプロセスが有効になっているか確認してください</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (既定)

## General Section - Browsing

browsing-title = ブラウズ
browsing-use-autoscroll =
    .label = 自動スクロール機能を使用する
    .accesskey = a
browsing-use-smooth-scrolling =
    .label = スムーズスクロール機能を使用する
    .accesskey = m
browsing-use-onscreen-keyboard =
    .label = 必要であればタッチキーボードを表示する
    .accesskey = k
browsing-use-cursor-navigation =
    .label = ページ中の移動にカーソルを使用する (キャレットブラウズモード)
    .accesskey = c
browsing-search-on-start-typing =
    .label = キー入力時に検索を開始する
    .accesskey = x

## General Section - Proxy

network-proxy-title = ネットワークプロキシ
network-proxy-connection-description = { -brand-short-name } のインターネットへの接続方法を設定します。
network-proxy-connection-learn-more = 詳細
network-proxy-connection-settings =
    .label = 接続設定...
    .accesskey = e

## Home Section


## Home Section - Home Page Customization

# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] 現在のページを使用
           *[other] 現在のタブグループを使用
        }
    .accesskey = C
choose-bookmark =
    .label = ブックマークを使う...
    .accesskey = B
restore-default =
    .label = 初期設定に戻す
    .accesskey = R

## Search Section

search-bar-header = 検索バー
search-bar-hidden =
    .label = 検索とナビゲーションにアドレスバーを使用する
search-bar-shown =
    .label = 検索バーをツールバーに追加する
search-engine-default-header = 既定の検索エンジン
search-engine-default-desc = アドレスバーと検索バーで使用される既定の検索エンジンを選択してください。
search-suggestions-option =
    .label = 検索候補を使用する
    .accesskey = s
search-show-suggestions-url-bar-option =
    .label = アドレスバーに検索候補を表示する
    .accesskey = l
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = アドレスバーの入力候補は、検索候補を閲覧履歴より先に表示する
search-suggestions-cant-show = 履歴を保存しない設定となっているため、ロケーションバーに検索候補が表示されません。
search-one-click-header = ワンクリック検索エンジン
search-one-click-desc = キーワードの入力開始時にアドレスバーや検索バーの下に現れる代替検索エンジンを選んでください。
search-choose-engine-column =
    .label = 検索エンジン
search-choose-keyword-column =
    .label = キーワード
search-restore-default =
    .label = 既定の検索エンジンを復元
    .accesskey = d
search-remove-engine =
    .label = 削除
    .accesskey = r
search-find-more-link = 他の検索エンジンを追加
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = キーワードが重複しています
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = "{ $name }" で使用されているキーワードを指定しました。他のものを指定してください。
search-keyword-warning-bookmark = ブックマークで使用されているキーワードを指定しました。他のものを指定してください。

## Containers Section

containers-back-link = « 戻る
containers-header = コンテナータブ
containers-add-button =
    .label = 新しいコンテナーを追加
    .accesskey = A
containers-preferences-button =
    .label = 設定
containers-remove-button =
    .label = 削除

## Sync Section - Signed out

sync-signedout-caption = ウェブを持ち歩こう
sync-signedout-description = ブックマークやページの表示履歴、タブ、パスワード、アドオン、設定を、お持ちのすべての端末間で同期できます。
sync-signedout-account-title = { -fxaccount-brand-name } で接続する
sync-signedout-account-create = アカウントをお持ちでない方は作成してください
    .accesskey = C
sync-signedout-account-signin =
    .label = ログイン...
    .accesskey = I
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Firefox for <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> または <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a> をダウンロードしてモバイル端末と同期しましょう。

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = プロフィール写真を変更します
sync-disconnect =
    .label = 切断...
    .accesskey = D
sync-manage-account = アカウントの管理
    .accesskey = o
sync-signedin-unverified = { $email } は認証されていません。
sync-signedin-login-failure = { $email } に再接続するにはログインしてください。
sync-resend-verification =
    .label = 認証メールを再送信
    .accesskey = d
sync-remove-account =
    .label = アカウントを削除
    .accesskey = R
sync-sign-in =
    .label = ログイン
    .accesskey = g
sync-signedin-settings-header = Sync 設定
sync-signedin-settings-desc = 端末上の同期して { -brand-short-name } で使用するものを選んでください。
sync-engine-bookmarks =
    .label = ブックマーク
    .accesskey = m
sync-engine-history =
    .label = 履歴
    .accesskey = r
sync-engine-tabs =
    .label = 開いたタブ
    .tooltiptext = 同期した端末で開いているすべてのタブの一覧です
    .accesskey = T
sync-engine-logins =
    .label = ログイン情報
    .tooltiptext = 保存したユーザー名とパスワードです
    .accesskey = L
sync-engine-addresses =
    .label = 所在地フォーム
    .tooltiptext = 保存した住所です (デスクトップのみ)
    .accesskey = e
sync-engine-creditcards =
    .label = クレジットカード
    .tooltiptext = クレジットカードの名義と番号と有効期限です (デスクトップのみ)
    .accesskey = C
sync-engine-addons =
    .label = アドオン
    .tooltiptext = デスクトップ版 Firefox の拡張機能とテーマです
    .accesskey = A
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] オプション
           *[other] 個人設定
        }
    .tooltiptext = 変更した一般、プライバシー、セキュリティの設定です
    .accesskey = S
sync-device-name-header = 端末名
sync-device-name-change =
    .label = 端末名を変更...
    .accesskey = h
sync-device-name-cancel =
    .label = キャンセル
    .accesskey = n
sync-device-name-save =
    .label = 保存
    .accesskey = v
sync-mobilepromo-single = 別の端末を接続
sync-mobilepromo-multi = 端末を管理
sync-tos-link = 利用規約
sync-fxa-privacy-notice = 個人情報保護方針

## Privacy Section

privacy-header = ブラウザープライバシー

## Privacy Section - Forms

forms-header = フォームとパスワード
forms-exceptions =
    .label = 例外サイト...
    .accesskey = x
forms-saved-logins =
    .label = 保存されているログイン情報...
    .accesskey = L
forms-master-pw-use =
    .label = マスターパスワードを使用する
    .accesskey = U
forms-master-pw-change =
    .label = マスターパスワードを変更...
    .accesskey = M

## Privacy Section - History

history-header = 履歴
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = { -brand-short-name } に
    .accesskey = w
history-remember-option-all =
    .label = 履歴を記憶させる
history-remember-option-never =
    .label = 履歴を一切記憶させない
history-remember-option-custom =
    .label = 記憶させる履歴を詳細設定する
history-remember-description = { -brand-short-name } は表示したページの履歴、ファイルのダウンロード履歴、検索やフォームの入力履歴を保存します。
history-dontremember-description = { -brand-short-name } はプライベートブラウジング中と同様に、表示したページの履歴などのプライバシーデータを一切保存しません。
history-private-browsing-permanent =
    .label = 常にプライベートブラウジングモード
    .accesskey = p
history-remember-option =
    .label = 表示したページとファイルのダウンロードの履歴を残す
    .accesskey = b
history-remember-search-option =
    .label = 検索やフォームの入力履歴を記憶させる
    .accesskey = f
history-clear-on-close-option =
    .label = { -brand-short-name } の終了時に履歴を消去する
    .accesskey = r
history-clear-on-close-settings =
    .label = 設定...
    .accesskey = t
history-clear-button =
    .label = 履歴を消去...
    .accesskey = s

## Privacy Section - Site Data

sitedata-header = Cookie とサイトデータ
sitedata-learn-more = 詳細情報
sitedata-accept-cookies-option =
    .label = ウェブサイトから送られてきた Cookie とサイトデータを保存する (推奨)
    .accesskey = A
sitedata-block-cookies-option =
    .label = Cookie とサイトデータをブロック (ウェブサイトが動作しない可能性があります)
    .accesskey = B
sitedata-keep-until = Cookie を保存する期間
    .accesskey = u
sitedata-accept-third-party-desc = サードパーティの Cookie とサイトデータを保存
    .accesskey = y
sitedata-accept-third-party-always-option =
    .label = 常に許可
sitedata-accept-third-party-visited-option =
    .label = 訪問したサイトのみ許可
sitedata-accept-third-party-never-option =
    .label = 常に拒否
sitedata-clear =
    .label = データを消去...
    .accesskey = l
sitedata-settings =
    .label = データを管理...
    .accesskey = M
sitedata-cookies-exceptions =
    .label = 例外サイト...
    .accesskey = E

## Privacy Section - Address Bar

addressbar-header = アドレスバー
addressbar-suggest = アドレスバーの使用時に表示する候補
addressbar-locbar-history-option =
    .label = ブラウジング履歴
    .accesskey = H
addressbar-locbar-bookmarks-option =
    .label = ブックマーク
    .accesskey = k
addressbar-locbar-openpage-option =
    .label = 開いているタブ
    .accesskey = O
addressbar-suggestions-settings = 検索エンジンの検索候補の設定を変更

## Privacy Section - Tracking

tracking-header = トラッキング防止
tracking-desc = トラッキング防止は、複数のウェブサイトにまたがるユーザーのブラウジングデータを収集するオンラインの追跡者をブロックします。 <a data-l10n-name="learn-more">トラッキング防止とプライバシーの詳細情報</a>
tracking-mode-label = トラッキング防止を使用して既知の追跡者をブロックする
tracking-mode-always =
    .label = 常に
    .accesskey = y
tracking-mode-private =
    .label = プライベートウィンドウのみ
    .accesskey = l
tracking-mode-never =
    .label = ブロックしない
    .accesskey = n
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = プライベートブラウジングモードでトラッキング防止を使用して既知の追跡者をブロックする
    .accesskey = v
tracking-exceptions =
    .label = 例外...
    .accesskey = x
tracking-change-block-list =
    .label = ブロックリストを変更...
    .accesskey = C

## Privacy Section - Permissions

permissions-header = 許可設定
permissions-location = 位置情報
permissions-location-settings =
    .label = 設定...
    .accesskey = t
permissions-camera = カメラ
permissions-camera-settings =
    .label = 設定...
    .accesskey = t
permissions-microphone = マイク
permissions-microphone-settings =
    .label = 設定...
    .accesskey = t
permissions-notification = 通知
permissions-notification-settings =
    .label = 設定...
    .accesskey = t
permissions-notification-link = 詳細情報
permissions-notification-pause =
    .label = { -brand-short-name } を再起動するまで通知を一時停止
    .accesskey = n
permissions-block-popups =
    .label = ポップアップウィンドウをブロックする
    .accesskey = B
permissions-block-popups-exceptions =
    .label = 許可サイト...
    .accesskey = E
permissions-addon-install-warning =
    .label = アドオンのインストールを求められたときに警告する
    .accesskey = W
permissions-addon-exceptions =
    .label = 許可サイト...
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = アクセシビリティサービスによるブラウザーへのアクセスを止める
    .accesskey = a
permissions-a11y-privacy-link = 詳細情報

## Privacy Section - Data Collection

collection-header = { -brand-short-name } のデータ収集と利用について
collection-description = 私たちはユーザーに選択肢を提供し、{ -brand-short-name } をすべての人に提供し改善するために必要なものだけを収集するよう努力しています。私たちは、個人情報を受け取る前に、常にユーザーの許可を求めます。
collection-privacy-notice = 個人情報保護方針
collection-health-report =
    .label = { -brand-short-name } が技術的な対話データを { -vendor-short-name } へ送信することを許可する
    .accesskey = r
collection-health-report-link = 詳細情報
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = このビルド設定ではヘルスレポートが無効化されています
collection-browser-errors =
    .label = { -brand-short-name } がブラウザーエラーレポート (エラーメッセージ含む) を { -vendor-short-name } へ送信することを許可する
    .accesskey = b
collection-browser-errors-link = 詳細情報
collection-backlogged-crash-reports =
    .label = { -brand-short-name } があなたに代わって未送信のクラッシュレポートを送信することを許可する
    .accesskey = c
collection-backlogged-crash-reports-link = 詳細情報

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = セキュリティ
security-browsing-protection = 詐欺コンテンツと危険なソフトウェアからの防護
security-enable-safe-browsing =
    .label = 危険な詐欺コンテンツをブロックする
    .accesskey = B
security-enable-safe-browsing-link = 詳細情報
security-block-downloads =
    .label = 危険なファイルのダウンロードをブロックする
    .accesskey = D
security-block-uncommon-software =
    .label = 不要な危険ソフトウェアを警告する
    .accesskey = C

## Privacy Section - Certificates

certs-header = 証明書
certs-personal-label = サーバーが個人証明書を要求したとき
certs-select-auto-option =
    .label = 自動的に選択する
    .accesskey = S
certs-select-ask-option =
    .label = 毎回自分で選択する
    .accesskey = A
certs-enable-ocsp =
    .label = OCSP レスポンダーサーバーに問い合わせて証明書の現在の正当性を確認する
    .accesskey = Q
certs-view =
    .label = 証明書を表示...
    .accesskey = C
certs-devices =
    .label = セキュリティデバイス...
    .accesskey = D
