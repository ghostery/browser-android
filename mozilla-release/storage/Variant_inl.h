/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=2 et lcs=trail\:.,tab\:>~ :
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
 * Note: This file is included by Variant.h.
 */

#ifndef mozilla_storage_Variant_h__
#  error "Do not include this file directly!"
#endif

namespace mozilla {
namespace storage {

////////////////////////////////////////////////////////////////////////////////
//// Variant_base

inline NS_IMPL_ADDREF(Variant_base) inline NS_IMPL_RELEASE(
    Variant_base) inline NS_IMPL_QUERY_INTERFACE(Variant_base, nsIVariant)

    ////////////////////////////////////////////////////////////////////////////////
    //// nsIVariant

    inline uint16_t Variant_base::GetDataType() {
  return nsIDataType::VTYPE_VOID;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsInt32(int32_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsInt32(int32_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsInt32(int32_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsInt64(int64_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsInt64(int64_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsInt64(int64_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsDouble(double *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsDouble(double *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsDouble(double*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsAUTF8String(nsACString &) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsAUTF8String(nsACString &)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsAUTF8String(nsACString&) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsAString(nsAString &) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsAString(nsAString &)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsAString(nsAString&) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsArray(uint16_t *, nsIID *, uint32_t *,
                                              void **) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsArray(uint16_t *,
                         nsIID *,
                         uint32_t *,
                         void **)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsArray(uint16_t*, nsIID*, uint32_t*,
                                              void**) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsInt8(uint8_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsInt8(uint8_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsInt8(uint8_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsInt16(int16_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsInt16(int16_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsInt16(int16_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsUint8(uint8_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsUint8(uint8_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsUint8(uint8_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsUint16(uint16_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsUint16(uint16_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsUint16(uint16_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsUint32(uint32_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsUint32(uint32_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsUint32(uint32_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsUint64(uint64_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsUint64(uint64_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsUint64(uint64_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsFloat(float *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsFloat(float *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsFloat(float*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsBool(bool *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsBool(bool *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsBool(bool*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsChar(char *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsChar(char *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsChar(char*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsWChar(char16_t *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsWChar(char16_t *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsWChar(char16_t*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsID(nsID *) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsID(nsID *)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsID(nsID*) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsString(char **) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsString(char **)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsString(char**) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsWString(char16_t **) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsWString(char16_t **)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsWString(char16_t**) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsISupports(nsISupports **) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsISupports(nsISupports **)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsISupports(nsISupports**) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsInterface(nsIID **, void **) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsInterface(nsIID **,
                             void **)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsInterface(nsIID**, void**) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsACString(nsACString &) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsACString(nsACString &)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsACString(nsACString&) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsStringWithSize(uint32_t *, char **) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsStringWithSize(uint32_t *,
                                  char **)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsStringWithSize(uint32_t*, char**) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

<<<<<<< HEAD
inline NS_IMETHODIMP Variant_base::GetAsWStringWithSize(uint32_t *,
                                                        char16_t **) {
||||||| merged common ancestors
inline
NS_IMETHODIMP
Variant_base::GetAsWStringWithSize(uint32_t *,
                                   char16_t **)
{
=======
inline NS_IMETHODIMP Variant_base::GetAsWStringWithSize(uint32_t*, char16_t**) {
>>>>>>> upstream-releases
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

inline NS_IMETHODIMP Variant_base::GetAsJSVal(JS::MutableHandle<JS::Value>) {
  return NS_ERROR_CANNOT_CONVERT_DATA;
}

}  // namespace storage
}  // namespace mozilla
