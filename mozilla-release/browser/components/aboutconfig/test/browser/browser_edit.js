<<<<<<< HEAD
/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

ChromeUtils.import("resource://gre/modules/Preferences.jsm", this);

const PAGE_URL = "chrome://browser/content/aboutconfig/aboutconfig.html";

add_task(async function setup() {
  await SpecialPowers.pushPrefEnv({
    set: [
      ["test.aboutconfig.modify.boolean", true],
      ["test.aboutconfig.modify.number", 1337],
      ["test.aboutconfig.modify.string", "the answer to the life the universe and everything"],
    ],
  });

  registerCleanupFunction(() => {
    Services.prefs.clearUserPref("accessibility.typeaheadfind.autostart");
    Services.prefs.clearUserPref("accessibility.typeaheadfind.soundURL");
    Services.prefs.clearUserPref("accessibility.typeaheadfind.casesensitive");
  });
});

add_task(async function test_add_user_pref() {
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: PAGE_URL,
  }, async browser => {
    await ContentTask.spawn(browser, null, () => {
      Assert.ok(!Services.prefs.getChildList("").find(pref => pref == "testPref"));
      let search = content.document.getElementById("search");
      search.value = "testPref";
      search.focus();
    });

    for (let [buttonSelector, expectedValue] of [
      [".add-true", true],
      [".add-false", false],
      [".add-Number", 0],
      [".add-String", ""],
    ]) {
      EventUtils.sendKey("return");
      await ContentTask.spawn(browser, [buttonSelector, expectedValue],
                                      // eslint-disable-next-line no-shadow
                                      ([buttonSelector, expectedValue]) => {
        ChromeUtils.import("resource://gre/modules/Preferences.jsm");

        content.document.querySelector("#prefs button" + buttonSelector).click();
        Assert.ok(Services.prefs.getChildList("").find(pref => pref == "testPref"));
        Assert.ok(Preferences.get("testPref") === expectedValue);
        content.document.querySelector("#prefs button[data-l10n-id='about-config-pref-delete']").click();
        let search = content.document.getElementById("search");
        search.value = "testPref";
        search.focus();
      });
    }
  });
});

add_task(async function test_delete_user_pref() {
  Services.prefs.setBoolPref("userAddedPref", true);
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: PAGE_URL,
  }, async browser => {
    await ContentTask.spawn(browser, null, () => {
      let list = [...content.document.getElementById("prefs")
        .getElementsByTagName("tr")];
      function getRow(name) {
        return list.find(row => row.querySelector("td").textContent == name);
      }
      Assert.ok(getRow("userAddedPref"));
      getRow("userAddedPref").lastChild.lastChild.click();
      list = [...content.document.getElementById("prefs")
        .getElementsByTagName("tr")];
      Assert.ok(!getRow("userAddedPref"));
      Assert.ok(!Services.prefs.getChildList("").includes("userAddedPref"));

      // Search for nothing to test gPrefArray
      let search = content.document.getElementById("search");
      search.focus();
    });

    EventUtils.sendKey("return");
    await ContentTask.spawn(browser, null, () => {
      let list = [...content.document.getElementById("prefs")
        .getElementsByTagName("tr")];
      function getRow(name) {
        return list.find(row => row.querySelector("td").textContent == name);
      }
      Assert.ok(!getRow("userAddedPref"));
    });
  });
});

