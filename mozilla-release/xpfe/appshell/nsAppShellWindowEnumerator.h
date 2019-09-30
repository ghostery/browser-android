/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsAppShellWindowEnumerator_h
#define nsAppShellWindowEnumerator_h

#include "nsCOMPtr.h"
#include "nsString.h"

#include "nsSimpleEnumerator.h"
#include "nsIXULWindow.h"

class nsWindowMediator;

//
// nsWindowInfo
//

<<<<<<< HEAD
struct nsWindowInfo {
  nsWindowInfo(nsIXULWindow *inWindow, int32_t inTimeStamp);
||||||| merged common ancestors
struct nsWindowInfo
{
  nsWindowInfo(nsIXULWindow* inWindow, int32_t inTimeStamp);
=======
struct nsWindowInfo {
  nsWindowInfo(nsIXULWindow* inWindow, int32_t inTimeStamp);
>>>>>>> upstream-releases
  ~nsWindowInfo();

  nsCOMPtr<nsIXULWindow> mWindow;
  int32_t mTimeStamp;
  uint32_t mZLevel;

  // each struct is in two, independent, circular, doubly-linked lists
<<<<<<< HEAD
  nsWindowInfo *mYounger,  // next younger in sequence
      *mOlder;
  nsWindowInfo *mLower,  // next lower in z-order
      *mHigher;

  bool TypeEquals(const nsAString &aType);
  void InsertAfter(nsWindowInfo *inOlder, nsWindowInfo *inHigher);
  void Unlink(bool inAge, bool inZ);
  void ReferenceSelf(bool inAge, bool inZ);
||||||| merged common ancestors
  nsWindowInfo              *mYounger, // next younger in sequence
                            *mOlder;
  nsWindowInfo              *mLower,   // next lower in z-order
                            *mHigher;

  bool TypeEquals(const nsAString &aType);
  void   InsertAfter(nsWindowInfo *inOlder, nsWindowInfo *inHigher);
  void   Unlink(bool inAge, bool inZ);
  void   ReferenceSelf(bool inAge, bool inZ);
=======
  nsWindowInfo *mYounger,  // next younger in sequence
      *mOlder;
  nsWindowInfo *mLower,  // next lower in z-order
      *mHigher;

  bool TypeEquals(const nsAString& aType);
  void InsertAfter(nsWindowInfo* inOlder, nsWindowInfo* inHigher);
  void Unlink(bool inAge, bool inZ);
  void ReferenceSelf(bool inAge, bool inZ);
>>>>>>> upstream-releases
};

//
// virtual enumerators
//

class nsAppShellWindowEnumerator : public nsSimpleEnumerator {
  friend class nsWindowMediator;

<<<<<<< HEAD
 public:
  nsAppShellWindowEnumerator(const char16_t *aTypeString,
                             nsWindowMediator &inMediator);
  NS_IMETHOD GetNext(nsISupports **retval) override = 0;
  NS_IMETHOD HasMoreElements(bool *retval) override;

 protected:
||||||| merged common ancestors
friend class nsWindowMediator;

public:
  nsAppShellWindowEnumerator(const char16_t* aTypeString,
                             nsWindowMediator& inMediator);
  NS_IMETHOD GetNext(nsISupports **retval) override = 0;
  NS_IMETHOD HasMoreElements(bool *retval) override;

protected:

=======
 public:
  nsAppShellWindowEnumerator(const char16_t* aTypeString,
                             nsWindowMediator& inMediator);
  NS_IMETHOD GetNext(nsISupports** retval) override = 0;
  NS_IMETHOD HasMoreElements(bool* retval) override;

 protected:
>>>>>>> upstream-releases
  ~nsAppShellWindowEnumerator() override;

  void AdjustInitialPosition();
  virtual nsWindowInfo* FindNext() = 0;

  void WindowRemoved(nsWindowInfo* inInfo);

<<<<<<< HEAD
  nsWindowMediator *mWindowMediator;
  nsString mType;
  nsWindowInfo *mCurrentPosition;
||||||| merged common ancestors
  nsWindowMediator *mWindowMediator;
  nsString          mType;
  nsWindowInfo     *mCurrentPosition;
=======
  nsWindowMediator* mWindowMediator;
  nsString mType;
  nsWindowInfo* mCurrentPosition;
>>>>>>> upstream-releases
};

class nsASDOMWindowEnumerator : public nsAppShellWindowEnumerator {
<<<<<<< HEAD
 public:
  nsASDOMWindowEnumerator(const char16_t *aTypeString,
                          nsWindowMediator &inMediator);
||||||| merged common ancestors

public:
  nsASDOMWindowEnumerator(const char16_t* aTypeString,
                          nsWindowMediator& inMediator);
=======
 public:
  nsASDOMWindowEnumerator(const char16_t* aTypeString,
                          nsWindowMediator& inMediator);
>>>>>>> upstream-releases
  virtual ~nsASDOMWindowEnumerator();
  NS_IMETHOD GetNext(nsISupports** retval) override;
};

class nsASXULWindowEnumerator : public nsAppShellWindowEnumerator {
<<<<<<< HEAD
 public:
  nsASXULWindowEnumerator(const char16_t *aTypeString,
                          nsWindowMediator &inMediator);
||||||| merged common ancestors

public:
  nsASXULWindowEnumerator(const char16_t* aTypeString,
                          nsWindowMediator& inMediator);
=======
 public:
  nsASXULWindowEnumerator(const char16_t* aTypeString,
                          nsWindowMediator& inMediator);
>>>>>>> upstream-releases
  virtual ~nsASXULWindowEnumerator();
  NS_IMETHOD GetNext(nsISupports** retval) override;

