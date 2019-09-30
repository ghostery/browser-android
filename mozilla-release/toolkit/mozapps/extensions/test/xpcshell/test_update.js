/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

// This verifies that add-on update checks work

// The test extension uses an insecure update url.
Services.prefs.setBoolPref(PREF_EM_CHECK_UPDATE_SECURITY, false);
<<<<<<< HEAD

// This test requires lightweight themes update to be enabled even if the app
// doesn't support lightweight themes.
Services.prefs.setBoolPref("lightweightThemes.update.enabled", true);

ChromeUtils.import("resource://gre/modules/LightweightThemeManager.jsm");

Cu.importGlobalProperties(["URLSearchParams"]);

var gInstallDate;
||||||| merged common ancestors
Services.prefs.setBoolPref(PREF_EM_STRICT_COMPATIBILITY, false);
// This test requires lightweight themes update to be enabled even if the app
// doesn't support lightweight themes.
Services.prefs.setBoolPref("lightweightThemes.update.enabled", true);

ChromeUtils.import("resource://gre/modules/LightweightThemeManager.jsm");

Cu.importGlobalProperties(["URLSearchParams"]);

var gInstallDate;
=======
>>>>>>> upstream-releases

const updateFile = "test_update.json";

const profileDir = gProfD.clone();
profileDir.append("extensions");

const ADDONS = {
  test_update: {
    id: "addon1@tests.mozilla.org",
    version: "2.0",
    name: "Test 1",
  },
  test_update8: {
    id: "addon8@tests.mozilla.org",
    version: "2.0",
    name: "Test 8",
  },
  test_update12: {
    id: "addon12@tests.mozilla.org",
    version: "2.0",
    name: "Test 12",
  },
  test_install2_1: {
    id: "addon2@tests.mozilla.org",
    version: "2.0",
    name: "Real Test 2",
  },
  test_install2_2: {
    id: "addon2@tests.mozilla.org",
    version: "3.0",
    name: "Real Test 3",
  },
};

<<<<<<< HEAD
var testserver = createHttpServer({hosts: ["example.com"]});
||||||| merged common ancestors
var testserver = AddonTestUtils.createHttpServer({hosts: ["example.com"]});
=======
var testserver = createHttpServer({ hosts: ["example.com"] });
>>>>>>> upstream-releases
testserver.registerDirectory("/data/", do_get_file("data"));

const XPIS = {};

add_task(async function setup() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1");

  Services.locale.requestedLocales = ["fr-FR"];

<<<<<<< HEAD
  for (let [name, info] of Object.entries(ADDONS)) {
    XPIS[name] = createTempWebExtensionFile({
      manifest: {
        name: info.name,
        version: info.version,
        applications: {gecko: {id: info.id}},
      },
    });
    testserver.registerFile(`/addons/${name}.xpi`, XPIS[name]);
  }

||||||| merged common ancestors
add_task(async function() {
=======
  for (let [name, info] of Object.entries(ADDONS)) {
    XPIS[name] = createTempWebExtensionFile({
      manifest: {
        name: info.name,
        version: info.version,
        applications: { gecko: { id: info.id } },
      },
    });
    testserver.registerFile(`/addons/${name}.xpi`, XPIS[name]);
  }

>>>>>>> upstream-releases
  AddonTestUtils.updateReason = AddonManager.UPDATE_WHEN_USER_REQUESTED;

  await promiseStartupManager();
});

// Verify that an update is available and can be installed.
add_task(async function test_apply_update() {
  await promiseInstallWebExtension({
    manifest: {
      name: "Test Addon 1",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon1@tests.mozilla.org",
          update_url: `http://example.com/data/${updateFile}`,
        },
      },
    },
  });

  let a1 = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  notEqual(a1, null);
  equal(a1.version, "1.0");
  equal(a1.applyBackgroundUpdates, AddonManager.AUTOUPDATE_DEFAULT);
  equal(a1.releaseNotesURI, null);
  notEqual(a1.syncGUID, null);

<<<<<<< HEAD
  let originalSyncGUID = a1.syncGUID;

  prepare_test({
    "addon1@tests.mozilla.org": [
      ["onPropertyChanged", ["applyBackgroundUpdates"]],
    ],
  });
  a1.applyBackgroundUpdates = AddonManager.AUTOUPDATE_DISABLE;
  check_test_completed();
||||||| merged common ancestors
  originalSyncGUID = a1.syncGUID;
  a1.applyBackgroundUpdates = AddonManager.AUTOUPDATE_DEFAULT;

  prepare_test({
    "addon1@tests.mozilla.org": [
      ["onPropertyChanged", ["applyBackgroundUpdates"]],
    ],
  });
  a1.applyBackgroundUpdates = AddonManager.AUTOUPDATE_DISABLE;
  check_test_completed();
=======
  let originalSyncGUID = a1.syncGUID;

  await expectEvents(
    {
      addonEvents: {
        "addon1@tests.mozilla.org": [
          {
            event: "onPropertyChanged",
            properties: ["applyBackgroundUpdates"],
          },
        ],
      },
    },
    async () => {
      a1.applyBackgroundUpdates = AddonManager.AUTOUPDATE_DISABLE;
    }
  );
>>>>>>> upstream-releases

  a1.applyBackgroundUpdates = AddonManager.AUTOUPDATE_DISABLE;

  let install;
  await expectEvents(
    {
      installEvents: [{ event: "onNewInstall" }],
    },
    async () => {
      ({
        updateAvailable: install,
      } = await AddonTestUtils.promiseFindAddonUpdates(a1));
    }
  );

  let installs = await AddonManager.getAllInstalls();
  equal(installs.length, 1);
  equal(installs[0], install);

  equal(install.name, a1.name);
  equal(install.version, "2.0");
  equal(install.state, AddonManager.STATE_AVAILABLE);
  equal(install.existingAddon, a1);
  equal(install.releaseNotesURI.spec, "http://example.com/updateInfo.xhtml");

  // Verify that another update check returns the same AddonInstall
  let {
    updateAvailable: install2,
  } = await AddonTestUtils.promiseFindAddonUpdates(a1);

  installs = await AddonManager.getAllInstalls();
  equal(installs.length, 1);
  equal(installs[0], install);
  equal(install2, install);

  await expectEvents(
    {
      installEvents: [
        { event: "onDownloadStarted" },
        { event: "onDownloadEnded", returnValue: false },
      ],
    },
    () => {
      install.install();
    }
  );

  equal(install.state, AddonManager.STATE_DOWNLOADED);

  // Continue installing the update.
  // Verify that another update check returns no new update
  let { updateAvailable } = await AddonTestUtils.promiseFindAddonUpdates(
    install.existingAddon
  );

  ok(
    !updateAvailable,
    "Should find no available updates when one is already downloading"
  );

  installs = await AddonManager.getAllInstalls();
  equal(installs.length, 1);
  equal(installs[0], install);

  await expectEvents(
    {
      addonEvents: {
        "addon1@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
      ],
    },
    () => {
      install.install();
    }
  );

  await AddonTestUtils.loadAddonsList(true);

  // Grab the current time so we can check the mtime of the add-on below
  // without worrying too much about how long other tests take.
  let startupTime = Date.now();

  ok(isExtensionInBootstrappedList(profileDir, "addon1@tests.mozilla.org"));

  a1 = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  notEqual(a1, null);
  equal(a1.version, "2.0");
  ok(isExtensionInBootstrappedList(profileDir, a1.id));
  equal(a1.applyBackgroundUpdates, AddonManager.AUTOUPDATE_DISABLE);
  equal(a1.releaseNotesURI.spec, "http://example.com/updateInfo.xhtml");
  notEqual(a1.syncGUID, null);
  equal(originalSyncGUID, a1.syncGUID);

  // Make sure that the extension lastModifiedTime was updated.
<<<<<<< HEAD
  let testURI = a1.getResourceURI("");
  let testFile = testURI.QueryInterface(Ci.nsIFileURL).file;
||||||| merged common ancestors
  let testURI = a1.getResourceURI(TEST_UNPACKED ? "install.rdf" : "");
  let testFile = testURI.QueryInterface(Ci.nsIFileURL).file;
=======
  let testFile = getAddonFile(a1);
>>>>>>> upstream-releases
  let difference = testFile.lastModifiedTime - startupTime;
  ok(Math.abs(difference) < MAX_TIME_DIFFERENCE);

<<<<<<< HEAD
  clearListeners();
||||||| merged common ancestors
  end_test();
=======
>>>>>>> upstream-releases
  await a1.uninstall();
});

