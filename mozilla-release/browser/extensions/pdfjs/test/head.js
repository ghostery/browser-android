<<<<<<< HEAD
async function waitForPdfJS(browser, url) {
  await SpecialPowers.pushPrefEnv({
    set: [
      ["pdfjs.eventBusDispatchToDOM", true],
    ],
  });
||||||| merged common ancestors
function waitForPdfJS(browser, url) {
=======
async function waitForPdfJS(browser, url) {
  await SpecialPowers.pushPrefEnv({
    set: [["pdfjs.eventBusDispatchToDOM", true]],
  });
>>>>>>> upstream-releases
  // Runs tests after all "load" event handlers have fired off
  return ContentTask.spawn(browser, url, async function(contentUrl) {
    await new Promise(resolve => {
      // NB: Add the listener to the global object so that we receive the
      // event fired from the new window.
<<<<<<< HEAD
      addEventListener("documentloaded", function listener() {
        removeEventListener("documentloaded", listener, false);
        resolve();
      }, false, true);
||||||| merged common ancestors
      addEventListener("documentload", function listener() {
        removeEventListener("documentload", listener, false);
        resolve();
      }, false, true);
=======
      addEventListener(
        "documentloaded",
        function listener() {
          removeEventListener("documentloaded", listener, false);
          resolve();
        },
        false,
        true
      );
>>>>>>> upstream-releases

      content.location = contentUrl;
    });
  });
}
