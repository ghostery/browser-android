/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

<<<<<<< HEAD
ChromeUtils.import("resource://gre/modules/Services.jsm");
ChromeUtils.import("resource://gre/modules/XPCOMUtils.jsm");

||||||| merged common ancestors
ChromeUtils.import("resource://gre/modules/Services.jsm");
ChromeUtils.import("resource://gre/modules/XPCOMUtils.jsm");

const bundle = Services.strings.createBundle(
  "chrome://global/locale/aboutServiceWorkers.properties");

const brandBundle = Services.strings.createBundle(
  "chrome://branding/locale/brand.properties");

=======
const { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");

>>>>>>> upstream-releases
var gSWM;
var gSWCount = 0;

function init() {
  let enabled = Services.prefs.getBoolPref("dom.serviceWorkers.enabled");
  if (!enabled) {
    let div = document.getElementById("warning_not_enabled");
    div.classList.add("active");
    return;
  }

  gSWM = Cc["@mozilla.org/serviceworkers/manager;1"].getService(
    Ci.nsIServiceWorkerManager
  );
  if (!gSWM) {
    dump(
      "AboutServiceWorkers: Failed to get the ServiceWorkerManager service!\n"
    );
    return;
  }

  let data = gSWM.getAllRegistrations();
  if (!data) {
    dump("AboutServiceWorkers: Failed to retrieve the registrations.\n");
    return;
  }

  let length = data.length;
  if (!length) {
    let div = document.getElementById("warning_no_serviceworkers");
    div.classList.add("active");
    return;
  }

  let ps = undefined;
  try {
    ps = Cc["@mozilla.org/push/Service;1"].getService(Ci.nsIPushService);
  } catch (e) {
    dump("Could not acquire PushService\n");
  }

  for (let i = 0; i < length; ++i) {
    let info = data.queryElementAt(i, Ci.nsIServiceWorkerRegistrationInfo);
    if (!info) {
      dump(
        "AboutServiceWorkers: Invalid nsIServiceWorkerRegistrationInfo interface.\n"
      );
      continue;
    }

    display(info, ps);
  }
}

async function display(info, pushService) {
  let parent = document.getElementById("serviceworkers");

  let div = document.createElement("div");
  parent.appendChild(div);

  let title = document.createElement("h2");
<<<<<<< HEAD
  document.l10n.setAttributes(title, "origin-title", { originTitle: info.principal.origin });
||||||| merged common ancestors
  let titleStr = bundle.formatStringFromName("title", [info.principal.origin], 1);
  title.appendChild(document.createTextNode(titleStr));
=======
  document.l10n.setAttributes(title, "origin-title", {
    originTitle: info.principal.origin,
  });
>>>>>>> upstream-releases
  div.appendChild(title);

<<<<<<< HEAD
  if (info.principal.appId) {
    let b2gtitle = document.createElement("h3");
    let trueFalse = info.principal.isInIsolatedMozBrowserElement ? "true" : "false";
    document.l10n.setAttributes(b2gtitle, "app-title", { appId: info.principal.appId, isInIsolatedElement: trueFalse });
    div.appendChild(b2gtitle);
  }

||||||| merged common ancestors
  if (info.principal.appId) {
    let b2gtitle = document.createElement("h3");
    let trueFalse = bundle.GetStringFromName(info.principal.isInIsolatedMozBrowserElement ? "true" : "false");

    let b2gtitleStr =
      bundle.formatStringFromName("b2gtitle", [ brandBundle.getString("brandShortName"),
                                                info.principal.appId,
                                                trueFalse], 2);
    b2gtitle.appendChild(document.createTextNode(b2gtitleStr));
    div.appendChild(b2gtitle);
  }

=======
>>>>>>> upstream-releases
  let list = document.createElement("ul");
  div.appendChild(list);

  function createItem(l10nId, value, makeLink) {
    let item = document.createElement("li");
    list.appendChild(item);
    let bold = document.createElement("strong");
    bold.setAttribute("data-l10n-name", "item-label");
    item.appendChild(bold);
    if (!value) {
      document.l10n.setAttributes(item, l10nId);
    } else if (makeLink) {
      let link = document.createElement("a");
      link.setAttribute("target", "_blank");
      link.setAttribute("data-l10n-name", "link");
      link.setAttribute("href", value);
      item.appendChild(link);
      document.l10n.setAttributes(item, l10nId, { url: value });
    } else {
      document.l10n.setAttributes(item, l10nId, { name: value });
    }
<<<<<<< HEAD
      return item;
||||||| merged common ancestors

    return textNode;
=======
    return item;
>>>>>>> upstream-releases
  }

  createItem("scope", info.scope);
  createItem("script-spec", info.scriptSpec, true);
  let currentWorkerURL = info.activeWorker ? info.activeWorker.scriptSpec : "";
  createItem("current-worker-url", currentWorkerURL, true);
  let activeCacheName = info.activeWorker ? info.activeWorker.cacheName : "";
  createItem("active-cache-name", activeCacheName);
  let waitingCacheName = info.waitingWorker ? info.waitingWorker.cacheName : "";
  createItem("waiting-cache-name", waitingCacheName);

  let pushItem = createItem("push-end-point-waiting");
  if (pushService) {
<<<<<<< HEAD
    pushService.getSubscription(info.scope, info.principal, (status, pushRecord) => {
      if (Components.isSuccessCode(status)) {
        document.l10n.setAttributes(pushItem, "push-end-point-result", { name: JSON.stringify(pushRecord) });
      } else {
        dump("about:serviceworkers - retrieving push registration failed\n");
||||||| merged common ancestors
    pushService.getSubscription(info.scope, info.principal, (status, pushRecord) => {
      if (Components.isSuccessCode(status)) {
        pushItem.data = JSON.stringify(pushRecord);
      } else {
        dump("about:serviceworkers - retrieving push registration failed\n");
=======
    pushService.getSubscription(
      info.scope,
      info.principal,
      (status, pushRecord) => {
        if (Components.isSuccessCode(status)) {
          document.l10n.setAttributes(pushItem, "push-end-point-result", {
            name: JSON.stringify(pushRecord),
          });
        } else {
          dump("about:serviceworkers - retrieving push registration failed\n");
        }
>>>>>>> upstream-releases
      }
    );
  }

  let updateButton = document.createElement("button");
  document.l10n.setAttributes(updateButton, "update-button");
  updateButton.onclick = function() {
    gSWM.propagateSoftUpdate(info.principal.originAttributes, info.scope);
  };
  div.appendChild(updateButton);

  let unregisterButton = document.createElement("button");
  document.l10n.setAttributes(unregisterButton, "unregister-button");
  div.appendChild(unregisterButton);

  let loadingMessage = document.createElement("span");
  document.l10n.setAttributes(loadingMessage, "waiting");
  loadingMessage.classList.add("inactive");
  div.appendChild(loadingMessage);

  unregisterButton.onclick = function() {
    let cb = {
      unregisterSucceeded() {
        parent.removeChild(div);

        if (!--gSWCount) {
          let div = document.getElementById("warning_no_serviceworkers");
          div.classList.add("active");
        }
      },

<<<<<<< HEAD
      async unregisterFailed() {
        let [alertMsg] = await document.l10n.formatValues([{ id: "unregister-error" }]);
        alert(alertMsg);
||||||| merged common ancestors
      unregisterFailed() {
        alert(bundle.GetStringFromName("unregisterError"));
=======
      async unregisterFailed() {
        let [alertMsg] = await document.l10n.formatValues([
          { id: "unregister-error" },
        ]);
        alert(alertMsg);
>>>>>>> upstream-releases
      },

      QueryInterface: ChromeUtils.generateQI([
        Ci.nsIServiceWorkerUnregisterCallback,
      ]),
    };

    loadingMessage.classList.remove("inactive");
    gSWM.propagateUnregister(info.principal, cb, info.scope);
  };

  let sep = document.createElement("hr");
  div.appendChild(sep);

  ++gSWCount;
}

window.addEventListener(
  "DOMContentLoaded",
  function() {
    init();
  },
  { once: true }
);