// Check that an update check finds compatibility updates and applies them
add_task(async function test_compat_update() {
  await promiseInstallWebExtension({
    manifest: {
      name: "Test Addon 2",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon2@tests.mozilla.org",
          update_url: "http://example.com/data/" + updateFile,
          strict_max_version: "0",
        },
      },
    },
  });

  let a2 = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  notEqual(a2, null);
  ok(a2.isActive);
  ok(a2.isCompatible);
  ok(!a2.appDisabled);
  ok(a2.isCompatibleWith("0", "0"));

  let result = await AddonTestUtils.promiseFindAddonUpdates(a2);
  ok(result.compatibilityUpdate, "Should have seen a compatibility update");
  ok(!result.updateAvailable, "Should not have seen a version update");

  ok(a2.isCompatible);
  ok(!a2.appDisabled);
  ok(a2.isActive);

  await promiseRestartManager();

  a2 = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  notEqual(a2, null);
  ok(a2.isActive);
  ok(a2.isCompatible);
  ok(!a2.appDisabled);
  await a2.uninstall();
});

// Checks that we see no compatibility information when there is none.
add_task(async function test_no_compat() {
  gAppInfo.platformVersion = "5";
  await promiseRestartManager("5");
  await promiseInstallWebExtension({
    manifest: {
      name: "Test Addon 3",
      applications: {
        gecko: {
          id: "addon3@tests.mozilla.org",
          update_url: `http://example.com/data/${updateFile}`,
          strict_min_version: "5",
        },
      },
    },
  });

  gAppInfo.platformVersion = "1";
  await promiseRestartManager("1");

  let a3 = await AddonManager.getAddonByID("addon3@tests.mozilla.org");
  notEqual(a3, null);
  ok(!a3.isActive);
  ok(!a3.isCompatible);
  ok(a3.appDisabled);
  ok(a3.isCompatibleWith("5", "5"));
  ok(!a3.isCompatibleWith("2", "2"));

  let result = await AddonTestUtils.promiseFindAddonUpdates(a3);
  ok(
    !result.compatibilityUpdate,
    "Should not have seen a compatibility update"
  );
  ok(!result.updateAvailable, "Should not have seen a version update");
});

// Checks that compatibility info for future apps are detected but don't make
// the item compatibile.
add_task(async function test_future_compat() {
  let a3 = await AddonManager.getAddonByID("addon3@tests.mozilla.org");
  notEqual(a3, null);
  ok(!a3.isActive);
  ok(!a3.isCompatible);
  ok(a3.appDisabled);
  ok(a3.isCompatibleWith("5", "5"));
  ok(!a3.isCompatibleWith("2", "2"));

  let result = await AddonTestUtils.promiseFindAddonUpdates(
    a3,
    undefined,
    "3.0",
    "3.0"
  );
  ok(result.compatibilityUpdate, "Should have seen a compatibility update");
  ok(!result.updateAvailable, "Should not have seen a version update");

  ok(!a3.isActive);
  ok(!a3.isCompatible);
  ok(a3.appDisabled);

  await promiseRestartManager();

  a3 = await AddonManager.getAddonByID("addon3@tests.mozilla.org");
  notEqual(a3, null);
  ok(!a3.isActive);
  ok(!a3.isCompatible);
  ok(a3.appDisabled);

  await a3.uninstall();
});

