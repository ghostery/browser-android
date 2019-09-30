/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_ContentBlockingLog_h
#define mozilla_dom_ContentBlockingLog_h

#include "mozilla/AntiTrackingCommon.h"
#include "mozilla/JSONWriter.h"
#include "mozilla/Maybe.h"
#include "mozilla/StaticPrefs.h"
#include "mozilla/Tuple.h"
#include "mozilla/UniquePtr.h"
#include "nsIWebProgressListener.h"
#include "nsReadableUtils.h"
#include "nsTArray.h"
#include "nsWindowSizes.h"

namespace mozilla {
namespace dom {

<<<<<<< HEAD
class ContentBlockingLog final {
||||||| merged common ancestors
class ContentBlockingLog final
{
=======
class ContentBlockingLog final {
  typedef AntiTrackingCommon::StorageAccessGrantedReason
      StorageAccessGrantedReason;

>>>>>>> upstream-releases
  struct LogEntry {
    uint32_t mType;
    uint32_t mRepeatCount;
    bool mBlocked;
    Maybe<AntiTrackingCommon::StorageAccessGrantedReason> mReason;
    nsTArray<nsCString> mTrackingFullHashes;
  };

  struct OriginDataEntry {
    OriginDataEntry() : mHasTrackingContentLoaded(false) {}

    bool mHasTrackingContentLoaded;
    Maybe<bool> mHasCookiesLoaded;
    nsTArray<LogEntry> mLogs;
  };

  struct OriginEntry {
    OriginEntry() { mData = MakeUnique<OriginDataEntry>(); }

    nsCString mOrigin;
    UniquePtr<OriginDataEntry> mData;
  };

  typedef nsTArray<OriginEntry> OriginDataTable;

  struct StringWriteFunc : public JSONWriteFunc {
    nsACString&
        mBuffer;  // The lifetime of the struct must be bound to the buffer
    explicit StringWriteFunc(nsACString& aBuffer) : mBuffer(aBuffer) {}

    void Write(const char* aStr) override { mBuffer.Append(aStr); }
  };

<<<<<<< HEAD
  // Each element is a tuple of (type, blocked, repeatCount). The type values
  // come from the blocking types defined in nsIWebProgressListener.
  typedef nsTArray<LogEntry> OriginLog;
  typedef Tuple<bool, Maybe<bool>, OriginLog> OriginData;
  typedef nsClassHashtable<nsStringHashKey, OriginData> OriginDataHashTable;

  struct StringWriteFunc : public JSONWriteFunc {
    nsAString&
        mBuffer;  // The lifetime of the struct must be bound to the buffer
    explicit StringWriteFunc(nsAString& aBuffer) : mBuffer(aBuffer) {}

