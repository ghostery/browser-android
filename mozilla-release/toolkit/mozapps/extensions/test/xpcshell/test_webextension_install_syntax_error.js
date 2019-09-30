const ADDON_ID = "webext-test@tests.mozilla.org";

add_task(async function setup() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1");
  await promiseStartupManager();
});

add_task(async function install_xpi() {
<<<<<<< HEAD

  // WebExtension with a JSON syntax error in manifest.json
  let xpi1 = Extension.generateXPI({
||||||| merged common ancestors

  // Data for WebExtension with syntax error
  let xpi1 = Extension.generateXPI({
=======
  // WebExtension with a JSON syntax error in manifest.json
  let xpi1 = AddonTestUtils.createTempWebExtensionFile({
>>>>>>> upstream-releases
    files: {
      "manifest.json": String.raw`{
        "manifest_version: 2,
        "applications": {"gecko": {"id": "${ADDON_ID}"}},
        "name": "Temp WebExt with Error",
        "version": "0.1"
      }`,
    },
  });

<<<<<<< HEAD
  // Valid WebExtension
  let xpi2 = Extension.generateXPI({
||||||| merged common ancestors
  // Data for WebExtension without syntax error
  let xpi2 = Extension.generateXPI({
=======
  // Valid WebExtension
  let xpi2 = AddonTestUtils.createTempWebExtensionFile({
>>>>>>> upstream-releases
    files: {
      "manifest.json": String.raw`{
        "manifest_version": 2,
        "applications": {"gecko": {"id": "${ADDON_ID}"}},
        "name": "Temp WebExt without Error",
        "version": "0.1"
      }`,
    },
  });

  let install1 = await AddonManager.getInstallForFile(xpi1);
  Assert.equal(install1.state, AddonManager.STATE_DOWNLOAD_FAILED);
  Assert.equal(install1.error, AddonManager.ERROR_CORRUPT_FILE);

  // Replace xpi1 with xpi2 to have the same filename to reproduce install error
  xpi2.moveTo(xpi1.parent, xpi1.leafName);

  let install2 = await AddonManager.getInstallForFile(xpi2);
<<<<<<< HEAD
  Assert.equal(install2.error, 0);

  xpi1.remove(false);
||||||| merged common ancestors
  Assert.notEqual(install2.error, AddonManager.ERROR_CORRUPT_FILE);

  xpi1.remove(false);
=======
  Assert.equal(install2.error, 0);
>>>>>>> upstream-releases
});
