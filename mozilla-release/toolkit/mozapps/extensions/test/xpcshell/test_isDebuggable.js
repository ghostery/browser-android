/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

var ID = "debuggable@tests.mozilla.org";

add_task(async function() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "2", "2");

  await promiseStartupManager();

<<<<<<< HEAD
  await promiseInstallWebExtension({
    manifest: {
      applications: {gecko: {id: ID}},
    },
  });
||||||| merged common ancestors
  await promiseInstallXPI(ADDON);
=======
  await promiseInstallWebExtension({
    manifest: {
      applications: { gecko: { id: ID } },
    },
  });
>>>>>>> upstream-releases

  let addon = await AddonManager.getAddonByID(ID);
  Assert.equal(addon.isDebuggable, true);
});
