<<<<<<< HEAD
"use strict";

module.exports = {
  "extends": [
    "plugin:mozilla/browser-test",
  ]
};
||||||| merged common ancestors
=======
"use strict";

module.exports = {
  overrides: [
    {
      files: [
        "file_module_loaded.js",
        "file_module_loaded2.js",
      ],
      parserOptions: {
        sourceType: "module",
      },
    },
  ],
  "extends": [
    "plugin:mozilla/browser-test"
  ]
};
>>>>>>> upstream-releases
