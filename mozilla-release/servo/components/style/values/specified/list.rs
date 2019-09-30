/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! `list` specified values.

use crate::parser::{Parse, ParserContext};
#[cfg(feature = "gecko")]
use crate::values::generics::CounterStyleOrNone;
#[cfg(feature = "gecko")]
<<<<<<< HEAD
use crate::values::CustomIdent;
use cssparser::{Parser, Token};
use servo_arc::Arc;
use style_traits::{ParseError, StyleParseErrorKind};
||||||| merged common ancestors
use values::generics::CounterStyleOrNone;
=======
use crate::values::CustomIdent;
use cssparser::{Parser, Token};
use style_traits::{ParseError, StyleParseErrorKind};
>>>>>>> upstream-releases

/// Specified and computed `list-style-type` property.
#[cfg(feature = "gecko")]
#[derive(
    Clone,
    Debug,
    Eq,
    MallocSizeOf,
    PartialEq,
    SpecifiedValueInfo,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
pub enum ListStyleType {
    /// <counter-style> | none
    CounterStyle(CounterStyleOrNone),
    /// <string>
    String(String),
}

#[cfg(feature = "gecko")]
impl ListStyleType {
    /// Initial specified value for `list-style-type`.
    #[inline]
    pub fn disc() -> Self {
        ListStyleType::CounterStyle(CounterStyleOrNone::disc())
    }

    /// Convert from gecko keyword to list-style-type.
    ///
    /// This should only be used for mapping type attribute to
    /// list-style-type, and thus only values possible in that
    /// attribute is considered here.
    pub fn from_gecko_keyword(value: u32) -> Self {
        use crate::gecko_bindings::structs;

        if value == structs::NS_STYLE_LIST_STYLE_NONE {
            return ListStyleType::CounterStyle(CounterStyleOrNone::None);
        }

        ListStyleType::CounterStyle(CounterStyleOrNone::Name(CustomIdent(match value {
            structs::NS_STYLE_LIST_STYLE_DISC => atom!("disc"),
            structs::NS_STYLE_LIST_STYLE_CIRCLE => atom!("circle"),
            structs::NS_STYLE_LIST_STYLE_SQUARE => atom!("square"),
            structs::NS_STYLE_LIST_STYLE_DECIMAL => atom!("decimal"),
            structs::NS_STYLE_LIST_STYLE_LOWER_ROMAN => atom!("lower-roman"),
            structs::NS_STYLE_LIST_STYLE_UPPER_ROMAN => atom!("upper-roman"),
            structs::NS_STYLE_LIST_STYLE_LOWER_ALPHA => atom!("lower-alpha"),
            structs::NS_STYLE_LIST_STYLE_UPPER_ALPHA => atom!("upper-alpha"),
            _ => unreachable!("Unknown counter style keyword value"),
        })))
    }
}

#[cfg(feature = "gecko")]
impl Parse for ListStyleType {
    fn parse<'i, 't>(
        context: &ParserContext,
        input: &mut Parser<'i, 't>,
    ) -> Result<Self, ParseError<'i>> {
        if let Ok(style) = input.try(|i| CounterStyleOrNone::parse(context, i)) {
            return Ok(ListStyleType::CounterStyle(style));
        }

        Ok(ListStyleType::String(
            input.expect_string()?.as_ref().to_owned(),
        ))
    }
}

<<<<<<< HEAD
/// A quote pair.
#[derive(Clone, Debug, MallocSizeOf, PartialEq, SpecifiedValueInfo, ToComputedValue, ToCss)]
pub struct QuotePair {
    /// The opening quote.
    pub opening: Box<str>,

