/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
#include <stdio.h>  // for printf
||||||| merged common ancestors
=======
#include "mozilla/EditorCommands.h"
>>>>>>> upstream-releases

#include "mozilla/Assertions.h"  // for MOZ_ASSERT, etc
#include "mozilla/EditorBase.h"  // for EditorBase
#include "mozilla/ErrorResult.h"
<<<<<<< HEAD
#include "mozilla/HTMLEditor.h"  // for HTMLEditor
#include "mozilla/HTMLEditorCommands.h"
||||||| merged common ancestors
#include "mozilla/HTMLEditor.h"         // for HTMLEditor
#include "mozilla/HTMLEditorCommands.h"
=======
#include "mozilla/HTMLEditor.h"  // for HTMLEditor
>>>>>>> upstream-releases
#include "mozilla/dom/Element.h"
#include "nsAString.h"
<<<<<<< HEAD
#include "nsCommandParams.h"          // for nsCommandParams, etc
#include "nsCOMPtr.h"                 // for nsCOMPtr, do_QueryInterface, etc
#include "nsComponentManagerUtils.h"  // for do_CreateInstance
#include "nsDebug.h"                  // for NS_ENSURE_TRUE, etc
#include "nsError.h"                  // for NS_OK, NS_ERROR_FAILURE, etc
#include "nsGkAtoms.h"                // for nsGkAtoms, nsGkAtoms::font, etc
#include "nsAtom.h"                   // for nsAtom, etc
#include "nsIClipboard.h"             // for nsIClipboard, etc
#include "nsID.h"
#include "nsIEditor.h"        // for nsIEditor
#include "nsIHTMLEditor.h"    // for nsIHTMLEditor, etc
#include "nsLiteralString.h"  // for NS_LITERAL_STRING
#include "nsReadableUtils.h"  // for EmptyString
#include "nsString.h"         // for nsAutoString, nsString, etc
#include "nsStringFwd.h"      // for nsString
||||||| merged common ancestors
#include "nsCommandParams.h"            // for nsCommandParams, etc
#include "nsCOMPtr.h"                   // for nsCOMPtr, do_QueryInterface, etc
#include "nsComponentManagerUtils.h"    // for do_CreateInstance
#include "nsDebug.h"                    // for NS_ENSURE_TRUE, etc
#include "nsError.h"                    // for NS_OK, NS_ERROR_FAILURE, etc
#include "nsGkAtoms.h"                  // for nsGkAtoms, nsGkAtoms::font, etc
#include "nsAtom.h"                    // for nsAtom, etc
#include "nsIClipboard.h"               // for nsIClipboard, etc
#include "nsID.h"
#include "nsIEditor.h"                  // for nsIEditor
#include "nsIHTMLEditor.h"              // for nsIHTMLEditor, etc
#include "nsLiteralString.h"            // for NS_LITERAL_STRING
#include "nsReadableUtils.h"            // for EmptyString
#include "nsString.h"                   // for nsAutoString, nsString, etc
#include "nsStringFwd.h"                // for nsString
=======
#include "nsAtom.h"                   // for nsAtom, nsStaticAtom, etc
#include "nsCommandParams.h"          // for nsCommandParams, etc
#include "nsComponentManagerUtils.h"  // for do_CreateInstance
#include "nsGkAtoms.h"                // for nsGkAtoms, nsGkAtoms::font, etc
#include "nsIClipboard.h"             // for nsIClipboard, etc
#include "nsIEditingSession.h"
#include "nsIPrincipal.h"     // for nsIPrincipal
#include "nsLiteralString.h"  // for NS_LITERAL_STRING
#include "nsReadableUtils.h"  // for EmptyString
#include "nsString.h"         // for nsAutoString, nsString, etc
#include "nsStringFwd.h"      // for nsString
>>>>>>> upstream-releases

class nsISupports;

