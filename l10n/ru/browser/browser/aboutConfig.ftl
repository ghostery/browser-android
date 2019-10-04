# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# An old map warning, see https://en.wikipedia.org/wiki/Here_be_dragons
about-config-warning-title = Здесь обитают драконы!
about-config-warning-text = Изменение этих настроек может привести к ухудшению стабильности, безопасности и производительности данного приложения. Вам следует изменять что-либо только в том случае, если вы уверены в том, что делаете.
about-config-warning-checkbox = Напомнить мне об этом в следующий раз!
about-config-warning-button = Я принимаю на себя риск
about-config-title = about:config
about-config2-title = Дополнительные настройки
about-config-search-input =
    .placeholder = Поиск
about-config-show-all = Показать все
about-config-pref-add = Добавить
about-config-pref-toggle = Переключить
about-config-pref-edit = Изменить
about-config-pref-save = Сохранить
about-config-pref-reset = Сбросить
about-config-pref-delete = Удалить

## Labels for the type selection radio buttons shown when adding preferences.

about-config-pref-add-type-boolean = Логическое
about-config-pref-add-type-number = Число
about-config-pref-add-type-string = Строка

## Preferences with a non-default value are differentiated visually, and at the
## same time the state is made accessible to screen readers using an aria-label
## that won't be visible or copied to the clipboard.
##
## Variables:
##   $value (String): The full value of the preference.

about-config-pref-accessible-value-default =
    .aria-label = { $value } (по умолчанию)
about-config-pref-accessible-value-custom =
    .aria-label = { $value } (изменено пользователем)
