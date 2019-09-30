#[macro_use]
extern crate darling;
#[macro_use]
extern crate syn;
#[macro_use]
extern crate quote;

use darling::FromDeriveInput;

#[derive(FromDeriveInput)]
#[darling(attributes(hello))]
#[allow(dead_code)]
struct Lorem {
    ident: syn::Ident,
    ipsum: Ipsum,
}

#[derive(FromMeta)]
struct Ipsum {
    #[darling(multiple)]
    dolor: Vec<String>,
}

#[test]
fn expand_many() {
<<<<<<< HEAD
    let di = syn::parse_str(
        r#"
||||||| merged common ancestors
    let di = syn::parse_str(r#"
=======
    let di = parse_quote! {
>>>>>>> upstream-releases
        #[hello(ipsum(dolor = "Hello", dolor = "World"))]
        pub struct Baz;
<<<<<<< HEAD
    "#,
    ).unwrap();
||||||| merged common ancestors
    "#).unwrap();
=======
    };
>>>>>>> upstream-releases

    let lorem: Lorem = Lorem::from_derive_input(&di).unwrap();
    assert_eq!(
        lorem.ipsum.dolor,
        vec!["Hello".to_string(), "World".to_string()]
    );
}
