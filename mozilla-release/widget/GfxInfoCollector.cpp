/* vim: se cin sw=2 ts=2 et : */
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GfxInfoCollector.h"
#include "jsapi.h"
#include "nsString.h"

using namespace mozilla;
using namespace widget;

<<<<<<< HEAD
void InfoObject::DefineProperty(const char *name, int value) {
  if (!mOk) return;
||||||| merged common ancestors
void
InfoObject::DefineProperty(const char *name, int value)
{
  if (!mOk)
    return;
=======
void InfoObject::DefineProperty(const char* name, int value) {
  if (!mOk) return;
>>>>>>> upstream-releases

  mOk = JS_DefineProperty(mCx, mObj, name, value, JSPROP_ENUMERATE);
}

<<<<<<< HEAD
void InfoObject::DefineProperty(const char *name, nsAString &value) {
  if (!mOk) return;
||||||| merged common ancestors
void
InfoObject::DefineProperty(const char *name, nsAString &value)
{
  if (!mOk)
    return;
=======
void InfoObject::DefineProperty(const char* name, nsAString& value) {
  if (!mOk) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  const nsString &flat = PromiseFlatString(value);
  JS::Rooted<JSString *> string(
      mCx, JS_NewUCStringCopyN(mCx, static_cast<const char16_t *>(flat.get()),
                               flat.Length()));
  if (!string) mOk = false;
||||||| merged common ancestors
  const nsString &flat = PromiseFlatString(value);
  JS::Rooted<JSString*> string(mCx, JS_NewUCStringCopyN(mCx, static_cast<const char16_t*>(flat.get()),
                                                        flat.Length()));
  if (!string)
    mOk = false;
=======
  const nsString& flat = PromiseFlatString(value);
  JS::Rooted<JSString*> string(
      mCx, JS_NewUCStringCopyN(mCx, static_cast<const char16_t*>(flat.get()),
                               flat.Length()));
  if (!string) mOk = false;
>>>>>>> upstream-releases

  if (!mOk) return;

  mOk = JS_DefineProperty(mCx, mObj, name, string, JSPROP_ENUMERATE);
}

<<<<<<< HEAD
void InfoObject::DefineProperty(const char *name, const char *value) {
||||||| merged common ancestors
void
InfoObject::DefineProperty(const char *name, const char *value)
{
=======
void InfoObject::DefineProperty(const char* name, const char* value) {
>>>>>>> upstream-releases
  nsAutoString string = NS_ConvertASCIItoUTF16(value);
  DefineProperty(name, string);
}

<<<<<<< HEAD
InfoObject::InfoObject(JSContext *aCx) : mCx(aCx), mObj(aCx), mOk(true) {
||||||| merged common ancestors
InfoObject::InfoObject(JSContext *aCx) : mCx(aCx), mObj(aCx), mOk(true)
{
=======
InfoObject::InfoObject(JSContext* aCx) : mCx(aCx), mObj(aCx), mOk(true) {
>>>>>>> upstream-releases
  mObj = JS_NewPlainObject(aCx);
  if (!mObj) mOk = false;
}
