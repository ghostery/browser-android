# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### Firefox Home / New Tab strings for about:home / about:newtab.

newtab-page-title = Nova aba
newtab-settings-button =
    .title = Personalizar sua página de nova aba

## Search box component.

# "Search" is a verb/action
newtab-search-box-search-button =
    .title = Pesquisar
    .aria-label = Pesquisar
newtab-search-box-search-the-web-text = Pesquisar na web
newtab-search-box-search-the-web-input =
    .placeholder = Pesquisar na web
    .title = Pesquisar na web
    .aria-label = Pesquisar na web

## Top Sites - General form dialog.

newtab-topsites-add-search-engine-header = Adicionar mecanismo de pesquisa
newtab-topsites-add-topsites-header = Novo site popular
newtab-topsites-edit-topsites-header = Editar site popular
newtab-topsites-title-label = Título
newtab-topsites-title-input =
    .placeholder = Digite um título
newtab-topsites-url-label = URL
newtab-topsites-url-input =
    .placeholder = Digite ou cole uma URL
newtab-topsites-url-validation = É necessário uma URL válida
newtab-topsites-image-url-label = URL de imagem personalizada
newtab-topsites-use-image-link = Usar uma imagem personalizada…
newtab-topsites-image-validation = Não foi possível carregar a imagem. Tente uma URL diferente.

## Top Sites - General form dialog buttons. These are verbs/actions.

newtab-topsites-cancel-button = Cancelar
newtab-topsites-delete-history-button = Excluir do histórico
newtab-topsites-save-button = Salvar
newtab-topsites-preview-button = Visualizar
newtab-topsites-add-button = Adicionar

## Top Sites - Delete history confirmation dialog. 


## Top Sites - Delete history confirmation dialog.

newtab-confirm-delete-history-p1 = Você tem certeza que deseja excluir todas as instâncias desta página do seu histórico?
# "This action" refers to deleting a page from history.
newtab-confirm-delete-history-p2 = Essa ação não pode ser desfeita.

## Context Menu - Action Tooltips.

# General tooltip for context menus.
newtab-menu-section-tooltip =
    .title = Abrir menu
    .aria-label = Abrir menu
# This tooltip is for the context menu of Pocket cards or Topsites
# Variables:
#  $title (String): The label or hostname of the site. This is for screen readers when the context menu button is focused/active.
newtab-menu-content-tooltip =
    .title = Abrir menu
    .aria-label = Abrir menu de contexto para { $title }
# Tooltip on an empty topsite box to open the New Top Site dialog.
newtab-menu-topsites-placeholder-tooltip =
    .title = Editar este site
    .aria-label = Editar este site

## Context Menu: These strings are displayed in a context menu and are meant as a call to action for a given page.

newtab-menu-edit-topsites = Editar
newtab-menu-open-new-window = Abrir em uma nova janela
newtab-menu-open-new-private-window = Abrir em uma nova janela privativa
newtab-menu-dismiss = Dispensar
newtab-menu-pin = Fixar
newtab-menu-unpin = Desafixar
newtab-menu-delete-history = Excluir do histórico
newtab-menu-save-to-pocket = Salvar no { -pocket-brand-name }
newtab-menu-delete-pocket = Excluir do { -pocket-brand-name }
newtab-menu-archive-pocket = Arquivar no { -pocket-brand-name }
# Bookmark is a noun in this case, "Remove bookmark".
newtab-menu-remove-bookmark = Remover favorito
# Bookmark is a verb here.
newtab-menu-bookmark = Adicionar aos favoritos

## Context Menu - Downloaded Menu. "Download" in these cases is not a verb, 
## it is a noun. As in, "Copy the link that belongs to this downloaded item".


## Context Menu - Downloaded Menu. "Download" in these cases is not a verb,
## it is a noun. As in, "Copy the link that belongs to this downloaded item".

newtab-menu-copy-download-link = Copiar link do download
newtab-menu-go-to-download-page = Abrir página de download
newtab-menu-remove-download = Remover do histórico

