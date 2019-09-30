/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:expandtab:shiftwidth=2:tabstop=2:
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef _ACCESSIBLE_ACTION_H
#define _ACCESSIBLE_ACTION_H

#include "nsISupports.h"

#include "AccessibleAction.h"

namespace mozilla {
namespace a11y {

class ia2AccessibleAction : public IAccessibleAction {
 public:
  // IUnknown
  STDMETHODIMP QueryInterface(REFIID, void **);

  // IAccessibleAction
  virtual HRESULT STDMETHODCALLTYPE nActions(
      /* [retval][out] */ long* nActions);

  virtual HRESULT STDMETHODCALLTYPE doAction(
      /* [in] */ long actionIndex);

  virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_description(
      /* [in] */ long actionIndex,
      /* [retval][out] */ BSTR* description);

  virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_keyBinding(
      /* [in] */ long actionIndex,
      /* [in] */ long nMaxBinding,
      /* [length_is][length_is][size_is][size_is][out] */ BSTR** keyBinding,
      /* [retval][out] */ long* nBinding);

  virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_name(
      /* [in] */ long actionIndex,
      /* [retval][out] */ BSTR* name);

  virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_localizedName(
      /* [in] */ long actionIndex,
<<<<<<< HEAD
      /* [retval][out] */ BSTR *localizedName);
||||||| merged common ancestors
      /* [retval][out] */ BSTR *localizedName);

=======
      /* [retval][out] */ BSTR* localizedName);
>>>>>>> upstream-releases
};

}  // namespace a11y
}  // namespace mozilla

#define FORWARD_IACCESSIBLEACTION(Class)                                       \
<<<<<<< HEAD
  virtual HRESULT STDMETHODCALLTYPE nActions(long *nActions) {                 \
    return Class::nActions(nActions);                                          \
  }                                                                            \
||||||| merged common ancestors
virtual HRESULT STDMETHODCALLTYPE nActions(long *nActions)                     \
{                                                                              \
  return Class::nActions(nActions);                                            \
}                                                                              \
=======
  virtual HRESULT STDMETHODCALLTYPE nActions(long* nActions) {                 \
    return Class::nActions(nActions);                                          \
  }                                                                            \
>>>>>>> upstream-releases
                                                                               \
  virtual HRESULT STDMETHODCALLTYPE doAction(long actionIndex) {               \
    return Class::doAction(actionIndex);                                       \
  }                                                                            \
                                                                               \
<<<<<<< HEAD
  virtual HRESULT STDMETHODCALLTYPE get_description(long actionIndex,          \
                                                    BSTR *description) {       \
    return Class::get_description(actionIndex, description);                   \
  }                                                                            \
||||||| merged common ancestors
virtual HRESULT STDMETHODCALLTYPE get_description(long actionIndex,            \
                                                  BSTR *description)           \
{                                                                              \
  return Class::get_description(actionIndex, description);                     \
}                                                                              \
=======
  virtual HRESULT STDMETHODCALLTYPE get_description(long actionIndex,          \
                                                    BSTR* description) {       \
    return Class::get_description(actionIndex, description);                   \
  }                                                                            \
>>>>>>> upstream-releases
                                                                               \
<<<<<<< HEAD
  virtual HRESULT STDMETHODCALLTYPE get_keyBinding(                            \
      long actionIndex, long nMaxBinding, BSTR **keyBinding, long *nBinding) { \
    return Class::get_keyBinding(actionIndex, nMaxBinding, keyBinding,         \
                                 nBinding);                                    \
  }                                                                            \
||||||| merged common ancestors
virtual HRESULT STDMETHODCALLTYPE get_keyBinding(long actionIndex,             \
                                                 long nMaxBinding,             \
                                                 BSTR **keyBinding,            \
                                                 long *nBinding)               \
{                                                                              \
  return Class::get_keyBinding(actionIndex, nMaxBinding, keyBinding, nBinding);\
}                                                                              \
=======
  virtual HRESULT STDMETHODCALLTYPE get_keyBinding(                            \
      long actionIndex, long nMaxBinding, BSTR** keyBinding, long* nBinding) { \
    return Class::get_keyBinding(actionIndex, nMaxBinding, keyBinding,         \
                                 nBinding);                                    \
  }                                                                            \
>>>>>>> upstream-releases
                                                                               \
<<<<<<< HEAD
  virtual HRESULT STDMETHODCALLTYPE get_name(long actionIndex, BSTR *name) {   \
    return Class::get_name(actionIndex, name);                                 \
  }                                                                            \
||||||| merged common ancestors
virtual HRESULT STDMETHODCALLTYPE get_name(long actionIndex, BSTR *name)       \
{                                                                              \
  return Class::get_name(actionIndex, name);                                   \
}                                                                              \
                                                                               \
virtual HRESULT STDMETHODCALLTYPE get_localizedName(long actionIndex,          \
                                                    BSTR *localizedName)       \
{                                                                              \
  return Class::get_localizedName(actionIndex, localizedName);                 \
}                                                                              \
=======
  virtual HRESULT STDMETHODCALLTYPE get_name(long actionIndex, BSTR* name) {   \
    return Class::get_name(actionIndex, name);                                 \
  }                                                                            \
>>>>>>> upstream-releases
                                                                               \
<<<<<<< HEAD
  virtual HRESULT STDMETHODCALLTYPE get_localizedName(long actionIndex,        \
                                                      BSTR *localizedName) {   \
    return Class::get_localizedName(actionIndex, localizedName);               \
  }
||||||| merged common ancestors
=======
  virtual HRESULT STDMETHODCALLTYPE get_localizedName(long actionIndex,        \
                                                      BSTR* localizedName) {   \
    return Class::get_localizedName(actionIndex, localizedName);               \
  }
>>>>>>> upstream-releases

#endif
