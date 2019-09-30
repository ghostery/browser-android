/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

const PREF_DB_SCHEMA = "extensions.databaseSchema";

const profileDir = gProfD.clone();
profileDir.append("extensions");

createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "49");

<<<<<<< HEAD
add_task(async function run_tests() {
  const ID = "schema-change@tests.mozilla.org";

  const xpi1 = createTempWebExtensionFile({
    manifest: {
      name: "Test Add-on",
      version: "1.0",
      applications: {gecko: {id: ID}},
    },
  });

  const xpi2 = createTempWebExtensionFile({
    manifest: {
      name: "Test Add-on 2",
      version: "2.0",
      applications: {gecko: {id: ID}},
    },
  });

  let xpiPath = OS.Path.join(profileDir.path, `${ID}.xpi`);

  const TESTS = [
    {
      what: "Schema change with no application update reloads metadata.",
      expectedVersion: "2.0",
      action() {
        Services.prefs.setIntPref(PREF_DB_SCHEMA, 0);
      },
    },
    {
      what: "Application update with no schema change does not reload metadata.",
      expectedVersion: "1.0",
      action() {
        gAppInfo.version = "2";
      },
    },
    {
      what: "App update and a schema change causes a reload of the manifest.",
      expectedVersion: "2.0",
      action() {
        gAppInfo.version = "3";
        Services.prefs.setIntPref(PREF_DB_SCHEMA, 0);
      },
    },
    {
      what: "No schema change, no manifest reload.",
      expectedVersion: "1.0",
      action() {},
    },
    {
      what: "Modified timestamp on the XPI causes a reload of the manifest.",
      expectedVersion: "2.0",
      async action() {
        let stat = await OS.File.stat(xpiPath);
        await OS.File.setDates(xpiPath, stat.lastAccessDate,
                               stat.lastModificationDate.valueOf() + 60 * 1000);
      },
    },
  ];

  await promiseStartupManager();

  for (let test of TESTS) {
    info(test.what);
    await promiseInstallFile(xpi1);

    let addon = await promiseAddonByID(ID);
    notEqual(addon, null, "Got an addon object as expected");
    equal(addon.version, "1.0", "Got the expected version");

    await promiseShutdownManager();

    let orig = await OS.File.stat(xpiPath);

    xpi2.copyTo(profileDir, `${ID}.xpi`);

    // Make sure the timestamp is unchanged, so it is not re-scanned for that reason.
    await OS.File.setDates(xpiPath, orig.lastAccessDate, orig.lastModificationDate);

    await test.action();

    await promiseStartupManager();
||||||| merged common ancestors
/**
 *  Schema change with no application update reloads metadata.
 */
add_task(async function schema_change() {
  await promiseStartupManager();

  const ID = "schema-change@tests.mozilla.org";

  let xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on",
    version: "1.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1.9.2",
    }],
  });

  await promiseInstallFile(xpiFile);

  let addon = await promiseAddonByID(ID);

  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "1.0", "Got the expected version");

  await promiseShutdownManager();

  xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on 2",
    version: "2.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1.9.2",
    }],
  });

  Services.prefs.setIntPref(PREF_DB_SCHEMA, 0);

  let file = profileDir.clone();
  file.append(`${ID}.xpi`);

  // Make sure the timestamp is unchanged, so it is not re-scanned for that reason.
  let timestamp = file.lastModifiedTime;
  xpiFile.moveTo(profileDir, `${ID}.xpi`);

  file.lastModifiedTime = timestamp;

  await promiseStartupManager();

  addon = await promiseAddonByID(ID);
  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "2.0", "Got the expected version");

  let waitUninstall = promiseAddonEvent("onUninstalled");
  await addon.uninstall();
  await waitUninstall;
});

/**
 *  Application update with no schema change does not reload metadata.
 */
