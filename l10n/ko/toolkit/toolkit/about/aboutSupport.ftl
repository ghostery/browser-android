# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

page-title = 문제 해결 정보
page-subtitle =
    현재 페이지는 여러분이 { -brand-short-name } 사용 상 문제점을 해결 할 수 있는
    다양한 기술적 정보를 담고 있습니다. 여러분이 문제의 해결을 얻기 위해서는
    이 정보를 토대로 <a data-l10n-name="support-link">고객 지원 사이트</a>에 문의하시기 바랍니다.
crashes-title = 충돌 보고서
crashes-id = 보고 ID
crashes-send-date = 전송됨
crashes-all-reports = 모든 충돌 보고서
crashes-no-config = 이 애플리케이션은 충돌 보고서를 표시하도록 설정되어 있지 않습니다.
extensions-title = 확장 기능
extensions-name = 이름
extensions-enabled = 사용함
extensions-version = 버전
extensions-id = ID
security-software-title = 보안 소프트웨어
security-software-type = 유형
security-software-name = 이름
security-software-antivirus = 바이러스 방지 프로그램
security-software-antispyware = 스파이웨어 방지 프로그램
security-software-firewall = 방화벽
features-title = { -brand-short-name } 주요 기능
features-name = 이름
features-version = 버전
features-id = ID
processes-title = 원격 프로세스
processes-type = 유형
processes-count = 수
app-basics-title = 응용 프로그램 기본 정보
app-basics-name = 이름
app-basics-version = 버전
app-basics-build-id = 빌드 ID
app-basics-update-channel = 업데이트 채널
app-basics-update-dir =
    { PLATFORM() ->
        [linux] 업데이트 디렉토리
       *[other] 업데이트 폴더
    }
app-basics-update-history = 업데이트 기록
app-basics-show-update-history = 업데이트 기록 표시
# Represents the path to the binary used to start the application.
app-basics-binary = 애플리케이션 바이너리
app-basics-profile-dir =
    { PLATFORM() ->
        [linux] 프로필 저장 폴더
       *[other] 프로필 폴더
    }
app-basics-enabled-plugins = 사용 중인 플러그인
app-basics-build-config = 빌드 설정
app-basics-user-agent = 사용자 에이전트
app-basics-os = OS
app-basics-memory-use = 메모리 사용
app-basics-performance = 성능
app-basics-service-workers = 등록된 Service Workers
app-basics-profiles = 프로필
app-basics-launcher-process-status = 런처 프로세스
app-basics-multi-process-support = 다중프로세스 윈도우
app-basics-process-count = 웹 콘텐츠 프로세스
app-basics-remote-processes-count = 원격 프로세스
app-basics-enterprise-policies = 엔터프라이즈 정책
app-basics-location-service-key-google = Google 위치 서비스 키
app-basics-safebrowsing-key-google = Google 세이프 브라우징 키
app-basics-key-mozilla = Mozilla 위치 서비스 키
app-basics-safe-mode = 안전 모드
show-dir-label =
    { PLATFORM() ->
        [macos] Finder에 표시
        [windows] 폴더 열기
       *[other] 디렉토리 열기
    }
