/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/ClearOnShutdown.h"
#include "mozilla/ScopeExit.h"
#include "mozilla/TextUtils.h"

#include "mozilla/dom/ToJSValue.h"

#include "mprio.h"

#include "PrioEncoder.h"

namespace mozilla {
namespace dom {

/* static */
StaticRefPtr<PrioEncoder> PrioEncoder::sSingleton;

/* static */
PublicKey PrioEncoder::sPublicKeyA = nullptr;
/* static */
PublicKey PrioEncoder::sPublicKeyB = nullptr;

// Production keys from bug 1552315 comment#3
/* static */
const char* kDefaultKeyA =
    "E780C1A9C50E3FC5A9B39469FCC92D62D2527BAE6AF76BBDEF128883FA400846";
/* static */
const char* kDefaultKeyB =
    "F992B575840AEC202289FBF99D6C04FB2A37B1DA1CDEB1DF8036E1340D46C561";

PrioEncoder::PrioEncoder() = default;
PrioEncoder::~PrioEncoder() {
  if (sPublicKeyA) {
    PublicKey_clear(sPublicKeyA);
    sPublicKeyA = nullptr;
  }

  if (sPublicKeyB) {
    PublicKey_clear(sPublicKeyB);
    sPublicKeyB = nullptr;
  }

  Prio_clear();
}

<<<<<<< HEAD
/* static */ void PrioEncoder::Encode(GlobalObject& aGlobal,
                                      const nsCString& aBatchID,
                                      const PrioParams& aPrioParams,
                                      RootedDictionary<PrioEncodedData>& aData,
                                      ErrorResult& aRv) {
||||||| merged common ancestors
/* static */ void
PrioEncoder::Encode(GlobalObject& aGlobal,
                    const nsCString& aBatchID,
                    const PrioParams& aPrioParams,
                    RootedDictionary<PrioEncodedData>& aData,
                    ErrorResult& aRv)
{
=======
/* static */
void PrioEncoder::Encode(GlobalObject& aGlobal, const nsCString& aBatchID,
                         const PrioParams& aPrioParams,
                         RootedDictionary<PrioEncodedData>& aData,
                         ErrorResult& aRv) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIGlobalObject> global = do_QueryInterface(aGlobal.GetAsSupports());
  if (!global) {
    aRv.Throw(NS_ERROR_UNEXPECTED);
    return;
  }

  nsCString aResult;
  nsCString bResult;
  nsresult rv = PrioEncoder::EncodeNative(aBatchID, aPrioParams.mBooleans,
                                          aResult, bResult);
  if (NS_FAILED(rv)) {
    aRv.Throw(rv);
    return;
  }

  nsTArray<uint8_t> arrayForServerA;
  nsTArray<uint8_t> arrayForServerB;

  if (!arrayForServerA.AppendElements(
          reinterpret_cast<const uint8_t*>(aResult.BeginReading()),
          aResult.Length(), fallible)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
  }

  if (!arrayForServerB.AppendElements(
          reinterpret_cast<const uint8_t*>(bResult.BeginReading()),
          bResult.Length(), fallible)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
  }

<<<<<<< HEAD
    // Check that both public keys are of the right length
    // and contain only hex digits 0-9a-fA-f
    if (!PrioEncoder::IsValidHexPublicKey(prioKeyA) ||
        !PrioEncoder::IsValidHexPublicKey(prioKeyB)) {
      aRv.Throw(NS_ERROR_UNEXPECTED);
      return;
    }
||||||| merged common ancestors
    // Check that both public keys are of the right length
    // and contain only hex digits 0-9a-fA-f
    if (!PrioEncoder::IsValidHexPublicKey(prioKeyA)
        || !PrioEncoder::IsValidHexPublicKey(prioKeyB))  {
      aRv.Throw(NS_ERROR_UNEXPECTED);
      return;
    }
=======
  JS::Rooted<JS::Value> valueA(aGlobal.Context());
  if (!ToJSValue(aGlobal.Context(),
                 TypedArrayCreator<Uint8Array>(arrayForServerA), &valueA)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
  }
>>>>>>> upstream-releases

  aData.mA.Construct().Init(&valueA.toObject());

  JS::Rooted<JS::Value> valueB(aGlobal.Context());
  if (!ToJSValue(aGlobal.Context(),
                 TypedArrayCreator<Uint8Array>(arrayForServerB), &valueB)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
  }

<<<<<<< HEAD
    prio_rv = PublicKey_import_hex(
        &sPublicKeyA,
        reinterpret_cast<const unsigned char*>(prioKeyA.BeginReading()),
        CURVE25519_KEY_LEN_HEX);
    if (prio_rv != SECSuccess) {
      aRv.Throw(NS_ERROR_UNEXPECTED);
      return;
    }
||||||| merged common ancestors
    prio_rv = PublicKey_import_hex(&sPublicKeyA,
                                   reinterpret_cast<const unsigned char*>(prioKeyA.BeginReading()),
                                   CURVE25519_KEY_LEN_HEX);
    if (prio_rv != SECSuccess) {
      aRv.Throw(NS_ERROR_UNEXPECTED);
      return;
    }
=======
  aData.mB.Construct().Init(&valueB.toObject());
}
>>>>>>> upstream-releases

<<<<<<< HEAD
    prio_rv = PublicKey_import_hex(
        &sPublicKeyB,
        reinterpret_cast<const unsigned char*>(prioKeyB.BeginReading()),
        CURVE25519_KEY_LEN_HEX);
    if (prio_rv != SECSuccess) {
      aRv.Throw(NS_ERROR_UNEXPECTED);
      return;
    }
||||||| merged common ancestors
    prio_rv = PublicKey_import_hex(&sPublicKeyB,
              reinterpret_cast<const unsigned char*>(prioKeyB.BeginReading()),
              CURVE25519_KEY_LEN_HEX);
    if (prio_rv != SECSuccess) {
      aRv.Throw(NS_ERROR_UNEXPECTED);
      return;
    }
=======
/* static */
nsresult PrioEncoder::EncodeNative(const nsCString& aBatchID,
                                   const nsTArray<bool>& aData,
                                   nsCString& aResult, nsCString& bResult) {
  SECStatus prio_rv = SECSuccess;
>>>>>>> upstream-releases

  nsresult rv = PrioEncoder::LazyInitSingleton();
  if (NS_FAILED(rv)) {
    return rv;
  }

<<<<<<< HEAD
  nsTArray<bool> dataItems = aPrioParams.mBooleans;
  if (dataItems.Length() > gNumBooleans) {
    aRv.ThrowRangeError<MSG_VALUE_OUT_OF_RANGE>(
        NS_LITERAL_STRING("Maximum boolean value exceeded"));
    return;
  }
||||||| merged common ancestors
  bool dataItems[] = {
    aPrioParams.mBrowserIsUserDefault,
    aPrioParams.mNewTabPageEnabled,
    aPrioParams.mPdfViewerUsed,
  };
=======
  if (aData.Length() > gNumBooleans) {
    return NS_ERROR_RANGE_ERR;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  PrioConfig prioConfig = PrioConfig_new(
      dataItems.Length(), sPublicKeyA, sPublicKeyB,
      reinterpret_cast<const unsigned char*>(aBatchID.BeginReading()),
      aBatchID.Length());
||||||| merged common ancestors
  PrioConfig prioConfig = PrioConfig_new(mozilla::ArrayLength(dataItems),
                                         sPublicKeyA,
                                         sPublicKeyB,
                                         reinterpret_cast<const unsigned char*>(aBatchID.BeginReading()),
                                         aBatchID.Length());
=======
  PrioConfig prioConfig = PrioConfig_new(
      aData.Length(), sPublicKeyA, sPublicKeyB,
      reinterpret_cast<const unsigned char*>(aBatchID.BeginReading()),
      aBatchID.Length());
>>>>>>> upstream-releases

  if (!prioConfig) {
    return NS_ERROR_FAILURE;
  }

  auto configGuard = MakeScopeExit([&] { PrioConfig_clear(prioConfig); });

  unsigned char* forServerA = nullptr;
  unsigned int lenA = 0;
  unsigned char* forServerB = nullptr;
  unsigned int lenB = 0;

<<<<<<< HEAD
  prio_rv = PrioClient_encode(prioConfig, dataItems.Elements(), &forServerA,
                              &lenA, &forServerB, &lenB);
||||||| merged common ancestors
  prio_rv = PrioClient_encode(prioConfig,
                              dataItems,
                              &forServerA,
                              &lenA,
                              &forServerB,
                              &lenB);
=======
  prio_rv = PrioClient_encode(prioConfig, aData.Elements(), &forServerA, &lenA,
                              &forServerB, &lenB);
>>>>>>> upstream-releases

  aResult.Adopt(reinterpret_cast<char*>(forServerA), lenA);
  bResult.Adopt(reinterpret_cast<char*>(forServerB), lenB);

<<<<<<< HEAD
  if (!arrayForServerA.AppendElements(reinterpret_cast<uint8_t*>(forServerA),
                                      lenA, fallible)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
||||||| merged common ancestors
  if (!arrayForServerA.AppendElements(reinterpret_cast<uint8_t*>(forServerA),
                                      lenA,
                                      fallible)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
=======
  if (prio_rv != SECSuccess) {
    return NS_ERROR_FAILURE;
>>>>>>> upstream-releases
  }
  return NS_OK;
}

bool PrioEncoder::IsValidHexPublicKey(mozilla::Span<const char> aStr) {
  if (aStr.Length() != CURVE25519_KEY_LEN_HEX) {
    return false;
  }

<<<<<<< HEAD
  if (!arrayForServerB.AppendElements(reinterpret_cast<uint8_t*>(forServerB),
                                      lenB, fallible)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
||||||| merged common ancestors
  if (!arrayForServerB.AppendElements(reinterpret_cast<uint8_t*>(forServerB),
                                      lenB,
                                      fallible)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return ;
=======
  for (auto c : aStr) {
    if (!IsAsciiHexDigit(c)) {
      return false;
    }
>>>>>>> upstream-releases
  }

  return true;
}

/* static */
nsresult PrioEncoder::SetKeys(const char* aKeyA, const char* aKeyB) {
  nsAutoCStringN<CURVE25519_KEY_LEN_HEX + 1> prioKeyA;
  if (aKeyA == nullptr) {
    prioKeyA = kDefaultKeyA;
  } else {
    prioKeyA = aKeyA;
  }

<<<<<<< HEAD
  JS::Rooted<JS::Value> valueA(aGlobal.Context());
  if (!ToJSValue(aGlobal.Context(),
                 TypedArrayCreator<Uint8Array>(arrayForServerA), &valueA)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
||||||| merged common ancestors
  JS::Rooted<JS::Value> valueA(aGlobal.Context());
  if (!ToJSValue(aGlobal.Context(), TypedArrayCreator<Uint8Array>(arrayForServerA), &valueA)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
=======
  nsAutoCStringN<CURVE25519_KEY_LEN_HEX + 1> prioKeyB;
  if (aKeyB == nullptr) {
    prioKeyB = kDefaultKeyB;
  } else {
    prioKeyB = aKeyB;
>>>>>>> upstream-releases
  }

  // Check that both public keys are of the right length
  // and contain only hex digits 0-9a-fA-f
  if (!PrioEncoder::IsValidHexPublicKey(prioKeyA) ||
      !PrioEncoder::IsValidHexPublicKey(prioKeyB)) {
    return NS_ERROR_UNEXPECTED;
  }

<<<<<<< HEAD
  JS::Rooted<JS::Value> valueB(aGlobal.Context());
  if (!ToJSValue(aGlobal.Context(),
                 TypedArrayCreator<Uint8Array>(arrayForServerB), &valueB)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
||||||| merged common ancestors
  JS::Rooted<JS::Value> valueB(aGlobal.Context());
  if (!ToJSValue(aGlobal.Context(), TypedArrayCreator<Uint8Array>(arrayForServerB), &valueB)) {
    aRv.Throw(NS_ERROR_OUT_OF_MEMORY);
    return;
=======
  SECStatus prio_rv = SECSuccess;
  prio_rv = Prio_init();

  if (prio_rv != SECSuccess) {
    return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases
  }

  prio_rv = PublicKey_import_hex(
      &sPublicKeyA,
      reinterpret_cast<const unsigned char*>(prioKeyA.BeginReading()),
      CURVE25519_KEY_LEN_HEX);
  if (prio_rv != SECSuccess) {
    return NS_ERROR_UNEXPECTED;
  }

<<<<<<< HEAD
bool PrioEncoder::IsValidHexPublicKey(mozilla::Span<const char> aStr) {
  if (aStr.Length() != CURVE25519_KEY_LEN_HEX) {
    return false;
||||||| merged common ancestors
bool
PrioEncoder::IsValidHexPublicKey(mozilla::Span<const char> aStr)
{
  if (aStr.Length() != CURVE25519_KEY_LEN_HEX) {
    return false;
=======
  prio_rv = PublicKey_import_hex(
      &sPublicKeyB,
      reinterpret_cast<const unsigned char*>(prioKeyB.BeginReading()),
      CURVE25519_KEY_LEN_HEX);
  if (prio_rv != SECSuccess) {
    return NS_ERROR_UNEXPECTED;
>>>>>>> upstream-releases
  }

  return NS_OK;
}

/* static */
nsresult PrioEncoder::LazyInitSingleton() {
  if (!sSingleton) {
    // Init to the default keys.
    nsresult rv = PrioEncoder::SetKeys();
    if (!NS_SUCCEEDED(rv)) {
      return rv;
    }

    sSingleton = new PrioEncoder();
    ClearOnShutdown(&sSingleton);
  }
  return NS_OK;
}

}  // namespace dom
}  // namespace mozilla
