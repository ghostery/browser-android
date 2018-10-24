# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = Enviar a los sitios web una señal “No rastrear” indicando que no quiere ser rastreado
do-not-track-learn-more = Más información
do-not-track-option-default =
    .label = Solo al usar protección contra rastreo
do-not-track-option-default-content-blocking =
    .label = Solo cuando { -brand-short-name } está configurado para bloquear los rastreadores detectados
do-not-track-option-always =
    .label = Siempre
pref-page =
    .title =
        { PLATFORM() ->
            [windows] Opciones
           *[other] Preferencias
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
            [windows] Encontrar en Opciones
           *[other] Encontrar en Preferencias
        }
policies-notice =
    { PLATFORM() ->
        [windows] Su organización ha deshabilitado la posibilidad de cambiar algunas opciones.
       *[other] Su organización ha deshabilitado la posibilidad de cambiar algunas preferencias.
    }
pane-general-title = General
category-general =
    .tooltiptext = { pane-general-title }
pane-home-title = Inicio
category-home =
    .tooltiptext = { pane-home-title }
pane-search-title = Buscar
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = Privacidad & Seguridad
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Cuenta Firefox
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = { -brand-short-name } Asistencia
focus-search =
    .key = f
close-button =
    .aria-label = Cerrar

## Browser Restart Dialog

feature-enable-requires-restart = { -brand-short-name } debe ser reiniciado para activar esta característica.
feature-disable-requires-restart = { -brand-short-name } debe ser reiniciado para activar esta característica.
should-restart-title = Reiniciar { -brand-short-name }
should-restart-ok = Reiniciar { -brand-short-name } ahora
cancel-no-restart-button = Cancelar
restart-later = Reiniciar más tarde

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
extension-controlled-homepage-override = Una extensión, <img data-l10n-name="icon"/> { $name }, controla su página de inicio.
# This string is shown to notify the user that their new tab page
# is being controlled by an extension.
extension-controlled-new-tab-url = Una extensión, <img data-l10n-name="icon"/> { $name }, controla su página de nueva pestaña.
# This string is shown to notify the user that the default search engine
# is being controlled by an extension.
extension-controlled-default-search = Una exensión, <img data-l10n-name="icon"/> { $name }, ha establecido su buscador predeterminado.
# This string is shown to notify the user that Container Tabs
# are being enabled by an extension.
extension-controlled-privacy-containers = Una extensión, <img data-l10n-name="icon"/> { $name }, requiere pestañas de contenedores.
# This string is shown to notify the user that their tracking protection preferences
# are being controlled by an extension.
extension-controlled-websites-tracking-protection-mode = Una extensión, <img data-l10n-name="icon"/> { $name }, está controlando la protección contra rastreo.
# This string is shown to notify the user that their content blocking "All Detected Trackers"
# preferences are being controlled by an extension.
extension-controlled-websites-content-blocking-all-trackers = Una extensión, <img data-l10n-name="icon"/> { $name }, está controlando esta configuración.
# This string is shown to notify the user that their proxy configuration preferences
# are being controlled by an extension.
extension-controlled-proxy-config = Una extensión, <img data-l10n-name="icon"/> { $name }, controla cómo se conecta { -brand-short-name } a Internet.
# This string is shown after the user disables an extension to notify the user
# how to enable an extension that they disabled.
#
# <img data-l10n-name="addons-icon"/> will be replaced with Add-ons icon
# <img data-l10n-name="menu-icon"/> will be replaced with Menu icon
extension-controlled-enable = Para activar la extensión vaya a Complementos <img data-l10n-name="addons-icon"/> en el menú <img data-l10n-name="menu-icon"/>.

## Preferences UI Search Results

search-results-header = Resultados de la búsqueda
# `<span data-l10n-name="query"></span>` will be replaced by the search term.
search-results-empty-message =
    { PLATFORM() ->
        [windows] ¡Lo sentimos! No hay resultados para "<span data-l10n-name="query"></span>" en Opciones.
       *[other] ¡Lo sentimos! No hay resultados para "<span data-l10n-name="query"></span>" en Preferencias.
    }
search-results-help-link = ¿Necesita ayuda? Visite <a data-l10n-name="url">Ayuda de { -brand-short-name }</a>

## General Section

