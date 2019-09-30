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
const Route = createFactory(require("devtools/client/shared/vendor/react-router-dom").Route);
const Switch = createFactory(require("devtools/client/shared/vendor/react-router-dom").Switch);
const Redirect = createFactory(require("devtools/client/shared/vendor/react-router-dom").Redirect);
const { withRouter } = require("devtools/client/shared/vendor/react-router-dom");

const Types = require("../types/index");
const { RUNTIMES } = require("../constants");
||||||| merged common ancestors
const { PAGES } = require("../constants");
const Types = require("../types");
=======
const Route = createFactory(
  require("devtools/client/shared/vendor/react-router-dom").Route
);
const Switch = createFactory(
  require("devtools/client/shared/vendor/react-router-dom").Switch
);
const Redirect = createFactory(
  require("devtools/client/shared/vendor/react-router-dom").Redirect
);

const Types = require("../types/index");
const { PAGE_TYPES, RUNTIMES } = require("../constants");
>>>>>>> upstream-releases

const ConnectPage = createFactory(require("./connect/ConnectPage"));
const RuntimePage = createFactory(require("./RuntimePage"));
const Sidebar = createFactory(require("./sidebar/Sidebar"));

class App extends PureComponent {
  static get propTypes() {
    return {
      adbAddonStatus: Types.adbAddonStatus,
      // The "dispatch" helper is forwarded to the App component via connect.
      // From that point, components are responsible for forwarding the dispatch
      // property to all components who need to dispatch actions.
      dispatch: PropTypes.func.isRequired,
<<<<<<< HEAD
      fluentBundles: PropTypes.arrayOf(PropTypes.object).isRequired,
      isScanningUsb: PropTypes.bool.isRequired,
      networkEnabled: PropTypes.bool.isRequired,
      networkLocations: PropTypes.arrayOf(PropTypes.string).isRequired,
||||||| merged common ancestors
      fluentBundles: PropTypes.arrayOf(PropTypes.object).isRequired,
      networkLocations: PropTypes.arrayOf(PropTypes.string).isRequired,
=======
      // getString prop is injected by the withLocalization wrapper
      getString: PropTypes.func.isRequired,
      isAdbReady: PropTypes.bool.isRequired,
      isScanningUsb: PropTypes.bool.isRequired,
      networkLocations: PropTypes.arrayOf(Types.location).isRequired,
>>>>>>> upstream-releases
      networkRuntimes: PropTypes.arrayOf(Types.runtime).isRequired,
<<<<<<< HEAD
      selectedPage: PropTypes.string,
      selectedRuntime: PropTypes.string,
||||||| merged common ancestors
      selectedPage: PropTypes.string,
=======
      selectedPage: Types.page,
      selectedRuntimeId: PropTypes.string,
>>>>>>> upstream-releases
      usbRuntimes: PropTypes.arrayOf(Types.runtime).isRequired,
      wifiEnabled: PropTypes.bool.isRequired,
    };
  }

<<<<<<< HEAD
  renderConnect() {
    const {
||||||| merged common ancestors
  getSelectedPageComponent() {
    const {
=======
  componentDidUpdate() {
    this.updateTitle();
  }

  updateTitle() {
    const { getString, selectedPage, selectedRuntimeId } = this.props;

    const pageTitle =
      selectedPage === PAGE_TYPES.RUNTIME
        ? getString("about-debugging-page-title-runtime-page", {
            selectedRuntimeId,
          })
        : getString("about-debugging-page-title-setup-page");

    document.title = pageTitle;
  }

  renderConnect() {
    const { adbAddonStatus, dispatch, networkLocations } = this.props;

    return ConnectPage({
>>>>>>> upstream-releases
      adbAddonStatus,
      dispatch,
      networkEnabled,
      networkLocations,
<<<<<<< HEAD
      wifiEnabled,
    } = this.props;
||||||| merged common ancestors
      selectedPage,
    } = this.props;
=======
    });
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    return ConnectPage({
      adbAddonStatus,
      dispatch,
      networkEnabled,
      networkLocations,
      wifiEnabled,
    });
  }

  // The `match` object here is passed automatically by the Route object.
  // We are using it to read the route path.
  // See react-router docs:
  // https://github.com/ReactTraining/react-router/blob/master/packages/react-router/docs/api/match.md
  renderRuntime({ match }) {
    // Redirect to This Firefox in these cases:
    // - If the runtimepage for a device is the first page shown (since we can't
    //   keep connections open between page reloads).
    // - If no runtimeId is given.
    // - If runtime is not found in the runtimes list (this is handled later)
    const isDeviceFirstPage =
      !this.props.selectedPage &&
      match.params.runtimeId !== RUNTIMES.THIS_FIREFOX;
    if (!match.params.runtimeId || isDeviceFirstPage) {
      return Redirect({ to: `/runtime/${RUNTIMES.THIS_FIREFOX}` });
    }
||||||| merged common ancestors
    if (!selectedPage) {
      // No page selected.
      return null;
    }
=======
  // The `match` object here is passed automatically by the Route object.
  // We are using it to read the route path.
  // See react-router docs:
  // https://github.com/ReactTraining/react-router/blob/master/packages/react-router/docs/api/match.md
  renderRuntime({ match }) {
    const isRuntimeAvailable = id => {
      const runtimes = [
        ...this.props.networkRuntimes,
        ...this.props.usbRuntimes,
      ];
      const runtime = runtimes.find(x => x.id === id);
      return runtime && runtime.runtimeDetails;
    };
>>>>>>> upstream-releases

<<<<<<< HEAD
    const isRuntimeAvailable = id => {
      const runtimes = [
        ...this.props.networkRuntimes,
        ...this.props.usbRuntimes,
      ];
      return !!runtimes.find(x => x.id === id);
    };

    const { dispatch } = this.props;

    let runtimeId = match.params.runtimeId || RUNTIMES.THIS_FIREFOX;
    if (match.params.runtimeId !== RUNTIMES.THIS_FIREFOX) {
      const rawId = decodeURIComponent(match.params.runtimeId);
      if (isRuntimeAvailable(rawId)) {
        runtimeId = rawId;
      } else {
        // Also redirect to "This Firefox" if runtime is not found
        return Redirect({ to: `/runtime/${RUNTIMES.THIS_FIREFOX}` });
      }
||||||| merged common ancestors
    switch (selectedPage) {
      case PAGES.CONNECT:
        return ConnectPage({
          adbAddonStatus,
          dispatch,
          networkLocations,
        });
      default:
        // All pages except for the CONNECT page are RUNTIME pages.
        return RuntimePage({ dispatch });
=======
    const { dispatch } = this.props;

    let runtimeId = match.params.runtimeId || RUNTIMES.THIS_FIREFOX;
    if (match.params.runtimeId !== RUNTIMES.THIS_FIREFOX) {
      const rawId = decodeURIComponent(match.params.runtimeId);
      if (isRuntimeAvailable(rawId)) {
        runtimeId = rawId;
      } else {
        // Also redirect to "This Firefox" if runtime is not found
        return Redirect({ to: `/runtime/${RUNTIMES.THIS_FIREFOX}` });
      }
>>>>>>> upstream-releases
    }
<<<<<<< HEAD

    // we need to pass a key so the component updates when we want to showcase
    // a different runtime
    return RuntimePage({ dispatch, key: runtimeId, runtimeId });
  }

  renderRoutes() {
    return Switch(
      {},
      Route({
        path: "/connect",
        render: () => this.renderConnect(),
      }),
      Route({
        path: "/runtime/:runtimeId",
        render: routeProps => this.renderRuntime(routeProps),
      }),
      Route({
        path: "/",
        exact: true,
        // will redirect to This Firefox
        render: routeProps => this.renderRuntime(routeProps),
      }),
      // default route when there's no match
      // TODO: maybe we'd like to do something else than a redirect. See:
      // https://bugzilla.mozilla.org/show_bug.cgi?id=1509897
      Route({
        render: () => Redirect({ to: "/"}),
      })
    );
||||||| merged common ancestors
=======

    // we need to pass a key so the component updates when we want to showcase
    // a different runtime
    return RuntimePage({ dispatch, key: runtimeId, runtimeId });
  }

  renderRoutes() {
    return Switch(
      {},
      Route({
        path: "/setup",
        render: () => this.renderConnect(),
      }),
      Route({
        path: "/runtime/:runtimeId",
        render: routeProps => this.renderRuntime(routeProps),
      }),
      // default route when there's no match which includes "/"
      // TODO: the url does not match "/" means invalid URL,
      // in this case maybe we'd like to do something else than a redirect.
      // See: https://bugzilla.mozilla.org/show_bug.cgi?id=1509897
      Route({
        render: routeProps => {
          const { pathname } = routeProps.location;
          // The old about:debugging supported the following routes:
          // about:debugging#workers, about:debugging#addons and about:debugging#tabs.
          // Such links can still be found in external documentation pages.
          // We redirect to This Firefox rather than the Setup Page here.
          if (
            pathname === "/workers" ||
            pathname === "/addons" ||
            pathname === "/tabs"
          ) {
            return Redirect({ to: `/runtime/${RUNTIMES.THIS_FIREFOX}` });
          }
          return Redirect({ to: "/setup" });
        },
      })
    );
>>>>>>> upstream-releases
  }

  render() {
    const {
      adbAddonStatus,
      dispatch,
<<<<<<< HEAD
      fluentBundles,
      isScanningUsb,
||||||| merged common ancestors
      fluentBundles,
=======
      isAdbReady,
      isScanningUsb,
>>>>>>> upstream-releases
      networkRuntimes,
      selectedPage,
<<<<<<< HEAD
      selectedRuntime,
||||||| merged common ancestors
=======
      selectedRuntimeId,
>>>>>>> upstream-releases
      usbRuntimes,
    } = this.props;

    return Localized(
      {},
      dom.div(
        { className: "app" },
<<<<<<< HEAD
        Sidebar({
          adbAddonStatus,
          className: "app__sidebar",
          dispatch,
          isScanningUsb,
          networkRuntimes,
          selectedPage,
          selectedRuntime,
          usbRuntimes,
        }),
        dom.main({ className: "app__content" }, this.renderRoutes())
||||||| merged common ancestors
        Sidebar(
          {
            adbAddonStatus,
            className: "app__sidebar",
            dispatch,
            networkRuntimes,
            selectedPage,
            usbRuntimes,
          }
        ),
        dom.main(
          { className: "app__content" },
          this.getSelectedPageComponent()
        )
=======
        Sidebar({
          adbAddonStatus,
          className: "app__sidebar",
          dispatch,
          isAdbReady,
          isScanningUsb,
          networkRuntimes,
          selectedPage,
          selectedRuntimeId,
          usbRuntimes,
        }),
        dom.main({ className: "app__content" }, this.renderRoutes())
>>>>>>> upstream-releases
      )
    );
  }
}

const mapStateToProps = state => {
  return {
    adbAddonStatus: state.ui.adbAddonStatus,
<<<<<<< HEAD
    isScanningUsb: state.ui.isScanningUsb,
    networkEnabled: state.ui.networkEnabled,
||||||| merged common ancestors
=======
    isAdbReady: state.ui.isAdbReady,
    isScanningUsb: state.ui.isScanningUsb,
>>>>>>> upstream-releases
    networkLocations: state.ui.networkLocations,
    networkRuntimes: state.runtimes.networkRuntimes,
    selectedPage: state.ui.selectedPage,
<<<<<<< HEAD
    selectedRuntime: state.ui.selectedRuntime,
||||||| merged common ancestors
=======
    selectedRuntimeId: state.runtimes.selectedRuntimeId,
>>>>>>> upstream-releases
    usbRuntimes: state.runtimes.usbRuntimes,
    wifiEnabled: state.ui.wifiEnabled,
  };
};

const mapDispatchToProps = dispatch => ({
  dispatch,
});

<<<<<<< HEAD
module.exports = withRouter(connect(mapStateToProps, mapDispatchToProps)(App));
||||||| merged common ancestors
module.exports = connect(mapStateToProps, mapDispatchToProps)(App);
=======
module.exports = FluentReact.withLocalization(
  connect(
    mapStateToProps,
    mapDispatchToProps
  )(App)
);
>>>>>>> upstream-releases
