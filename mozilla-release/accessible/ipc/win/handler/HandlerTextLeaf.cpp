/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#if defined(MOZILLA_INTERNAL_API)
<<<<<<< HEAD
#error This code is NOT for internal Gecko use!
#endif  // defined(MOZILLA_INTERNAL_API)
||||||| merged common ancestors
#error This code is NOT for internal Gecko use!
#endif // defined(MOZILLA_INTERNAL_API)
=======
#  error This code is NOT for internal Gecko use!
#endif  // defined(MOZILLA_INTERNAL_API)
>>>>>>> upstream-releases

#include "HandlerTextLeaf.h"
#include "mozilla/Assertions.h"

namespace mozilla {
namespace a11y {

<<<<<<< HEAD
HandlerTextLeaf::HandlerTextLeaf(IDispatch *aParent, long aIndexInParent,
                                 HWND aHwnd, AccChildData &aData)
    : mParent(aParent),
      mIndexInParent(aIndexInParent),
      mHwnd(aHwnd),
      mData(aData) {
||||||| merged common ancestors
HandlerTextLeaf::HandlerTextLeaf(IDispatch* aParent,
                                 long aIndexInParent, HWND aHwnd,
                                 AccChildData& aData)
: mParent(aParent)
, mIndexInParent(aIndexInParent)
, mHwnd(aHwnd)
, mData(aData)
{
=======
HandlerTextLeaf::HandlerTextLeaf(IDispatch* aParent, long aIndexInParent,
                                 HWND aHwnd, AccChildData& aData)
    : mParent(aParent),
      mIndexInParent(aIndexInParent),
      mHwnd(aHwnd),
      mData(aData) {
>>>>>>> upstream-releases
  MOZ_ASSERT(aParent);
}

HandlerTextLeaf::~HandlerTextLeaf() {
  if (mData.mText) {
    ::SysFreeString(mData.mText);
  }
}

IMPL_IUNKNOWN_QUERY_HEAD(HandlerTextLeaf)
IMPL_IUNKNOWN_QUERY_IFACE(IDispatch)
IMPL_IUNKNOWN_QUERY_IFACE(IAccessible)
IMPL_IUNKNOWN_QUERY_IFACE(IAccessible2)
IMPL_IUNKNOWN_QUERY_IFACE(IServiceProvider)
IMPL_IUNKNOWN_QUERY_TAIL

/*** IDispatch ***/

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::GetTypeInfoCount(UINT *pctinfo) { return E_NOTIMPL; }
||||||| merged common ancestors
HandlerTextLeaf::GetTypeInfoCount(UINT *pctinfo)
{
  return E_NOTIMPL;
}
=======
HandlerTextLeaf::GetTypeInfoCount(UINT* pctinfo) { return E_NOTIMPL; }
>>>>>>> upstream-releases

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo) {
||||||| merged common ancestors
HandlerTextLeaf::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
{
=======
HandlerTextLeaf::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames,
                               LCID lcid, DISPID *rgDispId) {
||||||| merged common ancestors
HandlerTextLeaf::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames,
                               LCID lcid, DISPID *rgDispId)
{
=======
HandlerTextLeaf::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames,
                               LCID lcid, DISPID* rgDispId) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
HandlerTextLeaf::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
<<<<<<< HEAD
                        WORD wFlags, DISPPARAMS *pDispParams,
                        VARIANT *pVarResult, EXCEPINFO *pExcepInfo,
                        UINT *puArgErr) {
||||||| merged common ancestors
                        WORD wFlags, DISPPARAMS *pDispParams,
                        VARIANT *pVarResult, EXCEPINFO *pExcepInfo,
                        UINT *puArgErr)
{
=======
                        WORD wFlags, DISPPARAMS* pDispParams,
                        VARIANT* pVarResult, EXCEPINFO* pExcepInfo,
                        UINT* puArgErr) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

/*** IAccessible ***/

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accParent(IDispatch **ppdispParent) {
||||||| merged common ancestors
HandlerTextLeaf::get_accParent(IDispatch **ppdispParent)
{
=======
HandlerTextLeaf::get_accParent(IDispatch** ppdispParent) {
>>>>>>> upstream-releases
  if (!ppdispParent) {
    return E_INVALIDARG;
  }

  RefPtr<IDispatch> parent(mParent);
  parent.forget(ppdispParent);
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accChildCount(long *pcountChildren) {
||||||| merged common ancestors
HandlerTextLeaf::get_accChildCount(long *pcountChildren)
{
=======
HandlerTextLeaf::get_accChildCount(long* pcountChildren) {
>>>>>>> upstream-releases
  if (!pcountChildren) {
    return E_INVALIDARG;
  }

  *pcountChildren = 0;
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accChild(VARIANT varChild, IDispatch **ppdispChild) {
||||||| merged common ancestors
HandlerTextLeaf::get_accChild(VARIANT varChild, IDispatch **ppdispChild)
{
=======
HandlerTextLeaf::get_accChild(VARIANT varChild, IDispatch** ppdispChild) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accName(VARIANT varChild, BSTR *pszName) {
||||||| merged common ancestors
HandlerTextLeaf::get_accName(VARIANT varChild, BSTR *pszName)
{
=======
HandlerTextLeaf::get_accName(VARIANT varChild, BSTR* pszName) {
>>>>>>> upstream-releases
  if (varChild.lVal != CHILDID_SELF || !pszName) {
    return E_INVALIDARG;
  }

  *pszName = CopyBSTR(mData.mText);
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accValue(VARIANT varChild, BSTR *pszValue) {
||||||| merged common ancestors
HandlerTextLeaf::get_accValue(VARIANT varChild, BSTR *pszValue)
{
=======
HandlerTextLeaf::get_accValue(VARIANT varChild, BSTR* pszValue) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accDescription(VARIANT varChild, BSTR *pszDescription) {
||||||| merged common ancestors
HandlerTextLeaf::get_accDescription(VARIANT varChild, BSTR *pszDescription)
{
=======
HandlerTextLeaf::get_accDescription(VARIANT varChild, BSTR* pszDescription) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accRole(VARIANT varChild, VARIANT *pvarRole) {
||||||| merged common ancestors
HandlerTextLeaf::get_accRole(VARIANT varChild, VARIANT *pvarRole)
{
=======
HandlerTextLeaf::get_accRole(VARIANT varChild, VARIANT* pvarRole) {
>>>>>>> upstream-releases
  if (varChild.lVal != CHILDID_SELF || !pvarRole) {
    return E_INVALIDARG;
  }

  pvarRole->vt = VT_I4;
  pvarRole->lVal = mData.mTextRole;
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accState(VARIANT varChild, VARIANT *pvarState) {
||||||| merged common ancestors
HandlerTextLeaf::get_accState(VARIANT varChild, VARIANT *pvarState)
{
=======
HandlerTextLeaf::get_accState(VARIANT varChild, VARIANT* pvarState) {
>>>>>>> upstream-releases
  if (varChild.lVal != CHILDID_SELF || !pvarState) {
    return E_INVALIDARG;
  }

  pvarState->vt = VT_I4;
  pvarState->lVal = mData.mTextState;
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accHelp(VARIANT varChild, BSTR *pszHelp) {
||||||| merged common ancestors
HandlerTextLeaf::get_accHelp(VARIANT varChild, BSTR *pszHelp)
{
=======
HandlerTextLeaf::get_accHelp(VARIANT varChild, BSTR* pszHelp) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accHelpTopic(BSTR *pszHelpFile, VARIANT varChild,
                                  long *pidTopic) {
||||||| merged common ancestors
HandlerTextLeaf::get_accHelpTopic(BSTR *pszHelpFile, VARIANT varChild,
                                  long *pidTopic)
{
=======
HandlerTextLeaf::get_accHelpTopic(BSTR* pszHelpFile, VARIANT varChild,
                                  long* pidTopic) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
HandlerTextLeaf::get_accKeyboardShortcut(VARIANT varChild,
<<<<<<< HEAD
                                         BSTR *pszKeyboardShortcut) {
||||||| merged common ancestors
                                         BSTR *pszKeyboardShortcut)
{
=======
                                         BSTR* pszKeyboardShortcut) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accFocus(VARIANT *pvarChild) { return E_NOTIMPL; }
||||||| merged common ancestors
HandlerTextLeaf::get_accFocus(VARIANT *pvarChild)
{
  return E_NOTIMPL;
}
=======
HandlerTextLeaf::get_accFocus(VARIANT* pvarChild) { return E_NOTIMPL; }
>>>>>>> upstream-releases

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_accSelection(VARIANT *pvarChildren) { return E_NOTIMPL; }
||||||| merged common ancestors
HandlerTextLeaf::get_accSelection(VARIANT *pvarChildren)
{
  return E_NOTIMPL;
}
=======
HandlerTextLeaf::get_accSelection(VARIANT* pvarChildren) { return E_NOTIMPL; }
>>>>>>> upstream-releases

HRESULT
HandlerTextLeaf::get_accDefaultAction(VARIANT varChild,
<<<<<<< HEAD
                                      BSTR *pszDefaultAction) {
||||||| merged common ancestors
                                      BSTR *pszDefaultAction)
{
=======
                                      BSTR* pszDefaultAction) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
HandlerTextLeaf::accSelect(long flagsSelect, VARIANT varChild) {
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::accLocation(long *pxLeft, long *pyTop, long *pcxWidth,
                             long *pcyHeight, VARIANT varChild) {
||||||| merged common ancestors
HandlerTextLeaf::accLocation(long *pxLeft, long *pyTop, long *pcxWidth,
                             long *pcyHeight, VARIANT varChild)
{
=======
HandlerTextLeaf::accLocation(long* pxLeft, long* pyTop, long* pcxWidth,
                             long* pcyHeight, VARIANT varChild) {
>>>>>>> upstream-releases
  if (varChild.lVal != CHILDID_SELF || !pxLeft || !pyTop || !pcxWidth ||
      !pcyHeight) {
    return E_INVALIDARG;
  }

  *pxLeft = mData.mTextLeft;
  *pyTop = mData.mTextTop;
  *pcxWidth = mData.mTextWidth;
  *pcyHeight = mData.mTextHeight;
  return S_OK;
}

HRESULT
HandlerTextLeaf::accNavigate(long navDir, VARIANT varStart,
<<<<<<< HEAD
                             VARIANT *pvarEndUpAt) {
||||||| merged common ancestors
                             VARIANT *pvarEndUpAt)
{
=======
                             VARIANT* pvarEndUpAt) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::accHitTest(long xLeft, long yTop, VARIANT *pvarChild) {
||||||| merged common ancestors
HandlerTextLeaf::accHitTest( long xLeft, long yTop, VARIANT *pvarChild)
{
=======
HandlerTextLeaf::accHitTest(long xLeft, long yTop, VARIANT* pvarChild) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
HandlerTextLeaf::accDoDefaultAction(VARIANT varChild) { return E_NOTIMPL; }

HRESULT
HandlerTextLeaf::put_accName(VARIANT varChild, BSTR szName) {
  return E_NOTIMPL;
}

HRESULT
HandlerTextLeaf::put_accValue(VARIANT varChild, BSTR szValue) {
  return E_NOTIMPL;
}

/*** IAccessible2 ***/

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_nRelations(long *nRelations) { return E_NOTIMPL; }
||||||| merged common ancestors
HandlerTextLeaf::get_nRelations(long* nRelations)
{
  return E_NOTIMPL;
}
=======
HandlerTextLeaf::get_nRelations(long* nRelations) { return E_NOTIMPL; }
>>>>>>> upstream-releases

HRESULT
HandlerTextLeaf::get_relation(long relationIndex,
<<<<<<< HEAD
                              IAccessibleRelation **relation) {
||||||| merged common ancestors
                              IAccessibleRelation** relation)
{
=======
                              IAccessibleRelation** relation) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
HandlerTextLeaf::get_relations(long maxRelations,
<<<<<<< HEAD
                               IAccessibleRelation **relations,
                               long *nRelations) {
||||||| merged common ancestors
                               IAccessibleRelation** relations,
                               long* nRelations)
{
=======
                               IAccessibleRelation** relations,
                               long* nRelations) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::role(long *role) {
||||||| merged common ancestors
HandlerTextLeaf::role(long* role)
{
=======
HandlerTextLeaf::role(long* role) {
>>>>>>> upstream-releases
  if (!role) {
    return E_INVALIDARG;
  }

  *role = mData.mTextRole;
  return S_OK;
}

HRESULT
HandlerTextLeaf::scrollTo(IA2ScrollType scrollType) { return E_NOTIMPL; }

HRESULT
HandlerTextLeaf::scrollToPoint(IA2CoordinateType coordinateType, long x,
                               long y) {
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_groupPosition(long *groupLevel, long *similarItemsInGroup,
                                   long *positionInGroup) {
||||||| merged common ancestors
HandlerTextLeaf::get_groupPosition(long* groupLevel, long* similarItemsInGroup,
                                   long* positionInGroup)
{
=======
HandlerTextLeaf::get_groupPosition(long* groupLevel, long* similarItemsInGroup,
                                   long* positionInGroup) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_states(AccessibleStates *states) {
||||||| merged common ancestors
HandlerTextLeaf::get_states(AccessibleStates* states)
{
=======
HandlerTextLeaf::get_states(AccessibleStates* states) {
>>>>>>> upstream-releases
  if (!states) {
    return E_INVALIDARG;
  }

  *states = 0;
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_extendedRole(BSTR *extendedRole) { return E_NOTIMPL; }
||||||| merged common ancestors
HandlerTextLeaf::get_extendedRole(BSTR* extendedRole)
{
  return E_NOTIMPL;
}
=======
HandlerTextLeaf::get_extendedRole(BSTR* extendedRole) { return E_NOTIMPL; }
>>>>>>> upstream-releases

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_localizedExtendedRole(BSTR *localizedExtendedRole) {
||||||| merged common ancestors
HandlerTextLeaf::get_localizedExtendedRole(BSTR* localizedExtendedRole)
{
=======
HandlerTextLeaf::get_localizedExtendedRole(BSTR* localizedExtendedRole) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_nExtendedStates(long *nExtendedStates) {
||||||| merged common ancestors
HandlerTextLeaf::get_nExtendedStates(long* nExtendedStates)
{
=======
HandlerTextLeaf::get_nExtendedStates(long* nExtendedStates) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
HandlerTextLeaf::get_extendedStates(long maxExtendedStates,
<<<<<<< HEAD
                                    BSTR **extendedStates,
                                    long *nExtendedStates) {
||||||| merged common ancestors
                                    BSTR** extendedStates,
                                    long* nExtendedStates)
{
=======
                                    BSTR** extendedStates,
                                    long* nExtendedStates) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
HandlerTextLeaf::get_localizedExtendedStates(long maxLocalizedExtendedStates,
<<<<<<< HEAD
                                             BSTR **localizedExtendedStates,
                                             long *nLocalizedExtendedStates) {
||||||| merged common ancestors
                                             BSTR** localizedExtendedStates,
                                             long* nLocalizedExtendedStates)
{
=======
                                             BSTR** localizedExtendedStates,
                                             long* nLocalizedExtendedStates) {
>>>>>>> upstream-releases
  return E_NOTIMPL;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_uniqueID(long *uniqueID) {
||||||| merged common ancestors
HandlerTextLeaf::get_uniqueID(long* uniqueID)
{
=======
HandlerTextLeaf::get_uniqueID(long* uniqueID) {
>>>>>>> upstream-releases
  if (!uniqueID) {
    return E_INVALIDARG;
  }

  *uniqueID = mData.mTextId;
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_windowHandle(HWND *windowHandle) {
||||||| merged common ancestors
HandlerTextLeaf::get_windowHandle(HWND* windowHandle)
{
=======
HandlerTextLeaf::get_windowHandle(HWND* windowHandle) {
>>>>>>> upstream-releases
  if (!windowHandle) {
    return E_INVALIDARG;
  }

  *windowHandle = mHwnd;
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_indexInParent(long *indexInParent) {
||||||| merged common ancestors
HandlerTextLeaf::get_indexInParent(long* indexInParent)
{
=======
HandlerTextLeaf::get_indexInParent(long* indexInParent) {
>>>>>>> upstream-releases
  if (!indexInParent) {
    return E_INVALIDARG;
  }

  *indexInParent = mIndexInParent;
  return S_OK;
}

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_locale(IA2Locale *locale) { return E_NOTIMPL; }
||||||| merged common ancestors
HandlerTextLeaf::get_locale(IA2Locale* locale)
{
  return E_NOTIMPL;
}
=======
HandlerTextLeaf::get_locale(IA2Locale* locale) { return E_NOTIMPL; }
>>>>>>> upstream-releases

HRESULT
<<<<<<< HEAD
HandlerTextLeaf::get_attributes(BSTR *attributes) { return E_NOTIMPL; }
||||||| merged common ancestors
HandlerTextLeaf::get_attributes(BSTR* attributes)
{
  return E_NOTIMPL;
}
=======
HandlerTextLeaf::get_attributes(BSTR* attributes) { return E_NOTIMPL; }
>>>>>>> upstream-releases

/*** IServiceProvider ***/

HRESULT
HandlerTextLeaf::QueryService(REFGUID aServiceId, REFIID aIid,
<<<<<<< HEAD
                              void **aOutInterface) {
||||||| merged common ancestors
                              void** aOutInterface)
{
=======
                              void** aOutInterface) {
>>>>>>> upstream-releases
  if (aIid == IID_IAccessible2) {
    RefPtr<IAccessible2> ia2(this);
    ia2.forget(aOutInterface);
    return S_OK;
  }

  return E_INVALIDARG;
}

}  // namespace a11y
}  // namespace mozilla
