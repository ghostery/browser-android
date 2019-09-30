/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow, max-nested-callbacks */

"use strict";

/**
 * Make sure that setting a breakpoint twice in a line without bytecodes works
 * as expected.
 */

const NUM_BREAKPOINTS = 10;
var gBpActor;
var gCount;

<<<<<<< HEAD
add_task(threadClientTest(({ threadClient, debuggee }) => {
  return new Promise(resolve => {
    threadClient.addOneTimeListener("paused", function(event, packet) {
      const source = threadClient.source(packet.frame.where.source);
      const location = { line: debuggee.line0 + 3};
||||||| merged common ancestors
function run_test() {
  run_test_with_server(DebuggerServer, function() {
    run_test_with_server(WorkerDebuggerServer, do_test_finished);
  });
  do_test_pending();
}

function run_test_with_server(server, callback) {
  gCallback = callback;
  gCount = 1;
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
=======
add_task(
  threadClientTest(({ threadClient, debuggee }) => {
    return new Promise(resolve => {
      threadClient.once("paused", async function(packet) {
        const source = await getSourceById(
          threadClient,
          packet.frame.where.actor
        );
        const location = { line: debuggee.line0 + 3 };

        source.setBreakpoint(location).then(function([response, bpClient]) {
          // Check that the breakpoint has properly skipped forward one line.
          Assert.equal(response.actualLocation.source.actor, source.actor);
          Assert.equal(response.actualLocation.line, location.line + 1);
          gBpActor = response.actor;
>>>>>>> upstream-releases

<<<<<<< HEAD
      source.setBreakpoint(location).then(function([response, bpClient]) {
        // Check that the breakpoint has properly skipped forward one line.
        Assert.equal(response.actualLocation.source.actor, source.actor);
        Assert.equal(response.actualLocation.line, location.line + 1);
        gBpActor = response.actor;

        // Set more breakpoints at the same location.
        set_breakpoints(source, location);
      });
    });
||||||| merged common ancestors
function test_child_skip_breakpoint() {
  gThreadClient.addOneTimeListener("paused", function(event, packet) {
    const source = gThreadClient.source(packet.frame.where.source);
    const location = { line: gDebuggee.line0 + 3};

    source.setBreakpoint(location).then(function([response, bpClient]) {
      // Check that the breakpoint has properly skipped forward one line.
      Assert.equal(response.actualLocation.source.actor, source.actor);
      Assert.equal(response.actualLocation.line, location.line + 1);
      gBpActor = response.actor;

      // Set more breakpoints at the same location.
      set_breakpoints(source, location);
    });
  });
=======
          // Set more breakpoints at the same location.
          set_breakpoints(source, location);
        });
      });
>>>>>>> upstream-releases

<<<<<<< HEAD
    /* eslint-disable no-multi-spaces */
    Cu.evalInSandbox("var line0 = Error().lineNumber;\n" +
                     "function foo() {\n" + // line0 + 1
                     "  this.a = 1;\n" +    // line0 + 2
                     "  // A comment.\n" +  // line0 + 3
                     "  this.b = 2;\n" +    // line0 + 4
                     "}\n" +                // line0 + 5
                     "debugger;\n" +        // line0 + 6
                     "foo();\n",            // line0 + 7
                     debuggee);
    /* eslint-enable no-multi-spaces */
||||||| merged common ancestors
  /* eslint-disable no-multi-spaces */
  Cu.evalInSandbox("var line0 = Error().lineNumber;\n" +
                   "function foo() {\n" + // line0 + 1
                   "  this.a = 1;\n" +    // line0 + 2
                   "  // A comment.\n" +  // line0 + 3
                   "  this.b = 2;\n" +    // line0 + 4
                   "}\n" +                // line0 + 5
                   "debugger;\n" +        // line0 + 6
                   "foo();\n",            // line0 + 7
                   gDebuggee);
  /* eslint-enable no-multi-spaces */
}
=======
      /* eslint-disable no-multi-spaces */
      Cu.evalInSandbox(
        "var line0 = Error().lineNumber;\n" +
        "function foo() {\n" + // line0 + 1
        "  this.a = 1;\n" + // line0 + 2
        "  // A comment.\n" + // line0 + 3
        "  this.b = 2;\n" + // line0 + 4
        "}\n" + // line0 + 5
        "debugger;\n" + // line0 + 6
          "foo();\n", // line0 + 7
        debuggee
      );
      /* eslint-enable no-multi-spaces */
>>>>>>> upstream-releases

<<<<<<< HEAD
    // Set many breakpoints at the same location.
    function set_breakpoints(source, location) {
      Assert.notEqual(gCount, NUM_BREAKPOINTS);
      source.setBreakpoint(location).then(function([response, bpClient]) {
        // Check that the breakpoint has properly skipped forward one line.
        Assert.equal(response.actualLocation.source.actor, source.actor);
        Assert.equal(response.actualLocation.line, location.line + 1);
        // Check that the same breakpoint actor was returned.
        Assert.equal(response.actor, gBpActor);
||||||| merged common ancestors
// Set many breakpoints at the same location.
function set_breakpoints(source, location) {
  Assert.notEqual(gCount, NUM_BREAKPOINTS);
  source.setBreakpoint(location).then(function([response, bpClient]) {
    // Check that the breakpoint has properly skipped forward one line.
    Assert.equal(response.actualLocation.source.actor, source.actor);
    Assert.equal(response.actualLocation.line, location.line + 1);
    // Check that the same breakpoint actor was returned.
    Assert.equal(response.actor, gBpActor);
=======
      // Set many breakpoints at the same location.
      function set_breakpoints(source, location) {
        Assert.notEqual(gCount, NUM_BREAKPOINTS);
        source.setBreakpoint(location).then(function([response, bpClient]) {
          // Check that the breakpoint has properly skipped forward one line.
          Assert.equal(response.actualLocation.source.actor, source.actor);
          Assert.equal(response.actualLocation.line, location.line + 1);
          // Check that the same breakpoint actor was returned.
          Assert.equal(response.actor, gBpActor);
>>>>>>> upstream-releases

<<<<<<< HEAD
        if (++gCount < NUM_BREAKPOINTS) {
          set_breakpoints(source, location);
          return;
        }
||||||| merged common ancestors
    if (++gCount < NUM_BREAKPOINTS) {
      set_breakpoints(source, location);
      return;
    }
=======
          if (++gCount < NUM_BREAKPOINTS) {
            set_breakpoints(source, location);
            return;
          }
>>>>>>> upstream-releases

<<<<<<< HEAD
        // After setting all the breakpoints, check that only one has effectively
        // remained.
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
    // After setting all the breakpoints, check that only one has effectively
    // remained.
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
          // After setting all the breakpoints, check that only one has effectively
          // remained.
          threadClient.once("paused", function(packet) {
            // Check the return value.
            Assert.equal(packet.type, "paused");
            Assert.equal(packet.frame.where.actor, source.actor);
            Assert.equal(packet.frame.where.line, location.line + 1);
            Assert.equal(packet.why.type, "breakpoint");
            Assert.equal(packet.why.actors[0], bpClient.actor);
            // Check that the breakpoint worked.
            Assert.equal(debuggee.a, 1);
            Assert.equal(debuggee.b, undefined);
>>>>>>> upstream-releases

<<<<<<< HEAD
          threadClient.addOneTimeListener("paused", function(event, packet) {
            // We don't expect any more pauses after the breakpoint was hit once.
            Assert.ok(false);
          });
          threadClient.resume(function() {
            // Give any remaining breakpoints a chance to trigger.
            do_timeout(1000, resolve);
          });
||||||| merged common ancestors
      gThreadClient.addOneTimeListener("paused", function(event, packet) {
        // We don't expect any more pauses after the breakpoint was hit once.
        Assert.ok(false);
      });
      gThreadClient.resume(function() {
        // Give any remaining breakpoints a chance to trigger.
        do_timeout(1000, function() {
          gClient.close().then(gCallback);
=======
            threadClient.once("paused", function(packet) {
              // We don't expect any more pauses after the breakpoint was hit once.
              Assert.ok(false);
            });
            threadClient.resume().then(function() {
              // Give any remaining breakpoints a chance to trigger.
              do_timeout(1000, resolve);
            });
          });
          // Continue until the breakpoint is hit.
          threadClient.resume();
>>>>>>> upstream-releases
        });
<<<<<<< HEAD
        // Continue until the breakpoint is hit.
        threadClient.resume();
      });
    }
  });
}));
||||||| merged common ancestors
      });
    });
    // Continue until the breakpoint is hit.
    gThreadClient.resume();
  });
}
=======
      }
    });
  })
);
>>>>>>> upstream-releases
