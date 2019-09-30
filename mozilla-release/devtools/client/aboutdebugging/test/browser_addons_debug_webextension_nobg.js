/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
"use strict";

// There are shutdown issues for which multiple rejections are left uncaught.
// See bug 1018184 for resolving these issues.
const { PromiseTestUtils } = ChromeUtils.import(
  "resource://testing-common/PromiseTestUtils.jsm"
);
PromiseTestUtils.whitelistRejectionsGlobally(/File closed/);

// Avoid test timeouts that can occur while waiting for the "addon-console-works" message.
requestLongerTimeout(2);

const ADDON_NOBG_ID = "test-devtools-webextension-nobg@mozilla.org";
const ADDON_NOBG_NAME = "test-devtools-webextension-nobg";

/**
 * This test file ensures that the webextension addon developer toolbox:
 * - the webextension developer toolbox is connected to a fallback page when the
 *   background page is not available (and in the fallback page document body contains
 *   the expected message, which warns the user that the current page is not a real
 *   webextension context);
 */
add_task(async function testWebExtensionsToolboxNoBackgroundPage() {
  const addonFile = ExtensionTestCommon.generateXPI({
    manifest: {
      name: ADDON_NOBG_NAME,
      applications: {
        gecko: { id: ADDON_NOBG_ID },
      },
    },
  });
  registerCleanupFunction(() => addonFile.remove(false));

  const { tab, document, debugBtn } = await setupTestAboutDebuggingWebExtension(
    ADDON_NOBG_NAME,
    addonFile
  );

<<<<<<< HEAD
  // Be careful, this JS function is going to be executed in the addon toolbox,
  // which lives in another process. So do not try to use any scope variable!
  const env = Cc["@mozilla.org/process/environment;1"]
        .getService(Ci.nsIEnvironment);
  const testScript = function() {
    /* eslint-disable no-undef */
    // This is webextension toolbox process. So we can't access mochitest framework.
    const waitUntil = async function(predicate, interval = 10) {
      if (await predicate()) {
        return true;
      }
      return new Promise(resolve => {
        toolbox.win.setTimeout(function() {
          waitUntil(predicate, interval).then(() => resolve(true));
        }, interval);
      });
    };
||||||| merged common ancestors
  // Be careful, this JS function is going to be executed in the addon toolbox,
  // which lives in another process. So do not try to use any scope variable!
  const env = Cc["@mozilla.org/process/environment;1"]
        .getService(Ci.nsIEnvironment);
  const testScript = function() {
    /* eslint-disable no-undef */
    toolbox.selectTool("inspector").then(async inspector => {
      const nodeActor = await inspector.walker.querySelector(
        inspector.walker.rootNode, "body");

      if (!nodeActor) {
        throw new Error("nodeActor not found");
      }
=======
  const onToolboxReady = gDevTools.once("toolbox-ready");
  const onToolboxClose = gDevTools.once("toolbox-destroyed");
  debugBtn.click();
  const toolbox = await onToolboxReady;
  testScript(toolbox);

  await onToolboxClose;
  ok(true, "Addon toolbox closed");
>>>>>>> upstream-releases

<<<<<<< HEAD
    toolbox.selectTool("inspector").then(async inspector => {
      let nodeActor;
||||||| merged common ancestors
      if (!(nodeActor.inlineTextChild)) {
        throw new Error("inlineTextChild not found");
      }
=======
  await uninstallAddon({ document, id: ADDON_NOBG_ID, name: ADDON_NOBG_NAME });
  await closeAboutDebugging(tab);
});
>>>>>>> upstream-releases

<<<<<<< HEAD
      dump(`Wait the fallback window to be fully loaded\n`);
      await waitUntil(async () => {
        nodeActor = await inspector.walker.querySelector(inspector.walker.rootNode, "h1");
        return nodeActor && nodeActor.inlineTextChild;
      });
||||||| merged common ancestors
      dump("Got a nodeActor with an inline text child\n");
=======
const testScript = function(toolbox) {
  toolbox
    .selectTool("inspector")
    .then(async inspector => {
      let nodeActor;

      dump(`Wait the fallback window to be fully loaded\n`);
      await asyncWaitUntil(async () => {
        nodeActor = await inspector.walker.querySelector(
          inspector.walker.rootNode,
          "h1"
        );
        return nodeActor && nodeActor.inlineTextChild;
      });
>>>>>>> upstream-releases

      dump("Got a nodeActor with an inline text child\n");
      const expectedValue = "Your addon does not have any document opened yet.";
      const actualValue = nodeActor.inlineTextChild._form.nodeValue;

      if (actualValue !== expectedValue) {
        throw new Error(
          `mismatched inlineTextchild value: "${actualValue}" !== "${expectedValue}"`
        );
      }

      dump("Got the expected inline text content in the selected node\n");

      await toolbox.destroy();
    })
    .catch(error => {
      dump("Error while running code in the browser toolbox process:\n");
      dump(error + "\n");
      dump("stack:\n" + error.stack + "\n");
    });
};