// Test that background update checks work
<<<<<<< HEAD
add_task(async function test_background_update() {
  await promiseInstallWebExtension({
    manifest: {
      name: "Test Addon 1",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon1@tests.mozilla.org",
          update_url: `http://example.com/data/${updateFile}`,
          strict_min_version: "1",
          strict_max_version: "1",
        },
      },
    },
  });

  let install = await new Promise(resolve => {
    prepare_test({}, [
      "onNewInstall",
      "onDownloadStarted",
      "onDownloadEnded",
    ], resolve);

    AddonManagerInternal.backgroundUpdateCheck();
||||||| merged common ancestors
add_task(async function test_6() {
  await promiseInstallXPI({
    id: "addon1@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/" + updateFile,
    targetApplications: [{
      id: appId,
      minVersion: "1",
      maxVersion: "1",
    }],
    name: "Test Addon 1",
  });

  let install = await new Promise(resolve => {
    prepare_test({}, [
      "onNewInstall",
      "onDownloadStarted",
      "onDownloadEnded",
    ], resolve);

    AddonManagerInternal.backgroundUpdateCheck();
=======
add_task(async function test_background_update() {
  await promiseInstallWebExtension({
    manifest: {
      name: "Test Addon 1",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon1@tests.mozilla.org",
          update_url: `http://example.com/data/${updateFile}`,
          strict_min_version: "1",
          strict_max_version: "1",
        },
      },
    },
>>>>>>> upstream-releases
  });

  function checkInstall(install) {
    notEqual(install.existingAddon, null);
    equal(install.existingAddon.id, "addon1@tests.mozilla.org");
  }

  await expectEvents(
    {
      addonEvents: {
        "addon1@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onNewInstall" },
        { event: "onDownloadStarted" },
        { event: "onDownloadEnded", callback: checkInstall },
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
      ],
    },
    () => {
      AddonManagerInternal.backgroundUpdateCheck();
    }
  );

  let a1 = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  notEqual(a1, null);
  equal(a1.version, "2.0");
  equal(a1.releaseNotesURI.spec, "http://example.com/updateInfo.xhtml");

  await a1.uninstall();
});

const PARAMS =
  "?" +
  [
    "req_version=%REQ_VERSION%",
    "item_id=%ITEM_ID%",
    "item_version=%ITEM_VERSION%",
    "item_maxappversion=%ITEM_MAXAPPVERSION%",
    "item_status=%ITEM_STATUS%",
    "app_id=%APP_ID%",
    "app_version=%APP_VERSION%",
    "current_app_version=%CURRENT_APP_VERSION%",
    "app_os=%APP_OS%",
    "app_abi=%APP_ABI%",
    "app_locale=%APP_LOCALE%",
    "update_type=%UPDATE_TYPE%",
  ].join("&");

const PARAM_ADDONS = {
  "addon1@tests.mozilla.org": {
    manifest: {
      name: "Test Addon 1",
      version: "5.0",
      applications: {
        gecko: {
          id: "addon1@tests.mozilla.org",
          update_url: `http://example.com/data/param_test.json${PARAMS}`,
          strict_min_version: "1",
          strict_max_version: "2",
        },
      },
    },
    params: {
      item_version: "5.0",
      item_maxappversion: "2",
      item_status: "userEnabled",
      app_version: "1",
      update_type: "97",
    },
    updateType: [AddonManager.UPDATE_WHEN_USER_REQUESTED],
  },

  "addon2@tests.mozilla.org": {
    manifest: {
      name: "Test Addon 2",
      version: "67.0.5b1",
      applications: {
        gecko: {
          id: "addon2@tests.mozilla.org",
          update_url: "http://example.com/data/param_test.json" + PARAMS,
          strict_min_version: "0",
          strict_max_version: "3",
        },
      },
    },
    initialState: {
      userDisabled: true,
    },
    params: {
      item_version: "67.0.5b1",
      item_maxappversion: "3",
      item_status: "userDisabled",
      app_version: "1",
      update_type: "49",
    },
    updateType: [AddonManager.UPDATE_WHEN_ADDON_INSTALLED],
    compatOnly: true,
  },

  "addon3@tests.mozilla.org": {
    manifest: {
      name: "Test Addon 3",
      version: "1.3+",
      applications: {
        gecko: {
          id: "addon3@tests.mozilla.org",
          update_url: `http://example.com/data/param_test.json${PARAMS}`,
        },
      },
    },
    params: {
      item_version: "1.3+",
      item_status: "userEnabled",
      app_version: "1",
      update_type: "112",
    },
    updateType: [AddonManager.UPDATE_WHEN_PERIODIC_UPDATE],
  },

  "addon4@tests.mozilla.org": {
    manifest: {
      name: "Test Addon 4",
      version: "0.5ab6",
      applications: {
        gecko: {
          id: "addon4@tests.mozilla.org",
          update_url: `http://example.com/data/param_test.json${PARAMS}`,
          strict_min_version: "1",
          strict_max_version: "5",
        },
      },
    },
    params: {
      item_version: "0.5ab6",
      item_maxappversion: "5",
      item_status: "userEnabled",
      app_version: "2",
      update_type: "98",
    },
    updateType: [AddonManager.UPDATE_WHEN_NEW_APP_DETECTED, "2"],
  },

  "addon5@tests.mozilla.org": {
    manifest: {
      name: "Test Addon 5",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon5@tests.mozilla.org",
          update_url: `http://example.com/data/param_test.json${PARAMS}`,
          strict_min_version: "1",
          strict_max_version: "1",
        },
      },
    },
    params: {
      item_version: "1.0",
      item_maxappversion: "1",
      item_status: "userEnabled",
      app_version: "1",
      update_type: "35",
    },
    updateType: [AddonManager.UPDATE_WHEN_NEW_APP_INSTALLED],
    compatOnly: true,
  },

  "addon6@tests.mozilla.org": {
    manifest: {
      name: "Test Addon 6",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon6@tests.mozilla.org",
          update_url: `http://example.com/data/param_test.json${PARAMS}`,
          strict_min_version: "1",
          strict_max_version: "1",
        },
      },
    },
    params: {
      item_version: "1.0",
      item_maxappversion: "1",
      item_status: "userEnabled",
      app_version: "1",
      update_type: "99",
    },
    updateType: [AddonManager.UPDATE_WHEN_NEW_APP_INSTALLED],
  },

  "blocklist1@tests.mozilla.org": {
    manifest: {
      name: "Test Addon 1",
      version: "5.0",
      applications: {
        gecko: {
          id: "blocklist1@tests.mozilla.org",
          update_url: `http://example.com/data/param_test.json${PARAMS}`,
          strict_min_version: "1",
          strict_max_version: "2",
        },
      },
    },
    params: {
      item_version: "5.0",
      item_maxappversion: "2",
      item_status: "userDisabled,softblocked",
      app_version: "1",
      update_type: "97",
    },
    updateType: [AddonManager.UPDATE_WHEN_USER_REQUESTED],
    blocklistState: "STATE_SOFTBLOCKED",
  },

  "blocklist2@tests.mozilla.org": {
    manifest: {
      name: "Test Addon 1",
      version: "5.0",
      applications: {
        gecko: {
          id: "blocklist2@tests.mozilla.org",
          update_url: `http://example.com/data/param_test.json${PARAMS}`,
          strict_min_version: "1",
          strict_max_version: "2",
        },
      },
    },
    params: {
      item_version: "5.0",
      item_maxappversion: "2",
      item_status: "userEnabled,blocklisted",
      app_version: "1",
      update_type: "97",
    },
    updateType: [AddonManager.UPDATE_WHEN_USER_REQUESTED],
    blocklistState: "STATE_BLOCKED",
  },
};

