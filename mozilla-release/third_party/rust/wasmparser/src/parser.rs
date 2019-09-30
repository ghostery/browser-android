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
// See https://github.com/WebAssembly/design/blob/master/BinaryEncoding.md

use std::boxed::Box;
use std::vec::Vec;

<<<<<<< HEAD
use limits::{
    MAX_WASM_FUNCTIONS, MAX_WASM_FUNCTION_LOCALS, MAX_WASM_STRING_SIZE, MAX_WASM_TABLE_ENTRIES,
};
||||||| merged common ancestors
use limits::{MAX_WASM_FUNCTION_LOCALS, MAX_WASM_FUNCTION_PARAMS, MAX_WASM_FUNCTION_RETURNS,
             MAX_WASM_FUNCTION_SIZE, MAX_WASM_STRING_SIZE, MAX_WASM_FUNCTIONS,
             MAX_WASM_TABLE_ENTRIES};
=======
use crate::limits::{
    MAX_WASM_FUNCTIONS, MAX_WASM_FUNCTION_LOCALS, MAX_WASM_STRING_SIZE, MAX_WASM_TABLE_ENTRIES,
};
>>>>>>> upstream-releases

<<<<<<< HEAD
use primitives::{
    BinaryReaderError, CustomSectionKind, ExternalKind, FuncType, GlobalType,
    ImportSectionEntryType, LinkingType, MemoryType, Naming, Operator, RelocType, Result,
    SectionCode, TableType, Type,
};
||||||| merged common ancestors
const MAX_WASM_BR_TABLE_SIZE: usize = MAX_WASM_FUNCTION_SIZE;
=======
use crate::primitives::{
    BinaryReaderError, CustomSectionKind, ExternalKind, FuncType, GlobalType,
    ImportSectionEntryType, LinkingType, MemoryType, Naming, Operator, RelocType, Result,
    SectionCode, TableType, Type,
};
>>>>>>> upstream-releases

<<<<<<< HEAD
use readers::{
    CodeSectionReader, Data, DataSectionReader, Element, ElementItems, ElementSectionReader,
    Export, ExportSectionReader, FunctionBody, FunctionSectionReader, Global, GlobalSectionReader,
    Import, ImportSectionReader, LinkingSectionReader, MemorySectionReader, ModuleReader, Name,
    NameSectionReader, NamingReader, OperatorsReader, Reloc, RelocSectionReader, Section,
    SectionReader, TableSectionReader, TypeSectionReader,
};
||||||| merged common ancestors
const MAX_DATA_CHUNK_SIZE: usize = MAX_WASM_STRING_SIZE;

#[derive(Debug,Copy,Clone)]
pub struct BinaryReaderError {
    pub message: &'static str,
    pub offset: usize,
}

pub type Result<T> = result::Result<T, BinaryReaderError>;

#[derive(Debug,Copy,Clone,PartialEq,Eq,PartialOrd,Ord)]
pub enum CustomSectionKind {
    Unknown,
    Name,
    SourceMappingURL,
    Reloc,
    Linking,
}

/// Section code as defined [here].
///
/// [here]: https://webassembly.github.io/spec/binary/modules.html#sections
#[derive(Debug,Copy,Clone,PartialEq,Eq,PartialOrd,Ord)]
pub enum SectionCode<'a> {
    Custom {
        name: &'a [u8],
        kind: CustomSectionKind,
    },
    Type, // Function signature declarations
    Import, // Import declarations
    Function, // Function declarations
    Table, // Indirect function table and other tables
    Memory, // Memory attributes
    Global, // Global declarations
    Export, // Exports
    Start, // Start function declaration
    Element, // Elements section
    Code, // Function bodies (code)
    Data, // Data segments
}

/// Types as defined [here].
///
/// [here]: https://webassembly.github.io/spec/syntax/types.html#types
#[derive(Debug,Copy,Clone,PartialEq,Eq)]
pub enum Type {
    I32,
    I64,
    F32,
    F64,
    AnyFunc,
    Func,
    EmptyBlockType,
}
=======
use crate::readers::{
    CodeSectionReader, Data, DataKind, DataSectionReader, Element, ElementItems, ElementKind,
    ElementSectionReader, Export, ExportSectionReader, FunctionBody, FunctionSectionReader, Global,
    GlobalSectionReader, Import, ImportSectionReader, LinkingSectionReader, MemorySectionReader,
    ModuleReader, Name, NameSectionReader, NamingReader, OperatorsReader, Reloc,
    RelocSectionReader, Section, SectionReader, TableSectionReader, TypeSectionReader,
};

use crate::binary_reader::{BinaryReader, Range};
>>>>>>> upstream-releases

<<<<<<< HEAD
use binary_reader::{BinaryReader, Range};

const MAX_DATA_CHUNK_SIZE: usize = MAX_WASM_STRING_SIZE;
||||||| merged common ancestors
#[derive(Debug)]
pub enum NameType {
    Module,
    Function,
    Local,
}

#[derive(Debug)]
pub struct Naming<'a> {
    pub index: u32,
    pub name: &'a [u8],
}
=======
const MAX_DATA_CHUNK_SIZE: usize = MAX_WASM_STRING_SIZE;
>>>>>>> upstream-releases

#[derive(Debug)]
pub struct LocalName<'a> {
    pub index: u32,
    pub locals: Box<[Naming<'a>]>,
}

