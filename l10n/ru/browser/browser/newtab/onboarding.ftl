# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### UI strings for the simplified onboarding modal / about:welcome
### Various strings use a non-breaking space to avoid a single dangling /
### widowed word, so test on various window sizes if you also want this.


## These button action text can be split onto multiple lines, so use explicit
## newlines in translations to control where the line break appears (e.g., to
## avoid breaking quoted text).

onboarding-button-label-learn-more = Подробнее
onboarding-button-label-try-now = Попробовать сейчас
onboarding-button-label-get-started = Начало работы

## Welcome modal dialog strings

onboarding-welcome-header = Добро пожаловать в { -brand-short-name }
onboarding-welcome-body = Теперь у вас есть браузер.<br/>Познакомьтесь с { -brand-product-name } поближе.
onboarding-welcome-learn-more = Узнать больше о преимуществах.
onboarding-join-form-header = Присоединитесь к сообществу { -brand-product-name }
onboarding-join-form-body = Введите ваш адрес эл. почты, чтобы начать.
onboarding-join-form-email =
    .placeholder = Введите адрес эл. почты
onboarding-join-form-email-error = Введите действующий адрес эл. почты
onboarding-join-form-legal = Продолжая, вы соглашаетесь с <a data-l10n-name="terms">условиями предоставления услуг</a> и <a data-l10n-name="privacy">уведомлением о конфиденциальности</a>.
onboarding-join-form-continue = Продолжить
onboarding-start-browsing-button-label = Начать веб-сёрфинг
onboarding-cards-dismiss =
    .title = Скрыть
    .aria-label = Скрыть

## Firefox Sync modal dialog strings.

onboarding-sync-welcome-header = Возьмите { -brand-product-name } с собой
onboarding-sync-welcome-content = Получите доступ к вашим закладкам, истории, паролям и другим параметрам на всех ваших устройствах.
onboarding-sync-welcome-learn-more-link = Узнайте больше об Аккаунтах Firefox
onboarding-sync-form-invalid-input = Введите действующий адрес электронной почты
onboarding-sync-legal-notice = Продолжая, вы соглашаетесь с <a data-l10n-name="terms">условиями службы</a> и <a data-l10n-name="privacy">политикой приватности</a>.
onboarding-sync-form-input =
    .placeholder = Эл. почта
onboarding-sync-form-continue-button = Продолжить
onboarding-sync-form-skip-login-button = Пропустить этот шаг

## This is part of the line "Enter your email to continue to Firefox Sync"

onboarding-sync-form-header = Введите ваш адрес электронной почты
onboarding-sync-form-sub-header = чтобы продолжить использовать { -sync-brand-name(case: "accusative") }

## These are individual benefit messages shown with an image, title and
## description.

onboarding-benefit-products-title = Полезные продукты
onboarding-benefit-products-text = Делайте свои дела с помощью семейства инструментов, которое уважает вашу приватность на всех ваших устройствах.
onboarding-benefit-knowledge-title = Практические знания
onboarding-benefit-knowledge-text = Узнайте всё, что вам необходимо знать об Интернете, и как оставаться в нём в безопасности.
onboarding-benefit-privacy-title = Истинная приватность
# "Personal Data Promise" is a concept that should be translated consistently
# across the product. It refers to a concept shown elsewhere to the user: "The
# Firefox Personal Data Promise is the way we honor your data in everything we
# make and do. We take less data. We keep it safe. And we make sure that we are
# transparent about how we use it."
onboarding-benefit-privacy-text = Во всём, что мы делаем, мы следуем нашему Обещанию по Личным Данным: Собирать меньше. Держать в безопасности. Никаких секретов.

## These strings belong to the individual onboarding messages.


## Each message has a title and a description of what the browser feature is.
## Each message also has an associated button for the user to try the feature.
## The string for the button is found above, in the UI strings section

