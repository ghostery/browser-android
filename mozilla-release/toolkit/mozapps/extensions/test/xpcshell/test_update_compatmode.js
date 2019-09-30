/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

// This verifies that add-on update check correctly fills in the
// %COMPATIBILITY_MODE% token in the update URL.

<<<<<<< HEAD
Cu.importGlobalProperties(["URLSearchParams"]);

||||||| merged common ancestors

=======
>>>>>>> upstream-releases
// The test extension uses an insecure update url.
Services.prefs.setBoolPref(PREF_EM_CHECK_UPDATE_SECURITY, false);

<<<<<<< HEAD
let testserver = createHttpServer({hosts: ["example.com"]});
||||||| merged common ancestors
var testserver = AddonTestUtils.createHttpServer({hosts: ["example.com"]});
testserver.registerDirectory("/data/", do_get_file("data"));
=======
let testserver = createHttpServer({ hosts: ["example.com"] });

let lastMode;
testserver.registerPathHandler("/update.json", (request, response) => {
  let params = new URLSearchParams(request.queryString);
  lastMode = params.get("mode");

  response.setHeader("content-type", "application/json", true);
  response.write(JSON.stringify({ addons: {} }));
});
>>>>>>> upstream-releases

<<<<<<< HEAD
let lastMode;
testserver.registerPathHandler("/update.json", (request, response) => {
  let params = new URLSearchParams(request.queryString);
  lastMode = params.get("mode");
||||||| merged common ancestors
const profileDir = gProfD.clone();
profileDir.append("extensions");
=======
const ID_NORMAL = "compatmode@tests.mozilla.org";
const ID_STRICT = "compatmode-strict@tests.mozilla.org";
>>>>>>> upstream-releases

<<<<<<< HEAD
  response.setHeader("content-type", "application/json", true);
  response.write(JSON.stringify({addons: {}}));
});

const ID_NORMAL = "compatmode@tests.mozilla.org";
const ID_STRICT = "compatmode-strict@tests.mozilla.org";

add_task(async function setup() {
||||||| merged common ancestors
async function run_test() {
  do_test_pending();
=======
add_task(async function setup() {
>>>>>>> upstream-releases
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1.9.2");

<<<<<<< HEAD
  let xpi = await createAddon({
    id: ID_NORMAL,
    updateURL: "http://example.com/update.json?mode=%COMPATIBILITY_MODE%",
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1",
    }],
  });
  await manuallyInstall(xpi, AddonTestUtils.profileExtensions, ID_NORMAL);

  xpi = await createAddon({
    id: ID_STRICT,
    updateURL: "http://example.com/update.json?mode=%COMPATIBILITY_MODE%",
    strictCompatibility: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1",
    }],
  });
  await manuallyInstall(xpi, AddonTestUtils.profileExtensions, ID_STRICT);
||||||| merged common ancestors
  await promiseWriteInstallRDFForExtension({
    id: "compatmode-normal@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/test_updatecompatmode_%COMPATIBILITY_MODE%.json",
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1",
    }],
    name: "Test Addon - normal",
  }, profileDir);

  await promiseWriteInstallRDFForExtension({
    id: "compatmode-strict@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/test_updatecompatmode_%COMPATIBILITY_MODE%.json",
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1",
    }],
    name: "Test Addon - strict",
  }, profileDir);

  await promiseWriteInstallRDFForExtension({
    id: "compatmode-strict-optin@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/test_updatecompatmode_%COMPATIBILITY_MODE%.json",
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1",
    }],
    name: "Test Addon - strict opt-in",
    strictCompatibility: true,
  }, profileDir);

  await promiseWriteInstallRDFForExtension({
    id: "compatmode-ignore@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/test_updatecompatmode_%COMPATIBILITY_MODE%.json",
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1",
    }],
    name: "Test Addon - ignore",
  }, profileDir);
=======
  let xpi = await createAddon({
    id: ID_NORMAL,
    updateURL: "http://example.com/update.json?mode=%COMPATIBILITY_MODE%",
    targetApplications: [
      {
        id: "xpcshell@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      },
    ],
  });
  await manuallyInstall(xpi, AddonTestUtils.profileExtensions, ID_NORMAL);

  xpi = await createAddon({
    id: ID_STRICT,
    updateURL: "http://example.com/update.json?mode=%COMPATIBILITY_MODE%",
    strictCompatibility: true,
    targetApplications: [
      {
        id: "xpcshell@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      },
    ],
  });
  await manuallyInstall(xpi, AddonTestUtils.profileExtensions, ID_STRICT);
>>>>>>> upstream-releases

  await promiseStartupManager();
});

