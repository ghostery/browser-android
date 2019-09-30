/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
   http://creativecommons.org/publicdomain/zero/1.0/ */
"use strict";

// Testing that searching for elements using the inspector search field
// produces correct suggestions.

const TEST_URL = URL_ROOT + "doc_inspector_search.html";

// An array of (key, suggestions) pairs where key is a key to press and
// suggestions is an array of suggestions that should be shown in the popup.
// Suggestion is an object with label of the entry and optional count
// (defaults to 1)
var TEST_DATA = [
  {
    key: "d",
<<<<<<< HEAD
    suggestions: [
      {label: "div"},
      {label: "#d1"},
      {label: "#d2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: "div"},
      {label: "#d1"},
      {label: "#d2"}
    ]
=======
    suggestions: [{ label: "div" }, { label: "#d1" }, { label: "#d2" }],
>>>>>>> upstream-releases
  },
  {
    key: "i",
<<<<<<< HEAD
    suggestions: [{label: "div"}],
||||||| merged common ancestors
    suggestions: [{label: "div"}]
=======
    suggestions: [{ label: "div" }],
>>>>>>> upstream-releases
  },
  {
    key: "v",
    suggestions: [],
  },
  {
    key: ".",
<<<<<<< HEAD
    suggestions: [{label: "div.c1"}],
||||||| merged common ancestors
    suggestions: [{label: "div.c1"}]
=======
    suggestions: [{ label: "div.c1" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "#",
<<<<<<< HEAD
    suggestions: [
      {label: "div#d1"},
      {label: "div#d2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: "div#d1"},
      {label: "div#d2"}
    ]
=======
    suggestions: [{ label: "div#d1" }, { label: "div#d2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "VK_BACK_SPACE",
<<<<<<< HEAD
    suggestions: [{label: "div"}],
||||||| merged common ancestors
    suggestions: [{label: "div"}]
=======
    suggestions: [{ label: "div" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
<<<<<<< HEAD
    suggestions: [
      {label: "div"},
      {label: "#d1"},
      {label: "#d2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: "div"},
      {label: "#d1"},
      {label: "#d2"}
    ]
=======
    suggestions: [{ label: "div" }, { label: "#d1" }, { label: "#d2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: ".",
<<<<<<< HEAD
    suggestions: [
      {label: ".c1"},
      {label: ".c2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: ".c1"},
      {label: ".c2"}
    ]
=======
    suggestions: [{ label: ".c1" }, { label: ".c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "c",
<<<<<<< HEAD
    suggestions: [
      {label: ".c1"},
      {label: ".c2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: ".c1"},
      {label: ".c2"}
    ]
=======
    suggestions: [{ label: ".c1" }, { label: ".c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "2",
    suggestions: [],
  },
  {
    key: "VK_BACK_SPACE",
<<<<<<< HEAD
    suggestions: [
      {label: ".c1"},
      {label: ".c2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: ".c1"},
      {label: ".c2"}
    ]
=======
    suggestions: [{ label: ".c1" }, { label: ".c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "1",
    suggestions: [],
  },
  {
    key: "#",
<<<<<<< HEAD
    suggestions: [
      {label: "#d2"},
      {label: "#p1"},
      {label: "#s2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: "#d2"},
      {label: "#p1"},
      {label: "#s2"}
    ]
=======
    suggestions: [{ label: "#d2" }, { label: "#p1" }, { label: "#s2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "VK_BACK_SPACE",
<<<<<<< HEAD
    suggestions: [
      {label: ".c1"},
      {label: ".c2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: ".c1"},
      {label: ".c2"}
    ]
=======
    suggestions: [{ label: ".c1" }, { label: ".c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
<<<<<<< HEAD
    suggestions: [
      {label: ".c1"},
      {label: ".c2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: ".c1"},
      {label: ".c2"}
    ]
=======
    suggestions: [{ label: ".c1" }, { label: ".c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "#",
    suggestions: [
<<<<<<< HEAD
      {label: "#b1"},
      {label: "#d1"},
      {label: "#d2"},
      {label: "#p1"},
      {label: "#p2"},
      {label: "#p3"},
      {label: "#s1"},
      {label: "#s2"},
    ],
||||||| merged common ancestors
      {label: "#b1"},
      {label: "#d1"},
      {label: "#d2"},
      {label: "#p1"},
      {label: "#p2"},
      {label: "#p3"},
      {label: "#s1"},
      {label: "#s2"}
    ]
=======
      { label: "#b1" },
      { label: "#d1" },
      { label: "#d2" },
      { label: "#p1" },
      { label: "#p2" },
      { label: "#p3" },
      { label: "#s1" },
      { label: "#s2" },
    ],
>>>>>>> upstream-releases
  },
  {
    key: "p",
<<<<<<< HEAD
    suggestions: [
      {label: "#p1"},
      {label: "#p2"},
      {label: "#p3"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: "#p1"},
      {label: "#p2"},
      {label: "#p3"}
    ]
=======
    suggestions: [{ label: "#p1" }, { label: "#p2" }, { label: "#p3" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [
<<<<<<< HEAD
      {label: "#b1"},
      {label: "#d1"},
      {label: "#d2"},
      {label: "#p1"},
      {label: "#p2"},
      {label: "#p3"},
      {label: "#s1"},
      {label: "#s2"},
    ],
||||||| merged common ancestors
      {label: "#b1"},
      {label: "#d1"},
      {label: "#d2"},
      {label: "#p1"},
      {label: "#p2"},
      {label: "#p3"},
      {label: "#s1"},
      {label: "#s2"}
    ]
=======
      { label: "#b1" },
      { label: "#d1" },
      { label: "#d2" },
      { label: "#p1" },
      { label: "#p2" },
      { label: "#p3" },
      { label: "#s1" },
      { label: "#s2" },
    ],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "p",
    suggestions: [
<<<<<<< HEAD
      {label: "p"},
      {label: "#p1"},
      {label: "#p2"},
      {label: "#p3"},
    ],
||||||| merged common ancestors
      {label: "p"},
      {label: "#p1"},
      {label: "#p2"},
      {label: "#p3"}
    ]
=======
      { label: "p" },
      { label: "#p1" },
      { label: "#p2" },
      { label: "#p3" },
    ],
>>>>>>> upstream-releases
  },
  {
<<<<<<< HEAD
    key: "[", suggestions: [],
||||||| merged common ancestors
    key: "[", suggestions: []
=======
    key: "[",
    suggestions: [],
>>>>>>> upstream-releases
  },
  {
<<<<<<< HEAD
    key: "i", suggestions: [],
||||||| merged common ancestors
    key: "i", suggestions: []
=======
    key: "i",
    suggestions: [],
>>>>>>> upstream-releases
  },
  {
<<<<<<< HEAD
    key: "d", suggestions: [],
||||||| merged common ancestors
    key: "d", suggestions: []
=======
    key: "d",
    suggestions: [],
>>>>>>> upstream-releases
  },
  {
<<<<<<< HEAD
    key: "*", suggestions: [],
||||||| merged common ancestors
    key: "*", suggestions: []
=======
    key: "*",
    suggestions: [],
>>>>>>> upstream-releases
  },
  {
<<<<<<< HEAD
    key: "=", suggestions: [],
||||||| merged common ancestors
    key: "=", suggestions: []
=======
    key: "=",
    suggestions: [],
>>>>>>> upstream-releases
  },
  {
<<<<<<< HEAD
    key: "p", suggestions: [],
||||||| merged common ancestors
    key: "p", suggestions: []
=======
    key: "p",
    suggestions: [],
>>>>>>> upstream-releases
  },
  {
<<<<<<< HEAD
    key: "]", suggestions: [],
||||||| merged common ancestors
    key: "]", suggestions: []
=======
    key: "]",
    suggestions: [],
>>>>>>> upstream-releases
  },
  {
    key: ".",
<<<<<<< HEAD
    suggestions: [
      {label: "p[id*=p].c1"},
      {label: "p[id*=p].c2"},
    ],
||||||| merged common ancestors
    suggestions: [
      {label: "p[id*=p].c1"},
      {label: "p[id*=p].c2"}
    ]
=======
    suggestions: [{ label: "p[id*=p].c1" }, { label: "p[id*=p].c2" }],
>>>>>>> upstream-releases
  },
  {
    key: "VK_BACK_SPACE",
    suggestions: [],
  },
  {
    key: "#",
    suggestions: [
<<<<<<< HEAD
      {label: "p[id*=p]#p1"},
      {label: "p[id*=p]#p2"},
      {label: "p[id*=p]#p3"},
    ],
  },
||||||| merged common ancestors
      {label: "p[id*=p]#p1"},
      {label: "p[id*=p]#p2"},
      {label: "p[id*=p]#p3"}
    ]
  }
=======
      { label: "p[id*=p]#p1" },
      { label: "p[id*=p]#p2" },
      { label: "p[id*=p]#p3" },
    ],
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
        actualSuggestions[i].label,
        suggestions[i].label,
        "The suggestion at " + i + "th index is correct."
      );
    }
  }
});

function formatSuggestions(suggestions) {
  return "[" + suggestions.map(s => "'" + s.label + "'").join(", ") + "]";
}
