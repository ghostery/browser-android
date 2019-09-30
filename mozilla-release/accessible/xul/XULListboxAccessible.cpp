/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "XULListboxAccessible.h"

#include "Accessible-inl.h"
#include "nsAccessibilityService.h"
#include "nsAccUtils.h"
#include "DocAccessible.h"
#include "Role.h"
#include "States.h"

#include "nsComponentManagerUtils.h"
#include "nsIAutoCompleteInput.h"
#include "nsIAutoCompletePopup.h"
#include "nsIDOMXULMenuListElement.h"
#include "nsIDOMXULMultSelectCntrlEl.h"
#include "nsIDOMXULSelectCntrlItemEl.h"
#include "nsIMutableArray.h"
#include "nsINodeList.h"
#include "nsIPersistentProperties2.h"

using namespace mozilla::a11y;

////////////////////////////////////////////////////////////////////////////////
// XULColumAccessible
////////////////////////////////////////////////////////////////////////////////

XULColumAccessible::XULColumAccessible(nsIContent* aContent,
                                       DocAccessible* aDoc)
    : AccessibleWrap(aContent, aDoc) {}

role XULColumAccessible::NativeRole() const { return roles::LIST; }

uint64_t XULColumAccessible::NativeState() const { return states::READONLY; }

////////////////////////////////////////////////////////////////////////////////
// XULColumnItemAccessible
////////////////////////////////////////////////////////////////////////////////

XULColumnItemAccessible::XULColumnItemAccessible(nsIContent* aContent,
                                                 DocAccessible* aDoc)
    : LeafAccessible(aContent, aDoc) {}

role XULColumnItemAccessible::NativeRole() const { return roles::COLUMNHEADER; }

uint64_t XULColumnItemAccessible::NativeState() const {
  return states::READONLY;
}

uint8_t XULColumnItemAccessible::ActionCount() const { return 1; }

void XULColumnItemAccessible::ActionNameAt(uint8_t aIndex, nsAString& aName) {
  if (aIndex == eAction_Click) aName.AssignLiteral("click");
}

bool XULColumnItemAccessible::DoAction(uint8_t aIndex) const {
  if (aIndex != eAction_Click) return false;

  DoCommand();
  return true;
}

////////////////////////////////////////////////////////////////////////////////
// XULListboxAccessible
////////////////////////////////////////////////////////////////////////////////

XULListboxAccessible::XULListboxAccessible(nsIContent* aContent,
                                           DocAccessible* aDoc)
    : XULSelectControlAccessible(aContent, aDoc) {
  nsIContent* parentContent = mContent->GetFlattenedTreeParent();
  if (parentContent) {
    nsCOMPtr<nsIAutoCompletePopup> autoCompletePopupElm =
<<<<<<< HEAD
        do_QueryInterface(parentContent);
    if (autoCompletePopupElm) mGenericTypes |= eAutoCompletePopup;
||||||| merged common ancestors
      do_QueryInterface(parentContent);
    if (autoCompletePopupElm)
      mGenericTypes |= eAutoCompletePopup;
=======
        parentContent->AsElement()->AsAutoCompletePopup();
    if (autoCompletePopupElm) mGenericTypes |= eAutoCompletePopup;
>>>>>>> upstream-releases
  }

  if (IsMulticolumn()) mGenericTypes |= eTable;
}

////////////////////////////////////////////////////////////////////////////////
// XULListboxAccessible: Accessible

uint64_t XULListboxAccessible::NativeState() const {
  // As a XULListboxAccessible we can have the following states:
  //   FOCUSED, READONLY, FOCUSABLE

  // Get focus status from base class
  uint64_t states = Accessible::NativeState();

  // see if we are multiple select if so set ourselves as such

  if (mContent->AsElement()->AttrValueIs(kNameSpaceID_None, nsGkAtoms::seltype,
                                         nsGkAtoms::multiple, eCaseMatters)) {
    states |= states::MULTISELECTABLE | states::EXTSELECTABLE;
  }

  return states;
}

/**
 * Our value is the label of our ( first ) selected child.
 */
