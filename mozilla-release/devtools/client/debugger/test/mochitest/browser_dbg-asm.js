add_task(async function() {
  const dbg = await initDebugger("doc-asm.html");
  await reload(dbg);

  // After reload() we are getting getSources notifiction for old sources,
  // using the debugger statement to really stop are reloaded page.
  await waitForPaused(dbg);
  await resume(dbg);

  await waitForSources(dbg, "doc-asm.html", "asm.js");

  // Make sure sources appear.
<<<<<<< HEAD:mozilla-release/devtools/client/debugger/new/test/mochitest/browser_dbg-asm.js
  is(findAllElements(dbg, "sourceNodes").length, 2);
||||||| merged common ancestors
  is(findAllElements(dbg, "sourceNodes").length, 4);
=======
  is(findAllElements(dbg, "sourceNodes").length, 3);
>>>>>>> upstream-releases:mozilla-release/devtools/client/debugger/test/mochitest/browser_dbg-asm.js

  await selectSource(dbg, "asm.js");

  await addBreakpoint(dbg, "asm.js", 7);
  invokeInTab("runAsm");

  await waitForPaused(dbg);
  assertPausedLocation(dbg, "asm.js", 7);
});
