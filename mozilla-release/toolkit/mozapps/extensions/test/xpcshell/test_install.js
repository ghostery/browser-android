/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

<<<<<<< HEAD
var testserver = createHttpServer({hosts: ["example.com"]});
||||||| merged common ancestors
ChromeUtils.import("resource://gre/modules/Services.jsm");
ChromeUtils.import("resource://gre/modules/NetUtil.jsm");
ChromeUtils.import("resource://testing-common/httpd.js");

var testserver = AddonTestUtils.createHttpServer({hosts: ["example.com"]});
=======
var testserver = createHttpServer({ hosts: ["example.com"] });
>>>>>>> upstream-releases
var gInstallDate;

const ADDONS = {
  test_install1: {
    manifest: {
      name: "Test 1",
<<<<<<< HEAD
      version: "1.0",
      applications: {gecko: {id: "addon1@tests.mozilla.org"}},
||||||| merged common ancestors
=======
      version: "1.0",
      applications: { gecko: { id: "addon1@tests.mozilla.org" } },
>>>>>>> upstream-releases
    },
  },
  test_install2_1: {
    manifest: {
      name: "Test 2",
      version: "2.0",
<<<<<<< HEAD
      applications: {gecko: {id: "addon2@tests.mozilla.org"}},
||||||| merged common ancestors
      name: "Real Test 2",
=======
      applications: { gecko: { id: "addon2@tests.mozilla.org" } },
>>>>>>> upstream-releases
    },
  },
  test_install2_2: {
    manifest: {
      name: "Test 2",
      version: "3.0",
<<<<<<< HEAD
      applications: {gecko: {id: "addon2@tests.mozilla.org"}},
||||||| merged common ancestors
      name: "Real Test 3",
=======
      applications: { gecko: { id: "addon2@tests.mozilla.org" } },
>>>>>>> upstream-releases
    },
  },
  test_install3: {
<<<<<<< HEAD
    manifest: {
      name: "Test 3",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon3@tests.mozilla.org",
          strict_min_version: "0",
          strict_max_version: "0",
          update_url: "http://example.com/update.json",
        },
      },
||||||| merged common ancestors
    "install.rdf": {
      id: "addon3@tests.mozilla.org",
      version: "1.0",
      name: "Real Test 4",

      updateURL: "http://example.com/data/test_install.rdf",

      targetApplications: [{
          id: "xpcshell@tests.mozilla.org",
          minVersion: "0",
          maxVersion: "0"}],
    },
  },
  test_install6: {
    "install.rdf": {
      id: "addon6@tests.mozilla.org",
      version: "1.0",
      name: "Addon Test 6",
    },
  },
  test_install7: {
    "install.rdf": {
      type: "32",
=======
    manifest: {
      name: "Test 3",
      version: "1.0",
      applications: {
        gecko: {
          id: "addon3@tests.mozilla.org",
          strict_min_version: "0",
          strict_max_version: "0",
          update_url: "http://example.com/update.json",
        },
      },
>>>>>>> upstream-releases
    },
  },
};

const XPIS = {};

// The test extension uses an insecure update url.
Services.prefs.setBoolPref(PREF_EM_CHECK_UPDATE_SECURITY, false);
Services.prefs.setBoolPref(PREF_EM_STRICT_COMPATIBILITY, false);

const profileDir = gProfD.clone();
profileDir.append("extensions");

<<<<<<< HEAD
const UPDATE_JSON = {
  addons: {
    "addon3@tests.mozilla.org": {
      updates: [
        {
          version: "1.0",
          applications: {
            gecko: {
              strict_min_version: "0",
              strict_max_version: "2",
            },
          },
        },
      ],
    },
  },
};

const GETADDONS_JSON = {
  page_size: 25,
  page_count: 1,
  count: 1,
  next: null,
  previous: null,
  results: [
    {
      name: "Test 2",
      type: "extension",
      guid: "addon2@tests.mozilla.org",
      current_version: {
        version: "1.0",
        files: [
          {
            size: 2,
            url: "http://example.com/test_install2_1.xpi",
          },
        ],
      },
      authors: [
        {
          name: "Test Creator",
          url: "http://example.com/creator.html",
        },
      ],
      summary: "Repository summary",
      description: "Repository description",
    },
  ],
};

const COMPAT_JSON = {
  page_size: 25,
  page_count: 1,
  count: 0,
  next: null,
  previous: null,
  results: [ ],
};

||||||| merged common ancestors
=======
const UPDATE_JSON = {
  addons: {
    "addon3@tests.mozilla.org": {
      updates: [
        {
          version: "1.0",
          applications: {
            gecko: {
              strict_min_version: "0",
              strict_max_version: "2",
            },
          },
        },
      ],
    },
  },
};

const GETADDONS_JSON = {
  page_size: 25,
  page_count: 1,
  count: 1,
  next: null,
  previous: null,
  results: [
    {
      name: "Test 2",
      type: "extension",
      guid: "addon2@tests.mozilla.org",
      current_version: {
        version: "1.0",
        files: [
          {
            size: 2,
            url: "http://example.com/test_install2_1.xpi",
          },
        ],
      },
      authors: [
        {
          name: "Test Creator",
          url: "http://example.com/creator.html",
        },
      ],
      summary: "Repository summary",
      description: "Repository description",
    },
  ],
};

const COMPAT_JSON = {
  page_size: 25,
  page_count: 1,
  count: 0,
  next: null,
  previous: null,
  results: [],
};

>>>>>>> upstream-releases
function checkInstall(install, expected) {
  for (let [key, value] of Object.entries(expected)) {
    if (value instanceof Ci.nsIURI) {
      equal(
        install[key] && install[key].spec,
        value.spec,
        `Expected value of install.${key}`
      );
    } else {
      deepEqual(install[key], value, `Expected value of install.${key}`);
    }
  }
}

add_task(async function setup() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1.9.2");

  for (let [name, data] of Object.entries(ADDONS)) {
    XPIS[name] = AddonTestUtils.createTempWebExtensionFile(data);
    testserver.registerFile(`/addons/${name}.xpi`, XPIS[name]);
  }

  await promiseStartupManager();

  // Create and configure the HTTP server.
  AddonTestUtils.registerJSON(testserver, "/update.json", UPDATE_JSON);
  testserver.registerDirectory("/data/", do_get_file("data"));
  testserver.registerPathHandler("/redirect", function(aRequest, aResponse) {
    aResponse.setStatusLine(null, 301, "Moved Permanently");
    let url = aRequest.host + ":" + aRequest.port + aRequest.queryString;
    aResponse.setHeader("Location", "http://" + url);
  });
  gPort = testserver.identity.primaryPort;
});

