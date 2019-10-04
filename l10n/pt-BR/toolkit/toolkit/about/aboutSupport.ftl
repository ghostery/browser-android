# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

page-title = Dados para suporte
page-subtitle =
    Esta página contém informações técnicas que podem ser úteis se você estiver
    tentando solucionar um problema. Se estiver procurando respostas para as dúvidas mais comuns
    do { -brand-short-name }, confira o <a data-l10n-name="support-link">site de suporte</a>.
crashes-title = Relatórios de travamentos
crashes-id = ID do relatório
crashes-send-date = Envio
crashes-all-reports = Todos os relatórios de travamentos
crashes-no-config = Este aplicativo não foi configurado para exibir relatórios de travamento.
extensions-title = Extensões
extensions-name = Nome
extensions-enabled = Ativado
extensions-version = Versão
extensions-id = ID
security-software-title = Software de segurança
security-software-type = Tipo
security-software-name = Nome
security-software-antivirus = Antivírus
security-software-antispyware = Antispyware
security-software-firewall = Firewall
features-title = Funcionalidades do { -brand-short-name }
features-name = Nome
features-version = Versão
features-id = ID
processes-title = Processos remotos
processes-type = Tipo
processes-count = Quantidade
app-basics-title = Informações básicas sobre o aplicativo
app-basics-name = Nome
app-basics-version = Versão
app-basics-build-id = ID da compilação
app-basics-update-channel = Canal de atualização
# This message refers to the folder used to store updates on the device,
# as in "Folder for updates". "Update" is a noun, not a verb.
app-basics-update-dir =
    { PLATFORM() ->
        [linux] Diretório de atualizações
       *[other] Pasta de atualizações
    }
app-basics-update-history = Histórico de atualizações
app-basics-show-update-history = Mostrar histórico de atualizações
# Represents the path to the binary used to start the application.
app-basics-binary = Binário da aplicação
app-basics-profile-dir =
    { PLATFORM() ->
        [linux] Diretório do perfil
       *[other] Pasta do perfil
    }
app-basics-enabled-plugins = Plugins ativados
app-basics-build-config = Configuração da compilação
app-basics-user-agent = User Agent
app-basics-os = OS
app-basics-memory-use = Uso de memória
app-basics-performance = Desempenho
app-basics-service-workers = Service Workers registrados
app-basics-profiles = Perfis
app-basics-launcher-process-status = Processo de lançamento
app-basics-multi-process-support = Janelas multiprocessadas
app-basics-remote-processes-count = Processos remotos
app-basics-enterprise-policies = Políticas empresariais
app-basics-location-service-key-google = Chave do Serviço de Localização do Google
app-basics-safebrowsing-key-google = Chave do Google Safebrowsing
app-basics-key-mozilla = Chave do serviço de localização da Mozilla
app-basics-safe-mode = Modo de segurança
show-dir-label =
    { PLATFORM() ->
        [macos] Mostrar no Finder
        [windows] Abrir pasta
       *[other] Abrir pasta
    }
