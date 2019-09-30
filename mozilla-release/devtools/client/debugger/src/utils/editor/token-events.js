<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/editor/token-events.js
import { getTokenLocation } from ".";
import { isEqual } from "lodash";

||||||| merged common ancestors
"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});
exports.onMouseOver = onMouseOver;

var _ = require("./index");

var _lodash = require("devtools/client/shared/vendor/lodash");

=======
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/editor/token-events.js
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/editor/token-events.js

function isInvalidTarget(target: HTMLElement) {
||||||| merged common ancestors
function isInvalidTarget(target) {
=======

// @flow

import { getTokenLocation } from ".";
import { isEqual } from "lodash";

function isInvalidTarget(target: HTMLElement) {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/editor/token-events.js
  if (!target || !target.innerText) {
    return true;
  }

  const tokenText = target.innerText.trim();
  const cursorPos = target.getBoundingClientRect();

  // exclude literal tokens where it does not make sense to show a preview
  const invalidType = ["cm-atom", ""].includes(target.className);

  // exclude syntax where the expression would be a syntax error
  const invalidToken =
    tokenText === "" || tokenText.match(/^[(){}\|&%,.;=<>\+-/\*\s](?=)/);

  // exclude codemirror elements that are not tokens
  const invalidTarget =
    (target.parentElement &&
      !target.parentElement.closest(".CodeMirror-line")) ||
    cursorPos.top == 0;

  const invalidClasses = ["editor-mount"];
  if (invalidClasses.some(className => target.classList.contains(className))) {
    return true;
  }

  if (target.closest(".popover")) {
    return true;
  }

  return invalidTarget || invalidToken || invalidType;
}

function dispatch(codeMirror, eventName, data) {
  codeMirror.constructor.signal(codeMirror, eventName, data);
}

function invalidLeaveTarget(target: ?HTMLElement) {
  if (!target || target.closest(".popover")) {
    return true;
  }

  return false;
}

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/editor/token-events.js
export function onMouseOver(codeMirror) {
||||||| merged common ancestors
function onMouseOver(codeMirror) {
=======
export function onMouseOver(codeMirror: any) {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/editor/token-events.js
  let prevTokenPos = null;

  function onMouseLeave(event) {
    if (invalidLeaveTarget(event.relatedTarget)) {
      return addMouseLeave(event.target);
    }

    prevTokenPos = null;
    dispatch(codeMirror, "tokenleave", event);
  }

  function addMouseLeave(target) {
    target.addEventListener("mouseleave", onMouseLeave, {
      capture: true,
      once: true,
    });
  }

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/editor/token-events.js
  return enterEvent => {
    const { target } = enterEvent;
||||||| merged common ancestors
  return enterEvent => {
    const {
      target
    } = enterEvent;
=======
  return (enterEvent: any) => {
    const { target } = enterEvent;
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/editor/token-events.js

    if (isInvalidTarget(target)) {
      return;
    }

    const tokenPos = getTokenLocation(codeMirror, target);

    if (!isEqual(prevTokenPos, tokenPos)) {
      addMouseLeave(target);

      dispatch(codeMirror, "tokenenter", {
        event: enterEvent,
        target,
        tokenPos,
      });
      prevTokenPos = tokenPos;
    }
  };
}
