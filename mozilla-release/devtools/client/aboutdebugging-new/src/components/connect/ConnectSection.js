/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

const { PureComponent } = require("devtools/client/shared/vendor/react");
const dom = require("devtools/client/shared/vendor/react-dom-factories");
const PropTypes = require("devtools/client/shared/vendor/react-prop-types");

class ConnectSection extends PureComponent {
  static get propTypes() {
    return {
<<<<<<< HEAD
      children: PropTypes.node.isRequired,
||||||| merged common ancestors
      children: PropTypes.any.isRequired,
=======
      children: PropTypes.node,
>>>>>>> upstream-releases
      className: PropTypes.string,
      extraContent: PropTypes.node,
      icon: PropTypes.string.isRequired,
      title: PropTypes.node.isRequired,
    };
  }

  renderExtraContent() {
    const { extraContent } = this.props;
    return dom.section(
      {
        className: "connect-section__extra",
      },
      extraContent
    );
  }

  render() {
    const { extraContent } = this.props;

    return dom.section(
      {
        className: `card connect-section ${this.props.className || ""}`,
      },
      dom.header(
        {
<<<<<<< HEAD
          className: "alt-subheading",
||||||| merged common ancestors
          className: "page__section__title",
=======
          className: "connect-section__header",
>>>>>>> upstream-releases
        },
        dom.img({
          className: "connect-section__header__icon",
          src: this.props.icon,
        }),
        dom.h1(
          {
<<<<<<< HEAD
            className: "alt-subheading__icon",
            src: this.props.icon,
          }
        ),
        this.props.title
||||||| merged common ancestors
            className: "page__section__icon",
            src: this.props.icon,
          }
        ),
        this.props.title
=======
            className: "card__heading connect-section__header__title",
          },
          this.props.title
        )
>>>>>>> upstream-releases
      ),
      this.props.children
        ? dom.div(
            {
              className: "connect-section__content",
            },
            this.props.children
          )
        : null,
      extraContent ? this.renderExtraContent() : null
    );
  }
}

module.exports = ConnectSection;
