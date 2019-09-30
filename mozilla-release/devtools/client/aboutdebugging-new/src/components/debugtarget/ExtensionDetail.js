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

<<<<<<< HEAD
const FieldPair = createFactory(require("./FieldPair"));

const Types = require("../../types/index");

||||||| merged common ancestors
=======
const DetailsLog = createFactory(require("../shared/DetailsLog"));
const FieldPair = createFactory(require("./FieldPair"));
const Message = createFactory(require("../shared/Message"));

const { MESSAGE_LEVEL } = require("../../constants");
const Types = require("../../types/index");

>>>>>>> upstream-releases
/**
 * This component displays detail information for extension.
 */
class ExtensionDetail extends PureComponent {
  static get propTypes() {
    return {
      children: PropTypes.node,
      // Provided by wrapping the component with FluentReact.withLocalization.
      getString: PropTypes.func.isRequired,
      target: Types.debugTarget.isRequired,
    };
  }

<<<<<<< HEAD
  renderUUID() {
    const { target } = this.props;
    const { manifestURL, uuid } = target.details;

    const value = [
      uuid,
      Localized(
        {
          id: "about-debugging-extension-manifest-link",
          key: "manifest",
        },
        dom.a(
||||||| merged common ancestors
  renderField(key, name, value, title) {
    return [
      dom.dt({ key: `${ key }-dt` }, name),
      dom.dd(
        {
          className: "ellipsis-text",
          key: `${ key }-dd`,
          title: title || value,
        },
        value,
      ),
    ];
  }

  renderUUID() {
    const { target } = this.props;
    const { manifestURL, uuid } = target.details;

    const value = [
      uuid,
      Localized(
        {
          id: "about-debugging-extension-manifest-link",
          key: "manifest",
        },
        dom.a(
=======
  renderWarnings() {
    const { warnings } = this.props.target.details;

    if (!warnings.length) {
      return null;
    }

    return dom.section(
      {
        className: "debug-target-item__messages",
      },
      warnings.map((warning, index) => {
        return Message(
>>>>>>> upstream-releases
          {
            level: MESSAGE_LEVEL.WARNING,
            isCloseable: true,
            key: `warning-${index}`,
          },
          DetailsLog(
            {
              type: MESSAGE_LEVEL.WARNING,
            },
            dom.p(
              {
                className: "technical-text",
              },
              warning
            )
          )
        );
      })
    );
  }

<<<<<<< HEAD
    return Localized(
      {
        id: "about-debugging-extension-uuid",
        attrs: { label: true },
      },
      FieldPair(
        {
          slug: "uuid",
          label: "Internal UUID",
          value,
        }
      )
    );
  }

  renderLocation() {
    const { location } = this.props.target.details;

    return Localized(
      {
        id: "about-debugging-extension-location",
        attrs: { label: true },
      },
      FieldPair(
        {
          slug: "location",
          label: "Location",
          value: location,
        }
      )
    );
||||||| merged common ancestors
    const uuidName = this.props.getString("about-debugging-extension-uuid");
    return this.renderField("uuid", uuidName, value, uuid);
=======
  renderUUID() {
    const { uuid } = this.props.target.details;
    if (!uuid) {
      return null;
    }

    return Localized(
      {
        id: "about-debugging-extension-uuid",
        attrs: { label: true },
      },
      FieldPair({
        label: "Internal UUID",
        value: uuid,
      })
    );
>>>>>>> upstream-releases
  }

  renderExtensionId() {
    const { id } = this.props.target;

    return Localized(
      {
        id: "about-debugging-extension-id",
        attrs: { label: true },
      },
      FieldPair({
        label: "Extension ID",
        value: id,
      })
    );
  }

  renderLocation() {
    const { location } = this.props.target.details;
    if (!location) {
      return null;
    }

    return Localized(
      {
        id: "about-debugging-extension-location",
        attrs: { label: true },
      },
      FieldPair({
        label: "Location",
        value: location,
      })
    );
  }

  renderManifest() {
    const { manifestURL } = this.props.target.details;
    if (!manifestURL) {
      return null;
    }

<<<<<<< HEAD
    return dom.dl(
||||||| merged common ancestors
    const locationName = this.props.getString("about-debugging-extension-location");
    const idName = this.props.getString("about-debugging-extension-id");

    return dom.dl(
=======
    const link = dom.a(
      {
        className: "qa-manifest-url",
        href: manifestURL,
        target: "_blank",
      },
      manifestURL
    );

    return Localized(
      {
        id: "about-debugging-extension-manifest-url",
        attrs: { label: true },
      },
      FieldPair({
        label: "Manifest URL",
        value: link,
      })
    );
  }

  render() {
    return dom.section(
>>>>>>> upstream-releases
      {
        className: "debug-target-item__detail",
      },
<<<<<<< HEAD
      location ? this.renderLocation() : null,
      Localized(
        {
          id: "about-debugging-extension-id",
          attrs: { label: true },
        },
        FieldPair(
          {
            slug: "extension",
            label: "Extension ID",
            value: id,
          }
        )
      ),
      uuid ? this.renderUUID() : null,
||||||| merged common ancestors
      location ? this.renderField("location", locationName, location) : null,
      this.renderField("extension", idName, id),
      uuid ? this.renderUUID() : null,
=======
      this.renderWarnings(),
      dom.dl(
        {},
        this.renderLocation(),
        this.renderExtensionId(),
        this.renderUUID(),
        this.renderManifest(),
        this.props.children
      )
>>>>>>> upstream-releases
    );
  }
}

module.exports = FluentReact.withLocalization(ExtensionDetail);
