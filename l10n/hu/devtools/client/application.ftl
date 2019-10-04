# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


### These strings are used inside the Application panel which is available
### by setting the preference `devtools-application-enabled` to true.


### The correct localization of this file might be to keep it in English, or another
### language commonly spoken among web developers. You want to make that choice consistent
### across the developer tools. A good criteria is the language in which you'd find the
### best documentation on web development on the web.

# Header for the list of Service Workers displayed in the application panel for the current page.
serviceworker-list-header = Service Workerek
# Text displayed next to the list of Service Workers to encourage users to check out
# about:debugging to see all registered Service Workers.
serviceworker-list-aboutdebugging = Nyissa meg az <a>about:debugging</a> oldalt a más tartományokból származó Service Workerek megjelenítéséhez
# Text for the button to unregister a Service Worker. Displayed for active Service Workers.
serviceworker-worker-unregister = Regisztráció törlése
# Text for the debug link displayed for an already started Service Worker. Clicking on the
# link opens a new devtools toolbox for this service worker. The title attribute is only
# displayed when the link is disabled.
serviceworker-worker-debug = Hibakeresés
    .title = Csak a futó service workerekben lehet hibát keresni
# Text for the start link displayed for a registered but not running Service Worker.
# Clicking on the link will attempt to start the service worker.
serviceworker-worker-start = Indítás
# Text for the debug link displayed for an already started Service Worker, when we
# are in multi e10s mode, which effectively disables this link.
serviceworker-worker-debug-forbidden = Hibakeresés
    .title = A service workerekben csak akkor lehet hibát keresni, ha a többszálas e10s ki van kapcsolva
# Text for the start link displayed for a registered but not running Service Worker.
# Clicking on the link will attempt to start the service worker.
serviceworker-worker-start2 = Indítás
    .title = A service workerek csak akkor indíthatóak el, ha a többszálas e10s ki van kapcsolva
# Text displayed for the updated time of the service worker. The <time> element will
# display the last update time of the service worker script.
serviceworker-worker-updated = Frissítve: <time>{ DATETIME($date, month: "long", year: "numeric", day: "numeric", hour: "numeric", minute: "numeric", second: "numeric") }</time>
# Text displayed next to the URL for the source of the service worker (e-g. "Source my/path/to/worker-js")
serviceworker-worker-source = Forrás
# Text displayed next to the current status of the service worker.
serviceworker-worker-status = Állapot

## Service Worker status strings: all serviceworker-worker-status-* strings are also
## defined in aboutdebugging.properties and should be synchronized with them.

# Service Worker status. A running service worker is registered, currently executed, can
# be debugged and stopped.
serviceworker-worker-status-running = Fut
# Service Worker status. A stopped service worker is registered but not currently active.
serviceworker-worker-status-stopped = Leállítva
# Service Worker status. A registering service worker is not yet registered and cannot be
# started or debugged.
serviceworker-worker-status-registering = Regisztrálás
# Text displayed when no service workers are visible for the current page. Clicking on the
# link will open https://developer-mozilla-org/docs/Web/API/Service_Worker_API/Using_Service_Workers
serviceworker-empty-intro = Regisztrálnia kell a Service Workert, hogy itt vizsgálhassa. <a>További tudnivalók</a>
# Text displayed when there are no Service Workers to display for the current page,
# introducing hints to debug Service Worker issues.
serviceworker-empty-suggestions = Ha a jelenlegi oldalon lennie kellene egy service workernek, akkor itt van néhány dolog amit megpróbálhat
# Suggestion to check for errors in the Console to investigate why a service worker is not
# registered. Clicking on the link opens the webconsole.
serviceworker-empty-suggestions-console = Keressen hibát a konzolban. <a>Nyissa meg a konzolt</a>
# Suggestion to use the debugger to investigate why a service worker is not registered.
# Clicking on the link will switch from the Application panel to the debugger.
serviceworker-empty-suggestions-debugger = Lépjen végig a Service Worker regisztráción, és keressen kivételeket. <a>Nyissa meg a hibakeresőt</a>
# Suggestion to go to about:debugging in order to see Service Workers for all domains.
# Clicking on the link will open about:debugging in a new tab.
serviceworker-empty-suggestions-aboutdebugging = Vizsgálja meg a más tartományokból származó Service Workereket. <a>Nyissa meg az about:debugging oldalt</a>
# Text displayed while we are loading the manifest file
manifest-loading = Leírófájl betöltése…
# Text displayed when the manifest has been successfully loaded
manifest-loaded-ok = Leírófájl betöltve.
# Text displayed when there has been an error while trying to load the manifest
manifest-loaded-error = Hiba történt a leírófájl betöltésekor:
# Text displayed when the page has no manifest available
manifest-non-existing = Nem található vizsgálható leírófájl.
