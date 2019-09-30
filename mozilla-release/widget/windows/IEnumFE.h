/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef IEnumeFE_h__
#define IEnumeFE_h__

/*
 * CEnumFormatEtc - implements IEnumFORMATETC
 */

#include <ole2.h>

#include "nsTArray.h"
#include "mozilla/Attributes.h"

// FORMATETC container
class FormatEtc {
 public:
  FormatEtc() { memset(&mFormat, 0, sizeof(FORMATETC)); }
<<<<<<< HEAD
  FormatEtc(const FormatEtc &copy) { CopyIn(&copy.mFormat); }
  ~FormatEtc() {
    if (mFormat.ptd) CoTaskMemFree(mFormat.ptd);
  }
||||||| merged common ancestors
  FormatEtc(const FormatEtc& copy) { CopyIn(&copy.mFormat); }
  ~FormatEtc() { if (mFormat.ptd) CoTaskMemFree(mFormat.ptd); }
=======
  FormatEtc(const FormatEtc& copy) { CopyIn(&copy.mFormat); }
  ~FormatEtc() {
    if (mFormat.ptd) CoTaskMemFree(mFormat.ptd);
  }
>>>>>>> upstream-releases

  void CopyIn(const FORMATETC* aSrc) {
    if (!aSrc) {
      memset(&mFormat, 0, sizeof(FORMATETC));
      return;
    }
    mFormat = *aSrc;
    if (aSrc->ptd) {
<<<<<<< HEAD
      mFormat.ptd = (DVTARGETDEVICE *)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
      *(mFormat.ptd) = *(aSrc->ptd);
||||||| merged common ancestors
        mFormat.ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
        *(mFormat.ptd) = *(aSrc->ptd);
=======
      mFormat.ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
      *(mFormat.ptd) = *(aSrc->ptd);
>>>>>>> upstream-releases
    }
  }

  void CopyOut(LPFORMATETC aDest) {
    if (!aDest) return;
    *aDest = mFormat;
    if (mFormat.ptd) {
<<<<<<< HEAD
      aDest->ptd = (DVTARGETDEVICE *)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
      *(aDest->ptd) = *(mFormat.ptd);
||||||| merged common ancestors
        aDest->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
        *(aDest->ptd) = *(mFormat.ptd);
=======
      aDest->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(DVTARGETDEVICE));
      *(aDest->ptd) = *(mFormat.ptd);
>>>>>>> upstream-releases
    }
  }

 private:
  FORMATETC mFormat;
};

/*
 * CEnumFormatEtc is created within IDataObject::EnumFormatEtc. This object
 * lives on its own, that is, QueryInterface only knows IUnknown and
 * IEnumFormatEtc, nothing more.  We still use an outer unknown for reference
 * counting, because as long as this enumerator lives, the data object should
 * live, thereby keeping the application up.
 */

<<<<<<< HEAD
class CEnumFormatEtc final : public IEnumFORMATETC {
 public:
  explicit CEnumFormatEtc(nsTArray<FormatEtc> &aArray);
  CEnumFormatEtc();
  ~CEnumFormatEtc();

  // IUnknown impl.
  STDMETHODIMP QueryInterface(REFIID riid, LPVOID *ppv);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

  // IEnumFORMATETC impl.
  STDMETHODIMP Next(ULONG aMaxToFetch, FORMATETC *aResult, ULONG *aNumFetched);
  STDMETHODIMP Skip(ULONG aSkipNum);
  STDMETHODIMP Reset();
  STDMETHODIMP Clone(LPENUMFORMATETC *aResult);  // Addrefs
||||||| merged common ancestors
class CEnumFormatEtc final : public IEnumFORMATETC
{
public:
    explicit CEnumFormatEtc(nsTArray<FormatEtc>& aArray);
    CEnumFormatEtc();
    ~CEnumFormatEtc();

    // IUnknown impl.
    STDMETHODIMP QueryInterface(REFIID riid, LPVOID *ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IEnumFORMATETC impl.
    STDMETHODIMP Next(ULONG aMaxToFetch, FORMATETC *aResult, ULONG *aNumFetched);
    STDMETHODIMP Skip(ULONG aSkipNum);
    STDMETHODIMP Reset();
    STDMETHODIMP Clone(LPENUMFORMATETC *aResult); // Addrefs

    // Utils 
    void AddFormatEtc(LPFORMATETC aFormat);

private:
    nsTArray<FormatEtc> mFormatList; // Formats
    ULONG mRefCnt; // Object reference count
    ULONG mCurrentIdx; // Current element

    void SetIndex(uint32_t aIdx);
};
=======
class CEnumFormatEtc final : public IEnumFORMATETC {
 public:
  explicit CEnumFormatEtc(nsTArray<FormatEtc>& aArray);
  CEnumFormatEtc();
  ~CEnumFormatEtc();

  // IUnknown impl.
  STDMETHODIMP QueryInterface(REFIID riid, LPVOID* ppv);
  STDMETHODIMP_(ULONG) AddRef();
  STDMETHODIMP_(ULONG) Release();

  // IEnumFORMATETC impl.
  STDMETHODIMP Next(ULONG aMaxToFetch, FORMATETC* aResult, ULONG* aNumFetched);
  STDMETHODIMP Skip(ULONG aSkipNum);
  STDMETHODIMP Reset();
  STDMETHODIMP Clone(LPENUMFORMATETC* aResult);  // Addrefs
>>>>>>> upstream-releases

  // Utils
  void AddFormatEtc(LPFORMATETC aFormat);

 private:
  nsTArray<FormatEtc> mFormatList;  // Formats
  ULONG mRefCnt;                    // Object reference count
  ULONG mCurrentIdx;                // Current element

  void SetIndex(uint32_t aIdx);
};

#endif  //_IENUMFE_H_
