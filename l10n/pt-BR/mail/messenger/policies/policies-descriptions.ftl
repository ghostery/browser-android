# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Thunderbird installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-3rdparty = Definir políticas que WebExtensions podem acessar via chrome.storage.managed.
policy-AppUpdateURL = Definir URL personalizada de atualização do aplicativo.
policy-Authentication = Configurar autenticação integrada em sites que a suportam.
policy-BlockAboutAddons = Bloquear acesso ao gerenciador de extensões (about:addons).
policy-BlockAboutConfig = Bloquear acesso à página about:config.
policy-BlockAboutProfiles = Bloquear acesso à página about:profiles.
policy-BlockAboutSupport = Bloquear acesso à página about:support.
policy-CaptivePortal = Ativar ou desativar suporte a portal cativo
policy-CertificatesDescription = Adicionar certificados ou usar certificados integrados.
policy-Cookies = Permitir ou negar que sites criem cookies.
policy-DefaultDownloadDirectory = Definir o diretório padrão de download.
policy-DisableAppUpdate = Impedir que o { -brand-short-name } seja atualizado.
policy-DisableDeveloperTools = Bloquear acesso às ferramentas do desenvolvedor.
policy-DisableFeedbackCommands = Desativar comandos de enviar opinião a partir do menu de ajuda (Enviar opinião e Denunciar site enganoso).
policy-DisableForgetButton = Impedir acesso ao botão Esquecer.
policy-DisableMasterPasswordCreation = Se for true, não pode ser criada uma senha mestra.
policy-DisableProfileImport = Desativar o comando no menu de importar dados de outra aplicação.
policy-DisableSafeMode = Desativar o recurso de reiniciar em Modo de Segurança. Nota: a tecla Shift para entrar em Modo de Segurança só pode ser desativada no Windows usando Política de Grupo.
policy-DisableSecurityBypass = Impedir que o usuário ignore certos avisos de segurança.
policy-DisableSystemAddonUpdate = Impedir que o { -brand-short-name } instale e atualize extensões do sistema.
policy-DisableTelemetry = Desativar a Telemetria.
policy-DisplayMenuBar = Mostrar a barra de menu por padrão.
policy-DNSOverHTTPS = Configurar DNS sobre HTTPS.
policy-DontCheckDefaultClient = Desativar verificação de cliente padrão ao iniciar.
policy-DownloadDirectory = Definir e bloquear o diretório de download.
# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = Ativar ou desativar o bloqueio de conteúdo e opcionalmente o bloquear.
# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs.
policy-Extensions = Instalar, desinstalar ou bloquear extensões. A opção 'Install' recebe URLs ou paths como parâmetros. As opções 'Uninstall' e 'Locked' recebem IDs de extensões.
policy-ExtensionSettings = Gerencia todos os aspectos de instalação de extensões.
policy-ExtensionUpdate = Ativar ou desativar atualizações automáticas de extensões.
policy-HardwareAcceleration = Se for false, desativar aceleração de hardware.
policy-InstallAddonsPermission = Permitir que certos sites instalem extensões.
policy-LocalFileLinks = Permitir que sites específicos tenham link para arquivos locais.
policy-NetworkPrediction = Ativar ou desativar predição de rede (DNS prefetching).
policy-OfferToSaveLogins = Impor as configurações para permitir que o { -brand-short-name } ofereça memorizar contas de acesso e senhas salvas. Tanto "true" como "false" são valores aceitos.
policy-OverrideFirstRunPage = Substituir a página de primeira execução. Defina esta política como vazio se quiser desativar a página de primeira execução.
policy-OverridePostUpdatePage = Substituir a página “Novidades” exibida após uma atualização. Defina esta política como vazio se quiser desativar a exibição de uma página após atualizações.
policy-Preferences = Definir e bloquear o valor de um subconjunto de preferências.
policy-PromptForDownloadLocation = Perguntar onde salvar arquivos ao baixar.
policy-Proxy = Configurar definições de proxy.
policy-RequestedLocales = Definir a lista de idiomas solicitados para a aplicação por ordem de preferência.
policy-SanitizeOnShutdown2 = Limpar dados de navegação ao sair.
policy-SearchEngines = Definir configurações de mecanismos de pesquisa. Esta política está disponível somente na versão Extended Support Release (ESR).
# For more information, see https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/PKCS11/Module_Installation
policy-SecurityDevices = Instalar módulos PKCS #11.
policy-SSLVersionMax = Definir a versão máxima de SSL.
policy-SSLVersionMin = Definir a versão mínima de SSL.
policy-SupportMenu = Adicionar um item de menu personalizado de suporte ao menu de ajuda.
# “format” refers to the format used for the value of this policy.
policy-WebsiteFilter = Bloquear a visita a sites. Veja a documentação para mais detalhes sobre o formato.