void XULListboxAccessible::Value(nsString& aValue) const {
  aValue.Truncate();

  nsCOMPtr<nsIDOMXULSelectControlElement> select = Elm()->AsXULSelectControl();
  if (select) {
<<<<<<< HEAD
    nsCOMPtr<nsIDOMXULSelectControlItemElement> selectedItem;
    select->GetSelectedItem(getter_AddRefs(selectedItem));
    if (selectedItem) selectedItem->GetLabel(aValue);
||||||| merged common ancestors
    nsCOMPtr<nsIDOMXULSelectControlItemElement> selectedItem;
    select->GetSelectedItem(getter_AddRefs(selectedItem));
    if (selectedItem)
      selectedItem->GetLabel(aValue);
=======
    RefPtr<Element> element;
    select->GetSelectedItem(getter_AddRefs(element));

    if (element) {
      nsCOMPtr<nsIDOMXULSelectControlItemElement> selectedItem =
          element->AsXULSelectControlItem();
      if (selectedItem) {
        selectedItem->GetLabel(aValue);
      }
    }
>>>>>>> upstream-releases
  }
}

role XULListboxAccessible::NativeRole() const {
  // A richlistbox is used with the new autocomplete URL bar, and has a parent
  // popup <panel>.
  if (mContent->GetParent() &&
      mContent->GetParent()->IsXULElement(nsGkAtoms::panel))
    return roles::COMBOBOX_LIST;

  return IsMulticolumn() ? roles::TABLE : roles::LISTBOX;
}

////////////////////////////////////////////////////////////////////////////////
// XULListboxAccessible: Table

uint32_t XULListboxAccessible::ColCount() const { return 0; }