modified-key-prefs-title = Preferências importantes modificadas
modified-prefs-name = Nome
modified-prefs-value = Valor
user-js-title = Preferências do user.js
user-js-description = Sua pasta do perfil contém um <a data-l10n-name="user-js-link">arquivo user.js</a>, que inclui preferências que não foram criadas pelo { -brand-short-name }.
locked-key-prefs-title = Preferências importantes bloqueadas
locked-prefs-name = Nome
locked-prefs-value = Valor
graphics-title = Gráficos
graphics-features-title = Funcionalidades
graphics-diagnostics-title = Diagnósticos
graphics-failure-log-title = Registro de falhas
graphics-gpu1-title = GPU #1
graphics-gpu2-title = GPU #2
graphics-decision-log-title = Registro de decisões
graphics-crash-guards-title = Recursos desativados da proteção contra travamentos
graphics-workarounds-title = Soluções alternativas
# Windowing system in use on Linux (e.g. X11, Wayland).
graphics-window-protocol = Protocolo de janelas
place-database-title = Base de dados de lugares
place-database-integrity = Integridade
place-database-verify-integrity = Verificar integridade
js-title = JavaScript
js-incremental-gc = GC incremental
a11y-title = Acessibilidade
a11y-activated = Ativado
a11y-force-disabled = Bloquear acessibilidade
a11y-handler-used = Identificador Acessível Utilizado
a11y-instantiator = Instanciador de Acessibilidade
library-version-title = Versões de bibliotecas
copy-text-to-clipboard-label = Copiar como texto
copy-raw-data-to-clipboard-label = Copiar como estrutura de dados
sandbox-title = Isolamento
sandbox-sys-call-log-title = Chamadas do sistema rejeitadas
sandbox-sys-call-index = #
sandbox-sys-call-age = Segundos atrás
sandbox-sys-call-pid = PID
sandbox-sys-call-tid = TID
sandbox-sys-call-proc-type = Tipo de processo
sandbox-sys-call-number = Chamadas de sistema
sandbox-sys-call-args = Argumentos
safe-mode-title = Experimentar o modo de segurança
restart-in-safe-mode-label = Reiniciar com extensões desativadas…
media-title = Mídia
media-output-devices-title = Dispositivos de Saída
media-input-devices-title = Dispositivos de Entrada
media-device-name = Nome
media-device-group = Grupo
media-device-vendor = Fabricante
media-device-state = Estado
media-device-preferred = Preferido
media-device-format = Formato
media-device-channels = Canais
media-device-rate = Taxa
media-device-latency = Latência
intl-title = Internacionalização & Localização
intl-app-title = Configurações do aplicativo
intl-locales-requested = Idiomas solicitados
intl-locales-available = Idiomas disponíveis
intl-locales-supported = Idiomas do aplicativo
intl-locales-default = Idioma padrão
intl-os-title = Sistema operacional
intl-os-prefs-system-locales = Idiomas do sistema
intl-regional-prefs = Preferências regionais
# Variables
# $days (Integer) - Number of days of crashes to log
report-crash-for-days =
    { $days ->
        [one] Relatórios de travamentos de { $days } dia
       *[other] Relatórios de travamentos dos últimos { $days } dias
    }
# Variables
# $minutes (integer) - Number of minutes since crash
crashes-time-minutes =
    { $minutes ->
        [one] Há { $minutes } minuto
       *[other] Há { $minutes } minutos
    }
# Variables
# $hours (integer) - Number of hours since crash
crashes-time-hours =
    { $hours ->
        [one] Há { $hours } hora
       *[other] Há { $hours } horas
    }
# Variables
# $days (integer) - Number of days since crash
crashes-time-days =
    { $days ->
        [one] Há { $days } dia
       *[other] Há { $days } dias
    }
# Variables
# $reports (integer) - Number of pending reports
pending-reports =
    { $reports ->
        [one] Todos os relatórios de travamentos (incluindo { $reports } travamento pendente na faixa de tempo especificada)
       *[other] Todos os relatórios de travamentos (incluindo { $reports } travamentos pendentes na faixa de tempo especificada)
    }
raw-data-copied = Dados copiados para a área de transferência
text-copied = Texto copiado para a área de transferência

## The verb "blocked" here refers to a graphics feature such as "Direct2D" or "OpenGL layers".

blocked-driver = Bloqueado para a versão do seu driver gráfico.
blocked-gfx-card = Bloqueado para sua placa gráfica devido a problemas de driver não resolvidos.
blocked-os-version = Bloqueado para a versão do seu sistema operacional.
blocked-mismatched-version = Bloqueado para a sua versão incompatível do driver de video no registro e DLL
# Variables
# $driverVersion - The graphics driver version string
try-newer-driver = Bloqueado para a versão do seu driver gráfico. Tentar atualizá-lo para, pelo menos, a versão { $driverVersion }.
# "ClearType" is a proper noun and should not be translated. Feel free to leave English strings if
# there are no good translations, these are only used in about:support
clear-type-parameters = Parâmetros ClearType
compositing = Composição
hardware-h264 = Decodificação H264 por hardware
main-thread-no-omtc = thread principal, sem OMTC
yes = Sim
no = Não
unknown = Desconhecido
virtual-monitor-disp = Exibição do monitor virtual

## The following strings indicate if an API key has been found.
## In some development versions, it's expected for some API keys that they are
## not found.

