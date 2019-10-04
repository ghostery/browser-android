# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

abuse-report-title-extension = Informar de esta extensión a { -vendor-short-name }
abuse-report-title-theme = Informar de este tema a { -vendor-short-name }
abuse-report-subtitle = ¿Cuál es el problema?
# Variables:
#   $author-name (string) - Name of the add-on author
abuse-report-addon-authored-by = por <a data-l10n-name="author-name">{ $author-name }</a>
abuse-report-learnmore = ¿No sabe qué problema seleccionar? <a data-l10n-name="learnmore-link">Descubre más sobre cómo informar de extensiones y temas</a>
abuse-report-submit-description = Describir el problema (opcional)
abuse-report-textarea =
    .placeholder = Para nosotros es más fácil solucionar un problema si tenemos detalles específicos. Describa su problema. Gracias por ayudarnos a que Internet siga siendo saludable.
abuse-report-submit-note = Nota: no incluya información personal (como nombres, dirección de correo, número de teléfono, dirección postal). { -vendor-short-name } guarda un registro de estos informes.

## Panel buttons.

abuse-report-cancel-button = Cancelar
abuse-report-next-button = Siguiente
abuse-report-goback-button = Volver atrás
abuse-report-submit-button = Enviar

## Message bars descriptions.


## Variables:
##   $addon-name (string) - Name of the add-on

abuse-report-messagebar-aborted = Se canceló el informe sobre <span data-l10n-name="addon-name">{ $addon-name }</span>.
abuse-report-messagebar-submitting = Enviando informe sobre <span data-l10n-name="addon-name">{ $addon-name }</span>.
abuse-report-messagebar-submitted = Gracias por enviar el informe. ¿Quiere eliminar <span data-l10n-name="addon-name">{ $addon-name }</span>?
abuse-report-messagebar-submitted-noremove = Gracias por enviar el informe.
abuse-report-messagebar-removed-extension = Gracias por enviar el informe. Ha eliminado la extensión <span data-l10n-name="addon-name">{ $addon-name }</span>.
abuse-report-messagebar-removed-theme = Gracias por enviar el informe. Ha eliminado el tema <span data-l10n-name="addon-name">{ $addon-name }</span>.
abuse-report-messagebar-error = Hubo un error al enviar el informe sobre <span data-l10n-name="addon-name">{ $addon-name }</span>.
abuse-report-messagebar-error-recent-submit = No se envió el informe sobre <span data-l10n-name="addon-name">{ $addon-name }</span> porque otro informe se envió recientemente.

## Message bars actions.

abuse-report-messagebar-action-remove-extension = Sí, eliminarlo
abuse-report-messagebar-action-keep-extension = No, guardarlo
abuse-report-messagebar-action-remove-theme = Sí, eliminarlo
abuse-report-messagebar-action-keep-theme = No, guardarlo
abuse-report-messagebar-action-retry = Reintentar
abuse-report-messagebar-action-cancel = Cancelar

## Abuse report reasons (optionally paired with related examples and/or suggestions)

abuse-report-damage-reason = Daña mi equipo e información
abuse-report-damage-example = Ejemplo: instaló malware o robó información
abuse-report-spam-reason = Crea spam o publicidad
abuse-report-spam-example = Ejemplo: inserta publicidad en páginas web
abuse-report-settings-reason = Cambió mi buscador, página de inicio o página Nueva pestaña sin informarme ni preguntarme
abuse-report-settings-suggestions = Antes de informar sobre la extensión, pruebe a cambiar la configuración:
abuse-report-settings-suggestions-search = Cambie la configuración predeterminada de las búsquedas
abuse-report-settings-suggestions-homepage = Cambie la página de inicio y de nueva pestaña
abuse-report-deceptive-reason = Pretender ser algo que no es
abuse-report-deceptive-example = Ejemplo: descripción o imágenes erróneas
abuse-report-broken-reason-extension = No funciona, cuelga los sitios web o ralentiza { -brand-product-name }
abuse-report-broken-reason-theme = No funciona o no se visualiza correctamente
abuse-report-broken-example = Ejemplo: la función es lenta, difícil de usar o no funciona; ciertas partes de los sitios web no se cargan o se muestran de forma incorrecta
abuse-report-broken-suggestions-extension = Parece que ha identificado un problema. Además de enviar un informe por aquí, lo mejor para conseguir que se resuelva un problema de funcionalidad es contactar con el desarrollador de la extensión. <a data-l10n-name="support-link">Acceda al sitio web de la extensión</a> para conseguir los datos de contacto del desarrollador.
abuse-report-broken-suggestions-theme = Parece que ha identificado un problema. Además de enviar un informe por aquí, lo mejor para conseguir que se resuelva un problema de funcionalidad es contactar con el desarrollador del tema. <a data-l10n-name="support-link">Acceda al sitio web del tema</a> para conseguir los datos de contacto del desarrollador.
abuse-report-policy-reason = Contenido violento, ilegal o de odio.
abuse-report-policy-suggestions =
    Nota: Los problemas relacionados con derechos de autor y marcas registradas deben informarse en un proceso separado.
    <a data-l10n-name="report-infringement-link">Utilice estas instrucciones</a> para
    informar del problema
abuse-report-unwanted-reason = Nunca quise esta extensión y no puedo desinstalarla
abuse-report-unwanted-example = Ejemplo: una aplicación la instaló sin mi permiso
abuse-report-other-reason = Otra cosa
