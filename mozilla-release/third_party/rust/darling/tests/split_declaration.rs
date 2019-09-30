//! When input is split across multiple attributes on one element,
//! darling should collapse that into one struct.

#[macro_use]
extern crate darling;
#[macro_use]
extern crate syn;
#[macro_use]
extern crate quote;

use std::string::ToString;

use darling::{Error, FromDeriveInput};

#[derive(Debug, FromDeriveInput, PartialEq, Eq)]
#[darling(attributes(split))]
struct Lorem {
    foo: String,
    bar: bool,
}

#[test]
fn split_attributes_accrue_to_instance() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[split(foo = "Hello")]
        #[split(bar)]
        pub struct Foo;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

    let parsed = Lorem::from_derive_input(&di).unwrap();
    assert_eq!(
        parsed,
        Lorem {
            foo: "Hello".to_string(),
            bar: true,
        }
    );
}

#[test]
fn duplicates_across_split_attrs_error() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[split(foo = "Hello")]
        #[split(foo = "World", bar)]
        pub struct Foo;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

<<<<<<< HEAD
    let pr = Lorem::from_derive_input(&di);
    assert_eq!(
        pr.unwrap_err().to_string(),
        Error::duplicate_field("foo").to_string()
    );
||||||| merged common ancestors
    let pr = Lorem::from_derive_input(&di);
    assert_eq!(pr.unwrap_err().to_string(), Error::duplicate_field("foo").to_string());
=======
    let pr = Lorem::from_derive_input(&di).unwrap_err();
    assert!(pr.has_span());
    assert_eq!(
        pr.to_string(),
        Error::duplicate_field("foo").to_string()
    );
>>>>>>> upstream-releases
}

#[test]
fn multiple_errors_accrue_to_instance() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[split(foo = "Hello")]
        #[split(foo = "World")]
        pub struct Foo;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

    let pr = Lorem::from_derive_input(&di);
    let err: Error = pr.unwrap_err();
    assert_eq!(2, err.len());
<<<<<<< HEAD
    let mut errs = err.into_iter();
    assert_eq!(
        errs.next().unwrap().to_string(),
        Error::duplicate_field("foo").to_string()
    );
    assert_eq!(
        errs.next().unwrap().to_string(),
        Error::missing_field("bar").to_string()
    );
||||||| merged common ancestors
    let mut errs = err.into_iter();
    assert_eq!(errs.next().unwrap().to_string(), Error::duplicate_field("foo").to_string());
    assert_eq!(errs.next().unwrap().to_string(), Error::missing_field("bar").to_string());
=======
    let mut errs = err.into_iter().peekable();
    assert_eq!(
        errs.peek().unwrap().to_string(),
        Error::duplicate_field("foo").to_string()
    );
    assert!(errs.next().unwrap().has_span());
    assert_eq!(
        errs.next().unwrap().to_string(),
        Error::missing_field("bar").to_string()
    );
>>>>>>> upstream-releases
    assert!(errs.next().is_none());
}