const PARAM_IDS = Object.keys(PARAM_ADDONS);

// Verify the parameter escaping in update urls.
add_task(async function test_params() {
  let blocklistAddons = new Map();
  for (let [id, options] of Object.entries(PARAM_ADDONS)) {
    if (options.blocklistState) {
      blocklistAddons.set(id, Ci.nsIBlocklistService[options.blocklistState]);
    }
  }
  let mockBlocklist = await AddonTestUtils.overrideBlocklist(blocklistAddons);

  for (let [id, options] of Object.entries(PARAM_ADDONS)) {
<<<<<<< HEAD
    await promiseInstallWebExtension({manifest: options.manifest});
||||||| merged common ancestors
    await promiseInstallXPI(options["install.rdf"], profileDir);
=======
    await promiseInstallWebExtension({ manifest: options.manifest });
>>>>>>> upstream-releases

    if (options.initialState) {
      let addon = await AddonManager.getAddonByID(id);
      await setInitialState(addon, options.initialState);
    }
  }

  let resultsPromise = new Promise(resolve => {
    let results = new Map();

    testserver.registerPathHandler("/data/param_test.json", function(
      request,
      response
    ) {
      let params = new URLSearchParams(request.queryString);
      let itemId = params.get("item_id");
      ok(
        !results.has(itemId),
        `Should not see a duplicate request for item ${itemId}`
      );

      results.set(itemId, params);

      if (results.size === PARAM_IDS.length) {
        resolve(results);
      }

      request.setStatusLine(null, 500, "Server Error");
    });
  });

  let addons = await getAddons(PARAM_IDS);
  for (let [id, options] of Object.entries(PARAM_ADDONS)) {
    // Having an onUpdateAvailable callback in the listener automagically adds
    // UPDATE_TYPE_NEWVERSION to the update type flags in the request.
    let listener = options.compatOnly ? {} : { onUpdateAvailable() {} };

    addons.get(id).findUpdates(listener, ...options.updateType);
  }

  let baseParams = {
    req_version: "2",
    app_id: "xpcshell@tests.mozilla.org",
    current_app_version: "1",
    app_os: "XPCShell",
    app_abi: "noarch-spidermonkey",
    app_locale: "fr-FR",
  };

  let results = await resultsPromise;
  for (let [id, options] of Object.entries(PARAM_ADDONS)) {
    info(`Checking update params for ${id}`);

    let expected = Object.assign({}, baseParams, options.params);
    let params = results.get(id);

    for (let [prop, value] of Object.entries(expected)) {
      equal(params.get(prop), value, `Expected value for ${prop}`);
    }
  }

  for (let [, addon] of await getAddons(PARAM_IDS)) {
    await addon.uninstall();
  }

  await mockBlocklist.unregister();
});

