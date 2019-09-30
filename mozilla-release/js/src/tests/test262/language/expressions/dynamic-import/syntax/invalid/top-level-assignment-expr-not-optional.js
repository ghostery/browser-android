<<<<<<< HEAD:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/invalid/top-level-assignment-expr-not-optional.js
// |reftest| error:SyntaxError
||||||| merged common ancestors
// |reftest| skip error:SyntaxError -- dynamic-import is not supported
=======
// |reftest| skip-if(!xulRuntime.shell) error:SyntaxError -- dynamic-import is not enabled unconditionally
>>>>>>> upstream-releases:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/invalid/top-level-assignment-expr-not-optional.js
// This file was procedurally generated from the following sources:
// - src/dynamic-import/assignment-expr-not-optional.case
// - src/dynamic-import/syntax/invalid/top-level.template
/*---
description: It's a SyntaxError if AssignmentExpression is omitted (top level syntax)
esid: sec-import-call-runtime-semantics-evaluation
features: [dynamic-import]
flags: [generated]
negative:
  phase: parse
  type: SyntaxError
info: |
    ImportCall :
        import( AssignmentExpression )


    ImportCall :
        import( AssignmentExpression[+In, ?Yield] )
---*/

$DONOTEVALUATE();

import();

/* The params region intentionally empty */
