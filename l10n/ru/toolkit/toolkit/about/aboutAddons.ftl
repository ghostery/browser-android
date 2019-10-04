# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

addons-window =
    .title = Управление дополнениями
search-header =
    .placeholder = Поиск на addons.mozilla.org
    .searchbuttonlabel = Поиск
search-header-shortcut =
    .key = f
loading-label =
    .value = Загрузка…
list-empty-installed =
    .value = У вас не установлено ни одного дополнения данного типа
list-empty-available-updates =
    .value = Обновлений не найдено
list-empty-recent-updates =
    .value = В последнее время вы не обновляли никаких дополнений
list-empty-find-updates =
    .label = Проверить наличие обновлений
list-empty-button =
    .label = Узнать больше о дополнениях
install-addon-from-file =
    .label = Установить дополнение из файла…
    .accesskey = а
help-button = Поддержка дополнений
preferences =
    { PLATFORM() ->
        [windows] Настройки { -brand-short-name }
       *[other] Настройки { -brand-short-name }
    }
tools-menu =
    .tooltiptext = Инструменты для всех дополнений
show-unsigned-extensions-button =
    .label = Некоторые расширения не могут быть проверены
show-all-extensions-button =
    .label = Показать все расширения
debug-addons =
    .label = Отладка дополнений
    .accesskey = л
cmd-show-details =
    .label = Подробнее
    .accesskey = р
cmd-find-updates =
    .label = Найти обновления
    .accesskey = а
cmd-preferences =
    .label =
        { PLATFORM() ->
            [windows] Настройки
           *[other] Настройки
        }
    .accesskey =
        { PLATFORM() ->
            [windows] с
           *[other] с
        }
cmd-enable-theme =
    .label = Использовать тему
    .accesskey = п
cmd-disable-theme =
    .label = Больше не использовать тему
    .accesskey = п
cmd-install-addon =
    .label = Установить
    .accesskey = н
cmd-contribute =
    .label = Внести вклад
    .accesskey = т
    .tooltiptext = Внести вклад в развитие этого дополнения
discover-title = Что такое дополнения?
discover-description = Дополнения — это приложения, которые позволяют вам настроить { -brand-short-name } по своему вкусу, добавив в него дополнительные функции или придав ему свой стиль. Попробуйте использовать экономящую время боковую панель, настройте уведомление о прогнозе погоды или измените его внешний вид, чтобы сделать { -brand-short-name } именно таким, каким вы желаете.
discover-footer = Когда вы соединитесь с Интернетом, в этой панели появятся некоторые из лучших и наиболее популярных дополнений, которые вы можете испытать в работе.
detail-version =
    .label = Версия
detail-last-updated =
    .label = Последнее обновление
detail-contributions-description = Разработчик этого дополнения просит вас помочь поддержать его дальнейшее развитие, внеся небольшое пожертвование.
detail-contributions-button = Принять участие
    .title = Принять участие в разработке этого дополнения
    .accesskey = я
detail-update-type =
    .value = Автоматическое обновление
detail-update-default =
    .label = По умолчанию
    .tooltiptext = Автоматически устанавливать обновления только если это настройка по умолчанию
detail-update-automatic =
    .label = Включено
    .tooltiptext = Устанавливать обновления автоматически
detail-update-manual =
    .label = Отключено
    .tooltiptext = Не устанавливать обновления автоматически
# Used as a description for the option to allow or block an add-on in private windows.
detail-private-browsing-label = Запуск в приватных окнах
detail-private-browsing-description2 = Когда разрешено, расширение будет иметь доступ к вашей активности в Интернете во время приватного просмотра. <label data-l10n-name="detail-private-browsing-learn-more">Подробнее</label>
# Some add-ons may elect to not run in private windows by setting incognito: not_allowed in the manifest.  This
# cannot be overridden by the user.
detail-private-disallowed-label = Не разрешено в приватных окнах
detail-private-disallowed-description = Это расширение не будет работать в режиме приватного просмотра. <label data-l10n-name="detail-private-browsing-learn-more">Подробнее</label>
# Some special add-ons are privileged, run in private windows automatically, and this permission can't be revoked
detail-private-required-label = Требует доступа к приватным окнам
detail-private-required-description = Это расширение имеет доступ к вашей активности в Интернете во время приватного просмотра. <label data-l10n-name="detail-private-browsing-learn-more">Подробнее</label>
detail-private-browsing-on =
    .label = Разрешить
    .tooltiptext = Включать при приватном просмотре
