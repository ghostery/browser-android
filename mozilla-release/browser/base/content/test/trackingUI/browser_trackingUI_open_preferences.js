/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

const TP_PREF = "privacy.trackingprotection.enabled";
const TPC_PREF = "network.cookie.cookieBehavior";
<<<<<<< HEAD
const TRACKING_PAGE = "http://tracking.example.org/browser/browser/base/content/test/trackingUI/trackingPage.html";

async function waitAndAssertPreferencesShown() {
  await BrowserTestUtils.waitForEvent(gIdentityHandler._identityPopup, "popuphidden");
  await TestUtils.waitForCondition(() => gBrowser.currentURI.spec == "about:preferences#privacy",
    "Should open about:preferences.");

  await ContentTask.spawn(gBrowser.selectedBrowser, {}, async function() {
    let doc = content.document;
    let section = await ContentTaskUtils.waitForCondition(
      () => doc.querySelector(".spotlight"), "The spotlight should appear.");
    is(section.getAttribute("data-subcategory"), "trackingprotection",
      "The trackingprotection section is spotlighted.");
  });
||||||| merged common ancestors
const FB_UI_PREF = "browser.contentblocking.fastblock.control-center.ui.enabled";
const TP_UI_PREF = "browser.contentblocking.trackingprotection.control-center.ui.enabled";
const RT_UI_PREF = "browser.contentblocking.rejecttrackers.control-center.ui.enabled";
const TRACKING_PAGE = "http://tracking.example.org/browser/browser/base/content/test/trackingUI/trackingPage.html";

async function waitAndAssertPreferencesShown() {
  await BrowserTestUtils.waitForEvent(gIdentityHandler._identityPopup, "popuphidden");
  await TestUtils.waitForCondition(() => gBrowser.currentURI.spec == "about:preferences#privacy",
    "Should open about:preferences.");

  await ContentTask.spawn(gBrowser.selectedBrowser, {}, async function() {
    let doc = content.document;
    let section = await ContentTaskUtils.waitForCondition(
      () => doc.querySelector(".spotlight"), "The spotlight should appear.");
    is(section.getAttribute("data-subcategory"), "trackingprotection",
      "The trackingprotection section is spotlighted.");
  });
=======
const TRACKING_PAGE =
  "http://tracking.example.org/browser/browser/base/content/test/trackingUI/trackingPage.html";

async function waitAndAssertPreferencesShown(_spotlight) {
  await BrowserTestUtils.waitForEvent(
    gIdentityHandler._identityPopup,
    "popuphidden"
  );
  await TestUtils.waitForCondition(
    () => gBrowser.currentURI.spec == "about:preferences#privacy",
    "Should open about:preferences."
  );

  await ContentTask.spawn(
    gBrowser.selectedBrowser,
    { spotlight: _spotlight },
    async function({ spotlight }) {
      let doc = content.document;
      let section = await ContentTaskUtils.waitForCondition(
        () => doc.querySelector(".spotlight"),
        "The spotlight should appear."
      );
      is(
        section.getAttribute("data-subcategory"),
        spotlight,
        "The correct section is spotlighted."
      );
    }
  );
>>>>>>> upstream-releases

  BrowserTestUtils.removeTab(gBrowser.selectedTab);
}

add_task(async function setup() {
  await UrlClassifierTestUtils.addTestTrackers();
  let oldCanRecord = Services.telemetry.canRecordExtended;
  Services.telemetry.canRecordExtended = true;

  registerCleanupFunction(() => {
    Services.telemetry.canRecordExtended = oldCanRecord;
    UrlClassifierTestUtils.cleanupTestTrackers();
  });
});

// Tests that pressing the content blocking preferences icon in the identity popup
// links to about:preferences
add_task(async function testOpenPreferencesFromCBPrefsButton() {
  await BrowserTestUtils.withNewTab("https://example.com", async function() {
    let promisePanelOpen = BrowserTestUtils.waitForEvent(
      gIdentityHandler._identityPopup,
      "popupshown"
    );
    gIdentityHandler._identityBox.click();
    await promisePanelOpen;

    let preferencesButton = document.getElementById(
      "tracking-protection-preferences-button"
    );

    ok(
      BrowserTestUtils.is_visible(preferencesButton),
      "The preferences button is shown."
    );

    Services.telemetry.clearEvents();

    let shown = waitAndAssertPreferencesShown("trackingprotection");
    preferencesButton.click();
    await shown;

    let events = Services.telemetry.snapshotEvents(
      Ci.nsITelemetry.DATASET_PRERELEASE_CHANNELS,
      true
    ).parent;
    let clickEvents = events.filter(
      e =>
        e[1] == "security.ui.identitypopup" &&
        e[2] == "click" &&
        e[3] == "cb_prefs_button"
    );
    is(clickEvents.length, 1, "recorded telemetry for the click");
  });
});

// Tests that pressing the permissions preferences icon in the identity popup
// links to about:preferences
add_task(async function testOpenPreferencesFromPermissionsPrefsButton() {
  await BrowserTestUtils.withNewTab("https://example.com", async function() {
    let promisePanelOpen = BrowserTestUtils.waitForEvent(
      gIdentityHandler._identityPopup,
      "popupshown"
    );
    gIdentityHandler._identityBox.click();
    await promisePanelOpen;

    let preferencesButton = document.getElementById(
      "identity-popup-permission-preferences-button"
    );

    ok(
      BrowserTestUtils.is_visible(preferencesButton),
      "The preferences button is shown."
    );

    Services.telemetry.clearEvents();

    let shown = waitAndAssertPreferencesShown("permissions");
    preferencesButton.click();
    await shown;

    let events = Services.telemetry.snapshotEvents(
      Ci.nsITelemetry.DATASET_PRERELEASE_CHANNELS,
      true
    ).parent;
    let clickEvents = events.filter(
      e =>
        e[1] == "security.ui.identitypopup" &&
        e[2] == "click" &&
        e[3] == "permission_prefs_btn"
    );
    is(clickEvents.length, 1, "recorded telemetry for the click");
  });
});

// Tests that pressing the preferences button in the trackers subview
// links to about:preferences
<<<<<<< HEAD
add_task(async function testOpenPreferencesFromAddBlockingButtons() {
  SpecialPowers.pushPrefEnv({set: [
    [TP_PREF, false],
    [TPC_PREF, Ci.nsICookieService.BEHAVIOR_ACCEPT],
  ]});
||||||| merged common ancestors
add_task(async function testOpenPreferencesFromAddBlockingButtons() {
  SpecialPowers.pushPrefEnv({set: [
    [CB_PREF, true],
    [CB_UI_PREF, true],
    [FB_PREF, false],
    [TP_PREF, false],
    [TPC_PREF, Ci.nsICookieService.BEHAVIOR_ACCEPT],
    [FB_UI_PREF, true],
    [TP_UI_PREF, true],
    [RT_UI_PREF, true],
  ]});
=======
add_task(async function testOpenPreferencesFromTrackersSubview() {
  Services.prefs.setBoolPref(TP_PREF, true);
>>>>>>> upstream-releases

  await BrowserTestUtils.withNewTab(TRACKING_PAGE, async function() {
    let promisePanelOpen = BrowserTestUtils.waitForEvent(
      gIdentityHandler._identityPopup,
      "popupshown"
    );
    gIdentityHandler._identityBox.click();
    await promisePanelOpen;

    let categoryItem = document.getElementById(
      "identity-popup-content-blocking-category-tracking-protection"
    );
    ok(
      BrowserTestUtils.is_visible(categoryItem),
      "TP category item is visible"
    );
    let trackersView = document.getElementById("identity-popup-trackersView");
    let viewShown = BrowserTestUtils.waitForEvent(trackersView, "ViewShown");
    categoryItem.click();
    await viewShown;

    ok(true, "Trackers view was shown");

    let preferencesButton = document.getElementById(
      "identity-popup-trackersView-settings-button"
    );

    ok(
      BrowserTestUtils.is_visible(preferencesButton),
      "The preferences button is shown."
    );

    Services.telemetry.clearEvents();

    let shown = waitAndAssertPreferencesShown("trackingprotection");
    preferencesButton.click();
    await shown;

    let events = Services.telemetry.snapshotEvents(
      Ci.nsITelemetry.DATASET_PRERELEASE_CHANNELS,
      true
    ).parent;
    let clickEvents = events.filter(
      e =>
        e[1] == "security.ui.identitypopup" &&
        e[2] == "click" &&
        e[3] == "trackers_prefs_btn"
    );
    is(clickEvents.length, 1, "recorded telemetry for the click");
  });

  Services.prefs.clearUserPref(TP_PREF);
});

// Tests that pressing the preferences button in the cookies subview
// links to about:preferences
add_task(async function testOpenPreferencesFromCookiesSubview() {
  Services.prefs.setIntPref(
    TPC_PREF,
    Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER
  );

  await BrowserTestUtils.withNewTab(TRACKING_PAGE, async function() {
    let promisePanelOpen = BrowserTestUtils.waitForEvent(
      gIdentityHandler._identityPopup,
      "popupshown"
    );
    gIdentityHandler._identityBox.click();
    await promisePanelOpen;

    let categoryItem = document.getElementById(
      "identity-popup-content-blocking-category-cookies"
    );
    ok(
      BrowserTestUtils.is_visible(categoryItem),
      "TP category item is visible"
    );
    let cookiesView = document.getElementById("identity-popup-cookiesView");
    let viewShown = BrowserTestUtils.waitForEvent(cookiesView, "ViewShown");
    categoryItem.click();
    await viewShown;

    ok(true, "Cookies view was shown");

    let preferencesButton = document.getElementById(
      "identity-popup-cookiesView-settings-button"
    );

    ok(
      BrowserTestUtils.is_visible(preferencesButton),
      "The preferences button is shown."
    );

    Services.telemetry.clearEvents();

    let shown = waitAndAssertPreferencesShown("trackingprotection");
    preferencesButton.click();
    await shown;

    let events = Services.telemetry.snapshotEvents(
      Ci.nsITelemetry.DATASET_PRERELEASE_CHANNELS,
      true
    ).parent;
    let clickEvents = events.filter(
      e =>
        e[1] == "security.ui.identitypopup" &&
        e[2] == "click" &&
        e[3] == "cookies_prefs_btn"
    );
    is(clickEvents.length, 1, "recorded telemetry for the click");
  });

  Services.prefs.clearUserPref(TPC_PREF);
});
