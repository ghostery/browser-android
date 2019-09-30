/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Common handling for the specified value CSS url() values.

<<<<<<< HEAD
use crate::values::generics::url::UrlOrNone as GenericUrlOrNone;
||||||| merged common ancestors
use values::generics::url::UrlOrNone as GenericUrlOrNone;
=======
use crate::values::generics::url::GenericUrlOrNone;
>>>>>>> upstream-releases

#[cfg(feature = "gecko")]
pub use crate::gecko::url::{SpecifiedImageUrl, SpecifiedUrl};
#[cfg(feature = "servo")]
pub use crate::servo::url::{SpecifiedImageUrl, SpecifiedUrl};

/// Specified <url> | <none>
pub type UrlOrNone = GenericUrlOrNone<SpecifiedUrl>;

/// Specified image <url> | <none>
pub type ImageUrlOrNone = GenericUrlOrNone<SpecifiedImageUrl>;
