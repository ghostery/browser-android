/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
const TP_PB_ENABLED_PREF = "privacy.trackingprotection.pbmode.enabled";
||||||| merged common ancestors
const TP_PB_ENABLED_PREF = "privacy.trackingprotection.pbmode.enabled";
const CB_ENABLED_PREF = "browser.contentblocking.enabled";
const CB_UI_ENABLED_PREF = "browser.contentblocking.ui.enabled";
=======
const { UrlbarTestUtils } = ChromeUtils.import(
  "resource://testing-common/UrlbarTestUtils.jsm"
);
>>>>>>> upstream-releases

/**
 * Opens a new private window and loads "about:privatebrowsing" there.
 */
async function openAboutPrivateBrowsing() {
  let win = await BrowserTestUtils.openNewBrowserWindow({
    private: true,
    waitForTabURL: "about:privatebrowsing",
  });
  let tab = win.gBrowser.selectedBrowser;
  return { win, tab };
}

/**
 * Clicks the given link and checks this opens the given URI in the same tab.
 *
 * This function does not return to the previous page.
 */
async function testLinkOpensUrl({ win, tab, elementId, expectedUrl }) {
  let loadedPromise = BrowserTestUtils.browserLoaded(tab);
  await ContentTask.spawn(tab, elementId, async function(elemId) {
    content.document.getElementById(elemId).click();
  });
  await loadedPromise;
  is(
    tab.currentURI.spec,
    expectedUrl,
    `Clicking ${elementId} opened ${expectedUrl} in the same tab.`
  );
}

/**
 * Tests the private-browsing-myths link in "about:privatebrowsing".
 */
add_task(async function test_myths_link() {
  Services.prefs.setCharPref("app.support.baseURL", "https://example.com/");
<<<<<<< HEAD
  Services.prefs.setCharPref("privacy.trackingprotection.introURL",
                             "https://example.com/tour");
||||||| merged common ancestors
  Services.prefs.setCharPref("privacy.trackingprotection.introURL",
                             "https://example.com/tour");
  Services.prefs.setBoolPref(CB_UI_ENABLED_PREF, false);
=======
>>>>>>> upstream-releases
  registerCleanupFunction(function() {
<<<<<<< HEAD
    Services.prefs.clearUserPref("privacy.trackingprotection.introURL");
    Services.prefs.clearUserPref("app.support.baseURL");
  });
||||||| merged common ancestors
    Services.prefs.clearUserPref(CB_UI_ENABLED_PREF);
    Services.prefs.clearUserPref("privacy.trackingprotection.introURL");
    Services.prefs.clearUserPref("app.support.baseURL");
  });

  let { win, tab } = await openAboutPrivateBrowsing();

  await testLinkOpensTab({ win, tab,
    elementId: "learnMore",
    expectedUrl: "https://example.com/private-browsing",
  });

  await testLinkOpensUrl({ win, tab,
    elementId: "startTour",
    expectedUrl: "https://example.com/tour?variation=0",
  });

  await BrowserTestUtils.closeWindow(win);
});

add_task(async function test_links_CB() {
  // Use full version and change the remote URLs to prevent network access.
  Services.prefs.setCharPref("app.support.baseURL", "https://example.com/");
  Services.prefs.setCharPref("privacy.trackingprotection.introURL",
                             "https://example.com/tour");
  Services.prefs.setBoolPref(CB_UI_ENABLED_PREF, true);
  registerCleanupFunction(function() {
    Services.prefs.clearUserPref(CB_UI_ENABLED_PREF);
    Services.prefs.clearUserPref("privacy.trackingprotection.introURL");
    Services.prefs.clearUserPref("app.support.baseURL");
  });
=======
    Services.prefs.clearUserPref("app.support.baseURL");
  });

  let { win, tab } = await openAboutPrivateBrowsing();

  await testLinkOpensUrl({
    win,
    tab,
    elementId: "private-browsing-myths",
    expectedUrl: "https://example.com/private-browsing-myths",
  });

  await BrowserTestUtils.closeWindow(win);
});

