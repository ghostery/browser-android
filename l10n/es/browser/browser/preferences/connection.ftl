# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

connection-window =
    .title = Configuración de conexión
    .style =
        { PLATFORM() ->
            [macos] width: 49em
           *[other] width: 54em
        }
connection-close-key =
    .key = w
connection-disable-extension =
    .label = Desactivar extensión
connection-proxy-configure = Configurar acceso proxy a Internet
connection-proxy-option-no =
    .label = Sin proxy
    .accesskey = y
connection-proxy-option-system =
    .label = Usar la configuración del proxy del sistema
    .accesskey = d
connection-proxy-option-auto =
    .label = Autodetectar configuración del proxy para esta red
    .accesskey = e
connection-proxy-option-manual =
    .label = Configuración manual del proxy
    .accesskey = C
connection-proxy-http = Proxy HTTP
    .accesskey = H
connection-proxy-http-port = Puerto
    .accesskey = P
connection-proxy-http-share =
    .label = Usar el mismo proxy para todo
    .accesskey = m
connection-proxy-ssl = Proxy SSL
    .accesskey = x
connection-proxy-ssl-port = Puerto
    .accesskey = o
connection-proxy-ftp = Proxy FTP
    .accesskey = F
connection-proxy-ftp-port = Puerto
    .accesskey = r
connection-proxy-socks = Host SOCKS
    .accesskey = S
connection-proxy-socks-port = Puerto
    .accesskey = t
connection-proxy-socks4 =
    .label = SOCKS v4
    .accesskey = K
connection-proxy-socks5 =
    .label = SOCKS v5
    .accesskey = v
connection-proxy-noproxy = No usar proxy para
    .accesskey = N
connection-proxy-noproxy-desc = Ejemplo: .mozilla.org, .net.nz, 192.168.1.0/24
connection-proxy-autotype =
    .label = URL de configuración automática del proxy
    .accesskey = L
connection-proxy-reload =
    .label = Recargar
    .accesskey = R
connection-proxy-autologin =
    .label = No preguntar identificación si la contraseña está guardada
    .accesskey = u
    .tooltip = Esta opción le identifica de manera silenciosa ante los proxys cuando ha guardado las credenciales para ellos. Se le preguntará si falla la identificación.
connection-proxy-socks-remote-dns =
    .label = DNS proxy usando SOCKS v5
    .accesskey = d
connection-dns-over-https =
    .label = Activar DNS sobre HTTPS
    .accesskey = A
connection-dns-over-https-url = URL
    .accesskey = U
    .tooltiptext = URL para resolver DNS sobre HTTPS
# Variables:
#   $url (String) - URL for the DNS over HTTPS provider
connection-dns-over-https-url-default =
    .label = Usar predeterminada ({ $url })
    .accesskey = U
    .tooltiptext = Usar la URL predeterminada para resolver DNS sobre HTTPS
connection-dns-over-https-url-custom =
    .label = Personalizada
    .accesskey = P
    .tooltiptext = Escriba su URL preferida para resolver DNS sobre HTTPS
