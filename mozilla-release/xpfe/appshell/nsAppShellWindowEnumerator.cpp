/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsAppShellWindowEnumerator.h"

#include "nsIContentViewer.h"
#include "nsIDocShell.h"
#include "mozilla/dom/Document.h"
#include "nsIDOMWindow.h"
#include "nsIFactory.h"
#include "nsIInterfaceRequestor.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsIXULWindow.h"
#include "mozilla/dom/Element.h"

#include "nsWindowMediator.h"

using mozilla::dom::Document;
using mozilla::dom::Element;

//
// static helper functions
//

<<<<<<< HEAD
static void GetAttribute(nsIXULWindow *inWindow, const nsAString &inAttribute,
                         nsAString &outValue);
static void GetWindowType(nsIXULWindow *inWindow, nsString &outType);
||||||| merged common ancestors
static void GetAttribute(nsIXULWindow *inWindow, const nsAString &inAttribute,
                         nsAString &outValue);
static void GetWindowType(nsIXULWindow* inWindow, nsString &outType);
=======
static void GetAttribute(nsIXULWindow* inWindow, const nsAString& inAttribute,
                         nsAString& outValue);
static void GetWindowType(nsIXULWindow* inWindow, nsString& outType);
>>>>>>> upstream-releases

<<<<<<< HEAD
static Element *GetElementFromDocShell(nsIDocShell *aShell) {
||||||| merged common ancestors
static Element* GetElementFromDocShell(nsIDocShell *aShell)
{
=======
static Element* GetElementFromDocShell(nsIDocShell* aShell) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIContentViewer> cv;
  aShell->GetContentViewer(getter_AddRefs(cv));
  if (cv) {
    RefPtr<Document> doc = cv->GetDocument();
    if (doc) {
      return doc->GetDocumentElement();
    }
  }

  return nullptr;
}

// generic "retrieve the value of a XUL attribute" function
<<<<<<< HEAD
void GetAttribute(nsIXULWindow *inWindow, const nsAString &inAttribute,
                  nsAString &outValue) {
||||||| merged common ancestors
void GetAttribute(nsIXULWindow *inWindow, const nsAString &inAttribute,
                  nsAString &outValue)
{
=======
void GetAttribute(nsIXULWindow* inWindow, const nsAString& inAttribute,
                  nsAString& outValue) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIDocShell> shell;
  if (inWindow && NS_SUCCEEDED(inWindow->GetDocShell(getter_AddRefs(shell)))) {
    RefPtr<Element> webshellElement = GetElementFromDocShell(shell);
    if (webshellElement) {
      webshellElement->GetAttribute(inAttribute, outValue);
    }
  }
}

// retrieve the window type, stored as the value of a particular
// attribute in its XUL window tag
<<<<<<< HEAD
void GetWindowType(nsIXULWindow *aWindow, nsString &outType) {
||||||| merged common ancestors
void GetWindowType(nsIXULWindow* aWindow, nsString &outType)
{
=======
void GetWindowType(nsIXULWindow* aWindow, nsString& outType) {
>>>>>>> upstream-releases
  GetAttribute(aWindow, NS_LITERAL_STRING("windowtype"), outType);
}

//
// nsWindowInfo
//

<<<<<<< HEAD
nsWindowInfo::nsWindowInfo(nsIXULWindow *inWindow, int32_t inTimeStamp)
    : mWindow(inWindow),
      mTimeStamp(inTimeStamp),
      mZLevel(nsIXULWindow::normalZ) {
||||||| merged common ancestors
nsWindowInfo::nsWindowInfo(nsIXULWindow* inWindow, int32_t inTimeStamp) :
  mWindow(inWindow),mTimeStamp(inTimeStamp),mZLevel(nsIXULWindow::normalZ)
{
=======
nsWindowInfo::nsWindowInfo(nsIXULWindow* inWindow, int32_t inTimeStamp)
    : mWindow(inWindow),
      mTimeStamp(inTimeStamp),
      mZLevel(nsIXULWindow::normalZ) {
>>>>>>> upstream-releases
  ReferenceSelf(true, true);
}

nsWindowInfo::~nsWindowInfo() {}

// return true if the window described by this WindowInfo has a type
// equal to the given type
<<<<<<< HEAD
bool nsWindowInfo::TypeEquals(const nsAString &aType) {
||||||| merged common ancestors
bool nsWindowInfo::TypeEquals(const nsAString &aType)
{
=======
bool nsWindowInfo::TypeEquals(const nsAString& aType) {
>>>>>>> upstream-releases
  nsAutoString rtnString;
  GetWindowType(mWindow, rtnString);
  return rtnString == aType;
}

// insert the struct into their two linked lists, in position after the
// given (independent) method arguments
<<<<<<< HEAD
void nsWindowInfo::InsertAfter(nsWindowInfo *inOlder, nsWindowInfo *inHigher) {
||||||| merged common ancestors
void nsWindowInfo::InsertAfter(nsWindowInfo *inOlder , nsWindowInfo *inHigher)
{
=======
void nsWindowInfo::InsertAfter(nsWindowInfo* inOlder, nsWindowInfo* inHigher) {
>>>>>>> upstream-releases
  if (inOlder) {
    mOlder = inOlder;
    mYounger = inOlder->mYounger;
    mOlder->mYounger = this;
    if (mOlder->mOlder == mOlder) mOlder->mOlder = this;
    mYounger->mOlder = this;
    if (mYounger->mYounger == mYounger) mYounger->mYounger = this;
  }
  if (inHigher) {
    mHigher = inHigher;
    mLower = inHigher->mLower;
    mHigher->mLower = this;
    if (mHigher->mHigher == mHigher) mHigher->mHigher = this;
    mLower->mHigher = this;
    if (mLower->mLower == mLower) mLower->mLower = this;
  }
}

// remove the struct from its linked lists
void nsWindowInfo::Unlink(bool inAge, bool inZ) {
  if (inAge) {
    mOlder->mYounger = mYounger;
    mYounger->mOlder = mOlder;
  }
  if (inZ) {
    mLower->mHigher = mHigher;
    mHigher->mLower = mLower;
  }
  ReferenceSelf(inAge, inZ);
}

// initialize the struct to be a valid linked list of one element
void nsWindowInfo::ReferenceSelf(bool inAge, bool inZ) {
  if (inAge) {
    mYounger = this;
    mOlder = this;
  }
  if (inZ) {
    mLower = this;
    mHigher = this;
  }
}

//
// nsAppShellWindowEnumerator
//

nsAppShellWindowEnumerator::nsAppShellWindowEnumerator(
<<<<<<< HEAD
    const char16_t *aTypeString, nsWindowMediator &aMediator)
    : mWindowMediator(&aMediator),
      mType(aTypeString),
      mCurrentPosition(nullptr) {
||||||| merged common ancestors
    const char16_t* aTypeString,
    nsWindowMediator& aMediator) :
      mWindowMediator(&aMediator), mType(aTypeString), mCurrentPosition(nullptr)
{
=======
    const char16_t* aTypeString, nsWindowMediator& aMediator)
    : mWindowMediator(&aMediator),
      mType(aTypeString),
      mCurrentPosition(nullptr) {
>>>>>>> upstream-releases
  mWindowMediator->AddEnumerator(this);
  NS_ADDREF(mWindowMediator);
}

nsAppShellWindowEnumerator::~nsAppShellWindowEnumerator() {
  mWindowMediator->RemoveEnumerator(this);
  NS_RELEASE(mWindowMediator);
}

// after mCurrentPosition has been initialized to point to the beginning
// of the appropriate list, adjust it if necessary
void nsAppShellWindowEnumerator::AdjustInitialPosition() {
  if (!mType.IsEmpty() && mCurrentPosition &&
      !mCurrentPosition->TypeEquals(mType))
    mCurrentPosition = FindNext();
}

<<<<<<< HEAD
NS_IMETHODIMP nsAppShellWindowEnumerator::HasMoreElements(bool *retval) {
  if (!retval) return NS_ERROR_INVALID_ARG;
||||||| merged common ancestors
NS_IMETHODIMP nsAppShellWindowEnumerator::HasMoreElements(bool *retval)
{
  if (!retval)
    return NS_ERROR_INVALID_ARG;
=======
NS_IMETHODIMP nsAppShellWindowEnumerator::HasMoreElements(bool* retval) {
  if (!retval) return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases

  *retval = mCurrentPosition ? true : false;
  return NS_OK;
}

// if a window is being removed adjust the iterator's current position
<<<<<<< HEAD
void nsAppShellWindowEnumerator::WindowRemoved(nsWindowInfo *inInfo) {
  if (mCurrentPosition == inInfo) mCurrentPosition = FindNext();
||||||| merged common ancestors
void nsAppShellWindowEnumerator::WindowRemoved(nsWindowInfo *inInfo)
{
  if (mCurrentPosition == inInfo)
    mCurrentPosition = FindNext();
=======
void nsAppShellWindowEnumerator::WindowRemoved(nsWindowInfo* inInfo) {
  if (mCurrentPosition == inInfo) mCurrentPosition = FindNext();
>>>>>>> upstream-releases
}

//
// nsASDOMWindowEnumerator
//

<<<<<<< HEAD
nsASDOMWindowEnumerator::nsASDOMWindowEnumerator(const char16_t *aTypeString,
                                                 nsWindowMediator &aMediator)
    : nsAppShellWindowEnumerator(aTypeString, aMediator) {}
||||||| merged common ancestors
nsASDOMWindowEnumerator::nsASDOMWindowEnumerator(
    const char16_t* aTypeString,
    nsWindowMediator& aMediator) :
      nsAppShellWindowEnumerator(aTypeString, aMediator)
{
}
=======
nsASDOMWindowEnumerator::nsASDOMWindowEnumerator(const char16_t* aTypeString,
                                                 nsWindowMediator& aMediator)
    : nsAppShellWindowEnumerator(aTypeString, aMediator) {}
>>>>>>> upstream-releases

nsASDOMWindowEnumerator::~nsASDOMWindowEnumerator() {}

<<<<<<< HEAD
NS_IMETHODIMP nsASDOMWindowEnumerator::GetNext(nsISupports **retval) {
  if (!retval) return NS_ERROR_INVALID_ARG;
||||||| merged common ancestors
NS_IMETHODIMP nsASDOMWindowEnumerator::GetNext(nsISupports **retval)
{
  if (!retval)
    return NS_ERROR_INVALID_ARG;
=======
NS_IMETHODIMP nsASDOMWindowEnumerator::GetNext(nsISupports** retval) {
  if (!retval) return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases

  *retval = nullptr;
  while (mCurrentPosition) {
    nsCOMPtr<nsPIDOMWindowOuter> domWindow;
    nsWindowMediator::GetDOMWindow(mCurrentPosition->mWindow, domWindow);
    mCurrentPosition = FindNext();
    if (domWindow) return CallQueryInterface(domWindow, retval);
  }
  return NS_ERROR_FAILURE;
}

//
// nsASXULWindowEnumerator
//

<<<<<<< HEAD
nsASXULWindowEnumerator::nsASXULWindowEnumerator(const char16_t *aTypeString,
                                                 nsWindowMediator &aMediator)
    : nsAppShellWindowEnumerator(aTypeString, aMediator) {}
||||||| merged common ancestors
nsASXULWindowEnumerator::nsASXULWindowEnumerator(
    const char16_t* aTypeString,
    nsWindowMediator& aMediator) :
      nsAppShellWindowEnumerator(aTypeString, aMediator)
{
}
=======
nsASXULWindowEnumerator::nsASXULWindowEnumerator(const char16_t* aTypeString,
                                                 nsWindowMediator& aMediator)
    : nsAppShellWindowEnumerator(aTypeString, aMediator) {}
>>>>>>> upstream-releases

nsASXULWindowEnumerator::~nsASXULWindowEnumerator() {}

<<<<<<< HEAD
NS_IMETHODIMP nsASXULWindowEnumerator::GetNext(nsISupports **retval) {
  if (!retval) return NS_ERROR_INVALID_ARG;
||||||| merged common ancestors
NS_IMETHODIMP nsASXULWindowEnumerator::GetNext(nsISupports **retval)
{
  if (!retval)
    return NS_ERROR_INVALID_ARG;
=======
NS_IMETHODIMP nsASXULWindowEnumerator::GetNext(nsISupports** retval) {
  if (!retval) return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases

  *retval = nullptr;
  if (mCurrentPosition) {
    CallQueryInterface(mCurrentPosition->mWindow, retval);
    mCurrentPosition = FindNext();
    return NS_OK;
  }
  return NS_ERROR_FAILURE;
}

//
// nsASDOMWindowEarlyToLateEnumerator
//

nsASDOMWindowEarlyToLateEnumerator::nsASDOMWindowEarlyToLateEnumerator(
<<<<<<< HEAD
    const char16_t *aTypeString, nsWindowMediator &aMediator)
    : nsASDOMWindowEnumerator(aTypeString, aMediator) {
||||||| merged common ancestors
    const char16_t *aTypeString,
    nsWindowMediator &aMediator) :
      nsASDOMWindowEnumerator(aTypeString, aMediator)
{
=======
    const char16_t* aTypeString, nsWindowMediator& aMediator)
    : nsASDOMWindowEnumerator(aTypeString, aMediator) {
>>>>>>> upstream-releases
  mCurrentPosition = aMediator.mOldestWindow;
  AdjustInitialPosition();
}

nsASDOMWindowEarlyToLateEnumerator::~nsASDOMWindowEarlyToLateEnumerator() {}

<<<<<<< HEAD
nsWindowInfo *nsASDOMWindowEarlyToLateEnumerator::FindNext() {
  nsWindowInfo *info, *listEnd;
  bool allWindows = mType.IsEmpty();
||||||| merged common ancestors
nsWindowInfo *nsASDOMWindowEarlyToLateEnumerator::FindNext()
{
  nsWindowInfo *info,
               *listEnd;
  bool          allWindows = mType.IsEmpty();
=======
nsWindowInfo* nsASDOMWindowEarlyToLateEnumerator::FindNext() {
  nsWindowInfo *info, *listEnd;
  bool allWindows = mType.IsEmpty();
>>>>>>> upstream-releases

  // see nsXULWindowEarlyToLateEnumerator::FindNext
  if (!mCurrentPosition) return nullptr;

  info = mCurrentPosition->mYounger;
  listEnd = mWindowMediator->mOldestWindow;

  while (info != listEnd) {
    if (allWindows || info->TypeEquals(mType)) return info;
    info = info->mYounger;
  }

  return nullptr;
}

//
// nsASXULWindowEarlyToLateEnumerator
//

nsASXULWindowEarlyToLateEnumerator::nsASXULWindowEarlyToLateEnumerator(
<<<<<<< HEAD
    const char16_t *aTypeString, nsWindowMediator &aMediator)
    : nsASXULWindowEnumerator(aTypeString, aMediator) {
||||||| merged common ancestors
    const char16_t *aTypeString,
    nsWindowMediator &aMediator) :
      nsASXULWindowEnumerator(aTypeString, aMediator)
{
=======
    const char16_t* aTypeString, nsWindowMediator& aMediator)
    : nsASXULWindowEnumerator(aTypeString, aMediator) {
>>>>>>> upstream-releases
  mCurrentPosition = aMediator.mOldestWindow;
  AdjustInitialPosition();
}

nsASXULWindowEarlyToLateEnumerator::~nsASXULWindowEarlyToLateEnumerator() {}

<<<<<<< HEAD
nsWindowInfo *nsASXULWindowEarlyToLateEnumerator::FindNext() {
  nsWindowInfo *info, *listEnd;
  bool allWindows = mType.IsEmpty();
||||||| merged common ancestors
nsWindowInfo *nsASXULWindowEarlyToLateEnumerator::FindNext()
{
  nsWindowInfo *info,
               *listEnd;
  bool          allWindows = mType.IsEmpty();
=======
nsWindowInfo* nsASXULWindowEarlyToLateEnumerator::FindNext() {
  nsWindowInfo *info, *listEnd;
  bool allWindows = mType.IsEmpty();
>>>>>>> upstream-releases

  /* mCurrentPosition null is assumed to mean that the enumerator has run
     its course and is now basically useless. It could also be interpreted
     to mean that it was created at a time when there were no windows. In
     that case it would probably be more appropriate to check to see whether
     windows have subsequently been added. But it's not guaranteed that we'll
     pick up newly added windows anyway (if they occurred previous to our
     current position) so we just don't worry about that. */
  if (!mCurrentPosition) return nullptr;

  info = mCurrentPosition->mYounger;
  listEnd = mWindowMediator->mOldestWindow;

  while (info != listEnd) {
    if (allWindows || info->TypeEquals(mType)) return info;
    info = info->mYounger;
  }

  return nullptr;
}

//
// nsASXULWindowFrontToBackEnumerator
//

nsASXULWindowFrontToBackEnumerator::nsASXULWindowFrontToBackEnumerator(
<<<<<<< HEAD
    const char16_t *aTypeString, nsWindowMediator &aMediator)
    : nsASXULWindowEnumerator(aTypeString, aMediator) {
||||||| merged common ancestors
    const char16_t *aTypeString,
    nsWindowMediator &aMediator) :
      nsASXULWindowEnumerator(aTypeString, aMediator)
{
=======
    const char16_t* aTypeString, nsWindowMediator& aMediator)
    : nsASXULWindowEnumerator(aTypeString, aMediator) {
>>>>>>> upstream-releases
  mCurrentPosition = aMediator.mTopmostWindow;
  AdjustInitialPosition();
}

nsASXULWindowFrontToBackEnumerator::~nsASXULWindowFrontToBackEnumerator() {}

<<<<<<< HEAD
nsWindowInfo *nsASXULWindowFrontToBackEnumerator::FindNext() {
  nsWindowInfo *info, *listEnd;
  bool allWindows = mType.IsEmpty();
||||||| merged common ancestors
nsWindowInfo *nsASXULWindowFrontToBackEnumerator::FindNext()
{
  nsWindowInfo *info,
               *listEnd;
  bool          allWindows = mType.IsEmpty();
=======
nsWindowInfo* nsASXULWindowFrontToBackEnumerator::FindNext() {
  nsWindowInfo *info, *listEnd;
  bool allWindows = mType.IsEmpty();
>>>>>>> upstream-releases

  // see nsXULWindowEarlyToLateEnumerator::FindNext
  if (!mCurrentPosition) return nullptr;

  info = mCurrentPosition->mLower;
  listEnd = mWindowMediator->mTopmostWindow;

  while (info != listEnd) {
    if (allWindows || info->TypeEquals(mType)) return info;
    info = info->mLower;
  }

  return nullptr;
}

//
// nsASXULWindowBackToFrontEnumerator
//

nsASXULWindowBackToFrontEnumerator::nsASXULWindowBackToFrontEnumerator(
<<<<<<< HEAD
    const char16_t *aTypeString, nsWindowMediator &aMediator)
    : nsASXULWindowEnumerator(aTypeString, aMediator) {
  mCurrentPosition =
      aMediator.mTopmostWindow ? aMediator.mTopmostWindow->mHigher : nullptr;
||||||| merged common ancestors
    const char16_t *aTypeString,
    nsWindowMediator &aMediator) :
      nsASXULWindowEnumerator(aTypeString, aMediator)
{
  mCurrentPosition = aMediator.mTopmostWindow ?
                     aMediator.mTopmostWindow->mHigher : nullptr;
=======
    const char16_t* aTypeString, nsWindowMediator& aMediator)
    : nsASXULWindowEnumerator(aTypeString, aMediator) {
  mCurrentPosition =
      aMediator.mTopmostWindow ? aMediator.mTopmostWindow->mHigher : nullptr;
>>>>>>> upstream-releases
  AdjustInitialPosition();
}

nsASXULWindowBackToFrontEnumerator::~nsASXULWindowBackToFrontEnumerator() {}

<<<<<<< HEAD
nsWindowInfo *nsASXULWindowBackToFrontEnumerator::FindNext() {
  nsWindowInfo *info, *listEnd;
  bool allWindows = mType.IsEmpty();
||||||| merged common ancestors
nsWindowInfo *nsASXULWindowBackToFrontEnumerator::FindNext()
{
  nsWindowInfo *info,
               *listEnd;
  bool          allWindows = mType.IsEmpty();
=======
nsWindowInfo* nsASXULWindowBackToFrontEnumerator::FindNext() {
  nsWindowInfo *info, *listEnd;
  bool allWindows = mType.IsEmpty();
>>>>>>> upstream-releases

  // see nsXULWindowEarlyToLateEnumerator::FindNext
  if (!mCurrentPosition) return nullptr;

  info = mCurrentPosition->mHigher;
  listEnd = mWindowMediator->mTopmostWindow;
  if (listEnd) listEnd = listEnd->mHigher;

  while (info != listEnd) {
    if (allWindows || info->TypeEquals(mType)) return info;
    info = info->mHigher;
  }

  return nullptr;
}
