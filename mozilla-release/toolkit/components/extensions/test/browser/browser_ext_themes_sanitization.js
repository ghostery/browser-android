"use strict";

// This test checks color sanitization in various situations

add_task(async function test_sanitization_invalid() {
  // This test checks that invalid values are sanitized
  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
<<<<<<< HEAD
      "theme": {
        "colors": {
          "frame": ACCENT_COLOR,
          "tab_background_text": TEXT_COLOR,
          "bookmark_text": "ntimsfavoriteblue",
||||||| merged common ancestors
      "theme": {
        "colors": {
          "accentcolor": ACCENT_COLOR,
          "textcolor": TEXT_COLOR,
          "toolbar_text": "ntimsfavoriteblue",
=======
      theme: {
        colors: {
          frame: ACCENT_COLOR,
          tab_background_text: TEXT_COLOR,
          bookmark_text: "ntimsfavoriteblue",
>>>>>>> upstream-releases
        },
      },
    },
  });

  await extension.startup();

  let navbar = document.querySelector("#nav-bar");
  Assert.equal(
    window.getComputedStyle(navbar).color,
    "rgb(0, 0, 0)",
    "All invalid values should always compute to black."
  );

  await extension.unload();
});

add_task(async function test_sanitization_css_variables() {
  // This test checks that CSS variables are sanitized
  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
<<<<<<< HEAD
      "theme": {
        "colors": {
          "frame": ACCENT_COLOR,
          "tab_background_text": TEXT_COLOR,
          "bookmark_text": "var(--arrowpanel-dimmed)",
||||||| merged common ancestors
      "theme": {
        "colors": {
          "accentcolor": ACCENT_COLOR,
          "textcolor": TEXT_COLOR,
          "toolbar_text": "var(--arrowpanel-dimmed)",
=======
      theme: {
        colors: {
          frame: ACCENT_COLOR,
          tab_background_text: TEXT_COLOR,
          bookmark_text: "var(--arrowpanel-dimmed)",
>>>>>>> upstream-releases
        },
      },
    },
  });

  await extension.startup();

  let navbar = document.querySelector("#nav-bar");
  Assert.equal(
    window.getComputedStyle(navbar).color,
    "rgb(0, 0, 0)",
    "All CSS variables should always compute to black."
  );

  await extension.unload();
});

<<<<<<< HEAD
add_task(async function test_sanitization_important() {
  // This test checks that the sanitizer cannot be fooled with !important
  let stylesheetAttr = `href="data:text/css,*{color:red!important}" type="text/css"`;
  let stylesheet =
    document.createProcessingInstruction("xml-stylesheet", stylesheetAttr);
  let load = BrowserTestUtils.waitForEvent(stylesheet, "load");
  document.insertBefore(stylesheet, document.documentElement);
  await load;

  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
      "theme": {
        "colors": {
          "frame": ACCENT_COLOR,
          "tab_background_text": TEXT_COLOR,
          "bookmark_text": "green",
        },
      },
    },
  });

  await extension.startup();

  let navbar = document.querySelector("#nav-bar");
  Assert.equal(
    window.getComputedStyle(navbar).color,
    "rgb(255, 0, 0)",
    "Sheet applies"
  );

  stylesheet.remove();

  Assert.equal(
    window.getComputedStyle(navbar).color,
    "rgb(0, 128, 0)",
    "Shouldn't be able to fool the color sanitizer with !important"
  );

  await extension.unload();
});

