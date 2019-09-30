/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const { connect } = require("devtools/client/shared/vendor/react-redux");
const {
  createFactory,
  PureComponent,
} = require("devtools/client/shared/vendor/react");
const dom = require("devtools/client/shared/vendor/react-dom-factories");
const PropTypes = require("devtools/client/shared/vendor/react-prop-types");

const FluentReact = require("devtools/client/shared/vendor/fluent-react");
const Localized = createFactory(FluentReact.Localized);

<<<<<<< HEAD
const ConnectionPromptSetting = createFactory(require("./ConnectionPromptSetting"));
||||||| merged common ancestors
=======
const CompatibilityWarning = createFactory(require("./CompatibilityWarning"));
>>>>>>> upstream-releases
const DebugTargetPane = createFactory(require("./debugtarget/DebugTargetPane"));
const ExtensionDetail = createFactory(require("./debugtarget/ExtensionDetail"));
const InspectAction = createFactory(require("./debugtarget/InspectAction"));
const ProfilerDialog = createFactory(require("./ProfilerDialog"));
const RuntimeActions = createFactory(require("./RuntimeActions"));
const RuntimeInfo = createFactory(require("./RuntimeInfo"));
const ServiceWorkerAction = createFactory(
  require("./debugtarget/ServiceWorkerAction")
);
const ServiceWorkerAdditionalActions = createFactory(
  require("./debugtarget/ServiceWorkerAdditionalActions")
);
const ServiceWorkersWarning = createFactory(require("./ServiceWorkersWarning"));
const ProcessDetail = createFactory(require("./debugtarget/ProcessDetail"));
const TabDetail = createFactory(require("./debugtarget/TabDetail"));
const TemporaryExtensionAdditionalActions = createFactory(
  require("./debugtarget/TemporaryExtensionAdditionalActions")
);
const TemporaryExtensionDetail = createFactory(
  require("./debugtarget/TemporaryExtensionDetail")
);
const TemporaryExtensionInstallSection = createFactory(
  require("./debugtarget/TemporaryExtensionInstallSection")
);
const WorkerDetail = createFactory(require("./debugtarget/WorkerDetail"));

<<<<<<< HEAD
const Actions = require("../actions/index");
const { DEBUG_TARGET_PANE, PAGE_TYPES, RUNTIMES } = require("../constants");

const {
  getCurrentConnectionPromptSetting,
  getCurrentRuntimeInfo,
} = require("../modules/runtimes-state-helper");
const { isSupportedDebugTargetPane } = require("../modules/debug-target-support");
||||||| merged common ancestors
const { DEBUG_TARGET_PANE } = require("../constants");
const { getCurrentRuntimeInfo } = require("../modules/runtimes-state-helper");
const { isSupportedDebugTargetPane } = require("../modules/debug-target-support");
=======
const Actions = require("../actions/index");
const {
  DEBUG_TARGETS,
  DEBUG_TARGET_PANE,
  PAGE_TYPES,
} = require("../constants");
const Types = require("../types/index");

const {
  getCurrentRuntimeDetails,
} = require("../modules/runtimes-state-helper");
const {
  isSupportedDebugTargetPane,
} = require("../modules/debug-target-support");
>>>>>>> upstream-releases

class RuntimePage extends PureComponent {
  static get propTypes() {
    return {
<<<<<<< HEAD
      collapsibilities: PropTypes.object.isRequired,
      connectionPromptEnabled: PropTypes.bool.isRequired,
||||||| merged common ancestors
      collapsibilities: PropTypes.object.isRequired,
=======
      collapsibilities: Types.collapsibilities.isRequired,
>>>>>>> upstream-releases
      dispatch: PropTypes.func.isRequired,
      installedExtensions: PropTypes.arrayOf(PropTypes.object).isRequired,
      otherWorkers: PropTypes.arrayOf(PropTypes.object).isRequired,
<<<<<<< HEAD
      runtimeId: PropTypes.string.isRequired,
      runtimeInfo: PropTypes.object,
||||||| merged common ancestors
      runtimeInfo: PropTypes.object,
=======
      runtimeDetails: Types.runtimeDetails,
      runtimeId: PropTypes.string.isRequired,
      processes: PropTypes.arrayOf(PropTypes.object).isRequired,
>>>>>>> upstream-releases
      serviceWorkers: PropTypes.arrayOf(PropTypes.object).isRequired,
      sharedWorkers: PropTypes.arrayOf(PropTypes.object).isRequired,
      showProfilerDialog: PropTypes.bool.isRequired,
      tabs: PropTypes.arrayOf(PropTypes.object).isRequired,
      temporaryExtensions: PropTypes.arrayOf(PropTypes.object).isRequired,
      temporaryInstallError: PropTypes.object,
    };
  }

<<<<<<< HEAD
  // TODO: avoid the use of this method
  // https://bugzilla.mozilla.org/show_bug.cgi?id=1508688
  componentWillMount() {
    const { dispatch, runtimeId } = this.props;
    dispatch(Actions.selectPage(PAGE_TYPES.RUNTIME, runtimeId));
  }

