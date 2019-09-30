/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Generic types for CSS values in SVG

<<<<<<< HEAD
use crate::parser::{Parse, ParserContext};
use crate::values::{Either, None_};
||||||| merged common ancestors
=======
use crate::parser::{Parse, ParserContext};
>>>>>>> upstream-releases
use cssparser::Parser;
<<<<<<< HEAD
use style_traits::{ParseError, StyleParseErrorKind};
||||||| merged common ancestors
use parser::{Parse, ParserContext};
use style_traits::{ParseError, StyleParseErrorKind};
use values::{Either, None_};
use values::computed::NumberOrPercentage;
use values::computed::length::LengthOrPercentage;
use values::distance::{ComputeSquaredDistance, SquaredDistance};
=======
use style_traits::ParseError;
>>>>>>> upstream-releases

/// The fallback of an SVG paint server value.
/// cbindgen:derive-tagged-enum-copy-constructor=true
#[derive(
    Animate,
    Clone,
    ComputeSquaredDistance,
    Debug,
    MallocSizeOf,
    PartialEq,
    Parse,
    SpecifiedValueInfo,
    ToAnimatedValue,
    ToAnimatedZero,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
#[repr(C, u8)]
pub enum GenericSVGPaintFallback<C> {
    /// The `none` keyword.
    None,
    /// A magic value that represents no fallback specified and serializes to
    /// the empty string.
    #[css(skip)]
    Unset,
    /// A color.
    Color(C),
}

pub use self::GenericSVGPaintFallback as SVGPaintFallback;

/// An SVG paint value
///
/// <https://www.w3.org/TR/SVG2/painting.html#SpecifyingPaint>
///
/// cbindgen:derive-tagged-enum-copy-constructor=true
#[animation(no_bound(Url))]
#[derive(
    Animate,
    Clone,
    ComputeSquaredDistance,
    Debug,
    MallocSizeOf,
    PartialEq,
    SpecifiedValueInfo,
    ToAnimatedValue,
    ToAnimatedZero,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
#[repr(C)]
pub struct GenericSVGPaint<Color, Url> {
    /// The paint source.
    pub kind: GenericSVGPaintKind<Color, Url>,
    /// The fallback color.
    pub fallback: GenericSVGPaintFallback<Color>,
}

pub use self::GenericSVGPaint as SVGPaint;

impl<C, U> Default for SVGPaint<C, U> {
    fn default() -> Self {
        Self {
            kind: SVGPaintKind::None,
            fallback: SVGPaintFallback::Unset,
        }
    }
}

/// An SVG paint value without the fallback.
///
/// Whereas the spec only allows PaintServer to have a fallback, Gecko lets the
/// context properties have a fallback as well.
///
/// cbindgen:derive-tagged-enum-copy-constructor=true
#[animation(no_bound(U))]
#[derive(
    Animate,
    Clone,
    ComputeSquaredDistance,
    Debug,
    MallocSizeOf,
    PartialEq,
    Parse,
    SpecifiedValueInfo,
    ToAnimatedValue,
    ToAnimatedZero,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
<<<<<<< HEAD
pub enum SvgLengthOrPercentageOrNumber<LengthOrPercentage, Number> {
    /// <length> | <percentage>
    LengthOrPercentage(LengthOrPercentage),
    /// <number>
    Number(Number),
}

