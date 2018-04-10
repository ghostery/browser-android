# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

do-not-track-description = Передавать сайтам сигнал “Не отслеживать”, означающий, чтобы вы не хотите быть отслеживаемыми
do-not-track-learn-more = Подробнее
do-not-track-option-default =
    .label = Только при использовании защиты от отслеживания
do-not-track-option-always =
    .label = Всегда
pref-page =
    .title = { PLATFORM() ->
            [windows] Настройки
           *[other] Настройки
        }
# This is used to determine the width of the search field in about:preferences,
# in order to make the entire placeholder string visible
#
# Notice: The value of the `.style` attribute is a CSS string, and the `width`
# is the name of the CSS property. It is intended only to adjust the element's width.
# Do not translate.
search-input =
    .style = width: 15.4em
pane-general-title = Основные
category-general =
    .tooltiptext = { pane-general-title }
pane-search-title = Поиск
category-search =
    .tooltiptext = { pane-search-title }
pane-privacy-title = Приватность и Защита
category-privacy =
    .tooltiptext = { pane-privacy-title }
# The word "account" can be translated, do not translate or transliterate "Firefox".
pane-sync-title = Аккаунт Firefox
category-sync =
    .tooltiptext = { pane-sync-title }
help-button-label = Поддержка { -brand-short-name }
focus-search =
    .key = f
close-button =
    .aria-label = Закрыть

## Browser Restart Dialog

feature-enable-requires-restart = Для включения этого режима необходимо перезапустить { -brand-short-name }.
feature-disable-requires-restart = Для отключения этого режима необходимо перезапустить { -brand-short-name }.
should-restart-title = Перезапуск { -brand-short-name }
should-restart-ok = Перезапустить { -brand-short-name } сейчас
revert-no-restart-button = Отменить изменение
restart-later = Перезапустить позже
