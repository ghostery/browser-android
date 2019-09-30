/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow */

"use strict";

/**
 * Check that setting breakpoints when the debuggee is running works.
 */

<<<<<<< HEAD
add_task(threadClientTest(({ threadClient, debuggee }) => {
  return new Promise(resolve => {
    threadClient.addOneTimeListener("paused", function(event, packet) {
      const location = { line: debuggee.line0 + 3 };

      threadClient.resume();

      // Setting the breakpoint later should interrupt the debuggee.
      threadClient.addOneTimeListener("paused", function(event, packet) {
        Assert.equal(packet.type, "paused");
        Assert.equal(packet.why.type, "interrupted");
||||||| merged common ancestors
var gDebuggee;
var gClient;
var gThreadClient;
var gCallback;

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
                             test_breakpoint_running();
                           });
  });
}

function test_breakpoint_running() {
  gThreadClient.addOneTimeListener("paused", function(event, packet) {
    const location = { line: gDebuggee.line0 + 3 };

    gThreadClient.resume();

    // Setting the breakpoint later should interrupt the debuggee.
    gThreadClient.addOneTimeListener("paused", function(event, packet) {
      Assert.equal(packet.type, "paused");
      Assert.equal(packet.why.type, "interrupted");
    });

    const source = gThreadClient.source(packet.frame.where.source);
    source.setBreakpoint(location).then(function() {
      executeSoon(function() {
        gClient.close().then(gCallback);
=======
add_task(
  threadClientTest(({ threadClient, client, debuggee }) => {
    return new Promise(resolve => {
      threadClient.once("paused", async function(packet) {
        const source = await getSourceById(
          threadClient,
          packet.frame.where.actor
        );
        const location = { sourceUrl: source.url, line: debuggee.line0 + 3 };

        await threadClient.resume();

        // Setting the breakpoint later should interrupt the debuggee.
        threadClient.once("paused", function(packet) {
          Assert.equal(packet.type, "paused");
          Assert.equal(packet.why.type, "interrupted");
        });

        threadClient.setBreakpoint(location, {});
        await client.waitForRequestsToSettle();
        executeSoon(resolve);
>>>>>>> upstream-releases
      });

<<<<<<< HEAD
      const source = threadClient.source(packet.frame.where.source);
      source.setBreakpoint(location).then(function() {
        executeSoon(resolve);
      }, function(response) {
        // Eval scripts don't stick around long enough for the breakpoint to be set,
        // so just make sure we got the expected response from the actor.
        Assert.notEqual(response.error, "noScript");

        executeSoon(resolve);
      });
||||||| merged common ancestors
      executeSoon(function() {
        gClient.close().then(gCallback);
      });
=======
      /* eslint-disable */
    Cu.evalInSandbox(
      "var line0 = Error().lineNumber;\n" +
      "debugger;\n" +
      "var a = 1;\n" +  // line0 + 2
      "var b = 2;\n",  // line0 + 3
      debuggee
    );
    /* eslint-enable */
>>>>>>> upstream-releases
    });
<<<<<<< HEAD

    /* eslint-disable */
    Cu.evalInSandbox(
      "var line0 = Error().lineNumber;\n" +
      "debugger;\n" +
      "var a = 1;\n" +  // line0 + 2
      "var b = 2;\n",  // line0 + 3
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
    "debugger;\n" +
    "var a = 1;\n" +  // line0 + 2
    "var b = 2;\n",  // line0 + 3
    gDebuggee
  );
  /* eslint-enable */
}
=======
  })
);
>>>>>>> upstream-releases
