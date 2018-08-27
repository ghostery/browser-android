# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = Enviar um sinal “Do Not Track” significando que não pretende ser monitorizado(a)
do-not-track-learn-more = Saber mais
do-not-track-option-default =
    .label = Apenas ao utilizar a Proteção contra monitorização
do-not-track-option-always =
    .label = Sempre
pref-page =
    .title =
        { PLATFORM() ->
            [windows] Opções
           *[other] Preferências
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Please keep the placeholder string short to avoid truncation.
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input-box =
    .style = width: 15.4em
    .placeholder =
        { PLATFORM() ->
            [windows] Localizar nas opções
           *[other] Localizar nas preferências
        }
policies-notice =
    { PLATFORM() ->
        [windows] A sua organização desativou a habilidade de alterar algumas opções.
       *[other] A sua organização desativou a habilidade de alterar algumas preferências.
    }
pane-general-title = Geral
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = Página inicial
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = Pesquisa
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = Privacidade e Segurança
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Conta Firefox
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = Apoio do { -brand-short-name }
focus-search =
    .key = f
close-button =
    .aria-label = Fechar

## Browser Restart Dialog

feature-enable-requires-restart = Tem que reiniciar o { -brand-short-name } para ativar esta funcionalidade.
feature-disable-requires-restart = Tem que reiniciar o { -brand-short-name } para desativar esta funcionalidade.
should-restart-title = Reiniciar o { -brand-short-name }
should-restart-ok = Reiniciar o { -brand-short-name } agora
cancel-no-restart-button = Cancelar
restart-later = Reiniciar mais tarde

## Extension Control Notifications
##
## These strings are used to inform the user
## about changes made by extensions to browser settings.
##
## <img data-l10n-name="icon"/> is going to be replaced by the extension icon.
##
## Variables:
##   $name (String): name of the extension

# This string is shown to notify the user that their home page
# is being controlled by an extension.
extension-controlled-homepage-override = Uma extensão, <img data-l10n-name="icon"/> { $name }, está a controlar a sua página inicial.
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = Uma extensão, <img data-l10n-name="icon"/> { $name }, está a controlar a sua página Novo separador.
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = Uma extensão, <img data-l10n-name="icon"/> { $name }, definiu o seu motor de pesquisa predefinido.
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = Uma extensão, <img data-l10n-name="icon"/> { $name }, requer separadores contentores.
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = Uma extensão, <img data-l10n-name="icon"/> { $name }, está a controlar a proteção contra monitorização.
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = Uma extensão, <img data-l10n-name="icon"/> { $name }, está a controlar como o { -brand-short-name } se liga à internet.
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = Para ativar esta extensão vá a <img data-l10n-name="addons-icon"/> Extras no menu <img data-l10n-name="menu-icon"/>.

## Preferences UI Search Results

search-results-header = Resultados da pesquisa
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] Desculpe! Não existem resultados nas opções para “<span data-l10n-name="query"></span>”.
       *[other] Desculpe! Não existem resultados nas preferências para “<span data-l10n-name="query"></span>”.
    }
search-results-help-link = Precisa de ajuda? Visite o <a data-l10n-name="url">Apoio do { -brand-short-name }</a>

## General Section

startup-header = Arranque
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = Permitir que o { -brand-short-name } e o Firefox sejam utilizados em simultâneo
use-firefox-sync = Dica: Isto utiliza perfis separados. Utilize o { -sync-brand-short-name } para partilhar dados entre eles.
get-started-not-logged-in = Iniciar sessão no { -sync-brand-short-name }…
get-started-configured = Abrir preferências do { -sync-brand-short-name }
always-check-default =
    .label = Verificar sempre se o { -brand-short-name } é o seu navegador predefinido
    .accesskey = V
is-default = O { -brand-short-name } é o seu navegador predefinido
is-not-default = O { -brand-short-name } não é o seu navegador predefinido
set-as-my-default-browser =
    .label = Tornar predefinido…
    .accesskey = d
startup-restore-previous-session =
    .label = Restaurar sessão anterior
    .accesskey = s
