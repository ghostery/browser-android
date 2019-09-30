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

class NetworkLocationsList extends PureComponent {
  static get propTypes() {
    return {
      dispatch: PropTypes.func.isRequired,
      networkLocations: PropTypes.arrayOf(Types.location).isRequired,
    };
  }

  renderList() {
    return dom.ul(
      {},
      this.props.networkLocations.map(location =>
        dom.li(
          {
            className: "network-location qa-network-location",
            key: location,
          },
          dom.span(
            {
              className: "ellipsis-text qa-network-location-value",
            },
            location
          ),
          Localized(
            {
              id: "about-debugging-network-locations-remove-button",
            },
            dom.button(
              {
<<<<<<< HEAD
                className: "default-button js-network-location-remove-button",
||||||| merged common ancestors
                className: "std-button js-network-location-remove-button",
=======
                className: "default-button qa-network-location-remove-button",
>>>>>>> upstream-releases
                onClick: () => {
                  this.props.dispatch(Actions.removeNetworkLocation(location));
                },
              },
              "Remove"
            )
          )
        )
      )
    );
  }

  render() {
    return this.props.networkLocations.length > 0 ? this.renderList() : null;
  }
}

module.exports = NetworkLocationsList;
