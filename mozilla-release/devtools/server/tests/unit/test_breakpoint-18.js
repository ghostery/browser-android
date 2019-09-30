/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

/**
 * Check that we only break on offsets that are entry points for the line we are
 * breaking on. Bug 907278.
 */

<<<<<<< HEAD
add_task(threadClientTest(({ threadClient, debuggee, client }) => {
  return new Promise(resolve => {
    // Expose console as the test script uses it
    debuggee.console = { log: x => void x };

    // Inline all paused listeners as promises won't resolve when paused
    client.addOneTimeListener("paused", async (event1, packet1) => {
      await setBreakpoint(packet1, threadClient, client);

      client.addOneTimeListener("paused", (event2, { why }) => {
        Assert.equal(why.type, "breakpoint");

        client.addOneTimeListener("paused", (event3, packet3) => {
          testDbgStatement(packet3);
          resolve();
        });
        threadClient.resume();
      });
      debuggee.test();
    });

    Cu.evalInSandbox(
      "debugger;\n" +
      function test() {
        console.log("foo bar");
        debugger;
      },
      debuggee,
      "1.8",
      "http://example.com/",
      1
    );
  });
}));
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
  gDebuggee = addTestGlobal("test-breakpoints", server);
  gDebuggee.console = { log: x => void x };
  gClient = new DebuggerClient(server.connectPipe());
  gClient.connect().then(function() {
    attachTestTabAndResume(gClient,
                           "test-breakpoints",
                           function(response, targetFront, threadClient) {
                             gThreadClient = threadClient;
                             setUpCode();
                           });
  });
}
=======
add_task(
  threadClientTest(({ threadClient, debuggee, client }) => {
    return new Promise(resolve => {
      // Expose console as the test script uses it
      debuggee.console = { log: x => void x };

      // Inline all paused listeners as promises won't resolve when paused
      threadClient.once("paused", async packet1 => {
        await setBreakpoint(packet1, threadClient, client);
>>>>>>> upstream-releases

<<<<<<< HEAD
function setBreakpoint(packet, threadClient, client) {
  return new Promise(resolve => {
    const source = threadClient.source(packet.frame.where.source);
    client.addOneTimeListener("resumed", resolve);
||||||| merged common ancestors
function setUpCode() {
  gClient.addOneTimeListener("paused", setBreakpoint);
  Cu.evalInSandbox(
    "debugger;\n" +
    function test() {
      console.log("foo bar");
      debugger;
    },
    gDebuggee,
    "1.8",
    "http://example.com/",
    1
  );
}
=======
        threadClient.once("paused", ({ why }) => {
          Assert.equal(why.type, "breakpoint");
>>>>>>> upstream-releases

<<<<<<< HEAD
    source.setBreakpoint({ line: 2 }).then(() => {
      threadClient.resume();
    });
  });
}
||||||| merged common ancestors
function setBreakpoint(event, packet) {
  const source = gThreadClient.source(packet.frame.where.source);
  gClient.addOneTimeListener("resumed", runCode);

  source.setBreakpoint({ line: 2 }).then(() => {
    gThreadClient.resume();
  });
}
=======
          threadClient.once("paused", packet3 => {
            testDbgStatement(packet3);
            resolve();
          });
          threadClient.resume();
        });
        debuggee.test();
      });

      Cu.evalInSandbox(
        "debugger;\n" +
          function test() {
            console.log("foo bar");
            debugger;
          },
        debuggee,
        "1.8",
        "http://example.com/",
        1
      );
    });
  })
);
>>>>>>> upstream-releases

<<<<<<< HEAD
function testDbgStatement({ why }) {
||||||| merged common ancestors
function runCode() {
  gClient.addOneTimeListener("paused", testBPHit);
  gDebuggee.test();
}

function testBPHit(event, { why }) {
  Assert.equal(why.type, "breakpoint");
  gClient.addOneTimeListener("paused", testDbgStatement);
  gThreadClient.resume();
}

function testDbgStatement(event, { why }) {
=======
function setBreakpoint(packet, threadClient, client) {
  return new Promise(async resolve => {
    const source = await getSourceById(threadClient, packet.frame.where.actor);
    threadClient.once("resumed", resolve);

    threadClient.setBreakpoint({ sourceUrl: source.url, line: 3 }, {});
    await client.waitForRequestsToSettle();
    await threadClient.resume();
  });
}

function testDbgStatement({ why }) {
>>>>>>> upstream-releases
  // Should continue to the debugger statement.
  Assert.equal(why.type, "debuggerStatement");
  // Not break on another offset from the same line (that isn't an entry point
  // to the line)
  Assert.notEqual(why.type, "breakpoint");
}