/// Parsing the SvgLengthOrPercentageOrNumber. At first, we need to parse number
/// since prevent converting to the length.
impl<LengthOrPercentageType: Parse, NumberType: Parse> Parse
    for SvgLengthOrPercentageOrNumber<LengthOrPercentageType, NumberType>
{
||||||| merged common ancestors
pub enum SvgLengthOrPercentageOrNumber<LengthOrPercentage, Number> {
    /// <length> | <percentage>
    LengthOrPercentage(LengthOrPercentage),
    /// <number>
    Number(Number),
}

impl<L, N> ComputeSquaredDistance for SvgLengthOrPercentageOrNumber<L, N>
where
    L: ComputeSquaredDistance + Copy + Into<NumberOrPercentage>,
    N: ComputeSquaredDistance + Copy + Into<NumberOrPercentage>,
{
    #[inline]
    fn compute_squared_distance(&self, other: &Self) -> Result<SquaredDistance, ()> {
        match (self, other) {
            (
                &SvgLengthOrPercentageOrNumber::LengthOrPercentage(ref from),
                &SvgLengthOrPercentageOrNumber::LengthOrPercentage(ref to),
            ) => from.compute_squared_distance(to),
            (
                &SvgLengthOrPercentageOrNumber::Number(ref from),
                &SvgLengthOrPercentageOrNumber::Number(ref to),
            ) => from.compute_squared_distance(to),
            (
                &SvgLengthOrPercentageOrNumber::LengthOrPercentage(from),
                &SvgLengthOrPercentageOrNumber::Number(to),
            ) => from.into().compute_squared_distance(&to.into()),
            (
                &SvgLengthOrPercentageOrNumber::Number(from),
                &SvgLengthOrPercentageOrNumber::LengthOrPercentage(to),
            ) => from.into().compute_squared_distance(&to.into()),
        }
    }
}

impl<LengthOrPercentageType, NumberType>
    SvgLengthOrPercentageOrNumber<LengthOrPercentageType, NumberType>
where
    LengthOrPercentage: From<LengthOrPercentageType>,
    LengthOrPercentageType: Copy,
{
    /// return true if this struct has calc value.
    pub fn has_calc(&self) -> bool {
        match self {
            &SvgLengthOrPercentageOrNumber::LengthOrPercentage(lop) => {
                match LengthOrPercentage::from(lop) {
                    LengthOrPercentage::Calc(_) => true,
                    _ => false,
                }
            },
            _ => false,
        }
    }
}

/// Parsing the SvgLengthOrPercentageOrNumber. At first, we need to parse number
/// since prevent converting to the length.
impl<LengthOrPercentageType: Parse, NumberType: Parse> Parse
    for SvgLengthOrPercentageOrNumber<LengthOrPercentageType, NumberType>
{
=======
#[repr(C, u8)]
pub enum GenericSVGPaintKind<C, U> {
    /// `none`
    #[animation(error)]
    None,
    /// `<color>`
    Color(C),
    /// `url(...)`
    #[animation(error)]
    PaintServer(U),
    /// `context-fill`
    ContextFill,
    /// `context-stroke`
    ContextStroke,
}

pub use self::GenericSVGPaintKind as SVGPaintKind;

impl<C: Parse, U: Parse> Parse for SVGPaint<C, U> {
>>>>>>> upstream-releases
    fn parse<'i, 't>(
        context: &ParserContext,
        input: &mut Parser<'i, 't>,
    ) -> Result<Self, ParseError<'i>> {
<<<<<<< HEAD
        if let Ok(num) = input.try(|i| NumberType::parse(context, i)) {
            return Ok(SvgLengthOrPercentageOrNumber::Number(num));
        }

        let lop = LengthOrPercentageType::parse(context, input)?;
        Ok(SvgLengthOrPercentageOrNumber::LengthOrPercentage(lop))
||||||| merged common ancestors
        if let Ok(num) = input.try(|i| NumberType::parse(context, i)) {
            return Ok(SvgLengthOrPercentageOrNumber::Number(num));
        }

        if let Ok(lop) = input.try(|i| LengthOrPercentageType::parse(context, i)) {
            return Ok(SvgLengthOrPercentageOrNumber::LengthOrPercentage(lop));
        }
        Err(input.new_custom_error(StyleParseErrorKind::UnspecifiedError))
=======
        let kind = SVGPaintKind::parse(context, input)?;
        if matches!(kind, SVGPaintKind::None | SVGPaintKind::Color(..)) {
            return Ok(SVGPaint {
                kind,
                fallback: SVGPaintFallback::Unset
            });
        }
        let fallback = input
            .try(|i| SVGPaintFallback::parse(context, i))
            .unwrap_or(SVGPaintFallback::Unset);
        Ok(SVGPaint { kind, fallback })
>>>>>>> upstream-releases
    }
}

/// An SVG length value supports `context-value` in addition to length.
#[derive(
    Animate,
    Clone,
    ComputeSquaredDistance,
    Copy,
    Debug,
    MallocSizeOf,
    PartialEq,
    SpecifiedValueInfo,
    ToAnimatedValue,
    ToAnimatedZero,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
pub enum SVGLength<L> {
    /// `<length> | <percentage> | <number>`
    LengthPercentage(L),
    /// `context-value`
    #[animation(error)]
    ContextValue,
}

/// Generic value for stroke-dasharray.
#[derive(
    Clone,
    Debug,
    MallocSizeOf,
    PartialEq,
    SpecifiedValueInfo,
    ToAnimatedValue,
    ToAnimatedZero,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
#[repr(C, u8)]
pub enum GenericSVGStrokeDashArray<L> {
    /// `[ <length> | <percentage> | <number> ]#`
    #[css(comma)]
    Values(#[css(if_empty = "none", iterable)] crate::OwnedSlice<L>),
    /// `context-value`
    ContextValue,
}

pub use self::GenericSVGStrokeDashArray as SVGStrokeDashArray;

/// An SVG opacity value accepts `context-{fill,stroke}-opacity` in
/// addition to opacity value.
#[derive(
    Animate,
    Clone,
    ComputeSquaredDistance,
    Copy,
    Debug,
    MallocSizeOf,
    PartialEq,
    Parse,
    SpecifiedValueInfo,
    ToAnimatedZero,
    ToComputedValue,
    ToCss,
    ToResolvedValue,
    ToShmem,
)]
#[repr(C, u8)]
pub enum GenericSVGOpacity<OpacityType> {
    /// `<opacity-value>`
    Opacity(OpacityType),
    /// `context-fill-opacity`
    #[animation(error)]
    ContextFillOpacity,
    /// `context-stroke-opacity`
    #[animation(error)]
    ContextStrokeOpacity,
}

pub use self::GenericSVGOpacity as SVGOpacity;
