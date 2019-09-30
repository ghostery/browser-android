/* Copyright 2017 Mozilla Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//! A simple event-driven library for parsing WebAssembly binary files
//! (or streams).
//!
//! The parser library reports events as they happend and only stores
//! parsing information for a brief period of time, making it very fast
//! and memory-efficient. The event-driven model, however, has some drawbacks.
//! If you need random access to the entire WebAssembly data-structure,
//! this is not the right library for you. You could however, build such
//! a data-structure using this library.

#![cfg_attr(not(feature = "std"), no_std)]

#[cfg(not(feature = "std"))]
#[macro_use]
extern crate alloc as std;
#[cfg(feature = "std")]
#[macro_use]
extern crate std;

#[cfg(not(feature = "std"))]
use hashbrown::HashSet;
#[cfg(feature = "std")]
use std::collections::HashSet;

<<<<<<< HEAD
pub use binary_reader::BinaryReader;
pub use binary_reader::Range;
use binary_reader::SectionHeader;

pub use parser::LocalName;
pub use parser::NameEntry;
pub use parser::Parser;
pub use parser::ParserInput;
pub use parser::ParserState;
pub use parser::RelocEntry;
pub use parser::WasmDecoder;
||||||| merged common ancestors
pub use parser::WasmDecoder;
pub use parser::Parser;
pub use parser::ParserState;
pub use parser::ParserInput;
pub use parser::BinaryReaderError;
pub use parser::BinaryReader;
pub use parser::Result;
pub use parser::Range;
pub use parser::SectionCode;
pub use parser::Operator;
pub use parser::Type;
pub use parser::CustomSectionKind;
pub use parser::NameType;
pub use parser::Naming;
pub use parser::LocalName;
pub use parser::NameEntry;
pub use parser::ExternalKind;
pub use parser::FuncType;
pub use parser::ResizableLimits;
pub use parser::TableType;
pub use parser::MemoryType;
pub use parser::GlobalType;
pub use parser::MemoryImmediate;
pub use parser::BrTable;
pub use parser::ImportSectionEntryType;
pub use parser::RelocType;
pub use parser::RelocEntry;
pub use parser::LinkingType;
pub use parser::Ieee32;
pub use parser::Ieee64;
=======
pub use crate::binary_reader::BinaryReader;
pub use crate::binary_reader::Range;
use crate::binary_reader::SectionHeader;
>>>>>>> upstream-releases

<<<<<<< HEAD
pub use primitives::BinaryReaderError;
pub use primitives::BrTable;
pub use primitives::CustomSectionKind;
pub use primitives::ExternalKind;
pub use primitives::FuncType;
pub use primitives::GlobalType;
pub use primitives::Ieee32;
pub use primitives::Ieee64;
pub use primitives::ImportSectionEntryType;
pub use primitives::LinkingType;
pub use primitives::MemoryImmediate;
pub use primitives::MemoryType;
pub use primitives::NameType;
pub use primitives::Naming;
pub use primitives::Operator;
pub use primitives::RelocType;
pub use primitives::ResizableLimits;
pub use primitives::Result;
pub use primitives::SectionCode;
pub use primitives::TableType;
pub use primitives::Type;

pub use validator::validate;
pub use validator::ValidatingOperatorParser;
pub use validator::ValidatingParser;
pub use validator::WasmModuleResources;
||||||| merged common ancestors
pub use validator::ValidatingParser;
pub use validator::ValidatingOperatorParser;
pub use validator::WasmModuleResources;
pub use validator::validate;
=======
pub use crate::parser::LocalName;
pub use crate::parser::NameEntry;
pub use crate::parser::Parser;
pub use crate::parser::ParserInput;
pub use crate::parser::ParserState;
pub use crate::parser::RelocEntry;
pub use crate::parser::WasmDecoder;
>>>>>>> upstream-releases

<<<<<<< HEAD
pub use readers::CodeSectionReader;
pub use readers::Data;
pub use readers::DataSectionReader;
pub use readers::Element;
pub use readers::ElementItems;
pub use readers::ElementItemsReader;
pub use readers::ElementSectionReader;
pub use readers::Export;
pub use readers::ExportSectionReader;
pub use readers::FunctionBody;
pub use readers::FunctionSectionReader;
pub use readers::Global;
pub use readers::GlobalSectionReader;
pub use readers::Import;
pub use readers::ImportSectionReader;
pub use readers::InitExpr;
pub use readers::LinkingSectionReader;
pub use readers::LocalsReader;
pub use readers::MemorySectionReader;
pub use readers::ModuleReader;
pub use readers::Name;
pub use readers::NameSectionReader;
pub use readers::NamingReader;
pub use readers::OperatorsReader;
pub use readers::Reloc;
pub use readers::RelocSectionReader;
pub use readers::Section;
pub use readers::SectionIterator;
pub use readers::SectionIteratorLimited;
pub use readers::SectionReader;
pub use readers::SectionWithLimitedItems;
pub use readers::TableSectionReader;
pub use readers::TypeSectionReader;

mod binary_reader;
||||||| merged common ancestors
mod parser;
mod validator;
=======
pub use crate::primitives::BinaryReaderError;
pub use crate::primitives::BrTable;
pub use crate::primitives::CustomSectionKind;
pub use crate::primitives::ExternalKind;
pub use crate::primitives::FuncType;
pub use crate::primitives::GlobalType;
pub use crate::primitives::Ieee32;
pub use crate::primitives::Ieee64;
pub use crate::primitives::ImportSectionEntryType;
pub use crate::primitives::LinkingType;
pub use crate::primitives::MemoryImmediate;
pub use crate::primitives::MemoryType;
pub use crate::primitives::NameType;
pub use crate::primitives::Naming;
pub use crate::primitives::Operator;
pub use crate::primitives::RelocType;
pub use crate::primitives::ResizableLimits;
pub use crate::primitives::Result;
pub use crate::primitives::SectionCode;
pub use crate::primitives::TableType;
pub use crate::primitives::Type;
pub use crate::primitives::TypeOrFuncType;
pub use crate::primitives::V128;

pub use crate::validator::validate;
pub use crate::validator::validate_function_body;
pub use crate::validator::ValidatingOperatorParser;
pub use crate::validator::ValidatingParser;
pub use crate::validator::ValidatingParserConfig;

pub use crate::operators_validator::OperatorValidatorConfig;
pub use crate::operators_validator::WasmModuleResources;

pub use crate::readers::CodeSectionReader;
pub use crate::readers::Data;
pub use crate::readers::DataKind;
pub use crate::readers::DataSectionReader;
pub use crate::readers::Element;
pub use crate::readers::ElementItems;
pub use crate::readers::ElementItemsReader;
pub use crate::readers::ElementKind;
pub use crate::readers::ElementSectionReader;
pub use crate::readers::Export;
pub use crate::readers::ExportSectionReader;
pub use crate::readers::FunctionBody;
pub use crate::readers::FunctionSectionReader;
pub use crate::readers::Global;
pub use crate::readers::GlobalSectionReader;
pub use crate::readers::Import;
pub use crate::readers::ImportSectionReader;
pub use crate::readers::InitExpr;
pub use crate::readers::LinkingSectionReader;
pub use crate::readers::LocalsReader;
pub use crate::readers::MemorySectionReader;
pub use crate::readers::ModuleReader;
pub use crate::readers::Name;
pub use crate::readers::NameSectionReader;
pub use crate::readers::NamingReader;
pub use crate::readers::OperatorsReader;
pub use crate::readers::ProducersField;
pub use crate::readers::ProducersFieldValue;
pub use crate::readers::ProducersSectionReader;
pub use crate::readers::Reloc;
pub use crate::readers::RelocSectionReader;
pub use crate::readers::Section;
pub use crate::readers::SectionIterator;
pub use crate::readers::SectionIteratorLimited;
pub use crate::readers::SectionReader;
pub use crate::readers::SectionWithLimitedItems;
pub use crate::readers::TableSectionReader;
pub use crate::readers::TypeSectionReader;

mod binary_reader;
>>>>>>> upstream-releases
mod limits;
<<<<<<< HEAD
mod parser;
mod primitives;
mod readers;
||||||| merged common ancestors
=======
mod operators_validator;
mod parser;
mod primitives;
mod readers;
>>>>>>> upstream-releases
mod tests;
<<<<<<< HEAD
mod validator;

#[cfg(not(feature = "std"))]
mod std {
    pub use alloc::{boxed, vec};
    pub use core::*;
    pub mod collections {
        pub use hashmap_core::HashSet;
    }
}
||||||| merged common ancestors

#[cfg(not(feature = "std"))]
mod std {
    pub use core::*;
    pub use alloc::vec;
    pub mod collections {
        pub use hashmap_core::HashSet;
    }
}
=======
mod validator;
>>>>>>> upstream-releases
