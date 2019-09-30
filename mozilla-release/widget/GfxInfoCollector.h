/* vim: se cin sw=2 ts=2 et : */
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __mozilla_widget_GfxInfoCollector_h__
#define __mozilla_widget_GfxInfoCollector_h__

#include "mozilla/Attributes.h"
#include "nsStringFwd.h"
#include "js/RootingAPI.h"

namespace mozilla {
namespace widget {

/* this is handy wrapper around JSAPI to make it more pleasant to use.
 * We collect the JSAPI errors and so that callers don't need to */
class MOZ_STACK_CLASS InfoObject {
  friend class GfxInfoBase;

<<<<<<< HEAD
 public:
  void DefineProperty(const char *name, int value);
  void DefineProperty(const char *name, nsAString &value);
  void DefineProperty(const char *name, const char *value);
||||||| merged common ancestors
  public:
  void DefineProperty(const char *name, int value);
  void DefineProperty(const char *name, nsAString &value);
  void DefineProperty(const char *name, const char *value);
=======
 public:
  void DefineProperty(const char* name, int value);
  void DefineProperty(const char* name, nsAString& value);
  void DefineProperty(const char* name, const char* value);
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  // We need to ensure that this object lives on the stack so that GC sees it
  // properly
  explicit InfoObject(JSContext *aCx);
  InfoObject(InfoObject &);
||||||| merged common ancestors
  private:
  // We need to ensure that this object lives on the stack so that GC sees it properly
  explicit InfoObject(JSContext *aCx);
  InfoObject(InfoObject&);
=======
 private:
  // We need to ensure that this object lives on the stack so that GC sees it
  // properly
  explicit InfoObject(JSContext* aCx);
  InfoObject(InfoObject&);
>>>>>>> upstream-releases

<<<<<<< HEAD
  JSContext *mCx;
  JS::Rooted<JSObject *> mObj;
||||||| merged common ancestors
  JSContext *mCx;
  JS::Rooted<JSObject*> mObj;
=======
  JSContext* mCx;
  JS::Rooted<JSObject*> mObj;
>>>>>>> upstream-releases
  bool mOk;
};

/*

   Here's an example usage:

   class Foo {
   Foo::Foo() : mInfoCollector(this, &Foo::GetAweseomeness) {}

   void GetAwesomeness(InfoObject &obj) {
     obj.DefineProperty("awesome", mAwesome);
   }

   int mAwesome;

   GfxInfoCollector<Foo> mInfoCollector;
   }

   This will define a property on the object
   returned from calling getInfo() on a
   GfxInfo object. e.g.

       gfxInfo = Cc["@mozilla.org/gfx/info;1"].getService(Ci.nsIGfxInfo);
       info = gfxInfo.getInfo();
       if (info.awesome)
          alert(info.awesome);

*/

class GfxInfoCollectorBase {
 public:
  GfxInfoCollectorBase();
  virtual void GetInfo(InfoObject& obj) = 0;
  virtual ~GfxInfoCollectorBase();
};

<<<<<<< HEAD
template <class T>
class GfxInfoCollector : public GfxInfoCollectorBase {
 public:
  GfxInfoCollector(T *aPointer, void (T::*aFunc)(InfoObject &obj))
      : mPointer(aPointer), mFunc(aFunc) {}
  virtual void GetInfo(InfoObject &obj) override { (mPointer->*mFunc)(obj); }
||||||| merged common ancestors
template<class T>
class GfxInfoCollector : public GfxInfoCollectorBase
{
  public:
  GfxInfoCollector(T* aPointer, void (T::*aFunc)(InfoObject &obj)) : mPointer(aPointer), mFunc(aFunc) {
  }
  virtual void GetInfo(InfoObject &obj) override {
    (mPointer->*mFunc)(obj);
  }

  protected:
  T* mPointer;
  void (T::*mFunc)(InfoObject &obj);
=======
template <class T>
class GfxInfoCollector : public GfxInfoCollectorBase {
 public:
  GfxInfoCollector(T* aPointer, void (T::*aFunc)(InfoObject& obj))
      : mPointer(aPointer), mFunc(aFunc) {}
  virtual void GetInfo(InfoObject& obj) override { (mPointer->*mFunc)(obj); }
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  T *mPointer;
  void (T::*mFunc)(InfoObject &obj);
||||||| merged common ancestors
=======
 protected:
  T* mPointer;
  void (T::*mFunc)(InfoObject& obj);
>>>>>>> upstream-releases
};

}  // namespace widget
}  // namespace mozilla

#endif
