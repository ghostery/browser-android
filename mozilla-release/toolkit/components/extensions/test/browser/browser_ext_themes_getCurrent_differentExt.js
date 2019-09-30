"use strict";

// This test checks whether browser.theme.getCurrent() works correctly when theme
// does not originate from extension querying the theme.

add_task(async function test_getcurrent() {
  const theme = ExtensionTestUtils.loadExtension({
    manifest: {
<<<<<<< HEAD
      "theme": {
        "images": {
          "theme_frame": "image1.png",
||||||| merged common ancestors
      "theme": {
        "images": {
          "headerURL": "image1.png",
=======
      theme: {
        images: {
          theme_frame: "image1.png",
>>>>>>> upstream-releases
        },
<<<<<<< HEAD
        "colors": {
          "frame": ACCENT_COLOR,
          "tab_background_text": TEXT_COLOR,
||||||| merged common ancestors
        "colors": {
          "accentcolor": ACCENT_COLOR,
          "textcolor": TEXT_COLOR,
=======
        colors: {
          frame: ACCENT_COLOR,
          tab_background_text: TEXT_COLOR,
>>>>>>> upstream-releases
        },
      },
    },
    files: {
      "image1.png": BACKGROUND,
    },
  });

  const extension = ExtensionTestUtils.loadExtension({
    background() {
      browser.theme.onUpdated.addListener(() => {
        browser.theme.getCurrent().then(theme => {
          browser.test.sendMessage("theme-updated", theme);
        });
      });
    },
  });

  await extension.startup();

  info("Testing getCurrent after static theme startup");
  let updatedPromise = extension.awaitMessage("theme-updated");
  await theme.startup();
  let receivedTheme = await updatedPromise;
<<<<<<< HEAD
  Assert.ok(receivedTheme.images.theme_frame.includes("image1.png"),
            "getCurrent returns correct theme_frame image");
  Assert.equal(receivedTheme.colors.frame, ACCENT_COLOR,
               "getCurrent returns correct frame color");
  Assert.equal(receivedTheme.colors.tab_background_text, TEXT_COLOR,
               "getCurrent returns correct tab_background_text color");
||||||| merged common ancestors
  Assert.ok(receivedTheme.images.headerURL.includes("image1.png"),
            "getCurrent returns correct headerURL");
  Assert.equal(receivedTheme.colors.accentcolor, ACCENT_COLOR,
               "getCurrent returns correct accentcolor");
  Assert.equal(receivedTheme.colors.textcolor, TEXT_COLOR,
               "getCurrent returns correct textcolor");
=======
  Assert.ok(
    receivedTheme.images.theme_frame.includes("image1.png"),
    "getCurrent returns correct theme_frame image"
  );
  Assert.equal(
    receivedTheme.colors.frame,
    ACCENT_COLOR,
    "getCurrent returns correct frame color"
  );
  Assert.equal(
    receivedTheme.colors.tab_background_text,
    TEXT_COLOR,
    "getCurrent returns correct tab_background_text color"
  );
>>>>>>> upstream-releases

  info("Testing getCurrent after static theme unload");
  updatedPromise = extension.awaitMessage("theme-updated");
  await theme.unload();
  receivedTheme = await updatedPromise;
  Assert.equal(Object.keys(receivedTheme), 0, "getCurrent returns empty theme");

  await extension.unload();
});
