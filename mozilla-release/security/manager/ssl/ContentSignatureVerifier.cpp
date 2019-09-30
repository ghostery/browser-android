/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "ContentSignatureVerifier.h"

#include "BRNameMatchingPolicy.h"
#include "CryptoTask.h"
#include "ScopedNSSTypes.h"
#include "SharedCertVerifier.h"
#include "cryptohi.h"
#include "keyhi.h"
#include "mozilla/Base64.h"
#include "mozilla/Unused.h"
#include "mozilla/dom/Promise.h"
#include "nsCOMPtr.h"
#include "nsPromiseFlatString.h"
#include "nsProxyRelease.h"
#include "nsSecurityHeaderParser.h"
#include "nsWhitespaceTokenizer.h"
#include "mozpkix/pkix.h"
#include "mozpkix/pkixtypes.h"
#include "secerr.h"

<<<<<<< HEAD
NS_IMPL_ISUPPORTS(ContentSignatureVerifier, nsIContentSignatureVerifier,
                  nsIInterfaceRequestor, nsIStreamListener)
||||||| merged common ancestors
NS_IMPL_ISUPPORTS(ContentSignatureVerifier,
                  nsIContentSignatureVerifier,
                  nsIInterfaceRequestor,
                  nsIStreamListener)
=======
NS_IMPL_ISUPPORTS(ContentSignatureVerifier, nsIContentSignatureVerifier)
>>>>>>> upstream-releases

using namespace mozilla;
using namespace mozilla::pkix;
using namespace mozilla::psm;
using dom::Promise;

static LazyLogModule gCSVerifierPRLog("ContentSignatureVerifier");
#define CSVerifier_LOG(args) MOZ_LOG(gCSVerifierPRLog, LogLevel::Debug, args)

// Content-Signature prefix
const unsigned char kPREFIX[] = {'C', 'o', 'n', 't', 'e', 'n', 't',
                                 '-', 'S', 'i', 'g', 'n', 'a', 't',
                                 'u', 'r', 'e', ':', 0};

class VerifyContentSignatureTask : public CryptoTask {
 public:
  VerifyContentSignatureTask(const nsACString& aData,
                             const nsACString& aCSHeader,
                             const nsACString& aCertChain,
                             const nsACString& aHostname,
                             RefPtr<Promise>& aPromise)
      : mData(aData),
        mCSHeader(aCSHeader),
        mCertChain(aCertChain),
        mHostname(aHostname),
        mSignatureVerified(false),
        mPromise(new nsMainThreadPtrHolder<Promise>(
            "VerifyContentSignatureTask::mPromise", aPromise)) {}

 private:
  virtual nsresult CalculateResult() override;
  virtual void CallCallback(nsresult rv) override;

  nsCString mData;
  nsCString mCSHeader;
  nsCString mCertChain;
  nsCString mHostname;
  bool mSignatureVerified;
  nsMainThreadPtrHandle<Promise> mPromise;
};

NS_IMETHODIMP
<<<<<<< HEAD
ContentSignatureVerifier::VerifyContentSignature(const nsACString& aData,
                                                 const nsACString& aCSHeader,
                                                 const nsACString& aCertChain,
                                                 const nsACString& aName,
                                                 bool* _retval) {
  NS_ENSURE_ARG(_retval);
  nsresult rv = CreateContext(aData, aCSHeader, aCertChain, aName);
||||||| merged common ancestors
ContentSignatureVerifier::VerifyContentSignature(
  const nsACString& aData, const nsACString& aCSHeader,
  const nsACString& aCertChain, const nsACString& aName, bool* _retval)
{
  NS_ENSURE_ARG(_retval);
  nsresult rv = CreateContext(aData, aCSHeader, aCertChain, aName);
=======
ContentSignatureVerifier::AsyncVerifyContentSignature(
    const nsACString& aData, const nsACString& aCSHeader,
    const nsACString& aCertChain, const nsACString& aHostname, JSContext* aCx,
    Promise** aPromise) {
  NS_ENSURE_ARG_POINTER(aCx);

  nsIGlobalObject* globalObject = xpc::CurrentNativeGlobal(aCx);
  if (NS_WARN_IF(!globalObject)) {
    return NS_ERROR_UNEXPECTED;
  }

  ErrorResult result;
  RefPtr<Promise> promise = Promise::Create(globalObject, result);
  if (NS_WARN_IF(result.Failed())) {
    return result.StealNSResult();
  }

  RefPtr<VerifyContentSignatureTask> task(new VerifyContentSignatureTask(
      aData, aCSHeader, aCertChain, aHostname, promise));
  nsresult rv = task->Dispatch();
>>>>>>> upstream-releases
  if (NS_FAILED(rv)) {
    return rv;
  }

  promise.forget(aPromise);
  return NS_OK;
}

static nsresult VerifyContentSignatureInternal(
    const nsACString& aData, const nsACString& aCSHeader,
    const nsACString& aCertChain, const nsACString& aHostname,
    /* out */
    mozilla::Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS&
        aErrorLabel,
    /* out */ nsACString& aCertFingerprint, /* out */ uint32_t& aErrorValue);
static nsresult ParseContentSignatureHeader(
    const nsACString& aContentSignatureHeader,
    /* out */ nsCString& aSignature);

nsresult VerifyContentSignatureTask::CalculateResult() {
  // 3 is the default, non-specific, "something failed" error.
  Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS errorLabel =
      Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err3;
  nsAutoCString certFingerprint;
  uint32_t errorValue = 3;
  nsresult rv =
      VerifyContentSignatureInternal(mData, mCSHeader, mCertChain, mHostname,
                                     errorLabel, certFingerprint, errorValue);
  if (NS_FAILED(rv)) {
    CSVerifier_LOG(("CSVerifier: Signature verification failed"));
    if (certFingerprint.Length() > 0) {
      Telemetry::AccumulateCategoricalKeyed(certFingerprint, errorLabel);
    }
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, errorValue);
    if (rv == NS_ERROR_INVALID_SIGNATURE) {
      return NS_OK;
    }
<<<<<<< HEAD
    // This failure can have many different reasons but we don't treat it as
    // invalid signature.
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 3);
    Telemetry::AccumulateCategoricalKeyed(
        mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err3);
||||||| merged common ancestors
    // This failure can have many different reasons but we don't treat it as
    // invalid signature.
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 3);
    Telemetry::AccumulateCategoricalKeyed(mFingerprint,
      Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err3);
=======
>>>>>>> upstream-releases
    return rv;
  }

  mSignatureVerified = true;
  Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 0);

  return NS_OK;
}

