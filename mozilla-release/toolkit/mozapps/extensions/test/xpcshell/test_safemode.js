/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

// Tests that extensions behave correctly in safe mode
<<<<<<< HEAD

||||||| merged common ancestors

var addon1 = {
  id: "addon1@tests.mozilla.org",
  version: "1.0",
  name: "Test 1",
  optionsURL: "chrome://foo/content/options.xul",
  aboutURL: "chrome://foo/content/about.xul",
  iconURL: "chrome://foo/content/icon.png",
  bootstrap: true,
  targetApplications: [{
    id: "xpcshell@tests.mozilla.org",
    minVersion: "1",
    maxVersion: "1",
  }],
};

=======
let scopes = AddonManager.SCOPE_PROFILE | AddonManager.SCOPE_APPLICATION;
Services.prefs.setIntPref("extensions.enabledScopes", scopes);

>>>>>>> upstream-releases
const profileDir = gProfD.clone();
profileDir.append("extensions");

<<<<<<< HEAD
const ID = "addon1@tests.mozilla.org";
const VERSION = "1.0";
||||||| merged common ancestors
var gIconURL = null;
=======
const ID = "addon1@tests.mozilla.org";
const BUILTIN_ID = "builtin@tests.mozilla.org";
const VERSION = "1.0";
>>>>>>> upstream-releases

// Sets up the profile by installing an add-on.
<<<<<<< HEAD
add_task(async function setup() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1.9.2");
||||||| merged common ancestors
async function run_test() {
  do_test_pending();
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1.9.2");
=======
add_task(async function setup() {
  AddonTestUtils.createAppInfo(
    "xpcshell@tests.mozilla.org",
    "XPCShell",
    "1",
    "1.9.2"
  );
>>>>>>> upstream-releases
  gAppInfo.inSafeMode = true;

  await promiseStartupManager();

  let a1 = await AddonManager.getAddonByID(ID);
  Assert.equal(a1, null);
<<<<<<< HEAD
  do_check_not_in_crash_annotation(ID, VERSION);

  await promiseInstallWebExtension({
    manifest: {
      name: "Test 1",
      version: VERSION,
      applications: {gecko: {id: ID}},
    },
  });
||||||| merged common ancestors
  do_check_not_in_crash_annotation(addon1.id, addon1.version);

  await promiseWriteInstallRDFForExtension(addon1, profileDir, addon1.id, "icon.png");
  gIconURL = do_get_addon_root_uri(profileDir.clone(), addon1.id) + "icon.png";
=======
  do_check_not_in_crash_annotation(ID, VERSION);

  await promiseInstallWebExtension({
    manifest: {
      name: "Test 1",
      version: VERSION,
      applications: { gecko: { id: ID } },
    },
  });
  let wrapper = await installBuiltinExtension({
    manifest: {
      applications: { gecko: { id: BUILTIN_ID } },
    },
  });

  let builtin = await AddonManager.getAddonByID(BUILTIN_ID);
  Assert.notEqual(builtin, null, "builtin extension is installed");
>>>>>>> upstream-releases

  await promiseRestartManager();

<<<<<<< HEAD
  a1 = await AddonManager.getAddonByID(ID);
  Assert.notEqual(a1, null);
  Assert.ok(!a1.isActive);
  Assert.ok(!a1.userDisabled);
  Assert.ok(isExtensionInBootstrappedList(profileDir, ID));
  Assert.ok(hasFlag(a1.permissions, AddonManager.PERM_CAN_DISABLE));
  Assert.ok(!hasFlag(a1.permissions, AddonManager.PERM_CAN_ENABLE));
  do_check_not_in_crash_annotation(ID, VERSION);
});
||||||| merged common ancestors
  let newa1 = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  Assert.notEqual(newa1, null);
  Assert.ok(!newa1.isActive);
  Assert.ok(!newa1.userDisabled);
  Assert.equal(newa1.aboutURL, null);
  Assert.equal(newa1.optionsURL, null);
  Assert.equal(newa1.iconURL, gIconURL);
  Assert.ok(isExtensionInBootstrappedList(profileDir, newa1.id));
  Assert.ok(hasFlag(newa1.permissions, AddonManager.PERM_CAN_DISABLE));
  Assert.ok(!hasFlag(newa1.permissions, AddonManager.PERM_CAN_ENABLE));
  Assert.equal(newa1.operationsRequiringRestart, AddonManager.OP_NEEDS_RESTART_NONE);
  do_check_not_in_crash_annotation(addon1.id, addon1.version);

  run_test_1();
}
=======
  a1 = await AddonManager.getAddonByID(ID);
  Assert.notEqual(a1, null);
  Assert.ok(!a1.isActive);
  Assert.ok(!a1.userDisabled);
  Assert.ok(isExtensionInBootstrappedList(profileDir, ID));
  Assert.ok(hasFlag(a1.permissions, AddonManager.PERM_CAN_DISABLE));
  Assert.ok(!hasFlag(a1.permissions, AddonManager.PERM_CAN_ENABLE));
  do_check_not_in_crash_annotation(ID, VERSION);

  builtin = await AddonManager.getAddonByID(BUILTIN_ID);
  Assert.notEqual(builtin, null, "builtin extension is installed");
  Assert.ok(builtin.isActive, "builtin extension is active");
  await wrapper.unload();
});
>>>>>>> upstream-releases

// Disabling an add-on should work
<<<<<<< HEAD
add_task(async function test_disable() {
  let a1 = await AddonManager.getAddonByID(ID);
  Assert.ok(!hasFlag(a1.operationsRequiringRestart,
                     AddonManager.OP_NEEDS_RESTART_DISABLE));
||||||| merged common ancestors
async function run_test_1() {
  prepare_test({
    "addon1@tests.mozilla.org": [
      ["onDisabling", false],
      "onDisabled",
    ],
  });

  let a1 = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  Assert.ok(!hasFlag(a1.operationsRequiringRestart,
                     AddonManager.OP_NEEDS_RESTART_DISABLE));
=======
add_task(async function test_disable() {
  let a1 = await AddonManager.getAddonByID(ID);
  Assert.ok(
    !hasFlag(
      a1.operationsRequiringRestart,
      AddonManager.OP_NEEDS_RESTART_DISABLE
    )
  );
>>>>>>> upstream-releases
  await a1.disable();
  Assert.ok(!a1.isActive);
  Assert.ok(!hasFlag(a1.permissions, AddonManager.PERM_CAN_DISABLE));
  Assert.ok(hasFlag(a1.permissions, AddonManager.PERM_CAN_ENABLE));
  do_check_not_in_crash_annotation(ID, VERSION);
});

// Enabling an add-on should happen but not become active.
add_task(async function test_enable() {
  let a1 = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  await a1.enable();
  Assert.ok(!a1.isActive);
  Assert.ok(hasFlag(a1.permissions, AddonManager.PERM_CAN_DISABLE));
  Assert.ok(!hasFlag(a1.permissions, AddonManager.PERM_CAN_ENABLE));

  do_check_not_in_crash_annotation(ID, VERSION);
});
