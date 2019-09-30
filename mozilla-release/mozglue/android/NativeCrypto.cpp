/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "NativeCrypto.h"
#include "APKOpen.h"

#include <jni.h>

#include <errno.h>
#include <stdlib.h>
#include <inttypes.h>

#include "mozilla/SHA1.h"
#include "pbkdf2_sha256.h"

/**
 * Helper function to invoke native PBKDF2 function with JNI
 * arguments.
 */
<<<<<<< HEAD
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_pbkdf2SHA256(
    JNIEnv *env, jclass jc, jbyteArray jpassword, jbyteArray jsalt, jint c,
    jint dkLen) {
||||||| merged common ancestors
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL Java_org_mozilla_gecko_background_nativecode_NativeCrypto_pbkdf2SHA256
    (JNIEnv *env, jclass jc, jbyteArray jpassword, jbyteArray jsalt, jint c, jint dkLen) {
=======
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_pbkdf2SHA256(
    JNIEnv* env, jclass jc, jbyteArray jpassword, jbyteArray jsalt, jint c,
    jint dkLen) {
>>>>>>> upstream-releases
  if (dkLen < 0) {
    env->ThrowNew(env->FindClass("java/lang/IllegalArgumentException"),
                  "dkLen should not be less than 0");
    return nullptr;
  }

  jbyte* password = env->GetByteArrayElements(jpassword, nullptr);
  size_t passwordLen = env->GetArrayLength(jpassword);

  jbyte* salt = env->GetByteArrayElements(jsalt, nullptr);
  size_t saltLen = env->GetArrayLength(jsalt);

  uint8_t hashResult[dkLen];
<<<<<<< HEAD
  PBKDF2_SHA256((uint8_t *)password, passwordLen, (uint8_t *)salt, saltLen,
                (uint64_t)c, hashResult, (size_t)dkLen);
||||||| merged common ancestors
  PBKDF2_SHA256((uint8_t *) password, passwordLen, (uint8_t *) salt, saltLen,
      (uint64_t) c, hashResult, (size_t) dkLen);
=======
  PBKDF2_SHA256((uint8_t*)password, passwordLen, (uint8_t*)salt, saltLen,
                (uint64_t)c, hashResult, (size_t)dkLen);
>>>>>>> upstream-releases

  env->ReleaseByteArrayElements(jpassword, password, JNI_ABORT);
  env->ReleaseByteArrayElements(jsalt, salt, JNI_ABORT);

  jbyteArray out = env->NewByteArray(dkLen);
  if (out == nullptr) {
    return nullptr;
  }
<<<<<<< HEAD
  env->SetByteArrayRegion(out, 0, dkLen, (jbyte *)hashResult);
||||||| merged common ancestors
  env->SetByteArrayRegion(out, 0, dkLen, (jbyte *) hashResult);
=======
  env->SetByteArrayRegion(out, 0, dkLen, (jbyte*)hashResult);
>>>>>>> upstream-releases

  return out;
}

using namespace mozilla;

/**
 * Helper function to invoke native SHA-1 function with JNI arguments.
 */
<<<<<<< HEAD
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha1(
    JNIEnv *env, jclass jc, jbyteArray jstr) {
  jbyte *str = env->GetByteArrayElements(jstr, nullptr);
||||||| merged common ancestors
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha1
    (JNIEnv *env, jclass jc, jbyteArray jstr) {
  jbyte *str = env->GetByteArrayElements(jstr, nullptr);
=======
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha1(
    JNIEnv* env, jclass jc, jbyteArray jstr) {
  jbyte* str = env->GetByteArrayElements(jstr, nullptr);
>>>>>>> upstream-releases
  size_t strLen = env->GetArrayLength(jstr);

  SHA1Sum sha1;
  SHA1Sum::Hash hashResult;
<<<<<<< HEAD
  sha1.update((void *)str, (uint32_t)strLen);
||||||| merged common ancestors
  sha1.update((void *) str, (uint32_t) strLen);
=======
  sha1.update((void*)str, (uint32_t)strLen);
>>>>>>> upstream-releases
  sha1.finish(hashResult);

  env->ReleaseByteArrayElements(jstr, str, JNI_ABORT);

  jbyteArray out = env->NewByteArray(SHA1Sum::kHashSize);
  if (out == nullptr) {
    return nullptr;
  }
<<<<<<< HEAD
  env->SetByteArrayRegion(out, 0, SHA1Sum::kHashSize, (jbyte *)hashResult);
||||||| merged common ancestors
  env->SetByteArrayRegion(out, 0, SHA1Sum::kHashSize, (jbyte *) hashResult);
=======
  env->SetByteArrayRegion(out, 0, SHA1Sum::kHashSize, (jbyte*)hashResult);
>>>>>>> upstream-releases

  return out;
}

/**
 * Helper function to invoke native SHA-256 init with JNI arguments.
 */
<<<<<<< HEAD
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256init(
    JNIEnv *env, jclass jc) {
||||||| merged common ancestors
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256init
    (JNIEnv *env, jclass jc) {
=======
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256init(
    JNIEnv* env, jclass jc) {
>>>>>>> upstream-releases
  jbyteArray out = env->NewByteArray(sizeof(SHA256_CTX));
  if (nullptr == out) {
    return nullptr;
  }

<<<<<<< HEAD
  SHA256_CTX *shaContext =
      (SHA256_CTX *)env->GetByteArrayElements(out, nullptr);
||||||| merged common ancestors
  SHA256_CTX *shaContext = (SHA256_CTX*)env->GetByteArrayElements(out, nullptr);
=======
  SHA256_CTX* shaContext = (SHA256_CTX*)env->GetByteArrayElements(out, nullptr);
>>>>>>> upstream-releases
  SHA256_Init(shaContext);

  env->ReleaseByteArrayElements(out, (jbyte *)shaContext, 0);

  return out;
}

/**
 * Helper function to invoke native SHA-256 update with JNI arguments.
 */
<<<<<<< HEAD
extern "C" JNIEXPORT void MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256update(
    JNIEnv *env, jclass jc, jbyteArray jctx, jbyteArray jstr, jint len) {
  jbyte *str = env->GetByteArrayElements(jstr, nullptr);
||||||| merged common ancestors
extern "C" JNIEXPORT void MOZ_JNICALL Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256update
    (JNIEnv *env, jclass jc, jbyteArray jctx, jbyteArray jstr, jint len) {
  jbyte *str = env->GetByteArrayElements(jstr, nullptr);
=======
extern "C" JNIEXPORT void MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256update(
    JNIEnv* env, jclass jc, jbyteArray jctx, jbyteArray jstr, jint len) {
  jbyte* str = env->GetByteArrayElements(jstr, nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  SHA256_CTX *shaContext =
      (SHA256_CTX *)env->GetByteArrayElements(jctx, nullptr);
||||||| merged common ancestors
  SHA256_CTX *shaContext = (SHA256_CTX*)env->GetByteArrayElements(jctx, nullptr);
=======
  SHA256_CTX* shaContext =
      (SHA256_CTX*)env->GetByteArrayElements(jctx, nullptr);
>>>>>>> upstream-releases

<<<<<<< HEAD
  SHA256_Update(shaContext, (void *)str, (size_t)len);
||||||| merged common ancestors
  SHA256_Update(shaContext, (void*)str, (size_t) len);
=======
  SHA256_Update(shaContext, (void*)str, (size_t)len);
>>>>>>> upstream-releases

  env->ReleaseByteArrayElements(jstr, str, JNI_ABORT);
  env->ReleaseByteArrayElements(jctx, (jbyte *)shaContext, 0);

  return;
}

/**
 * Helper function to invoke native SHA-256 finalize with JNI arguments.
 */
<<<<<<< HEAD
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256finalize(
    JNIEnv *env, jclass jc, jbyteArray jctx) {
  SHA256_CTX *shaContext =
      (SHA256_CTX *)env->GetByteArrayElements(jctx, nullptr);
||||||| merged common ancestors
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256finalize
    (JNIEnv *env, jclass jc, jbyteArray jctx) {
  SHA256_CTX *shaContext = (SHA256_CTX*)env->GetByteArrayElements(jctx, nullptr);
=======
extern "C" JNIEXPORT jbyteArray MOZ_JNICALL
Java_org_mozilla_gecko_background_nativecode_NativeCrypto_sha256finalize(
    JNIEnv* env, jclass jc, jbyteArray jctx) {
  SHA256_CTX* shaContext =
      (SHA256_CTX*)env->GetByteArrayElements(jctx, nullptr);
>>>>>>> upstream-releases

  unsigned char *digest = new unsigned char[32];
  SHA256_Final(digest, shaContext);

  env->ReleaseByteArrayElements(jctx, (jbyte *)shaContext, JNI_ABORT);

  jbyteArray out = env->NewByteArray(32);
  if (nullptr != out) {
    env->SetByteArrayRegion(out, 0, 32, (jbyte *)digest);
  }

  delete[] digest;

  return out;
}
