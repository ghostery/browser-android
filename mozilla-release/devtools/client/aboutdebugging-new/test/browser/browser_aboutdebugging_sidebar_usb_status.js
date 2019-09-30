/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

const { adbAddon } = require("devtools/shared/adb/adb-addon");
<<<<<<< HEAD
const { adbProcess } = require("devtools/shared/adb/adb-process");
||||||| merged common ancestors
const { ADB } = require("devtools/shared/adb/adb");
=======
>>>>>>> upstream-releases

/**
 * This test asserts that the sidebar shows a message describing the status of the USB
 * devices scanning.
 */
add_task(async function() {
  const mocks = new Mocks();

  await pushPref(
    "devtools.remote.adb.extensionURL",
    CHROME_URL_ROOT + "resources/test-adb-extension/adb-extension-#OS#.xpi"
  );
  const { document, tab } = await openAboutDebugging();

  const usbStatusElement = document.querySelector(".qa-sidebar-usb-status");
  ok(usbStatusElement, "Sidebar shows the USB status element");
  ok(
    usbStatusElement.textContent.includes("USB disabled"),
    "USB status element has 'disabled' content"
  );

  info("Install the adb extension and wait for the message to udpate");
<<<<<<< HEAD
  // Use "internal" as the install source to avoid triggering telemetry.
  adbAddon.install("internal");
  await waitUntil(() => usbStatusElement.textContent.includes("USB devices enabled"));

  // Right now we are resuming as soon as "USB devices enabled" is displayed, but ADB
  // might still be starting up. If we move to uninstall directly, the ADB startup will
  // fail and we will have an unhandled promise rejection.
  // See Bug 1498469.
  info("Wait until ADB has started.");
  await waitUntil(() => adbProcess.ready);

  info("Uninstall the adb extension and wait for the message to udpate");
||||||| merged common ancestors
  adbAddon.install();
  await waitUntil(() => usbStatusElement.textContent.includes("USB devices enabled"));

  // Right now we are resuming as soon as "USB devices enabled" is displayed, but ADB
  // might still be starting up. If we move to uninstall directly, the ADB startup will
  // fail and we will have an unhandled promise rejection.
  // See Bug 1498469.
  info("Wait until ADB has started.");
  await waitUntil(() => ADB.ready);

  info("Uninstall the adb extension and wait for the message to udpate");
=======
  // Use "internal" as the install source to avoid triggering telemetry.
  adbAddon.install("internal");
  // When using mocks, we manually control the .start() call
  await mocks.adbProcessMock.adbProcess.start();

  info("Wait till the USB status element has 'enabled' content");
  await waitUntil(() => {
    const el = document.querySelector(".qa-sidebar-usb-status");
    return el.textContent.includes("USB enabled");
  });

  info("Uninstall the adb extension and wait for USB status element to update");
>>>>>>> upstream-releases
  adbAddon.uninstall();
  await waitUntil(() => {
    const el = document.querySelector(".qa-sidebar-usb-status");
    return el.textContent.includes("USB disabled");
  });

  await removeTab(tab);
});
