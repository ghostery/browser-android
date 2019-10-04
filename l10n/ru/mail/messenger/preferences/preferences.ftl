# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

pane-general-title = Основные
category-general =
    .tooltiptext = { pane-general-title }
general-language-and-appearance-header = Язык и внешний вид
general-incoming-mail-header = Входящие сообщения
general-files-and-attachment-header = Файлы и вложения
general-tags-header = Метки
general-reading-and-display-header = Чтение и отображение
general-updates-header = Обновления
general-network-and-diskspace-header = Сеть и дисковое пространство
general-indexing-label = Индексация
composition-category-header = Составление
composition-attachments-header = Вложения
composition-spelling-title = Орфография
compose-html-style-title = HTML-стиль
composition-addressing-header = Адресация
privacy-main-header = Приватность
privacy-passwords-header = Пароли
privacy-junk-header = Спам
privacy-data-collection-header = Сбор и использование данных
privacy-security-header = Защита
privacy-scam-detection-title = Обнаружение мошенничества
privacy-anti-virus-title = Антивирус
privacy-certificates-title = Сертификаты
chat-pane-header = Чат
chat-status-title = Статус
chat-notifications-title = Уведомления
chat-pane-styling-header = Стили
choose-messenger-language-description = Выберите язык отображения меню, сообщений и уведомлений от { -brand-short-name }.
manage-messenger-languages-button =
    .label = Выбрать альтернативные…
    .accesskey = ы
confirm-messenger-language-change-description = Перезапустите { -brand-short-name } для применения этих изменений
confirm-messenger-language-change-button = Применить и перезапустить
update-pref-write-failure-title = Ошибка записи
# Variables:
#   $path (String) - Path to the configuration file
update-pref-write-failure-message = Не удалось сохранить настройку. Не удалось произвести запись в файл: { $path }
update-setting-write-failure-title = Ошибка при сохранении настроек обновления
# Variables:
#   $path (String) - Path to the configuration file
# The newlines between the main text and the line containing the path is
# intentional so the path is easier to identify.
update-setting-write-failure-message =
    { -brand-short-name } столкнулся с ошибкой и не смог сохранить это изменение. Обратите внимание, что для установки этой настройки обновления необходимо разрешение на запись в файл, указанный ниже. Вы или системный администратор можете исправить эту проблему, если предоставите группе «Пользователи» полный доступ к этому файлу.
    
    Не удалось произвести запись в файл: { $path }
update-in-progress-title = Идёт обновление
update-in-progress-message = Вы хотите продолжить обновление { -brand-short-name }?
update-in-progress-ok-button = &Отменить
# Continue is the cancel button so pressing escape or using a platform standard
# method of closing the UI will not discard the update.
update-in-progress-cancel-button = &Продолжить
