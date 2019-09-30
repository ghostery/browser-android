/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

/**
 * Check target-scoped actor lifetimes.
 */

const ACTORS_URL = EXAMPLE_URL + "testactors.js";
const TAB_URL = TEST_URI_ROOT + "doc_empty-tab-01.html";

add_task(async function() {
  const tab = await addTab(TAB_URL);

  await registerActorInContentProcess(ACTORS_URL, {
    prefix: "testOne",
    constructor: "TestActor1",
    type: { target: true },
  });

<<<<<<< HEAD
  const target = await TargetFactory.forTab(tab);
  await target.attach();
  const { client } = target;
  const targetFront = target.activeTab;
  const form = targetFront.targetForm;
||||||| merged common ancestors
  const transport = DebuggerServer.connectPipe();
  const client = new DebuggerClient(transport);
  const [type] = await client.connect();
  is(type, "browser",
    "Root actor should identify itself as a browser.");
=======
  const target = await TargetFactory.forTab(tab);
  await target.attach();
  const { client } = target;
  const form = target.targetForm;
>>>>>>> upstream-releases

  await testTargetScopedActor(client, form);
  await closeTab(client, form);
  await target.destroy();
});

<<<<<<< HEAD
async function testTargetScopedActor(client, form) {
  ok(form.testOneActor,
    "Found the test target-scoped actor.");
  ok(form.testOneActor.includes("testOne"),
    "testOneActor's actorPrefix should be used.");
||||||| merged common ancestors
async function testTargetScopedActor(client, grip) {
  ok(grip.testOneActor,
    "Found the test target-scoped actor.");
  ok(grip.testOneActor.includes("testOne"),
    "testOneActor's actorPrefix should be used.");
=======
async function testTargetScopedActor(client, form) {
  ok(form.testOneActor, "Found the test target-scoped actor.");
  ok(
    form.testOneActor.includes("testOne"),
    "testOneActor's actorPrefix should be used."
  );
>>>>>>> upstream-releases

<<<<<<< HEAD
  const response = await client.request({ to: form.testOneActor, type: "ping" });
  is(response.pong, "pong",
     "Actor should respond to requests.");
||||||| merged common ancestors
  const response = await client.request({ to: grip.testOneActor, type: "ping" });
  is(response.pong, "pong",
     "Actor should respond to requests.");
=======
  const response = await client.request({
    to: form.testOneActor,
    type: "ping",
  });
  is(response.pong, "pong", "Actor should respond to requests.");
>>>>>>> upstream-releases
}

<<<<<<< HEAD
async function closeTab(client, form) {
||||||| merged common ancestors
async function closeTab(client, grip) {
=======
async function closeTab(client, form) {
  // We need to start listening for the rejection before removing the tab
  /* eslint-disable-next-line mozilla/rejects-requires-await*/
  const onReject = Assert.rejects(
    client.request({ to: form.testOneActor, type: "ping" }),
    err =>
      err.message ===
      `'ping' active request packet to '${form.testOneActor}' ` +
        `can't be sent as the connection just closed.`,
    "testOneActor went away."
  );
>>>>>>> upstream-releases
  await removeTab(gBrowser.selectedTab);
<<<<<<< HEAD
  await Assert.rejects(
    client.request({ to: form.testOneActor, type: "ping" }),
    err => err.message === `'ping' active request packet to '${form.testOneActor}' ` +
                           `can't be sent as the connection just closed.`,
    "testOneActor went away.");
}
||||||| merged common ancestors
  await Assert.rejects(
    client.request({ to: grip.testOneActor, type: "ping" }),
    err => err.message === `'ping' active request packet to '${grip.testOneActor}' ` +
                           `can't be sent as the connection just closed.`,
    "testOneActor went away.");
}

async function attachTargetActorForUrl(client, url) {
  const grip = await getTargetActorForUrl(client, url);
  const [ response ] = await client.attachTarget(grip.actor);
  return [grip, response];
}

function getTargetActorForUrl(client, url) {
  const deferred = promise.defer();

  client.listTabs().then(response => {
    const targetActor = response.tabs.filter(grip => grip.url == url).pop();
    deferred.resolve(targetActor);
  });

  return deferred.promise;
}
=======
  await onReject;
}
>>>>>>> upstream-releases
