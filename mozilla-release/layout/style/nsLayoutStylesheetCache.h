/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsLayoutStylesheetCache_h__
#define nsLayoutStylesheetCache_h__

#include "nsIMemoryReporter.h"
#include "nsIObserver.h"
#include "base/shared_memory.h"
#include "mozilla/Attributes.h"
#include "mozilla/MemoryReporting.h"
#include "mozilla/PreferenceSheet.h"
#include "mozilla/NotNull.h"
#include "mozilla/StaticPtr.h"
#include "mozilla/css/Loader.h"

class nsIFile;
class nsIURI;

namespace mozilla {
class CSSStyleSheet;
}  // namespace mozilla

namespace mozilla {
namespace css {

// Enum defining how error should be handled.
<<<<<<< HEAD
enum FailureAction { eCrash = 0, eLogToConsole };
||||||| merged common ancestors
enum FailureAction {
  eCrash = 0,
  eLogToConsole
};
=======
enum FailureAction { eCrash = 0, eLogToConsole };

}  // namespace css
}  // namespace mozilla

// Reference counted wrapper around a base::SharedMemory that will store the
// User Agent style sheets.
struct nsLayoutStylesheetCacheShm final {
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(nsLayoutStylesheetCacheShm)
  base::SharedMemory mShm;

