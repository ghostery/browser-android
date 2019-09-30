/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/worker.js

export type Message = {
  data: {
    id: string,
    method: string,
    args: Array<any>
  }
};

||||||| merged common ancestors
=======

// @flow

export type Message = {
  data: {
    id: string,
    method: string,
    args: Array<any>,
  },
};

>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/worker.js
let msgId = 1;
/**
 * @memberof utils/utils
 * @static
 */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/worker.js
function workerTask(worker: any, method: string) {
  return function(...args: any) {
||||||| merged common ancestors

function workerTask(worker, method) {
  return function (...args) {
=======
function workerTask(worker: any, method: string) {
  return function(...args: any): Promise<any> {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/worker.js
    return new Promise((resolve, reject) => {
      const id = msgId++;
      worker.postMessage({ id, method, args });

      const listener = ({ data: result }) => {
        if (result.id !== id) {
          return;
        }

        worker.removeEventListener("message", listener);
        if (result.error) {
          reject(result.error);
        } else {
          resolve(result.response);
        }
      };

      worker.addEventListener("message", listener);
    });
  };
}

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/worker.js
function workerHandler(publicInterface) {
  return function onTask(msg: Message) {
    const { id, method, args } = msg.data;
||||||| merged common ancestors
function workerHandler(publicInterface) {
  return function onTask(msg) {
    const {
      id,
      method,
      args
    } = msg.data;
=======
function workerHandler(publicInterface: any) {
  return function onTask(msg: Message) {
    const { id, method, args } = msg.data;
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/worker.js
    const response = publicInterface[method].apply(null, args);

    if (response instanceof Promise) {
      response
        .then(val => self.postMessage({ id, response: val }))
        .catch(error => self.postMessage({ id, error }));
    } else {
      self.postMessage({ id, response });
    }
  };
}

export { workerTask, workerHandler };
