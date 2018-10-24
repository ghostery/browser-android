# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

connection-window =
    .title = 연결 설정
    .style =
        { PLATFORM() ->
            [macos] width: 44em
           *[other] width: 49em
        }
connection-close-key =
    .key = w
connection-disable-extension =
    .label = 확장기능 비활성화
connection-proxy-configure = 인터넷 프록시 접근 설정
connection-proxy-option-no =
    .label = 프록시 사용 안 함
    .accesskey = y
connection-proxy-option-system =
    .label = 시스템 프록시 설정 사용
    .accesskey = u
connection-proxy-option-auto =
    .label = 프록시 설정 자동 감지
    .accesskey = w
connection-proxy-option-manual =
    .label = 수동 프록시 설정
    .accesskey = m
connection-proxy-http = HTTP 프록시
    .accesskey = x
connection-proxy-http-port = 포트
    .accesskey = P
connection-proxy-http-share =
    .label = 모든 프로토콜에 위의 프록시 설정 사용
    .accesskey = s
connection-proxy-ssl = SSL 프록시
    .accesskey = L
connection-proxy-ssl-port = 포트
    .accesskey = o
connection-proxy-ftp = FTP 프록시
    .accesskey = F
connection-proxy-ftp-port = 포트
    .accesskey = r
connection-proxy-socks = SOCKS 호스트
    .accesskey = C
connection-proxy-socks-port = 포트
    .accesskey = t
connection-proxy-socks4 =
    .label = SOCKS v4
    .accesskey = K
connection-proxy-socks5 =
    .label = SOCKS v5
    .accesskey = v
connection-proxy-noproxy = 프록시를 지나지 않음
    .accesskey = n
connection-proxy-noproxy-desc = 예: .mozilla.org, .net.nz, 192.168.1.0/24
connection-proxy-autotype =
    .label = 자동 프록시 설정 URL
    .accesskey = A
connection-proxy-reload =
    .label = 새로 고침
    .accesskey = e
connection-proxy-autologin =
    .label = 비밀번호가 저장되어 있으면 인증시 묻지 않기
    .accesskey = i
    .tooltip = 저장한 자격 증명이 있으면 조용히 인증합니다. 인증이 되지 않으면 물어볼 것입니다.
connection-proxy-socks-remote-dns =
    .label = SOCKS v5를 사용할 때 프록시 DNS
    .accesskey = d
connection-dns-over-https =
    .label = HTTPS를 통한 DNS를 사용 하도록 설정
    .accesskey = H
connection-dns-over-https-url = URL
    .accesskey = R
    .tooltiptext = HTTPS를 통한 DNS를 처리하는 URL
