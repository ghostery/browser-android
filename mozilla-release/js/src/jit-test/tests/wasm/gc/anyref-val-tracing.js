// |jit-test| skip-if: !wasmReftypesEnabled()

gczeal(14, 1);
let { exports } = wasmEvalText(`(module
<<<<<<< HEAD
    (gc_feature_opt_in 2)
||||||| merged common ancestors
    (gc_feature_opt_in 1)
=======
>>>>>>> upstream-releases
    (global $anyref (import "glob" "anyref") anyref)
    (func (export "get") (result anyref) global.get $anyref)
)`, {
    glob: {
        anyref: { sentinel: "lol" },
    }
});
assertEq(exports.get().sentinel, "lol");