// Checks that an install from a local file proceeds as expected
add_task(async function test_install_file() {
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForFile(XPIS.test_install1),
  ]);

  let uri = Services.io.newFileURI(XPIS.test_install1);
  checkInstall(install, {
    type: "extension",
    version: "1.0",
    name: "Test 1",
    state: AddonManager.STATE_DOWNLOADED,
    sourceURI: uri,
  });

  let { addon } = install;
  checkAddon("addon1@tests.mozilla.org", addon, {
    install,
    sourceURI: uri,
  });
  notEqual(addon.syncGUID, null);
<<<<<<< HEAD
  equal(addon.getResourceURI("manifest.json").spec, `jar:${uri.spec}!/manifest.json`);
||||||| merged common ancestors
  equal(addon.getResourceURI("install.rdf").spec, `jar:${uri.spec}!/install.rdf`);
=======
  equal(
    addon.getResourceURI("manifest.json").spec,
    `jar:${uri.spec}!/manifest.json`
  );
>>>>>>> upstream-releases

  let activeInstalls = await AddonManager.getAllInstalls();
  equal(activeInstalls.length, 1);
  equal(activeInstalls[0], install);

  let fooInstalls = await AddonManager.getInstallsByTypes(["foo"]);
  equal(fooInstalls.length, 0);

  let extensionInstalls = await AddonManager.getInstallsByTypes(["extension"]);
  equal(extensionInstalls.length, 1);
  equal(extensionInstalls[0], install);

<<<<<<< HEAD
  prepare_test({
    "addon1@tests.mozilla.org": [
      ["onInstalling", false],
      "onInstalled",
    ],
  }, [
    "onInstallStarted",
    "onInstallEnded",
  ]);

  await install.install();

  ensure_test_completed();
||||||| merged common ancestors
  let installSyncGUID = await new Promise(resolve => {
    prepare_test({
      "addon1@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onInstallStarted",
      "onInstallEnded",
    ], () => {
      resolve(install.addon.syncGUID);
    });
    install.install();
  });

  ensure_test_completed();
=======
  await expectEvents(
    {
      addonEvents: {
        "addon1@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
    },
    () => install.install()
  );
>>>>>>> upstream-releases

  addon = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  ok(addon);

  ok(!hasFlag(addon.permissions, AddonManager.PERM_CAN_ENABLE));
  ok(hasFlag(addon.permissions, AddonManager.PERM_CAN_DISABLE));

  let updateDate = Date.now();

  await promiseRestartManager();

  activeInstalls = await AddonManager.getAllInstalls();
  equal(activeInstalls, 0);

  let a1 = await AddonManager.getAddonByID("addon1@tests.mozilla.org");
  let uri2 = do_get_addon_root_uri(profileDir, "addon1@tests.mozilla.org");

  checkAddon("addon1@tests.mozilla.org", a1, {
    type: "extension",
    version: "1.0",
    name: "Test 1",
    foreignInstall: false,
    sourceURI: Services.io.newFileURI(XPIS.test_install1),
  });

  notEqual(a1.syncGUID, null);
  ok(a1.syncGUID.length >= 9);

  ok(isExtensionInBootstrappedList(profileDir, a1.id));
  ok(XPIS.test_install1.exists());
  do_check_in_crash_annotation(a1.id, a1.version);

  let difference = a1.installDate.getTime() - updateDate;
  if (Math.abs(difference) > MAX_TIME_DIFFERENCE) {
    do_throw("Add-on install time was out by " + difference + "ms");
  }

  difference = a1.updateDate.getTime() - updateDate;
  if (Math.abs(difference) > MAX_TIME_DIFFERENCE) {
    do_throw("Add-on update time was out by " + difference + "ms");
  }

  equal(a1.getResourceURI("manifest.json").spec, uri2 + "manifest.json");

  // Ensure that extension bundle (or icon if unpacked) has updated
  // lastModifiedDate.
<<<<<<< HEAD
  let testURI = a1.getResourceURI("");
  let testFile = testURI.QueryInterface(Ci.nsIFileURL).file;
||||||| merged common ancestors
  let testURI = a1.getResourceURI(TEST_UNPACKED ? "icon.png" : "");
  let testFile = testURI.QueryInterface(Ci.nsIFileURL).file;
=======
  let testFile = getAddonFile(a1);
>>>>>>> upstream-releases
  ok(testFile.exists());
  difference = testFile.lastModifiedTime - Date.now();
  ok(Math.abs(difference) < MAX_TIME_DIFFERENCE);

  await a1.uninstall();
  let { id, version } = a1;
  await promiseRestartManager();
  do_check_not_in_crash_annotation(id, version);
});

// Tests that an install from a url downloads.
add_task(async function test_install_url() {
  let url = "http://example.com/addons/test_install2_1.xpi";
  let install = await AddonManager.getInstallForURL(url, {
    name: "Test 2",
    version: "1.0",
  });
  checkInstall(install, {
    version: "1.0",
    name: "Test 2",
    state: AddonManager.STATE_AVAILABLE,
    sourceURI: Services.io.newURI(url),
  });

  let activeInstalls = await AddonManager.getAllInstalls();
  equal(activeInstalls.length, 1);
  equal(activeInstalls[0], install);

<<<<<<< HEAD
  await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], () => {
      resolve();
      return false;
    });
    install.install();
  });
||||||| merged common ancestors
  install = await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], install1 => {
      resolve(install1);
      return false;
    });

    install.addListener({
      onDownloadProgress() {
        executeSoon(function() {
          Cu.forceGC();
        });
      },
    });

    install.install();
  });
=======
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
>>>>>>> upstream-releases

<<<<<<< HEAD
  ensure_test_completed();

||||||| merged common ancestors
  ensure_test_completed();
=======
>>>>>>> upstream-releases
  checkInstall(install, {
    version: "2.0",
    name: "Test 2",
    state: AddonManager.STATE_DOWNLOADED,
  });
  equal(install.addon.install, install);

<<<<<<< HEAD
  prepare_test({
    "addon2@tests.mozilla.org": [
      ["onInstalling", false],
      "onInstalled",
    ],
  }, [
    "onInstallStarted",
    "onInstallEnded",
  ]);
