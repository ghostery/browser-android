/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

// Tests AddonManager.strictCompatibility and it's related preference,
// extensions.strictCompatibility, and the strictCompatibility option in
// install.rdf

// The `compatbile` array defines which of the tests below the add-on
// should be compatible in. It's pretty gross.
const ADDONS = [
  // Always compatible
  {
    manifest: {
      id: "addon1@tests.mozilla.org",
<<<<<<< HEAD
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      }],
    },
||||||| merged common ancestors
      version: "1.0",
      name: "Test 1",
      bootstrap: true,
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      }],
    },
    expected: {
      strictCompatibility: false,
    },
=======
      targetApplications: [
        {
          id: "xpcshell@tests.mozilla.org",
          minVersion: "1",
          maxVersion: "1",
        },
      ],
    },
>>>>>>> upstream-releases
    compatible: {
      nonStrict: true,
      strict: true,
    },
  },

  // Incompatible in strict compatibility mode
  {
    manifest: {
      id: "addon2@tests.mozilla.org",
<<<<<<< HEAD
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "0.7",
        maxVersion: "0.8",
      }],
    },
||||||| merged common ancestors
      version: "1.0",
      name: "Test 2",
      bootstrap: true,
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "0.7",
        maxVersion: "0.8",
      }],
    },
    expected: {
      strictCompatibility: false,
    },
=======
      targetApplications: [
        {
          id: "xpcshell@tests.mozilla.org",
          minVersion: "0.7",
          maxVersion: "0.8",
        },
      ],
    },
>>>>>>> upstream-releases
    compatible: {
      nonStrict: true,
      strict: false,
    },
  },

  // Opt-in to strict compatibility - always incompatible
<<<<<<< HEAD
  {
    manifest: {
      id: "addon3@tests.mozilla.org",
      strictCompatibility: true,
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "0.8",
        maxVersion: "0.9",
      }],
    },
||||||| merged common ancestors
  "addon4@tests.mozilla.org": {
    "install.rdf": {
      id: "addon4@tests.mozilla.org",
      version: "1.0",
      name: "Test 4",
      bootstrap: true,
      strictCompatibility: true,
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "0.8",
        maxVersion: "0.9",
      }],
    },
    expected: {
      strictCompatibility: true,
    },
=======
  {
    manifest: {
      id: "addon3@tests.mozilla.org",
      strictCompatibility: true,
      targetApplications: [
        {
          id: "xpcshell@tests.mozilla.org",
          minVersion: "0.8",
          maxVersion: "0.9",
        },
      ],
    },
>>>>>>> upstream-releases
    compatible: {
      nonStrict: false,
      strict: false,
    },
  },

  // Addon from the future - would be marked as compatibile-by-default,
  // but minVersion is higher than the app version
<<<<<<< HEAD
  {
    manifest: {
      id: "addon4@tests.mozilla.org",
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "3",
        maxVersion: "5",
      }],
    },
||||||| merged common ancestors
  "addon5@tests.mozilla.org": {
    "install.rdf": {
      id: "addon5@tests.mozilla.org",
      version: "1.0",
      name: "Test 5",
      bootstrap: true,
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "3",
        maxVersion: "5",
      }],
    },
    expected: {
      strictCompatibility: false,
    },
=======
  {
    manifest: {
      id: "addon4@tests.mozilla.org",
      targetApplications: [
        {
          id: "xpcshell@tests.mozilla.org",
          minVersion: "3",
          maxVersion: "5",
        },
      ],
    },
>>>>>>> upstream-releases
    compatible: {
      nonStrict: false,
      strict: false,
    },
  },

  // Dictionary - compatible even in strict compatibility mode
<<<<<<< HEAD
  {
    manifest: {
      id: "addon5@tests.mozilla.org",
      type: "dictionary",
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "0.8",
        maxVersion: "0.9",
      }],
    },
||||||| merged common ancestors
  "addon7@tests.mozilla.org": {
    "install.rdf": {
      id: "addon7@tests.mozilla.org",
      version: "1.0",
      name: "Test 7",
      type: "64",
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "0.8",
        maxVersion: "0.9",
      }],
    },
    expected: {
      strictCompatibility: false,
    },
=======
  {
    manifest: {
      id: "addon5@tests.mozilla.org",
      type: "dictionary",
      targetApplications: [
        {
          id: "xpcshell@tests.mozilla.org",
          minVersion: "0.8",
          maxVersion: "0.9",
        },
      ],
    },
>>>>>>> upstream-releases
    compatible: {
      nonStrict: true,
      strict: true,
    },
  },
];

