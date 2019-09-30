/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsIDocumentViewerPrint_h___
#define nsIDocumentViewerPrint_h___

#include "nsISupports.h"
#include "mozilla/UniquePtr.h"

namespace mozilla {
class PresShell;
class ServoStyleSet;
<<<<<<< HEAD
}  // namespace mozilla
class nsIPresShell;
||||||| merged common ancestors
} // namespace mozilla
class nsIPresShell;
=======
}  // namespace mozilla
>>>>>>> upstream-releases
class nsPresContext;
class nsViewManager;

// {c6f255cf-cadd-4382-b57f-cd2a9874169b}
#define NS_IDOCUMENT_VIEWER_PRINT_IID                \
  {                                                  \
    0xc6f255cf, 0xcadd, 0x4382, {                    \
      0xb5, 0x7f, 0xcd, 0x2a, 0x98, 0x74, 0x16, 0x9b \
    }                                                \
  }

/**
 * A DocumentViewerPrint is an INTERNAL Interface used for interaction
 * between the DocumentViewer and nsPrintJob.
 */
class nsIDocumentViewerPrint : public nsISupports {
 public:
  NS_DECLARE_STATIC_IID_ACCESSOR(NS_IDOCUMENT_VIEWER_PRINT_IID)

  virtual void SetIsPrinting(bool aIsPrinting) = 0;
  virtual bool GetIsPrinting() = 0;

  virtual void SetIsPrintPreview(bool aIsPrintPreview) = 0;
  virtual bool GetIsPrintPreview() = 0;

<<<<<<< HEAD
  // The style set returned by CreateStyleSet is in the middle of an
  // update batch so that the caller can add sheets to it if needed.
  // Callers should call EndUpdate() on it when ready to use.
  virtual mozilla::UniquePtr<mozilla::ServoStyleSet> CreateStyleSet(
      nsIDocument* aDocument) = 0;

||||||| merged common ancestors
  // The style set returned by CreateStyleSet is in the middle of an
  // update batch so that the caller can add sheets to it if needed.
  // Callers should call EndUpdate() on it when ready to use.
  virtual mozilla::UniquePtr<mozilla::ServoStyleSet>
    CreateStyleSet(nsIDocument* aDocument) = 0;

=======
>>>>>>> upstream-releases
  /**
   * This is used by nsPagePrintTimer to make nsDocumentViewer::Destroy()
   * a no-op until printing is finished.  That prevents the nsDocumentViewer
   * and its document, presshell and prescontext from going away.
   */
  virtual void IncrementDestroyBlockedCount() = 0;
  virtual void DecrementDestroyBlockedCount() = 0;

  virtual void OnDonePrinting() = 0;

  /**
   * Returns true is InitializeForPrintPreview() has been called.
   */
  virtual bool IsInitializedForPrintPreview() = 0;

  /**
   * Marks this viewer to be used for print preview.
   */
  virtual void InitializeForPrintPreview() = 0;

  /**
   * Replaces the current presentation with print preview presentation.
   */
  virtual void SetPrintPreviewPresentation(nsViewManager* aViewManager,
                                           nsPresContext* aPresContext,
                                           mozilla::PresShell* aPresShell) = 0;
};

NS_DEFINE_STATIC_IID_ACCESSOR(nsIDocumentViewerPrint,
                              NS_IDOCUMENT_VIEWER_PRINT_IID)

/* Use this macro when declaring classes that implement this interface. */
<<<<<<< HEAD
#define NS_DECL_NSIDOCUMENTVIEWERPRINT                          \
  void SetIsPrinting(bool aIsPrinting) override;                \
  bool GetIsPrinting() override;                                \
  void SetIsPrintPreview(bool aIsPrintPreview) override;        \
  bool GetIsPrintPreview() override;                            \
  mozilla::UniquePtr<mozilla::ServoStyleSet> CreateStyleSet(    \
      nsIDocument* aDocument) override;                         \
  void IncrementDestroyBlockedCount() override;                 \
  void DecrementDestroyBlockedCount() override;                 \
  void OnDonePrinting() override;                               \
  bool IsInitializedForPrintPreview() override;                 \
  void InitializeForPrintPreview() override;                    \
||||||| merged common ancestors
#define NS_DECL_NSIDOCUMENTVIEWERPRINT \
  void SetIsPrinting(bool aIsPrinting) override; \
  bool GetIsPrinting() override; \
  void SetIsPrintPreview(bool aIsPrintPreview) override; \
  bool GetIsPrintPreview() override; \
  mozilla::UniquePtr<mozilla::ServoStyleSet> \
    CreateStyleSet(nsIDocument* aDocument) override; \
  void IncrementDestroyBlockedCount() override; \
  void DecrementDestroyBlockedCount() override; \
  void OnDonePrinting() override; \
  bool IsInitializedForPrintPreview() override; \
  void InitializeForPrintPreview() override; \
=======
#define NS_DECL_NSIDOCUMENTVIEWERPRINT                          \
  void SetIsPrinting(bool aIsPrinting) override;                \
  bool GetIsPrinting() override;                                \
  void SetIsPrintPreview(bool aIsPrintPreview) override;        \
  bool GetIsPrintPreview() override;                            \
  void IncrementDestroyBlockedCount() override;                 \
  void DecrementDestroyBlockedCount() override;                 \
  void OnDonePrinting() override;                               \
  bool IsInitializedForPrintPreview() override;                 \
  void InitializeForPrintPreview() override;                    \
>>>>>>> upstream-releases
  void SetPrintPreviewPresentation(nsViewManager* aViewManager, \
                                   nsPresContext* aPresContext, \
                                   mozilla::PresShell* aPresShell) override;

#endif /* nsIDocumentViewerPrint_h___ */