||||||| merged common ancestors
  install = await new Promise(resolve => {
    prepare_test({
      "addon2@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);
    install.install();
  });

  let updateDate = Date.now();
=======
  await expectEvents(
    {
      addonEvents: {
        "addon2@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
      ],
    },
    () => install.install()
  );

  let updateDate = Date.now();
>>>>>>> upstream-releases

<<<<<<< HEAD
  await install.install();
  ensure_test_completed();

  let updateDate = Date.now();

||||||| merged common ancestors
  ensure_test_completed();

=======
>>>>>>> upstream-releases
  await promiseRestartManager();

  let installs = await AddonManager.getAllInstalls();
  equal(installs, 0);

  let a2 = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", a2, {
    type: "extension",
    version: "2.0",
    name: "Test 2",
    sourceURI: Services.io.newURI(url),
  });
  notEqual(a2.syncGUID, null);

  ok(isExtensionInBootstrappedList(profileDir, a2.id));
  ok(XPIS.test_install2_1.exists());
  do_check_in_crash_annotation(a2.id, a2.version);

  let difference = a2.installDate.getTime() - updateDate;
<<<<<<< HEAD
  Assert.lessOrEqual(Math.abs(difference), MAX_TIME_DIFFERENCE,
                     "Add-on install time was correct");
||||||| merged common ancestors
  if (Math.abs(difference) > MAX_TIME_DIFFERENCE)
    do_throw("Add-on install time was out by " + difference + "ms");
=======
  Assert.lessOrEqual(
    Math.abs(difference),
    MAX_TIME_DIFFERENCE,
    "Add-on install time was correct"
  );
>>>>>>> upstream-releases

  difference = a2.updateDate.getTime() - updateDate;
<<<<<<< HEAD
  Assert.lessOrEqual(Math.abs(difference), MAX_TIME_DIFFERENCE,
                     "Add-on update time was correct");
||||||| merged common ancestors
  if (Math.abs(difference) > MAX_TIME_DIFFERENCE)
    do_throw("Add-on update time was out by " + difference + "ms");
=======
  Assert.lessOrEqual(
    Math.abs(difference),
    MAX_TIME_DIFFERENCE,
    "Add-on update time was correct"
  );
>>>>>>> upstream-releases

  gInstallDate = a2.installDate;
});

// Tests that installing a new version of an existing add-on works
add_task(async function test_install_new_version() {
  let url = "http://example.com/addons/test_install2_2.xpi";
<<<<<<< HEAD
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url, "application/x-xpinstall", null, "Test 2", null, "3.0"),
  ]);
||||||| merged common ancestors
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall", null, "Test 3", null, "3.0");
  ensure_test_completed();
=======
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url, {
      name: "Test 2",
      version: "3.0",
    }),
  ]);
>>>>>>> upstream-releases

  checkInstall(install, {
    version: "3.0",
    name: "Test 2",
    state: AddonManager.STATE_AVAILABLE,
    existingAddon: null,
  });

  let activeInstalls = await AddonManager.getAllInstalls();
  equal(activeInstalls.length, 1);
  equal(activeInstalls[0], install);

<<<<<<< HEAD
  await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], () => {
      resolve();
      return false;
    });
    install.install();
  });

  ensure_test_completed();
||||||| merged common ancestors
  install = await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], install1 => {
      resolve(install1);
      return false;
    });
    install.install();
  });

  ensure_test_completed();
=======
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
>>>>>>> upstream-releases

  checkInstall(install, {
    version: "3.0",
    name: "Test 2",
    state: AddonManager.STATE_DOWNLOADED,
    existingAddon: await AddonManager.getAddonByID("addon2@tests.mozilla.org"),
  });

  equal(install.addon.install, install);

  // Installation will continue when there is nothing returned.
<<<<<<< HEAD
  prepare_test({
    "addon2@tests.mozilla.org": [
      ["onInstalling", false],
      "onInstalled",
    ],
  }, [
    "onInstallStarted",
    "onInstallEnded",
  ]);

  await install.install();

  ensure_test_completed();
||||||| merged common ancestors
  install = await new Promise(resolve => {
    prepare_test({
      "addon2@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);
    install.install();
  });

  ensure_test_completed();
=======
  await expectEvents(
    {
      addonEvents: {
        "addon2@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
      ],
    },
    () => install.install()
  );
>>>>>>> upstream-releases

  await promiseRestartManager();

  let installs2 = await AddonManager.getInstallsByTypes(null);
  equal(installs2.length, 0);

  let a2 = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", a2, {
    type: "extension",
    version: "3.0",
    name: "Test 2",
    isActive: true,
    foreignInstall: false,
    sourceURI: Services.io.newURI(url),
    installDate: gInstallDate,
  });

  ok(isExtensionInBootstrappedList(profileDir, a2.id));
  ok(XPIS.test_install2_2.exists());
  do_check_in_crash_annotation(a2.id, a2.version);

  // Update date should be later (or the same if this test is too fast)
  ok(a2.installDate <= a2.updateDate);

  await a2.uninstall();
});

// Tests that an install that requires a compatibility update works
add_task(async function test_install_compat_update() {
  let url = "http://example.com/addons/test_install3.xpi";
<<<<<<< HEAD
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    await AddonManager.getInstallForURL(url, "application/x-xpinstall", null, "Test 3", null, "1.0"),
  ]);
||||||| merged common ancestors
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall", null, "Real Test 4", null, "1.0");
  ensure_test_completed();
=======
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    await AddonManager.getInstallForURL(url, {
      name: "Test 3",
      version: "1.0",
    }),
  ]);
>>>>>>> upstream-releases

  checkInstall(install, {
    version: "1.0",
    name: "Test 3",
    state: AddonManager.STATE_AVAILABLE,
  });

  let activeInstalls = await AddonManager.getInstallsByTypes(null);
  equal(activeInstalls.length, 1);
  equal(activeInstalls[0], install);

<<<<<<< HEAD
  await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], () => {
      resolve();
      return false;
    });
    install.install();
  });
  ensure_test_completed();

||||||| merged common ancestors
  install = await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], install1 => {
      resolve(install1);
      return false;
    });
    install.install();
  });

  ensure_test_completed();
=======
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

>>>>>>> upstream-releases
  checkInstall(install, {
    version: "1.0",
    name: "Test 3",
    state: AddonManager.STATE_DOWNLOADED,
    existingAddon: null,
  });
  checkAddon("addon3@tests.mozilla.org", install.addon, {
    appDisabled: false,
  });

  // Continue the install
