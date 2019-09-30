/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const {
  ADB_ADDON_STATUS_UPDATED,
  ADB_READY_UPDATED,
  DEBUG_TARGET_COLLAPSIBILITY_UPDATED,
<<<<<<< HEAD
  NETWORK_LOCATIONS_UPDATED,
  PAGE_SELECTED,
  USB_RUNTIMES_SCAN_START,
  USB_RUNTIMES_SCAN_SUCCESS,
||||||| merged common ancestors
  NETWORK_LOCATIONS_UPDATED,
  PAGE_SELECTED,
=======
  HIDE_PROFILER_DIALOG,
  NETWORK_LOCATIONS_UPDATE_SUCCESS,
  SELECT_PAGE_SUCCESS,
  SHOW_PROFILER_DIALOG,
  TEMPORARY_EXTENSION_INSTALL_FAILURE,
  TEMPORARY_EXTENSION_INSTALL_SUCCESS,
  USB_RUNTIMES_SCAN_START,
  USB_RUNTIMES_SCAN_SUCCESS,
>>>>>>> upstream-releases
} = require("../constants");

<<<<<<< HEAD
function UiState(locations = [], debugTargetCollapsibilities = {},
                 networkEnabled = false, wifiEnabled = false, showSystemAddons = false) {
||||||| merged common ancestors
function UiState(locations = [], debugTargetCollapsibilities = {}) {
=======
function UiState(
  locations = [],
  debugTargetCollapsibilities = {},
  showHiddenAddons = false
) {
>>>>>>> upstream-releases
  return {
    adbAddonStatus: null,
    debugTargetCollapsibilities,
<<<<<<< HEAD
    isScanningUsb: false,
    networkEnabled,
||||||| merged common ancestors
=======
    isAdbReady: false,
    isScanningUsb: false,
>>>>>>> upstream-releases
    networkLocations: locations,
    selectedPage: null,
<<<<<<< HEAD
    selectedRuntime: null,
    showSystemAddons,
    wifiEnabled,
||||||| merged common ancestors
=======
    showProfilerDialog: false,
    showHiddenAddons,
    temporaryInstallError: null,
>>>>>>> upstream-releases
  };
}

function uiReducer(state = UiState(), action) {
  switch (action.type) {
    case ADB_ADDON_STATUS_UPDATED: {
      const { adbAddonStatus } = action;
      return Object.assign({}, state, { adbAddonStatus });
    }

    case ADB_READY_UPDATED: {
      const { isAdbReady } = action;
      return Object.assign({}, state, { isAdbReady });
    }

    case DEBUG_TARGET_COLLAPSIBILITY_UPDATED: {
      const { isCollapsed, key } = action;
      const debugTargetCollapsibilities = new Map(
        state.debugTargetCollapsibilities
      );
      debugTargetCollapsibilities.set(key, isCollapsed);
      return Object.assign({}, state, { debugTargetCollapsibilities });
    }

    case NETWORK_LOCATIONS_UPDATE_SUCCESS: {
      const { locations } = action;
      return Object.assign({}, state, { networkLocations: locations });
    }

<<<<<<< HEAD
    case PAGE_SELECTED: {
      const { page, runtimeId } = action;
      return Object.assign({}, state,
        { selectedPage: page, selectedRuntime: runtimeId });
    }

    case USB_RUNTIMES_SCAN_START: {
      return Object.assign({}, state, { isScanningUsb: true });
    }

    case USB_RUNTIMES_SCAN_SUCCESS: {
      return Object.assign({}, state, { isScanningUsb: false });
||||||| merged common ancestors
    case PAGE_SELECTED: {
      const { page } = action;
      return Object.assign({}, state, { selectedPage: page });
=======
    case SELECT_PAGE_SUCCESS: {
      const { page } = action;
      return Object.assign({}, state, { selectedPage: page });
>>>>>>> upstream-releases
    }

    case SHOW_PROFILER_DIALOG: {
      return Object.assign({}, state, { showProfilerDialog: true });
    }

    case HIDE_PROFILER_DIALOG: {
      return Object.assign({}, state, { showProfilerDialog: false });
    }

    case USB_RUNTIMES_SCAN_START: {
      return Object.assign({}, state, { isScanningUsb: true });
    }

    case USB_RUNTIMES_SCAN_SUCCESS: {
      return Object.assign({}, state, { isScanningUsb: false });
    }

    case TEMPORARY_EXTENSION_INSTALL_SUCCESS: {
      return Object.assign({}, state, { temporaryInstallError: null });
    }

    case TEMPORARY_EXTENSION_INSTALL_FAILURE: {
      const { error } = action;
      return Object.assign({}, state, { temporaryInstallError: error });
    }

    default:
      return state;
  }
}

module.exports = {
  UiState,
  uiReducer,
};
