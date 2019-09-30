//! Support for configurable wasm translation.

mod dummy;
mod spec;

<<<<<<< HEAD
pub use environ::dummy::DummyEnvironment;
pub use environ::spec::{
    FuncEnvironment, GlobalVariable, ModuleEnvironment, ReturnMode, WasmError, WasmResult,
||||||| merged common ancestors
pub use environ::dummy::DummyEnvironment;
pub use environ::spec::{
    FuncEnvironment, GlobalVariable, ModuleEnvironment, WasmError, WasmResult,
=======
pub use crate::environ::dummy::DummyEnvironment;
pub use crate::environ::spec::{
    FuncEnvironment, GlobalVariable, ModuleEnvironment, ReturnMode, WasmError, WasmResult,
>>>>>>> upstream-releases
};
