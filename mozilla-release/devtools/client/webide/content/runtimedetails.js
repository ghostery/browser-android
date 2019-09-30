/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
const {require} = ChromeUtils.import("resource://devtools/shared/Loader.jsm", {});
const {AppManager} = require("devtools/client/webide/modules/app-manager");
const {Connection} = require("devtools/shared/client/connection-manager");

window.addEventListener("load", function() {
  document.querySelector("#close").onclick = CloseUI;
  AppManager.on("app-manager-update", OnAppManagerUpdate);
  BuildUI();
}, {capture: true, once: true});

window.addEventListener("unload", function() {
  AppManager.off("app-manager-update", OnAppManagerUpdate);
}, {once: true});
||||||| merged common ancestors
const {require} = ChromeUtils.import("resource://devtools/shared/Loader.jsm", {});
const Services = require("Services");
const {AppManager} = require("devtools/client/webide/modules/app-manager");
const {Connection} = require("devtools/shared/client/connection-manager");
const {RuntimeTypes} = require("devtools/client/webide/modules/runtime-types");
const Strings = Services.strings.createBundle("chrome://devtools/locale/webide.properties");

const UNRESTRICTED_HELP_URL = "https://developer.mozilla.org/docs/Tools/WebIDE/Running_and_debugging_apps#Unrestricted_app_debugging_%28including_certified_apps_main_process_etc.%29";

window.addEventListener("load", function() {
  document.querySelector("#close").onclick = CloseUI;
  document.querySelector("#devtools-check button").onclick = EnableCertApps;
  document.querySelector("#adb-check button").onclick = RootADB;
  document.querySelector("#unrestricted-privileges").onclick = function() {
    window.parent.UI.openInBrowser(UNRESTRICTED_HELP_URL);
  };
  AppManager.on("app-manager-update", OnAppManagerUpdate);
  BuildUI();
  CheckLockState();
}, {capture: true, once: true});

window.addEventListener("unload", function() {
  AppManager.off("app-manager-update", OnAppManagerUpdate);
}, {once: true});
=======
const { require } = ChromeUtils.import("resource://devtools/shared/Loader.jsm");
const { AppManager } = require("devtools/client/webide/modules/app-manager");
const { Connection } = require("devtools/shared/client/connection-manager");

window.addEventListener(
  "load",
  function() {
    document.querySelector("#close").onclick = CloseUI;
    AppManager.on("app-manager-update", OnAppManagerUpdate);
    BuildUI();
  },
  { capture: true, once: true }
);

window.addEventListener(
  "unload",
  function() {
    AppManager.off("app-manager-update", OnAppManagerUpdate);
  },
  { once: true }
);
>>>>>>> upstream-releases

function CloseUI() {
  window.parent.UI.openProject();
}

function OnAppManagerUpdate(what) {
  if (what == "connection" || what == "runtime-global-actors") {
    BuildUI();
  }
}

function generateFields(json) {
  const table = document.querySelector("table");
  for (const name in json) {
    const tr = document.createElement("tr");
    let td = document.createElement("td");
    td.textContent = name;
    tr.appendChild(td);
    td = document.createElement("td");
    td.textContent = json[name];
    tr.appendChild(td);
    table.appendChild(tr);
  }
}

// Used by tests
/* exported getDescriptionPromise */
var getDescriptionPromise;
function BuildUI() {
  const table = document.querySelector("table");
  table.innerHTML = "";
  if (
    AppManager.connection &&
    AppManager.connection.status == Connection.Status.CONNECTED &&
    AppManager.deviceFront
  ) {
    getDescriptionPromise = AppManager.deviceFront
      .getDescription()
      .then(json => generateFields(json));
  } else {
    CloseUI();
  }
}
