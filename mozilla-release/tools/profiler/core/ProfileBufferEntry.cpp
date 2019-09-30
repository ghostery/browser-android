/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
#include <ostream>
#include "platform.h"
#include "mozilla/HashFunctions.h"
#include "mozilla/Sprintf.h"
#include "mozilla/Logging.h"
||||||| merged common ancestors
#include <ostream>
#include "platform.h"
#include "mozilla/HashFunctions.h"
#include "mozilla/Sprintf.h"
=======
#include "ProfileBufferEntry.h"
>>>>>>> upstream-releases

#include "platform.h"
#include "ProfileBuffer.h"

#include "js/TrackedOptimizationInfo.h"
#include "jsapi.h"
#include "jsfriendapi.h"
#include "mozilla/Logging.h"
#include "mozilla/Sprintf.h"
#include "mozilla/StackWalk.h"
#include "nsThreadUtils.h"
#include "nsXULAppAPI.h"

#include <ostream>

using namespace mozilla;

////////////////////////////////////////////////////////////////////////
// BEGIN ProfileBufferEntry

<<<<<<< HEAD
ProfileBufferEntry::ProfileBufferEntry() : mKind(Kind::INVALID) {
  u.mString = nullptr;
}
||||||| merged common ancestors
ProfileBufferEntry::ProfileBufferEntry()
  : mKind(Kind::INVALID)
{
  u.mString = nullptr;
}
=======
ProfileBufferEntry::ProfileBufferEntry()
    : mKind(Kind::INVALID), mStorage{0, 0, 0, 0, 0, 0, 0, 0} {}
>>>>>>> upstream-releases

// aString must be a static string.
<<<<<<< HEAD
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, const char* aString)
    : mKind(aKind) {
  u.mString = aString;
||||||| merged common ancestors
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, const char *aString)
  : mKind(aKind)
{
  u.mString = aString;
=======
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, const char* aString)
    : mKind(aKind) {
  memcpy(mStorage, &aString, sizeof(aString));
>>>>>>> upstream-releases
}

