// |jit-test| skip-if: !wasmReftypesEnabled() || !wasmDebuggingIsSupported()

(function() {
    let g = newGlobal({newCompartment: true});
    let dbg = new Debugger(g);
<<<<<<< HEAD
    g.eval(`o = new WebAssembly.Instance(new WebAssembly.Module(wasmTextToBinary('(module (gc_feature_opt_in 2) (func (result anyref) (param anyref) get_local 0) (export "" 0))')));`);
||||||| merged common ancestors
    g.eval(`o = new WebAssembly.Instance(new WebAssembly.Module(wasmTextToBinary('(module (gc_feature_opt_in 1) (func (result anyref) (param anyref) get_local 0) (export "" 0))')));`);
=======
    g.eval(`o = new WebAssembly.Instance(new WebAssembly.Module(wasmTextToBinary('(module (func (result anyref) (param anyref) local.get 0) (export "" 0))')));`);
>>>>>>> upstream-releases
})();

(function() {
    var g = newGlobal({newCompartment: true});
    g.parent = this;

    let src = `
      (module
<<<<<<< HEAD
        (gc_feature_opt_in 2)
||||||| merged common ancestors
        (gc_feature_opt_in 1)
=======
>>>>>>> upstream-releases
        (func (export "func") (result anyref) (param $ref anyref)
            local.get $ref
        )
      )
    `;

    g.eval(`
        var obj = { somekey: 'somevalue' };
        Debugger(parent).onEnterFrame = function(frame) {
            let v = frame.environment.getVariable('var0');
            assertEq(typeof v, 'object');

            let prop = v.unwrap().getOwnPropertyDescriptor('somekey');
            assertEq(typeof prop, 'object');
            assertEq(typeof prop.value, 'string');
            assertEq(prop.value, 'somevalue');

            // Disable onEnterFrame hook.
            Debugger(parent).onEnterFrame = undefined;
        };
    `);

    new WebAssembly.Instance(new WebAssembly.Module(wasmTextToBinary(`${src}`))).exports.func(g.obj);
})();