startup-header = Inicio
# { -brand-short-name } will be 'Firefox Developer Edition',
# since this setting is only exposed in Firefox Developer Edition
separate-profile-mode =
    .label = Permitir a { -brand-short-name } y Firefox ejecutarse al mismo tiempo
use-firefox-sync = Consejo: esto usa perfiles separados. Use { -sync-brand-short-name } para compartir datos entre ellos.
get-started-not-logged-in = Conectarse a { -sync-brand-short-name }…
get-started-configured = Abrir preferencias de { -sync-brand-short-name }
always-check-default =
    .label = Comprobar siempre si { -brand-short-name } es su navegador predeterminado
    .accesskey = o
is-default = { -brand-short-name } es su navegador predeterminado
is-not-default = { -brand-short-name } no es su navegador predet.
set-as-my-default-browser =
    .label = Convertir en predeterminado…
    .accesskey = C
startup-restore-previous-session =
    .label = Restaurar sesión previa
    .accesskey = s
disable-extension =
    .label = Desactivar extensión
tabs-group-header = Pestañas
ctrl-tab-recently-used-order =
    .label = Ctrl+Tab pasa por las pestañas en orden de uso reciente
    .accesskey = T
open-new-link-as-tabs =
    .label = Abrir enlaces en pestañas en lugar de en ventanas nuevas
    .accesskey = v
warn-on-close-multiple-tabs =
    .label = Advertirle al cerrar múltiples pestañas
    .accesskey = A
warn-on-open-many-tabs =
    .label = Advertirle cuando al abrir múltiples pestañas se pueda ralentizar { -brand-short-name }
    .accesskey = v
switch-links-to-new-tabs =
    .label = Cuando abra un enlace en una pestaña nueva, cambiar inmediatamente a ella
    .accesskey = C
show-tabs-in-taskbar =
    .label = Mostrar miniaturas de las pestañas en la barra de tareas de Windows
    .accesskey = ñ
browser-containers-enabled =
    .label = Activar pestañas contenedoras
    .accesskey = ñ
browser-containers-learn-more = Saber más
browser-containers-settings =
    .label = Configuración…
    .accesskey = C
containers-disable-alert-title = ¿Cerrar todas las pestañas contenedoras?
containers-disable-alert-desc =
    { $tabCount ->
        [one] Si desactiva las pestañas contenedores ahora, se cerrará { $tabCount } pestaña contenedora. ¿Está seguro de que quiere desactivar las pestañas contenedoras?
       *[other] Si desactiva las pestañas contenedores ahora, se cerrarán { $tabCount } pestañas contenedoras. ¿Está seguro de que quiere desactivar las pestañas contenedoras?
    }
containers-disable-alert-ok-button =
    { $tabCount ->
        [one] Cerrar { $tabCount } pestaña contenedora
       *[other] Cerrar { $tabCount } pestañas contenedoras
    }
containers-disable-alert-cancel-button = Mantener activadas
containers-remove-alert-title = ¿Eliminar este contenedor?
# Variables:
#   $count (Number) - Number of tabs that will be closed.
containers-remove-alert-msg =
    { $count ->
        [one] Si elimina este contenedor ahora, se cerrará { $count } pestaña contenedora. ¿Está seguro de que quiere eliminar este contenedor?
       *[other] Si elimina este contenedor ahora, se cerrarán { $count } pestañas contenedoras. ¿Está seguro de que quiere eliminar este contenedor?
    }
containers-remove-ok-button = Eliminar este contenedor
containers-remove-cancel-button = No eliminar este contenedor

## General Section - Language & Appearance

language-and-appearance-header = Idioma y apariencia
fonts-and-colors-header = Tipografías y colores
default-font = Tipografía predeterminada
    .accesskey = T
default-font-size = Tamaño
    .accesskey = m
advanced-fonts =
    .label = Avanzadas…
    .accesskey = A
colors-settings =
    .label = Colores…
    .accesskey = C
language-header = Idioma
choose-language-description = Elegir el idioma preferido para mostrar las páginas web
choose-button =
    .label = Seleccionar…
    .accesskey = o
choose-browser-language-description = Seleccione los idiomas en los que se mostrarán los menús, mensajes y notificaciones de { -brand-short-name }.
manage-browser-languages-button =
    .label = Establecer alternativas…
    .accesskey = l
