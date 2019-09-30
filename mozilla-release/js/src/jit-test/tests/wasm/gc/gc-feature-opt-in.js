// |jit-test| skip-if: !wasmReftypesEnabled()
//
// Also see gc-feature-opt-in-struct.js for tests that use the struct feature.

// Version numbers

let CURRENT_VERSION = 3;
let SOME_OLDER_INCOMPATIBLE_VERSION = CURRENT_VERSION - 1;  // v1 incompatible with v2, v2 with v3
let SOME_FUTURE_VERSION = CURRENT_VERSION + 1;              // ok for now

// Version numbers

let CURRENT_VERSION = 2;
let OLDER_INCOMPATIBLE_VERSION = 1; // v1 incompatible with v2
let FUTURE_VERSION = 3;

// Encoding.  If the section is present it must be first.

var bad_order =
    new Uint8Array([0x00, 0x61, 0x73, 0x6d,
                    0x01, 0x00, 0x00, 0x00,

                    0x01,                   // Type section
                    0x01,                   // Section size
                    0x00,                   // Zero types

                    0x2a,                   // GcFeatureOptIn section
                    0x01,                   // Section size
                    CURRENT_VERSION]);      // Version

assertErrorMessage(() => new WebAssembly.Module(bad_order),
                   WebAssembly.CompileError,
                   /expected custom section/);

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in ${CURRENT_VERSION}))`));

new WebAssembly.Module(wasmTextToBinary(
    `(module
<<<<<<< HEAD
      (gc_feature_opt_in ${CURRENT_VERSION}))`));

assertErrorMessage(() => new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in ${OLDER_INCOMPATIBLE_VERSION}))`)),
                   WebAssembly.CompileError,
                   /GC feature version \d+ is no longer supported by this engine/);
||||||| merged common ancestors
      (gc_feature_opt_in 1))`));
=======
      (gc_feature_opt_in ${CURRENT_VERSION}))`));
>>>>>>> upstream-releases

assertErrorMessage(() => new WebAssembly.Module(wasmTextToBinary(
    `(module
<<<<<<< HEAD
      (gc_feature_opt_in ${FUTURE_VERSION}))`)),
||||||| merged common ancestors
      (gc_feature_opt_in 2))`)),
=======
      (gc_feature_opt_in ${SOME_OLDER_INCOMPATIBLE_VERSION}))`)),
>>>>>>> upstream-releases
                   WebAssembly.CompileError,
<<<<<<< HEAD
                   /GC feature version is unknown/);

// Struct types are only available if we opt in.

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in ${CURRENT_VERSION})
      (type (struct (field i32))))`));
||||||| merged common ancestors
                   /unsupported version of the gc feature/);

// Struct types are only available if we opt in.

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in 1)
      (type (struct (field i32))))`));
=======
                   /GC feature version.*no longer supported by this engine/);
>>>>>>> upstream-releases

assertErrorMessage(() => new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in ${SOME_FUTURE_VERSION}))`)),
                   WebAssembly.CompileError,
                   /GC feature version is unknown/);

// Parameters of anyref type are available regardless of whether we opt in.

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in ${CURRENT_VERSION})
      (type (func (param anyref))))`));

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (type (func (param anyref))))`));

// Ditto returns

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in ${CURRENT_VERSION})
      (type (func (result anyref))))`));

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (type (func (result anyref))))`));

// Ditto locals

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in ${CURRENT_VERSION})
      (func (result i32)
       (local anyref)
       (i32.const 0)))`));

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (func (result i32)
       (local anyref)
       (i32.const 0)))`));

// Ditto globals

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (gc_feature_opt_in ${CURRENT_VERSION})
      (global (mut anyref) (ref.null)))`));

new WebAssembly.Module(wasmTextToBinary(
    `(module
<<<<<<< HEAD
      (global (mut anyref) (ref.null)))`)),
                   WebAssembly.CompileError,
                   /reference types not enabled/);
||||||| merged common ancestors
      (global (mut anyref) (ref.null anyref)))`)),
                   WebAssembly.CompileError,
                   /reference types not enabled/);
=======
      (global (mut anyref) (ref.null)))`));
>>>>>>> upstream-releases

// ref.null and ref.is_null are available whetehr we opt in or not, but ref.eq
// only if we opt in
//
// When testing these we need to avoid struct types or parameters, locals,
// returns, or globals of ref type, or guards on those will preempt the guards
// on the instructions.

new WebAssembly.Module(wasmTextToBinary(
    `(module
<<<<<<< HEAD
      (func ref.null))`)),
                   WebAssembly.CompileError,
                   /unrecognized opcode/);
||||||| merged common ancestors
      (func (ref.null anyref)))`)),
                   WebAssembly.CompileError,
                   /unrecognized opcode/);
=======
      (func (result anyref) ref.null))`));
>>>>>>> upstream-releases

new WebAssembly.Module(wasmTextToBinary(
    `(module
      (func (param anyref) (result i32) (ref.is_null (local.get 0))))`));

assertErrorMessage(() => new WebAssembly.Module(wasmTextToBinary(
    `(module
      (func ref.eq))`)),
                   WebAssembly.CompileError,
                   /unrecognized opcode/);

