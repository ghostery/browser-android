/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

const profileDir = gProfD.clone();
profileDir.append("extensions");

createAppInfo("xpcshell@tests.mozilla.org", "XPCShell", "1", "1");

const ADDONS = [
  {
    id: "addon1@experiments.addons.mozilla.org",
    dependencies: ["experiments.addon2"],
  },
  {
    id: "addon2@experiments.addons.mozilla.org",
    dependencies: ["experiments.addon3"],
  },
  {
    id: "addon3@experiments.addons.mozilla.org",
  },
  {
    id: "addon4@experiments.addons.mozilla.org",
  },
  {
    id: "addon5@experiments.addons.mozilla.org",
    dependencies: ["experiments.addon2"],
  },
];

let addonFiles = [];

let events = [];
<<<<<<< HEAD

||||||| merged common ancestors
=======

function promiseAddonStartup(id) {
  return new Promise(resolve => {
    const onBootstrapMethod = (event, { method, params }) => {
      if (method == "startup" && params.id == id) {
        AddonTestUtils.off("bootstrap-method", onBootstrapMethod);
        resolve();
      }
    };

    AddonTestUtils.on("bootstrap-method", onBootstrapMethod);
  });
}

>>>>>>> upstream-releases
add_task(async function setup() {
  await promiseStartupManager();

<<<<<<< HEAD
  const onBootstrapMethod = (event, {method, params}) => {
    if (method == "startup" || method == "shutdown") {
      events.push([method, params.id]);
    }
||||||| merged common ancestors
  let startupObserver = (subject, topic, data) => {
    events.push(["startup", data]);
  };
  let shutdownObserver = (subject, topic, data) => {
    events.push(["shutdown", data]);
=======
  const onBootstrapMethod = (event, { method, params }) => {
    if (method == "startup" || method == "shutdown") {
      events.push([method, params.id]);
    }
>>>>>>> upstream-releases
  };

  AddonTestUtils.on("bootstrap-method", onBootstrapMethod);
  registerCleanupFunction(() => {
    AddonTestUtils.off("bootstrap-method", onBootstrapMethod);
  });

  for (let addon of ADDONS) {
<<<<<<< HEAD
    let manifest = {
      applications: {gecko: {id: addon.id}},
      permissions: addon.dependencies,
    };

    addonFiles.push(await createTempWebExtensionFile({manifest}));
||||||| merged common ancestors
    Object.assign(addon, {
      targetApplications: [{
        id: "xpcshell@tests.mozilla.org",
        minVersion: "1",
        maxVersion: "1",
      }],
      version: "1.0",
      name: addon.id,
      bootstrap: true,
    });

    addonFiles.push(createTempXPIFile(addon, {"bootstrap.js": BOOTSTRAP}));
=======
    let manifest = {
      applications: { gecko: { id: addon.id } },
      permissions: addon.dependencies,
    };

    addonFiles.push(await createTempWebExtensionFile({ manifest }));
>>>>>>> upstream-releases
  }
});

add_task(async function() {
  deepEqual(events, [], "Should have no events");

  await promiseInstallFile(addonFiles[3]);

  deepEqual(events, [["startup", ADDONS[3].id]]);

  events.length = 0;

  await promiseInstallFile(addonFiles[0]);
  deepEqual(events, [], "Should have no events");

  await promiseInstallFile(addonFiles[1]);
  deepEqual(events, [], "Should have no events");

<<<<<<< HEAD
  await promiseInstallFile(addonFiles[2]);
||||||| merged common ancestors
  await promiseInstallAllFiles([addonFiles[2]]);
=======
  await Promise.all([
    promiseInstallFile(addonFiles[2]),
    promiseAddonStartup(ADDONS[0].id),
  ]);
>>>>>>> upstream-releases

  deepEqual(events, [
    ["startup", ADDONS[2].id],
    ["startup", ADDONS[1].id],
    ["startup", ADDONS[0].id],
  ]);

  events.length = 0;

<<<<<<< HEAD
  await promiseInstallFile(addonFiles[2]);
||||||| merged common ancestors
  await promiseInstallAllFiles([addonFiles[2]]);
=======
  await Promise.all([
    promiseInstallFile(addonFiles[2]),
    promiseAddonStartup(ADDONS[0].id),
  ]);
>>>>>>> upstream-releases

  deepEqual(events, [
    ["shutdown", ADDONS[0].id],
    ["shutdown", ADDONS[1].id],
    ["shutdown", ADDONS[2].id],

    ["startup", ADDONS[2].id],
    ["startup", ADDONS[1].id],
    ["startup", ADDONS[0].id],
  ]);

  events.length = 0;

  await promiseInstallFile(addonFiles[4]);

  deepEqual(events, [["startup", ADDONS[4].id]]);

  events.length = 0;

  await promiseRestartManager();

  deepEqual(events, [
    ["shutdown", ADDONS[4].id],
    ["shutdown", ADDONS[3].id],
    ["shutdown", ADDONS[0].id],
    ["shutdown", ADDONS[1].id],
    ["shutdown", ADDONS[2].id],

    ["startup", ADDONS[2].id],
    ["startup", ADDONS[1].id],
    ["startup", ADDONS[0].id],
    ["startup", ADDONS[3].id],
    ["startup", ADDONS[4].id],
  ]);
<<<<<<< HEAD

  await promiseShutdownManager();
});
||||||| merged common ancestors
});
=======
>>>>>>> upstream-releases

  await promiseShutdownManager();
});
