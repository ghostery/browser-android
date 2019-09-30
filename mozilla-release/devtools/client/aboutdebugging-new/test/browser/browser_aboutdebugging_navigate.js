/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

/* import-globals-from helper-collapsibilities.js */
Services.scriptloader.loadSubScript(
  CHROME_URL_ROOT + "helper-collapsibilities.js",
  this
);

/**
 * Check that navigating from This Firefox to Connect and back to This Firefox works and
 * does not leak.
 */

const TAB_URL_1 = "data:text/html,<title>TAB1</title>";
const TAB_URL_2 = "data:text/html,<title>TAB2</title>";

add_task(async function() {
  info("Force all debug target panes to be expanded");
  prepareCollapsibilitiesTest();

<<<<<<< HEAD
  const { document, tab, window } = await openAboutDebugging();
  const AboutDebugging = window.AboutDebugging;
||||||| merged common ancestors
  const { document, tab } = await openAboutDebugging();
=======
  const { document, tab, window } = await openAboutDebugging();
  const AboutDebugging = window.AboutDebugging;
  await selectThisFirefoxPage(document, AboutDebugging.store);
>>>>>>> upstream-releases

<<<<<<< HEAD
  const connectSidebarItem = findSidebarItemByText("Connect", document);
  const connectLink = connectSidebarItem.querySelector(".js-sidebar-link");
||||||| merged common ancestors
  const connectSidebarItem = findSidebarItemByText("Connect", document);
=======
  const connectSidebarItem = findSidebarItemByText("Setup", document);
  const connectLink = connectSidebarItem.querySelector(".qa-sidebar-link");
>>>>>>> upstream-releases
  ok(connectSidebarItem, "Found the Connect sidebar item");

<<<<<<< HEAD
  const thisFirefoxSidebarItem = findSidebarItemByText("This Firefox", document);
  const thisFirefoxLink = thisFirefoxSidebarItem.querySelector(".js-sidebar-link");
||||||| merged common ancestors
  const thisFirefoxSidebarItem = findSidebarItemByText("This Firefox", document);
=======
  const thisFirefoxString = getThisFirefoxString(window);
  const thisFirefoxSidebarItem = findSidebarItemByText(
    thisFirefoxString,
    document
  );
  const thisFirefoxLink = thisFirefoxSidebarItem.querySelector(
    ".qa-sidebar-link"
  );
>>>>>>> upstream-releases
  ok(thisFirefoxSidebarItem, "Found the ThisFirefox sidebar item");
<<<<<<< HEAD
  ok(isSidebarItemSelected(thisFirefoxSidebarItem),
    "ThisFirefox sidebar item is selected by default");

||||||| merged common ancestors
  ok(isSidebarItemSelected(thisFirefoxSidebarItem),
    "ThisFirefox sidebar item is selected by default");

  // Wait until the about:debugging target is visible in the tab list
  // Otherwise, we might have a race condition where TAB1 is discovered by the initial
  // listTabs from the watchRuntime action, instead of being discovered after the
  // TAB_UPDATED event. See analysis in Bug 1493968.
  await waitUntil(() => findDebugTargetByText("about:debugging", document));

=======
  ok(
    isSidebarItemSelected(thisFirefoxSidebarItem),
    "ThisFirefox sidebar item is selected by default"
  );

>>>>>>> upstream-releases
  info("Open a new background tab TAB1");
  const backgroundTab1 = await addTab(TAB_URL_1, { background: true });

  info("Wait for the tab to appear in the debug targets with the correct name");
  await waitUntil(() => findDebugTargetByText("TAB1", document));

  await waitForRequestsToSettle(AboutDebugging.store);
  info("Click on the Connect item in the sidebar");
  connectLink.click();

  info("Wait until Connect page is displayed");
<<<<<<< HEAD
  await waitUntil(() => document.querySelector(".js-connect-page"));
  // we need to wait here because the sidebar isn't updated after mounting the page
  info("Wait until Connect sidebar item is selected");
  await waitUntil(() => isSidebarItemSelected(connectSidebarItem));
  ok(!document.querySelector(".js-runtime-page"), "Runtime page no longer rendered");
||||||| merged common ancestors
  await waitUntil(() => document.querySelector(".js-connect-page"));
  ok(isSidebarItemSelected(connectSidebarItem), "Connect sidebar item is selected");
  ok(!document.querySelector(".js-runtime-page"), "Runtime page no longer rendered");
=======
  await waitUntil(() => document.querySelector(".qa-connect-page"));
  // we need to wait here because the sidebar isn't updated after mounting the page
  info("Wait until Connect sidebar item is selected");
  await waitUntil(() => isSidebarItemSelected(connectSidebarItem));
  ok(
    !document.querySelector(".qa-runtime-page"),
    "Runtime page no longer rendered"
  );
>>>>>>> upstream-releases

  info("Open a new tab which should be listed when we go back to This Firefox");
  const backgroundTab2 = await addTab(TAB_URL_2, { background: true });

  info("Click on the ThisFirefox item in the sidebar");
<<<<<<< HEAD
  const requestsSuccess = waitForRequestsSuccess(window);
  thisFirefoxLink.click();

  info("Wait for all target requests to complete");
  await requestsSuccess;
||||||| merged common ancestors
  thisFirefoxSidebarItem.click();
=======
  const requestsSuccess = waitForRequestsSuccess(AboutDebugging.store);
  thisFirefoxLink.click();

  info("Wait for all target requests to complete");
  await requestsSuccess;
>>>>>>> upstream-releases

  info("Wait until ThisFirefox page is displayed");
  await waitUntil(() => document.querySelector(".qa-runtime-page"));
  ok(
    isSidebarItemSelected(thisFirefoxSidebarItem),
    "ThisFirefox sidebar item is selected again"
  );
  ok(
    !document.querySelector(".qa-connect-page"),
    "Connect page no longer rendered"
  );

  info("TAB2 should already be displayed in the debug targets");
  await waitUntil(() => findDebugTargetByText("TAB2", document));

  info("Remove first background tab");
  await removeTab(backgroundTab1);

  info(
    "Check TAB1 disappears, meaning ThisFirefox client is correctly connected"
  );
  await waitUntil(() => !findDebugTargetByText("TAB1", document));

  info("Remove second background tab");
  await removeTab(backgroundTab2);

  info(
    "Check TAB2 disappears, meaning ThisFirefox client is correctly connected"
  );
  await waitUntil(() => !findDebugTargetByText("TAB2", document));

  await waitForRequestsToSettle(AboutDebugging.store);

  await removeTab(tab);
});

function isSidebarItemSelected(item) {
<<<<<<< HEAD
  return item.classList.contains("js-sidebar-item-selected");
}
||||||| merged common ancestors
  return item.classList.contains("js-sidebar-item-selected");
}

function findDebugTargetByText(text, document) {
  const targets = [...document.querySelectorAll(".js-debug-target-item")];
  return targets.find(target => target.textContent.includes(text));
}
=======
  return item.classList.contains("qa-sidebar-item-selected");
}
>>>>>>> upstream-releases
