/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Computed values for UI properties

<<<<<<< HEAD
use crate::values::computed::color::Color;
use crate::values::computed::url::ComputedImageUrl;
use crate::values::computed::Number;
use crate::values::generics::ui as generics;
use crate::values::{Auto, Either};
||||||| merged common ancestors
use values::{Auto, Either};
use values::computed::Number;
use values::computed::color::Color;
use values::computed::url::ComputedImageUrl;
use values::generics::ui as generics;
=======
use crate::values::computed::color::Color;
use crate::values::computed::url::ComputedImageUrl;
use crate::values::computed::Number;
use crate::values::generics::ui as generics;
>>>>>>> upstream-releases

<<<<<<< HEAD
pub use crate::values::specified::ui::{MozForceBrokenImageIcon, UserSelect};

/// auto | <color>
pub type ColorOrAuto = Either<Color, Auto>;
||||||| merged common ancestors
pub use values::specified::ui::MozForceBrokenImageIcon;

/// auto | <color>
pub type ColorOrAuto = Either<Color, Auto>;
=======
pub use crate::values::specified::ui::CursorKind;
pub use crate::values::specified::ui::{MozForceBrokenImageIcon, UserSelect};
>>>>>>> upstream-releases

/// A computed value for the `cursor` property.
pub type Cursor = generics::Cursor<CursorImage>;

/// A computed value for item of `image cursors`.
pub type CursorImage = generics::CursorImage<ComputedImageUrl, Number>;

/// A computed value for `scrollbar-color` property.
pub type ScrollbarColor = generics::GenericScrollbarColor<Color>;
