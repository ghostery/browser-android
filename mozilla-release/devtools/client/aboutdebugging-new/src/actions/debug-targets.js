/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const { AddonManager } = require("resource://gre/modules/AddonManager.jsm");
<<<<<<< HEAD
const { gDevToolsBrowser } = require("devtools/client/framework/devtools-browser");
const { remoteClientManager } =
  require("devtools/client/shared/remote-debugging/remote-client-manager");

const { l10n } = require("../modules/l10n");
||||||| merged common ancestors
const { gDevToolsBrowser } = require("devtools/client/framework/devtools-browser");
=======
const {
  remoteClientManager,
} = require("devtools/client/shared/remote-debugging/remote-client-manager");
const Services = require("Services");

const { l10n } = require("../modules/l10n");

const {
  isSupportedDebugTargetPane,
} = require("../modules/debug-target-support");
>>>>>>> upstream-releases

const {
  openTemporaryExtension,
  uninstallAddon,
} = require("../modules/extensions-helper");

const {
  getCurrentClient,
  getCurrentRuntime,
} = require("../modules/runtimes-state-helper");

const {
  DEBUG_TARGETS,
  DEBUG_TARGET_PANE,
  REQUEST_EXTENSIONS_FAILURE,
  REQUEST_EXTENSIONS_START,
  REQUEST_EXTENSIONS_SUCCESS,
  REQUEST_PROCESSES_FAILURE,
  REQUEST_PROCESSES_START,
  REQUEST_PROCESSES_SUCCESS,
  REQUEST_TABS_FAILURE,
  REQUEST_TABS_START,
  REQUEST_TABS_SUCCESS,
  REQUEST_WORKERS_FAILURE,
  REQUEST_WORKERS_START,
  REQUEST_WORKERS_SUCCESS,
  TEMPORARY_EXTENSION_INSTALL_FAILURE,
  TEMPORARY_EXTENSION_INSTALL_START,
  TEMPORARY_EXTENSION_INSTALL_SUCCESS,
  TEMPORARY_EXTENSION_RELOAD_FAILURE,
  TEMPORARY_EXTENSION_RELOAD_START,
  TEMPORARY_EXTENSION_RELOAD_SUCCESS,
  RUNTIMES,
} = require("../constants");

const Actions = require("./index");

function isCachedActorNeeded(runtime, type, id) {
  // Unique ids for workers were introduced in Firefox 68 (Bug 1539328). When debugging
  // older browsers, the id falls back to the actor ID. Check if the target id is a worker
  // actorID (which means getActor() should return an actor with id).
  // Can be removed when Firefox 68 is in Release channel.
  return (
    type === DEBUG_TARGETS.WORKER &&
    runtime.runtimeDetails.clientWrapper.client.getActor(id)
  );
}

function getTabForUrl(url) {
  for (const navigator of Services.wm.getEnumerator("navigator:browser")) {
    for (const browser of navigator.gBrowser.browsers) {
      if (
        browser.contentWindow &&
        browser.contentWindow.location.href === url
      ) {
        return navigator.gBrowser.getTabForBrowser(browser);
      }
    }
  }

  return null;
}

