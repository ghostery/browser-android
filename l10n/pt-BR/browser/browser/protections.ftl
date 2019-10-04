# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
#   $count (Number) - Number of tracking events blocked.
graph-week-summary =
    { $count ->
        [one] O { -brand-short-name } bloqueou { $count } rastreador na semana passada
       *[other] O { -brand-short-name } bloqueou { $count } rastreadores na semana passada
    }
# Variables:
#   $count (Number) - Number of tracking events blocked.
#   $earliestDate (Number) - Unix timestamp in ms, representing a date. The
# earliest date recorded in the database.
graph-total-summary =
    { $count ->
        [one] { $count } rastreador bloqueado desde { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
       *[other] { $count } rastreadores bloqueados desde { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
    }
# The terminology used to refer to categories of Content Blocking is also used in chrome/browser/browser.properties and should be translated consistently.
# "Standard" in this case is an adjective, meaning "default" or "normal".
# The category name in the <b> tag will be bold.
protection-header-details-standard = O nível de proteção está definido como <b>Padrão</b>
protection-header-details-strict = O nível de proteção está definido como <b>Rigoroso</b>
protection-header-details-custom = O nível de proteção está definido como <b>Personalizado</b>
protection-report-page-title = Proteções de privacidade
protection-report-content-title = Proteções de privacidade
etp-card-title = Proteção aprimorada contra rastreamento
etp-card-content = Rastreadores seguem você online por todo canto para coletar informações sobre seus interesses e hábitos de navegação. O { -brand-short-name } bloqueia muitos desses rastreadores e outros scripts maliciosos.
# This string is used to label the X axis of a graph. Other days of the week are generated via Intl.DateTimeFormat,
# capitalization for this string should match the output for your locale.
graph-today = Hoje
# This string is used to describe the graph for screenreader users.
graph-legend-description = Um gráfico contendo o número total de cada tipo de rastreador bloqueado esta semana.
social-tab-title = Rastreadores de mídias sociais
social-tab-contant = Redes sociais colocam rastreadores em outros sites para seguir o que você faz, vê e assiste online. Isto permite que empresas de mídias sociais saibam mais sobre você, muito além do que você compartilha em seus perfis de mídias sociais. <a data-l10n-name="learn-more-link">Saiba mais</a>
cookie-tab-title = Cookies de rastreamento entre sites
cookie-tab-content = Esses cookies seguem você de um site para outro para coletar dados sobre o que você faz online. Eles são criados por terceiros, como anunciantes e empresas de análises. Bloquear cookies de rastreamento entre sites reduz o número de anúncios que seguem você por todo canto. <a data-l10n-name="learn-more-link">Saiba mais</a>
tracker-tab-title = Conteúdo de rastreamento
tracker-tab-content = Sites podem carregar anúncios, vídeos e outros conteúdos externos que contêm código de rastreamento. Bloquear conteúdo de rastreamento pode ajudar a agilizar o carregamento de sites, mas alguns botões, formulários e campos de acesso a contas podem não funcionar. <a data-l10n-name="learn-more-link">Saiba mais</a>
fingerprinter-tab-title = Fingerprinters
fingerprinter-tab-content = Fingerprinters coletam configurações do seu navegador e do computador para traçar um perfil seu. Usando esta identidade digital, eles podem rastrear você por vários sites. <a data-l10n-name="learn-more-link">Saiba mais</a>
cryptominer-tab-title = Criptomineradores
cryptominer-tab-content = Criptomineradores usam o poder computacional do seu sistema para minerar moedas digitais. Scripts de criptomineração drenam sua bateria, fazem seu computador ficar mais lento e podem aumentar sua conta de energia elétrica. <a data-l10n-name="learn-more-link">Saiba mais</a>
lockwise-title = Nunca esqueça uma senha novamente
lockwise-title-logged-in = { -lockwise-brand-name }
lockwise-header-content = O { -lockwise-brand-name } armazena com segurança suas senhas em seu navegador.
lockwise-header-content-logged-in = Armazene e sincronize suas senhas com segurança em todos os seus dispositivos.
open-about-logins-button = Abrir no { -brand-short-name }
lockwise-no-logins-content = Instale o aplicativo <a data-l10n-name="lockwise-inline-link">{ -lockwise-brand-name }</a> para ter suas senhas em qualquer lugar.
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
lockwise-passwords-stored =
    { $count ->
        [one] Senha armazenada com segurança <a data-l10n-name="lockwise-how-it-works">Como funciona</a>
       *[other] Senhas armazenadas com segurança <a data-l10n-name="lockwise-how-it-works">Como funciona</a>
    }
turn-on-sync = Ativar o { -sync-brand-short-name }…
    .title = Ir para as preferências de sincronização
manage-devices = Gerenciar dispositivos
# Variables:
#   $count (Number) - Number of devices connected with sync.
lockwise-sync-status =
    { $count ->
        [one] Sincronizando com { $count } outro dispositivo
       *[other] Sincronizando com { $count } outros dispositivos
    }
lockwise-sync-not-syncing = Não sincronizando com outros dispositivos.
monitor-title = Fique atento a violações de dados
monitor-link = Como funciona
monitor-header-content = Veja no { -monitor-brand-name } se você foi vítima de um vazamento de dados e receba alertas sobre novos vazamentos.
monitor-header-content-logged-in = O { -monitor-brand-name } avisa caso suas informações apareçam em um vazamento de dados conhecido
monitor-sign-up = Cadastre-se para receber alertas de vazamentos
auto-scan = Analisados automaticamente hoje
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-monitored-addresses =
    { $count ->
        [one] Endereço de e-mail sendo monitorado.
       *[other] Endereços de e-mail sendo monitorados.
    }
# This string is displayed after a large numeral that indicates the total number
# of known data breaches. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-known-breaches =
    { $count ->
        [one] Vazamento de dados conhecido expôs suas informações.
       *[other] Vazamentos de dados conhecidos expuseram suas informações.
    }
# This string is displayed after a large numeral that indicates the total number
# of exposed passwords. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-exposed-passwords =
    { $count ->
        [one] Senha exposta em meio a todos os vazamentos.
       *[other] Senhas expostas em meio a todos os vazamentos.
    }
full-report-link = Ver relatório completo no <a data-l10n-name="monitor-inline-link">{ -monitor-brand-name }</a>
# This string is displayed after a large numeral that indicates the total number
# of saved logins which may have been exposed. Don’t add $count to
# your localization, because it would result in the number showing twice.
password-warning =
    { $count ->
        [one] Conta salva pode ter sido exposta em um vazamento de dados. Mude esta senha para melhor segurança online. <a data-l10n-name="lockwise-link">Ver contas salvas</a>
       *[other] Contas salvas podem ter sido expostas em um vazamento de dados. Mude estas senhas para melhor segurança online. <a data-l10n-name="lockwise-link">Ver contas salvas</a>
    }
# This is the title attribute describing the graph report's link to about:settings#privacy
go-to-privacy-settings = Ir para as configurações de privacidade
# This is the title attribute describing the Lockwise card's link to about:logins
go-to-saved-logins = Ir para contas salvas

## The title attribute is used to display the type of protection.
## The aria-label is spoken by screen readers to make the visual graph accessible to blind users.
##
## Variables:
##   $count (Number) - Number of specific trackers
##   $percentage (Number) - Percentage this type of tracker contributes to the whole graph

bar-tooltip-social =
    .title = Rastreadores de mídias sociais
    .aria-label =
        { $count ->
            [one] { $count } rastreador de mídias sociais ({ $percentage }%)
           *[other] { $count } rastreadores de mídias sociais ({ $percentage }%)
        }
bar-tooltip-cookie =
    .title = Cookies de rastreamento entre sites
    .aria-label =
        { $count ->
            [one] { $count } cookie de rastreamento entre sites ({ $percentage }%)
           *[other] { $count } cookies de rastreamento entre sites ({ $percentage }%)
        }
bar-tooltip-tracker =
    .title = Conteúdo de rastreamento
    .aria-label =
        { $count ->
            [one] { $count } conteúdo de rastreamento ({ $percentage }%)
           *[other] { $count } conteúdos de rastreamento ({ $percentage }%)
        }
bar-tooltip-fingerprinter =
    .title = Fingerprinters
    .aria-label =
        { $count ->
            [one] { $count } fingerprinter ({ $percentage }%)
           *[other] { $count } fingerprinters ({ $percentage }%)
        }
bar-tooltip-cryptominer =
    .title = Criptomineradores
    .aria-label =
        { $count ->
            [one] { $count } criptominerador ({ $percentage }%)
           *[other] { $count } criptomineradores ({ $percentage }%)
        }
