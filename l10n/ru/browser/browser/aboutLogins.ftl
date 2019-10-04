# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

about-logins-page-title = Логины и пароли

# "Google Play" and "App Store" are both branding and should not be translated

login-app-promo-title = Возьмите свои пароли с собой
login-app-promo-subtitle = Загрузите бесплатное приложение { -lockwise-brand-name }
login-app-promo-android =
    .alt = Доступно на Google Play
login-app-promo-apple =
    .alt = Загрузите в App Store
login-filter =
    .placeholder = Поиск логинов
create-login-button = Создать новый логин
# This string is used as alternative text for favicon images.
# Variables:
#   $title (String) - The title of the website associated with the favicon.
login-favicon =
    .alt = Значок для { $title }
fxaccounts-sign-in-text = Получайте доступ к своим паролям на других устройствах
fxaccounts-sign-in-button = Войти в { -sync-brand-short-name(case: "accusative") }
fxaccounts-avatar-button =
    .title = Управление аккаунтом

## The ⋯ menu that is in the top corner of the page

menu =
    .title = Открыть меню
# This menuitem is only visible on Windows
menu-menuitem-import = Импортировать пароли…
menu-menuitem-preferences =
    { PLATFORM() ->
        [windows] Настройки
       *[other] Настройки
    }
menu-menuitem-feedback = Отправить отзыв
menu-menuitem-faq = Часто задаваемые вопросы
menu-menuitem-android-app = { -lockwise-brand-short-name } для Android
menu-menuitem-iphone-app = { -lockwise-brand-short-name } для iPhone и iPad

## Login List

login-list =
    .aria-label = Логины, соответствующие поисковому запросу
login-list-count =
    { $count ->
        [one] { $count } логин
        [few] { $count } логина
       *[many] { $count } логинов
    }
login-list-sort-label-text = Сортировать по:
login-list-name-option = В алфавитном порядке
login-list-breached-option = Взломанные сайты
login-list-last-changed-option = По последнему изменению
login-list-last-used-option = По последнему использованию
login-list-intro-title = Логины не найдены
login-list-intro-description = Когда вы сохраните пароль в { -brand-product-name }, он появится здесь.
login-list-item-title-new-login = Новый логин
login-list-item-subtitle-new-login = Введите свои учётные данные
login-list-item-subtitle-missing-username = (нет имени пользователя)

## Introduction screen

login-intro-heading = Ищите сохранённые логины? Настройте { -sync-brand-short-name(case: "accusative") }.
login-intro-description = Если вы сохранили ваши логины в { -brand-product-name } на другом устройстве, то вот как получить к ним доступ здесь:
login-intro-instruction-fxa = Создайте или войдите в ваш { -fxaccount-brand-name } на устройстве, где сохранены ваши логины
login-intro-instruction-fxa-settings = Убедитесь, что вы установили флажок «Логины» в настройках { -sync-brand-short-name(case: "genitive") }
login-intro-instruction-faq = Посетите <a data-l10n-name="faq">часто задаваемые вопросы</a> по { -lockwise-brand-short-name } для получения помощи

## Login

login-item-new-login-title = Создать новый логин
login-item-edit-button = Изменить
login-item-delete-button = Удалить
login-item-origin-label = Адрес веб-сайта
login-item-origin =
    .placeholder = https://www.example.com
login-item-open-site-button = Открыть
login-item-username-label = Имя пользователя
login-item-username =
    .placeholder = имя@example.com
login-item-copy-username-button-text = Копировать
login-item-copied-username-button-text = Скопировано!
login-item-password-label = Пароль
login-item-password-reveal-checkbox-show =
    .title = Показать пароль
login-item-password-reveal-checkbox-hide =
    .title = Скрыть пароль
login-item-copy-password-button-text = Копировать
login-item-copied-password-button-text = Скопировано!
login-item-save-changes-button = Сохранить изменения
login-item-save-new-button = Сохранить
login-item-cancel-button = Отмена
login-item-time-changed = Последнее изменение: { DATETIME($timeChanged, day: "numeric", month: "long", year: "numeric") }
login-item-time-created = Создано: { DATETIME($timeCreated, day: "numeric", month: "long", year: "numeric") }
login-item-time-used = Последнее использование: { DATETIME($timeUsed, day: "numeric", month: "long", year: "numeric") }

## Master Password notification

master-password-notification-message = Пожалуйста, введите ваш мастер-пароль для просмотра сохранённых логинов и паролей
master-password-reload-button =
    .label = Войти
    .accesskey = В

## Dialogs

confirmation-dialog-cancel-button = Отмена
confirmation-dialog-dismiss-button =
    .title = Отмена
enable-password-sync-notification-message =
    { PLATFORM() ->
        [windows] Хотите получать доступ к вашим логинам везде, где бы вы ни использовали { -brand-product-name }? Перейдите в настройки { -sync-brand-short-name(case: "genitive") } и выберите «Логины».
       *[other] Хотите получать доступ к вашим логинам везде, где бы вы ни использовали { -brand-product-name }? Перейдите в настройки { -sync-brand-short-name(case: "genitive") } и выберите «Логины».
    }
enable-password-sync-preferences-button =
    .label =
        { PLATFORM() ->
            [windows] Открыть настройки { -sync-brand-short-name(case: "genitive") }
           *[other] Открыть настройки { -sync-brand-short-name(case: "genitive") }
        }
    .accesskey = О
confirm-delete-dialog-title = Удалить этот логин?
confirm-delete-dialog-message = Это действие не может быть отменено.
confirm-delete-dialog-confirm-button = Удалить
confirm-discard-changes-dialog-title = Отменить несохранённые изменения?
confirm-discard-changes-dialog-message = Все несохранённые изменения будут потеряны.
confirm-discard-changes-dialog-confirm-button = Отменить

## Breach Alert notification

breach-alert-text = С момента последнего обновления данных для входа, с этого сайта произошла утечка или кража паролей. Измените ваш пароль, чтобы защитить свой аккаунт.
breach-alert-link = Узнать больше об этой утечке.
breach-alert-dismiss =
    .title = Закрыть это уведомление