    void Write(const char* aStr) override {
      mBuffer.Append(NS_ConvertUTF8toUTF16(aStr));
||||||| merged common ancestors
  // Each element is a tuple of (type, blocked, repeatCount). The type values
  // come from the blocking types defined in nsIWebProgressListener.
  typedef nsTArray<LogEntry> OriginLog;
  typedef nsClassHashtable<nsStringHashKey, OriginLog> OriginLogHashTable;

  struct StringWriteFunc : public JSONWriteFunc
  {
    nsAString& mBuffer; // The lifetime of the struct must be bound to the buffer
    explicit StringWriteFunc(nsAString& aBuffer)
      : mBuffer(aBuffer)
    {}

    void Write(const char* aStr) override
    {
      mBuffer.Append(NS_ConvertUTF8toUTF16(aStr));
=======
  struct Comparator {
   public:
    bool Equals(const OriginDataTable::elem_type& aLeft,
                const OriginDataTable::elem_type& aRight) const {
      return aLeft.mOrigin.Equals(aRight.mOrigin);
    }

    bool Equals(const OriginDataTable::elem_type& aLeft,
                const nsACString& aRight) const {
      return aLeft.mOrigin.Equals(aRight);
>>>>>>> upstream-releases
    }
  };

<<<<<<< HEAD
 public:
||||||| merged common ancestors
public:
=======
 public:
  static const nsLiteralCString kDummyOriginHash;

>>>>>>> upstream-releases
  ContentBlockingLog() = default;
  ~ContentBlockingLog() = default;

<<<<<<< HEAD
  void RecordLog(const nsAString& aOrigin, uint32_t aType, bool aBlocked) {
||||||| merged common ancestors
  void RecordLog(const nsAString& aOrigin, uint32_t aType, bool aBlocked)
  {
=======
  void RecordLog(
      const nsACString& aOrigin, uint32_t aType, bool aBlocked,
      const Maybe<AntiTrackingCommon::StorageAccessGrantedReason>& aReason,
      const nsTArray<nsCString>& aTrackingFullHashes) {
    DebugOnly<bool> isCookiesBlockedTracker =
        aType == nsIWebProgressListener::STATE_COOKIES_BLOCKED_TRACKER;
    MOZ_ASSERT_IF(aBlocked, aReason.isNothing());
    MOZ_ASSERT_IF(!isCookiesBlockedTracker, aReason.isNothing());
    MOZ_ASSERT_IF(isCookiesBlockedTracker && !aBlocked, aReason.isSome());

>>>>>>> upstream-releases
    if (aOrigin.IsVoid()) {
      return;
    }
<<<<<<< HEAD
    auto entry = mLog.LookupForAdd(aOrigin);
    if (entry) {
      auto& data = entry.Data();
      if (aType == nsIWebProgressListener::STATE_LOADED_TRACKING_CONTENT) {
        Get<0>(*data) = aBlocked;
        return;
      }
      if (aType == nsIWebProgressListener::STATE_COOKIES_LOADED) {
        if (Get<1>(*data).isSome()) {
          Get<1>(*data).ref() = aBlocked;
        } else {
          Get<1>(*data).emplace(aBlocked);
        }
        return;
      }
      auto& log = Get<2>(*data);
      if (!log.IsEmpty()) {
        auto& last = log.LastElement();
        if (last.mType == aType && last.mBlocked == aBlocked) {
||||||| merged common ancestors
    auto entry = mLog.LookupForAdd(aOrigin);
    if (entry) {
      auto& log = entry.Data();
      if (!log->IsEmpty()) {
        auto& last = log->LastElement();
        if (last.mType == aType &&
            last.mBlocked == aBlocked) {
=======
    auto index = mLog.IndexOf(aOrigin, 0, Comparator());
    if (index != OriginDataTable::NoIndex) {
      OriginEntry& entry = mLog[index];
      if (!entry.mData) {
        return;
      }

      if (aType == nsIWebProgressListener::STATE_LOADED_TRACKING_CONTENT) {
        entry.mData->mHasTrackingContentLoaded = aBlocked;
        return;
      }
      if (aType == nsIWebProgressListener::STATE_COOKIES_LOADED) {
        if (entry.mData->mHasCookiesLoaded.isSome()) {
          entry.mData->mHasCookiesLoaded.ref() = aBlocked;
        } else {
          entry.mData->mHasCookiesLoaded.emplace(aBlocked);
        }
        return;
      }
      if (!entry.mData->mLogs.IsEmpty()) {
        auto& last = entry.mData->mLogs.LastElement();
        if (last.mType == aType && last.mBlocked == aBlocked) {
>>>>>>> upstream-releases
          ++last.mRepeatCount;
          // Don't record recorded events.  This helps compress our log.
          // We don't care about if the the reason is the same, just keep the
          // first one.
          // Note: {aReason, aTrackingFullHashes} are not compared here and we
          // simply keep the first ones.
#ifdef DEBUG
          for (const auto& hash : aTrackingFullHashes) {
            MOZ_ASSERT(last.mTrackingFullHashes.Contains(hash));
          }
#endif
          return;
        }
      }
<<<<<<< HEAD
      if (log.Length() ==
          std::max(1u,
                   StaticPrefs::browser_contentblocking_originlog_length())) {
||||||| merged common ancestors
      if (log->Length() ==
            std::max(1u, StaticPrefs::browser_contentblocking_originlog_length())) {
=======
      if (entry.mData->mLogs.Length() ==
          std::max(1u,
                   StaticPrefs::browser_contentblocking_originlog_length())) {
>>>>>>> upstream-releases
        // Cap the size at the maximum length adjustable by the pref
<<<<<<< HEAD
        log.RemoveElementAt(0);
||||||| merged common ancestors
        log->RemoveElementAt(0);
=======
        entry.mData->mLogs.RemoveElementAt(0);
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      log.AppendElement(LogEntry{aType, 1u, aBlocked});
||||||| merged common ancestors
      log->AppendElement(LogEntry{aType, 1u, aBlocked});
=======
      entry.mData->mLogs.AppendElement(
          LogEntry{aType, 1u, aBlocked, aReason,
                   nsTArray<nsCString>(aTrackingFullHashes)});
      return;
    }

    // The entry has not been found.

    OriginEntry* entry = mLog.AppendElement();
    if (NS_WARN_IF(!entry || !entry->mData)) {
      return;
    }

    entry->mOrigin = aOrigin;

    if (aType == nsIWebProgressListener::STATE_LOADED_TRACKING_CONTENT) {
      entry->mData->mHasTrackingContentLoaded = aBlocked;
    } else if (aType == nsIWebProgressListener::STATE_COOKIES_LOADED) {
      MOZ_ASSERT(entry->mData->mHasCookiesLoaded.isNothing());
      entry->mData->mHasCookiesLoaded.emplace(aBlocked);
>>>>>>> upstream-releases
    } else {
<<<<<<< HEAD
      entry.OrInsert([=] {
        nsAutoPtr<OriginData> data(
            new OriginData(false, Maybe<bool>(), OriginLog()));
        if (aType == nsIWebProgressListener::STATE_LOADED_TRACKING_CONTENT) {
          Get<0>(*data) = aBlocked;
        } else if (aType == nsIWebProgressListener::STATE_COOKIES_LOADED) {
          if (Get<1>(*data).isSome()) {
            Get<1>(*data).ref() = aBlocked;
          } else {
            Get<1>(*data).emplace(aBlocked);
          }
        } else {
          Get<2>(*data).AppendElement(LogEntry{aType, 1u, aBlocked});
        }
        return data.forget();
      });
||||||| merged common ancestors
      entry.OrInsert([=] {
        auto log(MakeUnique<OriginLog>());
        log->AppendElement(LogEntry{aType, 1u, aBlocked});
        return log.release();
      });
=======
      entry->mData->mLogs.AppendElement(
          LogEntry{aType, 1u, aBlocked, aReason,
                   nsTArray<nsCString>(aTrackingFullHashes)});
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  nsAutoString Stringify() {
    nsAutoString buffer;
||||||| merged common ancestors
  nsAutoString Stringify()
  {
    nsAutoString buffer;
=======
  void ReportOrigins();
  void ReportLog(nsIPrincipal* aFirstPartyPrincipal);

  nsAutoCString Stringify() {
    nsAutoCString buffer;
>>>>>>> upstream-releases

    JSONWriter w(MakeUnique<StringWriteFunc>(buffer));
    w.Start();

<<<<<<< HEAD
    for (auto iter = mLog.Iter(); !iter.Done(); iter.Next()) {
      if (!iter.UserData()) {
        w.StartArrayProperty(NS_ConvertUTF16toUTF8(iter.Key()).get(),
                             w.SingleLineStyle);
        w.EndArray();
||||||| merged common ancestors
    for (auto iter = mLog.Iter(); !iter.Done(); iter.Next()) {
      if (!iter.UserData()) {
        w.StartArrayProperty(NS_ConvertUTF16toUTF8(iter.Key()).get(), w.SingleLineStyle);
        w.EndArray();
=======
    for (const OriginEntry& entry : mLog) {
      if (!entry.mData) {
>>>>>>> upstream-releases
        continue;
      }

<<<<<<< HEAD
      w.StartArrayProperty(NS_ConvertUTF16toUTF8(iter.Key()).get(),
                           w.SingleLineStyle);
      auto& data = *iter.UserData();
      if (Get<0>(data)) {
        w.StartArrayElement(w.SingleLineStyle);
        {
          w.IntElement(nsIWebProgressListener::STATE_LOADED_TRACKING_CONTENT);
          w.BoolElement(true);  // blocked
          w.IntElement(1);      // repeat count
        }
        w.EndArray();
      }
      if (Get<1>(data).isSome()) {
        w.StartArrayElement(w.SingleLineStyle);
        {
          w.IntElement(nsIWebProgressListener::STATE_COOKIES_LOADED);
          w.BoolElement(Get<1>(data).value());  // blocked
          w.IntElement(1);                      // repeat count
        }
        w.EndArray();
      }
      for (auto& item : Get<2>(data)) {
||||||| merged common ancestors
      w.StartArrayProperty(NS_ConvertUTF16toUTF8(iter.Key()).get(), w.SingleLineStyle);
      for (auto& item: *iter.UserData()) {
=======
      w.StartArrayProperty(entry.mOrigin.get(), w.SingleLineStyle);

      if (entry.mData->mHasTrackingContentLoaded) {
        w.StartArrayElement(w.SingleLineStyle);
        {
          w.IntElement(nsIWebProgressListener::STATE_LOADED_TRACKING_CONTENT);
          w.BoolElement(true);  // blocked
          w.IntElement(1);      // repeat count
        }
        w.EndArray();
      }
      if (entry.mData->mHasCookiesLoaded.isSome()) {
        w.StartArrayElement(w.SingleLineStyle);
        {
          w.IntElement(nsIWebProgressListener::STATE_COOKIES_LOADED);
          w.BoolElement(entry.mData->mHasCookiesLoaded.value());  // blocked
          w.IntElement(1);  // repeat count
        }
        w.EndArray();
      }
      for (const LogEntry& item : entry.mData->mLogs) {
>>>>>>> upstream-releases
        w.StartArrayElement(w.SingleLineStyle);
        {
          w.IntElement(item.mType);
          w.BoolElement(item.mBlocked);
          w.IntElement(item.mRepeatCount);
          if (item.mReason.isSome()) {
            w.IntElement(item.mReason.value());
          }
        }
        w.EndArray();
      }
      w.EndArray();
    }

    w.End();

    return buffer;
  }

<<<<<<< HEAD
  bool HasBlockedAnyOfType(uint32_t aType) {
    // Note: nothing inside this loop should return false, the goal for the
    // loop is to scan the log to see if we find a matching entry, and if so
    // we would return true, otherwise in the end of the function outside of
    // the loop we take the common `return false;` statement.
    for (auto iter = mLog.Iter(); !iter.Done(); iter.Next()) {
      if (!iter.UserData()) {
||||||| merged common ancestors
  bool HasBlockedAnyOfType(uint32_t aType)
  {
    for (auto iter = mLog.Iter(); !iter.Done(); iter.Next()) {
      if (!iter.UserData()) {
=======
  bool HasBlockedAnyOfType(uint32_t aType) const {
    // Note: nothing inside this loop should return false, the goal for the
    // loop is to scan the log to see if we find a matching entry, and if so
    // we would return true, otherwise in the end of the function outside of
    // the loop we take the common `return false;` statement.
    for (const OriginEntry& entry : mLog) {
      if (!entry.mData) {
>>>>>>> upstream-releases
        continue;
      }

<<<<<<< HEAD
      if (aType == nsIWebProgressListener::STATE_LOADED_TRACKING_CONTENT) {
        if (Get<0>(*iter.UserData())) {
          return true;
        }
      } else if (aType == nsIWebProgressListener::STATE_COOKIES_LOADED) {
        if (Get<1>(*iter.UserData()).isSome() &&
            Get<1>(*iter.UserData()).value()) {
||||||| merged common ancestors
      for (auto& item: *iter.UserData()) {
        if ((item.mType & aType) != 0) {
=======
      if (aType == nsIWebProgressListener::STATE_LOADED_TRACKING_CONTENT) {
        if (entry.mData->mHasTrackingContentLoaded) {
>>>>>>> upstream-releases
          return true;
        }
<<<<<<< HEAD
      } else {
        for (auto& item : Get<2>(*iter.UserData())) {
          if ((item.mType & aType) != 0) {
            return true;
          }
        }
||||||| merged common ancestors
=======
      } else if (aType == nsIWebProgressListener::STATE_COOKIES_LOADED) {
        if (entry.mData->mHasCookiesLoaded.isSome() &&
            entry.mData->mHasCookiesLoaded.value()) {
          return true;
        }
      } else {
        for (const auto& item : entry.mData->mLogs) {
          if (((item.mType & aType) != 0) && item.mBlocked) {
            return true;
          }
        }
>>>>>>> upstream-releases
      }
    }
    return false;
  }

  void AddSizeOfExcludingThis(nsWindowSizes& aSizes) const {
    aSizes.mDOMOtherSize +=
        mLog.ShallowSizeOfExcludingThis(aSizes.mState.mMallocSizeOf);

    // Now add the sizes of each origin log queue.
<<<<<<< HEAD
    // The const_cast is needed because the nsTHashtable::Iterator interface is
    // not const-safe.  :-(
    for (auto iter = const_cast<OriginDataHashTable&>(mLog).Iter();
         !iter.Done(); iter.Next()) {
      if (iter.UserData()) {
        aSizes.mDOMOtherSize +=
            aSizes.mState.mMallocSizeOf(iter.UserData()) +
            Get<2>(*iter.UserData())
                .ShallowSizeOfExcludingThis(aSizes.mState.mMallocSizeOf);
||||||| merged common ancestors
    // The const_cast is needed because the nsTHashtable::Iterator interface is
    // not const-safe.  :-(
    for (auto iter = const_cast<OriginLogHashTable&>(mLog).Iter();
         !iter.Done(); iter.Next()) {
      if (iter.UserData()) {
        aSizes.mDOMOtherSize +=
          iter.UserData()->ShallowSizeOfIncludingThis(aSizes.mState.mMallocSizeOf);
=======
    for (const OriginEntry& entry : mLog) {
      if (entry.mData) {
        aSizes.mDOMOtherSize += aSizes.mState.mMallocSizeOf(entry.mData.get()) +
                                entry.mData->mLogs.ShallowSizeOfExcludingThis(
                                    aSizes.mState.mMallocSizeOf);
>>>>>>> upstream-releases
      }
    }
  }

<<<<<<< HEAD
 private:
  OriginDataHashTable mLog;
||||||| merged common ancestors
private:
  OriginLogHashTable mLog;
=======
 private:
  OriginDataTable mLog;
>>>>>>> upstream-releases
};

}  // namespace dom
}  // namespace mozilla

#endif
