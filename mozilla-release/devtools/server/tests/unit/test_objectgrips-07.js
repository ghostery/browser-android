/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

/**
 * This test checks that objects which are not extensible report themselves as
 * such.
 */

Services.prefs.setBoolPref("security.allow_eval_with_system_principal", true);
registerCleanupFunction(() => {
  Services.prefs.clearUserPref("security.allow_eval_with_system_principal");
});

<<<<<<< HEAD
add_task(threadClientTest(async ({ threadClient, debuggee, client }) => {
  return new Promise(resolve => {
    threadClient.addOneTimeListener("paused", function(event, packet) {
      const [f, s, ne, e] = packet.frame.arguments;
      const [fClient, sClient, neClient, eClient] = packet.frame.arguments.map(
        a => threadClient.pauseGrip(a));
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
  gDebuggee = addTestGlobal("test-grips", server);
  gDebuggee.eval(function stopMe(arg1, arg2, arg3, arg4) {
    debugger;
  }.toString());

  gClient = new DebuggerClient(server.connectPipe());
  gClient.connect().then(function() {
    attachTestTabAndResume(gClient, "test-grips",
                           function(response, targetFront, threadClient) {
                             gThreadClient = threadClient;
                             test_object_grip();
                           });
  });
}
=======
add_task(
  threadClientTest(async ({ threadClient, debuggee, client }) => {
    return new Promise(resolve => {
      threadClient.once("paused", function(packet) {
        const [f, s, ne, e] = packet.frame.arguments;
        const [
          fClient,
          sClient,
          neClient,
          eClient,
        ] = packet.frame.arguments.map(a => threadClient.pauseGrip(a));

        Assert.ok(!f.extensible);
        Assert.ok(!fClient.isExtensible);

        Assert.ok(!s.extensible);
        Assert.ok(!sClient.isExtensible);
>>>>>>> upstream-releases

<<<<<<< HEAD
      Assert.ok(!f.extensible);
      Assert.ok(!fClient.isExtensible);
||||||| merged common ancestors
function test_object_grip() {
  gThreadClient.addOneTimeListener("paused", function(event, packet) {
    const [f, s, ne, e] = packet.frame.arguments;
    const [fClient, sClient, neClient, eClient] = packet.frame.arguments.map(
      a => gThreadClient.pauseGrip(a));
=======
        Assert.ok(!ne.extensible);
        Assert.ok(!neClient.isExtensible);
>>>>>>> upstream-releases

<<<<<<< HEAD
      Assert.ok(!s.extensible);
      Assert.ok(!sClient.isExtensible);
||||||| merged common ancestors
    Assert.ok(!f.extensible);
    Assert.ok(!fClient.isExtensible);
=======
        Assert.ok(e.extensible);
        Assert.ok(eClient.isExtensible);
>>>>>>> upstream-releases

<<<<<<< HEAD
      Assert.ok(!ne.extensible);
      Assert.ok(!neClient.isExtensible);
||||||| merged common ancestors
    Assert.ok(!s.extensible);
    Assert.ok(!sClient.isExtensible);
=======
        threadClient.resume().then(resolve);
      });
>>>>>>> upstream-releases

<<<<<<< HEAD
      Assert.ok(e.extensible);
      Assert.ok(eClient.isExtensible);

      threadClient.resume(resolve);
||||||| merged common ancestors
    Assert.ok(!ne.extensible);
    Assert.ok(!neClient.isExtensible);

    Assert.ok(e.extensible);
    Assert.ok(eClient.isExtensible);

    gThreadClient.resume(_ => {
      gClient.close().then(gCallback);
=======
      debuggee.eval(
        function stopMe(arg1) {
          debugger;
        }.toString()
      );
      /* eslint-disable no-undef */
      debuggee.eval(
        "(" +
          function() {
            const f = {};
            Object.freeze(f);
            const s = {};
            Object.seal(s);
            const ne = {};
            Object.preventExtensions(ne);
            stopMe(f, s, ne, {});
          } +
          "())"
      );
      /* eslint-enable no-undef */
>>>>>>> upstream-releases
    });
<<<<<<< HEAD

    debuggee.eval(function stopMe(arg1) {
      debugger;
    }.toString());
    /* eslint-disable no-undef */
    debuggee.eval("(" + function() {
      const f = {};
      Object.freeze(f);
      const s = {};
      Object.seal(s);
      const ne = {};
      Object.preventExtensions(ne);
      stopMe(f, s, ne, {});
    } + "())");
    /* eslint-enable no-undef */
  });
}));
||||||| merged common ancestors
  });

  /* eslint-disable no-undef */
  gDebuggee.eval("(" + function() {
    const f = {};
    Object.freeze(f);
    const s = {};
    Object.seal(s);
    const ne = {};
    Object.preventExtensions(ne);
    stopMe(f, s, ne, {});
  } + "())");
  /* eslint-enable no-undef */
}
=======
  })
);
>>>>>>> upstream-releases
