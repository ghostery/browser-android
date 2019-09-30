/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

/* Check default search engine is picked from list.json searchDefault in distros.
   This has to be a separate test because we need to set the default distribution ID
   which is cached. */

"use strict";

add_task(async function setup() {
  Services.prefs
    .getDefaultBranch(null)
    .setCharPref("distribution.id", "partner-test");
  await AddonTestUtils.promiseStartupManager();
});

// Giving defaultenginename prefs a user value for partner build
// shouldn't change the default engine assigned from list.json
add_task(async function test_defaultEngineNamePref() {
  let defaultEngineName = getDefaultEngineName();

  // Set the browser.search.defaultenginename pref.
  Services.prefs.setCharPref(kDefaultenginenamePref, "Bing");

<<<<<<< HEAD
  await asyncInit();
  Assert.equal(Services.search.defaultEngine.name,
               defaultEngineName, "expected default search engine after pref set");
||||||| merged common ancestors
  await asyncInit();
  Assert.equal(Services.search.currentEngine.name,
               defaultEngineName, "expected default search engine after pref set");
=======
  Assert.equal(
    (await Services.search.getDefault()).name,
    defaultEngineName,
    "expected default search engine after pref set"
  );
>>>>>>> upstream-releases

  Services.prefs.clearUserPref(kDefaultenginenamePref);
});

// Giving defaultenginename prefs a user value with region US
// shouldn't change the default engine assigned from list.json
add_task(async function test_defaultEngineNameUserPrefUS() {
  let defaultEngineName = getDefaultEngineName(true);

  Services.prefs.setCharPref("browser.search.region", "US");

  // Set the browser.search.defaultenginename pref.
  Services.prefs.setCharPref(kDefaultenginenamePref, "Bing");

<<<<<<< HEAD
  await asyncReInit();
  Assert.equal(Services.search.defaultEngine.name,
               defaultEngineName, "expected US default search engine after pref set");
||||||| merged common ancestors
  await asyncReInit();
  Assert.equal(Services.search.currentEngine.name,
               defaultEngineName, "expected US default search engine after pref set");
=======
  await asyncReInit({ skipReset: true });
  Assert.equal(
    (await Services.search.getDefault()).name,
    defaultEngineName,
    "expected US default search engine after pref set"
  );
>>>>>>> upstream-releases

  Services.prefs.clearUserPref(kDefaultenginenamePref);
  Services.prefs.clearUserPref("browser.search.region");
});

// Giving defaultenginename prefs a default value with region US
// should change the default engine assigned from list.json
// This needs to be the last test involving the defaultenengename prefs
// because it changes a default value that can't be unset
add_task(async function test_defaultEngineNameDefaultPrefUS() {
  Services.prefs.setCharPref("browser.search.region", "US");

  // Set the browser.search.defaultenginename pref.
  let defaultBranch = Services.prefs.getDefaultBranch(null);
<<<<<<< HEAD
  defaultBranch.setCharPref(kDefaultenginenamePref,
                            "data:text/plain,browser.search.defaultenginename=Bing");

  await asyncReInit();
  Assert.equal(Services.search.defaultEngine.name,
               "Bing", "expected new default search engine after pref set");
||||||| merged common ancestors
  defaultBranch.setCharPref(kDefaultenginenamePref,
                            "data:text/plain,browser.search.defaultenginename=Bing");

  await asyncReInit();
  Assert.equal(Services.search.currentEngine.name,
               "Bing", "expected new default search engine after pref set");
=======
  defaultBranch.setCharPref(
    kDefaultenginenamePref,
    "data:text/plain,browser.search.defaultenginename=Bing"
  );

  await asyncReInit({ skipReset: true });
  Assert.equal(
    (await Services.search.getDefault()).name,
    "Bing",
    "expected new default search engine after pref set"
  );
>>>>>>> upstream-releases

  Services.prefs.clearUserPref("browser.search.region");
});
