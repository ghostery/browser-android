# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = Передавать сайтам сигнал “Не отслеживать”, означающий, чтобы вы не хотите быть отслеживаемыми
do-not-track-learn-more = Подробнее
do-not-track-option-default =
    .label = Только при использовании защиты от отслеживания
do-not-track-option-always =
    .label = Всегда
pref-page =
    .title =
        { PLATFORM() ->
            [windows] Настройки
           *[other] Настройки
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
            [windows] Найти в настройках
           *[other] Найти в настройках
        }
policies-notice =
    { PLATFORM() ->
        [windows] Ваша организация запретила возможность изменения некоторых настроек.
       *[other] Ваша организация запретила возможность изменения некоторых настроек.
    }
pane-general-title = Основные
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = Начало
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = Поиск
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = Приватность и Защита
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Аккаунт Firefox
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = Поддержка { -brand-short-name }
focus-search =
    .key = f
close-button =
    .aria-label = Закрыть

## Browser Restart Dialog

feature-enable-requires-restart = Для включения этого режима необходимо перезапустить { -brand-short-name }.
feature-disable-requires-restart = Для отключения этого режима необходимо перезапустить { -brand-short-name }.
should-restart-title = Перезапуск { -brand-short-name }
should-restart-ok = Перезапустить { -brand-short-name } сейчас
cancel-no-restart-button = Отмена
restart-later = Перезапустить позже

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
extension-controlled-homepage-override = Расширение <img data-l10n-name="icon"/> { $name } контролирует вашу домашнюю страницу.
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = Расширение <img data-l10n-name="icon"/> { $name } контролирует вашу страницу новой вкладки.
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = Расширение <img data-l10n-name="icon"/> { $name } установило вашу поисковую систему по умолчанию.
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = Расширение <img data-l10n-name="icon"/> { $name } требует для своей работы «Вкладки в контейнере».
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = Расширение <img data-l10n-name="icon"/> { $name } контролирует параметры защиты от отслеживания.
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = Расширение <img data-l10n-name="icon"/> { $name } контролирует способ соединения { -brand-short-name } с Интернетом.
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = Чтобы включить расширение, перейдите в пункт <img data-l10n-name="addons-icon"/> Дополнения меню <img data-l10n-name="menu-icon"/>.

## Preferences UI Search Results

search-results-header = Результаты поиска
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] Извините! В настройках не найдено результатов для «<span data-l10n-name="query"></span>».
       *[other] Извините! В настройках не найдено результатов для «<span data-l10n-name="query"></span>».
    }
search-results-help-link = Нужна помощь? Посетите <a data-l10n-name="url">Сайт поддержки { -brand-short-name }</a>

## General Section

startup-header = Запуск
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = Разрешить одновременный запуск { -brand-short-name } и Firefox
use-firefox-sync = Совет: При этом используются отдельные профили. Используйте Синхронизацию для обмена между ними данными.
get-started-not-logged-in = Войти в Синхронизацию…
get-started-configured = Открыть настройки Синхронизации
always-check-default =
    .label = Всегда проверять, является ли { -brand-short-name } вашим браузером по умолчанию
    .accesskey = а
is-default = В настоящий момент { -brand-short-name } является вашим браузером по умолчанию
is-not-default = { -brand-short-name } не является вашим браузером по умолчанию
set-as-my-default-browser =
    .label = Установить по умолчанию…
    .accesskey = н
startup-page = При запуске { -brand-short-name }
    .accesskey = и
startup-user-homepage =
    .label = Показать домашнюю страницу
startup-blank-page =
    .label = Показать пустую страницу
startup-prev-session =
    .label = Показать окна и вкладки, открытые в прошлый раз
startup-restore-previous-session =
    .label = Восстановить предыдущую сессию
    .accesskey = о
disable-extension =
    .label = Отключить расширение
home-page-header = Домашняя страница
tabs-group-header = Вкладки
ctrl-tab-recently-used-order =
    .label = Ctrl+Tab переключает между вкладками в порядке недавнего использования
    .accesskey = ж
open-new-link-as-tabs =
    .label = Открывать ссылки во вкладках вместо новых окон
    .accesskey = ы
warn-on-close-multiple-tabs =
    .label = Предупреждать при закрытии нескольких вкладок
    .accesskey = д
