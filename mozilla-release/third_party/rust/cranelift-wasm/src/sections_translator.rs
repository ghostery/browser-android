//! Helper functions to gather information for each of the non-function sections of a
//! WebAssembly module.
//!
//! The code of these helper functions is straightforward since they only read metadata
//! about linear memories, tables, globals, etc. and store them for later use.
//!
//! The special case of the initialize expressions for table elements offsets or global variables
//! is handled, according to the semantics of WebAssembly, to only specific expressions that are
//! interpreted on the fly.
use crate::environ::{ModuleEnvironment, WasmError, WasmResult};
use crate::translation_utils::{
    tabletype_to_type, type_to_type, FuncIndex, Global, GlobalIndex, GlobalInit, Memory,
    MemoryIndex, SignatureIndex, Table, TableElementType, TableIndex,
};
use core::convert::TryFrom;
use cranelift_codegen::ir::{self, AbiParam, Signature};
use cranelift_entity::EntityRef;
<<<<<<< HEAD
use environ::{ModuleEnvironment, WasmResult};
use std::str::from_utf8;
||||||| merged common ancestors
use environ::{ModuleEnvironment, WasmError, WasmResult};
use std::str::from_utf8;
=======
>>>>>>> upstream-releases
use std::vec::Vec;
<<<<<<< HEAD
use translation_utils::{
    type_to_type, FuncIndex, Global, GlobalIndex, GlobalInit, Memory, MemoryIndex, SignatureIndex,
    Table, TableElementType, TableIndex,
};
||||||| merged common ancestors
use translation_utils::{
    type_to_type, FuncIndex, Global, GlobalIndex, GlobalInit, Memory, MemoryIndex, SignatureIndex,
    Table, TableElementType, TableIndex,
};
use wasmparser;
=======
>>>>>>> upstream-releases
use wasmparser::{
<<<<<<< HEAD
    self, CodeSectionReader, Data, DataSectionReader, Element, ElementSectionReader, Export,
    ExportSectionReader, ExternalKind, FuncType, FunctionSectionReader, GlobalSectionReader,
    GlobalType, ImportSectionEntryType, ImportSectionReader, MemorySectionReader, MemoryType,
    Operator, TableSectionReader, TypeSectionReader,
||||||| merged common ancestors
    ExternalKind, FuncType, ImportSectionEntryType, MemoryType, Operator, Parser, ParserState,
    WasmDecoder,
=======
    self, CodeSectionReader, Data, DataKind, DataSectionReader, Element, ElementKind,
    ElementSectionReader, Export, ExportSectionReader, ExternalKind, FuncType,
    FunctionSectionReader, GlobalSectionReader, GlobalType, ImportSectionEntryType,
    ImportSectionReader, MemorySectionReader, MemoryType, Operator, TableSectionReader,
    TypeSectionReader,
>>>>>>> upstream-releases
};

