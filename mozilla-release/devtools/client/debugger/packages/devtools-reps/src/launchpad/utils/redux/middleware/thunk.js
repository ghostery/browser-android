/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/actions/utils/middleware/thunk.js
// @flow

/* global window */

import type { ThunkArgs, ActionType } from "../../types";

||||||| merged common ancestors
/* global window */

=======
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/packages/devtools-reps/src/launchpad/utils/redux/middleware/thunk.js
/**
 * A middleware that allows thunks (functions) to be dispatched. If
 * it's a thunk, it is called with an argument that contains
 * `dispatch`, `getState`, and any additional args passed in via the
 * middleware constructure. This allows the action to create multiple
 * actions (most likely asynchronously).
 */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/actions/utils/middleware/thunk.js
export function thunk(makeArgs: any) {
  return ({ dispatch, getState }: ThunkArgs) => {
    const args = { dispatch, getState };

    return (next: Function) => (action: ActionType) => {
      return typeof action === "function"
        ? action(makeArgs ? makeArgs(args, getState()) : args)
        : next(action);
||||||| merged common ancestors
function thunk(makeArgs) {
  return ({
    dispatch,
    getState
  }) => {
    const args = {
      dispatch,
      getState
    };
    return next => action => {
      return typeof action === "function" ? action(makeArgs ? makeArgs(args, getState()) : args) : next(action);
=======
function thunk(makeArgs) {
  return ({ dispatch, getState }) => {
    const args = { dispatch, getState };

    return next => action => {
      return typeof action === "function"
        ? action(makeArgs ? makeArgs(args, getState()) : args)
        : next(action);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/packages/devtools-reps/src/launchpad/utils/redux/middleware/thunk.js
    };
  };
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/actions/utils/middleware/thunk.js
}
||||||| merged common ancestors
}
=======
}
exports.thunk = thunk;
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/packages/devtools-reps/src/launchpad/utils/redux/middleware/thunk.js
