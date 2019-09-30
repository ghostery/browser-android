// |jit-test| skip-if: !wasmGcEnabled()

// Parsing and resolving.

var bin = wasmTextToBinary(
    `(module
<<<<<<< HEAD
      (gc_feature_opt_in 2)
||||||| merged common ancestors
      (gc_feature_opt_in 1)
=======
      (gc_feature_opt_in 3)
>>>>>>> upstream-releases
      (type $cons (struct
                   (field $car i32)
                   (field $cdr (ref $cons))))

      (type $odd (struct
                  (field $odd.x i32)
                  (field $to_even (ref $even))))

      (type $even (struct
                   (field $even.x i32)
                   (field $to_odd (ref $odd))))

      ;; Use anyref on the API since struct types cannot be exposed outside the module yet.

      (import "m" "f" (func $imp (param anyref) (result anyref)))

      ;; The bodies do nothing since we have no operations on structs yet.
      ;; Note none of these functions are exported, as they use Ref types in their signatures.

      (func (param (ref $cons)) (result i32)
       (i32.const 0))

      (func $cdr (param $p (ref $cons)) (result (ref $cons))
       (local $l (ref $cons))
       ;; store null value of correct type
<<<<<<< HEAD
       (set_local $l (ref.null))
||||||| merged common ancestors
       (set_local $l (ref.null (ref $cons)))
=======
       (local.set $l (ref.null))
>>>>>>> upstream-releases
       ;; store local of correct type
       (local.set $l (local.get $p))
       ;; store call result of correct type
       (local.set $l (call $cdr (local.get $p)))
       ;; TODO: eventually also a test with global.get
       ;; blocks and if with result type
       (block (ref $cons)
        (if (ref $cons) (i32.eqz (i32.const 0))
            (unreachable)
            (ref.null))))

      (func (param (ref $even)) (result (ref $odd))
       (ref.null))

      (func (param (ref $odd)) (result (ref $even))
       (ref.null))

      (func (param (ref $cons))
       (call $cdr (local.get 0))
       drop
       (call $imp (local.get 0))
       drop)

      (func (param (ref $cons))
<<<<<<< HEAD
       (drop (ref.eq (get_local 0) (ref.null)))
       (drop (ref.eq (ref.null) (get_local 0)))
       (drop (ref.eq (get_local 0) (ref.null)))
       (drop (ref.eq (ref.null) (get_local 0))))
||||||| merged common ancestors
       (drop (ref.eq (get_local 0) (ref.null (ref $cons))))
       (drop (ref.eq (ref.null (ref $cons)) (get_local 0)))
       (drop (ref.eq (get_local 0) (ref.null anyref)))
       (drop (ref.eq (ref.null anyref) (get_local 0))))
=======
       (drop (ref.eq (local.get 0) (ref.null)))
       (drop (ref.eq (ref.null) (local.get 0)))
       (drop (ref.eq (local.get 0) (ref.null)))
       (drop (ref.eq (ref.null) (local.get 0))))
>>>>>>> upstream-releases
     )`);

// Validation

assertEq(WebAssembly.validate(bin), true);

// ref.is_null should work on any reference type

new WebAssembly.Module(wasmTextToBinary(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct))
 (func $null (param (ref $s)) (result i32)
   (ref.is_null (local.get 0))))
`))

// Automatic upcast to anyref

new WebAssembly.Module(wasmTextToBinary(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (func $f (param (ref $s)) (call $g (local.get 0)))
 (func $g (param anyref) (unreachable)))
`));

// Misc failure modes

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (func (param (ref $odd)) (unreachable)))
`),
SyntaxError, /Type label.*not found/);

// Ref type mismatch in parameter is allowed through the prefix rule
// but not if the structs are incompatible.

wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field i32)))
 (func $f (param (ref $s)) (unreachable))
 (func $g (param (ref $t)) (call $f (local.get 0)))
)`);

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field f32))) ;; Incompatible type
 (func $f (param (ref $s)) (unreachable))
 (func $g (param (ref $t)) (call $f (local.get 0)))
)`),
WebAssembly.CompileError, /expression has type ref.*but expected ref/);

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field (mut i32)))) ;; Incompatible mutability
 (func $f (param (ref $s)) (unreachable))
 (func $g (param (ref $t)) (call $f (local.get 0)))
)`),
WebAssembly.CompileError, /expression has type ref.*but expected ref/);

// Ref type mismatch in assignment to local but the prefix rule allows
// the assignment to succeed if the structs are the same.

wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field i32)))
 (func $f (param (ref $s)) (local (ref $t)) (local.set 1 (local.get 0))))
`)

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field f32)))
 (func $f (param (ref $s)) (local (ref $t)) (local.set 1 (local.get 0))))
`),
WebAssembly.CompileError, /expression has type ref.*but expected ref/);

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field (mut i32))))
 (func $f (param (ref $s)) (unreachable))
 (func $g (param (ref $t)) (call $f (local.get 0)))
)`),
WebAssembly.CompileError, /expression has type ref.*but expected ref/);

// Ref type mismatch in return but the prefix rule allows the return
// to succeed if the structs are the same.

wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field i32)))
 (func $f (param (ref $s)) (result (ref $t)) (local.get 0)))
`);

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field f32)))
 (func $f (param (ref $s)) (result (ref $t)) (local.get 0)))
`),
WebAssembly.CompileError, /expression has type ref.*but expected ref/);

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (type $t (struct (field (mut i32))))
 (func $f (param (ref $s)) (result (ref $t)) (local.get 0)))
`),
WebAssembly.CompileError, /expression has type ref.*but expected ref/);

// Ref type can't reference a function type

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $x (func (param i32)))
 (func $f (param (ref $x)) (unreachable)))
`),
SyntaxError, /Type label.*not found/);

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type (func (param i32)))
 (func $f (param (ref 0)) (unreachable)))
`),
WebAssembly.CompileError, /does not reference a struct type/);

// No automatic downcast from anyref

assertErrorMessage(() => wasmEvalText(`
(module
<<<<<<< HEAD
 (gc_feature_opt_in 2)
||||||| merged common ancestors
 (gc_feature_opt_in 1)
=======
 (gc_feature_opt_in 3)
>>>>>>> upstream-releases
 (type $s (struct (field i32)))
 (func $f (param anyref) (call $g (local.get 0)))
 (func $g (param (ref $s)) (unreachable)))
`),
WebAssembly.CompileError, /expression has type anyref but expected ref/);
