/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=2 et lcs=trail\:.,tab\:>~ :
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsError.h"
#include "nsMemory.h"
#include "nsString.h"

#include "mozStoragePrivateHelpers.h"
#include "mozStorageArgValueArray.h"

namespace mozilla {
namespace storage {

////////////////////////////////////////////////////////////////////////////////
//// ArgValueArray

<<<<<<< HEAD
ArgValueArray::ArgValueArray(int32_t aArgc, sqlite3_value **aArgv)
    : mArgc(aArgc), mArgv(aArgv) {}
||||||| merged common ancestors
ArgValueArray::ArgValueArray(int32_t aArgc,
                             sqlite3_value **aArgv)
: mArgc(aArgc)
, mArgv(aArgv)
{
}
=======
ArgValueArray::ArgValueArray(int32_t aArgc, sqlite3_value** aArgv)
    : mArgc(aArgc), mArgv(aArgv) {}
>>>>>>> upstream-releases

NS_IMPL_ISUPPORTS(ArgValueArray, mozIStorageValueArray)

////////////////////////////////////////////////////////////////////////////////
//// mozIStorageValueArray

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetNumEntries(uint32_t *_size) {
||||||| merged common ancestors
ArgValueArray::GetNumEntries(uint32_t *_size)
{
=======
ArgValueArray::GetNumEntries(uint32_t* _size) {
>>>>>>> upstream-releases
  *_size = mArgc;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetTypeOfIndex(uint32_t aIndex, int32_t *_type) {
||||||| merged common ancestors
ArgValueArray::GetTypeOfIndex(uint32_t aIndex,
                              int32_t *_type)
{
=======
ArgValueArray::GetTypeOfIndex(uint32_t aIndex, int32_t* _type) {
>>>>>>> upstream-releases
  ENSURE_INDEX_VALUE(aIndex, mArgc);

  int t = ::sqlite3_value_type(mArgv[aIndex]);
  switch (t) {
    case SQLITE_INTEGER:
      *_type = VALUE_TYPE_INTEGER;
      break;
    case SQLITE_FLOAT:
      *_type = VALUE_TYPE_FLOAT;
      break;
    case SQLITE_TEXT:
      *_type = VALUE_TYPE_TEXT;
      break;
    case SQLITE_BLOB:
      *_type = VALUE_TYPE_BLOB;
      break;
    case SQLITE_NULL:
      *_type = VALUE_TYPE_NULL;
      break;
    default:
      return NS_ERROR_FAILURE;
  }

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetInt32(uint32_t aIndex, int32_t *_value) {
||||||| merged common ancestors
ArgValueArray::GetInt32(uint32_t aIndex,
                        int32_t *_value)
{
=======
ArgValueArray::GetInt32(uint32_t aIndex, int32_t* _value) {
>>>>>>> upstream-releases
  ENSURE_INDEX_VALUE(aIndex, mArgc);

  *_value = ::sqlite3_value_int(mArgv[aIndex]);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetInt64(uint32_t aIndex, int64_t *_value) {
||||||| merged common ancestors
ArgValueArray::GetInt64(uint32_t aIndex,
                        int64_t *_value)
{
=======
ArgValueArray::GetInt64(uint32_t aIndex, int64_t* _value) {
>>>>>>> upstream-releases
  ENSURE_INDEX_VALUE(aIndex, mArgc);

  *_value = ::sqlite3_value_int64(mArgv[aIndex]);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetDouble(uint32_t aIndex, double *_value) {
||||||| merged common ancestors
ArgValueArray::GetDouble(uint32_t aIndex,
                         double *_value)
{
=======
ArgValueArray::GetDouble(uint32_t aIndex, double* _value) {
>>>>>>> upstream-releases
  ENSURE_INDEX_VALUE(aIndex, mArgc);

  *_value = ::sqlite3_value_double(mArgv[aIndex]);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetUTF8String(uint32_t aIndex, nsACString &_value) {
||||||| merged common ancestors
ArgValueArray::GetUTF8String(uint32_t aIndex,
                             nsACString &_value)
{
=======
ArgValueArray::GetUTF8String(uint32_t aIndex, nsACString& _value) {
>>>>>>> upstream-releases
  ENSURE_INDEX_VALUE(aIndex, mArgc);

  if (::sqlite3_value_type(mArgv[aIndex]) == SQLITE_NULL) {
    // NULL columns should have IsVoid set to distinguish them from an empty
    // string.
    _value.SetIsVoid(true);
<<<<<<< HEAD
  } else {
    _value.Assign(
        reinterpret_cast<const char *>(::sqlite3_value_text(mArgv[aIndex])),
        ::sqlite3_value_bytes(mArgv[aIndex]));
||||||| merged common ancestors
  }
  else {
    _value.Assign(reinterpret_cast<const char *>(::sqlite3_value_text(mArgv[aIndex])),
                  ::sqlite3_value_bytes(mArgv[aIndex]));
=======
  } else {
    _value.Assign(
        reinterpret_cast<const char*>(::sqlite3_value_text(mArgv[aIndex])),
        ::sqlite3_value_bytes(mArgv[aIndex]));
>>>>>>> upstream-releases
  }
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetString(uint32_t aIndex, nsAString &_value) {
||||||| merged common ancestors
ArgValueArray::GetString(uint32_t aIndex,
                         nsAString &_value)
{
=======
ArgValueArray::GetString(uint32_t aIndex, nsAString& _value) {
>>>>>>> upstream-releases
  ENSURE_INDEX_VALUE(aIndex, mArgc);

  if (::sqlite3_value_type(mArgv[aIndex]) == SQLITE_NULL) {
    // NULL columns should have IsVoid set to distinguish them from an empty
    // string.
    _value.SetIsVoid(true);
  } else {
<<<<<<< HEAD
    _value.Assign(
        static_cast<const char16_t *>(::sqlite3_value_text16(mArgv[aIndex])),
        ::sqlite3_value_bytes16(mArgv[aIndex]) / 2);
||||||| merged common ancestors
    _value.Assign(static_cast<const char16_t *>(::sqlite3_value_text16(mArgv[aIndex])),
                  ::sqlite3_value_bytes16(mArgv[aIndex]) / 2);
=======
    const char16_t* string =
        static_cast<const char16_t*>(::sqlite3_value_text16(mArgv[aIndex]));
    _value.Assign(string,
                  ::sqlite3_value_bytes16(mArgv[aIndex]) / sizeof(char16_t));
>>>>>>> upstream-releases
  }
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetBlob(uint32_t aIndex, uint32_t *_size, uint8_t **_blob) {
||||||| merged common ancestors
ArgValueArray::GetBlob(uint32_t aIndex,
                       uint32_t *_size,
                       uint8_t **_blob)
{
=======
ArgValueArray::GetBlob(uint32_t aIndex, uint32_t* _size, uint8_t** _blob) {
>>>>>>> upstream-releases
  ENSURE_INDEX_VALUE(aIndex, mArgc);

  int size = ::sqlite3_value_bytes(mArgv[aIndex]);
<<<<<<< HEAD
  void *blob = moz_xmemdup(::sqlite3_value_blob(mArgv[aIndex]), size);
  *_blob = static_cast<uint8_t *>(blob);
||||||| merged common ancestors
  void* blob = moz_xmemdup(::sqlite3_value_blob(mArgv[aIndex]), size);
  *_blob = static_cast<uint8_t *>(blob);
=======
  void* blob = moz_xmemdup(::sqlite3_value_blob(mArgv[aIndex]), size);
  *_blob = static_cast<uint8_t*>(blob);
>>>>>>> upstream-releases
  *_size = size;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetBlobAsString(uint32_t aIndex, nsAString &aValue) {
||||||| merged common ancestors
ArgValueArray::GetBlobAsString(uint32_t aIndex, nsAString& aValue)
{
=======
ArgValueArray::GetBlobAsString(uint32_t aIndex, nsAString& aValue) {
>>>>>>> upstream-releases
  return DoGetBlobAsString(this, aIndex, aValue);
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetBlobAsUTF8String(uint32_t aIndex, nsACString &aValue) {
||||||| merged common ancestors
ArgValueArray::GetBlobAsUTF8String(uint32_t aIndex, nsACString& aValue)
{
=======
ArgValueArray::GetBlobAsUTF8String(uint32_t aIndex, nsACString& aValue) {
>>>>>>> upstream-releases
  return DoGetBlobAsString(this, aIndex, aValue);
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetIsNull(uint32_t aIndex, bool *_isNull) {
||||||| merged common ancestors
ArgValueArray::GetIsNull(uint32_t aIndex,
                         bool *_isNull)
{
=======
ArgValueArray::GetIsNull(uint32_t aIndex, bool* _isNull) {
>>>>>>> upstream-releases
  // GetTypeOfIndex will check aIndex for us, so we don't have to.
  int32_t type;
  nsresult rv = GetTypeOfIndex(aIndex, &type);
  NS_ENSURE_SUCCESS(rv, rv);

  *_isNull = (type == VALUE_TYPE_NULL);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetSharedUTF8String(uint32_t aIndex, uint32_t *_length,
                                   const char **_string) {
  if (_length) *_length = ::sqlite3_value_bytes(mArgv[aIndex]);

  *_string =
      reinterpret_cast<const char *>(::sqlite3_value_text(mArgv[aIndex]));
||||||| merged common ancestors
ArgValueArray::GetSharedUTF8String(uint32_t aIndex,
                                   uint32_t *_length,
                                   const char **_string)
{
  if (_length)
    *_length = ::sqlite3_value_bytes(mArgv[aIndex]);

  *_string = reinterpret_cast<const char *>(::sqlite3_value_text(mArgv[aIndex]));
=======
ArgValueArray::GetSharedUTF8String(uint32_t aIndex, uint32_t* _byteLength,
                                   const char** _string) {
  *_string = reinterpret_cast<const char*>(::sqlite3_value_text(mArgv[aIndex]));
  if (_byteLength) {
    *_byteLength = ::sqlite3_value_bytes(mArgv[aIndex]);
  }
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetSharedString(uint32_t aIndex, uint32_t *_length,
                               const char16_t **_string) {
  if (_length) *_length = ::sqlite3_value_bytes(mArgv[aIndex]);

  *_string =
      static_cast<const char16_t *>(::sqlite3_value_text16(mArgv[aIndex]));
||||||| merged common ancestors
ArgValueArray::GetSharedString(uint32_t aIndex,
                               uint32_t *_length,
                               const char16_t **_string)
{
  if (_length)
    *_length = ::sqlite3_value_bytes(mArgv[aIndex]);

  *_string = static_cast<const char16_t *>(::sqlite3_value_text16(mArgv[aIndex]));
=======
ArgValueArray::GetSharedString(uint32_t aIndex, uint32_t* _byteLength,
                               const char16_t** _string) {
  *_string =
      static_cast<const char16_t*>(::sqlite3_value_text16(mArgv[aIndex]));
  if (_byteLength) {
    *_byteLength = ::sqlite3_value_bytes16(mArgv[aIndex]);
  }
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
ArgValueArray::GetSharedBlob(uint32_t aIndex, uint32_t *_size,
                             const uint8_t **_blob) {
  *_size = ::sqlite3_value_bytes(mArgv[aIndex]);
  *_blob = static_cast<const uint8_t *>(::sqlite3_value_blob(mArgv[aIndex]));
||||||| merged common ancestors
ArgValueArray::GetSharedBlob(uint32_t aIndex,
                             uint32_t *_size,
                             const uint8_t **_blob)
{
  *_size = ::sqlite3_value_bytes(mArgv[aIndex]);
  *_blob = static_cast<const uint8_t *>(::sqlite3_value_blob(mArgv[aIndex]));
=======
ArgValueArray::GetSharedBlob(uint32_t aIndex, uint32_t* _byteLength,
                             const uint8_t** _blob) {
  *_blob = static_cast<const uint8_t*>(::sqlite3_value_blob(mArgv[aIndex]));
  if (_byteLength) {
    *_byteLength = ::sqlite3_value_bytes(mArgv[aIndex]);
  }
>>>>>>> upstream-releases
  return NS_OK;
}

}  // namespace storage
}  // namespace mozilla
