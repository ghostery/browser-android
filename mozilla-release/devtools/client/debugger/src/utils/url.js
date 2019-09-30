/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/url.js

||||||| merged common ancestors
=======

// @flow
import { memoize } from "lodash";
import { URL } from "whatwg-url";

>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/url.js
const defaultUrl = {
  hash: "",
  host: "",
  hostname: "",
  href: "",
  origin: "null",
  password: "",
  path: "",
  pathname: "",
  port: "",
  protocol: "",
  search: "",
  // This should be a "URLSearchParams" object
  searchParams: {},
  username: "",
};

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/url.js
export function parse(url: string): URL | object {
||||||| merged common ancestors
function parse(url) {
=======
export const parse = memoize(function parse(url: string): any {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/url.js
  try {
    const urlObj = new URL(url);
    (urlObj: any).path = urlObj.pathname + urlObj.search;
    return urlObj;
  } catch (err) {
    // If we're given simply a filename...
    if (url) {
      return { ...defaultUrl, path: url, pathname: url };
    }

    return defaultUrl;
  }
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/url.js
}
||||||| merged common ancestors
}
=======
});
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/url.js
