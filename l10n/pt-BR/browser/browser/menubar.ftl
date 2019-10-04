# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## File Menu

menu-file =
    .label = Arquivo
    .accesskey = A
menu-file-new-tab =
    .label = Nova aba
    .accesskey = N
menu-file-new-container-tab =
    .label = Nova aba contêiner
    .accesskey = c
menu-file-new-window =
    .label = Nova janela
    .accesskey = j
menu-file-new-private-window =
    .label = Nova janela privativa
    .accesskey = p
# "Open Location" is only displayed on macOS, and only on windows
# that aren't main browser windows, or when there are no windows
# but Firefox is still running.
menu-file-open-location =
    .label = Abrir endereço…
menu-file-open-file =
    .label = Abrir arquivo…
    .accesskey = A
menu-file-close =
    .label = Fechar
    .accesskey = F
menu-file-close-window =
    .label = Fechar janela
    .accesskey = h
menu-file-save-page =
    .label = Salvar página como…
    .accesskey = v
menu-file-email-link =
    .label = Enviar link por e-mail…
    .accesskey = E
menu-file-print-setup =
    .label = Configurar página…
    .accesskey = C
menu-file-print-preview =
    .label = Visualizar impressão
    .accesskey = z
menu-file-print =
    .label = Imprimir…
    .accesskey = I
menu-file-import-from-another-browser =
    .label = Importar de outro navegador…
    .accesskey = I
menu-file-go-offline =
    .label = Trabalhar desconectado
    .accesskey = o

## Edit Menu

menu-edit =
    .label = Editar
    .accesskey = E
menu-edit-undo =
    .label = Desfazer
    .accesskey = D
menu-edit-redo =
    .label = Refazer
    .accesskey = R
menu-edit-cut =
    .label = Cortar
    .accesskey = t
menu-edit-copy =
    .label = Copiar
    .accesskey = C
menu-edit-paste =
    .label = Colar
    .accesskey = o
menu-edit-delete =
    .label = Excluir
    .accesskey = x
menu-edit-select-all =
    .label = Selecionar tudo
    .accesskey = S
menu-edit-find-on =
    .label = Procurar nesta página…
    .accesskey = n
menu-edit-find-again =
    .label = Procurar próximo
    .accesskey = x
menu-edit-bidi-switch-text-direction =
    .label = Alterar direção do texto
    .accesskey = A

## View Menu

menu-view =
    .label = Exibir
    .accesskey = x
menu-view-toolbars-menu =
    .label = Barras de ferramentas
    .accesskey = B
menu-view-customize-toolbar =
    .label = Personalizar…
    .accesskey = P
menu-view-sidebar =
    .label = Painel
    .accesskey = a
menu-view-bookmarks =
    .label = Favoritos
menu-view-history-button =
    .label = Histórico
menu-view-synced-tabs-sidebar =
    .label = Abas sincronizadas
menu-view-full-zoom =
    .label = Zoom
    .accesskey = Z
menu-view-full-zoom-enlarge =
    .label = Ampliar
    .accesskey = A
menu-view-full-zoom-reduce =
    .label = Reduzir
    .accesskey = R
menu-view-full-zoom-reset =
    .label = Redefinir
    .accesskey = R
menu-view-full-zoom-toggle =
    .label = Zoom só do texto
    .accesskey = S
menu-view-page-style-menu =
    .label = Estilo da página
    .accesskey = s
menu-view-page-style-no-style =
    .label = Sem estilo
    .accesskey = m
menu-view-page-basic-style =
    .label = Estilo de página básico
    .accesskey = b
menu-view-charset =
    .label = Codificação de texto
    .accesskey = c

## These should match what Safari and other Apple applications
## use on macOS.

menu-view-enter-full-screen =
    .label = Entrar em tela inteira
    .accesskey = t
menu-view-exit-full-screen =
    .label = Sair da tela inteira
    .accesskey = t
menu-view-full-screen =
    .label = Tela inteira
    .accesskey = T



menu-view-show-all-tabs =
    .label = Mostrar todas as abas
    .accesskey = t
menu-view-bidi-switch-page-direction =
    .label = Alterar direção da página
    .accesskey = e

## History Menu

menu-history =
    .label = Histórico
    .accesskey = H
menu-history-show-all-history =
    .label = Mostrar histórico
menu-history-clear-recent-history =
    .label = Limpar histórico recente…
menu-history-synced-tabs =
    .label = Abas sincronizadas
menu-history-restore-last-session =
    .label = Restaurar sessão anterior
menu-history-hidden-tabs =
    .label = Abas ocultas
menu-history-undo-menu =
    .label = Abas fechadas recentemente
menu-history-undo-window-menu =
    .label = Janelas fechadas recentemente

## Bookmarks Menu

menu-bookmarks-menu =
    .label = Favoritos
    .accesskey = v
menu-bookmarks-show-all =
    .label = Mostrar favoritos
menu-bookmarks-all-tabs =
    .label = Adicionar todas as abas…
menu-bookmarks-toolbar =
    .label = Barra de favoritos
menu-bookmarks-other =
    .label = Outros favoritos
menu-bookmarks-mobile =
    .label = Favoritos do celular

## Tools Menu

menu-tools =
    .label = Ferramentas
    .accesskey = F
menu-tools-downloads =
    .label = Downloads
    .accesskey = D
menu-tools-addons =
    .label = Extensões
    .accesskey = E
menu-tools-sync-sign-in =
    .label = Entrar no { -sync-brand-short-name }…
    .accesskey = y
menu-tools-sync-now =
    .label = Sincronizar agora
    .accesskey = n
menu-tools-sync-re-auth =
    .label = Reconectar-se ao { -sync-brand-short-name }…
    .accesskey = R
menu-tools-web-developer =
    .label = Web Developer
    .accesskey = W
menu-tools-page-source =
    .label = Código-fonte da página
    .accesskey = f
menu-tools-page-info =
    .label = Informações da página
    .accesskey = I
menu-preferences =
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
menu-tools-layout-debugger =
    .label = Depurador de layout
    .accesskey = L

## Window Menu

menu-window-menu =
    .label = Janela
menu-window-bring-all-to-front =
    .label = Trazer todas para frente

## Help Menu

menu-help =
    .label = Ajuda
    .accesskey = u
menu-help-product =
    .label = Ajuda do { -brand-shorter-name }
    .accesskey = u
menu-help-show-tour =
    .label = Tutorial do { -brand-shorter-name }
    .accesskey = o
menu-help-keyboard-shortcuts =
    .label = Atalhos de teclado
    .accesskey = h
menu-help-troubleshooting-info =
    .label = Informações para resolver problemas
    .accesskey = I
menu-help-feedback-page =
    .label = Enviar opinião…
    .accesskey = n
menu-help-safe-mode-without-addons =
    .label = Reiniciar com extensões desativadas…
    .accesskey = R
menu-help-safe-mode-with-addons =
    .label = Reiniciar com extensões ativadas
    .accesskey = R
# Label of the Help menu item. Either this or
# safeb.palm.notdeceptive.label from
# phishing-afterload-warning-message.dtd is shown.
menu-help-report-deceptive-site =
    .label = Denunciar site enganoso…
    .accesskey = r