<<<<<<< HEAD
bool IsNewLine(char16_t c) { return c == '\n' || c == '\r'; }
||||||| merged common ancestors
bool
IsNewLine(char16_t c)
{
  return c == '\n' || c == '\r';
}
=======
void VerifyContentSignatureTask::CallCallback(nsresult rv) {
  if (NS_FAILED(rv)) {
    mPromise->MaybeReject(rv);
  } else {
    mPromise->MaybeResolve(mSignatureVerified);
  }
}
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult ReadChainIntoCertList(const nsACString& aCertChain,
                               CERTCertList* aCertList) {
||||||| merged common ancestors
nsresult
ReadChainIntoCertList(const nsACString& aCertChain, CERTCertList* aCertList)
{
=======
bool IsNewLine(char16_t c) { return c == '\n' || c == '\r'; }

nsresult ReadChainIntoCertList(const nsACString& aCertChain,
                               CERTCertList* aCertList) {
>>>>>>> upstream-releases
  bool inBlock = false;
  bool certFound = false;

  const nsCString header = NS_LITERAL_CSTRING("-----BEGIN CERTIFICATE-----");
  const nsCString footer = NS_LITERAL_CSTRING("-----END CERTIFICATE-----");

  nsCWhitespaceTokenizerTemplate<IsNewLine> tokenizer(aCertChain);

  nsAutoCString blockData;
  while (tokenizer.hasMoreTokens()) {
    nsDependentCSubstring token = tokenizer.nextToken();
    if (token.IsEmpty()) {
      continue;
    }
    if (inBlock) {
      if (token.Equals(footer)) {
        inBlock = false;
        certFound = true;
        // base64 decode data, make certs, append to chain
        nsAutoCString derString;
        nsresult rv = Base64Decode(blockData, derString);
        if (NS_FAILED(rv)) {
          CSVerifier_LOG(("CSVerifier: decoding the signature failed"));
          return rv;
        }
        SECItem der = {
            siBuffer,
            BitwiseCast<unsigned char*, const char*>(derString.get()),
            derString.Length(),
        };
        UniqueCERTCertificate tmpCert(CERT_NewTempCertificate(
            CERT_GetDefaultCertDB(), &der, nullptr, false, true));
        if (!tmpCert) {
          return NS_ERROR_FAILURE;
        }
        // if adding tmpCert succeeds, tmpCert will now be owned by aCertList
        SECStatus res = CERT_AddCertToListTail(aCertList, tmpCert.get());
        if (res != SECSuccess) {
          return MapSECStatus(res);
        }
        Unused << tmpCert.release();
      } else {
        blockData.Append(token);
      }
    } else if (token.Equals(header)) {
      inBlock = true;
      blockData = "";
    }
  }
  if (inBlock || !certFound) {
    // the PEM data did not end; bad data.
    CSVerifier_LOG(("CSVerifier: supplied chain contains bad data"));
    return NS_ERROR_FAILURE;
  }
  return NS_OK;
}

<<<<<<< HEAD
nsresult ContentSignatureVerifier::CreateContextInternal(
    const nsACString& aData, const nsACString& aCertChain,
    const nsACString& aName) {
  MOZ_ASSERT(NS_IsMainThread());

||||||| merged common ancestors
nsresult
ContentSignatureVerifier::CreateContextInternal(const nsACString& aData,
                                                const nsACString& aCertChain,
                                                const nsACString& aName)
{
  MOZ_ASSERT(NS_IsMainThread());

=======
// Given data to verify, a content signature header value, a string representing
// a list of PEM-encoded certificates, and a hostname to validate the
// certificates against, this function attempts to validate the certificate
// chain, extract the signature from the header, and verify the data using the
// key in the end-entity certificate from the chain. Returns NS_OK if everything
// is satisfactory and a failing nsresult otherwise. The output parameters are
// filled with telemetry data to report in the case of failures.
static nsresult VerifyContentSignatureInternal(
    const nsACString& aData, const nsACString& aCSHeader,
    const nsACString& aCertChain, const nsACString& aHostname,
    /* out */
    Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS& aErrorLabel,
    /* out */ nsACString& aCertFingerprint,
    /* out */ uint32_t& aErrorValue) {
>>>>>>> upstream-releases
  UniqueCERTCertList certCertList(CERT_NewCertList());
  if (!certCertList) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

  nsresult rv = ReadChainIntoCertList(aCertChain, certCertList.get());
  if (NS_FAILED(rv)) {
    return rv;
  }

  CERTCertListNode* node = CERT_LIST_HEAD(certCertList.get());
  if (!node || CERT_LIST_END(node, certCertList.get()) || !node->cert) {
    return NS_ERROR_FAILURE;
  }

  SECItem* certSecItem = &node->cert->derCert;

  Input certDER;
  mozilla::pkix::Result result =
      certDER.Init(BitwiseCast<uint8_t*, unsigned char*>(certSecItem->data),
                   certSecItem->len);
  if (result != Success) {
    return NS_ERROR_FAILURE;
  }

  // Get EE certificate fingerprint for telemetry.
  unsigned char fingerprint[SHA256_LENGTH] = {0};
  SECStatus srv = PK11_HashBuf(SEC_OID_SHA256, fingerprint, certSecItem->data,
                               AssertedCast<int32_t>(certSecItem->len));
  if (srv != SECSuccess) {
    return NS_ERROR_FAILURE;
  }
  SECItem fingerprintItem = {siBuffer, fingerprint, SHA256_LENGTH};
  UniquePORTString tmpFingerprintString(CERT_Hexify(&fingerprintItem, 0));
  if (!tmpFingerprintString) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
  aCertFingerprint.Assign(tmpFingerprintString.get());

  // Check the signerCert chain is good
  CSTrustDomain trustDomain(certCertList);
  result = BuildCertChain(
      trustDomain, certDER, Now(), EndEntityOrCA::MustBeEndEntity,
      KeyUsage::noParticularKeyUsageRequired, KeyPurposeId::id_kp_codeSigning,
      CertPolicyId::anyPolicy, nullptr /*stapledOCSPResponse*/);
  if (result != Success) {
    // if there was a library error, return an appropriate error
    if (IsFatalError(result)) {
      return NS_ERROR_FAILURE;
    }
    // otherwise, assume the signature was invalid
    if (result == mozilla::pkix::Result::ERROR_EXPIRED_CERTIFICATE) {
<<<<<<< HEAD
      Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 4);
      Telemetry::AccumulateCategoricalKeyed(
          mFingerprint,
          Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err4);
||||||| merged common ancestors
      Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 4);
      Telemetry::AccumulateCategoricalKeyed(mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err4);
=======
      aErrorLabel =
          Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err4;
      aErrorValue = 4;
>>>>>>> upstream-releases
    } else if (result ==
               mozilla::pkix::Result::ERROR_NOT_YET_VALID_CERTIFICATE) {
<<<<<<< HEAD
      Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 5);
      Telemetry::AccumulateCategoricalKeyed(
          mFingerprint,
          Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err5);
||||||| merged common ancestors
      Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 5);
      Telemetry::AccumulateCategoricalKeyed(mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err5);
=======
      aErrorLabel =
          Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err5;
      aErrorValue = 5;
>>>>>>> upstream-releases
    } else {
      // Building cert chain failed for some other reason.
<<<<<<< HEAD
      Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 6);
      Telemetry::AccumulateCategoricalKeyed(
          mFingerprint,
          Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err6);
||||||| merged common ancestors
      Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 6);
      Telemetry::AccumulateCategoricalKeyed(mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err6);
=======
      aErrorLabel =
          Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err6;
      aErrorValue = 6;
>>>>>>> upstream-releases
    }
    CSVerifier_LOG(("CSVerifier: The supplied chain is bad (%s)",
                    MapResultToName(result)));
    return NS_ERROR_INVALID_SIGNATURE;
  }

  // Check the SAN
  Input hostnameInput;

  result = hostnameInput.Init(
<<<<<<< HEAD
      BitwiseCast<const uint8_t*, const char*>(aName.BeginReading()),
      aName.Length());
