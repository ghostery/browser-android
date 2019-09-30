<<<<<<< HEAD

||||||| merged common ancestors

ChromeUtils.import("resource://gre/modules/AppConstants.jsm");

=======
>>>>>>> upstream-releases
// Enable signature checks for these tests
gUseRealCertChecks = true;

const DATA = "data/signing_checks";
const ID = "test@somewhere.com";

const profileDir = gProfD.clone();
profileDir.append("extensions");

function verifySignatures() {
  return new Promise(resolve => {
    let observer = (subject, topic, data) => {
      Services.obs.removeObserver(observer, "xpi-signature-changed");
      resolve(JSON.parse(data));
    };
    Services.obs.addObserver(observer, "xpi-signature-changed");

    info("Verifying signatures");
    let XPIscope = ChromeUtils.import(
      "resource://gre/modules/addons/XPIProvider.jsm",
      null
    );
    XPIscope.XPIDatabase.verifySignatures();
  });
}

<<<<<<< HEAD
createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "4", "4");

add_task(async function test_no_change() {
  await promiseStartupManager();

  // Install the first add-on
  await promiseInstallFile(do_get_file(`${DATA}/signed1.xpi`));

  let addon = await promiseAddonByID(ID);
  Assert.notEqual(addon, null);
  Assert.equal(addon.appDisabled, false);
  Assert.equal(addon.isActive, true);
  Assert.equal(addon.pendingOperations, AddonManager.PENDING_NONE);
  Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_SIGNED);

  // Swap in the files from the next add-on
  manuallyUninstall(profileDir, ID);
  await manuallyInstall(do_get_file(`${DATA}/signed2.xpi`), profileDir, ID);

  let listener = {
    onPropetyChanged(_addon, properties) {
      Assert.ok(false, `Got unexpected onPropertyChanged for ${_addon.id}`);
    },
  };

  AddonManager.addAddonListener(listener);

  // Trigger the check
  let changes = await verifySignatures();
  Assert.equal(changes.enabled.length, 0);
  Assert.equal(changes.disabled.length, 0);

  Assert.equal(addon.appDisabled, false);
  Assert.equal(addon.isActive, true);
  Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_SIGNED);

  await addon.uninstall();
  AddonManager.removeAddonListener(listener);
});

add_task(async function test_diable() {
  // Install the first add-on
  await promiseInstallFile(do_get_file(`${DATA}/signed1.xpi`));

  let addon = await promiseAddonByID(ID);
  Assert.notEqual(addon, null);
  Assert.ok(addon.isActive);
  Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_SIGNED);

  // Swap in the files from the next add-on
  manuallyUninstall(profileDir, ID);
  await manuallyInstall(do_get_file(`${DATA}/unsigned.xpi`), profileDir, ID);
||||||| merged common ancestors
function run_test() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "4", "4");

  run_next_test();
}

function verify_no_change([startFile, startState], [endFile, endState]) {
  add_task(async function() {
    info("A switch from " + startFile + " to " + endFile + " should cause no change.");

    // Install the first add-on
    await manuallyInstall(do_get_file(DATA + startFile), profileDir, ID);
    await promiseStartupManager();

    let addon = await promiseAddonByID(ID);
    Assert.notEqual(addon, null);
    let wasAppDisabled = addon.appDisabled;
    Assert.notEqual(addon.appDisabled, addon.isActive);
    Assert.equal(addon.pendingOperations, AddonManager.PENDING_NONE);
    Assert.equal(addon.signedState, startState);

    // Swap in the files from the next add-on
    manuallyUninstall(profileDir, ID);
    await manuallyInstall(do_get_file(DATA + endFile), profileDir, ID);

    let events = {
      [ID]: [],
    };

    if (startState != endState)
      events[ID].unshift(["onPropertyChanged", ["signedState"]]);

    prepare_test(events);

    // Trigger the check
    let changes = await verifySignatures();
    Assert.equal(changes.enabled.length, 0);
    Assert.equal(changes.disabled.length, 0);

    Assert.equal(addon.appDisabled, wasAppDisabled);
    Assert.notEqual(addon.appDisabled, addon.isActive);
    Assert.equal(addon.pendingOperations, AddonManager.PENDING_NONE);
    Assert.equal(addon.signedState, endState);

    // Remove the add-on and restart to let it go away
    manuallyUninstall(profileDir, ID);
    await promiseRestartManager();
    await promiseShutdownManager();
  });
}

