/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

/**
 * Check that step out doesn't double stop on a breakpoint.  Bug 970469.
 */

<<<<<<< HEAD
add_task(threadClientTest(async ({ threadClient, debuggee, client }) => {
  dumpn("Evaluating test code and waiting for first debugger statement");
  const dbgStmt = await executeOnNextTickAndWaitForPause(
    () => evaluateTestCode(debuggee), client);
  equal(dbgStmt.frame.where.line, 3, "Should be at debugger statement on line 3");

  dumpn("Setting breakpoint in innerFunction");
  const source = threadClient.source(dbgStmt.frame.where.source);
  await source.setBreakpoint({ line: 7 });

  dumpn("Step in to innerFunction");
  const step1 = await stepIn(client, threadClient);
  equal(step1.frame.where.line, 7);

  dumpn("Step out of innerFunction");
  const step2 = await stepOut(client, threadClient);
  // The bug was that we'd stop again at the breakpoint on line 7.
  equal(step2.frame.where.line, 4);
}));

function evaluateTestCode(debuggee) {
||||||| merged common ancestors
var gDebuggee;
var gClient;
var gCallback;

function run_test() {
  do_test_pending();
  run_test_with_server(DebuggerServer, function() {
    run_test_with_server(WorkerDebuggerServer, do_test_finished);
  });
}

function run_test_with_server(server, callback) {
  gCallback = callback;
  initTestDebuggerServer(server);
  gDebuggee = addTestGlobal("test-stepping", server);
  gClient = new DebuggerClient(server.connectPipe());
  gClient.connect(testStepOutWithBreakpoint);
}

async function testStepOutWithBreakpoint() {
  const [attachResponse,, threadClient] = await attachTestTabAndResume(gClient,
                                                                       "test-stepping");
  ok(!attachResponse.error, "Should not get an error attaching");

  dumpn("Evaluating test code and waiting for first debugger statement");
  const dbgStmt = await executeOnNextTickAndWaitForPause(evaluateTestCode, gClient);
  equal(dbgStmt.frame.where.line, 3, "Should be at debugger statement on line 3");

  dumpn("Setting breakpoint in innerFunction");
  const source = threadClient.source(dbgStmt.frame.where.source);
  await source.setBreakpoint({ line: 7 });

  dumpn("Step in to innerFunction");
  const step1 = await stepIn(gClient, threadClient);
  equal(step1.frame.where.line, 7);

  dumpn("Step out of innerFunction");
  const step2 = await stepOut(gClient, threadClient);
  // The bug was that we'd stop again at the breakpoint on line 7.
  equal(step2.frame.where.line, 4);

  finishClient(gClient, gCallback);
}

function evaluateTestCode() {
=======
add_task(
  threadClientTest(async ({ threadClient, debuggee }) => {
    dumpn("Evaluating test code and waiting for first debugger statement");
    const dbgStmt = await executeOnNextTickAndWaitForPause(
      () => evaluateTestCode(debuggee),
      threadClient
    );
    equal(
      dbgStmt.frame.where.line,
      3,
      "Should be at debugger statement on line 3"
    );

    dumpn("Setting breakpoint in innerFunction");
    const source = await getSourceById(threadClient, dbgStmt.frame.where.actor);
    await threadClient.setBreakpoint({ sourceUrl: source.url, line: 7 }, {});

    dumpn("Step in to innerFunction");
    const step1 = await stepOver(threadClient);
    equal(step1.frame.where.line, 3);

    dumpn("Step in to innerFunction");
    const step2 = await stepIn(threadClient);
    equal(step2.frame.where.line, 7);

    dumpn("Step out of innerFunction");
    const step3 = await stepOut(threadClient);
    // The bug was that we'd stop again at the breakpoint on line 7.
    equal(step3.frame.where.line, 4);
  })
);

function evaluateTestCode(debuggee) {
>>>>>>> upstream-releases
  /* eslint-disable */
  Cu.evalInSandbox(
    `                                   //  1
    function outerFunction() {          //  2
      debugger; innerFunction();        //  3
    }                                   //  4
                                        //  5
    function innerFunction() {          //  6
      var x = 0;                        //  7
      var y = 72;                       //  8
      return x+y;                       //  9
    }                                   // 10
    outerFunction();                    // 11
    `,                                  // 12
    debuggee,
    "1.8",
    "test_stepping-08-test-code.js",
    1
  );
  /* eslint-enable */
}