<<<<<<< HEAD
uint32_t XULListboxAccessible::RowCount() {
  nsCOMPtr<nsIDOMXULSelectControlElement> element(do_QueryInterface(mContent));
||||||| merged common ancestors
uint32_t
XULListboxAccessible::RowCount()
{
  nsCOMPtr<nsIDOMXULSelectControlElement> element(do_QueryInterface(mContent));
=======
uint32_t XULListboxAccessible::RowCount() {
  nsCOMPtr<nsIDOMXULSelectControlElement> element = Elm()->AsXULSelectControl();
>>>>>>> upstream-releases

  uint32_t itemCount = 0;
  if (element) element->GetItemCount(&itemCount);

  return itemCount;
}

<<<<<<< HEAD
Accessible* XULListboxAccessible::CellAt(uint32_t aRowIndex,
                                         uint32_t aColumnIndex) {
  nsCOMPtr<nsIDOMXULSelectControlElement> control = do_QueryInterface(mContent);
||||||| merged common ancestors
Accessible*
XULListboxAccessible::CellAt(uint32_t aRowIndex, uint32_t aColumnIndex)
{
  nsCOMPtr<nsIDOMXULSelectControlElement> control =
    do_QueryInterface(mContent);
=======
Accessible* XULListboxAccessible::CellAt(uint32_t aRowIndex,
                                         uint32_t aColumnIndex) {
  nsCOMPtr<nsIDOMXULSelectControlElement> control = Elm()->AsXULSelectControl();
>>>>>>> upstream-releases
  NS_ENSURE_TRUE(control, nullptr);

<<<<<<< HEAD
  nsCOMPtr<nsIDOMXULSelectControlItemElement> item;
  control->GetItemAtIndex(aRowIndex, getter_AddRefs(item));
  if (!item) return nullptr;

  nsCOMPtr<nsIContent> itemContent(do_QueryInterface(item));
  if (!itemContent) return nullptr;
||||||| merged common ancestors
  nsCOMPtr<nsIDOMXULSelectControlItemElement> item;
  control->GetItemAtIndex(aRowIndex, getter_AddRefs(item));
  if (!item)
    return nullptr;

  nsCOMPtr<nsIContent> itemContent(do_QueryInterface(item));
  if (!itemContent)
    return nullptr;
=======
  RefPtr<Element> element;
  control->GetItemAtIndex(aRowIndex, getter_AddRefs(element));
  if (!element) return nullptr;
>>>>>>> upstream-releases

  Accessible* row = mDoc->GetAccessible(element);
  NS_ENSURE_TRUE(row, nullptr);

  return row->GetChildAt(aColumnIndex);
}

bool XULListboxAccessible::IsColSelected(uint32_t aColIdx) {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  int32_t selectedrowCount = 0;
  nsresult rv = control->GetSelectedCount(&selectedrowCount);
  NS_ENSURE_SUCCESS(rv, false);

  return selectedrowCount == static_cast<int32_t>(RowCount());
}

<<<<<<< HEAD
bool XULListboxAccessible::IsRowSelected(uint32_t aRowIdx) {
  nsCOMPtr<nsIDOMXULSelectControlElement> control = do_QueryInterface(mContent);
  NS_ASSERTION(control, "Doesn't implement nsIDOMXULSelectControlElement.");
||||||| merged common ancestors
bool
XULListboxAccessible::IsRowSelected(uint32_t aRowIdx)
{
  nsCOMPtr<nsIDOMXULSelectControlElement> control =
    do_QueryInterface(mContent);
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULSelectControlElement.");
=======
bool XULListboxAccessible::IsRowSelected(uint32_t aRowIdx) {
  nsCOMPtr<nsIDOMXULSelectControlElement> control = Elm()->AsXULSelectControl();
  NS_ASSERTION(control, "Doesn't implement nsIDOMXULSelectControlElement.");
>>>>>>> upstream-releases

  RefPtr<Element> element;
  nsresult rv = control->GetItemAtIndex(aRowIdx, getter_AddRefs(element));
  NS_ENSURE_SUCCESS(rv, false);
  if (!element) {
    return false;
  }

  nsCOMPtr<nsIDOMXULSelectControlItemElement> item =
      element->AsXULSelectControlItem();

  bool isSelected = false;
  item->GetSelected(&isSelected);
  return isSelected;
}

bool XULListboxAccessible::IsCellSelected(uint32_t aRowIdx, uint32_t aColIdx) {
  return IsRowSelected(aRowIdx);
}

uint32_t XULListboxAccessible::SelectedCellCount() {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  nsCOMPtr<nsINodeList> selectedItems;
  control->GetSelectedItems(getter_AddRefs(selectedItems));
  if (!selectedItems) return 0;

  uint32_t selectedItemsCount = selectedItems->Length();

  return selectedItemsCount * ColCount();
}

uint32_t XULListboxAccessible::SelectedColCount() {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  int32_t selectedRowCount = 0;
  nsresult rv = control->GetSelectedCount(&selectedRowCount);
  NS_ENSURE_SUCCESS(rv, 0);

  return selectedRowCount > 0 &&
                 selectedRowCount == static_cast<int32_t>(RowCount())
             ? ColCount()
             : 0;
}

uint32_t XULListboxAccessible::SelectedRowCount() {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  int32_t selectedRowCount = 0;
  nsresult rv = control->GetSelectedCount(&selectedRowCount);
  NS_ENSURE_SUCCESS(rv, 0);

  return selectedRowCount >= 0 ? selectedRowCount : 0;
}

void XULListboxAccessible::SelectedCells(nsTArray<Accessible*>* aCells) {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  nsCOMPtr<nsINodeList> selectedItems;
  control->GetSelectedItems(getter_AddRefs(selectedItems));
  if (!selectedItems) return;

  uint32_t selectedItemsCount = selectedItems->Length();

  for (uint32_t index = 0; index < selectedItemsCount; index++) {
    nsIContent* itemContent = selectedItems->Item(index);
    Accessible* item = mDoc->GetAccessible(itemContent);

    if (item) {
      uint32_t cellCount = item->ChildCount();
      for (uint32_t cellIdx = 0; cellIdx < cellCount; cellIdx++) {
        Accessible* cell = mChildren[cellIdx];
        if (cell->Role() == roles::CELL) aCells->AppendElement(cell);
      }
    }
  }
}

void XULListboxAccessible::SelectedCellIndices(nsTArray<uint32_t>* aCells) {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  nsCOMPtr<nsINodeList> selectedItems;
  control->GetSelectedItems(getter_AddRefs(selectedItems));
  if (!selectedItems) return;

  uint32_t selectedItemsCount = selectedItems->Length();

  uint32_t colCount = ColCount();
  aCells->SetCapacity(selectedItemsCount * colCount);
  aCells->AppendElements(selectedItemsCount * colCount);

  for (uint32_t selItemsIdx = 0, cellsIdx = 0; selItemsIdx < selectedItemsCount;
       selItemsIdx++) {
    nsIContent* itemContent = selectedItems->Item(selItemsIdx);
<<<<<<< HEAD
    nsCOMPtr<nsIDOMXULSelectControlItemElement> item =
        do_QueryInterface(itemContent);
||||||| merged common ancestors
    nsCOMPtr<nsIDOMXULSelectControlItemElement> item =
      do_QueryInterface(itemContent);
=======
>>>>>>> upstream-releases

    nsCOMPtr<nsIDOMXULSelectControlItemElement> item =
        itemContent->AsElement()->AsXULSelectControlItem();
    if (item) {
      int32_t itemIdx = -1;
      control->GetIndexOfItem(item, &itemIdx);
      if (itemIdx >= 0)
        for (uint32_t colIdx = 0; colIdx < colCount; colIdx++, cellsIdx++)
          aCells->ElementAt(cellsIdx) = itemIdx * colCount + colIdx;
    }
  }
}

void XULListboxAccessible::SelectedColIndices(nsTArray<uint32_t>* aCols) {
  uint32_t selColCount = SelectedColCount();
  aCols->SetCapacity(selColCount);

  for (uint32_t colIdx = 0; colIdx < selColCount; colIdx++)
    aCols->AppendElement(colIdx);
}

void XULListboxAccessible::SelectedRowIndices(nsTArray<uint32_t>* aRows) {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  nsCOMPtr<nsINodeList> selectedItems;
  control->GetSelectedItems(getter_AddRefs(selectedItems));
  if (!selectedItems) return;

  uint32_t rowCount = selectedItems->Length();

  if (!rowCount) return;

  aRows->SetCapacity(rowCount);
  aRows->AppendElements(rowCount);

  for (uint32_t rowIdx = 0; rowIdx < rowCount; rowIdx++) {
    nsIContent* itemContent = selectedItems->Item(rowIdx);
    nsCOMPtr<nsIDOMXULSelectControlItemElement> item =
<<<<<<< HEAD
        do_QueryInterface(itemContent);
||||||| merged common ancestors
      do_QueryInterface(itemContent);
=======
        itemContent->AsElement()->AsXULSelectControlItem();
>>>>>>> upstream-releases

    if (item) {
      int32_t itemIdx = -1;
      control->GetIndexOfItem(item, &itemIdx);
      if (itemIdx >= 0) aRows->ElementAt(rowIdx) = itemIdx;
    }
  }
}

void XULListboxAccessible::SelectRow(uint32_t aRowIdx) {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  RefPtr<Element> item;
  control->GetItemAtIndex(aRowIdx, getter_AddRefs(item));
  if (!item) {
    return;
  }

  nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm =
      item->AsXULSelectControlItem();
  control->SelectItem(itemElm);
}

void XULListboxAccessible::UnselectRow(uint32_t aRowIdx) {
  nsCOMPtr<nsIDOMXULMultiSelectControlElement> control =
<<<<<<< HEAD
      do_QueryInterface(mContent);
||||||| merged common ancestors
    do_QueryInterface(mContent);
=======
      Elm()->AsXULMultiSelectControl();
>>>>>>> upstream-releases
  NS_ASSERTION(control,
               "Doesn't implement nsIDOMXULMultiSelectControlElement.");

  RefPtr<Element> item;
  control->GetItemAtIndex(aRowIdx, getter_AddRefs(item));
  if (!item) {
    return;
  }

  nsCOMPtr<nsIDOMXULSelectControlItemElement> itemElm =
      item->AsXULSelectControlItem();
  control->RemoveItemFromSelection(itemElm);
}

////////////////////////////////////////////////////////////////////////////////
// XULListboxAccessible: Widgets

bool XULListboxAccessible::IsWidget() const { return true; }

bool XULListboxAccessible::IsActiveWidget() const {
  if (IsAutoCompletePopup()) {
<<<<<<< HEAD
    nsCOMPtr<nsIAutoCompletePopup> autoCompletePopupElm =
        do_QueryInterface(mContent->GetParent());

    if (autoCompletePopupElm) {
      bool isOpen = false;
      autoCompletePopupElm->GetPopupOpen(&isOpen);
      return isOpen;
||||||| merged common ancestors
    nsCOMPtr<nsIAutoCompletePopup> autoCompletePopupElm =
      do_QueryInterface(mContent->GetParent());

    if (autoCompletePopupElm) {
      bool isOpen = false;
      autoCompletePopupElm->GetPopupOpen(&isOpen);
      return isOpen;
=======
    nsIContent* parentContent = mContent->GetParent();
    if (parentContent) {
      nsCOMPtr<nsIAutoCompletePopup> autoCompletePopupElm =
          parentContent->AsElement()->AsAutoCompletePopup();
      if (autoCompletePopupElm) {
        bool isOpen = false;
        autoCompletePopupElm->GetPopupOpen(&isOpen);
        return isOpen;
      }
>>>>>>> upstream-releases
    }
  }
  return FocusMgr()->HasDOMFocus(mContent);
}

bool XULListboxAccessible::AreItemsOperable() const {
  if (IsAutoCompletePopup()) {
<<<<<<< HEAD
    nsCOMPtr<nsIAutoCompletePopup> autoCompletePopupElm =
        do_QueryInterface(mContent->GetParent());

    if (autoCompletePopupElm) {
      bool isOpen = false;
      autoCompletePopupElm->GetPopupOpen(&isOpen);
      return isOpen;
||||||| merged common ancestors
    nsCOMPtr<nsIAutoCompletePopup> autoCompletePopupElm =
      do_QueryInterface(mContent->GetParent());

    if (autoCompletePopupElm) {
      bool isOpen = false;
      autoCompletePopupElm->GetPopupOpen(&isOpen);
      return isOpen;
=======
    nsIContent* parentContent = mContent->GetParent();
    if (parentContent) {
      nsCOMPtr<nsIAutoCompletePopup> autoCompletePopupElm =
          parentContent->AsElement()->AsAutoCompletePopup();
      if (autoCompletePopupElm) {
        bool isOpen = false;
        autoCompletePopupElm->GetPopupOpen(&isOpen);
        return isOpen;
      }
>>>>>>> upstream-releases
    }
  }
  return true;
}

<<<<<<< HEAD
Accessible* XULListboxAccessible::ContainerWidget() const {
  if (IsAutoCompletePopup()) {
||||||| merged common ancestors
Accessible*
XULListboxAccessible::ContainerWidget() const
{
  if (IsAutoCompletePopup()) {
=======
Accessible* XULListboxAccessible::ContainerWidget() const {
  if (IsAutoCompletePopup() && mContent->GetParent()) {
>>>>>>> upstream-releases
    // This works for XUL autocompletes. It doesn't work for HTML forms
    // autocomplete because of potential crossprocess calls (when autocomplete
    // lives in content process while popup lives in chrome process). If that's
    // a problem then rethink Widgets interface.
    nsCOMPtr<nsIDOMXULMenuListElement> menuListElm =
<<<<<<< HEAD
        do_QueryInterface(mContent->GetParent());
||||||| merged common ancestors
      do_QueryInterface(mContent->GetParent());
=======
        mContent->GetParent()->AsElement()->AsXULMenuList();
>>>>>>> upstream-releases
    if (menuListElm) {
      RefPtr<mozilla::dom::Element> inputElm;
      menuListElm->GetInputField(getter_AddRefs(inputElm));
      if (inputElm) {
        Accessible* input = mDoc->GetAccessible(inputElm);
        return input ? input->ContainerWidget() : nullptr;
      }
    }
  }
  return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// XULListitemAccessible
////////////////////////////////////////////////////////////////////////////////

XULListitemAccessible::XULListitemAccessible(nsIContent* aContent,
                                             DocAccessible* aDoc)
    : XULMenuitemAccessible(aContent, aDoc) {
  mIsCheckbox = mContent->AsElement()->AttrValueIs(
      kNameSpaceID_None, nsGkAtoms::type, nsGkAtoms::checkbox, eCaseMatters);
  mType = eXULListItemType;

  // Walk XBL anonymous children for list items. Overrides the flag value from
  // base XULMenuitemAccessible class.
  mStateFlags &= ~eNoXBLKids;
}

XULListitemAccessible::~XULListitemAccessible() {}

Accessible* XULListitemAccessible::GetListAccessible() const {
  if (IsDefunct()) return nullptr;

  nsCOMPtr<nsIDOMXULSelectControlItemElement> listItem =
<<<<<<< HEAD
      do_QueryInterface(mContent);
  if (!listItem) return nullptr;

  nsCOMPtr<nsIDOMXULSelectControlElement> list;
  listItem->GetControl(getter_AddRefs(list));
||||||| merged common ancestors
    do_QueryInterface(mContent);
  if (!listItem)
    return nullptr;

  nsCOMPtr<nsIDOMXULSelectControlElement> list;
  listItem->GetControl(getter_AddRefs(list));
=======
      Elm()->AsXULSelectControlItem();
  if (!listItem) return nullptr;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIContent> listContent(do_QueryInterface(list));
  if (!listContent) return nullptr;
||||||| merged common ancestors
  nsCOMPtr<nsIContent> listContent(do_QueryInterface(list));
  if (!listContent)
    return nullptr;
=======
  RefPtr<Element> listElement;
  listItem->GetControl(getter_AddRefs(listElement));
  if (!listElement) return nullptr;
>>>>>>> upstream-releases

  return mDoc->GetAccessible(listElement);
}

////////////////////////////////////////////////////////////////////////////////
// XULListitemAccessible Accessible

void XULListitemAccessible::Description(nsString& aDesc) {
  AccessibleWrap::Description(aDesc);
}

////////////////////////////////////////////////////////////////////////////////
// XULListitemAccessible: Accessible

/**
 * Get the name from GetXULName.
 */
ENameValueFlag XULListitemAccessible::NativeName(nsString& aName) const {
  return Accessible::NativeName(aName);
}

role XULListitemAccessible::NativeRole() const {
  Accessible* list = GetListAccessible();
  if (!list) {
    NS_ERROR("No list accessible for listitem accessible!");
    return roles::NOTHING;
  }

  if (list->Role() == roles::TABLE) return roles::ROW;

  if (mIsCheckbox) return roles::CHECK_RICH_OPTION;

  if (mParent && mParent->Role() == roles::COMBOBOX_LIST)
    return roles::COMBOBOX_OPTION;

  return roles::RICH_OPTION;
}

uint64_t XULListitemAccessible::NativeState() const {
  if (mIsCheckbox) return XULMenuitemAccessible::NativeState();

  uint64_t states = NativeInteractiveState();

  nsCOMPtr<nsIDOMXULSelectControlItemElement> listItem =
<<<<<<< HEAD
      do_QueryInterface(mContent);

||||||| merged common ancestors
    do_QueryInterface(mContent);

=======
      Elm()->AsXULSelectControlItem();
>>>>>>> upstream-releases
  if (listItem) {
    bool isSelected;
    listItem->GetSelected(&isSelected);
    if (isSelected) states |= states::SELECTED;

    if (FocusMgr()->IsFocused(this)) states |= states::FOCUSED;
  }

  return states;
}

uint64_t XULListitemAccessible::NativeInteractiveState() const {
  return NativelyUnavailable() || (mParent && mParent->NativelyUnavailable())
             ? states::UNAVAILABLE
             : states::FOCUSABLE | states::SELECTABLE;
}

void XULListitemAccessible::ActionNameAt(uint8_t aIndex, nsAString& aName) {
  if (aIndex == eAction_Click && mIsCheckbox) {
    uint64_t states = NativeState();
    if (states & states::CHECKED)
      aName.AssignLiteral("uncheck");
    else
      aName.AssignLiteral("check");
  }
}

////////////////////////////////////////////////////////////////////////////////
// XULListitemAccessible: Widgets

Accessible* XULListitemAccessible::ContainerWidget() const { return Parent(); }
