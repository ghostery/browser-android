/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

/* eslint no-unused-vars: ["error", {vars: "local", args: "none"}] */

if (!_TEST_FILE[0].includes("toolkit/mozapps/extensions/test/xpcshell/")) {
  ok(
    false,
    "head_addons.js may not be loaded by tests outside of " +
      "the add-on manager component."
  );
}

const PREF_EM_CHECK_UPDATE_SECURITY = "extensions.checkUpdateSecurity";
const PREF_EM_STRICT_COMPATIBILITY = "extensions.strictCompatibility";
const PREF_GETADDONS_BYIDS = "extensions.getAddons.get.url";
const PREF_COMPAT_OVERRIDES = "extensions.getAddons.compatOverides.url";
const PREF_XPI_SIGNATURES_REQUIRED = "xpinstall.signatures.required";

<<<<<<< HEAD
const PREF_EM_CHECK_UPDATE_SECURITY   = "extensions.checkUpdateSecurity";
const PREF_EM_STRICT_COMPATIBILITY    = "extensions.strictCompatibility";
const PREF_GETADDONS_BYIDS               = "extensions.getAddons.get.url";
const PREF_COMPAT_OVERRIDES              = "extensions.getAddons.compatOverides.url";
const PREF_XPI_SIGNATURES_REQUIRED    = "xpinstall.signatures.required";

const PREF_DISABLE_SECURITY = ("security.turn_off_all_security_so_that_" +
                               "viruses_can_take_over_this_computer");

||||||| merged common ancestors
const PREF_EM_CHECK_UPDATE_SECURITY   = "extensions.checkUpdateSecurity";
const PREF_EM_STRICT_COMPATIBILITY    = "extensions.strictCompatibility";
const PREF_GETADDONS_BYIDS               = "extensions.getAddons.get.url";
const PREF_COMPAT_OVERRIDES              = "extensions.getAddons.compatOverides.url";
const PREF_XPI_SIGNATURES_REQUIRED    = "xpinstall.signatures.required";
const PREF_SYSTEM_ADDON_SET           = "extensions.systemAddonSet";
const PREF_SYSTEM_ADDON_UPDATE_URL    = "extensions.systemAddon.update.url";
const PREF_SYSTEM_ADDON_UPDATE_ENABLED = "extensions.systemAddon.update.enabled";
const PREF_DISABLE_SECURITY = ("security.turn_off_all_security_so_that_" +
                               "viruses_can_take_over_this_computer");

// Forcibly end the test if it runs longer than 15 minutes
const TIMEOUT_MS = 900000;

=======
const PREF_DISABLE_SECURITY =
  "security.turn_off_all_security_so_that_" +
  "viruses_can_take_over_this_computer";

>>>>>>> upstream-releases
// Maximum error in file modification times. Some file systems don't store
// modification times exactly. As long as we are closer than this then it
// still passes.
const MAX_TIME_DIFFERENCE = 3000;

// Time to reset file modified time relative to Date.now() so we can test that
// times are modified (10 hours old).
const MAKE_FILE_OLD_DIFFERENCE = 10 * 3600 * 1000;