disable-extension =
    .label = Desativar extensão
tabs-group-header = Separadores
ctrl-tab-recently-used-order =
    .label = Ctrl+Tab permuta em ciclo os separadores pela ordem dos mais recentemente utilizados
    .accesskey = T
open-new-link-as-tabs =
    .label = Abrir ligações em novos separadores em vez de novas janelas
    .accesskey = j
warn-on-close-multiple-tabs =
    .label = Avisar-lhe ao fechar múltiplos separadores
    .accesskey = m
warn-on-open-many-tabs =
    .label = Avisar-lhe se a abertura de múltiplos separadores puder tornar o { -brand-short-name } lento
    .accesskey = d
switch-links-to-new-tabs =
    .label = Ao abrir uma ligação num novo separador, mudar para o mesmo imediatamente
    .accesskey = o
show-tabs-in-taskbar =
    .label = Pré-visualizar separadores na barra de tarefas do Windows
    .accesskey = s
browser-containers-enabled =
    .label = Ativar separadores contentores
    .accesskey = n
browser-containers-learn-more = Saber mais
browser-containers-settings =
    .label = Definições…
    .accesskey = i
containers-disable-alert-title = Fechar todos os separadores contentores?
containers-disable-alert-desc =
    { $tabCount ->
        [one] Se desativar os separadores contentores agora, { $tabCount } contentor separador será fechado. Tem a certeza de que pretende desativar os separadores contentores?
       *[other] Se desativar os separadores contentores agora, { $tabCount } separadores contentores serão fechados. Tem a certeza de que pretende desativar os separadores contentores?
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] Fechar { $tabCount } separador contentor
       *[other] Fechar { $tabCount } separadores contentores
    }
containers-disable-alert-cancel-button = Manter ativado
containers-remove-alert-title = Remover este contentor?
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg =
    { $count ->
        [one] Se remover este contentor agora, { $count } separador contentor será fechado. Tem a certeza de que pretende remover este contentor?
       *[other] Se remover este contentor agora, { $count } separadores contentores serão fechados. Tem a certeza de que pretende remover este contentor?
    }
containers-remove-ok-button = Remover este contentor
containers-remove-cancel-button = Não remover este contentor

## General Section - Language & Appearance

language-and-appearance-header = Idioma e aparência
fonts-and-colors-header = Tipo de letra e cores
default-font = Tipo de letra predefinido
    .accesskey = d
default-font-size = Tamanho
    .accesskey = n
advanced-fonts =
    .label = Avançadas…
    .accesskey = A
colors-settings =
    .label = Cores…
    .accesskey = C
language-header = Idioma
choose-language-description = Escolha o seu idioma preferencial para apresentar as páginas
choose-button =
    .label = Escolher…
    .accesskey = o
choose-browser-language-description = Escolha os idiomas utilizados para mostrar menus, mensagens, e notificações do { -brand-short-name }.
confirm-browser-language-change-description = Reinicie o { -brand-short-name } para aplicar estas alterações
confirm-browser-language-change-button = Aplicar e reiniciar
translate-web-pages =
    .label = Traduzir conteúdo web
    .accesskey = T
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = Traduções por <img data-l10n-name="logo"/>
translate-exceptions =
    .label = Exceções…
    .accesskey = x
check-user-spelling =
    .label = Corrigir ortografia ao escrever
    .accesskey = t

## General Section - Files and Applications

files-and-applications-title = Ficheiros e aplicações
download-header = Transferências
download-save-to =
    .label = Guardar ficheiros em
    .accesskey = f
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] Escolher…
           *[other] Procurar…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] e
           *[other] o
        }
download-always-ask-where =
    .label = Perguntar sempre onde guardar ficheiros
    .accesskey = a
applications-header = Aplicações
applications-description = Escolha como o { -brand-short-name } manuseia os ficheiros que transfere da web ou as aplicações que utiliza enquanto navega.
applications-filter =
    .placeholder = Pesquisar tipos de ficheiros ou aplicações
applications-type-column =
    .label = Tipo de conteúdo
    .accesskey = T
applications-action-column =
    .label = Ação
    .accesskey = A