modified-key-prefs-title = 중요한 변경된 환경 설정
modified-prefs-name = 이름
modified-prefs-value = 값
user-js-title = user.js 환경 설정
user-js-description = 현재 프로필 폴더에는 <a data-l10n-name="user-js-link">user.js 파일</a>이 있습니다. 이는 { -brand-short-name }에 의해 초기 설정이 아닌 사용자 설정을 담고 있습니다.
locked-key-prefs-title = 중요한 잠긴 환경 설정
locked-prefs-name = 이름
locked-prefs-value = 값
graphics-title = 그래픽
graphics-features-title = 기능
graphics-diagnostics-title = 진단
graphics-failure-log-title = 실패 로그
graphics-gpu1-title = GPU #1
graphics-gpu2-title = GPU #2
graphics-decision-log-title = 의사결정 로그
graphics-crash-guards-title = 크래시 보호 비활성화 기능
graphics-workarounds-title = 회피방법
# Windowing system in use on Linux (e.g. X11, Wayland).
graphics-window-protocol = 윈도우 프로토콜
place-database-title = 위치 데이터베이스
place-database-integrity = 무결성
place-database-verify-integrity = 무결성 확인
js-title = JavaScript
js-incremental-gc = 인크리멘탈 GC
a11y-title = 접근성
a11y-activated = 활성화
a11y-force-disabled = 접근성 끄기
a11y-handler-used = 접근 가능한 핸들러 사용됨
a11y-instantiator = 접근성 기능 사용 앱
library-version-title = 라이브러리 버전
copy-text-to-clipboard-label = 클립보드에 텍스트 복사하기
copy-raw-data-to-clipboard-label = 클립보드에 원본 데이터 복사하기
sandbox-title = 샌드박스
sandbox-sys-call-log-title = 거절된 시스템 콜
sandbox-sys-call-index = #
sandbox-sys-call-age = 초 전
sandbox-sys-call-pid = PID
sandbox-sys-call-tid = TID
sandbox-sys-call-proc-type = 프로세스 타입
sandbox-sys-call-number = 시스템 콜
sandbox-sys-call-args = 인자
safe-mode-title = 안전 모드 시도
restart-in-safe-mode-label = 부가 기능 없이 재시작…
media-title = 미디어
media-output-devices-title = 출력 기기
media-input-devices-title = 입력 기기
media-device-name = 이름
media-device-group = 그룹
media-device-vendor = 벤더
media-device-state = 주
media-device-preferred = 선호
media-device-format = 형식
media-device-channels = 채널
media-device-rate = 비율
media-device-latency = 지연
intl-title = 국제화 & 현지화
intl-app-title = 응용 프로그램 설정
intl-locales-requested = 요청된 언어
intl-locales-available = 사용 가능한 언어
intl-locales-supported = 응용 프로그램 언어
intl-locales-default = 기본 언어
intl-os-title = 운영 체제
intl-os-prefs-system-locales = 시스템 언어
intl-regional-prefs = 지역 설정
# Variables
# $days (Integer) - Number of days of crashes to log
report-crash-for-days = 지난 { $days }일간의 충돌 보고서
# Variables
# $minutes (integer) - Number of minutes since crash
crashes-time-minutes = { $minutes }분 전
# Variables
# $hours (integer) - Number of hours since crash
crashes-time-hours = { $hours }시간 전
# Variables
# $days (integer) - Number of days since crash
crashes-time-days = { $days }일 전
# Variables
# $reports (integer) - Number of pending reports
pending-reports = 모든 충돌 보고서(기간안에 있는 { $reports }개의 보류된 충돌 보고서 포함)
raw-data-copied = 원본 데이터를 클립보드에 복사함
text-copied = 문자열을 클립보드에 복사함

## The verb "blocked" here refers to a graphics feature such as "Direct2D" or "OpenGL layers".

blocked-driver = 그래픽 드라이버 버전이 지원하지 않기 때문에 그리기 지원을 차단하였습니다.
blocked-gfx-card = 그래픽 카드 드라이버의 미해결 문제로 인해 그리기 지원을 차단하였습니다.
blocked-os-version = 운영 체제 버전에서 지원하지 않기 때문에 그리기 지원을 차단하였습니다.
blocked-mismatched-version = 레지스트리와 DLL간의 그래픽 드라이버 버전이 일치하지 않아서 차단하였습니다.
# Variables
# $driverVersion - The graphics driver version string
try-newer-driver = 그래픽 드라이버는 그리기 지원이 차단되었습니다.  그래픽 드라이버를 버전 { $driverVersion } 이상으로 업데이트해 보십시오.
# "ClearType" is a proper noun and should not be translated. Feel free to leave English strings if
# there are no good translations, these are only used in about:support
clear-type-parameters = ClearType 매개 변수
compositing = 합성
hardware-h264 = 하드웨어 H264 디코딩
main-thread-no-omtc = 주 쓰레드, OMTC 아님
yes = 네
no = 아니오
unknown = 알 수 없음
virtual-monitor-disp = 가상 모니터 디스플레이

## The following strings indicate if an API key has been found.
## In some development versions, it's expected for some API keys that they are
## not found.

