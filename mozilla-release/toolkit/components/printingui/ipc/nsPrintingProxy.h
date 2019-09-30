/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __nsPrintingProxy_h
#define __nsPrintingProxy_h

#include "nsIPrintingPromptService.h"
#include "mozilla/embedding/PPrintingChild.h"

namespace mozilla {
namespace layout {
class PRemotePrintJobChild;
}
<<<<<<< HEAD
}  // namespace mozilla
||||||| merged common ancestors
}
=======
}  // namespace mozilla

class nsPrintingProxy final : public nsIPrintingPromptService,
                              public mozilla::embedding::PPrintingChild {
  friend class mozilla::embedding::PPrintingChild;
>>>>>>> upstream-releases

<<<<<<< HEAD
class nsPrintingProxy final : public nsIPrintingPromptService,
                              public mozilla::embedding::PPrintingChild {
 public:
  static already_AddRefed<nsPrintingProxy> GetInstance();
||||||| merged common ancestors
class nsPrintingProxy final: public nsIPrintingPromptService,
                             public mozilla::embedding::PPrintingChild
{
public:
    static already_AddRefed<nsPrintingProxy> GetInstance();
=======
 public:
  static already_AddRefed<nsPrintingProxy> GetInstance();
>>>>>>> upstream-releases

  NS_DECL_ISUPPORTS
  NS_DECL_NSIPRINTINGPROMPTSERVICE

  /**
   * Used to proxy nsIPrintSettings.savePrintSettingsToPrefs calls to the
   * parent process.
   *
   * @param aFlags - kInitSave* flags from nsIPrintSettings's to specify
   *          which settings to save.
   */
  nsresult SavePrintSettings(nsIPrintSettings* aPS, bool aUsePrinterNamePrefix,
                             uint32_t aFlags);

<<<<<<< HEAD
 protected:
  virtual PPrintProgressDialogChild* AllocPPrintProgressDialogChild() override;
||||||| merged common ancestors
protected:
    virtual PPrintProgressDialogChild*
    AllocPPrintProgressDialogChild() override;
=======
 protected:
  PPrintProgressDialogChild* AllocPPrintProgressDialogChild() final;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPrintProgressDialogChild(
      PPrintProgressDialogChild* aActor) override;
||||||| merged common ancestors
    virtual bool
    DeallocPPrintProgressDialogChild(PPrintProgressDialogChild* aActor) override;
=======
  bool DeallocPPrintProgressDialogChild(
      PPrintProgressDialogChild* aActor) final;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PPrintSettingsDialogChild* AllocPPrintSettingsDialogChild() override;
||||||| merged common ancestors
    virtual PPrintSettingsDialogChild*
    AllocPPrintSettingsDialogChild() override;
=======
  PPrintSettingsDialogChild* AllocPPrintSettingsDialogChild() final;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPPrintSettingsDialogChild(
      PPrintSettingsDialogChild* aActor) override;
||||||| merged common ancestors
    virtual bool
    DeallocPPrintSettingsDialogChild(PPrintSettingsDialogChild* aActor) override;
=======
  bool DeallocPPrintSettingsDialogChild(
      PPrintSettingsDialogChild* aActor) final;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual PRemotePrintJobChild* AllocPRemotePrintJobChild() override;
||||||| merged common ancestors
    virtual PRemotePrintJobChild*
    AllocPRemotePrintJobChild() override;
=======
  PRemotePrintJobChild* AllocPRemotePrintJobChild() final;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual bool DeallocPRemotePrintJobChild(
      PRemotePrintJobChild* aActor) override;
||||||| merged common ancestors
    virtual bool
    DeallocPRemotePrintJobChild(PRemotePrintJobChild* aActor) override;
=======
  bool DeallocPRemotePrintJobChild(PRemotePrintJobChild* aActor) final;
>>>>>>> upstream-releases

 private:
  nsPrintingProxy();

<<<<<<< HEAD
  virtual ~nsPrintingProxy();
||||||| merged common ancestors
    virtual ~nsPrintingProxy();
=======
  ~nsPrintingProxy() final;
>>>>>>> upstream-releases

  nsresult Init();
};

#endif
