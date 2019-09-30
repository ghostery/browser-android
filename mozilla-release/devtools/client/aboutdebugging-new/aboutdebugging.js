/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const Services = require("Services");

const { bindActionCreators } = require("devtools/client/shared/vendor/redux");
<<<<<<< HEAD
const { createFactory } = require("devtools/client/shared/vendor/react");
const { render, unmountComponentAtNode } =
  require("devtools/client/shared/vendor/react-dom");
const Provider =
  createFactory(require("devtools/client/shared/vendor/react-redux").Provider);
||||||| merged common ancestors
const { createFactory } =
  require("devtools/client/shared/vendor/react");
const { render, unmountComponentAtNode } =
  require("devtools/client/shared/vendor/react-dom");
const Provider =
  createFactory(require("devtools/client/shared/vendor/react-redux").Provider);
const { L10nRegistry, FileSource } =
  require("resource://gre/modules/L10nRegistry.jsm");
=======
const { createFactory } = require("devtools/client/shared/vendor/react");
const {
  render,
  unmountComponentAtNode,
} = require("devtools/client/shared/vendor/react-dom");
const Provider = createFactory(
  require("devtools/client/shared/vendor/react-redux").Provider
);

const FluentReact = require("devtools/client/shared/vendor/fluent-react");
const LocalizationProvider = createFactory(FluentReact.LocalizationProvider);
>>>>>>> upstream-releases

const actions = require("./src/actions/index");
const { configureStore } = require("./src/create-store");
const {
  setDebugTargetCollapsibilities,
} = require("./src/modules/debug-target-collapsibilities");

const { l10n } = require("./src/modules/l10n");

const {
  addNetworkLocationsObserver,
  getNetworkLocations,
  removeNetworkLocationsObserver,
} = require("./src/modules/network-locations");
const {
  addUSBRuntimesObserver,
  getUSBRuntimes,
  removeUSBRuntimesObserver,
} = require("./src/modules/usb-runtimes");

<<<<<<< HEAD
loader.lazyRequireGetter(this, "adbAddon", "devtools/shared/adb/adb-addon", true);

const Router = createFactory(require("devtools/client/shared/vendor/react-router-dom").HashRouter);
||||||| merged common ancestors
loader.lazyRequireGetter(this, "adbAddon", "devtools/shared/adb/adb-addon", true);

=======
loader.lazyRequireGetter(this, "adb", "devtools/shared/adb/adb", true);
loader.lazyRequireGetter(
  this,
  "adbAddon",
  "devtools/shared/adb/adb-addon",
  true
);
loader.lazyRequireGetter(
  this,
  "adbProcess",
  "devtools/shared/adb/adb-process",
  true
);

const Router = createFactory(
  require("devtools/client/shared/vendor/react-router-dom").HashRouter
);
>>>>>>> upstream-releases
const App = createFactory(require("./src/components/App"));

