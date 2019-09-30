# -*- Mode: Java; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

<<<<<<< HEAD
# JS files which are needed by browser.xul but no other top level windows to
# support MacOS specific features should be loaded directly from browser.xul
# rather than this file.

||||||| merged common ancestors
=======
# JS files which are needed by browser.xhtml but no other top level windows to
# support MacOS specific features should be loaded directly from browser.xhtml
# rather than this file.

>>>>>>> upstream-releases
# If you update this list, you may need to add a mapping within the following
# file so that ESLint works correctly:
# tools/lint/eslint/eslint-plugin-mozilla/lib/environments/browser-window.js

<<<<<<< HEAD
<script type="text/javascript"
#ifdef BROWSER_XHTML
xmlns="http://www.w3.org/1999/xhtml"
#endif
>
Components.utils.import("resource://gre/modules/Services.jsm");

Services.scriptloader.loadSubScript("chrome://browser/content/browser.js", this);
Services.scriptloader.loadSubScript("chrome://browser/content/browser-places.js", this);
Services.scriptloader.loadSubScript("chrome://global/content/globalOverlay.js", this);
Services.scriptloader.loadSubScript("chrome://browser/content/utilityOverlay.js", this);
#ifdef XP_MACOSX
Services.scriptloader.loadSubScript("chrome://global/content/macWindowMenu.js", this);
#endif

||||||| merged common ancestors
<script type="text/javascript"
#ifdef BROWSER_XHTML
xmlns="http://www.w3.org/1999/xhtml"
#endif
>
Components.utils.import("resource://gre/modules/Services.jsm");

for (let script of [
  "chrome://browser/content/browser.js",
  "chrome://browser/content/search/searchbar.js",

  "chrome://browser/content/browser-captivePortal.js",
  "chrome://browser/content/browser-compacttheme.js",
  "chrome://browser/content/browser-contentblocking.js",
  "chrome://browser/content/browser-media.js",
  "chrome://browser/content/browser-pageActions.js",
  "chrome://browser/content/browser-places.js",
  "chrome://browser/content/browser-plugins.js",
  "chrome://browser/content/browser-sidebar.js",
  "chrome://browser/content/browser-siteIdentity.js",
  "chrome://browser/content/browser-tabsintitlebar.js",

  "chrome://global/content/globalOverlay.js",
  "chrome://browser/content/utilityOverlay.js",
#ifdef XP_MACOSX
  "chrome://global/content/macWindowMenu.js",
#endif
#ifdef MOZ_DATA_REPORTING
  "chrome://browser/content/browser-data-submission-info-bar.js",
#endif
#ifndef MOZILLA_OFFICIAL
  "chrome://browser/content/browser-development-helpers.js",
#endif
]) {
  Services.scriptloader.loadSubScript(script, this);
}
=======
<script type="text/javascript">
var {Services} = ChromeUtils.import("resource://gre/modules/Services.jsm");

Services.scriptloader.loadSubScript("chrome://browser/content/browser.js", this);
Services.scriptloader.loadSubScript("chrome://browser/content/browser-places.js", this);
Services.scriptloader.loadSubScript("chrome://global/content/globalOverlay.js", this);
Services.scriptloader.loadSubScript("chrome://global/content/editMenuOverlay.js", this);
Services.scriptloader.loadSubScript("chrome://browser/content/utilityOverlay.js", this);
if (AppConstants.platform == "macosx") {
  Services.scriptloader.loadSubScript("chrome://global/content/macWindowMenu.js", this);
}

>>>>>>> upstream-releases
</script>
