/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

<<<<<<< HEAD
const Services = require("Services");

const { applyMiddleware, createStore } = require("devtools/client/shared/vendor/redux");
||||||| merged common ancestors
const { applyMiddleware, createStore } = require("devtools/client/shared/vendor/redux");
=======
const Services = require("Services");

const {
  applyMiddleware,
  createStore,
} = require("devtools/client/shared/vendor/redux");
>>>>>>> upstream-releases
const { thunk } = require("devtools/client/shared/redux/middleware/thunk.js");
<<<<<<< HEAD
const { waitUntilService } = require("devtools/client/shared/redux/middleware/wait-service.js");
||||||| merged common ancestors
=======
const {
  waitUntilService,
} = require("devtools/client/shared/redux/middleware/wait-service.js");
>>>>>>> upstream-releases

const rootReducer = require("./reducers/index");
const { DebugTargetsState } = require("./reducers/debug-targets-state");
const { RuntimesState } = require("./reducers/runtimes-state");
const { UiState } = require("./reducers/ui-state");
const debugTargetListenerMiddleware = require("./middleware/debug-target-listener");
<<<<<<< HEAD
const errorLoggingMiddleware = require("./middleware/error-logging");
||||||| merged common ancestors
=======
const errorLoggingMiddleware = require("./middleware/error-logging");
const eventRecordingMiddleware = require("./middleware/event-recording");
>>>>>>> upstream-releases
const extensionComponentDataMiddleware = require("./middleware/extension-component-data");
const processComponentDataMiddleware = require("./middleware/process-component-data");
const tabComponentDataMiddleware = require("./middleware/tab-component-data");
const workerComponentDataMiddleware = require("./middleware/worker-component-data");
const {
  getDebugTargetCollapsibilities,
} = require("./modules/debug-target-collapsibilities");
const { getNetworkLocations } = require("./modules/network-locations");

const { PREFERENCES } = require("./constants");

function configureStore() {
  const initialState = {
    debugTargets: new DebugTargetsState(),
    runtimes: new RuntimesState(),
    ui: getUiState(),
  };

<<<<<<< HEAD
  const middleware = applyMiddleware(thunk,
                                     debugTargetListenerMiddleware,
                                     errorLoggingMiddleware,
                                     extensionComponentDataMiddleware,
                                     tabComponentDataMiddleware,
                                     workerComponentDataMiddleware,
                                     waitUntilService);
||||||| merged common ancestors
  const middleware = applyMiddleware(thunk,
                                     debugTargetListenerMiddleware,
                                     extensionComponentDataMiddleware,
                                     tabComponentDataMiddleware,
                                     workerComponentDataMiddleware);
=======
  const middleware = applyMiddleware(
    thunk,
    debugTargetListenerMiddleware,
    errorLoggingMiddleware,
    eventRecordingMiddleware,
    extensionComponentDataMiddleware,
    processComponentDataMiddleware,
    tabComponentDataMiddleware,
    workerComponentDataMiddleware,
    waitUntilService
  );
>>>>>>> upstream-releases

  return createStore(rootReducer, initialState, middleware);
}

function getUiState() {
  const collapsibilities = getDebugTargetCollapsibilities();
  const locations = getNetworkLocations();
<<<<<<< HEAD
  const networkEnabled = Services.prefs.getBoolPref(PREFERENCES.NETWORK_ENABLED, false);
  const wifiEnabled = Services.prefs.getBoolPref(PREFERENCES.WIFI_ENABLED, false);
  const showSystemAddons = Services.prefs.getBoolPref(PREFERENCES.SHOW_SYSTEM_ADDONS,
    false);
  return new UiState(locations, collapsibilities, networkEnabled, wifiEnabled,
    showSystemAddons);
||||||| merged common ancestors
  return new UiState(locations, collapsibilities);
=======
  const showHiddenAddons = Services.prefs.getBoolPref(
    PREFERENCES.SHOW_HIDDEN_ADDONS,
    false
  );
  return new UiState(locations, collapsibilities, showHiddenAddons);
>>>>>>> upstream-releases
}

exports.configureStore = configureStore;
