/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import React from "react";

export class OnboardingCard extends React.PureComponent {
  constructor(props) {
    super(props);
    this.onClick = this.onClick.bind(this);
  }

  onClick() {
    const { props } = this;
    const ping = {
      event: "CLICK_BUTTON",
      message_id: props.id,
      id: props.UISurface,
    };
    props.sendUserActionTelemetry(ping);
    props.onAction(props.content.primary_button.action);
  }

  render() {
    const { content } = this.props;
    const className = this.props.className || "onboardingMessage";
    return (
      <div className={className}>
        <div className={`onboardingMessageImage ${content.icon}`} />
        <div className="onboardingContent">
          <span>
            <h3
              className="onboardingTitle"
              data-l10n-id={content.title.string_id}
            />
            <p
              className="onboardingText"
              data-l10n-id={content.text.string_id}
            />
          </span>
<<<<<<< HEAD
          <span>
            <button tabIndex="1" className="button onboardingButton" onClick={this.onClick}> {content.primary_button.label} </button>
||||||| merged common ancestors
          <span>
            <button tabIndex="1" className="button onboardingButton" onClick={this.onClick}> {content.button_label} </button>
=======
          <span className="onboardingButtonContainer">
            <button
              data-l10n-id={content.primary_button.label.string_id}
              className="button onboardingButton"
              onClick={this.onClick}
            />
>>>>>>> upstream-releases
          </span>
        </div>
      </div>
    );
  }
}