  const nsID &DefaultInterface() override { return NS_GET_IID(nsIXULWindow); }
};

//
// concrete enumerators
//

class nsASDOMWindowEarlyToLateEnumerator : public nsASDOMWindowEnumerator {
<<<<<<< HEAD
 public:
  nsASDOMWindowEarlyToLateEnumerator(const char16_t *aTypeString,
                                     nsWindowMediator &inMediator);
||||||| merged common ancestors

public:
  nsASDOMWindowEarlyToLateEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);
=======
 public:
  nsASDOMWindowEarlyToLateEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);
>>>>>>> upstream-releases

  virtual ~nsASDOMWindowEarlyToLateEnumerator();

<<<<<<< HEAD
 protected:
  virtual nsWindowInfo *FindNext() override;
||||||| merged common ancestors
protected:
  virtual nsWindowInfo *FindNext() override;
=======
 protected:
  virtual nsWindowInfo* FindNext() override;
>>>>>>> upstream-releases
};

class nsASXULWindowEarlyToLateEnumerator : public nsASXULWindowEnumerator {
<<<<<<< HEAD
 public:
  nsASXULWindowEarlyToLateEnumerator(const char16_t *aTypeString,
                                     nsWindowMediator &inMediator);
||||||| merged common ancestors

public:
  nsASXULWindowEarlyToLateEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);
=======
 public:
  nsASXULWindowEarlyToLateEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);
>>>>>>> upstream-releases

  virtual ~nsASXULWindowEarlyToLateEnumerator();

<<<<<<< HEAD
 protected:
  virtual nsWindowInfo *FindNext() override;
||||||| merged common ancestors
protected:
  virtual nsWindowInfo *FindNext() override;
};

class nsASDOMWindowFrontToBackEnumerator : public nsASDOMWindowEnumerator {

public:
  nsASDOMWindowFrontToBackEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);

  virtual ~nsASDOMWindowFrontToBackEnumerator();

protected:
  virtual nsWindowInfo *FindNext() override;
=======
 protected:
  virtual nsWindowInfo* FindNext() override;
>>>>>>> upstream-releases
};

class nsASXULWindowFrontToBackEnumerator : public nsASXULWindowEnumerator {
<<<<<<< HEAD
 public:
  nsASXULWindowFrontToBackEnumerator(const char16_t *aTypeString,
                                     nsWindowMediator &inMediator);
||||||| merged common ancestors

public:
  nsASXULWindowFrontToBackEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);
=======
 public:
  nsASXULWindowFrontToBackEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);
>>>>>>> upstream-releases

  virtual ~nsASXULWindowFrontToBackEnumerator();

<<<<<<< HEAD
 protected:
  virtual nsWindowInfo *FindNext() override;
||||||| merged common ancestors
protected:
  virtual nsWindowInfo *FindNext() override;
};

class nsASDOMWindowBackToFrontEnumerator : public nsASDOMWindowEnumerator {

public:
  nsASDOMWindowBackToFrontEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);

  virtual ~nsASDOMWindowBackToFrontEnumerator();

protected:
  virtual nsWindowInfo *FindNext() override;
=======
 protected:
  virtual nsWindowInfo* FindNext() override;
>>>>>>> upstream-releases
};

class nsASXULWindowBackToFrontEnumerator : public nsASXULWindowEnumerator {
<<<<<<< HEAD
 public:
  nsASXULWindowBackToFrontEnumerator(const char16_t *aTypeString,
                                     nsWindowMediator &inMediator);
||||||| merged common ancestors

public:
  nsASXULWindowBackToFrontEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);
=======
 public:
  nsASXULWindowBackToFrontEnumerator(const char16_t* aTypeString,
                                     nsWindowMediator& inMediator);
>>>>>>> upstream-releases

  virtual ~nsASXULWindowBackToFrontEnumerator();

<<<<<<< HEAD
 protected:
  virtual nsWindowInfo *FindNext() override;
||||||| merged common ancestors
protected:
  virtual nsWindowInfo *FindNext() override;
=======
 protected:
  virtual nsWindowInfo* FindNext() override;
>>>>>>> upstream-releases
};

#endif