  renderConnectionPromptSetting() {
    const { connectionPromptEnabled, dispatch } = this.props;

    return dom.div(
      {
        className: "connection-prompt-setting",
      },
      ConnectionPromptSetting({ connectionPromptEnabled, dispatch }),
    );
  }

  renderDebugTargetPane(name, targets, actionComponent,
                        detailComponent, paneKey, localizationId) {
    const { collapsibilities, dispatch, runtimeInfo } = this.props;
||||||| merged common ancestors
  renderDebugTargetPane(name, targets, actionComponent,
                        detailComponent, paneKey, localizationId) {
    const { collapsibilities, dispatch, runtimeInfo } = this.props;
=======
  // TODO: avoid the use of this method
  // https://bugzilla.mozilla.org/show_bug.cgi?id=1508688
  componentWillMount() {
    const { dispatch, runtimeId } = this.props;
    dispatch(Actions.selectPage(PAGE_TYPES.RUNTIME, runtimeId));
  }

  getIconByType(type) {
    switch (type) {
      case DEBUG_TARGETS.EXTENSION:
        return "chrome://devtools/skin/images/debugging-addons.svg";
      case DEBUG_TARGETS.PROCESS:
        return "chrome://devtools/skin/images/aboutdebugging-process-icon.svg";
      case DEBUG_TARGETS.TAB:
        return "chrome://devtools/skin/images/debugging-tabs.svg";
      case DEBUG_TARGETS.WORKER:
        return "chrome://devtools/skin/images/debugging-workers.svg";
    }

    throw new Error(`Unsupported type [${type}]`);
  }

  renderDebugTargetPane(
    name,
    icon,
    targets,
    children,
    actionComponent,
    additionalActionsComponent,
    detailComponent,
    paneKey,
    localizationId
  ) {
    const { collapsibilities, dispatch, runtimeDetails } = this.props;
>>>>>>> upstream-releases

    if (!isSupportedDebugTargetPane(runtimeDetails.info.type, paneKey)) {
      return null;
    }

    return Localized(
      {
        id: localizationId,
        attrs: { name: true },
      },
      DebugTargetPane(
        {
          actionComponent,
          additionalActionsComponent,
          collapsibilityKey: paneKey,
          detailComponent,
          dispatch,
          icon,
          isCollapsed: collapsibilities.get(paneKey),
          name,
          targets,
        },
        children
      )
    );
  }

  renderTemporaryExtensionInstallSection() {
    if (
      !isSupportedDebugTargetPane(
        this.props.runtimeDetails.info.type,
        DEBUG_TARGET_PANE.TEMPORARY_EXTENSION
      )
    ) {
      return null;
    }

    const { dispatch, temporaryInstallError } = this.props;
    return TemporaryExtensionInstallSection({
      dispatch,
      temporaryInstallError,
    });
  }

  render() {
    const {
      dispatch,
      installedExtensions,
      otherWorkers,
<<<<<<< HEAD
      runtimeId,
      runtimeInfo,
||||||| merged common ancestors
      runtimeInfo,
=======
      processes,
      runtimeDetails,
      runtimeId,
>>>>>>> upstream-releases
      serviceWorkers,
      sharedWorkers,
      showProfilerDialog,
      tabs,
      temporaryExtensions,
    } = this.props;

    if (!runtimeDetails) {
      // runtimeInfo can be null when the selectPage action navigates from a runtime A
      // to a runtime B (between unwatchRuntime and watchRuntime).
      return null;
    }

<<<<<<< HEAD
    // do not show the connection prompt setting in 'This Firefox'
    const shallShowPromptSetting = runtimeId !== RUNTIMES.THIS_FIREFOX;

||||||| merged common ancestors
=======
    const { compatibilityReport } = runtimeDetails;

>>>>>>> upstream-releases
    return dom.article(
      {
        className: "page qa-runtime-page",
      },
<<<<<<< HEAD
      RuntimeInfo(runtimeInfo),
      shallShowPromptSetting
        ? this.renderConnectionPromptSetting()
        : null,
      isSupportedDebugTargetPane(runtimeInfo.type, DEBUG_TARGET_PANE.TEMPORARY_EXTENSION)
        ? TemporaryExtensionInstaller({ dispatch })
        : null,
      this.renderDebugTargetPane("Temporary Extensions",
                                 temporaryExtensions,
                                 TemporaryExtensionAction,
                                 ExtensionDetail,
                                 DEBUG_TARGET_PANE.TEMPORARY_EXTENSION,
                                 "about-debugging-runtime-temporary-extensions"),
      this.renderDebugTargetPane("Extensions",
                                 installedExtensions,
                                 InspectAction,
                                 ExtensionDetail,
                                 DEBUG_TARGET_PANE.INSTALLED_EXTENSION,
                                 "about-debugging-runtime-extensions"),
      this.renderDebugTargetPane("Tabs",
                                 tabs,
                                 InspectAction,
                                 TabDetail,
                                 DEBUG_TARGET_PANE.TAB,
                                 "about-debugging-runtime-tabs"),
      this.renderDebugTargetPane("Service Workers",
                                 serviceWorkers,
                                 ServiceWorkerAction,
                                 WorkerDetail,
                                 DEBUG_TARGET_PANE.SERVICE_WORKER,
                                 "about-debugging-runtime-service-workers"),
      this.renderDebugTargetPane("Shared Workers",
                                 sharedWorkers,
                                 InspectAction,
                                 WorkerDetail,
                                 DEBUG_TARGET_PANE.SHARED_WORKER,
                                 "about-debugging-runtime-shared-workers"),
      this.renderDebugTargetPane("Other Workers",
                                 otherWorkers,
                                 InspectAction,
                                 WorkerDetail,
                                 DEBUG_TARGET_PANE.OTHER_WORKER,
                                 "about-debugging-runtime-other-workers"),
||||||| merged common ancestors
      RuntimeInfo(runtimeInfo),
      isSupportedDebugTargetPane(runtimeInfo.type, DEBUG_TARGET_PANE.TEMPORARY_EXTENSION)
        ? TemporaryExtensionInstaller({ dispatch })
        : null,
      this.renderDebugTargetPane("Temporary Extensions",
                                 temporaryExtensions,
                                 TemporaryExtensionAction,
                                 ExtensionDetail,
                                 DEBUG_TARGET_PANE.TEMPORARY_EXTENSION,
                                 "about-debugging-runtime-temporary-extensions"),
      this.renderDebugTargetPane("Extensions",
                                 installedExtensions,
                                 InspectAction,
                                 ExtensionDetail,
                                 DEBUG_TARGET_PANE.INSTALLED_EXTENSION,
                                 "about-debugging-runtime-extensions"),
      this.renderDebugTargetPane("Tabs",
                                 tabs,
                                 InspectAction,
                                 TabDetail,
                                 DEBUG_TARGET_PANE.TAB,
                                 "about-debugging-runtime-tabs"),
      this.renderDebugTargetPane("Service Workers",
                                 serviceWorkers,
                                 ServiceWorkerAction,
                                 WorkerDetail,
                                 DEBUG_TARGET_PANE.SERVICE_WORKER,
                                 "about-debugging-runtime-service-workers"),
      this.renderDebugTargetPane("Shared Workers",
                                 sharedWorkers,
                                 InspectAction,
                                 WorkerDetail,
                                 DEBUG_TARGET_PANE.SHARED_WORKER,
                                 "about-debugging-runtime-shared-workers"),
      this.renderDebugTargetPane("Other Workers",
                                 otherWorkers,
                                 InspectAction,
                                 WorkerDetail,
                                 DEBUG_TARGET_PANE.OTHER_WORKER,
                                 "about-debugging-runtime-other-workers"),
=======
      RuntimeInfo({ ...runtimeDetails.info, runtimeId, dispatch }),
      RuntimeActions({ dispatch, runtimeId, runtimeDetails }),
      runtimeDetails.serviceWorkersAvailable ? null : ServiceWorkersWarning(),
      CompatibilityWarning({ compatibilityReport }),
      this.renderDebugTargetPane(
        "Tabs",
        this.getIconByType(DEBUG_TARGETS.TAB),
        tabs,
        null,
        InspectAction,
        null,
        TabDetail,
        DEBUG_TARGET_PANE.TAB,
        "about-debugging-runtime-tabs"
      ),
      this.renderDebugTargetPane(
        "Temporary Extensions",
        this.getIconByType(DEBUG_TARGETS.EXTENSION),
        temporaryExtensions,
        this.renderTemporaryExtensionInstallSection(),
        InspectAction,
        TemporaryExtensionAdditionalActions,
        TemporaryExtensionDetail,
        DEBUG_TARGET_PANE.TEMPORARY_EXTENSION,
        "about-debugging-runtime-temporary-extensions"
      ),
      this.renderDebugTargetPane(
        "Extensions",
        this.getIconByType(DEBUG_TARGETS.EXTENSION),
        installedExtensions,
        null,
        InspectAction,
        null,
        ExtensionDetail,
        DEBUG_TARGET_PANE.INSTALLED_EXTENSION,
        "about-debugging-runtime-extensions"
      ),
      this.renderDebugTargetPane(
        "Service Workers",
        this.getIconByType(DEBUG_TARGETS.WORKER),
        serviceWorkers,
        null,
        ServiceWorkerAction,
        ServiceWorkerAdditionalActions,
        WorkerDetail,
        DEBUG_TARGET_PANE.SERVICE_WORKER,
        "about-debugging-runtime-service-workers"
      ),
      this.renderDebugTargetPane(
        "Shared Workers",
        this.getIconByType(DEBUG_TARGETS.WORKER),
        sharedWorkers,
        null,
        InspectAction,
        null,
        WorkerDetail,
        DEBUG_TARGET_PANE.SHARED_WORKER,
        "about-debugging-runtime-shared-workers"
      ),
      this.renderDebugTargetPane(
        "Other Workers",
        this.getIconByType(DEBUG_TARGETS.WORKER),
        otherWorkers,
        null,
        InspectAction,
        null,
        WorkerDetail,
        DEBUG_TARGET_PANE.OTHER_WORKER,
        "about-debugging-runtime-other-workers"
      ),
      this.renderDebugTargetPane(
        "Processes",
        this.getIconByType(DEBUG_TARGETS.PROCESS),
        processes,
        null,
        InspectAction,
        null,
        ProcessDetail,
        DEBUG_TARGET_PANE.PROCESSES,
        "about-debugging-runtime-processes"
      ),

      showProfilerDialog ? ProfilerDialog({ dispatch, runtimeDetails }) : null
>>>>>>> upstream-releases
    );
  }
}

const mapStateToProps = state => {
  return {
    connectionPromptEnabled: getCurrentConnectionPromptSetting(state.runtimes),
    collapsibilities: state.ui.debugTargetCollapsibilities,
    installedExtensions: state.debugTargets.installedExtensions,
    processes: state.debugTargets.processes,
    otherWorkers: state.debugTargets.otherWorkers,
    runtimeDetails: getCurrentRuntimeDetails(state.runtimes),
    serviceWorkers: state.debugTargets.serviceWorkers,
    sharedWorkers: state.debugTargets.sharedWorkers,
    showProfilerDialog: state.ui.showProfilerDialog,
    tabs: state.debugTargets.tabs,
    temporaryExtensions: state.debugTargets.temporaryExtensions,
    temporaryInstallError: state.ui.temporaryInstallError,
  };
};

module.exports = connect(mapStateToProps)(RuntimePage);