warn-on-open-many-tabs =
    .label = Предупреждать, когда открытие нескольких вкладок может замедлить { -brand-short-name }
    .accesskey = р
switch-links-to-new-tabs =
    .label = Переключаться на открываемую вкладку
    .accesskey = к
show-tabs-in-taskbar =
    .label = Отображать эскизы вкладок на панели задач Windows
    .accesskey = б
browser-containers-enabled =
    .label = Включить «Вкладки в контейнере»
    .accesskey = ч
browser-containers-learn-more = Подробнее
browser-containers-settings =
    .label = Параметры…
    .accesskey = м
containers-disable-alert-title = Закрыть все вкладки в контейнере?
containers-disable-alert-desc =
    { $tabCount ->
        [one] Если вы отключите сейчас вкладки в контейнере, { $tabCount } вкладка в контейнере будет закрыта. Вы уверены, что хотите отключить вкладки в контейнере?
        [few] Если вы отключите сейчас вкладки в контейнере, { $tabCount } вкладки в контейнере будут закрыты. Вы уверены, что хотите отключить вкладки в контейнере?
       *[many] Если вы отключите сейчас вкладки в контейнере, { $tabCount } вкладок в контейнере будут закрыты. Вы уверены, что хотите отключить вкладки в контейнере?
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] Закрыть { $tabCount } вкладку в контейнере
        [few] Закрыть { $tabCount } вкладки в контейнере
       *[many] Закрыть { $tabCount } вкладок в контейнере
    }
containers-disable-alert-cancel-button = Оставить включёнными
containers-remove-alert-title = Удалить этот контейнер?
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg =
    { $count ->
        [one] Если вы удалите сейчас этот контейнер, { $count } вкладка в контейнере будет закрыта. Вы уверены, что хотите удалить этот контейнер?
        [few] Если вы удалите сейчас этот контейнер, { $count } вкладки в контейнере будут закрыты. Вы уверены, что хотите удалить этот контейнер?
       *[many] Если вы удалите сейчас этот контейнер, { $count } вкладок в контейнере будут закрыты. Вы уверены, что хотите удалить этот контейнер?
    }
containers-remove-ok-button = Удалить этот контейнер
containers-remove-cancel-button = Не удалять этот контейнер

## General Section - Language & Appearance

language-and-appearance-header = Язык и внешний вид
fonts-and-colors-header = Шрифты и цвета
default-font = Шрифт по умолчанию
    .accesskey = ф
default-font-size = Размер
    .accesskey = м
advanced-fonts =
    .label = Дополнительно…
    .accesskey = н
colors-settings =
    .label = Цвета…
    .accesskey = в
language-header = Язык
choose-language-description = Выберите язык, предпочитаемый вами для отображения веб-страниц
choose-button =
    .label = Выбрать…
    .accesskey = ы
translate-web-pages =
    .label = Перевод веб-страниц
    .accesskey = и
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = Перевод выполняется <img data-l10n-name="logo"/>
translate-exceptions =
    .label = Исключения…
    .accesskey = л
check-user-spelling =
    .label = Проверять орфографию при наборе текста
    .accesskey = в

## General Section - Files and Applications

files-and-applications-title = Файлы и Приложения
download-header = Загрузки
download-save-to =
    .label = Путь для сохранения файлов
    .accesskey = ь
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] Выбрать…
           *[other] Обзор…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] ы
           *[other] б
        }
download-always-ask-where =
    .label = Всегда выдавать запрос на сохранение файлов
    .accesskey = е
applications-header = Приложения
applications-description = Выберите, как { -brand-short-name } будет обрабатывать файлы, загружаемые из Интернета, или приложения, используемые при работе в Интернете.
applications-filter =
    .placeholder = Поиск типов файлов или приложений
applications-type-column =
    .label = Тип содержимого
    .accesskey = о
applications-action-column =
    .label = Действие
    .accesskey = е
drm-content-header = Содержимое использующее технические средства защиты авторских прав (DRM)
play-drm-content =
    .label = Воспроизводить контролируемое DRM содержимое
    .accesskey = п
