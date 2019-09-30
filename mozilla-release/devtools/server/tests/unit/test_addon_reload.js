/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow */

"use strict";

const { AddonManager } = require("resource://gre/modules/AddonManager.jsm");

startupAddonsManager();

function promiseAddonEvent(event) {
  return new Promise(resolve => {
    const listener = {
      [event]: function(...args) {
        AddonManager.removeAddonListener(listener);
        resolve(args);
      },
    };

    AddonManager.addAddonListener(listener);
  });
}

function promiseWebExtensionStartup() {
  const { Management } = ChromeUtils.import(
    "resource://gre/modules/Extension.jsm",
    null
  );

  return new Promise(resolve => {
    const listener = (evt, extension) => {
      Management.off("ready", listener);
      resolve(extension);
    };

    Management.on("ready", listener);
  });
}

<<<<<<< HEAD
async function reloadAddon(addonTargetFront) {
||||||| merged common ancestors
async function findAddonInRootList(client, addonId) {
  const result = await client.listAddons();
  const addonTargetActor = result.addons.filter(addon => addon.id === addonId)[0];
  ok(addonTargetActor, `Found add-on actor for ${addonId}`);
  return addonTargetActor;
}

async function reloadAddon(client, addonTargetActor) {
=======
async function reloadAddon(addonFront) {
>>>>>>> upstream-releases
  // The add-on will be re-installed after a successful reload.
  const onInstalled = promiseAddonEvent("onInstalled");
<<<<<<< HEAD
  await addonTargetFront.reload();
||||||| merged common ancestors
  await client.request({to: addonTargetActor.actor, type: "reload"});
=======
  await addonFront.reload();
>>>>>>> upstream-releases
  await onInstalled;
}

function getSupportFile(path) {
  const allowMissing = false;
  return do_get_file(path, allowMissing);
}

add_task(async function testReloadExitedAddon() {
  DebuggerServer.init();
  DebuggerServer.registerAllActors();

  const client = new DebuggerClient(DebuggerServer.connectPipe());
  await client.connect();

  // Install our main add-on to trigger reloads on.
  const addonFile = getSupportFile("addons/web-extension");
  const [installedAddon] = await Promise.all([
    AddonManager.installTemporaryAddon(addonFile),
    promiseWebExtensionStartup(),
  ]);

  // Install a decoy add-on.
  const addonFile2 = getSupportFile("addons/web-extension2");
  const [installedAddon2] = await Promise.all([
    AddonManager.installTemporaryAddon(addonFile2),
    promiseWebExtensionStartup(),
  ]);

<<<<<<< HEAD
  const addonTargetFront = await client.mainRoot.getAddon({ id: installedAddon.id });
||||||| merged common ancestors
  const addonTargetActor = await findAddonInRootList(client, installedAddon.id);
=======
  const addonFront = await client.mainRoot.getAddon({ id: installedAddon.id });
>>>>>>> upstream-releases

<<<<<<< HEAD
  await Promise.all([
    reloadAddon(addonTargetFront),
    promiseWebExtensionStartup(),
  ]);
||||||| merged common ancestors
  await Promise.all([
    reloadAddon(client, addonTargetActor),
    promiseWebExtensionStartup(),
  ]);
=======
  await Promise.all([reloadAddon(addonFront), promiseWebExtensionStartup()]);
>>>>>>> upstream-releases

  // Uninstall the decoy add-on, which should cause its actor to exit.
  const onUninstalled = promiseAddonEvent("onUninstalled");
  installedAddon2.uninstall();
  await onUninstalled;

  // Try to re-list all add-ons after a reload.
  // This was throwing an exception because of the exited actor.
<<<<<<< HEAD
  const newAddonFront = await client.mainRoot.getAddon({ id: installedAddon.id });
  equal(newAddonFront.id, addonTargetFront.id);
||||||| merged common ancestors
  const newAddonActor = await findAddonInRootList(client, installedAddon.id);
  equal(newAddonActor.id, addonTargetActor.id);
=======
  const newAddonFront = await client.mainRoot.getAddon({
    id: installedAddon.id,
  });
  equal(newAddonFront.id, addonFront.id);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The fronts should be the same after the reload
  equal(newAddonFront, addonTargetFront);
||||||| merged common ancestors
  // The actor id should be the same after the reload
  equal(newAddonActor.actor, addonTargetActor.actor);
=======
  // The fronts should be the same after the reload
  equal(newAddonFront, addonFront);
>>>>>>> upstream-releases

  const onAddonListChanged = client.mainRoot.once("addonListChanged");

  // Install an upgrade version of the first add-on.
  const addonUpgradeFile = getSupportFile("addons/web-extension-upgrade");
  const [upgradedAddon] = await Promise.all([
    AddonManager.installTemporaryAddon(addonUpgradeFile),
    promiseWebExtensionStartup(),
  ]);

  // Waiting for addonListChanged unsolicited event
  await onAddonListChanged;

  // re-list all add-ons after an upgrade.
<<<<<<< HEAD
  const upgradedAddonFront = await client.mainRoot.getAddon({ id: upgradedAddon.id });
  equal(upgradedAddonFront.id, addonTargetFront.id);
  // The fronts should be the same after the upgrade.
  equal(upgradedAddonFront, addonTargetFront);
||||||| merged common ancestors
  const upgradedAddonActor = await findAddonInRootList(client, upgradedAddon.id);
  equal(upgradedAddonActor.id, addonTargetActor.id);
  // The actor id should be the same after the upgrade.
  equal(upgradedAddonActor.actor, addonTargetActor.actor);
=======
  const upgradedAddonFront = await client.mainRoot.getAddon({
    id: upgradedAddon.id,
  });
  equal(upgradedAddonFront.id, addonFront.id);
  // The fronts should be the same after the upgrade.
  equal(upgradedAddonFront, addonFront);
>>>>>>> upstream-releases

  // The addon metadata has been updated.
  equal(upgradedAddonFront.name, "Test Addons Actor Upgrade");

  await close(client);
});
