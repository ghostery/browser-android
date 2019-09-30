/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

<<<<<<< HEAD
Services.prefs.setBoolPref("security.allow_eval_with_system_principal", true);
registerCleanupFunction(() => {
  Services.prefs.clearUserPref("security.allow_eval_with_system_principal");
});

add_task(threadClientTest(async ({ threadClient, debuggee, client }) => {
  return new Promise(resolve => {
    threadClient.addOneTimeListener("paused", function(event, packet) {
      const args = packet.frame.arguments;

      Assert.equal(args[0].class, "Object");

      const objClient = threadClient.pauseGrip(args[0]);
      objClient.getPrototypeAndProperties(function(response) {
        Assert.equal(response.ownProperties.a.configurable, true);
        Assert.equal(response.ownProperties.a.enumerable, true);
        Assert.equal(response.ownProperties.a.writable, true);
        Assert.equal(response.ownProperties.a.value.type, "Infinity");

        Assert.equal(response.ownProperties.b.configurable, true);
        Assert.equal(response.ownProperties.b.enumerable, true);
        Assert.equal(response.ownProperties.b.writable, true);
        Assert.equal(response.ownProperties.b.value.type, "-Infinity");

        Assert.equal(response.ownProperties.c.configurable, true);
        Assert.equal(response.ownProperties.c.enumerable, true);
        Assert.equal(response.ownProperties.c.writable, true);
        Assert.equal(response.ownProperties.c.value.type, "NaN");

        Assert.equal(response.ownProperties.d.configurable, true);
        Assert.equal(response.ownProperties.d.enumerable, true);
        Assert.equal(response.ownProperties.d.writable, true);
        Assert.equal(response.ownProperties.d.value.type, "-0");

        threadClient.resume(resolve);
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
  gDebuggee = addTestGlobal("test-grips", server);
  gDebuggee.eval(function stopMe(arg1) {
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

function test_object_grip() {
  gThreadClient.addOneTimeListener("paused", function(event, packet) {
    const args = packet.frame.arguments;

    Assert.equal(args[0].class, "Object");

    const objClient = gThreadClient.pauseGrip(args[0]);
    objClient.getPrototypeAndProperties(function(response) {
      Assert.equal(response.ownProperties.a.configurable, true);
      Assert.equal(response.ownProperties.a.enumerable, true);
      Assert.equal(response.ownProperties.a.writable, true);
      Assert.equal(response.ownProperties.a.value.type, "Infinity");

      Assert.equal(response.ownProperties.b.configurable, true);
      Assert.equal(response.ownProperties.b.enumerable, true);
      Assert.equal(response.ownProperties.b.writable, true);
      Assert.equal(response.ownProperties.b.value.type, "-Infinity");

      Assert.equal(response.ownProperties.c.configurable, true);
      Assert.equal(response.ownProperties.c.enumerable, true);
      Assert.equal(response.ownProperties.c.writable, true);
      Assert.equal(response.ownProperties.c.value.type, "NaN");

      Assert.equal(response.ownProperties.d.configurable, true);
      Assert.equal(response.ownProperties.d.enumerable, true);
      Assert.equal(response.ownProperties.d.writable, true);
      Assert.equal(response.ownProperties.d.value.type, "-0");

      gThreadClient.resume(function() {
        gClient.close().then(gCallback);
=======
Services.prefs.setBoolPref("security.allow_eval_with_system_principal", true);
registerCleanupFunction(() => {
  Services.prefs.clearUserPref("security.allow_eval_with_system_principal");
});

add_task(
  threadClientTest(async ({ threadClient, debuggee }) => {
    return new Promise(resolve => {
      const bigIntEnabled = Services.prefs.getBoolPref(
        "javascript.options.bigint"
      );
      threadClient.once("paused", function(packet) {
        const args = packet.frame.arguments;

        Assert.equal(args[0].class, "Object");

        const objClient = threadClient.pauseGrip(args[0]);
        objClient.getPrototypeAndProperties(function(response) {
          const { a, b, c, d } = response.ownProperties;
          testPropertyType(a, "Infinity");
          testPropertyType(b, "-Infinity");
          testPropertyType(c, "NaN");
          testPropertyType(d, "-0");

          if (bigIntEnabled) {
            const { e, f, g } = response.ownProperties;
            testPropertyType(e, "BigInt");
            testPropertyType(f, "BigInt");
            testPropertyType(g, "BigInt");
          }

          threadClient.resume().then(resolve);
        });
>>>>>>> upstream-releases
      });

      debuggee.eval(
        function stopMe(arg1) {
          debugger;
        }.toString()
      );
      debuggee.eval(`stopMe({
      a: Infinity,
      b: -Infinity,
      c: NaN,
      d: -0,
      ${
        bigIntEnabled
          ? `e: 1n,
      f: -2n,
      g: 0n,`
          : ``
      }
    })`);
    });
<<<<<<< HEAD
||||||| merged common ancestors
  });
=======
  })
);
>>>>>>> upstream-releases

<<<<<<< HEAD
    debuggee.eval(function stopMe(arg1) {
      debugger;
    }.toString());
    debuggee.eval("stopMe({ a: Infinity, b: -Infinity, c: NaN, d: -0 })");
  });
}));

||||||| merged common ancestors
  gDebuggee.eval("stopMe({ a: Infinity, b: -Infinity, c: NaN, d: -0 })");
}

=======
function testPropertyType(prop, expectedType) {
  Assert.equal(prop.configurable, true);
  Assert.equal(prop.enumerable, true);
  Assert.equal(prop.writable, true);
  Assert.equal(prop.value.type, expectedType);
}
>>>>>>> upstream-releases