play-drm-content-learn-more = Подробнее
update-application-title = Обновления { -brand-short-name }
update-application-description = Используйте последнюю версию { -brand-short-name } для наилучшей производительности, стабильности и безопасности.
update-application-info = Версия { $version } <a>Что нового</a>
update-application-version = Версия { $version } <a data-l10n-name="learn-more">Что нового</a>
update-history =
    .label = Показать журнал обновлений…
    .accesskey = ж
update-application-allow-description = Разрешить { -brand-short-name }
update-application-auto =
    .label = Автоматически устанавливать обновления (рекомендуется)
    .accesskey = ч
update-application-check-choose =
    .label = Проверять наличие обновлений, но позволять вам решать, устанавливать ли их
    .accesskey = в
update-application-manual =
    .label = Никогда не проверять наличие обновлений (не рекомендуется)
    .accesskey = и
update-application-use-service =
    .label = Использовать фоновую службу для установки обновлений
    .accesskey = ф
update-enable-search-update =
    .label = Автоматически обновлять поисковые системы
    .accesskey = л

## General Section - Performance

performance-title = Производительность
performance-use-recommended-settings-checkbox =
    .label = Использовать рекомендуемые настройки производительности
    .accesskey = н
performance-use-recommended-settings-desc = Эти настройки рассчитаны для вашего компьютера и операционной системы.
performance-settings-learn-more = Подробнее
performance-allow-hw-accel =
    .label = По возможности использовать аппаратное ускорение
    .accesskey = л
performance-limit-content-process-option = Максимальное число процессов контента
    .accesskey = в
performance-limit-content-process-enabled-desc = Дополнительные процессы контента могут улучшить производительность при работе со множеством вкладок, но также повысят потребление памяти.
performance-limit-content-process-disabled-desc = Изменение числа процессов контента возможно только в мультипроцессном { -brand-short-name }. <a>Узнайте, как проверить, включена ли мультипроцессность</a>
performance-limit-content-process-blocked-desc = Изменение числа процессов контента возможно только в мультипроцессном { -brand-short-name }. <a data-l10n-name="learn-more">Узнайте, как проверить, включена ли мультипроцессность</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (по умолчанию)

## General Section - Browsing

browsing-title = Просмотр сайтов
browsing-use-autoscroll =
    .label = Использовать автоматическую прокрутку
    .accesskey = с
browsing-use-smooth-scrolling =
    .label = Использовать плавную прокрутку
    .accesskey = о
browsing-use-onscreen-keyboard =
    .label = При необходимости отображать сенсорную клавиатуру
    .accesskey = н
browsing-use-cursor-navigation =
    .label = Всегда использовать клавиши курсора для навигации по страницам
    .accesskey = к
browsing-search-on-start-typing =
    .label = Искать текст на странице по мере его набора
    .accesskey = а

## General Section - Proxy

network-proxy-title = Прокси-сервер
network-proxy-connection-description = Настроить, как { -brand-short-name } соединяется с Интернетом.
network-proxy-connection-learn-more = Подробнее
network-proxy-connection-settings =
    .label = Настроить…
    .accesskey = с

## Home Section

home-new-windows-tabs-header = Новые окна и вкладки
home-new-windows-tabs-description2 = Выберите, что вы хотите увидеть, когда откроете домашнюю страницу, новые окна и новые вкладки.

## Home Section - Home Page Customization

home-homepage-mode-label = Домашняя страница и новые окна
home-newtabs-mode-label = Новые вкладки
home-restore-defaults =
    .label = Восстановить по умолчанию
    .accesskey = о
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Домашняя страница Firefox (по умолчанию)
home-mode-choice-custom =
    .label = Мои URL...
home-mode-choice-blank =
    .label = Пустая страница
home-homepage-custom-url =
    .placeholder = Вставьте URL...
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] Использовать текущую страницу
           *[other] Использовать текущие страницы
        }
    .accesskey = п
choose-bookmark =
    .label = Использовать закладку…
    .accesskey = в
restore-default =
    .label = Восстановить по умолчанию
    .accesskey = о

## Search Section

search-bar-header = Панель поиска
search-bar-hidden =
    .label = Использовать адресную строку для поиска и навигации
search-bar-shown =
    .label = Добавить панель поиска на панель инструментов
search-engine-default-header = Поисковая система по умолчанию
search-engine-default-desc = Выберите поисковую систему по умолчанию для использования в панели адреса и панели поиска.
search-suggestions-option =
    .label = Отображать поисковые предложения
    .accesskey = о
