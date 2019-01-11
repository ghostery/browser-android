# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.


## The Enterprise Policies feature is aimed at system administrators
## who want to deploy these settings across several Firefox installations
## all at once. This is traditionally done through the Windows Group Policy
## feature, but the system also supports other forms of deployment.
## These are short descriptions for individual policies, to be displayed
## in the documentation section in about:policies.

policy-AppUpdateURL = 커스텀 앱 업데이트 URL을 설정합니다.
policy-Authentication = 통합 인증을 지원하는 웹사이트를 위한 설정을 합니다.
policy-BlockAboutAddons = 부가기능 관리자(about:addons) 접근을 차단합니다.
policy-BlockAboutConfig = about:config 페이지 접근을 차단합니다.
policy-BlockAboutProfiles = about:profiles 페이지 접근을 차단합니다.
policy-BlockAboutSupport = about:support 페이지 접근을 차단합니다.
policy-Bookmarks = 북마크 도구 막대나 북마크 메뉴, 안의 특정 폴더에 북마크를 생성합니다.
policy-Certificates = 기본 탑재 인증서 사용 여부입니다. 지금은 윈도우에만 적용됩니다.
policy-CertificatesDescription = 인증서를 추가하거나 기본 제공 인증서를 사용합니다.
policy-Cookies = 웹사이트의 쿠키 설정을 허용하거나 거부합니다.
policy-DisableAppUpdate = 브라우저가 업데이트 되지 않게 합니다.
policy-DisableBuiltinPDFViewer = { -brand-short-name }에 탑재된 PDF 뷰어인 PDF.js를 비활성화 합니다.
policy-DisableDeveloperTools = 개발자 도구 접근을 차단합니다.
policy-DisableFeedbackCommands = 도움말 메뉴의 의견 보내기 명령(의견과 가짜 사이트 신고)을 비활성화합니다.
policy-DisableFirefoxAccounts = 동기화를 포함한 { -fxaccount-brand-name } 기반의 서비스를 비활성화 합니다.
# Firefox Screenshots is the name of the feature, and should not be translated.
policy-DisableFirefoxScreenshots = Firefox 스크린샷 기능을 비활성화 합니다.
policy-DisableFirefoxStudies = { -brand-short-name } 연구 실행을 막습니다.
policy-DisableForgetButton = Forget 버튼 접근을 막습니다.
policy-DisableFormHistory = 검색과 서식 기록을 저장하지 않습니다.
policy-DisableMasterPasswordCreation = True 값이면 마스터 비밀번호를 만들 수 없습니다.
policy-DisablePocket = Pocket에 웹사이트 저장하는 기능을 비활성화 합니다.
policy-DisablePrivateBrowsing = 사생활 보호 모드를 비활성화 합니다.
policy-DisableProfileImport = 메뉴에서 다른 브라우저의 데이터를 가져오는 명령을 비활성화 합니다.
policy-DisableProfileRefresh = about:support 페이지의 { -brand-short-name } 새로설정 버튼을 비활성화 합니다.
policy-DisableSafeMode = 안전 모드로 재시작하는 기능을 비활성화 합니다. 참고: 그룹 정책을 사용해서 쉬프트키를 눌러서 안전 모드로 들어가는 방법을 비활성화 하는 것은 윈도우에서만 가능합니다.
policy-DisableSecurityBypass = 사용자가 특정 보안 경고를 무시할 수 없게 합니다.
policy-DisableSetDesktopBackground = 이미지를 바탕화면 배경으로 설정하는 메뉴 명령을 비활성화 합니다.
policy-DisableSetAsDesktopBackground = 데스크톱 배경을 이미지로 설정하는 메뉴 명령을 비활성화 합니다.
policy-DisableSystemAddonUpdate = 브라우저가 시스템 부가 기능을 설치하고 업데이트 하지 못하게 합니다.
policy-DisableTelemetry = 원격 측정을 끕니다.
policy-DisplayBookmarksToolbar = 기본으로 북마크 도구 막대를 표시합니다.
policy-DisplayMenuBar = 기본으로 메뉴 막대를 표시합니다.
policy-DNSOverHTTPS = HTTPS를 통한 DNS를 설정합니다.
policy-DontCheckDefaultBrowser = 시작할 때 기본 브라우저 확인을 하지 않습니다.
# “lock” means that the user won’t be able to change this setting
policy-EnableTrackingProtection = 콘텐츠 차단을 활성화 하거나 비활성화 하고 선택적으로 고정합니다.
# A “locked” extension can’t be disabled or removed by the user. This policy
# takes 3 keys (“Install”, ”Uninstall”, ”Locked”), you can either keep them in
# English or translate them as verbs. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#extensions-machine-only
policy-Extensions = 확장기능을 설치하거나 제거, 잠급니다. 설치 옵션은 URL이나 경로를 파라메터로 받습니다. 설치 제거와 잠금 옵션은 확장기능의 ID를 받습니다.
policy-FlashPlugin = 플래시 플러그인의 사용을 허용하거나 거부합니다.
policy-HardwareAcceleration = False 값이면 하드웨어 가속 기능을 끕니다.
# “lock” means that the user won’t be able to change this setting
policy-Homepage = 홈페이지를 설정하고 선택적으로 잠급니다.
policy-InstallAddonsPermission = 특정 웹사이트가 부가 기능을 설치할 수 있게 허용합니다.
policy-NoDefaultBookmarks = { -brand-short-name } 기본 북마크 번들과 스마트 북마크(자주 방문, 최근 태그)가 생성되지 않게 합니다. 참고: 이 정책은 프로필을 처음으로 실행하기 전에만 효과적입니다.
policy-OfferToSaveLogins = { -brand-short-name }가 로그인과 비밀번호 기억을 제공하도록 허용하는 설정을 강제합니다. True와 false 값을 사용할 수 있습니다.
policy-OverrideFirstRunPage = 처음 시작 페이지 설정을 재정의 합니다. 처음 시작 페이지를 비활성화 하려면 이 정책을 빈칸으로 설정하세요.
policy-OverridePostUpdatePage = "새기능" 업데이트 후 페이지를 재정의 합니다. 업데이트 후 페이지를 비활성화 하려면 이 정책을 빈칸으로 설정하세요.
policy-Permissions = 카메라나 마이크, 위치, 알림 권한을 설정합니다.
policy-PopupBlocking = 기본으로 특정 웹사이트가 팝업을 보여주도록 허용합니다.
policy-Proxy = 프록시 설정을 구성합니다.
policy-RequestedLocales = 어플리케이션의 요청된 로케일의 목록을 설정 순서로 설정합니다.
policy-SanitizeOnShutdown = 종료시 모든 네비게이션 데이타를 삭제합니다.
policy-SearchBar = 검색 바의 기본 위치를 설정합니다. 사용자가 다시 재설정 할 수 있습니다.
policy-SearchEngines = 검색 엔진 설정을 구성합니다. 이 정책은 확장 지원 버전(ESR)에서만 가능합니다.
# For more information, see https://developer.mozilla.org/en-US/docs/Mozilla/Projects/NSS/PKCS11/Module_Installation
policy-SecurityDevices = PKCS #11 모듈을 설치합니다.
# “format” refers to the format used for the value of this policy. See also:
# https://github.com/mozilla/policy-templates/blob/master/README.md#websitefilter-machine-only
policy-WebsiteFilter = 특정 웹사이트 방문을 차단합니다. 자세한 형식에 대해서는 문서를 참고하세요.
