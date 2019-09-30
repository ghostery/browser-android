<<<<<<< HEAD:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/valid/top-level-nested-imports.js
// |reftest| 
||||||| merged common ancestors
// |reftest| skip -- dynamic-import is not supported
=======
// |reftest| skip-if(!xulRuntime.shell) -- dynamic-import is not enabled unconditionally
>>>>>>> upstream-releases:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/valid/top-level-nested-imports.js
// This file was procedurally generated from the following sources:
// - src/dynamic-import/nested-imports.case
// - src/dynamic-import/syntax/valid/top-level.template
/*---
description: ImportCall is a CallExpression can be nested in other import calls (top level syntax)
esid: sec-import-call-runtime-semantics-evaluation
features: [dynamic-import]
flags: [generated]
info: |
    ImportCall :
        import( AssignmentExpression )

---*/

import(import(import('./empty_FIXTURE.js')));

reportCompare(0, 0);