found = 찾음
missing = 없음
gpu-process-pid = GPUProcessPid
gpu-process = GPUProcess
gpu-description = 설명
gpu-vendor-id = 벤더 ID
gpu-device-id = 기기 ID
gpu-subsys-id = 하위시스템 ID
gpu-drivers = 드라이버
gpu-ram = RAM
gpu-driver-vendor = 드라이버 공급 업체
gpu-driver-version = 드라이버 버전
gpu-driver-date = 드라이버 날짜
gpu-active = 활성화
webgl1-wsiinfo = WebGL 1 드라이버 WSI 정보
webgl1-renderer = WebGL 1 드라이버 렌더러
webgl1-version = WebGL 1 드라이버 버전
webgl1-driver-extensions = WebGL 1 드라이버 확장 기능
webgl1-extensions = WebGL 1 확장 기능
webgl2-wsiinfo = WebGL 2 드라이버 WSI 정보
webgl2-renderer = WebGL2 드라이버 랜더러
webgl2-version = WebGL 2 드라이버 버전
webgl2-driver-extensions = WebGL 2 드라이버 확장 기능
webgl2-extensions = WebGL 2 확장 기능
blocklisted-bug = 알려진 문제로 차단된 목록
# Variables
# $bugNumber (string) - String of bug number from Bugzilla
bug-link = 버그 { $bugNumber }
# Variables
# $failureCode (string) - String that can be searched in the source tree.
unknown-failure = 차단됨; 실패 코드 { $failureCode }
d3d11layers-crash-guard = D3D11 컴포지터
d3d11video-crash-guard = D3D11 동영상 디코더
d3d9video-crash-buard = D3D9 동영상 디코더
glcontext-crash-guard = OpenGL
reset-on-next-restart = 다음 재시작에 초기화
gpu-process-kill-button = GPU 프로세스 종료
gpu-device-reset = 기기 초기화
gpu-device-reset-button = 기기 초기화 시작
uses-tiling = 타일링 사용
content-uses-tiling = 타일링(콘텐츠) 사용
off-main-thread-paint-enabled = 주 쓰레드 페인팅 끄기 활성화됨
off-main-thread-paint-worker-count = 주 쓰레드 페인팅 워커 카운트 끔
low-end-machine = 저성능 기기 감지
target-frame-rate = 목표 프레임 레이트
audio-backend = 오디오 백엔드
max-audio-channels = 최대 채널
channel-layout = 선호하는 채널 레이아웃
sample-rate = 선호하는 샘플 비율
min-lib-versions = 이용 중인 최소 버젼
loaded-lib-versions = 이용 중 버전
has-seccomp-bpf = Seccomp-BPF (시스템 호출 필터링)
has-seccomp-tsync = Seccomp 쓰레드 동기화
has-user-namespaces = 사용자 네임스페이스
has-privileged-user-namespaces = 권한있는 프로세스를 위한 사용자 네임스페이스
can-sandbox-content = 콘텐트 프로세스 샌드박싱
can-sandbox-media = 미디어 플러그인 샌드박싱
content-sandbox-level = 콘텐츠 프로세스 샌드박스 수준
effective-content-sandbox-level = 효과적인 콘텐츠 프로세스 샌드박스 수준
sandbox-proc-type-content = 내용
sandbox-proc-type-file = 파일 내용
sandbox-proc-type-media-plugin = 미디어 플러그인
sandbox-proc-type-data-decoder = 데이터 디코더
launcher-process-status-0 = 활성화됨
launcher-process-status-1 = 실패로 비활성화됨
launcher-process-status-2 = 강제로 비활성화됨
launcher-process-status-unknown = 알 수 없는 상태
# Variables
# $remoteWindows (integer) - Number of remote windows
# $totalWindows (integer) - Number of total windows
multi-process-windows = { $remoteWindows }/{ $totalWindows }
multi-process-status-0 = 사용자에 의해 활성화됨
multi-process-status-1 = 기본값에 의해 활성화
multi-process-status-2 = 비활성화
multi-process-status-4 = 접근성 도구에 의해 비활성화
multi-process-status-6 = 지원되지 않는 텍스트 입력으로 인한 비활성화
multi-process-status-7 = 부가기능에 의해서 비활성화
multi-process-status-8 = 강제로 비활성화됨
multi-process-status-unknown = 알 수 없는 상태
async-pan-zoom = 비동기 팬/확대
apz-none = 없음
wheel-enabled = 휠 입력 활성화됨
touch-enabled = 터치 입력 활성화됨
drag-enabled = 스크롤바 끌기 활성화됨
keyboard-enabled = 키보드 활성화됨
autoscroll-enabled = 자동 스크롤 활성화됨

## Variables
## $preferenceKey (string) - String ID of preference

wheel-warning = 다음의 지원되지 않는 설정때문에 비동기 휠 입력이 비활성화됨: { $preferenceKey }
touch-warning = 다음의 지원되지 않는 설정때문에 비동기 터치 입력이 비활성화됨: { $preferenceKey }

## Strings representing the status of the Enterprise Policies engine.

policies-inactive = 비활성화
policies-active = 활성화
policies-error = 오류