||||||| merged common ancestors
    BitwiseCast<const uint8_t*, const char*>(aName.BeginReading()),
    aName.Length());
=======
      BitwiseCast<const uint8_t*, const char*>(aHostname.BeginReading()),
      aHostname.Length());
>>>>>>> upstream-releases
  if (result != Success) {
    return NS_ERROR_FAILURE;
  }

  BRNameMatchingPolicy nameMatchingPolicy(BRNameMatchingPolicy::Mode::Enforce);
  result = CheckCertHostname(certDER, hostnameInput, nameMatchingPolicy);
  if (result != Success) {
    // EE cert isnot valid for the given host name.
<<<<<<< HEAD
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 7);
    Telemetry::AccumulateCategoricalKeyed(
        mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err7);
||||||| merged common ancestors
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 7);
    Telemetry::AccumulateCategoricalKeyed(mFingerprint,
      Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err7);
=======
    aErrorLabel = Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err7;
    aErrorValue = 7;
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_SIGNATURE;
  }

  mozilla::UniqueSECKEYPublicKey key(CERT_ExtractPublicKey(node->cert));
  // in case we were not able to extract a key
<<<<<<< HEAD
  if (!mKey) {
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 8);
    Telemetry::AccumulateCategoricalKeyed(
        mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err8);
    CSVerifier_LOG(("CSVerifier: unable to extract a key\n"));
