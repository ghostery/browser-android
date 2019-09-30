extern crate proc_macro;
#[macro_use]
<<<<<<< HEAD
extern crate quote;
#[macro_use]
||||||| merged common ancestors
extern crate quote;
=======
>>>>>>> upstream-releases
extern crate syn;

extern crate darling_core;

use proc_macro::TokenStream;

<<<<<<< HEAD
use darling_core::{codegen, options};
||||||| merged common ancestors
use darling_core::{options, codegen};
=======
use darling_core::{derive, Error};
>>>>>>> upstream-releases

<<<<<<< HEAD
#[proc_macro_derive(FromMeta, attributes(darling))]
pub fn derive_from_meta(input: TokenStream) -> TokenStream {
    let ast = parse_macro_input!(input as syn::DeriveInput);

    let container = options::FromMetaOptions::new(&ast).unwrap();
    let trait_impl = codegen::FromMetaImpl::from(&container);
    let result = quote!(#trait_impl);
||||||| merged common ancestors
#[proc_macro_derive(FromMetaItem, attributes(darling))]
pub fn derive_from_meta_item(input: TokenStream) -> TokenStream {
    let ast: syn::DeriveInput = syn::parse(input).unwrap();
    let container = options::FmiOptions::new(&ast).unwrap();
    let trait_impl = codegen::FmiImpl::from(&container);
    let result = quote!(#trait_impl);
=======
#[proc_macro_derive(FromMeta, attributes(darling))]
pub fn derive_from_meta(input: TokenStream) -> TokenStream {
    derive::from_meta(&parse_macro_input!(input)).into()
}
>>>>>>> upstream-releases

#[proc_macro_derive(FromMetaItem, attributes(darling))]
pub fn derive_from_meta_item(_input: TokenStream) -> TokenStream {
    Error::custom("darling::FromMetaItem has been replaced by darling::FromMeta")
        .write_errors()
        .into()
}

#[proc_macro_derive(FromMetaItem, attributes(darling))]
pub fn derive_from_meta_item(_input: TokenStream) -> TokenStream {
    panic!("darling::FromMetaItem has been replaced by darling::FromMeta");
}


#[proc_macro_derive(FromDeriveInput, attributes(darling))]
pub fn derive_from_input(input: TokenStream) -> TokenStream {
<<<<<<< HEAD
    let ast = parse_macro_input!(input as syn::DeriveInput);

    let container = options::FdiOptions::new(&ast).unwrap();
    let trait_impl = codegen::FromDeriveInputImpl::from(&container);
    let result = quote!(#trait_impl);

    result.into()
||||||| merged common ancestors
    let ast: syn::DeriveInput = syn::parse(input).unwrap();

    let container = options::FdiOptions::new(&ast).unwrap();
    let trait_impl = codegen::FromDeriveInputImpl::from(&container);
    let result = quote!(#trait_impl);

    result.into()
=======
    derive::from_derive_input(&parse_macro_input!(input)).into()
>>>>>>> upstream-releases
}

#[proc_macro_derive(FromField, attributes(darling))]
pub fn derive_field(input: TokenStream) -> TokenStream {
<<<<<<< HEAD
    let ast = parse_macro_input!(input as syn::DeriveInput);

    let fdic = options::FromFieldOptions::new(&ast).unwrap();
    let trait_impl = codegen::FromFieldImpl::from(&fdic);
    let result = quote!(#trait_impl);
||||||| merged common ancestors
    let ast: syn::DeriveInput = syn::parse(input).unwrap();

    let fdic = options::FromFieldOptions::new(&ast).unwrap();
    let trait_impl = codegen::FromFieldImpl::from(&fdic);
    let result = quote!(#trait_impl);
=======
    derive::from_field(&parse_macro_input!(input)).into()
}
>>>>>>> upstream-releases

#[proc_macro_derive(FromTypeParam, attributes(darling))]
pub fn derive_type_param(input: TokenStream) -> TokenStream {
    derive::from_type_param(&parse_macro_input!(input)).into()
}

#[proc_macro_derive(FromTypeParam, attributes(darling))]
pub fn derive_type_param(input: TokenStream) -> TokenStream {
    let ast = parse_macro_input!(input as syn::DeriveInput);

    let fdic = options::FromTypeParamOptions::new(&ast).unwrap();
    let trait_impl = codegen::FromTypeParamImpl::from(&fdic);
    let result = quote!(#trait_impl);

    result.into()
}

#[proc_macro_derive(FromVariant, attributes(darling))]
pub fn derive_variant(input: TokenStream) -> TokenStream {
<<<<<<< HEAD
    let ast = parse_macro_input!(input as syn::DeriveInput);

    let fdic = options::FromVariantOptions::new(&ast).unwrap();
    let trait_impl = codegen::FromVariantImpl::from(&fdic);
    let result = quote!(#trait_impl);

    result.into()
||||||| merged common ancestors
    let ast: syn::DeriveInput = syn::parse(input).unwrap();

    let fdic = options::FromVariantOptions::new(&ast).unwrap();
    let trait_impl = codegen::FromVariantImpl::from(&fdic);
    let result = quote!(#trait_impl);

    result.into()
=======
    derive::from_variant(&parse_macro_input!(input)).into()
>>>>>>> upstream-releases
}