// Tests that if a manifest claims compatibility then the add-on will be
// seen as compatible regardless of what the update payload says.
add_task(async function test_manifest_compat() {
  await promiseInstallWebExtension({
    manifest: {
      name: "Test Addon 1",
      version: "5.0",
      applications: {
        gecko: {
          id: "addon4@tests.mozilla.org",
          update_url: `http://example.com/data/${updateFile}`,
          strict_min_version: "0",
          strict_max_version: "1",
        },
      },
    },
  });

  let a4 = await AddonManager.getAddonByID("addon4@tests.mozilla.org");
  ok(a4.isActive, "addon4 is active");
  ok(a4.isCompatible, "addon4 is compatible");

  // Test that a normal update check won't decrease a targetApplication's
  // maxVersion but an update check for a new application will.
  await AddonTestUtils.promiseFindAddonUpdates(
<<<<<<< HEAD
    a4, AddonManager.UPDATE_WHEN_PERIODIC_UPDATE);
  ok(a4.isActive, "addon4 is active");
||||||| merged common ancestors
    a4, AddonManager.UPDATE_WHEN_PERIODIC_UPDATE);
=======
    a4,
    AddonManager.UPDATE_WHEN_PERIODIC_UPDATE
  );
  ok(a4.isActive, "addon4 is active");
>>>>>>> upstream-releases
  ok(a4.isCompatible, "addon4 is compatible");

  await AddonTestUtils.promiseFindAddonUpdates(
<<<<<<< HEAD
    a4, AddonManager.UPDATE_WHEN_NEW_APP_INSTALLED);
  ok(!a4.isActive, "addon4 is not active");
  ok(!a4.isCompatible, "addon4 is not compatible");
||||||| merged common ancestors
    a4, AddonManager.UPDATE_WHEN_NEW_APP_INSTALLED);
  ok(a4.isCompatible, "addon4 is not compatible");
});

// Check that the decreased maxVersion applied and disables the add-on
add_task(async function test_12() {
  let a4 = await AddonManager.getAddonByID("addon4@tests.mozilla.org");
  ok(a4.isActive);
  ok(a4.isCompatible);
=======
    a4,
    AddonManager.UPDATE_WHEN_NEW_APP_INSTALLED
  );
  ok(!a4.isActive, "addon4 is not active");
  ok(!a4.isCompatible, "addon4 is not compatible");
>>>>>>> upstream-releases

  await a4.uninstall();
});

// Test that the background update check doesn't update an add-on that isn't
// allowed to update automatically.
add_task(async function test_no_auto_update() {
  // Have an add-on there that will be updated so we see some events from it
  await promiseInstallWebExtension({
    manifest: {
      name: "Test Addon 1",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon1@tests.mozilla.org",
          update_url: `http://example.com/data/${updateFile}`,
        },
      },
    },
  });

  await promiseInstallWebExtension({
    manifest: {
      name: "Test Addon 8",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon8@tests.mozilla.org",
          update_url: `http://example.com/data/${updateFile}`,
        },
      },
    },
  });

  let a8 = await AddonManager.getAddonByID("addon8@tests.mozilla.org");
  a8.applyBackgroundUpdates = AddonManager.AUTOUPDATE_DISABLE;

  // The background update check will find updates for both add-ons but only
  // proceed to install one of them.
  let listener;
  await new Promise(resolve => {
    listener = {
      onNewInstall(aInstall) {
        let id = aInstall.existingAddon.id;
        ok(
          id == "addon1@tests.mozilla.org" || id == "addon8@tests.mozilla.org",
          "Saw unexpected onNewInstall for " + id
        );
      },

      onDownloadStarted(aInstall) {
        equal(aInstall.existingAddon.id, "addon1@tests.mozilla.org");
      },

      onDownloadEnded(aInstall) {
        equal(aInstall.existingAddon.id, "addon1@tests.mozilla.org");
      },

      onDownloadFailed(aInstall) {
        ok(false, "Should not have seen onDownloadFailed event");
      },

      onDownloadCancelled(aInstall) {
        ok(false, "Should not have seen onDownloadCancelled event");
      },

      onInstallStarted(aInstall) {
        equal(aInstall.existingAddon.id, "addon1@tests.mozilla.org");
      },

      onInstallEnded(aInstall) {
        equal(aInstall.existingAddon.id, "addon1@tests.mozilla.org");

        resolve();
      },

      onInstallFailed(aInstall) {
        ok(false, "Should not have seen onInstallFailed event");
      },

      onInstallCancelled(aInstall) {
        ok(false, "Should not have seen onInstallCancelled event");
      },
    };
    AddonManager.addInstallListener(listener);
    AddonManagerInternal.backgroundUpdateCheck();
  });
  AddonManager.removeInstallListener(listener);

  let a1;
  [a1, a8] = await AddonManager.getAddonsByIDs([
    "addon1@tests.mozilla.org",
    "addon8@tests.mozilla.org",
  ]);
  notEqual(a1, null);
  equal(a1.version, "2.0");
  await a1.uninstall();

  notEqual(a8, null);
  equal(a8.version, "1.0");
  await a8.uninstall();
});

