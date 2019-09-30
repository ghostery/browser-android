/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

// @flow

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/defer.js
export default function defer() {
  let resolve = () => {};
  let reject = () => {};
  const promise: Promise<any> = new Promise((_res, _rej) => {
||||||| merged common ancestors
  const promise = new Promise((_res, _rej) => {
=======
type Deferred<T> = {
  promise: Promise<T>,
  resolve: (arg: T) => mixed,
  reject: (arg: mixed) => mixed,
};

export default function defer<T>(): Deferred<T> {
  let resolve = () => {};
  let reject = () => {};
  const promise = new Promise((_res, _rej) => {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/defer.js
    resolve = _res;
    reject = _rej;
  });

  return { resolve, reject, promise };
}
