/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

// Test that we can properly stream heap snapshot files over the RDP as bulk
// data.

const { OS } = require("resource://gre/modules/osfile.jsm");

<<<<<<< HEAD
add_task(async () => {
  const { client, memoryFront } = await createTabMemoryFront();

||||||| merged common ancestors
const run_test = makeMemoryActorTest(async function(client, memoryFront) {
=======
add_task(async () => {
  const { memoryFront } = await createTabMemoryFront();

>>>>>>> upstream-releases
  const snapshotFilePath = await memoryFront.saveHeapSnapshot({
    forceCopy: true,
  });
  ok(
    !!(await OS.File.stat(snapshotFilePath)),
    "Should have the heap snapshot file"
  );
  const snapshot = ChromeUtils.readHeapSnapshot(snapshotFilePath);
<<<<<<< HEAD
  ok(snapshot instanceof HeapSnapshot,
     "And we should be able to read a HeapSnapshot instance from the file");

  await memoryFront.detach();
  await finishClient(client);
||||||| merged common ancestors
  ok(snapshot instanceof HeapSnapshot,
     "And we should be able to read a HeapSnapshot instance from the file");
=======
  ok(
    snapshot instanceof HeapSnapshot,
    "And we should be able to read a HeapSnapshot instance from the file"
  );
>>>>>>> upstream-releases
});
