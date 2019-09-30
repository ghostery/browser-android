/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

// @flow

/* global window */

/**
 * Redux middleware that sets performance markers for all actions such that they
 * will appear in performance tooling under the User Timing API
 */

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/actions/utils/middleware/timing.js
const mark =
  window.performance && window.performance.mark
    ? window.performance.mark.bind(window.performance)
    : () => {};

const measure =
  window.performance && window.performance.measure
    ? window.performance.measure.bind(window.performance)
    : () => {};

export function timing(store) {
  return next => action => {
||||||| merged common ancestors
function timing(store) {
  return next => action => {
=======
const mark =
  window.performance && window.performance.mark
    ? window.performance.mark.bind(window.performance)
    : a => {};

const measure =
  window.performance && window.performance.measure
    ? window.performance.measure.bind(window.performance)
    : (a, b, c) => {};

export function timing(store: any) {
  return (next: any) => (action: any) => {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/actions/utils/middleware/timing.js
    mark(`${action.type}_start`);
    const result = next(action);
    mark(`${action.type}_end`);
    measure(`${action.type}`, `${action.type}_start`, `${action.type}_end`);
    return result;
  };
}
