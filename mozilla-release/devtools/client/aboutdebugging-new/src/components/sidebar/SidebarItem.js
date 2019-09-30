/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

<<<<<<< HEAD
const { createFactory, PureComponent } = require("devtools/client/shared/vendor/react");
||||||| merged common ancestors
const { PureComponent } = require("devtools/client/shared/vendor/react");
=======
const {
  createFactory,
  PureComponent,
} = require("devtools/client/shared/vendor/react");
>>>>>>> upstream-releases
const dom = require("devtools/client/shared/vendor/react-dom-factories");
const PropTypes = require("devtools/client/shared/vendor/react-prop-types");
<<<<<<< HEAD
const Link = createFactory(require("devtools/client/shared/vendor/react-router-dom").Link);
||||||| merged common ancestors
=======
const Link = createFactory(
  require("devtools/client/shared/vendor/react-router-dom").Link
);
>>>>>>> upstream-releases

/**
 * This component is used as a wrapper by items in the sidebar.
 */
class SidebarItem extends PureComponent {
  static get propTypes() {
    return {
      children: PropTypes.node.isRequired,
      className: PropTypes.string,
      isSelected: PropTypes.bool.isRequired,
      to: PropTypes.string,
    };
  }

<<<<<<< HEAD
  renderContent() {
    const { children, to } = this.props;

    if (to) {
      return Link(
        {
          className: "sidebar-item__link js-sidebar-link",
          to,
        },
        children
      );
    }

    return children;
||||||| merged common ancestors
  onItemClick() {
    this.props.onSelect();
=======
  static get defaultProps() {
    return {
      isSelected: false,
    };
  }

  renderContent() {
    const { children, to } = this.props;

    if (to) {
      const isExternalUrl = /^http/.test(to);

      return isExternalUrl
        ? dom.a(
            {
              className: "sidebar-item__link",
              href: to,
              target: "_blank",
            },
            children
          )
        : Link(
            {
              className: "sidebar-item__link qa-sidebar-link",
              to,
            },
            children
          );
    }

    return children;
>>>>>>> upstream-releases
  }

  render() {
<<<<<<< HEAD
    const {className, isSelected, to } = this.props;
||||||| merged common ancestors
    const {children, className, isSelected, selectable } = this.props;
=======
    const { className, isSelected, to } = this.props;
>>>>>>> upstream-releases

    return dom.li(
      {
<<<<<<< HEAD
        className: "sidebar-item js-sidebar-item" +
                   (className ? ` ${className}` : "") +
                   (isSelected ?
                      " sidebar-item--selected js-sidebar-item-selected" :
                      ""
                   ) +
                   (to ? " sidebar-item--selectable" : ""),
||||||| merged common ancestors
        className: "sidebar-item js-sidebar-item" +
                   (className ? ` ${className}` : "") +
                   (isSelected ?
                      " sidebar-item--selected js-sidebar-item-selected" :
                      ""
                   ) +
                   (selectable ? " sidebar-item--selectable" : ""),
        onClick: selectable ? () => this.onItemClick() : null,
=======
        className:
          "sidebar-item qa-sidebar-item" +
          (className ? ` ${className}` : "") +
          (isSelected
            ? " sidebar-item--selected qa-sidebar-item-selected"
            : "") +
          (to ? " sidebar-item--selectable" : ""),
>>>>>>> upstream-releases
      },
      this.renderContent()
    );
  }
}

module.exports = SidebarItem;