<<<<<<< HEAD
// Test that background update checks work for lightweight themes
add_task(async function test_lwt_update() {
  LightweightThemeManager.currentTheme = {
    id: "1",
    version: "1",
    name: "Test LW Theme",
    description: "A test theme",
    author: "Mozilla",
    homepageURL: "http://example.com/data/index.html",
    headerURL: "http://example.com/data/header.png",
    previewURL: "http://example.com/data/preview.png",
    iconURL: "http://example.com/data/icon.png",
    updateURL: "http://example.com/data/lwtheme.js",
  };

  // XXX The lightweight theme manager strips non-https updateURLs so hack it
  // back in.
  let themes = JSON.parse(Services.prefs.getCharPref("lightweightThemes.usedThemes"));
  equal(themes.length, 1);
  themes[0].updateURL = "http://example.com/data/lwtheme.js";
  Services.prefs.setCharPref("lightweightThemes.usedThemes", JSON.stringify(themes));

  testserver.registerPathHandler("/data/lwtheme.js", function(request, response) {
    // Server will specify an expiry in one year.
    let expiry = new Date();
    expiry.setFullYear(expiry.getFullYear() + 1);
    response.setHeader("Expires", expiry.toUTCString(), false);
    response.write(JSON.stringify({
      id: "1",
      version: "2",
      name: "Updated Theme",
      description: "A test theme",
      author: "Mozilla",
      homepageURL: "http://example.com/data/index2.html",
      headerURL: "http://example.com/data/header.png",
      previewURL: "http://example.com/data/preview.png",
      iconURL: "http://example.com/data/icon2.png",
      updateURL: "http://example.com/data/lwtheme.js",
    }));
  });

  let p1 = await AddonManager.getAddonByID("1@personas.mozilla.org");
  notEqual(p1, null);
  equal(p1.version, "1");
  equal(p1.name, "Test LW Theme");
  ok(p1.isActive);
  equal(p1.installDate.getTime(), p1.updateDate.getTime());

  // 5 seconds leeway seems like a lot, but tests can run slow and really if
  // this is within 5 seconds it is fine. If it is going to be wrong then it
  // is likely to be hours out at least
  ok((Date.now() - p1.installDate.getTime()) < 5000);

  gInstallDate = p1.installDate.getTime();

  await new Promise(resolve => {
    prepare_test({
      "1@personas.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onExternalInstall",
    ], resolve);

    AddonManagerInternal.backgroundUpdateCheck();
  });

  p1 = await AddonManager.getAddonByID("1@personas.mozilla.org");
  notEqual(p1, null);
  equal(p1.version, "2");
  equal(p1.name, "Updated Theme");
  equal(p1.installDate.getTime(), gInstallDate);
  ok(p1.installDate.getTime() < p1.updateDate.getTime());

  // 5 seconds leeway seems like a lot, but tests can run slow and really if
  // this is within 5 seconds it is fine. If it is going to be wrong then it
  // is likely to be hours out at least
  ok((Date.now() - p1.updateDate.getTime()) < 5000);

  gInstallDate = p1.installDate.getTime();
});

// Test that background update checks for lightweight themes do not use the cache
// The update body from test 7 shouldn't be used since the cache should be bypassed.
add_task(async function() {
  // XXX The lightweight theme manager strips non-https updateURLs so hack it
  // back in.
  let themes = JSON.parse(Services.prefs.getCharPref("lightweightThemes.usedThemes"));
  equal(themes.length, 1);
  themes[0].updateURL = "http://example.com/data/lwtheme.js";
  Services.prefs.setCharPref("lightweightThemes.usedThemes", JSON.stringify(themes));

  testserver.registerPathHandler("/data/lwtheme.js", function(request, response) {
    response.write(JSON.stringify({
      id: "1",
      version: "3",
      name: "Updated Theme v.3",
      description: "A test theme v.3",
      author: "John Smith",
      homepageURL: "http://example.com/data/index3.html?v=3",
      headerURL: "http://example.com/data/header.png?v=3",
      previewURL: "http://example.com/data/preview.png?v=3",
      iconURL: "http://example.com/data/icon2.png?v=3",
      updateURL: "https://example.com/data/lwtheme.js?v=3",
    }));
  });

  let p1 = await AddonManager.getAddonByID("1@personas.mozilla.org");
  notEqual(p1, null);
  equal(p1.version, "2");
  equal(p1.name, "Updated Theme");
  ok(p1.isActive);
  equal(p1.installDate.getTime(), gInstallDate);
  ok(p1.installDate.getTime() < p1.updateDate.getTime());

  await new Promise(resolve => {
    prepare_test({
      "1@personas.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onExternalInstall",
    ], resolve);

    AddonManagerInternal.backgroundUpdateCheck();
  });

  p1 = await AddonManager.getAddonByID("1@personas.mozilla.org");
  let currentTheme = LightweightThemeManager.currentTheme;
  notEqual(p1, null);
  equal(p1.version, "3");
  equal(p1.name, "Updated Theme v.3");
  equal(p1.description, "A test theme v.3");
  info(JSON.stringify(p1));
  equal(p1.creator.name, "John Smith");
  equal(p1.homepageURL, "http://example.com/data/index3.html?v=3");
  equal(p1.screenshots[0].url, "http://example.com/data/preview.png?v=3");
  equal(p1.iconURL, "http://example.com/data/icon2.png?v=3");
  equal(currentTheme.headerURL, "http://example.com/data/header.png?v=3");
  equal(currentTheme.updateURL, "https://example.com/data/lwtheme.js?v=3");

  equal(p1.installDate.getTime(), gInstallDate);
  ok(p1.installDate.getTime() < p1.updateDate.getTime());
});

||||||| merged common ancestors
add_task(async function test_16() {
  let url = "http://example.com/addons/test_install2_1.xpi";
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await promiseCompleteInstall(install);

  let a1 = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  notEqual(a1.syncGUID, null);
  let oldGUID = a1.syncGUID;

  let url_2 = "http://example.com/addons/test_install2_2.xpi";
  let install_2 = await AddonManager.getInstallForURL(url_2, "application/x-xpinstall");
  await promiseCompleteInstall(install_2);

  let a2 = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  notEqual(a2.syncGUID, null);
  equal(oldGUID, a2.syncGUID);

  await a2.uninstall();
});