drm-content-header = Conteúdo com Gestão de Direitos Digitais (DRM)
play-drm-content =
    .label = Reproduzir conteúdo controlado por DRM
    .accesskey = p
play-drm-content-learn-more = Saber mais
update-application-title = Atualizações do { -brand-short-name }
update-application-description = Mantenha o { -brand-short-name } atualizado para o melhor desempenho, estabilidade, e segurança.
update-application-version = Versão { $version } <a data-l10n-name="learn-more">Novidades</a>
update-history =
    .label = Mostrar histórico de atualizações…
    .accesskey = i
update-application-allow-description = Permitir ao { -brand-short-name }
update-application-auto =
    .label = Instalar atualizações automaticamente (recomendado)
    .accesskey = a
update-application-check-choose =
    .label = Procurar atualizações mas deixar escolher quando as instalar
    .accesskey = c
update-application-manual =
    .label = Nunca procurar atualizações (não recomendado)
    .accesskey = N
update-application-use-service =
    .label = Utilizar um serviço em segundo plano para instalar atualizações
    .accesskey = t
update-enable-search-update =
    .label = Atualizar motores de pesquisa automaticamente
    .accesskey = e

## General Section - Performance

performance-title = Desempenho
performance-use-recommended-settings-checkbox =
    .label = Utilizar definições de desempenho recomendadas
    .accesskey = U
performance-use-recommended-settings-desc = Estas definições são ajustadas para o hardware e sistema operativo do seu computador.
performance-settings-learn-more = Saber mais
performance-allow-hw-accel =
    .label = Se disponível, utilizar aceleração de hardware
    .accesskey = r
performance-limit-content-process-option = Limite de processos de conteúdo
    .accesskey = L
performance-limit-content-process-enabled-desc = Processos de conteúdo adicionais podem melhorar o desempenho ao utilizar múltiplos separadores, mas também irá consumir mais memória.
performance-limit-content-process-blocked-desc = Modificar o número de processos de conteúdo é apenas possível com o multi-processo do { -brand-short-name }. <a data-l10n-name="learn-more">Saber como verificar se o multi-processo está ativado</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (predefinição)

## General Section - Browsing

browsing-title = Navegação
browsing-use-autoscroll =
    .label = Utilizar deslocação automática
    .accesskey = a
browsing-use-smooth-scrolling =
    .label = Utilizar deslocação suave
    .accesskey = u
browsing-use-onscreen-keyboard =
    .label = Mostrar um teclado de toque quando necessário
    .accesskey = t
browsing-use-cursor-navigation =
    .label = Utilizar sempre as teclas do cursor para navegar entre páginas
    .accesskey = c
browsing-search-on-start-typing =
    .label = Pesquisar texto quando começar a escrever
    .accesskey = x

## General Section - Proxy

network-proxy-title = Proxy de rede
network-proxy-connection-description = Configure como o { -brand-short-name } se liga à internet.
network-proxy-connection-learn-more = Saber mais
network-proxy-connection-settings =
    .label = Definições…
    .accesskey = e

## Home Section

home-new-windows-tabs-header = Novas janelas e separadores
home-new-windows-tabs-description2 = Escolha o que vê quando abre a sua página inicial, novas janelas, e novos separadores.

## Home Section - Home Page Customization

home-homepage-mode-label = Página inicial e novas janelas
home-newtabs-mode-label = Novos separadores
home-restore-defaults =
    .label = Restaurar predefinições
    .accesskey = R
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Página inicial do Firefox (Predefinição)
home-mode-choice-custom =
    .label = URLs personalizados...
home-mode-choice-blank =
    .label = Página em branco
home-homepage-custom-url =
    .placeholder = Cole um URL...
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] Utilizar a página atual
           *[other] Utilizar páginas atuais
        }
    .accesskey = u
choose-bookmark =
    .label = Utilizar marcador…
    .accesskey = m

## Search Section

search-bar-header = Barra de pesquisa
search-bar-hidden =
    .label = Utilizar a barra de endereço para pesquisa e navegação
