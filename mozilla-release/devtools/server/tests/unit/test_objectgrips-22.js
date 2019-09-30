/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow, max-nested-callbacks */

"use strict";

Services.prefs.setBoolPref("security.allow_eval_with_system_principal", true);
registerCleanupFunction(() => {
  Services.prefs.clearUserPref("security.allow_eval_with_system_principal");
});

<<<<<<< HEAD
add_task(threadClientTest(async ({ threadClient, debuggee, client }) => {
  await new Promise(function(resolve) {
    threadClient.addOneTimeListener("paused", async function(event, packet) {
      const [grip] = packet.frame.arguments;
      const objClient = threadClient.pauseGrip(grip);
      const {iterator} = await objClient.enumSymbols();
      const {ownSymbols} = await iterator.slice(0, iterator.count);
||||||| merged common ancestors
async function run_test_with_server(server) {
  initTestDebuggerServer(server);
  const title = "test_enum_symbols";
  gDebuggee = addTestGlobal(title, server);
  gDebuggee.eval(function stopMe(arg) {
    debugger;
  }.toString());
  const client = new DebuggerClient(server.connectPipe());
  await client.connect();
  [,, gThreadClient] = await attachTestTabAndResume(client, title);
  await test_enum_symbols();
  await client.close();
}

async function test_enum_symbols() {
  await new Promise(function(resolve) {
    gThreadClient.addOneTimeListener("paused", async function(event, packet) {
      const [grip] = packet.frame.arguments;
      const objClient = gThreadClient.pauseGrip(grip);
      const {iterator} = await objClient.enumSymbols();
      const {ownSymbols} = await iterator.slice(0, iterator.count);
=======
add_task(
  threadClientTest(async ({ threadClient, debuggee, client }) => {
    await new Promise(function(resolve) {
      threadClient.once("paused", async function(packet) {
        const [grip] = packet.frame.arguments;
        const objClient = threadClient.pauseGrip(grip);
        const { iterator } = await objClient.enumSymbols();
        const { ownSymbols } = await iterator.slice(0, iterator.count);
>>>>>>> upstream-releases

        strictEqual(ownSymbols.length, 1, "There is 1 symbol property.");
        const { name, descriptor } = ownSymbols[0];
        strictEqual(name, "Symbol(sym)", "Got right symbol name.");
        deepEqual(
          descriptor,
          {
            configurable: false,
            enumerable: false,
            writable: false,
            value: 1,
          },
          "Got right property descriptor."
        );

<<<<<<< HEAD
      await threadClient.resume();
      resolve();
||||||| merged common ancestors
      await gThreadClient.resume();
      resolve();
=======
        await threadClient.resume();
        resolve();
      });
      debuggee.eval(
        function stopMe(arg1) {
          debugger;
        }.toString()
      );
      debuggee.eval(
        `stopMe(Object.defineProperty({}, Symbol("sym"), {value: 1}));`
      );
>>>>>>> upstream-releases
    });
<<<<<<< HEAD
    debuggee.eval(function stopMe(arg1) {
      debugger;
    }.toString());
    debuggee.eval(`stopMe(Object.defineProperty({}, Symbol("sym"), {value: 1}));`);
  });
}));
||||||| merged common ancestors
    gDebuggee.eval(`stopMe(Object.defineProperty({}, Symbol("sym"), {value: 1}));`);
  });
}
=======
  })
);
>>>>>>> upstream-releases
