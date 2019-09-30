/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/actions/pause/breakOnNext.js
// @flow

import type { ThunkArgs } from "../types";

||||||| merged common ancestors
=======
// @flow

import type { ThunkArgs } from "../types";
import type { ThreadContext } from "../../types";

>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/actions/pause/breakOnNext.js
/**
 * Debugger breakOnNext command.
 * It's different from the comand action because we also want to
 * highlight the pause icon.
 *
 * @memberof actions/pause
 * @static
 */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/actions/pause/breakOnNext.js
export function breakOnNext() {
  return async ({ dispatch, client }: ThunkArgs) => {
    await client.breakOnNext();
    return dispatch({ type: "BREAK_ON_NEXT" });
||||||| merged common ancestors
function breakOnNext() {
  return async ({
    dispatch,
    client
  }) => {
    await client.breakOnNext();
    return dispatch({
      type: "BREAK_ON_NEXT"
    });
=======
export function breakOnNext(cx: ThreadContext) {
  return async ({ dispatch, getState, client }: ThunkArgs) => {
    await client.breakOnNext(cx.thread);
    return dispatch({ type: "BREAK_ON_NEXT", thread: cx.thread });
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/actions/pause/breakOnNext.js
  };
}