<<<<<<< HEAD
  prepare_test({
    "addon3@tests.mozilla.org": [
      ["onInstalling", false],
      "onInstalled",
    ],
  }, [
    "onInstallStarted",
    "onInstallEnded",
  ]);

  await install.install();
  ensure_test_completed();
||||||| merged common ancestors
  await new Promise(resolve => {
    prepare_test({
      "addon3@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);
    install.install();
  });

  ensure_test_completed();
=======
  await expectEvents(
    {
      addonEvents: {
        "addon3@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
      ],
    },
    () => install.install()
  );
>>>>>>> upstream-releases

  await promiseRestartManager();

  let installs = await AddonManager.getAllInstalls();
  equal(installs, 0);

  let a3 = await AddonManager.getAddonByID("addon3@tests.mozilla.org");
  checkAddon("addon3@tests.mozilla.org", a3, {
    type: "extension",
    version: "1.0",
    name: "Test 3",
    isActive: true,
    appDisabled: false,
  });
  notEqual(a3.syncGUID, null);

  ok(isExtensionInBootstrappedList(profileDir, a3.id));

  ok(XPIS.test_install3.exists());
  await a3.uninstall();
});

add_task(async function test_compat_update_local() {
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForFile(XPIS.test_install3),
  ]);
  ok(install.addon.isCompatible);

<<<<<<< HEAD
  prepare_test({
    "addon3@tests.mozilla.org": [
      ["onInstalling", false],
      "onInstalled",
    ],
  }, [
    "onInstallStarted",
    "onInstallEnded",
  ]);

  await install.install();
  ensure_test_completed();
||||||| merged common ancestors
  await new Promise(resolve => {
    prepare_test({
      "addon3@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);
    install.install();
  });
=======
  await expectEvents(
    {
      addonEvents: {
        "addon3@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
      ],
    },
    () => install.install()
  );
>>>>>>> upstream-releases

  await promiseRestartManager();

  let a3 = await AddonManager.getAddonByID("addon3@tests.mozilla.org");
  checkAddon("addon3@tests.mozilla.org", a3, {
    type: "extension",
    version: "1.0",
    name: "Test 3",
    isActive: true,
    appDisabled: false,
  });
  notEqual(a3.syncGUID, null);

  ok(isExtensionInBootstrappedList(profileDir, a3.id));

  ok(XPIS.test_install3.exists());
  await a3.uninstall();
});

// Test that after cancelling a download it is removed from the active installs
add_task(async function test_cancel() {
  let url = "http://example.com/addons/test_install3.xpi";
<<<<<<< HEAD
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url, "application/x-xpinstall", null, "Test 3", null, "1.0"),
  ]);
||||||| merged common ancestors
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall", null, "Real Test 4", null, "1.0");
  ensure_test_completed();
=======
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url, {
      name: "Test 3",
      version: "1.0",
    }),
  ]);
>>>>>>> upstream-releases

  checkInstall(install, {
    version: "1.0",
    name: "Test 3",
    state: AddonManager.STATE_AVAILABLE,
  });

  let activeInstalls = await AddonManager.getInstallsByTypes(null);
  equal(activeInstalls.length, 1);
  equal(activeInstalls[0], install);

<<<<<<< HEAD
  await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], () => {
      prepare_test({}, [
        "onDownloadCancelled",
      ], resolve);
||||||| merged common ancestors
  install = await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], () => {
      prepare_test({}, [
        "onDownloadCancelled",
      ], resolve);
=======
  let promise;
  function cancel() {
    promise = expectEvents(
      {
        installEvents: [{ event: "onDownloadCancelled" }],
      },
      () => {
        install.cancel();
      }
    );
  }
>>>>>>> upstream-releases

  await expectEvents(
    {
      installEvents: [
        { event: "onDownloadStarted" },
        { event: "onDownloadEnded", callback: cancel },
      ],
    },
    () => {
      install.install();
    }
  );

<<<<<<< HEAD
    install.install();
  });
  ensure_test_completed();
||||||| merged common ancestors
    install.install();
  });
=======
  await promise;
>>>>>>> upstream-releases

  let file = install.file;

  // Allow the file removal to complete
  activeInstalls = await AddonManager.getAllInstalls();
  equal(activeInstalls.length, 0);
  ok(!file.exists());
});

// Check that cancelling the install from onDownloadStarted actually cancels it
<<<<<<< HEAD
add_task(async function test_cancel_onDownloadStarted() {
  clearListeners();
||||||| merged common ancestors
add_task(async function test_14() {
  await promiseRestartManager();

  prepare_test({ }, [
    "onNewInstall",
  ]);

=======
add_task(async function test_cancel_onDownloadStarted() {
>>>>>>> upstream-releases
  let url = "http://example.com/addons/test_install2_1.xpi";
<<<<<<< HEAD
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url, "application/x-xpinstall"),
  ]);
||||||| merged common ancestors
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  ensure_test_completed();
=======
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url),
  ]);
>>>>>>> upstream-releases

  equal(install.file, null);

<<<<<<< HEAD
  install.addListener({
    onDownloadStarted() {
      install.removeListener(this);
      executeSoon(() => install.cancel());
    },
  });

||||||| merged common ancestors
  install = await new Promise(resolve => {
    prepare_test({ }, [
      "onDownloadStarted",
    ], resolve);
    install.install();
  });

  // Wait for the channel to be ready to cancel
  executeSoon(function() {
    install.cancel();
  });

  await new Promise(resolve => {
    prepare_test({ }, [
      "onDownloadCancelled",
    ], resolve);
  });
=======
  install.addListener({
    onDownloadStarted() {
      install.removeListener(this);
      executeSoon(() => install.cancel());
    },
  });
>>>>>>> upstream-releases

  let promise = AddonTestUtils.promiseInstallEvent("onDownloadCancelled");
  install.install();
  await promise;

  // Wait another tick to see if it continues downloading.
  // The listener only really tests if we give it time to see progress, the
  // file check isn't ideal either
  install.addListener({
    onDownloadProgress() {
      do_throw("Download should not have continued");
    },
    onDownloadEnded() {
      do_throw("Download should not have continued");
    },
  });

  let file = install.file;
  await Promise.resolve();
  ok(!file.exists());
});

// Checks that cancelling the install from onDownloadEnded actually cancels it
add_task(async function test_cancel_onDownloadEnded() {
  let url = "http://example.com/addons/test_install2_1.xpi";
<<<<<<< HEAD
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url, "application/x-xpinstall"),
  ]);
||||||| merged common ancestors
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  ensure_test_completed();
=======
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url),
  ]);
