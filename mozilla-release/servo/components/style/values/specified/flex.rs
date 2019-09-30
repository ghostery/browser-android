/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Specified types for CSS values related to flexbox.

<<<<<<< HEAD
use crate::parser::{Parse, ParserContext};
use crate::values::generics::flex::FlexBasis as GenericFlexBasis;
use cssparser::Parser;
use style_traits::ParseError;

/// The `width` value type.
#[cfg(feature = "servo")]
pub type Width = crate::values::specified::NonNegativeLengthOrPercentageOrAuto;

/// The `width` value type.
#[cfg(feature = "gecko")]
pub type Width = crate::values::specified::MozLength;
||||||| merged common ancestors
use cssparser::Parser;
use parser::{Parse, ParserContext};
use style_traits::ParseError;
use values::generics::flex::FlexBasis as GenericFlexBasis;

/// The `width` value type.
#[cfg(feature = "servo")]
pub type Width = ::values::specified::NonNegativeLengthOrPercentageOrAuto;

/// The `width` value type.
#[cfg(feature = "gecko")]
pub type Width = ::values::specified::MozLength;
=======
use crate::values::generics::flex::FlexBasis as GenericFlexBasis;
use crate::values::specified::Size;
>>>>>>> upstream-releases

/// A specified value for the `flex-basis` property.
pub type FlexBasis = GenericFlexBasis<Size>;

impl FlexBasis {
    /// `auto`
    #[inline]
    pub fn auto() -> Self {
        GenericFlexBasis::Size(Size::auto())
    }

    /// `0%`
    #[inline]
    pub fn zero_percent() -> Self {
        GenericFlexBasis::Size(Size::zero_percent())
    }
}
