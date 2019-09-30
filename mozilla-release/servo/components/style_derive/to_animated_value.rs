/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
use crate::cg;
use proc_macro2::{Span, TokenStream};
use syn::{DeriveInput, Ident};
||||||| merged common ancestors
use cg;
use quote;
use syn::DeriveInput;
=======
use proc_macro2::TokenStream;
use syn::DeriveInput;
>>>>>>> upstream-releases
use synstructure::BindStyle;
use to_computed_value;

<<<<<<< HEAD
pub fn derive(mut input: DeriveInput) -> TokenStream {
    let mut where_clause = input.generics.where_clause.take();
    for param in input.generics.type_params() {
        cg::add_predicate(
            &mut where_clause,
            parse_quote!(#param: crate::values::animated::ToAnimatedValue),
        );
    }
||||||| merged common ancestors
pub fn derive(mut input: DeriveInput) -> quote::Tokens {
    let mut where_clause = input.generics.where_clause.take();
    for param in input.generics.type_params() {
        cg::add_predicate(
            &mut where_clause,
            parse_quote!(#param: ::values::animated::ToAnimatedValue),
        );
    }
=======
pub fn derive(input: DeriveInput) -> TokenStream {
    let trait_impl = |from_body, to_body| {
        quote! {
             #[inline]
             fn from_animated_value(animated: Self::AnimatedValue) -> Self {
                 match animated {
                     #from_body
                 }
             }
>>>>>>> upstream-releases

<<<<<<< HEAD
    let to_body = cg::fmap_match(
        &input,
        BindStyle::Move,
        |binding| quote!(crate::values::animated::ToAnimatedValue::to_animated_value(#binding)),
    );
    let from_body = cg::fmap_match(
        &input,
        BindStyle::Move,
        |binding| quote!(crate::values::animated::ToAnimatedValue::from_animated_value(#binding)),
    );

    input.generics.where_clause = where_clause;
    let name = &input.ident;
    let (impl_generics, ty_generics, where_clause) = input.generics.split_for_impl();
    let animated_value_type = cg::fmap_trait_output(
        &input,
        &parse_quote!(crate::values::animated::ToAnimatedValue),
        Ident::new("AnimatedValue", Span::call_site()),
    );

    quote! {
        impl #impl_generics crate::values::animated::ToAnimatedValue for #name #ty_generics #where_clause {
            type AnimatedValue = #animated_value_type;
||||||| merged common ancestors
    let to_body = cg::fmap_match(
        &input,
        BindStyle::Move,
        |binding| quote!(::values::animated::ToAnimatedValue::to_animated_value(#binding)),
    );
    let from_body = cg::fmap_match(
        &input,
        BindStyle::Move,
        |binding| quote!(::values::animated::ToAnimatedValue::from_animated_value(#binding)),
    );

    input.generics.where_clause = where_clause;
    let name = &input.ident;
    let (impl_generics, ty_generics, where_clause) = input.generics.split_for_impl();
    let animated_value_type = cg::fmap_trait_output(
        &input,
        &parse_quote!(values::animated::ToAnimatedValue),
        "AnimatedValue".into(),
    );

    quote! {
        impl #impl_generics ::values::animated::ToAnimatedValue for #name #ty_generics #where_clause {
            type AnimatedValue = #animated_value_type;
=======
             #[inline]
             fn to_animated_value(self) -> Self::AnimatedValue {
                 match self {
                     #to_body
                 }
             }
        }
    };
>>>>>>> upstream-releases

    // TODO(emilio): Consider optimizing away non-generic cases as well?
    let non_generic_implementation = || None;

    to_computed_value::derive_to_value(
        input,
        parse_quote!(crate::values::animated::ToAnimatedValue),
        parse_quote!(AnimatedValue),
        BindStyle::Move,
        |_| false,
        |binding| quote!(crate::values::animated::ToAnimatedValue::from_animated_value(#binding)),
        |binding| quote!(crate::values::animated::ToAnimatedValue::to_animated_value(#binding)),
        trait_impl,
        non_generic_implementation,
    )
}
