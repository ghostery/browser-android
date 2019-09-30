// Build script.
//
// This program is run by Cargo when building cranelift-codegen. It is used to generate Rust code from
// the language definitions in the cranelift-codegen/meta directory.
//
// Environment:
//
// OUT_DIR
//     Directory where generated files should be placed.
//
// TARGET
//     Target triple provided by Cargo.
//
// The build script expects to be run from the directory where this build.rs file lives. The
// current directory is used to find the sources.

use cranelift_codegen_meta as meta;

use meta::isa::Isa;
use std::env;
use std::process;
use std::time::Instant;

fn main() {
    let start_time = Instant::now();

    let out_dir = env::var("OUT_DIR").expect("The OUT_DIR environment variable must be set");
    let target_triple = env::var("TARGET").expect("The TARGET environment variable must be set");

    // Configure isa targets cfg.
<<<<<<< HEAD
    match isa_targets(cranelift_targets, &target_triple) {
        Ok(isa_targets) => {
            for isa in &isa_targets {
                println!("cargo:rustc-cfg=build_{}", isa.to_string());
            }
        }
        Err(err) => {
            eprintln!("Error: {}", err);
            process::exit(1);
        }
    }

||||||| merged common ancestors
    match isa_targets(cranelift_targets, &target_triple) {
        Ok(isa_targets) => {
            for isa in &isa_targets {
                println!("cargo:rustc-cfg=build_{}", isa.name());
            }
        }
        Err(err) => {
            eprintln!("Error: {}", err);
            process::exit(1);
        }
    }

    println!("Build script generating files in {}", out_dir);

=======
    let isa_targets = meta::isa::Isa::all()
        .into_iter()
        .cloned()
        .filter(|isa| {
            let env_key = format!("CARGO_FEATURE_{}", isa.to_string().to_uppercase());
            env::var(env_key).is_ok()
        })
        .collect::<Vec<_>>();

    let isas = if isa_targets.is_empty() {
        // Try to match native target.
        let target_name = target_triple.split('-').next().unwrap();
        let isa = meta::isa_from_arch(&target_name).expect("error when identifying target");
        println!("cargo:rustc-cfg=feature=\"{}\"", isa);
        vec![isa]
    } else {
        isa_targets
    };

>>>>>>> upstream-releases
    let cur_dir = env::current_dir().expect("Can't access current working directory");
    let crate_dir = cur_dir.as_path();

    // Make sure we rebuild if this build script changes (will not happen with
    // if the path to this file contains non-UTF8 bytes). The `build.py` script
    // prints out its own dependencies.
    println!(
        "cargo:rerun-if-changed={}",
        crate_dir.join("build.rs").to_str().unwrap()
    );

    // Scripts are in `$crate_dir/meta-python`.
    let meta_dir = crate_dir.join("meta-python");
    let build_script = meta_dir.join("build.py");

    // Launch build script with Python. We'll just find python in the path.
    // Use -B to disable .pyc files, because they cause trouble for vendoring
    // scripts, and this is a build step that isn't run very often anyway.
    let python = identify_python();
    let status = process::Command::new(python)
        .current_dir(crate_dir)
        .arg("-B")
        .arg(build_script)
        .arg("--out-dir")
        .arg(out_dir.clone())
        .status()
        .expect("Failed to launch second-level build script; is python installed?");
    if !status.success() {
        process::exit(status.code().unwrap());
    }

    // DEVELOPMENT:
    // ------------------------------------------------------------------------
    // Now that the Python build process is complete, generate files that are
    // emitted by the `meta` crate.
    // ------------------------------------------------------------------------
<<<<<<< HEAD
    let isas = meta::isa::define_all();

    if let Err(err) = meta::gen_types::generate("types.rs", &out_dir) {
||||||| merged common ancestors
    if let Err(err) = meta::gen_types::generate("new_types.rs", &out_dir) {
=======

    if let Err(err) = meta::generate(&isas, &out_dir) {
>>>>>>> upstream-releases
        eprintln!("Error: {}", err);
        process::exit(1);
    }
<<<<<<< HEAD

    for isa in isas {
        if let Err(err) = meta::gen_registers::generate(isa, "registers", &out_dir) {
            eprintln!("Error: {}", err);
            process::exit(1);
        }
    }
||||||| merged common ancestors
=======

    if let Ok(_) = env::var("CRANELIFT_VERBOSE") {
        for isa in &isas {
            println!("cargo:warning=Includes support for {} ISA", isa.to_string());
        }
        println!(
            "cargo:warning=Build step took {:?}.",
            Instant::now() - start_time
        );
        println!("cargo:warning=Generated files are in {}", out_dir);
    }
>>>>>>> upstream-releases
}

fn identify_python() -> &'static str {
    for python in &["python", "python3", "python2.7"] {
        if process::Command::new(python)
            .arg("--version")
            .status()
            .is_ok()
        {
            return python;
        }
    }
    panic!("The Cranelift build requires Python (version 2.7 or version 3)");
}
<<<<<<< HEAD

/// Returns isa targets to configure conditional compilation.
fn isa_targets(cranelift_targets: Option<&str>, target_triple: &str) -> Result<Vec<Isa>, String> {
    match cranelift_targets {
        Some("native") => Isa::from_arch(target_triple.split('-').next().unwrap())
            .map(|isa| vec![isa])
            .ok_or_else(|| {
                format!(
                    "no supported isa found for target triple `{}`",
                    target_triple
                )
            }),
        Some(targets) => {
            let unknown_isa_targets = targets
                .split(',')
                .filter(|target| Isa::new(target).is_none())
                .collect::<Vec<_>>();
            let isa_targets = targets.split(',').flat_map(Isa::new).collect::<Vec<_>>();
            match (unknown_isa_targets.is_empty(), isa_targets.is_empty()) {
                (true, true) => Ok(Isa::all().to_vec()),
                (true, _) => Ok(isa_targets),
                (_, _) => Err(format!(
                    "unknown isa targets: `{}`",
                    unknown_isa_targets.join(", ")
                )),
            }
        }
        None => Ok(Isa::all().to_vec()),
    }
}
||||||| merged common ancestors

/// Represents known ISA target.
#[derive(Copy, Clone)]
enum Isa {
    Riscv,
    X86,
    Arm32,
    Arm64,
}

impl Isa {
    /// Creates isa target using name.
    fn new(name: &str) -> Option<Self> {
        Isa::all()
            .iter()
            .cloned()
            .filter(|isa| isa.name() == name)
            .next()
    }

    /// Creates isa target from arch.
    fn from_arch(arch: &str) -> Option<Isa> {
        Isa::all()
            .iter()
            .cloned()
            .filter(|isa| isa.is_arch_applicable(arch))
            .next()
    }

    /// Returns all supported isa targets.
    fn all() -> [Isa; 4] {
        [Isa::Riscv, Isa::X86, Isa::Arm32, Isa::Arm64]
    }

    /// Returns name of the isa target.
    fn name(&self) -> &'static str {
        match *self {
            Isa::Riscv => "riscv",
            Isa::X86 => "x86",
            Isa::Arm32 => "arm32",
            Isa::Arm64 => "arm64",
        }
    }

    /// Checks if arch is applicable for the isa target.
    fn is_arch_applicable(&self, arch: &str) -> bool {
        match *self {
            Isa::Riscv => arch == "riscv",
            Isa::X86 => ["x86_64", "i386", "i586", "i686"].contains(&arch),
            Isa::Arm32 => arch.starts_with("arm") || arch.starts_with("thumb"),
            Isa::Arm64 => arch == "aarch64",
        }
    }
}

/// Returns isa targets to configure conditional compilation.
fn isa_targets(cranelift_targets: Option<&str>, target_triple: &str) -> Result<Vec<Isa>, String> {
    match cranelift_targets {
        Some("native") => Isa::from_arch(target_triple.split('-').next().unwrap())
            .map(|isa| vec![isa])
            .ok_or_else(|| {
                format!(
                    "no supported isa found for target triple `{}`",
                    target_triple
                )
            }),
        Some(targets) => {
            let unknown_isa_targets = targets
                .split(',')
                .filter(|target| Isa::new(target).is_none())
                .collect::<Vec<_>>();
            let isa_targets = targets.split(',').flat_map(Isa::new).collect::<Vec<_>>();
            match (unknown_isa_targets.is_empty(), isa_targets.is_empty()) {
                (true, true) => Ok(Isa::all().to_vec()),
                (true, _) => Ok(isa_targets),
                (_, _) => Err(format!(
                    "unknown isa targets: `{}`",
                    unknown_isa_targets.join(", ")
                )),
            }
        }
        None => Ok(Isa::all().to_vec()),
    }
}
=======
>>>>>>> upstream-releases
