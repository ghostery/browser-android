# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

addons-window =
    .title = Gerenciador de extensões
search-header =
    .placeholder = Pesquisar em addons.mozilla.org
    .searchbuttonlabel = Pesquisar
search-header-shortcut =
    .key = f
loading-label =
    .value = Carregando…
list-empty-installed =
    .value = Não há nenhuma extensão deste tipo instalada
list-empty-available-updates =
    .value = Nenhuma atualização encontrada.
list-empty-recent-updates =
    .value = Você não atualizou recentemente nenhuma extensão
list-empty-find-updates =
    .label = Verificar se há atualizações
list-empty-button =
    .label = Saiba mais sobre extensões
install-addon-from-file =
    .label = Instalar de um arquivo…
    .accesskey = I
help-button = Suporte para extensões
preferences =
    { PLATFORM() ->
        [windows] Opções do { -brand-short-name }
       *[other] Preferências do { -brand-short-name }
    }
tools-menu =
    .tooltiptext = Ferramentas para todas as extensões
show-unsigned-extensions-button =
    .label = Algumas extensões não puderam ser verificadas
show-all-extensions-button =
    .label = Mostrar todas as extensões
debug-addons =
    .label = Depurar extensões
    .accesskey = x
cmd-show-details =
    .label = Mostrar mais informações
    .accesskey = E
cmd-find-updates =
    .label = Procurar atualizações
    .accesskey = P
