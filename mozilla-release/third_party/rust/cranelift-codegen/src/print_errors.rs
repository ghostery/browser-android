//! Utility routines for pretty-printing error messages.

<<<<<<< HEAD
use entity::SecondaryMap;
use ir;
use ir::entities::{AnyEntity, Ebb, Inst, Value};
use ir::function::Function;
use isa::TargetIsa;
use result::CodegenError;
||||||| merged common ancestors
use ir;
use ir::entities::{AnyEntity, Inst};
use ir::function::Function;
use isa::TargetIsa;
use result::CodegenError;
=======
use crate::entity::SecondaryMap;
use crate::ir;
use crate::ir::entities::{AnyEntity, Ebb, Inst, Value};
use crate::ir::function::Function;
use crate::isa::TargetIsa;
use crate::result::CodegenError;
use crate::verifier::{VerifierError, VerifierErrors};
use crate::write::{decorate_function, FuncWriter, PlainWriter};
use core::fmt;
use core::fmt::Write;
>>>>>>> upstream-releases
use std::boxed::Box;
use std::string::{String, ToString};
use std::vec::Vec;

/// Pretty-print a verifier error.
pub fn pretty_verifier_error<'a>(
    func: &ir::Function,
    isa: Option<&dyn TargetIsa>,
    func_w: Option<Box<dyn FuncWriter + 'a>>,
    errors: VerifierErrors,
) -> String {
    let mut errors = errors.0;
    let mut w = String::new();
    let num_errors = errors.len();

    decorate_function(
        &mut PrettyVerifierError(func_w.unwrap_or_else(|| Box::new(PlainWriter)), &mut errors),
        &mut w,
        func,
<<<<<<< HEAD
        isa,
    ).unwrap();

    writeln!(
        w,
        "\n; {} verifier error{} detected (see above). Compilation aborted.",
        num_errors,
        if num_errors == 1 { "" } else { "s" }
    ).unwrap();

||||||| merged common ancestors
        isa,
    ).unwrap();
=======
        &isa.into(),
    )
    .unwrap();

    writeln!(
        w,
        "\n; {} verifier error{} detected (see above). Compilation aborted.",
        num_errors,
        if num_errors == 1 { "" } else { "s" }
    )
    .unwrap();

>>>>>>> upstream-releases
    w
}

struct PrettyVerifierError<'a>(Box<dyn FuncWriter + 'a>, &'a mut Vec<VerifierError>);

impl<'a> FuncWriter for PrettyVerifierError<'a> {
<<<<<<< HEAD
    fn write_ebb_header(
        &mut self,
        w: &mut Write,
        func: &Function,
        isa: Option<&TargetIsa>,
        ebb: Ebb,
        indent: usize,
    ) -> fmt::Result {
        pretty_ebb_header_error(w, func, isa, ebb, indent, &mut *self.0, self.1)
    }

||||||| merged common ancestors
=======
    fn write_ebb_header(
        &mut self,
        w: &mut dyn Write,
        func: &Function,
        isa: Option<&dyn TargetIsa>,
        ebb: Ebb,
        indent: usize,
    ) -> fmt::Result {
        pretty_ebb_header_error(w, func, isa, ebb, indent, &mut *self.0, self.1)
    }

>>>>>>> upstream-releases
    fn write_instruction(
        &mut self,
        w: &mut dyn Write,
        func: &Function,
<<<<<<< HEAD
        aliases: &SecondaryMap<Value, Vec<Value>>,
        isa: Option<&TargetIsa>,
||||||| merged common ancestors
        isa: Option<&TargetIsa>,
=======
        aliases: &SecondaryMap<Value, Vec<Value>>,
        isa: Option<&dyn TargetIsa>,
>>>>>>> upstream-releases
        inst: Inst,
        indent: usize,
    ) -> fmt::Result {
        pretty_instruction_error(w, func, aliases, isa, inst, indent, &mut *self.0, self.1)
    }

