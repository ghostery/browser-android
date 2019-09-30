/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

//! Common handling for the specified value CSS url() values.

<<<<<<< HEAD
use crate::gecko_bindings::bindings;
use crate::gecko_bindings::structs::root::mozilla::css::URLValue;
use crate::gecko_bindings::structs::root::mozilla::CORSMode;
use crate::gecko_bindings::structs::root::nsStyleImageRequest;
use crate::gecko_bindings::sugar::ownership::{FFIArcHelpers, HasArcFFI};
use crate::gecko_bindings::sugar::refptr::RefPtr;
use crate::parser::{Parse, ParserContext};
use crate::stylesheets::UrlExtraData;
use crate::values::computed::{Context, ToComputedValue};
||||||| merged common ancestors
=======
use crate::gecko_bindings::bindings;
use crate::gecko_bindings::structs;
use crate::gecko_bindings::structs::nsStyleImageRequest;
use crate::gecko_bindings::sugar::refptr::RefPtr;
use crate::parser::{Parse, ParserContext};
use crate::stylesheets::{CorsMode, UrlExtraData};
use crate::values::computed::{Context, ToComputedValue};
>>>>>>> upstream-releases
use cssparser::Parser;
use malloc_size_of::{MallocSizeOf, MallocSizeOfOps};
use nsstring::nsCString;
use servo_arc::Arc;
use std::collections::HashMap;
use std::fmt::{self, Write};
use std::mem::ManuallyDrop;
use std::sync::RwLock;
use style_traits::{CssWriter, ParseError, ToCss};
<<<<<<< HEAD
||||||| merged common ancestors
use stylesheets::UrlExtraData;
use values::computed::{Context, ToComputedValue};
=======
use to_shmem::{SharedMemoryBuilder, ToShmem};
>>>>>>> upstream-releases

/// A CSS url() value for gecko.
#[css(function = "url")]
#[derive(Clone, Debug, PartialEq, SpecifiedValueInfo, ToCss, ToShmem)]
#[repr(C)]
pub struct CssUrl(pub Arc<CssUrlData>);

/// Data shared between CssUrls.
///
/// cbindgen:derive-eq=false
/// cbindgen:derive-neq=false
#[derive(Debug, SpecifiedValueInfo, ToCss, ToShmem)]
#[repr(C)]
pub struct CssUrlData {
    /// The URL in unresolved string form.
    serialization: crate::OwnedStr,

    /// The URL extra data.
    #[css(skip)]
    pub extra_data: UrlExtraData,

    /// The CORS mode that will be used for the load.
    #[css(skip)]
    cors_mode: CorsMode,

    /// Data to trigger a load from Gecko. This is mutable in C++.
    ///
    /// TODO(emilio): Maybe we can eagerly resolve URLs and make this immutable?
    #[css(skip)]
    load_data: LoadDataSource,
}

impl PartialEq for CssUrlData {
    fn eq(&self, other: &Self) -> bool {
        self.serialization == other.serialization &&
            self.extra_data == other.extra_data &&
            self.cors_mode == other.cors_mode
    }
}

impl CssUrl {
    fn parse_with_cors_mode<'i, 't>(
        context: &ParserContext,
        input: &mut Parser<'i, 't>,
        cors_mode: CorsMode,
    ) -> Result<Self, ParseError<'i>> {
        let url = input.expect_url()?;
        Ok(Self::parse_from_string(
            url.as_ref().to_owned(),
            context,
            cors_mode,
        ))
    }

    /// Parse a URL from a string value that is a valid CSS token for a URL.
    pub fn parse_from_string(url: String, context: &ParserContext, cors_mode: CorsMode) -> Self {
        CssUrl(Arc::new(CssUrlData {
            serialization: url.into(),
            extra_data: context.url_data.clone(),
            cors_mode,
            load_data: LoadDataSource::Owned(LoadData::default()),
        }))
    }

    /// Returns true if the URL is definitely invalid. We don't eagerly resolve
    /// URLs in gecko, so we just return false here.
    /// use its |resolved| status.
    pub fn is_invalid(&self) -> bool {
        false
    }

    /// Returns true if this URL looks like a fragment.
    /// See https://drafts.csswg.org/css-values/#local-urls
    #[inline]
    pub fn is_fragment(&self) -> bool {
        self.0.is_fragment()
    }

    /// Return the unresolved url as string, or the empty string if it's
    /// invalid.
    #[inline]
    pub fn as_str(&self) -> &str {
        self.0.as_str()
    }
}

impl CssUrlData {
    /// Returns true if this URL looks like a fragment.
    /// See https://drafts.csswg.org/css-values/#local-urls
    pub fn is_fragment(&self) -> bool {
        self.as_str().chars().next().map_or(false, |c| c == '#')
    }

    /// Return the unresolved url as string, or the empty string if it's
    /// invalid.
    pub fn as_str(&self) -> &str {
        &*self.serialization
    }
}

