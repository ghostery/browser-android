# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-intro = Witryna „{ $hostname }” używa nieprawidłowego certyfikatu bezpieczeństwa.
cert-error-mitm-intro = Strony internetowe dowodzą swojej tożsamości przy użyciu certyfikatów, wystawianych przez organizacje certyfikujące.
cert-error-mitm-mozilla = { -brand-short-name } to oprogramowanie tworzone przez organizację Mozilla, która zarządza całkowicie otwartym magazynem organów certyfikacji (CA). Magazyn ten pomaga dopilnować przestrzegania przez organy certyfikacji najlepszych praktyk dla bezpieczeństwa użytkowników.
cert-error-mitm-connection = Zamiast certyfikatów systemowych { -brand-short-name } używa magazynu CA organizacji Mozilla, aby weryfikować bezpieczeństwo połączeń. Połączenie nie jest uznawane za bezpieczne, jeśli oprogramowanie antywirusowe lub sieciowe przechwytuje połączenie z certyfikatem bezpieczeństwa wystawionym przez organizację certyfikującą nieobecną w magazynie CA organizacji Mozilla.
cert-error-trust-unknown-issuer-intro = Ktoś może próbować podszywać się pod tę witrynę. Odradzamy kontynuowanie.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-trust-unknown-issuer = Witryny potwierdzają swoją tożsamość poprzez certyfikaty. { -brand-short-name } nie ufa certyfikatowi witryny „{ $hostname }”, ponieważ jego wystawca jest nieznany, jest samopodpisany lub serwer nie przesyła właściwych certyfikatów pośrednich.
cert-error-trust-cert-invalid = Certyfikat nie jest zaufany, ponieważ został wystawiony przy użyciu nieprawidłowego certyfikatu CA.
cert-error-trust-untrusted-issuer = Certyfikat nie jest zaufany, ponieważ certyfikat wystawcy nie jest zaufany.
cert-error-trust-signature-algorithm-disabled = Certyfikat nie jest zaufany, ponieważ został podpisany algorytmem, który został zablokowany, ponieważ nie jest bezpieczny.
cert-error-trust-expired-issuer = Certyfikat nie jest zaufany, ponieważ certyfikat wystawcy utracił ważność.
cert-error-trust-self-signed = Certyfikat nie jest zaufany, ponieważ jest samopodpisany.
cert-error-trust-symantec = Certyfikaty wystawiane przez GeoTrust, RapidSSL, Symantec, Thawte i Verisign nie są już uznawane za bezpieczne, ponieważ wystawiające je organizacje nie przestrzegały zasad bezpieczeństwa.
cert-error-untrusted-default = Certyfikat nie pochodzi z zaufanego źródła.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-domain-mismatch = Witryny potwierdzają swoją tożsamość poprzez certyfikaty. { -brand-short-name } nie ufa certyfikatowi witryny „{ $hostname }”, ponieważ nie jest on dla niej prawidłowy.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $alt-name (String) - Alternate domain name for which the cert is valid.
cert-error-domain-mismatch-single = Witryny potwierdzają swoją tożsamość poprzez certyfikaty. { -brand-short-name } nie ufa certyfikatowi witryny „{ $hostname }”, ponieważ nie jest on dla niej prawidłowy. Ten certyfikat jest prawidłowy tylko dla domeny <a data-l10n-name="domain-mismatch-link">{ $alt-name }</a>.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $alt-name (String) - Alternate domain name for which the cert is valid.
cert-error-domain-mismatch-single-nolink = Witryny potwierdzają swoją tożsamość poprzez certyfikaty. { -brand-short-name } nie ufa certyfikatowi witryny „{ $hostname }”, ponieważ nie jest on dla niej prawidłowy. Ten certyfikat jest prawidłowy tylko dla domeny { $alt-name }.
# Variables:
# $subject-alt-names (String) - Alternate domain names for which the cert is valid.
cert-error-domain-mismatch-multiple = Witryny potwierdzają swoją tożsamość poprzez certyfikaty. { -brand-short-name } nie ufa certyfikatowi witryny „{ $hostname }”, ponieważ nie jest on dla niej prawidłowy. Certyfikat został wystawiony tylko dla następujących domen: { $subject-alt-names }
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $not-after-local-time (Date) - Certificate is not valid after this time.
cert-error-expired-now = Witryny potwierdzają swoją tożsamość poprzez certyfikaty, które są ważne w określonym czasie. Certyfikat witryny „{ $hostname }” utracił ważność { $not-after-local-time }.
# Variables:
# $hostname (String) - Hostname of the website with cert error.
# $not-before-local-time (Date) - Certificate is not valid before this time.
cert-error-not-yet-valid-now = Witryny potwierdzają swoją tożsamość poprzez certyfikaty, które są ważne w określonym czasie. Certyfikat witryny „{ $hostname }” nie będzie ważny do { $not-before-local-time }.
# Variables:
# $error (String) - NSS error code string that specifies type of cert error. e.g. unknown issuer, invalid cert, etc.
cert-error-code-prefix-link = Kod błędu: <a data-l10n-name="error-code-link">{ $error }</a>
# Variables:
# $hostname (String) - Hostname of the website with cert error.
cert-error-symantec-distrust-description = Strony internetowe dowodzą swojej tożsamości przy użyciu certyfikatów, wystawianych przez organizacje certyfikujące. Większość przeglądarek nie ufa już certyfikatom wystawianym przez GeoTrust, RapidSSL, Symantec, Thawte i Verisign. Tożsamość tej strony nie może zostać potwierdzona, ponieważ domena „{ $hostname }” używa certyfikatu od jednego z tych wystawców.
cert-error-symantec-distrust-admin = Można powiadomić administratora strony o tym problemie.
# Variables:
# $hasHSTS (Boolean) - Indicates whether HSTS header is present.
cert-error-details-hsts-label = HTTP Strict Transport Security: { $hasHSTS }
# Variables:
# $hasHPKP (Boolean) - Indicates whether HPKP header is present.
cert-error-details-key-pinning-label = HTTP Public Key Pinning: { $hasHPKP }
cert-error-details-cert-chain-label = Łańcuch certyfikatu:
