/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow, max-nested-callbacks */

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
      objClient.getPrototype(function(response) {
        Assert.ok(response.prototype != undefined);

        const protoClient = threadClient.pauseGrip(response.prototype);
        protoClient.getOwnPropertyNames(function(response) {
          Assert.equal(response.ownPropertyNames.length, 2);
          Assert.equal(response.ownPropertyNames[0], "b");
          Assert.equal(response.ownPropertyNames[1], "c");

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
    objClient.getPrototype(function(response) {
      Assert.ok(response.prototype != undefined);

      const protoClient = gThreadClient.pauseGrip(response.prototype);
      protoClient.getOwnPropertyNames(function(response) {
        Assert.equal(response.ownPropertyNames.length, 2);
        Assert.equal(response.ownPropertyNames[0], "b");
        Assert.equal(response.ownPropertyNames[1], "c");

        gThreadClient.resume(function() {
          gClient.close().then(gCallback);
=======
Services.prefs.setBoolPref("security.allow_eval_with_system_principal", true);
registerCleanupFunction(() => {
  Services.prefs.clearUserPref("security.allow_eval_with_system_principal");
});

add_task(
  threadClientTest(async ({ threadClient, debuggee, client }) => {
    return new Promise(resolve => {
      threadClient.once("paused", function(packet) {
        const args = packet.frame.arguments;

        Assert.equal(args[0].class, "Object");

        const objClient = threadClient.pauseGrip(args[0]);
        objClient.getPrototype(function(response) {
          Assert.ok(response.prototype != undefined);

          const protoClient = threadClient.pauseGrip(response.prototype);
          protoClient.getOwnPropertyNames(function(response) {
            Assert.equal(response.ownPropertyNames.length, 2);
            Assert.equal(response.ownPropertyNames[0], "b");
            Assert.equal(response.ownPropertyNames[1], "c");

            threadClient.resume().then(resolve);
          });
>>>>>>> upstream-releases
        });
      });
<<<<<<< HEAD
    });
||||||| merged common ancestors
    });
  });

  gDebuggee.eval(function Constr() {
    this.a = 1;
  }.toString());
  gDebuggee.eval(
    "Constr.prototype = { b: true, c: 'foo' }; var o = new Constr(); stopMe(o)");
}
=======
>>>>>>> upstream-releases

<<<<<<< HEAD
    debuggee.eval(function stopMe(arg1) {
      debugger;
    }.toString());
    debuggee.eval(function Constr() {
      this.a = 1;
    }.toString());
    debuggee.eval(
      "Constr.prototype = { b: true, c: 'foo' }; var o = new Constr(); stopMe(o)");
  });
}));
||||||| merged common ancestors
=======
      debuggee.eval(
        function stopMe(arg1) {
          debugger;
        }.toString()
      );
      debuggee.eval(
        function Constr() {
          this.a = 1;
        }.toString()
      );
      debuggee.eval(
        "Constr.prototype = { b: true, c: 'foo' }; var o = new Constr(); stopMe(o)"
      );
    });
  })
);
>>>>>>> upstream-releases
