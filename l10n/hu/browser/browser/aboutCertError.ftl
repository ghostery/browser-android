# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-intro = A(z) { $hostname } érvénytelen biztonsági tanúsítványt használ.
cert-error-mitm-intro = A weboldalak tanúsítványokkal igazolják a személyazonosságukat, amelyet tanúsítványkibocsátók állítanak ki.
cert-error-mitm-mozilla = A { -brand-short-name } mögött a nonprofit Mozilla áll, amely egy teljesen nyílt tanúsítványtárolót kezel. A CA tároló biztosítja, hogy a tanúsítványkibocsátók kövessék a felhasználói biztonságra vonatkozó legjobb gyakorlatokat.
cert-error-mitm-connection = A { -brand-short-name } a Mozilla CA tároló használatával ellenőrzi, hogy a kapcsolat biztonságos-e, és nem a felhasználó operációs rendszere által biztosított tanúsítványokkal. Tehát ha egy víruskereső program vagy egy hálózat elfogja a CA által kibocsátott biztonsági tanúsítványt, és az nincs a Mozilla CA tárolóban, akkor a kapcsolat nem biztonságosként lesz kezelve.
cert-error-trust-unknown-issuer-intro = Lehet hogy valaki megszemélyesíti az oldalt, ne folytassa.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-trust-unknown-issuer = A weboldalak tanúsítványokkal bizonyítják a személyazonosságukat. A { -brand-short-name } nem bízik a(z) { $hostname } oldalban, mert a tanúsítványkibocsátója ismeretlen, a tanúsítvány önaláírt, vagy a kiszolgáló nem küld megfelelő közbenső tanúsítványokat.
cert-error-trust-cert-invalid = A tanúsítvány nem megbízható, mert érvénytelen CA-tanúsítvánnyal bocsátották ki.
cert-error-trust-untrusted-issuer = A tanúsítvány nem megbízható, mert a kibocsátó tanúsítványa nem megbízható.
cert-error-trust-signature-algorithm-disabled = A tanúsítvány nem megbízható, mert a megbízhatatlansága miatt letiltott aláírási algoritmussal írták alá.
cert-error-trust-expired-issuer = A tanúsítvány nem megbízható, mert a kibocsátó tanúsítványa lejárt
cert-error-trust-self-signed = A tanúsítvány nem megbízható, mert a saját kibocsátója által van aláírva.
cert-error-trust-symantec = A GeoTrust, a RapidSSL, a Symantec, a Thawte és a VeriSign által kiadott tanúsítványok már nem minősülnek biztonságosnak, mert ezek a tanúsító hatóságok a múltban nem tartották be a biztonsági gyakorlatokat.
cert-error-untrusted-default = A tanúsítvány nem megbízható forrásból érkezik.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-domain-mismatch = A weboldalak tanúsítványokkal bizonyítják a személyazonosságukat. A { -brand-short-name } nem bízik az oldalban, mert olyan tanúsítványt használ, amely nem érvényes a(z) { $hostname } tartományra.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $alt-name (String) - Alternate domain name for which the cert is valid.
cert-error-domain-mismatch-single = A weboldalak tanúsítványokkal bizonyítják a személyazonosságukat. A { -brand-short-name } nem bízik az oldalban, mert olyan tanúsítványt használ, amely nem érvényes a(z) { $hostname } tartományra. A tanúsítvány csak a következőre érvényes: <a data-l10n-name="domain-mismatch-link">{ $alt-name }</a>.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $alt-name (String) - Alternate domain name for which the cert is valid.
cert-error-domain-mismatch-single-nolink = A weboldalak tanúsítványokkal bizonyítják a személyazonosságukat. A { -brand-short-name } nem bízik az oldalban, mert olyan tanúsítványt használ, amely nem érvényes a(z) { $hostname } tartományra. A tanúsítvány csak a következőre érvényes: { $alt-name }.
# Variables:
# $subject-alt-names (String) - Alternate domain names for which the cert is valid.
cert-error-domain-mismatch-multiple = A weboldalak tanúsítványokkal bizonyítják a személyazonosságukat. A { -brand-short-name } nem bízik az oldalban, mert olyan tanúsítványt használ, amely nem érvényes a(z) { $hostname } tartományra. A tanúsítvány csak a következő nevekre érvényes: { $subject-alt-names }
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $not-after-local-time (Date) - Certificate is not valid after this time.
cert-error-expired-now = A weboldalak tanúsítványokkal bizonyítják a személyazonosságukat, melyek egy adott időközben érvényesek. A(z) { $hostname } tanúsítványa ekkor lejárt: { $not-after-local-time }.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $not-before-local-time (Date) - Certificate is not valid before this time.
cert-error-not-yet-valid-now = A weboldalak tanúsítványokkal bizonyítják a személyazonosságukat, melyek egy adott időközben érvényesek. A(z) { $hostname } tanúsítványa ez után lesz érvényes: { $not-before-local-time }.
# Variables:
# $error (String) - NSS error code string that specifies type of cert error. e.g. unknown issuer, invalid cert, etc.
cert-error-code-prefix-link = Hibakód: <a data-l10n-name="error-code-link">{ $error }</a>
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-symantec-distrust-description = A weboldalak tanúsítványokkal bizonyítják a személyazonosságukat, ezeket pedig tanúsítványkibocsátók állítják ki. A legtöbb böngésző már nem bízik meg a GeoTrust, a RapidSSL, a Symantec, a Thawte és a VeriSign által kiadott tanúsítványokban. A(z) { $hostname } egy ilyen tanúsítványt használ, ezért a webhely személyazonosságát nem lehet bizonyítani.
cert-error-symantec-distrust-admin = Értesítheti a webhely rendszergazdáját a problémáról.
# Variables:
# $hasHSTS (Boolean) - Indicates whether HSTS header is present.
cert-error-details-hsts-label = HTTP Strict Transport Security: { $hasHSTS }
# Variables:
# $hasHPKP (Boolean) - Indicates whether HPKP header is present.
cert-error-details-key-pinning-label = HTTP Public Key Pinning: { $hasHPKP }
cert-error-details-cert-chain-label = Tanúsítványlánc:
