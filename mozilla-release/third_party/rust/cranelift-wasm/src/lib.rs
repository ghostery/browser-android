//! Performs translation from a wasm module in binary format to the in-memory form
//! of Cranelift IR. More particularly, it translates the code of all the functions bodies and
//! interacts with an environment implementing the
//! [`ModuleEnvironment`](trait.ModuleEnvironment.html)
//! trait to deal with tables, globals and linear memory.
//!
//! The crate provides a `DummyEnvironment` struct that will allow to translate the code of the
//! functions but will fail at execution.
//!
//! The main function of this module is [`translate_module`](fn.translate_module.html).

#![deny(missing_docs, trivial_numeric_casts, unused_extern_crates)]
#![warn(unused_import_braces)]
#![cfg_attr(feature = "std", deny(unstable_features))]
<<<<<<< HEAD
#![cfg_attr(
    feature = "clippy",
    plugin(clippy(conf_file = "../../clippy.toml"))
)]
#![cfg_attr(
    feature = "cargo-clippy",
    allow(new_without_default, new_without_default_derive)
)]
||||||| merged common ancestors
#![cfg_attr(feature = "clippy", plugin(clippy(conf_file = "../../clippy.toml")))]
#![cfg_attr(feature = "cargo-clippy", allow(new_without_default, new_without_default_derive))]
=======
#![cfg_attr(feature = "clippy", plugin(clippy(conf_file = "../../clippy.toml")))]
#![cfg_attr(feature = "cargo-clippy", allow(clippy::new_without_default))]
>>>>>>> upstream-releases
#![cfg_attr(
    feature = "cargo-clippy",
    warn(
<<<<<<< HEAD
        float_arithmetic,
        mut_mut,
        nonminimal_bool,
        option_map_unwrap_or,
        option_map_unwrap_or_else,
        print_stdout,
        unicode_not_nfc,
        use_self
||||||| merged common ancestors
        float_arithmetic, mut_mut, nonminimal_bool, option_map_unwrap_or, option_map_unwrap_or_else,
        print_stdout, unicode_not_nfc, use_self
=======
        clippy::float_arithmetic,
        clippy::mut_mut,
        clippy::nonminimal_bool,
        clippy::option_map_unwrap_or,
        clippy::option_map_unwrap_or_else,
        clippy::print_stdout,
        clippy::unicode_not_nfc,
        clippy::use_self
>>>>>>> upstream-releases
    )
)]
#![no_std]
#![cfg_attr(not(feature = "std"), feature(alloc))]

#[cfg(not(feature = "std"))]
#[macro_use]
<<<<<<< HEAD
extern crate cranelift_entity;
extern crate cranelift_frontend;
#[cfg(test)]
extern crate target_lexicon;
extern crate wasmparser;

extern crate failure;
||||||| merged common ancestors
extern crate cranelift_entity;
extern crate cranelift_frontend;
extern crate target_lexicon;
extern crate wasmparser;

extern crate failure;
=======
extern crate alloc as std;
#[cfg(feature = "std")]
>>>>>>> upstream-releases
#[macro_use]
extern crate std;

#[cfg(not(feature = "std"))]
use hashmap_core::{
    hash_map::Entry::{Occupied, Vacant},
    map as hash_map, HashMap,
};
#[cfg(feature = "std")]
use std::collections::{
    hash_map,
    hash_map::Entry::{Occupied, Vacant},
    HashMap,
};

mod code_translator;
mod environ;
mod func_translator;
mod module_translator;
mod sections_translator;
mod state;
mod translation_utils;

<<<<<<< HEAD
pub use environ::{
    DummyEnvironment, FuncEnvironment, GlobalVariable, ModuleEnvironment, ReturnMode, WasmError,
    WasmResult,
||||||| merged common ancestors
pub use environ::{
    DummyEnvironment, FuncEnvironment, GlobalVariable, ModuleEnvironment, WasmError, WasmResult,
=======
pub use crate::environ::{
    DummyEnvironment, FuncEnvironment, GlobalVariable, ModuleEnvironment, ReturnMode, WasmError,
    WasmResult,
>>>>>>> upstream-releases
};
<<<<<<< HEAD
pub use func_translator::FuncTranslator;
pub use module_translator::translate_module;
pub use translation_utils::{
    DefinedFuncIndex, DefinedGlobalIndex, DefinedMemoryIndex, DefinedTableIndex, FuncIndex, Global,
    GlobalIndex, GlobalInit, Memory, MemoryIndex, SignatureIndex, Table, TableIndex,
||||||| merged common ancestors
pub use func_translator::FuncTranslator;
pub use module_translator::translate_module;
pub use translation_utils::{
    DefinedFuncIndex, FuncIndex, Global, GlobalIndex, GlobalInit, Memory, MemoryIndex,
    SignatureIndex, Table, TableIndex,
=======
pub use crate::func_translator::FuncTranslator;
pub use crate::module_translator::translate_module;
pub use crate::translation_utils::{
    get_vmctx_value_label, DefinedFuncIndex, DefinedGlobalIndex, DefinedMemoryIndex,
    DefinedTableIndex, FuncIndex, Global, GlobalIndex, GlobalInit, Memory, MemoryIndex,
    SignatureIndex, Table, TableElementType, TableIndex,
>>>>>>> upstream-releases
};

<<<<<<< HEAD
#[cfg(not(feature = "std"))]
mod std {
    extern crate alloc;

    pub use self::alloc::string;
    pub use self::alloc::vec;
    pub use core::convert;
    pub use core::fmt;
    pub use core::option;
    pub use core::{cmp, i32, str, u32};
    pub mod collections {
        #[allow(unused_extern_crates)]
        extern crate hashmap_core;

        pub use self::hashmap_core::{map as hash_map, HashMap};
    }
}
||||||| merged common ancestors
#[cfg(not(feature = "std"))]
mod std {
    extern crate alloc;

    pub use self::alloc::string;
    pub use self::alloc::vec;
    pub use core::fmt;
    pub use core::option;
    pub use core::{cmp, i32, str, u32};
    pub mod collections {
        #[allow(unused_extern_crates)]
        extern crate hashmap_core;

        pub use self::hashmap_core::{map as hash_map, HashMap};
    }
}
=======
/// Version number of this crate.
pub const VERSION: &str = env!("CARGO_PKG_VERSION");
>>>>>>> upstream-releases
