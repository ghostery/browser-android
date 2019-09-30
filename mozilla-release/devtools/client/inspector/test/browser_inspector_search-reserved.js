/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
"use strict";

// Testing searching for ids and classes that contain reserved characters.
const TEST_URL = URL_ROOT + "doc_inspector_search-reserved.html";

// An array of (key, suggestions) pairs where key is a key to press and
// suggestions is an array of suggestions that should be shown in the popup.
// Suggestion is an object with label of the entry and optional count
// (defaults to 1)
const TEST_DATA = [
  {
    key: "#",
<<<<<<< HEAD
    suggestions: [{label: "#d1\\.d2"}],
||||||| merged common ancestors
    suggestions: [{label: "#d1\\.d2"}]
=======
    suggestions: [{ label: "#d1\\.d2" }],
>>>>>>> upstream-releases
  },
  {
    key: "d",
<<<<<<< HEAD
    suggestions: [{label: "#d1\\.d2"}],
||||||| merged common ancestors
    suggestions: [{label: "#d1\\.d2"}]
=======
    suggestions: [{ label: "#d1\\.d2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
<<<<<<< HEAD
    suggestions: [{label: "#d1\\.d2"}],
||||||| merged common ancestors
    suggestions: [{label: "#d1\\.d2"}]
=======
    suggestions: [{ label: "#d1\\.d2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: ".",
<<<<<<< HEAD
    suggestions: [{label: ".c1\\.c2"}],
||||||| merged common ancestors
    suggestions: [{label: ".c1\\.c2"}]
=======
    suggestions: [{ label: ".c1\\.c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "c",
<<<<<<< HEAD
    suggestions: [{label: ".c1\\.c2"}],
||||||| merged common ancestors
    suggestions: [{label: ".c1\\.c2"}]
=======
    suggestions: [{ label: ".c1\\.c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
<<<<<<< HEAD
    suggestions: [{label: ".c1\\.c2"}],
||||||| merged common ancestors
    suggestions: [{label: ".c1\\.c2"}]
=======
    suggestions: [{ label: ".c1\\.c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "d",
<<<<<<< HEAD
    suggestions: [{label: "div"},
                  {label: "#d1\\.d2"}],
||||||| merged common ancestors
    suggestions: [{label: "div"},
                  {label: "#d1\\.d2"}]
=======
    suggestions: [{ label: "div" }, { label: "#d1\\.d2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "c",
<<<<<<< HEAD
    suggestions: [{label: ".c1\\.c2"}],
||||||| merged common ancestors
    suggestions: [{label: ".c1\\.c2"}]
=======
    suggestions: [{ label: ".c1\\.c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "b",
<<<<<<< HEAD
    suggestions: [{label: "body"}],
||||||| merged common ancestors
    suggestions: [{label: "body"}]
=======
    suggestions: [{ label: "body" }],
>>>>>>> upstream-releases
  },
  {
    key: "o",
<<<<<<< HEAD
    suggestions: [{label: "body"}],
||||||| merged common ancestors
    suggestions: [{label: "body"}]
=======
    suggestions: [{ label: "body" }],
>>>>>>> upstream-releases
  },
  {
    key: "d",
<<<<<<< HEAD
    suggestions: [{label: "body"}],
||||||| merged common ancestors
    suggestions: [{label: "body"}]
=======
    suggestions: [{ label: "body" }],
>>>>>>> upstream-releases
  },
  {
    key: "y",
    suggestions: [],
  },
  {
    key: " ",
<<<<<<< HEAD
    suggestions: [{label: "body div"}],
||||||| merged common ancestors
    suggestions: [{label: "body div"}]
=======
    suggestions: [{ label: "body div" }],
>>>>>>> upstream-releases
  },
  {
    key: ".",
<<<<<<< HEAD
    suggestions: [{label: "body .c1\\.c2"}],
||||||| merged common ancestors
    suggestions: [{label: "body .c1\\.c2"}]
=======
    suggestions: [{ label: "body .c1\\.c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
<<<<<<< HEAD
    suggestions: [{label: "body div"}],
||||||| merged common ancestors
    suggestions: [{label: "body div"}]
=======
    suggestions: [{ label: "body div" }],
>>>>>>> upstream-releases
  },
  {
    key: "#",
<<<<<<< HEAD
    suggestions: [{label: "body #d1\\.d2"}],
  },
||||||| merged common ancestors
    suggestions: [{label: "body #d1\\.d2"}]
  }
=======
    suggestions: [{ label: "body #d1\\.d2" }],
  },
>>>>>>> upstream-releases
];

add_task(async function() {
  const { inspector } = await openInspectorForURL(TEST_URL);
  const searchBox = inspector.searchBox;
  const popup = inspector.searchSuggestions.searchPopup;

  await focusSearchBoxUsingShortcut(inspector.panelWin);

  for (const { key, suggestions } of TEST_DATA) {
    info("Pressing " + key + " to get " + formatSuggestions(suggestions));

    const command = once(searchBox, "input");
    EventUtils.synthesizeKey(key, {}, inspector.panelWin);
    await command;

    info("Waiting for search query to complete");
    await inspector.searchSuggestions._lastQuery;

    info(
      "Query completed. Performing checks for input '" + searchBox.value + "'"
    );
    const actualSuggestions = popup.getItems();

    is(
      popup.isOpen ? actualSuggestions.length : 0,
      suggestions.length,
      "There are expected number of suggestions."
    );

    for (let i = 0; i < suggestions.length; i++) {
      is(
        suggestions[i].label,
        actualSuggestions[i].label,
        "The suggestion at " + i + "th index is correct."
      );
    }
  }
});

function formatSuggestions(suggestions) {
  return "[" + suggestions.map(s => "'" + s.label + "'").join(", ") + "]";
}
