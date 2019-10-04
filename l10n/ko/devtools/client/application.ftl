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
serviceworker-list-header = 서비스 워커
# Text displayed next to the list of Service Workers to encourage users to check out
# about:debugging to see all registered Service Workers.
serviceworker-list-aboutdebugging = <a>about:debugging</a>을 열어서 다른 도메인의 서비스 워커 보기
# Text for the button to unregister a Service Worker. Displayed for active Service Workers.
serviceworker-worker-unregister = 등록 취소
# Text for the debug link displayed for an already started Service Worker. Clicking on the
# link opens a new devtools toolbox for this service worker. The title attribute is only
# displayed when the link is disabled.
serviceworker-worker-debug = 디버그
    .title = 실행되고 있는 서비스 워커만 디버그할 수 있음
# Text for the start link displayed for a registered but not running Service Worker.
# Clicking on the link will attempt to start the service worker.
serviceworker-worker-start = 시작
# Text for the debug link displayed for an already started Service Worker, when we
# are in multi e10s mode, which effectively disables this link.
serviceworker-worker-debug-forbidden = 디버그
    .title = 다중 e10s가 비활성화 된 경우에만 서비스 워커를 디버깅 할 수 있습니다.
# Text for the start link displayed for a registered but not running Service Worker.
# Clicking on the link will attempt to start the service worker.
serviceworker-worker-start2 = 시작
    .title = 다중 e10s가 비활성화 된 경우에만 서비스 워커를 시작할 수 있습니다.
# Text displayed for the updated time of the service worker. The <time> element will
# display the last update time of the service worker script.
serviceworker-worker-updated = <time>{ DATETIME($date, month: "long", year: "numeric", day: "numeric", hour: "numeric", minute: "numeric", second: "numeric") }</time>에 업데이트됨
# Text displayed next to the URL for the source of the service worker (e-g. "Source my/path/to/worker-js")
serviceworker-worker-source = 소스
# Text displayed next to the current status of the service worker.
serviceworker-worker-status = 상태

## Service Worker status strings: all serviceworker-worker-status-* strings are also
## defined in aboutdebugging.properties and should be synchronized with them.

# Service Worker status. A running service worker is registered, currently executed, can
# be debugged and stopped.
serviceworker-worker-status-running = 실행중
# Service Worker status. A stopped service worker is registered but not currently active.
serviceworker-worker-status-stopped = 중지됨
# Service Worker status. A registering service worker is not yet registered and cannot be
# started or debugged.
serviceworker-worker-status-registering = 등록중
# Text displayed when no service workers are visible for the current page. Clicking on the
# link will open https://developer-mozilla-org/docs/Web/API/Service_Worker_API/Using_Service_Workers
serviceworker-empty-intro = 여기서 검사하려면 서비스 워커를 등록해야 합니다. <a>자세히 보기</a>
# Text displayed when there are no Service Workers to display for the current page,
# introducing hints to debug Service Worker issues.
serviceworker-empty-suggestions = 현재 페이지에 서비스 워커가 있어야 한다면 몇가지 시도해 볼 수 있습니다
# Suggestion to check for errors in the Console to investigate why a service worker is not
# registered. Clicking on the link opens the webconsole.
serviceworker-empty-suggestions-console = 콘솔의 오류를 확인해 보세요. <a>콘솔 열기</a>
# Suggestion to use the debugger to investigate why a service worker is not registered.
# Clicking on the link will switch from the Application panel to the debugger.
serviceworker-empty-suggestions-debugger = 서비스 워커 등록과정을 살펴보고 예외를 확인해 보세요. <a>디버거 열기</a>
# Suggestion to go to about:debugging in order to see Service Workers for all domains.
# Clicking on the link will open about:debugging in a new tab.
serviceworker-empty-suggestions-aboutdebugging = 다른 도메인의 서비스 워커를 조사해 보세요. <a>about:debugging 열기</a>
# Text displayed when no manifest was found for the current page.
manifest-empty-intro = 검사할 매니페스트가 없습니다.
