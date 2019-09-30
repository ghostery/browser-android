#[macro_use]
extern crate darling;
#[macro_use]
extern crate syn;
#[macro_use]
extern crate quote;

use darling::FromDeriveInput;

<<<<<<< HEAD
extern crate syn;

#[derive(Debug, Default, PartialEq, Eq, FromMeta)]
||||||| merged common ancestors
extern crate syn;

#[derive(Debug, Default, PartialEq, Eq, FromMetaItem)]
=======
#[derive(Debug, Default, PartialEq, Eq, FromMeta)]
>>>>>>> upstream-releases
#[darling(default)]
pub struct Amet {
    hello: bool,
    world: String,
}

#[derive(Debug, PartialEq, Eq, FromMeta)]
#[darling(rename_all = "snake_case")]
pub enum Lorem {
    Ipsum(bool),
    Dolor(String),
    Sit(Amet),
}

#[derive(Debug, PartialEq, Eq, FromDeriveInput)]
#[darling(attributes(hello))]
pub struct Holder {
    lorem: Lorem,
}

impl PartialEq<Lorem> for Holder {
    fn eq(&self, other: &Lorem) -> bool {
        self.lorem == *other
    }
}

#[test]
fn bool_word() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[hello(lorem(ipsum))]
        pub struct Bar;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

    let pr = Holder::from_derive_input(&di).unwrap();
    assert_eq!(pr, Lorem::Ipsum(true));
}

#[test]
fn bool_literal() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[hello(lorem(ipsum = false))]
        pub struct Bar;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

    let pr = Holder::from_derive_input(&di).unwrap();
    assert_eq!(pr, Lorem::Ipsum(false));
}

#[test]
fn string_literal() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[hello(lorem(dolor = "Hello"))]
        pub struct Bar;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

    let pr = Holder::from_derive_input(&di).unwrap();
    assert_eq!(pr, Lorem::Dolor("Hello".to_string()));
}

#[test]
fn struct_nested() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[hello(lorem(sit(world = "Hello", hello = false)))]
        pub struct Bar;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

    let pr = Holder::from_derive_input(&di).unwrap();
    assert_eq!(
        pr,
        Lorem::Sit(Amet {
            hello: false,
            world: "Hello".to_string(),
        })
    );
}

#[test]
#[should_panic]
fn format_mismatch() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[hello(lorem(dolor(world = "Hello", hello = false)))]
        pub struct Bar;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

    Holder::from_derive_input(&di).unwrap();
}