search-show-suggestions-url-bar-option =
    .label = Отображать поисковые предложения при использовании панели адреса
    .accesskey = ж
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = Отображать поисковые предложения перед историей веб-сёрфинга при использовании панели адреса
search-suggestions-cant-show = При использовании панели адреса поисковые предложения отображаться не будут, так как вы настроили { -brand-short-name } никогда не запоминать историю.
search-one-click-header = Поиск одним щелчком
search-one-click-desc = Выберите альтернативные поисковые системы, которые появятся под панелью адреса и панелью поиска, когда вы начнёте вводить ключевое слово.
search-choose-engine-column =
    .label = Поисковая система
search-choose-keyword-column =
    .label = Краткое имя
search-restore-default =
    .label = Восстановить набор поисковых систем по умолчанию
    .accesskey = а
search-remove-engine =
    .label = Удалить
    .accesskey = и
search-find-more-link = Найти другие поисковые системы
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = Краткое имя уже используется
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = Вы выбрали краткое имя, которое в данный момент используется «{ $name }». Пожалуйста, выберите другое.
search-keyword-warning-bookmark = Вы выбрали краткое имя, которое в данный момент используется одной из закладок. Пожалуйста, выберите другое.

## Containers Section

containers-back-link = « Вернуться назад
containers-header = Вкладки в контейнере
containers-add-button =
    .label = Добавить новый контейнер
    .accesskey = а
containers-preferences-button =
    .label = Настройки
containers-remove-button =
    .label = Удалить

## Sync Section - Signed out

sync-signedout-caption = Возьмите свой Интернет с собой
sync-signedout-description = Синхронизируйте свои закладки, историю, вкладки, пароли, дополнения и настройки на всех ваших устройствах.
sync-signedout-account-title = Соединитесь с Аккаунтом Firefox
sync-signedout-account-create = Нет аккаунта? Давайте его создадим
    .accesskey = е
sync-signedout-account-signin =
    .label = Войти…
    .accesskey = о
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Загрузите Firefox для <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> или <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS,</a> чтобы синхронизироваться со своим мобильным устройством.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = Изменить фотографию в профиле
sync-disconnect =
    .label = Отсоединить…
    .accesskey = н
sync-manage-account = Управление аккаунтом
    .accesskey = в
sync-signedin-unverified = { $email } не подтверждён.
sync-signedin-login-failure = Войдите для повтора соединения с { $email }
sync-resend-verification =
    .label = Повторить отправку подтверждения
    .accesskey = и
sync-remove-account =
    .label = Удалить аккаунт
    .accesskey = л
sync-sign-in =
    .label = Войти
    .accesskey = о
sync-signedin-settings-header = Параметры синхронизации
sync-signedin-settings-desc = Выберите, что нужно синхронизировать на ваших устройствах при помощи { -brand-short-name }.
sync-engine-bookmarks =
    .label = Закладки
    .accesskey = к
sync-engine-history =
    .label = Историю
    .accesskey = т
sync-engine-tabs =
    .label = Открытые вкладки
    .tooltiptext = Список того, что открыто на всех синхронизированных устройствах
    .accesskey = л
sync-engine-logins =
    .label = Логины
    .tooltiptext = Сохранённые имена пользователей и пароли
    .accesskey = ы
sync-engine-addresses =
    .label = Адреса
    .tooltiptext = Сохранённые вами почтовые адреса (только для компьютера)
    .accesskey = с
sync-engine-creditcards =
    .label = Банковские карты
    .tooltiptext = Имена, номера и сроки действия (только для компьютера)
    .accesskey = н
sync-engine-addons =
    .label = Дополнения
    .tooltiptext = Расширения и темы для Firefox на компьютере
    .accesskey = п
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] Настройки
           *[other] Настройки
        }
    .tooltiptext = Изменённые вами настройки: Общие, Приватность и Защита
    .accesskey = й
sync-device-name-header = Имя устройства
sync-device-name-change =
    .label = Изменить имя устройства…
    .accesskey = м
sync-device-name-cancel =
    .label = Отмена
    .accesskey = е
sync-device-name-save =
    .label = Сохранить
    .accesskey = х
