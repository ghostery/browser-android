/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
/* eslint-disable no-shadow, max-nested-callbacks */

"use strict";

<<<<<<< HEAD
async function testPrincipal(options, globalPrincipal, debuggeeHasXrays) {
  const { debuggee } = options;
  let global, subsumes, isOpaque, globalIsInvisible;
||||||| merged common ancestors
var gServer;
var gDebuggee;
var gDebuggeeHasXrays;
var gClient;
var gThreadClient;
var gGlobal;
var gGlobalIsInvisible;
var gSubsumes;
var gIsOpaque;

function run_test() {
  run_test_with_server(DebuggerServer, function() {
    run_test_with_server(WorkerDebuggerServer, do_test_finished);
  });
  do_test_pending();
}

async function run_test_with_server(server, callback) {
  gServer = server;
  initTestDebuggerServer(server);

  // Run tests with a system principal debuggee.
  await run_tests_in_principal(systemPrincipal, "test-grips-system-principal");

  // Run tests with a null principal debuggee.
  await run_tests_in_principal(null, "test-grips-null-principal");

  callback();
}

async function run_tests_in_principal(debuggeePrincipal, title) {
  for (gDebuggeeHasXrays of [true, false]) {
    // Prepare the debuggee.
    const fullTitle = gDebuggeeHasXrays ? title + "-with-xrays" : title;
    gDebuggee = Cu.Sandbox(debuggeePrincipal, {wantXrays: gDebuggeeHasXrays});
    gDebuggee.__name = fullTitle;
    gServer.addTestGlobal(gDebuggee);
    gDebuggee.eval(function stopMe() {
      debugger;
    }.toString());
    gClient = new DebuggerClient(gServer.connectPipe());
    await gClient.connect();
    const [,, threadClient] = await attachTestTabAndResume(gClient, fullTitle);
    gThreadClient = threadClient;

    // Test objects created in the debuggee.
    await testPrincipal(undefined);

    // Test objects created in a system principal new global.
    await testPrincipal(systemPrincipal);

    // Test objects created in a cross-origin null principal new global.
    await testPrincipal(null);

    if (debuggeePrincipal === null) {
      // Test objects created in a same-origin null principal new global.
      await testPrincipal(Cu.getObjectPrincipal(gDebuggee));
    }

    // Finish.
    await gClient.close();
  }
}

async function testPrincipal(globalPrincipal) {
=======
Services.prefs.setBoolPref("security.allow_eval_with_system_principal", true);

registerCleanupFunction(() => {
  Services.prefs.clearUserPref("security.allow_eval_with_system_principal");
});

async function testPrincipal(options, globalPrincipal, debuggeeHasXrays) {
  const { debuggee } = options;
>>>>>>> upstream-releases
  // Create a global object with the specified security principal.
  // If none is specified, use the debuggee.
  if (globalPrincipal === undefined) {
<<<<<<< HEAD
    global = debuggee;
    subsumes = true;
    isOpaque = false;
    globalIsInvisible = false;
    await test(options, { global, subsumes, isOpaque, globalIsInvisible });
||||||| merged common ancestors
    gGlobal = gDebuggee;
    gSubsumes = true;
    gIsOpaque = false;
    gGlobalIsInvisible = false;
    await test();
=======
    await test(options, {
      global: debuggee,
      subsumes: true,
      isOpaque: false,
      globalIsInvisible: false,
    });
>>>>>>> upstream-releases
    return;
  }

<<<<<<< HEAD
  const debuggeePrincipal = Cu.getObjectPrincipal(debuggee);
  const sameOrigin = debuggeePrincipal === globalPrincipal;
  subsumes = sameOrigin || debuggeePrincipal === systemPrincipal;
||||||| merged common ancestors
  const debuggeePrincipal = Cu.getObjectPrincipal(gDebuggee);
  const sameOrigin = debuggeePrincipal === globalPrincipal;
  gSubsumes = sameOrigin || debuggeePrincipal === systemPrincipal;
=======
  const debuggeePrincipal = Cu.getObjectPrincipal(debuggee);
  const sameOrigin = debuggeePrincipal.origin === globalPrincipal.origin;
  const subsumes = debuggeePrincipal.subsumes(globalPrincipal);
>>>>>>> upstream-releases
  for (const globalHasXrays of [true, false]) {
<<<<<<< HEAD
    isOpaque = subsumes && globalPrincipal !== systemPrincipal
                && (sameOrigin && debuggeeHasXrays || globalHasXrays);
    for (globalIsInvisible of [true, false]) {
      global = Cu.Sandbox(globalPrincipal, {
||||||| merged common ancestors
    gIsOpaque = gSubsumes && globalPrincipal !== systemPrincipal
                && (sameOrigin && gDebuggeeHasXrays || globalHasXrays);
    for (gGlobalIsInvisible of [true, false]) {
      gGlobal = Cu.Sandbox(globalPrincipal, {
=======
    const isOpaque =
      subsumes &&
      globalPrincipal !== systemPrincipal &&
      ((sameOrigin && debuggeeHasXrays) || globalHasXrays);
    for (const globalIsInvisible of [true, false]) {
      let global = Cu.Sandbox(globalPrincipal, {
>>>>>>> upstream-releases
        wantXrays: globalHasXrays,
        invisibleToDebugger: globalIsInvisible,
      });
      // Previously, the Sandbox constructor would (bizarrely) waive xrays on
      // the return Sandbox if wantXrays was false. This has now been fixed,
      // but we need to mimic that behavior here to make the test continue
      // to pass.
      if (!globalHasXrays) {
        global = Cu.waiveXrays(global);
      }
      await test(options, { global, subsumes, isOpaque, globalIsInvisible });
    }
  }
}

function test({ threadClient, debuggee }, testOptions) {
  const { global } = testOptions;
  return new Promise(function(resolve) {
<<<<<<< HEAD
    threadClient.addOneTimeListener("paused", async function(event, packet) {
||||||| merged common ancestors
    gThreadClient.addOneTimeListener("paused", async function(event, packet) {
=======
    threadClient.once("paused", async function(packet) {
>>>>>>> upstream-releases
      // Get the grips.
      const [
        proxyGrip,
        inheritsProxyGrip,
        inheritsProxy2Grip,
      ] = packet.frame.arguments;

      // Check the grip of the proxy object.
<<<<<<< HEAD
      check_proxy_grip(debuggee, testOptions, proxyGrip);
||||||| merged common ancestors
      check_proxy_grip(proxyGrip);
=======
      check_proxy_grip(debuggee, testOptions, proxyGrip);

      // Check the target and handler slots of the proxy object.
      const proxyClient = threadClient.pauseGrip(proxyGrip);
      const proxySlots = await proxyClient.getProxySlots();
      check_proxy_slots(debuggee, testOptions, proxyGrip, proxySlots);
>>>>>>> upstream-releases

      // Check the prototype and properties of the proxy object.
<<<<<<< HEAD
      const proxyClient = threadClient.pauseGrip(proxyGrip);
||||||| merged common ancestors
      const proxyClient = gThreadClient.pauseGrip(proxyGrip);
=======
>>>>>>> upstream-releases
      const proxyResponse = await proxyClient.getPrototypeAndProperties();
<<<<<<< HEAD
      check_properties(testOptions, proxyResponse.ownProperties, true, false);
      check_prototype(debuggee, testOptions, proxyResponse.prototype, true, false);
||||||| merged common ancestors
      check_properties(proxyResponse.ownProperties, true, false);
      check_prototype(proxyResponse.prototype, true, false);
=======
      check_properties(testOptions, proxyResponse.ownProperties, true, false);
      check_prototype(
        debuggee,
        testOptions,
        proxyResponse.prototype,
        true,
        false
      );
>>>>>>> upstream-releases

      // Check the prototype and properties of the object which inherits from the proxy.
      const inheritsProxyClient = threadClient.pauseGrip(inheritsProxyGrip);
      const inheritsProxyResponse = await inheritsProxyClient.getPrototypeAndProperties();
<<<<<<< HEAD
      check_properties(testOptions, inheritsProxyResponse.ownProperties, false, false);
      check_prototype(debuggee, testOptions, inheritsProxyResponse.prototype, false,
        false);
||||||| merged common ancestors
      check_properties(inheritsProxyResponse.ownProperties, false, false);
      check_prototype(inheritsProxyResponse.prototype, false, false);
=======
      check_properties(
        testOptions,
        inheritsProxyResponse.ownProperties,
        false,
        false
      );
      check_prototype(
        debuggee,
        testOptions,
        inheritsProxyResponse.prototype,
        false,
        false
      );
>>>>>>> upstream-releases

      // The prototype chain was not iterated if the object was inaccessible, so now check
      // another object which inherits from the proxy, but was created in the debuggee.
<<<<<<< HEAD
      const inheritsProxy2Client = threadClient.pauseGrip(inheritsProxy2Grip);
      const inheritsProxy2Response =
        await inheritsProxy2Client.getPrototypeAndProperties();
      check_properties(testOptions, inheritsProxy2Response.ownProperties, false, true);
      check_prototype(debuggee, testOptions, inheritsProxy2Response.prototype, false,
        true);
||||||| merged common ancestors
      const inheritsProxy2Client = gThreadClient.pauseGrip(inheritsProxy2Grip);
      const inheritsProxy2Response =
        await inheritsProxy2Client.getPrototypeAndProperties();
      check_properties(inheritsProxy2Response.ownProperties, false, true);
      check_prototype(inheritsProxy2Response.prototype, false, true);
=======
      const inheritsProxy2Client = threadClient.pauseGrip(inheritsProxy2Grip);
      const inheritsProxy2Response = await inheritsProxy2Client.getPrototypeAndProperties();
      check_properties(
        testOptions,
        inheritsProxy2Response.ownProperties,
        false,
        true
      );
      check_prototype(
        debuggee,
        testOptions,
        inheritsProxy2Response.prototype,
        false,
        true
      );
>>>>>>> upstream-releases

      // Check that none of the above ran proxy traps.
      strictEqual(global.trapDidRun, false, "No proxy trap did run.");

      // Resume the debugger and finish the current test.
      await threadClient.resume();
      resolve();
    });

    // Create objects in `global`, and debug them in `debuggee`. They may get various
    // kinds of security wrappers, or no wrapper at all.
    // To detect that no proxy trap runs, the proxy handler should define all possible
    // traps, but the list is long and may change. Therefore a second proxy is used as
    // the handler, so that a single `get` trap suffices.
    global.eval(`
      var trapDidRun = false;
      var proxy = new Proxy({}, new Proxy({}, {get: (_, trap) => {
        trapDidRun = true;
        throw new Error("proxy trap '" + trap + "' was called.");
      }}));
      var inheritsProxy = Object.create(proxy, {x:{value:1}});
    `);
<<<<<<< HEAD
    const data = Cu.createObjectIn(debuggee, {defineAs: "data"});
    data.proxy = global.proxy;
    data.inheritsProxy = global.inheritsProxy;
    debuggee.eval(`
||||||| merged common ancestors
    const data = Cu.createObjectIn(gDebuggee, {defineAs: "data"});
    data.proxy = gGlobal.proxy;
    data.inheritsProxy = gGlobal.inheritsProxy;
    gDebuggee.eval(`
=======
    const data = Cu.createObjectIn(debuggee, { defineAs: "data" });
    data.proxy = global.proxy;
    data.inheritsProxy = global.inheritsProxy;
    debuggee.eval(`
>>>>>>> upstream-releases
      var inheritsProxy2 = Object.create(data.proxy, {x:{value:1}});
      stopMe(data.proxy, data.inheritsProxy, inheritsProxy2);
    `);
  });
}

<<<<<<< HEAD
function check_proxy_grip(debuggee, testOptions, grip) {
  const { global, isOpaque, subsumes, globalIsInvisible } = testOptions;
  const {preview} = grip;
||||||| merged common ancestors
function check_proxy_grip(grip) {
  const {preview} = grip;
=======
function check_proxy_grip(debuggee, testOptions, grip) {
  const { global, isOpaque, subsumes, globalIsInvisible } = testOptions;
  const { preview } = grip;
>>>>>>> upstream-releases

  if (global === debuggee) {
    // The proxy has no security wrappers.
    strictEqual(grip.class, "Proxy", "The grip has a Proxy class.");
<<<<<<< HEAD
    ok(grip.proxyTarget, "There is a [[ProxyTarget]] grip.");
    ok(grip.proxyHandler, "There is a [[ProxyHandler]] grip.");
    strictEqual(preview.ownPropertiesLength, 2, "The preview has 2 properties.");
    const target = preview.ownProperties["<target>"].value;
    strictEqual(target, grip.proxyTarget, "<target> contains the [[ProxyTarget]].");
    const handler = preview.ownProperties["<handler>"].value;
    strictEqual(handler, grip.proxyHandler, "<handler> contains the [[ProxyHandler]].");
  } else if (isOpaque) {
||||||| merged common ancestors
    ok(grip.proxyTarget, "There is a [[ProxyTarget]] grip.");
    ok(grip.proxyHandler, "There is a [[ProxyHandler]] grip.");
    strictEqual(preview.ownPropertiesLength, 2, "The preview has 2 properties.");
    const target = preview.ownProperties["<target>"].value;
    strictEqual(target, grip.proxyTarget, "<target> contains the [[ProxyTarget]].");
    const handler = preview.ownProperties["<handler>"].value;
    strictEqual(handler, grip.proxyHandler, "<handler> contains the [[ProxyHandler]].");
  } else if (gIsOpaque) {
=======
    strictEqual(
      preview.ownPropertiesLength,
      2,
      "The preview has 2 properties."
    );
    const props = preview.ownProperties;
    ok(props["<target>"].value, "<target> contains the [[ProxyTarget]].");
    ok(props["<handler>"].value, "<handler> contains the [[ProxyHandler]].");
  } else if (isOpaque) {
>>>>>>> upstream-releases
    // The proxy has opaque security wrappers.
    strictEqual(grip.class, "Opaque", "The grip has an Opaque class.");
    strictEqual(grip.ownPropertyLength, 0, "The grip has no properties.");
  } else if (!subsumes) {
    // The proxy belongs to compartment not subsumed by the debuggee.
<<<<<<< HEAD
    strictEqual(grip.class, "Restricted", "The grip has an Restricted class.");
    ok(!("ownPropertyLength" in grip), "The grip doesn't know the number of properties.");
  } else if (globalIsInvisible) {
||||||| merged common ancestors
    strictEqual(grip.class, "Restricted", "The grip has an Restricted class.");
    ok(!("ownPropertyLength" in grip), "The grip doesn't know the number of properties.");
  } else if (gGlobalIsInvisible) {
=======
    strictEqual(grip.class, "Restricted", "The grip has a Restricted class.");
    ok(
      !("ownPropertyLength" in grip),
      "The grip doesn't know the number of properties."
    );
  } else if (globalIsInvisible) {
>>>>>>> upstream-releases
    // The proxy belongs to an invisible-to-debugger compartment.
    strictEqual(
      grip.class,
      "InvisibleToDebugger: Object",
      "The grip has an InvisibleToDebugger class."
    );
    ok(
      !("ownPropertyLength" in grip),
      "The grip doesn't know the number of properties."
    );
  } else {
    // The proxy has non-opaque security wrappers.
    strictEqual(grip.class, "Proxy", "The grip has a Proxy class.");
    strictEqual(
      preview.ownPropertiesLength,
      0,
      "The preview has no properties."
    );
    ok(!("<target>" in preview), "The preview has no <target> property.");
    ok(!("<handler>" in preview), "The preview has no <handler> property.");
  }
}

<<<<<<< HEAD
function check_properties(testOptions, props, isProxy, createdInDebuggee) {
  const { subsumes, globalIsInvisible } = testOptions;
||||||| merged common ancestors
function check_properties(props, isProxy, createdInDebuggee) {
=======
function check_proxy_slots(debuggee, testOptions, grip, proxySlots) {
  const { global } = testOptions;

  if (grip.class !== "Proxy") {
    strictEqual(
      proxySlots,
      undefined,
      "Slots can only be retrived for Proxy grips."
    );
  } else if (global === debuggee) {
    const { proxyTarget, proxyHandler } = proxySlots;
    strictEqual(proxyTarget.type, "object", "There is a [[ProxyTarget]] grip.");
    strictEqual(
      proxyHandler.type,
      "object",
      "There is a [[ProxyHandler]] grip."
    );
  } else {
    const { proxyTarget, proxyHandler } = proxySlots;
    strictEqual(
      proxyTarget.type,
      "undefined",
      "There is no [[ProxyTarget]] grip."
    );
    strictEqual(
      proxyHandler.type,
      "undefined",
      "There is no [[ProxyHandler]] grip."
    );
  }
}

function check_properties(testOptions, props, isProxy, createdInDebuggee) {
  const { subsumes, globalIsInvisible } = testOptions;
>>>>>>> upstream-releases
  const ownPropertiesLength = Reflect.ownKeys(props).length;

<<<<<<< HEAD
  if (createdInDebuggee || !isProxy && subsumes && !globalIsInvisible) {
||||||| merged common ancestors
  if (createdInDebuggee || !isProxy && gSubsumes && !gGlobalIsInvisible) {
=======
  if (createdInDebuggee || (!isProxy && subsumes && !globalIsInvisible)) {
>>>>>>> upstream-releases
    // The debuggee can access the properties.
    strictEqual(ownPropertiesLength, 1, "1 own property was retrieved.");
    strictEqual(props.x.value, 1, "The property has the right value.");
  } else {
    // The debuggee is not allowed to access the object.
    strictEqual(ownPropertiesLength, 0, "No own property could be retrieved.");
  }
}

<<<<<<< HEAD
function check_prototype(debuggee, testOptions, proto, isProxy, createdInDebuggee) {
  const { global, isOpaque, subsumes, globalIsInvisible } = testOptions;
  if (isOpaque && !globalIsInvisible && !createdInDebuggee) {
||||||| merged common ancestors
function check_prototype(proto, isProxy, createdInDebuggee) {
  if (gIsOpaque && !gGlobalIsInvisible && !createdInDebuggee) {
=======
function check_prototype(
  debuggee,
  testOptions,
  proto,
  isProxy,
  createdInDebuggee
) {
  const { global, isOpaque, subsumes, globalIsInvisible } = testOptions;
  if (isOpaque && !globalIsInvisible && !createdInDebuggee) {
>>>>>>> upstream-releases
    // The object is or inherits from a proxy with opaque security wrappers.
    // The debuggee sees `Object.prototype` when retrieving the prototype.
    strictEqual(proto.class, "Object", "The prototype has a Object class.");
  } else if (isProxy && isOpaque && globalIsInvisible) {
    // The object is a proxy with opaque security wrappers in an invisible global.
    // The debuggee sees an inaccessible `Object.prototype` when retrieving the prototype.
<<<<<<< HEAD
    strictEqual(proto.class, "InvisibleToDebugger: Object",
                "The prototype has an InvisibleToDebugger class.");
  } else if (createdInDebuggee || !isProxy && subsumes && !globalIsInvisible) {
||||||| merged common ancestors
    strictEqual(proto.class, "InvisibleToDebugger: Object",
                "The prototype has an InvisibleToDebugger class.");
  } else if (createdInDebuggee || !isProxy && gSubsumes && !gGlobalIsInvisible) {
=======
    strictEqual(
      proto.class,
      "InvisibleToDebugger: Object",
      "The prototype has an InvisibleToDebugger class."
    );
  } else if (
    createdInDebuggee ||
    (!isProxy && subsumes && !globalIsInvisible)
  ) {
>>>>>>> upstream-releases
    // The object inherits from a proxy and has no security wrappers or non-opaque ones.
    // The debuggee sees the proxy when retrieving the prototype.
<<<<<<< HEAD
    check_proxy_grip(debuggee, { global, isOpaque, subsumes, globalIsInvisible }, proto);
||||||| merged common ancestors
    check_proxy_grip(proto);
=======
    check_proxy_grip(
      debuggee,
      { global, isOpaque, subsumes, globalIsInvisible },
      proto
    );
>>>>>>> upstream-releases
  } else {
    // The debuggee is not allowed to access the object. It sees a null prototype.
    strictEqual(proto.type, "null", "The prototype is null.");
  }
}
<<<<<<< HEAD

async function run_tests_in_principal(options, debuggeePrincipal, debuggeeHasXrays) {
  const { debuggee } = options;
  debuggee.eval(function stopMe(arg1, arg2) {
    debugger;
  }.toString());

  // Test objects created in the debuggee.
  await testPrincipal(options, undefined, debuggeeHasXrays);

  // Test objects created in a system principal new global.
  await testPrincipal(options, systemPrincipal, debuggeeHasXrays);

  // Test objects created in a cross-origin null principal new global.
  await testPrincipal(options, null, debuggeeHasXrays);

  if (debuggeePrincipal === null) {
    // Test objects created in a same-origin null principal new global.
    await testPrincipal(options, Cu.getObjectPrincipal(debuggee), debuggeeHasXrays);
  }
}

// threadClientTest uses systemPrincipal by default, but let's be explicit here.
add_task(threadClientTest(options => {
  return run_tests_in_principal(options, systemPrincipal, true);
}, { principal: systemPrincipal, wantXrays: true }));
add_task(threadClientTest(options => {
  return run_tests_in_principal(options, systemPrincipal, false);
}, { principal: systemPrincipal, wantXrays: false }));

const nullPrincipal = Cc["@mozilla.org/nullprincipal;1"].createInstance(Ci.nsIPrincipal);
add_task(threadClientTest(options => {
  return run_tests_in_principal(options, nullPrincipal, true);
}, { principal: nullPrincipal, wantXrays: true }));
add_task(threadClientTest(options => {
  return run_tests_in_principal(options, nullPrincipal, false);
}, { principal: nullPrincipal, wantXrays: false }));
||||||| merged common ancestors
=======

function createNullPrincipal() {
  return Cc["@mozilla.org/nullprincipal;1"].createInstance(Ci.nsIPrincipal);
}

async function run_tests_in_principal(
  options,
  debuggeePrincipal,
  debuggeeHasXrays
) {
  const { debuggee } = options;
  debuggee.eval(
    function stopMe(arg1, arg2) {
      debugger;
    }.toString()
  );

  // Test objects created in the debuggee.
  await testPrincipal(options, undefined, debuggeeHasXrays);

  // Test objects created in a system principal new global.
  await testPrincipal(options, systemPrincipal, debuggeeHasXrays);

  // Test objects created in a cross-origin null principal new global.
  await testPrincipal(options, createNullPrincipal(), debuggeeHasXrays);

  if (debuggeePrincipal != systemPrincipal) {
    // Test objects created in a same-origin principal new global.
    await testPrincipal(options, debuggeePrincipal, debuggeeHasXrays);
  }
}

for (const principal of [systemPrincipal, createNullPrincipal()]) {
  for (const wantXrays of [true, false]) {
    add_task(
      threadClientTest(
        options => run_tests_in_principal(options, principal, wantXrays),
        { principal, wantXrays }
      )
    );
  }
}
>>>>>>> upstream-releases
