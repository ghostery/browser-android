/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

// Tests that various operations with file pointers work and do not affect the
// source files

const ID1 = "addon1@tests.mozilla.org";
const ID2 = "addon2@tests.mozilla.org";

const profileDir = gProfD.clone();
profileDir.append("extensions");
profileDir.create(Ci.nsIFile.DIRECTORY_TYPE, 0o755);

const sourceDir = gProfD.clone();
sourceDir.append("source");

function promiseWriteWebExtension(path, data) {
  let files = ExtensionTestCommon.generateFiles(data);
  return AddonTestUtils.promiseWriteFilesToDir(path, files);
}

function promiseWritePointer(aId, aName) {
  let path = OS.Path.join(profileDir.path, aName || aId);

  let target = OS.Path.join(sourceDir.path, do_get_expected_addon_name(aId));

  return OS.File.writeAtomic(path, new TextEncoder().encode(target));
}

function promiseWriteRelativePointer(aId, aName) {
  let path = OS.Path.join(profileDir.path, aName || aId);

  let absTarget = sourceDir.clone();
  absTarget.append(do_get_expected_addon_name(aId));

  let relTarget = absTarget.getRelativeDescriptor(profileDir);

  return OS.File.writeAtomic(path, new TextEncoder().encode(relTarget));
}

add_task(async function setup() {
  ok(TEST_UNPACKED, "Pointer files only work with unpacked directories");

  // Unpacked extensions are never signed, so this can only run with
  // signature checks disabled.
  Services.prefs.setBoolPref(PREF_XPI_SIGNATURES_REQUIRED, false);

  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1");
});

// Tests that installing a new add-on by pointer works
add_task(async function test_new_pointer_install() {
<<<<<<< HEAD
  let target = OS.Path.join(sourceDir.path, ID1);
  await promiseWriteWebExtension(target, {
    manifest: {
      version: "1.0",
      applications: {gecko: {id: ID1}},
    },
  });
  await promiseWritePointer(ID1);
||||||| merged common ancestors
  await promiseWriteInstallRDFForExtension(addon1, sourceDir);
  await promiseWritePointer(addon1.id);
=======
  let target = OS.Path.join(sourceDir.path, ID1);
  await promiseWriteWebExtension(target, {
    manifest: {
      version: "1.0",
      applications: { gecko: { id: ID1 } },
    },
  });
  await promiseWritePointer(ID1);
>>>>>>> upstream-releases
  await promiseStartupManager();

  let addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "1.0");

  let file = getAddonFile(addon);
  equal(file.parent.path, sourceDir.path);

<<<<<<< HEAD
  let rootUri = do_get_addon_root_uri(sourceDir, ID1);
  let uri = addon.getResourceURI("/");
||||||| merged common ancestors
  let rootUri = do_get_addon_root_uri(sourceDir, addon1.id);
  let uri = addon.getResourceURI("/");
=======
  let rootUri = do_get_addon_root_uri(sourceDir, ID1);
  let uri = addon.getResourceURI();
>>>>>>> upstream-releases
  equal(uri.spec, rootUri);

  // Check that upgrade is disabled for addons installed by file-pointers.
  equal(addon.permissions & AddonManager.PERM_CAN_UPGRADE, 0);
});

// Tests that installing the addon from some other source doesn't clobber
// the original sources
add_task(async function test_addon_over_pointer() {
  let xpi = AddonTestUtils.createTempWebExtensionFile({
    manifest: {
      version: "2.0",
<<<<<<< HEAD
      applications: {gecko: {id: ID1}},
||||||| merged common ancestors
      name: "File Pointer Test",
      bootstrap: true,

      targetApplications: [{
          id: "xpcshell@tests.mozilla.org",
          minVersion: "1",
          maxVersion: "1"}],
=======
      applications: { gecko: { id: ID1 } },
>>>>>>> upstream-releases
    },
  });

<<<<<<< HEAD
  let install = await AddonManager.getInstallForFile(xpi, "application/x-xpinstall");
  await install.install();
||||||| merged common ancestors
  testserver.registerFile("/addons/test_filepointer.xpi", xpi);

  let url = "http://example.com/addons/test_filepointer.xpi";
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await new Promise(resolve => {
    ensure_test_completed();

    prepare_test({
      "addon1@tests.mozilla.org": [
        ["onInstalling", false],
        ["onInstalled", false],
      ],
    }, [
      "onDownloadStarted",
      "onDownloadEnded",
      "onInstallStarted",
      "onInstallEnded",
    ], callback_soon(resolve));

    install.install();
  });

  await promiseRestartManager();
=======
  let install = await AddonManager.getInstallForFile(
    xpi,
    "application/x-xpinstall"
  );
  await install.install();
>>>>>>> upstream-releases

  let addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "2.0");

  let url = addon.getResourceURI();
  if (url instanceof Ci.nsIJARURI) {
    url = url.JARFile;
  }
  let { file } = url.QueryInterface(Ci.nsIFileURL);
  equal(file.parent.path, profileDir.path);

<<<<<<< HEAD
  let rootUri = do_get_addon_root_uri(profileDir, ID1);
  let uri = addon.getResourceURI("/");
||||||| merged common ancestors
  let rootUri = do_get_addon_root_uri(profileDir, addon1.id);
  let uri = addon.getResourceURI("/");
=======
  let rootUri = do_get_addon_root_uri(profileDir, ID1);
  let uri = addon.getResourceURI();
>>>>>>> upstream-releases
  equal(uri.spec, rootUri);

  let source = sourceDir.clone();
  source.append(ID1);
  ok(source.exists());

  await addon.uninstall();
});