||||||| merged common ancestors
  if (!mKey) {
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 8);
    Telemetry::AccumulateCategoricalKeyed(mFingerprint,
      Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err8);
    CSVerifier_LOG(("CSVerifier: unable to extract a key\n"));
=======
  if (!key) {
    aErrorLabel = Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err8;
    aErrorValue = 8;
    CSVerifier_LOG(("CSVerifier: unable to extract a key"));
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_SIGNATURE;
  }

  nsAutoCString signature;
  rv = ParseContentSignatureHeader(aCSHeader, signature);
  if (NS_FAILED(rv)) {
    return rv;
  }

  // Base 64 decode the signature
  nsAutoCString rawSignature;
  rv = Base64Decode(signature, rawSignature);
  if (NS_FAILED(rv)) {
    CSVerifier_LOG(("CSVerifier: decoding the signature failed"));
    return rv;
  }

  // get signature object
  ScopedAutoSECItem signatureItem;
  SECItem rawSignatureItem = {
      siBuffer,
      BitwiseCast<unsigned char*, const char*>(rawSignature.get()),
      rawSignature.Length(),
  };
  // We have a raw ecdsa signature r||s so we have to DER-encode it first
  // Note that we have to check rawSignatureItem->len % 2 here as
  // DSAU_EncodeDerSigWithLen asserts this
  if (rawSignatureItem.len == 0 || rawSignatureItem.len % 2 != 0) {
    CSVerifier_LOG(("CSVerifier: signature length is bad"));
    return NS_ERROR_FAILURE;
  }
  if (DSAU_EncodeDerSigWithLen(&signatureItem, &rawSignatureItem,
                               rawSignatureItem.len) != SECSuccess) {
    CSVerifier_LOG(("CSVerifier: encoding the signature failed"));
    return NS_ERROR_FAILURE;
  }

  // this is the only OID we support for now
  SECOidTag oid = SEC_OID_ANSIX962_ECDSA_SHA384_SIGNATURE;
<<<<<<< HEAD

  mCx = UniqueVFYContext(
      VFY_CreateContext(mKey.get(), &signatureItem, oid, nullptr));
  if (!mCx) {
||||||| merged common ancestors

  mCx = UniqueVFYContext(
    VFY_CreateContext(mKey.get(), &signatureItem, oid, nullptr));
  if (!mCx) {
=======
  mozilla::UniqueVFYContext cx(
      VFY_CreateContext(key.get(), &signatureItem, oid, nullptr));
  if (!cx) {
>>>>>>> upstream-releases
    // Creating context failed.
<<<<<<< HEAD
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 9);
    Telemetry::AccumulateCategoricalKeyed(
        mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err9);
||||||| merged common ancestors
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 9);
    Telemetry::AccumulateCategoricalKeyed(mFingerprint,
      Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err9);
=======
    aErrorLabel = Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err9;
    aErrorValue = 9;
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_SIGNATURE;
  }

  if (VFY_Begin(cx.get()) != SECSuccess) {
    // Creating context failed.
<<<<<<< HEAD
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 9);
    Telemetry::AccumulateCategoricalKeyed(
        mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err9);
