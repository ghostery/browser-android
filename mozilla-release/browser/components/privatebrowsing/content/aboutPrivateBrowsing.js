/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* eslint-env mozilla/frame-script */

<<<<<<< HEAD
const TP_PB_ENABLED_PREF = "privacy.trackingprotection.pbmode.enabled";

||||||| merged common ancestors
const CB_ENABLED_PREF = "browser.contentblocking.enabled";
const CB_UI_ENABLED_PREF = "browser.contentblocking.ui.enabled";
const TP_ENABLED_PREF = "privacy.trackingprotection.enabled";
const TP_PB_ENABLED_PREF = "privacy.trackingprotection.pbmode.enabled";

let contentBlockingUIEnabled = false;

function updateTPInfo() {
  let tpButton = document.getElementById("tpButton");
  let tpToggle = document.getElementById("tpToggle");
  let title = document.getElementById("title");
  let titleTracking = document.getElementById("titleTracking");
  let tpSubHeader = document.getElementById("tpSubHeader");

  let tpTitle = document.getElementById("tpTitle");
  let cbTitle = document.getElementById("cbTitle");
  let tpDescription = document.getElementById("tpDescription");
  let cbDescription = document.getElementById("cbDescription");

  tpTitle.toggleAttribute("hidden", contentBlockingUIEnabled);
  tpDescription.toggleAttribute("hidden", contentBlockingUIEnabled);

  cbTitle.toggleAttribute("hidden", !contentBlockingUIEnabled);
  cbDescription.toggleAttribute("hidden", !contentBlockingUIEnabled);

  let globalTrackingEnabled = RPMGetBoolPref(TP_ENABLED_PREF);
  let trackingEnabled = globalTrackingEnabled || RPMGetBoolPref(TP_PB_ENABLED_PREF);

  if (contentBlockingUIEnabled) {
    let contentBlockingEnabled = RPMGetBoolPref(CB_ENABLED_PREF);
    trackingEnabled = trackingEnabled && contentBlockingEnabled;
  } else {
    title.toggleAttribute("hidden", trackingEnabled);
    titleTracking.toggleAttribute("hidden", !trackingEnabled);
  }

  // if tracking protection is enabled globally we don't even give the user
  // a choice here by hiding the toggle completely.
  tpButton.toggleAttribute("hidden", globalTrackingEnabled);
  tpToggle.checked = trackingEnabled;

  tpSubHeader.classList.toggle("tp-off", !trackingEnabled);
}

=======
>>>>>>> upstream-releases
document.addEventListener("DOMContentLoaded", function() {
  if (!RPMIsWindowPrivate()) {
    document.documentElement.classList.remove("private");
    document.documentElement.classList.add("normal");
    document
      .getElementById("startPrivateBrowsing")
      .addEventListener("click", function() {
        RPMSendAsyncMessage("OpenPrivateWindow");
      });
    return;
  }

<<<<<<< HEAD
  document.getElementById("startTour").addEventListener("click", function() {
    RPMSendAsyncMessage("DontShowIntroPanelAgain");
  });

  let introURL = RPMGetFormatURLPref("privacy.trackingprotection.introURL");
  // Variation 1 is specific to the Content Blocking UI.
  let variation = "?variation=1";

  document.getElementById("startTour").setAttribute("href", introURL + variation);
||||||| merged common ancestors
  contentBlockingUIEnabled = RPMGetBoolPref(CB_UI_ENABLED_PREF);

  document.getElementById("startTour").addEventListener("click", function() {
    RPMSendAsyncMessage("DontShowIntroPanelAgain");
  });

  let introURL = RPMGetFormatURLPref("privacy.trackingprotection.introURL");
  // If the CB UI is enabled, tell the tour page to show a different variation
  // that is updated to reflect the CB control center UI.
  let variation = "?variation=" + (contentBlockingUIEnabled ? "1" : "0");

  document.getElementById("startTour").setAttribute("href", introURL + variation);
=======
  // Setup the private browsing myths link.
  document
    .getElementById("private-browsing-myths")
    .setAttribute(
      "href",
      RPMGetFormatURLPref("app.support.baseURL") + "private-browsing-myths"
    );

  // Setup the search hand-off box.
  let btn = document.getElementById("search-handoff-button");
  let editable = document.getElementById("fake-editable");
  let HIDE_SEARCH_TOPIC = "HideSearch";
  let SHOW_SEARCH_TOPIC = "ShowSearch";
  let SEARCH_HANDOFF_TOPIC = "SearchHandoff";

  function showSearch() {
    btn.classList.remove("focused");
    btn.classList.remove("hidden");
    RPMRemoveMessageListener(SHOW_SEARCH_TOPIC, showSearch);
  }
>>>>>>> upstream-releases

  function hideSearch() {
    btn.classList.add("hidden");
  }

<<<<<<< HEAD
  let tpEnabled = RPMGetBoolPref(TP_PB_ENABLED_PREF);
  if (!tpEnabled) {
    document.getElementById("tpSubHeader").remove();
    document.getElementById("tpSection").remove();
  }
||||||| merged common ancestors
  let tpToggle = document.getElementById("tpToggle");
  document.getElementById("tpButton").addEventListener("click", () => {
    tpToggle.click();
  });
  tpToggle.addEventListener("change", async function() {
    let promises = [];
    if (tpToggle.checked && contentBlockingUIEnabled) {
      promises.push(RPMSetBoolPref(CB_ENABLED_PREF, true));
    }

    promises.push(RPMSetBoolPref(TP_PB_ENABLED_PREF, tpToggle.checked));

    await Promise.all(promises);

    updateTPInfo();
  });

  updateTPInfo();
=======
  function handoffSearch(text) {
    RPMSendAsyncMessage(SEARCH_HANDOFF_TOPIC, { text });
    RPMAddMessageListener(SHOW_SEARCH_TOPIC, showSearch);
    if (text) {
      hideSearch();
    } else {
      btn.classList.add("focused");
      RPMAddMessageListener(HIDE_SEARCH_TOPIC, hideSearch);
    }
  }
  btn.addEventListener("focus", function() {
    handoffSearch();
  });
  btn.addEventListener("click", function() {
    handoffSearch();
  });

  // Hand-off any text that gets dropped or pasted
  editable.addEventListener("drop", function(ev) {
    ev.preventDefault();
    let text = ev.dataTransfer.getData("text");
    if (text) {
      handoffSearch(text);
    }
  });
  editable.addEventListener("paste", function(ev) {
    ev.preventDefault();
    handoffSearch(ev.clipboardData.getData("Text"));
  });

  // Load contentSearchUI so it sets the search engine icon for us.
  // TODO: FIXME. We should eventually refector contentSearchUI to do only what
  // we need and have it do the common search handoff work for
  // about:newtab and about:privatebrowsing.
  let input = document.getElementById("dummy-input");
  new window.ContentSearchUIController(
    input,
    input.parentNode,
    "aboutprivatebrowsing",
    "aboutprivatebrowsing"
  );
>>>>>>> upstream-releases
});