 private:
  ~nsLayoutStylesheetCacheShm() = default;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
}  // namespace css
}  // namespace mozilla
||||||| merged common ancestors
}
}
=======
class nsLayoutStylesheetCache final : public nsIObserver,
                                      public nsIMemoryReporter {
 public:
  using Shm = nsLayoutStylesheetCacheShm;
>>>>>>> upstream-releases

<<<<<<< HEAD
class nsLayoutStylesheetCache final : public nsIObserver,
                                      public nsIMemoryReporter {
||||||| merged common ancestors
class nsLayoutStylesheetCache final
 : public nsIObserver
 , public nsIMemoryReporter
{
=======
>>>>>>> upstream-releases
  NS_DECL_ISUPPORTS
  NS_DECL_NSIOBSERVER
  NS_DECL_NSIMEMORYREPORTER

  static nsLayoutStylesheetCache* Singleton();

<<<<<<< HEAD
#define STYLE_SHEET(identifier_, url_, lazy_) \
  mozilla::StyleSheet* identifier_##Sheet();
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET

  mozilla::StyleSheet* UserContentSheet();
  mozilla::StyleSheet* UserChromeSheet();
  mozilla::StyleSheet* ChromePreferenceSheet(nsPresContext* aPresContext);
  mozilla::StyleSheet* ContentPreferenceSheet(nsPresContext* aPresContext);
||||||| merged common ancestors
  mozilla::StyleSheet* ScrollbarsSheet();
  mozilla::StyleSheet* FormsSheet();
  mozilla::StyleSheet* UserContentSheet();
  mozilla::StyleSheet* UserChromeSheet();
  mozilla::StyleSheet* UASheet();
  mozilla::StyleSheet* HTMLSheet();
  mozilla::StyleSheet* MinimalXULSheet();
  mozilla::StyleSheet* XULSheet();
  mozilla::StyleSheet* XULComponentsSheet();
  mozilla::StyleSheet* QuirkSheet();
  mozilla::StyleSheet* SVGSheet();
  mozilla::StyleSheet* MathMLSheet();
  mozilla::StyleSheet* CounterStylesSheet();
  mozilla::StyleSheet* NoScriptSheet();
  mozilla::StyleSheet* NoFramesSheet();
  mozilla::StyleSheet* ChromePreferenceSheet(nsPresContext* aPresContext);
  mozilla::StyleSheet* ContentPreferenceSheet(nsPresContext* aPresContext);
  mozilla::StyleSheet* ContentEditableSheet();
  mozilla::StyleSheet* DesignModeSheet();
=======
#define STYLE_SHEET(identifier_, url_, shared_) \
  mozilla::NotNull<mozilla::StyleSheet*> identifier_##Sheet();
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET

  mozilla::StyleSheet* GetUserContentSheet();
  mozilla::StyleSheet* GetUserChromeSheet();
  mozilla::StyleSheet* ChromePreferenceSheet();
  mozilla::StyleSheet* ContentPreferenceSheet();
>>>>>>> upstream-releases

  static void InvalidatePreferenceSheets();

  static void Shutdown();

  static void SetUserContentCSSURL(nsIURI* aURI);

  size_t SizeOfIncludingThis(mozilla::MallocSizeOf aMallocSizeOf) const;

<<<<<<< HEAD
 private:
||||||| merged common ancestors
private:
=======
  // Set the shared memory segment to load the shared UA sheets from.
  // Called early on in a content process' life from
  // ContentChild::InitSharedUASheets, before the nsLayoutStylesheetCache
  // singleton has been created.
  static void SetSharedMemory(const base::SharedMemoryHandle& aHandle,
                              uintptr_t aAddress);

  // Obtain a shared memory handle for the shared UA sheets to pass into a
  // content process.  Called by ContentParent::InitInternal shortly after
  // a content process has been created.
  bool ShareToProcess(base::ProcessId aProcessId,
                      base::SharedMemoryHandle* aHandle);

  // Returns the address of the shared memory segment that holds the shared UA
  // sheets.
  uintptr_t GetSharedMemoryAddress() {
    return mSharedMemory ? uintptr_t(mSharedMemory->mShm.memory()) : 0;
  }

  // Size of the shared memory buffer we'll create to store the shared UA
  // sheets.  We choose a value that is big enough on both 64 bit and 32 bit.
  //
  // If this isn't big enough for the current contents of the shared UA
  // sheets, we'll crash under InitSharedSheetsInParent.
  static constexpr size_t kSharedMemorySize = 1024 * 400;

 private:
  // Shared memory header.
  struct Header {
    static constexpr uint32_t kMagic = 0x55415353;
    uint32_t mMagic;  // Must be set to kMagic.
    const ServoCssRules* mSheets[size_t(mozilla::UserAgentStyleSheetID::Count)];
    uint8_t mBuffer[1];
  };

>>>>>>> upstream-releases
  nsLayoutStylesheetCache();
  ~nsLayoutStylesheetCache();

  void InitFromProfile();
  void InitSharedSheetsInParent();
  void InitSharedSheetsInChild(already_AddRefed<Shm> aSharedMemory);
  void InitMemoryReporter();
  void LoadSheetURL(const char* aURL, RefPtr<mozilla::StyleSheet>* aSheet,
                    mozilla::css::SheetParsingMode aParsingMode,
                    mozilla::css::FailureAction aFailureAction);
  void LoadSheetFile(nsIFile* aFile, RefPtr<mozilla::StyleSheet>* aSheet,
                     mozilla::css::SheetParsingMode aParsingMode,
                     mozilla::css::FailureAction aFailureAction);
  void LoadSheet(nsIURI* aURI, RefPtr<mozilla::StyleSheet>* aSheet,
                 mozilla::css::SheetParsingMode aParsingMode,
                 mozilla::css::FailureAction aFailureAction);
  void LoadSheetFromSharedMemory(const char* aURL,
                                 RefPtr<mozilla::StyleSheet>* aSheet,
                                 mozilla::css::SheetParsingMode aParsingMode,
                                 Shm* aSharedMemory, Header* aHeader,
                                 mozilla::UserAgentStyleSheetID aSheetID);
  void BuildPreferenceSheet(RefPtr<mozilla::StyleSheet>* aSheet,
                            const mozilla::PreferenceSheet::Prefs&);

  static mozilla::StaticRefPtr<nsLayoutStylesheetCache> gStyleCache;
  static mozilla::StaticRefPtr<mozilla::css::Loader> gCSSLoader;
  static mozilla::StaticRefPtr<nsIURI> gUserContentSheetURL;

#define STYLE_SHEET(identifier_, url_, shared_) \
  RefPtr<mozilla::StyleSheet> m##identifier_##Sheet;
#include "mozilla/UserAgentStyleSheetList.h"
#undef STYLE_SHEET

  RefPtr<mozilla::StyleSheet> mChromePreferenceSheet;
  RefPtr<mozilla::StyleSheet> mContentPreferenceSheet;
  RefPtr<mozilla::StyleSheet> mUserChromeSheet;
  RefPtr<mozilla::StyleSheet> mUserContentSheet;
<<<<<<< HEAD
  RefPtr<mozilla::StyleSheet> mXULSheet;
||||||| merged common ancestors
  RefPtr<mozilla::StyleSheet> mXULSheet;
  RefPtr<mozilla::StyleSheet> mXULComponentsSheet;
=======

  // Shared memory segment storing shared style sheets.
  RefPtr<Shm> mSharedMemory;

  // How much of the shared memory buffer we ended up using.  Used for memory
  // reporting.
  size_t mUsedSharedMemory;

  // The shared memory to use once the nsLayoutStylesheetCache instance is
  // created.
  static mozilla::StaticRefPtr<Shm> sSharedMemory;
>>>>>>> upstream-releases
};

#endif
