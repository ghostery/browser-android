/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Helper to iterate over `OriginFlags` bits.

<<<<<<< HEAD
use crate::gecko_bindings::structs::OriginFlags;
use crate::gecko_bindings::structs::OriginFlags_Author;
use crate::gecko_bindings::structs::OriginFlags_User;
use crate::gecko_bindings::structs::OriginFlags_UserAgent;
use crate::stylesheets::OriginSet;
||||||| merged common ancestors
use gecko_bindings::structs::OriginFlags;
use gecko_bindings::structs::OriginFlags_Author;
use gecko_bindings::structs::OriginFlags_User;
use gecko_bindings::structs::OriginFlags_UserAgent;
use stylesheets::OriginSet;
=======
use crate::gecko_bindings::structs::OriginFlags;
use crate::stylesheets::OriginSet;
>>>>>>> upstream-releases

/// Checks that the values for OriginFlags are the ones we expect.
pub fn assert_flags_match() {
<<<<<<< HEAD
    use crate::stylesheets::origin::*;
    debug_assert_eq!(OriginFlags_UserAgent.0, OriginSet::ORIGIN_USER_AGENT.bits());
    debug_assert_eq!(OriginFlags_Author.0, OriginSet::ORIGIN_AUTHOR.bits());
    debug_assert_eq!(OriginFlags_User.0, OriginSet::ORIGIN_USER.bits());
||||||| merged common ancestors
    use stylesheets::origin::*;
    debug_assert_eq!(OriginFlags_UserAgent.0, OriginSet::ORIGIN_USER_AGENT.bits());
    debug_assert_eq!(OriginFlags_Author.0, OriginSet::ORIGIN_AUTHOR.bits());
    debug_assert_eq!(OriginFlags_User.0, OriginSet::ORIGIN_USER.bits());
=======
    use crate::stylesheets::origin::*;
    debug_assert_eq!(
        OriginFlags::UserAgent.0,
        OriginSet::ORIGIN_USER_AGENT.bits()
    );
    debug_assert_eq!(OriginFlags::Author.0, OriginSet::ORIGIN_AUTHOR.bits());
    debug_assert_eq!(OriginFlags::User.0, OriginSet::ORIGIN_USER.bits());
>>>>>>> upstream-releases
}

impl From<OriginFlags> for OriginSet {
    fn from(flags: OriginFlags) -> Self {
        Self::from_bits_truncate(flags.0)
    }
}

impl From<OriginSet> for OriginFlags {
    fn from(set: OriginSet) -> Self {
        OriginFlags(set.bits())
    }
}
