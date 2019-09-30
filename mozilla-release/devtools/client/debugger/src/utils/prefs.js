/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js

// @flow

import { PrefsHelper } from "devtools-modules";
import { isDevelopment } from "devtools-environment";
import Services from "devtools-services";
import { asyncStoreHelper } from "./asyncStoreHelper";

const prefsSchemaVersion = "1.0.5";
||||||| merged common ancestors
const prefsSchemaVersion = "1.0.5";
const pref = _devtoolsServices2.default.pref;
=======
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
const pref = Services.pref;

if (isDevelopment()) {
  pref("devtools.debugger.logging", false);
||||||| merged common ancestors
if ((0, _devtoolsEnvironment.isDevelopment)()) {
=======
// @flow

import { PrefsHelper } from "devtools-modules";
import { isDevelopment } from "devtools-environment";
import Services from "devtools-services";
import { asyncStoreHelper } from "./asyncStoreHelper";

// Schema version to bump when the async store format has changed incompatibly
// and old stores should be cleared.
const prefsSchemaVersion = 11;
const pref = Services.pref;

if (isDevelopment()) {
  pref("devtools.debugger.logging", false);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
  pref("devtools.debugger.alphabetize-outline", false);
  pref("devtools.debugger.auto-pretty-print", false);
  pref("devtools.source-map.client-service.enabled", true);
  pref("devtools.chrome.enabled", false);
  pref("devtools.debugger.pause-on-exceptions", false);
  pref("devtools.debugger.pause-on-caught-exceptions", false);
  pref("devtools.debugger.ignore-caught-exceptions", true);
  pref("devtools.debugger.call-stack-visible", true);
  pref("devtools.debugger.scopes-visible", true);
  pref("devtools.debugger.component-visible", true);
  pref("devtools.debugger.workers-visible", true);
  pref("devtools.debugger.expressions-visible", true);
  pref("devtools.debugger.xhr-breakpoints-visible", true);
  pref("devtools.debugger.breakpoints-visible", true);
  pref("devtools.debugger.event-listeners-visible", true);
  pref("devtools.debugger.start-panel-collapsed", false);
  pref("devtools.debugger.end-panel-collapsed", false);
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
  pref("devtools.debugger.start-panel-size", 300);
  pref("devtools.debugger.end-panel-size", 300);
  pref("devtools.debugger.tabs", "[]");
||||||| merged common ancestors
  pref("devtools.debugger.tabs", "[]");
=======
  pref("devtools.debugger.start-panel-size", 300);
  pref("devtools.debugger.end-panel-size", 300);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
  pref("devtools.debugger.tabsBlackBoxed", "[]");
  pref("devtools.debugger.ui.editor-wrapping", false);
  pref("devtools.debugger.ui.framework-grouping-on", true);
  pref("devtools.debugger.pending-selected-location", "{}");
  pref("devtools.debugger.expressions", "[]");
  pref("devtools.debugger.file-search-case-sensitive", false);
  pref("devtools.debugger.file-search-whole-word", false);
  pref("devtools.debugger.file-search-regex-match", false);
  pref("devtools.debugger.project-directory-root", "");
  pref("devtools.debugger.map-scopes-enabled", false);
  pref("devtools.debugger.prefs-schema-version", prefsSchemaVersion);
  pref("devtools.debugger.skip-pausing", false);
  pref("devtools.debugger.features.workers", true);
  pref("devtools.debugger.features.async-stepping", true);
  pref("devtools.debugger.features.wasm", true);
  pref("devtools.debugger.features.shortcuts", true);
  pref("devtools.debugger.features.root", true);
  pref("devtools.debugger.features.map-scopes", true);
  pref("devtools.debugger.features.remove-command-bar-options", true);
  pref("devtools.debugger.features.code-folding", false);
  pref("devtools.debugger.features.outline", true);
  pref("devtools.debugger.features.column-breakpoints", true);
  pref("devtools.debugger.features.skip-pausing", true);
  pref("devtools.debugger.features.component-pane", false);
  pref("devtools.debugger.features.autocomplete-expressions", false);
  pref("devtools.debugger.features.map-expression-bindings", true);
  pref("devtools.debugger.features.map-await-expression", true);
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
  pref("devtools.debugger.features.xhr-breakpoints", true);
  pref("devtools.debugger.features.origial-blackbox", false);
||||||| merged common ancestors
=======
  pref("devtools.debugger.features.xhr-breakpoints", true);
  pref("devtools.debugger.features.original-blackbox", true);
  pref("devtools.debugger.features.windowless-workers", true);
  pref("devtools.debugger.features.event-listeners-breakpoints", true);
  pref("devtools.debugger.features.log-points", true);
  pref("devtools.debugger.log-actions", true);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
}

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
export const prefs = new PrefsHelper("devtools", {
  logging: ["Bool", "debugger.alphabetize-outline"],
||||||| merged common ancestors
const prefs = exports.prefs = new _devtoolsModules.PrefsHelper("devtools", {
=======
export const prefs = new PrefsHelper("devtools", {
  logging: ["Bool", "debugger.logging"],
  editorWrapping: ["Bool", "debugger.ui.editor-wrapping"],
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
  alphabetizeOutline: ["Bool", "debugger.alphabetize-outline"],
  autoPrettyPrint: ["Bool", "debugger.auto-pretty-print"],
  clientSourceMapsEnabled: ["Bool", "source-map.client-service.enabled"],
  chromeAndExtenstionsEnabled: ["Bool", "chrome.enabled"],
  pauseOnExceptions: ["Bool", "debugger.pause-on-exceptions"],
  pauseOnCaughtExceptions: ["Bool", "debugger.pause-on-caught-exceptions"],
  ignoreCaughtExceptions: ["Bool", "debugger.ignore-caught-exceptions"],
  callStackVisible: ["Bool", "debugger.call-stack-visible"],
  scopesVisible: ["Bool", "debugger.scopes-visible"],
  componentVisible: ["Bool", "debugger.component-visible"],
  workersVisible: ["Bool", "debugger.workers-visible"],
  breakpointsVisible: ["Bool", "debugger.breakpoints-visible"],
  expressionsVisible: ["Bool", "debugger.expressions-visible"],
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
  xhrBreakpointsVisible: ["Bool", "debugger.xhr-breakpoints-visible"],
||||||| merged common ancestors
=======
  xhrBreakpointsVisible: ["Bool", "debugger.xhr-breakpoints-visible"],
  eventListenersVisible: ["Bool", "debugger.event-listeners-visible"],
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
  startPanelCollapsed: ["Bool", "debugger.start-panel-collapsed"],
  endPanelCollapsed: ["Bool", "debugger.end-panel-collapsed"],
  startPanelSize: ["Int", "debugger.start-panel-size"],
  endPanelSize: ["Int", "debugger.end-panel-size"],
  frameworkGroupingOn: ["Bool", "debugger.ui.framework-grouping-on"],
  tabsBlackBoxed: ["Json", "debugger.tabsBlackBoxed", []],
  pendingSelectedLocation: ["Json", "debugger.pending-selected-location", {}],
  expressions: ["Json", "debugger.expressions", []],
  fileSearchCaseSensitive: ["Bool", "debugger.file-search-case-sensitive"],
  fileSearchWholeWord: ["Bool", "debugger.file-search-whole-word"],
  fileSearchRegexMatch: ["Bool", "debugger.file-search-regex-match"],
  debuggerPrefsSchemaVersion: ["Int", "debugger.prefs-schema-version"],
  projectDirectoryRoot: ["Char", "debugger.project-directory-root", ""],
  skipPausing: ["Bool", "debugger.skip-pausing"],
  mapScopes: ["Bool", "debugger.map-scopes-enabled"],
  logActions: ["Bool", "debugger.log-actions"],
});

export const features = new PrefsHelper("devtools.debugger.features", {
  asyncStepping: ["Bool", "async-stepping"],
  wasm: ["Bool", "wasm"],
  shortcuts: ["Bool", "shortcuts"],
  root: ["Bool", "root"],
  columnBreakpoints: ["Bool", "column-breakpoints"],
  mapScopes: ["Bool", "map-scopes"],
  removeCommandBarOptions: ["Bool", "remove-command-bar-options"],
  workers: ["Bool", "workers"],
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
||||||| merged common ancestors
  codeCoverage: ["Bool", "code-coverage"],
  eventListeners: ["Bool", "event-listeners"],
=======
  windowlessWorkers: ["Bool", "windowless-workers"],
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
  outline: ["Bool", "outline"],
  codeFolding: ["Bool", "code-folding"],
  skipPausing: ["Bool", "skip-pausing"],
  autocompleteExpression: ["Bool", "autocomplete-expressions"],
  mapExpressionBindings: ["Bool", "map-expression-bindings"],
  mapAwaitExpression: ["Bool", "map-await-expression"],
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
  componentPane: ["Bool", "component-pane"],
  xhrBreakpoints: ["Bool", "xhr-breakpoints"],
  originalBlackbox: ["Bool", "origial-blackbox"],
||||||| merged common ancestors
  componentPane: ["Bool", "component-pane"]
=======
  componentPane: ["Bool", "component-pane"],
  xhrBreakpoints: ["Bool", "xhr-breakpoints"],
  originalBlackbox: ["Bool", "original-blackbox"],
  eventListenersBreakpoints: ["Bool", "event-listeners-breakpoints"],
  logPoints: ["Bool", "log-points"],
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
});

export const asyncStore = asyncStoreHelper("debugger", {
  pendingBreakpoints: ["pending-breakpoints", {}],
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
  tabs: ["tabs", []],
  xhrBreakpoints: ["xhr-breakpoints", []]
||||||| merged common ancestors
  tabs: ["tabs", []]
=======
  tabs: ["tabs", []],
  xhrBreakpoints: ["xhr-breakpoints", []],
  eventListenerBreakpoints: ["event-listener-breakpoints", undefined],
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
});

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
if (prefs.debuggerPrefsSchemaVersion !== prefsSchemaVersion) {
  // clear pending Breakpoints
  prefs.pendingBreakpoints = {};
  prefs.tabs = [];
  prefs.xhrBreakpoints = [];
||||||| merged common ancestors
if (prefs.debuggerPrefsSchemaVersion !== prefsSchemaVersion) {
  // clear pending Breakpoints
  prefs.pendingBreakpoints = {};
  prefs.tabs = [];
=======
export function resetSchemaVersion() {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
  prefs.debuggerPrefsSchemaVersion = prefsSchemaVersion;
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/prefs.js
}
||||||| merged common ancestors
}
=======
}

export function verifyPrefSchema() {
  if (prefs.debuggerPrefsSchemaVersion < prefsSchemaVersion) {
    asyncStore.pendingBreakpoints = {};
    asyncStore.tabs = [];
    asyncStore.xhrBreakpoints = [];
    asyncStore.eventListenerBreakpoints = undefined;
    prefs.debuggerPrefsSchemaVersion = prefsSchemaVersion;
  }
}
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/prefs.js