ProfileBufferEntry::ProfileBufferEntry(Kind aKind, char aChars[kNumChars])
<<<<<<< HEAD
    : mKind(aKind) {
  memcpy(u.mChars, aChars, kNumChars);
||||||| merged common ancestors
  : mKind(aKind)
{
  memcpy(u.mChars, aChars, kNumChars);
=======
    : mKind(aKind) {
  memcpy(mStorage, aChars, kNumChars);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, void* aPtr) : mKind(aKind) {
  u.mPtr = aPtr;
||||||| merged common ancestors
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, void* aPtr)
  : mKind(aKind)
{
  u.mPtr = aPtr;
=======
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, void* aPtr) : mKind(aKind) {
  memcpy(mStorage, &aPtr, sizeof(aPtr));
>>>>>>> upstream-releases
}

ProfileBufferEntry::ProfileBufferEntry(Kind aKind, ProfilerMarker* aMarker)
<<<<<<< HEAD
    : mKind(aKind) {
  u.mMarker = aMarker;
||||||| merged common ancestors
  : mKind(aKind)
{
  u.mMarker = aMarker;
=======
    : mKind(aKind) {
  memcpy(mStorage, &aMarker, sizeof(aMarker));
>>>>>>> upstream-releases
}

ProfileBufferEntry::ProfileBufferEntry(Kind aKind, double aDouble)
<<<<<<< HEAD
    : mKind(aKind) {
  u.mDouble = aDouble;
||||||| merged common ancestors
  : mKind(aKind)
{
  u.mDouble = aDouble;
=======
    : mKind(aKind) {
  memcpy(mStorage, &aDouble, sizeof(aDouble));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, int aInt) : mKind(aKind) {
  u.mInt = aInt;
||||||| merged common ancestors
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, int aInt)
  : mKind(aKind)
{
  u.mInt = aInt;
=======
ProfileBufferEntry::ProfileBufferEntry(Kind aKind, int aInt) : mKind(aKind) {
  memcpy(mStorage, &aInt, sizeof(aInt));
>>>>>>> upstream-releases
}

ProfileBufferEntry::ProfileBufferEntry(Kind aKind, int64_t aInt64)
<<<<<<< HEAD
    : mKind(aKind) {
  u.mInt64 = aInt64;
||||||| merged common ancestors
  : mKind(aKind)
{
  u.mInt64 = aInt64;
=======
    : mKind(aKind) {
  memcpy(mStorage, &aInt64, sizeof(aInt64));
>>>>>>> upstream-releases
}

ProfileBufferEntry::ProfileBufferEntry(Kind aKind, uint64_t aUint64)
<<<<<<< HEAD
    : mKind(aKind) {
  u.mUint64 = aUint64;
||||||| merged common ancestors
  : mKind(aKind)
{
  u.mUint64 = aUint64;
=======
    : mKind(aKind) {
  memcpy(mStorage, &aUint64, sizeof(aUint64));
>>>>>>> upstream-releases
}

const char* ProfileBufferEntry::GetString() const {
  const char* result;
  memcpy(&result, mStorage, sizeof(result));
  return result;
}

<<<<<<< HEAD
class JSONSchemaWriter {
  JSONWriter& mWriter;
  uint32_t mIndex;
||||||| merged common ancestors
class JSONSchemaWriter
{
  JSONWriter& mWriter;
  uint32_t mIndex;
=======
void* ProfileBufferEntry::GetPtr() const {
  void* result;
  memcpy(&result, mStorage, sizeof(result));
  return result;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  explicit JSONSchemaWriter(JSONWriter& aWriter) : mWriter(aWriter), mIndex(0) {
    aWriter.StartObjectProperty("schema",
                                SpliceableJSONWriter::SingleLineStyle);
  }
||||||| merged common ancestors
public:
  explicit JSONSchemaWriter(JSONWriter& aWriter)
   : mWriter(aWriter)
   , mIndex(0)
  {
    aWriter.StartObjectProperty("schema",
                                SpliceableJSONWriter::SingleLineStyle);
  }
=======
ProfilerMarker* ProfileBufferEntry::GetMarker() const {
  ProfilerMarker* result;
  memcpy(&result, mStorage, sizeof(result));
  return result;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  void WriteField(const char* aName) { mWriter.IntProperty(aName, mIndex++); }
||||||| merged common ancestors
  void WriteField(const char* aName) {
    mWriter.IntProperty(aName, mIndex++);
  }
=======
double ProfileBufferEntry::GetDouble() const {
  double result;
  memcpy(&result, mStorage, sizeof(result));
  return result;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  ~JSONSchemaWriter() { mWriter.EndObject(); }
};
||||||| merged common ancestors
  ~JSONSchemaWriter() {
    mWriter.EndObject();
  }
};
=======
int ProfileBufferEntry::GetInt() const {
  int result;
  memcpy(&result, mStorage, sizeof(result));
  return result;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
struct TypeInfo {
||||||| merged common ancestors
struct TypeInfo
{
=======
int64_t ProfileBufferEntry::GetInt64() const {
  int64_t result;
  memcpy(&result, mStorage, sizeof(result));
  return result;
}

uint64_t ProfileBufferEntry::GetUint64() const {
  uint64_t result;
  memcpy(&result, mStorage, sizeof(result));
  return result;
}

void ProfileBufferEntry::CopyCharsInto(char (&aOutArray)[kNumChars]) const {
  memcpy(aOutArray, mStorage, kNumChars);
}

// END ProfileBufferEntry
////////////////////////////////////////////////////////////////////////

struct TypeInfo {
>>>>>>> upstream-releases
  Maybe<nsCString> mKeyedBy;
  Maybe<nsCString> mName;
  Maybe<nsCString> mLocation;
  Maybe<unsigned> mLineNumber;
};

template <typename LambdaT>
class ForEachTrackedOptimizationTypeInfoLambdaOp
    : public JS::ForEachTrackedOptimizationTypeInfoOp {
 public:
  // aLambda needs to be a function with the following signature:
  // void lambda(JS::TrackedTypeSite site, const char* mirType,
  //             const nsTArray<TypeInfo>& typeset)
  // aLambda will be called once per entry.
  explicit ForEachTrackedOptimizationTypeInfoLambdaOp(LambdaT&& aLambda)
      : mLambda(aLambda) {}

  // This is called 0 or more times per entry, *before* operator() is called
  // for that entry.
  void readType(const char* keyedBy, const char* name, const char* location,
                const Maybe<unsigned>& lineno) override {
    TypeInfo info = {keyedBy ? Some(nsCString(keyedBy)) : Nothing(),
                     name ? Some(nsCString(name)) : Nothing(),
                     location ? Some(nsCString(location)) : Nothing(), lineno};
    mTypesetForUpcomingEntry.AppendElement(std::move(info));
  }

  void operator()(JS::TrackedTypeSite site, const char* mirType) override {
    nsTArray<TypeInfo> typeset(std::move(mTypesetForUpcomingEntry));
    mLambda(site, mirType, typeset);
  }

 private:
  nsTArray<TypeInfo> mTypesetForUpcomingEntry;
  LambdaT mLambda;
};

<<<<<<< HEAD
template <typename LambdaT>
ForEachTrackedOptimizationTypeInfoLambdaOp<LambdaT>
MakeForEachTrackedOptimizationTypeInfoLambdaOp(LambdaT&& aLambda) {
  return ForEachTrackedOptimizationTypeInfoLambdaOp<LambdaT>(
      std::move(aLambda));
||||||| merged common ancestors
template<typename LambdaT> ForEachTrackedOptimizationTypeInfoLambdaOp<LambdaT>
MakeForEachTrackedOptimizationTypeInfoLambdaOp(LambdaT&& aLambda)
{
  return ForEachTrackedOptimizationTypeInfoLambdaOp<LambdaT>(std::move(aLambda));
=======
template <typename LambdaT>
ForEachTrackedOptimizationTypeInfoLambdaOp<LambdaT>
MakeForEachTrackedOptimizationTypeInfoLambdaOp(LambdaT&& aLambda) {
  return ForEachTrackedOptimizationTypeInfoLambdaOp<LambdaT>(
      std::forward<LambdaT>(aLambda));
>>>>>>> upstream-releases
}

// As mentioned in ProfileBufferEntry.h, the JSON format contains many
// arrays whose elements are laid out according to various schemas to help
// de-duplication. This RAII class helps write these arrays by keeping track of
// the last non-null element written and adding the appropriate number of null
// elements when writing new non-null elements. It also automatically opens and
// closes an array element on the given JSON writer.
//
// You grant the AutoArraySchemaWriter exclusive access to the JSONWriter and
// the UniqueJSONStrings objects for the lifetime of AutoArraySchemaWriter. Do
// not access them independently while the AutoArraySchemaWriter is alive.
// If you need to add complex objects, call FreeFormElement(), which will give
// you temporary access to the writer.
//
// Example usage:
//
//     // Define the schema of elements in this type of array: [FOO, BAR, BAZ]
//     enum Schema : uint32_t {
//       FOO = 0,
//       BAR = 1,
//       BAZ = 2
//     };
//
//     AutoArraySchemaWriter writer(someJsonWriter, someUniqueStrings);
//     if (shouldWriteFoo) {
//       writer.IntElement(FOO, getFoo());
//     }
//     ... etc ...
//
//     The elements need to be added in-order.
class MOZ_RAII AutoArraySchemaWriter {
 public:
  AutoArraySchemaWriter(SpliceableJSONWriter& aWriter,
                        UniqueJSONStrings& aStrings)
      : mJSONWriter(aWriter), mStrings(&aStrings), mNextFreeIndex(0) {
    mJSONWriter.StartArrayElement(SpliceableJSONWriter::SingleLineStyle);
  }

  explicit AutoArraySchemaWriter(SpliceableJSONWriter& aWriter)
      : mJSONWriter(aWriter), mStrings(nullptr), mNextFreeIndex(0) {
    mJSONWriter.StartArrayElement(SpliceableJSONWriter::SingleLineStyle);
  }

  ~AutoArraySchemaWriter() { mJSONWriter.EndArray(); }

  template <typename T>
  void IntElement(uint32_t aIndex, T aValue) {
    static_assert(!IsSame<T, uint64_t>::value,
                  "Narrowing uint64 -> int64 conversion not allowed");
    FillUpTo(aIndex);
    mJSONWriter.IntElement(static_cast<int64_t>(aValue));
  }

  void DoubleElement(uint32_t aIndex, double aValue) {
    FillUpTo(aIndex);
    mJSONWriter.DoubleElement(aValue);
  }

  void BoolElement(uint32_t aIndex, bool aValue) {
    FillUpTo(aIndex);
    mJSONWriter.BoolElement(aValue);
  }

  void StringElement(uint32_t aIndex, const char* aValue) {
    MOZ_RELEASE_ASSERT(mStrings);
    FillUpTo(aIndex);
    mStrings->WriteElement(mJSONWriter, aValue);
  }

  // Write an element using a callback that takes a JSONWriter& and a
  // UniqueJSONStrings&.
  template <typename LambdaT>
  void FreeFormElement(uint32_t aIndex, LambdaT aCallback) {
    MOZ_RELEASE_ASSERT(mStrings);
    FillUpTo(aIndex);
    aCallback(mJSONWriter, *mStrings);
  }

 private:
  void FillUpTo(uint32_t aIndex) {
    MOZ_ASSERT(aIndex >= mNextFreeIndex);
    mJSONWriter.NullElements(aIndex - mNextFreeIndex);
    mNextFreeIndex = aIndex + 1;
  }

  SpliceableJSONWriter& mJSONWriter;
  UniqueJSONStrings* mStrings;
  uint32_t mNextFreeIndex;
};

template <typename LambdaT>
class ForEachTrackedOptimizationAttemptsLambdaOp
    : public JS::ForEachTrackedOptimizationAttemptOp {
 public:
  explicit ForEachTrackedOptimizationAttemptsLambdaOp(LambdaT&& aLambda)
      : mLambda(std::move(aLambda)) {}
  void operator()(JS::TrackedStrategy aStrategy,
                  JS::TrackedOutcome aOutcome) override {
    mLambda(aStrategy, aOutcome);
  }

 private:
  LambdaT mLambda;
};

template <typename LambdaT>
ForEachTrackedOptimizationAttemptsLambdaOp<LambdaT>
MakeForEachTrackedOptimizationAttemptsLambdaOp(LambdaT&& aLambda) {
  return ForEachTrackedOptimizationAttemptsLambdaOp<LambdaT>(
      std::move(aLambda));
}

UniqueJSONStrings::UniqueJSONStrings() { mStringTableWriter.StartBareList(); }

UniqueJSONStrings::UniqueJSONStrings(const UniqueJSONStrings& aOther) {
  mStringTableWriter.StartBareList();
<<<<<<< HEAD
  if (aOther.mStringToIndexMap.Count() > 0) {
    for (auto iter = aOther.mStringToIndexMap.ConstIter(); !iter.Done();
         iter.Next()) {
      mStringToIndexMap.Put(iter.Key(), iter.Data());
||||||| merged common ancestors
  if (aOther.mStringToIndexMap.Count() > 0) {
    for (auto iter = aOther.mStringToIndexMap.ConstIter();
         !iter.Done(); iter.Next()) {
      mStringToIndexMap.Put(iter.Key(), iter.Data());
=======
  uint32_t count = mStringHashToIndexMap.count();
  if (count != 0) {
    MOZ_RELEASE_ASSERT(mStringHashToIndexMap.reserve(count));
    for (auto iter = aOther.mStringHashToIndexMap.iter(); !iter.done();
         iter.next()) {
      mStringHashToIndexMap.putNewInfallible(iter.get().key(),
                                             iter.get().value());
>>>>>>> upstream-releases
    }
    UniquePtr<char[]> stringTableJSON =
        aOther.mStringTableWriter.WriteFunc()->CopyData();
    mStringTableWriter.Splice(stringTableJSON.get());
  }
}

<<<<<<< HEAD
uint32_t UniqueJSONStrings::GetOrAddIndex(const char* aStr) {
  nsDependentCString str(aStr);

  uint32_t count = mStringToIndexMap.Count();
  auto entry = mStringToIndexMap.LookupForAdd(str);
||||||| merged common ancestors
uint32_t
UniqueJSONStrings::GetOrAddIndex(const char* aStr)
{
  nsDependentCString str(aStr);

  uint32_t count = mStringToIndexMap.Count();
  auto entry = mStringToIndexMap.LookupForAdd(str);
=======
uint32_t UniqueJSONStrings::GetOrAddIndex(const char* aStr) {
  uint32_t count = mStringHashToIndexMap.count();
  HashNumber hash = HashString(aStr);
  auto entry = mStringHashToIndexMap.lookupForAdd(hash);
>>>>>>> upstream-releases
  if (entry) {
    MOZ_ASSERT(entry->value() < count);
    return entry->value();
  }

<<<<<<< HEAD
  entry.OrInsert([&] { return count; });
||||||| merged common ancestors
  entry.OrInsert([&]{ return count; });
=======
  MOZ_RELEASE_ASSERT(mStringHashToIndexMap.add(entry, hash, count));
>>>>>>> upstream-releases
  mStringTableWriter.StringElement(aStr);
  return count;
}

UniqueStacks::StackKey UniqueStacks::BeginStack(const FrameKey& aFrame) {
  return StackKey(GetOrAddFrameIndex(aFrame));
}

<<<<<<< HEAD
UniqueStacks::StackKey UniqueStacks::AppendFrame(const StackKey& aStack,
                                                 const FrameKey& aFrame) {
  return StackKey(aStack, GetOrAddStackIndex(aStack),
                  GetOrAddFrameIndex(aFrame));
}

uint32_t JITFrameInfoForBufferRange::JITFrameKey::Hash() const {
  uint32_t hash = 0;
  hash = AddToHash(hash, mCanonicalAddress);
  hash = AddToHash(hash, mDepth);
  return hash;
||||||| merged common ancestors
UniqueStacks::StackKey
UniqueStacks::AppendFrame(const StackKey& aStack, const FrameKey& aFrame)
{
  return StackKey(aStack, GetOrAddStackIndex(aStack), GetOrAddFrameIndex(aFrame));
}

uint32_t
JITFrameInfoForBufferRange::JITFrameKey::Hash() const
{
  uint32_t hash = 0;
  hash = AddToHash(hash, mCanonicalAddress);
  hash = AddToHash(hash, mDepth);
  return hash;
=======
UniqueStacks::StackKey UniqueStacks::AppendFrame(const StackKey& aStack,
                                                 const FrameKey& aFrame) {
  return StackKey(aStack, GetOrAddStackIndex(aStack),
                  GetOrAddFrameIndex(aFrame));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool JITFrameInfoForBufferRange::JITFrameKey::operator==(
    const JITFrameKey& aOther) const {
  return mCanonicalAddress == aOther.mCanonicalAddress &&
         mDepth == aOther.mDepth;
}
||||||| merged common ancestors
bool
JITFrameInfoForBufferRange::JITFrameKey::operator==(const JITFrameKey& aOther) const
{
  return mCanonicalAddress == aOther.mCanonicalAddress &&
         mDepth == aOther.mDepth;
}
=======
JITFrameInfoForBufferRange JITFrameInfoForBufferRange::Clone() const {
  JITFrameInfoForBufferRange::JITAddressToJITFramesMap jitAddressToJITFramesMap;
  MOZ_RELEASE_ASSERT(
      jitAddressToJITFramesMap.reserve(mJITAddressToJITFramesMap.count()));
  for (auto iter = mJITAddressToJITFramesMap.iter(); !iter.done();
       iter.next()) {
    const mozilla::Vector<JITFrameKey>& srcKeys = iter.get().value();
    mozilla::Vector<JITFrameKey> destKeys;
    MOZ_RELEASE_ASSERT(destKeys.appendAll(srcKeys));
    jitAddressToJITFramesMap.putNewInfallible(iter.get().key(),
                                              std::move(destKeys));
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
template <class KeyClass, class T>
void CopyClassHashtable(nsClassHashtable<KeyClass, T>& aDest,
                        const nsClassHashtable<KeyClass, T>& aSrc) {
  for (auto iter = aSrc.ConstIter(); !iter.Done(); iter.Next()) {
    const T& objRef = *iter.Data();
    aDest.LookupOrAdd(iter.Key(), objRef);
||||||| merged common ancestors
template<class KeyClass, class T> void
CopyClassHashtable(nsClassHashtable<KeyClass, T>& aDest,
                   const nsClassHashtable<KeyClass, T>& aSrc)
{
  for (auto iter = aSrc.ConstIter(); !iter.Done(); iter.Next()) {
    const T& objRef = *iter.Data();
    aDest.LookupOrAdd(iter.Key(), objRef);
=======
  JITFrameInfoForBufferRange::JITFrameToFrameJSONMap jitFrameToFrameJSONMap;
  MOZ_RELEASE_ASSERT(
      jitFrameToFrameJSONMap.reserve(mJITFrameToFrameJSONMap.count()));
  for (auto iter = mJITFrameToFrameJSONMap.iter(); !iter.done(); iter.next()) {
    jitFrameToFrameJSONMap.putNewInfallible(iter.get().key(),
                                            iter.get().value());
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
JITFrameInfoForBufferRange JITFrameInfoForBufferRange::Clone() const {
  nsClassHashtable<nsPtrHashKey<void>, nsTArray<JITFrameKey>>
      jitAddressToJITFramesMap;
  nsClassHashtable<nsGenericHashKey<JITFrameKey>, nsCString>
      jitFrameToFrameJSONMap;
  CopyClassHashtable(jitAddressToJITFramesMap, mJITAddressToJITFramesMap);
  CopyClassHashtable(jitFrameToFrameJSONMap, mJITFrameToFrameJSONMap);
  return JITFrameInfoForBufferRange{mRangeStart, mRangeEnd,
                                    std::move(jitAddressToJITFramesMap),
                                    std::move(jitFrameToFrameJSONMap)};
||||||| merged common ancestors
JITFrameInfoForBufferRange
JITFrameInfoForBufferRange::Clone() const
{
  nsClassHashtable<nsPtrHashKey<void>, nsTArray<JITFrameKey>> jitAddressToJITFramesMap;
  nsClassHashtable<nsGenericHashKey<JITFrameKey>, nsCString> jitFrameToFrameJSONMap;
  CopyClassHashtable(jitAddressToJITFramesMap, mJITAddressToJITFramesMap);
  CopyClassHashtable(jitFrameToFrameJSONMap, mJITFrameToFrameJSONMap);
  return JITFrameInfoForBufferRange{
    mRangeStart, mRangeEnd,
    std::move(jitAddressToJITFramesMap), std::move(jitFrameToFrameJSONMap) };
=======
  return JITFrameInfoForBufferRange{mRangeStart, mRangeEnd,
                                    std::move(jitAddressToJITFramesMap),
                                    std::move(jitFrameToFrameJSONMap)};
>>>>>>> upstream-releases
}

JITFrameInfo::JITFrameInfo(const JITFrameInfo& aOther)
    : mUniqueStrings(MakeUnique<UniqueJSONStrings>(*aOther.mUniqueStrings)) {
  for (const JITFrameInfoForBufferRange& range : aOther.mRanges) {
    MOZ_RELEASE_ASSERT(mRanges.append(range.Clone()));
  }
}

bool UniqueStacks::FrameKey::NormalFrameData::operator==(
    const NormalFrameData& aOther) const {
  return mLocation == aOther.mLocation &&
<<<<<<< HEAD
         mRelevantForJS == aOther.mRelevantForJS && mLine == aOther.mLine &&
         mColumn == aOther.mColumn && mCategory == aOther.mCategory;
||||||| merged common ancestors
         mLine == aOther.mLine &&
         mColumn == aOther.mColumn &&
         mCategory == aOther.mCategory;
=======
         mRelevantForJS == aOther.mRelevantForJS && mLine == aOther.mLine &&
         mColumn == aOther.mColumn && mCategoryPair == aOther.mCategoryPair;
>>>>>>> upstream-releases
}

bool UniqueStacks::FrameKey::JITFrameData::operator==(
    const JITFrameData& aOther) const {
  return mCanonicalAddress == aOther.mCanonicalAddress &&
<<<<<<< HEAD
         mDepth == aOther.mDepth && mRangeIndex == aOther.mRangeIndex;
}

uint32_t UniqueStacks::FrameKey::Hash() const {
  uint32_t hash = 0;
  if (mData.is<NormalFrameData>()) {
    const NormalFrameData& data = mData.as<NormalFrameData>();
    if (!data.mLocation.IsEmpty()) {
      hash = AddToHash(hash, HashString(data.mLocation.get()));
    }
    hash = AddToHash(hash, data.mRelevantForJS);
    if (data.mLine.isSome()) {
      hash = AddToHash(hash, *data.mLine);
    }
    if (data.mColumn.isSome()) {
      hash = AddToHash(hash, *data.mColumn);
    }
    if (data.mCategory.isSome()) {
      hash = AddToHash(hash, *data.mCategory);
    }
  } else {
    const JITFrameData& data = mData.as<JITFrameData>();
    hash = AddToHash(hash, data.mCanonicalAddress);
    hash = AddToHash(hash, data.mDepth);
    hash = AddToHash(hash, data.mRangeIndex);
  }
  return hash;
||||||| merged common ancestors
         mDepth == aOther.mDepth &&
         mRangeIndex == aOther.mRangeIndex;
}

uint32_t
UniqueStacks::FrameKey::Hash() const
{
  uint32_t hash = 0;
  if (mData.is<NormalFrameData>()) {
    const NormalFrameData& data = mData.as<NormalFrameData>();
    if (!data.mLocation.IsEmpty()) {
      hash = AddToHash(hash, HashString(data.mLocation.get()));
    }
    if (data.mLine.isSome()) {
      hash = AddToHash(hash, *data.mLine);
    }
    if (data.mColumn.isSome()) {
      hash = AddToHash(hash, *data.mColumn);
    }
    if (data.mCategory.isSome()) {
      hash = AddToHash(hash, *data.mCategory);
    }
  } else {
    const JITFrameData& data = mData.as<JITFrameData>();
    hash = AddToHash(hash, data.mCanonicalAddress);
    hash = AddToHash(hash, data.mDepth);
    hash = AddToHash(hash, data.mRangeIndex);
  }
  return hash;
=======
         mDepth == aOther.mDepth && mRangeIndex == aOther.mRangeIndex;
>>>>>>> upstream-releases
}

// Consume aJITFrameInfo by stealing its string table and its JIT frame info
// ranges. The JIT frame info contains JSON which refers to strings from the
// JIT frame info's string table, so our string table needs to have the same
// strings at the same indices.
UniqueStacks::UniqueStacks(JITFrameInfo&& aJITFrameInfo)
    : mUniqueStrings(std::move(aJITFrameInfo.mUniqueStrings)),
      mJITInfoRanges(std::move(aJITFrameInfo.mRanges)) {
  mFrameTableWriter.StartBareList();
  mStackTableWriter.StartBareList();
}

<<<<<<< HEAD
uint32_t UniqueStacks::GetOrAddStackIndex(const StackKey& aStack) {
  uint32_t count = mStackToIndexMap.Count();
  auto entry = mStackToIndexMap.LookupForAdd(aStack);
||||||| merged common ancestors
uint32_t UniqueStacks::GetOrAddStackIndex(const StackKey& aStack)
{
  uint32_t count = mStackToIndexMap.Count();
  auto entry = mStackToIndexMap.LookupForAdd(aStack);
=======
uint32_t UniqueStacks::GetOrAddStackIndex(const StackKey& aStack) {
  uint32_t count = mStackToIndexMap.count();
  auto entry = mStackToIndexMap.lookupForAdd(aStack);
>>>>>>> upstream-releases
  if (entry) {
    MOZ_ASSERT(entry->value() < count);
    return entry->value();
  }

<<<<<<< HEAD
  entry.OrInsert([&] { return count; });
||||||| merged common ancestors
  entry.OrInsert([&]{ return count; });
=======
  MOZ_RELEASE_ASSERT(mStackToIndexMap.add(entry, aStack, count));
>>>>>>> upstream-releases
  StreamStack(aStack);
  return count;
}

template <typename RangeT, typename PosT>
struct PositionInRangeComparator final {
  bool Equals(const RangeT& aRange, PosT aPos) const {
    return aRange.mRangeStart <= aPos && aPos < aRange.mRangeEnd;
  }

  bool LessThan(const RangeT& aRange, PosT aPos) const {
    return aRange.mRangeEnd <= aPos;
  }
};

Maybe<Vector<UniqueStacks::FrameKey>>
UniqueStacks::LookupFramesForJITAddressFromBufferPos(void* aJITAddress,
<<<<<<< HEAD
                                                     uint64_t aBufferPos) {
  size_t rangeIndex = mJITInfoRanges.BinaryIndexOf(
      aBufferPos,
      PositionInRangeComparator<JITFrameInfoForBufferRange, uint64_t>());
  MOZ_RELEASE_ASSERT(
      rangeIndex != mJITInfoRanges.NoIndex,
      "Buffer position of jit address needs to be in one of the ranges");
||||||| merged common ancestors
                                                     uint64_t aBufferPos)
{
  size_t rangeIndex = mJITInfoRanges.BinaryIndexOf(aBufferPos,
    PositionInRangeComparator<JITFrameInfoForBufferRange, uint64_t>());
  MOZ_RELEASE_ASSERT(rangeIndex != mJITInfoRanges.NoIndex,
                     "Buffer position of jit address needs to be in one of the ranges");
=======
                                                     uint64_t aBufferPos) {
  JITFrameInfoForBufferRange* rangeIter =
      std::lower_bound(mJITInfoRanges.begin(), mJITInfoRanges.end(), aBufferPos,
                       [](const JITFrameInfoForBufferRange& aRange,
                          uint64_t aPos) { return aRange.mRangeEnd < aPos; });
  MOZ_RELEASE_ASSERT(
      rangeIter != mJITInfoRanges.end() &&
          rangeIter->mRangeStart <= aBufferPos &&
          aBufferPos < rangeIter->mRangeEnd,
      "Buffer position of jit address needs to be in one of the ranges");
>>>>>>> upstream-releases

  using JITFrameKey = JITFrameInfoForBufferRange::JITFrameKey;

<<<<<<< HEAD
  const JITFrameInfoForBufferRange& jitFrameInfoRange =
      mJITInfoRanges[rangeIndex];
  const nsTArray<JITFrameKey>* jitFrameKeys =
      jitFrameInfoRange.mJITAddressToJITFramesMap.Get(aJITAddress);
||||||| merged common ancestors
  const JITFrameInfoForBufferRange& jitFrameInfoRange = mJITInfoRanges[rangeIndex];
  const nsTArray<JITFrameKey>* jitFrameKeys =
    jitFrameInfoRange.mJITAddressToJITFramesMap.Get(aJITAddress);
=======
  const JITFrameInfoForBufferRange& jitFrameInfoRange = *rangeIter;
  auto jitFrameKeys =
      jitFrameInfoRange.mJITAddressToJITFramesMap.lookup(aJITAddress);
>>>>>>> upstream-releases
  if (!jitFrameKeys) {
    return Nothing();
  }

  // Map the array of JITFrameKeys to an array of FrameKeys, and ensure that
  // each of the FrameKeys exists in mFrameToIndexMap.
<<<<<<< HEAD
  nsTArray<FrameKey> frameKeys;
  for (const JITFrameKey& jitFrameKey : *jitFrameKeys) {
    FrameKey frameKey(jitFrameKey.mCanonicalAddress, jitFrameKey.mDepth,
                      rangeIndex);
    uint32_t index = mFrameToIndexMap.Count();
    auto entry = mFrameToIndexMap.LookupForAdd(frameKey);
||||||| merged common ancestors
  nsTArray<FrameKey> frameKeys;
  for (const JITFrameKey& jitFrameKey : *jitFrameKeys) {
    FrameKey frameKey(jitFrameKey.mCanonicalAddress, jitFrameKey.mDepth, rangeIndex);
    uint32_t index = mFrameToIndexMap.Count();
    auto entry = mFrameToIndexMap.LookupForAdd(frameKey);
=======
  Vector<FrameKey> frameKeys;
  MOZ_RELEASE_ASSERT(frameKeys.initCapacity(jitFrameKeys->value().length()));
  for (const JITFrameKey& jitFrameKey : jitFrameKeys->value()) {
    FrameKey frameKey(jitFrameKey.mCanonicalAddress, jitFrameKey.mDepth,
                      rangeIter - mJITInfoRanges.begin());
    uint32_t index = mFrameToIndexMap.count();
    auto entry = mFrameToIndexMap.lookupForAdd(frameKey);
>>>>>>> upstream-releases
    if (!entry) {
      // We need to add this frame to our frame table. The JSON for this frame
      // already exists in jitFrameInfoRange, we just need to splice it into
      // the frame table and give it an index.
<<<<<<< HEAD
      const nsCString* frameJSON =
          jitFrameInfoRange.mJITFrameToFrameJSONMap.Get(jitFrameKey);
||||||| merged common ancestors
      const nsCString* frameJSON =
        jitFrameInfoRange.mJITFrameToFrameJSONMap.Get(jitFrameKey);
=======
      auto frameJSON =
          jitFrameInfoRange.mJITFrameToFrameJSONMap.lookup(jitFrameKey);
>>>>>>> upstream-releases
      MOZ_RELEASE_ASSERT(frameJSON, "Should have cached JSON for this frame");
      mFrameTableWriter.Splice(frameJSON->value().get());
      MOZ_RELEASE_ASSERT(mFrameToIndexMap.add(entry, frameKey, index));
    }
    MOZ_RELEASE_ASSERT(frameKeys.append(std::move(frameKey)));
  }
  return Some(std::move(frameKeys));
}

<<<<<<< HEAD
uint32_t UniqueStacks::GetOrAddFrameIndex(const FrameKey& aFrame) {
  uint32_t count = mFrameToIndexMap.Count();
  auto entry = mFrameToIndexMap.LookupForAdd(aFrame);
||||||| merged common ancestors
uint32_t
UniqueStacks::GetOrAddFrameIndex(const FrameKey& aFrame)
{
  uint32_t count = mFrameToIndexMap.Count();
  auto entry = mFrameToIndexMap.LookupForAdd(aFrame);
=======
uint32_t UniqueStacks::GetOrAddFrameIndex(const FrameKey& aFrame) {
  uint32_t count = mFrameToIndexMap.count();
  auto entry = mFrameToIndexMap.lookupForAdd(aFrame);
>>>>>>> upstream-releases
  if (entry) {
    MOZ_ASSERT(entry->value() < count);
    return entry->value();
  }

<<<<<<< HEAD
  entry.OrInsert([&] { return count; });
||||||| merged common ancestors
  entry.OrInsert([&]{ return count; });
=======
  MOZ_RELEASE_ASSERT(mFrameToIndexMap.add(entry, aFrame, count));
>>>>>>> upstream-releases
  StreamNonJITFrame(aFrame);
  return count;
}

void UniqueStacks::SpliceFrameTableElements(SpliceableJSONWriter& aWriter) {
  mFrameTableWriter.EndBareList();
  aWriter.TakeAndSplice(mFrameTableWriter.WriteFunc());
}

void UniqueStacks::SpliceStackTableElements(SpliceableJSONWriter& aWriter) {
  mStackTableWriter.EndBareList();
  aWriter.TakeAndSplice(mStackTableWriter.WriteFunc());
}

void UniqueStacks::StreamStack(const StackKey& aStack) {
  enum Schema : uint32_t { PREFIX = 0, FRAME = 1 };

  AutoArraySchemaWriter writer(mStackTableWriter, *mUniqueStrings);
  if (aStack.mPrefixStackIndex.isSome()) {
    writer.IntElement(PREFIX, *aStack.mPrefixStackIndex);
  }
  writer.IntElement(FRAME, aStack.mFrameIndex);
}

void UniqueStacks::StreamNonJITFrame(const FrameKey& aFrame) {
  using NormalFrameData = FrameKey::NormalFrameData;

  enum Schema : uint32_t {
    LOCATION = 0,
<<<<<<< HEAD
    RELEVANT_FOR_JS = 1,
    IMPLEMENTATION = 2,
    OPTIMIZATIONS = 3,
    LINE = 4,
    COLUMN = 5,
    CATEGORY = 6
||||||| merged common ancestors
    IMPLEMENTATION = 1,
    OPTIMIZATIONS = 2,
    LINE = 3,
    COLUMN = 4,
    CATEGORY = 5
=======
    RELEVANT_FOR_JS = 1,
    IMPLEMENTATION = 2,
    OPTIMIZATIONS = 3,
    LINE = 4,
    COLUMN = 5,
    CATEGORY = 6,
    SUBCATEGORY = 7
>>>>>>> upstream-releases
  };

  AutoArraySchemaWriter writer(mFrameTableWriter, *mUniqueStrings);

  const NormalFrameData& data = aFrame.mData.as<NormalFrameData>();
  writer.StringElement(LOCATION, data.mLocation.get());
  writer.BoolElement(RELEVANT_FOR_JS, data.mRelevantForJS);
  if (data.mLine.isSome()) {
    writer.IntElement(LINE, *data.mLine);
  }
  if (data.mColumn.isSome()) {
    writer.IntElement(COLUMN, *data.mColumn);
  }
  if (data.mCategoryPair.isSome()) {
    const JS::ProfilingCategoryPairInfo& info =
        JS::GetProfilingCategoryPairInfo(*data.mCategoryPair);
    writer.IntElement(CATEGORY, uint32_t(info.mCategory));
    writer.IntElement(SUBCATEGORY, info.mSubcategoryIndex);
  }
}

static void StreamJITFrameOptimizations(
    SpliceableJSONWriter& aWriter, UniqueJSONStrings& aUniqueStrings,
    JSContext* aContext, const JS::ProfiledFrameHandle& aJITFrame) {
  aWriter.StartObjectElement();
  {
    aWriter.StartArrayProperty("types");
    {
      auto op = MakeForEachTrackedOptimizationTypeInfoLambdaOp(
          [&](JS::TrackedTypeSite site, const char* mirType,
              const nsTArray<TypeInfo>& typeset) {
            aWriter.StartObjectElement();
            {
              aUniqueStrings.WriteProperty(aWriter, "site",
                                           JS::TrackedTypeSiteString(site));
              aUniqueStrings.WriteProperty(aWriter, "mirType", mirType);

              if (!typeset.IsEmpty()) {
                aWriter.StartArrayProperty("typeset");
                for (const TypeInfo& typeInfo : typeset) {
                  aWriter.StartObjectElement();
                  {
                    aUniqueStrings.WriteProperty(aWriter, "keyedBy",
                                                 typeInfo.mKeyedBy->get());
                    if (typeInfo.mName) {
                      aUniqueStrings.WriteProperty(aWriter, "name",
                                                   typeInfo.mName->get());
                    }
                    if (typeInfo.mLocation) {
                      aUniqueStrings.WriteProperty(aWriter, "location",
                                                   typeInfo.mLocation->get());
                    }
                    if (typeInfo.mLineNumber.isSome()) {
                      aWriter.IntProperty("line", *typeInfo.mLineNumber);
                    }
                  }
                  aWriter.EndObject();
                }
                aWriter.EndArray();
              }
            }
            aWriter.EndObject();
          });
      aJITFrame.forEachOptimizationTypeInfo(op);
    }
    aWriter.EndArray();

    JS::Rooted<JSScript*> script(aContext);
    jsbytecode* pc;
    aWriter.StartObjectProperty("attempts");
    {
      {
        JSONSchemaWriter schema(aWriter);
        schema.WriteField("strategy");
        schema.WriteField("outcome");
      }

      aWriter.StartArrayProperty("data");
      {
        auto op = MakeForEachTrackedOptimizationAttemptsLambdaOp(
            [&](JS::TrackedStrategy strategy, JS::TrackedOutcome outcome) {
              enum Schema : uint32_t { STRATEGY = 0, OUTCOME = 1 };

              AutoArraySchemaWriter writer(aWriter, aUniqueStrings);
              writer.StringElement(STRATEGY,
                                   JS::TrackedStrategyString(strategy));
              writer.StringElement(OUTCOME, JS::TrackedOutcomeString(outcome));
            });
        aJITFrame.forEachOptimizationAttempt(op, script.address(), &pc);
      }
      aWriter.EndArray();
    }
    aWriter.EndObject();

    if (JSAtom* name = js::GetPropertyNameFromPC(script, pc)) {
      char buf[512];
      JS_PutEscapedFlatString(buf, ArrayLength(buf), js::AtomToFlatString(name),
                              0);
      aUniqueStrings.WriteProperty(aWriter, "propertyName", buf);
    }

    unsigned line, column;
    line = JS_PCToLineNumber(script, pc, &column);
    aWriter.IntProperty("line", line);
    aWriter.IntProperty("column", column);
  }
  aWriter.EndObject();
}

static void StreamJITFrame(JSContext* aContext, SpliceableJSONWriter& aWriter,
                           UniqueJSONStrings& aUniqueStrings,
                           const JS::ProfiledFrameHandle& aJITFrame) {
  enum Schema : uint32_t {
    LOCATION = 0,
<<<<<<< HEAD
    RELEVANT_FOR_JS = 1,
    IMPLEMENTATION = 2,
    OPTIMIZATIONS = 3,
    LINE = 4,
    COLUMN = 5,
    CATEGORY = 6
||||||| merged common ancestors
    IMPLEMENTATION = 1,
    OPTIMIZATIONS = 2,
    LINE = 3,
    COLUMN = 4,
    CATEGORY = 5
=======
    RELEVANT_FOR_JS = 1,
    IMPLEMENTATION = 2,
    OPTIMIZATIONS = 3,
    LINE = 4,
    COLUMN = 5,
    CATEGORY = 6,
    SUBCATEGORY = 7
>>>>>>> upstream-releases
  };

  AutoArraySchemaWriter writer(aWriter, aUniqueStrings);

  writer.StringElement(LOCATION, aJITFrame.label());
  writer.BoolElement(RELEVANT_FOR_JS, false);

  JS::ProfilingFrameIterator::FrameKind frameKind = aJITFrame.frameKind();
  MOZ_ASSERT(frameKind == JS::ProfilingFrameIterator::Frame_Ion ||
             frameKind == JS::ProfilingFrameIterator::Frame_Baseline);
  writer.StringElement(
      IMPLEMENTATION,
      frameKind == JS::ProfilingFrameIterator::Frame_Ion ? "ion" : "baseline");

  if (aJITFrame.hasTrackedOptimizations()) {
    writer.FreeFormElement(
        OPTIMIZATIONS,
        [&](SpliceableJSONWriter& aWriter, UniqueJSONStrings& aUniqueStrings) {
          StreamJITFrameOptimizations(aWriter, aUniqueStrings, aContext,
                                      aJITFrame);
        });
  }

  const JS::ProfilingCategoryPairInfo& info =
      JS::GetProfilingCategoryPairInfo(JS::ProfilingCategoryPair::JS);
  writer.IntElement(CATEGORY, uint32_t(info.mCategory));
  writer.IntElement(SUBCATEGORY, info.mSubcategoryIndex);
}

struct CStringWriteFunc : public JSONWriteFunc {
  nsACString& mBuffer;  // The struct must not outlive this buffer
  explicit CStringWriteFunc(nsACString& aBuffer) : mBuffer(aBuffer) {}

  void Write(const char* aStr) override { mBuffer.Append(aStr); }
};

static nsCString JSONForJITFrame(JSContext* aContext,
                                 const JS::ProfiledFrameHandle& aJITFrame,
                                 UniqueJSONStrings& aUniqueStrings) {
  nsCString json;
  SpliceableJSONWriter writer(MakeUnique<CStringWriteFunc>(json));
  StreamJITFrame(aContext, writer, aUniqueStrings, aJITFrame);
  return json;
}

void JITFrameInfo::AddInfoForRange(
    uint64_t aRangeStart, uint64_t aRangeEnd, JSContext* aCx,
    const std::function<void(const std::function<void(void*)>&)>&
        aJITAddressProvider) {
  if (aRangeStart == aRangeEnd) {
    return;
  }

  MOZ_RELEASE_ASSERT(aRangeStart < aRangeEnd);

  if (!mRanges.empty()) {
    const JITFrameInfoForBufferRange& prevRange = mRanges.back();
    MOZ_RELEASE_ASSERT(prevRange.mRangeEnd <= aRangeStart,
                       "Ranges must be non-overlapping and added in-order.");
  }

  using JITFrameKey = JITFrameInfoForBufferRange::JITFrameKey;

<<<<<<< HEAD
  nsClassHashtable<nsPtrHashKey<void>, nsTArray<JITFrameKey>>
      jitAddressToJITFrameMap;
  nsClassHashtable<nsGenericHashKey<JITFrameKey>, nsCString>
      jitFrameToFrameJSONMap;
||||||| merged common ancestors
  nsClassHashtable<nsPtrHashKey<void>, nsTArray<JITFrameKey>> jitAddressToJITFrameMap;
  nsClassHashtable<nsGenericHashKey<JITFrameKey>, nsCString> jitFrameToFrameJSONMap;
=======
  JITFrameInfoForBufferRange::JITAddressToJITFramesMap jitAddressToJITFrameMap;
  JITFrameInfoForBufferRange::JITFrameToFrameJSONMap jitFrameToFrameJSONMap;
>>>>>>> upstream-releases

  aJITAddressProvider([&](void* aJITAddress) {
    // Make sure that we have cached data for aJITAddress.
<<<<<<< HEAD
    if (!jitAddressToJITFrameMap.Contains(aJITAddress)) {
      nsTArray<JITFrameKey>& jitFrameKeys =
          *jitAddressToJITFrameMap.LookupOrAdd(aJITAddress);
      for (JS::ProfiledFrameHandle handle :
           JS::GetProfiledFrames(aCx, aJITAddress)) {
        uint32_t depth = jitFrameKeys.Length();
        JITFrameKey jitFrameKey{handle.canonicalAddress(), depth};
        if (!jitFrameToFrameJSONMap.Contains(jitFrameKey)) {
          nsCString& json = *jitFrameToFrameJSONMap.LookupOrAdd(jitFrameKey);
          json = JSONForJITFrame(aCx, handle, *mUniqueStrings);
||||||| merged common ancestors
    if (!jitAddressToJITFrameMap.Contains(aJITAddress)) {
      nsTArray<JITFrameKey>& jitFrameKeys =
        *jitAddressToJITFrameMap.LookupOrAdd(aJITAddress);
      for (JS::ProfiledFrameHandle handle : JS::GetProfiledFrames(aCx, aJITAddress)) {
        uint32_t depth = jitFrameKeys.Length();
        JITFrameKey jitFrameKey{ handle.canonicalAddress(), depth };
        if (!jitFrameToFrameJSONMap.Contains(jitFrameKey)) {
          nsCString& json = *jitFrameToFrameJSONMap.LookupOrAdd(jitFrameKey);
          json = JSONForJITFrame(aCx, handle, *mUniqueStrings);
=======
    auto addressEntry = jitAddressToJITFrameMap.lookupForAdd(aJITAddress);
    if (!addressEntry) {
      Vector<JITFrameKey> jitFrameKeys;
      for (JS::ProfiledFrameHandle handle :
           JS::GetProfiledFrames(aCx, aJITAddress)) {
        uint32_t depth = jitFrameKeys.length();
        JITFrameKey jitFrameKey{handle.canonicalAddress(), depth};
        auto frameEntry = jitFrameToFrameJSONMap.lookupForAdd(jitFrameKey);
        if (!frameEntry) {
          MOZ_RELEASE_ASSERT(jitFrameToFrameJSONMap.add(
              frameEntry, jitFrameKey,
              JSONForJITFrame(aCx, handle, *mUniqueStrings)));
>>>>>>> upstream-releases
        }
        MOZ_RELEASE_ASSERT(jitFrameKeys.append(jitFrameKey));
      }
      MOZ_RELEASE_ASSERT(jitAddressToJITFrameMap.add(addressEntry, aJITAddress,
                                                     std::move(jitFrameKeys)));
    }
  });

<<<<<<< HEAD
  mRanges.AppendElement(JITFrameInfoForBufferRange{
      aRangeStart, aRangeEnd, std::move(jitAddressToJITFrameMap),
      std::move(jitFrameToFrameJSONMap)});
||||||| merged common ancestors
  mRanges.AppendElement(JITFrameInfoForBufferRange{
    aRangeStart, aRangeEnd,
    std::move(jitAddressToJITFrameMap), std::move(jitFrameToFrameJSONMap)
  });
=======
  MOZ_RELEASE_ASSERT(mRanges.append(JITFrameInfoForBufferRange{
      aRangeStart, aRangeEnd, std::move(jitAddressToJITFrameMap),
      std::move(jitFrameToFrameJSONMap)}));
>>>>>>> upstream-releases
}

struct ProfileSample {
  uint32_t mStack;
  double mTime;
  Maybe<double> mResponsiveness;
  Maybe<double> mRSS;
  Maybe<double> mUSS;
};

static void WriteSample(SpliceableJSONWriter& aWriter,
                        UniqueJSONStrings& aUniqueStrings,
                        const ProfileSample& aSample) {
  enum Schema : uint32_t {
    STACK = 0,
    TIME = 1,
    RESPONSIVENESS = 2,
    RSS = 3,
    USS = 4
  };

  AutoArraySchemaWriter writer(aWriter, aUniqueStrings);

  writer.IntElement(STACK, aSample.mStack);

  writer.DoubleElement(TIME, aSample.mTime);

  if (aSample.mResponsiveness.isSome()) {
    writer.DoubleElement(RESPONSIVENESS, *aSample.mResponsiveness);
  }

  if (aSample.mRSS.isSome()) {
    writer.DoubleElement(RSS, *aSample.mRSS);
  }

  if (aSample.mUSS.isSome()) {
    writer.DoubleElement(USS, *aSample.mUSS);
  }
}

class EntryGetter {
 public:
  explicit EntryGetter(const ProfileBuffer& aBuffer,
                       uint64_t aInitialReadPos = 0)
      : mBuffer(aBuffer), mReadPos(aBuffer.mRangeStart) {
    if (aInitialReadPos != 0) {
      MOZ_RELEASE_ASSERT(aInitialReadPos >= aBuffer.mRangeStart &&
                         aInitialReadPos <= aBuffer.mRangeEnd);
      mReadPos = aInitialReadPos;
    }
  }

  bool Has() const { return mReadPos != mBuffer.mRangeEnd; }
  const ProfileBufferEntry& Get() const { return mBuffer.GetEntry(mReadPos); }
  void Next() { mReadPos++; }
  uint64_t CurPos() { return mReadPos; }

 private:
  const ProfileBuffer& mBuffer;
  uint64_t mReadPos;
};

// The following grammar shows legal sequences of profile buffer entries.
// The sequences beginning with a ThreadId entry are known as "samples".
//
// (
//   ( /* Samples */
//     ThreadId
//     Time
//     ( NativeLeafAddr
<<<<<<< HEAD
//     | Label FrameFlags? DynamicStringFragment* LineNumber? Category?
||||||| merged common ancestors
//     | Label DynamicStringFragment* LineNumber? Category?
=======
//     | Label FrameFlags? DynamicStringFragment* LineNumber? CategoryPair?
>>>>>>> upstream-releases
//     | JitReturnAddr
//     )+
//     Marker*
//     Responsiveness?
//     ResidentMemory?
//     UnsharedMemory?
//   )
//   | ( ResidentMemory UnsharedMemory? Time)  /* Memory */
//   | ( /* Counters */
//       CounterId
//       Time
//       (
//         CounterKey
//         Count
//         Number?
//       )*
//     )
//   | CollectionStart
//   | CollectionEnd
//   | Pause
//   | Resume
//   | ( ProfilerOverheadTime /* Sampling start timestamp */
//       ProfilerOverheadDuration /* Lock acquisition */
//       ProfilerOverheadDuration /* Expired markers cleaning */
//       ProfilerOverheadDuration /* Counters */
//       ProfilerOverheadDuration /* Threads */
//     )
// )*
//
// The most complicated part is the stack entry sequence that begins with
// Label. Here are some examples.
//
// - ProfilingStack frames without a dynamic string:
//
//     Label("js::RunScript")
//     CategoryPair(JS::ProfilingCategoryPair::JS)
//
//     Label("XREMain::XRE_main")
//     LineNumber(4660)
//     CategoryPair(JS::ProfilingCategoryPair::OTHER)
//
//     Label("ElementRestyler::ComputeStyleChangeFor")
//     LineNumber(3003)
//     CategoryPair(JS::ProfilingCategoryPair::CSS)
//
// - ProfilingStack frames with a dynamic string:
//
//     Label("nsObserverService::NotifyObservers")
//     FrameFlags(uint64_t(ProfilingStackFrame::Flags::IS_LABEL_FRAME))
//     DynamicStringFragment("domwindo")
//     DynamicStringFragment("wopened")
//     LineNumber(291)
//     CategoryPair(JS::ProfilingCategoryPair::OTHER)
//
//     Label("")
//     FrameFlags(uint64_t(ProfilingStackFrame::Flags::IS_JS_FRAME))
//     DynamicStringFragment("closeWin")
//     DynamicStringFragment("dow (chr")
//     DynamicStringFragment("ome://gl")
//     DynamicStringFragment("obal/con")
//     DynamicStringFragment("tent/glo")
//     DynamicStringFragment("balOverl")
//     DynamicStringFragment("ay.js:5)")
//     DynamicStringFragment("")          # this string holds the closing '\0'
//     LineNumber(25)
//     CategoryPair(JS::ProfilingCategoryPair::JS)
//
//     Label("")
//     FrameFlags(uint64_t(ProfilingStackFrame::Flags::IS_JS_FRAME))
//     DynamicStringFragment("bound (s")
//     DynamicStringFragment("elf-host")
//     DynamicStringFragment("ed:914)")
//     LineNumber(945)
//     CategoryPair(JS::ProfilingCategoryPair::JS)
//
// - A profiling stack frame with a dynamic string, but with privacy enabled:
//
//     Label("nsObserverService::NotifyObservers")
//     FrameFlags(uint64_t(ProfilingStackFrame::Flags::IS_LABEL_FRAME))
//     DynamicStringFragment("(private")
//     DynamicStringFragment(")")
//     LineNumber(291)
//     CategoryPair(JS::ProfilingCategoryPair::OTHER)
//
// - A profiling stack frame with an overly long dynamic string:
//
//     Label("")
//     FrameFlags(uint64_t(ProfilingStackFrame::Flags::IS_LABEL_FRAME))
//     DynamicStringFragment("(too lon")
//     DynamicStringFragment("g)")
//     LineNumber(100)
//     CategoryPair(JS::ProfilingCategoryPair::NETWORK)
//
// - A wasm JIT frame:
//
//     Label("")
//     FrameFlags(uint64_t(0))
//     DynamicStringFragment("wasm-fun")
//     DynamicStringFragment("ction[87")
//     DynamicStringFragment("36] (blo")
//     DynamicStringFragment("b:http:/")
//     DynamicStringFragment("/webasse")
//     DynamicStringFragment("mbly.org")
//     DynamicStringFragment("/3dc5759")
//     DynamicStringFragment("4-ce58-4")
//     DynamicStringFragment("626-975b")
//     DynamicStringFragment("-08ad116")
//     DynamicStringFragment("30bc1:38")
//     DynamicStringFragment("29856)")
//
// - A JS frame in a synchronous sample:
//
//     Label("")
//     FrameFlags(uint64_t(ProfilingStackFrame::Flags::IS_LABEL_FRAME))
//     DynamicStringFragment("u (https")
//     DynamicStringFragment("://perf-")
//     DynamicStringFragment("html.io/")
//     DynamicStringFragment("ac0da204")
//     DynamicStringFragment("aaa44d75")
//     DynamicStringFragment("a800.bun")
//     DynamicStringFragment("dle.js:2")
//     DynamicStringFragment("5)")

// Because this is a format entirely internal to the Profiler, any parsing
// error indicates a bug in the ProfileBuffer writing or the parser itself,
// or possibly flaky hardware.
#define ERROR_AND_CONTINUE(msg)                            \
  {                                                        \
    fprintf(stderr, "ProfileBuffer parse error: %s", msg); \
    MOZ_ASSERT(false, msg);                                \
    continue;                                              \
  }

void ProfileBuffer::StreamSamplesToJSON(SpliceableJSONWriter& aWriter,
                                        int aThreadId, double aSinceTime,
                                        UniqueStacks& aUniqueStacks) const {
  UniquePtr<char[]> dynStrBuf = MakeUnique<char[]>(kMaxFrameKeyLength);

  EntryGetter e(*this);

  for (;;) {
    // This block skips entries until we find the start of the next sample.
    // This is useful in three situations.
    //
    // - The circular buffer overwrites old entries, so when we start parsing
    //   we might be in the middle of a sample, and we must skip forward to the
    //   start of the next sample.
    //
    // - We skip samples that don't have an appropriate ThreadId or Time.
    //
    // - We skip range Pause, Resume, CollectionStart, Marker, Counter
    //   and CollectionEnd entries between samples.
    while (e.Has()) {
      if (e.Get().IsThreadId()) {
        break;
      } else {
        e.Next();
      }
    }

    if (!e.Has()) {
      break;
    }

    if (e.Get().IsThreadId()) {
      int threadId = e.Get().GetInt();
      e.Next();

      // Ignore samples that are for the wrong thread.
      if (threadId != aThreadId) {
        continue;
      }
    } else {
      // Due to the skip_to_next_sample block above, if we have an entry here
      // it must be a ThreadId entry.
      MOZ_CRASH();
    }

    ProfileSample sample;

    if (e.Has() && e.Get().IsTime()) {
      sample.mTime = e.Get().GetDouble();
      e.Next();

      // Ignore samples that are too old.
      if (sample.mTime < aSinceTime) {
        continue;
      }
    } else {
      ERROR_AND_CONTINUE("expected a Time entry");
    }

    UniqueStacks::StackKey stack =
        aUniqueStacks.BeginStack(UniqueStacks::FrameKey("(root)"));

    int numFrames = 0;
    while (e.Has()) {
      if (e.Get().IsNativeLeafAddr()) {
        numFrames++;

        void* pc = e.Get().GetPtr();
        e.Next();

        static const uint32_t BUF_SIZE = 256;
        char buf[BUF_SIZE];

        // Bug 753041: We need a double cast here to tell GCC that we don't
        // want to sign extend 32-bit addresses starting with 0xFXXXXXX.
        unsigned long long pcULL = (unsigned long long)(uintptr_t)pc;
        SprintfLiteral(buf, "%#llx", pcULL);

        // If the "MOZ_PROFILER_SYMBOLICATE" env-var is set, we add a local
        // symbolication description to the PC address. This is off by default,
        // and mainly intended for local development.
        static const bool preSymbolicate = []() {
          const char* symbolicate = getenv("MOZ_PROFILER_SYMBOLICATE");
          return symbolicate && symbolicate[0] != '\0';
        }();
        if (preSymbolicate) {
          MozCodeAddressDetails details;
          if (MozDescribeCodeAddress(pc, &details)) {
            // Replace \0 terminator with space.
            const uint32_t pcLen = strlen(buf);
            buf[pcLen] = ' ';
            // Add description after space. Note: Using a frame number of 0,
            // as using `numFrames` wouldn't help here, and would prevent
            // combining same function calls that happen at different depths.
            // TODO: Remove unsightly "#00: " if too annoying. :-)
            MozFormatCodeAddressDetails(buf + pcLen + 1, BUF_SIZE - (pcLen + 1),
                                        0, pc, &details);
          }
        }

        stack = aUniqueStacks.AppendFrame(stack, UniqueStacks::FrameKey(buf));

      } else if (e.Get().IsLabel()) {
        numFrames++;

<<<<<<< HEAD
        const char* label = e.Get().u.mString;
||||||| merged common ancestors
        // Copy the label into strbuf.
        const char* label = e.Get().u.mString;
        strncpy(strbuf.get(), label, kMaxFrameKeyLength);
        size_t i = strlen(label);
=======
        const char* label = e.Get().GetString();
>>>>>>> upstream-releases
        e.Next();

<<<<<<< HEAD
        using FrameFlags = js::ProfilingStackFrame::Flags;
        uint32_t frameFlags = 0;
        if (e.Has() && e.Get().IsFrameFlags()) {
          frameFlags = uint32_t(e.Get().u.mUint64);
          e.Next();
        }

        bool relevantForJS = frameFlags & uint32_t(FrameFlags::RELEVANT_FOR_JS);

        // Copy potential dynamic string fragments into dynStrBuf, so that
        // dynStrBuf will then contain the entire dynamic string.
        size_t i = 0;
        dynStrBuf[0] = '\0';
||||||| merged common ancestors
        bool seenFirstDynamicStringFragment = false;
=======
        using FrameFlags = js::ProfilingStackFrame::Flags;
        uint32_t frameFlags = 0;
        if (e.Has() && e.Get().IsFrameFlags()) {
          frameFlags = uint32_t(e.Get().GetUint64());
          e.Next();
        }

        bool relevantForJS = frameFlags & uint32_t(FrameFlags::RELEVANT_FOR_JS);

        // Copy potential dynamic string fragments into dynStrBuf, so that
        // dynStrBuf will then contain the entire dynamic string.
        size_t i = 0;
        dynStrBuf[0] = '\0';
>>>>>>> upstream-releases
        while (e.Has()) {
          if (e.Get().IsDynamicStringFragment()) {
<<<<<<< HEAD
            for (size_t j = 0; j < ProfileBufferEntry::kNumChars; j++) {
              const char* chars = e.Get().u.mChars;
||||||| merged common ancestors
            // If this is the first dynamic string fragment and we have a
            // non-empty label, insert a ' ' after the label and before the
            // dynamic string.
            if (!seenFirstDynamicStringFragment) {
              if (i > 0 && i < kMaxFrameKeyLength) {
                strbuf[i] = ' ';
                i++;
              }
              seenFirstDynamicStringFragment = true;
            }

            for (size_t j = 0; j < ProfileBufferEntry::kNumChars; j++) {
              const char* chars = e.Get().u.mChars;
=======
            char chars[ProfileBufferEntry::kNumChars];
            e.Get().CopyCharsInto(chars);
            for (char c : chars) {
>>>>>>> upstream-releases
              if (i < kMaxFrameKeyLength) {
<<<<<<< HEAD
                dynStrBuf[i] = chars[j];
||||||| merged common ancestors
                strbuf[i] = chars[j];
=======
                dynStrBuf[i] = c;
>>>>>>> upstream-releases
                i++;
              }
            }
            e.Next();
          } else {
            break;
          }
        }
        dynStrBuf[kMaxFrameKeyLength - 1] = '\0';
        bool hasDynamicString = (i != 0);

        nsCString frameLabel;
        if (label[0] != '\0' && hasDynamicString) {
          if (frameFlags & uint32_t(FrameFlags::STRING_TEMPLATE_METHOD)) {
            frameLabel.AppendPrintf("%s.%s", label, dynStrBuf.get());
          } else if (frameFlags &
                     uint32_t(FrameFlags::STRING_TEMPLATE_GETTER)) {
            frameLabel.AppendPrintf("get %s.%s", label, dynStrBuf.get());
          } else if (frameFlags &
                     uint32_t(FrameFlags::STRING_TEMPLATE_SETTER)) {
            frameLabel.AppendPrintf("set %s.%s", label, dynStrBuf.get());
          } else {
            frameLabel.AppendPrintf("%s %s", label, dynStrBuf.get());
          }
        } else if (hasDynamicString) {
          frameLabel.Append(dynStrBuf.get());
        } else {
          frameLabel.Append(label);
        }

        Maybe<unsigned> line;
        if (e.Has() && e.Get().IsLineNumber()) {
          line = Some(unsigned(e.Get().GetInt()));
          e.Next();
        }

        Maybe<unsigned> column;
        if (e.Has() && e.Get().IsColumnNumber()) {
          column = Some(unsigned(e.Get().GetInt()));
          e.Next();
        }

        Maybe<JS::ProfilingCategoryPair> categoryPair;
        if (e.Has() && e.Get().IsCategoryPair()) {
          categoryPair =
              Some(JS::ProfilingCategoryPair(uint32_t(e.Get().GetInt())));
          e.Next();
        }

        stack = aUniqueStacks.AppendFrame(
<<<<<<< HEAD
            stack, UniqueStacks::FrameKey(std::move(frameLabel), relevantForJS,
                                          line, column, category));
||||||| merged common ancestors
          stack, UniqueStacks::FrameKey(strbuf.get(), line, column, category));
=======
            stack, UniqueStacks::FrameKey(std::move(frameLabel), relevantForJS,
                                          line, column, categoryPair));
>>>>>>> upstream-releases

      } else if (e.Get().IsJitReturnAddr()) {
        numFrames++;

        // A JIT frame may expand to multiple frames due to inlining.
<<<<<<< HEAD
        void* pc = e.Get().u.mPtr;
        const Maybe<nsTArray<UniqueStacks::FrameKey>>& frameKeys =
            aUniqueStacks.LookupFramesForJITAddressFromBufferPos(pc,
                                                                 e.CurPos());
||||||| merged common ancestors
        void* pc = e.Get().u.mPtr;
        const Maybe<nsTArray<UniqueStacks::FrameKey>>& frameKeys =
          aUniqueStacks.LookupFramesForJITAddressFromBufferPos(pc, e.CurPos());
=======
        void* pc = e.Get().GetPtr();
        const Maybe<Vector<UniqueStacks::FrameKey>>& frameKeys =
            aUniqueStacks.LookupFramesForJITAddressFromBufferPos(pc,
                                                                 e.CurPos());
>>>>>>> upstream-releases
        MOZ_RELEASE_ASSERT(frameKeys,
                           "Attempting to stream samples for a buffer range "
                           "for which we don't have JITFrameInfo?");
        for (const UniqueStacks::FrameKey& frameKey : *frameKeys) {
          stack = aUniqueStacks.AppendFrame(stack, frameKey);
        }

        e.Next();

      } else {
        break;
      }
    }

    if (numFrames == 0) {
      // It is possible to have empty stacks if native stackwalking is
      // disabled. Skip samples with empty stacks. (See Bug 1497985).
      // Thus, don't use ERROR_AND_CONTINUE, but just continue.
      continue;
    }

    sample.mStack = aUniqueStacks.GetOrAddStackIndex(stack);

    // Skip over the markers. We process them in StreamMarkersToJSON().
    while (e.Has()) {
      if (e.Get().IsMarker()) {
        e.Next();
      } else {
        break;
      }
    }

    if (e.Has() && e.Get().IsResponsiveness()) {
      sample.mResponsiveness = Some(e.Get().GetDouble());
      e.Next();
    }

    if (e.Has() && e.Get().IsResidentMemory()) {
      sample.mRSS = Some(e.Get().GetDouble());
      e.Next();
    }

    if (e.Has() && e.Get().IsUnsharedMemory()) {
      sample.mUSS = Some(e.Get().GetDouble());
      e.Next();
    }

    WriteSample(aWriter, *aUniqueStacks.mUniqueStrings, sample);
  }
}

void ProfileBuffer::AddJITInfoForRange(uint64_t aRangeStart, int aThreadId,
                                       JSContext* aContext,
                                       JITFrameInfo& aJITFrameInfo) const {
  // We can only process JitReturnAddr entries if we have a JSContext.
  MOZ_RELEASE_ASSERT(aContext);

  aRangeStart = std::max(aRangeStart, mRangeStart);
  aJITFrameInfo.AddInfoForRange(
      aRangeStart, mRangeEnd, aContext,
      [&](const std::function<void(void*)>& aJITAddressConsumer) {
        // Find all JitReturnAddr entries in the given range for the given
        // thread, and call aJITAddressConsumer with those addresses.

        EntryGetter e(*this, aRangeStart);
        while (true) {
          // Advance to the next ThreadId entry.
          while (e.Has() && !e.Get().IsThreadId()) {
            e.Next();
          }
          if (!e.Has()) {
            break;
          }

<<<<<<< HEAD
          MOZ_ASSERT(e.Get().IsThreadId());
          int threadId = e.Get().u.mInt;
          e.Next();
||||||| merged common ancestors
        MOZ_ASSERT(e.Get().IsThreadId());
        int threadId = e.Get().u.mInt;
        e.Next();
=======
          MOZ_ASSERT(e.Get().IsThreadId());
          int threadId = e.Get().GetInt();
          e.Next();
>>>>>>> upstream-releases

          // Ignore samples that are for a different thread.
          if (threadId != aThreadId) {
            continue;
          }

<<<<<<< HEAD
          while (e.Has() && !e.Get().IsThreadId()) {
            if (e.Get().IsJitReturnAddr()) {
              aJITAddressConsumer(e.Get().u.mPtr);
            }
            e.Next();
||||||| merged common ancestors
        while (e.Has() && !e.Get().IsThreadId()) {
          if (e.Get().IsJitReturnAddr()) {
            aJITAddressConsumer(e.Get().u.mPtr);
=======
          while (e.Has() && !e.Get().IsThreadId()) {
            if (e.Get().IsJitReturnAddr()) {
              aJITAddressConsumer(e.Get().GetPtr());
            }
            e.Next();
>>>>>>> upstream-releases
          }
        }
      });
}

void ProfileBuffer::StreamMarkersToJSON(SpliceableJSONWriter& aWriter,
                                        int aThreadId,
                                        const TimeStamp& aProcessStartTime,
                                        double aSinceTime,
                                        UniqueStacks& aUniqueStacks) const {
  EntryGetter e(*this);

  // Stream all markers whose threadId matches aThreadId. We skip other entries,
  // because we process them in StreamSamplesToJSON().
  //
  // NOTE: The ThreadId of a marker is determined by its GetThreadId() method,
  // rather than ThreadId buffer entries, as markers can be added outside of
  // samples.
  while (e.Has()) {
    if (e.Get().IsMarker()) {
      const ProfilerMarker* marker = e.Get().GetMarker();
      if (marker->GetTime() >= aSinceTime &&
          marker->GetThreadId() == aThreadId) {
        marker->StreamJSON(aWriter, aProcessStartTime, aUniqueStacks);
      }
    }
    e.Next();
  }
}

<<<<<<< HEAD
struct CounterKeyedSample {
||||||| merged common ancestors

struct CounterKeyedSample
{
=======
void ProfileBuffer::StreamProfilerOverheadToJSON(
    SpliceableJSONWriter& aWriter, const TimeStamp& aProcessStartTime,
    double aSinceTime) const {
  enum Schema : uint32_t {
    TIME = 0,
    LOCKING = 1,
    MARKER_CLEANING = 2,
    COUNTERS = 3,
    THREADS = 4
  };

  EntryGetter e(*this);

  aWriter.StartObjectProperty("profilerOverhead_UNSTABLE");
  // Stream all sampling overhead data. We skip other entries, because we
  // process them in StreamSamplesToJSON()/etc.
  {
    JSONSchemaWriter schema(aWriter);
    schema.WriteField("time");
    schema.WriteField("locking");
    schema.WriteField("expiredMarkerCleaning");
    schema.WriteField("counters");
    schema.WriteField("threads");
  }

  aWriter.StartArrayProperty("data");
  double firstTime = 0.0;
  double lastTime = 0.0;
  struct Stats {
    unsigned n = 0;
    double sum = 0;
    double min = std::numeric_limits<double>::max();
    double max = 0;
    void Count(double v) {
      ++n;
      sum += v;
      if (v < min) {
        min = v;
      }
      if (v > max) {
        max = v;
      }
    }
  };
  Stats intervals, overheads, lockings, cleanings, counters, threads;
  while (e.Has()) {
    // valid sequence: ProfilerOverheadTime, ProfilerOverheadDuration * 4
    if (e.Get().IsProfilerOverheadTime()) {
      double time = e.Get().GetDouble();
      if (time >= aSinceTime) {
        e.Next();
        if (!e.Has() || !e.Get().IsProfilerOverheadDuration()) {
          ERROR_AND_CONTINUE(
              "expected a ProfilerOverheadDuration entry after "
              "ProfilerOverheadTime");
        }
        double locking = e.Get().GetDouble();
        e.Next();
        if (!e.Has() || !e.Get().IsProfilerOverheadDuration()) {
          ERROR_AND_CONTINUE(
              "expected a ProfilerOverheadDuration entry after "
              "ProfilerOverheadTime,ProfilerOverheadDuration");
        }
        double cleaning = e.Get().GetDouble();
        e.Next();
        if (!e.Has() || !e.Get().IsProfilerOverheadDuration()) {
          ERROR_AND_CONTINUE(
              "expected a ProfilerOverheadDuration entry after "
              "ProfilerOverheadTime,ProfilerOverheadDuration*2");
        }
        double counter = e.Get().GetDouble();
        e.Next();
        if (!e.Has() || !e.Get().IsProfilerOverheadDuration()) {
          ERROR_AND_CONTINUE(
              "expected a ProfilerOverheadDuration entry after "
              "ProfilerOverheadTime,ProfilerOverheadDuration*3");
        }
        double thread = e.Get().GetDouble();

        if (firstTime == 0.0) {
          firstTime = time;
        } else {
          // Note that we'll have 1 fewer interval than other numbers (because
          // we need both ends of an interval to know its duration). The final
          // difference should be insignificant over the expected many thousands
          // of iterations.
          intervals.Count(time - lastTime);
        }
        lastTime = time;
        overheads.Count(locking + cleaning + counter + thread);
        lockings.Count(locking);
        cleanings.Count(cleaning);
        counters.Count(counter);
        threads.Count(thread);

        AutoArraySchemaWriter writer(aWriter);
        writer.DoubleElement(TIME, time);
        writer.DoubleElement(LOCKING, locking);
        writer.DoubleElement(MARKER_CLEANING, cleaning);
        writer.DoubleElement(COUNTERS, counter);
        writer.DoubleElement(THREADS, thread);
      }
    }
    e.Next();
  }
  aWriter.EndArray();  // data

  // Only output statistics if there is at least one full interval (and
  // therefore at least two samplings.)
  if (intervals.n > 0) {
    aWriter.StartObjectProperty("statistics");
    aWriter.DoubleProperty("profiledDuration", lastTime - firstTime);
    aWriter.IntProperty("samplingCount", overheads.n);
    aWriter.DoubleProperty("overheadDurations", overheads.sum);
    aWriter.DoubleProperty("overheadPercentage",
                           overheads.sum / (lastTime - firstTime));
#define PROFILER_STATS(name, var)                           \
  aWriter.DoubleProperty("mean" name, (var).sum / (var).n); \
  aWriter.DoubleProperty("min" name, (var).min);            \
  aWriter.DoubleProperty("max" name, (var).max);
    PROFILER_STATS("Interval", intervals);
    PROFILER_STATS("Overhead", overheads);
    PROFILER_STATS("Lockings", lockings);
    PROFILER_STATS("Cleaning", cleanings);
    PROFILER_STATS("Counter", counters);
    PROFILER_STATS("Thread", threads);
#undef PROFILER_STATS
    aWriter.EndObject();  // statistics
  }
  aWriter.EndObject();  // profilerOverhead
}

struct CounterKeyedSample {
>>>>>>> upstream-releases
  double mTime;
  uint64_t mNumber;
  int64_t mCount;
};

using CounterKeyedSamples = Vector<CounterKeyedSample>;

static LazyLogModule sFuzzyfoxLog("Fuzzyfox");

<<<<<<< HEAD
static LazyLogModule sFuzzyfoxLog("Fuzzyfox");
||||||| merged common ancestors
=======
using CounterMap = HashMap<uint64_t, CounterKeyedSamples>;
>>>>>>> upstream-releases

// HashMap lookup, if not found, a default value is inserted.
// Returns reference to (existing or new) value inside the HashMap.
template <typename HashM, typename Key>
static auto& LookupOrAdd(HashM& aMap, Key&& aKey) {
  auto addPtr = aMap.lookupForAdd(aKey);
  if (!addPtr) {
    MOZ_RELEASE_ASSERT(aMap.add(addPtr, std::forward<Key>(aKey),
                                typename HashM::Entry::ValueType{}));
    MOZ_ASSERT(!!addPtr);
  }
  return addPtr->value();
}

void ProfileBuffer::StreamCountersToJSON(SpliceableJSONWriter& aWriter,
                                         const TimeStamp& aProcessStartTime,
                                         double aSinceTime) const {
  // Because this is a format entirely internal to the Profiler, any parsing
  // error indicates a bug in the ProfileBuffer writing or the parser itself,
  // or possibly flaky hardware.

  EntryGetter e(*this);
  enum Schema : uint32_t { TIME = 0, NUMBER = 1, COUNT = 2 };

  // Stream all counters. We skip other entries, because we process them in
  // StreamSamplesToJSON()/etc.
  //
  // Valid sequence in the buffer:
  // CounterID
  // Time
  // ( CounterKey Count Number? )*
  //
  // And the JSON (example):
  // "counters": {
  //  "name": "malloc",
  //  "category": "Memory",
  //  "description": "Amount of allocated memory",
  //  "sample_groups": {
  //   "id": 0,
  //   "samples": {
  //    "schema": {"time": 0, "number": 1, "count": 2},
  //    "data": [
  //     [
  //      16117.033968000002,
  //      2446216,
  //      6801320
  //     ],
  //     [
  //      16118.037638,
  //      2446216,
  //      6801320
  //     ],
  //    ],
  //   }
  //  }
  // },

  // Build the map of counters and populate it
  HashMap<void*, CounterMap> counters;

  while (e.Has()) {
    // skip all non-Counters, including if we start in the middle of a counter
    if (e.Get().IsCounterId()) {
      void* id = e.Get().GetPtr();
      CounterMap& counter = LookupOrAdd(counters, id);
      e.Next();
      if (!e.Has() || !e.Get().IsTime()) {
        ERROR_AND_CONTINUE("expected a Time entry");
      }
      double time = e.Get().GetDouble();
      if (time >= aSinceTime) {
        e.Next();
        while (e.Has() && e.Get().IsCounterKey()) {
          uint64_t key = e.Get().GetUint64();
          CounterKeyedSamples& data = LookupOrAdd(counter, key);
          e.Next();
          if (!e.Has() || !e.Get().IsCount()) {
            ERROR_AND_CONTINUE("expected a Count entry");
          }
          int64_t count = e.Get().GetUint64();
          e.Next();
          uint64_t number;
          if (!e.Has() || !e.Get().IsNumber()) {
            number = 0;
          } else {
            number = e.Get().GetInt64();
          }
          CounterKeyedSample sample = {time, number, count};
          MOZ_RELEASE_ASSERT(data.append(sample));
        }
      } else {
        // skip counter sample - only need to skip the initial counter
        // id, then let the loop at the top skip the rest
      }
    }
    e.Next();
  }
  // we have a map of a map of counter entries; dump them to JSON
  if (counters.count() == 0) {
    return;
  }

  aWriter.StartArrayProperty("counters");
<<<<<<< HEAD
  for (auto iter = counters.Iter(); !iter.Done(); iter.Next()) {
    CounterMap& counter = iter.Data();
    const BaseProfilerCount* base_counter =
        static_cast<const BaseProfilerCount*>(iter.Key());
||||||| merged common ancestors
  for (auto iter = counters.Iter(); !iter.Done(); iter.Next()) {
    CounterMap& counter = iter.Data();
    const BaseProfilerCount* base_counter = static_cast<const BaseProfilerCount*>(iter.Key());
=======
  for (auto iter = counters.iter(); !iter.done(); iter.next()) {
    CounterMap& counter = iter.get().value();
    const BaseProfilerCount* base_counter =
        static_cast<const BaseProfilerCount*>(iter.get().key());
>>>>>>> upstream-releases

    aWriter.Start();
    aWriter.StringProperty("name", base_counter->mLabel);
    aWriter.StringProperty("category", base_counter->mCategory);
    aWriter.StringProperty("description", base_counter->mDescription);

    aWriter.StartObjectProperty("sample_groups");
<<<<<<< HEAD
    for (auto counter_iter = counter.Iter(); !counter_iter.Done();
         counter_iter.Next()) {
      CounterKeyedSamples& samples = counter_iter.Data();
      uint64_t key = counter_iter.Key();
||||||| merged common ancestors
    for (auto counter_iter = counter.Iter(); !counter_iter.Done(); counter_iter.Next()) {
      CounterKeyedSamples& samples = counter_iter.Data();
      uint64_t key = counter_iter.Key();
=======
    for (auto counter_iter = counter.iter(); !counter_iter.done();
         counter_iter.next()) {
      CounterKeyedSamples& samples = counter_iter.get().value();
      uint64_t key = counter_iter.get().key();
>>>>>>> upstream-releases

      size_t size = samples.length();
      if (size == 0) {
        continue;
      }
      aWriter.IntProperty("id", static_cast<int64_t>(key));
      aWriter.StartObjectProperty("samples");
      {
        // XXX Can we assume a missing count means 0?
        JSONSchemaWriter schema(aWriter);
        schema.WriteField("time");
        schema.WriteField("number");
        schema.WriteField("count");
      }

      aWriter.StartArrayProperty("data");
      uint64_t previousNumber = 0;
      int64_t previousCount = 0;
      for (size_t i = 0; i < size; i++) {
        // Encode as deltas, and only encode if different than the last sample
        if (i == 0 || samples[i].mNumber != previousNumber ||
            samples[i].mCount != previousCount) {
          if (i != 0 && samples[i].mTime >= samples[i - 1].mTime) {
            MOZ_LOG(sFuzzyfoxLog, mozilla::LogLevel::Error,
                    ("Fuzzyfox Profiler Assertion: %f >= %f", samples[i].mTime,
                     samples[i - 1].mTime));
          }
          MOZ_ASSERT(i == 0 || samples[i].mTime >= samples[i - 1].mTime);
          MOZ_ASSERT(samples[i].mNumber >= previousNumber);
<<<<<<< HEAD

          aWriter.StartArrayElement(SpliceableJSONWriter::SingleLineStyle);
          aWriter.DoubleElement(samples[i].mTime);
          aWriter.IntElement(samples[i].mNumber - previousNumber);  // uint64_t
          aWriter.IntElement(samples[i].mCount - previousCount);    // int64_t
          aWriter.EndArray();
||||||| merged common ancestors

          aWriter.StartArrayElement(SpliceableJSONWriter::SingleLineStyle);
          aWriter.DoubleElement(samples[i].mTime);
          aWriter.IntElement(samples[i].mNumber - previousNumber); // uint64_t
          aWriter.IntElement(samples[i].mCount - previousCount); // int64_t
          aWriter.EndArray();
=======
          MOZ_ASSERT(samples[i].mNumber - previousNumber <=
                     uint64_t(std::numeric_limits<int64_t>::max()));

          AutoArraySchemaWriter writer(aWriter);
          writer.DoubleElement(TIME, samples[i].mTime);
          writer.IntElement(NUMBER, static_cast<int64_t>(samples[i].mNumber -
                                                         previousNumber));
          writer.IntElement(COUNT, samples[i].mCount - previousCount);
>>>>>>> upstream-releases
          previousNumber = samples[i].mNumber;
          previousCount = samples[i].mCount;
        }
      }
      aWriter.EndArray();   // data
      aWriter.EndObject();  // samples
    }
    aWriter.EndObject();  // sample groups
    aWriter.End();        // for each counter
  }
  aWriter.EndArray();  // counters
}

void ProfileBuffer::StreamMemoryToJSON(SpliceableJSONWriter& aWriter,
                                       const TimeStamp& aProcessStartTime,
                                       double aSinceTime) const {
  enum Schema : uint32_t { TIME = 0, RSS = 1, USS = 2 };

  EntryGetter e(*this);

  aWriter.StartObjectProperty("memory");
  // Stream all memory (rss/uss) data. We skip other entries, because we
  // process them in StreamSamplesToJSON()/etc.
  aWriter.IntProperty("initial_heap", 0);  // XXX FIX
  aWriter.StartObjectProperty("samples");
  {
    JSONSchemaWriter schema(aWriter);
    schema.WriteField("time");
    schema.WriteField("rss");
    schema.WriteField("uss");
  }

  aWriter.StartArrayProperty("data");
  int64_t previous_rss = 0;
  int64_t previous_uss = 0;
  while (e.Has()) {
    // valid sequence: Resident, Unshared?, Time
    if (e.Get().IsResidentMemory()) {
      int64_t rss = e.Get().GetInt64();
      int64_t uss = 0;
      e.Next();
      if (e.Has()) {
        if (e.Get().IsUnsharedMemory()) {
          uss = e.Get().GetDouble();
          e.Next();
          if (!e.Has()) {
            break;
          }
        }
        if (e.Get().IsTime()) {
          double time = e.Get().GetDouble();
          if (time >= aSinceTime &&
              (previous_rss != rss || previous_uss != uss)) {
<<<<<<< HEAD
            aWriter.StartArrayElement(SpliceableJSONWriter::SingleLineStyle);
            aWriter.DoubleElement(time);
            aWriter.IntElement(rss);  // int64_t
||||||| merged common ancestors
            aWriter.StartArrayElement(SpliceableJSONWriter::SingleLineStyle);
            aWriter.DoubleElement(time);
            aWriter.IntElement(rss); // int64_t
=======
            AutoArraySchemaWriter writer(aWriter);
            writer.DoubleElement(TIME, time);
            writer.IntElement(RSS, rss);
>>>>>>> upstream-releases
            if (uss != 0) {
<<<<<<< HEAD
              aWriter.IntElement(uss);  // int64_t
||||||| merged common ancestors
              aWriter.IntElement(uss); // int64_t
=======
              writer.IntElement(USS, uss);
>>>>>>> upstream-releases
            }
            previous_rss = rss;
            previous_uss = uss;
          }
        } else {
          ERROR_AND_CONTINUE("expected a Time entry");
        }
      }
    }
    e.Next();
  }
  aWriter.EndArray();   // data
  aWriter.EndObject();  // samples
  aWriter.EndObject();  // memory
}
#undef ERROR_AND_CONTINUE

static void AddPausedRange(SpliceableJSONWriter& aWriter, const char* aReason,
                           const Maybe<double>& aStartTime,
                           const Maybe<double>& aEndTime) {
  aWriter.Start();
  if (aStartTime) {
    aWriter.DoubleProperty("startTime", *aStartTime);
  } else {
    aWriter.NullProperty("startTime");
  }
  if (aEndTime) {
    aWriter.DoubleProperty("endTime", *aEndTime);
  } else {
    aWriter.NullProperty("endTime");
  }
  aWriter.StringProperty("reason", aReason);
  aWriter.End();
}

void ProfileBuffer::StreamPausedRangesToJSON(SpliceableJSONWriter& aWriter,
                                             double aSinceTime) const {
  EntryGetter e(*this);

  Maybe<double> currentPauseStartTime;
  Maybe<double> currentCollectionStartTime;

  while (e.Has()) {
    if (e.Get().IsPause()) {
      currentPauseStartTime = Some(e.Get().GetDouble());
    } else if (e.Get().IsResume()) {
<<<<<<< HEAD
      AddPausedRange(aWriter, "profiler-paused", currentPauseStartTime,
                     Some(e.Get().u.mDouble));
||||||| merged common ancestors
      AddPausedRange(aWriter, "profiler-paused",
                     currentPauseStartTime, Some(e.Get().u.mDouble));
=======
      AddPausedRange(aWriter, "profiler-paused", currentPauseStartTime,
                     Some(e.Get().GetDouble()));
>>>>>>> upstream-releases
      currentPauseStartTime = Nothing();
    } else if (e.Get().IsCollectionStart()) {
      currentCollectionStartTime = Some(e.Get().GetDouble());
    } else if (e.Get().IsCollectionEnd()) {
<<<<<<< HEAD
      AddPausedRange(aWriter, "collecting", currentCollectionStartTime,
                     Some(e.Get().u.mDouble));
||||||| merged common ancestors
      AddPausedRange(aWriter, "collecting",
                     currentCollectionStartTime, Some(e.Get().u.mDouble));
=======
      AddPausedRange(aWriter, "collecting", currentCollectionStartTime,
                     Some(e.Get().GetDouble()));
>>>>>>> upstream-releases
      currentCollectionStartTime = Nothing();
    }
    e.Next();
  }

  if (currentPauseStartTime) {
    AddPausedRange(aWriter, "profiler-paused", currentPauseStartTime,
                   Nothing());
  }
  if (currentCollectionStartTime) {
    AddPausedRange(aWriter, "collecting", currentCollectionStartTime,
                   Nothing());
  }
}

bool ProfileBuffer::DuplicateLastSample(int aThreadId,
                                        const TimeStamp& aProcessStartTime,
                                        Maybe<uint64_t>& aLastSample) {
  if (aLastSample && *aLastSample < mRangeStart) {
    // The last sample is no longer within the buffer range, so we cannot use
    // it. Reset the stored buffer position to Nothing().
    aLastSample.reset();
  }

  if (!aLastSample) {
    return false;
  }

  uint64_t lastSampleStartPos = *aLastSample;

  MOZ_RELEASE_ASSERT(GetEntry(lastSampleStartPos).IsThreadId() &&
                     GetEntry(lastSampleStartPos).GetInt() == aThreadId);

  aLastSample = Some(AddThreadIdEntry(aThreadId));

  EntryGetter e(*this, lastSampleStartPos + 1);

  // Go through the whole entry and duplicate it, until we find the next one.
  while (e.Has()) {
    switch (e.Get().GetKind()) {
      case ProfileBufferEntry::Kind::Pause:
      case ProfileBufferEntry::Kind::Resume:
      case ProfileBufferEntry::Kind::CollectionStart:
      case ProfileBufferEntry::Kind::CollectionEnd:
      case ProfileBufferEntry::Kind::ThreadId:
        // We're done.
        return true;
      case ProfileBufferEntry::Kind::Time:
        // Copy with new time
        AddEntry(ProfileBufferEntry::Time(
<<<<<<< HEAD
            (TimeStamp::Now() - aProcessStartTime).ToMilliseconds()));
||||||| merged common ancestors
          (TimeStamp::Now() - aProcessStartTime).ToMilliseconds()));
=======
            (TimeStamp::NowUnfuzzed() - aProcessStartTime).ToMilliseconds()));
>>>>>>> upstream-releases
        break;
      case ProfileBufferEntry::Kind::Marker:
      case ProfileBufferEntry::Kind::ResidentMemory:
      case ProfileBufferEntry::Kind::UnsharedMemory:
      case ProfileBufferEntry::Kind::CounterKey:
      case ProfileBufferEntry::Kind::Number:
      case ProfileBufferEntry::Kind::Count:
      case ProfileBufferEntry::Kind::Responsiveness:
        // Don't copy anything not part of a thread's stack sample
        break;
      case ProfileBufferEntry::Kind::CounterId:
        // CounterId is normally followed by Time - if so, we'd like
        // to skip it.  If we duplicate Time, it won't hurt anything, just
        // waste buffer space (and this can happen if the CounterId has
        // fallen off the end of the buffer, but Time (and Number/Count)
        // are still in the buffer).
        e.Next();
        if (e.Has() && e.Get().GetKind() != ProfileBufferEntry::Kind::Time) {
          // this would only happen if there was an invalid sequence
          // in the buffer.  Don't skip it.
          continue;
        }
        // we've skipped Time
        break;
      case ProfileBufferEntry::Kind::ProfilerOverheadTime:
        // ProfilerOverheadTime is normally followed by
        // ProfilerOverheadDuration*4 - if so, we'd like to skip it. Don't
        // duplicate, as we are in the middle of a sampling and will soon
        // capture its own overhead.
        e.Next();
        // A missing Time would only happen if there was an invalid sequence
        // in the buffer. Don't skip unexpected entry.
        if (e.Has() && e.Get().GetKind() !=
                           ProfileBufferEntry::Kind::ProfilerOverheadDuration) {
          continue;
        }
        e.Next();
        if (e.Has() && e.Get().GetKind() !=
                           ProfileBufferEntry::Kind::ProfilerOverheadDuration) {
          continue;
        }
        e.Next();
        if (e.Has() && e.Get().GetKind() !=
                           ProfileBufferEntry::Kind::ProfilerOverheadDuration) {
          continue;
        }
        e.Next();
        if (e.Has() && e.Get().GetKind() !=
                           ProfileBufferEntry::Kind::ProfilerOverheadDuration) {
          continue;
        }
        // we've skipped ProfilerOverheadTime and ProfilerOverheadDuration*4.
        break;
      default: {
        // Copy anything else we don't know about.
        ProfileBufferEntry entry = e.Get();
        AddEntry(entry);
        break;
      }
    }
    e.Next();
  }
  return true;
}

<<<<<<< HEAD
void ProfileBuffer::DiscardSamplesBeforeTime(double aTime) {
  EntryGetter e(*this);
  for (;;) {
    // This block skips entries until we find the start of the next sample.
    // This is useful in three situations.
    //
    // - The circular buffer overwrites old entries, so when we start parsing
    //   we might be in the middle of a sample, and we must skip forward to the
    //   start of the next sample.
    //
    // - We skip samples that don't have an appropriate ThreadId or Time.
    //
    // - We skip range Pause, Resume, CollectionStart, Marker, and CollectionEnd
    //   entries between samples.
    while (e.Has()) {
      if (e.Get().IsThreadId()) {
        break;
      } else {
        e.Next();
      }
    }

    if (!e.Has()) {
      break;
    }

    MOZ_RELEASE_ASSERT(e.Get().IsThreadId());
    uint64_t sampleStartPos = e.CurPos();
    e.Next();

    if (e.Has() && e.Get().IsTime()) {
      double sampleTime = e.Get().u.mDouble;

      if (sampleTime >= aTime) {
        // This is the first sample within the window of time that we want to
        // keep. Throw away all samples before sampleStartPos and return.
        mRangeStart = sampleStartPos;
        return;
      }
    }
  }
}

||||||| merged common ancestors
=======
void ProfileBuffer::DiscardSamplesBeforeTime(double aTime) {
  EntryGetter e(*this);
  for (;;) {
    // This block skips entries until we find the start of the next sample.
    // This is useful in three situations.
    //
    // - The circular buffer overwrites old entries, so when we start parsing
    //   we might be in the middle of a sample, and we must skip forward to the
    //   start of the next sample.
    //
    // - We skip samples that don't have an appropriate ThreadId or Time.
    //
    // - We skip range Pause, Resume, CollectionStart, Marker, and CollectionEnd
    //   entries between samples.
    while (e.Has()) {
      if (e.Get().IsThreadId()) {
        break;
      } else {
        e.Next();
      }
    }

    if (!e.Has()) {
      break;
    }

    MOZ_RELEASE_ASSERT(e.Get().IsThreadId());
    uint64_t sampleStartPos = e.CurPos();
    e.Next();

    if (e.Has() && e.Get().IsTime()) {
      double sampleTime = e.Get().GetDouble();

      if (sampleTime >= aTime) {
        // This is the first sample within the window of time that we want to
        // keep. Throw away all samples before sampleStartPos and return.
        mRangeStart = sampleStartPos;
        return;
      }
    }
  }
}

>>>>>>> upstream-releases
// END ProfileBuffer
////////////////////////////////////////////////////////////////////////
