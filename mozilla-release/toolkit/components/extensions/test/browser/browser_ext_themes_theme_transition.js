"use strict";

// This test checks whether the applied theme transition effects are applied
// correctly.

add_task(async function test_theme_transition_effects() {
  const TOOLBAR = "#f27489";
  const TEXT_COLOR = "#000000";
  const TRANSITION_PROPERTY = "background-color";

  let extension = ExtensionTestUtils.loadExtension({
    manifest: {
<<<<<<< HEAD
      "theme": {
        "colors": {
          "tab_background_text": TEXT_COLOR,
          "toolbar": TOOLBAR,
          "bookmark_text": TEXT_COLOR,
||||||| merged common ancestors
      "theme": {
        "colors": {
          "accentcolor": ACCENT_COLOR,
          "textcolor": TEXT_COLOR,
          "toolbar": TOOLBAR,
          "toolbar_text": TEXT_COLOR,
=======
      theme: {
        colors: {
          tab_background_text: TEXT_COLOR,
          toolbar: TOOLBAR,
          bookmark_text: TEXT_COLOR,
>>>>>>> upstream-releases
        },
      },
    },
  });

  await extension.startup();

  // check transition effect for toolbars
  let navbar = document.querySelector("#nav-bar");
  let navbarCS = window.getComputedStyle(navbar);

  Assert.ok(
    navbarCS
      .getPropertyValue("transition-property")
      .includes(TRANSITION_PROPERTY),
    "Transition property set for #nav-bar"
  );

  let bookmarksBar = document.querySelector("#PersonalToolbar");
  bookmarksBar.setAttribute("collapsed", "false");
  let bookmarksBarCS = window.getComputedStyle(bookmarksBar);

  Assert.ok(
    bookmarksBarCS
      .getPropertyValue("transition-property")
      .includes(TRANSITION_PROPERTY),
    "Transition property set for #PersonalToolbar"
  );

  await extension.unload();
});
