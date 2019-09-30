<<<<<<< HEAD:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/valid/top-level-empty-str-is-valid-assign-expr.js
// |reftest| 
||||||| merged common ancestors
// |reftest| skip -- dynamic-import is not supported
=======
// |reftest| skip-if(!xulRuntime.shell) -- dynamic-import is not enabled unconditionally
>>>>>>> upstream-releases:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/valid/top-level-empty-str-is-valid-assign-expr.js
// This file was procedurally generated from the following sources:
// - src/dynamic-import/empty-str-is-valid-assign-expr.case
// - src/dynamic-import/syntax/valid/top-level.template
/*---
description: Calling import('') (top level syntax)
esid: sec-import-call-runtime-semantics-evaluation
features: [dynamic-import]
flags: [generated]
info: |
    ImportCall :
        import( AssignmentExpression )

---*/

import('');

reportCompare(0, 0);
