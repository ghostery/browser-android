# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

about-logins-page-title = Contas e senhas

# "Google Play" and "App Store" are both branding and should not be translated

login-app-promo-title = Tenha suas senhas em qualquer lugar
login-app-promo-subtitle = Instale o aplicativo gratuito { -lockwise-brand-name }
login-app-promo-android =
    .alt = Instale a partir do Google Play
login-app-promo-apple =
    .alt = Baixe no App Store
login-filter =
    .placeholder = Pesquisar contas
create-login-button = Criar nova conta
# This string is used as alternative text for favicon images.
# Variables:
#   $title (String) - The title of the website associated with the favicon.
login-favicon =
    .alt = Favicon de { $title }
fxaccounts-sign-in-text = Tenha suas senhas em seus outros dispositivos
fxaccounts-sign-in-button = Entre no { -sync-brand-short-name }
fxaccounts-avatar-button =
    .title = Gerenciar conta

## The ⋯ menu that is in the top corner of the page

menu =
    .title = Abrir menu
# This menuitem is only visible on Windows
menu-menuitem-import = Importar senhas…
menu-menuitem-preferences =
    { PLATFORM() ->
        [windows] Opções
       *[other] Preferências
    }
menu-menuitem-feedback = Enviar opinião
menu-menuitem-faq = Perguntas frequentes
menu-menuitem-android-app = { -lockwise-brand-short-name } para Android
menu-menuitem-iphone-app = { -lockwise-brand-short-name } para iPhone e iPad

## Login List

login-list =
    .aria-label = Contas que combinar com a consulta
login-list-count =
    { $count ->
        [one] { $count } conta
       *[other] { $count } contas
    }
login-list-sort-label-text = Ordenar por:
login-list-name-option = Nome (A-Z)
login-list-breached-option = Sites vazados
login-list-last-changed-option = Última modificação
login-list-last-used-option = Último uso
login-list-intro-title = Nenhuma conta encontrada
login-list-intro-description = Quando você salva uma senha no { -brand-product-name }, ela aparece aqui.
login-list-item-title-new-login = Nova conta
login-list-item-subtitle-new-login = Informe as credenciais da sua conta
login-list-item-subtitle-missing-username = (sem nome de usuário)

## Introduction screen

login-intro-heading = Procurando suas contas salvas? Configure o { -sync-brand-short-name }.
login-intro-description = Se você salvou suas contas no { -brand-product-name } em outro dispositivo, veja como tê-las aqui:
login-intro-instruction-fxa = Crie ou entre na sua { -fxaccount-brand-name } no dispositivo onde suas contas estão salvas
login-intro-instruction-fxa-settings = Selecione a opção 'Contas de acesso' nas configurações do { -sync-brand-short-name }
login-intro-instruction-faq = Visite as <a data-l10n-name="faq">perguntas frequentes</a> do { -lockwise-brand-short-name } para mais ajuda

## Login

login-item-new-login-title = Criar nova conta
login-item-edit-button = Editar
login-item-delete-button = Excluir
login-item-origin-label = Endereço do site
login-item-origin =
    .placeholder = https://www.example.com
login-item-open-site-button = Iniciar
login-item-username-label = Nome de usuário
login-item-username =
    .placeholder = nome@example.com
login-item-copy-username-button-text = Copiar
login-item-copied-username-button-text = Copiado!
login-item-password-label = Senha
login-item-password-reveal-checkbox-show =
    .title = Mostrar senha
login-item-password-reveal-checkbox-hide =
    .title = Ocultar senha
login-item-copy-password-button-text = Copiar
login-item-copied-password-button-text = Copiado!
login-item-save-changes-button = Salvar alterações
login-item-save-new-button = Salvar
login-item-cancel-button = Cancelar
login-item-time-changed = Última modificação: { DATETIME($timeChanged, day: "numeric", month: "long", year: "numeric") }
login-item-time-created = Criado em: { DATETIME($timeCreated, day: "numeric", month: "long", year: "numeric") }
login-item-time-used = Último uso: { DATETIME($timeUsed, day: "numeric", month: "long", year: "numeric") }

## Master Password notification

master-password-notification-message = Digite sua senha mestra para ver contas e senhas salvas
master-password-reload-button =
    .label = Entrar
    .accesskey = E

## Dialogs

confirmation-dialog-cancel-button = Cancelar
confirmation-dialog-dismiss-button =
    .title = Cancelar
enable-password-sync-notification-message =
    { PLATFORM() ->
        [windows] Quer ter suas contas em todo lugar onde usa o { -brand-product-name }? Vá nas opções do { -sync-brand-short-name } e selecione a opção Contas de acesso.
       *[other] Quer ter suas contas em todo lugar onde usa o { -brand-product-name }? Vá nas preferências do { -sync-brand-short-name } e selecione a opção Contas de acesso.
    }
enable-password-sync-preferences-button =
    .label =
        { PLATFORM() ->
            [windows] Visite a opções do { -sync-brand-short-name }
           *[other] Visite as preferências do { -sync-brand-short-name }
        }
    .accesskey = V
confirm-delete-dialog-title = Excluir esta conta?
confirm-delete-dialog-message = Esta ação não pode ser desfeita.
confirm-delete-dialog-confirm-button = Excluir
confirm-discard-changes-dialog-title = Descartar alterações não salvas?
confirm-discard-changes-dialog-message = Todas as alterações não salvas serão perdidas.
confirm-discard-changes-dialog-confirm-button = Descartar

## Breach Alert notification

breach-alert-text = Senhas foram vazadas ou roubadas deste site desde a última vez que você atualizou seus detalhes de acesso. Mude a senha para proteger sua conta.
breach-alert-link = Saiba mais sobre este vazamento.
breach-alert-dismiss =
    .title = Fechar este alerta
