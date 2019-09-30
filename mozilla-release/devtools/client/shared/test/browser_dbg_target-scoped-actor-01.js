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

add_task(async function test() {
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
  const [ type ] = await client.connect();
  is(type, "browser", "Root actor should identify itself as a browser.");
=======
  const target = await TargetFactory.forTab(tab);
  await target.attach();
  const { client } = target;
  const form = target.targetForm;
>>>>>>> upstream-releases

  await testTargetScopedActor(client, form);
  await removeTab(gBrowser.selectedTab);
  await target.destroy();
});

<<<<<<< HEAD
async function testTargetScopedActor(client, form) {
  ok(form.testOneActor,
    "Found the test target-scoped actor.");
  ok(form.testOneActor.includes("testOne"),
    "testOneActor's actorPrefix should be used.");

  const response = await client.request({ to: form.testOneActor, type: "ping" });
  is(response.pong, "pong", "Actor should respond to requests.");
}
||||||| merged common ancestors
async function testTargetScopedActor(client, grip) {
  ok(grip.testOneActor,
    "Found the test target-scoped actor.");
  ok(grip.testOneActor.includes("testOne"),
    "testOneActor's actorPrefix should be used.");

  const response = await client.request({ to: grip.testOneActor, type: "ping" });
  is(response.pong, "pong", "Actor should respond to requests.");
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
async function testTargetScopedActor(client, form) {
  ok(form.testOneActor, "Found the test target-scoped actor.");
  ok(
    form.testOneActor.includes("testOne"),
    "testOneActor's actorPrefix should be used."
  );

  const response = await client.request({
    to: form.testOneActor,
    type: "ping",
  });
  is(response.pong, "pong", "Actor should respond to requests.");
}
>>>>>>> upstream-releases