>>>>>>> upstream-releases

  equal(install.file, null);

  let promise;
  function cancel() {
    promise = expectEvents(
      {
        installEvents: [{ event: "onDownloadCancelled" }],
      },
      async () => {
        install.cancel();
      }
    );
  }

  await expectEvents(
    {
      installEvents: [
        { event: "onDownloadStarted" },
        { event: "onDownloadEnded", callback: cancel },
      ],
    },
    () => {
      install.install();
    }
  );

  await promise;

  install.addListener({
    onInstallStarted() {
      do_throw("Install should not have continued");
    },
  });
});

// Verify that the userDisabled value carries over to the upgrade by default
<<<<<<< HEAD
add_task(async function test_userDisabled_update() {
  clearListeners();
||||||| merged common ancestors
add_task(async function test_16() {
  await promiseRestartManager();

=======
add_task(async function test_userDisabled_update() {
>>>>>>> upstream-releases
  let url = "http://example.com/addons/test_install2_1.xpi";
<<<<<<< HEAD
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url, "application/x-xpinstall"),
  ]);
||||||| merged common ancestors
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await new Promise(resolve => {
    aInstall.addListener({
      onInstallStarted() {
        ok(!aInstall.addon.userDisabled);
        aInstall.addon.disable();
      },
=======
  let [, install] = await Promise.all([
    AddonTestUtils.promiseInstallEvent("onNewInstall"),
    AddonManager.getInstallForURL(url),
  ]);
>>>>>>> upstream-releases

  await install.install();

  ok(!install.addon.userDisabled);
  await install.addon.disable();

  let addon = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", addon, {
    userDisabled: true,
    isActive: false,
  });

<<<<<<< HEAD
  url = "http://example.com/addons/test_install2_2.xpi";
  install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await install.install();
||||||| merged common ancestors
  let url_2 = "http://example.com/addons/test_install2_2.xpi";
  let aInstall_2 = await AddonManager.getInstallForURL(url_2, "application/x-xpinstall");
  await new Promise(resolve => {
    aInstall_2.addListener({
      onInstallEnded() {
        resolve();
      },
    });
    aInstall_2.install();
  });
=======
  url = "http://example.com/addons/test_install2_2.xpi";
  install = await AddonManager.getInstallForURL(url);
  await install.install();
>>>>>>> upstream-releases

  checkAddon("addon2@tests.mozilla.org", install.addon, {
    userDisabled: true,
    isActive: false,
  });

  await promiseRestartManager();

  addon = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", addon, {
    userDisabled: true,
    isActive: false,
  });

  await addon.uninstall();
});

// Verify that changing the userDisabled value before onInstallEnded works
add_task(async function test_userDisabled() {
  let url = "http://example.com/addons/test_install2_1.xpi";
<<<<<<< HEAD
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await install.install();
||||||| merged common ancestors
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await new Promise(resolve => {
    aInstall.addListener({
      onInstallEnded() {
        resolve();
      },
    });
    aInstall.install();
  });

  ok(!aInstall.addon.userDisabled);
=======
  let install = await AddonManager.getInstallForURL(url);
  await install.install();

  ok(!install.addon.userDisabled);
>>>>>>> upstream-releases

<<<<<<< HEAD
  ok(!install.addon.userDisabled);

  let addon = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", addon, {
||||||| merged common ancestors
  await promiseRestartManager();

  let a2 = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", a2, {
=======
  let addon = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", addon, {
>>>>>>> upstream-releases
    userDisabled: false,
    isActive: true,
  });

<<<<<<< HEAD
  url = "http://example.com/addons/test_install2_2.xpi";
  install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");

  install.addListener({
    onInstallStarted() {
      ok(!install.addon.userDisabled);
      install.addon.disable();
    },
||||||| merged common ancestors
  let url_2 = "http://example.com/addons/test_install2_2.xpi";
  let aInstall_2 = await AddonManager.getInstallForURL(url_2, "application/x-xpinstall");

  await new Promise(resolve => {
    aInstall_2.addListener({
      onInstallStarted() {
        ok(!aInstall_2.addon.userDisabled);
        aInstall_2.addon.disable();
      },

      onInstallEnded() {
        resolve();
      },
    });
    aInstall_2.install();
  });
  await promiseRestartManager();

  let a2_2 = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", a2_2, {
    userDisabled: true,
    isActive: false,
  });

  await a2_2.uninstall();
});

