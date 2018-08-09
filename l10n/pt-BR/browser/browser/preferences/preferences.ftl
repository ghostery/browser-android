# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = Enviar aos sites um sinal de “Não rastrear” informando que você não quer ser rastreado
do-not-track-learn-more = Saiba mais
do-not-track-option-default =
    .label = Somente quando usar a proteção contra rastreamento
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
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input =
    .style = width: 15.4em
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
            [windows] Pesquisar em opções
           *[other] Pesquisar em preferências
        }
policies-notice =
    { PLATFORM() ->
        [windows] Sua organização desativou a possibilidade de modificar algumas opções
       *[other] Sua organização desativou a possibilidade de modificar algumas preferências
    }
pane-general-title = Geral
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = Início
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
help-button-label = Suporte { -brand-short-name }
focus-search =
    .key = f
close-button =
    .aria-label = Fechar

## Browser Restart Dialog

feature-enable-requires-restart = O { -brand-short-name } deve ser reiniciado para habilitar esta funcionalidade.
feature-disable-requires-restart = O { -brand-short-name } deve ser reiniciado para desativar esta funcionalidade.
should-restart-title = Reiniciar o { -brand-short-name }
should-restart-ok = Reiniciar o { -brand-short-name } agora
cancel-no-restart-button = Cancelar
restart-later = Reiniciar depois

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
extension-controlled-homepage-override = Uma extensão, <img data-l10n-name="icon"/> { $name }, está controlando sua página inicial.
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = Uma extensão, <img data-l10n-name="icon"/> { $name }, está controlando a sua página de nova aba.
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = Uma extensão, <img data-l10n-name="icon"/> { $name }, definiu o seu mecanismo de pesquisa padrão.
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = Uma extensão requer abas contêiner: <img data-l10n-name="icon"/> { $name }.
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = Uma extensão, <img data-l10n-name="icon"/> { $name }, está controlando sua proteção contra rastreamento.
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = Uma extensão, <img data-l10n-name="icon"/> { $name }, está controlando como o { -brand-short-name } se conecta à Internet.
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = Para habilitar esta extensão vá para <img data-l10n-name="addons-icon"/> Extensões no menu <img data-l10n-name="menu-icon"/>.

## Preferences UI Search Results

search-results-header = Resultados da pesquisa
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] Desculpa! Não há nenhum resultado nas opções para “<span data-l10n-name="query"></span>”.
       *[other] Desculpa! Não há nenhum resultado nas preferências para “<span data-l10n-name="query"></span>”.
    }
search-results-help-link = Precisa de ajuda? Visite o <a data-l10n-name="url">Suporte do { -brand-short-name }</a>

## General Section

startup-header = Iniciar
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = Permitir que o { -brand-short-name } e o Firefox funcionem ao mesmo tempo
use-firefox-sync = Dica: São usados perfis separados. Use o { -sync-brand-short-name } para compartilhar dados entre eles.
get-started-not-logged-in = Entrar no { -sync-brand-short-name }…
get-started-configured = Abrir preferências { -sync-brand-short-name }
always-check-default =
    .label = Sempre verificar se o { -brand-short-name } é o navegador padrão
    .accesskey = S
is-default = { -brand-short-name } é o seu navegador padrão
is-not-default = { -brand-short-name } não é o seu navegador padrão
set-as-my-default-browser =
    .label = Tornar padrão…
    .accesskey = D
startup-page = Ao iniciar o { -brand-short-name }
    .accesskey = i
startup-user-homepage =
    .label = Mostrar sua página inicial
startup-blank-page =
    .label = Mostrar uma página em branco
startup-prev-session =
    .label = Mostrar suas janelas e abas anteriores
startup-restore-previous-session =
    .label = Restaurar a sessão anterior
    .accesskey = R
disable-extension =
    .label = Desativar extensão
home-page-header = Página inicial
tabs-group-header = Abas
ctrl-tab-recently-used-order =
    .label = Ctrl+Tab alterna entre as abas por ordem de uso
    .accesskey = T
open-new-link-as-tabs =
    .label = Abrir links em abas em vez de novas janelas
    .accesskey = j
