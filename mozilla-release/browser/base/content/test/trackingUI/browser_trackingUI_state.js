/*
 * Test that the Tracking Protection section is visible in the Control Center
 * and has the correct state for the cases when:
 *
 * In a normal window as well as a private window,
 *   With TP enabled
 *     1) A page with no tracking elements is loaded.
 *     2) A page with tracking elements is loaded and they are blocked.
 *     3) A page with tracking elements is loaded and they are not blocked.
 *   With TP disabled
 *     1) A page with no tracking elements is loaded.
 *     2) A page with tracking elements is loaded.
 *
 * See also Bugs 1175327, 1043801, 1178985
 */

const TP_PREF = "privacy.trackingprotection.enabled";
const TP_PB_PREF = "privacy.trackingprotection.pbmode.enabled";
const TPC_PREF = "network.cookie.cookieBehavior";
const DTSCBN_PREF = "dom.testing.sync-content-blocking-notifications";
const BENIGN_PAGE =
  "http://tracking.example.org/browser/browser/base/content/test/trackingUI/benignPage.html";
const TRACKING_PAGE =
  "http://tracking.example.org/browser/browser/base/content/test/trackingUI/trackingPage.html";
const COOKIE_PAGE =
  "http://not-tracking.example.com/browser/browser/base/content/test/trackingUI/cookiePage.html";
var ContentBlocking = null;
var TrackingProtection = null;
var ThirdPartyCookies = null;
var tabbrowser = null;
var gTrackingPageURL = TRACKING_PAGE;

registerCleanupFunction(function() {
<<<<<<< HEAD
  TrackingProtection = ContentBlocking =
    ThirdPartyCookies = tabbrowser = null;
||||||| merged common ancestors
  TrackingProtection = ContentBlocking = FastBlock =
    ThirdPartyCookies = tabbrowser = null;
=======
  TrackingProtection = ContentBlocking = ThirdPartyCookies = tabbrowser = null;
>>>>>>> upstream-releases
  UrlClassifierTestUtils.cleanupTestTrackers();
  Services.prefs.clearUserPref(TP_PREF);
  Services.prefs.clearUserPref(TP_PB_PREF);
  Services.prefs.clearUserPref(TPC_PREF);
  Services.prefs.clearUserPref(DTSCBN_PREF);
});

// This is a special version of "hidden" that doesn't check for item
// visibility and just asserts the display and opacity attributes.
// That way we can test elements even when their panel is hidden...
function hidden(sel) {
  let win = tabbrowser.ownerGlobal;
  let el = win.document.querySelector(sel);
  let display = win.getComputedStyle(el).getPropertyValue("display", null);
  let opacity = win.getComputedStyle(el).getPropertyValue("opacity", null);
  return display === "none" || opacity === "0";
}

function clickButton(sel) {
  let win = tabbrowser.ownerGlobal;
  let el = win.document.querySelector(sel);
  el.doCommand();
}

