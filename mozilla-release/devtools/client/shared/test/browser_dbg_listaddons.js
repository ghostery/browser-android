/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

<<<<<<< HEAD
/* import-globals-from helper_addons.js */
Services.scriptloader.loadSubScript(
  "chrome://mochitests/content/browser/devtools/client/shared/test/helper_addons.js",
  this);

||||||| merged common ancestors
=======
/* import-globals-from helper_addons.js */
Services.scriptloader.loadSubScript(
  "chrome://mochitests/content/browser/devtools/client/shared/test/helper_addons.js",
  this
);

>>>>>>> upstream-releases
var { DebuggerServer } = require("devtools/server/main");
var { DebuggerClient } = require("devtools/shared/client/debugger-client");

/**
 * Make sure the listAddons request works as specified.
 */
<<<<<<< HEAD
const ADDON1_ID = "test-addon-1@mozilla.org";
const ADDON1_PATH = "addons/test-addon-1/";
const ADDON2_ID = "test-addon-2@mozilla.org";
const ADDON2_PATH = "addons/test-addon-2/";

add_task(async function() {
||||||| merged common ancestors
const ADDON1_ID = "jid1-oBAwBoE5rSecNg@jetpack";
const ADDON1_PATH = "addon1.xpi";
const ADDON2_ID = "jid1-qjtzNGV8xw5h2A@jetpack";
const ADDON2_PATH = "addon2.xpi";

var gAddon1, gAddon1Actor, gAddon2, gClient;

function test() {
=======
const ADDON1_ID = "test-addon-1@mozilla.org";
const ADDON1_PATH = "addons/test-addon-1/";
const ADDON2_ID = "test-addon-2@mozilla.org";
const ADDON2_PATH = "addons/test-addon-2/";

add_task(async function() {
>>>>>>> upstream-releases
  DebuggerServer.init();
  DebuggerServer.registerAllActors();

  const transport = DebuggerServer.connectPipe();
<<<<<<< HEAD
  const client = new DebuggerClient(transport);

  const [type] = await client.connect();
  is(type, "browser", "Root actor should identify itself as a browser.");
||||||| merged common ancestors
  gClient = new DebuggerClient(transport);
  gClient.connect().then(([aType, aTraits]) => {
    is(aType, "browser",
      "Root actor should identify itself as a browser.");

    promise.resolve(null)
      .then(testFirstAddon)
      .then(testSecondAddon)
      .then(testRemoveFirstAddon)
      .then(testRemoveSecondAddon)
      .then(() => gClient.close())
      .then(finish)
      .catch(error => {
        ok(false, "Got an error: " + error.message + "\n" + error.stack);
      });
  });
}

function testFirstAddon() {
  let addonListChanged = false;
  gClient.addOneTimeListener("addonListChanged", () => {
    addonListChanged = true;
  });
=======
  const client = new DebuggerClient(transport);
>>>>>>> upstream-releases

<<<<<<< HEAD
  let addonListChangedEvents = 0;
  client.mainRoot.on("addonListChanged", () => addonListChangedEvents++);
||||||| merged common ancestors
  return addTemporaryAddon(ADDON1_PATH).then(addon => {
    gAddon1 = addon;
=======
  const [type] = await client.connect();
  is(type, "browser", "Root actor should identify itself as a browser.");
>>>>>>> upstream-releases

<<<<<<< HEAD
  const addon1 = await addTemporaryAddon(ADDON1_PATH);
  const addonFront1 = await client.mainRoot.getAddon({ id: ADDON1_ID });
  is(addonListChangedEvents, 0, "Should not receive addonListChanged yet.");
  ok(addonFront1, "Should find an addon actor for addon1.");

  const addon2 = await addTemporaryAddon(ADDON2_PATH);
  const front1AfterAddingAddon2 = await client.mainRoot.getAddon({ id: ADDON1_ID });
  const addonFront2 = await client.mainRoot.getAddon({ id: ADDON2_ID });

  is(addonListChangedEvents, 1, "Should have received an addonListChanged event.");
  ok(addonFront2, "Should find an addon actor for addon2.");
  is(addonFront1, front1AfterAddingAddon2, "Front for addon1 should be the same");
||||||| merged common ancestors
    return getAddonActorForId(gClient, ADDON1_ID).then(grip => {
      ok(!addonListChanged, "Should not yet be notified that list of addons changed.");
      ok(grip, "Should find an addon actor for addon1.");
      gAddon1Actor = grip.actor;
    });
  });
}

function testSecondAddon() {
  let addonListChanged = false;
  gClient.addOneTimeListener("addonListChanged", function() {
    addonListChanged = true;
  });

  return addTemporaryAddon(ADDON2_PATH).then(addon => {
    gAddon2 = addon;
=======
  let addonListChangedEvents = 0;
  client.mainRoot.on("addonListChanged", () => addonListChangedEvents++);
>>>>>>> upstream-releases

<<<<<<< HEAD
  await removeAddon(addon1);
  const front1AfterRemove = await client.mainRoot.getAddon({ id: ADDON1_ID });
  is(addonListChangedEvents, 2, "Should have received an addonListChanged event.");
  ok(!front1AfterRemove, "Should no longer get a front for addon1");

  await removeAddon(addon2);
  const front2AfterRemove = await client.mainRoot.getAddon({ id: ADDON2_ID });
  is(addonListChangedEvents, 3, "Should have received an addonListChanged event.");
  ok(!front2AfterRemove, "Should no longer get a front for addon1");
||||||| merged common ancestors
    return getAddonActorForId(gClient, ADDON1_ID).then(fistGrip => {
      return getAddonActorForId(gClient, ADDON2_ID).then(secondGrip => {
        ok(addonListChanged, "Should be notified that list of addons changed.");
        is(fistGrip.actor, gAddon1Actor, "First addon's actor shouldn't have changed.");
        ok(secondGrip, "Should find a addon actor for the second addon.");
      });
    });
  });
}

function testRemoveFirstAddon() {
  let addonListChanged = false;
  gClient.addOneTimeListener("addonListChanged", function() {
    addonListChanged = true;
  });
=======
  const addon1 = await addTemporaryAddon(ADDON1_PATH);
  const addonFront1 = await client.mainRoot.getAddon({ id: ADDON1_ID });
  is(addonListChangedEvents, 0, "Should not receive addonListChanged yet.");
  ok(addonFront1, "Should find an addon actor for addon1.");
>>>>>>> upstream-releases

<<<<<<< HEAD
  await client.close();
||||||| merged common ancestors
  return removeAddon(gAddon1).then(() => {
    return getAddonActorForId(gClient, ADDON1_ID).then(grip => {
      ok(addonListChanged, "Should be notified that list of addons changed.");
      ok(!grip, "Shouldn't find a addon actor for the first addon anymore.");
    });
  });
}

function testRemoveSecondAddon() {
  let addonListChanged = false;
  gClient.addOneTimeListener("addonListChanged", function() {
    addonListChanged = true;
  });

  return removeAddon(gAddon2).then(() => {
    return getAddonActorForId(gClient, ADDON2_ID).then(grip => {
      ok(addonListChanged, "Should be notified that list of addons changed.");
      ok(!grip, "Shouldn't find a addon actor for the second addon anymore.");
    });
  });
}

registerCleanupFunction(function() {
  gAddon1 = null;
  gAddon1Actor = null;
  gAddon2 = null;
  gClient = null;
=======
  const addon2 = await addTemporaryAddon(ADDON2_PATH);
  const front1AfterAddingAddon2 = await client.mainRoot.getAddon({
    id: ADDON1_ID,
  });
  const addonFront2 = await client.mainRoot.getAddon({ id: ADDON2_ID });

  is(
    addonListChangedEvents,
    1,
    "Should have received an addonListChanged event."
  );
  ok(addonFront2, "Should find an addon actor for addon2.");
  is(
    addonFront1,
    front1AfterAddingAddon2,
    "Front for addon1 should be the same"
  );

  await removeAddon(addon1);
  const front1AfterRemove = await client.mainRoot.getAddon({ id: ADDON1_ID });
  is(
    addonListChangedEvents,
    2,
    "Should have received an addonListChanged event."
  );
  ok(!front1AfterRemove, "Should no longer get a front for addon1");

  await removeAddon(addon2);
  const front2AfterRemove = await client.mainRoot.getAddon({ id: ADDON2_ID });
  is(
    addonListChangedEvents,
    3,
    "Should have received an addonListChanged event."
  );
  ok(!front2AfterRemove, "Should no longer get a front for addon1");

  await client.close();
>>>>>>> upstream-releases
});