search-bar-shown =
    .label = Adicionar barra de pesquisa à barra de ferramentas
search-engine-default-header = Motor de pesquisa predefinido
search-engine-default-desc = Escolha o motor de pesquisa predefinido a utilizar na barra de endereço e barra de pesquisa.
search-suggestions-option =
    .label = Mostrar sugestões de pesquisa
    .accesskey = s
search-show-suggestions-url-bar-option =
    .label = Mostrar sugestões de pesquisa nos resultados da barra de endereço
    .accesskey = l
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = Mostrar sugestões de pesquisa à frente do histórico de navegação nos resultados da barra de endereço
search-suggestions-cant-show = Sugestões de pesquisa não serão apresentadas nos resultados da barra de localização porque configurou o { -brand-short-name } para nunca memorizar histórico.
search-one-click-header = Motores de pesquisa de um clique
search-one-click-desc = Escolha os motores de pesquisa alternativos que aparecem debaixo da barra de endereço e barra de pesquisa quando começa a introduzir uma palavra-chave.
search-choose-engine-column =
    .label = Motor de pesquisa
search-choose-keyword-column =
    .label = Palavra-chave
search-restore-default =
    .label = Restaurar motores de pesquisa predefinidos
    .accesskey = d
search-remove-engine =
    .label = Remover
    .accesskey = R
search-find-more-link = Encontrar mais motores de pesquisa
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = Palavra-chave duplicada
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = Escolheu uma palavra-chave que está atualmente a ser utilizada por “{ $name }”. Por favor, selecione outra.
search-keyword-warning-bookmark = Escolheu uma palavra chave que está a ser utilizada por um marcador. Por favor, escolha outra.

## Containers Section

containers-back-link = « Retroceder
containers-header = Separadores contentores
containers-add-button =
    .label = Adicionar novo contentor
    .accesskey = A
containers-preferences-button =
    .label = Preferências
containers-remove-button =
    .label = Remover

## Sync Section - Signed out

sync-signedout-caption = Leve a sua Web consigo
sync-signedout-description = Sincronize os seus marcadores, histórico, separadores, palavras-passe, extras e preferências entre todos os seus dispositivos.
sync-signedout-account-title = Ligar com uma { -fxaccount-brand-name }
sync-signedout-account-create = Não tem uma conta? Comece aqui
    .accesskey = C
sync-signedout-account-signin =
    .label = Iniciar sessão…
    .accesskey = I
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Transfira o Firefox para <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> ou <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a> para sincronizar com o seu dispositivo móvel.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = Alterar imagem de perfil
sync-disconnect =
    .label = Desligar…
    .accesskey = D
sync-manage-account = Gerir conta
    .accesskey = o
sync-signedin-unverified = { $email } não está verificado.
sync-signedin-login-failure = Inicie sessão para voltar a estabelecer ligação { $email }
sync-resend-verification =
    .label = Reenviar verificação
    .accesskey = r
sync-remove-account =
    .label = Remover conta
    .accesskey = R
sync-sign-in =
    .label = Iniciar sessão
    .accesskey = c
sync-signedin-settings-header = Definições de sincronização
sync-signedin-settings-desc = Escolha o que sincronizar nos seus dispositivos que utilizam o { -brand-short-name }.
sync-engine-bookmarks =
    .label = Marcadores
    .accesskey = M
sync-engine-history =
    .label = Histórico
    .accesskey = r
sync-engine-tabs =
    .label = Separadores abertos
    .tooltiptext = Uma lista do que está aberto em todos os dispositivos sincronizados
    .accesskey = S
sync-engine-logins =
    .label = Credenciais
    .tooltiptext = Nomes de utilizador e palavras-passe que guardou
    .accesskey = i
sync-engine-addresses =
    .label = Endereços
    .tooltiptext = Endereços postais que guardou (computador apenas)
    .accesskey = e
sync-engine-creditcards =
    .label = Cartões de crédito
    .tooltiptext = Nomes, números e datas de expiração (computador apenas)
    .accesskey = C