<<<<<<< HEAD
/// Parses the Type section of the wasm module.
pub fn parse_type_section(
    types: TypeSectionReader,
    environ: &mut ModuleEnvironment,
||||||| merged common ancestors
/// Reads the Type Section of the wasm module and returns the corresponding function signatures.
pub fn parse_function_signatures(
    parser: &mut Parser,
    environ: &mut ModuleEnvironment,
=======
/// Parses the Type section of the wasm module.
pub fn parse_type_section(
    types: TypeSectionReader,
    environ: &mut dyn ModuleEnvironment,
>>>>>>> upstream-releases
) -> WasmResult<()> {
<<<<<<< HEAD
    for entry in types {
        match entry? {
            FuncType {
||||||| merged common ancestors
    loop {
        match *parser.read() {
            ParserState::EndSection => break,
            ParserState::TypeSectionEntry(FuncType {
=======
    environ.reserve_signatures(types.get_count());

    for entry in types {
        match entry? {
            FuncType {
>>>>>>> upstream-releases
                form: wasmparser::Type::Func,
                ref params,
                ref returns,
            } => {
                let mut sig = Signature::new(environ.target_config().default_call_conv);
                sig.params.extend(params.iter().map(|ty| {
                    let cret_arg: ir::Type = type_to_type(*ty)
                        .expect("only numeric types are supported in function signatures");
                    AbiParam::new(cret_arg)
                }));
                sig.returns.extend(returns.iter().map(|ty| {
                    let cret_arg: ir::Type = type_to_type(*ty)
                        .expect("only numeric types are supported in function signatures");
                    AbiParam::new(cret_arg)
                }));
                environ.declare_signature(sig);
            }
<<<<<<< HEAD
            ref s => panic!("unsupported type: {:?}", s),
||||||| merged common ancestors
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
=======
            _ => return Err(WasmError::Unsupported("unsupported type in type section")),
>>>>>>> upstream-releases
        }
    }
    Ok(())
}

/// Parses the Import section of the wasm module.
pub fn parse_import_section<'data>(
<<<<<<< HEAD
    imports: ImportSectionReader<'data>,
    environ: &mut ModuleEnvironment<'data>,
||||||| merged common ancestors
    parser: &mut Parser<'data>,
    environ: &mut ModuleEnvironment<'data>,
=======
    imports: ImportSectionReader<'data>,
    environ: &mut dyn ModuleEnvironment<'data>,
>>>>>>> upstream-releases
) -> WasmResult<()> {
<<<<<<< HEAD
    for entry in imports {
        let import = entry?;

        // The input has already been validated, so we should be able to
        // assume valid UTF-8 and use `from_utf8_unchecked` if performance
        // becomes a concern here.
        let module_name = from_utf8(import.module).unwrap();
        let field_name = from_utf8(import.field).unwrap();

        match import.ty {
            ImportSectionEntryType::Function(sig) => {
                environ.declare_func_import(
                    SignatureIndex::new(sig as usize),
                    module_name,
                    field_name,
                );
||||||| merged common ancestors
    loop {
        match *parser.read() {
            ParserState::ImportSectionEntry {
                ty: ImportSectionEntryType::Function(sig),
                module,
                field,
            } => {
                // The input has already been validated, so we should be able to
                // assume valid UTF-8 and use `from_utf8_unchecked` if performance
                // becomes a concern here.
                let module_name = from_utf8(module).unwrap();
                let field_name = from_utf8(field).unwrap();
                environ.declare_func_import(sig as SignatureIndex, module_name, field_name);
=======
    environ.reserve_imports(imports.get_count());

    for entry in imports {
        let import = entry?;
        let module_name = import.module;
        let field_name = import.field;

        match import.ty {
            ImportSectionEntryType::Function(sig) => {
                environ.declare_func_import(SignatureIndex::from_u32(sig), module_name, field_name);
>>>>>>> upstream-releases
            }
<<<<<<< HEAD
            ImportSectionEntryType::Memory(MemoryType {
                limits: ref memlimits,
                shared,
            }) => {
                environ.declare_memory_import(
                    Memory {
                        pages_count: memlimits.initial as usize,
                        maximum: memlimits.maximum.map(|x| x as usize),
||||||| merged common ancestors
            ParserState::ImportSectionEntry {
                ty:
                    ImportSectionEntryType::Memory(MemoryType {
                        limits: ref memlimits,
=======
            ImportSectionEntryType::Memory(MemoryType {
                limits: ref memlimits,
                shared,
            }) => {
                environ.declare_memory_import(
                    Memory {
                        minimum: memlimits.initial,
                        maximum: memlimits.maximum,
>>>>>>> upstream-releases
                        shared,
                    },
                    module_name,
                    field_name,
                );
            }
<<<<<<< HEAD
            ImportSectionEntryType::Global(ref ty) => {
                environ.declare_global_import(
                    Global {
                        ty: type_to_type(ty.content_type).unwrap(),
                        mutability: ty.mutable,
                        initializer: GlobalInit::Import(),
                    },
                    module_name,
                    field_name,
                );
||||||| merged common ancestors
            ParserState::ImportSectionEntry {
                ty: ImportSectionEntryType::Global(ref ty),
                ..
            } => {
                environ.declare_global(Global {
                    ty: type_to_type(ty.content_type).unwrap(),
                    mutability: ty.mutable,
                    initializer: GlobalInit::Import(),
                });
=======
            ImportSectionEntryType::Global(ref ty) => {
                environ.declare_global_import(
                    Global {
                        ty: type_to_type(ty.content_type).unwrap(),
                        mutability: ty.mutable,
                        initializer: GlobalInit::Import,
                    },
                    module_name,
                    field_name,
                );
>>>>>>> upstream-releases
            }
<<<<<<< HEAD
            ImportSectionEntryType::Table(ref tab) => {
                environ.declare_table_import(
                    Table {
                        ty: match type_to_type(tab.element_type) {
                            Ok(t) => TableElementType::Val(t),
                            Err(()) => TableElementType::Func(),
                        },
                        size: tab.limits.initial as usize,
                        maximum: tab.limits.maximum.map(|x| x as usize),
                    },
                    module_name,
                    field_name,
                );
            }
        }
||||||| merged common ancestors
            ParserState::ImportSectionEntry {
                ty: ImportSectionEntryType::Table(ref tab),
                ..
            } => environ.declare_table(Table {
                ty: match type_to_type(tab.element_type) {
                    Ok(t) => TableElementType::Val(t),
                    Err(()) => TableElementType::Func(),
                },
                size: tab.limits.initial as usize,
                maximum: tab.limits.maximum.map(|x| x as usize),
            }),
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
=======
            ImportSectionEntryType::Table(ref tab) => {
                environ.declare_table_import(
                    Table {
                        ty: match tabletype_to_type(tab.element_type)? {
                            Some(t) => TableElementType::Val(t),
                            None => TableElementType::Func,
                        },
                        minimum: tab.limits.initial,
                        maximum: tab.limits.maximum,
                    },
                    module_name,
                    field_name,
                );
            }
        }
>>>>>>> upstream-releases
    }

    environ.finish_imports();
    Ok(())
}

/// Parses the Function section of the wasm module.
pub fn parse_function_section(
<<<<<<< HEAD
    functions: FunctionSectionReader,
    environ: &mut ModuleEnvironment,
||||||| merged common ancestors
    parser: &mut Parser,
    environ: &mut ModuleEnvironment,
=======
    functions: FunctionSectionReader,
    environ: &mut dyn ModuleEnvironment,
>>>>>>> upstream-releases
) -> WasmResult<()> {
<<<<<<< HEAD
    for entry in functions {
        let sigindex = entry?;
        environ.declare_func_type(SignatureIndex::new(sigindex as usize));
||||||| merged common ancestors
    loop {
        match *parser.read() {
            ParserState::FunctionSectionEntry(sigindex) => {
                environ.declare_func_type(sigindex as SignatureIndex);
            }
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
=======
    environ.reserve_func_types(functions.get_count());

    for entry in functions {
        let sigindex = entry?;
        environ.declare_func_type(SignatureIndex::from_u32(sigindex));
>>>>>>> upstream-releases
    }

    Ok(())
}

<<<<<<< HEAD
/// Parses the Table section of the wasm module.
pub fn parse_table_section(
    tables: TableSectionReader,
    environ: &mut ModuleEnvironment,
||||||| merged common ancestors
/// Retrieves the names of the functions from the export section
pub fn parse_export_section<'data>(
    parser: &mut Parser<'data>,
    environ: &mut ModuleEnvironment<'data>,
=======
/// Parses the Table section of the wasm module.
pub fn parse_table_section(
    tables: TableSectionReader,
    environ: &mut dyn ModuleEnvironment,
>>>>>>> upstream-releases
) -> WasmResult<()> {
<<<<<<< HEAD
    for entry in tables {
        let table = entry?;
        environ.declare_table(Table {
            ty: match type_to_type(table.element_type) {
                Ok(t) => TableElementType::Val(t),
                Err(()) => TableElementType::Func(),
            },
            size: table.limits.initial as usize,
            maximum: table.limits.maximum.map(|x| x as usize),
        });
||||||| merged common ancestors
    loop {
        match *parser.read() {
            ParserState::ExportSectionEntry {
                field,
                ref kind,
                index,
            } => {
                // The input has already been validated, so we should be able to
                // assume valid UTF-8 and use `from_utf8_unchecked` if performance
                // becomes a concern here.
                let name = from_utf8(field).unwrap();
                let func_index = FuncIndex::new(index as usize);
                match *kind {
                    ExternalKind::Function => environ.declare_func_export(func_index, name),
                    ExternalKind::Table => environ.declare_table_export(func_index.index(), name),
                    ExternalKind::Memory => environ.declare_memory_export(func_index.index(), name),
                    ExternalKind::Global => environ.declare_global_export(func_index.index(), name),
                }
            }
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
    }
    Ok(())
}

/// Retrieves the start function index from the start section
pub fn parse_start_section(parser: &mut Parser, environ: &mut ModuleEnvironment) -> WasmResult<()> {
    loop {
        match *parser.read() {
            ParserState::StartSectionEntry(index) => {
                environ.declare_start_func(FuncIndex::new(index as usize));
            }
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
=======
    environ.reserve_tables(tables.get_count());

    for entry in tables {
        let table = entry?;
        environ.declare_table(Table {
            ty: match tabletype_to_type(table.element_type)? {
                Some(t) => TableElementType::Val(t),
                None => TableElementType::Func,
            },
            minimum: table.limits.initial,
            maximum: table.limits.maximum,
        });
>>>>>>> upstream-releases
    }

    Ok(())
}

/// Parses the Memory section of the wasm module.
pub fn parse_memory_section(
<<<<<<< HEAD
    memories: MemorySectionReader,
    environ: &mut ModuleEnvironment,
||||||| merged common ancestors
    parser: &mut Parser,
    environ: &mut ModuleEnvironment,
=======
    memories: MemorySectionReader,
    environ: &mut dyn ModuleEnvironment,
>>>>>>> upstream-releases
) -> WasmResult<()> {
<<<<<<< HEAD
    for entry in memories {
        let memory = entry?;
        environ.declare_memory(Memory {
            pages_count: memory.limits.initial as usize,
            maximum: memory.limits.maximum.map(|x| x as usize),
            shared: memory.shared,
        });
||||||| merged common ancestors
    loop {
        match *parser.read() {
            ParserState::MemorySectionEntry(ref ty) => {
                environ.declare_memory(Memory {
                    pages_count: ty.limits.initial as usize,
                    maximum: ty.limits.maximum.map(|x| x as usize),
                    shared: ty.shared,
                });
            }
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
=======
    environ.reserve_memories(memories.get_count());

    for entry in memories {
        let memory = entry?;
        environ.declare_memory(Memory {
            minimum: memory.limits.initial,
            maximum: memory.limits.maximum,
            shared: memory.shared,
        });
>>>>>>> upstream-releases
    }

    Ok(())
}

/// Parses the Global section of the wasm module.
pub fn parse_global_section(
<<<<<<< HEAD
    globals: GlobalSectionReader,
    environ: &mut ModuleEnvironment,
||||||| merged common ancestors
    parser: &mut Parser,
    environ: &mut ModuleEnvironment,
=======
    globals: GlobalSectionReader,
    environ: &mut dyn ModuleEnvironment,
>>>>>>> upstream-releases
) -> WasmResult<()> {
<<<<<<< HEAD
    for entry in globals {
        let wasmparser::Global {
            ty: GlobalType {
                content_type,
                mutable,
            },
            init_expr,
        } = entry?;
        let mut init_expr_reader = init_expr.get_binary_reader();
        let initializer = match init_expr_reader.read_operator()? {
            Operator::I32Const { value } => GlobalInit::I32Const(value),
            Operator::I64Const { value } => GlobalInit::I64Const(value),
            Operator::F32Const { value } => GlobalInit::F32Const(value.bits()),
            Operator::F64Const { value } => GlobalInit::F64Const(value.bits()),
            Operator::GetGlobal { global_index } => {
                GlobalInit::GlobalRef(GlobalIndex::new(global_index as usize))
||||||| merged common ancestors
    loop {
        let (content_type, mutability) = match *parser.read() {
            ParserState::BeginGlobalSectionEntry(ref ty) => (ty.content_type, ty.mutable),
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        match *parser.read() {
            ParserState::BeginInitExpressionBody => (),
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        }
        let initializer = match *parser.read() {
            ParserState::InitExpressionOperator(Operator::I32Const { value }) => {
                GlobalInit::I32Const(value)
            }
            ParserState::InitExpressionOperator(Operator::I64Const { value }) => {
                GlobalInit::I64Const(value)
            }
            ParserState::InitExpressionOperator(Operator::F32Const { value }) => {
                GlobalInit::F32Const(value.bits())
            }
            ParserState::InitExpressionOperator(Operator::F64Const { value }) => {
                GlobalInit::F64Const(value.bits())
            }
            ParserState::InitExpressionOperator(Operator::GetGlobal { global_index }) => {
                GlobalInit::GlobalRef(global_index as GlobalIndex)
=======
    environ.reserve_globals(globals.get_count());

    for entry in globals {
        let wasmparser::Global {
            ty: GlobalType {
                content_type,
                mutable,
            },
            init_expr,
        } = entry?;
        let mut init_expr_reader = init_expr.get_binary_reader();
        let initializer = match init_expr_reader.read_operator()? {
            Operator::I32Const { value } => GlobalInit::I32Const(value),
            Operator::I64Const { value } => GlobalInit::I64Const(value),
            Operator::F32Const { value } => GlobalInit::F32Const(value.bits()),
            Operator::F64Const { value } => GlobalInit::F64Const(value.bits()),
            Operator::GetGlobal { global_index } => {
                GlobalInit::GetGlobal(GlobalIndex::from_u32(global_index))
>>>>>>> upstream-releases
            }
            ref s => panic!("unsupported init expr in global section: {:?}", s),
        };
        let global = Global {
            ty: type_to_type(content_type).unwrap(),
            mutability: mutable,
            initializer,
        };
        environ.declare_global(global);
    }

    Ok(())
}

<<<<<<< HEAD
/// Parses the Export section of the wasm module.
pub fn parse_export_section<'data>(
    exports: ExportSectionReader<'data>,
    environ: &mut ModuleEnvironment<'data>,
||||||| merged common ancestors
pub fn parse_data_section<'data>(
    parser: &mut Parser<'data>,
    environ: &mut ModuleEnvironment<'data>,
=======
/// Parses the Export section of the wasm module.
pub fn parse_export_section<'data>(
    exports: ExportSectionReader<'data>,
    environ: &mut dyn ModuleEnvironment<'data>,
>>>>>>> upstream-releases
) -> WasmResult<()> {
<<<<<<< HEAD
    for entry in exports {
        let Export {
            field,
            ref kind,
            index,
        } = entry?;

        // The input has already been validated, so we should be able to
        // assume valid UTF-8 and use `from_utf8_unchecked` if performance
        // becomes a concern here.
        let name = from_utf8(field).unwrap();
        let index = index as usize;
        match *kind {
            ExternalKind::Function => environ.declare_func_export(FuncIndex::new(index), name),
            ExternalKind::Table => environ.declare_table_export(TableIndex::new(index), name),
            ExternalKind::Memory => environ.declare_memory_export(MemoryIndex::new(index), name),
            ExternalKind::Global => environ.declare_global_export(GlobalIndex::new(index), name),
||||||| merged common ancestors
    loop {
        let memory_index = match *parser.read() {
            ParserState::BeginDataSectionEntry(memory_index) => memory_index,
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        match *parser.read() {
            ParserState::BeginInitExpressionBody => (),
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        let (base, offset) = match *parser.read() {
            ParserState::InitExpressionOperator(Operator::I32Const { value }) => {
                (None, value as u32 as usize)
            }
            ParserState::InitExpressionOperator(Operator::GetGlobal { global_index }) => {
                match environ.get_global(global_index as GlobalIndex).initializer {
                    GlobalInit::I32Const(value) => (None, value as u32 as usize),
                    GlobalInit::Import() => (Some(global_index as GlobalIndex), 0),
                    _ => panic!("should not happen"),
                }
            }
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        match *parser.read() {
            ParserState::EndInitExpressionBody => (),
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        match *parser.read() {
            ParserState::BeginDataSectionEntryBody(_) => (),
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        let mut running_offset = offset;
        loop {
            let data = match *parser.read() {
                ParserState::DataSectionEntryBodyChunk(data) => data,
                ParserState::EndDataSectionEntryBody => break,
                ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
                ref s => panic!("unexpected section content: {:?}", s),
            };
            environ.declare_data_initialization(
                memory_index as MemoryIndex,
                base,
                running_offset,
                data,
            );
            running_offset += data.len();
=======
    environ.reserve_exports(exports.get_count());

    for entry in exports {
        let Export {
            field,
            ref kind,
            index,
        } = entry?;

        // The input has already been validated, so we should be able to
        // assume valid UTF-8 and use `from_utf8_unchecked` if performance
        // becomes a concern here.
        let index = index as usize;
        match *kind {
            ExternalKind::Function => environ.declare_func_export(FuncIndex::new(index), field),
            ExternalKind::Table => environ.declare_table_export(TableIndex::new(index), field),
            ExternalKind::Memory => environ.declare_memory_export(MemoryIndex::new(index), field),
            ExternalKind::Global => environ.declare_global_export(GlobalIndex::new(index), field),
>>>>>>> upstream-releases
        }
    }

    environ.finish_exports();
    Ok(())
}

<<<<<<< HEAD
/// Parses the Start section of the wasm module.
pub fn parse_start_section(index: u32, environ: &mut ModuleEnvironment) -> WasmResult<()> {
    environ.declare_start_func(FuncIndex::new(index as usize));
||||||| merged common ancestors
/// Retrieves the tables from the table section
pub fn parse_table_section(parser: &mut Parser, environ: &mut ModuleEnvironment) -> WasmResult<()> {
    loop {
        match *parser.read() {
            ParserState::TableSectionEntry(ref table) => environ.declare_table(Table {
                ty: match type_to_type(table.element_type) {
                    Ok(t) => TableElementType::Val(t),
                    Err(()) => TableElementType::Func(),
                },
                size: table.limits.initial as usize,
                maximum: table.limits.maximum.map(|x| x as usize),
            }),
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
    }
=======
/// Parses the Start section of the wasm module.
pub fn parse_start_section(index: u32, environ: &mut dyn ModuleEnvironment) -> WasmResult<()> {
    environ.declare_start_func(FuncIndex::from_u32(index));
>>>>>>> upstream-releases
    Ok(())
}

<<<<<<< HEAD
/// Parses the Element section of the wasm module.
pub fn parse_element_section<'data>(
    elements: ElementSectionReader<'data>,
    environ: &mut ModuleEnvironment,
||||||| merged common ancestors
/// Retrieves the tables from the table section
pub fn parse_elements_section(
    parser: &mut Parser,
    environ: &mut ModuleEnvironment,
=======
/// Parses the Element section of the wasm module.
pub fn parse_element_section<'data>(
    elements: ElementSectionReader<'data>,
    environ: &mut dyn ModuleEnvironment,
>>>>>>> upstream-releases
) -> WasmResult<()> {
<<<<<<< HEAD
    for entry in elements {
        let Element {
            table_index,
            init_expr,
            items,
        } = entry?;
        let mut init_expr_reader = init_expr.get_binary_reader();
        let (base, offset) = match init_expr_reader.read_operator()? {
            Operator::I32Const { value } => (None, value as u32 as usize),
            Operator::GetGlobal { global_index } => match environ
                .get_global(GlobalIndex::new(global_index as usize))
                .initializer
            {
                GlobalInit::I32Const(value) => (None, value as u32 as usize),
                GlobalInit::Import() => (Some(GlobalIndex::new(global_index as usize)), 0),
                _ => panic!("should not happen"),
            },
            ref s => panic!("unsupported init expr in element section: {:?}", s),
        };
        let items_reader = items.get_items_reader()?;
        let mut elems = Vec::new();
        for item in items_reader {
            let x = item?;
            elems.push(FuncIndex::new(x as usize));
        }
        environ.declare_table_elements(TableIndex::new(table_index as usize), base, offset, elems)
||||||| merged common ancestors
    loop {
        let table_index = match *parser.read() {
            ParserState::BeginElementSectionEntry(table_index) => table_index as TableIndex,
            ParserState::EndSection => break,
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        match *parser.read() {
            ParserState::BeginInitExpressionBody => (),
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        let (base, offset) = match *parser.read() {
            ParserState::InitExpressionOperator(Operator::I32Const { value }) => {
                (None, value as u32 as usize)
            }
            ParserState::InitExpressionOperator(Operator::GetGlobal { global_index }) => {
                match environ.get_global(global_index as GlobalIndex).initializer {
                    GlobalInit::I32Const(value) => (None, value as u32 as usize),
                    GlobalInit::Import() => (Some(global_index as GlobalIndex), 0),
                    _ => panic!("should not happen"),
                }
            }
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        match *parser.read() {
            ParserState::EndInitExpressionBody => (),
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        match *parser.read() {
            ParserState::ElementSectionEntryBody(ref elements) => {
                let elems: Vec<FuncIndex> = elements
                    .iter()
                    .map(|&x| FuncIndex::new(x as usize))
                    .collect();
                environ.declare_table_elements(table_index, base, offset, elems)
            }
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
        match *parser.read() {
            ParserState::EndElementSectionEntry => (),
            ParserState::Error(e) => return Err(WasmError::from_binary_reader_error(e)),
            ref s => panic!("unexpected section content: {:?}", s),
        };
=======
    environ.reserve_table_elements(elements.get_count());

    for entry in elements {
        let Element { kind, items } = entry?;
        if let ElementKind::Active {
            table_index,
            init_expr,
        } = kind
        {
            let mut init_expr_reader = init_expr.get_binary_reader();
            let (base, offset) = match init_expr_reader.read_operator()? {
                Operator::I32Const { value } => (None, value as u32 as usize),
                Operator::GetGlobal { global_index } => {
                    (Some(GlobalIndex::from_u32(global_index)), 0)
                }
                ref s => panic!("unsupported init expr in element section: {:?}", s),
            };
            let items_reader = items.get_items_reader()?;
            let mut elems = Vec::with_capacity(usize::try_from(items_reader.get_count()).unwrap());
            for item in items_reader {
                let x = item?;
                elems.push(FuncIndex::from_u32(x));
            }
            environ.declare_table_elements(
                TableIndex::from_u32(table_index),
                base,
                offset,
                elems.into_boxed_slice(),
            )
        } else {
            panic!("unsupported passive elements section");
        }
>>>>>>> upstream-releases
    }
    Ok(())
}

/// Parses the Code section of the wasm module.
pub fn parse_code_section<'data>(
<<<<<<< HEAD
    code: CodeSectionReader<'data>,
    environ: &mut ModuleEnvironment<'data>,
||||||| merged common ancestors
    parser: &mut Parser<'data>,
    environ: &mut ModuleEnvironment<'data>,
=======
    code: CodeSectionReader<'data>,
    environ: &mut dyn ModuleEnvironment<'data>,
>>>>>>> upstream-releases
) -> WasmResult<()> {
    for body in code {
        let mut reader = body?.get_binary_reader();
        let size = reader.bytes_remaining();
<<<<<<< HEAD
        environ.define_function_body(reader.read_bytes(size)?)?;
    }
    Ok(())
}

/// Parses the Data section of the wasm module.
pub fn parse_data_section<'data>(
    data: DataSectionReader<'data>,
    environ: &mut ModuleEnvironment<'data>,
) -> WasmResult<()> {
    for entry in data {
        let Data {
            memory_index,
            init_expr,
            data,
        } = entry?;
        let mut init_expr_reader = init_expr.get_binary_reader();
        let (base, offset) = match init_expr_reader.read_operator()? {
            Operator::I32Const { value } => (None, value as u32 as usize),
            Operator::GetGlobal { global_index } => match environ
                .get_global(GlobalIndex::new(global_index as usize))
                .initializer
            {
                GlobalInit::I32Const(value) => (None, value as u32 as usize),
                GlobalInit::Import() => (Some(GlobalIndex::new(global_index as usize)), 0),
                _ => panic!("should not happen"),
            },
            ref s => panic!("unsupported init expr in data section: {:?}", s),
        };
        environ.declare_data_initialization(
            MemoryIndex::new(memory_index as usize),
            base,
            offset,
            data,
        );
||||||| merged common ancestors
        environ.define_function_body(
            reader
                .read_bytes(size)
                .map_err(WasmError::from_binary_reader_error)?,
        )?;
=======
        let offset = reader.original_position();
        environ.define_function_body(reader.read_bytes(size)?, offset)?;
>>>>>>> upstream-releases
    }
    Ok(())
}

/// Parses the Data section of the wasm module.
pub fn parse_data_section<'data>(
    data: DataSectionReader<'data>,
    environ: &mut dyn ModuleEnvironment<'data>,
) -> WasmResult<()> {
    environ.reserve_data_initializers(data.get_count());

    for entry in data {
        let Data { kind, data } = entry?;
        if let DataKind::Active {
            memory_index,
            init_expr,
        } = kind
        {
            let mut init_expr_reader = init_expr.get_binary_reader();
            let (base, offset) = match init_expr_reader.read_operator()? {
                Operator::I32Const { value } => (None, value as u32 as usize),
                Operator::GetGlobal { global_index } => {
                    (Some(GlobalIndex::from_u32(global_index)), 0)
                }
                ref s => panic!("unsupported init expr in data section: {:?}", s),
            };
            environ.declare_data_initialization(
                MemoryIndex::from_u32(memory_index),
                base,
                offset,
                data,
            );
        } else {
            panic!("unsupported passive data section");
        }
    }

    Ok(())
}