add_task(async function schema_change() {
  const ID = "schema-change@tests.mozilla.org";

  let xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on",
    version: "1.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "2",
    }],
  });

  await promiseInstallFile(xpiFile);

  let addon = await promiseAddonByID(ID);

  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "1.0", "Got the expected version");

  await promiseShutdownManager();

  xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on 2",
    version: "2.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "2",
    }],
  });

  gAppInfo.version = "2";
  let file = profileDir.clone();
  file.append(`${ID}.xpi`);

  // Make sure the timestamp is unchanged, so it is not re-scanned for that reason.
  let timestamp = file.lastModifiedTime;
  xpiFile.moveTo(profileDir, `${ID}.xpi`);

  file.lastModifiedTime = timestamp;

  await promiseStartupManager();

  addon = await promiseAddonByID(ID);
  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "1.0", "Got the expected version");

  let waitUninstall = promiseAddonEvent("onUninstalled");
  await addon.uninstall();
  await waitUninstall;
});

/**
 *  App update and a schema change causes a reload of the manifest.
 */
add_task(async function schema_change_app_update() {
  const ID = "schema-change@tests.mozilla.org";

  let xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on",
    version: "1.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "3",
    }],
  });

  await promiseInstallFile(xpiFile);

  let addon = await promiseAddonByID(ID);

  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "1.0", "Got the expected version");

  await promiseShutdownManager();

  xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on 2",
    version: "2.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "3",
    }],
  });

  gAppInfo.version = "3";
  Services.prefs.setIntPref(PREF_DB_SCHEMA, 0);

  let file = profileDir.clone();
  file.append(`${ID}.xpi`);

  // Make sure the timestamp is unchanged, so it is not re-scanned for that reason.
  let timestamp = file.lastModifiedTime;
  xpiFile.moveTo(profileDir, `${ID}.xpi`);

  file.lastModifiedTime = timestamp;

  await promiseStartupManager();

  addon = await promiseAddonByID(ID);
  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.appDisabled, false);
  equal(addon.version, "2.0", "Got the expected version");

  let waitUninstall = promiseAddonEvent("onUninstalled");
  await addon.uninstall();
  await waitUninstall;
});

/**
 *  No schema change, no manifest reload.
 */
add_task(async function schema_change() {
  const ID = "schema-change@tests.mozilla.org";

  let xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on",
    version: "1.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1.9.2",
    }],
  });

  await promiseInstallFile(xpiFile);

  let addon = await promiseAddonByID(ID);

  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "1.0", "Got the expected version");

  await promiseShutdownManager();

  xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on 2",
    version: "2.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1.9.2",
    }],
  });

  let file = profileDir.clone();
  file.append(`${ID}.xpi`);

  // Make sure the timestamp is unchanged, so it is not re-scanned for that reason.
  let timestamp = file.lastModifiedTime;
  xpiFile.moveTo(profileDir, `${ID}.xpi`);

  file.lastModifiedTime = timestamp;

  await promiseStartupManager();

  addon = await promiseAddonByID(ID);
  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "1.0", "Got the expected version");

  let waitUninstall = promiseAddonEvent("onUninstalled");
  await addon.uninstall();
  await waitUninstall;
});

/**
 *  Modified timestamp on the XPI causes a reload of the manifest.
 */
add_task(async function schema_change() {
  const ID = "schema-change@tests.mozilla.org";

  let xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on",
    version: "1.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1.9.2",
    }],
  });

  await promiseInstallFile(xpiFile);

  let addon = await promiseAddonByID(ID);

  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "1.0", "Got the expected version");

  await promiseShutdownManager();

  xpiFile = createTempXPIFile({
    id: ID,
    name: "Test Add-on 2",
    version: "2.0",
    bootstrap: true,
    targetApplications: [{
      id: "xpcshell@tests.mozilla.org",
      minVersion: "1",
      maxVersion: "1.9.2",
    }],
  });

  xpiFile.moveTo(profileDir, `${ID}.xpi`);

  let file = profileDir.clone();
  file.append(`${ID}.xpi`);

  // Set timestamp in the future so manifest is re-scanned.
  let timestamp = new Date(Date.now() + 60000);
  xpiFile.moveTo(profileDir, `${ID}.xpi`);

  file.lastModifiedTime = timestamp;

  await promiseStartupManager();