    /// The closing quote.
    pub closing: Box<str>,
||||||| merged common ancestors
/// Specified and computed `quote` property.
///
/// FIXME(emilio): It's a shame that this allocates all the time it's computed,
/// probably should just be refcounted.
/// FIXME This can probably derive ToCss.
#[derive(Clone, Debug, MallocSizeOf, PartialEq, SpecifiedValueInfo, ToComputedValue)]
pub struct Quotes(#[css(if_empty = "none")] pub Box<[(Box<str>, Box<str>)]>);

impl ToCss for Quotes {
    fn to_css<W>(&self, dest: &mut CssWriter<W>) -> fmt::Result
    where
        W: Write,
    {
        let mut iter = self.0.iter();

        match iter.next() {
            Some(&(ref l, ref r)) => {
                l.to_css(dest)?;
                dest.write_char(' ')?;
                r.to_css(dest)?;
            },
            None => return dest.write_str("none"),
        }

        for &(ref l, ref r) in iter {
            dest.write_char(' ')?;
            l.to_css(dest)?;
            dest.write_char(' ')?;
            r.to_css(dest)?;
        }

        Ok(())
    }
=======
/// A quote pair.
#[derive(
    Clone,
    Debug,
    MallocSizeOf,
    PartialEq,
    SpecifiedValueInfo,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
#[repr(C)]
pub struct QuotePair {
    /// The opening quote.
    pub opening: crate::OwnedStr,

    /// The closing quote.
    pub closing: crate::OwnedStr,
>>>>>>> upstream-releases
}

<<<<<<< HEAD
/// Specified and computed `quotes` property.
#[derive(Clone, Debug, MallocSizeOf, PartialEq, SpecifiedValueInfo, ToComputedValue, ToCss)]
pub struct Quotes(
    #[css(iterable, if_empty = "none")]
    #[ignore_malloc_size_of = "Arc"]
    pub Arc<Box<[QuotePair]>>,
);

||||||| merged common ancestors
=======
/// Specified and computed `quotes` property.
#[derive(
    Clone,
    Debug,
    Default,
    MallocSizeOf,
    PartialEq,
    SpecifiedValueInfo,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
#[repr(C)]
pub struct Quotes(
    #[css(iterable, if_empty = "none")]
    #[ignore_malloc_size_of = "Arc"]
    pub crate::ArcSlice<QuotePair>,
);

>>>>>>> upstream-releases
impl Parse for Quotes {
    fn parse<'i, 't>(
        _: &ParserContext,
        input: &mut Parser<'i, 't>,
    ) -> Result<Quotes, ParseError<'i>> {
        if input
            .try(|input| input.expect_ident_matching("none"))
            .is_ok()
        {
<<<<<<< HEAD
            return Ok(Quotes(Arc::new(Box::new([]))));
||||||| merged common ancestors
            return Ok(Quotes(Vec::new().into_boxed_slice()));
=======
            return Ok(Self::default());
>>>>>>> upstream-releases
        }

        let mut quotes = Vec::new();
        loop {
            let location = input.current_source_location();
<<<<<<< HEAD
            let opening = match input.next() {
                Ok(&Token::QuotedString(ref value)) => value.as_ref().to_owned().into_boxed_str(),
||||||| merged common ancestors
            let first = match input.next() {
                Ok(&Token::QuotedString(ref value)) => value.as_ref().to_owned().into_boxed_str(),
=======
            let opening = match input.next() {
                Ok(&Token::QuotedString(ref value)) => value.as_ref().to_owned().into(),
>>>>>>> upstream-releases
                Ok(t) => return Err(location.new_unexpected_token_error(t.clone())),
                Err(_) => break,
            };

<<<<<<< HEAD
            let closing = input.expect_string()?.as_ref().to_owned().into_boxed_str();
            quotes.push(QuotePair { opening, closing });
||||||| merged common ancestors
            let second = input.expect_string()?.as_ref().to_owned().into_boxed_str();
            quotes.push((first, second))
=======
            let closing = input.expect_string()?.as_ref().to_owned().into();
            quotes.push(QuotePair { opening, closing });
>>>>>>> upstream-releases
        }

        if !quotes.is_empty() {
<<<<<<< HEAD
            Ok(Quotes(Arc::new(quotes.into_boxed_slice())))
||||||| merged common ancestors
            Ok(Quotes(quotes.into_boxed_slice()))
=======
            Ok(Quotes(crate::ArcSlice::from_iter(quotes.into_iter())))
>>>>>>> upstream-releases
        } else {
            Err(input.new_custom_error(StyleParseErrorKind::UnspecifiedError))
        }
    }
}

/// Specified and computed `-moz-list-reversed` property (for UA sheets only).
#[derive(
    Clone,
    Copy,
    Debug,
    Eq,
    Hash,
    MallocSizeOf,
    Parse,
    PartialEq,
    SpecifiedValueInfo,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
#[repr(u8)]
pub enum MozListReversed {
    /// the initial value
    False,
    /// exclusively used for <ol reversed> in our html.css UA sheet
    True,
}