confirm-browser-language-change-description = Reinicie { -brand-short-name } para aplicar los cambios
confirm-browser-language-change-button = Aplicar y reiniciar
translate-web-pages =
    .label = Traducir contenido web
    .accesskey = d
# The <img> element is replaced by the logo of the provider
# used to provide machine translations for web pages.
translate-attribution = Traducciones de <img data-l10n-name="logo"/>
translate-exceptions =
    .label = Excepciones…
    .accesskey = x
check-user-spelling =
    .label = Revisar la ortografía según escribe
    .accesskey = v

## General Section - Files and Applications

files-and-applications-title = Archivos y aplicaciones
download-header = Descargas
download-save-to =
    .label = Guardar archivos en
    .accesskey = G
download-choose-folder =
    .label =
        { PLATFORM() ->
            [macos] Elegir…
           *[other] Examinar…
        }
    .accesskey =
        { PLATFORM() ->
            [macos] e
           *[other] x
        }
download-always-ask-where =
    .label = Preguntar siempre dónde guardar los archivos
    .accesskey = s
applications-header = Aplicaciones
applications-description = Elija cómo gestiona { -brand-short-name } los archivos que usted descarga de la web o las aplicaciones que usa mientras navega.
applications-filter =
    .placeholder = Buscar tipos de archivo o aplicaciones
applications-type-column =
    .label = Tipo de contenido
    .accesskey = T
applications-action-column =
    .label = Acción
    .accesskey = A
drm-content-header = Contenido sujeto a administración de derechos de autor (DRM)
play-drm-content =
    .label = Reproducir contenido controlado por DRM
    .accesskey = R
play-drm-content-learn-more = Saber más
update-application-title = Actualizaciones de { -brand-short-name }
update-application-description = Mantenga { -brand-short-name } actualizado para un rendimiento, estabilidad y seguridad óptimos.
update-application-version = Versión { $version } <a data-l10n-name="learn-more">Novedades</a>
update-history =
    .label = Mostrar historial de actualizaciones…
    .accesskey = M
update-application-allow-description = Permitir a { -brand-short-name }
update-application-auto =
    .label = Instalar actualizaciones automáticamente (recomendado)
    .accesskey = I
update-application-check-choose =
    .label = Buscar actualizaciones, pero permitirle elegir si instalarlas
    .accesskey = B
update-application-manual =
    .label = No buscar actualizaciones (no recomendado)
    .accesskey = N
update-application-use-service =
    .label = Usar un servicio en segundo plano para instalar actualizaciones
    .accesskey = p
update-enable-search-update =
    .label = Actualizar automáticamente los buscadores
    .accesskey = A

## General Section - Performance

performance-title = Rendimiento
performance-use-recommended-settings-checkbox =
    .label = Usar configuración de rendimiento recomendada
    .accesskey = U
performance-use-recommended-settings-desc = Esta configuración está ajustada al hardware y sistema operativo de su equipo.
performance-settings-learn-more = Saber más
performance-allow-hw-accel =
    .label = Usar aceleración de hardware cuando esté disponible
    .accesskey = r
performance-limit-content-process-option = Límite de procesos de contenido
    .accesskey = L
performance-limit-content-process-enabled-desc = Más procesos de contenido pueden mejorar el rendimiento al usar múltiples pestañas, pero también usarán más memoria.
performance-limit-content-process-blocked-desc = Modificar el número de procesos de contenido solo es posible con { -brand-short-name } multiproceso. <a data-l10n-name="learn-more">Sepa cómo comprobar si el multiproceso está activado</a>
# Variables:
#   $num - default value of the `dom.ipc.processCount` pref.
performance-default-content-process-count =
    .label = { $num } (predeterminado)

## General Section - Browsing

browsing-title = Navegación
browsing-use-autoscroll =
    .label = Usar desplazamiento automático
    .accesskey = a
browsing-use-smooth-scrolling =
    .label = Usar desplazamiento suave
    .accesskey = v
browsing-use-onscreen-keyboard =
    .label = Mostrar un teclado táctil cuando sea necesario
    .accesskey = s
browsing-use-cursor-navigation =
    .label = Usar siempre las teclas del cursor para navegar dentro de las páginas
    .accesskey = c
browsing-search-on-start-typing =
    .label = Buscar texto cuando comience a escribir
    .accesskey = x

## General Section - Proxy

