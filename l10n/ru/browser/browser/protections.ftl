# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

# Variables:
#   $count (Number) - Number of tracking events blocked.
graph-week-summary =
    { $count ->
        [one] { -brand-short-name } заблокировал { $count } трекер за последнюю неделю
        [few] { -brand-short-name } заблокировал { $count } трекера за последнюю неделю
       *[many] { -brand-short-name } заблокировал { $count } трекеров за последнюю неделю
    }
# Variables:
#   $count (Number) - Number of tracking events blocked.
#   $earliestDate (Number) - Unix timestamp in ms, representing a date. The
# earliest date recorded in the database.
graph-total-summary =
    { $count ->
        [one] { $count } трекер заблокирован с { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
        [few] { $count } трекера заблокировано с { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
       *[many] { $count } трекеров заблокировано с { DATETIME($earliestDate, day: "numeric", month: "long", year: "numeric") }
    }
# The terminology used to refer to categories of Content Blocking is also used in chrome/browser/browser.properties and should be translated consistently.
# "Standard" in this case is an adjective, meaning "default" or "normal".
# The category name in the <b> tag will be bold.
protection-header-details-standard = Уровень защиты — <b>Стандартная</b>
protection-header-details-strict = Уровень защиты — <b>Строгая</b>
protection-header-details-custom = Уровень защиты — <b>Персональная</b>
protection-report-page-title = Защита приватности
protection-report-content-title = Защита приватности
etp-card-title = Улучшенная защита от отслеживания
etp-card-content = Трекеры отслеживают вас в Интернете, чтобы собирать информацию о ваших привычках и интересах. { -brand-short-name } блокирует многие из этих трекеров и других вредоносных скриптов.
# This string is used to label the X axis of a graph. Other days of the week are generated via Intl.DateTimeFormat,
# capitalization for this string should match the output for your locale.
graph-today = Сегодня
# This string is used to describe the graph for screenreader users.
graph-legend-description = График содержит общее число трекеров каждого типа, заблокированных на этой неделе.
social-tab-title = Трекеры социальных сетей
social-tab-contant = Социальные сети размещают трекеры на других веб-сайтах, чтобы следить за тем, что вы делаете, видите и смотрите в Интернете. Это позволяет их владельцам узнавать о вас больше, чем вы указываете в своих профилях в социальных сетях. <a data-l10n-name="learn-more-link">Подробнее</a>
cookie-tab-title = Межсайтовые отслеживающие куки
cookie-tab-content = Такие куки ходят за вами с сайта на сайт для сбора информации о том, что вы делаете в Интернете. Они устанавливаются такими сторонними организациями, как рекламодатели и аналитические компании. Блокировка межсайтовых отслеживающих куков снижает количество рекламы, отслеживающей вас. <a data-l10n-name="learn-more-link">Подробнее</a>
tracker-tab-title = Отслеживающее содержимое
tracker-tab-content = Веб-сайты могут загружать внешнюю рекламу, видео и другой контент, содержащий отслеживающий код. Блокировка отслеживающего содержимого может помочь сайтам загружаться быстрее, но некоторые кнопки, формы и поля для входа могут не работать. <a data-l10n-name="learn-more-link">Подробнее</a>
fingerprinter-tab-title = Сборщики цифровых отпечатков
fingerprinter-tab-content = Сборщики цифровых отпечатков используют параметры вашего браузера и компьютера, чтобы создать ваш профиль. Используя этот цифровой отпечаток, они могут отслеживать вас на различных веб-сайтах. <a data-l10n-name="learn-more-link">Подробнее</a>
cryptominer-tab-title = Криптомайнеры
cryptominer-tab-content = Криптомайнеры используют вычислительные мощности вашей системы для добычи цифровых валют. Такие скрипты разряжают вашу батарею, замедляют работу компьютера и могут увеличить ваш счёт за электроэнергию. <a data-l10n-name="learn-more-link">Подробнее</a>
lockwise-title = Никогда больше не забывайте свои пароли
lockwise-title-logged-in = { -lockwise-brand-name }
lockwise-header-content = { -lockwise-brand-name } надёжно хранит пароли в вашем браузере.
lockwise-header-content-logged-in = Надёжно храните и синхронизируйте свои пароли со всеми вашими устройствами.
open-about-logins-button = Открыть в { -brand-short-name }
lockwise-no-logins-content = Загрузите приложение <a data-l10n-name="lockwise-inline-link">{ -lockwise-brand-name }</a>, чтобы держать свои пароли всегда с собой.
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
lockwise-passwords-stored =
    { $count ->
        [one] Пароль надёжно сохранён <a data-l10n-name="lockwise-how-it-works">Как это работает</a>
        [few] Пароля надёжно сохранены <a data-l10n-name="lockwise-how-it-works">Как это работает</a>
       *[many] Паролей надёжно сохранено <a data-l10n-name="lockwise-how-it-works">Как это работает</a>
    }
turn-on-sync = Включить { -sync-brand-short-name(case: "accusative") }…
    .title = Перейти в настройки синхронизации
manage-devices = Управление устройствами
# Variables:
#   $count (Number) - Number of devices connected with sync.
lockwise-sync-status =
    { $count ->
        [one] Синхронизация с { $count } другим устройством
        [few] Синхронизация с { $count } другими устройствами
       *[many] Синхронизация с { $count } другими устройствами
    }
lockwise-sync-not-syncing = Нет синхронизации с другими устройствами.
monitor-title = Следите за утечками данных
monitor-link = Как это работает
monitor-header-content = Попробуйте { -monitor-brand-name }, чтобы узнать не стали ли вы жертвой утечки данных и получать уведомления о новых утечках.
monitor-header-content-logged-in = { -monitor-brand-name } предупредит вас, если ваша информация будет затронута новой утечкой данных
monitor-sign-up = Подпишитесь на уведомления об утечках
auto-scan = Автоматически просканировано сегодня
# This string is displayed after a large numeral that indicates the total number
# of email addresses being monitored. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-monitored-addresses =
    { $count ->
        [one] Адрес электронной почты отслеживается.
        [few] Адреса электронной почты отслеживаются.
       *[many] Адресов электронной почты отслеживаются.
    }
# This string is displayed after a large numeral that indicates the total number
# of known data breaches. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-known-breaches =
    { $count ->
        [one] Известная утечка данных раскрыла вашу информацию.
        [few] Известных утечки данных раскрыли вашу информацию.
       *[many] Известных утечек данных раскрыли вашу информацию.
    }
# This string is displayed after a large numeral that indicates the total number
# of exposed passwords. Don’t add $count to
# your localization, because it would result in the number showing twice.
info-exposed-passwords =
    { $count ->
        [one] Раскрытый пароль во всех утечках.
        [few] Раскрытых пароля во всех утечках.
       *[many] Раскрытых паролей во всех утечках.
    }
full-report-link = Посмотреть полный отчёт на <a data-l10n-name="monitor-inline-link">{ -monitor-brand-name }</a>
# This string is displayed after a large numeral that indicates the total number
# of saved logins which may have been exposed. Don’t add $count to
# your localization, because it would result in the number showing twice.
password-warning =
    { $count ->
        [one] Сохранённый логин был скомпрометирован утечкой данных. Измените пароль для обеспечения безопасности. <a data-l10n-name="lockwise-link">Посмотреть сохранённые логины</a>
        [few] Сохранённых логина были скомпрометированы утечкой данных. Измените пароль для обеспечения безопасности. <a data-l10n-name="lockwise-link">Посмотреть сохранённые логины</a>
       *[many] Сохранённых логинов были скомпрометированы утечкой данных. Измените пароль для обеспечения безопасности. <a data-l10n-name="lockwise-link">Посмотреть сохранённые логины</a>
    }
# This is the title attribute describing the graph report's link to about:settings#privacy
go-to-privacy-settings = Перейти в настройки приватности
# This is the title attribute describing the Lockwise card's link to about:logins
go-to-saved-logins = Перейти к сохранённым логинам

## The title attribute is used to display the type of protection.
## The aria-label is spoken by screen readers to make the visual graph accessible to blind users.
##
## Variables:
##   $count (Number) - Number of specific trackers
##   $percentage (Number) - Percentage this type of tracker contributes to the whole graph

bar-tooltip-social =
    .title = Трекеры социальных сетей
    .aria-label =
        { $count ->
            [one] { $count } трекер социальных сетей ({ $percentage }%)
            [few] { $count } трекера социальных сетей ({ $percentage }%)
           *[many] { $count } трекеров социальных сетей ({ $percentage }%)
        }
bar-tooltip-cookie =
    .title = Межсайтовые отслеживающие куки
    .aria-label =
        { $count ->
            [one] { $count } межсайтовая отслеживающая кука ({ $percentage }%)
            [few] { $count } межсайтовых отслеживающих куки ({ $percentage }%)
           *[many] { $count } межсайтовых отслеживающих куков ({ $percentage }%)
        }
bar-tooltip-tracker =
    .title = Отслеживающее содержимое
    .aria-label =
        { $count ->
            [one] { $count } отслеживающее содержимое ({ $percentage }%)
            [few] { $count } отслеживающих содержимых ({ $percentage }%)
           *[many] { $count } отслеживающих содержимых ({ $percentage }%)
        }
bar-tooltip-fingerprinter =
    .title = Сборщики цифровых отпечатков
    .aria-label =
        { $count ->
            [one] { $count } сборщик цифровых отпечатков ({ $percentage }%)
            [few] { $count } сборщика цифровых отпечатков ({ $percentage }%)
           *[many] { $count } сборщиков цифровых отпечатков ({ $percentage }%)
        }
bar-tooltip-cryptominer =
    .title = Криптомайнеры
    .aria-label =
        { $count ->
            [one] { $count } криптомайнер ({ $percentage }%)
            [few] { $count } криптомайнера ({ $percentage }%)
           *[many] { $count } криптомайнеров ({ $percentage }%)
        }
