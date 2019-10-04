# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

pane-general-title = General
category-general =
    .tooltiptext = { pane-general-title }

general-language-and-appearance-header = Idioma y apariencia

general-incoming-mail-header = Mensajes entrantes

general-files-and-attachment-header = Archivos y adjuntos

general-tags-header = Etiquetas

general-reading-and-display-header = Lectura y visualización

general-updates-header = Actualizaciones

general-network-and-diskspace-header = Red y espacio en disco

general-indexing-label = Indexado

composition-category-header = Redacción

composition-attachments-header = Adjuntos

composition-spelling-title = Ortografía

compose-html-style-title = Estilo HTML

composition-addressing-header = Direcciones

privacy-main-header = Privacidad

privacy-passwords-header = Contraseña

privacy-junk-header = Correo basura

privacy-data-collection-header = Recopilación y uso de datos

privacy-security-header = Seguridad

privacy-scam-detection-title = Detección de fraude

privacy-anti-virus-title = Antivirus

privacy-certificates-title = Certificados

chat-pane-header = Chat

chat-status-title = Estado

chat-notifications-title = Notificaciones

chat-pane-styling-header = Estilo

choose-messenger-language-description = Elija los idiomas usados para mostrar los menús, mensajes y notificaciones de { -brand-short-name }.
manage-messenger-languages-button =
  .label = Configurar alternativas…
  .accesskey = u
confirm-messenger-language-change-description = Reinicie { -brand-short-name } para aplicar los cambios
confirm-messenger-language-change-button = Aplicar y reiniciar

update-pref-write-failure-title = Fallo al escribir

# Variables:
#   $path (String) - Path to the configuration file
update-pref-write-failure-message = No se puede guardar la preferencia. No se puede escribir en el archivo: { $path }

update-setting-write-failure-title = Error al guardar las preferencias de actualización

# Variables:
#   $path (String) - Path to the configuration file
# The newlines between the main text and the line containing the path is
# intentional so the path is easier to identify.
update-setting-write-failure-message =
    { -brand-short-name } ha encontrado un error y no ha grabado este cambio. Tenga en cuenta que cambiar esta preferencia de actualización requiere permisos para escribir en el archivo de debajo. Usted o un administrador de sistemas pueden resolver el error concediendo al grupo Usuarios control completo sobre este archivo.

    No se puede escribir en el archivo: { $path }

update-in-progress-title = Actualización en progreso

update-in-progress-message = ¿Quiere que { -brand-short-name } continúe con esta actualización?

update-in-progress-ok-button = &Descartar
# Continue is the cancel button so pressing escape or using a platform standard
# method of closing the UI will not discard the update.
update-in-progress-cancel-button = &Continuar
