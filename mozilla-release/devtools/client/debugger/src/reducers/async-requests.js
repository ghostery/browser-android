/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

// @flow

/**
 * Async request reducer
 * @module reducers/async-request
 */

const initialAsyncRequestState = [];

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/reducers/async-requests.js
function update(state = initialAsyncRequestState, action) {
  const { seqId } = action;
||||||| merged common ancestors
function update(state = initialAsyncRequestState, action) {
  const {
    seqId
  } = action;
=======
function update(state: string[] = initialAsyncRequestState, action: any) {
  const { seqId } = action;
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/reducers/async-requests.js

  if (action.type === "NAVIGATE") {
    return initialAsyncRequestState;
  } else if (seqId) {
    let newState;
    if (action.status === "start") {
      newState = [...state, seqId];
    } else if (action.status === "error" || action.status === "done") {
      newState = (state.filter(id => id !== seqId): string[]);
    }

    return newState;
  }

  return state;
}

export default update;
