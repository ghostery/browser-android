<<<<<<< HEAD
/* eslint-env webextensions */
addEventListener("load", () => {
  browser.damp.startTest().then(results => {
    window.tpRecordTime(results.testResults.join(","), 0, results.testNames.join(","));
  });
});
||||||| merged common ancestors
=======
/* eslint-env webextensions */
addEventListener("load", () => {
  browser.damp.startTest().then(results => {
    window.tpRecordTime(
      results.testResults.join(","),
      0,
      results.testNames.join(",")
    );
  });
});
>>>>>>> upstream-releases