=======
add_task(async function run_tests() {
  // Fake installTelemetryInfo used in the addon installation,
  // to verify that they are preserved after the DB is updated
  // from the addon manifests.
  const fakeInstallTelemetryInfo = { source: "amo", method: "amWebAPI" };

  const ID = "schema-change@tests.mozilla.org";

  const xpi1 = createTempWebExtensionFile({
    manifest: {
      name: "Test Add-on",
      version: "1.0",
      applications: { gecko: { id: ID } },
    },
  });

  const xpi2 = createTempWebExtensionFile({
    manifest: {
      name: "Test Add-on 2",
      version: "2.0",
      applications: { gecko: { id: ID } },
    },
  });

  let xpiPath = OS.Path.join(profileDir.path, `${ID}.xpi`);

  const TESTS = [
    {
      what: "Schema change with no application update reloads metadata.",
      expectedVersion: "2.0",
      action() {
        Services.prefs.setIntPref(PREF_DB_SCHEMA, 0);
      },
    },
    {
      what:
        "Application update with no schema change does not reload metadata.",
      expectedVersion: "1.0",
      action() {
        gAppInfo.version = "2";
      },
    },
    {
      what: "App update and a schema change causes a reload of the manifest.",
      expectedVersion: "2.0",
      action() {
        gAppInfo.version = "3";
        Services.prefs.setIntPref(PREF_DB_SCHEMA, 0);
      },
    },
    {
      what: "No schema change, no manifest reload.",
      expectedVersion: "1.0",
      action() {},
    },
    {
      what: "Modified timestamp on the XPI causes a reload of the manifest.",
      expectedVersion: "2.0",
      async action() {
        let stat = await OS.File.stat(xpiPath);
        await OS.File.setDates(
          xpiPath,
          stat.lastAccessDate,
          stat.lastModificationDate.valueOf() + 60 * 1000
        );
      },
    },
  ];

  await promiseStartupManager();

  for (let test of TESTS) {
    info(test.what);
    await promiseInstallFile(xpi1, false, fakeInstallTelemetryInfo);

    let addon = await promiseAddonByID(ID);
    notEqual(addon, null, "Got an addon object as expected");
    equal(addon.version, "1.0", "Got the expected version");
    Assert.deepEqual(
      addon.installTelemetryInfo,
      fakeInstallTelemetryInfo,
      "Got the expected installTelemetryInfo after installing the addon"
    );

    await promiseShutdownManager();

    let orig = await OS.File.stat(xpiPath);

    xpi2.copyTo(profileDir, `${ID}.xpi`);

    // Make sure the timestamp is unchanged, so it is not re-scanned for that reason.
    await OS.File.setDates(
      xpiPath,
      orig.lastAccessDate,
      orig.lastModificationDate
    );

    await test.action();

    await promiseStartupManager();
>>>>>>> upstream-releases

<<<<<<< HEAD
    addon = await promiseAddonByID(ID);
    notEqual(addon, null, "Got an addon object as expected");
    equal(addon.version, test.expectedVersion, "Got the expected version");
||||||| merged common ancestors
  addon = await promiseAddonByID(ID);
  notEqual(addon, null, "Got an addon object as expected");
  equal(addon.version, "2.0", "Got the expected version");
=======
    addon = await promiseAddonByID(ID);
    notEqual(addon, null, "Got an addon object as expected");
    equal(addon.version, test.expectedVersion, "Got the expected version");
    Assert.deepEqual(
      addon.installTelemetryInfo,
      fakeInstallTelemetryInfo,
      "Got the expected installTelemetryInfo after rebuilding the DB"
    );
>>>>>>> upstream-releases

    await addon.uninstall();
  }
});
