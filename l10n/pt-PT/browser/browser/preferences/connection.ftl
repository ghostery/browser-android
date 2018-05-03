# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

connection-window =
    .title = Definições da ligação
    .style =
        { PLATFORM() ->
            [macos] width: 44em
           *[other] width: 49em
        }
connection-close-key =
    .key = w
connection-disable-extension =
    .label = Desativar extensão
connection-proxy-configure = Configurar acesso proxy à Internet
connection-proxy-option-no =
    .label = Sem proxy
    .accesskey = p
connection-proxy-option-system =
    .label = Utilizar definições de proxy do sistema
    .accesskey = x
connection-proxy-option-auto =
    .label = Detetar automaticamente as definições de proxy para esta rede
    .accesskey = d
connection-proxy-option-manual =
    .label = Configuração manual de proxy
    .accesskey = m
connection-proxy-http = Proxy HTTP
    .accesskey = x
connection-proxy-http-port = Porta
    .accesskey = P
connection-proxy-http-share =
    .label = Utilizar o mesmo proxy para todos os protocolos
    .accesskey = s
connection-proxy-ssl = Proxy SSL
    .accesskey = L
connection-proxy-ssl-port = Porta
    .accesskey = o
connection-proxy-ftp = Proxy FTP
    .accesskey = F
connection-proxy-ftp-port = Porta
    .accesskey = r
connection-proxy-socks = Servidor SOCKS
    .accesskey = C
connection-proxy-socks-port = Porta
    .accesskey = a
connection-proxy-socks4 =
    .label = SOCKS v4
    .accesskey = 4
connection-proxy-socks5 =
    .label = SOCKS v5
    .accesskey = 5
connection-proxy-noproxy = Nenhum proxy para
    .accesskey = n
connection-proxy-noproxy-desc = Exemplo: .mozilla.org, .net.nz, 192.168.1.0/24
connection-proxy-autotype =
    .label = URL de configuração automática de proxy
    .accesskey = a
connection-proxy-reload =
    .label = Recarregar
    .accesskey = c
connection-proxy-autologin =
    .label = Não solicitar autenticação se a palavra-passe estiver guardada
    .accesskey = i
    .tooltip = Esta opção autentica-lhe silenciosamente nos proxies quando tem credenciais para os mesmos. Será solicitado(a) se a autenticação falhar.
connection-proxy-socks-remote-dns =
    .label = Encaminhar DNS quando utilizar SOCKS v5
    .accesskey = d
