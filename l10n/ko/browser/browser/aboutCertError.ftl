# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-intro = { $hostname }는 유효하지 않은 보안 인증서를 사용합니다.
cert-error-mitm-intro = 웹사이트는 인증 기관에 의해 발행된 인증서를 통해 신원을 증명합니다.
cert-error-mitm-mozilla = { -brand-short-name }는 완전히 개방 된 CA(Certificate Authority) 저장소를 관리하는 비영리 재단 Mozilla의 지원을받습니다. CA 저장소는 인증 기관이 사용자 보안을 위한 모범 사례를 따르도록 합니다.
cert-error-mitm-connection = { -brand-short-name }는 사용자의 운영 체제가 제공하는 인증서가 아닌 Mozilla CA 저장소를 사용하여 연결이 안전한지 확인합니다. 따라서 바이러스 백신 프로그램이나 네트워크가 Mozilla CA 저장소에 없는 CA에서 발급 한 보안 인증서로 연결을 가로채는 경우 연결이 안전하지 않은 것으로 간주됩니다.
cert-error-trust-unknown-issuer-intro = 누군가 사이트를 위장할 수 있기 때문에 더이상 진행하면 안됩니다.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-trust-unknown-issuer = 웹사이트는 인증서로 신원을 증명합니다. { -brand-short-name }는 인증서 발행자를 알 수 없거나, 인증서가 자기 서명되어 있거나 서버가 올바른 중간 인증서를 보내지 않고 있기 때문에 { $hostname } 사이트를 신뢰할 수 없습니다.
cert-error-trust-cert-invalid = 유효하지 않은 인증 기관 (CA) 인증서로 발급된 인증서이므로 신뢰할 수 없습니다.
cert-error-trust-untrusted-issuer = 발급자 인증서를 신뢰할 수 없기 때문에 인증서를 신뢰할 수 없습니다.
cert-error-trust-signature-algorithm-disabled = 안전하지 않은 인증 알고리즘을 사용하여 서명된 인증서를 신뢰할 수 없습니다.
cert-error-trust-expired-issuer = 발급자 인증서가 만료되었기 때문에 인증서를 신뢰할 수 없습니다.
cert-error-trust-self-signed = 자기 스스로 서명하였으므로 인증서를 신뢰할 수 없습니다.
cert-error-trust-symantec = GeoTrust나 RapidSSL, Symantec, Thawte, VeriSign이 발행 한 인증서는 이전에 보안 관행을 따르지 않았기 때문에 더 이상 안전한 것으로 간주되지 않습니다.
cert-error-untrusted-default = 신뢰할 수 있는 출처의 인증서가 아닙니다.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-domain-mismatch = 웹사이트는 인증서로 신원을 증명합니다. { -brand-short-name }는 이 사이트가 { $hostname }에 대해 유효하지 않은 인증서를 사용하고 있기 때문에 신뢰하지 않습니다.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $alt-name (String) - Alternate domain name for which the cert is valid.
cert-error-domain-mismatch-single = 웹사이트는 인증서로 신원을 증명합니다. { -brand-short-name }는 이 사이트가 { $hostname }에 대해 유효하지 않은 인증서를 사용하고 있기 때문에 신뢰하지 않습니다. 인증서가 <a data-l10n-name="domain-mismatch-link">{ $alt-name }</a>에 대해서만 유효합니다.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $alt-name (String) - Alternate domain name for which the cert is valid.
cert-error-domain-mismatch-single-nolink = 웹사이트는 인증서로 신원을 증명합니다. { -brand-short-name }는 이 사이트가 { $hostname }에 대해 유효하지 않은 인증서를 사용하고 있기 때문에 신뢰하지 않습니다. 인증서가 { $alt-name }에 대해서만 유효합니다.
# Variables:
# $subject-alt-names (String) - Alternate domain names for which the cert is valid.
cert-error-domain-mismatch-multiple = 웹사이트는 인증서로 신원을 증명합니다. { -brand-short-name }는 이 사이트가 { $hostname }에 대해 유효하지 않은 인증서를 사용하고 있기 때문에 신뢰하지 않습니다. 인증서는 다음의 이름에 대해서만 유효합니다: { $subject-alt-names }
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $not-after-local-time (Date) - Certificate is not valid after this time.
cert-error-expired-now = 웹사이트는 지정된 기간동안 유효한 인증서를 통해 신원을 증명합니다. { $hostname }의 인증서가 { $not-after-local-time }에 만료되었습니다.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $not-before-local-time (Date) - Certificate is not valid before this time.
cert-error-not-yet-valid-now = 웹사이트는 지정된 기간동안 유효한 인증서를 통해 신원을 증명합니다. { $hostname }의 인증서가 { $not-before-local-time }까지 유효하지 않습니다.
# Variables:
# $error (String) - NSS error code string that specifies type of cert error. e.g. unknown issuer, invalid cert, etc.
cert-error-code-prefix-link = 오류 코드: <a data-l10n-name="error-code-link">{ $error }</a>
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-symantec-distrust-description = 웹사이트는 인증 기관에서 발행한 인증서를 통해 신원을 증명합니다. 대부분의 브라우저는 더 이상 GeoTrust나 RapidSSL, Symantec, Thawte, VeriSign에서 발급 한 인증서를 신뢰하지 않습니다. { $hostname }은 이러한 인증 중 하나를 사용하므로 웹 사이트 자신을 증명할 할 수 없습니다.
cert-error-symantec-distrust-admin = 웹사이트의 관리자에게 이 문제에 대해서 보고할 수 있습니다.
# Variables:
# $hasHSTS (Boolean) - Indicates whether HSTS header is present.
cert-error-details-hsts-label = HTTP 보안 강화 프로토콜: { $hasHSTS }
# Variables:
# $hasHPKP (Boolean) - Indicates whether HPKP header is present.
cert-error-details-key-pinning-label = HTTP 공개 키 고정: { $hasHPKP }
cert-error-details-cert-chain-label = 인증 체인:
