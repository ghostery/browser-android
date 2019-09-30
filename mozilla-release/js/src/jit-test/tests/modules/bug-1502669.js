<<<<<<< HEAD
// |jit-test| error: ReferenceError
var g = newGlobal();
g.parent = this;
g.eval("new Debugger(parent).onExceptionUnwind = function () { hits++; };");
import('')();
||||||| merged common ancestors
=======
// |jit-test| error: ReferenceError
var g = newGlobal({newCompartment: true});
g.parent = this;
g.eval("new Debugger(parent).onExceptionUnwind = function () { hits++; };");
import('')();
>>>>>>> upstream-releases