add_task(async function test_reset_user_pref() {
  await SpecialPowers.pushPrefEnv({"set": [["browser.autofocus", false]]});

  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: PAGE_URL,
  }, async browser => {
    await ContentTask.spawn(browser, null, () => {
      ChromeUtils.import("resource://gre/modules/Preferences.jsm");

      function getRow(name) {
        return list.find(row => row.querySelector("td").textContent == name);
      }
      function getValue(name) {
        return getRow(name).querySelector("td.cell-value").textContent;
      }
      let testPref = "browser.autofocus";
      // Click reset.
      let list = [...content.document.getElementById("prefs")
        .getElementsByTagName("tr")];
      let row = getRow(testPref);
      row.lastChild.lastChild.click();
      // Check new layout and reset.
      list = [...content.document.getElementById("prefs")
        .getElementsByTagName("tr")];
      Assert.ok(!row.classList.contains("has-user-value"));
      Assert.equal(row.lastChild.childNodes.length, 0);
      Assert.ok(!Services.prefs.prefHasUserValue(testPref));
      Assert.equal(getValue(testPref), "" + Preferences.get(testPref));

      // Search for nothing to test gPrefArray
      let search = content.document.getElementById("search");
      search.focus();
    });

    EventUtils.sendKey("return");
    await ContentTask.spawn(browser, null, () => {
      function getRow(name) {
        return list.find(row => row.querySelector("td").textContent == name);
      }
      function getValue(name) {
        return getRow(name).querySelector("td.cell-value").textContent;
      }
      let testPref = "browser.autofocus";
      // Check new layout and reset.
      let list = [...content.document.getElementById("prefs")
        .getElementsByTagName("tr")];
      let row = getRow(testPref);
      Assert.ok(!row.classList.contains("has-user-value"));
      Assert.equal(row.lastChild.childNodes.length, 0);
      Assert.equal(getValue(testPref), "" + Preferences.get(testPref));
    });
  });
});

add_task(async function test_modify() {
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: PAGE_URL,
  }, browser => {
    return ContentTask.spawn(browser, null, () => {
      ChromeUtils.import("resource://gre/modules/Preferences.jsm");

      function getRow(name, list) {
        return list.find(row => row.querySelector("td").textContent == name);
      }
      function getValue(name, list) {
        return getRow(name, list).querySelector("td.cell-value").textContent;
      }

      // Test toggle for boolean prefs.
      let list = [...content.document.getElementById("prefs")
        .getElementsByTagName("tr")];
      for (let nameOfBoolPref of [
        "test.aboutconfig.modify.boolean",
        "accessibility.typeaheadfind.autostart",
      ]) {
        let row = getRow(nameOfBoolPref, list);
        // Do this a two times to reset the pref.
        for (let i = 0; i < 2; i++) {
          row.querySelector("td.cell-edit").firstChild.click();
          // Check new layout and saving in backend.
          Assert.equal(getValue(nameOfBoolPref, list),
            "" + Preferences.get(nameOfBoolPref));
          let prefHasUserValue = Services.prefs.prefHasUserValue(nameOfBoolPref);
          Assert.equal(row.classList.contains("has-user-value"), prefHasUserValue);
          Assert.equal(row.lastChild.childNodes.length > 0, prefHasUserValue);
        }
      }

      // Test abort of edit by starting with string and continuing with editing Int pref.
      let row = getRow("test.aboutconfig.modify.string", list);
      row.querySelector("td.cell-edit").firstChild.click();
      row.querySelector("td.cell-value").firstChild.firstChild.value = "test";
      let intRow = getRow("test.aboutconfig.modify.number", list);
      intRow.querySelector("td.cell-edit").firstChild.click();
      Assert.equal(intRow.querySelector("td.cell-value").firstChild.firstChild.value,
        Preferences.get("test.aboutconfig.modify.number"));
      Assert.equal(getValue("test.aboutconfig.modify.string", list),
        "" + Preferences.get("test.aboutconfig.modify.string"));
      Assert.equal(row.querySelector("td.cell-value").textContent,
        Preferences.get("test.aboutconfig.modify.string"));

      // Test regex check for Int pref.
      intRow.querySelector("td.cell-value").firstChild.firstChild.value += "a";
      intRow.querySelector("td.cell-edit").firstChild.click();
      Assert.ok(!intRow.querySelector("td.cell-value").firstChild.firstChild.checkValidity());

      // Test correct saving and DOM-update.
      for (let prefName of [
        "test.aboutconfig.modify.string",
        "test.aboutconfig.modify.number",
        "accessibility.typeaheadfind.soundURL",
        "accessibility.typeaheadfind.casesensitive",
      ]) {
        row = getRow(prefName, list);
        // Activate edit and check displaying.
        row.querySelector("td.cell-edit").firstChild.click();
        Assert.equal(row.querySelector("td.cell-value").firstChild.firstChild.value,
          Preferences.get(prefName));
        row.querySelector("td.cell-value").firstChild.firstChild.value = "42";
        // Save and check saving.
        row.querySelector("td.cell-edit").firstChild.click();
        Assert.equal(getValue(prefName, list),
          "" + Preferences.get(prefName));
        let prefHasUserValue = Services.prefs.prefHasUserValue(prefName);
        Assert.equal(row.lastChild.childNodes.length > 0, prefHasUserValue);
        Assert.equal(row.classList.contains("has-user-value"), prefHasUserValue);
      }
    });
  });
});
||||||| merged common ancestors
=======
/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

