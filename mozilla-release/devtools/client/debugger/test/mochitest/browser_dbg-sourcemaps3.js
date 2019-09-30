/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

// Tests loading sourcemapped sources, setting breakpoints, and
// inspecting restored scopes.
requestLongerTimeout(2);

// This source map does not have source contents, so it's fetched separately
add_task(async function() {
  // NOTE: the CORS call makes the test run times inconsistent
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sourcemaps3.js
  await pushPref("devtools.debugger.features.map-scopes", true);

  const dbg = await initDebugger("doc-sourcemaps3.html", "bundle.js", "sorted.js", "test.js");
  const {
    selectors: { getBreakpoint, getBreakpointCount },
    getState
  } = dbg;

||||||| merged common ancestors
  await pushPref("devtools.debugger.features.map-scopes", true);

  const dbg = await initDebugger("doc-sourcemaps3.html");
  const {
    selectors: { getBreakpoint, getBreakpoints },
    getState
  } = dbg;

  await waitForSources(dbg, "bundle.js", "sorted.js", "test.js");

=======
  const dbg = await initDebugger(
    "doc-sourcemaps3.html",
    "bundle.js",
    "sorted.js",
    "test.js"
  );
  dbg.actions.toggleMapScopes();

>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sourcemaps3.js
  ok(true, "Original sources exist");
  const sortedSrc = findSource(dbg, "sorted.js");

  await selectSource(dbg, sortedSrc);

  // Test that breakpoint is not off by a line.
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sourcemaps3.js
  await addBreakpoint(dbg, sortedSrc, 9);
  is(getBreakpointCount(getState()), 1, "One breakpoint exists");
||||||| merged common ancestors
  await addBreakpoint(dbg, sortedSrc, 9);
  is(getBreakpoints(getState()).size, 1, "One breakpoint exists");
=======
  await addBreakpoint(dbg, sortedSrc, 9, 4);
  is(dbg.selectors.getBreakpointCount(), 1, "One breakpoint exists");
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sourcemaps3.js
  ok(
    dbg.selectors.getBreakpoint({ sourceId: sortedSrc.id, line: 9, column: 4 }),
    "Breakpoint has correct line"
  );

  invokeInTab("test");

  await waitForPaused(dbg);
  assertPausedLocation(dbg);

  is(getScopeLabel(dbg, 1), "Block");
  is(getScopeLabel(dbg, 2), "na");
  is(getScopeLabel(dbg, 3), "nb");

  is(getScopeLabel(dbg, 4), "Function Body");

  await toggleScopeNode(dbg, 4);

  is(getScopeLabel(dbg, 5), "ma");
  is(getScopeLabel(dbg, 6), "mb");

  await toggleScopeNode(dbg, 7);

  is(getScopeLabel(dbg, 8), "a");
  is(getScopeLabel(dbg, 9), "b");

  is(getScopeLabel(dbg, 10), "Module");

  await toggleScopeNode(dbg, 10);

  is(getScopeLabel(dbg, 11), "binaryLookup:o()");
  is(getScopeLabel(dbg, 12), "comparer:t()");
  is(getScopeLabel(dbg, 13), "fancySort");
});
