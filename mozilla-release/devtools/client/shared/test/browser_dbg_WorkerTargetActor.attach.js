/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

// Check to make sure that a worker can be attached to a toolbox
// and that the console works.

// Import helpers for the workers
/* import-globals-from helper_workers.js */
Services.scriptloader.loadSubScript(
  "chrome://mochitests/content/browser/devtools/client/shared/test/helper_workers.js",
  this
);

var MAX_TOTAL_VIEWERS = "browser.sessionhistory.max_total_viewers";

var TAB1_URL = EXAMPLE_URL + "doc_WorkerTargetActor.attach-tab1.html";
var TAB2_URL = EXAMPLE_URL + "doc_WorkerTargetActor.attach-tab2.html";
var WORKER1_URL = "code_WorkerTargetActor.attach-worker1.js";
var WORKER2_URL = "code_WorkerTargetActor.attach-worker2.js";

add_task(async function() {
  const oldMaxTotalViewers = SpecialPowers.getIntPref(MAX_TOTAL_VIEWERS);
  SpecialPowers.setIntPref(MAX_TOTAL_VIEWERS, 10);

<<<<<<< HEAD
    const tab = yield addTab(TAB1_URL);
    const target = yield TargetFactory.forTab(tab);
    yield target.attach();
    const targetFront = target.activeTab;
    yield listWorkers(targetFront);

    // If a page still has pending network requests, it will not be moved into
    // the bfcache. Consequently, we cannot use waitForWorkerListChanged here,
    // because the worker is not guaranteed to have finished loading when it is
    // registered. Instead, we have to wait for the promise returned by
    // createWorker in the tab to be resolved.
    yield createWorkerInTab(tab, WORKER1_URL);
    let { workers } = yield listWorkers(targetFront);
    let workerTargetFront1 = findWorker(workers, WORKER1_URL);
    yield workerTargetFront1.attach();
    is(workerTargetFront1.isClosed, false, "worker in tab 1 should not be closed");

    executeSoon(() => {
      BrowserTestUtils.loadURI(tab.linkedBrowser, TAB2_URL);
    });
    yield waitForWorkerClose(workerTargetFront1);
    is(workerTargetFront1.isClosed, true, "worker in tab 1 should be closed");
||||||| merged common ancestors
    DebuggerServer.init();
    DebuggerServer.registerAllActors();

    const client = new DebuggerClient(DebuggerServer.connectPipe());
    yield connect(client);

    const tab = yield addTab(TAB1_URL);
    const { tabs } = yield listTabs(client);
    const [, targetFront] = yield attachTarget(client, findTab(tabs, TAB1_URL));
    yield listWorkers(targetFront);

    // If a page still has pending network requests, it will not be moved into
    // the bfcache. Consequently, we cannot use waitForWorkerListChanged here,
    // because the worker is not guaranteed to have finished loading when it is
    // registered. Instead, we have to wait for the promise returned by
    // createWorker in the tab to be resolved.
    yield createWorkerInTab(tab, WORKER1_URL);
    let { workers } = yield listWorkers(targetFront);
    let [, workerTargetFront1] = yield attachWorker(targetFront,
                                               findWorker(workers, WORKER1_URL));
    is(workerTargetFront1.isClosed, false, "worker in tab 1 should not be closed");

    executeSoon(() => {
      BrowserTestUtils.loadURI(tab.linkedBrowser, TAB2_URL);
    });
    yield waitForWorkerClose(workerTargetFront1);
    is(workerTargetFront1.isClosed, true, "worker in tab 1 should be closed");
=======
  const tab = await addTab(TAB1_URL);
  const target = await TargetFactory.forTab(tab);
  await target.attach();
  await listWorkers(target);

  // If a page still has pending network requests, it will not be moved into
  // the bfcache. Consequently, we cannot use waitForWorkerListChanged here,
  // because the worker is not guaranteed to have finished loading when it is
  // registered. Instead, we have to wait for the promise returned by
  // createWorker in the tab to be resolved.
  await createWorkerInTab(tab, WORKER1_URL);
  let { workers } = await listWorkers(target);
  let workerTargetFront1 = findWorker(workers, WORKER1_URL);
  await workerTargetFront1.attach();
  ok(workerTargetFront1.actorID, "front for worker in tab 1 has been attached");

  executeSoon(() => {
    BrowserTestUtils.loadURI(tab.linkedBrowser, TAB2_URL);
  });
  await waitForWorkerClose(workerTargetFront1);
  ok(!!workerTargetFront1.actorID, "front for worker in tab 1 has been closed");
>>>>>>> upstream-releases

<<<<<<< HEAD
    yield createWorkerInTab(tab, WORKER2_URL);
    ({ workers } = yield listWorkers(targetFront));
    const workerTargetFront2 = findWorker(workers, WORKER2_URL);
    yield workerTargetFront2.attach();
    is(workerTargetFront2.isClosed, false, "worker in tab 2 should not be closed");
||||||| merged common ancestors
    yield createWorkerInTab(tab, WORKER2_URL);
    ({ workers } = yield listWorkers(targetFront));
    const [, workerTargetFront2] = yield attachWorker(targetFront,
                                               findWorker(workers, WORKER2_URL));
    is(workerTargetFront2.isClosed, false, "worker in tab 2 should not be closed");
=======
  await createWorkerInTab(tab, WORKER2_URL);
  ({ workers } = await listWorkers(target));
  const workerTargetFront2 = findWorker(workers, WORKER2_URL);
  await workerTargetFront2.attach();
  ok(workerTargetFront2.actorID, "front for worker in tab 2 has been attached");
>>>>>>> upstream-releases

  executeSoon(() => {
    tab.linkedBrowser.goBack();
  });
  await waitForWorkerClose(workerTargetFront2);
  ok(!!workerTargetFront2.actorID, "front for worker in tab 2 has been closed");

<<<<<<< HEAD
    ({ workers } = yield listWorkers(targetFront));
    workerTargetFront1 = findWorker(workers, WORKER1_URL);
    yield workerTargetFront1.attach();
    is(workerTargetFront1.isClosed, false, "worker in tab 1 should not be closed");
||||||| merged common ancestors
    ({ workers } = yield listWorkers(targetFront));
    [, workerTargetFront1] = yield attachWorker(targetFront,
                                           findWorker(workers, WORKER1_URL));
    is(workerTargetFront1.isClosed, false, "worker in tab 1 should not be closed");
=======
  ({ workers } = await listWorkers(target));
  workerTargetFront1 = findWorker(workers, WORKER1_URL);
  await workerTargetFront1.attach();
  ok(workerTargetFront1.actorID, "front for worker in tab 1 has been attached");
>>>>>>> upstream-releases

<<<<<<< HEAD
    yield target.destroy();
    SpecialPowers.setIntPref(MAX_TOTAL_VIEWERS, oldMaxTotalViewers);
    finish();
  });
}
||||||| merged common ancestors
    yield close(client);
    SpecialPowers.setIntPref(MAX_TOTAL_VIEWERS, oldMaxTotalViewers);
    finish();
  });
}
=======
  await target.destroy();
  SpecialPowers.setIntPref(MAX_TOTAL_VIEWERS, oldMaxTotalViewers);
  finish();
});
>>>>>>> upstream-releases