// Test that the update check correctly observes the
// extensions.strictCompatibility pref and compatibility overrides.
add_task(async function test_17() {
  await promiseInstallXPI({
    id: "addon9@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/" + updateFile,
    targetApplications: [{
      id: appId,
      minVersion: "0.1",
      maxVersion: "0.2",
    }],
    name: "Test Addon 9",
  });

  let listener;
  await new Promise(resolve => {
    listener = {
      onNewInstall(aInstall) {
        equal(aInstall.existingAddon.id, "addon9@tests.mozilla.org",
              "Saw unexpected onNewInstall for " + aInstall.existingAddon.id);
        equal(aInstall.version, "3.0");
      },
      onDownloadFailed(aInstall) {
        resolve();
      },
    };
    AddonManager.addInstallListener(listener);

    Services.prefs.setCharPref(PREF_GETADDONS_BYIDS,
                               `http://example.com/data/test_update_addons.json`);
    Services.prefs.setCharPref(PREF_COMPAT_OVERRIDES,
                               `http://example.com/data/test_update_compat.json`);
    Services.prefs.setBoolPref(PREF_GETADDONS_CACHE_ENABLED, true);

    AddonManagerInternal.backgroundUpdateCheck();
  });

  AddonManager.removeInstallListener(listener);

  let a9 = await AddonManager.getAddonByID("addon9@tests.mozilla.org");
  await a9.uninstall();
});

// Tests that compatibility updates are applied to addons when the updated
// compatibility data wouldn't match with strict compatibility enabled.
add_task(async function test_18() {
  await promiseInstallXPI({
    id: "addon10@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/" + updateFile,
    targetApplications: [{
      id: appId,
      minVersion: "0.1",
      maxVersion: "0.2",
    }],
    name: "Test Addon 10",
  });

  let a10 = await AddonManager.getAddonByID("addon10@tests.mozilla.org");
  notEqual(a10, null);

  let result = await AddonTestUtils.promiseFindAddonUpdates(a10);
  ok(result.compatibilityUpdate, "Should have seen a compatibility update");
  ok(!result.updateAvailable, "Should not have seen a version update");

  await a10.uninstall();
});

// Test that the update check correctly observes when an addon opts-in to
// strict compatibility checking.
add_task(async function test_19() {
  await promiseInstallXPI({
    id: "addon11@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/" + updateFile,
    targetApplications: [{
      id: appId,
      minVersion: "0.1",
      maxVersion: "0.2",
    }],
    name: "Test Addon 11",
  });

  let a11 = await AddonManager.getAddonByID("addon11@tests.mozilla.org");
  notEqual(a11, null);

  let result = await AddonTestUtils.promiseFindAddonUpdates(a11);
  ok(!result.compatibilityUpdate, "Should not have seen a compatibility update");
  ok(!result.updateAvailable, "Should not have seen a version update");

  await a11.uninstall();
});