warn-on-close-multiple-tabs =
    .label = Avisar quando fechar múltiplas abas
    .accesskey = m
warn-on-open-many-tabs =
    .label = Avisar que quando abrir muitas abas, o { -brand-short-name } pode ficar lento
    .accesskey = d
switch-links-to-new-tabs =
    .label = Quando abrir um link em uma nova aba, alterne para ela imediatamente
    .accesskey = u
show-tabs-in-taskbar =
    .label = Mostrar a visualização das abas na barra de tarefas do Windows
    .accesskey = v
browser-containers-enabled =
    .label = Habilitar abas contêiner
    .accesskey = n
browser-containers-learn-more = Saiba mais
browser-containers-settings =
    .label = Configurações…
    .accesskey = i
containers-disable-alert-title = Fechar todas as abas contêiner?
containers-disable-alert-desc =
    { $tabCount ->
        [one] Se desativar Abas Contêiner agora, { $tabCount } aba contêiner será fechada. Tem certeza que deseja desativar Abas Contêiner?
       *[other] Se desativar Abas Contêiner agora, { $tabCount } abas contêiner serão fechadas. Tem certeza que deseja desativar Abas Contêiner?
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] Fechar { $tabCount } aba contêiner
       *[other] Fechar { $tabCount } abas contêiner
    }
containers-disable-alert-cancel-button = Manter habilitado
containers-remove-alert-title = Remover este contêiner?
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg =
    { $count ->
        [one] Se você remover este Contêiner agora, { $count } aba contêiner será fechada. Tem certeza que deseja remover este Contêiner?
       *[other] Se você remover este Contêiner agora, { $count } abas contêiner serão fechadas. Tem certeza que deseja remover este Contêiner?
    }
containers-remove-ok-button = Remover este contêiner
containers-remove-cancel-button = Não remover este contêiner

## General Section - Language & Appearance

language-and-appearance-header = Idioma e Aparência
fonts-and-colors-header = Fontes e cores
default-font = Fonte padrão
    .accesskey = d
default-font-size = Tamanho
    .accesskey = T
advanced-fonts =
    .label = Avançado…
    .accesskey = v
colors-settings =
    .label = Cores…
    .accesskey = o
language-header = Idioma
choose-language-description = Escolha o idioma preferido para exibir páginas
choose-button =
    .label = Selecionar…
    .accesskey = S
translate-web-pages =
    .label = Traduzir conteúdo da web
    .accesskey = r
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = Traduções por <img data-l10n-name="logo"/>
translate-exceptions =
    .label = Exceções…
    .accesskey = x
check-user-spelling =
    .label = Verificar a ortografia ao digitar
    .accesskey = t

## General Section - Files and Applications

files-and-applications-title = Arquivos e Aplicativos
download-header = Downloads
download-save-to =
    .label = Salvar arquivos em
    .accesskey = S
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] Escolher…
           *[other] Procurar…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] e
           *[other] P
        }
download-always-ask-where =
    .label = Sempre perguntar onde salvar arquivos
    .accesskey = a
applications-header = Aplicativos
applications-description = Escolha como o { -brand-short-name } manipula os arquivos que você baixar da Web ou os aplicativos que você usar enquanto navega.
applications-filter =
    .placeholder = Pesquisar tipos de arquivos ou aplicativos
applications-type-column =
    .label = Tipo de conteúdo
    .accesskey = T
applications-action-column =
    .label = Ação
    .accesskey = A
drm-content-header = Conteúdo DRM (Digital Rights Management - Gerenciamento de Direitos Digitais)
play-drm-content =
    .label = Reproduzir conteúdo controlado por DRM
    .accesskey = R
play-drm-content-learn-more = Saiba mais
update-application-title = Atualizações do { -brand-short-name }
update-application-description = Manter o { -brand-short-name } atualizado para melhor desempenho, estabilidade e segurança.
update-application-info = Versão { $version } <a>Novidades</a>
update-application-version = Versão { $version } <a data-l10n-name="learn-more">Novidades</a>
update-history =
    .label = Mostrar histórico de atualizações…
    .accesskey = p
