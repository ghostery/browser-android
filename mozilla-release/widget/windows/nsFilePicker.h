/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsFilePicker_h__
#define nsFilePicker_h__

#include <windows.h>

#include "nsIFile.h"
#include "nsITimer.h"
#include "nsISimpleEnumerator.h"
#include "nsCOMArray.h"
#include "nsBaseFilePicker.h"
#include "nsString.h"
#include "nsdefs.h"
#include <commdlg.h>
#include <shobjidl.h>
#undef LogSeverity  // SetupAPI.h #defines this as DWORD

class nsILoadContext;

<<<<<<< HEAD
class nsBaseWinFilePicker : public nsBaseFilePicker {
 public:
  NS_IMETHOD GetDefaultString(nsAString &aDefaultString) override;
  NS_IMETHOD SetDefaultString(const nsAString &aDefaultString) override;
  NS_IMETHOD GetDefaultExtension(nsAString &aDefaultExtension) override;
  NS_IMETHOD SetDefaultExtension(const nsAString &aDefaultExtension) override;
||||||| merged common ancestors
class nsBaseWinFilePicker :
  public nsBaseFilePicker
{
public:
  NS_IMETHOD GetDefaultString(nsAString& aDefaultString) override;
  NS_IMETHOD SetDefaultString(const nsAString& aDefaultString) override;
  NS_IMETHOD GetDefaultExtension(nsAString& aDefaultExtension) override;
  NS_IMETHOD SetDefaultExtension(const nsAString& aDefaultExtension) override;
=======
class nsBaseWinFilePicker : public nsBaseFilePicker {
 public:
  NS_IMETHOD GetDefaultString(nsAString& aDefaultString) override;
  NS_IMETHOD SetDefaultString(const nsAString& aDefaultString) override;
  NS_IMETHOD GetDefaultExtension(nsAString& aDefaultExtension) override;
  NS_IMETHOD SetDefaultExtension(const nsAString& aDefaultExtension) override;
>>>>>>> upstream-releases

 protected:
  nsString mDefaultFilePath;
  nsString mDefaultFilename;
  nsString mDefaultExtension;
};

/**
 * Native Windows FileSelector wrapper
 */

class nsFilePicker : public IFileDialogEvents, public nsBaseWinFilePicker {
  virtual ~nsFilePicker();

 public:
  nsFilePicker();

<<<<<<< HEAD
  NS_IMETHOD Init(mozIDOMWindowProxy *aParent, const nsAString &aTitle,
                  int16_t aMode) override;
||||||| merged common ancestors
  NS_IMETHOD Init(mozIDOMWindowProxy *aParent, const nsAString& aTitle, int16_t aMode) override;
=======
  NS_IMETHOD Init(mozIDOMWindowProxy* aParent, const nsAString& aTitle,
                  int16_t aMode) override;
>>>>>>> upstream-releases

  NS_DECL_ISUPPORTS

  // IUnknown's QueryInterface
  STDMETHODIMP QueryInterface(REFIID refiid, void **ppvResult);

  // nsIFilePicker (less what's in nsBaseFilePicker and nsBaseWinFilePicker)
  NS_IMETHOD GetFilterIndex(int32_t* aFilterIndex) override;
  NS_IMETHOD SetFilterIndex(int32_t aFilterIndex) override;
<<<<<<< HEAD
  NS_IMETHOD GetFile(nsIFile **aFile) override;
  NS_IMETHOD GetFileURL(nsIURI **aFileURL) override;
  NS_IMETHOD GetFiles(nsISimpleEnumerator **aFiles) override;
  NS_IMETHOD AppendFilter(const nsAString &aTitle,
                          const nsAString &aFilter) override;
||||||| merged common ancestors
  NS_IMETHOD GetFile(nsIFile * *aFile) override;
  NS_IMETHOD GetFileURL(nsIURI * *aFileURL) override;
  NS_IMETHOD GetFiles(nsISimpleEnumerator **aFiles) override;
  NS_IMETHOD AppendFilter(const nsAString& aTitle, const nsAString& aFilter) override;
=======
  NS_IMETHOD GetFile(nsIFile** aFile) override;
  NS_IMETHOD GetFileURL(nsIURI** aFileURL) override;
  NS_IMETHOD GetFiles(nsISimpleEnumerator** aFiles) override;
  NS_IMETHOD AppendFilter(const nsAString& aTitle,
                          const nsAString& aFilter) override;
>>>>>>> upstream-releases

  // IFileDialogEvents
<<<<<<< HEAD
  HRESULT STDMETHODCALLTYPE OnFileOk(IFileDialog *pfd);
  HRESULT STDMETHODCALLTYPE OnFolderChanging(IFileDialog *pfd,
                                             IShellItem *psiFolder);
  HRESULT STDMETHODCALLTYPE OnFolderChange(IFileDialog *pfd);
  HRESULT STDMETHODCALLTYPE OnSelectionChange(IFileDialog *pfd);
  HRESULT STDMETHODCALLTYPE
  OnShareViolation(IFileDialog *pfd, IShellItem *psi,
                   FDE_SHAREVIOLATION_RESPONSE *pResponse);
  HRESULT STDMETHODCALLTYPE OnTypeChange(IFileDialog *pfd);
  HRESULT STDMETHODCALLTYPE OnOverwrite(IFileDialog *pfd, IShellItem *psi,
                                        FDE_OVERWRITE_RESPONSE *pResponse);

 protected:
||||||| merged common ancestors
  HRESULT STDMETHODCALLTYPE OnFileOk(IFileDialog *pfd);
  HRESULT STDMETHODCALLTYPE OnFolderChanging(IFileDialog *pfd, IShellItem *psiFolder);
  HRESULT STDMETHODCALLTYPE OnFolderChange(IFileDialog *pfd);
  HRESULT STDMETHODCALLTYPE OnSelectionChange(IFileDialog *pfd);
  HRESULT STDMETHODCALLTYPE OnShareViolation(IFileDialog *pfd, IShellItem *psi, FDE_SHAREVIOLATION_RESPONSE *pResponse);
  HRESULT STDMETHODCALLTYPE OnTypeChange(IFileDialog *pfd);
  HRESULT STDMETHODCALLTYPE OnOverwrite(IFileDialog *pfd, IShellItem *psi, FDE_OVERWRITE_RESPONSE *pResponse);

protected:
=======
  HRESULT STDMETHODCALLTYPE OnFileOk(IFileDialog* pfd);
  HRESULT STDMETHODCALLTYPE OnFolderChanging(IFileDialog* pfd,
                                             IShellItem* psiFolder);
  HRESULT STDMETHODCALLTYPE OnFolderChange(IFileDialog* pfd);
  HRESULT STDMETHODCALLTYPE OnSelectionChange(IFileDialog* pfd);
  HRESULT STDMETHODCALLTYPE
  OnShareViolation(IFileDialog* pfd, IShellItem* psi,
                   FDE_SHAREVIOLATION_RESPONSE* pResponse);
  HRESULT STDMETHODCALLTYPE OnTypeChange(IFileDialog* pfd);
  HRESULT STDMETHODCALLTYPE OnOverwrite(IFileDialog* pfd, IShellItem* psi,
                                        FDE_OVERWRITE_RESPONSE* pResponse);

 protected:
>>>>>>> upstream-releases
  /* method from nsBaseFilePicker */
<<<<<<< HEAD
  virtual void InitNative(nsIWidget *aParent, const nsAString &aTitle) override;
  nsresult Show(int16_t *aReturnVal) override;
  nsresult ShowW(int16_t *aReturnVal);
  void GetFilterListArray(nsString &aFilterList);
  bool ShowFolderPicker(const nsString &aInitialDir);
  bool ShowFilePicker(const nsString &aInitialDir);
||||||| merged common ancestors
  virtual void InitNative(nsIWidget *aParent,
                          const nsAString& aTitle) override;
  nsresult Show(int16_t *aReturnVal) override;
  nsresult ShowW(int16_t *aReturnVal);
  void GetFilterListArray(nsString& aFilterList);
  bool ShowFolderPicker(const nsString& aInitialDir);
  bool ShowFilePicker(const nsString& aInitialDir);
=======
  virtual void InitNative(nsIWidget* aParent, const nsAString& aTitle) override;
  nsresult Show(int16_t* aReturnVal) override;
  nsresult ShowW(int16_t* aReturnVal);
  void GetFilterListArray(nsString& aFilterList);
  bool ShowFolderPicker(const nsString& aInitialDir);
  bool ShowFilePicker(const nsString& aInitialDir);
>>>>>>> upstream-releases
  void RememberLastUsedDirectory();
  bool IsPrivacyModeEnabled();
  bool IsDefaultPathLink();
  bool IsDefaultPathHtml();
  void SetDialogHandle(HWND aWnd);
  bool ClosePickerIfNeeded();
  static void PickerCallbackTimerFunc(nsITimer* aTimer, void* aPicker);

  nsCOMPtr<nsILoadContext> mLoadContext;
<<<<<<< HEAD
  nsCOMPtr<nsIWidget> mParentWidget;
  nsString mTitle;
  nsCString mFile;
  nsString mFilterList;
  int16_t mSelectedType;
  nsCOMArray<nsIFile> mFiles;
  static char mLastUsedDirectory[];
  nsString mUnicodeFile;
  static char16_t *mLastUsedUnicodeDirectory;
  HWND mDlgWnd;

  class ComDlgFilterSpec {
   public:
||||||| merged common ancestors
  nsCOMPtr<nsIWidget>    mParentWidget;
  nsString               mTitle;
  nsCString              mFile;
  nsString               mFilterList;
  int16_t                mSelectedType;
  nsCOMArray<nsIFile>    mFiles;
  static char            mLastUsedDirectory[];
  nsString               mUnicodeFile;
  static char16_t      *mLastUsedUnicodeDirectory;
  HWND                   mDlgWnd;

  class ComDlgFilterSpec
  {
  public:
=======
  nsCOMPtr<nsIWidget> mParentWidget;
  nsString mTitle;
  nsCString mFile;
  nsString mFilterList;
  int16_t mSelectedType;
  nsCOMArray<nsIFile> mFiles;
  static char mLastUsedDirectory[];
  nsString mUnicodeFile;
  static char16_t* mLastUsedUnicodeDirectory;
  HWND mDlgWnd;

  class ComDlgFilterSpec {
   public:
>>>>>>> upstream-releases
    ComDlgFilterSpec() {}
    ~ComDlgFilterSpec() {}

<<<<<<< HEAD
    const uint32_t Length() { return mSpecList.Length(); }

    const bool IsEmpty() { return (mSpecList.Length() == 0); }
||||||| merged common ancestors
    const uint32_t Length() {
      return mSpecList.Length();
    }
=======
    const uint32_t Length() { return mSpecList.Length(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    const COMDLG_FILTERSPEC *get() { return mSpecList.Elements(); }
||||||| merged common ancestors
    const bool IsEmpty() {
      return (mSpecList.Length() == 0);
    }
=======
    const bool IsEmpty() { return (mSpecList.Length() == 0); }
>>>>>>> upstream-releases

<<<<<<< HEAD
    void Append(const nsAString &aTitle, const nsAString &aFilter);
||||||| merged common ancestors
    const COMDLG_FILTERSPEC* get() {
      return mSpecList.Elements();
    }
=======
    const COMDLG_FILTERSPEC* get() { return mSpecList.Elements(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
   private:
||||||| merged common ancestors
    void Append(const nsAString& aTitle, const nsAString& aFilter);
  private:
=======
    void Append(const nsAString& aTitle, const nsAString& aFilter);

   private:
>>>>>>> upstream-releases
    AutoTArray<COMDLG_FILTERSPEC, 1> mSpecList;
    AutoTArray<nsString, 2> mStrings;
  };

  ComDlgFilterSpec mComFilterList;
  DWORD mFDECookie;
};

#endif  // nsFilePicker_h__
