# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Firefox installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-AppUpdateURL = Definir um URL personalizado de atualização da aplicação.
policy-Authentication = Configurar autenticação integrada para os sites que a suportem.
policy-BlockAboutAddons = Bloquear acesso ao Gestor de extras (about:addons).
policy-BlockAboutConfig = Bloquear acesso  à página about:config.
policy-BlockAboutProfiles = Bloquear acesso  à página about:profiles.
policy-BlockAboutSupport = Bloquear acesso  à página about:support.
policy-Bookmarks = Criar marcadores na barra de ferramentas de marcadores, menus de marcadores ou uma pasta especificada dentro dos mesmos.
policy-Certificates = Se devem ou não ser utilizadas credenciais integradas. De momento esta política é exclusiva do Windows.
policy-CertificatesDescription = Adicionar certificados ou utilizar certificados integrados.
policy-Cookies = Permitir ou negar que os sites definam cookies.
policy-DisableAppUpdate = Impedir o navegador de ser atualizado.
policy-DisableBuiltinPDFViewer = Desativar o PDF.js, o leitor de PDF incorporado no { -brand-short-name }.
policy-DisableDeveloperTools = Bloquear acesso às ferramentas de programador.
policy-DisableFeedbackCommands = Desativar comandos para enviar comentários a partir do menu de Ajuda (enviar feedback e reportar sites fraudulentos)
policy-DisableFirefoxAccounts = Desativar os serviços baseados na { -fxaccount-brand-name }, incluindo o Sync.
# Firefox Screenshots is the name of the feature, and should not be translated.
policy-DisableFirefoxScreenshots = Desativar a funcionalidade Firefox Screenshots.
policy-DisableFirefoxStudies = Impedir o { -brand-short-name } de executar estudos.
policy-DisableForgetButton = Impedir o acesso ao botão Esquecer.
policy-DisableFormHistory = Não guardar histórico de pesquisas ou de formulários.
policy-DisableMasterPasswordCreation = Se verdadeiro, não poderá ser criada uma palavra-passe mestra.
policy-DisablePocket = Desativar a funcionalidade de guardar páginas web no Pocket.
policy-DisablePrivateBrowsing = Desativar a Navegação privada.
policy-DisableProfileImport = Desativar o menu de comando para importar dados de outro navegador.
policy-DisableProfileRefresh = Desativar o botão Restaurar { -brand-short-name } na página about:support.
policy-DisableSafeMode = Desativar a funcionalidade de reiniciar no Modo de segurança. Nota: o botão Shift para entrar no Modo de segurança apenas pode ser desativado no Windows utilizando Política de grupo.
policy-DisableSecurityBypass = Impedir o utilizador de contornar certos avisos de segurança.
policy-DisableSetDesktopBackground = Desativar o comando de menu Definir imagem como fundo do ambiente de trabalho.
policy-DisableSetAsDesktopBackground = Desativar o comando de menu Definir como fundo do ambiente de trabalho para imagens.
policy-DisableSystemAddonUpdate = Impedir o navegador de instalar e atualizar extras de sistema.
policy-DisableTelemetry = Desligar a Telemetria.
policy-DisplayBookmarksToolbar = Mostrar a Barra ferramentas de marcadores por predefinição.
policy-DisplayMenuBar = Mostrar a Barra de menu por predefinição.
policy-DNSOverHTTPS = Configurar DNS por HTTPS.
policy-DontCheckDefaultBrowser = Desativar verificação por navegador predefinido no arranque.
# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = Ativar ou desativar o Bloqueio de conteúdo e bloqueá-lo opcionalmente.
# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#extensions-machine-only
policy-Extensions = Instalar, desinstalar ou bloquear extensões. A opção Instalar usa URLs ou caminhos como parâmetros. As opções Desinstalar e Bloquear usam IDs de extensões.
policy-FlashPlugin = Permitir ou negar a utilização do plugin Flash.
policy-HardwareAcceleration = Se falso, desligar aceleração de hardware.
# “lock” means that the user won’t be able to change this setting
policy-Homepage = Definir e bloquear opcionalmente a página inicial.
policy-InstallAddonsPermission = Permitir a certos websites instalar extras.
policy-NoDefaultBookmarks = Desativar a criação de marcadores predefinidos empacotados com o { -brand-short-name }, e os Marcadores inteligentes (Mais visitados, Etiquetas recentes). Nota: esta política é apenas eficaz se utilizada antes da primeira execução do perfil.
policy-OfferToSaveLogins = Forçar a definição para permitir o { -brand-short-name } oferecer para memorizar credenciais guardadas e palavras-passe. Ambos os valores true e false são aceites.
policy-OverrideFirstRunPage = Substituir a página de primeira execução. Defina esta política para blank se pretende desativar a página de primeira execução.
policy-OverridePostUpdatePage = Substituir a página "Novidades" pós-atualização. Defina esta política para blank se pretende desativar a página pós-atualização.
policy-Permissions = Configurar permissões para a câmara, microfone, localização e notificações.
policy-PopupBlocking = Permitir certos websites de mostrar popups por predefinição.
policy-Proxy = Configurar definições proxy.
policy-RequestedLocales = Definir a lista de idiomas solicitados para a aplicação por ordem de preferência.
policy-SanitizeOnShutdown = Limpar todos os dados de navegação ao desligar.
policy-SearchBar = Definir a localização predefinida da barra de pesquisa. O utilizador ainda tem permissão para a personalizar.
policy-SearchEngines = Configurar as definições de motor de pesquisa. Esta política é apenas disponível na versão Extended Support Release (ESR).
# For more information, see https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/PKCS11/Module_Installation
policy-SecurityDevices = Instalar módulos PKCS #11.
# “format” refers to the format used for the value of this policy. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#websitefilter-machine-only
policy-WebsiteFilter = Bloquear websites de serem visitados. Veja a documentação para mais detalhes sobre o formato.