sync-engine-addons =
    .label = Extras
    .tooltiptext = Extensões e temas para o Firefox no computador
    .accesskey = a
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] Opções
           *[other] Preferências
        }
    .tooltiptext = Definições gerais, de privacidade, e segurança que alterou
    .accesskey = s
sync-device-name-header = Nome do dispositivo
sync-device-name-change =
    .label = Alterar nome do dispositivo…
    .accesskey = l
sync-device-name-cancel =
    .label = Cancelar
    .accesskey = n
sync-device-name-save =
    .label = Guardar
    .accesskey = r
sync-mobilepromo-single = Ligar outro dispositivo
sync-mobilepromo-multi = Gerir dispositivos
sync-tos-link = Termos de serviço
sync-fxa-privacy-notice = Aviso de privacidade

## Privacy Section

privacy-header = Privacidade do navegador

## Privacy Section - Forms

forms-header = Formulários e palavras-passe
forms-ask-to-save-logins =
    .label = Perguntar para guardar credenciais e palavras-passe para websites
    .accesskey = r
forms-exceptions =
    .label = Exceções…
    .accesskey = x
forms-saved-logins =
    .label = Credenciais guardadas…
    .accesskey = g
forms-master-pw-use =
    .label = Utilizar uma palavra-passe mestra
    .accesskey = U
forms-master-pw-change =
    .label = Alterar palavra-passe mestra…
    .accesskey = m

## Privacy Section - History

history-header = Histórico
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = O { -brand-short-name } irá
    .accesskey = i
history-remember-option-all =
    .label = Memorizar histórico
history-remember-option-never =
    .label = Nunca memorizar histórico
history-remember-option-custom =
    .label = Utilizar definições personalizadas para o histórico
history-remember-description = O { -brand-short-name } irá memorizar o seu histórico de navegação, transferências, formulários e pesquisa.
history-dontremember-description = O { -brand-short-name } irá utilizar as mesmas definições da navegação privada e não irá memorizar qualquer histórico enquanto navega na Web.
history-private-browsing-permanent =
    .label = Utilizar sempre o modo de navegação privada
    .accesskey = p
history-remember-option =
    .label = Memorizar o meu histórico de navegação e de transferências
    .accesskey = h
history-remember-browser-option =
    .label = Memorizar histórico de navegação e de transferências
    .accesskey = z
history-remember-search-option =
    .label = Memorizar histórico de pesquisas e de formulários
    .accesskey = f
history-clear-on-close-option =
    .label = Limpar o histórico quando o { -brand-short-name } for fechado
    .accesskey = i
history-clear-on-close-settings =
    .label = Definições…
    .accesskey = e
history-clear-button =
    .label = Limpar histórico…
    .accesskey = s

## Privacy Section - Site Data

sitedata-header = Cookies e dados de sites
sitedata-total-size-calculating = A calcular tamanho dos dados de sites e cache…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = Os seus cookies, dados de sites e cache estão atualmente a utilizar { $value } { $unit } de espaço em disco.
sitedata-learn-more = Saber mais
sitedata-accept-cookies-option =
    .label = Aceitar cookies e dados de sites de websites (recomendado)
    .accesskey = A
sitedata-block-cookies-option =
    .label = Bloquear cookies e dados de sites (pode fazer com que os websites deixam de funcionar)
    .accesskey = B
sitedata-keep-until = Manter até
    .accesskey = a
sitedata-keep-until-expire =
    .label = expirarem
sitedata-keep-until-closed =
    .label = o { -brand-short-name } ser fechado
sitedata-accept-third-party-desc = Aceitar cookies de terceiros e dados de sites
    .accesskey = s
sitedata-accept-third-party-always-option =
    .label = Sempre
sitedata-accept-third-party-visited-option =
    .label = Dos visitados
sitedata-accept-third-party-never-option =
    .label = Nunca
sitedata-allow-cookies-option =
    .label = Aceitar cookies e dados de sites
    .accesskey = A
sitedata-disallow-cookies-option =
    .label = Bloquear cookies e dados de sites
    .accesskey = B
# This label means 'type of content that is blocked', and is followed by a drop-down list with content types below.
# The list items are the strings named sitedata-block-*-option*.
sitedata-block-desc = Tipo bloqueado
    .accesskey = T