impl Parse for CssUrl {
    fn parse<'i, 't>(
        context: &ParserContext,
        input: &mut Parser<'i, 't>,
    ) -> Result<Self, ParseError<'i>> {
        Self::parse_with_cors_mode(context, input, CorsMode::None)
    }
}

impl Eq for CssUrl {}

impl MallocSizeOf for CssUrl {
    fn size_of(&self, _ops: &mut MallocSizeOfOps) -> usize {
        // XXX: measure `serialization` once bug 1397971 lands

        // We ignore `extra_data`, because RefPtr is tricky, and there aren't
        // many of them in practise (sharing is common).

        0
    }
}

/// A key type for LOAD_DATA_TABLE.
#[derive(Eq, Hash, PartialEq)]
struct LoadDataKey(*const LoadDataSource);

unsafe impl Sync for LoadDataKey {}
unsafe impl Send for LoadDataKey {}

<<<<<<< HEAD
    fn from_css_url_with_cors(url: CssUrl, cors: CORSMode) -> Self {
        let url_value = unsafe {
            let ptr = bindings::Gecko_URLValue_Create(url.0.clone().into_strong(), cors);
            // We do not expect Gecko_URLValue_Create returns null.
            debug_assert!(!ptr.is_null());
            RefPtr::from_addrefed(ptr)
        };
        Self { url, url_value }
    }
||||||| merged common ancestors
    fn from_css_url_with_cors(url: CssUrl, cors: CORSMode) -> Self {
        let url_value = unsafe {
            let ptr = bindings::Gecko_URLValue_Create(
                url.0.clone().into_strong(),
                cors,
            );
            // We do not expect Gecko_URLValue_Create returns null.
            debug_assert!(!ptr.is_null());
            RefPtr::from_addrefed(ptr)
        };
        Self { url, url_value }
    }