var { AppConstants } = ChromeUtils.import(
  "resource://gre/modules/AppConstants.jsm"
);
var { FileUtils } = ChromeUtils.import("resource://gre/modules/FileUtils.jsm");
var { NetUtil } = ChromeUtils.import("resource://gre/modules/NetUtil.jsm");
var { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");
var { XPCOMUtils } = ChromeUtils.import(
  "resource://gre/modules/XPCOMUtils.jsm"
);
var { AddonRepository } = ChromeUtils.import(
  "resource://gre/modules/addons/AddonRepository.jsm"
);
var { OS, require } = ChromeUtils.import("resource://gre/modules/osfile.jsm");

var { AddonTestUtils, MockAsyncShutdown } = ChromeUtils.import(
  "resource://testing-common/AddonTestUtils.jsm"
);

ChromeUtils.defineModuleGetter(
  this,
  "Blocklist",
  "resource://gre/modules/Blocklist.jsm"
);
ChromeUtils.defineModuleGetter(
  this,
  "Extension",
  "resource://gre/modules/Extension.jsm"
);
ChromeUtils.defineModuleGetter(
  this,
  "ExtensionTestUtils",
  "resource://testing-common/ExtensionXPCShellUtils.jsm"
);
ChromeUtils.defineModuleGetter(
  this,
  "ExtensionTestCommon",
  "resource://testing-common/ExtensionTestCommon.jsm"
);
ChromeUtils.defineModuleGetter(
  this,
  "HttpServer",
  "resource://testing-common/httpd.js"
);
ChromeUtils.defineModuleGetter(
  this,
  "MockRegistrar",
  "resource://testing-common/MockRegistrar.jsm"
);
ChromeUtils.defineModuleGetter(
  this,
  "MockRegistry",
  "resource://testing-common/MockRegistry.jsm"
);
ChromeUtils.defineModuleGetter(
  this,
  "PromiseTestUtils",
  "resource://testing-common/PromiseTestUtils.jsm"
);
ChromeUtils.defineModuleGetter(
  this,
  "RemoteSettings",
  "resource://services-settings/remote-settings.js"
);
ChromeUtils.defineModuleGetter(
  this,
  "TestUtils",
  "resource://testing-common/TestUtils.jsm"
);
ChromeUtils.defineModuleGetter(
  this,
  "setTimeout",
  "resource://gre/modules/Timer.jsm"
);

XPCOMUtils.defineLazyServiceGetter(
  this,
  "aomStartup",
  "@mozilla.org/addons/addon-manager-startup;1",
  "amIAddonManagerStartup"
);

const {
  createAppInfo,
  createHttpServer,
  createTempWebExtensionFile,
  getFileForAddon,
  manuallyInstall,
  manuallyUninstall,
  overrideBuiltIns,
  promiseAddonEvent,
  promiseCompleteAllInstalls,
  promiseCompleteInstall,
  promiseConsoleOutput,
  promiseFindAddonUpdates,
  promiseInstallAllFiles,
  promiseInstallFile,
  promiseRestartManager,
  promiseSetExtensionModifiedTime,
  promiseShutdownManager,
  promiseStartupManager,
  promiseWebExtensionStartup,
  promiseWriteProxyFileToDir,
  registerDirectory,
  setExtensionModifiedTime,
  writeFilesToZip,
} = AddonTestUtils;

// WebExtension wrapper for ease of testing
ExtensionTestUtils.init(this);

AddonTestUtils.init(this);
AddonTestUtils.overrideCertDB();

XPCOMUtils.defineLazyGetter(
  this,
  "BOOTSTRAP_REASONS",
  () => AddonManagerPrivate.BOOTSTRAP_REASONS
);

function getReasonName(reason) {
  for (let key of Object.keys(BOOTSTRAP_REASONS)) {
    if (BOOTSTRAP_REASONS[key] == reason) {
      return key;
    }
  }
  throw new Error("This shouldn't happen.");
}

Object.defineProperty(this, "gAppInfo", {
  get() {
    return AddonTestUtils.appInfo;
  },
});

Object.defineProperty(this, "gAddonStartup", {
  get() {
    return AddonTestUtils.addonStartup.clone();
  },
});

Object.defineProperty(this, "gInternalManager", {
  get() {
    return AddonTestUtils.addonIntegrationService.QueryInterface(
      Ci.nsITimerCallback
    );
  },
});

Object.defineProperty(this, "gProfD", {
  get() {
    return AddonTestUtils.profileDir.clone();
  },
});

Object.defineProperty(this, "gTmpD", {
  get() {
    return AddonTestUtils.tempDir.clone();
  },
});

Object.defineProperty(this, "gUseRealCertChecks", {
  get() {
    return AddonTestUtils.useRealCertChecks;
  },
  set(val) {
    return (AddonTestUtils.useRealCertChecks = val);
  },
});

Object.defineProperty(this, "TEST_UNPACKED", {
  get() {
    return AddonTestUtils.testUnpacked;
  },
  set(val) {
    return (AddonTestUtils.testUnpacked = val);
  },
});

// We need some internal bits of AddonManager
var AMscope = ChromeUtils.import(
  "resource://gre/modules/AddonManager.jsm",
  null
);
var { AddonManager, AddonManagerInternal, AddonManagerPrivate } = AMscope;

const promiseAddonByID = AddonManager.getAddonByID;
const promiseAddonsByIDs = AddonManager.getAddonsByIDs;

var gPort = null;

<<<<<<< HEAD
const BOOTSTRAP_MONITOR_BOOTSTRAP_JS = `
  ChromeUtils.import("resource://xpcshell-data/BootstrapMonitor.jsm").monitor(this);
`;

// Listens to messages from bootstrap.js telling us what add-ons were started
// and stopped etc. and performs some sanity checks that only installed add-ons
// are started etc.
this.BootstrapMonitor = {
  inited: false,

  // Contain the current state of add-ons in the system
  installed: new Map(),
  started: new Map(),

  // Contain the last state of shutdown and uninstall calls for an add-on
  stopped: new Map(),
  uninstalled: new Map(),

  startupPromises: [],
  installPromises: [],

  restartfulIds: new Set(),

  init() {
    this.inited = true;
    Services.obs.addObserver(this, "bootstrapmonitor-event");
  },

  shutdownCheck() {
    if (!this.inited)
      return;

    Assert.equal(this.started.size, 0);
  },

  clear(id) {
    this.installed.delete(id);
    this.started.delete(id);
    this.stopped.delete(id);
    this.uninstalled.delete(id);
  },

  promiseAddonStartup(id) {
    return new Promise(resolve => {
      this.startupPromises.push(resolve);
    });
  },

  promiseAddonInstall(id) {
    return new Promise(resolve => {
      this.installPromises.push(resolve);
    });
  },

  checkMatches(cached, current) {
    Assert.notEqual(cached, undefined);
    Assert.equal(current.data.version, cached.data.version);
    Assert.equal(current.data.installPath, cached.data.installPath);
    Assert.ok(Services.io.newURI(current.data.resourceURI).equals(Services.io.newURI(cached.data.resourceURI)),
              `Resource URIs match: "${current.data.resourceURI}" == "${cached.data.resourceURI}"`);
  },

  checkAddonStarted(id, version = undefined) {
    let started = this.started.get(id);
    Assert.notEqual(started, undefined);
    if (version != undefined)
      Assert.equal(started.data.version, version);

    // Chrome should be registered by now
    let installPath = new FileUtils.File(started.data.installPath);
    let isRegistered = isManifestRegistered(installPath);
    Assert.ok(isRegistered);
  },

  checkAddonNotStarted(id) {
    Assert.ok(!this.started.has(id));
  },

  checkAddonInstalled(id, version = undefined) {
    const installed = this.installed.get(id);
    notEqual(installed, undefined);
    if (version !== undefined) {
      equal(installed.data.version, version);
    }
    return installed;
  },

  checkAddonNotInstalled(id) {
    Assert.ok(!this.installed.has(id));
  },

  observe(subject, topic, data) {
    let info = JSON.parse(data);
    let id = info.data.id;
    let installPath = new FileUtils.File(info.data.installPath);

    if (subject && subject.wrappedJSObject) {
      // NOTE: in some of the new tests, we need to received the real objects instead of
      // their JSON representations, but most of the current tests expect intallPath
      // and resourceURI to have been converted to strings.
      info.data = Object.assign({}, subject.wrappedJSObject.data, {
        installPath: info.data.installPath,
        resourceURI: info.data.resourceURI,
      });
    }

    // If this is the install event the add-ons shouldn't already be installed
    if (info.event == "install") {
      this.checkAddonNotInstalled(id);

      this.installed.set(id, info);

      for (let resolve of this.installPromises)
        resolve();
      this.installPromises = [];
    } else {
      this.checkMatches(this.installed.get(id), info);
    }

    // If this is the shutdown event than the add-on should already be started
    if (info.event == "shutdown") {
      this.checkMatches(this.started.get(id), info);

      this.started.delete(id);
      this.stopped.set(id, info);

      // Chrome should still be registered at this point
      let isRegistered = isManifestRegistered(installPath);
      Assert.ok(isRegistered);

      // XPIProvider doesn't bother unregistering chrome on app shutdown but
      // since we simulate restarts we must do so manually to keep the registry
      // consistent.
      if (info.reason == 2 /* APP_SHUTDOWN */)
        Components.manager.removeBootstrappedManifestLocation(installPath);
    } else {
      this.checkAddonNotStarted(id);
    }

    if (info.event == "uninstall") {
      // We currently support registering, but not unregistering,
      // restartful add-on manifests during xpcshell AOM "restarts".
      if (!this.restartfulIds.has(id)) {
        // Chrome should be unregistered at this point
        let isRegistered = isManifestRegistered(installPath);
        Assert.ok(!isRegistered);
      }

      this.installed.delete(id);
      this.uninstalled.set(id, info);
    } else if (info.event == "startup") {
      this.started.set(id, info);

      // Chrome should be registered at this point
      let isRegistered = isManifestRegistered(installPath);
      Assert.ok(isRegistered);

      for (let resolve of this.startupPromises)
        resolve();
      this.startupPromises = [];
    }
  },
};

AddonTestUtils.on("addon-manager-shutdown", () => BootstrapMonitor.shutdownCheck());

var SlightlyLessDodgyBootstrapMonitor = {
||||||| merged common ancestors
const BOOTSTRAP_MONITOR_BOOTSTRAP_JS = `
  ChromeUtils.import("resource://xpcshell-data/BootstrapMonitor.jsm").monitor(this);
`;


const EMPTY_BOOTSTRAP_JS = `
  function startup() {}
  function shutdown() {}
  function install() {}
  function uninstall() {}
`;

// Listens to messages from bootstrap.js telling us what add-ons were started
// and stopped etc. and performs some sanity checks that only installed add-ons
// are started etc.
this.BootstrapMonitor = {
  inited: false,

  // Contain the current state of add-ons in the system
  installed: new Map(),
  started: new Map(),

  // Contain the last state of shutdown and uninstall calls for an add-on
  stopped: new Map(),
  uninstalled: new Map(),

  startupPromises: [],
  installPromises: [],

  restartfulIds: new Set(),

  init() {
    this.inited = true;
    Services.obs.addObserver(this, "bootstrapmonitor-event");
  },

  shutdownCheck() {
    if (!this.inited)
      return;

    Assert.equal(this.started.size, 0);
  },

  clear(id) {
    this.installed.delete(id);
    this.started.delete(id);
    this.stopped.delete(id);
    this.uninstalled.delete(id);
  },

  promiseAddonStartup(id) {
    return new Promise(resolve => {
      this.startupPromises.push(resolve);
    });
  },

  promiseAddonInstall(id) {
    return new Promise(resolve => {
      this.installPromises.push(resolve);
    });
  },

  checkMatches(cached, current) {
    Assert.notEqual(cached, undefined);
    Assert.equal(current.data.version, cached.data.version);
    Assert.equal(current.data.installPath, cached.data.installPath);
    Assert.ok(Services.io.newURI(current.data.resourceURI).equals(Services.io.newURI(cached.data.resourceURI)),
              `Resource URIs match: "${current.data.resourceURI}" == "${cached.data.resourceURI}"`);
  },

  checkAddonStarted(id, version = undefined) {
    let started = this.started.get(id);
    Assert.notEqual(started, undefined);
    if (version != undefined)
      Assert.equal(started.data.version, version);

    // Chrome should be registered by now
    let installPath = new FileUtils.File(started.data.installPath);
    let isRegistered = isManifestRegistered(installPath);
    Assert.ok(isRegistered);
  },

  checkAddonNotStarted(id) {
    Assert.ok(!this.started.has(id));
  },

  checkAddonInstalled(id, version = undefined) {
    const installed = this.installed.get(id);
    notEqual(installed, undefined);
    if (version !== undefined) {
      equal(installed.data.version, version);
    }
    return installed;
  },

  checkAddonNotInstalled(id) {
    Assert.ok(!this.installed.has(id));
  },

  observe(subject, topic, data) {
    let info = JSON.parse(data);
    let id = info.data.id;
    let installPath = new FileUtils.File(info.data.installPath);

    if (subject && subject.wrappedJSObject) {
      // NOTE: in some of the new tests, we need to received the real objects instead of
      // their JSON representations, but most of the current tests expect intallPath
      // and resourceURI to have been converted to strings.
      info.data = Object.assign({}, subject.wrappedJSObject.data, {
        installPath: info.data.installPath,
        resourceURI: info.data.resourceURI,
      });
    }

    // If this is the install event the add-ons shouldn't already be installed
    if (info.event == "install") {
      this.checkAddonNotInstalled(id);

      this.installed.set(id, info);

      for (let resolve of this.installPromises)
        resolve();
      this.installPromises = [];
    } else {
      this.checkMatches(this.installed.get(id), info);
    }

    // If this is the shutdown event than the add-on should already be started
    if (info.event == "shutdown") {
      this.checkMatches(this.started.get(id), info);

      this.started.delete(id);
      this.stopped.set(id, info);

      // Chrome should still be registered at this point
      let isRegistered = isManifestRegistered(installPath);
      Assert.ok(isRegistered);

      // XPIProvider doesn't bother unregistering chrome on app shutdown but
      // since we simulate restarts we must do so manually to keep the registry
      // consistent.
      if (info.reason == 2 /* APP_SHUTDOWN */)
        Components.manager.removeBootstrappedManifestLocation(installPath);
    } else {
      this.checkAddonNotStarted(id);
    }

    if (info.event == "uninstall") {
      // We currently support registering, but not unregistering,
      // restartful add-on manifests during xpcshell AOM "restarts".
      if (!this.restartfulIds.has(id)) {
        // Chrome should be unregistered at this point
        let isRegistered = isManifestRegistered(installPath);
        Assert.ok(!isRegistered);
      }

      this.installed.delete(id);
      this.uninstalled.set(id, info);
    } else if (info.event == "startup") {
      this.started.set(id, info);

      // Chrome should be registered at this point
      let isRegistered = isManifestRegistered(installPath);
      Assert.ok(isRegistered);

      for (let resolve of this.startupPromises)
        resolve();
      this.startupPromises = [];
    }
  },
};

AddonTestUtils.on("addon-manager-shutdown", () => BootstrapMonitor.shutdownCheck());

var SlightlyLessDodgyBootstrapMonitor = {
=======
var BootstrapMonitor = {
>>>>>>> upstream-releases
  started: new Map(),
  stopped: new Map(),
  installed: new Map(),
  uninstalled: new Map(),

  init() {
    this.onEvent = this.onEvent.bind(this);

    AddonTestUtils.on("addon-manager-shutdown", this.onEvent);
    AddonTestUtils.on("bootstrap-method", this.onEvent);
  },

  shutdownCheck() {
    equal(
      this.started.size,
      0,
      "Should have no add-ons that were started but not shutdown"
    );
  },

  onEvent(msg, data) {
    switch (msg) {
      case "addon-manager-shutdown":
        this.shutdownCheck();
        break;
      case "bootstrap-method":
        this.onBootstrapMethod(data.method, data.params, data.reason);
        break;
    }
  },

  onBootstrapMethod(method, params, reason) {
    let { id } = params;

    info(
      `Bootstrap method ${method} for ${params.id} version ${params.version}`
    );

    if (method !== "install") {
      this.checkInstalled(id);
    }

    switch (method) {
      case "install":
        this.checkNotInstalled(id);
        this.installed.set(id, { reason, params });
        this.uninstalled.delete(id);
        break;
      case "startup":
        this.checkNotStarted(id);
        this.started.set(id, { reason, params });
        this.stopped.delete(id);
        break;
      case "shutdown":
        this.checkMatches("shutdown", "startup", params, this.started.get(id));
        this.checkStarted(id);
        this.stopped.set(id, { reason, params });
        this.started.delete(id);
        break;
      case "uninstall":
        this.checkMatches(
          "uninstall",
          "install",
          params,
          this.installed.get(id)
        );
        this.uninstalled.set(id, { reason, params });
        this.installed.delete(id);
        break;
      case "update":
        this.checkMatches("update", "install", params, this.installed.get(id));
        this.installed.set(id, { reason, params });
        break;
    }
  },

  clear(id) {
    this.installed.delete(id);
    this.started.delete(id);
    this.stopped.delete(id);
    this.uninstalled.delete(id);
  },

  checkMatches(method, lastMethod, params, { params: lastParams } = {}) {
    ok(
      lastParams,
      `Expecting matching ${lastMethod} call for add-on ${
        params.id
      } ${method} call`
    );

    if (method == "update") {
      equal(
        params.oldVersion,
        lastParams.version,
        "params.version should match last call"
      );
    } else {
      equal(
        params.version,
        lastParams.version,
        "params.version should match last call"
      );
    }

    if (method !== "update" && method !== "uninstall") {
      equal(
        params.resourceURI.spec,
        lastParams.resourceURI.spec,
        `params.resourceURI should match last call`
      );

      ok(
        params.resourceURI.equals(lastParams.resourceURI),
        `params.resourceURI should match: "${params.resourceURI.spec}" == "${
          lastParams.resourceURI.spec
        }"`
      );
    }
  },

  checkStarted(id, version = undefined) {
    let started = this.started.get(id);
    ok(started, `Should have seen startup method call for ${id}`);

    if (version !== undefined) {
      equal(started.params.version, version, "Expected version number");
    }
  },

  checkNotStarted(id) {
    ok(
      !this.started.has(id),
      `Should not have seen startup method call for ${id}`
    );
  },

  checkInstalled(id, version = undefined) {
    const installed = this.installed.get(id);
    ok(installed, `Should have seen install call for ${id}`);

    if (version !== undefined) {
      equal(installed.params.version, version, "Expected version number");
    }

    return installed;
  },

  checkNotInstalled(id) {
    ok(
      !this.installed.has(id),
      `Should not have seen install method call for ${id}`
    );
  },
};

function isNightlyChannel() {
  var channel = Services.prefs.getCharPref("app.update.channel", "default");

  return (
    channel != "aurora" &&
    channel != "beta" &&
    channel != "release" &&
    channel != "esr"
  );
}

async function restartWithLocales(locales) {
  Services.locale.requestedLocales = locales;
  await promiseRestartManager();
}

function delay(msec) {
  return new Promise(resolve => {
    setTimeout(resolve, msec);
  });
}

/**
 * Returns a map of Addon objects for installed add-ons with the given
 * IDs. The returned map contains a key for the ID of each add-on that
 * is found. IDs for add-ons which do not exist are not present in the
 * map.
 *
 * @param {sequence<string>} ids
 *        The list of add-on IDs to get.
 * @returns {Promise<string, Addon>}
 *        Map of add-ons that were found.
 */
async function getAddons(ids) {
  let addons = new Map();
  for (let addon of await AddonManager.getAddonsByIDs(ids)) {
    if (addon) {
      addons.set(addon.id, addon);
    }
  }
  return addons;
}

/**
 * Checks that the given add-on has the given expected properties.
 *
 * @param {string} id
 *        The id of the add-on.
 * @param {Addon?} addon
 *        The add-on object, or null if the add-on does not exist.
 * @param {object?} expected
 *        An object containing the expected values for properties of the
 *        add-on, or null if the add-on is expected not to exist.
 */
function checkAddon(id, addon, expected) {
  info(`Checking state of addon ${id}`);

  if (expected === null) {
    ok(!addon, `Addon ${id} should not exist`);
  } else {
    ok(addon, `Addon ${id} should exist`);
    for (let [key, value] of Object.entries(expected)) {
      if (value instanceof Ci.nsIURI) {
        equal(
          addon[key] && addon[key].spec,
          value.spec,
          `Expected value of addon.${key}`
        );
      } else {
        deepEqual(addon[key], value, `Expected value of addon.${key}`);
      }
    }
  }
}

/**
 * Tests that an add-on does appear in the crash report annotations, if
 * crash reporting is enabled. The test will fail if the add-on is not in the
 * annotation.
 * @param  aId
 *         The ID of the add-on
 * @param  aVersion
 *         The version of the add-on
 */
function do_check_in_crash_annotation(aId, aVersion) {
  if (!AppConstants.MOZ_CRASHREPORTER) {
    return;
  }

  if (!("Add-ons" in gAppInfo.annotations)) {
    Assert.ok(false, "Cannot find Add-ons entry in crash annotations");
    return;
  }

  let addons = gAppInfo.annotations["Add-ons"].split(",");
  Assert.ok(
    addons.includes(
      `${encodeURIComponent(aId)}:${encodeURIComponent(aVersion)}`
    )
  );
}

/**
 * Tests that an add-on does not appear in the crash report annotations, if
 * crash reporting is enabled. The test will fail if the add-on is in the
 * annotation.
 * @param  aId
 *         The ID of the add-on
 * @param  aVersion
 *         The version of the add-on
 */
function do_check_not_in_crash_annotation(aId, aVersion) {
  if (!AppConstants.MOZ_CRASHREPORTER) {
    return;
  }

  if (!("Add-ons" in gAppInfo.annotations)) {
    Assert.ok(true);
    return;
  }

  let addons = gAppInfo.annotations["Add-ons"].split(",");
  Assert.ok(
    !addons.includes(
      `${encodeURIComponent(aId)}:${encodeURIComponent(aVersion)}`
    )
  );
}

function do_get_file_hash(aFile, aAlgorithm) {
  if (!aAlgorithm) {
    aAlgorithm = "sha1";
  }

  let crypto = Cc["@mozilla.org/security/hash;1"].createInstance(
    Ci.nsICryptoHash
  );
  crypto.initWithString(aAlgorithm);
  let fis = Cc["@mozilla.org/network/file-input-stream;1"].createInstance(
    Ci.nsIFileInputStream
  );
  fis.init(aFile, -1, -1, false);
  crypto.updateFromStream(fis, aFile.fileSize);

  // return the two-digit hexadecimal code for a byte
  let toHexString = charCode => ("0" + charCode.toString(16)).slice(-2);

  let binary = crypto.finish(false);
  let hash = Array.from(binary, c => toHexString(c.charCodeAt(0)));
  return aAlgorithm + ":" + hash.join("");
}

/**
 * Returns an extension uri spec
 *
 * @param  aProfileDir
 *         The extension install directory
 * @return a uri spec pointing to the root of the extension
 */
function do_get_addon_root_uri(aProfileDir, aId) {
  let path = aProfileDir.clone();
  path.append(aId);
  if (!path.exists()) {
    path.leafName += ".xpi";
    return "jar:" + Services.io.newFileURI(path).spec + "!/";
  }
  return Services.io.newFileURI(path).spec;
}

function do_get_expected_addon_name(aId) {
  if (TEST_UNPACKED) {
    return aId;
  }
  return aId + ".xpi";
}

/**
 * Returns the file containing the add-on. For packed add-ons, this is
 * an XPI file. For unpacked add-ons, it is the add-on's root directory.
 *
 * @param {Addon} addon
 * @returns {nsIFile}
 */
function getAddonFile(addon) {
  let uri = addon.getResourceURI("");
  if (uri instanceof Ci.nsIJARURI) {
    uri = uri.JARFile;
  }
  return uri.QueryInterface(Ci.nsIFileURL).file;
}

/**
 * Check that an array of actual add-ons is the same as an array of
 * expected add-ons.
 *
 * @param  aActualAddons
 *         The array of actual add-ons to check.
 * @param  aExpectedAddons
 *         The array of expected add-ons to check against.
 * @param  aProperties
 *         An array of properties to check.
 */
function do_check_addons(aActualAddons, aExpectedAddons, aProperties) {
  Assert.notEqual(aActualAddons, null);
  Assert.equal(aActualAddons.length, aExpectedAddons.length);
  for (let i = 0; i < aActualAddons.length; i++) {
    do_check_addon(aActualAddons[i], aExpectedAddons[i], aProperties);
  }
}

/**
 * Check that the actual add-on is the same as the expected add-on.
 *
 * @param  aActualAddon
 *         The actual add-on to check.
 * @param  aExpectedAddon
 *         The expected add-on to check against.
 * @param  aProperties
 *         An array of properties to check.
 */
function do_check_addon(aActualAddon, aExpectedAddon, aProperties) {
  Assert.notEqual(aActualAddon, null);

  aProperties.forEach(function(aProperty) {
    let actualValue = aActualAddon[aProperty];
    let expectedValue = aExpectedAddon[aProperty];

    // Check that all undefined expected properties are null on actual add-on
    if (!(aProperty in aExpectedAddon)) {
      if (actualValue !== undefined && actualValue !== null) {
        do_throw(
          "Unexpected defined/non-null property for add-on " +
            aExpectedAddon.id +
            " (addon[" +
            aProperty +
            "] = " +
            actualValue.toSource() +
            ")"
        );
      }

      return;
    } else if (expectedValue && !actualValue) {
      do_throw(
        "Missing property for add-on " +
          aExpectedAddon.id +
          ": expected addon[" +
          aProperty +
          "] = " +
          expectedValue
      );
      return;
    }

    switch (aProperty) {
      case "creator":
        do_check_author(actualValue, expectedValue);
        break;

      case "developers":
        Assert.equal(actualValue.length, expectedValue.length);
        for (let i = 0; i < actualValue.length; i++) {
          do_check_author(actualValue[i], expectedValue[i]);
        }
        break;

      case "screenshots":
        Assert.equal(actualValue.length, expectedValue.length);
        for (let i = 0; i < actualValue.length; i++) {
          do_check_screenshot(actualValue[i], expectedValue[i]);
        }
        break;

      case "sourceURI":
        Assert.equal(actualValue.spec, expectedValue);
        break;

      case "updateDate":
        Assert.equal(actualValue.getTime(), expectedValue.getTime());
        break;

      case "compatibilityOverrides":
        Assert.equal(actualValue.length, expectedValue.length);
        for (let i = 0; i < actualValue.length; i++) {
          do_check_compatibilityoverride(actualValue[i], expectedValue[i]);
        }
        break;

      case "icons":
        do_check_icons(actualValue, expectedValue);
        break;

      default:
        if (actualValue !== expectedValue) {
          do_throw(
            "Failed for " +
              aProperty +
              " for add-on " +
              aExpectedAddon.id +
              " (" +
              actualValue +
              " === " +
              expectedValue +
              ")"
          );
        }
    }
  });
}

/**
 * Check that the actual author is the same as the expected author.
 *
 * @param  aActual
 *         The actual author to check.
 * @param  aExpected
 *         The expected author to check against.
 */
function do_check_author(aActual, aExpected) {
  Assert.equal(aActual.toString(), aExpected.name);
  Assert.equal(aActual.name, aExpected.name);
  Assert.equal(aActual.url, aExpected.url);
}

/**
 * Check that the actual screenshot is the same as the expected screenshot.
 *
 * @param  aActual
 *         The actual screenshot to check.
 * @param  aExpected
 *         The expected screenshot to check against.
 */
function do_check_screenshot(aActual, aExpected) {
  Assert.equal(aActual.toString(), aExpected.url);
  Assert.equal(aActual.url, aExpected.url);
  Assert.equal(aActual.width, aExpected.width);
  Assert.equal(aActual.height, aExpected.height);
  Assert.equal(aActual.thumbnailURL, aExpected.thumbnailURL);
  Assert.equal(aActual.thumbnailWidth, aExpected.thumbnailWidth);
  Assert.equal(aActual.thumbnailHeight, aExpected.thumbnailHeight);
  Assert.equal(aActual.caption, aExpected.caption);
}

/**
 * Check that the actual compatibility override is the same as the expected
 * compatibility override.
 *
 * @param  aAction
 *         The actual compatibility override to check.
 * @param  aExpected
 *         The expected compatibility override to check against.
 */
function do_check_compatibilityoverride(aActual, aExpected) {
  Assert.equal(aActual.type, aExpected.type);
  Assert.equal(aActual.minVersion, aExpected.minVersion);
  Assert.equal(aActual.maxVersion, aExpected.maxVersion);
  Assert.equal(aActual.appID, aExpected.appID);
  Assert.equal(aActual.appMinVersion, aExpected.appMinVersion);
  Assert.equal(aActual.appMaxVersion, aExpected.appMaxVersion);
}

function do_check_icons(aActual, aExpected) {
  for (var size in aExpected) {
    Assert.equal(aActual[size], aExpected[size]);
  }
}

function isThemeInAddonsList(aDir, aId) {
  return AddonTestUtils.addonsList.hasTheme(aDir, aId);
}

function isExtensionInBootstrappedList(aDir, aId) {
  return AddonTestUtils.addonsList.hasExtension(aDir, aId);
}

/**
 * Writes a manifest.json manifest into an extension using the properties passed
 * in a JS object.
 *
 * @param   aManifest
 *          The data to write
 * @param   aDir
 *          The install directory to add the extension to
 * @param   aId
 *          An optional string to override the default installation aId
 * @return  A file pointing to where the extension was installed
 */
function promiseWriteWebManifestForExtension(
  aData,
  aDir,
  aId = aData.applications.gecko.id
) {
  let files = {
    "manifest.json": JSON.stringify(aData),
  };
  return AddonTestUtils.promiseWriteFilesToExtension(aDir.path, aId, files);
}

function hasFlag(aBits, aFlag) {
  return (aBits & aFlag) != 0;
}

class EventChecker {
  constructor(options) {
    this.expectedEvents = options.addonEvents || {};
    this.expectedInstalls = options.installEvents || null;

    this.finished = new Promise(resolve => {
      this.resolveFinished = resolve;
    });

    AddonManager.addAddonListener(this);
    if (this.expectedInstalls) {
      AddonManager.addInstallListener(this);
    }
  }

  cleanup() {
    AddonManager.removeAddonListener(this);
    if (this.expectedInstalls) {
      AddonManager.removeInstallListener(this);
    }
  }

  checkValue(prop, value, flagName) {
    if (Array.isArray(flagName)) {
      let names = flagName.map(name => `AddonManager.${name}`);

      Assert.ok(
        flagName.map(name => AddonManager[name]).includes(value),
        `${prop} value \`${value}\` should be one of [${names.join(", ")}`
      );
    } else {
      Assert.equal(
        value,
        AddonManager[flagName],
        `${prop} should have value AddonManager.${flagName}`
      );
    }
  }

  checkFlag(prop, value, flagName) {
    Assert.equal(
      value & AddonManager[flagName],
      AddonManager[flagName],
      `${prop} should have flag AddonManager.${flagName}`
    );
  }

  checkNoFlag(prop, value, flagName) {
    Assert.ok(
      !(value & AddonManager[flagName]),
      `${prop} should not have flag AddonManager.${flagName}`
    );
  }

  checkComplete() {
    if (this.expectedInstalls && this.expectedInstalls.length) {
      return;
    }

    if (Object.values(this.expectedEvents).some(events => events.length)) {
      return;
    }

    info("Test complete");
    this.cleanup();
    this.resolveFinished();
  }

  ensureComplete() {
    this.cleanup();

    for (let [id, events] of Object.entries(this.expectedEvents)) {
      Assert.equal(
        events.length,
        0,
        `Should have no remaining events for ${id}`
      );
    }
    if (this.expectedInstalls) {
      Assert.deepEqual(
        this.expectedInstalls,
        [],
        "Should have no remaining install events"
      );
    }
  }

  // Add-on listener events
  getExpectedEvent(aId) {
    if (!(aId in this.expectedEvents)) {
      return null;
    }

    let events = this.expectedEvents[aId];
    Assert.ok(events.length > 0, `Should be expecting events for ${aId}`);

    return events.shift();
  }

  checkAddonEvent(event, addon, details = {}) {
    info(`Got event "${event}" for add-on ${addon.id}`);

    if ("requiresRestart" in details) {
      Assert.equal(
        details.requiresRestart,
        false,
        "requiresRestart should always be false"
      );
    }

    let expected = this.getExpectedEvent(addon.id);
    if (!expected) {
      return undefined;
    }

    Assert.equal(
      expected.event,
      event,
      `Expecting event "${expected.event}" got "${event}"`
    );

    for (let prop of ["properties"]) {
      if (prop in expected) {
        Assert.deepEqual(
          expected[prop],
          details[prop],
          `Expected value for ${prop}`
        );
      }
    }

    this.checkComplete();

    if ("returnValue" in expected) {
      return expected.returnValue;
    }
    return undefined;
  }

  onPropertyChanged(addon, properties) {
    return this.checkAddonEvent("onPropertyChanged", addon, { properties });
  }

  onEnabling(addon, requiresRestart) {
    let result = this.checkAddonEvent("onEnabling", addon, { requiresRestart });

    this.checkNoFlag("addon.permissions", addon.permissions, "PERM_CAN_ENABLE");

    return result;
  }

  onEnabled(addon) {
    let result = this.checkAddonEvent("onEnabled", addon);

    this.checkNoFlag("addon.permissions", addon.permissions, "PERM_CAN_ENABLE");

    return result;
  }

  onDisabling(addon, requiresRestart) {
    let result = this.checkAddonEvent("onDisabling", addon, {
      requiresRestart,
    });

    this.checkNoFlag(
      "addon.permissions",
      addon.permissions,
      "PERM_CAN_DISABLE"
    );
    return result;
  }

  onDisabled(addon) {
    let result = this.checkAddonEvent("onDisabled", addon);

    this.checkNoFlag(
      "addon.permissions",
      addon.permissions,
      "PERM_CAN_DISABLE"
    );

    return result;
  }

  onInstalling(addon, requiresRestart) {
    return this.checkAddonEvent("onInstalling", addon, { requiresRestart });
  }

  onInstalled(addon) {
    return this.checkAddonEvent("onInstalled", addon);
  }

  onUninstalling(addon, requiresRestart) {
    return this.checkAddonEvent("onUninstalling", addon);
  }

  onUninstalled(addon) {
    return this.checkAddonEvent("onUninstalled", addon);
  }

  onOperationCancelled(addon) {
    return this.checkAddonEvent("onOperationCancelled", addon);
  }

  // Install listener events.
  checkInstall(event, install, details = {}) {
    info(`Got install event "${event}"`);

    let expected = this.expectedInstalls.shift();
    Assert.ok(expected, "Should be expecting install event");

    Assert.equal(
      expected.event,
      event,
      "Should be expecting onExternalInstall event"
    );

    if ("state" in details) {
      this.checkValue("install.state", install.state, details.state);
    }

    this.checkComplete();

    if ("callback" in expected) {
      expected.callback(install);
    }

    if ("returnValue" in expected) {
      return expected.returnValue;
    }
    return undefined;
  }

  onNewInstall(install) {
    let result = this.checkInstall("onNewInstall", install, {
      state: ["STATE_DOWNLOADED", "STATE_DOWNLOAD_FAILED", "STATE_AVAILABLE"],
    });

    if (install.state != AddonManager.STATE_DOWNLOAD_FAILED) {
      Assert.equal(install.error, 0, "Should have no error");
    } else {
      Assert.notEqual(install.error, 0, "Should have error");
    }

    return result;
  }

  onDownloadStarted(install) {
    return this.checkInstall("onDownloadStarted", install, {
      state: "STATE_DOWNLOADING",
      error: 0,
    });
  }

  onDownloadEnded(install) {
    return this.checkInstall("onDownloadEnded", install, {
      state: "STATE_DOWNLOADED",
      error: 0,
    });
  }

  onDownloadFailed(install) {
    return this.checkInstall("onDownloadFailed", install, {
      state: "STATE_FAILED",
    });
  }

  onDownloadCancelled(install) {
    return this.checkInstall("onDownloadCancelled", install, {
      state: "STATE_CANCELLED",
      error: 0,
    });
  }

  onInstallStarted(install) {
    return this.checkInstall("onInstallStarted", install, {
      state: "STATE_INSTALLING",
      error: 0,
    });
  }

  onInstallEnded(install, newAddon) {
    return this.checkInstall("onInstallEnded", install, {
      state: "STATE_INSTALLED",
      error: 0,
    });
  }

  onInstallFailed(install) {
    return this.checkInstall("onInstallFailed", install, {
      state: "STATE_FAILED",
    });
  }

  onInstallCancelled(install) {
    // If the install was cancelled by a listener returning false from
    // onInstallStarted, then the state will revert to STATE_DOWNLOADED.
<<<<<<< HEAD
    let possibleStates = [AddonManager.STATE_CANCELLED,
                          AddonManager.STATE_DOWNLOADED];
    Assert.ok(possibleStates.includes(install.state));
    Assert.equal(install.error, 0);
    Assert.equal("onInstallCancelled", getExpectedInstall(install.addon));
    return check_test_completed(arguments);
  },

  onExternalInstall(aAddon, existingAddon, aRequiresRestart) {
    Assert.equal("onExternalInstall", getExpectedInstall(aAddon));
    Assert.ok(!aRequiresRestart);
    return check_test_completed(arguments);
  },
};

function hasFlag(aBits, aFlag) {
  return (aBits & aFlag) != 0;
}

// Just a wrapper around setting the expected events
function prepare_test(aExpectedEvents, aExpectedInstalls, aNext) {
  AddonManager.addAddonListener(AddonListener);
  AddonManager.addInstallListener(InstallListener);

  gExpectedInstalls = aExpectedInstalls;
  gExpectedEvents = aExpectedEvents;
  gNext = aNext;
}

function clearListeners() {
  AddonManager.removeAddonListener(AddonListener);
  AddonManager.removeInstallListener(InstallListener);
}

function end_test() {
  clearListeners();
}

// Checks if all expected events have been seen and if so calls the callback
function check_test_completed(aArgs) {
  if (!gNext)
    return undefined;

  if (gExpectedInstalls instanceof Array &&
      gExpectedInstalls.length > 0)
    return undefined;

  for (let id in gExpectedInstalls) {
    let installList = gExpectedInstalls[id];
    if (installList.length > 0)
      return undefined;
||||||| merged common ancestors
    let possibleStates = [AddonManager.STATE_CANCELLED,
                          AddonManager.STATE_DOWNLOADED];
    Assert.ok(possibleStates.includes(install.state));
    Assert.equal(install.error, 0);
    Assert.equal("onInstallCancelled", getExpectedInstall(install.addon));
    return check_test_completed(arguments);
  },

  onExternalInstall(aAddon, existingAddon, aRequiresRestart) {
    Assert.equal("onExternalInstall", getExpectedInstall(aAddon));
    Assert.ok(!aRequiresRestart);
    return check_test_completed(arguments);
  },
};

function hasFlag(aBits, aFlag) {
  return (aBits & aFlag) != 0;
}

// Just a wrapper around setting the expected events
function prepare_test(aExpectedEvents, aExpectedInstalls, aNext) {
  AddonManager.addAddonListener(AddonListener);
  AddonManager.addInstallListener(InstallListener);

  gExpectedInstalls = aExpectedInstalls;
  gExpectedEvents = aExpectedEvents;
  gNext = aNext;
}

function end_test() {
  AddonManager.removeAddonListener(AddonListener);
  AddonManager.removeInstallListener(InstallListener);
}

// Checks if all expected events have been seen and if so calls the callback
function check_test_completed(aArgs) {
  if (!gNext)
    return undefined;

  if (gExpectedInstalls instanceof Array &&
      gExpectedInstalls.length > 0)
    return undefined;

  for (let id in gExpectedInstalls) {
    let installList = gExpectedInstalls[id];
    if (installList.length > 0)
      return undefined;
=======
    return this.checkInstall("onInstallCancelled", install, {
      state: ["STATE_CANCELED", "STATE_DOWNLOADED"],
      error: 0,
    });
>>>>>>> upstream-releases
  }

  onExternalInstall(addon, existingAddon, requiresRestart) {
    let expected = this.expectedInstalls.shift();
    Assert.ok(expected, "Should be expecting install event");

    Assert.equal(
      expected.event,
      "onExternalInstall",
      "Should be expecting onExternalInstall event"
    );
    Assert.ok(!requiresRestart, "Should never require restart");

    this.checkComplete();
    if ("returnValue" in expected) {
      return expected.returnValue;
    }
    return undefined;
  }
}

/**
 * Run the giving callback function, and expect the given set of add-on
 * and install listener events to be emitted, and returns a promise
 * which resolves when they have all been observed.
 *
 * If `callback` returns a promise, all events are expected to be
 * observed by the time the promise resolves. If not, simply waits for
 * all events to be observed before resolving the returned promise.
 *
 * @param {object} details
 * @param {function} callback
 * @returns {Promise}
 */
/* exported expectEvents */
async function expectEvents(details, callback) {
  let checker = new EventChecker(details);

  try {
    let result = callback();

    if (
      result &&
      typeof result === "object" &&
      typeof result.then === "function"
    ) {
      result = await result;
      checker.ensureComplete();
    } else {
      await checker.finished;
    }

    return result;
  } catch (e) {
    do_throw(e);
    return undefined;
  }
}

const EXTENSIONS_DB = "extensions.json";
var gExtensionsJSON = gProfD.clone();
gExtensionsJSON.append(EXTENSIONS_DB);

async function promiseInstallWebExtension(aData) {
  let addonFile = createTempWebExtensionFile(aData);

  let { addon } = await promiseInstallFile(addonFile);
  return addon;
}

// By default use strict compatibility
Services.prefs.setBoolPref("extensions.strictCompatibility", true);

// Ensure signature checks are enabled by default
Services.prefs.setBoolPref(PREF_XPI_SIGNATURES_REQUIRED, true);

Services.prefs.setBoolPref("extensions.legacy.enabled", true);

// Copies blocklistFile (an nsIFile) to gProfD/blocklist.xml.
function copyBlocklistToProfile(blocklistFile) {
  var dest = gProfD.clone();
  dest.append("blocklist.xml");
  if (dest.exists()) {
    dest.remove(false);
  }
  blocklistFile.copyTo(gProfD, "blocklist.xml");
  dest.lastModifiedTime = Date.now();
}

<<<<<<< HEAD
// Make sure that a given path does not exist
function pathShouldntExist(file) {
  if (file.exists()) {
    do_throw(`Test cleanup: path ${file.path} exists when it should not`);
  }
||||||| merged common ancestors
// Throw a failure and attempt to abandon the test if it looks like it is going
// to timeout
function timeout() {
  timer = null;
  do_throw("Test ran longer than " + TIMEOUT_MS + "ms");

  // Attempt to bail out of the test
  do_test_finished();
}

var timer = Cc["@mozilla.org/timer;1"].createInstance(Ci.nsITimer);
timer.init(timeout, TIMEOUT_MS, Ci.nsITimer.TYPE_ONE_SHOT);

// Make sure that a given path does not exist
function pathShouldntExist(file) {
  if (file.exists()) {
    do_throw(`Test cleanup: path ${file.path} exists when it should not`);
  }
=======
async function mockGfxBlocklistItemsFromDisk(path) {
  Cu.importGlobalProperties(["fetch"]);
  let response = await fetch(Services.io.newFileURI(do_get_file(path)).spec);
  let json = await response.json();
  return mockGfxBlocklistItems(json);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// Wrap a function (typically a callback) to catch and report exceptions
function do_exception_wrap(func) {
  return function() {
    try {
      func.apply(null, arguments);
    } catch (e) {
      do_report_unexpected_exception(e);
    }
  };
||||||| merged common ancestors
registerCleanupFunction(function addon_cleanup() {
  if (timer)
    timer.cancel();
});

// Wrap a function (typically a callback) to catch and report exceptions
function do_exception_wrap(func) {
  return function() {
    try {
      func.apply(null, arguments);
    } catch (e) {
      do_report_unexpected_exception(e);
    }
  };
=======
async function mockGfxBlocklistItems(items) {
  const { generateUUID } = Cc["@mozilla.org/uuid-generator;1"].getService(
    Ci.nsIUUIDGenerator
  );
  let bsPass = ChromeUtils.import("resource://gre/modules/Blocklist.jsm", null);
  const client = RemoteSettings(
    Services.prefs.getCharPref("services.blocklist.gfx.collection"),
    { bucketNamePref: "services.blocklist.bucket" }
  );
  const collection = await client.openCollection();
  await collection.clear();
  await collection.loadDump(
    items.map(item => {
      if (item.id && item.last_modified) {
        return item;
      }
      return Object.assign(
        {
          id: generateUUID()
            .toString()
            .replace(/[{}]/g, ""),
          last_modified: Date.now(),
        },
        item
      );
    })
  );
  let rv = await bsPass.GfxBlocklistRS.checkForEntries();
  return rv;
>>>>>>> upstream-releases
}

/**
 * Change the schema version of the JSON extensions database
 */
async function changeXPIDBVersion(aNewVersion) {
  let json = await loadJSON(gExtensionsJSON.path);
  json.schemaVersion = aNewVersion;
  await saveJSON(json, gExtensionsJSON.path);
}

/**
 * Load a file into a string
 */
async function loadFile(aFile) {
  let buffer = await OS.File.read(aFile);
  return new TextDecoder().decode(buffer);
}

/**
 * Raw load of a JSON file
 */
async function loadJSON(aFile) {
  let data = await loadFile(aFile);
  info("Loaded JSON file " + aFile);
  return JSON.parse(data);
}

/**
 * Raw save of a JSON blob to file
 */
async function saveJSON(aData, aFile) {
  info("Starting to save JSON file " + aFile);
  await OS.File.writeAtomic(
    aFile,
    new TextEncoder().encode(JSON.stringify(aData, null, 2))
  );
  info("Done saving JSON file " + aFile.path);
}

<<<<<<< HEAD
/**
 * Create a callback function that calls do_execute_soon on an actual callback and arguments
 */
function callback_soon(aFunction) {
  return function(...args) {
    executeSoon(function() {
      aFunction.apply(null, args);
    }, aFunction.name ? "delayed callback " + aFunction.name : "delayed callback");
  };
}

XPCOMUtils.defineLazyServiceGetter(this, "pluginHost",
                                  "@mozilla.org/plugin/host;1", "nsIPluginHost");
||||||| merged common ancestors
/**
 * Create a callback function that calls do_execute_soon on an actual callback and arguments
 */
function callback_soon(aFunction) {
  return function(...args) {
    executeSoon(function() {
      aFunction.apply(null, args);
    }, aFunction.name ? "delayed callback " + aFunction.name : "delayed callback");
  };
}

async function serveSystemUpdate(xml, perform_update, testserver) {
  testserver.registerPathHandler("/data/update.xml", (request, response) => {
    response.write(xml);
  });

  try {
    await perform_update();
  } finally {
    testserver.registerPathHandler("/data/update.xml", null);
  }
}

// Runs an update check making it use the passed in xml string. Uses the direct
// call to the update function so we get rejections on failure.
async function installSystemAddons(xml, testserver) {
  info("Triggering system add-on update check.");

  await serveSystemUpdate(xml, async function() {
    let { XPIProvider } = ChromeUtils.import("resource://gre/modules/addons/XPIProvider.jsm", {});
    await XPIProvider.updateSystemAddons();
  }, testserver);
}

// Runs a full add-on update check which will in some cases do a system add-on
// update check. Always succeeds.
async function updateAllSystemAddons(xml, testserver) {
  info("Triggering full add-on update check.");

  await serveSystemUpdate(xml, function() {
    return new Promise(resolve => {
      Services.obs.addObserver(function observer() {
        Services.obs.removeObserver(observer, "addons-background-update-complete");

        resolve();
      }, "addons-background-update-complete");

      // Trigger the background update timer handler
      gInternalManager.notify(null);
    });
  }, testserver);
}

// Builds an update.xml file for an update check based on the data passed.
function buildSystemAddonUpdates(addons, root) {
  let xml = `<?xml version="1.0" encoding="UTF-8"?>\n\n<updates>\n`;
  if (addons) {
    xml += `  <addons>\n`;
    for (let addon of addons) {
      xml += `    <addon id="${addon.id}" URL="${root + addon.path}" version="${addon.version}"`;
      if (addon.hashFunction)
        xml += ` hashFunction="${addon.hashFunction}"`;
      if (addon.hashValue)
        xml += ` hashValue="${addon.hashValue}"`;
      xml += `/>\n`;
    }
    xml += `  </addons>\n`;
  }
  xml += `</updates>\n`;

  return xml;
}

function initSystemAddonDirs() {
  let hiddenSystemAddonDir = FileUtils.getDir("ProfD", ["sysfeatures", "hidden"], true);
  do_get_file("data/system_addons/system1_1.xpi").copyTo(hiddenSystemAddonDir, "system1@tests.mozilla.org.xpi");
  do_get_file("data/system_addons/system2_1.xpi").copyTo(hiddenSystemAddonDir, "system2@tests.mozilla.org.xpi");

  let prefilledSystemAddonDir = FileUtils.getDir("ProfD", ["sysfeatures", "prefilled"], true);
  do_get_file("data/system_addons/system2_2.xpi").copyTo(prefilledSystemAddonDir, "system2@tests.mozilla.org.xpi");
  do_get_file("data/system_addons/system3_2.xpi").copyTo(prefilledSystemAddonDir, "system3@tests.mozilla.org.xpi");
}

/**
 * Returns current system add-on update directory (stored in pref).
 */
function getCurrentSystemAddonUpdatesDir() {
  const updatesDir = FileUtils.getDir("ProfD", ["features"], false);
  let dir = updatesDir.clone();
  let set = JSON.parse(Services.prefs.getCharPref(PREF_SYSTEM_ADDON_SET));
  dir.append(set.directory);
  return dir;
}

/**
 * Removes all files from system add-on update directory.
 */
function clearSystemAddonUpdatesDir() {
  const updatesDir = FileUtils.getDir("ProfD", ["features"], false);
  // Delete any existing directories
  if (updatesDir.exists())
    updatesDir.remove(true);

  Services.prefs.clearUserPref(PREF_SYSTEM_ADDON_SET);
}

/**
 * Installs a known set of add-ons into the system add-on update directory.
 */
function buildPrefilledUpdatesDir() {
  clearSystemAddonUpdatesDir();

  // Build the test set
  let dir = FileUtils.getDir("ProfD", ["features", "prefilled"], true);

  do_get_file("data/system_addons/system2_2.xpi").copyTo(dir, "system2@tests.mozilla.org.xpi");
  do_get_file("data/system_addons/system3_2.xpi").copyTo(dir, "system3@tests.mozilla.org.xpi");

  // Mark these in the past so the startup file scan notices when files have changed properly
  FileUtils.getFile("ProfD", ["features", "prefilled", "system2@tests.mozilla.org.xpi"]).lastModifiedTime -= 10000;
  FileUtils.getFile("ProfD", ["features", "prefilled", "system3@tests.mozilla.org.xpi"]).lastModifiedTime -= 10000;

  Services.prefs.setCharPref(PREF_SYSTEM_ADDON_SET, JSON.stringify({
    schema: 1,
    directory: dir.leafName,
    addons: {
      "system2@tests.mozilla.org": {
        version: "2.0",
      },
      "system3@tests.mozilla.org": {
        version: "2.0",
      },
    },
  }));
}

/**
 * Check currently installed ssystem add-ons against a set of conditions.
 *
 * @param {Array<Object>} conditions - an array of objects of the form { isUpgrade: false, version: null}
 * @param {nsIFile} distroDir - the system add-on distribution directory (the "features" dir in the app directory)
 */
async function checkInstalledSystemAddons(conditions, distroDir) {
  for (let i = 0; i < conditions.length; i++) {
    let condition = conditions[i];
    let id = "system" + (i + 1) + "@tests.mozilla.org";
    let addon = await promiseAddonByID(id);

    if (!("isUpgrade" in condition) || !("version" in condition)) {
      throw Error("condition must contain isUpgrade and version");
    }
    let isUpgrade = conditions[i].isUpgrade;
    let version = conditions[i].version;

    let expectedDir = isUpgrade ? getCurrentSystemAddonUpdatesDir() : distroDir;

    if (version) {
      info(`Checking state of add-on ${id}, expecting version ${version}`);

      // Add-on should be installed
      Assert.notEqual(addon, null);
      Assert.equal(addon.version, version);
      Assert.ok(addon.isActive);
      Assert.ok(!addon.foreignInstall);
      Assert.ok(addon.hidden);
      Assert.ok(addon.isSystem);

      // Verify the add-ons file is in the right place
      let file = expectedDir.clone();
      file.append(id + ".xpi");
      Assert.ok(file.exists());
      Assert.ok(file.isFile());

      let uri = addon.getResourceURI(null);
      Assert.ok(uri instanceof Ci.nsIFileURL);
      Assert.equal(uri.file.path, file.path);

      if (isUpgrade) {
        Assert.equal(addon.signedState, AddonManager.SIGNEDSTATE_SYSTEM);
      }

      // Verify the add-on actually started
      BootstrapMonitor.checkAddonStarted(id, version);
    } else {
      info(`Checking state of add-on ${id}, expecting it to be missing`);

      if (isUpgrade) {
        // Add-on should not be installed
        Assert.equal(addon, null);
      }

      BootstrapMonitor.checkAddonNotStarted(id);

      if (addon)
        BootstrapMonitor.checkAddonInstalled(id);
      else
        BootstrapMonitor.checkAddonNotInstalled(id);
    }
  }
}

/**
 * Returns all system add-on updates directories.
 */
async function getSystemAddonDirectories() {
  const updatesDir = FileUtils.getDir("ProfD", ["features"], false);
  let subdirs = [];

  if (await OS.File.exists(updatesDir.path)) {
    let iterator = new OS.File.DirectoryIterator(updatesDir.path);
    await iterator.forEach(entry => {
      if (entry.isDir) {
        subdirs.push(entry);
      }
    });
    iterator.close();
  }

  return subdirs;
}

/**
 * Sets up initial system add-on update conditions.
 *
 * @param {Object<function, Array<Object>} setup - an object containing a setup function and an array of objects
 *  of the form {isUpgrade: false, version: null}
 *
 * @param {nsIFile} distroDir - the system add-on distribution directory (the "features" dir in the app directory)
 */
async function setupSystemAddonConditions(setup, distroDir) {
  info("Clearing existing database.");
  Services.prefs.clearUserPref(PREF_SYSTEM_ADDON_SET);
  distroDir.leafName = "empty";

  let updateList = [];
  await overrideBuiltIns({ "system": updateList });
  await promiseStartupManager();
  await promiseShutdownManager();

  info("Setting up conditions.");
  await setup.setup();

  if (distroDir) {
    if (distroDir.path.endsWith("hidden")) {
      updateList = ["system1@tests.mozilla.org", "system2@tests.mozilla.org"];
    } else if (distroDir.path.endsWith("prefilled")) {
      updateList = ["system2@tests.mozilla.org", "system3@tests.mozilla.org"];
    }
  }
  await overrideBuiltIns({ "system": updateList });
  await promiseStartupManager();

  // Make sure the initial state is correct
  info("Checking initial state.");
  await checkInstalledSystemAddons(setup.initialState, distroDir);
}

/**
 * Verify state of system add-ons after installation.
 *
 * @param {Array<Object>} initialState - an array of objects of the form {isUpgrade: false, version: null}
 * @param {Array<Object>} finalState - an array of objects of the form {isUpgrade: false, version: null}
 * @param {Boolean} alreadyUpgraded - whether a restartless upgrade has already been performed.
 * @param {nsIFile} distroDir - the system add-on distribution directory (the "features" dir in the app directory)
 */
async function verifySystemAddonState(initialState, finalState = undefined, alreadyUpgraded = false, distroDir) {
  let expectedDirs = 0;

  // If the initial state was using the profile set then that directory will
  // still exist.

  if (initialState.some(a => a.isUpgrade)) {
    expectedDirs++;
  }

  if (finalState == undefined) {
    finalState = initialState;
  } else if (finalState.some(a => a.isUpgrade)) {
    // If the new state is using the profile then that directory will exist.
    expectedDirs++;
  }

  // Since upgrades are restartless now, the previous update dir hasn't been removed.
  if (alreadyUpgraded) {
    expectedDirs++;
  }

  info("Checking final state.");

  let dirs = await getSystemAddonDirectories();
  Assert.equal(dirs.length, expectedDirs);

  await checkInstalledSystemAddons(...finalState, distroDir);

  // Check that the new state is active after a restart
  await promiseShutdownManager();

  let updateList = [];

  if (distroDir) {
    if (distroDir.path.endsWith("hidden")) {
      updateList = ["system1@tests.mozilla.org", "system2@tests.mozilla.org"];
    } else if (distroDir.path.endsWith("prefilled")) {
      updateList = ["system2@tests.mozilla.org", "system3@tests.mozilla.org"];
    }
  }
  await overrideBuiltIns({ "system": updateList });
  await promiseStartupManager();
  await checkInstalledSystemAddons(finalState, distroDir);
}

/**
 * Run system add-on tests and compare the results against a set of expected conditions.
 *
 * @param {String} setupName - name of the current setup conditions.
 * @param {Object<function, Array<Object>} setup -  Defines the set of initial conditions to run each test against. Each should
 *                                                  define the following properties:
 *    setup:        A task to setup the profile into the initial state.
 *    initialState: The initial expected system add-on state after setup has run.
 * @param {Array<Object>} test -  The test to run. Each test must define an updateList or test. The following
 *                                properties are used:
 *    updateList: The set of add-ons the server should respond with.
 *    test:       A function to run to perform the update check (replaces
 *                updateList)
 *    fails:      An optional property, if true the update check is expected to
 *                fail.
 *    finalState: An optional property, the expected final state of system add-ons,
 *                if missing the test condition's initialState is used.
 * @param {nsIFile} distroDir - the system add-on distribution directory (the "features" dir in the app directory)
 * @param {String} root - HTTP URL to the test server
 * @param {HttpServer} testserver - existing HTTP test server to use
 */

async function execSystemAddonTest(setupName, setup, test, distroDir, root, testserver) {
  await setupSystemAddonConditions(setup, distroDir);

  try {
    if ("test" in test) {
      await test.test();
    } else {
      await installSystemAddons(await buildSystemAddonUpdates(test.updateList, root), testserver);
    }

    if (test.fails) {
      do_throw("Expected this test to fail");
    }
  } catch (e) {
    if (!test.fails) {
      do_throw(e);
    }
  }

  // some tests have a different expected combination of default
  // and updated add-ons.
  if (test.finalState && setupName in test.finalState) {
    await verifySystemAddonState(setup.initialState, test.finalState[setupName], false, distroDir);
  } else {
    await verifySystemAddonState(setup.initialState, undefined, false, distroDir);
  }

  await promiseShutdownManager();
}

XPCOMUtils.defineLazyServiceGetter(this, "pluginHost",
                                  "@mozilla.org/plugin/host;1", "nsIPluginHost");
=======
XPCOMUtils.defineLazyServiceGetter(
  this,
  "pluginHost",
  "@mozilla.org/plugin/host;1",
  "nsIPluginHost"
);
>>>>>>> upstream-releases

class MockPluginTag {
  constructor(opts, enabledState = Ci.nsIPluginTag.STATE_ENABLED) {
    this.pluginTag = pluginHost.createFakePlugin({
      handlerURI: "resource://fake-plugin/${Math.random()}.xhtml",
      mimeEntries: [{ type: "application/x-fake-plugin" }],
      fileName: `${opts.name}.so`,
      ...opts,
    });
    this.pluginTag.enabledState = enabledState;

    this.name = opts.name;
    this.version = opts.version;
  }
  async isBlocklisted() {
    let state = await Blocklist.getPluginBlocklistState(this.pluginTag);
    return state == Services.blocklist.STATE_BLOCKED;
  }
  get disabled() {
    return this.pluginTag.enabledState == Ci.nsIPluginTag.STATE_DISABLED;
  }
  set disabled(val) {
    this.enabledState =
      Ci.nsIPluginTag[val ? "STATE_DISABLED" : "STATE_ENABLED"];
  }
  get enabledState() {
    return this.pluginTag.enabledState;
  }
  set enabledState(val) {
    this.pluginTag.enabledState = val;
  }
}

function mockPluginHost(plugins) {
  let PluginHost = {
    getPluginTags() {
      return plugins.map(p => p.pluginTag);
    },

    QueryInterface: ChromeUtils.generateQI(["nsIPluginHost"]),
  };

  MockRegistrar.register("@mozilla.org/plugin/host;1", PluginHost);
}

async function setInitialState(addon, initialState) {
  if (initialState.userDisabled) {
    await addon.disable();
  } else if (initialState.userDisabled === false) {
    await addon.enable();
  }
}

async function installBuiltinExtension(extensionData) {
  let xpi = await AddonTestUtils.createTempWebExtensionFile(extensionData);

  // The built-in location requires a resource: URL that maps to a
  // jar: or file: URL.  This would typically be something bundled
  // into omni.ja but for testing we just use a temp file.
  let base = Services.io.newURI(`jar:file:${xpi.path}!/`);
  let resProto = Services.io
    .getProtocolHandler("resource")
    .QueryInterface(Ci.nsIResProtocolHandler);
  resProto.setSubstitution("ext-test", base);

  let id = extensionData.manifest.applications.gecko.id;
  let wrapper = ExtensionTestUtils.expectExtension(id);
  await AddonManager.installBuiltinAddon("resource://ext-test/");
  await wrapper.awaitStartup();
  return wrapper;
}
