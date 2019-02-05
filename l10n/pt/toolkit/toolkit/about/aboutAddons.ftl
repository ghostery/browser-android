# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

addons-window =
    .title = Gerenciador de extensões
search-header =
    .placeholder = Pesquisar addons.mozilla.org
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
help-button = Suporte para Extensões
preferences =
    { PLATFORM() ->
        [windows] Opções do { -brand-short-name }
       *[other] Preferências do { -brand-short-name }
    }
tools-menu =
    .tooltiptext = Ferramentas para todos os complementos
show-unsigned-extensions-button =
    .label = Algumas extensões não puderam ser verificadas
show-all-extensions-button =
    .label = Mostrar todas as extensões
debug-addons =
    .label = Depurar extensões
    .accesskey = B
cmd-show-details =
    .label = Mostrar mais informações
    .accesskey = E
cmd-find-updates =
    .label = Localizar atualizações
    .accesskey = V
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
    Quando estiver conectado à internet, este painel apresentará alguns
      dos melhores e mais populares complementos para você experimentar.
detail-version =
    .label = Versão
detail-last-updated =
    .label = Última atualização
detail-contributions-description = O autor solicita o seu apoio no desenvolvimento através de uma pequena contribuição.
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
    .value = Algumas extensões foram desabilitadas
disabled-unsigned-description = As seguintes extensões não foram verificadas para uso no { -brand-short-name }. Você pode <label data-l10n-name="find-addons">localizar substitutos</label> ou solicitar que o desenvolvedor peça a sua verificação.
disabled-unsigned-learn-more = Saiba mais sobre nossos esforços para ajudar a manter os usuários seguros.
disabled-unsigned-devinfo = Os desenvolvedores interessados que seus complementos sejam verificados, devem ler o nosso <label data-l10n-name="learn-more">manual</label>.
plugin-deprecation-description = Faltando alguma coisa? Alguns plugins não são mais suportados pelo { -brand-short-name }. <label data-l10n-name="learn-more">Saiba mais.</label>
legacy-warning-show-legacy = Mostrar extensões legadas
legacy-extensions =
    .value = Extensões legadas
legacy-extensions-description = Estas extensões não cumprem os padrões atuais do { -brand-short-name }, então eles foram desativados. <label data-l10n-name="legacy-learn-more">Saiba mais sobre as alterações das extensões</label>
extensions-view-discover =
    .name = Adicionar
    .tooltiptext = { extensions-view-discover.name }
extensions-view-recent-updates =
    .name = Atualizações recentes
    .tooltiptext = { extensions-view-recent-updates.name }
extensions-view-available-updates =
    .name = Atualizações disponíveis
    .tooltiptext = { extensions-view-available-updates.name }

## These are global warnings

extensions-warning-safe-mode-label =
    .value = Todos as extensões foram desabilitadas pelo modo seguro.
extensions-warning-safe-mode-container =
    .tooltiptext = { extensions-warning-safe-mode-label.value }
extensions-warning-check-compatibility-label =
    .value = A verificação de compatibilidade está desabilitada. Pode ter complementos incompatíveis.
extensions-warning-check-compatibility-container =
    .tooltiptext = { extensions-warning-check-compatibility-label.value }
extensions-warning-check-compatibility-enable =
    .label = Ativar
    .tooltiptext = Ativar verificação de compatibilidade de extensões
extensions-warning-update-security-label =
    .value = A verificação de segurança da conexão está desativada. Atualizações de complementos podem comprometer seu computador.
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
    .label = Definir atualização automática para todos
    .accesskey = D
extensions-updates-reset-updates-to-manual =
    .label = Definir atualização manual para todos
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
