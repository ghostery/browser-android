/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
use crate::cg;
use proc_macro2::{Span, TokenStream};
use syn::{DeriveInput, Ident};
use synstructure::BindStyle;
||||||| merged common ancestors
use cg;
use quote::Tokens;
use syn::DeriveInput;
use synstructure::BindStyle;
=======
use derive_common::cg;
use proc_macro2::TokenStream;
use syn::{DeriveInput, Ident, Path};
use synstructure::{BindStyle, BindingInfo};

pub fn derive_to_value(
    mut input: DeriveInput,
    trait_path: Path,
    output_type_name: Ident,
    bind_style: BindStyle,
    // Returns whether to apply the field bound for a given item.
    mut field_bound: impl FnMut(&BindingInfo) -> bool,
    // Returns a token stream of the form: trait_path::from_foo(#binding)
    mut call_from: impl FnMut(&BindingInfo) -> TokenStream,
    mut call_to: impl FnMut(&BindingInfo) -> TokenStream,
    // Returns a tokenstream of the form:
    // fn from_function_syntax(foobar) -> Baz {
    //     #first_arg
    // }
    //
    // fn to_function_syntax(foobar) -> Baz {
    //     #second_arg
    // }
    mut trait_impl: impl FnMut(TokenStream, TokenStream) -> TokenStream,
    // if this is provided, the derive for non-generic types will be simplified
    // to this token stream, which should be the body of the impl block.
    non_generic_implementation: impl FnOnce() -> Option<TokenStream>,
) -> TokenStream {
    let name = &input.ident;

    if input.generics.type_params().next().is_none() {
        if let Some(non_generic_implementation) = non_generic_implementation() {
            let (impl_generics, ty_generics, where_clause) = input.generics.split_for_impl();
            return quote! {
                impl #impl_generics #trait_path for #name #ty_generics
                #where_clause
                {
                    #non_generic_implementation
                }
            };
        }
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
pub fn derive(mut input: DeriveInput) -> TokenStream {
||||||| merged common ancestors
pub fn derive(mut input: DeriveInput) -> Tokens {
=======
>>>>>>> upstream-releases
    let mut where_clause = input.generics.where_clause.take();
    cg::propagate_clauses_to_output_type(
        &mut where_clause,
        &input.generics,
        &trait_path,
        &output_type_name,
    );
    let (to_body, from_body) = {
        let params = input.generics.type_params().collect::<Vec<_>>();
        for param in &params {
<<<<<<< HEAD
            cg::add_predicate(
                &mut where_clause,
                parse_quote!(#param: crate::values::computed::ToComputedValue),
            );
||||||| merged common ancestors
            cg::add_predicate(
                &mut where_clause,
                parse_quote!(#param: ::values::computed::ToComputedValue),
            );
=======
            cg::add_predicate(&mut where_clause, parse_quote!(#param: #trait_path));
>>>>>>> upstream-releases
        }

        let to_body = cg::fmap_match(&input, bind_style, |binding| {
            if field_bound(&binding) {
                let ty = &binding.ast().ty;

                let output_type = cg::map_type_params(
                    ty,
                    &params,
<<<<<<< HEAD
                    &mut |ident| parse_quote!(<#ident as crate::values::computed::ToComputedValue>::ComputedValue),
||||||| merged common ancestors
                    &mut |ident| parse_quote!(<#ident as ::values::computed::ToComputedValue>::ComputedValue),
=======
                    &mut |ident| parse_quote!(<#ident as #trait_path>::#output_type_name),
>>>>>>> upstream-releases
                );

                cg::add_predicate(
                    &mut where_clause,
                    parse_quote!(
<<<<<<< HEAD
                        #ty: crate::values::computed::ToComputedValue<ComputedValue = #output_type>
||||||| merged common ancestors
                        #ty: ::values::computed::ToComputedValue<ComputedValue = #output_type>
=======
                        #ty: #trait_path<#output_type_name = #output_type>
>>>>>>> upstream-releases
                    ),
                );
            }
<<<<<<< HEAD
            quote! {
                crate::values::computed::ToComputedValue::to_computed_value(#binding, context)
            }
        });
        let from_body = cg::fmap_match(&input, BindStyle::Ref, |binding| {
            quote! {
                crate::values::computed::ToComputedValue::from_computed_value(#binding)
            }
||||||| merged common ancestors
            quote! {
                ::values::computed::ToComputedValue::to_computed_value(#binding, context)
            }
        });
        let from_body = cg::fmap_match(&input, BindStyle::Ref, |binding| {
            quote! {
                ::values::computed::ToComputedValue::from_computed_value(#binding)
            }
=======
            call_to(&binding)
>>>>>>> upstream-releases
        });
        let from_body = cg::fmap_match(&input, bind_style, |binding| call_from(&binding));

        (to_body, from_body)
    };

    input.generics.where_clause = where_clause;
    let (impl_generics, ty_generics, where_clause) = input.generics.split_for_impl();
    let computed_value_type = cg::fmap_trait_output(&input, &trait_path, &output_type_name);

<<<<<<< HEAD
    if input.generics.type_params().next().is_none() {
        return quote! {
            impl #impl_generics crate::values::computed::ToComputedValue for #name #ty_generics
            #where_clause
            {
                type ComputedValue = Self;

                #[inline]
                fn to_computed_value(
                    &self,
                    _context: &crate::values::computed::Context,
                ) -> Self::ComputedValue {
                    std::clone::Clone::clone(self)
                }
||||||| merged common ancestors
    if input.generics.type_params().next().is_none() {
        return quote! {
            impl #impl_generics ::values::computed::ToComputedValue for #name #ty_generics
            #where_clause
            {
                type ComputedValue = Self;

                #[inline]
                fn to_computed_value(
                    &self,
                    _context: &::values::computed::Context,
                ) -> Self::ComputedValue {
                    ::std::clone::Clone::clone(self)
                }
=======
    let impl_ = trait_impl(from_body, to_body);
>>>>>>> upstream-releases

<<<<<<< HEAD
                #[inline]
                fn from_computed_value(computed: &Self::ComputedValue) -> Self {
                    std::clone::Clone::clone(computed)
                }
            }
        };
||||||| merged common ancestors
                #[inline]
                fn from_computed_value(computed: &Self::ComputedValue) -> Self {
                    ::std::clone::Clone::clone(computed)
                }
            }
        };
=======
    quote! {
        impl #impl_generics #trait_path for #name #ty_generics #where_clause {
            type #output_type_name = #computed_value_type;

            #impl_
        }
>>>>>>> upstream-releases
    }
}

<<<<<<< HEAD
    let computed_value_type = cg::fmap_trait_output(
        &input,
        &parse_quote!(crate::values::computed::ToComputedValue),
        Ident::new("ComputedValue", Span::call_site()),
    );
||||||| merged common ancestors
    let computed_value_type = cg::fmap_trait_output(
        &input,
        &parse_quote!(values::computed::ToComputedValue),
        "ComputedValue".into(),
    );
=======
pub fn derive(input: DeriveInput) -> TokenStream {
    let trait_impl = |from_body, to_body| {
        quote! {
             #[inline]
             fn from_computed_value(computed: &Self::ComputedValue) -> Self {
                 match *computed {
                     #from_body
                 }
             }
>>>>>>> upstream-releases

<<<<<<< HEAD
    quote! {
        impl #impl_generics crate::values::computed::ToComputedValue for #name #ty_generics #where_clause {
            type ComputedValue = #computed_value_type;
||||||| merged common ancestors
    quote! {
        impl #impl_generics ::values::computed::ToComputedValue for #name #ty_generics #where_clause {
            type ComputedValue = #computed_value_type;
=======
             #[allow(unused_variables)]
             #[inline]
             fn to_computed_value(&self, context: &crate::values::computed::Context) -> Self::ComputedValue {
                 match *self {
                     #to_body
                 }
             }
        }
    };

    let non_generic_implementation = || {
        Some(quote! {
            type ComputedValue = Self;
>>>>>>> upstream-releases

            #[inline]
<<<<<<< HEAD
            fn to_computed_value(&self, context: &crate::values::computed::Context) -> Self::ComputedValue {
                match *self {
                    #to_body
                }
||||||| merged common ancestors
            fn to_computed_value(&self, context: &::values::computed::Context) -> Self::ComputedValue {
                match *self {
                    #to_body
                }
=======
            fn to_computed_value(&self, _: &crate::values::computed::Context) -> Self::ComputedValue {
                std::clone::Clone::clone(self)
>>>>>>> upstream-releases
            }

            #[inline]
            fn from_computed_value(computed: &Self::ComputedValue) -> Self {
                std::clone::Clone::clone(computed)
            }
        })
    };

    derive_to_value(
        input,
        parse_quote!(crate::values::computed::ToComputedValue),
        parse_quote!(ComputedValue),
        BindStyle::Ref,
        |binding| cg::parse_field_attrs::<ComputedValueAttrs>(&binding.ast()).field_bound,
        |binding| quote!(crate::values::computed::ToComputedValue::from_computed_value(#binding)),
        |binding| quote!(crate::values::computed::ToComputedValue::to_computed_value(#binding, context)),
        trait_impl,
        non_generic_implementation,
    )
}

#[darling(attributes(compute), default)]
#[derive(Default, FromField)]
struct ComputedValueAttrs {
    field_bound: bool,
}
