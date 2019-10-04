# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

addons-window =
    .title = Administrador de complementos
search-header =
    .placeholder = Buscar en addons.mozilla.org
    .searchbuttonlabel = Buscar
search-header-shortcut =
    .key = f
loading-label =
    .value = Cargando…
list-empty-installed =
    .value = No tiene instalado ningún complemento de este tipo
list-empty-available-updates =
    .value = No se han encontrado actualizaciones
list-empty-recent-updates =
    .value = No ha actualizado recientemente ningún complemento
list-empty-find-updates =
    .label = Buscar actualizaciones
list-empty-button =
    .label = Aprender más sobre los complementos
install-addon-from-file =
    .label = Instalar complemento desde archivo…
    .accesskey = I
help-button = Soporte para complementos
preferences =
    { PLATFORM() ->
        [windows] Opciones de { -brand-short-name }
       *[other] Preferencias de { -brand-short-name }
    }
tools-menu =
    .tooltiptext = Herramientas para todos los complementos
show-unsigned-extensions-button =
    .label = Algunas extensiones no han podido ser verificadas
show-all-extensions-button =
    .label = Mostrar todas las extensiones
debug-addons =
    .label = Depurar complementos
    .accesskey = u
cmd-show-details =
    .label = Mostrar más información
    .accesskey = M
cmd-find-updates =
    .label = Buscar actualizaciones
    .accesskey = B
