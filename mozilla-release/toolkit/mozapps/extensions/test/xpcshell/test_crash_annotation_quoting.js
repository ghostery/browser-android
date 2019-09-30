/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

// This verifies that strange characters in an add-on version don't break the
// crash annotation.

createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1.9.2");

add_task(async function run_test() {
  await promiseStartupManager();

<<<<<<< HEAD
  let n = 1;
  for (let version in [ "1,0", "1:0" ]) {
    let id = `addon${n++}@tests.mozilla.org`;
    await promiseInstallWebExtension({
      manifest: {
        version,
        applications: {gecko: {id}},
      },
    });

    do_check_in_crash_annotation(id, version);
  }
||||||| merged common ancestors
  await promiseInstallXPI(addon3);
  await promiseInstallXPI(addon4);

  let [a3, a4] = await AddonManager.getAddonsByIDs(["addon3@tests.mozilla.org",
                                                    "addon4@tests.mozilla.org"]);

  Assert.notEqual(a3, null);
  do_check_in_crash_annotation(addon3.id, addon3.version);

  Assert.notEqual(a4, null);
  do_check_in_crash_annotation(addon4.id, addon4.version);
=======
  let n = 1;
  for (let version in ["1,0", "1:0"]) {
    let id = `addon${n++}@tests.mozilla.org`;
    await promiseInstallWebExtension({
      manifest: {
        version,
        applications: { gecko: { id } },
      },
    });

    do_check_in_crash_annotation(id, version);
  }
>>>>>>> upstream-releases
});
