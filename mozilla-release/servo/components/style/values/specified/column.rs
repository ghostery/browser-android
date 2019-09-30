/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Specified types for the column properties.

<<<<<<< HEAD
use crate::parser::{Parse, ParserContext};
use crate::values::generics::column::ColumnCount as GenericColumnCount;
use crate::values::specified::PositiveInteger;
use cssparser::Parser;
use style_traits::ParseError;
||||||| merged common ancestors
use cssparser::Parser;
use parser::{Parse, ParserContext};
use style_traits::ParseError;
use values::generics::column::ColumnCount as GenericColumnCount;
use values::specified::PositiveInteger;
=======
use crate::values::generics::column::ColumnCount as GenericColumnCount;
use crate::values::specified::PositiveInteger;
>>>>>>> upstream-releases

/// A specified type for `column-count` values.
pub type ColumnCount = GenericColumnCount<PositiveInteger>;
