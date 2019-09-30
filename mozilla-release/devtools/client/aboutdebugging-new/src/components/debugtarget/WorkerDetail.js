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

const FluentReact = require("devtools/client/shared/vendor/fluent-react");
const Localized = createFactory(FluentReact.Localized);

const { SERVICE_WORKER_FETCH_STATES } = require("../../constants");

const FieldPair = createFactory(require("./FieldPair"));

const Types = require("../../types/index");

const FieldPair = createFactory(require("./FieldPair"));

const Types = require("../../types/index");

/**
 * This component displays detail information for worker.
 */
class WorkerDetail extends PureComponent {
  static get propTypes() {
    return {
      // Provided by wrapping the component with FluentReact.withLocalization.
      getString: PropTypes.func.isRequired,
      target: Types.debugTarget.isRequired,
    };
  }

  renderFetch() {
    const { fetch } = this.props.target.details;
<<<<<<< HEAD
    const status = fetch === SERVICE_WORKER_FETCH_STATES.LISTENING
                    ? "listening"
                    : "not-listening";

    return Localized(
      {
        id: "about-debugging-worker-fetch",
        attrs: { label: true, value: true },
        $status: status,
      },
      FieldPair(
        {
          slug: "fetch",
          label: "Fetch",
          value: status,
        }
      )
    );
  }
||||||| merged common ancestors
    const name = this.props.getString("about-debugging-worker-fetch");
    const label = fetch === SERVICE_WORKER_FETCH_STATES.LISTENING
                    ? this.props.getString("about-debugging-worker-fetch-listening")
                    : this.props.getString("about-debugging-worker-fetch-not-listening");
    return this.renderField("fetch", name, label);
  }
=======
    const isListening = fetch === SERVICE_WORKER_FETCH_STATES.LISTENING;
    const localizationId = isListening
      ? "about-debugging-worker-fetch-listening"
      : "about-debugging-worker-fetch-not-listening";
>>>>>>> upstream-releases

<<<<<<< HEAD
  renderScope() {
    const { scope } = this.props.target.details;

    return Localized(
      {
        id: "about-debugging-worker-scope",
        attrs: { label: true },
      },
      FieldPair(
        {
          slug: "scope",
          label: "Scope",
          value: scope,
        }
      ),
    );
||||||| merged common ancestors
  renderField(key, name, value) {
    return [
      dom.dt({ key: `${ key }-dt` }, name),
      dom.dd(
        {
          className: "ellipsis-text",
          key: `${ key }-dd`,
          title: value,
        },
        value,
      ),
    ];
=======
    return Localized(
      {
        id: localizationId,
        attrs: {
          label: true,
          value: true,
        },
      },
      FieldPair({
        className: isListening
          ? "qa-worker-fetch-listening"
          : "qa-worker-fetch-not-listening",
        label: "Fetch",
        slug: "fetch",
        value: "about-debugging-worker-fetch-value",
      })
    );
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  renderStatus() {
    const status = this.props.target.details.status.toLowerCase();
||||||| merged common ancestors
  renderStatus() {
    const status = this.props.target.details.status.toLowerCase();
    const ftlId = this.getStatusFtlId(status);
=======
  renderPushService() {
    const { pushServiceEndpoint } = this.props.target.details;
>>>>>>> upstream-releases

    return FieldPair(
      {
<<<<<<< HEAD
        slug: "status",
        label: Localized(
          {
            id: "about-debugging-worker-status",
            $status: status,
          },
          dom.span(
            { className: `badge ${status === "running" ? "badge--success" : ""}`},
            status
          )
        ),
      }
||||||| merged common ancestors
        id: ftlId,
      },
      dom.div(
        {
          className: `worker-detail__status worker-detail__status--${ status }`,
        },
        status
      )
=======
        id: "about-debugging-worker-push-service",
        attrs: { label: true },
      },
      FieldPair({
        slug: "push-service",
        label: "Push Service",
        value: dom.span(
          {
            className: "qa-worker-push-service-value",
          },
          pushServiceEndpoint
        ),
      })
    );
  }

  renderScope() {
    const { scope } = this.props.target.details;

    return Localized(
      {
        id: "about-debugging-worker-scope",
        attrs: { label: true },
      },
      FieldPair({
        slug: "scope",
        label: "Scope",
        value: scope,
      })
>>>>>>> upstream-releases
    );
  }

  render() {
    const { fetch, pushServiceEndpoint, scope } = this.props.target.details;

    const isEmptyList = !pushServiceEndpoint && !fetch && !scope && !status;

    return dom.dl(
      {
        className:
          "debug-target-item__detail" +
          (isEmptyList ? " debug-target-item__detail--empty" : ""),
      },
      pushServiceEndpoint ? this.renderPushService() : null,
      fetch ? this.renderFetch() : null,
<<<<<<< HEAD
      scope ? this.renderScope() : null,
      status ? this.renderStatus() : null,
||||||| merged common ancestors
      scope ? this.renderField("scope", "Scope", scope) : null,
      status ? this.renderStatus() : null,
=======
      scope ? this.renderScope() : null
>>>>>>> upstream-releases
    );
  }
}

module.exports = FluentReact.withLocalization(WorkerDetail);