sync-mobilepromo-single = Подключить другое устройство
sync-mobilepromo-multi = Управление устройствами
sync-tos-link = Условия службы
sync-fxa-privacy-notice = Уведомление о приватности

## Privacy Section

privacy-header = Приватность браузера

## Privacy Section - Forms

forms-header = Формы и Пароли
forms-ask-to-save-logins =
    .label = Запрашивать сохранение логинов и паролей для веб-сайтов
    .accesskey = ш
forms-exceptions =
    .label = Исключения…
    .accesskey = ю
forms-saved-logins =
    .label = Сохранённые логины…
    .accesskey = х
forms-master-pw-use =
    .label = Использовать мастер-пароль
    .accesskey = о
forms-master-pw-change =
    .label = Сменить мастер-пароль…
    .accesskey = м

## Privacy Section - History

history-header = История
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = { -brand-short-name }
    .accesskey = i
history-remember-option-all =
    .label = будет запоминать историю
history-remember-option-never =
    .label = не будет запоминать историю
history-remember-option-custom =
    .label = будет использовать ваши настройки хранения истории
history-remember-description = { -brand-short-name } будет помнить историю посещений, загрузок, поиска и сохранять данные форм.
history-dontremember-description = { -brand-short-name } будет использовать те же настройки, что и при приватном просмотре, и не будет помнить историю вашей работы с веб-сайтами.
history-private-browsing-permanent =
    .label = Всегда работать в режиме приватного просмотра
    .accesskey = т
history-remember-option =
    .label = Помнить историю посещений и загрузок
    .accesskey = с
history-remember-browser-option =
    .label = Помнить историю посещений и загрузок
    .accesskey = и
history-remember-search-option =
    .label = Помнить историю поиска и данных форм
    .accesskey = и
history-clear-on-close-option =
    .label = Удалять историю при закрытии { -brand-short-name }
    .accesskey = я
history-clear-on-close-settings =
    .label = Параметры…
    .accesskey = ы
history-clear-button =
    .label = Удалить историю…
    .accesskey = л

## Privacy Section - Site Data

sitedata-header = Куки и данные сайтов
sitedata-total-size-calculating = Вычисление объема данных сайтов и кэша…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = Ваши сохранённые куки, данные сайтов и кэш сейчас занимают на диске { $value } { $unit }.
sitedata-learn-more = Подробнее
sitedata-accept-cookies-option =
    .label = Принимать куки и данные сайтов с веб-сайтов (рекомендуется)
    .accesskey = и
sitedata-block-cookies-option =
    .label = Блокировать куки и данные сайтов (может нарушить работу веб-сайтов)
    .accesskey = л
sitedata-keep-until = Хранить их
    .accesskey = ь
sitedata-keep-until-expire =
    .label = до истечения срока их действия
sitedata-keep-until-closed =
    .label = до закрытия { -brand-short-name }
sitedata-accept-third-party-desc = Принимать куки и данные сайтов со сторонних веб-сайтов
    .accesskey = н
sitedata-accept-third-party-always-option =
    .label = Всегда
sitedata-accept-third-party-visited-option =
    .label = С посещённых
sitedata-accept-third-party-never-option =
    .label = Никогда
sitedata-clear =
    .label = Удалить данные…
    .accesskey = а
sitedata-settings =
    .label = Управление данными…
    .accesskey = ы
sitedata-cookies-exceptions =
    .label = Исключения…
    .accesskey = ю

## Privacy Section - Address Bar

addressbar-header = Панель адреса
addressbar-suggest = При использовании панели адреса предлагать ссылки
addressbar-locbar-history-option =
    .label = из журнала посещений
    .accesskey = ж
addressbar-locbar-bookmarks-option =
    .label = из закладок
    .accesskey = д
addressbar-locbar-openpage-option =
    .label = из открытых вкладок
    .accesskey = к
addressbar-suggestions-settings = Изменить настройки для предложений поисковых систем

## Privacy Section - Tracking

tracking-header = Защита от отслеживания
tracking-desc = Защита от отслеживания блокирует онлайн-трекеры, которые собирают данные вашего веб-сёрфинга на различных веб-сайтах. <a data-l10n-name="learn-more">Узнайте больше о защите от отслеживания и вашей приватности</a>
tracking-mode-label = Использовать защиту от отслеживания для блокировки известных трекеров
tracking-mode-always =
    .label = Всегда
    .accesskey = е
