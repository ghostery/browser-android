/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow */

"use strict";

/**
 * Test that setting ignoreCaughtExceptions will cause the debugger to ignore
 * caught exceptions, but not uncaught ones.
 */

<<<<<<< HEAD
add_task(threadClientTest(async ({ threadClient, client, debuggee }) => {
  await executeOnNextTickAndWaitForPause(() => evaluateTestCode(debuggee), client);

  threadClient.pauseOnExceptions(true, true);
  await resume(threadClient);
  const paused = await waitForPause(client);
  Assert.equal(paused.why.type, "exception");
  equal(paused.frame.where.line, 6, "paused at throw");

  await resume(threadClient);
}, {
  // Bug 1508289, exception tests fails in worker scope
  doNotRunWorker: true,
}));

function evaluateTestCode(debuggee) {
||||||| merged common ancestors
var gDebuggee;
var gClient;

function run_test() {
  do_test_pending();
  run_test_with_server(DebuggerServer, function() {
    run_test_with_server(WorkerDebuggerServer, do_test_finished);
  });
}

function run_test_with_server(server, callback) {
  initTestDebuggerServer(server);
  gDebuggee = addTestGlobal("test-pausing", server);
  gClient = new DebuggerClient(server.connectPipe());
  gClient.connect(test_pause_frame);
}

async function test_pause_frame() {
  const [,, threadClient] = await attachTestTabAndResume(gClient, "test-pausing");
  await executeOnNextTickAndWaitForPause(evaluateTestCode, gClient);

  evaluateTestCode();

  threadClient.pauseOnExceptions(true, true);
  await resume(threadClient);
  const paused = await waitForPause(gClient);
  Assert.equal(paused.why.type, "exception");
  equal(paused.frame.where.line, 6, "paused at throw");

  await resume(threadClient);
  finishClient(gClient);
}

function evaluateTestCode() {
=======
add_task(
  threadClientTest(
    async ({ threadClient, debuggee }) => {
      await executeOnNextTickAndWaitForPause(
        () => evaluateTestCode(debuggee),
        threadClient
      );

      threadClient.pauseOnExceptions(true, true);
      await resume(threadClient);
      const paused = await waitForPause(threadClient);
      Assert.equal(paused.why.type, "exception");
      equal(paused.frame.where.line, 6, "paused at throw");

      await resume(threadClient);
    },
    {
      // Bug 1508289, exception tests fails in worker scope
      doNotRunWorker: true,
    }
  )
);

function evaluateTestCode(debuggee) {
>>>>>>> upstream-releases
  /* eslint-disable */
  try {
  Cu.evalInSandbox(`                    // 1
   debugger;                            // 2
   try {                                // 3
     throw "foo";                       // 4
   } catch (e) {}                       // 5
   throw "bar";                         // 6
  `,                                    // 7
    debuggee,
    "1.8",
    "test_pause_exceptions-03.js",
    1
  );
  } catch (e) {}
  /* eslint-disable */
}
