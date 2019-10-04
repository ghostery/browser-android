# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### UI strings for the simplified onboarding modal / about:welcome
### Various strings use a non-breaking space to avoid a single dangling /
### widowed word, so test on various window sizes if you also want this.


## These button action text can be split onto multiple lines, so use explicit
## newlines in translations to control where the line break appears (e.g., to
## avoid breaking quoted text).

onboarding-button-label-learn-more = Saber más
onboarding-button-label-try-now = Pruébelo ahora
onboarding-button-label-get-started = Comenzar

## Welcome modal dialog strings

onboarding-welcome-header = Bienvenido a { -brand-short-name }
onboarding-welcome-body = Ya tiene el navegador.<br/>Conozca el resto de { -brand-product-name }.
onboarding-welcome-learn-more = Saber más sobre las ventajas.
onboarding-join-form-header = Unirse a { -brand-product-name }
onboarding-join-form-body = Para empezar, introduzca su dirección de correo.
onboarding-join-form-email =
    .placeholder = Introducir dirección de correo
onboarding-join-form-email-error = Una dirección de correo válida es obligatoria
onboarding-join-form-legal = Si continúas, aceptas los <a data-l10n-name="terms">Términos del servicio</a> y la <a data-l10n-name="privacy">Política de privacidad</a>.
onboarding-join-form-continue = Continuar
onboarding-start-browsing-button-label = Empezar a navegar
onboarding-cards-dismiss =
    .title = Ignorar
    .aria-label = Ignorar

## Firefox Sync modal dialog strings.

onboarding-sync-welcome-header = Llévese { -brand-product-name } consigo
onboarding-sync-welcome-content = Acceda a sus marcadores, historial, contraseñas y más ajustes en todos sus dispositivos.
onboarding-sync-welcome-learn-more-link = Descubra más sobre las Cuentas de Firefox
onboarding-sync-form-invalid-input = Se requiere un correo válido
onboarding-sync-legal-notice = Al continuar aceptas los <a data-l10n-name="terms">Términos del servicio</a> y el <a data-l10n-name="privacy">Aviso de privacidad</a>.
onboarding-sync-form-input =
    .placeholder = Correo electrónico
onboarding-sync-form-continue-button = Continuar
onboarding-sync-form-skip-login-button = Saltar este paso

## This is part of the line "Enter your email to continue to Firefox Sync"

onboarding-sync-form-header = Introduzca su correo electrónico
onboarding-sync-form-sub-header = para acceder a { -sync-brand-name }.

## These are individual benefit messages shown with an image, title and
## description.

onboarding-benefit-products-title = Productos útiles
onboarding-benefit-products-text = Haga las cosas con una familia de herramientas que respete su privacidad en todos sus dispositivos.
onboarding-benefit-knowledge-title = Conocimiento práctico
onboarding-benefit-knowledge-text = Aprenda todo lo que necesita saber para mantenerse en línea de manera más inteligente y segura.
onboarding-benefit-privacy-title = Privacidad de verdad
# "Personal Data Promise" is a concept that should be translated consistently
# across the product. It refers to a concept shown elsewhere to the user: "The
# Firefox Personal Data Promise is the way we honor your data in everything we
# make and do. We take less data. We keep it safe. And we make sure that we are
# transparent about how we use it."
onboarding-benefit-privacy-text = Todo lo que hacemos respeta nuestra promesa de datos personales: Tomar menos. Mantenerlo seguro. Sin secretos.

## These strings belong to the individual onboarding messages.


## Each message has a title and a description of what the browser feature is.
## Each message also has an associated button for the user to try the feature.
## The string for the button is found above, in the UI strings section

