<<<<<<< HEAD
// The debugger can force an early return from any instruction before the initial yield.

let g = newGlobal();
g.eval(`
  function* f() {
    yield 1;
  }
`);

function test(ttl) {
    let dbg = new Debugger(g);
    let exiting = false;  // we ran out of time-to-live and have forced return
    let done = false;  // we reached the initial yield without forced return
    dbg.onEnterFrame = frame => {
        assertEq(frame.callee.name, "f");
        frame.onEnterFrame = undefined;
        frame.onStep = () => {
            if (ttl == 0) {
                exiting = true;
                // Forced return here causes the generator object, if any, not
                // to be exposed.
                return {return: "ponies"};
            }
            ttl--;
        };
        frame.onPop = completion => {
            if (!exiting)
                done = true;
        };
    };

    let result = g.f();
    if (done)
        assertEq(result instanceof g.f, true);
    else
        assertEq(result, "ponies");

    dbg.enabled = false;
    return done;
}

for (let ttl = 0; !test(ttl); ttl++) {}
||||||| merged common ancestors
=======
// The debugger can't force return from a generator before the initial yield.

let g = newGlobal({newCompartment: true});
g.eval(`
  function* f() {
    yield 1;
  }
`);

let dbg = new Debugger(g);
let steps = 0;
let uncaughtErrorsReported = 0;
dbg.onEnterFrame = frame => {
  assertEq(frame.callee.name, "f");
  dbg.onEnterFrame = undefined;
  frame.onStep = () => {
    steps++;

    // This test case never resumes the generator after the initial
    // yield. Therefore the initial yield has not happened yet. So this
    // force-return will be an error.
    return {return: "ponies"};
  };

  // Having an onPop hook exercises some assertions that don't happen
  // otherwise.
  frame.onPop = completion => {};
};

dbg.uncaughtExceptionHook = (reason) => {
  // When onEnterFrame returns an invalid resumption value,
  // the error is reported here.
  assertEq(reason instanceof TypeError, true);
  uncaughtErrorsReported++;
  return undefined;  // Cancel the force-return. Let the debuggee continue.
};

let result = g.f();
assertEq(result instanceof g.f, true);

assertEq(steps > 0, true);
assertEq(uncaughtErrorsReported, steps);
>>>>>>> upstream-releases
