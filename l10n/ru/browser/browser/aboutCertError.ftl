# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-intro = { $hostname } использует недействительный сертификат безопасности.
cert-error-mitm-intro = Веб-сайты подтверждают свою подлинность с помощью сертификатов, выдаваемых центрами сертификации.
cert-error-mitm-mozilla = { -brand-short-name } поддерживается некоммерческой организацией Mozilla, которая имеет собственное полностью открытое хранилище сертификатов центров сертификации. Это хранилище помогает убедиться, что центры сертификации следуют лучшим практикам обеспечения безопасности пользователей.
cert-error-mitm-connection = Для проверки защиты соединения { -brand-short-name } использует хранилище сертификатов центров сертификации Mozilla, а не хранилище, встроенное в операционную систему пользователя. Так что, если антивирусная или сетевая программа перехватывает соединение, используя сертификат безопасности, выданный центром сертификации, отсутствующем в хранилище Mozilla, соединение считается небезопасным.
cert-error-trust-unknown-issuer-intro = Кто-то может пытаться подменить настоящий сайт и вам лучше не продолжать.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-trust-unknown-issuer = Веб-сайты подтверждают свою подлинность с помощью сертификатов. { -brand-short-name } не доверяет { $hostname }, потому что издатель его сертификата неизвестен, сертификат является самоподписанным, или сервер не отправляет корректные промежуточные сертификаты.
cert-error-trust-cert-invalid = К сертификату нет доверия, так как он был издан с использованием недействительного сертификата центра сертификации (CA).
cert-error-trust-untrusted-issuer = К сертификату нет доверия, так как к сертификату его издателя нет доверия.
cert-error-trust-signature-algorithm-disabled = К сертификату нет доверия, так как он был подписан с использованием алгоритма подписи, который был отключён, так как алгоритм небезопасен.
cert-error-trust-expired-issuer = К сертификату нет доверия, так как у сертификата его издателя истёк срок действия.
cert-error-trust-self-signed = К сертификату нет доверия, так как он является самоподписанным.
cert-error-trust-symantec = Сертификаты, выпущенные GeoTrust, RapidSSL, Symantec, Thawte и VeriSign, более не считаются безопасными, так как эти центры сертификации в прошлом не соблюдали правила обеспечения безопасности.
cert-error-untrusted-default = К источнику, издавшему сертификат, нет доверия.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-domain-mismatch = Веб-сайты подтверждают свою подлинность с помощью сертификатов. { -brand-short-name } не доверяет этому сайту, потому что он использует сертификат, недействительный для { $hostname }.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $alt-name (String) - Alternate domain name for which the cert is valid.
cert-error-domain-mismatch-single = Веб-сайты подтверждают свою подлинность с помощью сертификатов. { -brand-short-name } не доверяет этому сайту, потому что он использует сертификат, недействительный для { $hostname }. Сертификат действителен только для <a data-l10n-name="domain-mismatch-link">{ $alt-name }</a>.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $alt-name (String) - Alternate domain name for which the cert is valid.
cert-error-domain-mismatch-single-nolink = Веб-сайты подтверждают свою подлинность с помощью сертификатов. { -brand-short-name } не доверяет этому сайту, потому что он использует сертификат, недействительный для { $hostname }. Сертификат действителен только для { $alt-name }.
# Variables:
# $subject-alt-names (String) - Alternate domain names for which the cert is valid.
cert-error-domain-mismatch-multiple = Веб-сайты подтверждают свою подлинность с помощью сертификатов. { -brand-short-name } не доверяет этому сайту, потому что он использует сертификат, недействительный для { $hostname }. Сертификат действителен только для следующих доменов: { $subject-alt-names }
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $not-after-local-time (Date) - Certificate is not valid after this time.
cert-error-expired-now = Веб-сайты подтверждают свою подлинность с помощью сертификатов, имеющих ограниченный срок действия. Срок действия сертификата для { $hostname } истёк { $not-after-local-time }.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $not-before-local-time (Date) - Certificate is not valid before this time.
cert-error-not-yet-valid-now = Веб-сайты подтверждают свою подлинность с помощью сертификатов, имеющих ограниченный срок действия. Сертификат для { $hostname } начнёт действовать не ранее { $not-before-local-time }.
# Variables:
# $error (String) - NSS error code string that specifies type of cert error. e.g. unknown issuer, invalid cert, etc.
cert-error-code-prefix-link = Код ошибки: <a data-l10n-name="error-code-link">{ $error }</a>
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-symantec-distrust-description = Веб-сайты подтверждают свою подлинность с помощью сертификатов, выдаваемых центрами сертификации. Большинство браузеров больше не доверяют сертификатам, выпущенным GeoTrust, RapidSSL, Symantec, Thawte и VeriSign. { $hostname } использует сертификат от одного из этих центров, поэтому его подлинность не может быть подтверждена.
cert-error-symantec-distrust-admin = Вы можете уведомить об этой проблеме администратора веб-сайта.
# Variables:
# $hasHSTS (Boolean) - Indicates whether HSTS header is present.
cert-error-details-hsts-label = Форсированное защищённое соединение HTTP (HSTS): { $hasHSTS }
# Variables:
# $hasHPKP (Boolean) - Indicates whether HPKP header is present.
cert-error-details-key-pinning-label = Привязка открытого ключа HTTP (HPKP): { $hasHPKP }
cert-error-details-cert-chain-label = Цепочка сертификата:
