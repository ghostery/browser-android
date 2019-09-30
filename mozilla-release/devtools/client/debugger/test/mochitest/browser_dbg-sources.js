/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */

// Tests that the source tree works.

add_task(async function() {
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sources.js
  const dbg = await initDebugger("doc-sources.html", "simple1", "simple2", "nested-source", "long.js");
||||||| merged common ancestors
  const dbg = await initDebugger("doc-sources.html");
=======
  const dbg = await initDebugger(
    "doc-sources.html",
    "simple1",
    "simple2",
    "nested-source",
    "long.js"
  );
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sources.js
  const {
    selectors: { getSelectedSource },
    getState
  } = dbg;

  // Expand nodes and make sure more sources appear.
  await assertSourceCount(dbg, 3);
  await clickElement(dbg, "sourceDirectoryLabel", 3);

  await assertSourceCount(dbg, 8);
  await clickElement(dbg, "sourceDirectoryLabel", 4);
  await assertSourceCount(dbg, 9);

  const selected = waitForDispatch(dbg, "SET_SELECTED_LOCATION");
  await clickElement(dbg, "sourceNode", 5);
  await selected;
  await waitForSelectedSource(dbg);

  // Ensure the source file clicked is now focused
  await waitForElementWithSelector(dbg, ".sources-list .focused");

  const focusedNode = findElementWithSelector(dbg, ".sources-list .focused");
  const fourthNode = findElement(dbg, "sourceNode", 5);
  const selectedSource = getSelectedSource().url;

  ok(fourthNode.classList.contains("focused"), "4th node is focused");
  ok(selectedSource.includes("nested-source.js"), "nested-source is selected");
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sources.js
  await assertNodeIsFocused(dbg, 4);
||||||| merged common ancestors

=======
  await assertNodeIsFocused(dbg, 5);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sources.js
  await waitForSelectedSource(dbg, "nested-source");

  // Make sure new sources appear in the list.
  ContentTask.spawn(gBrowser.selectedBrowser, null, function() {
    const script = content.document.createElement("script");
    script.src = "math.min.js";
    content.document.body.appendChild(script);
  });

<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-sources.js
  await waitForSourceCount(dbg, 9);
  await assertNodeIsFocused(dbg, 4);
||||||| merged common ancestors
  await waitForSourceCount(dbg, 9);
=======
  await waitForSourceCount(dbg, 10);
  await assertNodeIsFocused(dbg, 5);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-sources.js
  is(
    getSourceNodeLabel(dbg, 8),
    "math.min.js",
    "math.min.js - The dynamic script exists"
  );
});