||||||| merged common ancestors
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 9);
    Telemetry::AccumulateCategoricalKeyed(mFingerprint,
      Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err9);
=======
    aErrorLabel = Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err9;
    aErrorValue = 9;
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_SIGNATURE;
  }
<<<<<<< HEAD

  rv = UpdateInternal(kPREFIX);
  if (NS_FAILED(rv)) {
    return rv;
  }
  // add data if we got any
  return UpdateInternal(aData);
}

nsresult ContentSignatureVerifier::DownloadCertChain() {
  MOZ_ASSERT(NS_IsMainThread());

  if (mCertChainURL.IsEmpty()) {
||||||| merged common ancestors

  rv = UpdateInternal(kPREFIX);
  if (NS_FAILED(rv)) {
    return rv;
  }
  // add data if we got any
  return UpdateInternal(aData);
}

nsresult
ContentSignatureVerifier::DownloadCertChain()
{
  MOZ_ASSERT(NS_IsMainThread());

  if (mCertChainURL.IsEmpty()) {
=======
  if (VFY_Update(cx.get(), kPREFIX, sizeof(kPREFIX)) != SECSuccess) {
    aErrorLabel = Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err1;
    aErrorValue = 1;
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_SIGNATURE;
  }
  if (VFY_Update(cx.get(),
                 reinterpret_cast<const unsigned char*>(aData.BeginReading()),
                 aData.Length()) != SECSuccess) {
    aErrorLabel = Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err1;
    aErrorValue = 1;
    return NS_ERROR_INVALID_SIGNATURE;
  }
<<<<<<< HEAD

  rv = NS_NewChannel(getter_AddRefs(mChannel), certChainURI,
                     nsContentUtils::GetSystemPrincipal(),
                     nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL,
                     nsIContentPolicy::TYPE_OTHER);
  if (NS_FAILED(rv)) {
    return rv;
  }

  // we need this chain soon
  nsCOMPtr<nsISupportsPriority> priorityChannel = do_QueryInterface(mChannel);
  if (priorityChannel) {
    priorityChannel->AdjustPriority(nsISupportsPriority::PRIORITY_HIGHEST);
  }

  rv = mChannel->AsyncOpen2(this);
  if (NS_FAILED(rv)) {
    return rv;
  }

  return NS_OK;
}

// Create a context for content signature verification using CreateContext
// below. This function doesn't require a cert chain to be passed, but instead
// aCSHeader must contain an x5u value that is then used to download the cert
// chain.
NS_IMETHODIMP
ContentSignatureVerifier::CreateContextWithoutCertChain(
    nsIContentSignatureReceiverCallback* aCallback, const nsACString& aCSHeader,
    const nsACString& aName) {
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aCallback);
  if (mInitialised) {
    return NS_ERROR_ALREADY_INITIALIZED;
  }
  mInitialised = true;

  // we get the raw content-signature header here, so first parse aCSHeader
  nsresult rv = ParseContentSignatureHeader(aCSHeader);
  if (NS_FAILED(rv)) {
    return rv;
  }

  mCallback = aCallback;
  mName.Assign(aName);

  // We must download the cert chain now.
  // This is async and blocks createContextInternal calls.
  return DownloadCertChain();
}

// Create a context for a content signature verification.
// It sets signature, certificate chain and name that should be used to verify
// the data. The data parameter is the first part of the data to verify (this
// can be the empty string).
NS_IMETHODIMP
ContentSignatureVerifier::CreateContext(const nsACString& aData,
                                        const nsACString& aCSHeader,
                                        const nsACString& aCertChain,
                                        const nsACString& aName) {
  if (mInitialised) {
    return NS_ERROR_ALREADY_INITIALIZED;
  }
  mInitialised = true;
  // The cert chain is given in aCertChain so we don't have to download
  // anything.
  mHasCertChain = true;

  // we get the raw content-signature header here, so first parse aCSHeader
  nsresult rv = ParseContentSignatureHeader(aCSHeader);
  if (NS_FAILED(rv)) {
    return rv;
  }

  return CreateContextInternal(aData, aCertChain, aName);
}

nsresult ContentSignatureVerifier::UpdateInternal(const nsACString& aData) {
  if (!aData.IsEmpty()) {
    if (VFY_Update(mCx.get(),
                   (const unsigned char*)nsPromiseFlatCString(aData).get(),
                   aData.Length()) != SECSuccess) {
      return NS_ERROR_INVALID_SIGNATURE;
    }
  }
  return NS_OK;
}

/**
 * Add data to the context that shold be verified.
 */
NS_IMETHODIMP
ContentSignatureVerifier::Update(const nsACString& aData) {
  MOZ_ASSERT(NS_IsMainThread());

  // If we didn't create the context yet, bail!
  if (!mHasCertChain) {
    MOZ_ASSERT_UNREACHABLE(
        "Someone called ContentSignatureVerifier::Update before "
        "downloading the cert chain.");
    return NS_ERROR_FAILURE;
  }

  return UpdateInternal(aData);
}

/**
 * Finish signature verification and return the result in _retval.
 */
NS_IMETHODIMP
ContentSignatureVerifier::End(bool* _retval) {
  NS_ENSURE_ARG(_retval);
  MOZ_ASSERT(NS_IsMainThread());

  // If we didn't create the context yet, bail!
  if (!mHasCertChain) {
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 2);
    MOZ_ASSERT_UNREACHABLE(
        "Someone called ContentSignatureVerifier::End before "
        "downloading the cert chain.");
    return NS_ERROR_FAILURE;
  }

  bool result = (VFY_End(mCx.get()) == SECSuccess);
  if (result) {
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 0);
  } else {
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 1);
    Telemetry::AccumulateCategoricalKeyed(
        mFingerprint,
        Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err1);