network-proxy-title = Proxy de red
network-settings-title = Configuración de red
network-proxy-connection-description = Configurar cómo se conecta { -brand-short-name } a Internet.
network-proxy-connection-learn-more = Saber más
network-proxy-connection-settings =
    .label = Configuración…
    .accesskey = o

## Home Section

home-new-windows-tabs-header = Nuevas ventanas y pestañas
home-new-windows-tabs-description2 = Elige lo que ves cuando abres tu página de inicio, nuevas ventanas y nuevas pestañas.

## Home Section - Home Page Customization

home-homepage-mode-label = Página de inicio y ventanas nuevas
home-newtabs-mode-label = Nuevas pestañas
home-restore-defaults =
    .label = Restaurar ajustes predeterminados
    .accesskey = R
# "Firefox" should be treated as a brand and kept in English,
# while "Home" and "(Default)" can be localized.
home-mode-choice-default =
    .label = Inicio de Firefox (predeterminado)
home-mode-choice-custom =
    .label = URLs personalizadas...
home-mode-choice-blank =
    .label = Página en blanco
home-homepage-custom-url =
    .placeholder = Pegar URL...
# This string has a special case for '1' and [other] (default). If necessary for
# your language, you can add {$tabCount} to your translations and use the
# standard CLDR forms, or only use the form for [other] if both strings should
# be identical.
use-current-pages =
    .label =
        { $tabCount ->
            [1] Usar página actual
           *[other] Usar páginas actuales
        }
    .accesskey = C
choose-bookmark =
    .label = Usar marcador…
    .accesskey = m

## Search Section

search-bar-header = Barra de búsqueda
search-bar-hidden =
    .label = Use la barra de direcciones para búsquedas y navegación
search-bar-shown =
    .label = Añadir barra de búsqueda en la barra de herramientas
search-engine-default-header = Buscador predeterminado
search-engine-default-desc = Elija el buscador por omisión que desea usar en las barras de direcciones y de búsqueda.
search-suggestions-option =
    .label = Proporcionar sugerencias de búsqueda
    .accesskey = P
search-show-suggestions-url-bar-option =
    .label = Mostrar sugerencias de búsqueda en los resultados de la barra de direcciones
    .accesskey = M
# This string describes what the user will observe when the system
# prioritizes search suggestions over browsing history in the results
# that extend down from the address bar. In the original English string,
# "ahead" refers to location (appearing most proximate to), not time
# (appearing before).
search-show-suggestions-above-history-option =
    .label = Mostrar sugerencias de búsqueda antes del historial de navegación en los resultados de la barra de direcciones
search-suggestions-cant-show = Las sugerencias de búsqueda no se mostrarán en los resultados de la barra de direcciones porque ha configurado { -brand-short-name } para que nunca recuerde el historial.
search-one-click-header = Buscadores con un clic
search-one-click-desc = Elija los buscadores alternativos que aparecen bajo las barras de direcciones y de búsqueda cuando comienza a escribir una palabra clave.
search-choose-engine-column =
    .label = Buscador
search-choose-keyword-column =
    .label = Palabra clave
search-restore-default =
    .label = Restaurar buscadores predeterminados
    .accesskey = R
search-remove-engine =
    .label = Eliminar
    .accesskey = E
search-find-more-link = Encontrar más buscadores
# This warning is displayed when the chosen keyword is already in use
# ('Duplicate' is an adjective)
search-keyword-warning-title = Palabra clave duplicada
# Variables:
#   $name (String) - Name of a search engine.
search-keyword-warning-engine = Ha seleccionado una palabra clave que está siendo usada por "{ $name }". Por favor, seleccione otra.
search-keyword-warning-bookmark = Ha seleccionado una palabra clave que está siendo usada por otro marcador. Por favor, seleccione otra.

## Containers Section

containers-back-link = « Volver atrás
containers-header = Pestañas contenedoras
containers-add-button =
    .label = Añadir nuevo contenedor
    .accesskey = A
containers-preferences-button =
    .label = Preferencias
containers-remove-button =
    .label = Eliminar

## Sync Section - Signed out

sync-signedout-caption = Llévese la web con usted
sync-signedout-description = Sincronice sus marcadores, historial, pestañas, contraseñas, complementos y preferencias en todos sus dispositivos.
sync-signedout-account-title = Conectar con una { -fxaccount-brand-name }
sync-signedout-account-create = ¿No tiene una cuenta? Comience ahora
    .accesskey = C