#[derive(Debug)]
pub enum NameEntry<'a> {
<<<<<<< HEAD
    Module(&'a [u8]),
    Function(Box<[Naming<'a>]>),
    Local(Box<[LocalName<'a>]>),
||||||| merged common ancestors
    Module(&'a [u8]),
    Function(Vec<Naming<'a>>),
    Local(Vec<LocalName<'a>>),
}

/// External types as defined [here].
///
/// [here]: https://webassembly.github.io/spec/syntax/types.html#external-types
#[derive(Debug, Copy, Clone)]
pub enum ExternalKind {
    Function,
    Table,
    Memory,
    Global,
}

#[derive(Debug,Clone)]
pub struct FuncType {
    pub form: Type,
    pub params: Vec<Type>,
    pub returns: Vec<Type>,
}

#[derive(Debug,Copy,Clone)]
pub struct ResizableLimits {
    pub initial: u32,
    pub maximum: Option<u32>,
}

#[derive(Debug,Copy,Clone)]
pub struct TableType {
    pub element_type: Type,
    pub limits: ResizableLimits,
}

#[derive(Debug,Copy,Clone)]
pub struct MemoryType {
    pub limits: ResizableLimits,
    pub shared: bool,
}

#[derive(Debug,Copy,Clone)]
pub struct GlobalType {
    pub content_type: Type,
    pub mutable: bool,
}

#[derive(Debug)]
pub struct MemoryImmediate {
    pub flags: u32,
    pub offset: u32,
}

/// A br_table entries representation.
#[derive(Debug)]
pub struct BrTable<'a> {
    buffer: &'a [u8],
}

impl<'a> BrTable<'a> {
    /// Reads br_table entries.
    ///
    /// # Examples
    /// ```rust
    /// let buf = vec![0x0e, 0x02, 0x01, 0x02, 0x00];
    /// let mut reader = wasmparser::BinaryReader::new(&buf);
    /// let op = reader.read_operator().unwrap();
    /// if let wasmparser::Operator::BrTable { ref table } = op {
    ///     let br_table_depths = table.read_table();
    ///     assert!(br_table_depths.0 == vec![1,2] &&
    ///             br_table_depths.1 == 0);
    /// } else {
    ///     unreachable!();
    /// }
    /// ```
    pub fn read_table(&self) -> (Vec<u32>, u32) {
        let mut reader = BinaryReader::new(self.buffer);
        let mut table = Vec::new();
        while !reader.eof() {
            table.push(reader.read_var_u32().unwrap());
        }
        let default_target = table.pop().unwrap();
        (table, default_target)
    }
}

/// Iterator for `BrTable`.
///
/// #Examples
/// ```rust
/// let buf = vec![0x0e, 0x02, 0x01, 0x02, 0x00];
/// let mut reader = wasmparser::BinaryReader::new(&buf);
/// let op = reader.read_operator().unwrap();
/// if let wasmparser::Operator::BrTable { ref table } = op {
///     for depth in table {
///         println!("BrTable depth: {}", depth);
///     }
/// }
/// ```
pub struct BrTableIterator<'a> {
    reader: BinaryReader<'a>,
}

impl<'a> IntoIterator for &'a BrTable<'a> {
    type Item = u32;
    type IntoIter = BrTableIterator<'a>;

    fn into_iter(self) -> Self::IntoIter {
        BrTableIterator { reader: BinaryReader::new(self.buffer) }
    }
}

impl<'a> Iterator for BrTableIterator<'a> {
    type Item = u32;

    fn next(&mut self) -> Option<u32> {
        if self.reader.eof() {
            return None;
        }
        Some(self.reader.read_var_u32().unwrap())
    }
}

#[derive(Debug)]
pub enum ImportSectionEntryType {
    Function(u32),
    Table(TableType),
    Memory(MemoryType),
    Global(GlobalType),
}

#[derive(Debug)]
pub enum RelocType {
    FunctionIndexLEB,
    TableIndexSLEB,
    TableIndexI32,
    GlobalAddrLEB,
    GlobalAddrSLEB,
    GlobalAddrI32,
    TypeIndexLEB,
    GlobalIndexLEB,
}

#[derive(Debug)]
pub enum LinkingType {
    StackPointer(u32),
=======
    Module(&'a str),
    Function(Box<[Naming<'a>]>),
    Local(Box<[LocalName<'a>]>),
>>>>>>> upstream-releases
}

#[derive(Debug)]
pub struct RelocEntry {
    pub ty: RelocType,
    pub offset: u32,
    pub index: u32,
    pub addend: Option<u32>,
}

enum InitExpressionContinuation {
    GlobalSection,
    ElementSection,
    DataSection,
}

#[derive(Debug)]
pub enum ParserState<'a> {
    Error(BinaryReaderError),
    Initial,
    BeginWasm {
        version: u32,
    },
    EndWasm,
    BeginSection {
        code: SectionCode<'a>,
        range: Range,
    },
    EndSection,
    SkippingSection,
    ReadingCustomSection(CustomSectionKind),
    ReadingSectionRawData,
    SectionRawData(&'a [u8]),

    TypeSectionEntry(FuncType),
    ImportSectionEntry {
        module: &'a str,
        field: &'a str,
        ty: ImportSectionEntryType,
    },
    FunctionSectionEntry(u32),
    TableSectionEntry(TableType),
    MemorySectionEntry(MemoryType),
    ExportSectionEntry {
        field: &'a str,
        kind: ExternalKind,
        index: u32,
    },
    NameSectionEntry(NameEntry<'a>),
    StartSectionEntry(u32),
    DataCountSectionEntry(u32),

    BeginInitExpressionBody,
    InitExpressionOperator(Operator<'a>),
    EndInitExpressionBody,

    BeginFunctionBody {
        range: Range,
    },
    FunctionBodyLocals {
        locals: Box<[(u32, Type)]>,
    },
    CodeOperator(Operator<'a>),
    EndFunctionBody,
    SkippingFunctionBody,

<<<<<<< HEAD
    BeginElementSectionEntry(u32),
    ElementSectionEntryBody(Box<[u32]>),
||||||| merged common ancestors
    BeginElementSectionEntry(u32),
    ElementSectionEntryBody(Vec<u32>),
=======
    BeginPassiveElementSectionEntry(Type),
    BeginActiveElementSectionEntry(u32),
    ElementSectionEntryBody(Box<[u32]>),
>>>>>>> upstream-releases
    EndElementSectionEntry,

    BeginPassiveDataSectionEntry,
    BeginActiveDataSectionEntry(u32),
    EndDataSectionEntry,
    BeginDataSectionEntryBody(u32),
    DataSectionEntryBodyChunk(&'a [u8]),
    EndDataSectionEntryBody,

    BeginGlobalSectionEntry(GlobalType),
    EndGlobalSectionEntry,

    RelocSectionHeader(SectionCode<'a>),
    RelocSectionEntry(RelocEntry),
    LinkingSectionEntry(LinkingType),

    SourceMappingURL(&'a str),
}

#[derive(Debug, Copy, Clone)]
pub enum ParserInput {
    Default,
    SkipSection,
    SkipFunctionBody,
    ReadCustomSection,
    ReadSectionRawData,
}

pub trait WasmDecoder<'a> {
    fn read(&mut self) -> &ParserState<'a>;
    fn push_input(&mut self, input: ParserInput);
    fn read_with_input(&mut self, input: ParserInput) -> &ParserState<'a>;
    fn create_binary_reader<'b>(&mut self) -> BinaryReader<'b>
    where
        'a: 'b;
    fn last_state(&self) -> &ParserState<'a>;
}

enum ParserSectionReader<'a> {
    None,
    CodeSectionReader(CodeSectionReader<'a>),
    DataSectionReader(DataSectionReader<'a>),
    ElementSectionReader(ElementSectionReader<'a>),
    ExportSectionReader(ExportSectionReader<'a>),
    FunctionSectionReader(FunctionSectionReader<'a>),
    GlobalSectionReader(GlobalSectionReader<'a>),
    ImportSectionReader(ImportSectionReader<'a>),
    MemorySectionReader(MemorySectionReader<'a>),
    TableSectionReader(TableSectionReader<'a>),
    TypeSectionReader(TypeSectionReader<'a>),
    NameSectionReader(NameSectionReader<'a>),
    LinkingSectionReader(LinkingSectionReader<'a>),
    RelocSectionReader(RelocSectionReader<'a>),
}

macro_rules! section_reader {
    ($self:ident, $ty_and_name:ident) => {
        if let ParserSectionReader::$ty_and_name(ref mut reader) = $self.section_reader {
            reader
        } else {
            panic!("expected {} reader", stringify!($ty_and_name));
        }
    };
}

macro_rules! start_section_reader {
    ($self:ident, $ty_and_name:ident, $factory:ident) => {{
        let reader = $self
            .current_section
            .as_ref()
            .expect("section")
            .$factory()?;
        $self.section_entries_left = reader.get_count();
        $self.section_reader = ParserSectionReader::$ty_and_name(reader);
    }};
}

/// The `Parser` type. A simple event-driven parser of WebAssembly binary
/// format. The `read(&mut self)` is used to iterate through WebAssembly records.
pub struct Parser<'a> {
    data: &'a [u8],
    state: ParserState<'a>,
    module_reader: Option<ModuleReader<'a>>,
    current_section: Option<Section<'a>>,
    section_reader: ParserSectionReader<'a>,
    element_items: Option<ElementItems<'a>>,
    current_function_body: Option<FunctionBody<'a>>,
    init_expr_continuation: Option<InitExpressionContinuation>,
    current_data_segment: Option<&'a [u8]>,
    binary_reader: Option<BinaryReader<'a>>,
    operators_reader: Option<OperatorsReader<'a>>,
    section_entries_left: u32,
}

impl<'a> Parser<'a> {
    /// Constructs `Parser` type.
    ///
    /// # Examples
    /// ```
    /// let data: &[u8] = &[0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00,
    ///     0x01, 0x4, 0x01, 0x60, 0x00, 0x00, 0x03, 0x02, 0x01, 0x00,
    ///     0x0a, 0x05, 0x01, 0x03, 0x00, 0x01, 0x0b];
    /// let mut parser = wasmparser::Parser::new(data);
    /// ```
    pub fn new(data: &[u8]) -> Parser {
        Parser {
            data,
            state: ParserState::Initial,
            module_reader: None,
            current_section: None,
            section_reader: ParserSectionReader::None,
            element_items: None,
            current_function_body: None,
            init_expr_continuation: None,
            current_data_segment: None,
            binary_reader: None,
            operators_reader: None,
            section_entries_left: 0,
        }
    }

    pub fn eof(&self) -> bool {
        match self.state {
            ParserState::EndWasm => true,
            ParserState::BeginWasm { .. } | ParserState::EndSection => {
                self.module_reader.as_ref().expect("module reader").eof()
            }
            _ => false, // in-process of reading
        }
    }

    pub fn current_position(&self) -> usize {
<<<<<<< HEAD
        if let ParserState::Initial = self.state {
            return 0;
        }
        if self.binary_reader.is_some() {
            return self
                .binary_reader
                .as_ref()
                .expect("binary reader")
                .original_position();
        }
        if self.operators_reader.is_some() {
            return self
                .operators_reader
                .as_ref()
                .expect("operators reader")
                .original_position();
        }
        match self.section_reader {
            ParserSectionReader::CodeSectionReader(ref reader) => return reader.original_position(),
            ParserSectionReader::DataSectionReader(ref reader) => return reader.original_position(),
            ParserSectionReader::ElementSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::ExportSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::FunctionSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::GlobalSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::ImportSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::MemorySectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::TableSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::TypeSectionReader(ref reader) => return reader.original_position(),
            ParserSectionReader::NameSectionReader(ref reader) => return reader.original_position(),
            ParserSectionReader::LinkingSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::RelocSectionReader(ref reader) => {
                return reader.original_position()
            }
            _ => (),
        };
        // TODO might not cover all cases
        self.module_reader
            .as_ref()
            .expect("module reader")
            .current_position()
    }

    fn read_module(&mut self) -> Result<()> {
        let module_reader = ModuleReader::new(self.data)?;
        let version = module_reader.get_version();
        self.module_reader = Some(module_reader);
        self.state = ParserState::BeginWasm { version };
||||||| merged common ancestors
        self.reader.current_position()
    }

    fn read_header(&mut self) -> Result<()> {
        let magic_number = self.reader.read_u32()?;
        if magic_number != WASM_MAGIC_NUMBER {
            return Err(BinaryReaderError {
                           message: "Bad magic number",
                           offset: self.reader.position - 4,
                       });
        }
        let version = self.reader.read_u32()?;
        if version != WASM_SUPPORTED_VERSION && version != WASM_EXPERIMENTAL_VERSION {
            return Err(BinaryReaderError {
                           message: "Bad version number",
                           offset: self.reader.position - 4,
                       });
        }
        self.state = ParserState::BeginWasm { version: version };
=======
        if let ParserState::Initial = self.state {
            return 0;
        }
        if self.binary_reader.is_some() {
            return self
                .binary_reader
                .as_ref()
                .expect("binary reader")
                .original_position();
        }
        if self.operators_reader.is_some() {
            return self
                .operators_reader
                .as_ref()
                .expect("operators reader")
                .original_position();
        }
        match self.section_reader {
            ParserSectionReader::CodeSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::DataSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::ElementSectionReader(ref reader) => {
                return reader.original_position();
            }
            ParserSectionReader::ExportSectionReader(ref reader) => {
                return reader.original_position();
            }
            ParserSectionReader::FunctionSectionReader(ref reader) => {
                return reader.original_position();
            }
            ParserSectionReader::GlobalSectionReader(ref reader) => {
                return reader.original_position();
            }
            ParserSectionReader::ImportSectionReader(ref reader) => {
                return reader.original_position();
            }
            ParserSectionReader::MemorySectionReader(ref reader) => {
                return reader.original_position();
            }
            ParserSectionReader::TableSectionReader(ref reader) => {
                return reader.original_position();
            }
            ParserSectionReader::TypeSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::NameSectionReader(ref reader) => {
                return reader.original_position()
            }
            ParserSectionReader::LinkingSectionReader(ref reader) => {
                return reader.original_position();
            }
            ParserSectionReader::RelocSectionReader(ref reader) => {
                return reader.original_position();
            }
            _ => (),
        };
        // TODO might not cover all cases
        self.module_reader
            .as_ref()
            .expect("module reader")
            .current_position()
    }

    fn read_module(&mut self) -> Result<()> {
        let module_reader = ModuleReader::new(self.data)?;
        let version = module_reader.get_version();
        self.module_reader = Some(module_reader);
        self.state = ParserState::BeginWasm { version };
>>>>>>> upstream-releases
        Ok(())
    }

    fn read_section_header(&mut self) -> Result<()> {
<<<<<<< HEAD
        let section = self.module_reader.as_mut().expect("module reader").read()?;
        let code = section.code;
        let range = section.get_range();
        self.current_section = Some(section);
||||||| merged common ancestors
        let id_position = self.reader.position;
        let id = self.reader.read_var_u7()?;
        let payload_len = self.reader.read_var_u32()? as usize;
        let payload_end = self.reader.position + payload_len;
        let code = self.reader.read_section_code(id, id_position)?;
        if self.reader.buffer.len() < payload_end {
            return Err(BinaryReaderError {
                           message: "Section body extends past end of file",
                           offset: self.reader.buffer.len(),
                       });
        }
        if self.reader.position > payload_end {
            return Err(BinaryReaderError {
                           message: "Section header is too big to fit into section body",
                           offset: payload_end,
                       });
        }
        let range = Range {
            start: self.reader.position,
            end: payload_end,
        };
=======
        let section = self.module_reader.as_mut().expect("module reader").read()?;
        let code = section.code;
        let range = section.range();
        self.current_section = Some(section);
>>>>>>> upstream-releases
        self.state = ParserState::BeginSection { code, range };
        Ok(())
    }

    fn read_type_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            return self.check_section_end();
        }
        let type_entry = section_reader!(self, TypeSectionReader).read()?;
        self.state = ParserState::TypeSectionEntry(type_entry);
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_import_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
<<<<<<< HEAD
            return self.check_section_end();
        }
        let Import { module, field, ty } = section_reader!(self, ImportSectionReader).read()?;
        self.state = ParserState::ImportSectionEntry { module, field, ty };
||||||| merged common ancestors
            return self.position_to_section_end();
        }
        let module = self.reader.read_string()?;
        let field = self.reader.read_string()?;
        let kind = self.reader.read_external_kind()?;
        let ty: ImportSectionEntryType;
        match kind {
            ExternalKind::Function => {
                ty = ImportSectionEntryType::Function(self.reader.read_var_u32()?)
            }
            ExternalKind::Table => {
                ty = ImportSectionEntryType::Table(self.reader.read_table_type()?)
            }
            ExternalKind::Memory => {
                ty = ImportSectionEntryType::Memory(self.reader.read_memory_type()?)
            }
            ExternalKind::Global => {
                ty = ImportSectionEntryType::Global(self.reader.read_global_type()?)
            }
        }

        self.state = ParserState::ImportSectionEntry {
            module: module,
            field: field,
            ty: ty,
        };
=======
            return self.check_section_end();
        }
        let Import { module, field, ty } = section_reader!(self, ImportSectionReader).read()?;
        self.state = ParserState::ImportSectionEntry { module, field, ty };
>>>>>>> upstream-releases
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_function_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            return self.check_section_end();
        }
        let func_type = section_reader!(self, FunctionSectionReader).read()?;
        self.state = ParserState::FunctionSectionEntry(func_type);
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_memory_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            return self.check_section_end();
        }
        let memory_type = section_reader!(self, MemorySectionReader).read()?;
        self.state = ParserState::MemorySectionEntry(memory_type);
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_global_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            return self.check_section_end();
        }
        let Global { ty, init_expr } = section_reader!(self, GlobalSectionReader).read()?;
        self.state = ParserState::BeginGlobalSectionEntry(ty);
        self.operators_reader = Some(init_expr.get_operators_reader());
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_init_expression_body(&mut self, cont: InitExpressionContinuation) {
        self.state = ParserState::BeginInitExpressionBody;
        self.init_expr_continuation = Some(cont);
    }

    fn read_init_expression_operator(&mut self) -> Result<()> {
        let op = self
            .operators_reader
            .as_mut()
            .expect("operator reader")
            .read()?;
        if let Operator::End = op {
            self.operators_reader = None;
            self.state = ParserState::EndInitExpressionBody;
            return Ok(());
        }
        self.state = ParserState::InitExpressionOperator(op);
        Ok(())
    }

    fn read_export_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            return self.check_section_end();
        }
        let Export { field, kind, index } = section_reader!(self, ExportSectionReader).read()?;
        self.state = ParserState::ExportSectionEntry { field, kind, index };
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_element_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
<<<<<<< HEAD
            return self.check_section_end();
        }
        let Element {
            table_index,
            init_expr,
            items,
        } = section_reader!(self, ElementSectionReader).read()?;
        self.state = ParserState::BeginElementSectionEntry(table_index);
        self.operators_reader = Some(init_expr.get_operators_reader());
        self.element_items = Some(items);
||||||| merged common ancestors
            return self.position_to_section_end();
        }
        self.state = ParserState::BeginElementSectionEntry(self.reader.read_var_u32()?);
=======
            return self.check_section_end();
        }
        let Element { kind, items } = section_reader!(self, ElementSectionReader).read()?;
        match kind {
            ElementKind::Passive(ty) => {
                self.state = ParserState::BeginPassiveElementSectionEntry(ty);
            }
            ElementKind::Active {
                table_index,
                init_expr,
            } => {
                self.state = ParserState::BeginActiveElementSectionEntry(table_index);
                self.operators_reader = Some(init_expr.get_operators_reader());
            }
        }
        self.element_items = Some(items);
>>>>>>> upstream-releases
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_element_entry_body(&mut self) -> Result<()> {
        let mut reader = self
            .element_items
            .take()
            .expect("element items")
            .get_items_reader()?;
        let num_elements = reader.get_count() as usize;
        if num_elements > MAX_WASM_TABLE_ENTRIES {
            return Err(BinaryReaderError {
                message: "num_elements is out of bounds",
                offset: 0, // reader.position - 1, // TODO offset
            });
        }
        let mut elements: Vec<u32> = Vec::with_capacity(num_elements);
        for _ in 0..num_elements {
            elements.push(reader.read()?);
        }
        self.state = ParserState::ElementSectionEntryBody(elements.into_boxed_slice());
        Ok(())
    }

    fn read_function_body(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            self.current_function_body = None;
            return self.check_section_end();
        }
<<<<<<< HEAD
        let function_body = section_reader!(self, CodeSectionReader).read()?;
        let range = function_body.get_range();
||||||| merged common ancestors
        let size = self.reader.read_var_u32()? as usize;
        let body_end = self.reader.position + size;
        let range = Range {
            start: self.reader.position,
            end: body_end,
        };
=======
        let function_body = section_reader!(self, CodeSectionReader).read()?;
        let range = function_body.range();
>>>>>>> upstream-releases
        self.state = ParserState::BeginFunctionBody { range };
        self.current_function_body = Some(function_body);
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_function_body_locals(&mut self) -> Result<()> {
        let function_body = self.current_function_body.as_mut().expect("function body");
        let mut reader = function_body.get_locals_reader()?;
        let local_count = reader.get_count() as usize;
        if local_count > MAX_WASM_FUNCTION_LOCALS {
            return Err(BinaryReaderError {
                message: "local_count is out of bounds",
                offset: reader.original_position() - 1,
            });
        }
        let mut locals: Vec<(u32, Type)> = Vec::with_capacity(local_count);
        let mut locals_total: usize = 0;
        for _ in 0..local_count {
            let (count, ty) = reader.read()?;
            locals_total =
                locals_total
                    .checked_add(count as usize)
                    .ok_or_else(|| BinaryReaderError {
                        message: "locals_total is out of bounds",
                        offset: reader.original_position() - 1,
                    })?;
            if locals_total > MAX_WASM_FUNCTION_LOCALS {
                return Err(BinaryReaderError {
                    message: "locals_total is out of bounds",
                    offset: reader.original_position() - 1,
                });
            }
            locals.push((count, ty));
        }
        self.operators_reader = Some(function_body.get_operators_reader()?);
        self.state = ParserState::FunctionBodyLocals {
            locals: locals.into_boxed_slice(),
        };
        Ok(())
    }

    fn read_code_operator(&mut self) -> Result<()> {
        if self
            .operators_reader
            .as_ref()
            .expect("operator reader")
            .eof()
        {
            if let ParserState::CodeOperator(Operator::End) = self.state {
                self.state = ParserState::EndFunctionBody;
                self.operators_reader = None;
                self.current_function_body = None;
                return Ok(());
            }
            let reader = self.operators_reader.as_ref().expect("operator reader");
            return Err(BinaryReaderError {
                message: "Expected end of function marker",
                offset: reader.original_position(),
            });
        }
        let reader = self.operators_reader.as_mut().expect("operator reader");
        let op = reader.read()?;
        self.state = ParserState::CodeOperator(op);
        Ok(())
    }

    fn read_table_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            return self.check_section_end();
        }
        let table_entry = section_reader!(self, TableSectionReader).read()?;
        self.state = ParserState::TableSectionEntry(table_entry);
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_data_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
<<<<<<< HEAD
            return self.check_section_end();
        }
        let Data {
            memory_index,
            init_expr,
            data,
        } = section_reader!(self, DataSectionReader).read()?;
        self.state = ParserState::BeginDataSectionEntry(memory_index);
        self.operators_reader = Some(init_expr.get_operators_reader());
        self.current_data_segment = Some(data);
||||||| merged common ancestors
            return self.position_to_section_end();
        }
        let index = self.reader.read_var_u32()?;
        self.state = ParserState::BeginDataSectionEntry(index);
=======
            return self.check_section_end();
        }
        let Data { kind, data } = section_reader!(self, DataSectionReader).read()?;
        match kind {
            DataKind::Passive => {
                self.state = ParserState::BeginPassiveDataSectionEntry;
            }
            DataKind::Active {
                memory_index,
                init_expr,
            } => {
                self.state = ParserState::BeginActiveDataSectionEntry(memory_index);
                self.operators_reader = Some(init_expr.get_operators_reader());
            }
        }
        self.current_data_segment = Some(data);
>>>>>>> upstream-releases
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_data_entry_body(&mut self) -> Result<()> {
        let size = self.current_data_segment.expect("data entry").len();
        self.state = ParserState::BeginDataSectionEntryBody(size as u32);
        Ok(())
    }

    fn read_naming<'b>(
        mut naming_reader: NamingReader<'a>,
        limit: usize,
    ) -> Result<Box<[Naming<'b>]>>
    where
        'a: 'b,
    {
        let count = naming_reader.get_count() as usize;
        if count > limit {
            return Err(BinaryReaderError {
                message: "name map size is out of bound",
                offset: naming_reader.original_position() - 1,
            });
        }
        let mut result = Vec::with_capacity(count);
        for _ in 0..count {
            result.push(naming_reader.read()?);
        }
        Ok(result.into_boxed_slice())
    }

    fn read_name_entry(&mut self) -> Result<()> {
        if section_reader!(self, NameSectionReader).eof() {
            return self.position_to_section_end();
        }
        let entry = match section_reader!(self, NameSectionReader).read()? {
            Name::Module(name) => NameEntry::Module(name.get_name()?),
            Name::Function(func) => {
                NameEntry::Function(Self::read_naming(func.get_map()?, MAX_WASM_FUNCTIONS)?)
            }
            Name::Local(locals) => {
                let mut reader = locals.get_function_local_reader()?;
                let funcs_len = reader.get_count() as usize;
                if funcs_len > MAX_WASM_FUNCTIONS {
                    return Err(BinaryReaderError {
                        message: "function count is out of bounds",
                        offset: reader.original_position() - 1,
                    });
                }
                let mut funcs: Vec<LocalName<'a>> = Vec::with_capacity(funcs_len);
                for _ in 0..funcs_len {
                    let func = reader.read()?;
                    funcs.push(LocalName {
                        index: func.func_index,
                        locals: Self::read_naming(func.get_map()?, MAX_WASM_FUNCTION_LOCALS)?,
                    });
                }
                NameEntry::Local(funcs.into_boxed_slice())
            }
        };
        self.state = ParserState::NameSectionEntry(entry);
        Ok(())
    }

    fn read_source_mapping(&mut self) -> Result<()> {
        let url = self
            .current_section
            .as_ref()
            .expect("section")
            .get_sourcemappingurl_section_content()?;
        self.state = ParserState::SourceMappingURL(url);
        Ok(())
    }

    // See https://github.com/WebAssembly/tool-conventions/blob/master/Linking.md
    fn read_reloc_header(&mut self) -> Result<()> {
        let section_code = section_reader!(self, RelocSectionReader).get_section_code();
        self.state = ParserState::RelocSectionHeader(section_code);
        Ok(())
    }

    fn read_reloc_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            return self.check_section_end();
        }
        let Reloc {
            ty,
            offset,
            index,
            addend,
        } = section_reader!(self, RelocSectionReader).read()?;
        self.state = ParserState::RelocSectionEntry(RelocEntry {
            ty,
            offset,
            index,
            addend,
        });
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_linking_entry(&mut self) -> Result<()> {
        if self.section_entries_left == 0 {
            return self.check_section_end();
        }
        let entry = section_reader!(self, LinkingSectionReader).read()?;
        self.state = ParserState::LinkingSectionEntry(entry);
        self.section_entries_left -= 1;
        Ok(())
    }

    fn read_section_body(&mut self) -> Result<()> {
        match self.state {
            ParserState::BeginSection {
                code: SectionCode::Type,
                ..
            } => {
                start_section_reader!(self, TypeSectionReader, get_type_section_reader);
                self.read_type_entry()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Import,
                ..
            } => {
                start_section_reader!(self, ImportSectionReader, get_import_section_reader);
                self.read_import_entry()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Function,
                ..
            } => {
                start_section_reader!(self, FunctionSectionReader, get_function_section_reader);
                self.read_function_entry()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Memory,
                ..
            } => {
                start_section_reader!(self, MemorySectionReader, get_memory_section_reader);
                self.read_memory_entry()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Global,
                ..
            } => {
                start_section_reader!(self, GlobalSectionReader, get_global_section_reader);
                self.read_global_entry()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Export,
                ..
            } => {
                start_section_reader!(self, ExportSectionReader, get_export_section_reader);
                self.read_export_entry()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Element,
                ..
            } => {
                start_section_reader!(self, ElementSectionReader, get_element_section_reader);
                self.read_element_entry()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Code,
                ..
            } => {
                start_section_reader!(self, CodeSectionReader, get_code_section_reader);
                self.read_function_body()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Table,
                ..
            } => {
                start_section_reader!(self, TableSectionReader, get_table_section_reader);
                self.read_table_entry()?;
            }
            ParserState::BeginSection {
                code: SectionCode::Data,
                ..
            } => {
                start_section_reader!(self, DataSectionReader, get_data_section_reader);
                self.read_data_entry()?;
            }
<<<<<<< HEAD
            ParserState::BeginSection {
                code: SectionCode::Start,
                ..
            } => {
                let func_index = self
                    .current_section
                    .as_ref()
                    .expect("section")
                    .get_start_section_content()?;
                self.state = ParserState::StartSectionEntry(func_index);
||||||| merged common ancestors
            ParserState::BeginSection { code: SectionCode::Start, .. } => {
                self.state = ParserState::StartSectionEntry(self.reader.read_var_u32()?);
=======
            ParserState::BeginSection {
                code: SectionCode::Start,
                ..
            } => {
                let func_index = self
                    .current_section
                    .as_ref()
                    .expect("section")
                    .get_start_section_content()?;
                self.state = ParserState::StartSectionEntry(func_index);
            }
            ParserState::BeginSection {
                code: SectionCode::DataCount,
                ..
            } => {
                let func_index = self
                    .current_section
                    .as_ref()
                    .expect("section")
                    .get_data_count_section_content()?;
                self.state = ParserState::DataCountSectionEntry(func_index);
>>>>>>> upstream-releases
            }
            ParserState::BeginSection {
                code: SectionCode::Custom { .. },
                ..
            } => {
                self.create_custom_section_binary_reader();
                self.read_section_body_bytes()?;
            }
            _ => unreachable!(),
        }
        Ok(())
    }

    fn create_custom_section_binary_reader(&mut self) {
        let reader = self
            .current_section
            .as_ref()
            .expect("section")
            .get_binary_reader();
        self.binary_reader = Some(reader);
    }

    fn read_custom_section_body(&mut self) -> Result<()> {
        match self.state {
            ParserState::ReadingCustomSection(CustomSectionKind::Name) => {
                let reader = self
                    .current_section
                    .as_ref()
                    .expect("section")
                    .get_name_section_reader()?;
                self.section_reader = ParserSectionReader::NameSectionReader(reader);
                self.read_name_entry()?;
            }
            ParserState::ReadingCustomSection(CustomSectionKind::SourceMappingURL) => {
                self.read_source_mapping()?;
            }
            ParserState::ReadingCustomSection(CustomSectionKind::Reloc) => {
                start_section_reader!(self, RelocSectionReader, get_reloc_section_reader);
                self.read_reloc_header()?;
            }
            ParserState::ReadingCustomSection(CustomSectionKind::Linking) => {
                start_section_reader!(self, LinkingSectionReader, get_linking_section_reader);
                self.read_linking_entry()?;
            }
<<<<<<< HEAD
            ParserState::ReadingCustomSection(CustomSectionKind::Unknown) => {
                self.create_custom_section_binary_reader();
||||||| merged common ancestors
            ParserState::ReadingCustomSection(CustomSectionKind::Unknown) => {
=======
            ParserState::ReadingCustomSection(CustomSectionKind::Producers)
            | ParserState::ReadingCustomSection(CustomSectionKind::Unknown) => {
                self.create_custom_section_binary_reader();
>>>>>>> upstream-releases
                self.read_section_body_bytes()?;
            }
            _ => unreachable!(),
        }
        Ok(())
    }

    fn position_to_section_end(&mut self) -> Result<()> {
        self.current_section = None;
        self.binary_reader = None;
        self.state = ParserState::EndSection;
        Ok(())
    }

    fn check_section_end(&mut self) -> Result<()> {
        match self.section_reader {
            ParserSectionReader::CodeSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::DataSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::ElementSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::ExportSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::FunctionSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::GlobalSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::ImportSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::MemorySectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::TableSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::TypeSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::LinkingSectionReader(ref reader) => reader.ensure_end()?,
            ParserSectionReader::RelocSectionReader(ref reader) => reader.ensure_end()?,
            _ => unreachable!(),
        }
        self.position_to_section_end()
    }

    fn read_section_body_bytes(&mut self) -> Result<()> {
        if self.binary_reader.as_ref().expect("binary reader").eof() {
            self.state = ParserState::EndSection;
            self.binary_reader = None;
            return Ok(());
        }
        let binary_reader = self.binary_reader.as_mut().expect("binary reader");
        let to_read = if binary_reader.buffer.len() - binary_reader.position < MAX_DATA_CHUNK_SIZE {
            binary_reader.buffer.len() - binary_reader.position
        } else {
            MAX_DATA_CHUNK_SIZE
        };
        let bytes = binary_reader.read_bytes(to_read)?;
        self.state = ParserState::SectionRawData(bytes);
        Ok(())
    }

    fn read_data_chunk(&mut self) -> Result<()> {
        let data = self.current_data_segment.expect("data");
        if data.len() == 0 {
            self.state = ParserState::EndDataSectionEntryBody;
            self.current_data_segment = None;
            return Ok(());
        }
        let to_read = if data.len() > MAX_DATA_CHUNK_SIZE {
            MAX_DATA_CHUNK_SIZE
        } else {
            data.len()
        };
        let (head, tail) = data.split_at(to_read);
        self.current_data_segment = Some(tail);
        self.state = ParserState::DataSectionEntryBodyChunk(head);
        Ok(())
    }

    fn read_next_section(&mut self) -> Result<()> {
        if self.module_reader.as_ref().expect("module_reader").eof() {
            self.current_section = None;
            self.state = ParserState::EndWasm;
        } else {
            self.read_section_header()?;
        }
        Ok(())
    }

    fn read_wrapped(&mut self) -> Result<()> {
        match self.state {
            ParserState::EndWasm => panic!("Parser in end state"),
            ParserState::Error(_) => panic!("Parser in error state"),
            ParserState::Initial => self.read_module()?,
            ParserState::BeginWasm { .. } | ParserState::EndSection => self.read_next_section()?,
            ParserState::BeginSection { .. } => self.read_section_body()?,
            ParserState::SkippingSection => {
                self.position_to_section_end()?;
                self.read_next_section()?;
            }
            ParserState::TypeSectionEntry(_) => self.read_type_entry()?,
            ParserState::ImportSectionEntry { .. } => self.read_import_entry()?,
            ParserState::FunctionSectionEntry(_) => self.read_function_entry()?,
            ParserState::MemorySectionEntry(_) => self.read_memory_entry()?,
            ParserState::TableSectionEntry(_) => self.read_table_entry()?,
            ParserState::ExportSectionEntry { .. } => self.read_export_entry()?,
            ParserState::BeginGlobalSectionEntry(_) => {
                self.read_init_expression_body(InitExpressionContinuation::GlobalSection)
            }
            ParserState::EndGlobalSectionEntry => self.read_global_entry()?,
            ParserState::BeginPassiveElementSectionEntry(_) => self.read_element_entry_body()?,
            ParserState::BeginActiveElementSectionEntry(_) => {
                self.read_init_expression_body(InitExpressionContinuation::ElementSection)
            }
<<<<<<< HEAD
            ParserState::BeginInitExpressionBody | ParserState::InitExpressionOperator(_) => {
                self.read_init_expression_operator()?
            }
            ParserState::BeginDataSectionEntry(_) => {
||||||| merged common ancestors
            ParserState::BeginInitExpressionBody |
            ParserState::InitExpressionOperator(_) => self.read_init_expression_operator()?,
            ParserState::BeginDataSectionEntry(_) => {
=======
            ParserState::BeginInitExpressionBody | ParserState::InitExpressionOperator(_) => {
                self.read_init_expression_operator()?
            }
            ParserState::BeginPassiveDataSectionEntry => {
                self.read_data_entry_body()?;
            }
            ParserState::BeginActiveDataSectionEntry(_) => {
>>>>>>> upstream-releases
                self.read_init_expression_body(InitExpressionContinuation::DataSection)
            }
            ParserState::EndInitExpressionBody => {
                match self.init_expr_continuation {
                    Some(InitExpressionContinuation::GlobalSection) => {
                        self.state = ParserState::EndGlobalSectionEntry
                    }
                    Some(InitExpressionContinuation::ElementSection) => {
                        self.read_element_entry_body()?
                    }
                    Some(InitExpressionContinuation::DataSection) => self.read_data_entry_body()?,
                    None => unreachable!(),
                }
                self.init_expr_continuation = None;
            }
            ParserState::BeginFunctionBody { .. } => self.read_function_body_locals()?,
            ParserState::FunctionBodyLocals { .. } | ParserState::CodeOperator(_) => {
                self.read_code_operator()?
            }
            ParserState::EndFunctionBody => self.read_function_body()?,
            ParserState::SkippingFunctionBody => {
                self.current_function_body = None;
                self.read_function_body()?;
            }
            ParserState::EndDataSectionEntry => self.read_data_entry()?,
            ParserState::BeginDataSectionEntryBody(_)
            | ParserState::DataSectionEntryBodyChunk(_) => self.read_data_chunk()?,
            ParserState::EndDataSectionEntryBody => {
                self.state = ParserState::EndDataSectionEntry;
            }
            ParserState::ElementSectionEntryBody(_) => {
                self.state = ParserState::EndElementSectionEntry;
            }
            ParserState::EndElementSectionEntry => self.read_element_entry()?,
            ParserState::StartSectionEntry(_) => self.position_to_section_end()?,
            ParserState::DataCountSectionEntry(_) => self.position_to_section_end()?,
            ParserState::NameSectionEntry(_) => self.read_name_entry()?,
            ParserState::SourceMappingURL(_) => self.position_to_section_end()?,
            ParserState::RelocSectionHeader(_) => {
                let mut reader = self
                    .current_section
                    .as_ref()
                    .expect("section")
                    .get_binary_reader();
                self.section_entries_left = reader.read_var_u32()?;
                self.binary_reader = Some(reader);
                self.read_reloc_entry()?;
            }
            ParserState::RelocSectionEntry(_) => self.read_reloc_entry()?,
            ParserState::LinkingSectionEntry(_) => self.read_linking_entry()?,
            ParserState::ReadingCustomSection(_) => self.read_custom_section_body()?,
            ParserState::ReadingSectionRawData | ParserState::SectionRawData(_) => {
                self.read_section_body_bytes()?
            }
        }
        Ok(())
    }

    fn skip_section(&mut self) {
        match self.state {
            ParserState::Initial
            | ParserState::EndWasm
            | ParserState::Error(_)
            | ParserState::BeginWasm { .. }
            | ParserState::EndSection => panic!("Invalid reader state during skip section"),
            _ => self.state = ParserState::SkippingSection,
        }
    }

    fn skip_function_body(&mut self) {
        match self.state {
            ParserState::BeginFunctionBody { .. }
            | ParserState::FunctionBodyLocals { .. }
            | ParserState::CodeOperator(_) => self.state = ParserState::SkippingFunctionBody,
            _ => panic!("Invalid reader state during skip function body"),
        }
    }

    fn read_custom_section(&mut self) {
        match self.state {
            ParserState::BeginSection {
                code: SectionCode::Custom { kind, .. },
                ..
            } => {
                self.state = ParserState::ReadingCustomSection(kind);
            }
            _ => panic!("Invalid reader state during reading custom section"),
        }
    }

    fn read_raw_section_data(&mut self) {
        match self.state {
            ParserState::BeginSection { .. } => {
                self.binary_reader = Some(
                    self.current_section
                        .as_ref()
                        .expect("section")
                        .get_binary_reader(),
                );
                self.state = ParserState::ReadingSectionRawData;
            }
            _ => panic!("Invalid reader state during reading raw section data"),
        }
    }
}

impl<'a> WasmDecoder<'a> for Parser<'a> {
    /// Reads next record from the WebAssembly binary data. The methods returns
    /// reference to current state of the parser. See `ParserState` num.
    ///
    /// # Examples
    /// ```
    /// # let data: &[u8] = &[0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00,
    /// #     0x01, 0x4, 0x01, 0x60, 0x00, 0x00, 0x03, 0x02, 0x01, 0x00,
    /// #     0x0a, 0x05, 0x01, 0x03, 0x00, 0x01, 0x0b];
    /// use wasmparser::WasmDecoder;
    /// let mut parser = wasmparser::Parser::new(data);
    /// {
    ///     let state = parser.read();
    ///     println!("First state {:?}", state);
    /// }
    /// {
    ///     let state = parser.read();
    ///     println!("Second state {:?}", state);
    /// }
    /// ```
    fn read(&mut self) -> &ParserState<'a> {
        let result = self.read_wrapped();
        if result.is_err() {
            self.state = ParserState::Error(result.err().unwrap());
        }
        &self.state
    }

    fn push_input(&mut self, input: ParserInput) {
        match input {
            ParserInput::Default => (),
            ParserInput::SkipSection => self.skip_section(),
            ParserInput::SkipFunctionBody => self.skip_function_body(),
            ParserInput::ReadCustomSection => self.read_custom_section(),
            ParserInput::ReadSectionRawData => self.read_raw_section_data(),
        }
    }

    /// Creates a BinaryReader when current state is ParserState::BeginSection
    /// or ParserState::BeginFunctionBody.
    ///
    /// # Examples
    /// ```
    /// # let data = &[0x0, 0x61, 0x73, 0x6d, 0x1, 0x0, 0x0, 0x0, 0x1, 0x84,
    /// #              0x80, 0x80, 0x80, 0x0, 0x1, 0x60, 0x0, 0x0, 0x3, 0x83,
    /// #              0x80, 0x80, 0x80, 0x0, 0x2, 0x0, 0x0, 0x6, 0x81, 0x80,
    /// #              0x80, 0x80, 0x0, 0x0, 0xa, 0x91, 0x80, 0x80, 0x80, 0x0,
    /// #              0x2, 0x83, 0x80, 0x80, 0x80, 0x0, 0x0, 0x1, 0xb, 0x83,
    /// #              0x80, 0x80, 0x80, 0x0, 0x0, 0x0, 0xb];
    /// use wasmparser::{WasmDecoder, Parser, ParserState};
    /// let mut parser = Parser::new(data);
    /// let mut function_readers = Vec::new();
    /// loop {
    ///     match *parser.read() {
    ///         ParserState::Error(_) |
    ///         ParserState::EndWasm => break,
    ///         ParserState::BeginFunctionBody {..} => {
    ///             let reader = parser.create_binary_reader();
    ///             function_readers.push(reader);
    ///         }
    ///         _ => continue
    ///     }
    /// }
    /// for (i, reader) in function_readers.iter_mut().enumerate() {
    ///     println!("Function {}", i);
    ///     while let Ok(ref op) = reader.read_operator() {
    ///       println!("  {:?}", op);
    ///     }
    /// }
    /// ```
    fn create_binary_reader<'b>(&mut self) -> BinaryReader<'b>
    where
        'a: 'b,
    {
        match self.state {
            ParserState::BeginSection { .. } => self
                .current_section
                .as_ref()
                .expect("section")
                .get_binary_reader(),
            ParserState::BeginFunctionBody { .. } | ParserState::FunctionBodyLocals { .. } => self
                .current_function_body
                .as_ref()
                .expect("function body")
                .get_binary_reader(),
            _ => panic!("Invalid reader state during get binary reader operation"),
        }
    }

    /// Reads next record from the WebAssembly binary data. It also allows to
    /// control how parser will treat the next record(s). The method accepts the
    /// `ParserInput` parameter that allows e.g. to skip section or function
    /// operators. The methods returns reference to current state of the parser.
    ///
    /// # Examples
    /// ```
    /// # let data: &[u8] = &[0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00,
    /// #     0x01, 0x4, 0x01, 0x60, 0x00, 0x00, 0x03, 0x02, 0x01, 0x00,
    /// #     0x0a, 0x05, 0x01, 0x03, 0x00, 0x01, 0x0b];
    /// use wasmparser::WasmDecoder;
    /// let mut parser = wasmparser::Parser::new(data);
    /// let mut next_input = wasmparser::ParserInput::Default;
    /// loop {
    ///     let state = parser.read_with_input(next_input);
    ///     match *state {
    ///         wasmparser::ParserState::EndWasm => break,
    ///         wasmparser::ParserState::BeginWasm { .. } |
    ///         wasmparser::ParserState::EndSection =>
    ///             next_input = wasmparser::ParserInput::Default,
    ///         wasmparser::ParserState::BeginSection { ref code, .. } => {
    ///             println!("Found section: {:?}", code);
    ///             next_input = wasmparser::ParserInput::SkipSection;
    ///         },
    ///         _ => unreachable!()
    ///     }
    /// }
    /// ```
    fn read_with_input(&mut self, input: ParserInput) -> &ParserState<'a> {
        self.push_input(input);
        self.read()
    }

    fn last_state(&self) -> &ParserState<'a> {
        &self.state
    }
}
