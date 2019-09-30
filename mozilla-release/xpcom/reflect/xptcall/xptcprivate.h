/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* All the xptcall private declarations - only include locally. */

#ifndef xptcprivate_h___
#define xptcprivate_h___

#include "xptcall.h"
#include "nsAutoPtr.h"
#include "mozilla/Attributes.h"

<<<<<<< HEAD
#if !defined(__ia64) || \
    (!defined(__hpux) && !defined(__linux__) && !defined(__FreeBSD__))
#define STUB_ENTRY(n) NS_IMETHOD Stub##n() = 0;
||||||| merged common ancestors
#if !defined(__ia64) || (!defined(__hpux) && !defined(__linux__) && !defined(__FreeBSD__))
#define STUB_ENTRY(n) NS_IMETHOD Stub##n() = 0;
=======
#if !defined(__ia64) || \
    (!defined(__hpux) && !defined(__linux__) && !defined(__FreeBSD__))
#  define STUB_ENTRY(n) NS_IMETHOD Stub##n() = 0;
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#define STUB_ENTRY(n)                                                  \
  NS_IMETHOD Stub##n(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, \
                     uint64_t, uint64_t, uint64_t) = 0;
||||||| merged common ancestors
#define STUB_ENTRY(n) NS_IMETHOD Stub##n(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t) = 0;
=======
#  define STUB_ENTRY(n)                                                  \
    NS_IMETHOD Stub##n(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, \
                       uint64_t, uint64_t, uint64_t) = 0;
>>>>>>> upstream-releases
#endif

#define SENTINEL_ENTRY(n) NS_IMETHOD Sentinel##n() = 0;

class nsIXPTCStubBase : public nsISupports {
 public:
#include "xptcstubsdef.inc"
};

#undef STUB_ENTRY
#undef SENTINEL_ENTRY

<<<<<<< HEAD
#if !defined(__ia64) || \
    (!defined(__hpux) && !defined(__linux__) && !defined(__FreeBSD__))
#define STUB_ENTRY(n) NS_IMETHOD Stub##n() override;
||||||| merged common ancestors
#if !defined(__ia64) || (!defined(__hpux) && !defined(__linux__) && !defined(__FreeBSD__))
#define STUB_ENTRY(n) NS_IMETHOD Stub##n() override;
=======
#if !defined(__ia64) || \
    (!defined(__hpux) && !defined(__linux__) && !defined(__FreeBSD__))
#  define STUB_ENTRY(n) NS_IMETHOD Stub##n() override;
>>>>>>> upstream-releases
#else
<<<<<<< HEAD
#define STUB_ENTRY(n)                                                  \
  NS_IMETHOD Stub##n(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, \
                     uint64_t, uint64_t, uint64_t) override;
||||||| merged common ancestors
#define STUB_ENTRY(n) NS_IMETHOD Stub##n(uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t,uint64_t) override;
=======
#  define STUB_ENTRY(n)                                                  \
    NS_IMETHOD Stub##n(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, \
                       uint64_t, uint64_t, uint64_t) override;
>>>>>>> upstream-releases
#endif

#define SENTINEL_ENTRY(n) NS_IMETHOD Sentinel##n() override;

class nsXPTCStubBase final : public nsIXPTCStubBase {
 public:
  NS_DECL_ISUPPORTS_INHERITED

#include "xptcstubsdef.inc"

  nsXPTCStubBase(nsIXPTCProxy* aOuter, const nsXPTInterfaceInfo* aEntry)
      : mOuter(aOuter), mEntry(aEntry) {}

  nsIXPTCProxy* mOuter;
  const nsXPTInterfaceInfo* mEntry;

  ~nsXPTCStubBase() {}
};

#undef STUB_ENTRY
#undef SENTINEL_ENTRY

#if defined(__clang__) || defined(__GNUC__)
<<<<<<< HEAD
#define ATTRIBUTE_USED __attribute__((__used__))
||||||| merged common ancestors
#define ATTRIBUTE_USED __attribute__ ((__used__))
=======
#  define ATTRIBUTE_USED __attribute__((__used__))
>>>>>>> upstream-releases
#else
#  define ATTRIBUTE_USED
#endif

#endif /* xptcprivate_h___ */