update-application-allow-description = Permitir que o { -brand-short-name }
update-application-auto =
    .label = Instale atualizações automaticamente (recomendado)
    .accesskey = A
update-application-check-choose =
    .label = Busque atualizações, mas permita que você opte por instalá-las
    .accesskey = C
update-application-manual =
    .label = Nunca busque atualizações (não recomendado)
    .accesskey = N
update-application-use-service =
    .label = Usar um serviço em segundo plano para instalar atualizações
    .accesskey = s
update-enable-search-update =
    .label = Atualizar automaticamente os mecanismos de pesquisa
    .accesskey = e

## General Section - Performance

performance-title = Desempenho
performance-use-recommended-settings-checkbox =
    .label = Usar as configurações de desempenho recomendadas
    .accesskey = U
performance-use-recommended-settings-desc = Essas configurações são adaptadas ao hardware e sistema operacional do seu computador.
performance-settings-learn-more = Saiba mais
performance-allow-hw-accel =
    .label = Usar aceleração de hardware quando disponível
    .accesskey = r
performance-limit-content-process-option = Limite de processos de conteúdo
    .accesskey = L
performance-limit-content-process-enabled-desc = Processos de conteúdo adicionais podem melhorar o desempenho ao usar várias abas, mas também usam mais memória.
performance-limit-content-process-disabled-desc = Modificar o número de processos de conteúdo só é possível com o multiprocessamento do { -brand-short-name }. <a>Saiba como verificar se o multiprocessamento está ativado</a>
performance-limit-content-process-blocked-desc = Modificar o número de processos de conteúdo só é possível com o multiprocessamento do { -brand-short-name }. <a data-l10n-name="learn-more">Saiba como verificar se o multiprocessamento está ativado</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (padrão)

## General Section - Browsing

browsing-title = Navegação
browsing-use-autoscroll =
    .label = Usar autorrolagem
    .accesskey = a
browsing-use-smooth-scrolling =
    .label = Usar rolagem suave
    .accesskey = m
browsing-use-onscreen-keyboard =
    .label = Mostrar um teclado sensível ao toque quando necessário
    .accesskey = t
browsing-use-cursor-navigation =
    .label = Sempre usar as teclas de cursor para navegar dentro das páginas
    .accesskey = c
browsing-search-on-start-typing =
    .label = Procurar texto quando começar a digitar
    .accesskey = x

## General Section - Proxy

network-proxy-title = Proxy de Rede
network-proxy-connection-description = Configure como o { -brand-short-name } se conecta à Internet.
network-proxy-connection-learn-more = Saiba mais
network-proxy-connection-settings =
    .label = Configurar conexão…
    .accesskey = e

## Home Section

home-new-windows-tabs-header = Novas janelas e abas
home-new-windows-tabs-description2 = Escolha o que você vê quando abrir sua página inicial, novas janelas e novas abas.

## Home Section - Home Page Customization

home-homepage-mode-label = Página inicial e novas janelas
home-newtabs-mode-label = Novas abas
home-restore-defaults =
    .label = Restaurar padrões
    .accesskey = R
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Página inicial do Firefox (padrão)
home-mode-choice-custom =
    .label = URLs personalizadas...
home-mode-choice-blank =
    .label = Página em branco
home-homepage-custom-url =
    .placeholder = Cole uma URL...
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] Usar a página aberta
           *[other] Usar as páginas abertas
        }
    .accesskey = a
choose-bookmark =
    .label = Usar favorito…
    .accesskey = f
restore-default =
    .label = Restaurar o padrão
    .accesskey = R

## Search Section

search-bar-header = Barra de pesquisa
search-bar-hidden =
    .label = Use a barra de endereços para pesquisar e navegar
search-bar-shown =
    .label = Adicionar a barra de pesquisa na barra de ferramentas
search-engine-default-header = Mecanismo de pesquisa padrão
search-engine-default-desc = Escolha o mecanismo de pesquisa padrão para usar na barra de endereços e na barra de pesquisa.
search-suggestions-option =
    .label = Sugestões de pesquisa
    .accesskey = S
