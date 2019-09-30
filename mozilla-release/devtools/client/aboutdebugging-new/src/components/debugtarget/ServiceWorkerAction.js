/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const {
  createFactory,
  PureComponent,
} = require("devtools/client/shared/vendor/react");
const dom = require("devtools/client/shared/vendor/react-dom-factories");
const PropTypes = require("devtools/client/shared/vendor/react-prop-types");
const { connect } = require("devtools/client/shared/vendor/react-redux");

const FluentReact = require("devtools/client/shared/vendor/fluent-react");
const Localized = createFactory(FluentReact.Localized);

const {
  getCurrentRuntimeDetails,
} = require("../../modules/runtimes-state-helper");

const InspectAction = createFactory(require("./InspectAction"));

<<<<<<< HEAD
const Actions = require("../../actions/index");
const Types = require("../../types/index");
||||||| merged common ancestors
const Actions = require("../../actions/index");
=======
const Types = require("../../types/index");
const { SERVICE_WORKER_STATUSES } = require("../../constants");
>>>>>>> upstream-releases

/**
 * This component displays buttons for service worker.
 */
class ServiceWorkerAction extends PureComponent {
  static get propTypes() {
    return {
      dispatch: PropTypes.func.isRequired,
<<<<<<< HEAD
      // Provided by wrapping the component with FluentReact.withLocalization.
      getString: PropTypes.func.isRequired,
      target: Types.debugTarget.isRequired,
||||||| merged common ancestors
      // Provided by wrapping the component with FluentReact.withLocalization.
      getString: PropTypes.func.isRequired,
      target: PropTypes.object.isRequired,
=======
      // Provided by redux state
      runtimeDetails: Types.runtimeDetails.isRequired,
      target: Types.debugTarget.isRequired,
>>>>>>> upstream-releases
    };
  }

  _renderInspectAction() {
    const { status } = this.props.target.details;
    const shallRenderInspectAction =
      status === SERVICE_WORKER_STATUSES.RUNNING ||
      status === SERVICE_WORKER_STATUSES.REGISTERING;

    if (!shallRenderInspectAction) {
      return null;
    }

    return InspectAction({
      disabled: this.props.runtimeDetails.isMultiE10s,
      dispatch: this.props.dispatch,
      target: this.props.target,
    });
  }

  _getStatusLocalizationId(status) {
    switch (status) {
      case SERVICE_WORKER_STATUSES.REGISTERING.toLowerCase():
        return "about-debugging-worker-status-registering";
      case SERVICE_WORKER_STATUSES.RUNNING.toLowerCase():
        return "about-debugging-worker-status-running";
      case SERVICE_WORKER_STATUSES.STOPPED.toLowerCase():
        return "about-debugging-worker-status-stopped";
      default:
        // Assume status is stopped for unknown status value.
        return "about-debugging-worker-status-stopped";
    }
  }

  _renderStatus() {
    const status = this.props.target.details.status.toLowerCase();
    const statusClassName =
      status === SERVICE_WORKER_STATUSES.RUNNING.toLowerCase()
        ? "service-worker-action__status--running"
        : "";

    return Localized(
      {
<<<<<<< HEAD
        className: "default-button",
        onClick: e => onClick(),
||||||| merged common ancestors
        className: "aboutdebugging-button",
        onClick: e => onClick(),
=======
        id: this._getStatusLocalizationId(status),
>>>>>>> upstream-releases
      },
      dom.span(
        {
          className: `service-worker-action__status qa-worker-status ${statusClassName}`,
        },
        status
      )
    );
  }

  render() {
<<<<<<< HEAD
    return dom.div(
      {
        className: "toolbar",
      },
      this._renderAction()
    );
||||||| merged common ancestors
    return dom.div({}, this._renderAction());
=======
    return dom.div(
      {
        className: "service-worker-action",
      },
      this._renderStatus(),
      this._renderInspectAction()
    );
>>>>>>> upstream-releases
  }
}

const mapStateToProps = state => {
  return {
    runtimeDetails: getCurrentRuntimeDetails(state.runtimes),
  };
};

module.exports = connect(mapStateToProps)(ServiceWorkerAction);
