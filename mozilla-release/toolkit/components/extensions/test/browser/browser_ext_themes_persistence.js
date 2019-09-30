"use strict";

// This test checks whether applied WebExtension themes are persisted and applied
// on newly opened windows.

add_task(async function test_multiple_windows() {
  let extension = ExtensionTestUtils.loadExtension({
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

  await extension.startup();

  let docEl = window.document.documentElement;
  let style = window.getComputedStyle(docEl);

  Assert.ok(docEl.hasAttribute("lwtheme"), "LWT attribute should be set");
  Assert.equal(
    docEl.getAttribute("lwthemetextcolor"),
    "bright",
    "LWT text color attribute should be set"
  );
  Assert.ok(
    style.backgroundImage.includes("image1.png"),
    "Expected background image"
  );

  // Now we'll open a new window to see if the theme is also applied there.
  let window2 = await BrowserTestUtils.openNewBrowserWindow();
  docEl = window2.document.documentElement;
  style = window2.getComputedStyle(docEl);

  Assert.ok(docEl.hasAttribute("lwtheme"), "LWT attribute should be set");
<<<<<<< HEAD
  Assert.equal(docEl.getAttribute("lwthemetextcolor"), "bright",
               "LWT text color attribute should be set");
  // This time around we should be using the persisted theme file from the profile.
  Assert.ok(style.backgroundImage.includes("lightweighttheme-header"), "Expected background image");
||||||| merged common ancestors
  Assert.equal(docEl.getAttribute("lwthemetextcolor"), "bright",
               "LWT text color attribute should be set");
  Assert.ok(style.backgroundImage.includes("image1.png"), "Expected background image");
=======
  Assert.equal(
    docEl.getAttribute("lwthemetextcolor"),
    "bright",
    "LWT text color attribute should be set"
  );
  Assert.ok(
    style.backgroundImage.includes("image1.png"),
    "Expected background image"
  );
>>>>>>> upstream-releases

  await BrowserTestUtils.closeWindow(window2);
  await extension.unload();
});