function urlBarHasHiddenFocus(win) {
  return (
    win.gURLBar.hasAttribute("focused") &&
    win.gURLBar.textbox.classList.contains("hidden-focus")
  );
}
>>>>>>> upstream-releases

function urlBarHasNormalFocus(win) {
  return (
    win.gURLBar.hasAttribute("focused") &&
    !win.gURLBar.textbox.classList.contains("hidden-focus")
  );
}

/**
 * Tests the search hand-off on character keydown in "about:privatebrowsing".
 */
add_task(async function test_search_handoff_on_keydown() {
  let { win, tab } = await openAboutPrivateBrowsing();

  await ContentTask.spawn(tab, null, async function() {
    let btn = content.document.getElementById("search-handoff-button");
    btn.click();
    ok(btn.classList.contains("focused"), "in-content search has focus styles");
  });
  ok(urlBarHasHiddenFocus(win), "url bar has hidden focused");
  await new Promise(r => EventUtils.synthesizeKey("f", {}, win, r));
  await ContentTask.spawn(tab, null, async function() {
    ok(
      content.document
        .getElementById("search-handoff-button")
        .classList.contains("hidden"),
      "in-content search is hidden"
    );
  });
  ok(urlBarHasNormalFocus(win), "url bar has normal focused");
  is(win.gURLBar.value, "@google f", "url bar has search text");
  await UrlbarTestUtils.promiseSearchComplete(win);
  // Close the popup.
  await UrlbarTestUtils.promisePopupClose(win);

  // Hitting ESC should reshow the in-content search
  await new Promise(r => EventUtils.synthesizeKey("KEY_Escape", {}, win, r));
  await ContentTask.spawn(tab, null, async function() {
    ok(
      !content.document
        .getElementById("search-handoff-button")
        .classList.contains("hidden"),
      "in-content search is not hidden"
    );
  });

  await BrowserTestUtils.closeWindow(win);
});

<<<<<<< HEAD
||||||| merged common ancestors
function waitForPrefChanged(pref) {
  return new Promise(resolve => {
    let prefObserver = {
      QueryInterface: ChromeUtils.generateQI([Ci.nsIObserver]),
      observe() {
        Services.prefs.removeObserver(pref, prefObserver);
        resolve();
      },
    };
    Services.prefs.addObserver(pref, prefObserver);
  });
}

/**
 * Tests the action to disable and re-enable Tracking Protection in
 * "about:privatebrowsing".
 */
add_task(async function test_toggleTrackingProtection() {
  // Use tour version but disable Tracking Protection.
  Services.prefs.setBoolPref(TP_PB_ENABLED_PREF, true);
  // For good measure, check that content blocking being off
  // has no impact if the contentblocking UI is not shown.
  Services.prefs.setBoolPref(CB_ENABLED_PREF, false);
  Services.prefs.setBoolPref(CB_UI_ENABLED_PREF, false);

  registerCleanupFunction(function() {
    Services.prefs.clearUserPref(TP_PB_ENABLED_PREF);
    Services.prefs.clearUserPref(CB_ENABLED_PREF);
    Services.prefs.clearUserPref(CB_UI_ENABLED_PREF);
  });

  let { win, tab } = await openAboutPrivateBrowsing();

  // Set up the observer for the preference change before triggering the action.
  let promisePrefChanged = waitForPrefChanged(TP_PB_ENABLED_PREF);
  await ContentTask.spawn(tab, {}, async function() {
    is(content.document.getElementById("tpToggle").checked, true, "toggle is active");
    content.document.getElementById("tpButton").click();
  });
  await promisePrefChanged;
  ok(!Services.prefs.getBoolPref(TP_PB_ENABLED_PREF), "Tracking Protection is disabled.");

  promisePrefChanged = waitForPrefChanged(TP_PB_ENABLED_PREF);
  await ContentTask.spawn(tab, {}, async function() {
    is(content.document.getElementById("tpToggle").checked, false, "toggle is not active");
    content.document.getElementById("tpButton").click();
  });
  await promisePrefChanged;
  ok(Services.prefs.getBoolPref(TP_PB_ENABLED_PREF), "Tracking Protection is enabled.");

  await BrowserTestUtils.closeWindow(win);
});

