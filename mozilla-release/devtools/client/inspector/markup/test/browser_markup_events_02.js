/* vim: set ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 http://creativecommons.org/publicdomain/zero/1.0/ */
/* import-globals-from helper_events_test_runner.js */

"use strict";

// Test that markup view event bubbles show the correct event info for DOM
// events.

const TEST_URL = URL_ROOT + "doc_markup_events_02.html";

loadHelperScript("helper_events_test_runner.js");

const TEST_DATA = [ // eslint-disable-line
  {
    selector: "#fatarrow",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":39",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "() => {\n" +
                 "  alert(\"Fat arrow without params!\");\n" +
                 "}",
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "() => {\n" +
                 "  alert(\"Fat arrow without params!\");\n" +
                 "}"
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "() => {\n" + '  alert("Fat arrow without params!");\n' + "}",
>>>>>>> upstream-releases
      },
      {
        type: "click",
        filename: TEST_URL + ":43",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "event => {\n" +
                 "  alert(\"Fat arrow with 1 param!\");\n" +
                 "}",
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "event => {\n" +
                 "  alert(\"Fat arrow with 1 param!\");\n" +
                 "}"
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "event => {\n" + '  alert("Fat arrow with 1 param!");\n' + "}",
>>>>>>> upstream-releases
      },
      {
        type: "click",
        filename: TEST_URL + ":47",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "(event, foo, bar) => {\n" +
                 "  alert(\"Fat arrow with 3 params!\");\n" +
                 "}",
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "(event, foo, bar) => {\n" +
                 "  alert(\"Fat arrow with 3 params!\");\n" +
                 "}"
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "(event, foo, bar) => {\n" +
          '  alert("Fat arrow with 3 params!");\n' +
          "}",
>>>>>>> upstream-releases
      },
      {
        type: "click",
        filename: TEST_URL + ":51",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "b => b",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "b => b"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "b => b",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#bound",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":62",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function(event) {\n" +
                 "  alert(\"Bound event\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function(event) {\n" +
                 "  alert(\"Bound event\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function(event) {\n" + '  alert("Bound event");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#boundhe",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":85",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function() {\n" +
                 "  alert(\"boundHandleEvent\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function() {\n" +
                 "  alert(\"boundHandleEvent\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function() {\n" + '  alert("boundHandleEvent");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#comment-inline",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":91",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function functionProceededByInlineComment() {\n" +
                 "  alert(\"comment-inline\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function functionProceededByInlineComment() {\n" +
                 "  alert(\"comment-inline\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function functionProceededByInlineComment() {\n" +
          '  alert("comment-inline");\n' +
          "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#comment-streaming",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":96",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function functionProceededByStreamingComment() {\n" +
                 "  alert(\"comment-streaming\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function functionProceededByStreamingComment() {\n" +
                 "  alert(\"comment-streaming\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function functionProceededByStreamingComment() {\n" +
          '  alert("comment-streaming");\n' +
          "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#anon-object-method",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":71",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function() {\n" +
                 "  alert(\"obj.anonObjectMethod\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function() {\n" +
                 "  alert(\"obj.anonObjectMethod\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function() {\n" + '  alert("obj.anonObjectMethod");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#object-method",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":75",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function kay() {\n" +
                 "  alert(\"obj.objectMethod\");\n" +
                 "}",
      },
    ],
  },
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function kay() {\n" +
                 "  alert(\"obj.objectMethod\");\n" +
                 "}"
      }
    ]
  }
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function kay() {\n" + '  alert("obj.objectMethod");\n' + "}",
      },
    ],
  },
>>>>>>> upstream-releases
];

add_task(async function() {
  await runEventPopupTests(TEST_URL, TEST_DATA);
});
