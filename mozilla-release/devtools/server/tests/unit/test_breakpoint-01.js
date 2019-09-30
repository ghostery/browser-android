/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow, max-nested-callbacks */

"use strict";

/**
 * Check basic breakpoint functionality.
 */

<<<<<<< HEAD
add_task(threadClientTest(({ threadClient, debuggee }) => {
  return new Promise(resolve => {
    threadClient.addOneTimeListener("paused", function(event, packet) {
      const source = threadClient.source(packet.frame.where.source);
      const location = {
        line: debuggee.line0 + 3,
      };

      source.setBreakpoint(location).then(function([response, bpClient]) {
        threadClient.addOneTimeListener("paused", function(event, packet) {
          // Check the return value.
          Assert.equal(packet.type, "paused");
          Assert.equal(packet.frame.where.source.actor, source.actor);
          Assert.equal(packet.frame.where.line, location.line);
          Assert.equal(packet.why.type, "breakpoint");
          Assert.equal(packet.why.actors[0], bpClient.actor);
          // Check that the breakpoint worked.
          Assert.equal(debuggee.a, 1);
          Assert.equal(debuggee.b, undefined);

          // Remove the breakpoint.
          bpClient.remove(function(response) {
            threadClient.resume(resolve);
          });
        });

        // Continue until the breakpoint is hit.
        threadClient.resume();
      });
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
                             test_simple_breakpoint();
                           });
  });
}

function test_simple_breakpoint() {
  gThreadClient.addOneTimeListener("paused", function(event, packet) {
    const source = gThreadClient.source(packet.frame.where.source);
    const location = {
      line: gDebuggee.line0 + 3,
    };

    source.setBreakpoint(location).then(function([response, bpClient]) {
      gThreadClient.addOneTimeListener("paused", function(event, packet) {
        // Check the return value.
        Assert.equal(packet.type, "paused");
        Assert.equal(packet.frame.where.source.actor, source.actor);
        Assert.equal(packet.frame.where.line, location.line);
        Assert.equal(packet.why.type, "breakpoint");
        Assert.equal(packet.why.actors[0], bpClient.actor);
        // Check that the breakpoint worked.
        Assert.equal(gDebuggee.a, 1);
        Assert.equal(gDebuggee.b, undefined);

        // Remove the breakpoint.
        bpClient.remove(function(response) {
          gThreadClient.resume(function() {
            gClient.close().then(gCallback);
          });
        });
      });

      // Continue until the breakpoint is hit.
      gThreadClient.resume();
=======
add_task(
  threadClientTest(async ({ threadClient, debuggee }) => {
    (async () => {
      info("Wait for the debugger statement to be hit");
      let packet = await waitForPause(threadClient);
      const source = await getSourceById(
        threadClient,
        packet.frame.where.actor
      );

      const location = { sourceUrl: source.url, line: debuggee.line0 + 3 };

      threadClient.setBreakpoint(location, {});

      await threadClient.resume();
      packet = await waitForPause(threadClient);

      info("Paused at the breakpoint");
      Assert.equal(packet.type, "paused");
      Assert.equal(packet.frame.where.actor, source.actor);
      Assert.equal(packet.frame.where.line, location.line);
      Assert.equal(packet.why.type, "breakpoint");

      info("Check that the breakpoint worked.");
      Assert.equal(debuggee.a, 1);
      Assert.equal(debuggee.b, undefined);

      await threadClient.resume();
    })();

    /*
     * Be sure to run debuggee code in its own HTML 'task', so that when we call
     * the onDebuggerStatement hook, the test's own microtasks don't get suspended
     * along with the debuggee's.
     */
    do_timeout(0, () => {
    /* eslint-disable */
    Cu.evalInSandbox(
      "var line0 = Error().lineNumber;\n" +
        "debugger;\n" +   // line0 + 1
        "var a = 1;\n" +  // line0 + 2
        "var b = 2;\n",   // line0 + 3
        debuggee
    );
    /* eslint-enable */
>>>>>>> upstream-releases
    });
<<<<<<< HEAD

    /* eslint-disable */
    Cu.evalInSandbox(
      "var line0 = Error().lineNumber;\n" +
      "debugger;\n" +   // line0 + 1
      "var a = 1;\n" +  // line0 + 2
      "var b = 2;\n",   // line0 + 3
      debuggee
    );
    /* eslint-enable */
  });
}));
||||||| merged common ancestors
  });

  /* eslint-disable */
  Cu.evalInSandbox(
    "var line0 = Error().lineNumber;\n" +
    "debugger;\n" +   // line0 + 1
    "var a = 1;\n" +  // line0 + 2
    "var b = 2;\n",   // line0 + 3
    gDebuggee
  );
  /* eslint-enable */
}
=======
  })
);
>>>>>>> upstream-releases
