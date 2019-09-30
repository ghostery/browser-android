/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/isMinified.js

// @flow

import type { Source } from "../types";

||||||| merged common ancestors
=======

// @flow

import type { SourceWithContent } from "../types";
import { isFulfilled } from "./async-value";

>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/isMinified.js
// Used to detect minification for automatic pretty printing
const SAMPLE_SIZE = 50;
const INDENT_COUNT_THRESHOLD = 5;
const CHARACTER_LIMIT = 250;
const _minifiedCache = new Map();

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/isMinified.js
export function isMinified(source: Source) {
||||||| merged common ancestors
function isMinified(source) {
=======
export function isMinified({ source, content }: SourceWithContent) {
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/isMinified.js
  if (_minifiedCache.has(source.id)) {
    return _minifiedCache.get(source.id);
  }

  if (!content || !isFulfilled(content) || content.value.type !== "text") {
    return false;
  }

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/src/utils/isMinified.js
  let text = source.text;
  if (!text) {
    return false;
  }
||||||| merged common ancestors
  let text = source.text;

  if (!text) {
    return false;
  }
=======
  let text = content.value.value;
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/src/utils/isMinified.js

  let lineEndIndex = 0;
  let lineStartIndex = 0;
  let lines = 0;
  let indentCount = 0;
  let overCharLimit = false;

  // Strip comments.
  text = text.replace(/\/\*[\S\s]*?\*\/|\/\/(.+|\n)/g, "");

  while (lines++ < SAMPLE_SIZE) {
    lineEndIndex = text.indexOf("\n", lineStartIndex);
    if (lineEndIndex == -1) {
      break;
    }
    if (/^\s+/.test(text.slice(lineStartIndex, lineEndIndex))) {
      indentCount++;
    }
    // For files with no indents but are not minified.
    if (lineEndIndex - lineStartIndex > CHARACTER_LIMIT) {
      overCharLimit = true;
      break;
    }
    lineStartIndex = lineEndIndex + 1;
  }

  const minified =
    (indentCount / lines) * 100 < INDENT_COUNT_THRESHOLD || overCharLimit;

  _minifiedCache.set(source.id, minified);
  return minified;
}
