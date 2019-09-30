/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
requestLongerTimeout(2);

async function waitForBreakpointCount(dbg, count) {
  return waitForState(
    dbg,
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sourcemaps-reloading.js
    state => dbg.selectors.getBreakpointCount(state) === count
||||||| merged common ancestors
    state => dbg.selectors.getBreakpoints(state).size === count
=======
    state => dbg.selectors.getBreakpointCount() === count
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sourcemaps-reloading.js
  );
}

add_task(async function() {
  // NOTE: the CORS call makes the test run times inconsistent
  const dbg = await initDebugger("doc-sourcemaps.html");
  const {
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sourcemaps-reloading.js
    selectors: { getBreakpoint, getBreakpointCount },
    getState
||||||| merged common ancestors
    selectors: { getBreakpoint, getBreakpoints },
    getState
=======
    selectors: { getBreakpoint, getBreakpointCount }
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sourcemaps-reloading.js
  } = dbg;

  await waitForSources(dbg, "entry.js", "output.js", "times2.js", "opts.js");
  ok(true, "Original sources exist");
  const entrySrc = findSource(dbg, "entry.js");

  await selectSource(dbg, entrySrc);
  ok(
    getCM(dbg)
      .getValue()
      .includes("window.keepMeAlive"),
    "Original source text loaded correctly"
  );

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sourcemaps-reloading.js
  // Test that breakpoint sliding is not attempted. The breakpoint
  // should not move anywhere.
  await addBreakpoint(dbg, entrySrc, 13);
  is(getBreakpointCount(getState()), 1, "One breakpoint exists");

  ok(
    getBreakpoint(getState(), { sourceId: entrySrc.id, line: 13 }),
    "Breakpoint has correct line"
  );

||||||| merged common ancestors
  // Test that breakpoint sliding is not attempted. The breakpoint
  // should not move anywhere.
  await addBreakpoint(dbg, entrySrc, 13);
  is(getBreakpoints(getState()).size, 1, "One breakpoint exists");

  ok(
    getBreakpoint(getState(), { sourceId: entrySrc.id, line: 13 }),
    "Breakpoint has correct line"
  );

=======
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sourcemaps-reloading.js
  await addBreakpoint(dbg, entrySrc, 5);
  await addBreakpoint(dbg, entrySrc, 15, 0);
  await disableBreakpoint(dbg, entrySrc, 15, 0);

  // Test reloading the debugger
  await reload(dbg, "opts.js");
  await waitForDispatch(dbg, "LOAD_SOURCE_TEXT");

  await waitForPaused(dbg);
  assertPausedLocation(dbg);

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sourcemaps-reloading.js
  await waitForBreakpointCount(dbg, 3);
  is(getBreakpointCount(getState()), 3, "Three breakpoints exist");
||||||| merged common ancestors
  await waitForBreakpointCount(dbg, 3);
  is(getBreakpoints(getState()).size, 3, "Three breakpoints exist");
=======
  await waitForBreakpointCount(dbg, 2);
  is(getBreakpointCount(), 2, "Three breakpoints exist");
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sourcemaps-reloading.js

  ok(
    getBreakpoint({ sourceId: entrySrc.id, line: 15, column: 0 }),
    "Breakpoint has correct line"
  );

  ok(
    getBreakpoint({
      sourceId: entrySrc.id,
      line: 15,
      column: 0,
      disabled: true
    }),
    "Breakpoint has correct line"
  );
});
