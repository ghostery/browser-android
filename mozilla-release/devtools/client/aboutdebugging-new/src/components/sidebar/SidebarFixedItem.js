/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const {
  PureComponent,
  createFactory,
} = require("devtools/client/shared/vendor/react");
const dom = require("devtools/client/shared/vendor/react-dom-factories");
const PropTypes = require("devtools/client/shared/vendor/react-prop-types");

const SidebarItem = createFactory(require("./SidebarItem"));

/**
 * This component displays a fixed item in the Sidebar component.
 */
class SidebarFixedItem extends PureComponent {
  static get propTypes() {
    return {
      icon: PropTypes.string.isRequired,
      isSelected: PropTypes.bool.isRequired,
      name: PropTypes.string.isRequired,
      to: PropTypes.string,
    };
  }

  render() {
<<<<<<< HEAD
    const {
      icon,
      isSelected,
      name,
      to,
    } = this.props;
||||||| merged common ancestors
    const {
      dispatch,
      id,
      icon,
      isSelected,
      name,
      runtimeId,
    } = this.props;
=======
    const { icon, isSelected, name, to } = this.props;
>>>>>>> upstream-releases

    return SidebarItem(
      {
        className: "sidebar-item--tall",
        isSelected,
        to,
      },
<<<<<<< HEAD
      dom.div(
        {
          className: "sidebar-fixed-item__container",
        },
        dom.img(
          {
            className: "sidebar-fixed-item__icon",
            src: icon,
          }
        ),
        dom.span(
          {
            className: "ellipsis-text",
            title: name,
          },
          name
        )
||||||| merged common ancestors
      dom.img(
        {
          className: "sidebar-fixed-item__icon",
          src: icon,
        }
      ),
      dom.span(
        {
          className: "ellipsis-text",
          title: name,
        },
        name
=======
      dom.div(
        {
          className: "sidebar-fixed-item__container",
        },
        dom.img({
          className: "sidebar-fixed-item__icon",
          src: icon,
        }),
        dom.span(
          {
            className: "ellipsis-text",
            title: name,
          },
          name
        )
>>>>>>> upstream-releases
      )
    );
  }
}

module.exports = SidebarFixedItem;
