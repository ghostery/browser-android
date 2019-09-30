<<<<<<< HEAD
const URL = "http://mochi.test:8888/browser/browser/base/content/test/trackingUI/file_trackingUI_fetch.html";

||||||| merged common ancestors
const URL = "http://mochi.test:8888/browser/browser/base/content/test/trackingUI/file_trackingUI_fetch.html";

function waitForSecurityChange(numChanges = 1) {
  return new Promise(resolve => {
    let n = 0;
    let listener = {
      onSecurityChange() {
        n = n + 1;
        info("Received onSecurityChange event " + n + " of " + numChanges);
        if (n >= numChanges) {
          gBrowser.removeProgressListener(listener);
          resolve();
        }
      },
    };
    gBrowser.addProgressListener(listener);
  });
}

=======
const URL =
  "http://mochi.test:8888/browser/browser/base/content/test/trackingUI/file_trackingUI_fetch.html";

>>>>>>> upstream-releases
add_task(async function test_fetch() {
<<<<<<< HEAD
  await SpecialPowers.pushPrefEnv({ set: [
    ["privacy.trackingprotection.enabled", true],
  ]});
||||||| merged common ancestors
  await SpecialPowers.pushPrefEnv({ set: [
    ["privacy.trackingprotection.enabled", true],
    ["browser.contentblocking.enabled", true],
  ]});
=======
  await SpecialPowers.pushPrefEnv({
    set: [["privacy.trackingprotection.enabled", true]],
  });
>>>>>>> upstream-releases

  await BrowserTestUtils.withNewTab({ gBrowser, url: URL }, async function(
    newTabBrowser
  ) {
    let contentBlockingEvent = waitForContentBlockingEvent();
    await ContentTask.spawn(newTabBrowser, null, async function() {
      await content.wrappedJSObject
        .test_fetch()
        .then(response => Assert.ok(false, "should have denied the request"))
        .catch(e => Assert.ok(true, `Caught exception: ${e}`));
    });
    await contentBlockingEvent;

    let ContentBlocking = newTabBrowser.ownerGlobal.ContentBlocking;
    ok(ContentBlocking, "got CB object");

    ok(
      ContentBlocking.content.hasAttribute("detected"),
      "has detected content blocking"
    );
    ok(ContentBlocking.iconBox.hasAttribute("active"), "icon box is active");
    is(
      ContentBlocking.iconBox.getAttribute("tooltiptext"),
      gNavigatorBundle.getString("trackingProtection.icon.activeTooltip"),
      "correct tooltip"
    );
  });
});