// Strict compatibility checking disabled.
add_task(async function test_strict_disabled() {
  Services.prefs.setBoolPref(PREF_EM_STRICT_COMPATIBILITY, false);
  let addon = await AddonManager.getAddonByID(ID_NORMAL);
  Assert.notEqual(addon, null);

<<<<<<< HEAD
  await promiseFindAddonUpdates(addon, AddonManager.UPDATE_WHEN_USER_REQUESTED);
  Assert.equal(lastMode, "normal", "COMPATIBIILITY_MODE normal was set correctly");
});
||||||| merged common ancestors
    onUpdateAvailable(unused, install) {
      Assert.equal(install.version, "2.0");
    },

    onUpdateFinished() {
      run_test_2();
    },
  }, AddonManager.UPDATE_WHEN_USER_REQUESTED);
}
=======
  await promiseFindAddonUpdates(addon, AddonManager.UPDATE_WHEN_USER_REQUESTED);
  Assert.equal(
    lastMode,
    "normal",
    "COMPATIBIILITY_MODE normal was set correctly"
  );
});
>>>>>>> upstream-releases

// Strict compatibility checking enabled.
add_task(async function test_strict_enabled() {
  Services.prefs.setBoolPref(PREF_EM_STRICT_COMPATIBILITY, true);
  let addon = await AddonManager.getAddonByID(ID_NORMAL);
  Assert.notEqual(addon, null);

<<<<<<< HEAD
  await promiseFindAddonUpdates(addon, AddonManager.UPDATE_WHEN_USER_REQUESTED);
  Assert.equal(lastMode, "strict", "COMPATIBILITY_MODE strict was set correctly");
});
||||||| merged common ancestors
    onUpdateFinished() {
      run_test_3();
    },
  }, AddonManager.UPDATE_WHEN_USER_REQUESTED);
}
=======
  await promiseFindAddonUpdates(addon, AddonManager.UPDATE_WHEN_USER_REQUESTED);
  Assert.equal(
    lastMode,
    "strict",
    "COMPATIBILITY_MODE strict was set correctly"
  );
});
>>>>>>> upstream-releases

// Strict compatibility checking opt-in.
add_task(async function test_strict_optin() {
  Services.prefs.setBoolPref(PREF_EM_STRICT_COMPATIBILITY, false);
  let addon = await AddonManager.getAddonByID(ID_STRICT);
  Assert.notEqual(addon, null);

<<<<<<< HEAD
  await promiseFindAddonUpdates(addon, AddonManager.UPDATE_WHEN_USER_REQUESTED);
  Assert.equal(lastMode, "normal", "COMPATIBILITY_MODE is normal even for an addon with strictCompatibility");
});
||||||| merged common ancestors
    onUpdateAvailable() {
      do_throw("Should not have seen an available update");
    },

    onUpdateFinished() {
      run_test_4();
    },
  }, AddonManager.UPDATE_WHEN_USER_REQUESTED);
}
=======
  await promiseFindAddonUpdates(addon, AddonManager.UPDATE_WHEN_USER_REQUESTED);
  Assert.equal(
    lastMode,
    "normal",
    "COMPATIBILITY_MODE is normal even for an addon with strictCompatibility"
  );
});
>>>>>>> upstream-releases

// Compatibility checking disabled.
add_task(async function test_compat_disabled() {
  AddonManager.checkCompatibility = false;
  let addon = await AddonManager.getAddonByID(ID_NORMAL);
  Assert.notEqual(addon, null);
<<<<<<< HEAD

  await promiseFindAddonUpdates(addon, AddonManager.UPDATE_WHEN_USER_REQUESTED);
  Assert.equal(lastMode, "ignore", "COMPATIBILITY_MODE ignore was set correctly");
});
||||||| merged common ancestors
  addon.findUpdates({
    onCompatibilityUpdateAvailable() {
      do_throw("Should have not have seen compatibility information");
    },

    onNoUpdateAvailable() {
      do_throw("Should have seen an available update");
    },

    onUpdateAvailable(unused, install) {
      Assert.equal(install.version, "2.0");
    },

    onUpdateFinished() {
      end_test();
    },
  }, AddonManager.UPDATE_WHEN_USER_REQUESTED);
}
=======

  await promiseFindAddonUpdates(addon, AddonManager.UPDATE_WHEN_USER_REQUESTED);
  Assert.equal(
    lastMode,
    "ignore",
    "COMPATIBILITY_MODE ignore was set correctly"
  );
});
>>>>>>> upstream-releases