||||||| merged common ancestors

  rv = NS_NewChannel(getter_AddRefs(mChannel), certChainURI,
                     nsContentUtils::GetSystemPrincipal(),
                     nsILoadInfo::SEC_ALLOW_CROSS_ORIGIN_DATA_IS_NULL,
                     nsIContentPolicy::TYPE_OTHER);
  if (NS_FAILED(rv)) {
    return rv;
  }

  // we need this chain soon
  nsCOMPtr<nsISupportsPriority> priorityChannel = do_QueryInterface(mChannel);
  if (priorityChannel) {
    priorityChannel->AdjustPriority(nsISupportsPriority::PRIORITY_HIGHEST);
  }

  rv = mChannel->AsyncOpen2(this);
  if (NS_FAILED(rv)) {
    return rv;
  }

  return NS_OK;
}

// Create a context for content signature verification using CreateContext below.
// This function doesn't require a cert chain to be passed, but instead aCSHeader
// must contain an x5u value that is then used to download the cert chain.
NS_IMETHODIMP
ContentSignatureVerifier::CreateContextWithoutCertChain(
  nsIContentSignatureReceiverCallback *aCallback, const nsACString& aCSHeader,
  const nsACString& aName)
{
  MOZ_ASSERT(NS_IsMainThread());
  MOZ_ASSERT(aCallback);
  if (mInitialised) {
    return NS_ERROR_ALREADY_INITIALIZED;
  }
  mInitialised = true;

  // we get the raw content-signature header here, so first parse aCSHeader
  nsresult rv = ParseContentSignatureHeader(aCSHeader);
  if (NS_FAILED(rv)) {
    return rv;
  }

  mCallback = aCallback;
  mName.Assign(aName);

  // We must download the cert chain now.
  // This is async and blocks createContextInternal calls.
  return DownloadCertChain();
}

// Create a context for a content signature verification.
// It sets signature, certificate chain and name that should be used to verify
// the data. The data parameter is the first part of the data to verify (this
// can be the empty string).
NS_IMETHODIMP
ContentSignatureVerifier::CreateContext(const nsACString& aData,
                                        const nsACString& aCSHeader,
                                        const nsACString& aCertChain,
                                        const nsACString& aName)
{
  if (mInitialised) {
    return NS_ERROR_ALREADY_INITIALIZED;
  }
  mInitialised = true;
  // The cert chain is given in aCertChain so we don't have to download anything.
  mHasCertChain = true;

  // we get the raw content-signature header here, so first parse aCSHeader
  nsresult rv = ParseContentSignatureHeader(aCSHeader);
  if (NS_FAILED(rv)) {
    return rv;
  }

  return CreateContextInternal(aData, aCertChain, aName);
}

nsresult
ContentSignatureVerifier::UpdateInternal(const nsACString& aData)
{
  if (!aData.IsEmpty()) {
    if (VFY_Update(mCx.get(), (const unsigned char*)nsPromiseFlatCString(aData).get(),
                   aData.Length()) != SECSuccess){
      return NS_ERROR_INVALID_SIGNATURE;
    }
  }
  return NS_OK;
}

/**
 * Add data to the context that shold be verified.
 */
NS_IMETHODIMP
ContentSignatureVerifier::Update(const nsACString& aData)
{
  MOZ_ASSERT(NS_IsMainThread());

  // If we didn't create the context yet, bail!
  if (!mHasCertChain) {
    MOZ_ASSERT_UNREACHABLE(
      "Someone called ContentSignatureVerifier::Update before "
      "downloading the cert chain.");
    return NS_ERROR_FAILURE;
  }

  return UpdateInternal(aData);
}

/**
 * Finish signature verification and return the result in _retval.
 */