// Verify that changing the userDisabled value before onInstallEnded works
add_task(async function test_18() {
  await promiseRestartManager();

  let url = "http://example.com/addons/test_install2_1.xpi";
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await new Promise(resolve => {
    aInstall.addListener({
      onInstallStarted() {
        ok(!aInstall.addon.userDisabled);
        aInstall.addon.disable();
      },

      onInstallEnded() {
        resolve();
      },
    });
    aInstall.install();
=======
  url = "http://example.com/addons/test_install2_2.xpi";
  install = await AddonManager.getInstallForURL(url);

  install.addListener({
    onInstallStarted() {
      ok(!install.addon.userDisabled);
      install.addon.disable();
    },
>>>>>>> upstream-releases
  });

  await install.install();

  addon = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  checkAddon("addon2@tests.mozilla.org", addon, {
    userDisabled: true,
    isActive: false,
  });

  await addon.uninstall();
});

// Checks that metadata is not stored if the pref is set to false
add_task(async function test_18_1() {
<<<<<<< HEAD
  AddonTestUtils.registerJSON(testserver, "/getaddons.json", GETADDONS_JSON);
  Services.prefs.setCharPref(PREF_GETADDONS_BYIDS,
                             "http://example.com/getaddons.json");

  AddonTestUtils.registerJSON(testserver, "/compat.json", COMPAT_JSON);
  Services.prefs.setCharPref(PREF_COMPAT_OVERRIDES,
                             "http://example.com/compat.json");

  Services.prefs.setBoolPref("extensions.getAddons.cache.enabled", true);
  Services.prefs.setBoolPref("extensions.addon2@tests.mozilla.org.getAddons.cache.enabled", false);
||||||| merged common ancestors
  await promiseRestartManager();

  Services.prefs.setBoolPref("extensions.getAddons.cache.enabled", true);
  Services.prefs.setCharPref(PREF_GETADDONS_BYIDS,
                             "http://example.com/data/test_install_addons.json");
  Services.prefs.setCharPref(PREF_COMPAT_OVERRIDES,
                             "http://example.com/data/test_install_compat.json");

  Services.prefs.setBoolPref("extensions.addon2@tests.mozilla.org.getAddons.cache.enabled", false);
=======
  AddonTestUtils.registerJSON(testserver, "/getaddons.json", GETADDONS_JSON);
  Services.prefs.setCharPref(
    PREF_GETADDONS_BYIDS,
    "http://example.com/getaddons.json"
  );

  AddonTestUtils.registerJSON(testserver, "/compat.json", COMPAT_JSON);
  Services.prefs.setCharPref(
    PREF_COMPAT_OVERRIDES,
    "http://example.com/compat.json"
  );

  Services.prefs.setBoolPref("extensions.getAddons.cache.enabled", true);
  Services.prefs.setBoolPref(
    "extensions.addon2@tests.mozilla.org.getAddons.cache.enabled",
    false
  );
>>>>>>> upstream-releases

  let url = "http://example.com/addons/test_install2_1.xpi";
<<<<<<< HEAD
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await install.install();
||||||| merged common ancestors
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");

  let addon = await new Promise(resolve => {
    aInstall.addListener({
      onInstallEnded(unused, aAddon) {
        resolve(aAddon);
      },
    });
    aInstall.install();
  });
=======
  let install = await AddonManager.getInstallForURL(url);
  await install.install();
>>>>>>> upstream-releases

  notEqual(install.addon.fullDescription, "Repository description");

  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  notEqual(addon.fullDescription, "Repository description");

  await addon.uninstall();
});

// Checks that metadata is downloaded for new installs and is visible before and
// after restart
<<<<<<< HEAD
add_task(async function test_metadata() {
  Services.prefs.setBoolPref("extensions.addon2@tests.mozilla.org.getAddons.cache.enabled", true);
||||||| merged common ancestors
add_task(async function test_19() {
  await promiseRestartManager();
  Services.prefs.setBoolPref("extensions.addon2@tests.mozilla.org.getAddons.cache.enabled", true);
=======
add_task(async function test_metadata() {
  Services.prefs.setBoolPref(
    "extensions.addon2@tests.mozilla.org.getAddons.cache.enabled",
    true
  );
>>>>>>> upstream-releases

  let url = "http://example.com/addons/test_install2_1.xpi";
<<<<<<< HEAD
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await install.install();
||||||| merged common ancestors
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await new Promise(resolve => {
    aInstall.addListener({
      onInstallEnded(unused, aAddon) {
        resolve(aAddon);
      },
    });
    aInstall.install();
  });
=======
  let install = await AddonManager.getInstallForURL(url);
  await install.install();
>>>>>>> upstream-releases

  equal(install.addon.fullDescription, "Repository description");

  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  equal(addon.fullDescription, "Repository description");

  await addon.uninstall();
});

// Do the same again to make sure it works when the data is already in the cache
add_task(async function test_metadata_again() {
  let url = "http://example.com/addons/test_install2_1.xpi";
<<<<<<< HEAD
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await install.install();
||||||| merged common ancestors
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  await new Promise(resolve => {
    aInstall.addListener({
      onInstallEnded(unused, aAddon) {
        resolve(aAddon);
      },
    });
    aInstall.install();
  });
=======
  let install = await AddonManager.getInstallForURL(url);
  await install.install();
>>>>>>> upstream-releases

  equal(install.addon.fullDescription, "Repository description");

  await promiseRestartManager();

  let addon = await AddonManager.getAddonByID("addon2@tests.mozilla.org");
  equal(addon.fullDescription, "Repository description");

  await addon.uninstall();
});

// Tests that an install can be restarted after being cancelled
<<<<<<< HEAD
add_task(async function test_restart() {
  let url = "http://example.com/addons/test_install1.xpi";
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  equal(install.state, AddonManager.STATE_AVAILABLE);

  install.addListener({
    onDownloadEnded() {
      install.removeListener(this);
      install.cancel();
    },
||||||| merged common ancestors
add_task(async function test_22() {
  prepare_test({ }, [
    "onNewInstall",
  ]);

  let url = "http://example.com/addons/test_install3.xpi";
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  ensure_test_completed();

  checkInstall(aInstall, {
    state: AddonManager.STATE_AVAILABLE,
  });

  let install = await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], install1 => {
      prepare_test({}, [
        "onDownloadCancelled",
      ]);
      aInstall.cancel();
      resolve(install1);
    });
    aInstall.install();
=======
add_task(async function test_restart() {
  let url = "http://example.com/addons/test_install1.xpi";
  let install = await AddonManager.getInstallForURL(url);
  equal(install.state, AddonManager.STATE_AVAILABLE);

  install.addListener({
    onDownloadEnded() {
      install.removeListener(this);
      install.cancel();
    },
>>>>>>> upstream-releases
  });

<<<<<<< HEAD
  try {
    await install.install();
    ok(false, "Install should not have succeeded");
  } catch (err) { }
||||||| merged common ancestors
  ensure_test_completed();
=======
  try {
    await install.install();
    ok(false, "Install should not have succeeded");
  } catch (err) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  let promise = new Promise(resolve => {
    prepare_test({
      "addon1@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
||||||| merged common ancestors
  await new Promise(resolve => {
    prepare_test({
      "addon3@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
=======
  let promise = expectEvents(
    {
      addonEvents: {
        "addon1@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onDownloadStarted" },
        { event: "onDownloadEnded" },
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
>>>>>>> upstream-releases
      ],
<<<<<<< HEAD
    }, [
      "onDownloadStarted",
      "onDownloadEnded",
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);
    install.install();
  });
||||||| merged common ancestors
    }, [
      "onDownloadStarted",
      "onDownloadEnded",
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);

    install.install();
  });
=======
    },
    () => {
      install.install();
    }
  );
>>>>>>> upstream-releases

<<<<<<< HEAD
  await Promise.all([
    promise,
    promiseWebExtensionStartup("addon1@tests.mozilla.org"),
  ]);
  ensure_test_completed();
||||||| merged common ancestors
  ensure_test_completed();
=======
  await Promise.all([
    promise,
    promiseWebExtensionStartup("addon1@tests.mozilla.org"),
  ]);
>>>>>>> upstream-releases

<<<<<<< HEAD
  clearListeners();

||||||| merged common ancestors
  AddonManager.removeAddonListener(AddonListener);
=======
>>>>>>> upstream-releases
  await install.addon.uninstall();
});

// Tests that an install can be restarted after being cancelled when a hash
// was provided
<<<<<<< HEAD
add_task(async function test_restart_hash() {
  let url = "http://example.com/addons/test_install1.xpi";
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall", do_get_file_hash(XPIS.test_install1));
  equal(install.state, AddonManager.STATE_AVAILABLE);

  install.addListener({
    onDownloadEnded() {
      install.removeListener(this);
||||||| merged common ancestors
add_task(async function test_23() {
  prepare_test({ }, [
    "onNewInstall",
  ]);

  let url = "http://example.com/addons/test_install3.xpi";
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall", do_get_file_hash(XPIS.test_install3));

  ensure_test_completed();

  checkInstall(install, {
    state: AddonManager.STATE_AVAILABLE,
  });

  await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], () => {
      prepare_test({}, [
        "onDownloadCancelled",
      ]);

=======
add_task(async function test_restart_hash() {
  let url = "http://example.com/addons/test_install1.xpi";
  let install = await AddonManager.getInstallForURL(url, {
    hash: do_get_file_hash(XPIS.test_install1),
  });
  equal(install.state, AddonManager.STATE_AVAILABLE);

  install.addListener({
    onDownloadEnded() {
      install.removeListener(this);
>>>>>>> upstream-releases
      install.cancel();
    },
  });

<<<<<<< HEAD
  try {
    await install.install();
    ok(false, "Install should not have succeeded");
  } catch (err) { }
||||||| merged common ancestors
  ensure_test_completed();
=======
  try {
    await install.install();
    ok(false, "Install should not have succeeded");
  } catch (err) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
  let promise = new Promise(resolve => {
    prepare_test({
      "addon1@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
||||||| merged common ancestors
  await new Promise(resolve => {
    prepare_test({
      "addon3@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
=======
  let promise = expectEvents(
    {
      addonEvents: {
        "addon1@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onDownloadStarted" },
        { event: "onDownloadEnded" },
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
>>>>>>> upstream-releases
      ],
<<<<<<< HEAD
    }, [
      "onDownloadStarted",
      "onDownloadEnded",
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);
    install.install();
  });
||||||| merged common ancestors
    }, [
      "onDownloadStarted",
      "onDownloadEnded",
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);

    install.install();
  });
=======
    },
    () => {
      install.install();
    }
  );
>>>>>>> upstream-releases

<<<<<<< HEAD
  await Promise.all([
    promise,
    promiseWebExtensionStartup("addon1@tests.mozilla.org"),
  ]);
  ensure_test_completed();
||||||| merged common ancestors
  ensure_test_completed();
=======
  await Promise.all([
    promise,
    promiseWebExtensionStartup("addon1@tests.mozilla.org"),
  ]);
>>>>>>> upstream-releases

<<<<<<< HEAD
  clearListeners();

||||||| merged common ancestors
  AddonManager.removeAddonListener(AddonListener);
=======
>>>>>>> upstream-releases
  await install.addon.uninstall();

});

// Tests that an install with a bad hash can be restarted after it fails, though
// it will only fail again
<<<<<<< HEAD
add_task(async function test_restart_badhash() {
  let url = "http://example.com/addons/test_install1.xpi";
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall", "sha1:foo");
  equal(install.state, AddonManager.STATE_AVAILABLE);

  install.addListener({
    onDownloadEnded() {
      install.removeListener(this);
      install.cancel();
    },
||||||| merged common ancestors
add_task(async function test_24() {
  prepare_test({ }, [
    "onNewInstall",
  ]);

  let url = "http://example.com/addons/test_install3.xpi";
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall", "sha1:foo");
  ensure_test_completed();

  checkInstall(aInstall, {
    state: AddonManager.STATE_AVAILABLE,
  });

  let install = await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadFailed",
    ], resolve);
    aInstall.install();
=======
add_task(async function test_restart_badhash() {
  let url = "http://example.com/addons/test_install1.xpi";
  let install = await AddonManager.getInstallForURL(url, { hash: "sha1:foo" });
  equal(install.state, AddonManager.STATE_AVAILABLE);

  install.addListener({
    onDownloadEnded() {
      install.removeListener(this);
      install.cancel();
    },
>>>>>>> upstream-releases
  });

<<<<<<< HEAD
  try {
    await install.install();
    ok(false, "Install should not have succeeded");
  } catch (err) { }
||||||| merged common ancestors
  await new Promise(resolve => {
    prepare_test({ }, [
      "onDownloadStarted",
      "onDownloadFailed",
    ], resolve);
=======
  try {
    await install.install();
    ok(false, "Install should not have succeeded");
  } catch (err) {}
>>>>>>> upstream-releases

  try {
    await install.install();
    ok(false, "Install should not have succeeded");
  } catch (err) {
    ok(true, "Resumed install should have failed");
  }
});

// Tests that installs with a hash for a local file work
<<<<<<< HEAD
add_task(async function test_local_hash() {
  let url = Services.io.newFileURI(XPIS.test_install1).spec;
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall", do_get_file_hash(XPIS.test_install1));

  checkInstall(install, {
||||||| merged common ancestors
add_task(async function test_25() {
  prepare_test({ }, [
    "onNewInstall",
  ]);

  let url = Services.io.newFileURI(XPIS.test_install3).spec;
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall", do_get_file_hash(XPIS.test_install3));
  ensure_test_completed();

  checkInstall(aInstall, {
=======
add_task(async function test_local_hash() {
  let url = Services.io.newFileURI(XPIS.test_install1).spec;
  let install = await AddonManager.getInstallForURL(url, {
    hash: do_get_file_hash(XPIS.test_install1),
  });

  checkInstall(install, {
>>>>>>> upstream-releases
    state: AddonManager.STATE_DOWNLOADED,
    error: 0,
  });

  install.cancel();
});

// Test that an install may be canceled after a redirect.
add_task(async function test_cancel_redirect() {
  let url = "http://example.com/redirect?/addons/test_install1.xpi";
<<<<<<< HEAD
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");

  install.addListener({
    onDownloadProgress() {
      install.cancel();
    },
  });

  let promise = AddonTestUtils.promiseInstallEvent("onDownloadCancelled");

  install.install();
  await promise;

  equal(install.state, AddonManager.STATE_CANCELLED);
});

// Tests that an install can be restarted during onDownloadCancelled after being
// cancelled in mid-download
add_task(async function test_restart2() {
  let url = "http://example.com/addons/test_install1.xpi";
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall");

  equal(install.state, AddonManager.STATE_AVAILABLE);
||||||| merged common ancestors
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");

  await new Promise(resolve => {
    let observerService = Cc["@mozilla.org/network/http-activity-distributor;1"].
                          getService(Ci.nsIHttpActivityDistributor);
    observerService.addObserver({
      observeActivity(aChannel, aType, aSubtype, aTimestamp, aSizeData,
                                aStringData) {
        aChannel.QueryInterface(Ci.nsIChannel);
        // Wait for the final event for the redirected URL
        if (aChannel.URI.spec != "http://example.com/addons/test_install1.xpi" ||
            aType != Ci.nsIHttpActivityObserver.ACTIVITY_TYPE_HTTP_TRANSACTION ||
            aSubtype != Ci.nsIHttpActivityObserver.ACTIVITY_SUBTYPE_TRANSACTION_CLOSE)
          return;

        // Request should have been cancelled
        equal(aChannel.status, Cr.NS_BINDING_ABORTED);

        observerService.removeObserver(this);

        resolve();
      },
    });

    aInstall.addListener({
      onDownloadProgress(aDownloadProgressInstall) {
        aDownloadProgressInstall.cancel();
      },
    });

    aInstall.install();
  });
});


// Tests that an install can be restarted during onDownloadCancelled after being
// cancelled in mid-download
add_task(async function test_27() {
  prepare_test({ }, [
    "onNewInstall",
  ]);

  let url = "http://example.com/addons/test_install3.xpi";
  let aInstall = await AddonManager.getInstallForURL(url, "application/x-xpinstall");
  ensure_test_completed();
=======
  let install = await AddonManager.getInstallForURL(url);
>>>>>>> upstream-releases

  install.addListener({
    onDownloadProgress() {
<<<<<<< HEAD
      install.removeListener(this);
      install.cancel();
||||||| merged common ancestors
      aInstall.removeListener(this);
      aInstall.cancel();
=======
      install.cancel();
>>>>>>> upstream-releases
    },
  });

<<<<<<< HEAD
  let promise = AddonTestUtils.promiseInstallEvent("onDownloadCancelled");
  install.install();
  await promise;

  equal(install.state, AddonManager.STATE_CANCELLED);
||||||| merged common ancestors
  let install = await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadCancelled",
    ], resolve);
    aInstall.install();
  });
=======
  let promise = AddonTestUtils.promiseInstallEvent("onDownloadCancelled");
>>>>>>> upstream-releases

<<<<<<< HEAD
  promise = new Promise(resolve => {
    prepare_test({
      "addon1@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onDownloadStarted",
      "onDownloadEnded",
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);

    let file = install.file;
    install.install();
    notEqual(file.path, install.file.path);
    ok(!file.exists());
  });

  await Promise.all([
    promise,
    promiseWebExtensionStartup("addon1@tests.mozilla.org"),
||||||| merged common ancestors
  install = await new Promise(resolve => {
    prepare_test({
      "addon3@tests.mozilla.org": [
        ["onInstalling", false],
        "onInstalled",
      ],
    }, [
      "onDownloadStarted",
      "onDownloadEnded",
      "onInstallStarted",
      "onInstallEnded",
    ], resolve);

    let file = install.file;
    install.install();
    notEqual(file.path, install.file.path);
    ok(!file.exists());
  });

  ensure_test_completed();

  AddonManager.removeAddonListener(AddonListener);
  await install.addon.uninstall();
});

// Tests that an install with a matching compatibility override has appDisabled
// set correctly.
add_task(async function test_29() {
  Services.prefs.setBoolPref("extensions.getAddons.cache.enabled", true);

  prepare_test({ }, [
    "onNewInstall",
  ]);

  let url = "http://example.com/addons/test_install6.xpi";
  let install = await AddonManager.getInstallForURL(url, "application/x-xpinstall", null, "Addon Test 6", null, "1.0");
  ensure_test_completed();

  checkInstall(install, {
    version: "1.0",
    name: "Addon Test 6",
    state: AddonManager.STATE_AVAILABLE,
  });

  let activeInstalls = await AddonManager.getInstallsByTypes(null);
  equal(activeInstalls.length, 1);
  equal(activeInstalls[0], install);

  install = await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadStarted",
      "onDownloadEnded",
    ], install2 => {
      resolve(install2);
      return false;
    });
    install.install();
  });

  // ensure_test_completed();
  checkInstall(install, {
    state: AddonManager.STATE_DOWNLOADED,
  });
  checkAddon("addon6@tests.mozilla.org", install.addon, {
    isCompatible: false,
    appDisabled: true,
  });

  await new Promise(resolve => {
    prepare_test({}, [
      "onDownloadCancelled",
    ], resolve);
    install.cancel();
  });
});

// Tests that a multi-package XPI with no add-ons inside shows up as a
// corrupt file
add_task(async function test_30() {
  prepare_test({ }, [
    "onNewInstall",
=======
  install.install();
  await promise;

  equal(install.state, AddonManager.STATE_CANCELLED);
});

// Tests that an install can be restarted during onDownloadCancelled after being
// cancelled in mid-download
add_task(async function test_restart2() {
  let url = "http://example.com/addons/test_install1.xpi";
  let install = await AddonManager.getInstallForURL(url);

  equal(install.state, AddonManager.STATE_AVAILABLE);

  install.addListener({
    onDownloadProgress() {
      install.removeListener(this);
      install.cancel();
    },
  });

  let promise = AddonTestUtils.promiseInstallEvent("onDownloadCancelled");
  install.install();
  await promise;

  equal(install.state, AddonManager.STATE_CANCELLED);

  promise = expectEvents(
    {
      addonEvents: {
        "addon1@tests.mozilla.org": [
          { event: "onInstalling" },
          { event: "onInstalled" },
        ],
      },
      installEvents: [
        { event: "onDownloadStarted" },
        { event: "onDownloadEnded" },
        { event: "onInstallStarted" },
        { event: "onInstallEnded" },
      ],
    },
    () => {
      let file = install.file;
      install.install();
      notEqual(file.path, install.file.path);
      ok(!file.exists());
    }
  );

  await Promise.all([
    promise,
    promiseWebExtensionStartup("addon1@tests.mozilla.org"),
>>>>>>> upstream-releases
  ]);

<<<<<<< HEAD
  ensure_test_completed();

  clearListeners();
  await install.addon.uninstall();
||||||| merged common ancestors
  let install = await AddonManager.getInstallForFile(XPIS.test_install7);
  ensure_test_completed();

  checkInstall(install, {
    state: AddonManager.STATE_DOWNLOAD_FAILED,
    error: AddonManager.ERROR_CORRUPT_FILE,
  });
=======
  await install.addon.uninstall();
>>>>>>> upstream-releases
});
