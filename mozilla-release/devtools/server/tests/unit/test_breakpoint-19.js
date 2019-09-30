/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

/**
 * Make sure that setting a breakpoint in a not-yet-existing script doesn't throw
 * an error (see bug 897567). Also make sure that this breakpoint works.
 */

const URL = "test.js";

function setUpCode(debuggee) {
  /* eslint-disable */
  Cu.evalInSandbox(
    "" + function test() { // 1
      var a = 1;           // 2
      debugger;            // 3
    } +                    // 4
    "\ndebugger;",         // 5
    debuggee,
    "1.8",
    URL
  );
  /* eslint-enable */
}

<<<<<<< HEAD
add_task(threadClientTest(async ({ threadClient, debuggee, client }) => {
  const source = await getSource(threadClient, URL);
  const [response ] = await setBreakpoint(source, {line: 2});
  ok(!response.error);

  const actor = response.actor;
  ok(actor);

  await executeOnNextTickAndWaitForPause(() => setUpCode(debuggee), client);
  await resume(threadClient);

  const packet = await executeOnNextTickAndWaitForPause(debuggee.test, client);
  equal(packet.why.type, "breakpoint");
  notEqual(packet.why.actors.indexOf(actor), -1);
}));
||||||| merged common ancestors
const testBreakpoint = async function() {
  const source = await getSource(gThreadClient, URL);
  const [response ] = await setBreakpoint(source, {line: 2});
  ok(!response.error);

  const actor = response.actor;
  ok(actor);

  await executeOnNextTickAndWaitForPause(setUpCode, gClient);
  await resume(gThreadClient);

  const packet = await executeOnNextTickAndWaitForPause(gDebuggee.test, gClient);
  equal(packet.why.type, "breakpoint");
  notEqual(packet.why.actors.indexOf(actor), -1);

  finishClient(gClient);
};
=======
add_task(
  threadClientTest(async ({ threadClient, debuggee }) => {
    setBreakpoint(threadClient, { sourceUrl: URL, line: 2 });

    await executeOnNextTickAndWaitForPause(
      () => setUpCode(debuggee),
      threadClient
    );
    await resume(threadClient);

    const packet = await executeOnNextTickAndWaitForPause(
      debuggee.test,
      threadClient
    );
    equal(packet.why.type, "breakpoint");
  })
);
>>>>>>> upstream-releases
