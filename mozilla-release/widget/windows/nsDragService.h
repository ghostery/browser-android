/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsDragService_h__
#define nsDragService_h__

#include "nsBaseDragService.h"
#include <windows.h>
#include <shlobj.h>

struct IDataObject;
class nsDataObjCollection;

/**
 * Native Win32 DragService wrapper
 */

class nsDragService : public nsBaseDragService {
 public:
  nsDragService();
  virtual ~nsDragService();

  // nsBaseDragService
<<<<<<< HEAD
  virtual nsresult InvokeDragSessionImpl(
      nsIArray* anArrayTransferables,
      const mozilla::Maybe<mozilla::CSSIntRegion>& aRegion,
      uint32_t aActionType);
||||||| merged common ancestors
  virtual nsresult InvokeDragSessionImpl(nsIArray* anArrayTransferables,
                                         const mozilla::Maybe<mozilla::CSSIntRegion>& aRegion,
                                         uint32_t aActionType);
=======
  MOZ_CAN_RUN_SCRIPT virtual nsresult InvokeDragSessionImpl(
      nsIArray* anArrayTransferables,
      const mozilla::Maybe<mozilla::CSSIntRegion>& aRegion,
      uint32_t aActionType);
>>>>>>> upstream-releases

  // nsIDragSession
<<<<<<< HEAD
  NS_IMETHOD GetData(nsITransferable* aTransferable, uint32_t anItem) override;
  NS_IMETHOD GetNumDropItems(uint32_t* aNumItems) override;
  NS_IMETHOD IsDataFlavorSupported(const char* aDataFlavor,
                                   bool* _retval) override;
  NS_IMETHOD EndDragSession(bool aDoneDrag, uint32_t aKeyModifiers) override;
  NS_IMETHOD UpdateDragImage(nsINode* aImage, int32_t aImageX,
                             int32_t aImageY) override;
||||||| merged common ancestors
  NS_IMETHOD GetData(nsITransferable * aTransferable, uint32_t anItem) override;
  NS_IMETHOD GetNumDropItems(uint32_t * aNumItems) override;
  NS_IMETHOD IsDataFlavorSupported(const char *aDataFlavor, bool *_retval) override;
  NS_IMETHOD EndDragSession(bool aDoneDrag, uint32_t aKeyModifiers) override;
  NS_IMETHOD UpdateDragImage(nsINode* aImage, int32_t aImageX, int32_t aImageY) override;
=======
  NS_IMETHOD GetData(nsITransferable* aTransferable, uint32_t anItem) override;
  NS_IMETHOD GetNumDropItems(uint32_t* aNumItems) override;
  NS_IMETHOD IsDataFlavorSupported(const char* aDataFlavor,
                                   bool* _retval) override;
  MOZ_CAN_RUN_SCRIPT NS_IMETHOD EndDragSession(bool aDoneDrag,
                                               uint32_t aKeyModifiers) override;
  NS_IMETHOD UpdateDragImage(nsINode* aImage, int32_t aImageX,
                             int32_t aImageY) override;
>>>>>>> upstream-releases

  // native impl.
<<<<<<< HEAD
  NS_IMETHOD SetIDataObject(IDataObject* aDataObj);
  NS_IMETHOD StartInvokingDragSession(IDataObject* aDataObj,
                                      uint32_t aActionType);
||||||| merged common ancestors
  NS_IMETHOD SetIDataObject(IDataObject * aDataObj);
  NS_IMETHOD StartInvokingDragSession(IDataObject * aDataObj,
                                      uint32_t aActionType);
=======
  NS_IMETHOD SetIDataObject(IDataObject* aDataObj);
  MOZ_CAN_RUN_SCRIPT nsresult StartInvokingDragSession(IDataObject* aDataObj,
                                                       uint32_t aActionType);
>>>>>>> upstream-releases

  // A drop occurred within the application vs. outside of it.
  void SetDroppedLocal();

  IDataObject* GetDataObject() { return mDataObject; }

 protected:
  nsDataObjCollection* GetDataObjCollection(IDataObject* aDataObj);

  // determine if we have a single data object or one of our private
  // collections
  bool IsCollectionObject(IDataObject* inDataObj);

  // Create a bitmap for drag operations
  bool CreateDragImage(nsINode* aDOMNode,
                       const mozilla::Maybe<mozilla::CSSIntRegion>& aRegion,
                       SHDRAGIMAGE* psdi);

  IDataObject* mDataObject;
  bool mSentLocalDropEvent;
};

#endif  // nsDragService_h__
