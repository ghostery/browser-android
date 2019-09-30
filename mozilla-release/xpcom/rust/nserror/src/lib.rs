extern crate nsstring;

use nsstring::{nsACString, nsCString};
use std::error::Error;
use std::fmt;

/// The type of errors in gecko.  Uses a newtype to provide additional type
/// safety in Rust and #[repr(transparent)] to ensure the same representation
/// as the C++ equivalent.
#[repr(transparent)]
#[allow(non_camel_case_types)]
#[derive(Clone, Copy, Eq, PartialEq, Ord, PartialOrd, Hash)]
pub struct nsresult(pub u32);

<<<<<<< HEAD
/// An extension trait that adds methods to `nsresult` types.
pub trait NsresultExt {
    fn failed(self) -> bool;
    fn succeeded(self) -> bool;
    fn to_result(self) -> Result<(), nsresult>;

    /// Get a printable name for the nsresult error code. This function returns
    /// a nsCString<'static>, which implements `Display`.
    fn error_name(self) -> nsCString;
}

impl NsresultExt for nsresult {
    fn failed(self) -> bool {
||||||| merged common ancestors
/// An extension trait that adds methods to `nsresult` types.
pub trait NsresultExt {
    fn failed(self) -> bool;
    fn succeeded(self) -> bool;
    fn to_result(self) -> Result<nsresult, nsresult>;

    /// Get a printable name for the nsresult error code. This function returns
    /// a nsCString<'static>, which implements `Display`.
    fn error_name(self) -> nsCString;
}

impl NsresultExt for nsresult {
    fn failed(self) -> bool {
=======
impl nsresult {
    pub fn failed(self) -> bool {
>>>>>>> upstream-releases
        (self.0 >> 31) != 0
    }

    pub fn succeeded(self) -> bool {
        !self.failed()
    }

<<<<<<< HEAD
    fn to_result(self) -> Result<(), nsresult> {
||||||| merged common ancestors
    fn to_result(self) -> Result<nsresult, nsresult> {
=======
    pub fn to_result(self) -> Result<(), nsresult> {
>>>>>>> upstream-releases
        if self.failed() {
            Err(self)
        } else {
            Ok(())
        }
    }

    /// Get a printable name for the nsresult error code. This function returns
    /// a nsCString<'static>, which implements `Display`.
    pub fn error_name(self) -> nsCString {
        let mut cstr = nsCString::new();
        unsafe {
            Gecko_GetErrorName(self, &mut *cstr);
        }
        cstr
    }
}

impl fmt::Display for nsresult {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.error_name())
    }
}

impl fmt::Debug for nsresult {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "{}", self.error_name())
    }
}

impl<T, E> From<Result<T, E>> for nsresult
where
    E: Into<nsresult>,
{
    fn from(result: Result<T, E>) -> nsresult {
        match result {
            Ok(_) => NS_OK,
            Err(e) => e.into(),
        }
    }
}

impl Error for nsresult {}

extern "C" {
    fn Gecko_GetErrorName(rv: nsresult, cstr: *mut nsACString);
}

mod error_list {
    include!(concat!(env!("MOZ_TOPOBJDIR"), "/xpcom/base/error_list.rs"));
}

pub use error_list::*;
