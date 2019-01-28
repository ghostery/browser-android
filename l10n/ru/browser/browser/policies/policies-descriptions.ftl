# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Firefox installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-AppUpdateURL = Устанавливает собственный URL обновления приложения.
policy-Authentication = Настраивает интегрированную авторизацию для поддерживающих это веб-сайтов.
policy-BlockAboutAddons = Блокирует доступ к менеджеру дополнений (about:addons).
policy-BlockAboutConfig = Блокирует доступ к странице about:config.
policy-BlockAboutProfiles = Блокирует доступ к странице about:profiles.
policy-BlockAboutSupport = Блокирует доступ к странице about:support.
policy-Bookmarks = Создаёт закладки в панели закладок, меню закладок, или в отдельной папке внутри них.
policy-Certificates = Решает, использовать или нет встроенные сертификаты. Эта политика пока касается только Windows.
policy-CertificatesDescription = Добавляет сертификаты или использует встроенные сертификаты.
policy-Cookies = Разрешает или запрещает веб-сайтам устанавливать куки.
policy-DisableAppUpdate = Запрещает обновление браузера.
policy-DisableBuiltinPDFViewer = Отключает PDF.js, встроенный просмотрщик PDF в { -brand-short-name }.
policy-DisableDeveloperTools = Блокирует доступ к инструментам разработчика.
policy-DisableFeedbackCommands = Отключает команды отправки отзывов в меню Справка («Отправить отзыв...» и «Сообщить о поддельном сайте...»).
policy-DisableFirefoxAccounts = Отключает службы, основанные на { -fxaccount-brand-name[prepositional] }, включая Синхронизацию.
# Firefox Screenshots is the name of the feature, and should not be translated.
policy-DisableFirefoxScreenshots = Отключает функцию Firefox Screenshots.
policy-DisableFirefoxStudies = Запрещает { -brand-short-name } выполнять исследования.
policy-DisableForgetButton = Закрывает доступ к кнопке «Забыть».
policy-DisableFormHistory = Отключает запоминание истории поиска и данных форм.
policy-DisableMasterPasswordCreation = Не позволяет установить мастер-пароль, если установлена в true.
policy-DisablePocket = Отключает сохранение страниц в Pocket.
policy-DisablePrivateBrowsing = Отключает приватный просмотр.
policy-DisableProfileImport = Отключает команды меню импорта данных из другого браузера.
policy-DisableProfileRefresh = Отключает кнопку Обновить { -brand-short-name } на странице about:support.
policy-DisableSafeMode = Отключает функцию для перезапуска в безопасном режиме. Примечание: Клавишу Shift для входа в безопасный режим можно отключить только в Windows с помощью групповой политики.
policy-DisableSecurityBypass = Не даёт пользователю игнорировать определенные предупреждения системы безопасности.
policy-DisableSetDesktopBackground = Отключает команду меню «Сделать фоновым рисунком рабочего стола…».
policy-DisableSetAsDesktopBackground = Отключает команду меню «Сделать фоновым рисунком рабочего стола…» для изображений.
policy-DisableSystemAddonUpdate = Запрещает браузеру устанавливать и обновлять системные дополнения.
policy-DisableTelemetry = Отключает телеметрию.
policy-DisplayBookmarksToolbar = Отображает панель закладок по умолчанию.
policy-DisplayMenuBar = Отображает панель меню по умолчанию.
policy-DNSOverHTTPS = Настраивает DNS через HTTPS.
policy-DontCheckDefaultBrowser = Отключает проверку браузера по умолчанию при запуске.
# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = Включает или отключает Блокирование содержимого и, по желанию, блокирует изменение этой функции.
# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#extensions-machine-only
policy-Extensions = Устанавливает, удаляет или блокирует установку/удаление расширений. Параметр «Установка» использует URL-адреса или пути в качестве параметров. Параметры «Удаление» и «Блокировка» принимают идентификаторы расширений.
policy-FlashPlugin = Разрешает или запрещает использование плагина Flash.
policy-HardwareAcceleration = Отключает аппаратное ускорение, если установлена в false.
# “lock” means that the user won’t be able to change this setting
policy-Homepage = Устанавливает домашнюю страницу и, по желанию, блокирует её смену.
policy-InstallAddonsPermission = Разрешает некоторым веб-сайтам устанавливать дополнения.
policy-NoDefaultBookmarks = Отключает создание закладок по умолчанию, идущих вместе с { -brand-short-name }, и Умных Закладок (Часто посещаемые, Последние метки). Примечание: эта политика действует только в том случае, если она используется до первого запуска профиля.
policy-OfferToSaveLogins = Разрашает { -brand-short-name } предлагать запоминать сохранённые логины и пароли. Принимаются значения как true, так и false.
policy-OverrideFirstRunPage = Переопределяет первую страницу после запуска. Установите эту политику в пустую, если хотите отключить первую страницу после запуска.
policy-OverridePostUpdatePage = Переопределяет страницу «Что нового», открывающуюся после обновления. Установите эту политику в пустую, если хотите отключить страницу, открывающуюся после обновления.
policy-Permissions = Настраивает разрешения для камеры, микрофона, местоположения и уведомлений.
policy-PopupBlocking = Разрешает некоторым веб-сайтам открывать всплывающие окна по умолчанию.
policy-Proxy = Настраивает параметры прокси.
policy-RequestedLocales = Настраивает список запрашиваемых языков для приложения в порядке предпочтения.
policy-SanitizeOnShutdown = Удаляет все данные веб-сёрфинга при закрытии.
policy-SearchBar = Устанавливает расположение панели поиска по умолчанию. Пользователю всё же разрешено его настраивать.
policy-SearchEngines = Настраивает параметры поисковой системы. Эта политика доступна только в версии длительной поддержки (ESR).
# For more information, see https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/PKCS11/Module_Installation
policy-SecurityDevices = Устанавливает модули PKCS #11.
# “format” refers to the format used for the value of this policy. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#websitefilter-machine-only
policy-WebsiteFilter = Блокирует посещение веб-сайтов. Для получения дополнительной информации о формате обратитесь к документации.