// Tests that uninstalling doesn't clobber the original sources
add_task(async function test_uninstall_pointer() {
  await promiseWritePointer(ID1);
  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "1.0");

  await addon.uninstall();

  let source = sourceDir.clone();
  source.append(ID1);
  ok(source.exists());
});

// Tests that misnaming a pointer doesn't clobber the sources
add_task(async function test_bad_pointer() {
  await promiseWritePointer(ID2, ID1);

  let [a1, a2] = await AddonManager.getAddonsByIDs([ID1, ID2]);
  equal(a1, null);
  equal(a2, null);

  let source = sourceDir.clone();
  source.append(ID1);
  ok(source.exists());

  let pointer = profileDir.clone();
  pointer.append(ID2);
  ok(!pointer.exists());
});

// Tests that changing the ID of an existing add-on doesn't clobber the sources
add_task(async function test_bad_pointer_id() {
  let dir = sourceDir.clone();
  dir.append(ID1);

  // Make sure the modification time changes enough to be detected.
  setExtensionModifiedTime(dir, dir.lastModifiedTime - 5000);
  await promiseWritePointer(ID1);
  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "1.0");

<<<<<<< HEAD
  await promiseWriteWebExtension(dir.path, {
    manifest: {
      version: "1.0",
      applications: {gecko: {id: ID2}},
    },
  });
  setExtensionModifiedTime(dir, dir.lastModifiedTime - 5000);
||||||| merged common ancestors
  await promiseWriteInstallRDFForExtension(addon2, sourceDir, addon1.id);
  setExtensionModifiedTime(dest, dest.lastModifiedTime - 5000);
=======
  await promiseWriteWebExtension(dir.path, {
    manifest: {
      version: "1.0",
      applications: { gecko: { id: ID2 } },
    },
  });
  setExtensionModifiedTime(dir, dir.lastModifiedTime - 5000);
>>>>>>> upstream-releases

  await promiseRestartManager();

  let [a1, a2] = await AddonManager.getAddonsByIDs([ID1, ID2]);
  equal(a1, null);
  equal(a2, null);

  let source = sourceDir.clone();
  source.append(ID1);
  ok(source.exists());

  let pointer = profileDir.clone();
  pointer.append(ID1);
  ok(!pointer.exists());
});

// Removing the pointer file should uninstall the add-on
add_task(async function test_remove_pointer() {
<<<<<<< HEAD
  let dir = sourceDir.clone();
  dir.append(ID1);

  await promiseWriteWebExtension(dir.path, {
    manifest: {
      version: "1.0",
      applications: {gecko: {id: ID1}},
    },
  });

  setExtensionModifiedTime(dir, dir.lastModifiedTime - 5000);
  await promiseWritePointer(ID1);
||||||| merged common ancestors
  var dest = await promiseWriteInstallRDFForExtension(addon1, sourceDir);
  // Make sure the modification time changes enough to be detected in run_test_8.
  setExtensionModifiedTime(dest, dest.lastModifiedTime - 5000);
  await promiseWritePointer(addon1.id);
=======
  let dir = sourceDir.clone();
  dir.append(ID1);

  await promiseWriteWebExtension(dir.path, {
    manifest: {
      version: "1.0",
      applications: { gecko: { id: ID1 } },
    },
  });

  setExtensionModifiedTime(dir, dir.lastModifiedTime - 5000);
  await promiseWritePointer(ID1);
>>>>>>> upstream-releases

  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "1.0");

  let pointer = profileDir.clone();
  pointer.append(ID1);
  pointer.remove(false);

  await promiseRestartManager();

  addon = await AddonManager.getAddonByID(ID1);
  equal(addon, null);
});

