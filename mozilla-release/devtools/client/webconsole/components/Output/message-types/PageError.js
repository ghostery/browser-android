/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

"use strict";

// React & Redux
const { createFactory } = require("devtools/client/shared/vendor/react");
const PropTypes = require("devtools/client/shared/vendor/react-prop-types");
const Message = createFactory(
  require("devtools/client/webconsole/components/Output/Message")
);
const { MODE, REPS } = require("devtools/client/shared/components/reps/reps");

PageError.displayName = "PageError";

PageError.propTypes = {
  message: PropTypes.object.isRequired,
  open: PropTypes.bool,
  timestampsVisible: PropTypes.bool.isRequired,
  serviceContainer: PropTypes.object,
<<<<<<< HEAD:mozilla-release/devtools/client/webconsole/components/message-types/PageError.js
  maybeScrollToBottom: PropTypes.func,
||||||| merged common ancestors
=======
  maybeScrollToBottom: PropTypes.func,
  inWarningGroup: PropTypes.bool.isRequired,
>>>>>>> upstream-releases:mozilla-release/devtools/client/webconsole/components/Output/message-types/PageError.js
};

PageError.defaultProps = {
  open: false,
};

function PageError(props) {
  const {
    dispatch,
    message,
    open,
    repeat,
    serviceContainer,
    timestampsVisible,
    isPaused,
<<<<<<< HEAD:mozilla-release/devtools/client/webconsole/components/message-types/PageError.js
    maybeScrollToBottom,
||||||| merged common ancestors
=======
    maybeScrollToBottom,
    inWarningGroup,
>>>>>>> upstream-releases:mozilla-release/devtools/client/webconsole/components/Output/message-types/PageError.js
  } = props;
  const {
    id: messageId,
<<<<<<< HEAD:mozilla-release/devtools/client/webconsole/components/message-types/PageError.js
    executionPoint,
    indent,
||||||| merged common ancestors
    indent,
=======
    executionPoint,
>>>>>>> upstream-releases:mozilla-release/devtools/client/webconsole/components/Output/message-types/PageError.js
    source,
    type,
    level,
    messageText,
    stacktrace,
    frame,
    exceptionDocURL,
    timeStamp,
    notes,
  } = message;

  const messageBody = REPS.StringRep.rep({
    object: messageText,
    mode: MODE.LONG,
    useQuotes: false,
    escapeWhitespace: false,
    urlCropLimit: 120,
    openLink: serviceContainer.openLink,
  });

  return Message({
    dispatch,
    messageId,
    executionPoint,
    isPaused,
    open,
    collapsible: Array.isArray(stacktrace),
    source,
    type,
    level,
    topLevelClasses: [],
    indent: message.indent,
    inWarningGroup,
    messageBody,
    repeat,
    frame,
    stacktrace,
    serviceContainer,
    exceptionDocURL,
    timeStamp,
    notes,
    timestampsVisible,
<<<<<<< HEAD:mozilla-release/devtools/client/webconsole/components/message-types/PageError.js
    maybeScrollToBottom,
||||||| merged common ancestors
=======
    maybeScrollToBottom,
    message,
>>>>>>> upstream-releases:mozilla-release/devtools/client/webconsole/components/Output/message-types/PageError.js
  });
}

module.exports = PageError;
