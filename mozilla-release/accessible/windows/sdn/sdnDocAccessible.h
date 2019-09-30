/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_a11y_sdnDocAccessible_h_
#define mozilla_a11y_sdnDocAccessible_h_

#include "ISimpleDOM.h"
#include "IUnknownImpl.h"

#include "DocAccessibleWrap.h"

namespace mozilla {
namespace a11y {

<<<<<<< HEAD
class sdnDocAccessible final : public ISimpleDOMDocument {
 public:
  explicit sdnDocAccessible(DocAccessibleWrap *aAccessible)
      : mAccessible(aAccessible){};
  ~sdnDocAccessible(){};
||||||| merged common ancestors
class sdnDocAccessible final : public ISimpleDOMDocument
{
public:
  explicit sdnDocAccessible(DocAccessibleWrap* aAccessible) : mAccessible(aAccessible) {};
  ~sdnDocAccessible() { };
=======
class sdnDocAccessible final : public ISimpleDOMDocument {
 public:
  explicit sdnDocAccessible(DocAccessibleWrap* aAccessible)
      : mAccessible(aAccessible){};
  ~sdnDocAccessible(){};
>>>>>>> upstream-releases

  DECL_IUNKNOWN

  // ISimpleDOMDocument
  virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_URL(
<<<<<<< HEAD
      /* [out] */ BSTR __RPC_FAR *url);
||||||| merged common ancestors
    /* [out] */ BSTR __RPC_FAR *url);
=======
      /* [out] */ BSTR __RPC_FAR* url);
>>>>>>> upstream-releases

  virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_title(
<<<<<<< HEAD
      /* [out] */ BSTR __RPC_FAR *title);
||||||| merged common ancestors
    /* [out] */ BSTR __RPC_FAR *title);
=======
      /* [out] */ BSTR __RPC_FAR* title);
>>>>>>> upstream-releases

  virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_mimeType(
<<<<<<< HEAD
      /* [out] */ BSTR __RPC_FAR *mimeType);
||||||| merged common ancestors
    /* [out] */ BSTR __RPC_FAR *mimeType);
=======
      /* [out] */ BSTR __RPC_FAR* mimeType);
>>>>>>> upstream-releases

  virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_docType(
<<<<<<< HEAD
      /* [out] */ BSTR __RPC_FAR *docType);
||||||| merged common ancestors
    /* [out] */ BSTR __RPC_FAR *docType);
=======
      /* [out] */ BSTR __RPC_FAR* docType);
>>>>>>> upstream-releases

  virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_nameSpaceURIForID(
<<<<<<< HEAD
      /* [in] */ short nameSpaceID,
      /* [out] */ BSTR __RPC_FAR *nameSpaceURI);
||||||| merged common ancestors
    /* [in] */ short nameSpaceID,
    /* [out] */ BSTR __RPC_FAR *nameSpaceURI);
=======
      /* [in] */ short nameSpaceID,
      /* [out] */ BSTR __RPC_FAR* nameSpaceURI);
>>>>>>> upstream-releases

  virtual /* [id] */ HRESULT STDMETHODCALLTYPE put_alternateViewMediaTypes(
<<<<<<< HEAD
      /* [in] */ BSTR __RPC_FAR *commaSeparatedMediaTypes);
||||||| merged common ancestors
    /* [in] */ BSTR __RPC_FAR *commaSeparatedMediaTypes);
=======
      /* [in] */ BSTR __RPC_FAR* commaSeparatedMediaTypes);
>>>>>>> upstream-releases

 protected:
  RefPtr<DocAccessibleWrap> mAccessible;
};

}  // namespace a11y
}  // namespace mozilla

#endif
