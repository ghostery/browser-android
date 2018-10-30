# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

connection-window =
    .title = Ustawienia połączenia
    .style =
        { PLATFORM() ->
            [macos] width: 45em
           *[other] width: 49em
        }
connection-close-key =
    .key = w
connection-disable-extension =
    .label = Wyłącz rozszerzenie
connection-proxy-configure = Konfiguracja proxy do łączenia z Internetem
connection-proxy-option-no =
    .label = Bez serwera proxy
    .accesskey = B
connection-proxy-option-system =
    .label = Używaj systemowych ustawień serwerów proxy
    .accesskey = w
connection-proxy-option-auto =
    .label = Automatycznie wykrywaj ustawienia serwerów proxy dla tej sieci
    .accesskey = A
connection-proxy-option-manual =
    .label = Ręczna konfiguracja serwerów proxy:
    .accesskey = k
connection-proxy-http = Serwer proxy HTTP:
    .accesskey = H
connection-proxy-http-port = Port:
    .accesskey = o
connection-proxy-http-share =
    .label = Użyj tego serwera proxy dla wszystkich protokołów
    .accesskey = U
connection-proxy-ssl = Serwer proxy SSL:
    .accesskey = S
connection-proxy-ssl-port = Port:
    .accesskey = r
connection-proxy-ftp = Serwer proxy FTP:
    .accesskey = F
connection-proxy-ftp-port = Port:
    .accesskey = t
connection-proxy-socks = Host SOCKS:
    .accesskey = C
connection-proxy-socks-port = Port:
    .accesskey = P
connection-proxy-socks4 =
    .label = SOCKS v4
    .accesskey = 4
connection-proxy-socks5 =
    .label = SOCKS v5
    .accesskey = 5
connection-proxy-noproxy = Nie używaj proxy dla:
    .accesskey = N
connection-proxy-noproxy-desc = Przykład: .mozilla.org, .com.pl, 192.168.1.0/24
connection-proxy-autotype =
    .label = Adres URL automatycznej konfiguracji proxy:
    .accesskey = e
connection-proxy-reload =
    .label = Odśwież
    .accesskey = d
connection-proxy-autologin =
    .label = Nie pytaj o uwierzytelnianie, jeśli istnieje zachowane hasło
    .accesskey = j
    .tooltip = Umożliwia automatyczne uwierzytelnianie na serwerach proxy, jeśli wcześniej zostały zachowane dane logowania. W przypadku nieudanego uwierzytelniania zostanie wyświetlone standardowe pytanie.
connection-proxy-socks-remote-dns =
    .label = Proxy DNS podczas używania SOCKS v5
    .accesskey = x

connection-dns-over-https=
  .label=DNS poprzez HTTPS
  .accesskey=D
connection-dns-over-https-url=Adres URL
  .accesskey=U
  .tooltiptext=Adres URL serwera DNS udostępnionego poprzez HTTPS