## Context Menu - Download Menu: These are platform specific strings found in the context menu of an item that has
## been downloaded. The intention behind "this action" is that it will show where the downloaded file exists on the file
## system for each operating system.

newtab-menu-show-file =
    { PLATFORM() ->
        [macos] Mostrar no Finder
       *[other] Abrir pasta
    }
newtab-menu-open-file = Abrir arquivo

## Card Labels: These labels are associated to pages to give
## context on how the element is related to the user, e.g. type indicates that
## the page is bookmarked, or is currently open on another device.

newtab-label-visited = Visitado
newtab-label-bookmarked = Adicionado aos favoritos
newtab-label-removed-bookmark = Favorito removido
newtab-label-recommended = Em alta
newtab-label-saved = Salvo no { -pocket-brand-name }
newtab-label-download = Baixado

## Section Menu: These strings are displayed in the section context menu and are 
## meant as a call to action for the given section.


## Section Menu: These strings are displayed in the section context menu and are
## meant as a call to action for the given section.

newtab-section-menu-remove-section = Remover seção
newtab-section-menu-collapse-section = Recolher seção
newtab-section-menu-expand-section = Expandir seção
newtab-section-menu-manage-section = Gerenciar seção
newtab-section-menu-manage-webext = Gerenciar extensão
newtab-section-menu-add-topsite = Adicionar ao sites preferidos
newtab-section-menu-add-search-engine = Adicionar mecanismo de pesquisa
newtab-section-menu-move-up = Mover para cima
newtab-section-menu-move-down = Mover para baixo
newtab-section-menu-privacy-notice = Aviso de privacidade

## Section aria-labels

newtab-section-collapse-section-label =
    .aria-label = Recolher seção
newtab-section-expand-section-label =
    .aria-label = Expandir seção

## Section Headers.

newtab-section-header-topsites = Sites preferidos
newtab-section-header-highlights = Destaques
# Variables:
#  $provider (String): Name of the corresponding content provider.
newtab-section-header-pocket = Recomendado pelo { $provider }

## Empty Section States: These show when there are no more items in a section. Ex. When there are no more Pocket story recommendations, in the space where there would have been stories, this is shown instead.

newtab-empty-section-highlights = Comece a navegar e mostraremos aqui alguns ótimos artigos, vídeos e outras páginas que você visitou ou adicionou aos favoritos recentemente.
# Ex. When there are no more Pocket story recommendations, in the space where there would have been stories, this is shown instead.
# Variables:
#  $provider (String): Name of the content provider for this section, e.g "Pocket".
newtab-empty-section-topstories = Você já viu tudo. Volte mais tarde para mais histórias do { $provider }. Não consegue esperar? Escolha um assunto popular para encontrar mais grandes histórias através da web.

## Empty Section (Content Discovery Experience). These show when there are no more stories or when some stories fail to load.

newtab-discovery-empty-section-topstories-header = Você está em dia!
newtab-discovery-empty-section-topstories-content = Volte mais tarde para ver mais histórias.
newtab-discovery-empty-section-topstories-try-again-button = Tentar novamente
newtab-discovery-empty-section-topstories-loading = Carregando...
# Displays when a layout in a section took too long to fetch articles.
newtab-discovery-empty-section-topstories-timed-out = Opa! Nós quase carregamos esta seção, mas não completamente.

## Pocket Content Section.

# This is shown at the bottom of the trending stories section and precedes a list of links to popular topics.
newtab-pocket-read-more = Tópicos populares:
newtab-pocket-more-recommendations = Mais recomendações
newtab-pocket-how-it-works = Como funciona
newtab-pocket-cta-button = Adicionar o { -pocket-brand-name }
newtab-pocket-cta-text = Salve as histórias que você gosta no { -pocket-brand-name } e abasteça sua mente com leituras fascinantes.

## Error Fallback Content.
## This message and suggested action link are shown in each section of UI that fails to render.

newtab-error-fallback-info = Opa, algo deu errado ao carregar esse conteúdo.
newtab-error-fallback-refresh-link = Atualize a página para tentar novamente.
