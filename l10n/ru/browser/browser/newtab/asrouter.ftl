# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## These messages are used as headings in the recommendation doorhanger

cfr-doorhanger-extension-heading = Рекомендуемое расширение
cfr-doorhanger-feature-heading = Рекомендуемая функция
cfr-doorhanger-pintab-heading = Попробуйте: Закрепление вкладок



cfr-doorhanger-extension-sumo-link =
    .tooltiptext = Почему я это вижу
cfr-doorhanger-extension-cancel-button = Не сейчас
    .accesskey = е
cfr-doorhanger-extension-ok-button = Добавить
    .accesskey = а
cfr-doorhanger-pintab-ok-button = Закрепить эту вкладку
    .accesskey = З
cfr-doorhanger-extension-manage-settings-button = Управление настройками рекомендаций
    .accesskey = п
cfr-doorhanger-extension-never-show-recommendation = Не показывать мне эту рекомендацию
    .accesskey = е
cfr-doorhanger-extension-learn-more-link = Подробнее
# This string is used on a new line below the add-on name
# Variables:
#   $name (String) - Add-on author name
cfr-doorhanger-extension-author = от { $name }
# This is a notification displayed in the address bar.
# When clicked it opens a panel with a message for the user.
cfr-doorhanger-extension-notification = Рекомендация

## Add-on statistics
## These strings are used to display the total number of
## users and rating for an add-on. They are shown next to each other.

# Variables:
#   $total (Number) - The rating of the add-on from 1 to 5
cfr-doorhanger-extension-rating =
    .tooltiptext =
        { $total ->
            [one] { $total } звезда
            [few] { $total } звезды
           *[many] { $total } звёзд
        }
# Variables:
#   $total (Number) - The total number of users using the add-on
cfr-doorhanger-extension-total-users =
    { $total ->
        [one] { $total } пользователь
        [few] { $total } пользователя
       *[many] { $total } пользователей
    }
cfr-doorhanger-pintab-description = Получите лёгкий доступ к наиболее часто используемым вами сайтам. Оставляйте сайты открытыми (даже после перезапуска браузера).

## These messages are steps on how to use the feature and are shown together.

cfr-doorhanger-pintab-step1 = <b>Щёлкните правой кнопкой мыши</b> по вкладке, которую вы хотите закрепить.
cfr-doorhanger-pintab-step2 = Выберите <b>Закрепить вкладку</b> в меню.
cfr-doorhanger-pintab-step3 = Если на сайте произошло обновление, вы увидите синюю точку на закрепленной вкладке.
cfr-doorhanger-pintab-animation-pause = Приостановить
cfr-doorhanger-pintab-animation-resume = Возобновить

## Firefox Accounts Message

cfr-doorhanger-bookmark-fxa-header = Синхронизируйте свои закладки, где бы вы ни находились.
cfr-doorhanger-bookmark-fxa-body = Отличная находка! Не оставайтесь без этой закладки на своих мобильных устройствах. Создайте { -fxaccount-brand-name }.
cfr-doorhanger-bookmark-fxa-link-text = Синхронизировать закладки сейчас…
cfr-doorhanger-bookmark-fxa-close-btn-tooltip =
    .aria-label = Кнопка закрытия
    .title = Закрыть

## Protections panel

cfr-protections-panel-header = Сёрфите по Интернету без слежки
cfr-protections-panel-body = Храните свои данные при себе. { -brand-short-name } защитит вас от многих наиболее известных трекеров, которые следят за вашим поведением в Интернете.
cfr-protections-panel-link-text = Подробнее

## What's New toolbar button and panel

cfr-whatsnew-button =
    .label = Что нового
    .tooltiptext = Что нового
cfr-whatsnew-panel-header = Что нового

## Bookmark Sync

cfr-doorhanger-sync-bookmarks-header = Перенесите эту закладку на свой телефон
cfr-doorhanger-sync-bookmarks-body = Получайте доступ к закладкам, паролям, истории и другой информации на всех устройствах, где бы вы ни вошли в { -brand-product-name }.
cfr-doorhanger-sync-bookmarks-ok-button = Включить { -sync-brand-short-name(case: "accusative") }…
    .accesskey = ю

## Send Tab

cfr-doorhanger-send-tab-header = Читайте на ходу
cfr-doorhanger-send-tab-recipe-header = Возьмите этот рецепт на кухню
cfr-doorhanger-send-tab-body = Отправка вкладок позволяет вам легко поделиться этой ссылкой со своим телефоном или везде, где бы вы ни вошли в { -brand-product-name }.
cfr-doorhanger-send-tab-ok-button = Попробуйте отправить вкладку
    .accesskey = е

## Firefox Send

cfr-doorhanger-firefox-send-header = Безопасно поделитесь этим PDF-документом
cfr-doorhanger-firefox-send-body = Защитите свои важные документы от посторонних глаз благодаря сквозному шифрованию и ссылке, которая исчезнет, когда это потребуется.
cfr-doorhanger-firefox-send-ok-button = Попробуйте { -send-brand-name }
    .accesskey = й