detail-private-browsing-off =
    .label = Не разрешать
    .tooltiptext = Отключать при приватном просмотре
detail-home =
    .label = Домашняя страница
detail-home-value =
    .value = { detail-home.label }
detail-repository =
    .label = Профиль дополнения
detail-repository-value =
    .value = { detail-repository.label }
detail-check-for-updates =
    .label = Проверить наличие обновлений
    .accesskey = в
    .tooltiptext = Проверить наличие обновлений для этого дополнения
detail-show-preferences =
    .label =
        { PLATFORM() ->
            [windows] Настройки
           *[other] Настройки
        }
    .accesskey =
        { PLATFORM() ->
            [windows] с
           *[other] с
        }
    .tooltiptext =
        { PLATFORM() ->
            [windows] Изменить настройки этого дополнения
           *[other] Изменить настройки этого дополнения
        }
detail-rating =
    .value = Рейтинг
addon-restart-now =
    .label = Перезапустить сейчас
disabled-unsigned-heading =
    .value = Некоторые дополнения были отключены
disabled-unsigned-description = Работа следующих дополнений в { -brand-short-name } не была проверена. Вы можете <label data-l10n-name="find-addons">найти им замену</label> или попросить разработчика произвести их проверку.
disabled-unsigned-learn-more = Узнайте больше о наших усилиях по обеспечению вашей безопасности в Интернете.
disabled-unsigned-devinfo = Разработчики, заинтересованные в проверке своих дополнений, могут прочесть наше <label data-l10n-name="learn-more">руководство</label>.
plugin-deprecation-description = Что-то отсутствует? { -brand-short-name } больше не поддерживает некоторые плагины. <label data-l10n-name="learn-more">Подробнее.</label>
legacy-warning-show-legacy = Показать устаревшие расширения
legacy-extensions =
    .value = Устаревшие расширения
legacy-extensions-description = Эти расширения не соответствуют текущим стандартам { -brand-short-name }, поэтому они были отключены. <label data-l10n-name="legacy-learn-more">Узнайте об изменениях в дополнениях</label>
private-browsing-description2 =
    { -brand-short-name } изменяет работу расширений в приватном режиме. Любые новые расширения, которые вы добавите в
    { -brand-short-name }, не будут запускаться по умолчанию в приватных окнах. Если вы не разрешите этого в настройках,
    расширение не будет работать во время приватного просмотра и не будет иметь доступа к вашей активности в Интернете.
    Мы внесли это изменение, чтобы сделать ваш приватный просмотр по-настоящему приватным.
    <label data-l10n-name="private-browsing-learn-more">Узнайте, как управлять настройками расширений.</label>
extensions-view-discopane =
    .name = Рекомендации
    .tooltiptext = { extensions-view-discopane.name }
extensions-view-recent-updates =
    .name = Недавние обновления
    .tooltiptext = { extensions-view-recent-updates.name }
extensions-view-available-updates =
    .name = Доступные обновления
    .tooltiptext = { extensions-view-available-updates.name }

## These are global warnings

extensions-warning-safe-mode-label =
    .value = В безопасном режиме все дополнения отключены.
extensions-warning-safe-mode-container =
    .tooltiptext = { extensions-warning-safe-mode-label.value }
extensions-warning-check-compatibility-label =
    .value = Проверка совместимости дополнений отключена. У вас могут иметься несовместимые дополнения.
extensions-warning-check-compatibility-container =
    .tooltiptext = { extensions-warning-check-compatibility-label.value }
extensions-warning-check-compatibility-enable =
    .label = Включить
    .tooltiptext = Включить проверку совместимости дополнений
extensions-warning-update-security-label =
    .value = Проверка безопасного обновления дополнений отключена. Обновления могут поставить вас под угрозу.
extensions-warning-update-security-container =
    .tooltiptext = { extensions-warning-update-security-label.value }
extensions-warning-update-security-enable =
    .label = Включить
    .tooltiptext = Включить проверку безопасного обновления дополнений

## Strings connected to add-on updates

extensions-updates-check-for-updates =
    .label = Проверить наличие обновлений
    .accesskey = о
