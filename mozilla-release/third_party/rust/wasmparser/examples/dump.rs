extern crate wasmparser;

use std::env;
use std::fs::File;
use std::io;
use std::io::prelude::*;
use std::str;
use wasmparser::Parser;
use wasmparser::ParserState;
<<<<<<< HEAD
use wasmparser::WasmDecoder;

fn get_name(bytes: &[u8]) -> &str {
    str::from_utf8(bytes).ok().unwrap()
}
||||||| merged common ancestors

fn get_name(bytes: &[u8]) -> &str {
    str::from_utf8(bytes).ok().unwrap()
}
=======
use wasmparser::WasmDecoder;
>>>>>>> upstream-releases

fn main() {
    let args = env::args().collect::<Vec<_>>();
    if args.len() != 2 {
        println!("Usage: {} in.wasm", args[0]);
        return;
    }

    let buf: Vec<u8> = read_wasm(&args[1]).unwrap();
    let mut parser = Parser::new(&buf);
    loop {
        print!("0x{:08x}\t", parser.current_position());
        let state = parser.read();
        match *state {
            ParserState::ExportSectionEntry {
                field,
                ref kind,
                index,
            } => {
<<<<<<< HEAD
                println!(
                    "ExportSectionEntry {{ field: \"{}\", kind: {:?}, index: {} }}",
                    get_name(field),
                    kind,
                    index
                );
||||||| merged common ancestors
                println!("ExportSectionEntry {{ field: \"{}\", kind: {:?}, index: {} }}",
                         get_name(field),
                         kind,
                         index);
=======
                println!(
                    "ExportSectionEntry {{ field: \"{}\", kind: {:?}, index: {} }}",
                    field, kind, index
                );
>>>>>>> upstream-releases
            }
            ParserState::ImportSectionEntry {
                module,
                field,
                ref ty,
            } => {
<<<<<<< HEAD
                println!(
                    "ImportSectionEntry {{ module: \"{}\", field: \"{}\", ty: {:?} }}",
                    get_name(module),
                    get_name(field),
                    ty
                );
||||||| merged common ancestors
                println!("ImportSectionEntry {{ module: \"{}\", field: \"{}\", ty: {:?} }}",
                         get_name(module),
                         get_name(field),
                         ty);
=======
                println!(
                    "ImportSectionEntry {{ module: \"{}\", field: \"{}\", ty: {:?} }}",
                    module, field, ty
                );
>>>>>>> upstream-releases
            }
            ParserState::EndWasm => break,
            ParserState::Error(err) => panic!("Error: {:?}", err),
            _ => println!("{:?}", state),
        }
    }
}

fn read_wasm(file: &str) -> io::Result<Vec<u8>> {
    let mut data = Vec::new();
    let mut f = File::open(file)?;
    f.read_to_end(&mut data)?;
    Ok(data)
}
