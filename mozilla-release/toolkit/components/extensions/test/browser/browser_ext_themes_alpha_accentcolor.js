"use strict";

add_task(async function test_alpha_frame_color() {
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
          "frame": "rgba(230, 128, 0, 0.1)",
          "tab_background_text": TEXT_COLOR,
||||||| merged common ancestors
        "colors": {
          "accentcolor": "rgba(230, 128, 0, 0.1)",
          "textcolor": TEXT_COLOR,
=======
        colors: {
          frame: "rgba(230, 128, 0, 0.1)",
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

  // Add the event listener before loading the extension
  let docEl = window.document.documentElement;
  let style = window.getComputedStyle(docEl);

  Assert.equal(
    style.backgroundColor,
    "rgb(230, 128, 0)",
    "Window background color should be opaque"
  );

  await extension.unload();
});