cmd-preferences =
    .label =
        { PLATFORM() ->
            [windows] Opções
           *[other] Preferências
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
cmd-enable-theme =
    .label = Aplicar tema
    .accesskey = A
cmd-disable-theme =
    .label = Stop Wearing Theme
    .accesskey = W
cmd-install-addon =
    .label = Instalar
    .accesskey = I
cmd-contribute =
    .label = Contribuir
    .accesskey = C
    .tooltiptext = Contribuir para o desenvolvimento desta extensão
discover-title = O que são extensões?
discover-description =
    Extensões são aplicativos que personalizam o { -brand-short-name } com
    funcionalidades ou estilos extras. Experimente um painel que otimiza seu tempo, um notificador sobre o clima ou um tema para tornar seu { -brand-short-name }
    único.
discover-footer =
    Quando estiver conectado à internet, este painel apresenta algumas
    das melhores e mais populares extensões para você experimentar.
detail-version =
    .label = Versão
detail-last-updated =
    .label = Última atualização
detail-contributions-description = O autor solicita o seu apoio no desenvolvimento através de uma pequena contribuição.
detail-contributions-button = Contribua
    .title = Contribua com o desenvolvimento desta extensão
    .accesskey = C
detail-update-type =
    .value = Atualização automática
detail-update-default =
    .label = Padrão
    .tooltiptext = Instalar atualizações automaticamente somente se isto for o padrão
detail-update-automatic =
    .label = Ativada
    .tooltiptext = Instalar atualizações automaticamente
detail-update-manual =
    .label = Desativada
    .tooltiptext = Não instalar atualizações automaticamente
# Used as a description for the option to allow or block an add-on in private windows.
detail-private-browsing-label = Funcionar em janelas privativas
detail-private-browsing-description2 = Quando permitido, a extensão terá acesso à sua atividade online durante a navegação privativa. <label data-l10n-name="detail-private-browsing-learn-more">Saiba mais</label>
# Some add-ons may elect to not run in private windows by setting incognito: not_allowed in the manifest.  This
# cannot be overridden by the user.
detail-private-disallowed-label = Não permitido em janelas privativas
detail-private-disallowed-description = Esta extensão não funciona durante a navegação privativa. <label data-l10n-name="detail-private-browsing-learn-more">Saiba mais</label>
# Some special add-ons are privileged, run in private windows automatically, and this permission can't be revoked
detail-private-required-label = Requer acesso a janelas privativas
detail-private-required-description = Esta extensão tem acesso a suas atividades online durante a navegação privativa. <label data-l10n-name="detail-private-browsing-learn-more">Saiba mais</label>
detail-private-browsing-on =
    .label = Permitir
    .tooltiptext = Ativar na navegação privativa
detail-private-browsing-off =
    .label = Não permitir
    .tooltiptext = Desativar na navegação privativa
detail-home =
    .label = Site
detail-home-value =
    .value = { detail-home.label }
detail-repository =
    .label = Perfil da extensão
detail-repository-value =
    .value = { detail-repository.label }
detail-check-for-updates =
    .label = Verificar se há atualizações
    .accesskey = V
    .tooltiptext = Verificar se há atualizações para esta extensão
detail-show-preferences =
    .label =
        { PLATFORM() ->
            [windows] Opções
           *[other] Preferências
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
    .tooltiptext =
        { PLATFORM() ->
            [windows] Alterar as opções desta extensão
           *[other] Alterar as preferências desta extensão
        }
detail-rating =
    .value = Classificação
addon-restart-now =
    .label = Reiniciar agora
disabled-unsigned-heading =
    .value = Algumas extensões foram desativadas
disabled-unsigned-description = As seguintes extensões não foram verificadas para uso no { -brand-short-name }. Você pode <label data-l10n-name="find-addons">procurar substitutos</label> ou solicitar que o desenvolvedor peça a sua verificação.
disabled-unsigned-learn-more = Saiba mais sobre nossos esforços para ajudar a manter os usuários seguros.
disabled-unsigned-devinfo = Desenvolvedores interessados em ter suas extensões verificadas devem ler nosso <label data-l10n-name="learn-more">manual</label>.
plugin-deprecation-description = Faltando alguma coisa? Alguns plugins não são mais suportados pelo { -brand-short-name }. <label data-l10n-name="learn-more">Saiba mais.</label>
legacy-warning-show-legacy = Mostrar extensões legadas
legacy-extensions =
    .value = Extensões legadas
legacy-extensions-description = Estas extensões não cumprem os padrões atuais do { -brand-short-name }, então foram desativadas. <label data-l10n-name="legacy-learn-more">Saiba mais sobre mudanças em extensões</label>
private-browsing-description2 =
    O { -brand-short-name } está mudando o modo como extensões funcionam na navegação privativa. Qualquer nova extensão que você adicionar ao
    { -brand-short-name } por padrão não funcionará em janelas privativas. A menos que você permita nas configurações, a
    extensão não funcionará durante a navegação privativa e não terá acesso à sua atividade online.
    Fizemos esta alteração para assegurar sua navegação privativa.
    <label data-l10n-name="private-browsing-learn-more">Saiba como gerenciar configurações de extensões.</label>
extensions-view-discopane =
    .name = Recomendações
    .tooltiptext = { extensions-view-discopane.name }
extensions-view-recent-updates =
    .name = Atualizações recentes
    .tooltiptext = { extensions-view-recent-updates.name }
extensions-view-available-updates =
    .name = Atualizações disponíveis
    .tooltiptext = { extensions-view-available-updates.name }

## These are global warnings

extensions-warning-safe-mode-label =
    .value = Todos as extensões foram desativadas pelo modo de segurança.
extensions-warning-safe-mode-container =
    .tooltiptext = { extensions-warning-safe-mode-label.value }
extensions-warning-check-compatibility-label =
    .value = A verificação de compatibilidade de extensões está desativada. Você pode ter extensões incompatíveis.
extensions-warning-check-compatibility-container =
    .tooltiptext = { extensions-warning-check-compatibility-label.value }
extensions-warning-check-compatibility-enable =
    .label = Ativar
    .tooltiptext = Ativar verificação de compatibilidade de extensões
extensions-warning-update-security-label =
    .value = A verificação de segurança de atualizações de extensões está desativada. Atualizações de extensões podem comprometer seu computador.
extensions-warning-update-security-container =
    .tooltiptext = { extensions-warning-update-security-label.value }
extensions-warning-update-security-enable =
    .label = Ativar
    .tooltiptext = Ativar verificação de segurança da atualização de extensões

## Strings connected to add-on updates

extensions-updates-check-for-updates =
    .label = Verificar se há atualizações
    .accesskey = V
extensions-updates-view-updates =
    .label = Ver atualizações recentes
    .accesskey = E

# This menu item is a checkbox that toggles the default global behavior for
# add-on update checking.

extensions-updates-update-addons-automatically =
    .label = Atualizar automaticamente
    .accesskey = A

## Specific add-ons can have custom update checking behaviors ("Manually",
## "Automatically", "Use default global behavior"). These menu items reset the
## update checking behavior for all add-ons to the default global behavior
## (which itself is either "Automatically" or "Manually", controlled by the
## extensions-updates-update-addons-automatically.label menu item).

extensions-updates-reset-updates-to-automatic =
    .label = Definir atualização automática de tudo
    .accesskey = D
extensions-updates-reset-updates-to-manual =
    .label = Definir atualização manual de tudo
    .accesskey = D

## Status messages displayed when updating add-ons

extensions-updates-updating =
    .value = Atualizando as extensões
extensions-updates-installed =
    .value = Suas extensões foram atualizadas.
extensions-updates-downloaded =
    .value = As atualizações de suas extensões foram baixadas.
extensions-updates-restart =
    .label = Reinicie agora para concluir a instalação
extensions-updates-none-found =
    .value = Nenhuma atualização encontrada
extensions-updates-manual-updates-found =
    .label = Ver atualizações disponíveis
extensions-updates-update-selected =
    .label = Instalar atualizações
    .tooltiptext = Instalar as atualizações disponíveis desta lista

## Extension shortcut management

manage-extensions-shortcuts =
    .label = Gerenciar atalhos de extensões
    .accesskey = h
shortcuts-no-addons = Você não tem nenhuma extensão ativada.
shortcuts-no-commands = As seguintes extensões não têm atalhos de teclado:
shortcuts-input =
    .placeholder = Digite um atalho
shortcuts-browserAction = Ativar extensão
shortcuts-pageAction = Ativar ação de página
shortcuts-sidebarAction = Exibir/ocultar o painel lateral
shortcuts-modifier-mac = Incluir Ctrl, Alt ou ⌘
shortcuts-modifier-other = Incluir Ctrl ou Alt
shortcuts-invalid = Combinação inválida
shortcuts-letter = Digite uma letra
shortcuts-system = Não é possível substituir um atalho do { -brand-short-name }
# String displayed in warning label when there is a duplicate shortcut
shortcuts-duplicate = Atalho duplicado
# String displayed when a keyboard shortcut is already assigned to more than one add-on
# Variables:
#   $shortcut (string) - Shortcut string for the add-on
shortcuts-duplicate-warning-message = { $shortcut } está sendo usado como atalho em mais de um caso. Atalhos duplicados podem causar comportamento inesperado.
# String displayed when a keyboard shortcut is already used by another add-on
# Variables:
#   $addon (string) - Name of the add-on
shortcuts-exists = Já em uso por { $addon }
shortcuts-card-expand-button =
    { $numberToShow ->
        [one] Mostrar mais { $numberToShow }
       *[other] Mostrar mais { $numberToShow }
    }
shortcuts-card-collapse-button = Mostrar menos
go-back-button =
    .tooltiptext = Voltar

## Recommended add-ons page

# Explanatory introduction to the list of recommended add-ons. The action word
# ("recommends") in the final sentence is a link to external documentation.
discopane-intro =
    Extensões e temas são como aplicativos para seu navegador. Eles permitem
    proteger senhas, baixar vídeos, encontrar ofertas, bloquear anúncios chatos,
    mudar a aparência do navegador e muito mais. Esses pequenos programas são
    geralmente desenvolvidos por terceiros. Aqui está uma seleção que o { -brand-product-name }
    <a data-l10n-name="learn-more-trigger">recomenda</a> por suas excepcionais características de segurança, desempenho e funcionalidade.
# Notice to make user aware that the recommendations are personalized.
discopane-notice-recommendations =
    Algumas dessas recomendações são personalizadas. Elas são feitas com base 
    em outras extensões que você instalou, preferências de perfil e estatísticas de uso.
discopane-notice-learn-more = Saiba mais
privacy-policy = Política de privacidade
# Refers to the author of an add-on, shown below the name of the add-on.
# Variables:
#   $author (string) - The name of the add-on developer.
created-by-author = por <a data-l10n-name="author">{ $author }</a>
# Shows the number of daily users of the add-on.
# Variables:
#   $dailyUsers (number) - The number of daily users.
user-count = Usuários: { $dailyUsers }
install-extension-button = Adicionar ao { -brand-product-name }
install-theme-button = Instalar tema
# The label of the button that appears after installing an add-on. Upon click,
# the detailed add-on view is opened, from where the add-on can be managed.
manage-addon-button = Gerenciar
find-more-addons = Encontrar mais extensões

## Add-on actions

report-addon-button = Denunciar
remove-addon-button = Remover
disable-addon-button = Desativar
enable-addon-button = Ativar
expand-addon-button = Mais opções
preferences-addon-button =
    { PLATFORM() ->
        [windows] Opções
       *[other] Preferências
    }
details-addon-button = Detalhes
release-notes-addon-button = Notas da versão
permissions-addon-button = Permissões
addons-enabled-heading = Ativado
addons-disabled-heading = Desativado
extension-enabled-heading = Ativado
extension-disabled-heading = Desativado
theme-enabled-heading = Ativado
theme-disabled-heading = Desativado
plugin-enabled-heading = Ativado
plugin-disabled-heading = Desativado
dictionary-enabled-heading = Ativado
dictionary-disabled-heading = Desativado
locale-enabled-heading = Ativado
locale-disabled-heading = Desativado
ask-to-activate-button = Perguntar para ativar
always-activate-button = Sempre ativar
never-activate-button = Nunca ativar
addon-detail-author-label = Autor
addon-detail-version-label = Versão
addon-detail-last-updated-label = Última atualização
addon-detail-homepage-label = Página web
addon-detail-rating-label = Classificação
# The average rating that the add-on has received.
# Variables:
#   $rating (number) - A number between 0 and 5. The translation should show at most one digit after the comma.
five-star-rating =
    .title = Avaliação: { NUMBER($rating, maximumFractionDigits: 1) } de 5
# This string is used to show that an add-on is disabled.
# Variables:
#   $name (string) - The name of the add-on
addon-name-disabled = { $name } (desativado)
# The number of reviews that an add-on has received on AMO.
# Variables:
#   $numberOfReviews (number) - The number of reviews received
addon-detail-reviews-link =
    { $numberOfReviews ->
        [one] { $numberOfReviews } avaliação
       *[other] { $numberOfReviews } avaliações
    }

## Pending uninstall message bar

# Variables:
#   $addon (string) - Name of the add-on
pending-uninstall-description = <span data-l10n-name="addon-name">{ $addon }</span> foi removido.
pending-uninstall-undo-button = Desfazer
addon-detail-updates-label = Atualização automática
addon-detail-updates-radio-default = Padrão
addon-detail-updates-radio-on = Ativada
addon-detail-updates-radio-off = Desativada
addon-detail-update-check-label = Procurar atualização
install-update-button = Atualizar
# This is the tooltip text for the private browsing badge in about:addons. The
# badge is the private browsing icon included next to the extension's name.
addon-badge-private-browsing-allowed =
    .title = Permitida em janelas privativas
addon-detail-private-browsing-help = Com permissão, a extensão tem acesso à sua atividade online durante a navegação privativa. <a data-l10n-name="learn-more">Saiba mais</a>
addon-detail-private-browsing-allow = Permitir
addon-detail-private-browsing-disallow = Não permitir
# This is the tooltip text for the recommended badge for an extension in about:addons. The
# badge is a small icon displayed next to an extension when it is recommended on AMO.
addon-badge-recommended =
    .title = Recomendado
    .alt = Recomendado
available-updates-heading = Atualizações disponíveis
recent-updates-heading = Atualizações recentes
release-notes-loading = Carregando…
release-notes-error = Desculpe, houve um erro ao carregar as notas de lançamento.
addon-permissions-empty = Esta extensão não exige nenhum permissão
recommended-extensions-heading = Extensões recomendadas
recommended-themes-heading = Temas recomendados
# A recommendation for the Firefox Color theme shown at the bottom of the theme
# list view. The "Firefox Color" name itself should not be translated.
recommended-theme-1 = Sentindo criatividade? <a data-l10n-name="link">Crie seu próprio tema com o Firefox Color.</a>
