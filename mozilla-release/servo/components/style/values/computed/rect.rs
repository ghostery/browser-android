/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Computed types for CSS borders.

<<<<<<< HEAD
use crate::values::computed::length::LengthOrNumber;
use crate::values::generics::rect::Rect;
||||||| merged common ancestors
use values::computed::length::LengthOrNumber;
use values::generics::rect::Rect;
=======
use crate::values::computed::length::NonNegativeLengthOrNumber;
use crate::values::generics::rect::Rect;
>>>>>>> upstream-releases

/// A specified rectangle made of four `<length-or-number>` values.
pub type NonNegativeLengthOrNumberRect = Rect<NonNegativeLengthOrNumber>;