search-show-suggestions-url-bar-option =
    .label = Mostrar sugestões de pesquisa nos resultados da barra de endereços
    .accesskey = l
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = Mostrar sugestões de pesquisa antes do histórico de navegação nos resultados da barra de endereço
search-suggestions-cant-show = As sugestões de pesquisa não serão mostradas nos resultados da barra de endereço, porque você configurou o { -brand-short-name } para nunca lembrar o histórico.
search-one-click-header = Mecanismos de pesquisa em um clique
search-one-click-desc = Escolha os mecanismos de pesquisa alternativos que aparecem abaixo da barra de endereços e da barra de pesquisa quando você começa a inserir uma palavra-chave.
search-choose-engine-column =
    .label = Mecanismo de pesquisa
search-choose-keyword-column =
    .label = Palavra-chave
search-restore-default =
    .label = Restaurar mecanismos de pesquisa padrão
    .accesskey = p
search-remove-engine =
    .label = Remover
    .accesskey = R
search-find-more-link = Procurar mais mecanismos de pesquisa
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = Palavra-chave duplicada
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = A palavra-chave escolhida já é usada por “{ $name }”. Por favor, selecione outra.
search-keyword-warning-bookmark = A palavra-chave escolhida já é usada por um favorito. Por favor, selecione outra.

## Containers Section

containers-back-link = « Voltar
containers-header = Abas contêiner
containers-add-button =
    .label = Adicionar novo contêiner
    .accesskey = A
containers-preferences-button =
    .label = Preferências
containers-remove-button =
    .label = Remover

## Sync Section - Signed out

sync-signedout-caption = Leve a web com você
sync-signedout-description = Sincronizar seus favoritos, histórico, abas, senhas, extensões e preferências entre os seus dispositivos.
sync-signedout-account-title = Conectar com uma { -fxaccount-brand-name }
sync-signedout-account-create = Não tem uma conta? Inscreva-se agora
    .accesskey = C
sync-signedout-account-signin =
    .label = Entrar…
    .accesskey = I
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Baixar o Firefox para <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> ou <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a> para sincronizar com o seu dispositivo móvel.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = Alterar imagem do perfil
sync-disconnect =
    .label = Desconectar…
    .accesskey = D
sync-manage-account = Gerenciar conta
    .accesskey = o
sync-signedin-unverified = { $email } não foi verificado.
sync-signedin-login-failure = Entre para reconectar { $email }
sync-resend-verification =
    .label = Reenviar verificação
    .accesskey = r
sync-remove-account =
    .label = Remover conta
    .accesskey = R
sync-sign-in =
    .label = Entrar
    .accesskey = t
sync-signedin-settings-header = Configurações do Sync
sync-signedin-settings-desc = Escolha o que sincronizar em seus dispositivos usando o { -brand-short-name }.
sync-engine-bookmarks =
    .label = Favoritos
    .accesskey = F
sync-engine-history =
    .label = Histórico
    .accesskey = H
sync-engine-tabs =
    .label = Abas abertas
    .tooltiptext = Uma lista do que está aberto em todos os dispositivos sincronizados
    .accesskey = A
sync-engine-logins =
    .label = Contas de acesso
    .tooltiptext = Nomes de usuário e senhas que você salvou
    .accesskey = L
sync-engine-addresses =
    .label = Endereços
    .tooltiptext = Endereços postais que você salvou (computador apenas)
    .accesskey = e
sync-engine-creditcards =
    .label = Cartões de crédito
    .tooltiptext = Nomes, números e datas de expiração (computador apenas)
    .accesskey = C
sync-engine-addons =
    .label = Extensões
    .tooltiptext = Extensões e temas para o Firefox no computador
    .accesskey = x
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] Opções
           *[other] Preferências
        }
    .tooltiptext = Configurações gerais, de privacidade e de segurança que você alterou
    .accesskey = P
sync-device-name-header = Nome do dispositivo
sync-device-name-change =
    .label = Alterar nome do dispositivo…
    .accesskey = h
sync-device-name-cancel =
    .label = Cancelar
    .accesskey = n
sync-device-name-save =
    .label = Salvar
    .accesskey = v
sync-mobilepromo-single = Conectar outro dispositivo
sync-mobilepromo-multi = Gerenciar dispositivos
sync-tos-link = Termos de serviço
sync-fxa-privacy-notice = Nota sobre privacidade