/**
 * This is a temporary workaround to
 * be resolved in bug 1539000.
 */
ChromeUtils.import("resource://testing-common/PromiseTestUtils.jsm", this);
PromiseTestUtils.whitelistRejectionsGlobally(
  /Too many characters in placeable/
);

add_task(async function setup() {
  await SpecialPowers.pushPrefEnv({
    set: [
      ["test.aboutconfig.modify.boolean", true],
      ["test.aboutconfig.modify.number", 1337],
      [
        "test.aboutconfig.modify.string",
        "the answer to the life the universe and everything",
      ],
    ],
  });

  registerCleanupFunction(() => {
    Services.prefs.clearUserPref(PREF_BOOLEAN_DEFAULT_TRUE);
    Services.prefs.clearUserPref(PREF_NUMBER_DEFAULT_ZERO);
    Services.prefs.clearUserPref(PREF_STRING_DEFAULT_EMPTY);
  });
});

add_task(async function test_add_user_pref() {
  Assert.equal(
    Services.prefs.getPrefType(PREF_NEW),
    Ci.nsIPrefBranch.PREF_INVALID
  );

  await AboutConfigTest.withNewTab(async function() {
    // The row for a new preference appears when searching for its name.
    Assert.ok(!this.getRow(PREF_NEW));
    this.search(PREF_NEW);
    let row = this.getRow(PREF_NEW);
    Assert.ok(row.hasClass("deleted"));

    for (let [radioIndex, expectedValue, expectedEditingMode] of [
      [0, true, false],
      [1, 0, true],
      [2, "", true],
    ]) {
      // Adding the preference should set the default for the data type.
      row.element.querySelectorAll("input")[radioIndex].click();
      row.editColumnButton.click();
      Assert.ok(!row.hasClass("deleted"));
      Assert.ok(Preferences.get(PREF_NEW) === expectedValue);

      // Number and String preferences should be in edit mode.
      Assert.equal(!!row.valueInput, expectedEditingMode);

      // Repeat the search to verify that the preference remains.
      this.search(PREF_NEW);
      row = this.getRow(PREF_NEW);
      Assert.ok(!row.hasClass("deleted"));
      Assert.ok(!row.valueInput);

      // Reset the preference, then continue by adding a different type.
      row.resetColumnButton.click();
      Assert.equal(
        Services.prefs.getPrefType(PREF_NEW),
        Ci.nsIPrefBranch.PREF_INVALID
      );
    }
  });
});

add_task(async function test_delete_user_pref() {
  for (let [radioIndex, testValue] of [[0, false], [1, -1], [2, "value"]]) {
    Preferences.set(PREF_NEW, testValue);
    await AboutConfigTest.withNewTab(async function() {
      // Deleting the preference should keep the row.
      let row = this.getRow(PREF_NEW);
      row.resetColumnButton.click();
      Assert.ok(row.hasClass("deleted"));
      Assert.equal(
        Services.prefs.getPrefType(PREF_NEW),
        Ci.nsIPrefBranch.PREF_INVALID
      );

      // Re-adding the preference should keep the same value.
      Assert.ok(row.element.querySelectorAll("input")[radioIndex].checked);
      row.editColumnButton.click();
      Assert.ok(!row.hasClass("deleted"));
      Assert.ok(Preferences.get(PREF_NEW) === testValue);

      // Filtering again after deleting should remove the row.
      row.resetColumnButton.click();
      this.showAll();
      Assert.ok(!this.getRow(PREF_NEW));
    });
  }
});