async function checkCompatStatus(strict, index) {
  info(`Checking compat status for test ${index}\n`);

  equal(AddonManager.strictCompatibility, strict);

<<<<<<< HEAD
  for (let test of ADDONS) {
    let {id} = test.manifest;
    let addon = await promiseAddonByID(id);
    checkAddon(id, addon, {
      isCompatible: test.compatible[index],
      appDisabled: !test.compatible[index],
||||||| merged common ancestors
  let addons = await getAddons(IDS);
  for (let [id, addon] of Object.entries(ADDONS)) {
    checkAddon(id, addons.get(id), {
      ...addon.expected,
      isCompatible: addon.compatible[index],
      appDisabled: !addon.compatible[index],
=======
  for (let test of ADDONS) {
    let { id } = test.manifest;
    let addon = await promiseAddonByID(id);
    checkAddon(id, addon, {
      isCompatible: test.compatible[index],
      appDisabled: !test.compatible[index],
>>>>>>> upstream-releases
    });
  }
}

add_task(async function setup() {
  createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1.9.2");

<<<<<<< HEAD
  for (let addon of ADDONS) {
    let xpi = await createAddon(addon.manifest);
    await manuallyInstall(xpi, AddonTestUtils.profileExtensions, addon.manifest.id);
||||||| merged common ancestors
  for (let addon of Object.values(ADDONS)) {
    await promiseWriteInstallRDFForExtension(addon["install.rdf"], profileDir);
=======
  for (let addon of ADDONS) {
    let xpi = await createAddon(addon.manifest);
    await manuallyInstall(
      xpi,
      AddonTestUtils.profileExtensions,
      addon.manifest.id
    );
>>>>>>> upstream-releases
  }

  await promiseStartupManager();
});

add_task(async function test_1() {
  info("Test 1");
  Services.prefs.setBoolPref(PREF_EM_STRICT_COMPATIBILITY, false);
  await checkCompatStatus(false, "nonStrict");
  await promiseRestartManager();
  await checkCompatStatus(false, "nonStrict");
});

add_task(async function test_2() {
  info("Test 2");
  Services.prefs.setBoolPref(PREF_EM_STRICT_COMPATIBILITY, true);
  await checkCompatStatus(true, "strict");
  await promiseRestartManager();
  await checkCompatStatus(true, "strict");
});

const CHECK_COMPAT_ADDONS = [
  // Cannot be enabled as it has no target app info for the applciation
  {
    manifest: {
      id: "cc-addon1@tests.mozilla.org",
<<<<<<< HEAD
      targetApplications: [{
        id: "unknown@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      }],
||||||| merged common ancestors
      version: "1.0",
      name: "Test 1",
      bootstrap: true,
      targetApplications: [{
        id: "unknown@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      }],
=======
      targetApplications: [
        {
          id: "unknown@tests.mozilla.org",
          minVersion: "1",
          maxVersion: "1",
        },
      ],
>>>>>>> upstream-releases
    },
    compatible: false,
    canOverride: false,
  },

<<<<<<< HEAD

  // Always appears incompatible but can be enabled if compatibility checking is
  // disabled
  {
    manifest: {
||||||| merged common ancestors
  "cc-addon2@tests.mozilla.org": {
    "install.rdf": {
      // Always appears incompatible but can be enabled if compatibility checking is
      // disabled
=======
  // Always appears incompatible but can be enabled if compatibility checking is
  // disabled
  {
    manifest: {
>>>>>>> upstream-releases
      id: "cc-addon2@tests.mozilla.org",
<<<<<<< HEAD
      targetApplications: [{
        id: "toolkit@mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      }],
||||||| merged common ancestors
      version: "1.0",
      name: "Test 2",
      bootstrap: true,
      targetApplications: [{
        id: "toolkit@mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      }],
=======
      targetApplications: [
        {
          id: "toolkit@mozilla.org",
          minVersion: "1",
          maxVersion: "1",
        },
      ],
>>>>>>> upstream-releases
    },
    compatible: false,
    canOverride: true,
  },

<<<<<<< HEAD
  // Always compatible and enabled
  {
    manifest: {
      id: "cc-addon3@tests.mozilla.org",
      targetApplications: [{
        id: "toolkit@mozilla.org",
        minVersion: "1",
        maxVersion: "2",
      }],
||||||| merged common ancestors
  "cc-addon4@tests.mozilla.org": {
    "install.rdf": {
      // Always compatible and enabled
      id: "cc-addon4@tests.mozilla.org",
      version: "1.0",
      name: "Test 4",
      bootstrap: true,
      targetApplications: [{
        id: "toolkit@mozilla.org",
        minVersion: "1",
        maxVersion: "2",
      }],
=======
  // Always compatible and enabled
  {
    manifest: {
      id: "cc-addon3@tests.mozilla.org",
      targetApplications: [
        {
          id: "toolkit@mozilla.org",
          minVersion: "1",
          maxVersion: "2",
        },
      ],
>>>>>>> upstream-releases
    },
    compatible: true,
  },

<<<<<<< HEAD
  // Always compatible and enabled
  {
    manifest: {
      id: "cc-addon4@tests.mozilla.org",
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "3",
      }],
||||||| merged common ancestors
  "cc-addon5@tests.mozilla.org": {
    "install.rdf": {
      // Always compatible and enabled
      id: "cc-addon5@tests.mozilla.org",
      version: "1.0",
      name: "Test 5",
      bootstrap: true,
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "3",
      }],
=======
  // Always compatible and enabled
  {
    manifest: {
      id: "cc-addon4@tests.mozilla.org",
      targetApplications: [
        {
          id: "xpcshell@tests.mozilla.org",
          minVersion: "1",
          maxVersion: "3",
        },
      ],
>>>>>>> upstream-releases
    },
    compatible: true,
  },
];

async function checkCompatOverrides(overridden) {
<<<<<<< HEAD
  for (let test of CHECK_COMPAT_ADDONS) {
    let {id} = test.manifest;
    let addon = await promiseAddonByID(id);
    checkAddon(id, addon, {
      isCompatible: test.compatible,
      isActive: test.compatible || (overridden && test.canOverride),
||||||| merged common ancestors
  let addons = await getAddons(CHECK_COMPAT_IDS);

  for (let [id, addon] of Object.entries(CHECK_COMPAT_ADDONS)) {
    checkAddon(id, addons.get(id), {
      isCompatible: addon.compatible,
      isActive: addon.compatible || (overridden && addon.canOverride),
=======
  for (let test of CHECK_COMPAT_ADDONS) {
    let { id } = test.manifest;
    let addon = await promiseAddonByID(id);
    checkAddon(id, addon, {
      isCompatible: test.compatible,
      isActive: test.compatible || (overridden && test.canOverride),
>>>>>>> upstream-releases
    });
  }
}

var gIsNightly;

add_task(async function setupCheckCompat() {
  gIsNightly = isNightlyChannel();

  Services.prefs.setBoolPref(PREF_EM_STRICT_COMPATIBILITY, true);

  Object.assign(AddonTestUtils.appInfo, {
    version: "2.2.3",
    platformVersion: "2",
  });

<<<<<<< HEAD
  for (let addon of CHECK_COMPAT_ADDONS) {
    let {manifest} = addon;
    let xpi = await createAddon(manifest);
    await manuallyInstall(xpi, AddonTestUtils.profileExtensions, manifest.id);
||||||| merged common ancestors
  for (let addon of Object.values(CHECK_COMPAT_ADDONS)) {
    await promiseWriteInstallRDFForExtension(addon["install.rdf"], profileDir);
=======
  for (let addon of CHECK_COMPAT_ADDONS) {
    let { manifest } = addon;
    let xpi = await createAddon(manifest);
    await manuallyInstall(xpi, AddonTestUtils.profileExtensions, manifest.id);
>>>>>>> upstream-releases
  }
  await promiseRestartManager("2.2.3");
});

// Tests that with compatibility checking enabled we see the incompatible
// add-ons disabled
add_task(async function test_compat_overrides_1() {
  await checkCompatOverrides(false);
});

// Tests that with compatibility checking disabled we see the incompatible
// add-ons enabled
add_task(async function test_compat_overrides_2() {
  if (gIsNightly) {
    Services.prefs.setBoolPref("extensions.checkCompatibility.nightly", false);
  } else {
    Services.prefs.setBoolPref("extensions.checkCompatibility.2.2", false);
  }

  await promiseRestartManager();

  await checkCompatOverrides(true);
});

// Tests that with compatibility checking disabled we see the incompatible
// add-ons enabled.
add_task(async function test_compat_overrides_3() {
  if (!gIsNightly) {
    Services.prefs.setBoolPref("extensions.checkCompatibility.2.1a", false);
  }
  await promiseRestartManager("2.1a4");

  await checkCompatOverrides(true);
});

// Tests that with compatibility checking enabled we see the incompatible
// add-ons disabled.
add_task(async function test_compat_overrides_4() {
  if (gIsNightly) {
    Services.prefs.setBoolPref("extensions.checkCompatibility.nightly", true);
  } else {
    Services.prefs.setBoolPref("extensions.checkCompatibility.2.1a", true);
  }
  await promiseRestartManager();

  await checkCompatOverrides(false);
});