sync-signedout-account-signin =
    .label = Conectarse…
    .accesskey = C
# This message contains two links and two icon images.
#   `<img data-l10n-name="android-icon"/>` - Android logo icon
#   `<a data-l10n-name="android-link">` - Link to Android Download
#   `<img data-l10n-name="ios-icon">` - iOS logo icon
#   `<a data-l10n-name="ios-link">` - Link to iOS Download
#
# They can be moved within the sentence as needed to adapt
# to your language, but should not be changed or translated.
sync-mobile-promo = Descargar Firefox para <img data-l10n-name="android-icon"/> <a data-l10n-name="android-link">Android</a> o <img data-l10n-name="ios-icon"/> <a data-l10n-name="ios-link">iOS</a> para sincronizar con su dispositivo móvil.

## Sync Section - Signed in

sync-profile-picture =
    .tooltiptext = Cambiar imagen del perfil
sync-disconnect =
    .label = Desconectar…
    .accesskey = D
sync-manage-account = Administrar cuenta
    .accesskey = A
sync-signedin-unverified = { $email } no está verificado.
sync-signedin-login-failure = Inicie sesión para reconectar { $email }
sync-resend-verification =
    .label = Reenviar verificación
    .accesskey = v
sync-remove-account =
    .label = Eliminar cuenta
    .accesskey = E
sync-sign-in =
    .label = Conectarse
    .accesskey = n
sync-signedin-settings-header = Configuración de Sync
sync-signedin-settings-desc = Elija qué sincronizar en sus dispositivos usando { -brand-short-name }.
sync-engine-bookmarks =
    .label = Marcadores
    .accesskey = m
sync-engine-history =
    .label = Historial
    .accesskey = r
sync-engine-tabs =
    .label = Pestañas abiertas
    .tooltiptext = Una lista de lo que está abierto en todos los dispositivos sincronizados
    .accesskey = t
sync-engine-logins =
    .label = Inicios de sesión
    .tooltiptext = Nombres de usuario y contraseñas que ha guardado
    .accesskey = I
sync-engine-addresses =
    .label = Direcciones
    .tooltiptext = Direcciones postales que ha guardado (solo escritorio)
    .accesskey = D
sync-engine-creditcards =
    .label = Tarjetas de crédito
    .tooltiptext = Nombres, números y fechas de caducidad (solo escritorio)
    .accesskey = T
sync-engine-addons =
    .label = Complementos
    .tooltiptext = Extensiones y temas para Firefox de escritorio
    .accesskey = C
sync-engine-prefs =
    .label =
        { PLATFORM() ->
            [windows] Opciones
           *[other] Preferencias
        }
    .tooltiptext = Configuración general, de privacidad y de seguridad que ha cambiado
    .accesskey = S
sync-device-name-header = Nombre del dispositivo
sync-device-name-change =
    .label = Cambiar nombre del dispositivo…
    .accesskey = a
sync-device-name-cancel =
    .label = Cancelar
    .accesskey = e
sync-device-name-save =
    .label = Guardar
    .accesskey = u
sync-mobilepromo-single = Conectar otro dispositivo
sync-mobilepromo-multi = Administrar dispositivos
sync-tos-link = Términos del Servicio
sync-fxa-privacy-notice = Aviso sobre privacidad

## Privacy Section

privacy-header = Privacidad del navegador

## Privacy Section - Forms

forms-header = Formularios y contraseñas
forms-ask-to-save-logins =
    .label = Preguntar para guardar contraseñas e inicios de sesión de sitios web
    .accesskey = r
forms-exceptions =
    .label = Excepciones…
    .accesskey = x
forms-saved-logins =
    .label = Cuentas guardadas…
    .accesskey = C
forms-master-pw-use =
    .label = Usar una contraseña maestra
    .accesskey = s
forms-master-pw-change =
    .label = Cambiar contraseña maestra…
    .accesskey = M

## Privacy Section - History

history-header = Historial
# This label is followed, on the same line, by a dropdown list of options
# (Remember history, etc.).
# In English it visually creates a full sentence, e.g.
# "Firefox will" + "Remember history".
#
# If this doesn't work for your language, you can translate this message:
#   - Simply as "Firefox", moving the verb into each option.
#     This will result in "Firefox" + "Will remember history", etc.
#   - As a stand-alone message, for example "Firefox history settings:".
history-remember-label = { -brand-short-name } podrá
    .accesskey = o
