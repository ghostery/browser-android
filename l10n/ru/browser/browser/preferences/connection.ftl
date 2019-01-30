# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

connection-window =
    .title = Параметры соединения
    .style =
        { PLATFORM() ->
            [macos] width: 44em
           *[other] width: 49em
        }
connection-close-key =
    .key = w
connection-disable-extension =
    .label = Отключить расширение
connection-proxy-configure = Настройка прокси для доступа в Интернет
connection-proxy-option-no =
    .label = Без прокси
    .accesskey = е
connection-proxy-option-system =
    .label = Использовать системные настройки прокси
    .accesskey = л
connection-proxy-option-auto =
    .label = Автоматически определять настройки прокси для этой сети
    .accesskey = в
connection-proxy-option-manual =
    .label = Ручная настройка прокси
    .accesskey = ч
connection-proxy-http = HTTP прокси
    .accesskey = с
connection-proxy-http-port = Порт
    .accesskey = о
connection-proxy-http-share =
    .label = Использовать этот прокси-сервер для всех протоколов
    .accesskey = е
connection-proxy-ssl = SSL прокси
    .accesskey = к
connection-proxy-ssl-port = Порт
    .accesskey = р
connection-proxy-ftp = FTP прокси
    .accesskey = и
connection-proxy-ftp-port = Порт
    .accesskey = п
connection-proxy-socks = Узел SOCKS
    .accesskey = з
connection-proxy-socks-port = Порт
    .accesskey = т
connection-proxy-socks4 =
    .label = SOCKS 4
    .accesskey = 4
connection-proxy-socks5 =
    .label = SOCKS 5
    .accesskey = 5
connection-proxy-noproxy = Не использовать прокси для
    .accesskey = д
connection-proxy-noproxy-desc = Пример: .mozilla-russia.org, .net.nz, 192.168.1.0/24
connection-proxy-autotype =
    .label = URL автоматической настройки прокси
    .accesskey = а
connection-proxy-reload =
    .label = Обновить
    .accesskey = б
connection-proxy-autologin =
    .label = Не запрашивать аутентификацию (если был сохранён пароль)
    .accesskey = ш
    .tooltip = Эта настройка аутентифицирует вас на прокси, не выдавая запросов, если вы сохранили для них учётные данные. Если аутентификация не удастся, вам будет выдан запрос.
connection-proxy-socks-remote-dns =
    .label = Отправлять DNS-запросы через прокси при использовании SOCKS 5
    .accesskey = я
connection-dns-over-https =
    .label = Включить DNS через HTTPS
    .accesskey = л
connection-dns-over-https-url = URL
    .accesskey = U
    .tooltiptext = URL для разрешения DNS через HTTPS
# Variables:
#   $url (String) - URL for the DNS over HTTPS provider
connection-dns-over-https-url-default =
    .label = Использовать URL по умолчанию ({ $url })
    .accesskey = п
    .tooltiptext = Использовать URL по умолчанию для разрешения DNS через HTTPS
connection-dns-over-https-url-custom =
    .label = Другой URL
    .accesskey = о
    .tooltiptext = Введите свой URL для разрешения DNS через HTTPS