/**
 * Tests the action to disable and re-enable Tracking Protection in
 * "about:privatebrowsing" when content blocking is disabled.
 */
add_task(async function test_toggleTrackingProtectionContentBlocking() {
  Services.prefs.setBoolPref(TP_PB_ENABLED_PREF, true);
  Services.prefs.setBoolPref(CB_ENABLED_PREF, false);
  Services.prefs.setBoolPref(CB_UI_ENABLED_PREF, true);

  registerCleanupFunction(function() {
    Services.prefs.clearUserPref(TP_PB_ENABLED_PREF);
    Services.prefs.clearUserPref(CB_ENABLED_PREF);
    Services.prefs.clearUserPref(CB_UI_ENABLED_PREF);
  });

  let { win, tab } = await openAboutPrivateBrowsing();

  let promiseCBPrefChanged = waitForPrefChanged(CB_ENABLED_PREF);
  await ContentTask.spawn(tab, {}, async function() {
    is(content.document.getElementById("tpToggle").checked, false, "toggle is not active");
    content.document.getElementById("tpButton").click();
  });
  await promiseCBPrefChanged;
  ok(Services.prefs.getBoolPref(TP_PB_ENABLED_PREF), "Tracking Protection is enabled.");
  ok(Services.prefs.getBoolPref(CB_ENABLED_PREF), "Content Blocking is enabled.");

  let promiseTPPrefChanged = waitForPrefChanged(TP_PB_ENABLED_PREF);
  await ContentTask.spawn(tab, {}, async function() {
    is(content.document.getElementById("tpToggle").checked, true, "toggle is active");
    content.document.getElementById("tpButton").click();
  });
  await promiseTPPrefChanged;
  ok(!Services.prefs.getBoolPref(TP_PB_ENABLED_PREF), "Tracking Protection is disabled.");
  ok(Services.prefs.getBoolPref(CB_ENABLED_PREF), "Content Blocking is enabled.");

  await BrowserTestUtils.closeWindow(win);
});
=======
/**
 * Tests the search hand-off on composition start in "about:privatebrowsing".
 */
add_task(async function test_search_handoff_on_composition_start() {
  let { win, tab } = await openAboutPrivateBrowsing();

  await ContentTask.spawn(tab, null, async function() {
    content.document.getElementById("search-handoff-button").click();
  });
  ok(urlBarHasHiddenFocus(win), "url bar has hidden focused");
  await new Promise(r =>
    EventUtils.synthesizeComposition({ type: "compositionstart" }, win, r)
  );
  ok(urlBarHasNormalFocus(win), "url bar has normal focused");

  await BrowserTestUtils.closeWindow(win);
});

/**
 * Tests the search hand-off on paste in "about:privatebrowsing".
 */
add_task(async function test_search_handoff_on_paste() {
  let { win, tab } = await openAboutPrivateBrowsing();

  await ContentTask.spawn(tab, null, async function() {
    content.document.getElementById("search-handoff-button").click();
  });
  ok(urlBarHasHiddenFocus(win), "url bar has hidden focused");
  var helper = SpecialPowers.Cc[
    "@mozilla.org/widget/clipboardhelper;1"
  ].getService(SpecialPowers.Ci.nsIClipboardHelper);
  helper.copyString("words");
  await new Promise(r =>
    EventUtils.synthesizeKey("v", { accelKey: true }, win, r)
  );
  // TODO: Bug 1539199 We should be able to wait for search complete for AwesomeBar
  // as well.
  if (UrlbarPrefs.get("quantumbar")) {
    await UrlbarTestUtils.promiseSearchComplete(win);
  }
  ok(urlBarHasNormalFocus(win), "url bar has normal focused");
  is(win.gURLBar.value, "@google words", "url bar has search text");

  await BrowserTestUtils.closeWindow(win);
});
>>>>>>> upstream-releases
