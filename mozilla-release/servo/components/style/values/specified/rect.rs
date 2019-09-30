/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Specified types for CSS borders.

<<<<<<< HEAD
use crate::parser::ParserContext;
use crate::values::generics::rect::Rect;
use crate::values::specified::length::LengthOrNumber;
use cssparser::Parser;
use style_traits::ParseError;
||||||| merged common ancestors
use cssparser::Parser;
use parser::ParserContext;
use style_traits::ParseError;
use values::generics::rect::Rect;
use values::specified::length::LengthOrNumber;
=======
use crate::values::generics::rect::Rect;
use crate::values::specified::length::NonNegativeLengthOrNumber;
>>>>>>> upstream-releases

/// A specified rectangle made of four `<length-or-number>` values.
pub type NonNegativeLengthOrNumberRect = Rect<NonNegativeLengthOrNumber>;
