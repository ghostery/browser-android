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

const Actions = require("../../actions/index");
const Types = require("../../types/index");

/**
 * This component provides inspect button.
 */
class InspectAction extends PureComponent {
  static get propTypes() {
    return {
      dispatch: PropTypes.func.isRequired,
<<<<<<< HEAD
      target: Types.debugTarget.isRequired,
||||||| merged common ancestors
      target: PropTypes.object.isRequired,
=======
      target: Types.debugTarget.isRequired,
      disabled: PropTypes.bool,
>>>>>>> upstream-releases
    };
  }

  inspect() {
    const { dispatch, target } = this.props;
    dispatch(Actions.inspectDebugTarget(target.type, target.id));
  }

  render() {
    const { disabled } = this.props;

    return Localized(
      {
        id: "about-debugging-debug-target-inspect-button",
      },
      dom.button(
        {
          onClick: e => this.inspect(),
<<<<<<< HEAD
          className: "default-button  js-debug-target-inspect-button",
||||||| merged common ancestors
          className: "aboutdebugging-button",
=======
          className: "default-button  qa-debug-target-inspect-button",
          disabled,
>>>>>>> upstream-releases
        },
        "Inspect"
      )
    );
  }
}

module.exports = InspectAction;