## Privacy Section

privacy-header = Privacidade do navegador

## Privacy Section - Forms

forms-header = Formulários e Senhas
forms-ask-to-save-logins =
    .label = Perguntar para contas de acesso e senhas de sites
    .accesskey = r
forms-exceptions =
    .label = Exceções…
    .accesskey = x
forms-saved-logins =
    .label = Contas de acesso salvas…
    .accesskey = l
forms-master-pw-use =
    .label = Usar uma senha mestra
    .accesskey = U
forms-master-pw-change =
    .label = Alterar senha mestra…
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
    .accesskey = w
history-remember-option-all =
    .label = Memorizar tudo
history-remember-option-never =
    .label = Nunca memorizar
history-remember-option-custom =
    .label = Usar minhas configurações
history-remember-description = { -brand-short-name } irá lembrar dos seus dados de navegação, download, formulário e histórico de pesquisa.
history-dontremember-description = O { -brand-short-name } usará as mesmas configurações da navegação privativa e não memorizará o histórico de navegação.
history-private-browsing-permanent =
    .label = Sempre usar o modo de navegação privativa
    .accesskey = v
history-remember-option =
    .label = Memorizar histórico de navegação e downloads
    .accesskey = M
history-remember-browser-option =
    .label = Memorizar histórico de navegação e downloads
    .accesskey = h
history-remember-search-option =
    .label = Memorizar histórico de pesquisa e formulários
    .accesskey = z
history-clear-on-close-option =
    .label = Limpar histórico quando o { -brand-short-name } fechar
    .accesskey = i
history-clear-on-close-settings =
    .label = Configurar…
    .accesskey = C
history-clear-button =
    .label = Limpar histórico…
    .accesskey = h

## Privacy Section - Site Data

sitedata-header = Cookies e Dados de sites
sitedata-total-size-calculating = Calculando o tamanho dos dados de sites e cache…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = Seus cookies, dados de sites e cache armazenados estão atualmente usando { $value } { $unit } do espaço em disco.
sitedata-learn-more = Saiba mais
sitedata-accept-cookies-option =
    .label = Aceitar cookies e dados de sites (recomendado)
    .accesskey = A
sitedata-block-cookies-option =
    .label = Bloquear cookies e dados de sites (pode quebrar páginas)
    .accesskey = B
sitedata-keep-until = Manter até
    .accesskey = m
sitedata-keep-until-expire =
    .label = Expirarem
sitedata-keep-until-closed =
    .label = { -brand-short-name } ser fechado
sitedata-accept-third-party-desc = Aceitar cookies e dados de site de terceiros
    .accesskey = t
sitedata-accept-third-party-always-option =
    .label = Sempre
sitedata-accept-third-party-visited-option =
    .label = De sites visitados
sitedata-accept-third-party-never-option =
    .label = Nunca
sitedata-clear =
    .label = Limpar dados…
    .accesskey = L
sitedata-settings =
    .label = Gerenciar dados…
    .accesskey = G
sitedata-cookies-exceptions =
    .label = Exceções…
    .accesskey = E

## Privacy Section - Address Bar

addressbar-header = Barra de Endereços
addressbar-suggest = Ao usar a barra de endereços, sugerir
addressbar-locbar-history-option =
    .label = Histórico de navegação
    .accesskey = H
addressbar-locbar-bookmarks-option =
    .label = Favoritos
    .accesskey = F
addressbar-locbar-openpage-option =
    .label = Abas abertas
    .accesskey = A
addressbar-suggestions-settings = Alterar preferências das sugestões do mecanismo de pesquisa

## Privacy Section - Tracking

tracking-header = Proteção contra rastreamento
tracking-desc = A proteção contra rastreamento bloqueia rastreadores on-line que coletam seus dados de navegação através de múltiplos sites. <a data-l10n-name="learn-more">Saiba mais sobre a proteção contra rastreamento e sua privacidade</a>
tracking-mode-label = Use a proteção contra rastreamento para bloquear rastreadores conhecidos
tracking-mode-always =
    .label = Sempre
    .accesskey = y