namespace mozilla {
using dom::Element;

<<<<<<< HEAD
// prototype
static nsresult GetListState(HTMLEditor* aHTMLEditor, bool* aMixed,
                             nsAString& aLocalName);
||||||| merged common ancestors
//prototype
static nsresult
GetListState(HTMLEditor* aHTMLEditor, bool* aMixed, nsAString& aLocalName);
=======
// prototype
MOZ_CAN_RUN_SCRIPT
static nsresult GetListState(HTMLEditor* aHTMLEditor, bool* aMixed,
                             nsAString& aLocalName);
>>>>>>> upstream-releases

// defines
#define STATE_ENABLED "state_enabled"
#define STATE_ALL "state_all"
#define STATE_ANY "state_any"
#define STATE_MIXED "state_mixed"
#define STATE_BEGIN "state_begin"
#define STATE_END "state_end"
#define STATE_ATTRIBUTE "state_attribute"
#define STATE_DATA "state_data"

<<<<<<< HEAD
HTMLEditorCommandBase::HTMLEditorCommandBase() {}

NS_IMPL_ISUPPORTS(HTMLEditorCommandBase, nsIControllerCommand)

StateUpdatingCommandBase::StateUpdatingCommandBase(nsAtom* aTagName)
    : HTMLEditorCommandBase(), mTagName(aTagName) {
  MOZ_ASSERT(mTagName);
}

StateUpdatingCommandBase::~StateUpdatingCommandBase() {}

NS_IMETHODIMP
StateUpdatingCommandBase::IsCommandEnabled(const char* aCommandName,
                                           nsISupports* refCon,
                                           bool* outCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
||||||| merged common ancestors

HTMLEditorCommandBase::HTMLEditorCommandBase()
{
}

NS_IMPL_ISUPPORTS(HTMLEditorCommandBase, nsIControllerCommand)


StateUpdatingCommandBase::StateUpdatingCommandBase(nsAtom* aTagName)
  : HTMLEditorCommandBase()
  , mTagName(aTagName)
{
  MOZ_ASSERT(mTagName);
}

StateUpdatingCommandBase::~StateUpdatingCommandBase()
{
}

NS_IMETHODIMP
StateUpdatingCommandBase::IsCommandEnabled(const char* aCommandName,
                                           nsISupports* refCon,
                                           bool* outCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
=======
/*****************************************************************************
 * mozilla::StateUpdatingCommandBase
 *****************************************************************************/

bool StateUpdatingCommandBase::IsCommandEnabled(Command aCommand,
                                                TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
  }
  if (!htmlEditor->IsSelectionEditable()) {
    return false;
  }
  if (aCommand == Command::FormatAbsolutePosition) {
    return htmlEditor->IsAbsolutePositionEditorEnabled();
  }
  return true;
}

<<<<<<< HEAD
NS_IMETHODIMP
StateUpdatingCommandBase::DoCommand(const char* aCommandName,
                                    nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
||||||| merged common ancestors

NS_IMETHODIMP
StateUpdatingCommandBase::DoCommand(const char* aCommandName,
                                    nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
=======
nsresult StateUpdatingCommandBase::DoCommand(Command aCommand,
                                             TextEditor& aTextEditor,
                                             nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
>>>>>>> upstream-releases
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
<<<<<<< HEAD
  return ToggleState(htmlEditor);
}

NS_IMETHODIMP
StateUpdatingCommandBase::DoCommandParams(const char* aCommandName,
                                          nsICommandParams* aParams,
                                          nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
||||||| merged common ancestors
  return ToggleState(htmlEditor);
}

NS_IMETHODIMP
StateUpdatingCommandBase::DoCommandParams(const char* aCommandName,
                                          nsICommandParams* aParams,
                                          nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
=======
  nsAtom* tagName = GetTagName(aCommand);
  if (NS_WARN_IF(!tagName)) {
    return NS_ERROR_UNEXPECTED;
  }
  return ToggleState(MOZ_KnownLive(tagName), MOZ_KnownLive(htmlEditor),
                     aPrincipal);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
StateUpdatingCommandBase::GetCommandStateParams(const char* aCommandName,
                                                nsICommandParams* aParams,
                                                nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
||||||| merged common ancestors
NS_IMETHODIMP
StateUpdatingCommandBase::GetCommandStateParams(const char* aCommandName,
                                                nsICommandParams* aParams,
                                                nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
=======
nsresult StateUpdatingCommandBase::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  if (!aTextEditor) {
>>>>>>> upstream-releases
    return NS_OK;
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
  nsAtom* tagName = GetTagName(aCommand);
  if (NS_WARN_IF(!tagName)) {
    return NS_ERROR_UNEXPECTED;
  }
  return GetCurrentState(MOZ_KnownLive(tagName), MOZ_KnownLive(htmlEditor),
                         aParams);
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteNoFormattingCommand::IsCommandEnabled(const char* aCommandName,
                                           nsISupports* refCon,
                                           bool* outCmdEnabled) {
  NS_ENSURE_ARG_POINTER(outCmdEnabled);
  *outCmdEnabled = false;
||||||| merged common ancestors
NS_IMETHODIMP
PasteNoFormattingCommand::IsCommandEnabled(const char* aCommandName,
                                           nsISupports* refCon,
                                           bool* outCmdEnabled)
{
  NS_ENSURE_ARG_POINTER(outCmdEnabled);
  *outCmdEnabled = false;
=======
/*****************************************************************************
 * mozilla::PasteNoFormattingCommand
 *****************************************************************************/
>>>>>>> upstream-releases

StaticRefPtr<PasteNoFormattingCommand> PasteNoFormattingCommand::sInstance;

bool PasteNoFormattingCommand::IsCommandEnabled(Command aCommand,
                                                TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
  }
  return htmlEditor->CanPaste(nsIClipboard::kGlobalClipboard);
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteNoFormattingCommand::DoCommand(const char* aCommandName,
                                    nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
||||||| merged common ancestors
NS_IMETHODIMP
PasteNoFormattingCommand::DoCommand(const char* aCommandName,
                                    nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
=======
nsresult PasteNoFormattingCommand::DoCommand(Command aCommand,
                                             TextEditor& aTextEditor,
                                             nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
>>>>>>> upstream-releases
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
  // Known live because we hold a ref above in "editor"
  nsresult rv = MOZ_KnownLive(htmlEditor)
                    ->PasteNoFormattingAsAction(nsIClipboard::kGlobalClipboard,
                                                aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "PasteNoFormattingAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteNoFormattingCommand::DoCommandParams(const char* aCommandName,
                                          nsICommandParams* aParams,
                                          nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
||||||| merged common ancestors
NS_IMETHODIMP
PasteNoFormattingCommand::DoCommandParams(const char* aCommandName,
                                          nsICommandParams* aParams,
                                          nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
=======
nsresult PasteNoFormattingCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteNoFormattingCommand::GetCommandStateParams(const char* aCommandName,
                                                nsICommandParams* aParams,
                                                nsISupports* refCon) {
  NS_ENSURE_ARG_POINTER(aParams);
||||||| merged common ancestors
NS_IMETHODIMP
PasteNoFormattingCommand::GetCommandStateParams(const char* aCommandName,
                                                nsICommandParams* aParams,
                                                nsISupports* refCon)
{
  NS_ENSURE_ARG_POINTER(aParams);
=======
/*****************************************************************************
 * mozilla::StyleUpdatingCommand
 *****************************************************************************/
>>>>>>> upstream-releases

StaticRefPtr<StyleUpdatingCommand> StyleUpdatingCommand::sInstance;

<<<<<<< HEAD
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, enabled);
}

StyleUpdatingCommand::StyleUpdatingCommand(nsAtom* aTagName)
    : StateUpdatingCommandBase(aTagName) {}

nsresult StyleUpdatingCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                               nsICommandParams* aParams) {
  if (NS_WARN_IF(!aHTMLEditor)) {
||||||| merged common ancestors
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, enabled);
}

StyleUpdatingCommand::StyleUpdatingCommand(nsAtom* aTagName)
  : StateUpdatingCommandBase(aTagName)
{
}

nsresult
StyleUpdatingCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                      nsICommandParams *aParams)
{
  if (NS_WARN_IF(!aHTMLEditor)) {
=======
nsresult StyleUpdatingCommand::GetCurrentState(nsAtom* aTagName,
                                               HTMLEditor* aHTMLEditor,
                                               nsCommandParams& aParams) const {
  if (NS_WARN_IF(!aTagName) || NS_WARN_IF(!aHTMLEditor)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  bool firstOfSelectionHasProp = false;
  bool anyOfSelectionHasProp = false;
  bool allOfSelectionHasProp = false;

<<<<<<< HEAD
  nsresult rv = aHTMLEditor->GetInlineProperty(
      mTagName, nullptr, EmptyString(), &firstOfSelectionHasProp,
      &anyOfSelectionHasProp, &allOfSelectionHasProp);

  nsCommandParams* params = aParams->AsCommandParams();
  params->SetBool(STATE_ENABLED, NS_SUCCEEDED(rv));
  params->SetBool(STATE_ALL, allOfSelectionHasProp);
  params->SetBool(STATE_ANY, anyOfSelectionHasProp);
  params->SetBool(STATE_MIXED, anyOfSelectionHasProp && !allOfSelectionHasProp);
  params->SetBool(STATE_BEGIN, firstOfSelectionHasProp);
  params->SetBool(STATE_END, allOfSelectionHasProp);  // not completely accurate
||||||| merged common ancestors
  nsresult rv = aHTMLEditor->GetInlineProperty(mTagName, nullptr,
                                               EmptyString(),
                                               &firstOfSelectionHasProp,
                                               &anyOfSelectionHasProp,
                                               &allOfSelectionHasProp);

  nsCommandParams* params = aParams->AsCommandParams();
  params->SetBool(STATE_ENABLED, NS_SUCCEEDED(rv));
  params->SetBool(STATE_ALL, allOfSelectionHasProp);
  params->SetBool(STATE_ANY, anyOfSelectionHasProp);
  params->SetBool(STATE_MIXED, anyOfSelectionHasProp && !allOfSelectionHasProp);
  params->SetBool(STATE_BEGIN, firstOfSelectionHasProp);
  params->SetBool(STATE_END, allOfSelectionHasProp); //not completely accurate
=======
  nsresult rv = aHTMLEditor->GetInlineProperty(
      aTagName, nullptr, EmptyString(), &firstOfSelectionHasProp,
      &anyOfSelectionHasProp, &allOfSelectionHasProp);

  aParams.SetBool(STATE_ENABLED, NS_SUCCEEDED(rv));
  aParams.SetBool(STATE_ALL, allOfSelectionHasProp);
  aParams.SetBool(STATE_ANY, anyOfSelectionHasProp);
  aParams.SetBool(STATE_MIXED, anyOfSelectionHasProp && !allOfSelectionHasProp);
  aParams.SetBool(STATE_BEGIN, firstOfSelectionHasProp);
  aParams.SetBool(STATE_END, allOfSelectionHasProp);  // not completely accurate
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
nsresult StyleUpdatingCommand::ToggleState(HTMLEditor* aHTMLEditor) {
  if (NS_WARN_IF(!aHTMLEditor)) {
||||||| merged common ancestors
nsresult
StyleUpdatingCommand::ToggleState(HTMLEditor* aHTMLEditor)
{
  if (NS_WARN_IF(!aHTMLEditor)) {
=======
nsresult StyleUpdatingCommand::ToggleState(nsAtom* aTagName,
                                           HTMLEditor* aHTMLEditor,
                                           nsIPrincipal* aPrincipal) const {
  if (NS_WARN_IF(!aTagName) || NS_WARN_IF(!aHTMLEditor)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  RefPtr<nsCommandParams> params = new nsCommandParams();

  // tags "href" and "name" are special cases in the core editor
  // they are used to remove named anchor/link and shouldn't be used for
  // insertion
  bool doTagRemoval;
  if (aTagName == nsGkAtoms::href || aTagName == nsGkAtoms::name) {
    doTagRemoval = true;
  } else {
    // check current selection; set doTagRemoval if formatting should be removed
    nsresult rv = GetCurrentState(aTagName, aHTMLEditor, *params);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    ErrorResult error;
    doTagRemoval = params->GetBool(STATE_ALL, error);
    if (NS_WARN_IF(error.Failed())) {
      return error.StealNSResult();
    }
  }

  if (doTagRemoval) {
    // Also remove equivalent properties (bug 317093)
    // XXX Why don't we make the following two transactions as an atomic
    //     transaction?  If the element is <b>, <i> or <strike>, user
    //     needs to undo twice.
<<<<<<< HEAD
    if (mTagName == nsGkAtoms::b) {
      nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(
          *nsGkAtoms::strong, nullptr);
||||||| merged common ancestors
    if (mTagName == nsGkAtoms::b) {
      nsresult rv =
        aHTMLEditor->RemoveInlineProperty(nsGkAtoms::strong, nullptr);
=======
    if (aTagName == nsGkAtoms::b) {
      nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(
          *nsGkAtoms::strong, nullptr, aPrincipal);
>>>>>>> upstream-releases
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
<<<<<<< HEAD
    } else if (mTagName == nsGkAtoms::i) {
      nsresult rv =
          aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::em, nullptr);
||||||| merged common ancestors
    } else if (mTagName == nsGkAtoms::i) {
      nsresult rv =
        aHTMLEditor->RemoveInlineProperty(nsGkAtoms::em, nullptr);
=======
    } else if (aTagName == nsGkAtoms::i) {
      nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(
          *nsGkAtoms::em, nullptr, aPrincipal);
>>>>>>> upstream-releases
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
<<<<<<< HEAD
    } else if (mTagName == nsGkAtoms::strike) {
      nsresult rv =
          aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::s, nullptr);
||||||| merged common ancestors
    } else if (mTagName == nsGkAtoms::strike) {
      nsresult rv =
        aHTMLEditor->RemoveInlineProperty(nsGkAtoms::s, nullptr);
=======
    } else if (aTagName == nsGkAtoms::strike) {
      nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(
          *nsGkAtoms::s, nullptr, aPrincipal);
>>>>>>> upstream-releases
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
    }

<<<<<<< HEAD
    nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(*mTagName, nullptr);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  nsresult rv =
      aHTMLEditor->SetInlinePropertyAsAction(*mTagName, nullptr, EmptyString());
  if (NS_WARN_IF(NS_FAILED(rv))) {
||||||| merged common ancestors
    nsresult rv = aHTMLEditor->RemoveInlineProperty(mTagName, nullptr);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  // Superscript and Subscript styles are mutually exclusive.
  AutoTransactionBatch bundleAllTransactions(*aHTMLEditor);

  if (mTagName == nsGkAtoms::sub || mTagName == nsGkAtoms::sup) {
    nsresult rv = aHTMLEditor->RemoveInlineProperty(mTagName, nullptr);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
  }

  nsresult rv =
    aHTMLEditor->SetInlineProperty(*mTagName, nullptr, EmptyString());
  if (NS_WARN_IF(NS_FAILED(rv))) {
=======
    nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(*aTagName, nullptr,
                                                            aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                         "RemoveInlinePropertyAsAction() failed");
>>>>>>> upstream-releases
    return rv;
  }
<<<<<<< HEAD
  return NS_OK;
||||||| merged common ancestors

  return rv;
=======

  nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
      *aTagName, nullptr, EmptyString(), aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetInlinePropertyAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
ListCommand::ListCommand(nsAtom* aTagName)
    : StateUpdatingCommandBase(aTagName) {}
||||||| merged common ancestors
ListCommand::ListCommand(nsAtom* aTagName)
  : StateUpdatingCommandBase(aTagName)
{
}
=======
/*****************************************************************************
 * mozilla::ListCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult ListCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                      nsICommandParams* aParams) {
  if (NS_WARN_IF(!aHTMLEditor)) {
||||||| merged common ancestors
nsresult
ListCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                             nsICommandParams* aParams)
{
  if (NS_WARN_IF(!aHTMLEditor)) {
=======
StaticRefPtr<ListCommand> ListCommand::sInstance;

nsresult ListCommand::GetCurrentState(nsAtom* aTagName, HTMLEditor* aHTMLEditor,
                                      nsCommandParams& aParams) const {
  if (NS_WARN_IF(!aTagName) || NS_WARN_IF(!aHTMLEditor)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  bool bMixed;
  nsAutoString localName;
  nsresult rv = GetListState(aHTMLEditor, &bMixed, localName);
  NS_ENSURE_SUCCESS(rv, rv);

  bool inList = aTagName->Equals(localName);
  aParams.SetBool(STATE_ALL, !bMixed && inList);
  aParams.SetBool(STATE_MIXED, bMixed);
  aParams.SetBool(STATE_ENABLED, true);
  return NS_OK;
}

<<<<<<< HEAD
nsresult ListCommand::ToggleState(HTMLEditor* aHTMLEditor) {
  if (NS_WARN_IF(!aHTMLEditor)) {
||||||| merged common ancestors
nsresult
ListCommand::ToggleState(HTMLEditor* aHTMLEditor)
{
  if (NS_WARN_IF(!aHTMLEditor)) {
=======
nsresult ListCommand::ToggleState(nsAtom* aTagName, HTMLEditor* aHTMLEditor,
                                  nsIPrincipal* aPrincipal) const {
  if (NS_WARN_IF(!aTagName) || NS_WARN_IF(!aHTMLEditor)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  nsresult rv;
  RefPtr<nsCommandParams> params = new nsCommandParams();
  rv = GetCurrentState(aTagName, aHTMLEditor, *params);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  ErrorResult error;
  bool inList = params->GetBool(STATE_ALL, error);
  if (NS_WARN_IF(error.Failed())) {
    return error.StealNSResult();
  }

  nsDependentAtomString listType(aTagName);
  if (inList) {
    rv = aHTMLEditor->RemoveListAsAction(listType, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "RemoveListAsAction() failed");
    return rv;
  }

  rv = aHTMLEditor->MakeOrChangeListAsAction(listType, false, EmptyString(),
                                             aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "MakeOrChangeListAsAction() failed");
  return rv;
}

<<<<<<< HEAD
ListItemCommand::ListItemCommand(nsAtom* aTagName)
    : StateUpdatingCommandBase(aTagName) {}
||||||| merged common ancestors
ListItemCommand::ListItemCommand(nsAtom* aTagName)
  : StateUpdatingCommandBase(aTagName)
{
}
=======
/*****************************************************************************
 * mozilla::ListItemCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult ListItemCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                          nsICommandParams* aParams) {
  if (NS_WARN_IF(!aHTMLEditor)) {
||||||| merged common ancestors
nsresult
ListItemCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                 nsICommandParams *aParams)
{
  if (NS_WARN_IF(!aHTMLEditor)) {
=======
StaticRefPtr<ListItemCommand> ListItemCommand::sInstance;

nsresult ListItemCommand::GetCurrentState(nsAtom* aTagName,
                                          HTMLEditor* aHTMLEditor,
                                          nsCommandParams& aParams) const {
  if (NS_WARN_IF(!aTagName) || NS_WARN_IF(!aHTMLEditor)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  bool bMixed, bLI, bDT, bDD;
  nsresult rv = aHTMLEditor->GetListItemState(&bMixed, &bLI, &bDT, &bDD);
  NS_ENSURE_SUCCESS(rv, rv);

  bool inList = false;
  if (!bMixed) {
    if (bLI) {
      inList = aTagName == nsGkAtoms::li;
    } else if (bDT) {
      inList = aTagName == nsGkAtoms::dt;
    } else if (bDD) {
      inList = aTagName == nsGkAtoms::dd;
    }
  }

  aParams.SetBool(STATE_ALL, !bMixed && inList);
  aParams.SetBool(STATE_MIXED, bMixed);

  return NS_OK;
}

<<<<<<< HEAD
nsresult ListItemCommand::ToggleState(HTMLEditor* aHTMLEditor) {
  if (NS_WARN_IF(!aHTMLEditor)) {
||||||| merged common ancestors
nsresult
ListItemCommand::ToggleState(HTMLEditor* aHTMLEditor)
{
  if (NS_WARN_IF(!aHTMLEditor)) {
=======
nsresult ListItemCommand::ToggleState(nsAtom* aTagName, HTMLEditor* aHTMLEditor,
                                      nsIPrincipal* aPrincipal) const {
  if (NS_WARN_IF(!aTagName) || NS_WARN_IF(!aHTMLEditor)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  // Need to use aTagName????
  RefPtr<nsCommandParams> params = new nsCommandParams();
  GetCurrentState(aTagName, aHTMLEditor, *params);
  ErrorResult error;
  bool inList = params->GetBool(STATE_ALL, error);
  if (NS_WARN_IF(error.Failed())) {
    return error.StealNSResult();
  }

  if (inList) {
    // To remove a list, first get what kind of list we're in
    bool bMixed;
    nsAutoString localName;
    nsresult rv = GetListState(aHTMLEditor, &bMixed, localName);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    if (localName.IsEmpty() || bMixed) {
      return NS_OK;
    }
    rv = aHTMLEditor->RemoveListAsAction(localName, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "RemoveListAsAction() failed");
    return rv;
  }

  // Set to the requested paragraph type
  // XXX Note: This actually doesn't work for "LI",
  //    but we currently don't use this for non DL lists anyway.
  // Problem: won't this replace any current block paragraph style?
  nsresult rv = aHTMLEditor->SetParagraphFormatAsAction(
      nsDependentAtomString(aTagName), aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetParagraphFormatAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
RemoveListCommand::IsCommandEnabled(const char* aCommandName,
                                    nsISupports* refCon, bool* outCmdEnabled) {
  *outCmdEnabled = false;
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
||||||| merged common ancestors
NS_IMETHODIMP
RemoveListCommand::IsCommandEnabled(const char* aCommandName,
                                    nsISupports* refCon,
                                    bool* outCmdEnabled)
{
  *outCmdEnabled = false;
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
=======
/*****************************************************************************
 * mozilla::RemoveListCommand
 *****************************************************************************/
>>>>>>> upstream-releases

StaticRefPtr<RemoveListCommand> RemoveListCommand::sInstance;

bool RemoveListCommand::IsCommandEnabled(Command aCommand,
                                         TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }

  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
  }

  if (!htmlEditor->IsSelectionEditable()) {
    return false;
  }

  // It is enabled if we are in any list type
  bool bMixed;
  nsAutoString localName;
  nsresult rv = GetListState(MOZ_KnownLive(htmlEditor), &bMixed, localName);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return false;
  }
  return bMixed || !localName.IsEmpty();
}

<<<<<<< HEAD
NS_IMETHODIMP
RemoveListCommand::DoCommand(const char* aCommandName, nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
||||||| merged common ancestors

NS_IMETHODIMP
RemoveListCommand::DoCommand(const char* aCommandName, nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
=======
nsresult RemoveListCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                      nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
>>>>>>> upstream-releases
    return NS_OK;
  }
  // This removes any list type
<<<<<<< HEAD
  return htmlEditor->RemoveList(EmptyString());
}

NS_IMETHODIMP
RemoveListCommand::DoCommandParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
}

NS_IMETHODIMP
RemoveListCommand::GetCommandStateParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon) {
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
}

NS_IMETHODIMP
IndentCommand::IsCommandEnabled(const char* aCommandName, nsISupports* refCon,
                                bool* outCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
  }
  mozilla::EditorBase* editorBase = editor->AsEditorBase();
  MOZ_ASSERT(editorBase);
  *outCmdEnabled = editorBase->IsSelectionEditable();
  return NS_OK;
||||||| merged common ancestors
  return htmlEditor->RemoveList(EmptyString());
}

NS_IMETHODIMP
RemoveListCommand::DoCommandParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
}

NS_IMETHODIMP
RemoveListCommand::GetCommandStateParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon)
{
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
}

NS_IMETHODIMP
IndentCommand::IsCommandEnabled(const char* aCommandName,
                                nsISupports* refCon, bool* outCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
  }
  mozilla::EditorBase* editorBase = editor->AsEditorBase();
  MOZ_ASSERT(editorBase);
  *outCmdEnabled = editorBase->IsSelectionEditable();
  return NS_OK;
=======
  nsresult rv = htmlEditor->RemoveListAsAction(EmptyString(), aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "RemoveListAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
IndentCommand::DoCommand(const char* aCommandName, nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
IndentCommand::DoCommand(const char* aCommandName, nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
=======
nsresult RemoveListCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
}

/*****************************************************************************
 * mozilla::IndentCommand
 *****************************************************************************/

StaticRefPtr<IndentCommand> IndentCommand::sInstance;

bool IndentCommand::IsCommandEnabled(Command aCommand,
                                     TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
  }
  return htmlEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
IndentCommand::DoCommandParams(const char* aCommandName,
                               nsICommandParams* aParams, nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
||||||| merged common ancestors
NS_IMETHODIMP
IndentCommand::DoCommandParams(const char* aCommandName,
                               nsICommandParams* aParams,
                               nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
=======
nsresult IndentCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                  nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_OK;
  }
  nsresult rv = MOZ_KnownLive(htmlEditor)->IndentAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "IndentAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
IndentCommand::GetCommandStateParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* refCon) {
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
||||||| merged common ancestors
NS_IMETHODIMP
IndentCommand::GetCommandStateParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* refCon)
{
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
=======
nsresult IndentCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
// OUTDENT
||||||| merged common ancestors

//OUTDENT
=======
/*****************************************************************************
 * mozilla::OutdentCommand
 *****************************************************************************/

StaticRefPtr<OutdentCommand> OutdentCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
OutdentCommand::IsCommandEnabled(const char* aCommandName, nsISupports* refCon,
                                 bool* outCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
OutdentCommand::IsCommandEnabled(const char* aCommandName,
                                 nsISupports* refCon,
                                 bool* outCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
=======
bool OutdentCommand::IsCommandEnabled(Command aCommand,
                                      TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  mozilla::EditorBase* editorBase = editor->AsEditorBase();
  MOZ_ASSERT(editorBase);
  *outCmdEnabled = editorBase->IsSelectionEditable();
  return NS_OK;
}

NS_IMETHODIMP
OutdentCommand::DoCommand(const char* aCommandName, nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
||||||| merged common ancestors
  mozilla::EditorBase* editorBase = editor->AsEditorBase();
  MOZ_ASSERT(editorBase);
  *outCmdEnabled = editorBase->IsSelectionEditable();
  return NS_OK;
}

NS_IMETHODIMP
OutdentCommand::DoCommand(const char* aCommandName, nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
=======
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
>>>>>>> upstream-releases
  if (!htmlEditor) {
    return false;
  }
  return htmlEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
OutdentCommand::DoCommandParams(const char* aCommandName,
                                nsICommandParams* aParams,
                                nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
}

NS_IMETHODIMP
OutdentCommand::GetCommandStateParams(const char* aCommandName,
                                      nsICommandParams* aParams,
                                      nsISupports* refCon) {
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
||||||| merged common ancestors
NS_IMETHODIMP
OutdentCommand::DoCommandParams(const char* aCommandName,
                                nsICommandParams* aParams,
                                nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
}

NS_IMETHODIMP
OutdentCommand::GetCommandStateParams(const char* aCommandName,
                                      nsICommandParams* aParams,
                                      nsISupports* refCon)
{
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
=======
nsresult OutdentCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                   nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_OK;
  }
  nsresult rv = MOZ_KnownLive(htmlEditor)->OutdentAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "OutdentAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
MultiStateCommandBase::MultiStateCommandBase() : HTMLEditorCommandBase() {}
||||||| merged common ancestors
MultiStateCommandBase::MultiStateCommandBase()
  : HTMLEditorCommandBase()
{
}
=======
nsresult OutdentCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
MultiStateCommandBase::~MultiStateCommandBase() {}
||||||| merged common ancestors
MultiStateCommandBase::~MultiStateCommandBase()
{
}
=======
/*****************************************************************************
 * mozilla::MultiStateCommandBase
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
MultiStateCommandBase::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* refCon,
                                        bool* outCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
MultiStateCommandBase::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* refCon,
                                        bool *outCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
=======
bool MultiStateCommandBase::IsCommandEnabled(Command aCommand,
                                             TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
>>>>>>> upstream-releases
  }
  // should be disabled sometimes, like if the current selection is an image
  return htmlEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
MultiStateCommandBase::DoCommand(const char* aCommandName,
                                 nsISupports* refCon) {
#ifdef DEBUG
  printf(
      "who is calling MultiStateCommandBase::DoCommand \
          (no implementation)? %s\n",
      aCommandName);
#endif

||||||| merged common ancestors
NS_IMETHODIMP
MultiStateCommandBase::DoCommand(const char* aCommandName, nsISupports* refCon)
{
#ifdef DEBUG
  printf("who is calling MultiStateCommandBase::DoCommand \
          (no implementation)? %s\n", aCommandName);
#endif

=======
nsresult MultiStateCommandBase::DoCommand(Command aCommand,
                                          TextEditor& aTextEditor,
                                          nsIPrincipal* aPrincipal) const {
  NS_WARNING(
      "who is calling MultiStateCommandBase::DoCommand (no implementation)?");
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
MultiStateCommandBase::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
||||||| merged common ancestors
NS_IMETHODIMP
MultiStateCommandBase::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
=======
nsresult MultiStateCommandBase::DoCommandParam(Command aCommand,
                                               const nsAString& aStringParam,
                                               TextEditor& aTextEditor,
                                               nsIPrincipal* aPrincipal) const {
  NS_WARNING_ASSERTION(aCommand != Command::FormatJustify,
                       "Command::FormatJustify should be used only for "
                       "IsCommandEnabled() and GetCommandStateParams()");
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
>>>>>>> upstream-releases
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
  nsresult rv = SetState(MOZ_KnownLive(htmlEditor), aStringParam, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetState() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
MultiStateCommandBase::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
||||||| merged common ancestors
NS_IMETHODIMP
MultiStateCommandBase::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
=======
nsresult MultiStateCommandBase::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  if (!aTextEditor) {
>>>>>>> upstream-releases
    return NS_OK;
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
  return GetCurrentState(MOZ_KnownLive(htmlEditor), aParams);
}

<<<<<<< HEAD
ParagraphStateCommand::ParagraphStateCommand() : MultiStateCommandBase() {}
||||||| merged common ancestors
ParagraphStateCommand::ParagraphStateCommand()
  : MultiStateCommandBase()
{
}
=======
/*****************************************************************************
 * mozilla::ParagraphStateCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult ParagraphStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                                nsICommandParams* aParams) {
||||||| merged common ancestors
nsresult
ParagraphStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                       nsICommandParams* aParams)
{
=======
StaticRefPtr<ParagraphStateCommand> ParagraphStateCommand::sInstance;

nsresult ParagraphStateCommand::GetCurrentState(
    HTMLEditor* aHTMLEditor, nsCommandParams& aParams) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  bool outMixed;
  nsAutoString outStateString;
  nsresult rv = aHTMLEditor->GetParagraphState(&outMixed, outStateString);
  if (NS_SUCCEEDED(rv)) {
    nsAutoCString tOutStateString;
    LossyCopyUTF16toASCII(outStateString, tOutStateString);
    aParams.SetBool(STATE_MIXED, outMixed);
    aParams.SetCString(STATE_ATTRIBUTE, tOutStateString);
  }
  return rv;
}

<<<<<<< HEAD
nsresult ParagraphStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                         const nsString& newState) {
||||||| merged common ancestors
nsresult
ParagraphStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                const nsString& newState)
{
=======
nsresult ParagraphStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                         const nsAString& aNewState,
                                         nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsresult rv = aHTMLEditor->SetParagraphFormatAsAction(aNewState, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetParagraphFormatAsAction() failed");
  return rv;
}

<<<<<<< HEAD
FontFaceStateCommand::FontFaceStateCommand() : MultiStateCommandBase() {}
||||||| merged common ancestors
FontFaceStateCommand::FontFaceStateCommand()
  : MultiStateCommandBase()
{
}
=======
/*****************************************************************************
 * mozilla::FontFaceStateCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult FontFaceStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                               nsICommandParams* aParams) {
||||||| merged common ancestors
nsresult
FontFaceStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                      nsICommandParams* aParams)
{
=======
StaticRefPtr<FontFaceStateCommand> FontFaceStateCommand::sInstance;

nsresult FontFaceStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                               nsCommandParams& aParams) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsAutoString outStateString;
  bool outMixed;
  nsresult rv = aHTMLEditor->GetFontFaceState(&outMixed, outStateString);
  if (NS_SUCCEEDED(rv)) {
    aParams.SetBool(STATE_MIXED, outMixed);
    aParams.SetCString(STATE_ATTRIBUTE, NS_ConvertUTF16toUTF8(outStateString));
  }
  return rv;
}

<<<<<<< HEAD
nsresult FontFaceStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                        const nsString& newState) {
||||||| merged common ancestors
nsresult
FontFaceStateCommand::SetState(HTMLEditor* aHTMLEditor,
                               const nsString& newState)
{
=======
nsresult FontFaceStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                        const nsAString& aNewState,
                                        nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  if (aNewState.EqualsLiteral("tt")) {
    // The old "teletype" attribute
<<<<<<< HEAD
    nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
        *nsGkAtoms::tt, nullptr, EmptyString());
||||||| merged common ancestors
    nsresult rv = aHTMLEditor->SetInlineProperty(*nsGkAtoms::tt, nullptr,
                                                 EmptyString());
=======
    nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
        *nsGkAtoms::tt, nullptr, EmptyString(), aPrincipal);
>>>>>>> upstream-releases
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    // Clear existing font face
<<<<<<< HEAD
    rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::font,
                                                   nsGkAtoms::face);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
||||||| merged common ancestors
    rv = aHTMLEditor->RemoveInlineProperty(nsGkAtoms::font, nsGkAtoms::face);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
=======
    rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::font,
                                                   nsGkAtoms::face, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                         "RemoveInlinePropertyAsAction() failed");
    return rv;
>>>>>>> upstream-releases
  }

  // Remove any existing TT nodes
<<<<<<< HEAD
  nsresult rv =
      aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::tt, nullptr);
||||||| merged common ancestors
  nsresult rv = aHTMLEditor->RemoveInlineProperty(nsGkAtoms::tt, nullptr);
=======
  nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::tt,
                                                          nullptr, aPrincipal);
>>>>>>> upstream-releases
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

<<<<<<< HEAD
  if (newState.IsEmpty() || newState.EqualsLiteral("normal")) {
    rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::font,
                                                   nsGkAtoms::face);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  rv = aHTMLEditor->SetInlinePropertyAsAction(*nsGkAtoms::font, nsGkAtoms::face,
                                              newState);
  if (NS_WARN_IF(NS_FAILED(rv))) {
||||||| merged common ancestors
  if (newState.IsEmpty() || newState.EqualsLiteral("normal")) {
    rv = aHTMLEditor->RemoveInlineProperty(nsGkAtoms::font, nsGkAtoms::face);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  rv = aHTMLEditor->SetInlineProperty(*nsGkAtoms::font,
                                      nsGkAtoms::face,
                                      newState);
  if (NS_WARN_IF(NS_FAILED(rv))) {
=======
  if (aNewState.IsEmpty() || aNewState.EqualsLiteral("normal")) {
    rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::font,
                                                   nsGkAtoms::face, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                         "RemoveInlinePropertyAsAction() failed");
>>>>>>> upstream-releases
    return rv;
  }

<<<<<<< HEAD
FontSizeStateCommand::FontSizeStateCommand() : MultiStateCommandBase() {}
||||||| merged common ancestors
FontSizeStateCommand::FontSizeStateCommand()
  : MultiStateCommandBase()
{
}
=======
  rv = aHTMLEditor->SetInlinePropertyAsAction(*nsGkAtoms::font, nsGkAtoms::face,
                                              aNewState, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetInlinePropertyAsAction() failed");
  return rv;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult FontSizeStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                               nsICommandParams* aParams) {
||||||| merged common ancestors
nsresult
FontSizeStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                      nsICommandParams* aParams)
{
=======
/*****************************************************************************
 * mozilla::FontSizeStateCommand
 *****************************************************************************/

StaticRefPtr<FontSizeStateCommand> FontSizeStateCommand::sInstance;

nsresult FontSizeStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                               nsCommandParams& aParams) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsAutoString outStateString;
  bool firstHas, anyHas, allHas;
  nsresult rv = aHTMLEditor->GetInlinePropertyWithAttrValue(
      nsGkAtoms::font, nsGkAtoms::size, EmptyString(), &firstHas, &anyHas,
      &allHas, outStateString);
  NS_ENSURE_SUCCESS(rv, rv);

  nsAutoCString tOutStateString;
  LossyCopyUTF16toASCII(outStateString, tOutStateString);
  aParams.SetBool(STATE_MIXED, anyHas && !allHas);
  aParams.SetCString(STATE_ATTRIBUTE, tOutStateString);
  aParams.SetBool(STATE_ENABLED, true);

  return rv;
}

<<<<<<< HEAD
// acceptable values for "newState" are:
||||||| merged common ancestors

// acceptable values for "newState" are:
=======
// acceptable values for "aNewState" are:
>>>>>>> upstream-releases
//   -2
//   -1
//    0
//   +1
//   +2
//   +3
//   medium
//   normal
<<<<<<< HEAD
nsresult FontSizeStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                        const nsString& newState) {
||||||| merged common ancestors
nsresult
FontSizeStateCommand::SetState(HTMLEditor* aHTMLEditor,
                               const nsString& newState)
{
=======
nsresult FontSizeStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                        const nsAString& aNewState,
                                        nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

<<<<<<< HEAD
  if (!newState.IsEmpty() && !newState.EqualsLiteral("normal") &&
      !newState.EqualsLiteral("medium")) {
    nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
        *nsGkAtoms::font, nsGkAtoms::size, newState);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
||||||| merged common ancestors
  if (!newState.IsEmpty() &&
      !newState.EqualsLiteral("normal") &&
      !newState.EqualsLiteral("medium")) {
    nsresult rv = aHTMLEditor->SetInlineProperty(*nsGkAtoms::font,
                                                 nsGkAtoms::size,
                                                 newState);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
=======
  if (!aNewState.IsEmpty() && !aNewState.EqualsLiteral("normal") &&
      !aNewState.EqualsLiteral("medium")) {
    nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
        *nsGkAtoms::font, nsGkAtoms::size, aNewState, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                         "SetInlinePropertyAsAction() failed");
    return rv;
>>>>>>> upstream-releases
  }

  // remove any existing font size, big or small
<<<<<<< HEAD
  nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::font,
                                                          nsGkAtoms::size);
||||||| merged common ancestors
  nsresult rv = aHTMLEditor->RemoveInlineProperty(nsGkAtoms::font,
                                                  nsGkAtoms::size);
=======
  nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(
      *nsGkAtoms::font, nsGkAtoms::size, aPrincipal);
>>>>>>> upstream-releases
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

<<<<<<< HEAD
  rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::big, nullptr);
||||||| merged common ancestors
  rv = aHTMLEditor->RemoveInlineProperty(nsGkAtoms::big, nullptr);
=======
  rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::big, nullptr,
                                                 aPrincipal);
>>>>>>> upstream-releases
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

<<<<<<< HEAD
  rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::small, nullptr);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
||||||| merged common ancestors
  rv = aHTMLEditor->RemoveInlineProperty(nsGkAtoms::small, nullptr);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
=======
  rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::small, nullptr,
                                                 aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                       "RemoveInlinePropertyAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
FontColorStateCommand::FontColorStateCommand() : MultiStateCommandBase() {}
||||||| merged common ancestors
FontColorStateCommand::FontColorStateCommand()
  : MultiStateCommandBase()
{
}
=======
/*****************************************************************************
 * mozilla::FontColorStateCommand
 *****************************************************************************/

StaticRefPtr<FontColorStateCommand> FontColorStateCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult FontColorStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                                nsICommandParams* aParams) {
||||||| merged common ancestors
nsresult
FontColorStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                       nsICommandParams* aParams)
{
=======
nsresult FontColorStateCommand::GetCurrentState(
    HTMLEditor* aHTMLEditor, nsCommandParams& aParams) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  bool outMixed;
  nsAutoString outStateString;
  nsresult rv = aHTMLEditor->GetFontColorState(&outMixed, outStateString);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  nsAutoCString tOutStateString;
  LossyCopyUTF16toASCII(outStateString, tOutStateString);
  aParams.SetBool(STATE_MIXED, outMixed);
  aParams.SetCString(STATE_ATTRIBUTE, tOutStateString);
  return NS_OK;
}

<<<<<<< HEAD
nsresult FontColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                         const nsString& newState) {
||||||| merged common ancestors
nsresult
FontColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                const nsString& newState)
{
=======
nsresult FontColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                         const nsAString& aNewState,
                                         nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

<<<<<<< HEAD
  if (newState.IsEmpty() || newState.EqualsLiteral("normal")) {
    nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::font,
                                                            nsGkAtoms::color);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
      *nsGkAtoms::font, nsGkAtoms::color, newState);
  if (NS_WARN_IF(NS_FAILED(rv))) {
||||||| merged common ancestors
  if (newState.IsEmpty() || newState.EqualsLiteral("normal")) {
    nsresult rv = aHTMLEditor->RemoveInlineProperty(nsGkAtoms::font,
                                                    nsGkAtoms::color);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  nsresult rv = aHTMLEditor->SetInlineProperty(*nsGkAtoms::font,
                                               nsGkAtoms::color,
                                               newState);
  if (NS_WARN_IF(NS_FAILED(rv))) {
=======
  if (aNewState.IsEmpty() || aNewState.EqualsLiteral("normal")) {
    nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(
        *nsGkAtoms::font, nsGkAtoms::color, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                         "RemoveInlinePropertyAsAction() failed");
>>>>>>> upstream-releases
    return rv;
  }

<<<<<<< HEAD
HighlightColorStateCommand::HighlightColorStateCommand()
    : MultiStateCommandBase() {}
||||||| merged common ancestors
HighlightColorStateCommand::HighlightColorStateCommand()
  : MultiStateCommandBase()
{
}
=======
  nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
      *nsGkAtoms::font, nsGkAtoms::color, aNewState, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetInlinePropertyAsAction() failed");
  return rv;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult HighlightColorStateCommand::GetCurrentState(
    HTMLEditor* aHTMLEditor, nsICommandParams* aParams) {
||||||| merged common ancestors
nsresult
HighlightColorStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                            nsICommandParams* aParams)
{
=======
/*****************************************************************************
 * mozilla::HighlightColorStateCommand
 *****************************************************************************/

StaticRefPtr<HighlightColorStateCommand> HighlightColorStateCommand::sInstance;

nsresult HighlightColorStateCommand::GetCurrentState(
    HTMLEditor* aHTMLEditor, nsCommandParams& aParams) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  bool outMixed;
  nsAutoString outStateString;
  nsresult rv = aHTMLEditor->GetHighlightColorState(&outMixed, outStateString);
  NS_ENSURE_SUCCESS(rv, rv);

  nsAutoCString tOutStateString;
  LossyCopyUTF16toASCII(outStateString, tOutStateString);
  aParams.SetBool(STATE_MIXED, outMixed);
  aParams.SetCString(STATE_ATTRIBUTE, tOutStateString);
  return NS_OK;
}

<<<<<<< HEAD
nsresult HighlightColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                              const nsString& newState) {
||||||| merged common ancestors
nsresult
HighlightColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                     const nsString& newState)
{
=======
nsresult HighlightColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                              const nsAString& aNewState,
                                              nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

<<<<<<< HEAD
  if (newState.IsEmpty() || newState.EqualsLiteral("normal")) {
    nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(*nsGkAtoms::font,
                                                            nsGkAtoms::bgcolor);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
      *nsGkAtoms::font, nsGkAtoms::bgcolor, newState);
  if (NS_WARN_IF(NS_FAILED(rv))) {
||||||| merged common ancestors
  if (newState.IsEmpty() || newState.EqualsLiteral("normal")) {
    nsresult rv = aHTMLEditor->RemoveInlineProperty(nsGkAtoms::font,
                                                    nsGkAtoms::bgcolor);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  nsresult rv = aHTMLEditor->SetInlineProperty(*nsGkAtoms::font,
                                               nsGkAtoms::bgcolor,
                                               newState);
  if (NS_WARN_IF(NS_FAILED(rv))) {
=======
  if (aNewState.IsEmpty() || aNewState.EqualsLiteral("normal")) {
    nsresult rv = aHTMLEditor->RemoveInlinePropertyAsAction(
        *nsGkAtoms::font, nsGkAtoms::bgcolor, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                         "RemoveInlinePropertyAsAction() failed");
>>>>>>> upstream-releases
    return rv;
  }

<<<<<<< HEAD
NS_IMETHODIMP
HighlightColorStateCommand::IsCommandEnabled(const char* aCommandName,
                                             nsISupports* refCon,
                                             bool* outCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
  }
  mozilla::EditorBase* editorBase = editor->AsEditorBase();
  MOZ_ASSERT(editorBase);
  *outCmdEnabled = editorBase->IsSelectionEditable();
  return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
HighlightColorStateCommand::IsCommandEnabled(const char* aCommandName,
                                             nsISupports* refCon,
                                             bool* outCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
  }
  mozilla::EditorBase* editorBase = editor->AsEditorBase();
  MOZ_ASSERT(editorBase);
  *outCmdEnabled = editorBase->IsSelectionEditable();
  return NS_OK;
=======
  nsresult rv = aHTMLEditor->SetInlinePropertyAsAction(
      *nsGkAtoms::font, nsGkAtoms::bgcolor, aNewState, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetInlinePropertyAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
BackgroundColorStateCommand::BackgroundColorStateCommand()
    : MultiStateCommandBase() {}
||||||| merged common ancestors
BackgroundColorStateCommand::BackgroundColorStateCommand()
  : MultiStateCommandBase()
{
}
=======
/*****************************************************************************
 * mozilla::BackgroundColorStateCommand
 *****************************************************************************/

StaticRefPtr<BackgroundColorStateCommand>
    BackgroundColorStateCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult BackgroundColorStateCommand::GetCurrentState(
    HTMLEditor* aHTMLEditor, nsICommandParams* aParams) {
||||||| merged common ancestors
nsresult
BackgroundColorStateCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                             nsICommandParams* aParams)
{
=======
nsresult BackgroundColorStateCommand::GetCurrentState(
    HTMLEditor* aHTMLEditor, nsCommandParams& aParams) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  bool outMixed;
  nsAutoString outStateString;
  nsresult rv = aHTMLEditor->GetBackgroundColorState(&outMixed, outStateString);
  NS_ENSURE_SUCCESS(rv, rv);

  nsAutoCString tOutStateString;
  LossyCopyUTF16toASCII(outStateString, tOutStateString);
  aParams.SetBool(STATE_MIXED, outMixed);
  aParams.SetCString(STATE_ATTRIBUTE, tOutStateString);
  return NS_OK;
}

<<<<<<< HEAD
nsresult BackgroundColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                               const nsString& newState) {
||||||| merged common ancestors
nsresult
BackgroundColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                      const nsString& newState)
{
=======
nsresult BackgroundColorStateCommand::SetState(HTMLEditor* aHTMLEditor,
                                               const nsAString& aNewState,
                                               nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsresult rv = aHTMLEditor->SetBackgroundColorAsAction(aNewState, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetBackgroundColorAsAction() failed");
  return rv;
}

<<<<<<< HEAD
AlignCommand::AlignCommand() : MultiStateCommandBase() {}
||||||| merged common ancestors
AlignCommand::AlignCommand()
  : MultiStateCommandBase()
{
}
=======
/*****************************************************************************
 * mozilla::AlignCommand
 *****************************************************************************/

StaticRefPtr<AlignCommand> AlignCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult AlignCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                       nsICommandParams* aParams) {
||||||| merged common ancestors
nsresult
AlignCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                              nsICommandParams* aParams)
{
=======
nsresult AlignCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                       nsCommandParams& aParams) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsIHTMLEditor::EAlignment firstAlign;
  bool outMixed;
  nsresult rv = aHTMLEditor->GetAlignment(&outMixed, &firstAlign);

  NS_ENSURE_SUCCESS(rv, rv);

  nsAutoString outStateString;
  switch (firstAlign) {
    default:
    case nsIHTMLEditor::eLeft:
      outStateString.AssignLiteral("left");
      break;

    case nsIHTMLEditor::eCenter:
      outStateString.AssignLiteral("center");
      break;

    case nsIHTMLEditor::eRight:
      outStateString.AssignLiteral("right");
      break;

    case nsIHTMLEditor::eJustify:
      outStateString.AssignLiteral("justify");
      break;
  }
  nsAutoCString tOutStateString;
  LossyCopyUTF16toASCII(outStateString, tOutStateString);
  aParams.SetBool(STATE_MIXED, outMixed);
  aParams.SetCString(STATE_ATTRIBUTE, tOutStateString);
  return NS_OK;
}

<<<<<<< HEAD
nsresult AlignCommand::SetState(HTMLEditor* aHTMLEditor,
                                const nsString& newState) {
||||||| merged common ancestors
nsresult
AlignCommand::SetState(HTMLEditor* aHTMLEditor,
                       const nsString& newState)
{
=======
nsresult AlignCommand::SetState(HTMLEditor* aHTMLEditor,
                                const nsAString& aNewState,
                                nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsresult rv = aHTMLEditor->AlignAsAction(aNewState, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "AlignAsAction() failed");
  return rv;
}

<<<<<<< HEAD
AbsolutePositioningCommand::AbsolutePositioningCommand()
    : StateUpdatingCommandBase(nsGkAtoms::_empty) {}
||||||| merged common ancestors
AbsolutePositioningCommand::AbsolutePositioningCommand()
  : StateUpdatingCommandBase(nsGkAtoms::_empty)
{
}
=======
/*****************************************************************************
 * mozilla::AbsolutePositioningCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
AbsolutePositioningCommand::IsCommandEnabled(const char* aCommandName,
                                             nsISupports* aCommandRefCon,
                                             bool* aOutCmdEnabled) {
  *aOutCmdEnabled = false;
||||||| merged common ancestors
NS_IMETHODIMP
AbsolutePositioningCommand::IsCommandEnabled(const char* aCommandName,
                                             nsISupports* aCommandRefCon,
                                             bool* aOutCmdEnabled)
{
  *aOutCmdEnabled = false;
=======
StaticRefPtr<AbsolutePositioningCommand> AbsolutePositioningCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
    return NS_OK;
  }
  if (!htmlEditor->IsSelectionEditable()) {
    return NS_OK;
  }
  *aOutCmdEnabled = htmlEditor->IsAbsolutePositionEditorEnabled();
  return NS_OK;
}

nsresult AbsolutePositioningCommand::GetCurrentState(
    HTMLEditor* aHTMLEditor, nsICommandParams* aParams) {
||||||| merged common ancestors
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
    return NS_OK;
  }
  if (!htmlEditor->IsSelectionEditable()) {
    return NS_OK;
  }
  *aOutCmdEnabled = htmlEditor->IsAbsolutePositionEditorEnabled();
  return NS_OK;
}

nsresult
AbsolutePositioningCommand::GetCurrentState(HTMLEditor* aHTMLEditor,
                                            nsICommandParams* aParams)
{
=======
nsresult AbsolutePositioningCommand::GetCurrentState(
    nsAtom* aTagName, HTMLEditor* aHTMLEditor, nsCommandParams& aParams) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  if (!aHTMLEditor->IsAbsolutePositionEditorEnabled()) {
    aParams.SetBool(STATE_MIXED, false);
    aParams.SetCString(STATE_ATTRIBUTE, EmptyCString());
    return NS_OK;
  }

  RefPtr<Element> container =
<<<<<<< HEAD
      aHTMLEditor->GetAbsolutelyPositionedSelectionContainer();
  params->SetBool(STATE_MIXED, false);
  params->SetCString(STATE_ATTRIBUTE, container ? NS_LITERAL_CSTRING("absolute")
                                                : EmptyCString());
||||||| merged common ancestors
    aHTMLEditor->GetAbsolutelyPositionedSelectionContainer();
  params->SetBool(STATE_MIXED,  false);
  params->SetCString(STATE_ATTRIBUTE,
                     container ? NS_LITERAL_CSTRING("absolute") :
                                 EmptyCString());
=======
      aHTMLEditor->GetAbsolutelyPositionedSelectionContainer();
  aParams.SetBool(STATE_MIXED, false);
  aParams.SetCString(STATE_ATTRIBUTE, container ? NS_LITERAL_CSTRING("absolute")
                                                : EmptyCString());
>>>>>>> upstream-releases
  return NS_OK;
}

<<<<<<< HEAD
nsresult AbsolutePositioningCommand::ToggleState(HTMLEditor* aHTMLEditor) {
||||||| merged common ancestors
nsresult
AbsolutePositioningCommand::ToggleState(HTMLEditor* aHTMLEditor)
{
=======
nsresult AbsolutePositioningCommand::ToggleState(
    nsAtom* aTagName, HTMLEditor* aHTMLEditor, nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  if (NS_WARN_IF(!aHTMLEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  RefPtr<Element> container =
<<<<<<< HEAD
      aHTMLEditor->GetAbsolutelyPositionedSelectionContainer();
  return aHTMLEditor->SetSelectionToAbsoluteOrStatic(!container);
||||||| merged common ancestors
    aHTMLEditor->GetAbsolutelyPositionedSelectionContainer();
  return aHTMLEditor->SetSelectionToAbsoluteOrStatic(!container);
=======
      aHTMLEditor->GetAbsolutelyPositionedSelectionContainer();
  nsresult rv = aHTMLEditor->SetSelectionToAbsoluteOrStaticAsAction(!container,
                                                                    aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                       "SetSelectionToAbsoluteOrStaticAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
DecreaseZIndexCommand::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* aRefCon,
                                        bool* aOutCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors

NS_IMETHODIMP
DecreaseZIndexCommand::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* aRefCon,
                                        bool* aOutCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
/*****************************************************************************
 * mozilla::DecreaseZIndexCommand
 *****************************************************************************/

StaticRefPtr<DecreaseZIndexCommand> DecreaseZIndexCommand::sInstance;

bool DecreaseZIndexCommand::IsCommandEnabled(Command aCommand,
                                             TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
  }
  if (!htmlEditor->IsAbsolutePositionEditorEnabled()) {
    return false;
  }
  RefPtr<Element> positionedElement = htmlEditor->GetPositionedElement();
  if (!positionedElement) {
    return false;
  }
  return htmlEditor->GetZIndex(*positionedElement) > 0;
}

<<<<<<< HEAD
NS_IMETHODIMP
DecreaseZIndexCommand::DoCommand(const char* aCommandName,
                                 nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
||||||| merged common ancestors
NS_IMETHODIMP
DecreaseZIndexCommand::DoCommand(const char* aCommandName,
                                 nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
=======
nsresult DecreaseZIndexCommand::DoCommand(Command aCommand,
                                          TextEditor& aTextEditor,
                                          nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
>>>>>>> upstream-releases
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
  nsresult rv = htmlEditor->AddZIndexAsAction(-1, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "AddZIndexAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
DecreaseZIndexCommand::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
||||||| merged common ancestors
NS_IMETHODIMP
DecreaseZIndexCommand::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
=======
nsresult DecreaseZIndexCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
DecreaseZIndexCommand::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* refCon) {
  NS_ENSURE_ARG_POINTER(aParams);
||||||| merged common ancestors
NS_IMETHODIMP
DecreaseZIndexCommand::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* refCon)
{
  NS_ENSURE_ARG_POINTER(aParams);
=======
/*****************************************************************************
 * mozilla::IncreaseZIndexCommand
 *****************************************************************************/
>>>>>>> upstream-releases

StaticRefPtr<IncreaseZIndexCommand> IncreaseZIndexCommand::sInstance;

<<<<<<< HEAD
NS_IMETHODIMP
IncreaseZIndexCommand::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* aRefCon,
                                        bool* aOutCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
IncreaseZIndexCommand::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* aRefCon,
                                        bool* aOutCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
bool IncreaseZIndexCommand::IsCommandEnabled(Command aCommand,
                                             TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
  }
  if (!htmlEditor->IsAbsolutePositionEditorEnabled()) {
    return false;
  }
  return !!htmlEditor->GetPositionedElement();
}

<<<<<<< HEAD
NS_IMETHODIMP
IncreaseZIndexCommand::DoCommand(const char* aCommandName,
                                 nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
||||||| merged common ancestors
NS_IMETHODIMP
IncreaseZIndexCommand::DoCommand(const char* aCommandName,
                                 nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
=======
nsresult IncreaseZIndexCommand::DoCommand(Command aCommand,
                                          TextEditor& aTextEditor,
                                          nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
>>>>>>> upstream-releases
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
  nsresult rv = htmlEditor->AddZIndexAsAction(1, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "AddZIndexAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
IncreaseZIndexCommand::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
||||||| merged common ancestors
NS_IMETHODIMP
IncreaseZIndexCommand::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
=======
nsresult IncreaseZIndexCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
IncreaseZIndexCommand::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* refCon) {
  NS_ENSURE_ARG_POINTER(aParams);
||||||| merged common ancestors
NS_IMETHODIMP
IncreaseZIndexCommand::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* refCon)
{
  NS_ENSURE_ARG_POINTER(aParams);
=======
/*****************************************************************************
 * mozilla::RemoveStylesCommand
 *****************************************************************************/
>>>>>>> upstream-releases

StaticRefPtr<RemoveStylesCommand> RemoveStylesCommand::sInstance;

<<<<<<< HEAD
NS_IMETHODIMP
RemoveStylesCommand::IsCommandEnabled(const char* aCommandName,
                                      nsISupports* refCon,
                                      bool* outCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
RemoveStylesCommand::IsCommandEnabled(const char* aCommandName,
                                      nsISupports* refCon,
                                      bool* outCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
=======
bool RemoveStylesCommand::IsCommandEnabled(Command aCommand,
                                           TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
>>>>>>> upstream-releases
  }
  // test if we have any styles?
  return htmlEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
RemoveStylesCommand::DoCommand(const char* aCommandName, nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
||||||| merged common ancestors
NS_IMETHODIMP
RemoveStylesCommand::DoCommand(const char* aCommandName,
                               nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
=======
nsresult RemoveStylesCommand::DoCommand(Command aCommand,
                                        TextEditor& aTextEditor,
                                        nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
>>>>>>> upstream-releases
    return NS_OK;
  }
  nsresult rv =
      MOZ_KnownLive(htmlEditor)->RemoveAllInlinePropertiesAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                       "RemoveAllInlinePropertiesAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
RemoveStylesCommand::DoCommandParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
||||||| merged common ancestors
NS_IMETHODIMP
RemoveStylesCommand::DoCommandParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
=======
nsresult RemoveStylesCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
RemoveStylesCommand::GetCommandStateParams(const char* aCommandName,
                                           nsICommandParams* aParams,
                                           nsISupports* refCon) {
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
}
||||||| merged common ancestors
NS_IMETHODIMP
RemoveStylesCommand::GetCommandStateParams(const char* aCommandName,
                                           nsICommandParams* aParams,
                                           nsISupports* refCon)
{
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
}
=======
/*****************************************************************************
 * mozilla::IncreaseFontSizeCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
IncreaseFontSizeCommand::IsCommandEnabled(const char* aCommandName,
                                          nsISupports* refCon,
                                          bool* outCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
IncreaseFontSizeCommand::IsCommandEnabled(const char* aCommandName,
                                          nsISupports* refCon,
                                          bool* outCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
=======
StaticRefPtr<IncreaseFontSizeCommand> IncreaseFontSizeCommand::sInstance;

bool IncreaseFontSizeCommand::IsCommandEnabled(Command aCommand,
                                               TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
>>>>>>> upstream-releases
  }
  // test if we are at max size?
  return htmlEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
IncreaseFontSizeCommand::DoCommand(const char* aCommandName,
                                   nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
||||||| merged common ancestors

NS_IMETHODIMP
IncreaseFontSizeCommand::DoCommand(const char* aCommandName,
                                   nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
=======
nsresult IncreaseFontSizeCommand::DoCommand(Command aCommand,
                                            TextEditor& aTextEditor,
                                            nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
>>>>>>> upstream-releases
    return NS_OK;
  }
  nsresult rv = MOZ_KnownLive(htmlEditor)->IncreaseFontSizeAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "IncreaseFontSizeAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
IncreaseFontSizeCommand::DoCommandParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
||||||| merged common ancestors
NS_IMETHODIMP
IncreaseFontSizeCommand::DoCommandParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
=======
nsresult IncreaseFontSizeCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
IncreaseFontSizeCommand::GetCommandStateParams(const char* aCommandName,
                                               nsICommandParams* aParams,
                                               nsISupports* refCon) {
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
}
||||||| merged common ancestors
NS_IMETHODIMP
IncreaseFontSizeCommand::GetCommandStateParams(const char* aCommandName,
                                               nsICommandParams* aParams,
                                               nsISupports* refCon)
{
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
}
=======
/*****************************************************************************
 * mozilla::DecreaseFontSizeCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
DecreaseFontSizeCommand::IsCommandEnabled(const char* aCommandName,
                                          nsISupports* refCon,
                                          bool* outCmdEnabled) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
DecreaseFontSizeCommand::IsCommandEnabled(const char* aCommandName,
                                          nsISupports* refCon,
                                          bool* outCmdEnabled)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
=======
StaticRefPtr<DecreaseFontSizeCommand> DecreaseFontSizeCommand::sInstance;

bool DecreaseFontSizeCommand::IsCommandEnabled(Command aCommand,
                                               TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
>>>>>>> upstream-releases
  }
  // test if we are at min size?
  return htmlEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
DecreaseFontSizeCommand::DoCommand(const char* aCommandName,
                                   nsISupports* refCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
||||||| merged common ancestors
NS_IMETHODIMP
DecreaseFontSizeCommand::DoCommand(const char* aCommandName,
                                   nsISupports* refCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    return NS_OK;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
=======
nsresult DecreaseFontSizeCommand::DoCommand(Command aCommand,
                                            TextEditor& aTextEditor,
                                            nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
>>>>>>> upstream-releases
    return NS_OK;
  }
  nsresult rv = MOZ_KnownLive(htmlEditor)->DecreaseFontSizeAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "DecreaseFontSizeAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
DecreaseFontSizeCommand::DoCommandParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon) {
  return DoCommand(aCommandName, refCon);
||||||| merged common ancestors
NS_IMETHODIMP
DecreaseFontSizeCommand::DoCommandParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon)
{
  return DoCommand(aCommandName, refCon);
=======
nsresult DecreaseFontSizeCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
DecreaseFontSizeCommand::GetCommandStateParams(const char* aCommandName,
                                               nsICommandParams* aParams,
                                               nsISupports* refCon) {
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
}
||||||| merged common ancestors
NS_IMETHODIMP
DecreaseFontSizeCommand::GetCommandStateParams(const char* aCommandName,
                                               nsICommandParams* aParams,
                                               nsISupports* refCon)
{
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
}
=======
/*****************************************************************************
 * mozilla::InsertHTMLCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
InsertHTMLCommand::IsCommandEnabled(const char* aCommandName,
                                    nsISupports* refCon, bool* outCmdEnabled) {
  NS_ENSURE_ARG_POINTER(outCmdEnabled);
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
InsertHTMLCommand::IsCommandEnabled(const char* aCommandName,
                                    nsISupports* refCon,
                                    bool* outCmdEnabled)
{
  NS_ENSURE_ARG_POINTER(outCmdEnabled);
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
=======
StaticRefPtr<InsertHTMLCommand> InsertHTMLCommand::sInstance;

bool InsertHTMLCommand::IsCommandEnabled(Command aCommand,
                                         TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
  }
  return htmlEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertHTMLCommand::DoCommand(const char* aCommandName, nsISupports* refCon) {
  // If nsInsertHTMLCommand is called with no parameters, it was probably called
  // with an empty string parameter ''. In this case, it should act the same as
  // the delete command
  NS_ENSURE_ARG_POINTER(refCon);

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
||||||| merged common ancestors
NS_IMETHODIMP
InsertHTMLCommand::DoCommand(const char* aCommandName, nsISupports* refCon)
{
  // If nsInsertHTMLCommand is called with no parameters, it was probably called with
  // an empty string parameter ''. In this case, it should act the same as the delete command
  NS_ENSURE_ARG_POINTER(refCon);

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  mozilla::HTMLEditor* htmlEditor = editor->AsHTMLEditor();
=======
nsresult InsertHTMLCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                      nsIPrincipal* aPrincipal) const {
  // If InsertHTMLCommand is called with no parameters, it was probably called
  // with an empty string parameter ''. In this case, it should act the same as
  // the delete command
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
>>>>>>> upstream-releases
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
  nsresult rv =
      MOZ_KnownLive(htmlEditor)->InsertHTMLAsAction(EmptyString(), aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "InsertHTMLAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertHTMLCommand::DoCommandParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* refCon) {
  NS_ENSURE_ARG_POINTER(aParams);
  NS_ENSURE_ARG_POINTER(refCon);

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
InsertHTMLCommand::DoCommandParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* refCon)
{
  NS_ENSURE_ARG_POINTER(aParams);
  NS_ENSURE_ARG_POINTER(refCon);

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
nsresult InsertHTMLCommand::DoCommandParam(Command aCommand,
                                           const nsAString& aStringParam,
                                           TextEditor& aTextEditor,
                                           nsIPrincipal* aPrincipal) const {
  if (NS_WARN_IF(aStringParam.IsVoid())) {
    return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases
  }

  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }
  nsresult rv =
      MOZ_KnownLive(htmlEditor)->InsertHTMLAsAction(aStringParam, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "InsertHTMLAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertHTMLCommand::GetCommandStateParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon) {
  NS_ENSURE_ARG_POINTER(aParams);
  NS_ENSURE_ARG_POINTER(refCon);

  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
||||||| merged common ancestors
NS_IMETHODIMP
InsertHTMLCommand::GetCommandStateParams(const char *aCommandName,
                                         nsICommandParams *aParams,
                                         nsISupports *refCon)
{
  NS_ENSURE_ARG_POINTER(aParams);
  NS_ENSURE_ARG_POINTER(refCon);

  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, outCmdEnabled);
=======
nsresult InsertHTMLCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

<<<<<<< HEAD
InsertTagCommand::InsertTagCommand(nsAtom* aTagName)
    : HTMLEditorCommandBase(), mTagName(aTagName) {
  MOZ_ASSERT(mTagName);
}
||||||| merged common ancestors
InsertTagCommand::InsertTagCommand(nsAtom* aTagName)
  : HTMLEditorCommandBase()
  , mTagName(aTagName)
{
  MOZ_ASSERT(mTagName);
}
=======
/*****************************************************************************
 * mozilla::InsertTagCommand
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
InsertTagCommand::~InsertTagCommand() {}
||||||| merged common ancestors
InsertTagCommand::~InsertTagCommand()
{
}
=======
StaticRefPtr<InsertTagCommand> InsertTagCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
InsertTagCommand::IsCommandEnabled(const char* aCommandName,
                                   nsISupports* refCon, bool* outCmdEnabled) {
  NS_ENSURE_ARG_POINTER(outCmdEnabled);
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
InsertTagCommand::IsCommandEnabled(const char* aCommandName,
                                   nsISupports* refCon,
                                   bool* outCmdEnabled)
{
  NS_ENSURE_ARG_POINTER(outCmdEnabled);
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
=======
bool InsertTagCommand::IsCommandEnabled(Command aCommand,
                                        TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
  if (!htmlEditor) {
    return false;
  }
  return htmlEditor->IsSelectionEditable();
}

// corresponding STATE_ATTRIBUTE is: src (img) and href (a)
<<<<<<< HEAD
NS_IMETHODIMP
InsertTagCommand::DoCommand(const char* aCmdName, nsISupports* refCon) {
  NS_ENSURE_TRUE(mTagName == nsGkAtoms::hr, NS_ERROR_NOT_IMPLEMENTED);

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
InsertTagCommand::DoCommand(const char* aCmdName, nsISupports* refCon)
{
  NS_ENSURE_TRUE(mTagName == nsGkAtoms::hr, NS_ERROR_NOT_IMPLEMENTED);

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
nsresult InsertTagCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                     nsIPrincipal* aPrincipal) const {
  nsAtom* tagName = GetTagName(aCommand);
  if (NS_WARN_IF(tagName != nsGkAtoms::hr)) {
    return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
  }

  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }

<<<<<<< HEAD
  RefPtr<Element> newElement = htmlEditor->CreateElementWithDefaults(*mTagName);
||||||| merged common ancestors
  RefPtr<Element> newElement =
    htmlEditor->CreateElementWithDefaults(*mTagName);
=======
  RefPtr<Element> newElement =
      MOZ_KnownLive(htmlEditor)
          ->CreateElementWithDefaults(MOZ_KnownLive(*tagName));
>>>>>>> upstream-releases
  if (NS_WARN_IF(!newElement)) {
    return NS_ERROR_FAILURE;
  }
  nsresult rv =
      MOZ_KnownLive(htmlEditor)
          ->InsertElementAtSelectionAsAction(newElement, true, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                       "InsertElementAtSelectionAsAction() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertTagCommand::DoCommandParams(const char* aCommandName,
                                  nsICommandParams* aParams,
                                  nsISupports* refCon) {
  NS_ENSURE_ARG_POINTER(refCon);
||||||| merged common ancestors
NS_IMETHODIMP
InsertTagCommand::DoCommandParams(const char *aCommandName,
                                  nsICommandParams *aParams,
                                  nsISupports *refCon)
{
  NS_ENSURE_ARG_POINTER(refCon);
=======
nsresult InsertTagCommand::DoCommandParam(Command aCommand,
                                          const nsAString& aStringParam,
                                          TextEditor& aTextEditor,
                                          nsIPrincipal* aPrincipal) const {
  MOZ_ASSERT(aCommand != Command::InsertHorizontalRule);
>>>>>>> upstream-releases

  if (NS_WARN_IF(aStringParam.IsEmpty())) {
    return NS_ERROR_INVALID_ARG;
  }
  nsAtom* tagName = GetTagName(aCommand);
  if (NS_WARN_IF(!tagName)) {
    return NS_ERROR_UNEXPECTED;
  }

  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (NS_WARN_IF(!htmlEditor)) {
    return NS_ERROR_FAILURE;
  }

<<<<<<< HEAD
  // do we have an href to use for creating link?
  nsAutoCString asciiAttribute;
  nsresult rv =
      aParams->AsCommandParams()->GetCString(STATE_ATTRIBUTE, asciiAttribute);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  nsAutoString attribute;
  CopyASCIItoUTF16(asciiAttribute, attribute);

  if (attribute.IsEmpty()) {
    return NS_ERROR_INVALID_ARG;
  }

||||||| merged common ancestors
  // do we have an href to use for creating link?
  nsAutoCString asciiAttribute;
  nsresult rv =
    aParams->AsCommandParams()->GetCString(STATE_ATTRIBUTE, asciiAttribute);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  nsAutoString attribute;
  CopyASCIItoUTF16(asciiAttribute, attribute);

  if (attribute.IsEmpty()) {
    return NS_ERROR_INVALID_ARG;
  }

=======
>>>>>>> upstream-releases
  // filter out tags we don't know how to insert
  nsAtom* attribute = nullptr;
  if (tagName == nsGkAtoms::a) {
    attribute = nsGkAtoms::href;
  } else if (tagName == nsGkAtoms::img) {
    attribute = nsGkAtoms::src;
  } else {
    return NS_ERROR_NOT_IMPLEMENTED;
  }

<<<<<<< HEAD
  RefPtr<Element> newElement = htmlEditor->CreateElementWithDefaults(*mTagName);
||||||| merged common ancestors
  RefPtr<Element> newElement =
    htmlEditor->CreateElementWithDefaults(*mTagName);
=======
  RefPtr<Element> newElement =
      MOZ_KnownLive(htmlEditor)
          ->CreateElementWithDefaults(MOZ_KnownLive(*tagName));
>>>>>>> upstream-releases
  if (NS_WARN_IF(!newElement)) {
    return NS_ERROR_FAILURE;
  }

  ErrorResult err;
  newElement->SetAttr(attribute, aStringParam, err);
  if (NS_WARN_IF(err.Failed())) {
    return err.StealNSResult();
  }

  // do actual insertion
  if (tagName == nsGkAtoms::a) {
    nsresult rv =
        MOZ_KnownLive(htmlEditor)
            ->InsertLinkAroundSelectionAsAction(newElement, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                         "InsertLinkAroundSelectionAsAction() failed");
    return rv;
  }
<<<<<<< HEAD
  return NS_OK;
}

NS_IMETHODIMP
InsertTagCommand::GetCommandStateParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* refCon) {
  NS_ENSURE_ARG_POINTER(aParams);
  NS_ENSURE_ARG_POINTER(refCon);
||||||| merged common ancestors
  return NS_OK;
}

NS_IMETHODIMP
InsertTagCommand::GetCommandStateParams(const char *aCommandName,
                                        nsICommandParams *aParams,
                                        nsISupports *refCon)
{
  NS_ENSURE_ARG_POINTER(aParams);
  NS_ENSURE_ARG_POINTER(refCon);
=======
>>>>>>> upstream-releases

  nsresult rv =
      MOZ_KnownLive(htmlEditor)
          ->InsertElementAtSelectionAsAction(newElement, true, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                       "InsertElementAtSelectionAsAction() failed");
  return rv;
}

<<<<<<< HEAD
/****************************/
// HELPER METHODS
/****************************/
||||||| merged common ancestors

/****************************/
//HELPER METHODS
/****************************/
=======
nsresult InsertTagCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
}

/*****************************************************************************
 * Helper methods
 *****************************************************************************/
>>>>>>> upstream-releases

<<<<<<< HEAD
static nsresult GetListState(HTMLEditor* aHTMLEditor, bool* aMixed,
                             nsAString& aLocalName) {
||||||| merged common ancestors
static nsresult
GetListState(HTMLEditor* aHTMLEditor, bool* aMixed, nsAString& aLocalName)
{
=======
static nsresult GetListState(HTMLEditor* aHTMLEditor, bool* aMixed,
                             nsAString& aLocalName)
    MOZ_CAN_RUN_SCRIPT_FOR_DEFINITION {
>>>>>>> upstream-releases
  MOZ_ASSERT(aHTMLEditor);
  MOZ_ASSERT(aMixed);

  *aMixed = false;
  aLocalName.Truncate();

  bool bOL, bUL, bDL;
  nsresult rv = aHTMLEditor->GetListState(aMixed, &bOL, &bUL, &bDL);
  NS_ENSURE_SUCCESS(rv, rv);

  if (*aMixed) {
    return NS_OK;
  }

  if (bOL) {
    aLocalName.AssignLiteral("ol");
  } else if (bUL) {
    aLocalName.AssignLiteral("ul");
  } else if (bDL) {
    aLocalName.AssignLiteral("dl");
  }
  return NS_OK;
}

}  // namespace mozilla