add_task(async function test_sanitization_transparent() {
  // This test checks whether transparent values are applied properly
||||||| merged common ancestors
add_task(async function test_sanitization_transparent() {
  // This test checks whether transparent values are applied properly
=======
add_task(async function test_sanitization_important() {
  // This test checks that the sanitizer cannot be fooled with !important
  let stylesheetAttr = `href="data:text/css,*{color:red!important}" type="text/css"`;
  let stylesheet = document.createProcessingInstruction(
    "xml-stylesheet",
    stylesheetAttr
  );
  let load = BrowserTestUtils.waitForEvent(stylesheet, "load");
  document.insertBefore(stylesheet, document.documentElement);
  await load;

>>>>>>> upstream-releases
  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
<<<<<<< HEAD
      "theme": {
        "colors": {
          "frame": ACCENT_COLOR,
          "tab_background_text": TEXT_COLOR,
          "toolbar_top_separator": "transparent",
||||||| merged common ancestors
      "theme": {
        "colors": {
          "accentcolor": ACCENT_COLOR,
          "textcolor": TEXT_COLOR,
          "toolbar_top_separator": "transparent",
=======
      theme: {
        colors: {
          frame: ACCENT_COLOR,
          tab_background_text: TEXT_COLOR,
          bookmark_text: "green",
>>>>>>> upstream-releases
        },
      },
    },
  });

  await extension.startup();

  let navbar = document.querySelector("#nav-bar");
  Assert.equal(
    window.getComputedStyle(navbar).color,
    "rgb(255, 0, 0)",
    "Sheet applies"
  );

  stylesheet.remove();

  Assert.equal(
    window.getComputedStyle(navbar).color,
    "rgb(0, 128, 0)",
    "Shouldn't be able to fool the color sanitizer with !important"
  );

  await extension.unload();
});

<<<<<<< HEAD
add_task(async function test_sanitization_transparent_frame_color() {
  // This test checks whether transparent frame color falls back to white.
||||||| merged common ancestors
add_task(async function test_sanitization_transparent_accentcolor() {
  // This test checks whether transparent accentcolor falls back to white.
=======
add_task(async function test_sanitization_transparent() {
  // This test checks whether transparent values are applied properly
>>>>>>> upstream-releases
  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
<<<<<<< HEAD
      "theme": {
        "colors": {
          "frame": "transparent",
          "tab_background_text": TEXT_COLOR,
||||||| merged common ancestors
      "theme": {
        "colors": {
          "accentcolor": "transparent",
          "textcolor": TEXT_COLOR,
=======
      theme: {
        colors: {
          frame: ACCENT_COLOR,
          tab_background_text: TEXT_COLOR,
          toolbar_top_separator: "transparent",
>>>>>>> upstream-releases
        },
      },
    },
  });

  await extension.startup();

<<<<<<< HEAD
  let docEl = document.documentElement;
  Assert.equal(
    window.getComputedStyle(docEl).backgroundColor,
    "rgb(255, 255, 255)",
    "Accent color should be white",
||||||| merged common ancestors
  Assert.equal(
    window.getComputedStyle(docEl).backgroundColor,
    "rgb(255, 255, 255)",
    "Accent color should be white",
=======
  let navbar = document.querySelector("#nav-bar");
  Assert.ok(
    window.getComputedStyle(navbar).boxShadow.includes("rgba(0, 0, 0, 0)"),
    "Top separator should be transparent"
>>>>>>> upstream-releases
  );

  await extension.unload();
});

<<<<<<< HEAD
add_task(async function test_sanitization_transparent_tab_background_text_color() {
  // This test checks whether transparent textcolor falls back to black.
||||||| merged common ancestors
add_task(async function test_sanitization_transparent_textcolor() {
  // This test checks whether transparent textcolor falls back to black.
=======
add_task(async function test_sanitization_transparent_frame_color() {
  // This test checks whether transparent frame color falls back to white.
>>>>>>> upstream-releases
  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
<<<<<<< HEAD
      "theme": {
        "colors": {
          "frame": ACCENT_COLOR,
          "tab_background_text": "transparent",
||||||| merged common ancestors
      "theme": {
        "colors": {
          "accentcolor": ACCENT_COLOR,
          "textcolor": "transparent",
=======
      theme: {
        colors: {
          frame: "transparent",
          tab_background_text: TEXT_COLOR,
>>>>>>> upstream-releases
        },
      },
    },
  });

  await extension.startup();

  let docEl = document.documentElement;
  Assert.equal(
    window.getComputedStyle(docEl).backgroundColor,
    "rgb(255, 255, 255)",
    "Accent color should be white"
  );

  await extension.unload();
});

add_task(
  async function test_sanitization_transparent_tab_background_text_color() {
    // This test checks whether transparent textcolor falls back to black.
    let extension = ExtensionTestUtils.loadExtension({
      manifest: {
        theme: {
          colors: {
            frame: ACCENT_COLOR,
            tab_background_text: "transparent",
          },
        },
      },
    });

    await extension.startup();

    let docEl = document.documentElement;
    Assert.equal(
      window.getComputedStyle(docEl).color,
      "rgb(0, 0, 0)",
      "Text color should be black"
    );

    await extension.unload();
  }
);