history-remember-option-all =
    .label = Recordar el historial
history-remember-option-never =
    .label = No recordar el historial
history-remember-option-custom =
    .label = Usar una configuración personalizada para el historial
history-remember-description = { -brand-short-name } recordará su historial de navegación, descargas, formularios y búsqueda.
history-dontremember-description = { -brand-short-name } usará la misma configuración que en la navegación privada, y no recordará ningún dato de su historial mientras navega por la Web.
history-private-browsing-permanent =
    .label = Modo permanente de navegación privada
    .accesskey = n
history-remember-browser-option =
    .label = Recordar historial de navegación y descargas
    .accesskey = h
history-remember-search-option =
    .label = Recordar el historial de formularios y búsquedas
    .accesskey = f
history-clear-on-close-option =
    .label = Limpiar el historial cuando { -brand-short-name } se cierre
    .accesskey = h
history-clear-on-close-settings =
    .label = Configuración…
    .accesskey = g
history-clear-button =
    .label = Limpiar historial…
    .accesskey = s

## Privacy Section - Site Data

sitedata-header = Cookies y datos del sitio
sitedata-total-size-calculating = Calculando el tamaño de los datos del sitio y del caché…
# Variables:
#   $value (Number) - Value of the unit (for example: 4.6, 500)
#   $unit (String) - Name of the unit (for example: "bytes", "KB")
sitedata-total-size = Sus cookies, datos del sitio y caché almacenados ocupan actualmente un { $value } { $unit } del espacio en disco.
sitedata-learn-more = Saber más
sitedata-accept-cookies-option =
    .label = Aceptar cookies y datos de los sitios web (recomendado)
    .accesskey = A
sitedata-block-cookies-option =
    .label = Bloquear cookies y datos del sitio (puede que algunos sitios no funcionen)
    .accesskey = B
sitedata-keep-until = Mantener hasta que
    .accesskey = q
sitedata-keep-until-expire =
    .label = Caduquen
sitedata-keep-until-closed =
    .label = cierre { -brand-short-name }
sitedata-accept-third-party-desc = Aceptar cookies de terceros y datos del sitio
    .accesskey = y
sitedata-accept-third-party-always-option =
    .label = Siempre
sitedata-accept-third-party-visited-option =
    .label = De los visitados
sitedata-accept-third-party-never-option =
    .label = Nunca
sitedata-allow-cookies-option =
    .label = Aceptar cookies y datos del sitio
    .accesskey = A
sitedata-disallow-cookies-option =
    .label = Bloquear cookies y datos del sitio
    .accesskey = B
# This label means 'type of content that is blocked', and is followed by a drop-down list with content types below.
# The list items are the strings named sitedata-block-*-option*.
sitedata-block-desc = Tipo bloqueado
    .accesskey = T
sitedata-block-trackers-option-recommended =
    .label = Rastreadores de terceros (recomendado)
sitedata-block-trackers-option =
    .label = Rastreadores de terceros
sitedata-block-unvisited-option =
    .label = Cookies de sitios web no visitados
sitedata-block-all-third-party-option =
    .label = Todas las cookies de terceros (puede causar errores en los sitios web)
sitedata-block-all-option =
    .label = Todas las cookies (causará errores en los sitios web)
sitedata-clear =
    .label = Limpiar datos…
    .accesskey = L
sitedata-settings =
    .label = Administrar datos…
    .accesskey = M
sitedata-cookies-exceptions =
    .label = Excepciones…
    .accesskey = E
# This is a warning message shown next to a yellow warning icon when the Cookies and Site Data subsection
# in Preferences has been disabled due to Content Blocking being disabled. It is displayed next to the
# Cookies and Site Data section.
sitedata-warning-your-settings-prevent-changes = Su configuración del bloqueo de contenido está impidiendo el cambio de los ajustes de cookies y datos del sitio.

## Privacy Section - Address Bar

addressbar-header = Barra de direcciones
addressbar-suggest = Al usar la barra de direcciones, sugerir
addressbar-locbar-history-option =
    .label = Historial de navegación
    .accesskey = t
addressbar-locbar-bookmarks-option =
    .label = Marcadores
    .accesskey = M
