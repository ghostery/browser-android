/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=2 et lcs=trail\:.,tab\:>~ :
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_storage_Variant_h__
#define mozilla_storage_Variant_h__

#include <utility>

#include "nsIVariant.h"
#include "nsMemory.h"
#include "nsString.h"
#include "nsTArray.h"

#define VARIANT_BASE_IID                             \
  { /* 78888042-0fa3-4f7a-8b19-7996f99bf1aa */       \
    0x78888042, 0x0fa3, 0x4f7a, {                    \
      0x8b, 0x19, 0x79, 0x96, 0xf9, 0x9b, 0xf1, 0xaa \
    }                                                \
  }

/**
 * This class is used by the storage module whenever an nsIVariant needs to be
 * returned.  We provide traits for the basic sqlite types to make use easier.
 * The following types map to the indicated sqlite type:
 * int64_t   -> INTEGER (use IntegerVariant)
 * double    -> FLOAT (use FloatVariant)
 * nsString  -> TEXT (use TextVariant)
 * nsCString -> TEXT (use UTF8TextVariant)
 * uint8_t[] -> BLOB (use BlobVariant)
 * nullptr   -> NULL (use NullVariant)
 *
 * The kvstore component also reuses this class as a common implementation
 * of a simple threadsafe variant for the storage of primitive values only.
 * The BooleanVariant type has been introduced for kvstore use cases and should
 * be enhanced to provide full boolean variant support for mozStorage.
 *
 * Bug 1494102 tracks that work.
 */

namespace mozilla {
namespace storage {

////////////////////////////////////////////////////////////////////////////////
//// Base Class

class Variant_base : public nsIVariant {
 public:
  NS_DECL_THREADSAFE_ISUPPORTS
  NS_DECL_NSIVARIANT
  NS_DECLARE_STATIC_IID_ACCESSOR(VARIANT_BASE_IID)