tracking-mode-private =
    .label = Só em janelas privativas
    .accesskey = l
tracking-mode-never =
    .label = Nunca
    .accesskey = n
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = Usar a proteção contra rastreamento na navegação privativa para bloquear rastreadores conhecidos
    .accesskey = v
tracking-exceptions =
    .label = Exceções…
    .accesskey = x
tracking-change-block-list =
    .label = Alterar Lista de Bloqueios…
    .accesskey = B

## Privacy Section - Permissions

permissions-header = Permissões
permissions-location = Localização
permissions-location-settings =
    .label = Configurações…
    .accesskey = C
permissions-camera = Câmera
permissions-camera-settings =
    .label = Configurações…
    .accesskey = c
permissions-microphone = Microfone
permissions-microphone-settings =
    .label = Configurações…
    .accesskey = m
permissions-notification = Notificações
permissions-notification-settings =
    .label = Configurações…
    .accesskey = n
permissions-notification-link = Saiba mais
permissions-notification-pause =
    .label = Pausar as notificações até reiniciar o { -brand-short-name }
    .accesskey = n
permissions-block-autoplay-media =
    .label = Bloquear reprodução automática de mídia com som em sites
    .accesskey = B
permissions-block-autoplay-media-exceptions =
    .label = Exceções…
    .accesskey = E
permissions-block-popups =
    .label = Bloquear janelas popup
    .accesskey = B
permissions-block-popups-exceptions =
    .label = Exceções…
    .accesskey = E
permissions-addon-install-warning =
    .label = Avisar quando sites tentarem instalar extensões
    .accesskey = A
permissions-addon-exceptions =
    .label = Exceções…
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = Impedir que os serviços de acessibilidade acessem o seu navegador
    .accesskey = a
permissions-a11y-privacy-link = Saiba mais

## Privacy Section - Data Collection

collection-header = Coleta e Uso de Dados pelo { -brand-short-name }
collection-description = Nós nos esforçamos para lhe proporcionar escolhas e coletar somente o que precisamos para fornecer e melhorar o { -brand-short-name } para todos. Nós sempre pedimos permissão antes de receber informações pessoais.
collection-privacy-notice = Nota sobre privacidade
collection-health-report =
    .label = Permitir que o { -brand-short-name } envie dados técnicos e de interação para a { -vendor-short-name }
    .accesskey = r
collection-health-report-link = Saiba mais
collection-studies =
    .label = Permitir que o { -brand-short-name } instale e execute pesquisas
collection-studies-link = Ver pesquisas do { -brand-short-name }
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = O relatório de dados está desabilitado nesta configuração
collection-browser-errors =
    .label = Permitir que o { -brand-short-name } envie relatórios de erro do navegador (incluindo mensagens de erro) para a { -vendor-short-name }
    .accesskey = b
collection-browser-errors-link = Saiba mais
collection-backlogged-crash-reports =
    .label = Permitir ao { -brand-short-name } enviar relatórios de travamento em seu nome
    .accesskey = e
collection-backlogged-crash-reports-link = Saiba mais

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = Segurança
security-browsing-protection = Conteúdo Enganoso e Proteção Contra Software Perigoso
security-enable-safe-browsing =
    .label = Bloquear conteúdo perigoso e malicioso
    .accesskey = B
security-enable-safe-browsing-link = Saiba mais
security-block-downloads =
    .label = Bloquear downloads perigosos
    .accesskey = d
security-block-uncommon-software =
    .label = Avisar sobre softwares indesejados e incomuns
    .accesskey = c

## Privacy Section - Certificates

certs-header = Certificados
certs-personal-label = Quando um servidor solicita seu certificado pessoal
certs-select-auto-option =
    .label = Selecionar um automaticamente
    .accesskey = S
certs-select-ask-option =
    .label = Perguntar todas as vezes
    .accesskey = a
certs-enable-ocsp =
    .label = Consultar servidores OCSP para confirmar a validade atual dos certificados
    .accesskey = o
certs-view =
    .label = Ver Certificados…
    .accesskey = C
certs-devices =
    .label = Dispositivos de Segurança…
    .accesskey = D