    fn write_entity_definition(
        &mut self,
        w: &mut dyn Write,
        func: &Function,
        entity: AnyEntity,
        value: &dyn fmt::Display,
    ) -> fmt::Result {
        pretty_preamble_error(w, func, entity, value, &mut *self.0, self.1)
    }
}

<<<<<<< HEAD
/// Pretty-print a function verifier error for a given EBB.
fn pretty_ebb_header_error(
    w: &mut Write,
||||||| merged common ancestors
/// Pretty-print a function verifier error.
fn pretty_instruction_error(
    w: &mut Write,
=======
/// Pretty-print a function verifier error for a given EBB.
fn pretty_ebb_header_error(
    w: &mut dyn Write,
>>>>>>> upstream-releases
    func: &Function,
<<<<<<< HEAD
    isa: Option<&TargetIsa>,
    cur_ebb: Ebb,
||||||| merged common ancestors
    isa: Option<&TargetIsa>,
    cur_inst: Inst,
=======
    isa: Option<&dyn TargetIsa>,
    cur_ebb: Ebb,
>>>>>>> upstream-releases
    indent: usize,
    func_w: &mut dyn FuncWriter,
    errors: &mut Vec<VerifierError>,
) -> fmt::Result {
    let mut s = String::new();
    func_w.write_ebb_header(&mut s, func, isa, cur_ebb, indent)?;
    write!(w, "{}", s)?;

    // TODO: Use drain_filter here when it gets stabilized
    let mut i = 0;
    let mut printed_error = false;
    while i != errors.len() {
        match errors[i].location {
            ir::entities::AnyEntity::Ebb(ebb) if ebb == cur_ebb => {
                if !printed_error {
                    print_arrow(w, &s)?;
                    printed_error = true;
                }
                let err = errors.remove(i);
                print_error(w, err)?;
            }
            _ => i += 1,
        }
    }

<<<<<<< HEAD
    if printed_error {
        w.write_char('\n')?;
    }

    Ok(())
}

/// Pretty-print a function verifier error for a given instruction.
fn pretty_instruction_error(
    w: &mut Write,
    func: &Function,
    aliases: &SecondaryMap<Value, Vec<Value>>,
    isa: Option<&TargetIsa>,
    cur_inst: Inst,
    indent: usize,
    func_w: &mut FuncWriter,
    errors: &mut Vec<VerifierError>,
) -> fmt::Result {
    let mut s = String::new();
    func_w.write_instruction(&mut s, func, aliases, isa, cur_inst, indent)?;
    write!(w, "{}", s)?;
||||||| merged common ancestors
                if !printed_instr {
                    func_w.write_instruction(w, func, isa, cur_inst, indent)?;
                    printed_instr = true;
                }
=======
    if printed_error {
        w.write_char('\n')?;
    }

    Ok(())
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    // TODO: Use drain_filter here when it gets stabilized
    let mut i = 0;
    let mut printed_error = false;
    while i != errors.len() {
        match errors[i].location {
            ir::entities::AnyEntity::Inst(inst) if inst == cur_inst => {
                if !printed_error {
                    print_arrow(w, &s)?;
                    printed_error = true;
||||||| merged common ancestors
                write!(w, "{1:0$}^", indent, "")?;
                for _c in cur_inst.to_string().chars() {
                    write!(w, "~")?;
=======
/// Pretty-print a function verifier error for a given instruction.
fn pretty_instruction_error(
    w: &mut dyn Write,
    func: &Function,
    aliases: &SecondaryMap<Value, Vec<Value>>,
    isa: Option<&dyn TargetIsa>,
    cur_inst: Inst,
    indent: usize,
    func_w: &mut dyn FuncWriter,
    errors: &mut Vec<VerifierError>,
) -> fmt::Result {
    let mut s = String::new();
    func_w.write_instruction(&mut s, func, aliases, isa, cur_inst, indent)?;
    write!(w, "{}", s)?;

    // TODO: Use drain_filter here when it gets stabilized
    let mut i = 0;
    let mut printed_error = false;
    while i != errors.len() {
        match errors[i].location {
            ir::entities::AnyEntity::Inst(inst) if inst == cur_inst => {
                if !printed_error {
                    print_arrow(w, &s)?;
                    printed_error = true;
>>>>>>> upstream-releases
                }
                let err = errors.remove(i);
                print_error(w, err)?;
            }
            _ => i += 1,
        }
    }

    if printed_error {
        w.write_char('\n')?;
    }

    Ok(())
}

fn pretty_preamble_error(
    w: &mut dyn Write,
    func: &Function,
    entity: AnyEntity,
    value: &dyn fmt::Display,
    func_w: &mut dyn FuncWriter,
    errors: &mut Vec<VerifierError>,
) -> fmt::Result {
    let mut s = String::new();
    func_w.write_entity_definition(&mut s, func, entity, value)?;
    write!(w, "{}", s)?;

    // TODO: Use drain_filter here when it gets stabilized
    let mut i = 0;
    let mut printed_error = false;
    while i != errors.len() {
        if entity == errors[i].location {
<<<<<<< HEAD
            if !printed_error {
                print_arrow(w, &s)?;
                printed_error = true;
            }
            let err = errors.remove(i);
            print_error(w, err)?;
||||||| merged common ancestors
            let err = errors.remove(i);

            if !printed_entity {
                func_w.write_entity_definition(w, func, entity, value)?;
                printed_entity = true;
            }

            write!(w, "{1:0$}^", indent, "")?;
            for _c in entity.to_string().chars() {
                write!(w, "~")?;
            }
            writeln!(w, " verifier {}", err.to_string())?;
=======
            if !printed_error {
                print_arrow(w, &s)?;
                printed_error = true;
            }
            let err = errors.remove(i);
            print_error(w, err)?;
>>>>>>> upstream-releases
        } else {
            i += 1
        }
    }

    if printed_error {
        w.write_char('\n')?;
    }

    Ok(())
}

<<<<<<< HEAD
/// Prints:
///    ;   ^~~~~~
fn print_arrow(w: &mut Write, entity: &str) -> fmt::Result {
    write!(w, ";")?;

    let indent = entity.len() - entity.trim_start().len();
    if indent != 0 {
        write!(w, "{1:0$}^", indent - 1, "")?;
    }

    for _ in 0..entity.trim().len() - 1 {
        write!(w, "~")?;
    }

    writeln!(w)
}

/// Prints:
///    ; error: [ERROR BODY]
fn print_error(w: &mut Write, err: VerifierError) -> fmt::Result {
    writeln!(w, "; error: {}", err.to_string())?;
    Ok(())
}

||||||| merged common ancestors
=======
/// Prints:
///    ;   ^~~~~~
fn print_arrow(w: &mut dyn Write, entity: &str) -> fmt::Result {
    write!(w, ";")?;

    let indent = entity.len() - entity.trim_start().len();
    if indent != 0 {
        write!(w, "{1:0$}^", indent - 1, "")?;
    }

    for _ in 0..entity.trim().len() - 1 {
        write!(w, "~")?;
    }

    writeln!(w)
}

/// Prints:
///    ; error: [ERROR BODY]
fn print_error(w: &mut dyn Write, err: VerifierError) -> fmt::Result {
    writeln!(w, "; error: {}", err.to_string())?;
    Ok(())
}

>>>>>>> upstream-releases
/// Pretty-print a Cranelift error.
pub fn pretty_error(func: &ir::Function, isa: Option<&dyn TargetIsa>, err: CodegenError) -> String {
    if let CodegenError::Verifier(e) = err {
        pretty_verifier_error(func, isa, None, e)
    } else {
        err.to_string()
    }
}