function testBenignPage() {
  info("Non-tracking content must not be blocked");
<<<<<<< HEAD
  ok(!ContentBlocking.content.hasAttribute("detected"), "no trackers are detected");
  ok(!ContentBlocking.content.hasAttribute("hasException"), "content shows no exception");
||||||| merged common ancestors
  ok(!ContentBlocking.content.hasAttribute("detected"), "no trackers are detected");
  ok(!ContentBlocking.content.hasAttribute("hasException"), "content shows no exception");
  ok(!ContentBlocking.content.hasAttribute("active"), "content is not active");
=======
  ok(
    !ContentBlocking.content.hasAttribute("detected"),
    "no trackers are detected"
  );
  ok(
    !ContentBlocking.content.hasAttribute("hasException"),
    "content shows no exception"
  );
>>>>>>> upstream-releases

  ok(!ContentBlocking.iconBox.hasAttribute("active"), "shield is not active");
  ok(
    !ContentBlocking.iconBox.hasAttribute("hasException"),
    "icon box shows no exception"
  );
  ok(
    !ContentBlocking.iconBox.hasAttribute("tooltiptext"),
    "icon box has no tooltip"
  );

  ok(BrowserTestUtils.is_hidden(ContentBlocking.iconBox), "icon box is hidden");
  ok(hidden("#tracking-action-block"), "blockButton is hidden");
  ok(hidden("#tracking-action-unblock"), "unblockButton is hidden");

<<<<<<< HEAD
  ok(!hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is visible");
  ok(hidden("#identity-popup-content-blocking-detected"), "blocking detected label is hidden");
  ok(hidden("#identity-popup-content-blocking-category-cookies"),
    "Not showing cookie restrictions category");
  ok(hidden("#identity-popup-content-blocking-category-tracking-protection"),
    "Not showing trackers category");
||||||| merged common ancestors
  ok(!hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is visible");
  ok(hidden("#identity-popup-content-blocking-detected"), "blocking detected label is hidden");

  if (Services.prefs.getBoolPref(CB_UI_PREF)) {
    ok(hidden("#identity-popup-content-blocking-category-list"), "category list is hidden");
  }
=======
  ok(
    !hidden("#identity-popup-content-blocking-not-detected"),
    "blocking not detected label is visible"
  );
  ok(
    hidden("#identity-popup-content-blocking-detected"),
    "blocking detected label is hidden"
  );
  ok(
    hidden("#identity-popup-content-blocking-category-cookies"),
    "Not showing cookie restrictions category"
  );
  ok(
    hidden("#identity-popup-content-blocking-category-tracking-protection"),
    "Not showing trackers category"
  );
>>>>>>> upstream-releases
}

function testBenignPageWithException() {
  info("Non-tracking content must not be blocked");
<<<<<<< HEAD
  ok(!ContentBlocking.content.hasAttribute("detected"), "no trackers are detected");
  ok(ContentBlocking.content.hasAttribute("hasException"), "content shows exception");
||||||| merged common ancestors
  ok(!ContentBlocking.content.hasAttribute("detected"), "no trackers are detected");
  ok(ContentBlocking.content.hasAttribute("hasException"), "content shows exception");
  ok(!ContentBlocking.content.hasAttribute("active"), "content is not active");
=======
  ok(
    !ContentBlocking.content.hasAttribute("detected"),
    "no trackers are detected"
  );
  ok(
    ContentBlocking.content.hasAttribute("hasException"),
    "content shows exception"
  );
>>>>>>> upstream-releases

  ok(!ContentBlocking.iconBox.hasAttribute("active"), "shield is not active");
<<<<<<< HEAD
  ok(ContentBlocking.iconBox.hasAttribute("hasException"), "shield shows exception");
  is(ContentBlocking.iconBox.getAttribute("tooltiptext"),
     gNavigatorBundle.getString("trackingProtection.icon.disabledTooltip"), "correct tooltip");

  ok(!BrowserTestUtils.is_hidden(ContentBlocking.iconBox), "icon box is not hidden");
  ok(!hidden("#tracking-action-block"), "blockButton is visible");
||||||| merged common ancestors
  is(ContentBlocking.iconBox.hasAttribute("hasException"), ContentBlocking.enabled,
    "shield shows exception if CB is on");
  is(ContentBlocking.iconBox.getAttribute("tooltiptext"),
     gNavigatorBundle.getString("trackingProtection.icon.disabledTooltip"), "correct tooltip");

  is(!BrowserTestUtils.is_hidden(ContentBlocking.iconBox), ContentBlocking.enabled,
    "icon box is not hidden if CB is on");
  is(!hidden("#tracking-action-block"), ContentBlocking.enabled,
     "blockButton is visible if CB is on");
=======
  ok(
    ContentBlocking.iconBox.hasAttribute("hasException"),
    "shield shows exception"
  );
  is(
    ContentBlocking.iconBox.getAttribute("tooltiptext"),
    gNavigatorBundle.getString("trackingProtection.icon.disabledTooltip"),
    "correct tooltip"
  );

  ok(
    !BrowserTestUtils.is_hidden(ContentBlocking.iconBox),
    "icon box is not hidden"
  );
  ok(!hidden("#tracking-action-block"), "blockButton is visible");
>>>>>>> upstream-releases
  ok(hidden("#tracking-action-unblock"), "unblockButton is hidden");

<<<<<<< HEAD
  ok(!hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is visible");
  ok(hidden("#identity-popup-content-blocking-detected"), "blocking detected label is hidden");
  ok(hidden("#identity-popup-content-blocking-category-cookies"),
    "Not showing cookie restrictions category");
  ok(hidden("#identity-popup-content-blocking-category-tracking-protection"),
    "Not showing trackers category");
||||||| merged common ancestors
  ok(!hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is visible");
  ok(hidden("#identity-popup-content-blocking-detected"), "blocking detected label is hidden");

  if (Services.prefs.getBoolPref(CB_UI_PREF)) {
    ok(hidden("#identity-popup-content-blocking-category-list"), "category list is hidden");
  }
=======
  ok(
    !hidden("#identity-popup-content-blocking-not-detected"),
    "blocking not detected label is visible"
  );
  ok(
    hidden("#identity-popup-content-blocking-detected"),
    "blocking detected label is hidden"
  );
  ok(
    hidden("#identity-popup-content-blocking-category-cookies"),
    "Not showing cookie restrictions category"
  );
  ok(
    hidden("#identity-popup-content-blocking-category-tracking-protection"),
    "Not showing trackers category"
  );
>>>>>>> upstream-releases
}

function areTrackersBlocked(isPrivateBrowsing) {
<<<<<<< HEAD
  let blockedByTP = Services.prefs.getBoolPref(isPrivateBrowsing ? TP_PB_PREF : TP_PREF);
  let blockedByTPC = Services.prefs.getIntPref(TPC_PREF) == Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER;
  return blockedByTP || blockedByTPC;
||||||| merged common ancestors
  let cbEnabled = Services.prefs.getBoolPref(CB_PREF);
  let blockedByTP = cbEnabled &&
                    Services.prefs.getBoolPref(isPrivateBrowsing ? TP_PB_PREF : TP_PREF);
  let blockedByFB = cbEnabled &&
                    Services.prefs.getBoolPref(FB_PREF) &&
                    // The timeout pref is only checked for completeness,
                    // checking it is technically unneeded for this test.
                    Services.prefs.getIntPref(FB_TIMEOUT_PREF) == 0;
  let blockedByTPC = cbEnabled &&
                     Services.prefs.getIntPref(TPC_PREF) == Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER;
  return blockedByTP || blockedByFB || blockedByTPC;
=======
  let blockedByTP = Services.prefs.getBoolPref(
    isPrivateBrowsing ? TP_PB_PREF : TP_PREF
  );
  let blockedByTPC =
    Services.prefs.getIntPref(TPC_PREF) ==
    Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER;
  return blockedByTP || blockedByTPC;
>>>>>>> upstream-releases
}

function testTrackingPage(window) {
  info("Tracking content must be blocked");
  ok(ContentBlocking.content.hasAttribute("detected"), "trackers are detected");
  ok(
    !ContentBlocking.content.hasAttribute("hasException"),
    "content shows no exception"
  );

<<<<<<< HEAD
  let isPrivateBrowsing = PrivateBrowsingUtils.isWindowPrivate(window);
  let blockedByTP = areTrackersBlocked(isPrivateBrowsing);
  is(BrowserTestUtils.is_visible(ContentBlocking.iconBox), blockedByTP,
     "icon box is" + (blockedByTP ? "" : " not") + " visible");
  is(ContentBlocking.iconBox.hasAttribute("active"), blockedByTP,
      "shield is" + (blockedByTP ? "" : " not") + " active");
  ok(!ContentBlocking.iconBox.hasAttribute("hasException"), "icon box shows no exception");
  is(ContentBlocking.iconBox.getAttribute("tooltiptext"),
     blockedByTP ? gNavigatorBundle.getString("trackingProtection.icon.activeTooltip") : "",
     "correct tooltip");
||||||| merged common ancestors
  let isPrivateBrowsing = PrivateBrowsingUtils.isWindowPrivate(window);
  let blockedByTP = areTrackersBlocked(isPrivateBrowsing);
  is(BrowserTestUtils.is_visible(ContentBlocking.iconBox), blockedByTP,
     "icon box is" + (blockedByTP ? "" : " not") + " visible");
  is(ContentBlocking.content.hasAttribute("active"), blockedByTP,
      "content is" + (blockedByTP ? "" : " not") + " active");
  is(ContentBlocking.iconBox.hasAttribute("active"), blockedByTP,
      "shield is" + (blockedByTP ? "" : " not") + " active");
  ok(!ContentBlocking.iconBox.hasAttribute("hasException"), "icon box shows no exception");
  is(ContentBlocking.iconBox.getAttribute("tooltiptext"),
     blockedByTP ? gNavigatorBundle.getString("trackingProtection.icon.activeTooltip") : "",
     "correct tooltip");
=======
  let isWindowPrivate = PrivateBrowsingUtils.isWindowPrivate(window);
  let blockedByTP = areTrackersBlocked(isWindowPrivate);
  is(
    BrowserTestUtils.is_visible(ContentBlocking.iconBox),
    blockedByTP,
    "icon box is" + (blockedByTP ? "" : " not") + " visible"
  );
  is(
    ContentBlocking.iconBox.hasAttribute("active"),
    blockedByTP,
    "shield is" + (blockedByTP ? "" : " not") + " active"
  );
  ok(
    !ContentBlocking.iconBox.hasAttribute("hasException"),
    "icon box shows no exception"
  );
  is(
    ContentBlocking.iconBox.getAttribute("tooltiptext"),
    blockedByTP
      ? gNavigatorBundle.getString("trackingProtection.icon.activeTooltip")
      : "",
    "correct tooltip"
  );
>>>>>>> upstream-releases

  ok(hidden("#tracking-action-block"), "blockButton is hidden");

<<<<<<< HEAD
  let isWindowPrivate = PrivateBrowsingUtils.isWindowPrivate(window);
||||||| merged common ancestors
  let isWindowPrivate = PrivateBrowsingUtils.isWindowPrivate(window);
  let cbUIEnabled = Services.prefs.getBoolPref(CB_UI_PREF);
=======
>>>>>>> upstream-releases
  if (isWindowPrivate) {
    ok(hidden("#tracking-action-unblock"), "unblockButton is hidden");
    is(
      !hidden("#tracking-action-unblock-private"),
      blockedByTP,
      "unblockButtonPrivate is" + (blockedByTP ? "" : " not") + " visible"
    );
  } else {
    ok(
      hidden("#tracking-action-unblock-private"),
      "unblockButtonPrivate is hidden"
    );
    is(
      !hidden("#tracking-action-unblock"),
      blockedByTP,
      "unblockButton is" + (blockedByTP ? "" : " not") + " visible"
    );
  }

<<<<<<< HEAD
  ok(hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is hidden");
  ok(!hidden("#identity-popup-content-blocking-detected"), "blocking detected label is visible");

  ok(!hidden("#identity-popup-content-blocking-category-tracking-protection"),
    "Showing trackers category");
  if (gTrackingPageURL == COOKIE_PAGE) {
    ok(!hidden("#identity-popup-content-blocking-category-cookies"),
      "Showing cookie restrictions category");
||||||| merged common ancestors
  ok(hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is hidden");
  ok(!hidden("#identity-popup-content-blocking-detected"), "blocking detected label is visible");

  if (cbUIEnabled) {
    ok(!hidden("#identity-popup-content-blocking-category-list"), "category list is visible");
    let category;
    if (Services.prefs.getBoolPref(FB_PREF)) {
      category = "#identity-popup-content-blocking-category-fastblock";
    } else {
      category = Services.prefs.getIntPref(TPC_PREF) == Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER ?
                   "#identity-popup-content-blocking-category-3rdpartycookies" :
                   "#identity-popup-content-blocking-category-tracking-protection";
    }
    is(hidden(category + " > .identity-popup-content-blocking-category-add-blocking"), blockedByTP,
      "Category item is" + (blockedByTP ? " not" : "") + " showing add blocking");
    is(hidden(category + " > .identity-popup-content-blocking-category-state-label"), !blockedByTP,
      "Category item is" + (blockedByTP ? "" : " not") + " set to blocked");
  }

  if (Services.prefs.getIntPref(TPC_PREF) == Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER) {
    ok(hidden("#identity-popup-content-blocking-category-label-default"),
      "Not showing default cookie restrictions label.");
    ok(!hidden("#identity-popup-content-blocking-category-label-trackers"),
      "Showing trackers cookie restrictions label.");
=======
  ok(
    hidden("#identity-popup-content-blocking-not-detected"),
    "blocking not detected label is hidden"
  );
  ok(
    !hidden("#identity-popup-content-blocking-detected"),
    "blocking detected label is visible"
  );

  ok(
    !hidden("#identity-popup-content-blocking-category-tracking-protection"),
    "Showing trackers category"
  );
  if (gTrackingPageURL == COOKIE_PAGE) {
    ok(
      !hidden("#identity-popup-content-blocking-category-cookies"),
      "Showing cookie restrictions category"
    );
>>>>>>> upstream-releases
  } else {
<<<<<<< HEAD
    ok(hidden("#identity-popup-content-blocking-category-cookies"),
      "Not showing cookie restrictions category");
||||||| merged common ancestors
    ok(hidden("#identity-popup-content-blocking-category-label-trackers"),
      "Not showing trackers cookie restrictions label.");
    ok(!hidden("#identity-popup-content-blocking-category-label-default"),
      "Showing default cookie restrictions label.");
=======
    ok(
      hidden("#identity-popup-content-blocking-category-cookies"),
      "Not showing cookie restrictions category"
    );
>>>>>>> upstream-releases
  }
}

function testTrackingPageUnblocked(blockedByTP, window) {
  info("Tracking content must be white-listed and not blocked");
  ok(ContentBlocking.content.hasAttribute("detected"), "trackers are detected");
  ok(
    ContentBlocking.content.hasAttribute("hasException"),
    "content shows exception"
  );

  ok(!ContentBlocking.iconBox.hasAttribute("active"), "shield is not active");
<<<<<<< HEAD
  ok(ContentBlocking.iconBox.hasAttribute("hasException"), "shield shows exception");
  is(ContentBlocking.iconBox.getAttribute("tooltiptext"),
     gNavigatorBundle.getString("trackingProtection.icon.disabledTooltip"), "correct tooltip");

  ok(BrowserTestUtils.is_visible(ContentBlocking.iconBox), "icon box is visible");
  ok(!hidden("#tracking-action-block"), "blockButton is visible");
||||||| merged common ancestors
  is(ContentBlocking.iconBox.hasAttribute("hasException"), blockingEnabled,
     "shield" + (blockingEnabled ? " shows" : " doesn't show") + " exception");
  is(ContentBlocking.iconBox.getAttribute("tooltiptext"),
     gNavigatorBundle.getString("trackingProtection.icon.disabledTooltip"), "correct tooltip");

  is(BrowserTestUtils.is_visible(ContentBlocking.iconBox), blockingEnabled,
     "icon box is" + (blockingEnabled ? "" : " not") + " visible");
  is(hidden("#tracking-action-block"), !blockingEnabled,
     "blockButton is" + (blockingEnabled ? " not" : "") + " visible");
=======
  ok(
    ContentBlocking.iconBox.hasAttribute("hasException"),
    "shield shows exception"
  );
  is(
    ContentBlocking.iconBox.getAttribute("tooltiptext"),
    gNavigatorBundle.getString("trackingProtection.icon.disabledTooltip"),
    "correct tooltip"
  );

  ok(
    BrowserTestUtils.is_visible(ContentBlocking.iconBox),
    "icon box is visible"
  );
  ok(!hidden("#tracking-action-block"), "blockButton is visible");
>>>>>>> upstream-releases
  ok(hidden("#tracking-action-unblock"), "unblockButton is hidden");
<<<<<<< HEAD

  ok(hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is hidden");
  ok(!hidden("#identity-popup-content-blocking-detected"), "blocking detected label is visible");

  ok(!hidden("#identity-popup-content-blocking-category-tracking-protection"),
    "Showing trackers category");
  if (gTrackingPageURL == COOKIE_PAGE) {
    ok(!hidden("#identity-popup-content-blocking-category-cookies"),
      "Showing cookie restrictions category");
  } else {
    ok(hidden("#identity-popup-content-blocking-category-cookies"),
      "Not showing cookie restrictions category");
||||||| merged common ancestors
  ok(!hidden("#identity-popup-content-blocking-disabled-label"), "disabled label is visible");

  ok(hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is hidden");
  ok(!hidden("#identity-popup-content-blocking-detected"), "blocking detected label is visible");

  if (Services.prefs.getBoolPref(CB_UI_PREF)) {
    ok(!hidden("#identity-popup-content-blocking-category-list"), "category list is visible");
    let category;
    if (Services.prefs.getBoolPref(FB_PREF)) {
      category = "#identity-popup-content-blocking-category-fastblock";
    } else {
      category = Services.prefs.getIntPref(TPC_PREF) == Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER ?
                   "#identity-popup-content-blocking-category-3rdpartycookies" :
                   "#identity-popup-content-blocking-category-tracking-protection";
    }
    is(hidden(category + " > .identity-popup-content-blocking-category-add-blocking"), blockedByTP,
      "Category item is" + (blockedByTP ? " not" : "") + " showing add blocking");
    // Always hidden no matter if blockedByTP or not, since we have an exception.
    ok(hidden("#identity-popup-content-blocking-category-tracking-protection > .identity-popup-content-blocking-category-state-label"),
      "TP category item is not set to blocked");
  }
}

function testTrackingPageWithCBDisabled() {
  info("Tracking content must be white-listed and not blocked");
  ok(ContentBlocking.content.hasAttribute("detected"), "trackers are detected");
  ok(!ContentBlocking.content.hasAttribute("hasException"), "content shows no exception");
  ok(!ContentBlocking.content.hasAttribute("active"), "content is not active");

  ok(!ContentBlocking.iconBox.hasAttribute("active"), "shield is not active");
  ok(!ContentBlocking.iconBox.hasAttribute("hasException"), "shield shows no exception");
  ok(!ContentBlocking.iconBox.getAttribute("tooltiptext"), "icon box has no tooltip");

  ok(BrowserTestUtils.is_hidden(ContentBlocking.iconBox), "icon box is hidden");
  ok(hidden("#tracking-action-block"), "blockButton is hidden");
  ok(hidden("#tracking-action-unblock"), "unblockButton is hidden");
  ok(!hidden("#identity-popup-content-blocking-disabled-label"), "disabled label is visible");

  ok(hidden("#identity-popup-content-blocking-not-detected"), "blocking not detected label is hidden");
  ok(!hidden("#identity-popup-content-blocking-detected"), "blocking detected label is visible");

  if (Services.prefs.getBoolPref(CB_UI_PREF)) {
    ok(!hidden("#identity-popup-content-blocking-category-list"), "category list is visible");
    ok(!hidden("#identity-popup-content-blocking-category-tracking-protection > .identity-popup-content-blocking-category-add-blocking"),
      "TP category item is showing add blocking");
    ok(hidden("#identity-popup-content-blocking-category-tracking-protection > .identity-popup-content-blocking-category-state-label"),
      "TP category item is not set to blocked");
=======

  ok(
    hidden("#identity-popup-content-blocking-not-detected"),
    "blocking not detected label is hidden"
  );
  ok(
    !hidden("#identity-popup-content-blocking-detected"),
    "blocking detected label is visible"
  );

  ok(
    !hidden("#identity-popup-content-blocking-category-tracking-protection"),
    "Showing trackers category"
  );
  if (gTrackingPageURL == COOKIE_PAGE) {
    ok(
      !hidden("#identity-popup-content-blocking-category-cookies"),
      "Showing cookie restrictions category"
    );
  } else {
    ok(
      hidden("#identity-popup-content-blocking-category-cookies"),
      "Not showing cookie restrictions category"
    );
>>>>>>> upstream-releases
  }
}

async function testContentBlocking(tab) {
  info("Testing with Tracking Protection ENABLED.");

  info("Load a test page not containing tracking elements");
  await promiseTabLoadEvent(tab, BENIGN_PAGE);
  testBenignPage();

  info(
    "Load a test page not containing tracking elements which has an exception."
  );

  await promiseTabLoadEvent(tab, "https://example.org/?round=1");

  ContentBlockingAllowList.add(tab.linkedBrowser);
  // Load another page from the same origin to ensure there is an onlocationchange
  // notification which would trigger an oncontentblocking notification for us.
  await promiseTabLoadEvent(tab, "https://example.org/?round=2");

  testBenignPageWithException();

  ContentBlockingAllowList.remove(tab.linkedBrowser);

  info("Load a test page containing tracking elements");
  await promiseTabLoadEvent(tab, gTrackingPageURL);
  testTrackingPage(tab.ownerGlobal);

  info("Disable CB for the page (which reloads the page)");
  let tabReloadPromise = promiseTabLoadEvent(tab);
  clickButton("#tracking-action-unblock");
  await tabReloadPromise;
  let isPrivateBrowsing = PrivateBrowsingUtils.isWindowPrivate(tab.ownerGlobal);
  let blockedByTP = areTrackersBlocked(isPrivateBrowsing);
  testTrackingPageUnblocked(blockedByTP, tab.ownerGlobal);

  info("Re-enable TP for the page (which reloads the page)");
  tabReloadPromise = promiseTabLoadEvent(tab);
  clickButton("#tracking-action-block");
  await tabReloadPromise;
  testTrackingPage(tab.ownerGlobal);
}

add_task(async function testNormalBrowsing() {
  await UrlClassifierTestUtils.addTestTrackers();

  Services.prefs.setBoolPref(DTSCBN_PREF, true);

  tabbrowser = gBrowser;
  let tab = (tabbrowser.selectedTab = BrowserTestUtils.addTab(tabbrowser));

  ContentBlocking = gBrowser.ownerGlobal.ContentBlocking;
  ok(ContentBlocking, "CB is attached to the browser window");
  TrackingProtection = gBrowser.ownerGlobal.TrackingProtection;
  ok(TrackingProtection, "TP is attached to the browser window");
  is(
    TrackingProtection.enabled,
    Services.prefs.getBoolPref(TP_PREF),
    "TP.enabled is based on the original pref value"
  );

  Services.prefs.setIntPref(TPC_PREF, Ci.nsICookieService.BEHAVIOR_ACCEPT);

  await testContentBlocking(tab);

  Services.prefs.setBoolPref(TP_PREF, true);
  ok(TrackingProtection.enabled, "TP is enabled after setting the pref");
<<<<<<< HEAD

  await testContentBlocking(tab);
||||||| merged common ancestors
  Services.prefs.setBoolPref(CB_PREF, true);
  ok(ContentBlocking.enabled, "CB is enabled after setting the pref");

  await testContentBlockingEnabled(tab);

  if (Services.prefs.getBoolPref(CB_UI_PREF)) {
    Services.prefs.setBoolPref(CB_PREF, false);
    ok(!ContentBlocking.enabled, "CB is disabled after setting the pref");
  } else {
    Services.prefs.setBoolPref(TP_PREF, false);
    ok(!TrackingProtection.enabled, "TP is disabled after setting the pref");
  }

  await testContentBlockingDisabled(tab);
=======

  await testContentBlocking(tab);
>>>>>>> upstream-releases

  gBrowser.removeCurrentTab();

  Services.prefs.clearUserPref(TPC_PREF);
});

add_task(async function testPrivateBrowsing() {
  let privateWin = await BrowserTestUtils.openNewBrowserWindow({
    private: true,
  });
  tabbrowser = privateWin.gBrowser;
  let tab = (tabbrowser.selectedTab = BrowserTestUtils.addTab(tabbrowser));

  // Set the normal mode pref to false to check the pbmode pref.
  Services.prefs.setBoolPref(TP_PREF, false);

  Services.prefs.setIntPref(TPC_PREF, Ci.nsICookieService.BEHAVIOR_ACCEPT);

  ContentBlocking = tabbrowser.ownerGlobal.ContentBlocking;
  ok(ContentBlocking, "CB is attached to the private window");
  TrackingProtection = tabbrowser.ownerGlobal.TrackingProtection;
  ok(TrackingProtection, "TP is attached to the private window");
<<<<<<< HEAD
  is(TrackingProtection.enabled, Services.prefs.getBoolPref(TP_PB_PREF),
     "TP.enabled is based on the pb pref value");

  await testContentBlocking(tab);
||||||| merged common ancestors
  is(TrackingProtection.enabled, Services.prefs.getBoolPref(TP_PB_PREF),
     "TP.enabled is based on the pb pref value");

  await testContentBlockingEnabled(tab);

  if (Services.prefs.getBoolPref(CB_UI_PREF)) {
    Services.prefs.setBoolPref(CB_PREF, false);
    ok(!ContentBlocking.enabled, "CB is disabled after setting the pref");
  } else {
    Services.prefs.setBoolPref(TP_PB_PREF, false);
    ok(!TrackingProtection.enabled, "TP is disabled after setting the pref");
  }

  await testContentBlockingDisabled(tab);
=======
  is(
    TrackingProtection.enabled,
    Services.prefs.getBoolPref(TP_PB_PREF),
    "TP.enabled is based on the pb pref value"
  );

  await testContentBlocking(tab);
>>>>>>> upstream-releases

  Services.prefs.setBoolPref(TP_PB_PREF, true);
  ok(TrackingProtection.enabled, "TP is enabled after setting the pref");

  await testContentBlocking(tab);

  privateWin.close();

  Services.prefs.clearUserPref(TPC_PREF);
});

add_task(async function testThirdPartyCookies() {
  await UrlClassifierTestUtils.addTestTrackers();
  gTrackingPageURL = COOKIE_PAGE;

  tabbrowser = gBrowser;
  let tab = (tabbrowser.selectedTab = BrowserTestUtils.addTab(tabbrowser));

  ContentBlocking = gBrowser.ownerGlobal.ContentBlocking;
  ok(ContentBlocking, "CB is attached to the browser window");
  ThirdPartyCookies = gBrowser.ownerGlobal.ThirdPartyCookies;
  ok(ThirdPartyCookies, "TP is attached to the browser window");
<<<<<<< HEAD
  is(ThirdPartyCookies.enabled,
     Services.prefs.getIntPref(TPC_PREF) == Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER,
     "TPC.enabled is based on the original pref value");

  await testContentBlocking(tab);

  Services.prefs.setIntPref(TPC_PREF, Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER);
||||||| merged common ancestors
  is(ThirdPartyCookies.enabled,
     Services.prefs.getIntPref(TPC_PREF) == Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER,
     "TPC.enabled is based on the original pref value");
  Services.prefs.setBoolPref(CB_PREF, true);
  ok(ContentBlocking.enabled, "CB is enabled after setting the pref");

  await testContentBlockingEnabled(tab);

  ok(Services.prefs.getBoolPref(CB_UI_PREF), "CB UI must be enabled here");
  Services.prefs.setBoolPref(CB_PREF, false);
  ok(!ContentBlocking.enabled, "CB is disabled after setting the pref");

  await testContentBlockingDisabled(tab);

  Services.prefs.setIntPref(TPC_PREF, Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER);
=======
  is(
    ThirdPartyCookies.enabled,
    Services.prefs.getIntPref(TPC_PREF) ==
      Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER,
    "TPC.enabled is based on the original pref value"
  );

  await testContentBlocking(tab);

  Services.prefs.setIntPref(
    TPC_PREF,
    Ci.nsICookieService.BEHAVIOR_REJECT_TRACKER
  );
>>>>>>> upstream-releases
  ok(ThirdPartyCookies.enabled, "TPC is enabled after setting the pref");

  await testContentBlocking(tab);

  Services.prefs.clearUserPref(TPC_PREF);
  gBrowser.removeCurrentTab();
});
