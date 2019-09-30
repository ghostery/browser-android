/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const { DEBUG_TARGET_PANE, PREFERENCES, RUNTIMES } = require("../constants");

<<<<<<< HEAD
const ALL_DEBUG_TARGETS = [
  DEBUG_TARGETS.EXTENSION,
  DEBUG_TARGETS.TAB,
  DEBUG_TARGETS.WORKER,
];

const SUPPORTED_TARGET_BY_RUNTIME = {
  [RUNTIMES.THIS_FIREFOX]: ALL_DEBUG_TARGETS,
  [RUNTIMES.USB]: [
    DEBUG_TARGETS.EXTENSION,
    DEBUG_TARGETS.TAB,
  ],
  [RUNTIMES.NETWORK]: [
    DEBUG_TARGETS.EXTENSION,
    DEBUG_TARGETS.TAB,
  ],
};
||||||| merged common ancestors
function isSupportedDebugTarget(runtimeType, debugTargetType) {
  if (runtimeType === RUNTIMES.THIS_FIREFOX) {
    return true;
  }
=======
const Services = require("Services");
>>>>>>> upstream-releases

<<<<<<< HEAD
function isSupportedDebugTarget(runtimeType, debugTargetType) {
  return SUPPORTED_TARGET_BY_RUNTIME[runtimeType].includes(debugTargetType);
||||||| merged common ancestors
  return debugTargetType === DEBUG_TARGETS.TAB;
=======
// Process target debugging is disabled by default.
function isProcessDebuggingSupported() {
  return Services.prefs.getBoolPref(
    PREFERENCES.PROCESS_DEBUGGING_ENABLED,
    false
  );
>>>>>>> upstream-releases
}

<<<<<<< HEAD
const ALL_DEBUG_TARGET_PANES = [
  DEBUG_TARGET_PANE.INSTALLED_EXTENSION,
  DEBUG_TARGET_PANE.OTHER_WORKER,
  DEBUG_TARGET_PANE.SERVICE_WORKER,
  DEBUG_TARGET_PANE.SHARED_WORKER,
  DEBUG_TARGET_PANE.TAB,
  DEBUG_TARGET_PANE.TEMPORARY_EXTENSION,
];

const SUPPORTED_TARGET_PANE_BY_RUNTIME = {
  [RUNTIMES.THIS_FIREFOX]: ALL_DEBUG_TARGET_PANES,
  [RUNTIMES.USB]: [
    DEBUG_TARGET_PANE.INSTALLED_EXTENSION,
    DEBUG_TARGET_PANE.TAB,
  ],
  [RUNTIMES.NETWORK]: [
    DEBUG_TARGET_PANE.INSTALLED_EXTENSION,
    DEBUG_TARGET_PANE.TAB,
  ],
};
||||||| merged common ancestors
function isSupportedDebugTargetPane(runtimeType, debugTargetPaneKey) {
  if (runtimeType === RUNTIMES.THIS_FIREFOX) {
    return true;
  }
=======
// Process target debugging is disabled by default.
function isLocalTabDebuggingSupported() {
  return Services.prefs.getBoolPref(
    PREFERENCES.LOCAL_TAB_DEBUGGING_ENABLED,
    false
  );
}

// Installing extensions can be disabled in enterprise policy.
// Note: Temporary Extensions are only supported when debugging This Firefox, so checking
// the local preference is acceptable here. If we enable Temporary extensions for remote
// runtimes, we should retrieve the preference from the target runtime instead.
function isTemporaryExtensionSupported() {
  return Services.prefs.getBoolPref(PREFERENCES.XPINSTALL_ENABLED, true);
}

const ALL_DEBUG_TARGET_PANES = [
  DEBUG_TARGET_PANE.INSTALLED_EXTENSION,
  ...(isProcessDebuggingSupported() ? [DEBUG_TARGET_PANE.PROCESSES] : []),
  DEBUG_TARGET_PANE.OTHER_WORKER,
  DEBUG_TARGET_PANE.SERVICE_WORKER,
  DEBUG_TARGET_PANE.SHARED_WORKER,
  DEBUG_TARGET_PANE.TAB,
  ...(isTemporaryExtensionSupported()
    ? [DEBUG_TARGET_PANE.TEMPORARY_EXTENSION]
    : []),
];
>>>>>>> upstream-releases

<<<<<<< HEAD
/**
 * A debug target pane is more specialized than a debug target. For instance EXTENSION is
 * a DEBUG_TARGET but INSTALLED_EXTENSION and TEMPORARY_EXTENSION are DEBUG_TARGET_PANES.
 */
function isSupportedDebugTargetPane(runtimeType, debugTargetPaneKey) {
  return SUPPORTED_TARGET_PANE_BY_RUNTIME[runtimeType].includes(debugTargetPaneKey);
||||||| merged common ancestors
  return debugTargetPaneKey === DEBUG_TARGET_PANE.TAB;
=======
// All debug target panes except temporary extensions
const REMOTE_DEBUG_TARGET_PANES = ALL_DEBUG_TARGET_PANES.filter(
  p => p !== DEBUG_TARGET_PANE.TEMPORARY_EXTENSION
);

const THIS_FIREFOX_DEBUG_TARGET_PANES = ALL_DEBUG_TARGET_PANES
  // Main process debugging is not available for This Firefox.
  // At the moment only the main process is listed under processes, so remove the category
  // for this runtime.
  .filter(p => p !== DEBUG_TARGET_PANE.PROCESSES)
  // Showing tab targets for This Firefox is behind a preference.
  .filter(p => p !== DEBUG_TARGET_PANE.TAB || isLocalTabDebuggingSupported());

const SUPPORTED_TARGET_PANE_BY_RUNTIME = {
  [RUNTIMES.THIS_FIREFOX]: THIS_FIREFOX_DEBUG_TARGET_PANES,
  [RUNTIMES.USB]: REMOTE_DEBUG_TARGET_PANES,
  [RUNTIMES.NETWORK]: REMOTE_DEBUG_TARGET_PANES,
};

/**
 * A debug target pane is more specialized than a debug target. For instance EXTENSION is
 * a DEBUG_TARGET but INSTALLED_EXTENSION and TEMPORARY_EXTENSION are DEBUG_TARGET_PANES.
 */
function isSupportedDebugTargetPane(runtimeType, debugTargetPaneKey) {
  return SUPPORTED_TARGET_PANE_BY_RUNTIME[runtimeType].includes(
    debugTargetPaneKey
  );
>>>>>>> upstream-releases
}
exports.isSupportedDebugTargetPane = isSupportedDebugTargetPane;