addressbar-locbar-openpage-option =
    .label = Abrir pestañas
    .accesskey = A
addressbar-suggestions-settings = Cambiar preferencias de sugerencias de buscadores

## Privacy Section - Content Blocking

content-blocking-header = Bloqueo de contenido
content-blocking-desc = Bloquee el contenido de terceros, como anuncios o código, que puede ralentizar la navegación y realizar un seguimiento de los sitios que visita. Personalice su configuración para obtener el mejor equilibrio entre protección y rendimiento.
content-blocking-learn-more = Saber más
content-blocking-restore-defaults =
    .label = Restaurar ajustes predeterminados
    .accesskey = R
content-blocking-toggle-on =
    .tooltiptext = Desactivar el bloqueo de contenido
content-blocking-toggle-off =
    .tooltiptext = Activar el bloqueo de contenido
content-blocking-toggle-label-on = ACTIVADO
    .accesskey = A
content-blocking-toggle-label-off = DESACTIVADO
    .accesskey = D
content-blocking-category-label = Elija qué va a bloquear
# "Slow" in this instance means "slow to load on the network".
# FastBlock is a feature that blocks requests to tracking sites if they
# have not finished loading after a certain threshold of seconds.
content-blocking-fastblock-slow-loading-trackers-label =
    .label = Rastreadores de carga lenta
    .accesskey = S
content-blocking-fastblock-new-description = Bloquear solo los rastreadores que impiden cargar rápidamente las páginas.
content-blocking-tracking-protection-all-detected-trackers-label =
    .label = Todos los rastreadores detectados
    .accesskey = T
content-blocking-tracking-protection-new-description = Bloquear todos los rastreadores conocidos (puede impedir cargar algunas páginas).
content-blocking-tracking-protection-option-always =
    .label = Siempre
    .accesskey = S
content-blocking-tracking-protection-option-private =
    .label = Solo en ventanas privadas
    .accesskey = p
content-blocking-tracking-protection-change-block-list = Cambiar la lista de bloqueo
content-blocking-third-party-cookies-label =
    .label = Cookies de terceros
    .accesskey = C
content-blocking-reject-trackers-description = Bloquear todas las cookies de terceros o solo aquellas establecidas por rastreadores.
# This is a warning message shown next to a yellow warning icon when the Third-Party Cookies subsection
# of the Content Blocking UI in Preferences has been disabled due to the either the "All cookies" option
# or the "Cookies from unvisited websites" option being selected in the Cookies and Site Data section of
# the UI.
content-blocking-reject-trackers-warning-your-settings-prevent-changes = Su configuración en cookies y datos de sitio  está impidiendo el cambio de los ajustes de cookies de terceros.
content-blocking-change-cookie-settings =
    .label = Cambiar la configuración de cookies
    .accesskey = S
content-blocking-reject-trackers-block-trackers-option-recommended =
    .label = Rastreadores (recomendado)
    .accesskey = R
content-blocking-reject-trackers-block-trackers-option =
    .label = Rastreadores
    .accesskey = R
content-blocking-reject-trackers-all-third-parties-option =
    .label = Todas las cookies de terceros (puede causar errores en los sitios web)
    .accesskey = A

## Privacy Section - Tracking

tracking-header = Protección contra rastreo
tracking-desc = La protección contra rastreo bloquea los rastreadores en línea que recopilan sus datos de navegación a través de múltiples sitios web. <a data-l10n-name="learn-more">Más información sobre la protección contra rastreo y su privacidad</a>
tracking-mode-label = Usar protección contra rastreo para bloquear los rastreadores conocidos
tracking-mode-always =
    .label = Siempre
    .accesskey = i
tracking-mode-private =
    .label = Solo en ventanas privadas
    .accesskey = v
tracking-mode-never =
    .label = Nunca
    .accesskey = u
# This string is displayed if privacy.trackingprotection.ui.enabled is set to false.
# This currently happens on the release and beta channel.
tracking-pbm-label = Usar protección contra rastreo en la navegación privada para bloquear los rastreadores conocidos
    .accesskey = d
tracking-exceptions =
    .label = Excepciones…
    .accesskey = x
tracking-change-block-list =
    .label = Cambiar lista de bloqueo…
    .accesskey = l

## Privacy Section - Permissions

