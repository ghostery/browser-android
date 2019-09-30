/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

<<<<<<< HEAD
const PAGE_URL = "chrome://browser/content/aboutconfig/aboutconfig.html";

add_task(async function setup() {
  await SpecialPowers.pushPrefEnv({
    set: [
      ["random.user.pref", "chrome://test/locale/testing.properties"],
    ],
  });
});

||||||| merged common ancestors
=======
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
      [
        "test.aboutconfig.userValueLikeLocalized",
        "chrome://test/locale/testing.properties",
      ],
    ],
  });
});

>>>>>>> upstream-releases
add_task(async function test_load_title() {
<<<<<<< HEAD
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: PAGE_URL,
  }, browser => {
    info("about:config loaded");
    return ContentTask.spawn(browser, null, async () => {
      await content.document.l10n.ready;
      Assert.equal(content.document.title, "about:config");
    });
||||||| merged common ancestors
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: "chrome://browser/content/aboutconfig/aboutconfig.html",
  }, browser => {
    info("about:config loaded");
    return ContentTask.spawn(browser, null, async () => {
      await content.document.l10n.ready;
      Assert.equal(content.document.title, "about:config");
    });
=======
  await AboutConfigTest.withNewTab(async function() {
    Assert.equal(this.document.title, "Advanced Configurations");
  });
});

add_task(async function test_load_settings() {
  await AboutConfigTest.withNewTab(async function() {
    // Test if page contains elements.
    Assert.equal(this.getRow(PREF_NUMBER_DEFAULT_ZERO).value, 0);
    Assert.equal(this.getRow(PREF_STRING_DEFAULT_EMPTY).value, "");

    // Test if the modified state is displayed for the right prefs.
    Assert.ok(
      !this.getRow(PREF_BOOLEAN_DEFAULT_TRUE).hasClass("has-user-value")
    );
    Assert.ok(
      this.getRow(PREF_BOOLEAN_USERVALUE_TRUE).hasClass("has-user-value")
    );

    // Test to see if values are localized, sampling from different files. If
    // any of these are removed or their value changes, just update the value
    // here or point to a different preference in the same file.
    Assert.equal(this.getRow("font.language.group").value, "x-western");
    Assert.equal(this.getRow("intl.ellipsis").value, "\u2026");
    Assert.equal(
      this.getRow("gecko.handlerService.schemes.mailto.1.uriTemplate").value,
      "https://mail.google.com/mail/?extsrc=mailto&url=%s"
    );

    // Test to see if user created value is not empty string when it matches
    // /^chrome:\/\/.+\/locale\/.+\.properties/.
    Assert.equal(
      this.getRow("test.aboutconfig.userValueLikeLocalized").value,
      "chrome://test/locale/testing.properties"
    );

    // Test to see if empty string when value matches
    // /^chrome:\/\/.+\/locale\/.+\.properties/ and an exception is thrown.
    Assert.equal(this.getRow(PREF_STRING_LOCALIZED_MISSING).value, "");
>>>>>>> upstream-releases
  });
});

add_task(async function test_load_settings() {
  await BrowserTestUtils.withNewTab({
    gBrowser,
    url: PAGE_URL,
  }, browser => {
    return ContentTask.spawn(browser, null, () => {
      ChromeUtils.import("resource://gre/modules/Services.jsm");

      let list = [...content.document.getElementById("prefs")
        .getElementsByTagName("tr")];
      function getRow(name) {
        return list.find(row => row.querySelector("td").textContent == name);
      }
      function getValue(name) {
        return getRow(name).querySelector("td.cell-value").textContent;
      }

      // Test if page contains elements.
      Assert.ok(getRow("plugins.testmode"));
      Assert.ok(getRow("dom.vr.enabled"));
      Assert.ok(getRow("accessibility.AOM.enabled"));

      // Test if the modified state is displayed for the right prefs.
      let prefArray = Services.prefs.getChildList("");
      let nameOfEdited = prefArray.find(
        name => Services.prefs.prefHasUserValue(name));
      let nameOfDefault = prefArray.find(
        name => !Services.prefs.prefHasUserValue(name));
      Assert.ok(!getRow(nameOfDefault).classList.contains("has-user-value"));
      Assert.ok(getRow(nameOfEdited).classList.contains("has-user-value"));

      // Test to see if values are localized.
      Assert.equal(getValue("font.language.group"), "x-western");
      Assert.equal(getValue("intl.ellipsis"), "\u2026");
      Assert.equal(
        getValue("gecko.handlerService.schemes.mailto.1.uriTemplate"),
        "https://mail.google.com/mail/?extsrc=mailto&url=%s");

      // Test to see if user created value is not empty string when it matches /^chrome:\/\/.+\/locale\/.+\.properties/.
      Assert.equal(getValue("random.user.pref"),
        "chrome://test/locale/testing.properties");
      // Test to see if empty string when value matches /^chrome:\/\/.+\/locale\/.+\.properties/ and an exception is thrown.
      Assert.equal(getValue("gecko.handlerService.schemes.irc.1.name"), "");
    });
  });
});
