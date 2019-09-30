/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _NSSYSTEMINFO_H_
#define _NSSYSTEMINFO_H_

#include "nsHashPropertyBag.h"
<<<<<<< HEAD
#if defined(XP_WIN)
#include "nsIObserver.h"
#endif  // defined(XP_WIN)
||||||| merged common ancestors
#if defined(XP_WIN)
#include "nsIObserver.h"
#endif // defined(XP_WIN)
=======
#include "nsISystemInfo.h"
#include "mozilla/MozPromise.h"
>>>>>>> upstream-releases

#ifdef MOZ_WIDGET_ANDROID
<<<<<<< HEAD
#include "mozilla/dom/PContent.h"
#endif  // MOZ_WIDGET_ANDROID

class nsSystemInfo final : public nsHashPropertyBag
#if defined(XP_WIN)
    ,
                           public nsIObserver
#endif  // defined(XP_WIN)
{
 public:
#if defined(XP_WIN)
||||||| merged common ancestors
#include "mozilla/dom/PContent.h"
#endif // MOZ_WIDGET_ANDROID

class nsSystemInfo final
  : public nsHashPropertyBag
#if defined(XP_WIN)
  , public nsIObserver
#endif // defined(XP_WIN)
{
public:
#if defined(XP_WIN)
=======
#  include "mozilla/dom/PContent.h"
#endif  // MOZ_WIDGET_ANDROID

struct FolderDiskInfo {
  nsCString model;
  nsCString revision;
  bool isSSD;
};

struct DiskInfo {
  FolderDiskInfo binary;
  FolderDiskInfo profile;
  FolderDiskInfo system;
};

typedef mozilla::MozPromise<DiskInfo, nsresult, /* IsExclusive */ false>
    DiskInfoPromise;

class nsSystemInfo final : public nsISystemInfo, public nsHashPropertyBag {
 public:
>>>>>>> upstream-releases
  NS_DECL_ISUPPORTS_INHERITED
<<<<<<< HEAD
  NS_DECL_NSIOBSERVER
#endif  // defined(XP_WIN)
||||||| merged common ancestors
  NS_DECL_NSIOBSERVER
#endif // defined(XP_WIN)
=======
  NS_DECL_NSISYSTEMINFO
>>>>>>> upstream-releases

  nsSystemInfo();

  nsresult Init();

  // Slot for NS_InitXPCOM to pass information to nsSystemInfo::Init.
  // See comments above the variable definition and in NS_InitXPCOM.
  static uint32_t gUserUmask;

#ifdef MOZ_WIDGET_ANDROID
  static void GetAndroidSystemInfo(mozilla::dom::AndroidSystemInfo* aInfo);

 protected:
  void SetupAndroidInfo(const mozilla::dom::AndroidSystemInfo&);
#endif

 protected:
  void SetInt32Property(const nsAString& aPropertyName, const int32_t aValue);
  void SetUint32Property(const nsAString& aPropertyName, const uint32_t aValue);
  void SetUint64Property(const nsAString& aPropertyName, const uint64_t aValue);

 private:
  ~nsSystemInfo();

<<<<<<< HEAD
#if defined(XP_WIN)
  nsresult GetProfileHDDInfo();
#endif  // defined(XP_WIN)
||||||| merged common ancestors
#if defined(XP_WIN)
  nsresult GetProfileHDDInfo();
#endif // defined(XP_WIN)
=======
  RefPtr<DiskInfoPromise> mDiskInfoPromise;
>>>>>>> upstream-releases
};

#define NS_SYSTEMINFO_CONTRACTID "@mozilla.org/system-info;1"
#define NS_SYSTEMINFO_CID                            \
  {                                                  \
    0xd962398a, 0x99e5, 0x49b2, {                    \
      0x85, 0x7a, 0xc1, 0x59, 0x04, 0x9c, 0x7f, 0x6c \
    }                                                \
  }

#endif /* _NSSYSTEMINFO_H_ */
