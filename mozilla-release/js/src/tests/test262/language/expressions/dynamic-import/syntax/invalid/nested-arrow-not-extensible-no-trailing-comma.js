<<<<<<< HEAD:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/invalid/nested-arrow-not-extensible-no-trailing-comma.js
// |reftest| error:SyntaxError
||||||| merged common ancestors
// |reftest| skip error:SyntaxError -- dynamic-import is not supported
=======
// |reftest| skip-if(!xulRuntime.shell) error:SyntaxError -- dynamic-import is not enabled unconditionally
>>>>>>> upstream-releases:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/invalid/nested-arrow-not-extensible-no-trailing-comma.js
// This file was procedurally generated from the following sources:
// - src/dynamic-import/not-extensible-no-trailing-comma.case
// - src/dynamic-import/syntax/invalid/nested-arrow.template
/*---
description: ImportCall is not extensible - trailing comma (nested arrow syntax)
esid: sec-import-call-runtime-semantics-evaluation
features: [dynamic-import]
flags: [generated]
negative:
  phase: parse
  type: SyntaxError
info: |
    ImportCall :
        import( AssignmentExpression )

    1. Let referencingScriptOrModule be ! GetActiveScriptOrModule().
    2. Assert: referencingScriptOrModule is a Script Record or Module Record (i.e. is not null).
    3. Let argRef be the result of evaluating AssignmentExpression.
    4. Let specifier be ? GetValue(argRef).
    5. Let promiseCapability be ! NewPromiseCapability(%Promise%).
    6. Let specifierString be ToString(specifier).
    7. IfAbruptRejectPromise(specifierString, promiseCapability).
    8. Perform ! HostImportModuleDynamically(referencingScriptOrModule, specifierString, promiseCapability).
    9. Return promiseCapability.[[Promise]].


    ImportCall :
        import( AssignmentExpression[+In, ?Yield] )

    Forbidden Extensions

    - ImportCall must not be extended.
---*/

$DONOTEVALUATE();

let f = () => {
  import('',);
};
