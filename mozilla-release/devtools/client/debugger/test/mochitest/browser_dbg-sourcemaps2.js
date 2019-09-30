/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
requestLongerTimeout(2);

function assertBpInGutter(dbg, lineNumber) {
  const el = findElement(dbg, "breakpoint");
  const bpLineNumber = +el.querySelector(".CodeMirror-linenumber").innerText;
  is(
    bpLineNumber,
    lineNumber,
    "Breakpoint is on the correct line in the gutter"
  );
}

// Tests loading sourcemapped sources, setting breakpoints, and
// stepping in them.

// This source map does not have source contents, so it's fetched separately
add_task(async function() {
  // NOTE: the CORS call makes the test run times inconsistent
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sourcemaps2.js
  const dbg = await initDebugger("doc-sourcemaps2.html", "main.js", "main.min.js");
||||||| merged common ancestors
  const dbg = await initDebugger("doc-sourcemaps2.html");
=======
  const dbg = await initDebugger(
    "doc-sourcemaps2.html",
    "main.js",
    "main.min.js"
  );
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sourcemaps2.js
  const {
    selectors: { getBreakpoint, getBreakpointCount },
    getState
  } = dbg;

  ok(true, "Original sources exist");
  const mainSrc = findSource(dbg, "main.js");

  await selectSource(dbg, mainSrc);

  // Test that breakpoint is not off by a line.
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sourcemaps2.js
  await addBreakpoint(dbg, mainSrc, 4);
  is(getBreakpointCount(getState()), 1, "One breakpoint exists");
||||||| merged common ancestors
  await addBreakpoint(dbg, mainSrc, 4);
  is(getBreakpoints(getState()).size, 1, "One breakpoint exists");
=======
  await addBreakpoint(dbg, mainSrc, 4, 2);
  is(getBreakpointCount(), 1, "One breakpoint exists");
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sourcemaps2.js
  ok(
    getBreakpoint({ sourceId: mainSrc.id, line: 4, column: 2 }),
    "Breakpoint has correct line"
  );

  assertBpInGutter(dbg, 4);
  invokeInTab("logMessage");

  await waitForPaused(dbg);
  assertPausedLocation(dbg);

  // Tests the existence of the sourcemap link in the original source.
  ok(findElement(dbg, "sourceMapLink"), "Sourcemap link in original source");
  await selectSource(dbg, "main.min.js");

  ok(
    !findElement(dbg, "sourceMapLink"),
    "No Sourcemap link exists in generated source"
  );
});