extensions-updates-view-updates =
    .label = Показать недавние обновления
    .accesskey = к

# This menu item is a checkbox that toggles the default global behavior for
# add-on update checking.

extensions-updates-update-addons-automatically =
    .label = Автоматически обновлять дополнения
    .accesskey = в

## Specific add-ons can have custom update checking behaviors ("Manually",
## "Automatically", "Use default global behavior"). These menu items reset the
## update checking behavior for all add-ons to the default global behavior
## (which itself is either "Automatically" or "Manually", controlled by the
## extensions-updates-update-addons-automatically.label menu item).

extensions-updates-reset-updates-to-automatic =
    .label = Установить для всех дополнений режим автоматического обновления
    .accesskey = с
extensions-updates-reset-updates-to-manual =
    .label = Установить для всех дополнений режим ручного обновления
    .accesskey = с

## Status messages displayed when updating add-ons

extensions-updates-updating =
    .value = Обновление дополнений
extensions-updates-installed =
    .value = Ваши дополнения были обновлены.
extensions-updates-downloaded =
    .value = Обновления дополнений загружены.
extensions-updates-restart =
    .label = Перезапустить для завершения установки
extensions-updates-none-found =
    .value = Обновлений не найдено
extensions-updates-manual-updates-found =
    .label = Показать доступные обновления
extensions-updates-update-selected =
    .label = Установить обновления
    .tooltiptext = Установить доступные обновления из этого списка

## Extension shortcut management

manage-extensions-shortcuts =
    .label = Управление горячими клавишами расширений
    .accesskey = п
shortcuts-no-addons = У вас не включено ни одного расширения.
shortcuts-no-commands = У следующих расширений нет горячих клавиш:
shortcuts-input =
    .placeholder = Введите горячую клавишу
shortcuts-browserAction = Активировать расширение
shortcuts-pageAction = Активировать действие на странице
shortcuts-sidebarAction = Показать/скрыть боковую панель
shortcuts-modifier-mac = Добавьте Ctrl, Alt или ⌘
shortcuts-modifier-other = Добавьте Ctrl или Alt
shortcuts-invalid = Неверная комбинация
shortcuts-letter = Введите букву
shortcuts-system = Нельзя переопределить горячую клавишу { -brand-short-name }
# String displayed in warning label when there is a duplicate shortcut
shortcuts-duplicate = Дублирующееся сочетание клавиш
# String displayed when a keyboard shortcut is already assigned to more than one add-on
# Variables:
#   $shortcut (string) - Shortcut string for the add-on
shortcuts-duplicate-warning-message = { $shortcut } используется более одного раза. Дублирующиеся сочетания клавиш могут вызвать неожиданное поведение.
# String displayed when a keyboard shortcut is already used by another add-on
# Variables:
#   $addon (string) - Name of the add-on
shortcuts-exists = Уже используется { $addon }
shortcuts-card-expand-button =
    { $numberToShow ->
        [one] Показать ещё { $numberToShow }
        [few] Показать ещё { $numberToShow }
       *[many] Показать ещё { $numberToShow }
    }
shortcuts-card-collapse-button = Показать меньше
go-back-button =
    .tooltiptext = Вернуться назад

## Recommended add-ons page

# Explanatory introduction to the list of recommended add-ons. The action word
# ("recommends") in the final sentence is a link to external documentation.
discopane-intro =
    Расширения и темы — это как приложения для вашего браузера, они позволяют вам
    защищать пароли, загружать видео, находить скидки, блокировать раздражающую рекламу, изменять
    внешний вид браузера и многое другое. Эти небольшие программные продукты
    обычно разрабатываются сторонними разработчиками. Вот подборка расширений и тем, <a data-l10n-name="learn-more-trigger">рекомендуемых</a> { -brand-product-name } за свою исключительную
    безопасность, производительность и функциональность.
# Notice to make user aware that the recommendations are personalized.
discopane-notice-recommendations =
    Некоторые из этих рекомендаций персонализированы. Они основаны на других
    установленных вами расширениях, настройках профиля и статистике использования.
