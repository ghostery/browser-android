/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

const ID = "addon1@tests.mozilla.org";
add_task(async function run_test() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1.9.2");

<<<<<<< HEAD
  let xpi = createAddon({
    id: ID,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "0.1",
      maxVersion: "0.2",
    }],
  });
  await manuallyInstall(xpi, AddonTestUtils.profileExtensions, ID);
||||||| merged common ancestors
  await promiseWriteInstallRDFForExtension({
    id: "addon1@tests.mozilla.org",
    version: "1.0",
    name: "Test 1",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "0.1",
      maxVersion: "0.2",
    }],
  }, profileDir);

  await promiseStartupManager();
=======
  let xpi = createAddon({
    id: ID,
    targetApplications: [
      {
        id: "xpcshell@tests.mozilla.org",
        minVersion: "0.1",
        maxVersion: "0.2",
      },
    ],
  });
  await manuallyInstall(xpi, AddonTestUtils.profileExtensions, ID);
>>>>>>> upstream-releases

  AddonManager.strictCompatibility = false;
  await promiseStartupManager();

  let addon = await AddonManager.getAddonByID(ID);
  Assert.notEqual(addon, null);
  await addon.disable();

  Assert.ok(addon.userDisabled);
  Assert.ok(!addon.isActive);
  Assert.ok(!addon.appDisabled);

  let promise = promiseAddonEvent("onPropertyChanged");
  AddonManager.strictCompatibility = true;
<<<<<<< HEAD
  let [, properties] = await promise;

  Assert.deepEqual(properties, ["appDisabled"],
                   "Got onPropertyChanged for appDisabled");
  Assert.ok(addon.appDisabled);
||||||| merged common ancestors
}

async function run_test_2() {
  let aAddon = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  Assert.notEqual(aAddon, null);
  Assert.ok(aAddon.userDisabled);
  Assert.ok(!aAddon.isActive);
  Assert.ok(aAddon.appDisabled);

  prepare_test({
    "addon1@tests.mozilla.org": [
      ["onPropertyChanged", ["appDisabled"]],
    ],
  }, [], callback_soon(do_test_finished));
=======
  let [, properties] = await promise;

  Assert.deepEqual(
    properties,
    ["appDisabled"],
    "Got onPropertyChanged for appDisabled"
  );
  Assert.ok(addon.appDisabled);
>>>>>>> upstream-releases

  promise = promiseAddonEvent("onPropertyChanged");
  AddonManager.strictCompatibility = false;
<<<<<<< HEAD
  [, properties] = await promise;

  Assert.deepEqual(properties, ["appDisabled"],
                   "Got onPropertyChanged for appDisabled");
  Assert.ok(!addon.appDisabled);
});

||||||| merged common ancestors
}
=======
  [, properties] = await promise;

  Assert.deepEqual(
    properties,
    ["appDisabled"],
    "Got onPropertyChanged for appDisabled"
  );
  Assert.ok(!addon.appDisabled);
});
>>>>>>> upstream-releases
