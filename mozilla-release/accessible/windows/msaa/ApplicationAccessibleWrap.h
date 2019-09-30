/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:expandtab:shiftwidth=4:tabstop=4:
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_a11y_ApplicationAccessibleWrap_h__
#define mozilla_a11y_ApplicationAccessibleWrap_h__

#include "ApplicationAccessible.h"

#include "AccessibleApplication.h"

namespace mozilla {
namespace a11y {

class ApplicationAccessibleWrap : public ApplicationAccessible,
                                  public IAccessibleApplication {
  ~ApplicationAccessibleWrap() {}

 public:
  // nsISupporst
  NS_DECL_ISUPPORTS_INHERITED

  // nsAccessible
  virtual already_AddRefed<nsIPersistentProperties> NativeAttributes() override;

  // IUnknown
  STDMETHODIMP QueryInterface(REFIID, void **);

  // IAccessibleApplication
  virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_appName(
<<<<<<< HEAD
      /* [retval][out] */ BSTR *name);
||||||| merged common ancestors
            /* [retval][out] */ BSTR *name);
=======
      /* [retval][out] */ BSTR* name);
>>>>>>> upstream-releases

  virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_appVersion(
      /* [retval][out] */ BSTR* version);

  virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_toolkitName(
      /* [retval][out] */ BSTR* name);

  virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_toolkitVersion(
<<<<<<< HEAD
      /* [retval][out] */ BSTR *version);
||||||| merged common ancestors
          /* [retval][out] */ BSTR *version);

=======
      /* [retval][out] */ BSTR* version);
>>>>>>> upstream-releases
};

}  // namespace a11y
}  // namespace mozilla

#endif