const AboutDebugging = {
  async init() {
    if (!Services.prefs.getBoolPref("devtools.enabled", true)) {
      // If DevTools are disabled, navigate to about:devtools.
      window.location = "about:devtools?reason=AboutDebugging";
      return;
    }

    this.onAdbAddonUpdated = this.onAdbAddonUpdated.bind(this);
    this.onAdbProcessReady = this.onAdbProcessReady.bind(this);
    this.onNetworkLocationsUpdated = this.onNetworkLocationsUpdated.bind(this);
    this.onUSBRuntimesUpdated = this.onUSBRuntimesUpdated.bind(this);

    this.store = configureStore();
    this.actions = bindActionCreators(actions, this.store.dispatch);

<<<<<<< HEAD
    await l10n.init();
||||||| merged common ancestors
    const fluentBundles = await this.createFluentBundles();
=======
    const width = this.getRoundedViewportWidth();
    this.actions.recordTelemetryEvent("open_adbg", { width });
>>>>>>> upstream-releases

<<<<<<< HEAD
    render(
      Provider(
        {
          store: this.store,
        },
        Router(
          {},
          App(
            {
              fluentBundles: l10n.getBundles(),
            }
          )
        )
      ),
      this.mount
    );
||||||| merged common ancestors
    render(
      Provider({ store: this.store }, App({ fluentBundles })),
      this.mount
    );
=======
    await l10n.init(["branding/brand.ftl", "devtools/aboutdebugging.ftl"]);
>>>>>>> upstream-releases

<<<<<<< HEAD
    this.actions.updateNetworkLocations(getNetworkLocations());
||||||| merged common ancestors
    this.actions.selectPage(PAGES.THIS_FIREFOX, RUNTIMES.THIS_FIREFOX);
    this.actions.updateNetworkLocations(getNetworkLocations());
=======
    this.actions.createThisFirefoxRuntime();
>>>>>>> upstream-releases

    // Listen to Network locations updates and retrieve the initial list of locations.
    addNetworkLocationsObserver(this.onNetworkLocationsUpdated);
<<<<<<< HEAD

    // Listen to USB runtime updates and retrieve the initial list of runtimes.
    addUSBRuntimesObserver(this.onUSBRuntimesUpdated);
    getUSBRuntimes();
||||||| merged common ancestors
    addUSBRuntimesObserver(this.onUSBRuntimesUpdated);
    await enableUSBRuntimes();
=======
    await this.onNetworkLocationsUpdated();
>>>>>>> upstream-releases

<<<<<<< HEAD
    adbAddon.on("update", this.onAdbAddonUpdated);
    this.onAdbAddonUpdated();
||||||| merged common ancestors
    adbAddon.on("update", this.onAdbAddonUpdated);
    this.onAdbAddonUpdated();
  },

  async createFluentBundles() {
    // XXX Until the strings for the updated about:debugging stabilize, we
    // locate them outside the regular directory for locale resources so that
    // they don't get picked up by localization tools.
    if (!L10nRegistry.sources.has("aboutdebugging")) {
      const temporarySource = new FileSource(
        "aboutdebugging",
        ["en-US"],
        "chrome://devtools/content/aboutdebugging-new/tmp-locale/{locale}/"
      );
      L10nRegistry.registerSource(temporarySource);
    }

    const locales = Services.locale.appLocalesAsBCP47;
    const generator =
      L10nRegistry.generateBundles(locales, ["aboutdebugging.ftl"]);

    const bundles = [];
    for await (const bundle of generator) {
      bundles.push(bundle);
    }
=======
    // Listen to USB runtime updates and retrieve the initial list of runtimes.

    // If ADB is already started, wait for the initial runtime list to be able to restore
    // already connected runtimes.
    const isProcessStarted = await adb.isProcessStarted();
    const onAdbRuntimesReady = isProcessStarted
      ? adb.once("runtime-list-ready")
      : null;
    addUSBRuntimesObserver(this.onUSBRuntimesUpdated);
    await onAdbRuntimesReady;

    await this.onUSBRuntimesUpdated();

    render(
      Provider(
        {
          store: this.store,
        },
        LocalizationProvider(
          { messages: l10n.getBundles() },
          Router({}, App({}))
        )
      ),
      this.mount
    );

    adbAddon.on("update", this.onAdbAddonUpdated);
    this.onAdbAddonUpdated();
    adbProcess.on("adb-ready", this.onAdbProcessReady);
    // get the initial status of adb process, in case it's already started
    this.onAdbProcessReady();
>>>>>>> upstream-releases

    // Remove deprecated remote debugging extensions.
    await adbAddon.uninstallUnsupportedExtensions();
  },

  onAdbAddonUpdated() {
    this.actions.updateAdbAddonStatus(adbAddon.status);
  },

  onAdbProcessReady() {
    this.actions.updateAdbReady(adbProcess.ready);
  },

  onNetworkLocationsUpdated() {
    return this.actions.updateNetworkLocations(getNetworkLocations());
  },

  async onUSBRuntimesUpdated() {
    const runtimes = await getUSBRuntimes();
    return this.actions.updateUSBRuntimes(runtimes);
  },

  async destroy() {
<<<<<<< HEAD
    const state = this.store.getState();

    l10n.destroy();
||||||| merged common ancestors
    const state = this.store.getState();

    L10nRegistry.removeSource("aboutdebugging");
=======
    const width = this.getRoundedViewportWidth();
    this.actions.recordTelemetryEvent("close_adbg", { width });
>>>>>>> upstream-releases

    const state = this.store.getState();
    const currentRuntimeId = state.runtimes.selectedRuntimeId;
    if (currentRuntimeId) {
      await this.actions.unwatchRuntime(currentRuntimeId);
    }

    // Remove all client listeners.
    this.actions.removeRuntimeListeners();

    removeNetworkLocationsObserver(this.onNetworkLocationsUpdated);
    removeUSBRuntimesObserver(this.onUSBRuntimesUpdated);
    adbAddon.off("update", this.onAdbAddonUpdated);
    adbProcess.off("adb-ready", this.onAdbProcessReady);
    setDebugTargetCollapsibilities(state.ui.debugTargetCollapsibilities);
    unmountComponentAtNode(this.mount);
  },

  get mount() {
    return document.getElementById("mount");
  },

  /**
   * Computed viewport width, rounded at 50px. Used for telemetry events.
   */
  getRoundedViewportWidth() {
    return Math.ceil(window.outerWidth / 50) * 50;
  },
};

window.addEventListener(
  "DOMContentLoaded",
  () => {
    AboutDebugging.init();
  },
  { once: true }
);

window.addEventListener(
  "unload",
  () => {
    AboutDebugging.destroy();
  },
  { once: true }
);

<<<<<<< HEAD
window.addEventListener("unload", () => {
  AboutDebugging.destroy();
}, {once: true});

// Expose AboutDebugging to tests so that they can access to the store.
window.AboutDebugging = AboutDebugging;
||||||| merged common ancestors
window.addEventListener("unload", () => {
  AboutDebugging.destroy();
}, {once: true});
=======
// Expose AboutDebugging to tests so that they can access to the store.
window.AboutDebugging = AboutDebugging;
>>>>>>> upstream-releases