sitedata-block-trackers-option-recommended =
    .label = Trackers de terceiros (recomendado)
sitedata-block-trackers-option =
    .label = Trackers de terceiros
sitedata-block-unvisited-option =
    .label = Cookies de sites não visitados
sitedata-block-all-third-parties-option =
    .label = Todos os cookies de terceiros
sitedata-block-always-option =
    .label = Todos os cookies (pode causar que os websites quebrem)
sitedata-clear =
    .label = Limpar dados…
    .accesskey = L
sitedata-settings =
    .label = Gerir dados…
    .accesskey = G
sitedata-cookies-exceptions =
    .label = Exceções…
    .accesskey = E

## Privacy Section - Address Bar

addressbar-header = Barra de endereço
addressbar-suggest = Ao utilizar a barra de endereço, sugerir
addressbar-locbar-history-option =
    .label = Histórico de navegação
    .accesskey = H
addressbar-locbar-bookmarks-option =
    .label = Marcadores
    .accesskey = M
addressbar-locbar-openpage-option =
    .label = Separadores abertos
    .accesskey = o
addressbar-suggestions-settings = Alterar preferências para as sugestões dos motores de pesquisa

## Privacy Section - Content Blocking

content-blocking-header = Bloqueio de conteúdo
content-blocking-desc = Bloqueie conteúdo de terceiros, tal como anúncios ou código, que podem tornar o seu navegador mais lento e lhe monitorizam ao longo da web. Personalize as suas definições para o melhor equilíbrio de proteção e desempenho.
content-blocking-learn-more = Saber mais
content-blocking-restore-defaults =
    .label = Repor predefinições
    .accesskey = R
content-blocking-toggle-on =
    .tooltiptext = Desativar o bloqueio de conteúdo
content-blocking-toggle-off =
    .tooltiptext = Ativar o bloqueio de conteúdo
content-blocking-toggle-label-on = LIGADO
    .accesskey = L
content-blocking-toggle-label-off = DESLIGADO
    .accesskey = D
content-blocking-category-label = Escolha o que bloquear
# "Slow" in this instance means "slow to load on the network".
# FastBlock is a feature that blocks requests to tracking sites if they
# have not finished loading after a certain threshold of seconds.
content-blocking-fastblock-label = Elementos de monitorização lentos
    .accesskey = s
content-blocking-fastblock-description = Bloqueia conteúdo de terceiros que demora mais do que 5 segundos a carregar.
content-blocking-fastblock-option-enabled =
    .label = Bloquear sempre
content-blocking-fastblock-option-disabled =
    .label = Nunca bloquear
content-blocking-tracking-protection-label = Trackers
    .accesskey = T
content-blocking-tracking-protection-description = Bloqueia todos os trackers conhecidos (Nota: também pode impedir algumas páginas de serem carregadas).
content-blocking-tracking-protection-option-enabled =
    .label = Bloquear sempre
content-blocking-tracking-protection-option-pbm =
    .label = Bloquear apenas em janelas privadas
content-blocking-tracking-protection-option-disabled =
    .label = Nunca bloquear
content-blocking-tracking-protection-change-blocklist = Alterar lista de bloqueio…

## Privacy Section - Tracking

tracking-header = Proteção contra monitorização
tracking-desc = A Proteção contra monitorização bloqueia trackers online que recolhem os seus dados de navegação através de múltiplos websites. <a data-l10n-name="learn-more">Saber mais acerca da Proteção contra monitorização e a sua privacidade</a>
tracking-mode-label = Utilizar Proteção contra monitorização para bloquear trackers conhecidos
tracking-mode-always =
    .label = Sempre
    .accesskey = r
tracking-mode-private =
    .label = Apenas em janelas privadas
    .accesskey = l
tracking-mode-never =
    .label = Nunca
    .accesskey = N
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = Utilizar Proteção contra monitorização na Navegação privada para bloquear trackers conhecidos
    .accesskey = v
tracking-exceptions =
    .label = Exceções…
    .accesskey = x