add_task(async function test_reset_user_pref() {
  await SpecialPowers.pushPrefEnv({
    set: [
      [PREF_BOOLEAN_DEFAULT_TRUE, false],
      [PREF_STRING_LOCALIZED_MISSING, "user-value"],
    ],
  });

  await AboutConfigTest.withNewTab(async function() {
    // Click reset.
    let row = this.getRow(PREF_BOOLEAN_DEFAULT_TRUE);
    row.resetColumnButton.click();
    // Check new layout and reset.
    Assert.ok(!row.hasClass("has-user-value"));
    Assert.ok(!row.resetColumnButton);
    Assert.ok(!Services.prefs.prefHasUserValue(PREF_BOOLEAN_DEFAULT_TRUE));
    Assert.equal(this.getRow(PREF_BOOLEAN_DEFAULT_TRUE).value, "true");

    // Filter again to test the preference cache.
    this.showAll();
    row = this.getRow(PREF_BOOLEAN_DEFAULT_TRUE);
    Assert.ok(!row.hasClass("has-user-value"));
    Assert.ok(!row.resetColumnButton);
    Assert.equal(this.getRow(PREF_BOOLEAN_DEFAULT_TRUE).value, "true");

    // Clicking reset on a localized preference without a corresponding value.
    row = this.getRow(PREF_STRING_LOCALIZED_MISSING);
    Assert.equal(row.value, "user-value");
    row.resetColumnButton.click();
    // Check new layout and reset.
    Assert.ok(!row.hasClass("has-user-value"));
    Assert.ok(!row.resetColumnButton);
    Assert.ok(!Services.prefs.prefHasUserValue(PREF_STRING_LOCALIZED_MISSING));
    Assert.equal(this.getRow(PREF_STRING_LOCALIZED_MISSING).value, "");
  });
});

add_task(async function test_modify() {
  await AboutConfigTest.withNewTab(async function() {
    // Test toggle for boolean prefs.
    for (let nameOfBoolPref of [
      "test.aboutconfig.modify.boolean",
      PREF_BOOLEAN_DEFAULT_TRUE,
    ]) {
      let row = this.getRow(nameOfBoolPref);
      // Do this a two times to reset the pref.
      for (let i = 0; i < 2; i++) {
        row.editColumnButton.click();
        // Check new layout and saving in backend.
        Assert.equal(
          this.getRow(nameOfBoolPref).value,
          "" + Preferences.get(nameOfBoolPref)
        );
        let prefHasUserValue = Services.prefs.prefHasUserValue(nameOfBoolPref);
        Assert.equal(row.hasClass("has-user-value"), prefHasUserValue);
        Assert.equal(!!row.resetColumnButton, prefHasUserValue);
      }
    }

    // Test abort of edit by starting with string and continuing with editing Int pref.
    let row = this.getRow("test.aboutconfig.modify.string");
    row.editColumnButton.click();
    row.valueInput.value = "test";
    let intRow = this.getRow("test.aboutconfig.modify.number");
    intRow.editColumnButton.click();
    Assert.equal(
      intRow.valueInput.value,
      Preferences.get("test.aboutconfig.modify.number")
    );
    Assert.ok(!row.valueInput);
    Assert.equal(row.value, Preferences.get("test.aboutconfig.modify.string"));

    // Test validation of integer values.
    for (let invalidValue of [
      "",
      " ",
      "a",
      "1.5",
      "-2147483649",
      "2147483648",
    ]) {
      intRow.valueInput.value = invalidValue;
      intRow.editColumnButton.click();
      // We should still be in edit mode.
      Assert.ok(intRow.valueInput);
    }

    // Test correct saving and DOM-update.
    for (let [prefName, willDelete] of [
      ["test.aboutconfig.modify.string", true],
      ["test.aboutconfig.modify.number", true],
      [PREF_NUMBER_DEFAULT_ZERO, false],
      [PREF_STRING_DEFAULT_EMPTY, false],
    ]) {
      row = this.getRow(prefName);
      // Activate edit and check displaying.
      row.editColumnButton.click();
      Assert.equal(row.valueInput.value, Preferences.get(prefName));
      row.valueInput.value = "42";
      // Save and check saving.
      row.editColumnButton.click();
      Assert.equal(Preferences.get(prefName), "42");
      Assert.equal(row.value, "42");
      Assert.ok(row.hasClass("has-user-value"));
      // Reset or delete the preference while editing.
      row.editColumnButton.click();
      Assert.equal(row.valueInput.value, Preferences.get(prefName));
      row.resetColumnButton.click();
      Assert.ok(!row.hasClass("has-user-value"));
      Assert.equal(row.hasClass("deleted"), willDelete);
    }
  });
});
>>>>>>> upstream-releases
