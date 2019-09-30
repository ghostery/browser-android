/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

ignoreAllUncaughtExceptions();

add_task(async function() {
  info(
    "Check that performing a search fires a search event and records to Telemetry."
  );

<<<<<<< HEAD
  await BrowserTestUtils.withNewTab({ gBrowser, url: "about:home" }, async function(browser) {
    let currEngine = Services.search.defaultEngine;
    let engine = await promiseNewEngine("searchSuggestionEngine.xml");
    // Make this actually work in healthreport by giving it an ID:
    Object.defineProperty(engine.wrappedJSObject, "identifier",
                          { value: "org.mozilla.testsearchsuggestions" });
||||||| merged common ancestors
  await BrowserTestUtils.withNewTab({ gBrowser, url: "about:home" }, async function(browser) {
    let currEngine = Services.search.currentEngine;
    let engine = await promiseNewEngine("searchSuggestionEngine.xml");
    // Make this actually work in healthreport by giving it an ID:
    Object.defineProperty(engine.wrappedJSObject, "identifier",
                          { value: "org.mozilla.testsearchsuggestions" });
=======
  await BrowserTestUtils.withNewTab(
    { gBrowser, url: "about:home" },
    async function(browser) {
      let currEngine = await Services.search.getDefault();
      let engine = await promiseNewEngine("searchSuggestionEngine.xml");
      // Make this actually work in healthreport by giving it an ID:
      Object.defineProperty(engine.wrappedJSObject, "identifier", {
        value: "org.mozilla.testsearchsuggestions",
      });
>>>>>>> upstream-releases

<<<<<<< HEAD
    let p = promiseContentSearchChange(browser, engine.name);
    Services.search.defaultEngine = engine;
    await p;
||||||| merged common ancestors
    let p = promiseContentSearchChange(browser, engine.name);
    Services.search.currentEngine = engine;
    await p;
=======
      await Promise.all([
        promiseContentSearchChange(browser, engine.name),
        Services.search.setDefault(engine),
      ]);
>>>>>>> upstream-releases

      await ContentTask.spawn(
        browser,
        { expectedName: engine.name },
        async function(args) {
          let engineName =
            content.wrappedJSObject.gContentSearchController.defaultEngine.name;
          is(
            engineName,
            args.expectedName,
            "Engine name in DOM should match engine we just added"
          );
        }
      );

      let numSearchesBefore = 0;
      // Get the current number of recorded searches.
      let histogramKey = engine.identifier + ".abouthome";
      try {
        let hs = Services.telemetry
          .getKeyedHistogramById("SEARCH_COUNTS")
          .snapshot();
        if (histogramKey in hs) {
          numSearchesBefore = hs[histogramKey].sum;
        }
      } catch (ex) {
        // No searches performed yet, not a problem, |numSearchesBefore| is 0.
      }

      let searchStr = "a search";

<<<<<<< HEAD
    let expectedURL = Services.search.defaultEngine
      .getSubmission(searchStr, null, "homepage").uri.spec;
    let promise = waitForDocLoadAndStopIt(expectedURL, browser);
||||||| merged common ancestors
    let expectedURL = Services.search.currentEngine
      .getSubmission(searchStr, null, "homepage").uri.spec;
    let promise = waitForDocLoadAndStopIt(expectedURL, browser);
=======
      let expectedURL = (await Services.search.getDefault()).getSubmission(
        searchStr,
        null,
        "homepage"
      ).uri.spec;
      let promise = BrowserTestUtils.waitForDocLoadAndStopIt(
        expectedURL,
        browser
      );
>>>>>>> upstream-releases

      // Perform a search to increase the SEARCH_COUNT histogram.
      await ContentTask.spawn(browser, { searchStr }, async function(args) {
        let doc = content.document;
        info("Perform a search.");
        let el = doc.querySelector(["#searchText", "#newtab-search-text"]);
        el.value = args.searchStr;
        doc.getElementById("searchSubmit").click();
      });

      await promise;

      // Make sure the SEARCH_COUNTS histogram has the right key and count.
      let hs = Services.telemetry
        .getKeyedHistogramById("SEARCH_COUNTS")
        .snapshot();
      Assert.ok(histogramKey in hs, "histogram with key should be recorded");
      Assert.equal(
        hs[histogramKey].sum,
        numSearchesBefore + 1,
        "histogram sum should be incremented"
      );

<<<<<<< HEAD
    Services.search.defaultEngine = currEngine;
    try {
      Services.search.removeEngine(engine);
    } catch (ex) {}
  });
||||||| merged common ancestors
    Services.search.currentEngine = currEngine;
    try {
      Services.search.removeEngine(engine);
    } catch (ex) {}
  });
=======
      await Services.search.setDefault(currEngine);
      try {
        await Services.search.removeEngine(engine);
      } catch (ex) {}
    }
  );
>>>>>>> upstream-releases
});