=======
/// The load data for a given URL. This is mutable from C++, for now at least.
#[repr(C)]
#[derive(Debug)]
pub struct LoadData {
    resolved: RefPtr<structs::nsIURI>,
    load_id: u64,
    tried_to_resolve: bool,
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    fn from_css_url(url: CssUrl) -> Self {
        use crate::gecko_bindings::structs::root::mozilla::CORSMode_CORS_NONE;
        Self::from_css_url_with_cors(url, CORSMode_CORS_NONE)
||||||| merged common ancestors
    fn from_css_url(url: CssUrl) -> Self {
        use gecko_bindings::structs::root::mozilla::CORSMode_CORS_NONE;
        Self::from_css_url_with_cors(url, CORSMode_CORS_NONE)
=======
impl Drop for LoadData {
    fn drop(&mut self) {
        if self.load_id != 0 {
            unsafe {
                bindings::Gecko_LoadData_DeregisterLoad(self);
            }
        }
>>>>>>> upstream-releases
    }
}

<<<<<<< HEAD
    fn from_css_url_with_cors_anonymous(url: CssUrl) -> Self {
        use crate::gecko_bindings::structs::root::mozilla::CORSMode_CORS_ANONYMOUS;
        Self::from_css_url_with_cors(url, CORSMode_CORS_ANONYMOUS)
||||||| merged common ancestors
    fn from_css_url_with_cors_anonymous(url: CssUrl) -> Self {
        use gecko_bindings::structs::root::mozilla::CORSMode_CORS_ANONYMOUS;
        Self::from_css_url_with_cors(url, CORSMode_CORS_ANONYMOUS)
=======
impl Default for LoadData {
    fn default() -> Self {
        Self {
            resolved: RefPtr::null(),
            load_id: 0,
            tried_to_resolve: false,
        }
>>>>>>> upstream-releases
    }
}

/// The data for a load, or a lazy-loaded, static member that will be stored in
/// LOAD_DATA_TABLE, keyed by the memory location of this object, which is
/// always in the heap because it's inside the CssUrlData object.
///
/// This type is meant not to be used from C++ so we don't derive helper
/// methods.
///
/// cbindgen:derive-helper-methods=false
#[derive(Debug)]
#[repr(u8, C)]
pub enum LoadDataSource {
    /// An owned copy of the load data.
    Owned(LoadData),
    /// A lazily-resolved copy of it.
    Lazy,
}

impl LoadDataSource {
    /// Gets the load data associated with the source.
    ///
    /// This relies on the source on being in a stable location if lazy.
    #[inline]
    pub unsafe fn get(&self) -> *const LoadData {
        match *self {
            LoadDataSource::Owned(ref d) => return d,
            LoadDataSource::Lazy => {},
        }

        let key = LoadDataKey(self);

        {
            let guard = LOAD_DATA_TABLE.read().unwrap();
            if let Some(r) = guard.get(&key) {
                return &**r;
            }
        }
        let mut guard = LOAD_DATA_TABLE.write().unwrap();
        let r = guard.entry(key).or_insert_with(Default::default);
        &**r
    }
}

impl ToShmem for LoadDataSource {
    fn to_shmem(&self, _builder: &mut SharedMemoryBuilder) -> ManuallyDrop<Self> {
        ManuallyDrop::new(match self {
            LoadDataSource::Owned(..) => LoadDataSource::Lazy,
            LoadDataSource::Lazy => LoadDataSource::Lazy,
        })
    }
}

/// A specified non-image `url()` value.
pub type SpecifiedUrl = CssUrl;

/// Clears LOAD_DATA_TABLE.  Entries in this table, which are for specified URL
/// values that come from shared memory style sheets, would otherwise persist
/// until the end of the process and be reported as leaks.
pub fn shutdown() {
    LOAD_DATA_TABLE.write().unwrap().clear();
}

impl ToComputedValue for SpecifiedUrl {
    type ComputedValue = ComputedUrl;

    #[inline]
    fn to_computed_value(&self, _: &Context) -> Self::ComputedValue {
        ComputedUrl(self.clone())
    }

    #[inline]
    fn from_computed_value(computed: &Self::ComputedValue) -> Self {
        computed.0.clone()
    }
}

/// A specified image `url()` value.
#[derive(Clone, Debug, Eq, MallocSizeOf, PartialEq, SpecifiedValueInfo, ToCss, ToShmem)]
pub struct SpecifiedImageUrl(pub SpecifiedUrl);

impl SpecifiedImageUrl {
    /// Parse a URL from a string value that is a valid CSS token for a URL.
    pub fn parse_from_string(url: String, context: &ParserContext, cors_mode: CorsMode) -> Self {
        SpecifiedImageUrl(SpecifiedUrl::parse_from_string(url, context, cors_mode))
    }

    /// Provides an alternate method for parsing that associates the URL
    /// with anonymous CORS headers.
    pub fn parse_with_cors_anonymous<'i, 't>(
        context: &ParserContext,
        input: &mut Parser<'i, 't>,
    ) -> Result<Self, ParseError<'i>> {
        Ok(SpecifiedImageUrl(SpecifiedUrl::parse_with_cors_mode(
            context,
            input,
            CorsMode::Anonymous,
        )?))
    }
}

impl Parse for SpecifiedImageUrl {
    fn parse<'i, 't>(
        context: &ParserContext,
        input: &mut Parser<'i, 't>,
    ) -> Result<Self, ParseError<'i>> {
        SpecifiedUrl::parse(context, input).map(SpecifiedImageUrl)
    }
}

impl ToComputedValue for SpecifiedImageUrl {
    type ComputedValue = ComputedImageUrl;

    #[inline]
    fn to_computed_value(&self, context: &Context) -> Self::ComputedValue {
        ComputedImageUrl(self.0.to_computed_value(context))
    }

    #[inline]
    fn from_computed_value(computed: &Self::ComputedValue) -> Self {
        SpecifiedImageUrl(ToComputedValue::from_computed_value(&computed.0))
    }
}

/// The computed value of a CSS non-image `url()`.
///
/// The only difference between specified and computed URLs is the
/// serialization.
#[derive(Clone, Debug, Eq, MallocSizeOf, PartialEq)]
#[repr(C)]
pub struct ComputedUrl(pub SpecifiedUrl);

impl ComputedUrl {
    fn serialize_with<W>(
        &self,
        function: unsafe extern "C" fn(*const Self, *mut nsCString),
        dest: &mut CssWriter<W>,
    ) -> fmt::Result
    where
        W: Write,
    {
<<<<<<< HEAD
        serialize_computed_url(&self.0.url_value, dest, bindings::Gecko_GetComputedURLSpec)
||||||| merged common ancestors
        serialize_computed_url(
            &self.0.url_value,
            dest,
            bindings::Gecko_GetComputedURLSpec,
        )
=======
        dest.write_str("url(")?;
        unsafe {
            let mut string = nsCString::new();
            function(self, &mut string);
            string.as_str_unchecked().to_css(dest)?;
        }
        dest.write_char(')')
>>>>>>> upstream-releases
    }
}

impl ToCss for ComputedUrl {
    fn to_css<W>(&self, dest: &mut CssWriter<W>) -> fmt::Result
    where
        W: Write,
    {
        self.serialize_with(bindings::Gecko_GetComputedURLSpec, dest)
    }
}

/// The computed value of a CSS image `url()`.
#[derive(Clone, Debug, Eq, MallocSizeOf, PartialEq)]
#[repr(transparent)]
pub struct ComputedImageUrl(pub ComputedUrl);

impl ComputedImageUrl {
    /// Convert from nsStyleImageRequest to ComputedImageUrl.
    pub unsafe fn from_image_request(image_request: &nsStyleImageRequest) -> Self {
        image_request.mImageURL.clone()
    }
}

impl ToCss for ComputedImageUrl {
    fn to_css<W>(&self, dest: &mut CssWriter<W>) -> fmt::Result
    where
        W: Write,
    {
        self.0
            .serialize_with(bindings::Gecko_GetComputedImageURLSpec, dest)
    }
}

lazy_static! {
    /// A table mapping CssUrlData objects to their lazily created LoadData
    /// objects.
    static ref LOAD_DATA_TABLE: RwLock<HashMap<LoadDataKey, Box<LoadData>>> = {
        Default::default()
    };
}