onboarding-private-browsing-title = Приватный просмотр
onboarding-private-browsing-text = Пользуйтесь Интернетом для себя. Приватный просмотр с Блокировкой содержимого блокирует трекеры, следящие за вами в Интернете.
onboarding-screenshots-title = Скриншоты
onboarding-screenshots-text = Создавайте, сохраняйте и делитесь скриншотами — не выходя из { -brand-short-name }. При веб-сёрфинге захватите прямоугольный участок или всю страницу. Затем сохраните скриншот в Интернете, чтобы легко получить к нему доступ и поделиться.
onboarding-addons-title = Дополнения
onboarding-addons-text = Добавьте ещё больше функций в { -brand-short-name }, чтобы удовлетворить свои потребности. Сравнивайте цены, смотрите погоду, или меняйте внешний вид браузера с помощью тем, чтобы сделать его особенным.
onboarding-ghostery-title = Ghostery
onboarding-ghostery-text = Пользуйтесь Интернетом быстрее, удобнее и безопаснее, с помощью таких расширений, как Ghostery, позволяющего блокировать надоедливую рекламу.
# Note: "Sync" in this case is a generic verb, as in "to synchronize"
onboarding-fxa-title = Синхронизировать
onboarding-fxa-text = Зарегистрируйте { -fxaccount-brand-name(case: "nominative") } и синхронизируйте свои закладки, пароли и открытые вкладки везде, где бы вы не использовали { -brand-short-name }.
onboarding-tracking-protection-title2 = Защита от отслеживания
onboarding-tracking-protection-text2 = { -brand-short-name } помогает остановить отслеживание ваших действий в Интернете, что затрудняет отслеживание вас рекламой в Интернете.
onboarding-tracking-protection-button2 = Как это работает
onboarding-data-sync-title = Возьмите свои настройки с собой
# "Sync" is short for synchronize.
onboarding-data-sync-text2 = Синхронизируйте ваши закладки, пароли и многое другое, где бы вы ни использовали { -brand-product-name }.
onboarding-data-sync-button2 = Войти в { -sync-brand-short-name(case: "accusative") }
onboarding-firefox-monitor-title = Подпишитесь на уведомления об утечках данных
onboarding-firefox-monitor-text = { -monitor-brand-name } следит, не был ли ваш адрес электронной почты затронут утечками данных, и если да, то сразу уведомляет вас об этом.
onboarding-firefox-monitor-button = Подписаться на уведомления
onboarding-browse-privately-title = Сёрфите приватно
onboarding-browse-privately-text = Приватный просмотр удаляет вашу историю поиска и просмотров страниц, чтобы держать её в тайне от других людей, которые используют этот компьютер.
onboarding-browse-privately-button = Открыть приватное окно
onboarding-firefox-send-title = Приватно обменивайтесь файлами
onboarding-firefox-send-text2 = Загружайте свои файлы с помощью { -send-brand-name }, чтобы делиться ими со сквозным шифрованием и ограниченным сроком действия ссылки на загрузку.
onboarding-firefox-send-button = Попробовать { -send-brand-name }
onboarding-mobile-phone-title = Загрузите { -brand-product-name } на ваш телефон
onboarding-mobile-phone-text = Загрузите { -brand-product-name } для iOS или Android и синхронизируйте данные между всеми своими устройствами.
# "Mobile" is short for mobile/cellular phone, "Browser" is short for web
# browser.
onboarding-mobile-phone-button = Загрузить мобильный браузер
onboarding-send-tabs-title = Мгновенно отправляйте вкладки самому себе
# "Send Tabs" refers to "Send Tab to Device" feature that appears when opening a
# tab's context menu.
onboarding-send-tabs-text = Send Tabs мгновенно обменивается страницами между вашими устройствами без необходимости копирования, вставки или выхода из браузера.
onboarding-send-tabs-button = Начать использовать отправку вкладок
onboarding-pocket-anywhere-title = Читайте и слушайте, где бы вы не находились
onboarding-pocket-anywhere-text2 = Сохраняйте ваши любимые статьи с помощью { -pocket-brand-name } и читайте, слушайте или просматривайте их даже без Интернета в любое удобное для вас время.
onboarding-pocket-anywhere-button = Попробовать { -pocket-brand-name }
onboarding-lockwise-passwords-title = Возьмите свои пароли с собой
onboarding-lockwise-passwords-text2 = Храните пароли в безопасности и легко входите в аккаунты с помощью { -lockwise-brand-name }.
onboarding-lockwise-passwords-button2 = Загрузить приложение
onboarding-facebook-container-title = Установите границы для Фейсбука
onboarding-facebook-container-text2 = { -facebook-container-brand-name } отделяет ваш профиль от всего остального, затрудняя Фейсбуку отслеживание вас для показа целевой рекламы.
onboarding-facebook-container-button = Установить расширение

## Message strings belonging to the Return to AMO flow

return-to-amo-sub-header = Отлично, вы установили { -brand-short-name }
# <icon></icon> will be replaced with the icon belonging to the extension
#
# Variables:
#   $addon-name (String) - Name of the add-on
return-to-amo-addon-header = Теперь давайте установим <icon></icon><b>{ $addon-name }.</b>
return-to-amo-extension-button = Добавить расширение
return-to-amo-get-started-button = Начните работу с { -brand-short-name }