cmd-preferences =
    .label =
        { PLATFORM() ->
            [windows] Opciones
           *[other] Preferencias
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
cmd-enable-theme =
    .label = Usar tema
    .accesskey = U
cmd-disable-theme =
    .label = Dejar de usar el tema
    .accesskey = u
cmd-install-addon =
    .label = Instalar
    .accesskey = I
cmd-contribute =
    .label = Colaborar
    .accesskey = C
    .tooltiptext = Colaborar con el desarrollo de este complemento
discover-title = ¿Qué son los complementos?
discover-description = Los complementos son aplicaciones que le permiten personalizar { -brand-short-name } con funcionalidades o estilos adicionales. Pruebe un panel lateral que le ahorrará tiempo, un indicador del tiempo meteorológico o un tema gráfico para hacer { -brand-short-name } verdaderamente suyo.
discover-footer = Cuando esté conectado a Internet, este panel incluirá algunos de los mejores y más populares complementos para que los pruebe.
detail-version =
    .label = Versión
detail-last-updated =
    .label = Última actualización
detail-contributions-description = El desarrollador de este complemento solicita que ayudes a continuar su desarrollo haciendo una pequeña contribución.
detail-contributions-button = Colaborar
    .title = Colaborar con el desarrollo de este complemento
    .accesskey = C
detail-update-type =
    .value = Actualizaciones automáticas
detail-update-default =
    .label = Por omisión
    .tooltiptext = Instalar actualizaciones automáticamente sólo si ése es el valor predeterminado
detail-update-automatic =
    .label = Sí
    .tooltiptext = Instalar actualizaciones automáticamente
detail-update-manual =
    .label = No
    .tooltiptext = No instalar automáticamente las actualizaciones
# Used as a description for the option to allow or block an add-on in private windows.
detail-private-browsing-label = Ejecutar en ventana privada
detail-private-browsing-description2 = Cuando esté permitido, la extensión tendrá acceso a su actividad en línea mientras navega de forma privada. <label data-l10n-name="detail-private-browsing-learn-more">Descubra más</label>
# Some add-ons may elect to not run in private windows by setting incognito: not_allowed in the manifest.  This
# cannot be overridden by the user.
detail-private-disallowed-label = No se permite en ventanas privadas
detail-private-disallowed-description = Esta extensión no se ejecuta durante la navegación privada. <label data-l10n-name="detail-private-browsing-learn-more">Descubra más</label>
# Some special add-ons are privileged, run in private windows automatically, and this permission can't be revoked
detail-private-required-label = Necesita acceder a ventanas privadas
detail-private-required-description = Esta extensión tiene acceso a sus actividades en línea mientras navega de forma privada. <label data-l10n-name="detail-private-browsing-learn-more">Descubra más</label>
detail-private-browsing-on =
    .label = Permitir
    .tooltiptext = Activar en navegación privada
detail-private-browsing-off =
    .label = No permitir
    .tooltiptext = Desactivar en navegación privada
detail-home =
    .label = Página de inicio
detail-home-value =
    .value = { detail-home.label }
detail-repository =
    .label = Perfil del complemento
detail-repository-value =
    .value = { detail-repository.label }
detail-check-for-updates =
    .label = Buscar actualizaciones
    .accesskey = B
    .tooltiptext = Buscar actualizaciones de este complemento
detail-show-preferences =
    .label =
        { PLATFORM() ->
            [windows] Opciones
           *[other] Preferencias
        }
    .accesskey =
        { PLATFORM() ->
            [windows] O
           *[other] P
        }
    .tooltiptext =
        { PLATFORM() ->
            [windows] Cambiar las opciones de este complemento
           *[other] Cambiar las preferencias de este complemento
        }
detail-rating =
    .value = Calificación
addon-restart-now =
    .label = Reiniciar ahora
disabled-unsigned-heading =
    .value = Algunos complementos han sido desactivados
disabled-unsigned-description = Los siguientes complementos no han sido verificados para su uso en { -brand-short-name }. Puede <label data-l10n-name="find-addons">encontrar reemplazos</label> o pedir al desarrollador que consiga que sean verificados.
disabled-unsigned-learn-more = Conozca más sobre nuestros esfuerzos para ayudarle a mantenerse seguro en Internet.
disabled-unsigned-devinfo = Los desarrolladores interesados en conseguir que sus complementos sean verificados pueden continuar leyendo nuestro <label data-l10n-name="learn-more">manual</label>.
plugin-deprecation-description = ¿Echa algo en falta? Algunos plugins ya no están admitidos por { -brand-short-name }. <label data-l10n-name="learn-more">Saber más.</label>
legacy-warning-show-legacy = Mostrar extensiones clásicas
legacy-extensions =
    .value = Extensiones clásicas
legacy-extensions-description = Estas extensiones no cumplen los estándares actuales de { -brand-short-name } por lo que han sido desactivadas. <label data-l10n-name="legacy-learn-more">Conozca sobre los cambios a los complementos</label>
private-browsing-description2 =
    { -brand-short-name } está cambiando la forma en que funcionan las extensiones en la navegación privada. Cualquier nueva extensión que agregues a
    { -brand-short-name } no se ejecutará de forma predeterminada en las ventanas privadas. A menos que lo permitas en los Ajustes, la
    la extensión no funcionará durante la navegación privada, y no tendrá acceso a tus actividades en línea
    ahí. Hemos realizado este cambio para mantener privada tu navegación privada.
    <label data-l10n-name = "private-browsing-learn-more">Descubre cómo administrar la configuración de las extensiones.</label>
extensions-view-discover =
    .name = Obtener complementos
    .tooltiptext = { extensions-view-discover.name }
extensions-view-discopane =
    .name = Recomendaciones
    .tooltiptext = { extensions-view-discopane.name }
extensions-view-recent-updates =
    .name = Actualizaciones recientes
    .tooltiptext = { extensions-view-recent-updates.name }
extensions-view-available-updates =
    .name = Actualizaciones disponibles
    .tooltiptext = { extensions-view-available-updates.name }

## These are global warnings

extensions-warning-safe-mode-label =
    .value = Todos los complementos han sido desactivados por el modo seguro.
extensions-warning-safe-mode-container =
    .tooltiptext = { extensions-warning-safe-mode-label.value }
extensions-warning-check-compatibility-label =
    .value = La comprobación de compatibilidad de complementos está desactivada. Puede tener algunos incompatibles.
extensions-warning-check-compatibility-container =
    .tooltiptext = { extensions-warning-check-compatibility-label.value }
extensions-warning-check-compatibility-enable =
    .label = Activar
    .tooltiptext = Activar comprobación de compatibilidad de complementos
extensions-warning-update-security-label =
    .value = La comprobación de seguridad de los complementos está desactivada. Puede ver su seguridad comprometida durante las actualizaciones.
extensions-warning-update-security-container =
    .tooltiptext = { extensions-warning-update-security-label.value }
extensions-warning-update-security-enable =
    .label = Activar
    .tooltiptext = Activar comprobación de seguridad de complementos

## Strings connected to add-on updates

extensions-updates-check-for-updates =
    .label = Buscar actualizaciones
    .accesskey = B
extensions-updates-view-updates =
    .label = Ver actualizaciones recientes
    .accesskey = V

# This menu item is a checkbox that toggles the default global behavior for
# add-on update checking.

extensions-updates-update-addons-automatically =
    .label = Actualizar complementos automáticamente
    .accesskey = A

## Specific add-ons can have custom update checking behaviors ("Manually",
## "Automatically", "Use default global behavior"). These menu items reset the
## update checking behavior for all add-ons to the default global behavior
## (which itself is either "Automatically" or "Manually", controlled by the
## extensions-updates-update-addons-automatically.label menu item).

extensions-updates-reset-updates-to-automatic =
    .label = Configurar todos los complementos para que se actualicen automáticamente
    .accesskey = R
extensions-updates-reset-updates-to-manual =
    .label = Configurar todos los complementos para que se actualicen manualmente
    .accesskey = R

## Status messages displayed when updating add-ons

extensions-updates-updating =
    .value = Actualizando complementos
extensions-updates-installed =
    .value = Se han actualizado sus complementos.
extensions-updates-downloaded =
    .value = Descargadas actualizaciones de sus complementos.
extensions-updates-restart =
    .label = Reiniciar ahora para instalar
extensions-updates-none-found =
    .value = No se han encontrado actualizaciones
extensions-updates-manual-updates-found =
    .label = Ver actualizaciones disponibles
extensions-updates-update-selected =
    .label = Instalar actualizaciones
    .tooltiptext = Instalar actualizaciones disponibles en esta lista

## Extension shortcut management

manage-extensions-shortcuts =
    .label = Administrar atajos de extensiones
    .accesskey = s
shortcuts-no-addons = No tiene ninguna extensión habilitada.
shortcuts-no-commands = Las siguientes extensiones no tienen atajos:
shortcuts-input =
    .placeholder = Escriba un atajo
shortcuts-browserAction = Activar extensión
shortcuts-pageAction = Activar acción de página
shortcuts-sidebarAction = Alternar la barra lateral
shortcuts-modifier-mac = Incluir Ctrl, Alt o ⌘
shortcuts-modifier-other = Incluir Ctrl o Alt
shortcuts-invalid = Combinación no válida
shortcuts-letter = Escriba una letra
shortcuts-system = No se puede sobrescribir un acceso directo de { -brand-short-name }.
# String displayed when a keyboard shortcut is already used by another add-on
# Variables:
#   $addon (string) - Name of the add-on
shortcuts-exists = Ya lo está usando { $addon }
shortcuts-card-expand-button =
    { $numberToShow ->
        [one] Mostrar { $numberToShow } más
       *[other] Mostrar { $numberToShow } más
    }
shortcuts-card-collapse-button = Mostrar menos
go-back-button =
    .tooltiptext = Retroceder

## Recommended add-ons page

# Explanatory introduction to the list of recommended add-ons. The action word
# ("recommends") in the final sentence is a link to external documentation.
discopane-intro =
    Las extensiones y temas son como aplicaciones para el navegador y le permiten
    proteger sus contraseñas, descargar vídeos, encontrar ofertas, bloquear publicidad,
    cambiar la apariencia del navegador y mucho más. Suelen ser terceros los que desarrollan
    estos pequeños programas de software. Te ofrecemos una selección de { -brand-product-name }
    <a data-l10n-name="learn-more-trigger">recomendados</a> en seguridad, rendimiento
    y funcionalidad excepcionales.
# Notice to make user aware that the recommendations are personalized.
discopane-notice-recommendations = Algunas de estas recomendaciones son personalizadas. Se basan en otras extensiones que tiene instaladas, preferencias de su perfil y estadísticas de uso.
discopane-notice-learn-more = Saber más
privacy-policy = Política de privacidad
# Refers to the author of an add-on, shown below the name of the add-on.
# Variables:
#   $author (string) - The name of the add-on developer.
created-by-author = por <a data-l10n-name="author">{ $author }</a>
# Shows the number of daily users of the add-on.
# Variables:
#   $dailyUsers (number) - The number of daily users.
user-count = Usuarios: { $dailyUsers }
install-extension-button = Agregar a { -brand-product-name }
install-theme-button = Instalar tema
# The label of the button that appears after installing an add-on. Upon click,
# the detailed add-on view is opened, from where the add-on can be managed.
manage-addon-button = Administrar
find-more-addons = Buscar más complementos

## Add-on actions

report-addon-button = Informe
remove-addon-button = Eliminar
disable-addon-button = Desactivar
enable-addon-button = Activar
expand-addon-button = Más opciones
preferences-addon-button =
    { PLATFORM() ->
        [windows] Opciones
       *[other] Preferencias
    }
details-addon-button = Detalles
release-notes-addon-button = Notas de la versión
permissions-addon-button = Permisos
addons-enabled-heading = Activado
addons-disabled-heading = Desactivado
ask-to-activate-button = Preguntar antes de activar
always-activate-button = Activar siempre
never-activate-button = No activar nunca
addon-detail-author-label = Autor
addon-detail-version-label = Versión
addon-detail-last-updated-label = Últ. actualización
addon-detail-homepage-label = Página de inicio
addon-detail-rating-label = Calificación
# The average rating that the add-on has received.
# Variables:
#   $rating (number) - A number between 0 and 5. The translation should show at most one digit after the comma.
five-star-rating =
    .title = Calificación de { NUMBER($rating, maximumFractionDigits: 1) } sobre 5
# This string is used to show that an add-on is disabled.
# Variables:
#   $name (string) - The name of the add-on
addon-name-disabled = { $name } (deshabilitado)
# The number of reviews that an add-on has received on AMO.
# Variables:
#   $numberOfReviews (number) - The number of reviews received
addon-detail-reviews-link =
    { $numberOfReviews ->
        [one] { $numberOfReviews } revisión
       *[other] { $numberOfReviews } revisiones
    }

## Pending uninstall message bar

# Variables:
#   $addon (string) - Name of the add-on
pending-uninstall-description = Se ha eliminado <span data-l10n-name="addon-name">{ $addon }</span>.
pending-uninstall-undo-button = Deshacer
addon-detail-updates-label = Permitir actualizaciones automáticas
addon-detail-updates-radio-default = Predeterminado
addon-detail-updates-radio-on = Activado
addon-detail-updates-radio-off = Desactivar
addon-detail-update-check-label = Comprobar si hay actualizaciones
install-update-button = Actualizar
# This is the tooltip text for the private browsing badge in about:addons. The
# badge is the private browsing icon included next to the extension's name.
addon-badge-private-browsing-allowed =
    .title = Permitida en ventanas privadas
addon-detail-private-browsing-help = Cuando está activada, la extensión tendrá acceso a todo lo que haces mientras navegas de forma privada. <a data-l10n-name="learn-more">Descubre más</a>
addon-detail-private-browsing-allow = Permitir
addon-detail-private-browsing-disallow = No permitir
# This is the tooltip text for the recommended badge for an extension in about:addons. The
# badge is a small icon displayed next to an extension when it is recommended on AMO.
addon-badge-recommended =
    .title = Recomendado
    .alt = Recomendado
available-updates-heading = Actualizaciones disponibles
recent-updates-heading = Actualizaciones recientes
release-notes-loading = Cargando…
release-notes-error = Lo sentimos, pero ha sucedido un error al cargar las notas de versión.
addon-permissions-empty = Esta extensión no requiere ningún permiso.
recommended-extensions-heading = Extensiones recomendadas
recommended-themes-heading = Temas recomendados
# A recommendation for the Firefox Color theme shown at the bottom of the theme
# list view. The "Firefox Color" name itself should not be translated.
recommended-theme-1 = ¿Se siente creativo? <a data-l10n-name="link">Cree su propio tema con Firefox Color.</a>