discopane-notice-learn-more = Подробнее
privacy-policy = Политика приватности
# Refers to the author of an add-on, shown below the name of the add-on.
# Variables:
#   $author (string) - The name of the add-on developer.
created-by-author = от <a data-l10n-name="author">{ $author }</a>
# Shows the number of daily users of the add-on.
# Variables:
#   $dailyUsers (number) - The number of daily users.
user-count = Пользователей: { $dailyUsers }
install-extension-button = Добавить в { -brand-product-name }
install-theme-button = Установить тему
# The label of the button that appears after installing an add-on. Upon click,
# the detailed add-on view is opened, from where the add-on can be managed.
manage-addon-button = Управление
find-more-addons = Найти больше дополнений

## Add-on actions

report-addon-button = Пожаловаться
remove-addon-button = Удалить
disable-addon-button = Отключить
enable-addon-button = Включить
expand-addon-button = Другие настройки
preferences-addon-button =
    { PLATFORM() ->
        [windows] Настройки
       *[other] Настройки
    }
details-addon-button = Подробности
release-notes-addon-button = Примечания к выпуску
permissions-addon-button = Разрешения
addons-enabled-heading = Включены
addons-disabled-heading = Отключены
extension-enabled-heading = Включены
extension-disabled-heading = Отключены
theme-enabled-heading = Включены
theme-disabled-heading = Отключены
plugin-enabled-heading = Включены
plugin-disabled-heading = Отключены
dictionary-enabled-heading = Включены
dictionary-disabled-heading = Отключены
locale-enabled-heading = Включены
locale-disabled-heading = Отключены
ask-to-activate-button = Включать по запросу
always-activate-button = Всегда включать
never-activate-button = Никогда не включать
addon-detail-author-label = Автор
addon-detail-version-label = Версия
addon-detail-last-updated-label = Последнее обновление
addon-detail-homepage-label = Домашняя страница
addon-detail-rating-label = Рейтинг
# The average rating that the add-on has received.
# Variables:
#   $rating (number) - A number between 0 and 5. The translation should show at most one digit after the comma.
five-star-rating =
    .title = Оценено на { NUMBER($rating, maximumFractionDigits: 1) } из 5
# This string is used to show that an add-on is disabled.
# Variables:
#   $name (string) - The name of the add-on
addon-name-disabled = { $name } (отключено)
# The number of reviews that an add-on has received on AMO.
# Variables:
#   $numberOfReviews (number) - The number of reviews received
addon-detail-reviews-link =
    { $numberOfReviews ->
        [one] { $numberOfReviews } отзыв
        [few] { $numberOfReviews } отзыва
       *[many] { $numberOfReviews } отзывов
    }

## Pending uninstall message bar

# Variables:
#   $addon (string) - Name of the add-on
pending-uninstall-description = <span data-l10n-name="addon-name">{ $addon }</span> было удалено.
pending-uninstall-undo-button = Отмена
addon-detail-updates-label = Разрешить автоматические обновления
addon-detail-updates-radio-default = По умолчанию
addon-detail-updates-radio-on = Включено
addon-detail-updates-radio-off = Отключено
addon-detail-update-check-label = Проверить наличие обновлений
install-update-button = Обновить
# This is the tooltip text for the private browsing badge in about:addons. The
# badge is the private browsing icon included next to the extension's name.
addon-badge-private-browsing-allowed =
    .title = Разрешено в приватных окнах
addon-detail-private-browsing-help = Когда разрешено, расширение будет иметь доступ к вашей активности в Интернете во время приватного просмотра. <a data-l10n-name="learn-more">Подробнее</a>
addon-detail-private-browsing-allow = Разрешить
addon-detail-private-browsing-disallow = Не разрешать
# This is the tooltip text for the recommended badge for an extension in about:addons. The
# badge is a small icon displayed next to an extension when it is recommended on AMO.
addon-badge-recommended =
    .title = Рекомендуемые
    .alt = Рекомендуемые
available-updates-heading = Доступные обновления
recent-updates-heading = Недавно обновлённые
release-notes-loading = Загрузка…
release-notes-error = При загрузке примечаний к выпуску возникли проблемы.
addon-permissions-empty = Это расширение не требует дополнительных разрешений
recommended-extensions-heading = Рекомендуемые расширения
recommended-themes-heading = Рекомендуемые темы
# A recommendation for the Firefox Color theme shown at the bottom of the theme
# list view. The "Firefox Color" name itself should not be translated.
recommended-theme-1 = Чувствуете прилив вдохновения? <a data-l10n-name="link">Создайте свою собственную тему с помощью Firefox Color.</a>
