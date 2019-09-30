/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow, max-nested-callbacks */

"use strict";

/**
 * Check that setting a breakpoint in a line without code in a child script
 * will skip forward, in a file with two scripts.
 */

<<<<<<< HEAD
add_task(threadClientTest(({ threadClient, debuggee }) => {
  return new Promise(resolve => {
    threadClient.addOneTimeListener("paused", function(event, packet) {
      threadClient.eval(packet.frame.actor, "foo", function(response) {
        threadClient.addOneTimeListener("paused", function(event, packet) {
          const obj = threadClient.pauseGrip(packet.why.frameFinished.return);
          obj.getDefinitionSite(runWithBreakpoint);
        });
      });

      function runWithBreakpoint(packet) {
        const source = threadClient.source(packet.source);
        const location = { line: debuggee.line0 + 3 };

        source.setBreakpoint(location).then(function([response, bpClient]) {
          // Check that the breakpoint has properly skipped forward one line.
          Assert.equal(response.actualLocation.source.actor, source.actor);
          Assert.equal(response.actualLocation.line, location.line + 1);
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
                             test_child_skip_breakpoint();
                           });
  });
}

function test_child_skip_breakpoint() {
  gThreadClient.addOneTimeListener("paused", function(event, packet) {
    gThreadClient.eval(packet.frame.actor, "foo", function(response) {
      gThreadClient.addOneTimeListener("paused", function(event, packet) {
        const obj = gThreadClient.pauseGrip(packet.why.frameFinished.return);
        obj.getDefinitionSite(runWithBreakpoint);
      });
    });

    function runWithBreakpoint(packet) {
      const source = gThreadClient.source(packet.source);
      const location = { line: gDebuggee.line0 + 3 };

      source.setBreakpoint(location).then(function([response, bpClient]) {
        // Check that the breakpoint has properly skipped forward one line.
        Assert.equal(response.actualLocation.source.actor, source.actor);
        Assert.equal(response.actualLocation.line, location.line + 1);
=======
add_task(
  threadClientTest(({ threadClient, debuggee }) => {
    return new Promise(resolve => {
      threadClient.once("paused", async function(packet) {
        const line = debuggee.line0 + 3;
        const source = await getSourceById(
          threadClient,
          packet.frame.where.actor
        );

        // this test has been disabled for a long time so the functionality doesn't work
        const response = await threadClient.setBreakpoint(
          { sourceUrl: source.url, line: line },
          {}
        );
        // check that the breakpoint has properly skipped forward one line.
        assert.equal(response.actuallocation.source.actor, source.actor);
        Assert.equal(response.actualLocation.line, location.line + 1);
>>>>>>> upstream-releases

<<<<<<< HEAD
          threadClient.addOneTimeListener("paused", function(event, packet) {
            // Check the return value.
            Assert.equal(packet.type, "paused");
            Assert.equal(packet.frame.where.source.actor, source.actor);
            Assert.equal(packet.frame.where.line, location.line + 1);
            Assert.equal(packet.why.type, "breakpoint");
            Assert.equal(packet.why.actors[0], bpClient.actor);
            // Check that the breakpoint worked.
            Assert.equal(debuggee.a, 1);
            Assert.equal(debuggee.b, undefined);
||||||| merged common ancestors
        gThreadClient.addOneTimeListener("paused", function(event, packet) {
          // Check the return value.
          Assert.equal(packet.type, "paused");
          Assert.equal(packet.frame.where.source.actor, source.actor);
          Assert.equal(packet.frame.where.line, location.line + 1);
          Assert.equal(packet.why.type, "breakpoint");
          Assert.equal(packet.why.actors[0], bpClient.actor);
          // Check that the breakpoint worked.
          Assert.equal(gDebuggee.a, 1);
          Assert.equal(gDebuggee.b, undefined);
=======
        threadClient.once("paused", function(packet) {
          // Check the return value.
          Assert.equal(packet.type, "paused");
          Assert.equal(packet.frame.where.actor, source.actor);
          Assert.equal(packet.frame.where.line, location.line + 1);
          Assert.equal(packet.why.type, "breakpoint");
          Assert.equal(packet.why.actors[0], response.bpClient.actor);
          // Check that the breakpoint worked.
          Assert.equal(debuggee.a, 1);
          Assert.equal(debuggee.b, undefined);
>>>>>>> upstream-releases

<<<<<<< HEAD
            // Remove the breakpoint.
            bpClient.remove(function(response) {
              threadClient.resume(resolve);
            });
||||||| merged common ancestors
          // Remove the breakpoint.
          bpClient.remove(function(response) {
            gThreadClient.resume(function() {
              gClient.close().then(gCallback);
            });
=======
          // Remove the breakpoint.
          response.bpClient.remove(function(response) {
            threadClient.resume().then(resolve);
>>>>>>> upstream-releases
          });

<<<<<<< HEAD
          // Continue until the breakpoint is hit.
          threadClient.resume();
        });
      }
    });
||||||| merged common ancestors
        // Continue until the breakpoint is hit.
        gThreadClient.resume();
      });
    }
  });
=======
        // Continue until the breakpoint is hit.
        threadClient.resume();
      });
>>>>>>> upstream-releases

    /* eslint-disable */
    Cu.evalInSandbox("var line0 = Error().lineNumber;\n" +
                     "function foo() {\n" + // line0 + 1
                     "  this.a = 1;\n" +    // line0 + 2
                     "  // A comment.\n" +  // line0 + 3
                     "  this.b = 2;\n" +    // line0 + 4
                     "}\n",                 // line0 + 5
                     debuggee,
                     "1.7",
                     "script1.js");

<<<<<<< HEAD
    Cu.evalInSandbox("var line1 = Error().lineNumber;\n" +
                     "debugger;\n" +        // line1 + 1
                     "foo();\n",           // line1 + 2
                     debuggee,
                     "1.7",
                     "script2.js");
    /* eslint-enable */
  });
}));
||||||| merged common ancestors
  Cu.evalInSandbox("var line1 = Error().lineNumber;\n" +
                   "debugger;\n" +        // line1 + 1
                   "foo();\n",           // line1 + 2
                   gDebuggee,
                   "1.7",
                   "script2.js");
  /* eslint-enable */
}
=======
    Cu.evalInSandbox("var line1 = Error().lineNumber;\n" +
                     "debugger;\n" +        // line1 + 1
                     "foo();\n",           // line1 + 2
                     debuggee,
                     "1.7",
                     "script2.js");
    /* eslint-enable */
    });
  })
);
>>>>>>> upstream-releases
