/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

add_task(async function setup() {
  await AddonTestUtils.promiseStartupManager();
});

add_task(async function test_fallbacks() {
  Assert.ok((await Services.search.getVisibleEngines()).length > 1);
  Assert.ok(Services.search.isInitialized);

  // Remove the current engine...
<<<<<<< HEAD
  let currentEngine = Services.search.defaultEngine;
  Services.search.removeEngine(currentEngine);
||||||| merged common ancestors
  let currentEngine = Services.search.currentEngine;
  Services.search.removeEngine(currentEngine);
=======
  let defaultEngine = await Services.search.getDefault();
  await Services.search.removeEngine(defaultEngine);
>>>>>>> upstream-releases

  // ... and verify a new current engine has been set.
<<<<<<< HEAD
  Assert.notEqual(Services.search.defaultEngine.name, currentEngine.name);
  Assert.ok(currentEngine.hidden);
||||||| merged common ancestors
  Assert.notEqual(Services.search.currentEngine.name, currentEngine.name);
  Assert.ok(currentEngine.hidden);
=======
  Assert.notEqual(
    (await Services.search.getDefault()).name,
    defaultEngine.name
  );
  Assert.ok(defaultEngine.hidden);
>>>>>>> upstream-releases

  // Remove all the other engines.
  for (let engine of await Services.search.getVisibleEngines()) {
    await Services.search.removeEngine(engine);
  }
  Assert.strictEqual((await Services.search.getVisibleEngines()).length, 0);

  // Verify the original default engine is used as a fallback and no
  // longer hidden.
<<<<<<< HEAD
  Assert.equal(Services.search.defaultEngine.name, currentEngine.name);
  Assert.ok(!currentEngine.hidden);
  Assert.equal(Services.search.getVisibleEngines().length, 1);
}
||||||| merged common ancestors
  Assert.equal(Services.search.currentEngine.name, currentEngine.name);
  Assert.ok(!currentEngine.hidden);
  Assert.equal(Services.search.getVisibleEngines().length, 1);
}
=======
  Assert.equal((await Services.search.getDefault()).name, defaultEngine.name);
  Assert.ok(!defaultEngine.hidden);
  Assert.equal((await Services.search.getVisibleEngines()).length, 1);
});
>>>>>>> upstream-releases