function verify_disables([startFile, startState], [endFile, endState]) {
  add_task(async function() {
    info("A switch from " + startFile + " to " + endFile + " should disable the add-on.");

    // Install the first add-on
    await manuallyInstall(do_get_file(DATA + startFile), profileDir, ID);
    await promiseStartupManager();

    let addon = await promiseAddonByID(ID);
    Assert.notEqual(addon, null);
    Assert.ok(addon.isActive);
    Assert.equal(addon.pendingOperations, AddonManager.PENDING_NONE);
    Assert.equal(addon.signedState, startState);

    let needsRestart = hasFlag(addon.operationsRequiringRestart, AddonManager.OP_NEEDS_RESTART_DISABLE);

    // Swap in the files from the next add-on
    manuallyUninstall(profileDir, ID);
    await manuallyInstall(do_get_file(DATA + endFile), profileDir, ID);

    let events = {};
    if (!needsRestart) {
      events[ID] = [
        ["onPropertyChanged", ["appDisabled"]],
        ["onDisabling", false],
        "onDisabled",
      ];
    } else {
      events[ID] = [
        ["onPropertyChanged", ["appDisabled"]],
        "onDisabling",
      ];
    }

    if (startState != endState)
      events[ID].unshift(["onPropertyChanged", ["signedState"]]);

    prepare_test(events);

    // Trigger the check
    let changes = await verifySignatures();
    Assert.equal(changes.enabled.length, 0);
    Assert.equal(changes.disabled.length, 1);
    Assert.equal(changes.disabled[0], ID);

    Assert.ok(addon.appDisabled);
    if (needsRestart)
      Assert.notEqual(addon.pendingOperations, AddonManager.PENDING_NONE);
    else
      Assert.ok(!addon.isActive);
    Assert.equal(addon.signedState, endState);

    ensure_test_completed();

    // Remove the add-on and restart to let it go away
    manuallyUninstall(profileDir, ID);
    await promiseRestartManager();
    await promiseShutdownManager();
  });
}
=======
createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "4", "4");

add_task(async function test_no_change() {
  await promiseStartupManager();

  // Install the first add-on
  await promiseInstallFile(do_get_file(`${DATA}/signed1.xpi`));

  let addon = await promiseAddonByID(ID);
  Assert.notEqual(addon, null);
  Assert.equal(addon.appDisabled, false);
  Assert.equal(addon.isActive, true);
  Assert.equal(addon.pendingOperations, AddonManager.PENDING_NONE);
  Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_SIGNED);

  // Swap in the files from the next add-on
  manuallyUninstall(profileDir, ID);
  await manuallyInstall(do_get_file(`${DATA}/signed2.xpi`), profileDir, ID);

  let listener = {
    onPropetyChanged(_addon, properties) {
      Assert.ok(false, `Got unexpected onPropertyChanged for ${_addon.id}`);
    },
  };

  AddonManager.addAddonListener(listener);

  // Trigger the check
  let changes = await verifySignatures();
  Assert.equal(changes.enabled.length, 0);
  Assert.equal(changes.disabled.length, 0);

  Assert.equal(addon.appDisabled, false);
  Assert.equal(addon.isActive, true);
  Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_SIGNED);

  await addon.uninstall();
  AddonManager.removeAddonListener(listener);
});

add_task(async function test_diable() {
  // Install the first add-on
  await promiseInstallFile(do_get_file(`${DATA}/signed1.xpi`));

  let addon = await promiseAddonByID(ID);
  Assert.notEqual(addon, null);
  Assert.ok(addon.isActive);
  Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_SIGNED);

  // Swap in the files from the next add-on
  manuallyUninstall(profileDir, ID);
  await manuallyInstall(do_get_file(`${DATA}/unsigned.xpi`), profileDir, ID);

  let changedProperties = [];
  let listener = {
    onPropertyChanged(_, properties) {
      changedProperties.push(...properties);
    },
  };
  AddonManager.addAddonListener(listener);

  // Trigger the check
  let [changes] = await Promise.all([
    verifySignatures(),
    promiseAddonEvent("onDisabling"),
  ]);

  Assert.equal(changes.enabled.length, 0);
  Assert.equal(changes.disabled.length, 1);
  Assert.equal(changes.disabled[0], ID);

  Assert.deepEqual(
    changedProperties,
    ["signedState", "appDisabled"],
    "Got onPropertyChanged events for signedState and appDisabled"
  );
>>>>>>> upstream-releases

<<<<<<< HEAD
  let changedProperties = [];
  let listener = {
    onPropertyChanged(_, properties) {
      changedProperties.push(...properties);
    },
  };
  AddonManager.addAddonListener(listener);

  // Trigger the check
  let [changes] = await Promise.all([
    verifySignatures(),
    promiseAddonEvent("onDisabling"),
  ]);

  Assert.equal(changes.enabled.length, 0);
  Assert.equal(changes.disabled.length, 1);
  Assert.equal(changes.disabled[0], ID);

  Assert.deepEqual(changedProperties, ["signedState", "appDisabled"],
                   "Got onPropertyChanged events for signedState and appDisabled");

  Assert.ok(addon.appDisabled);
  Assert.ok(!addon.isActive);
  Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_MISSING);

  await addon.uninstall();
  AddonManager.removeAddonListener(listener);
});
||||||| merged common ancestors
for (let start of GOOD) {
  for (let end of BAD) {
    verify_disables(start, end);
  }
}
=======
  Assert.ok(addon.appDisabled);
  Assert.ok(!addon.isActive);
  Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_MISSING);
>>>>>>> upstream-releases

<<<<<<< HEAD
||||||| merged common ancestors
for (let start of GOOD) {
  for (let end of GOOD.filter(f => f != start)) {
    verify_no_change(start, end);
  }
}
=======
  await addon.uninstall();
  AddonManager.removeAddonListener(listener);
});
>>>>>>> upstream-releases