tracking-change-block-list =
    .label = Alterar lista de bloqueio…
    .accesskey = A

## Privacy Section - Permissions

permissions-header = Permissões
permissions-location = Localização
permissions-location-settings =
    .label = Definições…
    .accesskey = f
permissions-camera = Câmara
permissions-camera-settings =
    .label = Definições…
    .accesskey = f
permissions-microphone = Microfone
permissions-microphone-settings =
    .label = Definições…
    .accesskey = f
permissions-notification = Notificações
permissions-notification-settings =
    .label = Definições…
    .accesskey = n
permissions-notification-link = Saber mais
permissions-notification-pause =
    .label = Pausar notificações até o { -brand-short-name } reiniciar
    .accesskey = n
permissions-block-autoplay-media =
    .label = Bloquear websites de reproduzir automaticamente multimédia com som
    .accesskey = B
permissions-block-autoplay-media-menu = Para websites que reproduzem automaticamente som
permissions-block-autoplay-media-exceptions =
    .label = Exceções…
    .accesskey = E
autoplay-option-ask =
    .label = Perguntar sempre
autoplay-option-allow =
    .label = Permitir reprodução automática
autoplay-option-dont =
    .label = Não reproduzir automaticamente
permissions-autoplay-link = Saber mais
permissions-block-popups =
    .label = Bloquear janelas pop-up
    .accesskey = B
permissions-block-popups-exceptions =
    .label = Exceções…
    .accesskey = E
permissions-addon-install-warning =
    .label = Avisar-lhe quando os websites tentam instalar extras
    .accesskey = A
permissions-addon-exceptions =
    .label = Exceções…
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = Impedir os serviços de acessibilidade de aceder ao seu navegador
    .accesskey = a
permissions-a11y-privacy-link = Saber mais

## Privacy Section - Data Collection

collection-header = Recolha de dados e utilização do { -brand-short-name }
collection-description = Nós esforçamos-nos para lhe fornecer escolhas e recolher apenas o que precisamos para fornecer e melhorar o { -brand-short-name } para toda a gente. Pedimos sempre permissão antes de receber informação pessoal.
collection-privacy-notice = Aviso de privacidade
collection-health-report =
    .label = Permitir ao { -brand-short-name } enviar dados técnicos e de interação para a { -vendor-short-name }
    .accesskey = r
collection-health-report-link = Saber mais
collection-studies =
    .label = Permitir ao { -brand-short-name } instalar e executar estudos
collection-studies-link = Ver estudos do { -brand-short-name }
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = Relato de dados está desativado para a configuração desta compilação
collection-browser-errors =
    .label = Permitir ao { -brand-short-name } enviar relatórios de erro do navegador (incluindo mensagens de erro) para a { -vendor-short-name }
    .accesskey = n
collection-browser-errors-link = Saber mais
collection-backlogged-crash-reports =
    .label = Permitir ao { -brand-short-name } enviar relatórios de falha pendentes em seu nome
    .accesskey = f
collection-backlogged-crash-reports-link = Saber mais

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = Segurança
security-browsing-protection = Conteúdo decetivo e proteção contra software perigoso
security-enable-safe-browsing =
    .label = Bloquear conteúdo perigoso e decetivo
    .accesskey = B
security-enable-safe-browsing-link = Saber mais
security-block-downloads =
    .label = Bloquear transferências perigosas
    .accesskey = t
security-block-uncommon-software =
    .label = Avisar-lhe acerca de software não-solicitado e incomum
    .accesskey = c

## Privacy Section - Certificates

certs-header = Certificados
certs-personal-label = Quando um servidor solicitar o seu certificado pessoal
certs-select-auto-option =
    .label = Selecionar um automaticamente
    .accesskey = S
certs-select-ask-option =
    .label = Perguntar-lhe sempre
    .accesskey = A
certs-enable-ocsp =
    .label = Consultar os servidores de resposta OCSP para confirmar a validade de certificados
    .accesskey = O
certs-view =
    .label = Ver certificados…
    .accesskey = c
certs-devices =
    .label = Dispositivos de segurança…
    .accesskey = D
