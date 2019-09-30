/* vim: set ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 http://creativecommons.org/publicdomain/zero/1.0/ */
/* import-globals-from helper_events_test_runner.js */

"use strict";

// Test that markup view event bubbles show the correct event info for DOM
// events.

const TEST_URL = URL_ROOT + "doc_markup_events_03.html";

loadHelperScript("helper_events_test_runner.js");

const TEST_DATA = [ // eslint-disable-line
  {
    selector: "#es6-method",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":66",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "es6Method(foo, bar) {\n" +
                 "  alert(\"obj.es6Method\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "es6Method(foo, bar) {\n" +
                 "  alert(\"obj.es6Method\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "es6Method(foo, bar) {\n" + '  alert("obj.es6Method");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#generator",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":85",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function* generator() {\n" +
                 "  alert(\"generator\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function* generator() {\n" +
                 "  alert(\"generator\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function* generator() {\n" + '  alert("generator");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#anon-generator",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":43",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function*() {\n" +
                 "  alert(\"anonGenerator\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function*() {\n" +
                 "  alert(\"anonGenerator\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function*() {\n" + '  alert("anonGenerator");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#named-function-expression",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":20",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function foo() {\n" +
                 "  alert(\"namedFunctionExpression\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function foo() {\n" +
                 "  alert(\"namedFunctionExpression\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function foo() {\n" + '  alert("namedFunctionExpression");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#anon-function-expression",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":24",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function() {\n" +
                 "  alert(\"anonFunctionExpression\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function() {\n" +
                 "  alert(\"anonFunctionExpression\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler:
          "function() {\n" + '  alert("anonFunctionExpression");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
  {
    selector: "#returned-function",
    expected: [
      {
        type: "click",
        filename: TEST_URL + ":29",
<<<<<<< HEAD
        attributes: [
          "Bubbling",
          "DOM2",
        ],
        handler: "function bar() {\n" +
                 "  alert(\"returnedFunction\");\n" +
                 "}",
      },
    ],
||||||| merged common ancestors
        attributes: [
          "Bubbling",
          "DOM2"
        ],
        handler: "function bar() {\n" +
                 "  alert(\"returnedFunction\");\n" +
                 "}"
      }
    ]
=======
        attributes: ["Bubbling", "DOM2"],
        handler: "function bar() {\n" + '  alert("returnedFunction");\n' + "}",
      },
    ],
>>>>>>> upstream-releases
  },
];

add_task(async function() {
  await runEventPopupTests(TEST_URL, TEST_DATA);
});