permissions-header = Permisos
permissions-location = Ubicación
permissions-location-settings =
    .label = Configuración…
    .accesskey = u
permissions-camera = Cámara
permissions-camera-settings =
    .label = Configuración…
    .accesskey = u
permissions-microphone = Micrófono
permissions-microphone-settings =
    .label = Configuración…
    .accesskey = u
permissions-notification = Notificaciones
permissions-notification-settings =
    .label = Configuración…
    .accesskey = u
permissions-notification-link = Saber más
permissions-notification-pause =
    .label = Pausar notificaciones hasta que { -brand-short-name } se reinicie
    .accesskey = n
permissions-block-autoplay-media =
    .label = Bloquear en los sitios web la reproducción automática de medios con sonido
    .accesskey = B
permissions-block-autoplay-media-menu = Para sitios web que reproducen sonidos automáticamente
permissions-block-autoplay-media-exceptions =
    .label = Excepciones…
    .accesskey = E
autoplay-option-ask =
    .label = Preguntar siempre
autoplay-option-allow =
    .label = Permitir reproducción automática
autoplay-option-dont =
    .label = No reproducir de manera automática
permissions-autoplay-link = Más información
permissions-block-popups =
    .label = Bloquear ventanas emergentes
    .accesskey = B
permissions-block-popups-exceptions =
    .label = Excepciones…
    .accesskey = E
permissions-addon-install-warning =
    .label = Advertirle cuando los sitios web intenten instalar complementos
    .accesskey = A
permissions-addon-exceptions =
    .label = Excepciones…
    .accesskey = E
permissions-a11y-privacy-checkbox =
    .label = Impedir que los servicios de accesibilidad accedan a su navegador
    .accesskey = I
permissions-a11y-privacy-link = Saber más

## Privacy Section - Data Collection

collection-header = Recopilación y uso de datos de { -brand-short-name }
collection-description = Nos esforzamos en proporcionarle opciones y recopilamos solo lo que necesitamos para proporcionarle y mejorar { -brand-short-name } para todos. Siempre pedimos permiso antes de recibir información personal.
collection-privacy-notice = Aviso sobre privacidad
collection-health-report =
    .label = Permitir a { -brand-short-name } enviar datos técnicos y de interacción a { -vendor-short-name }
    .accesskey = P
collection-health-report-link = Saber más
collection-studies =
    .label = Permitir que { -brand-short-name } instale y ejecute estudios
collection-studies-link = Ver los estudios de { -brand-short-name }
# This message is displayed above disabled data sharing options in developer builds
# or builds with no Telemetry support available.
collection-health-report-disabled = La recopilación de datos está deshabilitada en esta configuración de compilación
collection-browser-errors =
    .label = Permitir a { -brand-short-name } enviar informes de error del navegador (incluyendo mensajes de error) a { -vendor-short-name }
    .accesskey = e
collection-browser-errors-link = Saber más
collection-backlogged-crash-reports =
    .label = Permitir que { -brand-short-name } envíe los informes de fallos pendientes en su nombre
    .accesskey = r
collection-backlogged-crash-reports-link = Saber más

## Privacy Section - Security
##
## It is important that wording follows the guidelines outlined on this page:
## https://developers.google.com/safe-browsing/developers_guide_v2#AcceptableUsage

security-header = Seguridad
security-browsing-protection = Protección contra contenido engañoso y software peligroso
security-enable-safe-browsing =
    .label = Bloquear contenido peligroso y engañoso
    .accesskey = B
security-enable-safe-browsing-link = Saber más
security-block-downloads =
    .label = Bloquear descargas peligrosas
    .accesskey = d
security-block-uncommon-software =
    .label = Advertirle sobre software no deseado y poco usual
    .accesskey = v

## Privacy Section - Certificates

certs-header = Certificados
certs-personal-label = Cuando un servidor solicite su certificado personal
certs-select-auto-option =
    .label = Seleccionar uno automáticamente
    .accesskey = S
certs-select-ask-option =
    .label = Preguntar cada vez
    .accesskey = P
certs-enable-ocsp =
    .label = Consultar a los servidores respondedores OCSP para confirmar la validez actual de los certificados
    .accesskey = u
certs-view =
    .label = Ver certificados…
    .accesskey = e
certs-devices =
    .label = Dispositivos de seguridad…
    .accesskey = D
