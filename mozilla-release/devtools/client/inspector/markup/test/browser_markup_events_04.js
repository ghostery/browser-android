/* vim: set ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 http://creativecommons.org/publicdomain/zero/1.0/ */
/* import-globals-from helper_events_test_runner.js */

"use strict";

// Test that markup view event bubbles show the correct event info for DOM
// events.

const TEST_URL = URL_ROOT + "doc_markup_events_04.html";

loadHelperScript("helper_events_test_runner.js");

const TEST_DATA = [ // eslint-disable-line
  {
    selector: "html",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":56",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function(foo2, bar2) {\n" +
                 "  alert(\"documentElement event listener clicked\");\n" +
                 "}",
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function(foo2, bar2) {\n" +
                 "  alert(\"documentElement event listener clicked\");\n" +
                 "}"
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function(foo2, bar2) {\n" +
          '  alert("documentElement event listener clicked");\n' +
          "}",
>>>>>>> upstream-releases
      },
      {
        type: "click",
        filename: TEST_URL + ":52",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function(foo, bar) {\n" +
                 "  alert(\"document event listener clicked\");\n" +
                 "}",
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function(foo, bar) {\n" +
                 "  alert(\"document event listener clicked\");\n" +
                 "}"
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function(foo, bar) {\n" +
          '  alert("document event listener clicked");\n' +
          "}",
>>>>>>> upstream-releases
      },
      {
        type: "load",
        filename: TEST_URL,
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM0",
        ],
        handler: "function onload(event) {\n" +
                 "  init();\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM0"
        ],
        handler: "function onload(event) {\n" +
                 "  init();\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM0"],
        handler: "function onload(event) {\n" + "  init();\n" + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#constructed-function",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":1",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function anonymous() {\n" +
                 "\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function anonymous() {\n" +
                 "\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function anonymous() {\n" + "\n" + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#constructed-function-with-body-string",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":1",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function anonymous(a, b, c) {\n" +
                 "  alert(\"constructedFuncWithBodyString\");\n" +
        "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function anonymous(a, b, c) {\n" +
                 "  alert(\"constructedFuncWithBodyString\");\n" +
        "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function anonymous(a, b, c) {\n" +
          '  alert("constructedFuncWithBodyString");\n' +
          "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#multiple-assignment",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":24",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function multi() {\n" +
                 "  alert(\"multipleAssignment\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function multi() {\n" +
                 "  alert(\"multipleAssignment\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function multi() {\n" + '  alert("multipleAssignment");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#promise",
    expected: [
      {
        type: "click",
        filename: "[native code]",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function() {\n" +
                 "  [native code]\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function() {\n" +
                 "  [native code]\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function() {\n" + "  [native code]\n" + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#arraysort",
    expected: [
      {
        type: "click",
        filename: "[native code]",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function sort(arr, comparefn) {\n" +
                 "  [native code]\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function sort(arr, comparefn) {\n" +
                 "  [native code]\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function sort(, ) {\n" + "  [native code]\n" + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#handleEvent",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":77",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function(event) {\n" +
                 "  switch (event.type) {\n" +
                 "    case \"click\":\n" +
                 "      alert(\"handleEvent click\");\n" +
                 "  }\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function(event) {\n" +
                 "  switch (event.type) {\n" +
                 "    case \"click\":\n" +
                 "      alert(\"handleEvent click\");\n" +
                 "  }\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function(event) {\n" +
          "  switch (event.type) {\n" +
          '    case "click":\n' +
          '      alert("handleEvent click");\n' +
          "  }\n" +
          "}",
      },
    ],
>>>>>>> upstream-releases
  },
];

add_task(async function() {
  await runEventPopupTests(TEST_URL, TEST_DATA);
});