 protected:
  virtual ~Variant_base() {}
};

NS_DEFINE_STATIC_IID_ACCESSOR(Variant_base, VARIANT_BASE_IID)

////////////////////////////////////////////////////////////////////////////////
//// Traits

/**
 * Generics
 */

template <typename DataType>
struct variant_traits {
  static inline uint16_t type() { return nsIDataType::VTYPE_EMPTY; }
};

template <typename DataType, bool Adopting = false>
struct variant_storage_traits {
  typedef DataType ConstructorType;
  typedef DataType StorageType;
<<<<<<< HEAD
  static inline void storage_conversion(const ConstructorType aData,
                                        StorageType *_storage) {
||||||| merged common ancestors
  static inline void storage_conversion(const ConstructorType aData, StorageType* _storage)
  {
=======
  static inline void storage_conversion(const ConstructorType aData,
                                        StorageType* _storage) {
>>>>>>> upstream-releases
    *_storage = aData;
  }

<<<<<<< HEAD
  static inline void destroy(const StorageType &_storage) {}
||||||| merged common ancestors
  static inline void destroy(const StorageType& _storage)
  { }
=======
  static inline void destroy(const StorageType& _storage) {}
>>>>>>> upstream-releases
};

#define NO_CONVERSION return NS_ERROR_CANNOT_CONVERT_DATA;

<<<<<<< HEAD
template <typename DataType, bool Adopting = false>
struct variant_integer_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asInt32(const StorageType &, int32_t *) {
    NO_CONVERSION
  }
  static inline nsresult asInt64(const StorageType &, int64_t *) {
    NO_CONVERSION
  }
||||||| merged common ancestors
template <typename DataType, bool Adopting=false>
struct variant_integer_traits
{
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType StorageType;
  static inline nsresult asInt32(const StorageType &, int32_t *) { NO_CONVERSION }
  static inline nsresult asInt64(const StorageType &, int64_t *) { NO_CONVERSION }
=======
template <typename DataType, bool Adopting = false>
struct variant_boolean_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asBool(const StorageType&, bool*) { NO_CONVERSION }
};

template <typename DataType, bool Adopting = false>
struct variant_integer_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asInt32(const StorageType&, int32_t*) { NO_CONVERSION }
  static inline nsresult asInt64(const StorageType&, int64_t*) { NO_CONVERSION }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
template <typename DataType, bool Adopting = false>
struct variant_float_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asDouble(const StorageType &, double *) {
    NO_CONVERSION
  }
||||||| merged common ancestors
template <typename DataType, bool Adopting=false>
struct variant_float_traits
{
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType StorageType;
  static inline nsresult asDouble(const StorageType &, double *) { NO_CONVERSION }
=======
template <typename DataType, bool Adopting = false>
struct variant_float_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asDouble(const StorageType&, double*) { NO_CONVERSION }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
template <typename DataType, bool Adopting = false>
struct variant_text_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asUTF8String(const StorageType &, nsACString &) {
    NO_CONVERSION
  }
  static inline nsresult asString(const StorageType &, nsAString &) {
    NO_CONVERSION
  }
||||||| merged common ancestors
template <typename DataType, bool Adopting=false>
struct variant_text_traits
{
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType StorageType;
  static inline nsresult asUTF8String(const StorageType &, nsACString &) { NO_CONVERSION }
  static inline nsresult asString(const StorageType &, nsAString &) { NO_CONVERSION }
=======
template <typename DataType, bool Adopting = false>
struct variant_text_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asUTF8String(const StorageType&, nsACString&) {
    NO_CONVERSION
  }
  static inline nsresult asString(const StorageType&, nsAString&) {
    NO_CONVERSION
  }
>>>>>>> upstream-releases
};

<<<<<<< HEAD
template <typename DataType, bool Adopting = false>
struct variant_blob_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asArray(const StorageType &, uint16_t *, uint32_t *,
                                 void **) {
    NO_CONVERSION
  }
||||||| merged common ancestors
template <typename DataType, bool Adopting=false>
struct variant_blob_traits
{
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType StorageType;
  static inline nsresult asArray(const StorageType &, uint16_t *, uint32_t *, void **)
  { NO_CONVERSION }
=======
template <typename DataType, bool Adopting = false>
struct variant_blob_traits {
  typedef typename variant_storage_traits<DataType, Adopting>::StorageType
      StorageType;
  static inline nsresult asArray(const StorageType&, uint16_t*, uint32_t*,
                                 void**) {
    NO_CONVERSION
  }
>>>>>>> upstream-releases
};

#undef NO_CONVERSION

/**
 * BOOLEAN type
 */

template <>
struct variant_traits<bool> {
  static inline uint16_t type() { return nsIDataType::VTYPE_BOOL; }
};
template <>
struct variant_boolean_traits<bool> {
  static inline nsresult asBool(bool aValue, bool* _result) {
    *_result = aValue;
    return NS_OK;
  }

  // NB: It might be worth also providing conversions to int types.

  // NB: It'd be nice to implement asBool conversions for 0 and 1, too.
  // That would let us clean up some conversions in Places, such as:
  // https://searchfox.org/mozilla-central/rev/0640ea80fbc8d48f8b197cd363e2535c95a15eb3/toolkit/components/places/SQLFunctions.cpp#564-565
  // https://searchfox.org/mozilla-central/rev/0640ea80fbc8d48f8b197cd363e2535c95a15eb3/toolkit/components/places/SQLFunctions.cpp#1057
  // https://searchfox.org/mozilla-central/rev/0640ea80fbc8d48f8b197cd363e2535c95a15eb3/toolkit/components/places/nsNavHistory.cpp#3189
};

/**
 * INTEGER types
 */

template <>
struct variant_traits<int64_t> {
  static inline uint16_t type() { return nsIDataType::VTYPE_INT64; }
};
<<<<<<< HEAD
template <>
struct variant_integer_traits<int64_t> {
  static inline nsresult asInt32(int64_t aValue, int32_t *_result) {
||||||| merged common ancestors
template < >
struct variant_integer_traits<int64_t>
{
  static inline nsresult asInt32(int64_t aValue,
                                 int32_t *_result)
  {
=======
template <>
struct variant_integer_traits<int64_t> {
  static inline nsresult asInt32(int64_t aValue, int32_t* _result) {
>>>>>>> upstream-releases
    if (aValue > INT32_MAX || aValue < INT32_MIN)
      return NS_ERROR_CANNOT_CONVERT_DATA;

    *_result = static_cast<int32_t>(aValue);
    return NS_OK;
  }
<<<<<<< HEAD
  static inline nsresult asInt64(int64_t aValue, int64_t *_result) {
||||||| merged common ancestors
  static inline nsresult asInt64(int64_t aValue,
                                 int64_t *_result)
  {
=======
  static inline nsresult asInt64(int64_t aValue, int64_t* _result) {
>>>>>>> upstream-releases
    *_result = aValue;
    return NS_OK;
  }
};
// xpcvariant just calls get double for integers...
<<<<<<< HEAD
template <>
struct variant_float_traits<int64_t> {
  static inline nsresult asDouble(int64_t aValue, double *_result) {
||||||| merged common ancestors
template < >
struct variant_float_traits<int64_t>
{
  static inline nsresult asDouble(int64_t aValue,
                                  double *_result)
  {
=======
template <>
struct variant_float_traits<int64_t> {
  static inline nsresult asDouble(int64_t aValue, double* _result) {
>>>>>>> upstream-releases
    *_result = double(aValue);
    return NS_OK;
  }
};

/**
 * FLOAT types
 */

template <>
struct variant_traits<double> {
  static inline uint16_t type() { return nsIDataType::VTYPE_DOUBLE; }
};
<<<<<<< HEAD
template <>
struct variant_float_traits<double> {
  static inline nsresult asDouble(double aValue, double *_result) {
||||||| merged common ancestors
template < >
struct variant_float_traits<double>
{
  static inline nsresult asDouble(double aValue,
                                  double *_result)
  {
=======
template <>
struct variant_float_traits<double> {
  static inline nsresult asDouble(double aValue, double* _result) {
>>>>>>> upstream-releases
    *_result = aValue;
    return NS_OK;
  }
};

/**
 * TEXT types
 */

template <>
struct variant_traits<nsString> {
  static inline uint16_t type() { return nsIDataType::VTYPE_ASTRING; }
};
<<<<<<< HEAD
template <>
struct variant_storage_traits<nsString> {
  typedef const nsAString &ConstructorType;
||||||| merged common ancestors
template < >
struct variant_storage_traits<nsString>
{
  typedef const nsAString & ConstructorType;
=======
template <>
struct variant_storage_traits<nsString> {
  typedef const nsAString& ConstructorType;
>>>>>>> upstream-releases
  typedef nsString StorageType;
<<<<<<< HEAD
  static inline void storage_conversion(ConstructorType aText,
                                        StorageType *_outData) {
||||||| merged common ancestors
  static inline void storage_conversion(ConstructorType aText, StorageType* _outData)
  {
=======
  static inline void storage_conversion(ConstructorType aText,
                                        StorageType* _outData) {
>>>>>>> upstream-releases
    *_outData = aText;
  }
<<<<<<< HEAD
  static inline void destroy(const StorageType &_outData) {}
||||||| merged common ancestors
  static inline void destroy(const StorageType& _outData)
  { }
=======
  static inline void destroy(const StorageType& _outData) {}
>>>>>>> upstream-releases
};
<<<<<<< HEAD
template <>
struct variant_text_traits<nsString> {
  static inline nsresult asUTF8String(const nsString &aValue,
                                      nsACString &_result) {
||||||| merged common ancestors
template < >
struct variant_text_traits<nsString>
{
  static inline nsresult asUTF8String(const nsString &aValue,
                                      nsACString &_result)
  {
=======
template <>
struct variant_text_traits<nsString> {
  static inline nsresult asUTF8String(const nsString& aValue,
                                      nsACString& _result) {
>>>>>>> upstream-releases
    CopyUTF16toUTF8(aValue, _result);
    return NS_OK;
  }
<<<<<<< HEAD
  static inline nsresult asString(const nsString &aValue, nsAString &_result) {
||||||| merged common ancestors
  static inline nsresult asString(const nsString &aValue,
                                  nsAString &_result)
  {
=======
  static inline nsresult asString(const nsString& aValue, nsAString& _result) {
>>>>>>> upstream-releases
    _result = aValue;
    return NS_OK;
  }
};

template <>
struct variant_traits<nsCString> {
  static inline uint16_t type() { return nsIDataType::VTYPE_UTF8STRING; }
};
<<<<<<< HEAD
template <>
struct variant_storage_traits<nsCString> {
  typedef const nsACString &ConstructorType;
||||||| merged common ancestors
template < >
struct variant_storage_traits<nsCString>
{
  typedef const nsACString & ConstructorType;
=======
template <>
struct variant_storage_traits<nsCString> {
  typedef const nsACString& ConstructorType;
>>>>>>> upstream-releases
  typedef nsCString StorageType;
<<<<<<< HEAD
  static inline void storage_conversion(ConstructorType aText,
                                        StorageType *_outData) {
||||||| merged common ancestors
  static inline void storage_conversion(ConstructorType aText, StorageType* _outData)
  {
=======
  static inline void storage_conversion(ConstructorType aText,
                                        StorageType* _outData) {
>>>>>>> upstream-releases
    *_outData = aText;
  }
<<<<<<< HEAD
  static inline void destroy(const StorageType &aData) {}
||||||| merged common ancestors
  static inline void destroy(const StorageType &aData)
  { }
=======
  static inline void destroy(const StorageType& aData) {}
>>>>>>> upstream-releases
};
<<<<<<< HEAD
template <>
struct variant_text_traits<nsCString> {
  static inline nsresult asUTF8String(const nsCString &aValue,
                                      nsACString &_result) {
||||||| merged common ancestors
template < >
struct variant_text_traits<nsCString>
{
  static inline nsresult asUTF8String(const nsCString &aValue,
                                      nsACString &_result)
  {
=======
template <>
struct variant_text_traits<nsCString> {
  static inline nsresult asUTF8String(const nsCString& aValue,
                                      nsACString& _result) {
>>>>>>> upstream-releases
    _result = aValue;
    return NS_OK;
  }
<<<<<<< HEAD
  static inline nsresult asString(const nsCString &aValue, nsAString &_result) {
||||||| merged common ancestors
  static inline nsresult asString(const nsCString &aValue,
                                  nsAString &_result)
  {
=======
  static inline nsresult asString(const nsCString& aValue, nsAString& _result) {
>>>>>>> upstream-releases
    CopyUTF8toUTF16(aValue, _result);
    return NS_OK;
  }
};

/**
 * BLOB types
 */

template <>
struct variant_traits<uint8_t[]> {
  static inline uint16_t type() { return nsIDataType::VTYPE_ARRAY; }
};
<<<<<<< HEAD
template <>
struct variant_storage_traits<uint8_t[], false> {
  typedef std::pair<const void *, int> ConstructorType;
||||||| merged common ancestors
template < >
struct variant_storage_traits<uint8_t[], false>
{
  typedef std::pair<const void *, int> ConstructorType;
=======
template <>
struct variant_storage_traits<uint8_t[], false> {
  typedef std::pair<const void*, int> ConstructorType;
>>>>>>> upstream-releases
  typedef FallibleTArray<uint8_t> StorageType;
<<<<<<< HEAD
  static inline void storage_conversion(ConstructorType aBlob,
                                        StorageType *_outData) {
||||||| merged common ancestors
  static inline void storage_conversion(ConstructorType aBlob, StorageType* _outData)
  {
=======
  static inline void storage_conversion(ConstructorType aBlob,
                                        StorageType* _outData) {
>>>>>>> upstream-releases
    _outData->Clear();
    (void)_outData->AppendElements(static_cast<const uint8_t*>(aBlob.first),
                                   aBlob.second, fallible);
  }
<<<<<<< HEAD
  static inline void destroy(const StorageType &_outData) {}
||||||| merged common ancestors
  static inline void destroy(const StorageType& _outData)
  { }
=======
  static inline void destroy(const StorageType& _outData) {}
>>>>>>> upstream-releases
};
<<<<<<< HEAD
template <>
struct variant_storage_traits<uint8_t[], true> {
  typedef std::pair<uint8_t *, int> ConstructorType;
  typedef std::pair<uint8_t *, int> StorageType;
  static inline void storage_conversion(ConstructorType aBlob,
                                        StorageType *_outData) {
||||||| merged common ancestors
template < >
struct variant_storage_traits<uint8_t[], true>
{
  typedef std::pair<uint8_t *, int> ConstructorType;
  typedef std::pair<uint8_t *, int> StorageType;
  static inline void storage_conversion(ConstructorType aBlob, StorageType* _outData)
  {
=======
template <>
struct variant_storage_traits<uint8_t[], true> {
  typedef std::pair<uint8_t*, int> ConstructorType;
  typedef std::pair<uint8_t*, int> StorageType;
  static inline void storage_conversion(ConstructorType aBlob,
                                        StorageType* _outData) {
>>>>>>> upstream-releases
    *_outData = aBlob;
  }
<<<<<<< HEAD
  static inline void destroy(StorageType &aData) {
||||||| merged common ancestors
  static inline void destroy(StorageType &aData)
  {
=======
  static inline void destroy(StorageType& aData) {
>>>>>>> upstream-releases
    if (aData.first) {
      free(aData.first);
      aData.first = nullptr;
    }
  }
};
<<<<<<< HEAD
template <>
struct variant_blob_traits<uint8_t[], false> {
  static inline nsresult asArray(FallibleTArray<uint8_t> &aData,
                                 uint16_t *_type, uint32_t *_size,
                                 void **_result) {
||||||| merged common ancestors
template < >
struct variant_blob_traits<uint8_t[], false>
{
  static inline nsresult asArray(FallibleTArray<uint8_t> &aData,
                                 uint16_t *_type,
                                 uint32_t *_size,
                                 void **_result)
  {
=======
template <>
struct variant_blob_traits<uint8_t[], false> {
  static inline nsresult asArray(FallibleTArray<uint8_t>& aData,
                                 uint16_t* _type, uint32_t* _size,
                                 void** _result) {
>>>>>>> upstream-releases
    // For empty blobs, we return nullptr.
    if (aData.Length() == 0) {
      *_result = nullptr;
      *_type = nsIDataType::VTYPE_UINT8;
      *_size = 0;
      return NS_OK;
    }

    // Otherwise, we copy the array.
    *_result = moz_xmemdup(aData.Elements(), aData.Length() * sizeof(uint8_t));

    // Set type and size
    *_type = nsIDataType::VTYPE_UINT8;
    *_size = aData.Length();
    return NS_OK;
  }
};

<<<<<<< HEAD
template <>
struct variant_blob_traits<uint8_t[], true> {
  static inline nsresult asArray(std::pair<uint8_t *, int> &aData,
                                 uint16_t *_type, uint32_t *_size,
                                 void **_result) {
||||||| merged common ancestors
template < >
struct variant_blob_traits<uint8_t[], true>
{
  static inline nsresult asArray(std::pair<uint8_t *, int> &aData,
                                 uint16_t *_type,
                                 uint32_t *_size,
                                 void **_result)
  {
=======
template <>
struct variant_blob_traits<uint8_t[], true> {
  static inline nsresult asArray(std::pair<uint8_t*, int>& aData,
                                 uint16_t* _type, uint32_t* _size,
                                 void** _result) {
>>>>>>> upstream-releases
    // For empty blobs, we return nullptr.
    if (aData.second == 0) {
      *_result = nullptr;
      *_type = nsIDataType::VTYPE_UINT8;
      *_size = 0;
      return NS_OK;
    }

    // Otherwise, transfer the data out.
    *_result = aData.first;
    aData.first = nullptr;
    MOZ_ASSERT(*_result);  // We asked for it twice, better not use adopting!

    // Set type and size
    *_type = nsIDataType::VTYPE_UINT8;
    *_size = aData.second;
    return NS_OK;
  }
};

/**
 * nullptr type
 */

class NullVariant : public Variant_base {
 public:
  uint16_t GetDataType() override { return nsIDataType::VTYPE_EMPTY; }

<<<<<<< HEAD
  NS_IMETHOD GetAsAUTF8String(nsACString &_str) override {
||||||| merged common ancestors
  NS_IMETHOD GetAsAUTF8String(nsACString &_str) override
  {
=======
  NS_IMETHOD GetAsAUTF8String(nsACString& _str) override {
>>>>>>> upstream-releases
    // Return a void string.
    _str.SetIsVoid(true);
    return NS_OK;
  }

<<<<<<< HEAD
  NS_IMETHOD GetAsAString(nsAString &_str) override {
||||||| merged common ancestors
  NS_IMETHOD GetAsAString(nsAString &_str) override
  {
=======
  NS_IMETHOD GetAsAString(nsAString& _str) override {
>>>>>>> upstream-releases
    // Return a void string.
    _str.SetIsVoid(true);
    return NS_OK;
  }
};

////////////////////////////////////////////////////////////////////////////////
//// Template Implementation

template <typename DataType, bool Adopting = false>
class Variant final : public Variant_base {
  ~Variant() { variant_storage_traits<DataType, Adopting>::destroy(mData); }

 public:
  explicit Variant(
      const typename variant_storage_traits<DataType, Adopting>::ConstructorType
          aData) {
    variant_storage_traits<DataType, Adopting>::storage_conversion(aData,
                                                                   &mData);
  }

<<<<<<< HEAD
  uint16_t GetDataType() override { return variant_traits<DataType>::type(); }
  NS_IMETHOD GetAsInt32(int32_t *_integer) override {
||||||| merged common ancestors
  NS_IMETHOD GetDataType(uint16_t *_type) override
  {
    *_type = variant_traits<DataType>::type();
    return NS_OK;
  }
  NS_IMETHOD GetAsInt32(int32_t *_integer) override
  {
=======
  uint16_t GetDataType() override { return variant_traits<DataType>::type(); }

  NS_IMETHOD GetAsBool(bool* _boolean) override {
    return variant_boolean_traits<DataType, Adopting>::asBool(mData, _boolean);
  }

  NS_IMETHOD GetAsInt32(int32_t* _integer) override {
>>>>>>> upstream-releases
    return variant_integer_traits<DataType, Adopting>::asInt32(mData, _integer);
  }

<<<<<<< HEAD
  NS_IMETHOD GetAsInt64(int64_t *_integer) override {
||||||| merged common ancestors
  NS_IMETHOD GetAsInt64(int64_t *_integer) override
  {
=======
  NS_IMETHOD GetAsInt64(int64_t* _integer) override {
>>>>>>> upstream-releases
    return variant_integer_traits<DataType, Adopting>::asInt64(mData, _integer);
  }

<<<<<<< HEAD
  NS_IMETHOD GetAsDouble(double *_double) override {
||||||| merged common ancestors
  NS_IMETHOD GetAsDouble(double *_double) override
  {
=======
  NS_IMETHOD GetAsDouble(double* _double) override {
>>>>>>> upstream-releases
    return variant_float_traits<DataType, Adopting>::asDouble(mData, _double);
  }

<<<<<<< HEAD
  NS_IMETHOD GetAsAUTF8String(nsACString &_str) override {
||||||| merged common ancestors
  NS_IMETHOD GetAsAUTF8String(nsACString &_str) override
  {
=======
  NS_IMETHOD GetAsAUTF8String(nsACString& _str) override {
>>>>>>> upstream-releases
    return variant_text_traits<DataType, Adopting>::asUTF8String(mData, _str);
  }

<<<<<<< HEAD
  NS_IMETHOD GetAsAString(nsAString &_str) override {
||||||| merged common ancestors
  NS_IMETHOD GetAsAString(nsAString &_str) override
  {
=======
  NS_IMETHOD GetAsAString(nsAString& _str) override {
>>>>>>> upstream-releases
    return variant_text_traits<DataType, Adopting>::asString(mData, _str);
  }

<<<<<<< HEAD
  NS_IMETHOD GetAsArray(uint16_t *_type, nsIID *, uint32_t *_size,
                        void **_data) override {
    return variant_blob_traits<DataType, Adopting>::asArray(mData, _type, _size,
                                                            _data);
||||||| merged common ancestors
  NS_IMETHOD GetAsArray(uint16_t *_type,
                        nsIID *,
                        uint32_t *_size,
                        void **_data) override
  {
    return variant_blob_traits<DataType, Adopting>::asArray(mData, _type, _size, _data);
=======
  NS_IMETHOD GetAsArray(uint16_t* _type, nsIID*, uint32_t* _size,
                        void** _data) override {
    return variant_blob_traits<DataType, Adopting>::asArray(mData, _type, _size,
                                                            _data);
>>>>>>> upstream-releases
  }

 private:
  typename variant_storage_traits<DataType, Adopting>::StorageType mData;
};

////////////////////////////////////////////////////////////////////////////////
//// Handy typedefs!  Use these for the right mapping.

// Currently, BooleanVariant is only useful for kvstore.
// Bug 1494102 tracks implementing full boolean variant support for mozStorage.
typedef Variant<bool> BooleanVariant;

typedef Variant<int64_t> IntegerVariant;
typedef Variant<double> FloatVariant;
typedef Variant<nsString> TextVariant;
typedef Variant<nsCString> UTF8TextVariant;
typedef Variant<uint8_t[], false> BlobVariant;
typedef Variant<uint8_t[], true> AdoptedBlobVariant;

}  // namespace storage
}  // namespace mozilla

#include "Variant_inl.h"

#endif  // mozilla_storage_Variant_h__