// Removing the pointer file and replacing it with a directory should work
add_task(async function test_replace_pointer() {
  await promiseWritePointer(ID1);
  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "1.0");

  let pointer = profileDir.clone();
  pointer.append(ID1);
  pointer.remove(false);

<<<<<<< HEAD
  await promiseWriteWebExtension(OS.Path.join(profileDir.path, ID1), {
    manifest: {
      version: "2.0",
      applications: {gecko: {id: ID1}},
    },
  });
||||||| merged common ancestors
  await promiseWriteInstallRDFForExtension(addon1_2, profileDir);
=======
  await promiseWriteWebExtension(OS.Path.join(profileDir.path, ID1), {
    manifest: {
      version: "2.0",
      applications: { gecko: { id: ID1 } },
    },
  });
>>>>>>> upstream-releases

  await promiseRestartManager();

  addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "2.0");

  await addon.uninstall();
});

// Changes to the source files should be detected
add_task(async function test_change_pointer_sources() {
  await promiseWritePointer(ID1);
  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "1.0");

<<<<<<< HEAD
  let dir = sourceDir.clone();
  dir.append(ID1);
  await promiseWriteWebExtension(dir.path, {
    manifest: {
      version: "2.0",
      applications: {gecko: {id: ID1}},
    },
  });
  setExtensionModifiedTime(dir, dir.lastModifiedTime - 5000);
||||||| merged common ancestors
  let dest = await promiseWriteInstallRDFForExtension(addon1_2, sourceDir);
  setExtensionModifiedTime(dest, dest.lastModifiedTime - 5000);
=======
  let dir = sourceDir.clone();
  dir.append(ID1);
  await promiseWriteWebExtension(dir.path, {
    manifest: {
      version: "2.0",
      applications: { gecko: { id: ID1 } },
    },
  });
  setExtensionModifiedTime(dir, dir.lastModifiedTime - 5000);
>>>>>>> upstream-releases

  await promiseRestartManager();

  addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "2.0");

  await addon.uninstall();
});

// Removing the add-on the pointer file points at should uninstall the add-on
add_task(async function test_remove_pointer_target() {
<<<<<<< HEAD
  let target = OS.Path.join(sourceDir.path, ID1);
  await promiseWriteWebExtension(target, {
    manifest: {
      version: "1.0",
      applications: {gecko: {id: ID1}},
    },
  });
  await promiseWritePointer(ID1);
||||||| merged common ancestors
  await promiseRestartManager();
  var dest = await promiseWriteInstallRDFForExtension(addon1, sourceDir);
  await promiseWritePointer(addon1.id);
=======
  let target = OS.Path.join(sourceDir.path, ID1);
  await promiseWriteWebExtension(target, {
    manifest: {
      version: "1.0",
      applications: { gecko: { id: ID1 } },
    },
  });
  await promiseWritePointer(ID1);
>>>>>>> upstream-releases
  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID(ID1);
  notEqual(addon, null);
  equal(addon.version, "1.0");

  await OS.File.removeDir(target);

  await promiseRestartManager();

  addon = await AddonManager.getAddonByID(ID1);
  equal(addon, null);

  let pointer = profileDir.clone();
  pointer.append(ID1);
  ok(!pointer.exists());
});


// Tests that installing a new add-on by pointer with a relative path works
add_task(async function test_new_relative_pointer() {
<<<<<<< HEAD
  let target = OS.Path.join(sourceDir.path, ID1);
  await promiseWriteWebExtension(target, {
    manifest: {
      version: "1.0",
      applications: {gecko: {id: ID1}},
    },
  });
  await promiseWriteRelativePointer(ID1);
||||||| merged common ancestors
  await promiseWriteInstallRDFForExtension(addon1, sourceDir);
  await promiseWriteRelativePointer(addon1.id);
=======
  let target = OS.Path.join(sourceDir.path, ID1);
  await promiseWriteWebExtension(target, {
    manifest: {
      version: "1.0",
      applications: { gecko: { id: ID1 } },
    },
  });
  await promiseWriteRelativePointer(ID1);
>>>>>>> upstream-releases
  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID(ID1);
  equal(addon.version, "1.0");

  let { file } = addon.getResourceURI().QueryInterface(Ci.nsIFileURL);
  equal(file.parent.path, sourceDir.path);

<<<<<<< HEAD
  let rootUri = do_get_addon_root_uri(sourceDir, ID1);
  let uri = addon.getResourceURI("/");
||||||| merged common ancestors
  let rootUri = do_get_addon_root_uri(sourceDir, addon1.id);
  let uri = addon.getResourceURI("/");
=======
  let rootUri = do_get_addon_root_uri(sourceDir, ID1);
  let uri = addon.getResourceURI();
>>>>>>> upstream-releases
  equal(uri.spec, rootUri);

  // Check that upgrade is disabled for addons installed by file-pointers.
  equal(addon.permissions & AddonManager.PERM_CAN_UPGRADE, 0);
});
