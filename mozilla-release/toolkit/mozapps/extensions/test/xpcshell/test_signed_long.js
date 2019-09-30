gUseRealCertChecks = true;

const ID = "123456789012345678901234567890123456789012345678901@somewhere.com";

// Tests that signature verification works correctly on an extension with
// an ID that does not fit into a certificate CN field.
add_task(async function test_long_id() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1");
  await promiseStartupManager();

  Assert.ok(ID.length > 64, "ID is > 64 characters");

  await promiseInstallFile(do_get_file("data/signing_checks/long.xpi"));
  let addon = await promiseAddonByID(ID);

<<<<<<< HEAD
  Assert.notEqual(addon, null, "Addon install properly");
  Assert.ok(addon.signedState > AddonManager.SIGNEDSTATE_MISSING,
            "Signature verification worked properly");
||||||| merged common ancestors
// Checks the cases that should be broken
add_task(async function test_broken() {
  let promises = [AddonManager.getInstallForFile(do_get_file(DATA + "long_63_hash.xpi")),
                  AddonManager.getInstallForFile(do_get_file(DATA + "long_64_hash.xpi"))];
  let installs = await Promise.all(promises);
=======
  Assert.notEqual(addon, null, "Addon install properly");
  Assert.ok(
    addon.signedState > AddonManager.SIGNEDSTATE_MISSING,
    "Signature verification worked properly"
  );
>>>>>>> upstream-releases

  await addon.uninstall();
});
