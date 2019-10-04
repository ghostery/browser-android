# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
#   $count (Number) - Number of tracking events blocked.
graph-week-summary =
    { $count ->
        [one] { -brand-short-name } a bloqué { $count } traqueur au cours de la semaine passée
       *[other] { -brand-short-name } a bloqué { $count } traqueurs au cours de la semaine passée
    }
# Variables:
#   $count (Number) - Number of tracking events blocked.
#   $earliestDate (Number) - Unix timestamp in ms, representing a date. The
# earliest date recorded in the database.
graph-total-summary =
    { $count ->
        [one] { $count } traqueur bloqué depuis le { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
       *[other] { $count } traqueurs bloqués depuis le { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
    }
# The terminology used to refer to categories of Content Blocking is also used in chrome/browser/browser.properties and should be translated consistently.
# "Standard" in this case is an adjective, meaning "default" or "normal".
# The category name in the <b> tag will be bold.
protection-header-details-standard = Le niveau de protection est réglé sur <b>Standard</b>.
protection-header-details-strict = Le niveau de protection est réglé sur <b>Strict</b>.
protection-header-details-custom = Le niveau de protection est réglé sur <b>Personnalisé</b>.
protection-report-page-title = Protections de la vie privée
protection-report-content-title = Protections de la vie privée
etp-card-title = Protection renforcée contre le pistage
etp-card-content = Les traqueurs vous suivent en ligne pour collecter des informations sur vos habitudes de navigation et vos centres d’intérêt. { -brand-short-name } bloque un grand nombre de ces traqueurs et de scripts malveillants.
# This string is used to label the X axis of a graph. Other days of the week are generated via Intl.DateTimeFormat,
# capitalization for this string should match the output for your locale.
graph-today = aujourd’hui
# This string is used to describe the graph for screenreader users.
graph-legend-description = Un graphique contenant le nombre total de chaque type de traqueur bloqué cette semaine.
social-tab-title = Traqueurs de réseaux sociaux
social-tab-contant = Les réseaux sociaux placent des traqueurs sur d’autres sites web pour suivre ce que vous faites, lisez et regardez en ligne. Cela permet aux entreprises de réseaux sociaux d’en savoir plus sur vous au-delà de ce que vous partagez sur vos profils en ligne. <a data-l10n-name="learn-more-link">En savoir plus</a>
cookie-tab-title = Cookies utilisés pour le pistage entre les sites
cookie-tab-content = Ces cookies vous suivent de site en site pour collecter des données sur vos faits et gestes en ligne. Ils sont déposés par des tiers, tels que des annonceurs ou des entreprises d’analyse de données. Bloquer les cookies utilisés pour le pistage entre les sites permet de réduire le nombre de publicités qui vous suivent d’un site à l’autre. <a data-l10n-name="learn-more-link">En savoir plus</a>
tracker-tab-title = Contenu utilisé pour le pistage
tracker-tab-content = Les sites web peuvent charger des publicités, des vidéos et d’autres contenus externes qui contiennent des éléments de pistage. Le blocage du contenu utilisé pour le pistage peut accélérer le chargement des sites, mais certains boutons, formulaires ou champs de connexion risquent de ne pas fonctionner. <a data-l10n-name="learn-more-link">En savoir plus</a>
fingerprinter-tab-title = Détecteurs d’empreinte numérique
fingerprinter-tab-content = Les détecteurs d’empreinte numérique recueillent les paramètres de votre navigateur et de votre ordinateur pour créer un profil de vous. En utilisant cette empreinte numérique, ils peuvent vous pister sur différents sites web. <a data-l10n-name="learn-more-link">En savoir plus</a>
cryptominer-tab-title = Mineurs de cryptomonnaies
cryptominer-tab-content = Les mineurs de cryptomonnaies utilisent la puissance de calcul de votre système pour « extraire » de l’argent numérique. Les scripts de cryptominage déchargent votre batterie, ralentissent votre ordinateur et peuvent augmenter votre facture énergétique. <a data-l10n-name="learn-more-link">En savoir plus</a>
lockwise-title = N’oubliez plus jamais vos mots de passe
lockwise-title-logged-in = { -lockwise-brand-name }
lockwise-header-content = { -lockwise-brand-name } conserve de manière sécurisée vos mots de passe dans votre navigateur.
lockwise-header-content-logged-in = Enregistrez et synchronisez vos mots de passe sur tous vos appareils en toute sécurité.
open-about-logins-button = Ouvrir dans { -brand-short-name }
lockwise-no-logins-content = Installez l’application <a data-l10n-name="lockwise-inline-link">{ -lockwise-brand-name }</a> pour avoir vos mots de passe toujours avec vous.
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
lockwise-passwords-stored =
    { $count ->
        [one] mot de passe stocké de façon sécurisée <a data-l10n-name="lockwise-how-it-works">Comment cela fonctionne</a>
       *[other] mots de passe stockés de façon sécurisée <a data-l10n-name="lockwise-how-it-works">Comment cela fonctionne</a>
    }
turn-on-sync = Activer { -sync-brand-short-name }…
    .title = Ouvrir les préférences de synchronisation
manage-devices = Gérer les appareils
# Variables:
#   $count (Number) - Number of devices connected with sync.
lockwise-sync-status =
    { $count ->
        [one] Synchronisation avec { $count } autre appareil
       *[other] Synchronisation avec { $count } autres appareils
    }
lockwise-sync-not-syncing = Pas de synchronisation avec d’autres appareils.
monitor-title = Gardez un œil sur les fuites de données
monitor-link = Principes de fonctionnement
monitor-header-content = Consultez { -monitor-brand-name } pour vérifier si une fuite de données vous concerne et pour recevoir des alertes en cas de nouvelles fuites.
monitor-header-content-logged-in = { -monitor-brand-name } vous alerte si vos informations apparaissent dans une fuite de données connue
monitor-sign-up = S’inscrire aux alertes de fuites de données
auto-scan = Vérifiées aujourd’hui automatiquement
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-monitored-addresses =
    { $count ->
        [one] adresse électronique surveillée.
       *[other] adresses électroniques surveillées.
    }
# This string is displayed after a large numeral that indicates the total number
# of known data breaches. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-known-breaches =
    { $count ->
        [one] fuite de données connue a compromis vos informations.
       *[other] fuites de données connues ont compromis vos informations.
    }
# This string is displayed after a large numeral that indicates the total number
# of exposed passwords. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-exposed-passwords =
    { $count ->
        [one] mot de passe compromis parmi toutes les fuites de données.
       *[other] mots de passe compromis parmi toutes les fuites de données.
    }
full-report-link = Consultez le rapport complet sur <a data-l10n-name="monitor-inline-link">{ -monitor-brand-name }</a>
# This string is displayed after a large numeral that indicates the total number
# of saved logins which may have been exposed. Don’t add $count to
# your localization, because it would result in the number showing twice.
password-warning =
    { $count ->
        [one] mot de passe enregistré peut avoir été compromis par une fuite de données. Changez ce mot de passe pour une sécurité en ligne accrue. <a data-l10n-name="lockwise-link">Voir les mots de passe enregistrés</a>
       *[other] mots de passe enregistrés peuvent avoir été compromis par une fuite de données. Changez ces mots de passe pour une sécurité en ligne accrue. <a data-l10n-name="lockwise-link">Voir les mots de passe enregistrés</a>
    }
# This is the title attribute describing the graph report's link to about:settings#privacy
go-to-privacy-settings = Ouvrir les paramètres de vie privée
# This is the title attribute describing the Lockwise card's link to about:logins
go-to-saved-logins = Ouvrir les identifiants enregistrés

## The title attribute is used to display the type of protection.
## The aria-label is spoken by screen readers to make the visual graph accessible to blind users.
##
## Variables:
##   $count (Number) - Number of specific trackers
##   $percentage (Number) - Percentage this type of tracker contributes to the whole graph

bar-tooltip-social =
    .title = Traqueurs de réseaux sociaux
    .aria-label =
        { $count ->
            [one] { $count } traqueur de réseau social ({ $percentage } %)
           *[other] { $count } traqueurs de réseaux sociaux ({ $percentage } %)
        }
bar-tooltip-cookie =
    .title = Cookies utilisés pour le pistage entre les sites
    .aria-label =
        { $count ->
            [one] { $count } cookie utilisé pour le pistage entre les sites ({ $percentage } %)
           *[other] { $count } cookies utilisés pour le pistage entre les sites ({ $percentage } %)
        }
bar-tooltip-tracker =
    .title = Contenu utilisé pour le pistage
    .aria-label =
        { $count ->
            [one] { $count } contenu utilisé pour le pistage ({ $percentage } %)
           *[other] { $count } contenus utilisés pour le pistage ({ $percentage } %)
        }
bar-tooltip-fingerprinter =
    .title = Détecteurs d’empreinte numérique
    .aria-label =
        { $count ->
            [one] { $count } détecteur d’empreinte numérique ({ $percentage } %)
           *[other] { $count } détecteurs d’empreinte numérique ({ $percentage } %)
        }
bar-tooltip-cryptominer =
    .title = Mineurs de cryptomonnaies
    .aria-label =
        { $count ->
            [one] { $count } mineur de cryptomonnaies ({ $percentage } %)
           *[other] { $count } mineurs de cryptomonnaies ({ $percentage } %)
        }
