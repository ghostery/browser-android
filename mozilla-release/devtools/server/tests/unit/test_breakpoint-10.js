/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow, max-nested-callbacks */

"use strict";

/**
 * Check that setting a breakpoint in a line with multiple entry points
 * triggers no matter which entry point we reach.
 */

<<<<<<< HEAD
add_task(threadClientTest(({ threadClient, debuggee }) => {
  return new Promise(resolve => {
    threadClient.addOneTimeListener("paused", function(event, packet) {
      const source = threadClient.source(packet.frame.where.source);
      const location = { line: debuggee.line0 + 3 };
||||||| merged common ancestors
var gDebuggee;
var gClient;
var gThreadClient;
var gCallback;
=======
add_task(
  threadClientTest(({ threadClient, client, debuggee }) => {
    return new Promise(resolve => {
      threadClient.once("paused", async function(packet) {
        const source = await getSourceById(
          threadClient,
          packet.frame.where.actor
        );
        const location = {
          sourceUrl: source.url,
          line: debuggee.line0 + 3,
          column: 5,
        };
>>>>>>> upstream-releases

<<<<<<< HEAD
      source.setBreakpoint(location).then(function([response, bpClient]) {
        // actualLocation is not returned when breakpoints don't skip forward.
        Assert.equal(response.actualLocation, undefined);

        threadClient.addOneTimeListener("paused", function(event, packet) {
||||||| merged common ancestors
function run_test() {
  run_test_with_server(DebuggerServer, function() {
    run_test_with_server(WorkerDebuggerServer, do_test_finished);
  });
  do_test_pending();
}

function run_test_with_server(server, callback) {
  gCallback = callback;
  initTestDebuggerServer(server);
  gDebuggee = addTestGlobal("test-stack", server);
  gClient = new DebuggerClient(server.connectPipe());
  gClient.connect().then(function() {
    attachTestTabAndResume(gClient, "test-stack",
                           function(response, targetFront, threadClient) {
                             gThreadClient = threadClient;
                             test_child_breakpoint();
                           });
  });
}

function test_child_breakpoint() {
  gThreadClient.addOneTimeListener("paused", function(event, packet) {
    const source = gThreadClient.source(packet.frame.where.source);
    const location = { line: gDebuggee.line0 + 3 };

    source.setBreakpoint(location).then(function([response, bpClient]) {
      // actualLocation is not returned when breakpoints don't skip forward.
      Assert.equal(response.actualLocation, undefined);

      gThreadClient.addOneTimeListener("paused", function(event, packet) {
        // Check the return value.
        Assert.equal(packet.type, "paused");
        Assert.equal(packet.why.type, "breakpoint");
        Assert.equal(packet.why.actors[0], bpClient.actor);
        // Check that the breakpoint worked.
        Assert.equal(gDebuggee.i, 0);

        gThreadClient.addOneTimeListener("paused", function(event, packet) {
=======
        threadClient.setBreakpoint(location, {});
        await client.waitForRequestsToSettle();

        threadClient.once("paused", async function(packet) {
>>>>>>> upstream-releases
          // Check the return value.
          Assert.equal(packet.why.type, "breakpoint");
          // Check that the breakpoint worked.
<<<<<<< HEAD
          Assert.equal(debuggee.i, 0);

          threadClient.addOneTimeListener("paused", function(event, packet) {
            // Check the return value.
            Assert.equal(packet.type, "paused");
            Assert.equal(packet.why.type, "breakpoint");
            Assert.equal(packet.why.actors[0], bpClient.actor);
            // Check that the breakpoint worked.
            Assert.equal(debuggee.i, 1);

            // Remove the breakpoint.
            bpClient.remove(function(response) {
              threadClient.resume(resolve);
            });
||||||| merged common ancestors
          Assert.equal(gDebuggee.i, 1);

          // Remove the breakpoint.
          bpClient.remove(function(response) {
            gThreadClient.resume(function() {
              gClient.close().then(gCallback);
            });
=======
          Assert.equal(debuggee.i, 0);

          // Remove the breakpoint.
          threadClient.removeBreakpoint(location);
          await client.waitForRequestsToSettle();

          const location2 = {
            sourceUrl: source.url,
            line: debuggee.line0 + 3,
            column: 12,
          };
          threadClient.setBreakpoint(location2, {});
          await client.waitForRequestsToSettle();

          threadClient.once("paused", async function(packet) {
            // Check the return value.
            Assert.equal(packet.why.type, "breakpoint");
            // Check that the breakpoint worked.
            Assert.equal(debuggee.i, 1);

            // Remove the breakpoint.
            threadClient.removeBreakpoint(location2);
            await client.waitForRequestsToSettle();

            threadClient.resume().then(resolve);
>>>>>>> upstream-releases
          });
<<<<<<< HEAD
||||||| merged common ancestors
        });
=======

          // Continue until the breakpoint is hit again.
          await threadClient.resume();
        });
>>>>>>> upstream-releases

<<<<<<< HEAD
          // Continue until the breakpoint is hit again.
          threadClient.resume();
        });
        // Continue until the breakpoint is hit.
        threadClient.resume();
||||||| merged common ancestors
        // Continue until the breakpoint is hit again.
        gThreadClient.resume();
=======
        // Continue until the breakpoint is hit.
        await threadClient.resume();
>>>>>>> upstream-releases
      });
<<<<<<< HEAD
    });
||||||| merged common ancestors
      // Continue until the breakpoint is hit.
      gThreadClient.resume();
    });
  });
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
    /* eslint-disable */
    Cu.evalInSandbox("var line0 = Error().lineNumber;\n" +
                     "debugger;\n" +                      // line0 + 1
                     "var a, i = 0;\n" +                  // line0 + 2
                     "for (i = 1; i <= 2; i++) {\n" +     // line0 + 3
                     "  a = i;\n" +                       // line0 + 4
                     "}\n",                               // line0 + 5
                     debuggee);
    /* eslint-enable */
  });
}));
||||||| merged common ancestors
  /* eslint-disable */
  Cu.evalInSandbox("var line0 = Error().lineNumber;\n" +
                   "debugger;\n" +                      // line0 + 1
                   "var a, i = 0;\n" +                  // line0 + 2
                   "for (i = 1; i <= 2; i++) {\n" +     // line0 + 3
                   "  a = i;\n" +                       // line0 + 4
                   "}\n",                               // line0 + 5
                   gDebuggee);
  /* eslint-enable */
}
=======
      /* eslint-disable */
    Cu.evalInSandbox("var line0 = Error().lineNumber;\n" +
                     "debugger;\n" +                      // line0 + 1
                     "var a, i = 0;\n" +                  // line0 + 2
                     "for (i = 1; i <= 2; i++) {\n" +     // line0 + 3
                     "  a = i;\n" +                       // line0 + 4
                     "}\n",                               // line0 + 5
                     debuggee);
    /* eslint-enable */
    });
  })
);
>>>>>>> upstream-releases