onboarding-private-browsing-title = Navegación privada
onboarding-private-browsing-text = Navegue en solitario. La navegación privada con bloqueo de contenido impide que los rastreadores en línea le sigan por la web.
onboarding-screenshots-title = Capturas de pantalla
onboarding-screenshots-text = Haga, guarde y comparta capturas de pantalla - todo sin salir de { -brand-short-name }. Capture una región o una página completa mientras navega. A continuación, guárdelo en la web para poder acceder a él y compartirlo fácilmente.
onboarding-addons-title = Complementos
onboarding-addons-text = Añada aún más funciones que hagan que { -brand-short-name } trabaje más duro para usted. Compare precios, sepa qué tiempo hará mañana o exprese su personalidad con un tema personalizado.
onboarding-ghostery-title = Ghostery
onboarding-ghostery-text = Navegue más rápido, de manera más inteligente o segura, con extensiones como Ghostery, que le permite bloquear anuncios molestos.
# Note: "Sync" in this case is a generic verb, as in "to synchronize"
onboarding-fxa-title = Sincronizar
onboarding-fxa-text = Regístrese para tener una { -fxaccount-brand-name } y sincronice sus marcadores, contraseñas y pestañas abiertas allá donde utilice { -brand-short-name }
onboarding-tracking-protection-title2 = Protección contra rastreo
onboarding-tracking-protection-text2 = { -brand-short-name } le ayuda a impedir que los sitios le rastreen en línea, por lo que será más difícil que la publicidad le siga mientras navega.
onboarding-tracking-protection-button2 = Cómo funciona
onboarding-data-sync-title = Llévate tus ajustes contigo
# "Sync" is short for synchronize.
onboarding-data-sync-text2 = Sincronice sus marcadores, contraseñas y mucho más donde quiera que use { -brand-product-name }.
onboarding-data-sync-button2 = Iniciar sesión en { -sync-brand-short-name }
onboarding-firefox-monitor-title = Mantente al día sobre las filtraciones de datos
onboarding-firefox-monitor-text = { -monitor-brand-name } comprueba si tu dirección de correo ha aparecido en una filtración de datos y te avisa si vuelve a aparecer.
onboarding-firefox-monitor-button = Suscríbase para recibir alertas
onboarding-browse-privately-title = Navegue de forma privada
onboarding-browse-privately-text = La navegación privada elimina tu historial de búsquedas y de navegación para que nadie que use tu equipo tenga acceso a ellos.
onboarding-browse-privately-button = Abrir una ventana privada
onboarding-firefox-send-title = Mantenga privados sus archivos compartidos
onboarding-firefox-send-text2 = Suba sus archivos a { -send-brand-name } para compartirlos con cifrado de extremo a extremo y un enlace que expira automáticamente.
onboarding-firefox-send-button = Pruebe { -send-brand-name }
onboarding-mobile-phone-title = Consiga { -brand-product-name } en su teléfono
onboarding-mobile-phone-text = Descargue { -brand-product-name } para iOS o Android y sincronice su información en todos los dispositivos.
# "Mobile" is short for mobile/cellular phone, "Browser" is short for web
# browser.
onboarding-mobile-phone-button = Descargar navegador móvil
onboarding-send-tabs-title = Envíese las pestañas instantáneamente
# "Send Tabs" refers to "Send Tab to Device" feature that appears when opening a
# tab's context menu.
onboarding-send-tabs-text = Send Tabs comparte páginas de forma instantánea entre tus dispositivos sin copiar, pegar ni abandonar el navegador.
onboarding-send-tabs-button = Empieza a usar Send Tabs
onboarding-pocket-anywhere-title = Lee y escucha sea donde sea
onboarding-pocket-anywhere-text2 = Guarde su contenido favorito sin conexión gracias a la aplicación de { -pocket-brand-name } y léalo, escúchelo o véalo dónde y cuándo mejor le convenga.
onboarding-pocket-anywhere-button = Pruebe { -pocket-brand-name }
onboarding-lockwise-passwords-title = Llévese sus contraseñas consigo a todas partes
onboarding-lockwise-passwords-text2 = Mantenga seguras las contraseñas que guarda e inicie sesión fácilmente en sus cuentas con { -lockwise-brand-name }.
onboarding-lockwise-passwords-button2 = Conseguir la aplicación
onboarding-facebook-container-title = Establezca límites con Facebook
onboarding-facebook-container-text2 = { -facebook-container-brand-name } separa su identidad de Facebook de todo lo demás, dificultando así que Facebook pueda mostrarle publicidad personalizada.
onboarding-facebook-container-button = Agregar extensión

## Message strings belonging to the Return to AMO flow

return-to-amo-sub-header = ¡Bien! Ya tiene { -brand-short-name }
# <icon></icon> will be replaced with the icon belonging to the extension
#
# Variables:
#   $addon-name (String) - Name of the add-on
return-to-amo-addon-header = Ahora obtenga <icon></icon><b>{ $addon-name }.</b>
return-to-amo-extension-button = Añadir la extensión
return-to-amo-get-started-button = Comenzar con { -brand-short-name }
