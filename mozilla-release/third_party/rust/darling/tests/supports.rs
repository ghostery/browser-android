#[macro_use]
extern crate darling;
#[macro_use]
extern crate syn;
#[macro_use]
extern crate quote;

use darling::ast;
use darling::FromDeriveInput;

#[derive(Debug, FromDeriveInput)]
#[darling(attributes(from_variants), supports(enum_any))]
pub struct Container {
    data: ast::Data<Variant, ()>,
}

#[derive(Default, Debug, FromVariant)]
#[darling(default, attributes(from_variants), supports(newtype, unit))]
pub struct Variant {
    into: Option<bool>,
    skip: Option<bool>,
}

<<<<<<< HEAD
#[derive(Debug, FromDeriveInput)]
#[darling(attributes(from_struct), supports(struct_named))]
pub struct StructContainer {
    data: ast::Data<(), syn::Field>,
}

mod source {
    use syn::{self, DeriveInput};

    pub fn newtype_enum() -> DeriveInput {
        syn::parse_str(
            r#"
        enum Hello {
            World(bool),
            String(String),
||||||| merged common ancestors
#[test]
fn expansion() {
    let di = syn::parse_str(r#"
        enum Hello {
            World(bool),
            String(String),
=======
#[derive(Debug, FromDeriveInput)]
#[darling(attributes(from_struct), supports(struct_named))]
pub struct StructContainer {
    data: ast::Data<(), syn::Field>,
}

mod source {
    use syn::DeriveInput;

    pub fn newtype_enum() -> DeriveInput {
        parse_quote!{
            enum Hello {
                World(bool),
                String(String),
            }
        }
    }

    pub fn named_field_enum() -> DeriveInput {
        parse_quote! {
            enum Hello {
                Foo(u16),
                World {
                    name: String
                },
            }
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
    "#,
        ).unwrap()
    }
||||||| merged common ancestors
    "#).unwrap();
=======
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    pub fn named_field_enum() -> DeriveInput {
        syn::parse_str(
            r#"
        enum Hello {
            Foo(u16),
            World {
                name: String
            },
        }
    "#,
        ).unwrap()
    }

    pub fn named_struct() -> DeriveInput {
        syn::parse_str(
            r#"
        struct Hello {
            world: bool,
        }
    "#,
        ).unwrap()
    }

    pub fn tuple_struct() -> DeriveInput {
        syn::parse_str("struct Hello(String, bool);").unwrap()
    }
}

#[test]
fn enum_newtype_or_unit() {
    // Should pass
    Container::from_derive_input(&source::newtype_enum()).unwrap();

    // Should error
    Container::from_derive_input(&source::named_field_enum()).unwrap_err();
    Container::from_derive_input(&source::named_struct()).unwrap_err();
}

#[test]
fn struct_named() {
    // Should pass
    StructContainer::from_derive_input(&source::named_struct()).unwrap();
||||||| merged common ancestors
    Container::from_derive_input(&di).unwrap();
}

#[test]
fn unsupported_shape() {
    let di = syn::parse_str(r#"
        enum Hello {
            Foo(u16),
            World {
                name: String
            },
        }
    "#).unwrap();
=======
    pub fn named_struct() -> DeriveInput {
        parse_quote! {
            struct Hello {
                world: bool,
            }
        }
    }

    pub fn tuple_struct() -> DeriveInput {
        parse_quote! { struct Hello(String, bool); }
    }
}

#[test]
fn enum_newtype_or_unit() {
    // Should pass
    Container::from_derive_input(&source::newtype_enum()).unwrap();

    // Should error
    Container::from_derive_input(&source::named_field_enum()).unwrap_err();
    Container::from_derive_input(&source::named_struct()).unwrap_err();
}

#[test]
fn struct_named() {
    // Should pass
    StructContainer::from_derive_input(&source::named_struct()).unwrap();
>>>>>>> upstream-releases

    // Should fail
    StructContainer::from_derive_input(&source::tuple_struct()).unwrap_err();
    StructContainer::from_derive_input(&source::named_field_enum()).unwrap_err();
    StructContainer::from_derive_input(&source::newtype_enum()).unwrap_err();
}