function inspectDebugTarget(type, id) {
  return async (dispatch, getState) => {
    const runtime = getCurrentRuntime(getState().runtimes);
<<<<<<< HEAD
    const { runtimeDetails, type: runtimeType } = runtime;

    switch (type) {
      case DEBUG_TARGETS.TAB: {
        // Open tab debugger in new window.
        if (runtimeType === RUNTIMES.NETWORK || runtimeType === RUNTIMES.USB) {
          // Pass the remote id from the client manager so that about:devtools-toolbox can
          // retrieve the connected client directly.
          const remoteId = remoteClientManager.getRemoteId(runtime.id, runtime.type);
          window.open(`about:devtools-toolbox?type=tab&id=${id}&remoteId=${remoteId}`);
        } else if (runtimeType === RUNTIMES.THIS_FIREFOX) {
          window.open(`about:devtools-toolbox?type=tab&id=${id}`);
        }
        break;
      }
      case DEBUG_TARGETS.EXTENSION: {
        if (runtimeType === RUNTIMES.NETWORK || runtimeType === RUNTIMES.USB) {
          const devtoolsClient = runtimeDetails.clientWrapper.client;
          await debugRemoteAddon(id, devtoolsClient);
        } else if (runtimeType === RUNTIMES.THIS_FIREFOX) {
          debugLocalAddon(id);
        }
        break;
      }
      case DEBUG_TARGETS.WORKER: {
        // Open worker toolbox in new window.
        const devtoolsClient = runtimeDetails.clientWrapper.client;
        const front = devtoolsClient.getActor(id);
        gDevToolsBrowser.openWorkerToolbox(front);
        break;
      }
||||||| merged common ancestors
    const { connection, type: runtimeType } = runtime;

    switch (type) {
      case DEBUG_TARGETS.TAB: {
        // Open tab debugger in new window.
        if (runtimeType === RUNTIMES.NETWORK || runtimeType === RUNTIMES.USB) {
          const { host, port } = connection.transportDetails;
          window.open(`about:devtools-toolbox?type=tab&id=${id}` +
                      `&host=${host}&port=${port}`);
        } else if (runtimeType === RUNTIMES.THIS_FIREFOX) {
          window.open(`about:devtools-toolbox?type=tab&id=${id}`);
        }
        break;
      }
      case DEBUG_TARGETS.EXTENSION: {
        if (runtimeType === RUNTIMES.NETWORK) {
          await debugRemoteAddon(id, connection.client);
        } else if (runtimeType === RUNTIMES.THIS_FIREFOX) {
          debugLocalAddon(id);
        }
        break;
      }
      case DEBUG_TARGETS.WORKER: {
        // Open worker toolbox in new window.
        gDevToolsBrowser.openWorkerToolbox(connection.client, id);
        break;
      }
=======
    id = encodeURIComponent(id);
>>>>>>> upstream-releases

    let url;
    if (
      runtime.id === RUNTIMES.THIS_FIREFOX &&
      !isCachedActorNeeded(runtime, type, id)
    ) {
      // Even when debugging on This Firefox we need to re-use the client since the worker
      // actor is cached in the client instance. Instead we should pass an id that does
      // not depend on the client (such as the worker url). This will be fixed in
      // Bug 1539328.
      // Once the target is destroyed after closing the toolbox, the front will be gone
      // and can no longer be used. When debugging This Firefox, workers are regularly
      // updated so this is not an issue. On remote runtimes however, trying to inspect a
      // worker a second time after closing the corresponding about:devtools-toolbox tab
      // will fail. See Bug 1534201.
      url = `about:devtools-toolbox?type=${type}&id=${id}`;
    } else {
      const remoteId = remoteClientManager.getRemoteId(
        runtime.id,
        runtime.type
      );
      url = `about:devtools-toolbox?type=${type}&id=${id}&remoteId=${remoteId}`;
    }

    const existingTab = getTabForUrl(url);
    if (existingTab) {
      const navigator = existingTab.ownerGlobal;
      navigator.gBrowser.selectedTab = existingTab;
      navigator.focus();
    } else {
      window.open(url);
    }

    dispatch(
      Actions.recordTelemetryEvent("inspect", {
        target_type: type.toUpperCase(),
        runtime_type: runtime.type,
      })
    );
  };
}

<<<<<<< HEAD
function installTemporaryExtension() {
  const message = l10n.getString("about-debugging-tmp-extension-install-message");
||||||| merged common ancestors
function installTemporaryExtension(message) {
=======
function installTemporaryExtension() {
  const message = l10n.getString(
    "about-debugging-tmp-extension-install-message"
  );
>>>>>>> upstream-releases
  return async (dispatch, getState) => {
    dispatch({ type: TEMPORARY_EXTENSION_INSTALL_START });
    const file = await openTemporaryExtension(window, message);
    try {
      await AddonManager.installTemporaryAddon(file);
      dispatch({ type: TEMPORARY_EXTENSION_INSTALL_SUCCESS });
    } catch (e) {
      dispatch({ type: TEMPORARY_EXTENSION_INSTALL_FAILURE, error: e });
    }
  };
}

function pushServiceWorker(id) {
  return async (_, getState) => {
    const clientWrapper = getCurrentClient(getState().runtimes);

    try {
<<<<<<< HEAD
      await clientWrapper.request({ to: actor, type: "push" });
||||||| merged common ancestors
      await client.request({ to: actor, type: "push" });
=======
      const workerActor = await clientWrapper.getServiceWorkerFront({ id });
      await workerActor.push();
>>>>>>> upstream-releases
    } catch (e) {
      console.error(e);
    }
  };
}

<<<<<<< HEAD
function reloadTemporaryExtension(id) {
  return async (_, getState) => {
    const clientWrapper = getCurrentClient(getState().runtimes);
||||||| merged common ancestors
function reloadTemporaryExtension(actor) {
  return async (_, getState) => {
    const client = getCurrentClient(getState().runtimes);
=======
function reloadTemporaryExtension(id) {
  return async (dispatch, getState) => {
    dispatch({ type: TEMPORARY_EXTENSION_RELOAD_START, id });
    const clientWrapper = getCurrentClient(getState().runtimes);
>>>>>>> upstream-releases

    try {
<<<<<<< HEAD
      const addonTargetFront = await clientWrapper.getAddon({ id });
      await addonTargetFront.reload();
||||||| merged common ancestors
      await client.request({ to: actor, type: "reload" });
=======
      const addonTargetFront = await clientWrapper.getAddon({ id });
      await addonTargetFront.reload();
      dispatch({ type: TEMPORARY_EXTENSION_RELOAD_SUCCESS, id });
>>>>>>> upstream-releases
    } catch (e) {
      const error = typeof e === "string" ? new Error(e) : e;
      dispatch({ type: TEMPORARY_EXTENSION_RELOAD_FAILURE, id, error });
    }
  };
}

function removeTemporaryExtension(id) {
  return async () => {
    try {
      await uninstallAddon(id);
    } catch (e) {
      console.error(e);
    }
  };
}

function requestTabs() {
  return async (dispatch, getState) => {
    dispatch({ type: REQUEST_TABS_START });

<<<<<<< HEAD
    const clientWrapper = getCurrentClient(getState().runtimes);
||||||| merged common ancestors
    const client = getCurrentClient(getState().runtimes);
=======
    const runtime = getCurrentRuntime(getState().runtimes);
    const clientWrapper = getCurrentClient(getState().runtimes);
>>>>>>> upstream-releases

    try {
<<<<<<< HEAD
      const { tabs } = await clientWrapper.listTabs({ favicons: true });
||||||| merged common ancestors
      const { tabs } = await client.listTabs({ favicons: true });
=======
      const isSupported = isSupportedDebugTargetPane(
        runtime.runtimeDetails.info.type,
        DEBUG_TARGET_PANE.TAB
      );
      const tabs = isSupported
        ? await clientWrapper.listTabs({ favicons: true })
        : [];
>>>>>>> upstream-releases

      dispatch({ type: REQUEST_TABS_SUCCESS, tabs });
    } catch (e) {
      dispatch({ type: REQUEST_TABS_FAILURE, error: e });
    }
  };
}

function requestExtensions() {
  return async (dispatch, getState) => {
    dispatch({ type: REQUEST_EXTENSIONS_START });

    const runtime = getCurrentRuntime(getState().runtimes);
    const clientWrapper = getCurrentClient(getState().runtimes);

    try {
<<<<<<< HEAD
      const addons = await clientWrapper.listAddons();
      let extensions = addons.filter(a => a.debuggable);

      // Filter out system addons unless the dedicated preference is set to true.
      if (!getState().ui.showSystemAddons) {
        extensions = extensions.filter(e => !e.isSystem);
      }

      if (runtime.type !== RUNTIMES.THIS_FIREFOX) {
        // manifestURL can only be used when debugging local addons, remove this
        // information for the extension data.
        extensions.forEach(extension => {
          extension.manifestURL = null;
        });
      }

      const installedExtensions = extensions.filter(e => !e.temporarilyInstalled);
      const temporaryExtensions = extensions.filter(e => e.temporarilyInstalled);
||||||| merged common ancestors
      const { addons } = await client.listAddons();
      const extensions = addons.filter(a => a.debuggable);
      const installedExtensions = extensions.filter(e => !e.temporarilyInstalled);
      const temporaryExtensions = extensions.filter(e => e.temporarilyInstalled);
=======
      const isIconDataURLRequired = runtime.type !== RUNTIMES.THIS_FIREFOX;
      const addons = await clientWrapper.listAddons({
        iconDataURL: isIconDataURLRequired,
      });
      let extensions = addons.filter(a => a.debuggable);

      // Filter out hidden & system addons unless the dedicated preference is set to true.
      if (!getState().ui.showHiddenAddons) {
        // System addons should normally also have the hidden flag. However on DevTools
        // side, `hidden` is not available on FF67 servers or older. Check both flags for
        // backward compatibility.
        extensions = extensions.filter(e => !e.isSystem && !e.hidden);
      }

      if (runtime.type !== RUNTIMES.THIS_FIREFOX) {
        // manifestURL can only be used when debugging local addons, remove this
        // information for the extension data.
        extensions.forEach(extension => {
          extension.manifestURL = null;
        });
      }

      const installedExtensions = extensions.filter(
        e => !e.temporarilyInstalled
      );
      const temporaryExtensions = extensions.filter(
        e => e.temporarilyInstalled
      );
>>>>>>> upstream-releases

      dispatch({
        type: REQUEST_EXTENSIONS_SUCCESS,
        installedExtensions,
        temporaryExtensions,
      });
    } catch (e) {
<<<<<<< HEAD
      dispatch({ type: REQUEST_EXTENSIONS_FAILURE, error: e });
||||||| merged common ancestors
      dispatch({ type: REQUEST_EXTENSIONS_FAILURE, error: e.message });
=======
      dispatch({ type: REQUEST_EXTENSIONS_FAILURE, error: e });
    }
  };
}

function requestProcesses() {
  return async (dispatch, getState) => {
    dispatch({ type: REQUEST_PROCESSES_START });

    const clientWrapper = getCurrentClient(getState().runtimes);

    try {
      const mainProcessFront = await clientWrapper.getMainProcess();

      dispatch({
        type: REQUEST_PROCESSES_SUCCESS,
        mainProcess: {
          id: 0,
          processFront: mainProcessFront,
        },
      });
    } catch (e) {
      dispatch({ type: REQUEST_PROCESSES_FAILURE, error: e });
>>>>>>> upstream-releases
    }
  };
}

function requestWorkers() {
  return async (dispatch, getState) => {
    dispatch({ type: REQUEST_WORKERS_START });

    const clientWrapper = getCurrentClient(getState().runtimes);

    try {
      const {
<<<<<<< HEAD
        otherWorkers,
        serviceWorkers,
        sharedWorkers,
      } = await clientWrapper.listWorkers();
||||||| merged common ancestors
        other: otherWorkers,
        service: serviceWorkers,
        shared: sharedWorkers,
      } = await client.mainRoot.listAllWorkers();
=======
        otherWorkers,
        serviceWorkers,
        sharedWorkers,
      } = await clientWrapper.listWorkers();

      for (const serviceWorker of serviceWorkers) {
        const { registrationFront } = serviceWorker;
        if (!registrationFront) {
          continue;
        }

        const subscription = await registrationFront.getPushSubscription();
        serviceWorker.subscription = subscription;
      }
>>>>>>> upstream-releases

      dispatch({
        type: REQUEST_WORKERS_SUCCESS,
        otherWorkers,
        serviceWorkers,
        sharedWorkers,
      });
    } catch (e) {
      dispatch({ type: REQUEST_WORKERS_FAILURE, error: e });
    }
  };
}

function startServiceWorker(registrationFront) {
  return async (_, getState) => {
<<<<<<< HEAD
    const clientWrapper = getCurrentClient(getState().runtimes);
||||||| merged common ancestors
    const client = getCurrentClient(getState().runtimes);
=======
    try {
      await registrationFront.start();
    } catch (e) {
      console.error(e);
    }
  };
}
>>>>>>> upstream-releases

function unregisterServiceWorker(registrationFront) {
  return async (_, getState) => {
    try {
<<<<<<< HEAD
      await clientWrapper.request({ to: actor, type: "start" });
||||||| merged common ancestors
      await client.request({ to: actor, type: "start" });
=======
      await registrationFront.unregister();
>>>>>>> upstream-releases
    } catch (e) {
      console.error(e);
    }
  };
}

module.exports = {
  inspectDebugTarget,
  installTemporaryExtension,
  pushServiceWorker,
  reloadTemporaryExtension,
  removeTemporaryExtension,
  requestTabs,
  requestExtensions,
  requestProcesses,
  requestWorkers,
  startServiceWorker,
  unregisterServiceWorker,
};