// Test that the update succeeds when the update.rdf URN contains a type prefix
// different from the add-on type
add_task(async function test_20() {
  await promiseInstallXPI({
    id: "addon12@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/" + updateFile,
    targetApplications: [{
      id: appId,
      minVersion: "1",
      maxVersion: "1",
    }],
    name: "Test Addon 12",
  });

  let install = await new Promise(resolve => {
    prepare_test({}, [
      "onNewInstall",
      "onDownloadStarted",
      "onDownloadEnded",
    ], resolve);

    AddonManagerPrivate.backgroundUpdateCheck();
  });

  notEqual(install.existingAddon, null);
  equal(install.existingAddon.id, "addon12@tests.mozilla.org");

  await new Promise(resolve => {
    prepare_test({
      "addon12@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);
  });

  let a12 = await AddonManager.getAddonByID("addon12@tests.mozilla.org");
  notEqual(a12, null);
  equal(a12.version, "2.0");
  equal(a12.type, "extension");

  end_test();
  await a12.uninstall();
});

add_task(async function cleanup() {
  let addons = await AddonManager.getAddonsByTypes(["extension"]);

  for (let addon of addons)
    await addon.uninstall();
});

// Test that background update checks work for lightweight themes
add_task(async function test_7() {
  LightweightThemeManager.currentTheme = {
    id: "1",
    version: "1",
    name: "Test LW Theme",
    description: "A test theme",
    author: "Mozilla",
    homepageURL: "http://example.com/data/index.html",
    headerURL: "http://example.com/data/header.png",
    previewURL: "http://example.com/data/preview.png",
    iconURL: "http://example.com/data/icon.png",
    updateURL: "http://example.com/data/lwtheme.js",
  };

  // XXX The lightweight theme manager strips non-https updateURLs so hack it
  // back in.
  let themes = JSON.parse(Services.prefs.getCharPref("lightweightThemes.usedThemes"));
  equal(themes.length, 1);
  themes[0].updateURL = "http://example.com/data/lwtheme.js";
  Services.prefs.setCharPref("lightweightThemes.usedThemes", JSON.stringify(themes));

  testserver.registerPathHandler("/data/lwtheme.js", function(request, response) {
    // Server will specify an expiry in one year.
    let expiry = new Date();
    expiry.setFullYear(expiry.getFullYear() + 1);
    response.setHeader("Expires", expiry.toUTCString(), false);
    response.write(JSON.stringify({
      id: "1",
      version: "2",
      name: "Updated Theme",
      description: "A test theme",
      author: "Mozilla",
      homepageURL: "http://example.com/data/index2.html",
      headerURL: "http://example.com/data/header.png",
      previewURL: "http://example.com/data/preview.png",
      iconURL: "http://example.com/data/icon2.png",
      updateURL: "http://example.com/data/lwtheme.js",
    }));
  });

  let p1 = await AddonManager.getAddonByID("1@personas.mozilla.org");
  notEqual(p1, null);
  equal(p1.version, "1");
  equal(p1.name, "Test LW Theme");
  ok(p1.isActive);
  equal(p1.installDate.getTime(), p1.updateDate.getTime());

  // 5 seconds leeway seems like a lot, but tests can run slow and really if
  // this is within 5 seconds it is fine. If it is going to be wrong then it
  // is likely to be hours out at least
  ok((Date.now() - p1.installDate.getTime()) < 5000);

  gInstallDate = p1.installDate.getTime();

  await new Promise(resolve => {
    prepare_test({
      "1@personas.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onExternalInstall",
    ], resolve);

    AddonManagerInternal.backgroundUpdateCheck();
  });

  p1 = await AddonManager.getAddonByID("1@personas.mozilla.org");
  notEqual(p1, null);
  equal(p1.version, "2");
  equal(p1.name, "Updated Theme");
  equal(p1.installDate.getTime(), gInstallDate);
  ok(p1.installDate.getTime() < p1.updateDate.getTime());

  // 5 seconds leeway seems like a lot, but tests can run slow and really if
  // this is within 5 seconds it is fine. If it is going to be wrong then it
  // is likely to be hours out at least
  ok((Date.now() - p1.updateDate.getTime()) < 5000);

  gInstallDate = p1.installDate.getTime();
});

// Test that background update checks for lightweight themes do not use the cache
// The update body from test 7 shouldn't be used since the cache should be bypassed.
add_task(async function() {
  // XXX The lightweight theme manager strips non-https updateURLs so hack it
  // back in.
  let themes = JSON.parse(Services.prefs.getCharPref("lightweightThemes.usedThemes"));
  equal(themes.length, 1);
  themes[0].updateURL = "http://example.com/data/lwtheme.js";
  Services.prefs.setCharPref("lightweightThemes.usedThemes", JSON.stringify(themes));

  testserver.registerPathHandler("/data/lwtheme.js", function(request, response) {
    response.write(JSON.stringify({
      id: "1",
      version: "3",
      name: "Updated Theme v.3",
      description: "A test theme v.3",
      author: "John Smith",
      homepageURL: "http://example.com/data/index3.html?v=3",
      headerURL: "http://example.com/data/header.png?v=3",
      previewURL: "http://example.com/data/preview.png?v=3",
      iconURL: "http://example.com/data/icon2.png?v=3",
      updateURL: "https://example.com/data/lwtheme.js?v=3",
    }));
  });

  let p1 = await AddonManager.getAddonByID("1@personas.mozilla.org");
  notEqual(p1, null);
  equal(p1.version, "2");
  equal(p1.name, "Updated Theme");
  ok(p1.isActive);
  equal(p1.installDate.getTime(), gInstallDate);
  ok(p1.installDate.getTime() < p1.updateDate.getTime());

  await new Promise(resolve => {
    prepare_test({
      "1@personas.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onExternalInstall",
    ], resolve);

    AddonManagerInternal.backgroundUpdateCheck();
  });

  p1 = await AddonManager.getAddonByID("1@personas.mozilla.org");
  let currentTheme = LightweightThemeManager.currentTheme;
  notEqual(p1, null);
  equal(p1.version, "3");
  equal(p1.name, "Updated Theme v.3");
  equal(p1.description, "A test theme v.3");
  info(JSON.stringify(p1));
  equal(p1.creator.name, "John Smith");
  equal(p1.homepageURL, "http://example.com/data/index3.html?v=3");
  equal(p1.screenshots[0].url, "http://example.com/data/preview.png?v=3");
  equal(p1.iconURL, "http://example.com/data/icon2.png?v=3");
  equal(currentTheme.headerURL, "http://example.com/data/header.png?v=3");
  equal(currentTheme.updateURL, "https://example.com/data/lwtheme.js?v=3");

  equal(p1.installDate.getTime(), gInstallDate);
  ok(p1.installDate.getTime() < p1.updateDate.getTime());
});

=======
>>>>>>> upstream-releases
// Test that the update check returns nothing for addons in locked install
// locations.
add_task(async function run_test_locked_install() {
  const lockedDir = gProfD.clone();
  lockedDir.append("locked_extensions");
  registerDirectory("XREAppFeat", lockedDir);

  await promiseShutdownManager();

<<<<<<< HEAD
  let xpi = await createTempWebExtensionFile({
    manifest: {
      name: "Test Addon 13",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon13@tests.mozilla.org",
          update_url: "http://example.com/data/test_update.json",
        },
      },
    },
  });
  xpi.copyTo(lockedDir, "addon13@tests.mozilla.org.xpi");

  let validAddons = { "system": ["addon13@tests.mozilla.org"] };
||||||| merged common ancestors
  await promiseWriteInstallRDFToXPI({
    id: "addon13@tests.mozilla.org",
    version: "1.0",
    bootstrap: true,
    updateURL: "http://example.com/data/test_update.json",
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "0.1",
      maxVersion: "0.2",
    }],
    name: "Test Addon 13",
  }, lockedDir);

  let validAddons = { "system": ["addon13@tests.mozilla.org"] };
=======
  let xpi = await createTempWebExtensionFile({
    manifest: {
      name: "Test Addon 13",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon13@tests.mozilla.org",
          update_url: "http://example.com/data/test_update.json",
        },
      },
    },
  });
  xpi.copyTo(lockedDir, "addon13@tests.mozilla.org.xpi");

  let validAddons = { system: ["addon13@tests.mozilla.org"] };
>>>>>>> upstream-releases
  await overrideBuiltIns(validAddons);

  await promiseStartupManager();

  let a13 = await AddonManager.getAddonByID("addon13@tests.mozilla.org");
  notEqual(a13, null);

  let result = await AddonTestUtils.promiseFindAddonUpdates(a13);
  ok(
    !result.compatibilityUpdate,
    "Should not have seen a compatibility update"
  );
  ok(!result.updateAvailable, "Should not have seen a version update");

  let installs = await AddonManager.getAllInstalls();
  equal(installs.length, 0);
});

