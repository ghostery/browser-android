/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

// @flow

import type { Why } from "../../../types";
import type { NamedValue } from "./types";

export function getFramePopVariables(why: Why, path: string): NamedValue[] {
  const vars: Array<NamedValue> = [];

  if (why && why.frameFinished) {
    const frameFinished = why.frameFinished;

    // Always display a `throw` property if present, even if it is falsy.
    if (Object.prototype.hasOwnProperty.call(frameFinished, "throw")) {
      vars.push({
        name: "<exception>",
        path: `${path}/<exception>`,
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/pause/scopes/utils.js
        contents: { value: frameFinished.throw }
||||||| merged common ancestors
        contents: {
          value: frameFinished.throw
        }
=======
        contents: { value: frameFinished.throw },
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/pause/scopes/utils.js
      });
    }

    if (Object.prototype.hasOwnProperty.call(frameFinished, "return")) {
      const returned = frameFinished.return;

      // Do not display undefined. Do display falsy values like 0 and false. The
      // protocol grip for undefined is a JSON object: { type: "undefined" }.
      if (typeof returned !== "object" || returned.type !== "undefined") {
        vars.push({
          name: "<return>",
          path: `${path}/<return>`,
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/pause/scopes/utils.js
          contents: { value: returned }
||||||| merged common ancestors
          contents: {
            value: returned
          }
=======
          contents: { value: returned },
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/pause/scopes/utils.js
        });
      }
    }
  }

  return vars;
}

export function getThisVariable(this_: any, path: string): ?NamedValue {
  if (!this_) {
    return null;
  }

  return {
    name: "<this>",
    path: `${path}/<this>`,
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/pause/scopes/utils.js
    contents: { value: this_ }
||||||| merged common ancestors
    contents: {
      value: this_
    }
=======
    contents: { value: this_ },
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/pause/scopes/utils.js
  };
}
