<<<<<<< HEAD
// |jit-test| skip-if: !('oomTest' in this)

(function () {
    g = newGlobal();
    g.parent = this;
    g.eval("(function() { var dbg = Debugger(parent); dbg.onEnterFrame = function() {} } )")
    ``;
    oomTest(async function() {}, {expectExceptionOnFailure: false});
})()
||||||| merged common ancestors
=======
// |jit-test| skip-if: !('oomTest' in this)

ignoreUnhandledRejections();

(function () {
    g = newGlobal({newCompartment: true});
    g.parent = this;
    g.eval("(function() { var dbg = Debugger(parent); dbg.onEnterFrame = function() {} } )")
    ``;
    oomTest(async function() {}, {expectExceptionOnFailure: false});
})()
>>>>>>> upstream-releases