found = Encontrado
missing = Faltando
gpu-process-pid = GPUProcessPid
gpu-process = GPUProcess
gpu-description = Descrição
gpu-vendor-id = ID do fornecedor
gpu-device-id = ID do dispositivo
gpu-subsys-id = ID do subsistema
gpu-drivers = Drivers
gpu-ram = RAM
gpu-driver-vendor = Fornecedor do driver
gpu-driver-version = Versão do driver
gpu-driver-date = Data do driver
gpu-active = Ativo
webgl1-wsiinfo = Info WSI do driver WebGL 1
webgl1-renderer = Renderizador do driver WebGL 1
webgl1-version = Versão do driver WebGL 1
webgl1-driver-extensions = Extensões do driver WebGL 1
webgl1-extensions = Extensões WebGL 1
webgl2-wsiinfo = Info WSI do driver WebGL 2
webgl2-renderer = Renderizador do driver WebGL 2
webgl2-version = Versão do driver WebGL 2
webgl2-driver-extensions = Extensões do driver WebGL 2
webgl2-extensions = Extensões WebGL 2
blocklisted-bug = Bloqueado devido a problemas conhecidos
# Variables
# $bugNumber (string) - String of bug number from Bugzilla
bug-link = bug { $bugNumber }
# Variables
# $failureCode (string) - String that can be searched in the source tree.
unknown-failure = Bloqueado; código de erro { $failureCode }
d3d11layers-crash-guard = Compositor D3D11
d3d11video-crash-guard = Decodificador de vídeo D3D11
d3d9video-crash-buard = Decodificador de vídeo D3D9
glcontext-crash-guard = OpenGL
reset-on-next-restart = Redefinir na próxima reinicialização
gpu-process-kill-button = Finalizar processo GPU
gpu-device-reset = Redefinir dispositivo
gpu-device-reset-button = Ativar a Redefinição de Dispositivo
uses-tiling = Usa mosaicos
content-uses-tiling = Usa mosaicos (conteúdo)
off-main-thread-paint-enabled = Ativado o desenho fora do processo principal
off-main-thread-paint-worker-count = Contagem de desenho fora do thread principal
target-frame-rate = Alvo de taxa de atualização
audio-backend = Infraestrutura de Áudio
max-audio-channels = Máximo de Canais
channel-layout = Formato de Canal Preferido
sample-rate = Taxa de amostragem preferida
min-lib-versions = Versão mínima esperada
loaded-lib-versions = Versão em uso
has-seccomp-bpf = Seccomp-BPF (Sistema de filtragem de chamadas)
has-seccomp-tsync = Sincronização do thread Seccomp
has-user-namespaces = Espaço de nomes do usuário
has-privileged-user-namespaces = Espaço de nomes do usuário para processos privilegiados
can-sandbox-content = Isolamento de processamento de conteúdo
can-sandbox-media = Isolamento de plugins de mídia
content-sandbox-level = Nível de isolamento de processamento de conteúdo
effective-content-sandbox-level = Nível efetivo de isolamento de processamento de conteúdo
sandbox-proc-type-content = conteúdo
sandbox-proc-type-file = conteúdo do arquivo
sandbox-proc-type-media-plugin = plugin de mídia
sandbox-proc-type-data-decoder = decodificador de dados
launcher-process-status-0 = Ativado
launcher-process-status-1 = Desativado devido a falha
launcher-process-status-2 = Desativado à força
launcher-process-status-unknown = Status desconhecido
# Variables
# $remoteWindows (integer) - Number of remote windows
# $totalWindows (integer) - Number of total windows
multi-process-windows = { $remoteWindows }/{ $totalWindows }
multi-process-status-0 = Ativado pelo usuário
multi-process-status-1 = Ativado por padrão
multi-process-status-2 = Desativado
multi-process-status-4 = Desativado por ferramentas de acessibilidade
multi-process-status-6 = Desativado por entrada de texto não suportada
multi-process-status-7 = Desativado por extensões
multi-process-status-8 = Desativado à força
multi-process-status-unknown = Status desconhecido
async-pan-zoom = Deslocamento/Zoom assíncrono
apz-none = nenhum
wheel-enabled = entrada com roda do mouse ativada
touch-enabled = entrada touch ativada
drag-enabled = arrasto da barra de rolagem ativado
keyboard-enabled = teclado ativado
autoscroll-enabled = rolagem automática ativada

## Variables
## $preferenceKey (string) - String ID of preference

wheel-warning = entrada com roda do mouse assíncrona desativada devido a preferência não suportada: { $preferenceKey }
touch-warning = entrada touch assíncrona desativada devido a preferência não suportada: { $preferenceKey }

## Strings representing the status of the Enterprise Policies engine.

policies-inactive = Inativo
policies-active = Ativo
policies-error = Erro