tracking-mode-private =
    .label = Только в приватных окнах
    .accesskey = в
tracking-mode-never =
    .label = Никогда
    .accesskey = а
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = Использовать защиту от отслеживания в приватном просмотре для блокировки известных трекеров
    .accesskey = о
tracking-exceptions =
    .label = Исключения…
    .accesskey = л
tracking-change-block-list =
    .label = Сменить список блокировки…
    .accesskey = м

## Privacy Section - Permissions

permissions-header = Разрешения
permissions-location = Местоположение
permissions-location-settings =
    .label = Параметры…
    .accesskey = м
permissions-camera = Камера
permissions-camera-settings =
    .label = Параметры…
    .accesskey = а
permissions-microphone = Микрофон
permissions-microphone-settings =
    .label = Параметры…
    .accesskey = ы
permissions-notification = Уведомления
permissions-notification-settings =
    .label = Параметры…
    .accesskey = е
permissions-notification-link = Подробнее
permissions-notification-pause =
    .label = Отключить уведомления до перезапуска { -brand-short-name }
    .accesskey = ю
permissions-block-autoplay-media =
    .label = Не разрешать веб-сайтам автоматически воспроизводить медиа со звуком
    .accesskey = ш
permissions-block-autoplay-media-exceptions =
    .label = Исключения…
    .accesskey = ю
permissions-block-popups =
    .label = Блокировать всплывающие окна
    .accesskey = о
permissions-block-popups-exceptions =
    .label = Исключения…
    .accesskey = ю
permissions-addon-install-warning =
    .label = Предупреждать при попытке веб-сайтов установить дополнения
    .accesskey = е
permissions-addon-exceptions =
    .label = Исключения…
    .accesskey = с
permissions-a11y-privacy-checkbox =
    .label = Запретить службам поддержки доступности доступ к вашему браузеру
    .accesskey = е
permissions-a11y-privacy-link = Подробнее

## Privacy Section - Data Collection

collection-header = Сбор и использование данных { -brand-short-name }
collection-description = Мы стремимся предоставить вам выбор и собирать только то, что нам нужно, для выпуска и улучшения { -brand-short-name } для всех и каждого. Мы всегда спрашиваем разрешения перед получением личной информации.
collection-privacy-notice = Уведомление о приватности
collection-health-report =
    .label = Разрешить { -brand-short-name } отправлять технические данные и данные взаимодействия в { -vendor-short-name }
    .accesskey = е
collection-health-report-link = Подробнее
collection-studies =
    .label = Разрешить { -brand-short-name } устанавливать и проводить исследования
collection-studies-link = Просмотреть исследования { -brand-short-name }
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = Для этой конфигурации сборки отправка данных отключена
collection-browser-errors =
    .label = Разрешить { -brand-short-name } отправлять сообщения об ошибках браузера (в том числе текст ошибок) в { -vendor-short-name }
    .accesskey = з
collection-browser-errors-link = Подробнее
collection-backlogged-crash-reports =
    .label = Разрешить { -brand-short-name } отправлять от вашего имени накопившиеся сообщения о падении
    .accesskey = ш
collection-backlogged-crash-reports-link = Подробнее

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = Защита
security-browsing-protection = Поддельное содержимое и защита от вредоносных программ
security-enable-safe-browsing =
    .label = Блокировать опасное и обманывающее содержимое
    .accesskey = л
security-enable-safe-browsing-link = Подробнее
security-block-downloads =
    .label = Блокировать опасные загрузки
    .accesskey = к
security-block-uncommon-software =
    .label = Предупреждать о нежелательных и нетипичных программах
    .accesskey = ж

## Privacy Section - Certificates

certs-header = Сертификаты
certs-personal-label = Когда сервер запрашивает личный сертификат
certs-select-auto-option =
    .label = Отправлять автоматически
    .accesskey = а
certs-select-ask-option =
    .label = Спрашивать каждый раз
    .accesskey = ш
certs-enable-ocsp =
    .label = Запрашивать у OCSP-серверов подтверждение текущего статуса сертификатов
    .accesskey = п
certs-view =
    .label = Просмотр сертификатов…
    .accesskey = м
certs-devices =
    .label = Устройства защиты…
    .accesskey = т
