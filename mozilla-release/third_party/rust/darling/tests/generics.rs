#[macro_use]
extern crate darling;
#[macro_use]
extern crate syn;
#[macro_use]
extern crate quote;

use darling::FromDeriveInput;

#[derive(Debug, Clone, FromMeta)]
struct Wrapper<T>(pub T);

#[derive(Debug, FromDeriveInput)]
#[darling(attributes(hello))]
struct Foo<T> {
    lorem: Wrapper<T>,
}

#[test]
fn expansion() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[hello(lorem = "Hello")]
        pub struct Foo;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#)
        .unwrap();
=======
    };
>>>>>>> upstream-releases

    Foo::<String>::from_derive_input(&di).unwrap();
}
