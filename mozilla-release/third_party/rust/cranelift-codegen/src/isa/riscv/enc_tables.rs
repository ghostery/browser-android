//! Encoding tables for RISC-V.

use super::registers::*;
<<<<<<< HEAD
use ir;
use isa;
use isa::constraints::*;
use isa::enc_tables::*;
use isa::encoding::{base_size, RecipeSizing};
||||||| merged common ancestors
use ir;
use isa;
use isa::constraints::*;
use isa::enc_tables::*;
use isa::encoding::RecipeSizing;
use predicates;
=======
use crate::ir;
use crate::isa;
use crate::isa::constraints::*;
use crate::isa::enc_tables::*;
use crate::isa::encoding::{base_size, RecipeSizing};
>>>>>>> upstream-releases

// Include the generated encoding tables:
// - `LEVEL1_RV32`
// - `LEVEL1_RV64`
// - `LEVEL2`
// - `ENCLIST`
// - `INFO`
include!(concat!(env!("OUT_DIR"), "/encoding-riscv.rs"));
include!(concat!(env!("OUT_DIR"), "/legalize-riscv.rs"));
