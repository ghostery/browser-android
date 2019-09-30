/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Computed types for SVG properties.

<<<<<<< HEAD
use crate::values::computed::color::Color;
use crate::values::computed::url::ComputedUrl;
use crate::values::computed::{LengthOrPercentage, NonNegativeLengthOrPercentage};
use crate::values::computed::{NonNegativeNumber, Number, Opacity};
use crate::values::generics::svg as generic;
use crate::values::RGBA;
||||||| merged common ancestors
use app_units::Au;
use values::RGBA;
use values::computed::{LengthOrPercentage, NonNegativeLength};
use values::computed::{NonNegativeLengthOrPercentage, NonNegativeNumber, Number};
use values::computed::Opacity;
use values::computed::color::Color;
use values::computed::url::ComputedUrl;
use values::generics::svg as generic;
=======
use crate::values::computed::color::Color;
use crate::values::computed::url::ComputedUrl;
use crate::values::computed::{LengthPercentage, NonNegativeLengthPercentage, Opacity};
use crate::values::generics::svg as generic;
use crate::values::RGBA;
use crate::Zero;
>>>>>>> upstream-releases

<<<<<<< HEAD
pub use crate::values::specified::SVGPaintOrder;

pub use crate::values::specified::MozContextProperties;
||||||| merged common ancestors
pub use values::specified::SVGPaintOrder;

pub use values::specified::MozContextProperties;
=======
pub use crate::values::specified::{SVGPaintOrder, MozContextProperties};
>>>>>>> upstream-releases

/// Computed SVG Paint value
pub type SVGPaint = generic::GenericSVGPaint<Color, ComputedUrl>;

/// Computed SVG Paint Kind value
pub type SVGPaintKind = generic::GenericSVGPaintKind<Color, ComputedUrl>;

impl SVGPaint {
    /// Opaque black color
    pub fn black() -> Self {
        let rgba = RGBA::from_floats(0., 0., 0., 1.).into();
        SVGPaint {
            kind: generic::SVGPaintKind::Color(rgba),
            fallback: generic::SVGPaintFallback::Unset,
        }
    }
}

/// <length> | <percentage> | <number> | context-value
pub type SVGLength = generic::SVGLength<LengthPercentage>;

<<<<<<< HEAD
impl SVGLength {
    /// `0px`
    pub fn zero() -> Self {
        generic::SVGLength::Length(generic::SvgLengthOrPercentageOrNumber::LengthOrPercentage(
            LengthOrPercentage::zero(),
        ))
    }
}

/// A value of <length> | <percentage> | <number> for stroke-width/stroke-dasharray.
/// <https://www.w3.org/TR/SVG11/painting.html#StrokeProperties>
pub type NonNegativeSvgLengthOrPercentageOrNumber =
    generic::SvgLengthOrPercentageOrNumber<NonNegativeLengthOrPercentage, NonNegativeNumber>;

// FIXME(emilio): This is really hacky, and can go away with a bit of work on
// the clone_stroke_width code in gecko.mako.rs.
impl Into<NonNegativeSvgLengthOrPercentageOrNumber> for SvgLengthOrPercentageOrNumber {
    fn into(self) -> NonNegativeSvgLengthOrPercentageOrNumber {
        match self {
            generic::SvgLengthOrPercentageOrNumber::LengthOrPercentage(lop) => {
                generic::SvgLengthOrPercentageOrNumber::LengthOrPercentage(lop.into())
            },
            generic::SvgLengthOrPercentageOrNumber::Number(num) => {
                generic::SvgLengthOrPercentageOrNumber::Number(num.into())
            },
        }
||||||| merged common ancestors
impl From<Au> for SVGLength {
    fn from(length: Au) -> Self {
        generic::SVGLength::Length(generic::SvgLengthOrPercentageOrNumber::LengthOrPercentage(
            length.into(),
        ))
    }
}

/// A value of <length> | <percentage> | <number> for stroke-width/stroke-dasharray.
/// <https://www.w3.org/TR/SVG11/painting.html#StrokeProperties>
pub type NonNegativeSvgLengthOrPercentageOrNumber =
    generic::SvgLengthOrPercentageOrNumber<NonNegativeLengthOrPercentage, NonNegativeNumber>;

impl Into<NonNegativeSvgLengthOrPercentageOrNumber> for SvgLengthOrPercentageOrNumber {
    fn into(self) -> NonNegativeSvgLengthOrPercentageOrNumber {
        match self {
            generic::SvgLengthOrPercentageOrNumber::LengthOrPercentage(lop) => {
                generic::SvgLengthOrPercentageOrNumber::LengthOrPercentage(lop.into())
            },
            generic::SvgLengthOrPercentageOrNumber::Number(num) => {
                generic::SvgLengthOrPercentageOrNumber::Number(num.into())
            },
        }
=======
impl SVGLength {
    /// `0px`
    pub fn zero() -> Self {
        generic::SVGLength::LengthPercentage(LengthPercentage::zero())
>>>>>>> upstream-releases
    }
}

/// An non-negative wrapper of SVGLength.
pub type SVGWidth = generic::SVGLength<NonNegativeLengthPercentage>;

<<<<<<< HEAD
impl SVGWidth {
    /// `1px`.
    pub fn one() -> Self {
        use crate::values::generics::NonNegative;
        generic::SVGLength::Length(generic::SvgLengthOrPercentageOrNumber::LengthOrPercentage(
            NonNegative(LengthOrPercentage::one()),
        ))
||||||| merged common ancestors
impl From<NonNegativeLength> for SVGWidth {
    fn from(length: NonNegativeLength) -> Self {
        generic::SVGLength::Length(generic::SvgLengthOrPercentageOrNumber::LengthOrPercentage(
            length.into(),
        ))
=======
impl SVGWidth {
    /// `1px`.
    pub fn one() -> Self {
        use crate::values::generics::NonNegative;
        generic::SVGLength::LengthPercentage(NonNegative(LengthPercentage::one()))
>>>>>>> upstream-releases
    }
}

/// [ <length> | <percentage> | <number> ]# | context-value
pub type SVGStrokeDashArray = generic::GenericSVGStrokeDashArray<NonNegativeLengthPercentage>;

impl Default for SVGStrokeDashArray {
    fn default() -> Self {
        generic::SVGStrokeDashArray::Values(Default::default())
    }
}

/// <opacity-value> | context-fill-opacity | context-stroke-opacity
pub type SVGOpacity = generic::GenericSVGOpacity<Opacity>;

impl Default for SVGOpacity {
    fn default() -> Self {
        generic::SVGOpacity::Opacity(1.)
    }
}
