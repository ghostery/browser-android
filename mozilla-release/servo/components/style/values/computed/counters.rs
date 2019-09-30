/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Computed values for counter properties

<<<<<<< HEAD
use crate::values::computed::url::ComputedImageUrl;
use crate::values::generics::counters as generics;
use crate::values::generics::counters::CounterIncrement as GenericCounterIncrement;
use crate::values::generics::counters::CounterReset as GenericCounterReset;
||||||| merged common ancestors
use values::computed::url::ComputedImageUrl;
use values::generics::counters as generics;
use values::generics::counters::CounterIncrement as GenericCounterIncrement;
use values::generics::counters::CounterReset as GenericCounterReset;
=======
use crate::values::computed::url::ComputedImageUrl;
use crate::values::generics::counters as generics;
use crate::values::generics::counters::CounterIncrement as GenericCounterIncrement;
use crate::values::generics::counters::CounterSetOrReset as GenericCounterSetOrReset;
>>>>>>> upstream-releases

/// A computed value for the `counter-increment` property.
pub type CounterIncrement = GenericCounterIncrement<i32>;

/// A computed value for the `counter-set` and `counter-reset` properties.
pub type CounterSetOrReset = GenericCounterSetOrReset<i32>;

/// A computed value for the `content` property.
pub type Content = generics::Content<ComputedImageUrl>;

/// A computed content item.
pub type ContentItem = generics::ContentItem<ComputedImageUrl>;
