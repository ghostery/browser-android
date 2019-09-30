/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=2 et lcs=trail\:.,tab\:>~ :
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <limits.h>

#include "mozilla/UniquePtrExtensions.h"
#include "nsString.h"

#include "mozStorageError.h"
#include "mozStoragePrivateHelpers.h"
#include "mozStorageBindingParams.h"
#include "mozStorageBindingParamsArray.h"
#include "Variant.h"

namespace mozilla {
namespace storage {

////////////////////////////////////////////////////////////////////////////////
//// Local Helper Objects

namespace {

<<<<<<< HEAD
struct BindingColumnData {
  BindingColumnData(sqlite3_stmt *aStmt, int aColumn)
      : stmt(aStmt), column(aColumn) {}
  sqlite3_stmt *stmt;
||||||| merged common ancestors
struct BindingColumnData
{
  BindingColumnData(sqlite3_stmt *aStmt,
                    int aColumn)
  : stmt(aStmt)
  , column(aColumn)
  {
  }
  sqlite3_stmt *stmt;
=======
struct BindingColumnData {
  BindingColumnData(sqlite3_stmt* aStmt, int aColumn)
      : stmt(aStmt), column(aColumn) {}
  sqlite3_stmt* stmt;
>>>>>>> upstream-releases
  int column;
};

////////////////////////////////////////////////////////////////////////////////
//// Variant Specialization Functions (variantToSQLiteT)

int sqlite3_T_int(BindingColumnData aData, int aValue) {
  return ::sqlite3_bind_int(aData.stmt, aData.column + 1, aValue);
}

int sqlite3_T_int64(BindingColumnData aData, sqlite3_int64 aValue) {
  return ::sqlite3_bind_int64(aData.stmt, aData.column + 1, aValue);
}

int sqlite3_T_double(BindingColumnData aData, double aValue) {
  return ::sqlite3_bind_double(aData.stmt, aData.column + 1, aValue);
}

<<<<<<< HEAD
int sqlite3_T_text(BindingColumnData aData, const nsCString &aValue) {
  return ::sqlite3_bind_text(aData.stmt, aData.column + 1, aValue.get(),
                             aValue.Length(), SQLITE_TRANSIENT);
}

int sqlite3_T_text16(BindingColumnData aData, const nsString &aValue) {
  return ::sqlite3_bind_text16(aData.stmt, aData.column + 1, aValue.get(),
                               aValue.Length() * 2,  // Length in bytes!
                               SQLITE_TRANSIENT);
||||||| merged common ancestors
int
sqlite3_T_text(BindingColumnData aData,
               const nsCString& aValue)
{
  return ::sqlite3_bind_text(aData.stmt,
                             aData.column + 1,
                             aValue.get(),
                             aValue.Length(),
                             SQLITE_TRANSIENT);
}

int
sqlite3_T_text16(BindingColumnData aData,
                 const nsString& aValue)
{
  return ::sqlite3_bind_text16(aData.stmt,
                               aData.column + 1,
                               aValue.get(),
                               aValue.Length() * 2, // Length in bytes!
                               SQLITE_TRANSIENT);
=======
int sqlite3_T_text(BindingColumnData aData, const nsCString& aValue) {
  return ::sqlite3_bind_text(aData.stmt, aData.column + 1, aValue.get(),
                             aValue.Length(), SQLITE_TRANSIENT);
}

int sqlite3_T_text16(BindingColumnData aData, const nsString& aValue) {
  return ::sqlite3_bind_text16(
      aData.stmt, aData.column + 1, aValue.get(),
      aValue.Length() * sizeof(char16_t),  // Length in bytes!
      SQLITE_TRANSIENT);
>>>>>>> upstream-releases
}

int sqlite3_T_null(BindingColumnData aData) {
  return ::sqlite3_bind_null(aData.stmt, aData.column + 1);
}

<<<<<<< HEAD
int sqlite3_T_blob(BindingColumnData aData, const void *aBlob, int aSize) {
  return ::sqlite3_bind_blob(aData.stmt, aData.column + 1, aBlob, aSize, free);
||||||| merged common ancestors
int
sqlite3_T_blob(BindingColumnData aData,
               const void *aBlob,
               int aSize)
{
  return ::sqlite3_bind_blob(aData.stmt, aData.column + 1, aBlob, aSize,
                             free);

=======
int sqlite3_T_blob(BindingColumnData aData, const void* aBlob, int aSize) {
  return ::sqlite3_bind_blob(aData.stmt, aData.column + 1, aBlob, aSize, free);
>>>>>>> upstream-releases
}

#include "variantToSQLiteT_impl.h"

}  // namespace

////////////////////////////////////////////////////////////////////////////////
//// BindingParams

<<<<<<< HEAD
BindingParams::BindingParams(mozIStorageBindingParamsArray *aOwningArray,
                             Statement *aOwningStatement)
    : mLocked(false),
      mOwningArray(aOwningArray),
      mOwningStatement(aOwningStatement),
      mParamCount(0) {
||||||| merged common ancestors
BindingParams::BindingParams(mozIStorageBindingParamsArray *aOwningArray,
                             Statement *aOwningStatement)
: mLocked(false)
, mOwningArray(aOwningArray)
, mOwningStatement(aOwningStatement)
, mParamCount(0)
{
=======
BindingParams::BindingParams(mozIStorageBindingParamsArray* aOwningArray,
                             Statement* aOwningStatement)
    : mLocked(false),
      mOwningArray(aOwningArray),
      mOwningStatement(aOwningStatement),
      mParamCount(0) {
>>>>>>> upstream-releases
  (void)mOwningStatement->GetParameterCount(&mParamCount);
  mParameters.SetCapacity(mParamCount);
}

<<<<<<< HEAD
BindingParams::BindingParams(mozIStorageBindingParamsArray *aOwningArray)
    : mLocked(false),
      mOwningArray(aOwningArray),
      mOwningStatement(nullptr),
      mParamCount(0) {}
||||||| merged common ancestors
BindingParams::BindingParams(mozIStorageBindingParamsArray *aOwningArray)
: mLocked(false)
, mOwningArray(aOwningArray)
, mOwningStatement(nullptr)
, mParamCount(0)
{
}
=======
BindingParams::BindingParams(mozIStorageBindingParamsArray* aOwningArray)
    : mLocked(false),
      mOwningArray(aOwningArray),
      mOwningStatement(nullptr),
      mParamCount(0) {}
>>>>>>> upstream-releases

AsyncBindingParams::AsyncBindingParams(
<<<<<<< HEAD
    mozIStorageBindingParamsArray *aOwningArray)
    : BindingParams(aOwningArray) {}
||||||| merged common ancestors
  mozIStorageBindingParamsArray *aOwningArray
)
: BindingParams(aOwningArray)
{
}
=======
    mozIStorageBindingParamsArray* aOwningArray)
    : BindingParams(aOwningArray) {}
>>>>>>> upstream-releases

void BindingParams::lock() {
  NS_ASSERTION(mLocked == false, "Parameters have already been locked!");
  mLocked = true;

  // We no longer need to hold a reference to our statement or our owning array.
  // The array owns us at this point, and it will own a reference to the
  // statement.
  mOwningStatement = nullptr;
  mOwningArray = nullptr;
}

<<<<<<< HEAD
void BindingParams::unlock(Statement *aOwningStatement) {
||||||| merged common ancestors
void
BindingParams::unlock(Statement *aOwningStatement)
{
=======
void BindingParams::unlock(Statement* aOwningStatement) {
>>>>>>> upstream-releases
  NS_ASSERTION(mLocked == true, "Parameters were not yet locked!");
  mLocked = false;
  mOwningStatement = aOwningStatement;
}

<<<<<<< HEAD
const mozIStorageBindingParamsArray *BindingParams::getOwner() const {
||||||| merged common ancestors
const mozIStorageBindingParamsArray *
BindingParams::getOwner() const
{
=======
const mozIStorageBindingParamsArray* BindingParams::getOwner() const {
>>>>>>> upstream-releases
  return mOwningArray;
}

////////////////////////////////////////////////////////////////////////////////
//// nsISupports

NS_IMPL_ISUPPORTS(BindingParams, mozIStorageBindingParams,
                  IStorageBindingParamsInternal)

////////////////////////////////////////////////////////////////////////////////
//// IStorageBindingParamsInternal

<<<<<<< HEAD
already_AddRefed<mozIStorageError> BindingParams::bind(
    sqlite3_stmt *aStatement) {
||||||| merged common ancestors
already_AddRefed<mozIStorageError>
BindingParams::bind(sqlite3_stmt *aStatement)
{
=======
already_AddRefed<mozIStorageError> BindingParams::bind(
    sqlite3_stmt* aStatement) {
>>>>>>> upstream-releases
  // Iterate through all of our stored data, and bind it.
  for (size_t i = 0; i < mParameters.Length(); i++) {
    int rc = variantToSQLiteT(BindingColumnData(aStatement, i), mParameters[i]);
    if (rc != SQLITE_OK) {
      // We had an error while trying to bind.  Now we need to create an error
      // object with the right message.  Note that we special case
      // SQLITE_MISMATCH, but otherwise get the message from SQLite.
      const char* msg = "Could not covert nsIVariant to SQLite type.";
      if (rc != SQLITE_MISMATCH)
        msg = ::sqlite3_errmsg(::sqlite3_db_handle(aStatement));

      nsCOMPtr<mozIStorageError> err(new Error(rc, msg));
      return err.forget();
    }
  }

  return nullptr;
}

<<<<<<< HEAD
already_AddRefed<mozIStorageError> AsyncBindingParams::bind(
    sqlite3_stmt *aStatement) {
||||||| merged common ancestors
already_AddRefed<mozIStorageError>
AsyncBindingParams::bind(sqlite3_stmt * aStatement)
{
=======
already_AddRefed<mozIStorageError> AsyncBindingParams::bind(
    sqlite3_stmt* aStatement) {
>>>>>>> upstream-releases
  // We should bind by index using the super-class if there is nothing in our
  // hashtable.
  if (!mNamedParameters.Count()) return BindingParams::bind(aStatement);

  nsCOMPtr<mozIStorageError> err;

  for (auto iter = mNamedParameters.Iter(); !iter.Done(); iter.Next()) {
    const nsACString& key = iter.Key();

    // We do not accept any forms of names other than ":name", but we need to
    // add the colon for SQLite.
    nsAutoCString name(":");
    name.Append(key);
    int oneIdx = ::sqlite3_bind_parameter_index(aStatement, name.get());

    if (oneIdx == 0) {
      nsAutoCString errMsg(key);
      errMsg.AppendLiteral(" is not a valid named parameter.");
      err = new Error(SQLITE_RANGE, errMsg.get());
      break;
    }

    // XPCVariant's AddRef and Release are not thread-safe and so we must not
    // do anything that would invoke them here on the async thread.  As such we
    // can't cram aValue into mParameters using ReplaceObjectAt so that
    // we can freeload off of the BindingParams::Bind implementation.
    int rc = variantToSQLiteT(BindingColumnData(aStatement, oneIdx - 1),
                              iter.UserData());
    if (rc != SQLITE_OK) {
      // We had an error while trying to bind.  Now we need to create an error
      // object with the right message.  Note that we special case
      // SQLITE_MISMATCH, but otherwise get the message from SQLite.
      const char* msg = "Could not covert nsIVariant to SQLite type.";
      if (rc != SQLITE_MISMATCH) {
        msg = ::sqlite3_errmsg(::sqlite3_db_handle(aStatement));
      }
      err = new Error(rc, msg);
      break;
    }
  }

  return err.forget();
}

///////////////////////////////////////////////////////////////////////////////
//// mozIStorageBindingParams

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindByName(const nsACString &aName, nsIVariant *aValue) {
||||||| merged common ancestors
BindingParams::BindByName(const nsACString &aName,
                          nsIVariant *aValue)
{
=======
BindingParams::BindByName(const nsACString& aName, nsIVariant* aValue) {
>>>>>>> upstream-releases
  NS_ENSURE_FALSE(mLocked, NS_ERROR_UNEXPECTED);

  // Get the column index that we need to store this at.
  uint32_t index;
  nsresult rv = mOwningStatement->GetParameterIndex(aName, &index);
  NS_ENSURE_SUCCESS(rv, rv);

  return BindByIndex(index, aValue);
}

NS_IMETHODIMP
<<<<<<< HEAD
AsyncBindingParams::BindByName(const nsACString &aName, nsIVariant *aValue) {
||||||| merged common ancestors
AsyncBindingParams::BindByName(const nsACString &aName,
                               nsIVariant *aValue)
{
=======
AsyncBindingParams::BindByName(const nsACString& aName, nsIVariant* aValue) {
>>>>>>> upstream-releases
  NS_ENSURE_FALSE(mLocked, NS_ERROR_UNEXPECTED);

  RefPtr<Variant_base> variant = convertVariantToStorageVariant(aValue);
  if (!variant) return NS_ERROR_UNEXPECTED;

  mNamedParameters.Put(aName, variant);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindUTF8StringByName(const nsACString &aName,
                                    const nsACString &aValue) {
||||||| merged common ancestors
BindingParams::BindUTF8StringByName(const nsACString &aName,
                                    const nsACString &aValue)
{
=======
BindingParams::BindUTF8StringByName(const nsACString& aName,
                                    const nsACString& aValue) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new UTF8TextVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByName(aName, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindStringByName(const nsACString &aName,
                                const nsAString &aValue) {
||||||| merged common ancestors
BindingParams::BindStringByName(const nsACString &aName,
                                const nsAString &aValue)
{
=======
BindingParams::BindStringByName(const nsACString& aName,
                                const nsAString& aValue) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new TextVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByName(aName, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindDoubleByName(const nsACString &aName, double aValue) {
||||||| merged common ancestors
BindingParams::BindDoubleByName(const nsACString &aName,
                                double aValue)
{
=======
BindingParams::BindDoubleByName(const nsACString& aName, double aValue) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new FloatVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByName(aName, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindInt32ByName(const nsACString &aName, int32_t aValue) {
||||||| merged common ancestors
BindingParams::BindInt32ByName(const nsACString &aName,
                               int32_t aValue)
{
=======
BindingParams::BindInt32ByName(const nsACString& aName, int32_t aValue) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new IntegerVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByName(aName, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindInt64ByName(const nsACString &aName, int64_t aValue) {
||||||| merged common ancestors
BindingParams::BindInt64ByName(const nsACString &aName,
                               int64_t aValue)
{
=======
BindingParams::BindInt64ByName(const nsACString& aName, int64_t aValue) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new IntegerVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByName(aName, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindNullByName(const nsACString &aName) {
||||||| merged common ancestors
BindingParams::BindNullByName(const nsACString &aName)
{
=======
BindingParams::BindNullByName(const nsACString& aName) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new NullVariant());
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByName(aName, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindBlobByName(const nsACString &aName, const uint8_t *aValue,
                              uint32_t aValueSize) {
||||||| merged common ancestors
BindingParams::BindBlobByName(const nsACString &aName,
                              const uint8_t *aValue,
                              uint32_t aValueSize)
{
=======
BindingParams::BindBlobByName(const nsACString& aName, const uint8_t* aValue,
                              uint32_t aValueSize) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_MAX(aValueSize, INT_MAX);
<<<<<<< HEAD
  std::pair<const void *, int> data(static_cast<const void *>(aValue),
                                    int(aValueSize));
||||||| merged common ancestors
  std::pair<const void *, int> data(
    static_cast<const void *>(aValue),
    int(aValueSize)
  );
=======
  std::pair<const void*, int> data(static_cast<const void*>(aValue),
                                   int(aValueSize));
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new BlobVariant(data));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByName(aName, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindStringAsBlobByName(const nsACString &aName,
                                      const nsAString &aValue) {
||||||| merged common ancestors
BindingParams::BindStringAsBlobByName(const nsACString& aName,
                                      const nsAString& aValue)
{
=======
BindingParams::BindBlobArrayByName(const nsACString& aName,
                                   const nsTArray<uint8_t>& aValue) {
  return BindBlobByName(aName, aValue.Elements(), aValue.Length());
}

NS_IMETHODIMP
BindingParams::BindStringAsBlobByName(const nsACString& aName,
                                      const nsAString& aValue) {
>>>>>>> upstream-releases
  return DoBindStringAsBlobByName(this, aName, aValue);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindUTF8StringAsBlobByName(const nsACString &aName,
                                          const nsACString &aValue) {
||||||| merged common ancestors
BindingParams::BindUTF8StringAsBlobByName(const nsACString& aName,
                                          const nsACString& aValue)
{
=======
BindingParams::BindUTF8StringAsBlobByName(const nsACString& aName,
                                          const nsACString& aValue) {
>>>>>>> upstream-releases
  return DoBindStringAsBlobByName(this, aName, aValue);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindAdoptedBlobByName(const nsACString &aName, uint8_t *aValue,
                                     uint32_t aValueSize) {
||||||| merged common ancestors
BindingParams::BindAdoptedBlobByName(const nsACString &aName,
                                     uint8_t *aValue,
                                     uint32_t aValueSize)
{
=======
BindingParams::BindAdoptedBlobByName(const nsACString& aName, uint8_t* aValue,
                                     uint32_t aValueSize) {
>>>>>>> upstream-releases
  UniqueFreePtr<uint8_t> uniqueValue(aValue);
  NS_ENSURE_ARG_MAX(aValueSize, INT_MAX);
  std::pair<uint8_t*, int> data(uniqueValue.release(), int(aValueSize));
  nsCOMPtr<nsIVariant> value(new AdoptedBlobVariant(data));

  return BindByName(aName, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindByIndex(uint32_t aIndex, nsIVariant *aValue) {
||||||| merged common ancestors
BindingParams::BindByIndex(uint32_t aIndex,
                           nsIVariant *aValue)
{
=======
BindingParams::BindByIndex(uint32_t aIndex, nsIVariant* aValue) {
>>>>>>> upstream-releases
  NS_ENSURE_FALSE(mLocked, NS_ERROR_UNEXPECTED);
  ENSURE_INDEX_VALUE(aIndex, mParamCount);

  // Store the variant for later use.
  RefPtr<Variant_base> variant = convertVariantToStorageVariant(aValue);
  if (!variant) return NS_ERROR_UNEXPECTED;
  if (mParameters.Length() <= aIndex) {
    (void)mParameters.SetLength(aIndex);
    (void)mParameters.AppendElement(variant);
  } else {
    NS_ENSURE_TRUE(mParameters.ReplaceElementAt(aIndex, variant),
                   NS_ERROR_OUT_OF_MEMORY);
  }
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
AsyncBindingParams::BindByIndex(uint32_t aIndex, nsIVariant *aValue) {
||||||| merged common ancestors
AsyncBindingParams::BindByIndex(uint32_t aIndex,
                                nsIVariant *aValue)
{
=======
AsyncBindingParams::BindByIndex(uint32_t aIndex, nsIVariant* aValue) {
>>>>>>> upstream-releases
  NS_ENSURE_FALSE(mLocked, NS_ERROR_UNEXPECTED);
  // In the asynchronous case we do not know how many parameters there are to
  // bind to, so we cannot check the validity of aIndex.

  RefPtr<Variant_base> variant = convertVariantToStorageVariant(aValue);
  if (!variant) return NS_ERROR_UNEXPECTED;
  if (mParameters.Length() <= aIndex) {
    mParameters.SetLength(aIndex);
    mParameters.AppendElement(variant);
  } else {
    NS_ENSURE_TRUE(mParameters.ReplaceElementAt(aIndex, variant),
                   NS_ERROR_OUT_OF_MEMORY);
  }
  return NS_OK;
}

NS_IMETHODIMP
BindingParams::BindUTF8StringByIndex(uint32_t aIndex,
<<<<<<< HEAD
                                     const nsACString &aValue) {
||||||| merged common ancestors
                                     const nsACString &aValue)
{
=======
                                     const nsACString& aValue) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new UTF8TextVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByIndex(aIndex, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindStringByIndex(uint32_t aIndex, const nsAString &aValue) {
||||||| merged common ancestors
BindingParams::BindStringByIndex(uint32_t aIndex,
                                 const nsAString &aValue)
{
=======
BindingParams::BindStringByIndex(uint32_t aIndex, const nsAString& aValue) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new TextVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByIndex(aIndex, value);
}

NS_IMETHODIMP
BindingParams::BindDoubleByIndex(uint32_t aIndex, double aValue) {
  nsCOMPtr<nsIVariant> value(new FloatVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByIndex(aIndex, value);
}

NS_IMETHODIMP
BindingParams::BindInt32ByIndex(uint32_t aIndex, int32_t aValue) {
  nsCOMPtr<nsIVariant> value(new IntegerVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByIndex(aIndex, value);
}

NS_IMETHODIMP
BindingParams::BindInt64ByIndex(uint32_t aIndex, int64_t aValue) {
  nsCOMPtr<nsIVariant> value(new IntegerVariant(aValue));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByIndex(aIndex, value);
}

NS_IMETHODIMP
BindingParams::BindNullByIndex(uint32_t aIndex) {
  nsCOMPtr<nsIVariant> value(new NullVariant());
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByIndex(aIndex, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindBlobByIndex(uint32_t aIndex, const uint8_t *aValue,
                               uint32_t aValueSize) {
||||||| merged common ancestors
BindingParams::BindBlobByIndex(uint32_t aIndex,
                               const uint8_t *aValue,
                               uint32_t aValueSize)
{
=======
BindingParams::BindBlobByIndex(uint32_t aIndex, const uint8_t* aValue,
                               uint32_t aValueSize) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_MAX(aValueSize, INT_MAX);
<<<<<<< HEAD
  std::pair<const void *, int> data(static_cast<const void *>(aValue),
                                    int(aValueSize));
||||||| merged common ancestors
  std::pair<const void *, int> data(
    static_cast<const void *>(aValue),
    int(aValueSize)
  );
=======
  std::pair<const void*, int> data(static_cast<const void*>(aValue),
                                   int(aValueSize));
>>>>>>> upstream-releases
  nsCOMPtr<nsIVariant> value(new BlobVariant(data));
  NS_ENSURE_TRUE(value, NS_ERROR_OUT_OF_MEMORY);

  return BindByIndex(aIndex, value);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindStringAsBlobByIndex(uint32_t aIndex,
                                       const nsAString &aValue) {
||||||| merged common ancestors
BindingParams::BindStringAsBlobByIndex(uint32_t aIndex, const nsAString& aValue)
{
=======
BindingParams::BindBlobArrayByIndex(uint32_t aIndex,
                                    const nsTArray<uint8_t>& aValue) {
  return BindBlobByIndex(aIndex, aValue.Elements(), aValue.Length());
}

NS_IMETHODIMP
BindingParams::BindStringAsBlobByIndex(uint32_t aIndex,
                                       const nsAString& aValue) {
>>>>>>> upstream-releases
  return DoBindStringAsBlobByIndex(this, aIndex, aValue);
}

NS_IMETHODIMP
BindingParams::BindUTF8StringAsBlobByIndex(uint32_t aIndex,
<<<<<<< HEAD
                                           const nsACString &aValue) {
||||||| merged common ancestors
                                           const nsACString& aValue)
{
=======
                                           const nsACString& aValue) {
>>>>>>> upstream-releases
  return DoBindStringAsBlobByIndex(this, aIndex, aValue);
}

NS_IMETHODIMP
<<<<<<< HEAD
BindingParams::BindAdoptedBlobByIndex(uint32_t aIndex, uint8_t *aValue,
                                      uint32_t aValueSize) {
||||||| merged common ancestors
BindingParams::BindAdoptedBlobByIndex(uint32_t aIndex,
                                      uint8_t *aValue,
                                      uint32_t aValueSize)
{
=======
BindingParams::BindAdoptedBlobByIndex(uint32_t aIndex, uint8_t* aValue,
                                      uint32_t aValueSize) {
>>>>>>> upstream-releases
  UniqueFreePtr<uint8_t> uniqueValue(aValue);
  NS_ENSURE_ARG_MAX(aValueSize, INT_MAX);
  std::pair<uint8_t*, int> data(uniqueValue.release(), int(aValueSize));
  nsCOMPtr<nsIVariant> value(new AdoptedBlobVariant(data));

  return BindByIndex(aIndex, value);
}

}  // namespace storage
}  // namespace mozilla
