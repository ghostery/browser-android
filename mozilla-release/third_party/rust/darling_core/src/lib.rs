#![recursion_limit = "256"]
#![cfg_attr(feature = "diagnostics", feature(proc_macro_diagnostic))]

#[macro_use]
extern crate quote;
#[macro_use]
extern crate syn;
<<<<<<< HEAD
extern crate proc_macro2;

extern crate fnv;
||||||| merged common ancestors
extern crate proc_macro2;

=======
extern crate fnv;
>>>>>>> upstream-releases
extern crate ident_case;
#[cfg(feature = "diagnostics")]
extern crate proc_macro;
extern crate proc_macro2;
#[cfg(feature = "suggestions")]
extern crate strsim;

#[macro_use]
mod macros_private;
#[macro_use]
mod macros_public;

pub mod ast;
pub(crate) mod codegen;
pub mod derive;
pub mod error;
mod from_derive_input;
mod from_field;
mod from_generic_param;
mod from_generics;
mod from_meta;
mod from_type_param;
mod from_variant;
<<<<<<< HEAD
pub mod options;
pub mod usage;
||||||| merged common ancestors
pub mod options;
=======
pub(crate) mod options;
pub mod usage;
>>>>>>> upstream-releases
pub mod util;

pub use error::{Error, Result};
pub use from_derive_input::FromDeriveInput;
pub use from_field::FromField;
pub use from_generic_param::FromGenericParam;
pub use from_generics::FromGenerics;
pub use from_meta::FromMeta;
pub use from_type_param::FromTypeParam;
pub use from_variant::FromVariant;

<<<<<<< HEAD
// Re-export tokenizer
#[doc(hidden)]
pub use quote::ToTokens;
||||||| merged common ancestors
#[cfg(test)]
mod tests {
    
}
=======
// Re-export tokenizer
#[doc(hidden)]
pub use quote::ToTokens;
>>>>>>> upstream-releases
