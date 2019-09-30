/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsLayoutStylesheetCache.h"

#include "nsAppDirectoryServiceDefs.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/Preferences.h"
#include "mozilla/StyleSheet.h"
#include "mozilla/StyleSheetInlines.h"
#include "mozilla/Telemetry.h"
#include "mozilla/css/Loader.h"
#include "mozilla/dom/SRIMetadata.h"
#include "mozilla/ipc/SharedMemory.h"
#include "MainThreadUtils.h"
#include "nsColor.h"
#include "nsContentUtils.h"
#include "nsIConsoleService.h"
#include "nsIFile.h"
#include "nsIObserverService.h"
#include "nsIXULRuntime.h"
#include "nsNetUtil.h"
#include "nsPresContext.h"
#include "nsPrintfCString.h"
#include "nsServiceManagerUtils.h"
#include "nsXULAppAPI.h"

#include <mozilla/ServoBindings.h>

// The nsLayoutStylesheetCache is responsible for sharing user agent style sheet
// contents across processes using shared memory.  Here is a high level view of
// how that works:
//
// * In the parent process, in the nsLayoutStylesheetCache constructor (which is
//   called early on in a process' lifetime), we parse all UA style sheets into
//   Gecko StyleSheet objects.
//
// * The constructor calls InitSharedSheetsInParent, which creates a shared
//   memory segment that we know ahead of time will be big enough to store the
//   UA sheets.
//
// * It then creates a Rust SharedMemoryBuilder object and passes it a pointer
//   to the start of the shared memory.
//
// * For each UA sheet, we call Servo_SharedMemoryBuilder_AddStylesheet, which
//   takes the StylesheetContents::rules (an Arc<Locked<CssRules>>), produces a
//   deep clone of it, and writes that clone into the shared memory:
//
//   * The deep clone isn't a clone() call, but a call to ToShmem::to_shmem. The
//     ToShmem trait must be implemented on every type that is reachable under
//     the Arc<Locked<CssRules>>. The to_shmem call for each type will clone the
//     value, but any heap allocation will be cloned and placed into the shared
//     memory buffer, rather than heap allocated.
//
//   * For most types, the ToShmem implementation is simple, and we just
//     #[derive(ToShmem)] it. For the types that need special handling due to
//     having heap allocations (Vec<T>, Box<T>, Arc<T>, etc.) we have impls that
//     call to_shmem on the heap allocated data, and then create a new container
//     (e.g. using Box::from_raw) that points into the shared memory.
//
//   * Arc<T> and Locked<T> want to perform atomic writes on data that needs to
//     be in the shared memory buffer (the reference count for Arc<T>, and the
//     SharedRwLock's AtomicRefCell for Locked<T>), so we add special modes to
//     those objects that skip the writes.  For Arc<T>, that means never
//     dropping the object since we don't track the reference count.  That's
//     fine, since we want to just drop the entire shared memory buffer at
//     shutdown.  For Locked<T>, we just panic on attempting to take the lock
//     for writing.  That's also fine, since we don't want devtools being able
//     to modify UA sheets.
//
//   * For Atoms in Rust, static atoms are represented by an index into the
//     static atom table.  Then if we need to Deref the Atom we look up the
//     table.  We panic if any Atom we encounter in the UA style sheets is
//     not a static atom.
//
// * For each UA sheet, we create a new C++ StyleSheet object using the shared
//   memory clone of the sheet contents, and throw away the original heap
//   allocated one.  (We could avoid creating a new C++ StyleSheet object
//   wrapping the shared contents, and update the original StyleSheet object's
//   contents, but it's doubtful that matters.)
//
// * When we initially map the shared memory in the parent process in
//   InitSharedSheetsInParent, we choose an address which is far away from the
//   current extent of the heap.  Although not too far, since we don't want to
//   unnecessarily fragment the virtual address space.
//
// * In the child process, as early as possible (in
//   ContentChild::InitSharedUASheets), we try to map the shared memory at that
//   same address, then pass the shared memory buffer to
//   nsLayoutStylesheetCache::SetSharedMemory.  Since we map at the same
//   address, this means any internal pointers in the UA sheets back into the
//   shared memory buffer that were written by the parent process are valid in
//   the child process too.
//
// * In practice, mapping at the address we need in the child process this works
//   nearly all the time.  If we fail to map at the address we need, the child
//   process falls back to parsing and allocating its own copy of the UA sheets.

using namespace mozilla;
using namespace mozilla::css;

<<<<<<< HEAD
NS_IMPL_ISUPPORTS(nsLayoutStylesheetCache, nsIObserver, nsIMemoryReporter)
||||||| merged common ancestors
NS_IMPL_ISUPPORTS(
  nsLayoutStylesheetCache, nsIObserver, nsIMemoryReporter)
=======
#define PREF_LEGACY_STYLESHEET_CUSTOMIZATION \
  "toolkit.legacyUserProfileCustomizations.stylesheets"

NS_IMPL_ISUPPORTS(nsLayoutStylesheetCache, nsIObserver, nsIMemoryReporter)
>>>>>>> upstream-releases

nsresult nsLayoutStylesheetCache::Observe(nsISupports* aSubject,
                                          const char* aTopic,
                                          const char16_t* aData) {
  if (!strcmp(aTopic, "profile-before-change")) {
    mUserContentSheet = nullptr;
    mUserChromeSheet = nullptr;
  } else if (!strcmp(aTopic, "profile-do-change")) {
    InitFromProfile();
<<<<<<< HEAD
  } else if (strcmp(aTopic, "chrome-flush-skin-caches") == 0 ||
             strcmp(aTopic, "chrome-flush-caches") == 0) {
    mScrollbarsSheet = nullptr;
    mFormsSheet = nullptr;
  } else {
||||||| merged common ancestors
  }
  else if (strcmp(aTopic, "chrome-flush-skin-caches") == 0 ||
           strcmp(aTopic, "chrome-flush-caches") == 0) {
    mScrollbarsSheet = nullptr;
    mFormsSheet = nullptr;
  }
  else {
=======
  } else {
>>>>>>> upstream-releases
    MOZ_ASSERT_UNREACHABLE("Unexpected observer topic.");
  }
  return NS_OK;
}

<<<<<<< HEAD
#define STYLE_SHEET(identifier_, url_, lazy_)                                  \
  StyleSheet* nsLayoutStylesheetCache::identifier_##Sheet() {                  \
    if (lazy_ && !m##identifier_##Sheet) {                                     \
      LoadSheetURL(url_, &m##identifier_##Sheet, eAgentSheetFeatures, eCrash); \
    }                                                                          \
    return m##identifier_##Sheet;                                              \
  }
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET

StyleSheet* nsLayoutStylesheetCache::UserContentSheet() {
||||||| merged common ancestors
StyleSheet*
nsLayoutStylesheetCache::ScrollbarsSheet()
{
  if (!mScrollbarsSheet) {
    // Scrollbars don't need access to unsafe rules
    LoadSheetURL("chrome://global/skin/scrollbars.css",
                 &mScrollbarsSheet, eSafeAgentSheetFeatures, eCrash);
  }

  return mScrollbarsSheet;
}

StyleSheet*
nsLayoutStylesheetCache::FormsSheet()
{
  if (!mFormsSheet) {
    // forms.css needs access to unsafe rules
    LoadSheetURL("resource://gre-resources/forms.css",
                 &mFormsSheet, eAgentSheetFeatures, eCrash);
  }

  return mFormsSheet;
}

StyleSheet*
nsLayoutStylesheetCache::UserContentSheet()
{
=======
#define STYLE_SHEET(identifier_, url_, shared_)                                \
  NotNull<StyleSheet*> nsLayoutStylesheetCache::identifier_##Sheet() {         \
    if (!m##identifier_##Sheet) {                                              \
      LoadSheetURL(url_, &m##identifier_##Sheet, eAgentSheetFeatures, eCrash); \
    }                                                                          \
    return WrapNotNull(m##identifier_##Sheet);                                 \
  }
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET

StyleSheet* nsLayoutStylesheetCache::GetUserContentSheet() {
>>>>>>> upstream-releases
  return mUserContentSheet;
}

<<<<<<< HEAD
StyleSheet* nsLayoutStylesheetCache::UserChromeSheet() {
||||||| merged common ancestors
StyleSheet*
nsLayoutStylesheetCache::UserChromeSheet()
{
=======
StyleSheet* nsLayoutStylesheetCache::GetUserChromeSheet() {
>>>>>>> upstream-releases
  return mUserChromeSheet;
}

<<<<<<< HEAD
StyleSheet* nsLayoutStylesheetCache::ChromePreferenceSheet(
    nsPresContext* aPresContext) {
||||||| merged common ancestors
StyleSheet*
nsLayoutStylesheetCache::UASheet()
{
  if (!mUASheet) {
    LoadSheetURL("resource://gre-resources/ua.css",
                 &mUASheet, eAgentSheetFeatures, eCrash);
  }

  return mUASheet;
}

StyleSheet*
nsLayoutStylesheetCache::HTMLSheet()
{
  return mHTMLSheet;
}

StyleSheet*
nsLayoutStylesheetCache::MinimalXULSheet()
{
  return mMinimalXULSheet;
}

StyleSheet*
nsLayoutStylesheetCache::XULSheet()
{
  if (!mXULSheet) {
    LoadSheetURL("chrome://global/content/xul.css",
                 &mXULSheet, eAgentSheetFeatures, eCrash);
  }

  return mXULSheet;
}

StyleSheet*
nsLayoutStylesheetCache::XULComponentsSheet()
{
  if (!mXULComponentsSheet) {
    LoadSheetURL("chrome://global/content/components.css",
                 &mXULComponentsSheet, eAgentSheetFeatures, eCrash);
  }

  return mXULComponentsSheet;
}

StyleSheet*
nsLayoutStylesheetCache::QuirkSheet()
{
  return mQuirkSheet;
}

StyleSheet*
nsLayoutStylesheetCache::SVGSheet()
{
  return mSVGSheet;
}

StyleSheet*
nsLayoutStylesheetCache::MathMLSheet()
{
  if (!mMathMLSheet) {
    LoadSheetURL("resource://gre-resources/mathml.css",
                 &mMathMLSheet, eAgentSheetFeatures, eCrash);
  }

  return mMathMLSheet;
}

StyleSheet*
nsLayoutStylesheetCache::CounterStylesSheet()
{
  return mCounterStylesSheet;
}

StyleSheet*
nsLayoutStylesheetCache::NoScriptSheet()
{
  if (!mNoScriptSheet) {
    LoadSheetURL("resource://gre-resources/noscript.css",
                 &mNoScriptSheet, eAgentSheetFeatures, eCrash);
  }

  return mNoScriptSheet;
}

StyleSheet*
nsLayoutStylesheetCache::NoFramesSheet()
{
  if (!mNoFramesSheet) {
    LoadSheetURL("resource://gre-resources/noframes.css",
                 &mNoFramesSheet, eAgentSheetFeatures, eCrash);
  }

  return mNoFramesSheet;
}

StyleSheet*
nsLayoutStylesheetCache::ChromePreferenceSheet(nsPresContext* aPresContext)
{
=======
StyleSheet* nsLayoutStylesheetCache::ChromePreferenceSheet() {
>>>>>>> upstream-releases
  if (!mChromePreferenceSheet) {
    BuildPreferenceSheet(&mChromePreferenceSheet,
                         PreferenceSheet::ChromePrefs());
  }

  return mChromePreferenceSheet;
}

<<<<<<< HEAD
StyleSheet* nsLayoutStylesheetCache::ContentPreferenceSheet(
    nsPresContext* aPresContext) {
||||||| merged common ancestors
StyleSheet*
nsLayoutStylesheetCache::ContentPreferenceSheet(nsPresContext* aPresContext)
{
=======
StyleSheet* nsLayoutStylesheetCache::ContentPreferenceSheet() {
>>>>>>> upstream-releases
  if (!mContentPreferenceSheet) {
    BuildPreferenceSheet(&mContentPreferenceSheet,
                         PreferenceSheet::ContentPrefs());
  }

  return mContentPreferenceSheet;
}

void nsLayoutStylesheetCache::Shutdown() {
  gCSSLoader = nullptr;
  NS_WARNING_ASSERTION(!gStyleCache || !gUserContentSheetURL,
                       "Got the URL but never used?");
  gStyleCache = nullptr;
  gUserContentSheetURL = nullptr;
  for (auto& r : URLExtraData::sShared) {
    r = nullptr;
  }
  // Some content processes don't get around to consuming the shared memory
  // buffer we store in sSharedMemory (e.g. a preloaded content process that
  // doesn't get a document loaded in it), so clear it out here to avoid leaks.
  sSharedMemory = nullptr;
}

void nsLayoutStylesheetCache::SetUserContentCSSURL(nsIURI* aURI) {
  MOZ_ASSERT(XRE_IsContentProcess(), "Only used in content processes.");
  gUserContentSheetURL = aURI;
}

MOZ_DEFINE_MALLOC_SIZE_OF(LayoutStylesheetCacheMallocSizeOf)

NS_IMETHODIMP
nsLayoutStylesheetCache::CollectReports(nsIHandleReportCallback* aHandleReport,
<<<<<<< HEAD
                                        nsISupports* aData, bool aAnonymize) {
  MOZ_COLLECT_REPORT("explicit/layout/style-sheet-cache", KIND_HEAP,
                     UNITS_BYTES,
                     SizeOfIncludingThis(LayoutStylesheetCacheMallocSizeOf),
                     "Memory used for some built-in style sheets.");
||||||| merged common ancestors
                                        nsISupports* aData, bool aAnonymize)
{
  MOZ_COLLECT_REPORT(
    "explicit/layout/style-sheet-cache", KIND_HEAP, UNITS_BYTES,
    SizeOfIncludingThis(LayoutStylesheetCacheMallocSizeOf),
    "Memory used for some built-in style sheets.");
=======
                                        nsISupports* aData, bool aAnonymize) {
  MOZ_COLLECT_REPORT("explicit/layout/style-sheet-cache/unshared", KIND_HEAP,
                     UNITS_BYTES,
                     SizeOfIncludingThis(LayoutStylesheetCacheMallocSizeOf),
                     "Memory used for built-in style sheets that are not "
                     "shared between processes.");

  if (XRE_IsParentProcess()) {
    MOZ_COLLECT_REPORT(
        "explicit/layout/style-sheet-cache/shared", KIND_NONHEAP, UNITS_BYTES,
        mSharedMemory ? mUsedSharedMemory : 0,
        "Memory used for built-in style sheets that are shared to "
        "child processes.");
  }
>>>>>>> upstream-releases

  return NS_OK;
}

size_t nsLayoutStylesheetCache::SizeOfIncludingThis(
    mozilla::MallocSizeOf aMallocSizeOf) const {
  size_t n = aMallocSizeOf(this);

<<<<<<< HEAD
#define MEASURE(s) n += s ? s->SizeOfIncludingThis(aMallocSizeOf) : 0;

#define STYLE_SHEET(identifier_, url_, lazy_) MEASURE(m##identifier_##Sheet);
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET
||||||| merged common ancestors
  #define MEASURE(s) n += s ? s->SizeOfIncludingThis(aMallocSizeOf) : 0;
=======
#define MEASURE(s) n += s ? s->SizeOfIncludingThis(aMallocSizeOf) : 0;

#define STYLE_SHEET(identifier_, url_, shared_) MEASURE(m##identifier_##Sheet);
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET
>>>>>>> upstream-releases

  MEASURE(mChromePreferenceSheet);
  MEASURE(mContentPreferenceSheet);
  MEASURE(mUserChromeSheet);
  MEASURE(mUserContentSheet);

  // Measurement of the following members may be added later if DMD finds it is
  // worthwhile:
  // - gCSSLoader

  return n;
}

<<<<<<< HEAD
nsLayoutStylesheetCache::nsLayoutStylesheetCache() {
  nsCOMPtr<nsIObserverService> obsSvc = mozilla::services::GetObserverService();
||||||| merged common ancestors
nsLayoutStylesheetCache::nsLayoutStylesheetCache()
{
  nsCOMPtr<nsIObserverService> obsSvc =
    mozilla::services::GetObserverService();
=======
nsLayoutStylesheetCache::nsLayoutStylesheetCache() : mUsedSharedMemory(0) {
  nsCOMPtr<nsIObserverService> obsSvc = mozilla::services::GetObserverService();
>>>>>>> upstream-releases
  NS_ASSERTION(obsSvc, "No global observer service?");

  if (obsSvc) {
    obsSvc->AddObserver(this, "profile-before-change", false);
    obsSvc->AddObserver(this, "profile-do-change", false);
  }

  // Load user style sheets.
  InitFromProfile();

<<<<<<< HEAD
  // And make sure that we load our UA sheets.  No need to do this
  // per-profile, since they're profile-invariant.
#define STYLE_SHEET(identifier_, url_, lazy_)                                \
  if (!lazy_) {                                                              \
    LoadSheetURL(url_, &m##identifier_##Sheet, eAgentSheetFeatures, eCrash); \
  }
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET

||||||| merged common ancestors
  // And make sure that we load our UA sheets.  No need to do this
  // per-profile, since they're profile-invariant.
  LoadSheetURL("resource://gre-resources/counterstyles.css",
               &mCounterStylesSheet, eAgentSheetFeatures, eCrash);
  LoadSheetURL("resource://gre-resources/html.css",
               &mHTMLSheet, eAgentSheetFeatures, eCrash);
  LoadSheetURL("chrome://global/content/minimal-xul.css",
               &mMinimalXULSheet, eAgentSheetFeatures, eCrash);
  LoadSheetURL("resource://gre-resources/quirk.css",
               &mQuirkSheet, eAgentSheetFeatures, eCrash);
  LoadSheetURL("resource://gre/res/svg.css",
               &mSVGSheet, eAgentSheetFeatures, eCrash);
=======
>>>>>>> upstream-releases
  if (XRE_IsParentProcess()) {
    // We know we need xul.css for the UI, so load that now too:
    XULSheet();
  }

  if (gUserContentSheetURL) {
    MOZ_ASSERT(XRE_IsContentProcess(), "Only used in content processes.");
    LoadSheet(gUserContentSheetURL, &mUserContentSheet, eUserSheetFeatures,
              eLogToConsole);
    gUserContentSheetURL = nullptr;
  }

  // If we are the in the parent process, then we load all of the UA sheets that
  // are shareable and store them into shared memory.  In both the parent and
  // the content process, we load these sheets out of shared memory.
  //
  // The shared memory buffer's format is a Header object, which contains
  // internal pointers to each of the shared style sheets, followed by the style
  // sheets themselves.
  if (StaticPrefs::layout_css_shared_memory_ua_sheets_enabled()) {
    if (XRE_IsParentProcess()) {
      MOZ_ASSERT(!sSharedMemory);
      // Load the style sheets and store them in a new shared memory buffer.
      InitSharedSheetsInParent();
    } else if (sSharedMemory) {
      // Use the shared memory handle that was given to us by a SetSharedMemory
      // call under ContentChild::InitXPCOM.
      mSharedMemory = sSharedMemory.forget();
    }
  }

  // If we get here and we don't have a shared memory handle, then it means
  // either we failed to create the shared memory buffer in the parent process
  // (unexpected), or we failed to map the shared memory buffer at the address
  // we needed in the content process (might happen).
  //
  // In the parent process, this means we'll just leave our eagerly loaded
  // non-shared sheets in the mFooSheet fields.  In a content process, we'll
  // lazily load our own copies of the sheets later.
  if (mSharedMemory) {
    Header* header = static_cast<Header*>(mSharedMemory->mShm.memory());
    MOZ_RELEASE_ASSERT(header->mMagic == Header::kMagic);

#define STYLE_SHEET(identifier_, url_, shared_)                           \
  if (shared_) {                                                          \
    LoadSheetFromSharedMemory(url_, &m##identifier_##Sheet,               \
                              eAgentSheetFeatures, mSharedMemory, header, \
                              UserAgentStyleSheetID::identifier_);        \
  }
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET
  }
}

<<<<<<< HEAD
nsLayoutStylesheetCache::~nsLayoutStylesheetCache() {
||||||| merged common ancestors
nsLayoutStylesheetCache::~nsLayoutStylesheetCache()
{
=======
void nsLayoutStylesheetCache::LoadSheetFromSharedMemory(
    const char* aURL, RefPtr<StyleSheet>* aSheet, SheetParsingMode aParsingMode,
    Shm* aSharedMemory, Header* aHeader, UserAgentStyleSheetID aSheetID) {
  auto i = size_t(aSheetID);

  auto sheet = MakeRefPtr<StyleSheet>(
      aParsingMode, CORS_NONE, mozilla::net::RP_Unset, dom::SRIMetadata());

  nsCOMPtr<nsIURI> uri;
  MOZ_ALWAYS_SUCCEEDS(NS_NewURI(getter_AddRefs(uri), aURL));

  sheet->SetPrincipal(nsContentUtils::GetSystemPrincipal());
  sheet->SetURIs(uri, uri, uri);
  sheet->SetSharedContents(aSharedMemory, aHeader->mSheets[i]);
  sheet->SetComplete();

  URLExtraData::sShared[i] = sheet->URLData();

  *aSheet = sheet.forget();
}

void nsLayoutStylesheetCache::InitSharedSheetsInParent() {
  MOZ_ASSERT(XRE_IsParentProcess());

  mSharedMemory = new Shm();
  mSharedMemory->mShm.Create(kSharedMemorySize);

  // We need to choose an address to map the shared memory in the parent process
  // that we'll also be able to use in content processes.  There's no way to
  // pick an address that is guaranteed to be free in future content processes,
  // so instead we pick an address that is some distance away from current heap
  // allocations and hope that by the time the content process maps the shared
  // memory, that address will be free.
  //
  // On 64 bit, we have a large amount of address space, so we pick an address
  // half way through the next 8 GiB of free space, and this has a very good
  // chance of succeeding.  On 32 bit, address space is more constrained.  We
  // only have 3 GiB of space to work with, and we don't want to pick a location
  // right in the middle, since that could cause future large allocations to
  // fail.  So we pick an address half way through the next 512 MiB of free
  // space.  Experimentally this seems to work 9 times out of 10; this is good
  // enough, as it means only 1 in 10 content processes will have its own unique
  // copies of the UA style sheets, and we're still getting a significant
  // overall memory saving.
  //
  // In theory ASLR could reduce the likelihood of the mapping succeeding in
  // content processes, due to our expectations of where the heap is being
  // wrong, but in practice this isn't an issue.
#ifdef HAVE_64BIT_BUILD
  constexpr size_t kOffset = 0x200000000ULL;  // 8 GiB
#else
  constexpr size_t kOffset = 0x20000000;  // 512 MiB
#endif

  void* address = nullptr;
  if (void* p = base::SharedMemory::FindFreeAddressSpace(2 * kOffset)) {
    address = reinterpret_cast<void*>(uintptr_t(p) + kOffset);
  }
  if (!mSharedMemory->mShm.Map(kSharedMemorySize, address)) {
    // Failed to map at the address we computed for some reason.  Fall back
    // to just allocating at a location of the OS's choosing, and hope that
    // it works in the content process.
    mSharedMemory->mShm.Map(kSharedMemorySize);
  }

  Header* header = static_cast<Header*>(mSharedMemory->mShm.memory());
  header->mMagic = Header::kMagic;
#ifdef DEBUG
  for (auto ptr : header->mSheets) {
    MOZ_RELEASE_ASSERT(!ptr, "expected shared memory to have been zeroed");
  }
#endif

  UniquePtr<RawServoSharedMemoryBuilder> builder(
      Servo_SharedMemoryBuilder_Create(
          header->mBuffer, kSharedMemorySize - offsetof(Header, mBuffer)));

  // Copy each one into the shared memory, and record its pointer.
#define STYLE_SHEET(identifier_, url_, shared_)            \
  if (shared_) {                                           \
    StyleSheet* sheet = identifier_##Sheet();              \
    size_t i = size_t(UserAgentStyleSheetID::identifier_); \
    URLExtraData::sShared[i] = sheet->URLData();           \
    header->mSheets[i] = sheet->ToShared(builder.get());   \
  }
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET

  // Record how must of the shared memory we have used, for memory reporting
  // later.  We round up to the nearest page since the free space at the end
  // of the page isn't really usable for anything else.
  //
  // TODO(heycam): This won't be true on Windows unless we allow creating the
  // shared memory with SEC_RESERVE so that the pages are reserved but not
  // committed.
  size_t pageSize = ipc::SharedMemory::SystemPageSize();
  mUsedSharedMemory =
      (Servo_SharedMemoryBuilder_GetLength(builder.get()) + pageSize - 1) &
      ~(pageSize - 1);
}

nsLayoutStylesheetCache::~nsLayoutStylesheetCache() {
>>>>>>> upstream-releases
  mozilla::UnregisterWeakMemoryReporter(this);
}

void nsLayoutStylesheetCache::InitMemoryReporter() {
  mozilla::RegisterWeakMemoryReporter(this);
}

<<<<<<< HEAD
/* static */ nsLayoutStylesheetCache* nsLayoutStylesheetCache::Singleton() {
||||||| merged common ancestors
/* static */ nsLayoutStylesheetCache*
nsLayoutStylesheetCache::Singleton()
{
=======
/* static */
nsLayoutStylesheetCache* nsLayoutStylesheetCache::Singleton() {
>>>>>>> upstream-releases
  MOZ_ASSERT(NS_IsMainThread());

  if (!gStyleCache) {
    gStyleCache = new nsLayoutStylesheetCache;
    gStyleCache->InitMemoryReporter();

    // For each pref that controls a CSS feature that a UA style sheet depends
    // on (such as a pref that enables a property that a UA style sheet uses),
    // register DependentPrefChanged as a callback to ensure that the relevant
    // style sheets will be re-parsed.
    // Preferences::RegisterCallback(&DependentPrefChanged,
    //                               "layout.css.example-pref.enabled");
  }

  return gStyleCache;
}

<<<<<<< HEAD
void nsLayoutStylesheetCache::InitFromProfile() {
  nsCOMPtr<nsIXULRuntime> appInfo =
      do_GetService("@mozilla.org/xre/app-info;1");
||||||| merged common ancestors
void
nsLayoutStylesheetCache::InitFromProfile()
{
  nsCOMPtr<nsIXULRuntime> appInfo = do_GetService("@mozilla.org/xre/app-info;1");
=======
void nsLayoutStylesheetCache::InitFromProfile() {
  if (!Preferences::GetBool(PREF_LEGACY_STYLESHEET_CUSTOMIZATION)) {
    return;
  }

  nsCOMPtr<nsIXULRuntime> appInfo =
      do_GetService("@mozilla.org/xre/app-info;1");
>>>>>>> upstream-releases
  if (appInfo) {
    bool inSafeMode = false;
    appInfo->GetInSafeMode(&inSafeMode);
    if (inSafeMode) return;
  }
  nsCOMPtr<nsIFile> contentFile;
  nsCOMPtr<nsIFile> chromeFile;

  NS_GetSpecialDirectory(NS_APP_USER_CHROME_DIR, getter_AddRefs(contentFile));
  if (!contentFile) {
    // if we don't have a profile yet, that's OK!
    return;
  }

  contentFile->Clone(getter_AddRefs(chromeFile));
  if (!chromeFile) return;

  contentFile->Append(NS_LITERAL_STRING("userContent.css"));
  chromeFile->Append(NS_LITERAL_STRING("userChrome.css"));

<<<<<<< HEAD
  LoadSheetFile(contentFile, &mUserContentSheet, eUserSheetFeatures,
                eLogToConsole);
  LoadSheetFile(chromeFile, &mUserChromeSheet, eUserSheetFeatures,
                eLogToConsole);

  if (XRE_IsParentProcess()) {
    // We're interested specifically in potential chrome customizations,
    // so we only need data points from the parent process
    Telemetry::Accumulate(Telemetry::USER_CHROME_CSS_LOADED,
                          mUserChromeSheet != nullptr);
  }
||||||| merged common ancestors
  LoadSheetFile(contentFile, &mUserContentSheet, eUserSheetFeatures, eLogToConsole);
  LoadSheetFile(chromeFile, &mUserChromeSheet, eUserSheetFeatures, eLogToConsole);

  if (XRE_IsParentProcess()) {
    // We're interested specifically in potential chrome customizations,
    // so we only need data points from the parent process
    Telemetry::Accumulate(Telemetry::USER_CHROME_CSS_LOADED, mUserChromeSheet != nullptr);
  }
=======
  LoadSheetFile(contentFile, &mUserContentSheet, eUserSheetFeatures,
                eLogToConsole);
  LoadSheetFile(chromeFile, &mUserChromeSheet, eUserSheetFeatures,
                eLogToConsole);
>>>>>>> upstream-releases
}

void nsLayoutStylesheetCache::LoadSheetURL(const char* aURL,
                                           RefPtr<StyleSheet>* aSheet,
                                           SheetParsingMode aParsingMode,
                                           FailureAction aFailureAction) {
  nsCOMPtr<nsIURI> uri;
  NS_NewURI(getter_AddRefs(uri), aURL);
  LoadSheet(uri, aSheet, aParsingMode, aFailureAction);
  if (!aSheet) {
    NS_ERROR(nsPrintfCString("Could not load %s", aURL).get());
  }
}

void nsLayoutStylesheetCache::LoadSheetFile(nsIFile* aFile,
                                            RefPtr<StyleSheet>* aSheet,
                                            SheetParsingMode aParsingMode,
                                            FailureAction aFailureAction) {
  bool exists = false;
  aFile->Exists(&exists);

  if (!exists) return;

  nsCOMPtr<nsIURI> uri;
  NS_NewFileURI(getter_AddRefs(uri), aFile);

  LoadSheet(uri, aSheet, aParsingMode, aFailureAction);
}

<<<<<<< HEAD
static inline nsresult ComputeCRC32(nsIFile* aFile, uint32_t* aResult) {
  PRFileDesc* fd;
  nsresult rv = aFile->OpenNSPRFileDesc(PR_RDONLY, 0, &fd);
  NS_ENSURE_SUCCESS(rv, rv);

  uint32_t crc = crc32(0, nullptr, 0);

  unsigned char buf[512];
  int32_t n;
  while ((n = PR_Read(fd, buf, sizeof(buf))) > 0) {
    crc = crc32(crc, buf, n);
  }
  PR_Close(fd);

  if (n < 0) {
    return NS_ERROR_FAILURE;
  }

  *aResult = crc;
  return NS_OK;
}

static void ListInterestingFiles(
    nsString& aAnnotation, nsIFile* aFile,
    const nsTArray<nsString>& aInterestingFilenames) {
  nsString filename;
  aFile->GetLeafName(filename);
  for (const nsString& interestingFilename : aInterestingFilenames) {
    if (interestingFilename == filename) {
      nsString path;
      aFile->GetPath(path);
      aAnnotation.AppendLiteral("  ");
      aAnnotation.Append(path);
      aAnnotation.AppendLiteral(" (");
      int64_t size;
      if (NS_SUCCEEDED(aFile->GetFileSize(&size))) {
        aAnnotation.AppendPrintf("%" PRId64, size);
      } else {
        aAnnotation.AppendLiteral("???");
      }
      aAnnotation.AppendLiteral(" bytes, crc32 = ");
      uint32_t crc;
      nsresult rv = ComputeCRC32(aFile, &crc);
      if (NS_SUCCEEDED(rv)) {
        aAnnotation.AppendPrintf("0x%08x)\n", crc);
      } else {
        aAnnotation.AppendPrintf("error 0x%08x)\n", uint32_t(rv));
      }
      return;
    }
  }

  bool isDir = false;
  aFile->IsDirectory(&isDir);

  if (!isDir) {
    return;
  }

  nsCOMPtr<nsIDirectoryEnumerator> entries;
  if (NS_FAILED(aFile->GetDirectoryEntries(getter_AddRefs(entries)))) {
    aAnnotation.AppendLiteral("  (failed to enumerated directory)\n");
    return;
  }

  for (;;) {
    nsCOMPtr<nsIFile> file;
    if (NS_FAILED(entries->GetNextFile(getter_AddRefs(file)))) {
      aAnnotation.AppendLiteral("  (failed during directory enumeration)\n");
      return;
    }
    if (!file) {
      break;
    }
    ListInterestingFiles(aAnnotation, file, aInterestingFilenames);
  }
}

// Generate a crash report annotation to help debug issues with style
// sheets failing to load (bug 1194856).
static void AnnotateCrashReport(nsIURI* aURI) {
  nsAutoCString spec;
  nsAutoCString scheme;
  nsDependentCSubstring filename;
  if (aURI) {
    spec = aURI->GetSpecOrDefault();
    aURI->GetScheme(scheme);
    int32_t i = spec.RFindChar('/');
    if (i != -1) {
      filename.Rebind(spec, i + 1);
    }
  }

  nsString annotation;

  // The URL of the sheet that failed to load.
  annotation.AppendLiteral("Error loading sheet: ");
  annotation.Append(NS_ConvertUTF8toUTF16(spec).get());
  annotation.Append('\n');

  annotation.AppendLiteral("NS_ERROR_FILE_CORRUPTION reason: ");
  if (nsZipArchive::sFileCorruptedReason) {
    annotation.Append(
        NS_ConvertUTF8toUTF16(nsZipArchive::sFileCorruptedReason).get());
    annotation.Append('\n');
  } else {
    annotation.AppendLiteral("(none)\n");
  }

  // The jar: or file: URL that the sheet's resource: or chrome: URL
  // resolves to.
  if (scheme.EqualsLiteral("resource")) {
    annotation.AppendLiteral("Real location: ");
    nsCOMPtr<nsISubstitutingProtocolHandler> handler;
    nsCOMPtr<nsIIOService> io(do_GetIOService());
    if (io) {
      nsCOMPtr<nsIProtocolHandler> ph;
      io->GetProtocolHandler(scheme.get(), getter_AddRefs(ph));
      if (ph) {
        handler = do_QueryInterface(ph);
      }
    }
    if (!handler) {
      annotation.AppendLiteral("(ResolveURI failed)\n");
    } else {
      nsAutoCString resolvedSpec;
      nsresult rv = handler->ResolveURI(aURI, resolvedSpec);
      if (NS_FAILED(rv)) {
        annotation.AppendPrintf("(ResolveURI failed with 0x%08" PRIx32 ")\n",
                                static_cast<uint32_t>(rv));
      }
      annotation.Append(NS_ConvertUTF8toUTF16(resolvedSpec));
      annotation.Append('\n');
    }
  } else if (scheme.EqualsLiteral("chrome")) {
    annotation.AppendLiteral("Real location: ");
    nsCOMPtr<nsIChromeRegistry> reg =
        mozilla::services::GetChromeRegistryService();
    if (!reg) {
      annotation.AppendLiteral("(no chrome registry)\n");
    } else {
      nsCOMPtr<nsIURI> resolvedURI;
      reg->ConvertChromeURL(aURI, getter_AddRefs(resolvedURI));
      if (!resolvedURI) {
        annotation.AppendLiteral("(ConvertChromeURL failed)\n");
      } else {
        annotation.Append(
            NS_ConvertUTF8toUTF16(resolvedURI->GetSpecOrDefault()));
        annotation.Append('\n');
      }
    }
  }

  nsTArray<nsString> interestingFiles;
  interestingFiles.AppendElement(NS_LITERAL_STRING("chrome.manifest"));
  interestingFiles.AppendElement(NS_LITERAL_STRING("omni.ja"));
  interestingFiles.AppendElement(NS_ConvertUTF8toUTF16(filename));

  annotation.AppendLiteral("GRE directory: ");
  nsCOMPtr<nsIFile> file;
  nsDirectoryService::gService->Get(NS_GRE_DIR, NS_GET_IID(nsIFile),
                                    getter_AddRefs(file));
  if (file) {
    // The Firefox installation directory.
    nsString path;
    file->GetPath(path);
    annotation.Append(path);
    annotation.Append('\n');

    // List interesting files -- any chrome.manifest or omni.ja file or any file
    // whose name is the sheet's filename -- under the Firefox installation
    // directory.
    annotation.AppendLiteral("Interesting files in the GRE directory:\n");
    ListInterestingFiles(annotation, file, interestingFiles);

    // If the Firefox installation directory has a chrome.manifest file, let's
    // see what's in it.
    file->Append(NS_LITERAL_STRING("chrome.manifest"));
    bool exists = false;
    file->Exists(&exists);
    if (exists) {
      annotation.AppendLiteral("Contents of chrome.manifest:\n[[[\n");
      PRFileDesc* fd;
      if (NS_SUCCEEDED(file->OpenNSPRFileDesc(PR_RDONLY, 0, &fd))) {
        nsCString contents;
        char buf[512];
        int32_t n;
        while ((n = PR_Read(fd, buf, sizeof(buf))) > 0) {
          contents.Append(buf, n);
        }
        if (n < 0) {
          annotation.AppendLiteral("  (error while reading)\n");
        } else {
          annotation.Append(NS_ConvertUTF8toUTF16(contents));
        }
        PR_Close(fd);
      }
      annotation.AppendLiteral("]]]\n");
    }
  } else {
    annotation.AppendLiteral("(none)\n");
  }

  // The jar: or file: URL prefix that chrome: and resource: URLs get translated
  // to.
  annotation.AppendLiteral("GRE omnijar URI string: ");
  nsCString uri;
  nsresult rv = Omnijar::GetURIString(Omnijar::GRE, uri);
  if (NS_FAILED(rv)) {
    annotation.AppendLiteral("(failed)\n");
  } else {
    annotation.Append(NS_ConvertUTF8toUTF16(uri));
    annotation.Append('\n');
  }

  RefPtr<nsZipArchive> zip = Omnijar::GetReader(Omnijar::GRE);
  if (zip) {
    // List interesting files in the GRE omnijar.
    annotation.AppendLiteral("Interesting files in the GRE omnijar:\n");
    nsZipFind* find;
    rv = zip->FindInit(nullptr, &find);
    if (NS_FAILED(rv)) {
      annotation.AppendPrintf("  (FindInit failed with 0x%08" PRIx32 ")\n",
                              static_cast<uint32_t>(rv));
    } else if (!find) {
      annotation.AppendLiteral("  (FindInit returned null)\n");
    } else {
      const char* result;
      uint16_t len;
      while (NS_SUCCEEDED(find->FindNext(&result, &len))) {
        nsCString itemPathname;
        nsString itemFilename;
        itemPathname.Append(result, len);
        int32_t i = itemPathname.RFindChar('/');
        if (i != -1) {
          itemFilename = NS_ConvertUTF8toUTF16(Substring(itemPathname, i + 1));
        }
        for (const nsString& interestingFile : interestingFiles) {
          if (interestingFile == itemFilename) {
            annotation.AppendLiteral("  ");
            annotation.Append(NS_ConvertUTF8toUTF16(itemPathname));
            nsZipItem* item = zip->GetItem(itemPathname.get());
            if (!item) {
              annotation.AppendLiteral(" (GetItem failed)\n");
            } else {
              annotation.AppendPrintf(" (%d bytes, crc32 = 0x%08x)\n",
                                      item->RealSize(), item->CRC32());
            }
            break;
          }
        }
      }
      delete find;
    }
  } else {
    annotation.AppendLiteral("No GRE omnijar\n");
  }

  CrashReporter::AnnotateCrashReport(
      CrashReporter::Annotation::SheetLoadFailure,
      NS_ConvertUTF16toUTF8(annotation));
}

static void ErrorLoadingSheet(nsIURI* aURI, const char* aMsg,
                              FailureAction aFailureAction) {
  nsPrintfCString errorMessage("%s loading built-in stylesheet '%s'", aMsg,
||||||| merged common ancestors
static inline nsresult
ComputeCRC32(nsIFile* aFile, uint32_t* aResult)
{
  PRFileDesc* fd;
  nsresult rv = aFile->OpenNSPRFileDesc(PR_RDONLY, 0, &fd);
  NS_ENSURE_SUCCESS(rv, rv);

  uint32_t crc = crc32(0, nullptr, 0);

  unsigned char buf[512];
  int32_t n;
  while ((n = PR_Read(fd, buf, sizeof(buf))) > 0) {
    crc = crc32(crc, buf, n);
  }
  PR_Close(fd);

  if (n < 0) {
    return NS_ERROR_FAILURE;
  }

  *aResult = crc;
  return NS_OK;
}

static void
ListInterestingFiles(nsString& aAnnotation, nsIFile* aFile,
                     const nsTArray<nsString>& aInterestingFilenames)
{
  nsString filename;
  aFile->GetLeafName(filename);
  for (const nsString& interestingFilename : aInterestingFilenames) {
    if (interestingFilename == filename) {
      nsString path;
      aFile->GetPath(path);
      aAnnotation.AppendLiteral("  ");
      aAnnotation.Append(path);
      aAnnotation.AppendLiteral(" (");
      int64_t size;
      if (NS_SUCCEEDED(aFile->GetFileSize(&size))) {
        aAnnotation.AppendPrintf("%" PRId64, size);
      } else {
        aAnnotation.AppendLiteral("???");
      }
      aAnnotation.AppendLiteral(" bytes, crc32 = ");
      uint32_t crc;
      nsresult rv = ComputeCRC32(aFile, &crc);
      if (NS_SUCCEEDED(rv)) {
        aAnnotation.AppendPrintf("0x%08x)\n", crc);
      } else {
        aAnnotation.AppendPrintf("error 0x%08x)\n", uint32_t(rv));
      }
      return;
    }
  }

  bool isDir = false;
  aFile->IsDirectory(&isDir);

  if (!isDir) {
    return;
  }

  nsCOMPtr<nsIDirectoryEnumerator> entries;
  if (NS_FAILED(aFile->GetDirectoryEntries(getter_AddRefs(entries)))) {
    aAnnotation.AppendLiteral("  (failed to enumerated directory)\n");
    return;
  }

  for (;;) {
    nsCOMPtr<nsIFile> file;
    if (NS_FAILED(entries->GetNextFile(getter_AddRefs(file)))) {
      aAnnotation.AppendLiteral("  (failed during directory enumeration)\n");
      return;
    }
    if (!file) {
      break;
    }
    ListInterestingFiles(aAnnotation, file, aInterestingFilenames);
  }
}

// Generate a crash report annotation to help debug issues with style
// sheets failing to load (bug 1194856).
static void
AnnotateCrashReport(nsIURI* aURI)
{
  nsAutoCString spec;
  nsAutoCString scheme;
  nsDependentCSubstring filename;
  if (aURI) {
    spec = aURI->GetSpecOrDefault();
    aURI->GetScheme(scheme);
    int32_t i = spec.RFindChar('/');
    if (i != -1) {
      filename.Rebind(spec, i + 1);
    }
  }

  nsString annotation;

  // The URL of the sheet that failed to load.
  annotation.AppendLiteral("Error loading sheet: ");
  annotation.Append(NS_ConvertUTF8toUTF16(spec).get());
  annotation.Append('\n');

  annotation.AppendLiteral("NS_ERROR_FILE_CORRUPTION reason: ");
  if (nsZipArchive::sFileCorruptedReason) {
    annotation.Append(NS_ConvertUTF8toUTF16(nsZipArchive::sFileCorruptedReason).get());
    annotation.Append('\n');
  } else {
    annotation.AppendLiteral("(none)\n");
  }

  // The jar: or file: URL that the sheet's resource: or chrome: URL
  // resolves to.
  if (scheme.EqualsLiteral("resource")) {
    annotation.AppendLiteral("Real location: ");
    nsCOMPtr<nsISubstitutingProtocolHandler> handler;
    nsCOMPtr<nsIIOService> io(do_GetIOService());
    if (io) {
      nsCOMPtr<nsIProtocolHandler> ph;
      io->GetProtocolHandler(scheme.get(), getter_AddRefs(ph));
      if (ph) {
        handler = do_QueryInterface(ph);
      }
    }
    if (!handler) {
      annotation.AppendLiteral("(ResolveURI failed)\n");
    } else {
      nsAutoCString resolvedSpec;
      nsresult rv = handler->ResolveURI(aURI, resolvedSpec);
      if (NS_FAILED(rv)) {
        annotation.AppendPrintf("(ResolveURI failed with 0x%08" PRIx32 ")\n",
                                static_cast<uint32_t>(rv));
      }
      annotation.Append(NS_ConvertUTF8toUTF16(resolvedSpec));
      annotation.Append('\n');
    }
  } else if (scheme.EqualsLiteral("chrome")) {
    annotation.AppendLiteral("Real location: ");
    nsCOMPtr<nsIChromeRegistry> reg =
      mozilla::services::GetChromeRegistryService();
    if (!reg) {
      annotation.AppendLiteral("(no chrome registry)\n");
    } else {
      nsCOMPtr<nsIURI> resolvedURI;
      reg->ConvertChromeURL(aURI, getter_AddRefs(resolvedURI));
      if (!resolvedURI) {
        annotation.AppendLiteral("(ConvertChromeURL failed)\n");
      } else {
        annotation.Append(
          NS_ConvertUTF8toUTF16(resolvedURI->GetSpecOrDefault()));
        annotation.Append('\n');
      }
    }
  }

  nsTArray<nsString> interestingFiles;
  interestingFiles.AppendElement(NS_LITERAL_STRING("chrome.manifest"));
  interestingFiles.AppendElement(NS_LITERAL_STRING("omni.ja"));
  interestingFiles.AppendElement(NS_ConvertUTF8toUTF16(filename));

  annotation.AppendLiteral("GRE directory: ");
  nsCOMPtr<nsIFile> file;
  nsDirectoryService::gService->Get(NS_GRE_DIR, NS_GET_IID(nsIFile),
                                    getter_AddRefs(file));
  if (file) {
    // The Firefox installation directory.
    nsString path;
    file->GetPath(path);
    annotation.Append(path);
    annotation.Append('\n');

    // List interesting files -- any chrome.manifest or omni.ja file or any file
    // whose name is the sheet's filename -- under the Firefox installation
    // directory.
    annotation.AppendLiteral("Interesting files in the GRE directory:\n");
    ListInterestingFiles(annotation, file, interestingFiles);

    // If the Firefox installation directory has a chrome.manifest file, let's
    // see what's in it.
    file->Append(NS_LITERAL_STRING("chrome.manifest"));
    bool exists = false;
    file->Exists(&exists);
    if (exists) {
      annotation.AppendLiteral("Contents of chrome.manifest:\n[[[\n");
      PRFileDesc* fd;
      if (NS_SUCCEEDED(file->OpenNSPRFileDesc(PR_RDONLY, 0, &fd))) {
        nsCString contents;
        char buf[512];
        int32_t n;
        while ((n = PR_Read(fd, buf, sizeof(buf))) > 0) {
          contents.Append(buf, n);
        }
        if (n < 0) {
          annotation.AppendLiteral("  (error while reading)\n");
        } else {
          annotation.Append(NS_ConvertUTF8toUTF16(contents));
        }
        PR_Close(fd);
      }
      annotation.AppendLiteral("]]]\n");
    }
  } else {
    annotation.AppendLiteral("(none)\n");
  }

  // The jar: or file: URL prefix that chrome: and resource: URLs get translated
  // to.
  annotation.AppendLiteral("GRE omnijar URI string: ");
  nsCString uri;
  nsresult rv = Omnijar::GetURIString(Omnijar::GRE, uri);
  if (NS_FAILED(rv)) {
    annotation.AppendLiteral("(failed)\n");
  } else {
    annotation.Append(NS_ConvertUTF8toUTF16(uri));
    annotation.Append('\n');
  }

  RefPtr<nsZipArchive> zip = Omnijar::GetReader(Omnijar::GRE);
  if (zip) {
    // List interesting files in the GRE omnijar.
    annotation.AppendLiteral("Interesting files in the GRE omnijar:\n");
    nsZipFind* find;
    rv = zip->FindInit(nullptr, &find);
    if (NS_FAILED(rv)) {
      annotation.AppendPrintf("  (FindInit failed with 0x%08" PRIx32 ")\n",
                              static_cast<uint32_t>(rv));
    } else if (!find) {
      annotation.AppendLiteral("  (FindInit returned null)\n");
    } else {
      const char* result;
      uint16_t len;
      while (NS_SUCCEEDED(find->FindNext(&result, &len))) {
        nsCString itemPathname;
        nsString itemFilename;
        itemPathname.Append(result, len);
        int32_t i = itemPathname.RFindChar('/');
        if (i != -1) {
          itemFilename = NS_ConvertUTF8toUTF16(Substring(itemPathname, i + 1));
        }
        for (const nsString& interestingFile : interestingFiles) {
          if (interestingFile == itemFilename) {
            annotation.AppendLiteral("  ");
            annotation.Append(NS_ConvertUTF8toUTF16(itemPathname));
            nsZipItem* item = zip->GetItem(itemPathname.get());
            if (!item) {
              annotation.AppendLiteral(" (GetItem failed)\n");
            } else {
              annotation.AppendPrintf(" (%d bytes, crc32 = 0x%08x)\n",
                                      item->RealSize(),
                                      item->CRC32());
            }
            break;
          }
        }
      }
      delete find;
    }
  } else {
    annotation.AppendLiteral("No GRE omnijar\n");
  }

  CrashReporter::AnnotateCrashReport(
    CrashReporter::Annotation::SheetLoadFailure,
    NS_ConvertUTF16toUTF8(annotation));
}

static void
ErrorLoadingSheet(nsIURI* aURI, const char* aMsg, FailureAction aFailureAction)
{
  nsPrintfCString errorMessage("%s loading built-in stylesheet '%s'",
                               aMsg,
=======
static void ErrorLoadingSheet(nsIURI* aURI, const char* aMsg,
                              FailureAction aFailureAction) {
  nsPrintfCString errorMessage("%s loading built-in stylesheet '%s'", aMsg,
>>>>>>> upstream-releases
                               aURI ? aURI->GetSpecOrDefault().get() : "");
  if (aFailureAction == eLogToConsole) {
    nsCOMPtr<nsIConsoleService> cs =
        do_GetService(NS_CONSOLESERVICE_CONTRACTID);
    if (cs) {
      cs->LogStringMessage(NS_ConvertUTF8toUTF16(errorMessage).get());
      return;
    }
  }

  MOZ_CRASH_UNSAFE(errorMessage.get());
}

void nsLayoutStylesheetCache::LoadSheet(nsIURI* aURI,
                                        RefPtr<StyleSheet>* aSheet,
                                        SheetParsingMode aParsingMode,
                                        FailureAction aFailureAction) {
  if (!aURI) {
    ErrorLoadingSheet(aURI, "null URI", eCrash);
    return;
  }

  if (!gCSSLoader) {
    gCSSLoader = new Loader;
    if (!gCSSLoader) {
      ErrorLoadingSheet(aURI, "no Loader", eCrash);
      return;
    }
  }

  // Note: The parallel parsing code assume that UA sheets are always loaded
  // synchronously like they are here, and thus that we'll never attempt
  // parallel parsing on them. If that ever changes, we'll either need to find a
  // different way to prohibit parallel parsing for UA sheets, or handle
  // -moz-bool-pref and various other things in the parallel parsing code.
  nsresult rv = gCSSLoader->LoadSheetSync(aURI, aParsingMode, true, aSheet);
  if (NS_FAILED(rv)) {
    ErrorLoadingSheet(
        aURI,
        nsPrintfCString("LoadSheetSync failed with error %" PRIx32,
                        static_cast<uint32_t>(rv))
            .get(),
        aFailureAction);
  }
}

<<<<<<< HEAD
/* static */ void nsLayoutStylesheetCache::InvalidatePreferenceSheets() {
||||||| merged common ancestors
/* static */ void
nsLayoutStylesheetCache::InvalidatePreferenceSheets()
{
=======
/* static */
void nsLayoutStylesheetCache::InvalidatePreferenceSheets() {
>>>>>>> upstream-releases
  if (gStyleCache) {
    gStyleCache->mContentPreferenceSheet = nullptr;
    gStyleCache->mChromePreferenceSheet = nullptr;
  }
}

<<<<<<< HEAD
void nsLayoutStylesheetCache::BuildPreferenceSheet(
    RefPtr<StyleSheet>* aSheet, nsPresContext* aPresContext) {
  *aSheet = new StyleSheet(eAgentSheetFeatures, CORS_NONE,
                           mozilla::net::RP_Unset, dom::SRIMetadata());
||||||| merged common ancestors
void
nsLayoutStylesheetCache::BuildPreferenceSheet(RefPtr<StyleSheet>* aSheet,
                                              nsPresContext* aPresContext)
{
  *aSheet = new StyleSheet(eAgentSheetFeatures,
                           CORS_NONE,
                           mozilla::net::RP_Unset,
                           dom::SRIMetadata());
=======
void nsLayoutStylesheetCache::BuildPreferenceSheet(
    RefPtr<StyleSheet>* aSheet, const PreferenceSheet::Prefs& aPrefs) {
  *aSheet = new StyleSheet(eAgentSheetFeatures, CORS_NONE,
                           mozilla::net::RP_Unset, dom::SRIMetadata());
>>>>>>> upstream-releases

  StyleSheet* sheet = *aSheet;

  nsCOMPtr<nsIURI> uri;
  NS_NewURI(getter_AddRefs(uri), "about:PreferenceStyleSheet", nullptr);
  MOZ_ASSERT(uri, "URI creation shouldn't fail");

  sheet->SetURIs(uri, uri, uri);
  sheet->SetComplete();

  static const uint32_t kPreallocSize = 1024;

  nsCString sheetText;
  sheetText.SetCapacity(kPreallocSize);

#define NS_GET_R_G_B(color_) \
  NS_GET_R(color_), NS_GET_G(color_), NS_GET_B(color_)

  sheetText.AppendLiteral(
      "@namespace url(http://www.w3.org/1999/xhtml);\n"
      "@namespace svg url(http://www.w3.org/2000/svg);\n");

  // Rules for link styling.
  nscolor linkColor = aPrefs.mLinkColor;
  nscolor activeColor = aPrefs.mActiveLinkColor;
  nscolor visitedColor = aPrefs.mVisitedLinkColor;

  sheetText.AppendPrintf(
      "*|*:link { color: #%02x%02x%02x; }\n"
      "*|*:any-link:active { color: #%02x%02x%02x; }\n"
      "*|*:visited { color: #%02x%02x%02x; }\n",
      NS_GET_R_G_B(linkColor), NS_GET_R_G_B(activeColor),
      NS_GET_R_G_B(visitedColor));

<<<<<<< HEAD
  bool underlineLinks =
      aPresContext->GetCachedBoolPref(kPresContext_UnderlineLinks);
  sheetText.AppendPrintf("*|*:any-link%s { text-decoration: %s; }\n",
                         underlineLinks ? ":not(svg|a)" : "",
                         underlineLinks ? "underline" : "none");
||||||| merged common ancestors
  bool underlineLinks =
    aPresContext->GetCachedBoolPref(kPresContext_UnderlineLinks);
  sheetText.AppendPrintf(
      "*|*:any-link%s { text-decoration: %s; }\n",
      underlineLinks ? ":not(svg|a)" : "",
      underlineLinks ? "underline" : "none");
=======
  bool underlineLinks = aPrefs.mUnderlineLinks;
  sheetText.AppendPrintf("*|*:any-link%s { text-decoration: %s; }\n",
                         underlineLinks ? ":not(svg|a)" : "",
                         underlineLinks ? "underline" : "none");
>>>>>>> upstream-releases

  // Rules for focus styling.

  bool focusRingOnAnything = aPrefs.mFocusRingOnAnything;
  uint8_t focusRingWidth = aPrefs.mFocusRingWidth;
  uint8_t focusRingStyle = aPrefs.mFocusRingStyle;

  if ((focusRingWidth != 1 && focusRingWidth <= 4) || focusRingOnAnything) {
    if (focusRingWidth != 1) {
      // If the focus ring width is different from the default, fix buttons
      // with rings.
      sheetText.AppendPrintf(
          "button::-moz-focus-inner, input[type=\"reset\"]::-moz-focus-inner, "
          "input[type=\"button\"]::-moz-focus-inner, "
          "input[type=\"submit\"]::-moz-focus-inner { "
          "border: %dpx %s transparent !important; }\n",
          focusRingWidth, focusRingStyle == 0 ? "solid" : "dotted");

      sheetText.AppendLiteral(
          "button:focus::-moz-focus-inner, "
          "input[type=\"reset\"]:focus::-moz-focus-inner, "
          "input[type=\"button\"]:focus::-moz-focus-inner, "
          "input[type=\"submit\"]:focus::-moz-focus-inner { "
          "border-color: ButtonText !important; }\n");
    }

    sheetText.AppendPrintf(
        "%s { outline: %dpx %s !important; %s}\n",
        focusRingOnAnything ? ":focus" : "*|*:link:focus, *|*:visited:focus",
        focusRingWidth,
        focusRingStyle == 0 ?  // solid
            "solid -moz-mac-focusring"
                            : "dotted WindowText",
        focusRingStyle == 0 ?  // solid
            "-moz-outline-radius: 3px; outline-offset: 1px; "
                            : "");
  }

  if (aPrefs.mUseFocusColors) {
    nscolor focusText = aPrefs.mFocusTextColor;
    nscolor focusBG = aPrefs.mFocusBackgroundColor;
    sheetText.AppendPrintf(
        "*:focus, *:focus > font { color: #%02x%02x%02x !important; "
        "background-color: #%02x%02x%02x !important; }\n",
        NS_GET_R_G_B(focusText), NS_GET_R_G_B(focusBG));
  }

  NS_ASSERTION(sheetText.Length() <= kPreallocSize,
               "kPreallocSize should be big enough to build preference style "
               "sheet without reallocation");

  // NB: The pref sheet never has @import rules, thus no loader.
  sheet->ParseSheetSync(nullptr, sheetText,
                        /* aLoadData = */ nullptr,
                        /* aLineNumber = */ 0);

#undef NS_GET_R_G_B
}

/* static */ void nsLayoutStylesheetCache::SetSharedMemory(
    const base::SharedMemoryHandle& aHandle, uintptr_t aAddress) {
  MOZ_ASSERT(!XRE_IsParentProcess());
  MOZ_ASSERT(!gStyleCache,
             "Too late, nsLayoutStylesheetCache already created!");
  MOZ_ASSERT(!sSharedMemory, "Shouldn't call this more than once");

  RefPtr<Shm> shm = new Shm();
  if (shm->mShm.SetHandle(aHandle, /* read_only */ true) &&
      shm->mShm.Map(kSharedMemorySize, reinterpret_cast<void*>(aAddress))) {
    sSharedMemory = shm.forget();
  }
}

bool nsLayoutStylesheetCache::ShareToProcess(
    base::ProcessId aProcessId, base::SharedMemoryHandle* aHandle) {
  MOZ_ASSERT(XRE_IsParentProcess());
  return mSharedMemory &&
         mSharedMemory->mShm.ShareToProcess(aProcessId, aHandle);
}

mozilla::StaticRefPtr<nsLayoutStylesheetCache>
<<<<<<< HEAD
    nsLayoutStylesheetCache::gStyleCache;
||||||| merged common ancestors
nsLayoutStylesheetCache::gStyleCache;
=======
    nsLayoutStylesheetCache::gStyleCache;

mozilla::StaticRefPtr<mozilla::css::Loader> nsLayoutStylesheetCache::gCSSLoader;
>>>>>>> upstream-releases

<<<<<<< HEAD
mozilla::StaticRefPtr<mozilla::css::Loader> nsLayoutStylesheetCache::gCSSLoader;
||||||| merged common ancestors
mozilla::StaticRefPtr<mozilla::css::Loader>
nsLayoutStylesheetCache::gCSSLoader;
=======
mozilla::StaticRefPtr<nsIURI> nsLayoutStylesheetCache::gUserContentSheetURL;
>>>>>>> upstream-releases

<<<<<<< HEAD
mozilla::StaticRefPtr<nsIURI> nsLayoutStylesheetCache::gUserContentSheetURL;
||||||| merged common ancestors
mozilla::StaticRefPtr<nsIURI>
nsLayoutStylesheetCache::gUserContentSheetURL;
=======
StaticRefPtr<nsLayoutStylesheetCacheShm> nsLayoutStylesheetCache::sSharedMemory;
>>>>>>> upstream-releases