NS_IMETHODIMP
ContentSignatureVerifier::End(bool* _retval)
{
  NS_ENSURE_ARG(_retval);
  MOZ_ASSERT(NS_IsMainThread());

  // If we didn't create the context yet, bail!
  if (!mHasCertChain) {
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 2);
    MOZ_ASSERT_UNREACHABLE(
      "Someone called ContentSignatureVerifier::End before "
      "downloading the cert chain.");
    return NS_ERROR_FAILURE;
  }

  bool result = (VFY_End(mCx.get()) == SECSuccess);
  if (result) {
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 0);
  } else {
    Accumulate(Telemetry::CONTENT_SIGNATURE_VERIFICATION_STATUS, 1);
    Telemetry::AccumulateCategoricalKeyed(mFingerprint,
      Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err1);
=======
  if (VFY_End(cx.get()) != SECSuccess) {
    aErrorLabel = Telemetry::LABELS_CONTENT_SIGNATURE_VERIFICATION_ERRORS::err1;
    aErrorValue = 1;
    return NS_ERROR_INVALID_SIGNATURE;
>>>>>>> upstream-releases
  }

  return NS_OK;
}

<<<<<<< HEAD
nsresult ContentSignatureVerifier::ParseContentSignatureHeader(
    const nsACString& aContentSignatureHeader) {
  MOZ_ASSERT(NS_IsMainThread());
  // We only support p384 ecdsa according to spec
||||||| merged common ancestors
nsresult
ContentSignatureVerifier::ParseContentSignatureHeader(
  const nsACString& aContentSignatureHeader)
{
  MOZ_ASSERT(NS_IsMainThread());
  // We only support p384 ecdsa according to spec
=======
static nsresult ParseContentSignatureHeader(
    const nsACString& aContentSignatureHeader,
    /* out */ nsCString& aSignature) {
  // We only support p384 ecdsa.
>>>>>>> upstream-releases
  NS_NAMED_LITERAL_CSTRING(signature_var, "p384ecdsa");

  aSignature.Truncate();

  const nsCString& flatHeader = PromiseFlatCString(aContentSignatureHeader);
  nsSecurityHeaderParser parser(flatHeader);
  nsresult rv = parser.Parse();
  if (NS_FAILED(rv)) {
    CSVerifier_LOG(("CSVerifier: could not parse ContentSignature header"));
    return NS_ERROR_FAILURE;
  }
  LinkedList<nsSecurityHeaderDirective>* directives = parser.GetDirectives();

  for (nsSecurityHeaderDirective* directive = directives->getFirst();
       directive != nullptr; directive = directive->getNext()) {
<<<<<<< HEAD
    CSVerifier_LOG(
        ("CSVerifier: found directive %s\n", directive->mName.get()));
||||||| merged common ancestors
    CSVerifier_LOG(("CSVerifier: found directive %s\n", directive->mName.get()));
=======
    CSVerifier_LOG(
        ("CSVerifier: found directive '%s'", directive->mName.get()));
>>>>>>> upstream-releases
    if (directive->mName.Length() == signature_var.Length() &&
        directive->mName.EqualsIgnoreCase(signature_var.get(),
                                          signature_var.Length())) {
      if (!aSignature.IsEmpty()) {
        CSVerifier_LOG(("CSVerifier: found two ContentSignatures"));
        return NS_ERROR_INVALID_SIGNATURE;
      }

      CSVerifier_LOG(("CSVerifier: found a ContentSignature directive"));
      aSignature.Assign(directive->mValue);
    }
  }

  // we have to ensure that we found a signature at this point
<<<<<<< HEAD
  if (mSignature.IsEmpty()) {
    CSVerifier_LOG(
        ("CSVerifier: got a Content-Signature header but didn't find a "
         "signature.\n"));
||||||| merged common ancestors
  if (mSignature.IsEmpty()) {
    CSVerifier_LOG(("CSVerifier: got a Content-Signature header but didn't find a signature.\n"));
=======
  if (aSignature.IsEmpty()) {
    CSVerifier_LOG(
        ("CSVerifier: got a Content-Signature header but didn't find a "
         "signature."));
>>>>>>> upstream-releases
    return NS_ERROR_FAILURE;
  }

  // Bug 769521: We have to change b64 url to regular encoding as long as we
  // don't have a b64 url decoder. This should change soon, but in the meantime
  // we have to live with this.
  aSignature.ReplaceChar('-', '+');
  aSignature.ReplaceChar('_', '/');

  return NS_OK;
}
<<<<<<< HEAD

/* nsIStreamListener implementation */

NS_IMETHODIMP
ContentSignatureVerifier::OnStartRequest(nsIRequest* aRequest,
                                         nsISupports* aContext) {
  MOZ_ASSERT(NS_IsMainThread());
  return NS_OK;
}

NS_IMETHODIMP
ContentSignatureVerifier::OnStopRequest(nsIRequest* aRequest,
                                        nsISupports* aContext,
                                        nsresult aStatus) {
  MOZ_ASSERT(NS_IsMainThread());
  nsCOMPtr<nsIContentSignatureReceiverCallback> callback;
  callback.swap(mCallback);
  nsresult rv;

  // Check HTTP status code and return if it's not 200.
  nsCOMPtr<nsIHttpChannel> http = do_QueryInterface(aRequest, &rv);
  uint32_t httpResponseCode;
  if (NS_FAILED(rv) || NS_FAILED(http->GetResponseStatus(&httpResponseCode)) ||
      httpResponseCode != 200) {
    callback->ContextCreated(false);
    return NS_OK;
  }

  if (NS_FAILED(aStatus)) {
    callback->ContextCreated(false);
    return NS_OK;
  }

  nsAutoCString certChain;
  for (uint32_t i = 0; i < mCertChain.Length(); ++i) {
    certChain.Append(mCertChain[i]);
  }

  // We got the cert chain now. Let's create the context.
  rv = CreateContextInternal(NS_LITERAL_CSTRING(""), certChain, mName);
  if (NS_FAILED(rv)) {
    callback->ContextCreated(false);
    return NS_OK;
  }

  mHasCertChain = true;
  callback->ContextCreated(true);
  return NS_OK;
}

NS_IMETHODIMP
ContentSignatureVerifier::OnDataAvailable(nsIRequest* aRequest,
                                          nsISupports* aContext,
                                          nsIInputStream* aInputStream,
                                          uint64_t aOffset, uint32_t aCount) {
  MOZ_ASSERT(NS_IsMainThread());
  nsAutoCString buffer;

  nsresult rv = NS_ConsumeStream(aInputStream, aCount, buffer);
  if (NS_FAILED(rv)) {
    return rv;
  }

  if (!mCertChain.AppendElement(buffer, fallible)) {
    mCertChain.TruncateLength(0);
    return NS_ERROR_OUT_OF_MEMORY;
  }

  return NS_OK;
}

NS_IMETHODIMP
ContentSignatureVerifier::GetInterface(const nsIID& uuid, void** result) {
  return QueryInterface(uuid, result);
}
||||||| merged common ancestors

/* nsIStreamListener implementation */

NS_IMETHODIMP
ContentSignatureVerifier::OnStartRequest(nsIRequest* aRequest,
                                         nsISupports* aContext)
{
  MOZ_ASSERT(NS_IsMainThread());
  return NS_OK;
}

NS_IMETHODIMP
ContentSignatureVerifier::OnStopRequest(nsIRequest* aRequest,
                                        nsISupports* aContext, nsresult aStatus)
{
  MOZ_ASSERT(NS_IsMainThread());
  nsCOMPtr<nsIContentSignatureReceiverCallback> callback;
  callback.swap(mCallback);
  nsresult rv;

  // Check HTTP status code and return if it's not 200.
  nsCOMPtr<nsIHttpChannel> http = do_QueryInterface(aRequest, &rv);
  uint32_t httpResponseCode;
  if (NS_FAILED(rv) || NS_FAILED(http->GetResponseStatus(&httpResponseCode)) ||
      httpResponseCode != 200) {
    callback->ContextCreated(false);
    return NS_OK;
  }

  if (NS_FAILED(aStatus)) {
    callback->ContextCreated(false);
    return NS_OK;
  }

  nsAutoCString certChain;
  for (uint32_t i = 0; i < mCertChain.Length(); ++i) {
    certChain.Append(mCertChain[i]);
  }

  // We got the cert chain now. Let's create the context.
  rv = CreateContextInternal(NS_LITERAL_CSTRING(""), certChain, mName);
  if (NS_FAILED(rv)) {
    callback->ContextCreated(false);
    return NS_OK;
  }

  mHasCertChain = true;
  callback->ContextCreated(true);
  return NS_OK;
}

NS_IMETHODIMP
ContentSignatureVerifier::OnDataAvailable(nsIRequest* aRequest,
                                          nsISupports* aContext,
                                          nsIInputStream* aInputStream,
                                          uint64_t aOffset, uint32_t aCount)
{
  MOZ_ASSERT(NS_IsMainThread());
  nsAutoCString buffer;

  nsresult rv = NS_ConsumeStream(aInputStream, aCount, buffer);
  if (NS_FAILED(rv)) {
    return rv;
  }

  if (!mCertChain.AppendElement(buffer, fallible)) {
    mCertChain.TruncateLength(0);
    return NS_ERROR_OUT_OF_MEMORY;
  }

  return NS_OK;
}

NS_IMETHODIMP
ContentSignatureVerifier::GetInterface(const nsIID& uuid, void** result)
{
  return QueryInterface(uuid, result);
}
=======
>>>>>>> upstream-releases
