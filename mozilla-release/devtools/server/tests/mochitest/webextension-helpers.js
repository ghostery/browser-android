/* exported attachAddon, setWebExtensionOOPMode, waitForFramesUpdated, reloadAddon,
   collectFrameUpdates, generateWebExtensionXPI, promiseInstallFile,
   promiseWebExtensionStartup, promiseWebExtensionShutdown
 */

"use strict";

const { require, loader } = ChromeUtils.import(
  "resource://devtools/shared/Loader.jsm"
);
const { DebuggerClient } = require("devtools/shared/client/debugger-client");
const { DebuggerServer } = require("devtools/server/main");

const {
  AddonTestUtils,
} = require("resource://testing-common/AddonTestUtils.jsm");
const {
  ExtensionTestCommon,
} = require("resource://testing-common/ExtensionTestCommon.jsm");

loader.lazyImporter(
  this,
  "ExtensionParent",
  "resource://gre/modules/ExtensionParent.jsm"
);

// Initialize a minimal DebuggerServer and connect to the webextension addon actor.
if (!DebuggerServer.initialized) {
  DebuggerServer.init();
  DebuggerServer.registerAllActors();
  SimpleTest.registerCleanupFunction(function() {
    DebuggerServer.destroy();
  });
}

SimpleTest.registerCleanupFunction(function() {
  const { hiddenXULWindow } = ExtensionParent.DebugUtils;
  const debugBrowserMapSize =
    ExtensionParent.DebugUtils.debugBrowserPromises.size;

  if (debugBrowserMapSize > 0) {
    is(
      debugBrowserMapSize,
      0,
      "ExtensionParent DebugUtils debug browsers have not been released"
    );
  }

  if (hiddenXULWindow) {
    ok(
      false,
      "ExtensionParent DebugUtils hiddenXULWindow has not been destroyed"
    );
  }
});

// Test helpers related to the webextensions debugging RDP actors.

function setWebExtensionOOPMode(oopMode) {
  return SpecialPowers.pushPrefEnv({
    set: [["extensions.webextensions.remote", oopMode]],
  });
}

function waitForFramesUpdated(target, matchFn) {
  return new Promise(resolve => {
    const listener = data => {
      if (typeof matchFn === "function" && !matchFn(data)) {
        return;
      } else if (!data.frames) {
        return;
      }

      target.off("frameUpdate", listener);
      resolve(data.frames);
    };
    target.on("frameUpdate", listener);
  });
}

function collectFrameUpdates({ client }, matchFn) {
  const collected = [];

  const listener = data => {
    if (matchFn(data)) {
      collected.push(data);
    }
  };

  client.on("frameUpdate", listener);
  let unsubscribe = () => {
    unsubscribe = null;
    client.off("frameUpdate", listener);
    return collected;
  };

  SimpleTest.registerCleanupFunction(function() {
    if (unsubscribe) {
      unsubscribe();
    }
  });

  return unsubscribe;
}

async function attachAddon(addonId) {
  const transport = DebuggerServer.connectPipe();
  const client = new DebuggerClient(transport);

  await client.connect();

<<<<<<< HEAD
  const addonTargetFront = await client.mainRoot.getAddon({ id: addonId });
||||||| merged common ancestors
  const {addons} = await client.mainRoot.listAddons();
  const addonTargetActor = addons.filter(actor => actor.id === addonId).pop();
=======
  const addonFront = await client.mainRoot.getAddon({ id: addonId });
  const addonTarget = await addonFront.connect();
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (!addonTargetFront) {
||||||| merged common ancestors
  if (!addonTargetActor) {
=======
  if (!addonTarget) {
>>>>>>> upstream-releases
    client.close();
    throw new Error(`No WebExtension Actor found for ${addonId}`);
  }

<<<<<<< HEAD
  const addonTarget = await TargetFactory.forRemoteTab({
    activeTab: addonTargetFront,
    client,
    chrome: true,
  });

||||||| merged common ancestors
  const addonTarget = await TargetFactory.forRemoteTab({
    form: addonTargetActor,
    client,
    chrome: true,
  });

=======
>>>>>>> upstream-releases
  return addonTarget;
}

<<<<<<< HEAD
async function reloadAddon({client}, addonId) {
  const addonTargetFront = await client.mainRoot.getAddon({ id: addonId });
||||||| merged common ancestors
async function reloadAddon({client}, addonId) {
  const {addons} = await client.mainRoot.listAddons();
  const addonTargetActor = addons.filter(actor => actor.id === addonId).pop();
=======
async function reloadAddon({ client }, addonId) {
  const addonTargetFront = await client.mainRoot.getAddon({ id: addonId });
>>>>>>> upstream-releases

  if (!addonTargetFront) {
    client.close();
    throw new Error(`No WebExtension Actor found for ${addonId}`);
  }

  await addonTargetFront.reload();
}

// Test helpers related to the AddonManager.

function generateWebExtensionXPI(extDetails) {
  return ExtensionTestCommon.generateXPI(extDetails);
}

let {
  promiseInstallFile,
  promiseWebExtensionStartup,
  promiseWebExtensionShutdown,
} = AddonTestUtils;
