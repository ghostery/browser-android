<<<<<<< HEAD:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/valid/nested-block-labeled-nested-imports.js
// |reftest| 
||||||| merged common ancestors
// |reftest| skip -- dynamic-import is not supported
=======
// |reftest| skip-if(!xulRuntime.shell) -- dynamic-import is not enabled unconditionally
>>>>>>> upstream-releases:mozilla-release/js/src/tests/test262/language/expressions/dynamic-import/syntax/valid/nested-block-labeled-nested-imports.js
// This file was procedurally generated from the following sources:
// - src/dynamic-import/nested-imports.case
// - src/dynamic-import/syntax/valid/nested-block-labeled.template
/*---
description: ImportCall is a CallExpression can be nested in other import calls (nested block syntax)
esid: sec-import-call-runtime-semantics-evaluation
features: [dynamic-import]
flags: [generated]
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

---*/

label: {
  import(import(import('./empty_FIXTURE.js')));
};

reportCompare(0, 0);
