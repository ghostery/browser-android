/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "XULSelectControlAccessible.h"

#include "nsAccessibilityService.h"
#include "DocAccessible.h"

#include "nsIDOMXULSelectCntrlItemEl.h"
#include "nsIDOMXULMultSelectCntrlEl.h"
#include "nsIMutableArray.h"
#include "nsIServiceManager.h"

#include "mozilla/dom/Element.h"
#include "mozilla/dom/KeyboardEventBinding.h"

using namespace mozilla;
using namespace mozilla::a11y;

////////////////////////////////////////////////////////////////////////////////
// XULSelectControlAccessible
////////////////////////////////////////////////////////////////////////////////

XULSelectControlAccessible::XULSelectControlAccessible(nsIContent* aContent,
                                                       DocAccessible* aDoc)
    : AccessibleWrap(aContent, aDoc) {
  mGenericTypes |= eSelect;
  mSelectControl = aContent->AsElement();
}

////////////////////////////////////////////////////////////////////////////////
// XULSelectControlAccessible: Accessible

void XULSelectControlAccessible::Shutdown() {
  mSelectControl = nullptr;
  AccessibleWrap::Shutdown();
}

////////////////////////////////////////////////////////////////////////////////
// XULSelectControlAccessible: SelectAccessible

void XULSelectControlAccessible::SelectedItems(nsTArray<Accessible*>* aItems) {
  // For XUL multi-select control
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> xulMultiSelect =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
=======
      mSelectControl->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  if (xulMultiSelect) {
    int32_t length = 0;
    xulMultiSelect->GetSelectedCount(&length);
    for (int32_t index = 0; index < length; index++) {
<<<<<<< HEAD
      nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm;
      xulMultiSelect->MultiGetSelectedItem(index, getter_AddRefs(itemElm));
      nsCOMPtr<nsINode> itemNode(do_QueryInterface(itemElm));
      Accessible* item = mDoc->GetAccessible(itemNode);
      if (item) aItems->AppendElement(item);
||||||| merged common ancestors
      nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm;
      xulMultiSelect->MultiGetSelectedItem(index, getter_AddRefs(itemElm));
      nsCOMPtr<nsINode> itemNode(do_QueryInterface(itemElm));
      Accessible* item = mDoc->GetAccessible(itemNode);
      if (item)
        aItems->AppendElement(item);
=======
      RefPtr<Element> element;
      xulMultiSelect->MultiGetSelectedItem(index, getter_AddRefs(element));
      Accessible* item = mDoc->GetAccessible(element);
      if (item) aItems->AppendElement(item);
>>>>>>> upstream-releases
    }
  } else {  // Single select?
<<<<<<< HEAD
    nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm;
    mSelectControl->GetSelectedItem(getter_AddRefs(itemElm));
    nsCOMPtr<nsINode> itemNode(do_QueryInterface(itemElm));
    if (itemNode) {
      Accessible* item = mDoc->GetAccessible(itemNode);
      if (item) aItems->AppendElement(item);
||||||| merged common ancestors
    nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm;
    mSelectControl->GetSelectedItem(getter_AddRefs(itemElm));
    nsCOMPtr<nsINode> itemNode(do_QueryInterface(itemElm));
    if (itemNode) {
      Accessible* item = mDoc->GetAccessible(itemNode);
      if (item)
        aItems->AppendElement(item);
=======
    nsCOMPtr<nsIDOMXULSelectControlElement> selectControl =
        mSelectControl->AsXULSelectControl();
    RefPtr<Element> element;
    selectControl->GetSelectedItem(getter_AddRefs(element));
    if (element) {
      Accessible* item = mDoc->GetAccessible(element);
      if (item) aItems->AppendElement(item);
>>>>>>> upstream-releases
    }
  }
}

Accessible* XULSelectControlAccessible::GetSelectedItem(uint32_t aIndex) {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> multiSelectControl =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
=======
      mSelectControl->AsXULMultiSelectControl();
>>>>>>> upstream-releases

  RefPtr<Element> element;
  if (multiSelectControl) {
    multiSelectControl->MultiGetSelectedItem(aIndex, getter_AddRefs(element));
  } else if (aIndex == 0) {
    nsCOMPtr<nsIDOMXULSelectControlElement> selectControl =
        mSelectControl->AsXULSelectControl();
    if (selectControl) {
      selectControl->GetSelectedItem(getter_AddRefs(element));
    }
  }

  return element && mDoc ? mDoc->GetAccessible(element) : nullptr;
}

uint32_t XULSelectControlAccessible::SelectedItemCount() {
  // For XUL multi-select control
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> multiSelectControl =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
=======
      mSelectControl->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  if (multiSelectControl) {
    int32_t count = 0;
    multiSelectControl->GetSelectedCount(&count);
    return count;
  }

  // For XUL single-select control/menulist
  nsCOMPtr<nsIDOMXULSelectControlElement> selectControl =
      mSelectControl->AsXULSelectControl();
  if (selectControl) {
    int32_t index = -1;
    selectControl->GetSelectedIndex(&index);
    return (index >= 0) ? 1 : 0;
  }

  return 0;
}

bool XULSelectControlAccessible::AddItemToSelection(uint32_t aIndex) {
  Accessible* item = GetChildAt(aIndex);
<<<<<<< HEAD
  if (!item) return false;
||||||| merged common ancestors
  if (!item)
    return false;
=======
  if (!item || !item->GetContent()) return false;
>>>>>>> upstream-releases

  nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm =
<<<<<<< HEAD
      do_QueryInterface(item->GetContent());
  if (!itemElm) return false;
||||||| merged common ancestors
    do_QueryInterface(item->GetContent());
  if (!itemElm)
    return false;
=======
      item->GetContent()->AsElement()->AsXULSelectControlItem();
  if (!itemElm) return false;
>>>>>>> upstream-releases

  bool isItemSelected = false;
  itemElm->GetSelected(&isItemSelected);
  if (isItemSelected) return true;

  nsCOMPtr<nsIDOMXULMultiSelectControlElement> multiSelectControl =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
=======
      mSelectControl->AsXULMultiSelectControl();
>>>>>>> upstream-releases

  if (multiSelectControl) {
    multiSelectControl->AddItemToSelection(itemElm);
  } else {
    nsCOMPtr<nsIDOMXULSelectControlElement> selectControl =
        mSelectControl->AsXULSelectControl();
    if (selectControl) {
      selectControl->SetSelectedItem(item->Elm());
    }
  }

  return true;
}

bool XULSelectControlAccessible::RemoveItemFromSelection(uint32_t aIndex) {
  Accessible* item = GetChildAt(aIndex);
<<<<<<< HEAD
  if (!item) return false;
||||||| merged common ancestors
  if (!item)
    return false;
=======
  if (!item || !item->GetContent()) return false;
>>>>>>> upstream-releases

  nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm =
<<<<<<< HEAD
      do_QueryInterface(item->GetContent());
  if (!itemElm) return false;
||||||| merged common ancestors
      do_QueryInterface(item->GetContent());
  if (!itemElm)
    return false;
=======
      item->GetContent()->AsElement()->AsXULSelectControlItem();
  if (!itemElm) return false;
>>>>>>> upstream-releases

  bool isItemSelected = false;
  itemElm->GetSelected(&isItemSelected);
  if (!isItemSelected) return true;

  nsCOMPtr<nsIDOMXULMultiSelectControlElement> multiSelectControl =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
=======
      mSelectControl->AsXULMultiSelectControl();
>>>>>>> upstream-releases

  if (multiSelectControl) {
    multiSelectControl->RemoveItemFromSelection(itemElm);
  } else {
    nsCOMPtr<nsIDOMXULSelectControlElement> selectControl =
        mSelectControl->AsXULSelectControl();
    if (selectControl) {
      selectControl->SetSelectedItem(nullptr);
    }
  }

  return true;
}

bool XULSelectControlAccessible::IsItemSelected(uint32_t aIndex) {
  Accessible* item = GetChildAt(aIndex);
<<<<<<< HEAD
  if (!item) return false;
||||||| merged common ancestors
  if (!item)
    return false;
=======
  if (!item || !item->GetContent()) return false;
>>>>>>> upstream-releases

  nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm =
<<<<<<< HEAD
      do_QueryInterface(item->GetContent());
  if (!itemElm) return false;
||||||| merged common ancestors
    do_QueryInterface(item->GetContent());
  if (!itemElm)
    return false;
=======
      item->GetContent()->AsElement()->AsXULSelectControlItem();
  if (!itemElm) return false;
>>>>>>> upstream-releases

  bool isItemSelected = false;
  itemElm->GetSelected(&isItemSelected);
  return isItemSelected;
}

bool XULSelectControlAccessible::UnselectAll() {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> multiSelectControl =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
  multiSelectControl ? multiSelectControl->ClearSelection()
                     : mSelectControl->SetSelectedIndex(-1);
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
  multiSelectControl ?
    multiSelectControl->ClearSelection() : mSelectControl->SetSelectedIndex(-1);
=======
      mSelectControl->AsXULMultiSelectControl();
  if (multiSelectControl) {
    multiSelectControl->ClearSelection();
  } else {
    nsCOMPtr<nsIDOMXULSelectControlElement> selectControl =
        mSelectControl->AsXULSelectControl();
    if (selectControl) {
      selectControl->SetSelectedIndex(-1);
    }
  }
>>>>>>> upstream-releases

  return true;
}

bool XULSelectControlAccessible::SelectAll() {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> multiSelectControl =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
=======
      mSelectControl->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  if (multiSelectControl) {
    multiSelectControl->SelectAll();
    return true;
  }

  // otherwise, don't support this method
  return false;
}

////////////////////////////////////////////////////////////////////////////////
// XULSelectControlAccessible: Widgets

Accessible* XULSelectControlAccessible::CurrentItem() const {
  if (!mSelectControl) return nullptr;

  RefPtr<Element> currentItemElm;
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> multiSelectControl =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
  if (multiSelectControl)
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
  if (multiSelectControl)
=======
      mSelectControl->AsXULMultiSelectControl();
  if (multiSelectControl) {
>>>>>>> upstream-releases
    multiSelectControl->GetCurrentItem(getter_AddRefs(currentItemElm));
<<<<<<< HEAD
  else
    mSelectControl->GetSelectedItem(getter_AddRefs(currentItemElm));

  nsCOMPtr<nsINode> DOMNode;
  if (currentItemElm) DOMNode = do_QueryInterface(currentItemElm);
||||||| merged common ancestors
  else
    mSelectControl->GetSelectedItem(getter_AddRefs(currentItemElm));

  nsCOMPtr<nsINode> DOMNode;
  if (currentItemElm)
    DOMNode = do_QueryInterface(currentItemElm);
=======
  } else {
    nsCOMPtr<nsIDOMXULSelectControlElement> selectControl =
        mSelectControl->AsXULSelectControl();
    if (selectControl) {
      selectControl->GetSelectedItem(getter_AddRefs(currentItemElm));
    }
  }
>>>>>>> upstream-releases

  if (currentItemElm) {
    DocAccessible* document = Document();
<<<<<<< HEAD
    if (document) return document->GetAccessible(DOMNode);
||||||| merged common ancestors
    if (document)
      return document->GetAccessible(DOMNode);
=======
    if (document) return document->GetAccessible(currentItemElm);
>>>>>>> upstream-releases
  }

  return nullptr;
}

void XULSelectControlAccessible::SetCurrentItem(const Accessible* aItem) {
  if (!mSelectControl) return;

<<<<<<< HEAD
  nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm =
      do_QueryInterface(aItem->GetContent());
||||||| merged common ancestors
  nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm =
    do_QueryInterface(aItem->GetContent());
=======
  nsCOMPtr<Element> itemElm = aItem->Elm();
>>>>>>> upstream-releases
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> multiSelectControl =
<<<<<<< HEAD
      do_QueryInterface(mSelectControl);
  if (multiSelectControl)
||||||| merged common ancestors
    do_QueryInterface(mSelectControl);
  if (multiSelectControl)
=======
      itemElm->AsXULMultiSelectControl();
  if (multiSelectControl) {
>>>>>>> upstream-releases
    multiSelectControl->SetCurrentItem(itemElm);
  } else {
    nsCOMPtr<nsIDOMXULSelectControlElement> selectControl =
        mSelectControl->AsXULSelectControl();
    if (selectControl) {
      selectControl->SetSelectedItem(itemElm);
    }
  }
}
