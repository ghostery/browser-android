/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "EditorCommands.h"

#include "mozilla/ArrayUtils.h"
#include "mozilla/Assertions.h"
#include "mozilla/FlushType.h"
#include "mozilla/Maybe.h"
#include "mozilla/MozPromise.h"  // for mozilla::detail::Any
#include "mozilla/TextEditor.h"
#include "mozilla/dom/Document.h"
#include "mozilla/dom/Selection.h"
#include "nsCommandParams.h"
#include "nsIClipboard.h"
#include "nsIEditingSession.h"
#include "nsIPrincipal.h"
#include "nsISelectionController.h"
#include "nsITransferable.h"
#include "nsString.h"
#include "nsAString.h"

class nsISupports;

#define STATE_ENABLED "state_enabled"
#define STATE_ATTRIBUTE "state_attribute"
#define STATE_DATA "state_data"

namespace mozilla {

<<<<<<< HEAD
/******************************************************************************
 * mozilla::EditorCommandBase
 ******************************************************************************/

EditorCommandBase::EditorCommandBase() {}

NS_IMPL_ISUPPORTS(EditorCommandBase, nsIControllerCommand)
||||||| merged common ancestors
/******************************************************************************
 * mozilla::EditorCommandBase
 ******************************************************************************/

EditorCommandBase::EditorCommandBase()
{
}

NS_IMPL_ISUPPORTS(EditorCommandBase, nsIControllerCommand)
=======
using detail::Any;
>>>>>>> upstream-releases

/******************************************************************************
 * mozilla::EditorCommand
 ******************************************************************************/

NS_IMPL_ISUPPORTS(EditorCommand, nsIControllerCommand)

NS_IMETHODIMP
<<<<<<< HEAD
UndoCommand::IsCommandEnabled(const char* aCommandName,
                              nsISupports* aCommandRefCon, bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
||||||| merged common ancestors
UndoCommand::IsCommandEnabled(const char* aCommandName,
                              nsISupports* aCommandRefCon,
                              bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
=======
EditorCommand::IsCommandEnabled(const char* aCommandName,
                                nsISupports* aCommandRefCon, bool* aIsEnabled) {
  if (NS_WARN_IF(!aCommandName) || NS_WARN_IF(!aIsEnabled)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  TextEditor* textEditor = editor ? editor->AsTextEditor() : nullptr;
  *aIsEnabled = IsCommandEnabled(GetInternalCommand(aCommandName),
                                 MOZ_KnownLive(textEditor));
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
UndoCommand::DoCommand(const char* aCommandName, nsISupports* aCommandRefCon) {
||||||| merged common ancestors
UndoCommand::DoCommand(const char* aCommandName,
                       nsISupports* aCommandRefCon)
{
=======
EditorCommand::DoCommand(const char* aCommandName,
                         nsISupports* aCommandRefCon) {
  if (NS_WARN_IF(!aCommandName) || NS_WARN_IF(!aCommandRefCon)) {
    return NS_ERROR_INVALID_ARG;
  }
>>>>>>> upstream-releases
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_INVALID_ARG;
  }
<<<<<<< HEAD
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  return textEditor->Undo(1);
}

NS_IMETHODIMP
UndoCommand::DoCommandParams(const char* aCommandName,
                             nsICommandParams* aParams,
                             nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
UndoCommand::GetCommandStateParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  return textEditor->Undo(1);
}

NS_IMETHODIMP
UndoCommand::DoCommandParams(const char* aCommandName,
                             nsICommandParams* aParams,
                             nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
UndoCommand::GetCommandStateParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
  nsresult rv = DoCommand(GetInternalCommand(aCommandName),
                          MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
  NS_WARNING_ASSERTION(
      NS_SUCCEEDED(rv),
      "Failed to do command from nsIControllerCommand::DoCommand()");
  return rv;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
RedoCommand::IsCommandEnabled(const char* aCommandName,
                              nsISupports* aCommandRefCon, bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
||||||| merged common ancestors
RedoCommand::IsCommandEnabled(const char* aCommandName,
                              nsISupports* aCommandRefCon,
                              bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
=======
EditorCommand::DoCommandParams(const char* aCommandName,
                               nsICommandParams* aParams,
                               nsISupports* aCommandRefCon) {
  if (NS_WARN_IF(!aCommandName) || NS_WARN_IF(!aCommandRefCon)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCommandParams* params = aParams ? aParams->AsCommandParams() : nullptr;
  Command command = GetInternalCommand(aCommandName, params);
  EditorCommandParamType paramType = EditorCommand::GetParamType(command);
  if (paramType == EditorCommandParamType::None) {
    nsresult rv = DoCommandParam(
        command, MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
    NS_WARNING_ASSERTION(
        NS_SUCCEEDED(rv),
        "Failed to do command from nsIControllerCommand::DoCommandParams()");
    return rv;
  }

<<<<<<< HEAD
NS_IMETHODIMP
RedoCommand::DoCommand(const char* aCommandName, nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
RedoCommand::DoCommand(const char* aCommandName,
                       nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
=======
  if (Any(paramType & EditorCommandParamType::Bool)) {
    if (Any(paramType & EditorCommandParamType::StateAttribute)) {
      Maybe<bool> boolParam = Nothing();
      if (params) {
        ErrorResult error;
        boolParam = Some(params->GetBool(STATE_ATTRIBUTE, error));
        if (NS_WARN_IF(error.Failed())) {
          return error.StealNSResult();
        }
      }
      nsresult rv = DoCommandParam(
          command, boolParam, MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
      NS_WARNING_ASSERTION(
          NS_SUCCEEDED(rv),
          "Failed to do command from nsIControllerCommand::DoCommandParams()");
      return rv;
    }
    MOZ_ASSERT_UNREACHABLE("Unexpected state for bool");
    return NS_ERROR_NOT_IMPLEMENTED;
  }

  // Special case for MultiStateCommandBase.  It allows both CString and String
  // in STATE_ATTRIBUTE and CString is preferred.
  if (Any(paramType & EditorCommandParamType::CString) &&
      Any(paramType & EditorCommandParamType::String)) {
    if (!params) {
      nsresult rv =
          DoCommandParam(command, VoidString(),
                         MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
      NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                           "Failed to do command from "
                           "nsIControllerCommand::DoCommandParams()");
      return rv;
    }
    if (Any(paramType & EditorCommandParamType::StateAttribute)) {
      nsCString cStringParam;
      nsresult rv = params->GetCString(STATE_ATTRIBUTE, cStringParam);
      if (NS_SUCCEEDED(rv)) {
        NS_ConvertUTF8toUTF16 stringParam(cStringParam);
        nsresult rv =
            DoCommandParam(command, stringParam,
                           MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
        NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                             "Failed to do command from "
                             "nsIControllerCommand::DoCommandParams()");
        return rv;
      }
      nsString stringParam;
      DebugOnly<nsresult> rvIgnored =
          params->GetString(STATE_ATTRIBUTE, stringParam);
      NS_WARNING_ASSERTION(NS_SUCCEEDED(rvIgnored),
                           "Failed to get string from STATE_ATTRIBUTE");
      rv = DoCommandParam(command, stringParam,
                          MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
      NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                           "Failed to do command from "
                           "nsIControllerCommand::DoCommandParams()");
      return rv;
    }
    MOZ_ASSERT_UNREACHABLE("Unexpected state for CString/String");
    return NS_ERROR_NOT_IMPLEMENTED;
  }

  if (Any(paramType & EditorCommandParamType::CString)) {
    if (!params) {
      nsresult rv =
          DoCommandParam(command, VoidCString(),
                         MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
      NS_WARNING_ASSERTION(
          NS_SUCCEEDED(rv),
          "Failed to do command from nsIControllerCommand::DoCommandParams()");
      return rv;
    }
    if (Any(paramType & EditorCommandParamType::StateAttribute)) {
      nsCString cStringParam;
      nsresult rv = params->GetCString(STATE_ATTRIBUTE, cStringParam);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
      rv = DoCommandParam(command, cStringParam,
                          MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
      NS_WARNING_ASSERTION(
          NS_SUCCEEDED(rv),
          "Failed to do command from nsIControllerCommand::DoCommandParams()");
      return rv;
    }
    MOZ_ASSERT_UNREACHABLE("Unexpected state for CString");
    return NS_ERROR_NOT_IMPLEMENTED;
  }

  if (Any(paramType & EditorCommandParamType::String)) {
    if (!params) {
      nsresult rv =
          DoCommandParam(command, VoidString(),
                         MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
      NS_WARNING_ASSERTION(
          NS_SUCCEEDED(rv),
          "Failed to do command from nsIControllerCommand::DoCommandParams()");
      return rv;
    }
    nsString stringParam;
    if (Any(paramType & EditorCommandParamType::StateAttribute)) {
      nsresult rv = params->GetString(STATE_ATTRIBUTE, stringParam);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
    } else if (Any(paramType & EditorCommandParamType::StateData)) {
      nsresult rv = params->GetString(STATE_DATA, stringParam);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
    } else {
      MOZ_ASSERT_UNREACHABLE("Unexpected state for String");
      return NS_ERROR_NOT_IMPLEMENTED;
    }
    nsresult rv = DoCommandParam(
        command, stringParam, MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
    NS_WARNING_ASSERTION(
        NS_SUCCEEDED(rv),
        "Failed to do command from nsIControllerCommand::DoCommandParams()");
    return rv;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
NS_IMETHODIMP
RedoCommand::DoCommandParams(const char* aCommandName,
                             nsICommandParams* aParams,
                             nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
}
||||||| merged common ancestors
NS_IMETHODIMP
RedoCommand::DoCommandParams(const char* aCommandName,
                             nsICommandParams* aParams,
                             nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
}
=======
  if (Any(paramType & EditorCommandParamType::Transferable)) {
    nsCOMPtr<nsITransferable> transferable;
    if (params) {
      nsCOMPtr<nsISupports> supports = params->GetISupports("transferable");
      transferable = do_QueryInterface(supports);
    }
    nsresult rv = DoCommandParam(
        command, transferable, MOZ_KnownLive(*editor->AsTextEditor()), nullptr);
    NS_WARNING_ASSERTION(
        NS_SUCCEEDED(rv),
        "Failed to do command from nsIControllerCommand::DoCommandParams()");
    return rv;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
RedoCommand::GetCommandStateParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
RedoCommand::GetCommandStateParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
  MOZ_ASSERT_UNREACHABLE("Unexpected param type");
  return NS_ERROR_NOT_IMPLEMENTED;
>>>>>>> upstream-releases
}

NS_IMETHODIMP
<<<<<<< HEAD
ClearUndoCommand::IsCommandEnabled(const char* aCommandName,
                                   nsISupports* aCommandRefCon,
                                   bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
||||||| merged common ancestors
ClearUndoCommand::IsCommandEnabled(const char* aCommandName,
                                   nsISupports* aCommandRefCon,
                                   bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
=======
EditorCommand::GetCommandStateParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* aCommandRefCon) {
  if (NS_WARN_IF(!aCommandName) || NS_WARN_IF(!aParams)) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (editor) {
    return GetCommandStateParams(GetInternalCommand(aCommandName),
                                 MOZ_KnownLive(*aParams->AsCommandParams()),
                                 MOZ_KnownLive(editor->AsTextEditor()),
                                 nullptr);
  }
  nsCOMPtr<nsIEditingSession> editingSession =
      do_QueryInterface(aCommandRefCon);
  if (editingSession) {
    return GetCommandStateParams(GetInternalCommand(aCommandName),
                                 MOZ_KnownLive(*aParams->AsCommandParams()),
                                 nullptr, editingSession);
  }
  return GetCommandStateParams(GetInternalCommand(aCommandName),
                               MOZ_KnownLive(*aParams->AsCommandParams()),
                               nullptr, nullptr);
}

<<<<<<< HEAD
NS_IMETHODIMP
ClearUndoCommand::DoCommand(const char* aCommandName,
                            nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
ClearUndoCommand::DoCommand(const char* aCommandName,
                            nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
=======
/******************************************************************************
 * mozilla::UndoCommand
 ******************************************************************************/

StaticRefPtr<UndoCommand> UndoCommand::sInstance;

bool UndoCommand::IsCommandEnabled(Command aCommand,
                                   TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  // XXX Should we return NS_ERROR_FAILURE if ClearUndoRedo() returns false?
  DebugOnly<bool> clearedUndoRedo = textEditor->ClearUndoRedo();
  NS_WARNING_ASSERTION(clearedUndoRedo,
                       "Failed to clear undo/redo transactions");
  return NS_OK;
||||||| merged common ancestors
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  // XXX Should we return NS_ERROR_FAILURE if ClearUndoRedo() returns false?
  DebugOnly<bool> clearedUndoRedo = textEditor->ClearUndoRedo();
  NS_WARNING_ASSERTION(clearedUndoRedo,
    "Failed to clear undo/redo transactions");
  return NS_OK;
=======
  return aTextEditor->IsSelectionEditable() && aTextEditor->CanUndo();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
ClearUndoCommand::DoCommandParams(const char* aCommandName,
                                  nsICommandParams* aParams,
                                  nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
||||||| merged common ancestors
NS_IMETHODIMP
ClearUndoCommand::DoCommandParams(const char* aCommandName,
                                  nsICommandParams* aParams,
                                  nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
=======
nsresult UndoCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                nsIPrincipal* aPrincipal) const {
  nsresult rv = aTextEditor.UndoAsAction(1, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "UndoAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
ClearUndoCommand::GetCommandStateParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon) {
  NS_ENSURE_ARG_POINTER(aParams);

  bool enabled;
  nsresult rv = IsCommandEnabled(aCommandName, aCommandRefCon, &enabled);
  NS_ENSURE_SUCCESS(rv, rv);

  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, enabled);
||||||| merged common ancestors
NS_IMETHODIMP
ClearUndoCommand::GetCommandStateParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon)
{
  NS_ENSURE_ARG_POINTER(aParams);

  bool enabled;
  nsresult rv = IsCommandEnabled(aCommandName, aCommandRefCon, &enabled);
  NS_ENSURE_SUCCESS(rv, rv);

  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, enabled);
=======
nsresult UndoCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::RedoCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
CutCommand::IsCommandEnabled(const char* aCommandName,
                             nsISupports* aCommandRefCon, bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  *aIsEnabled = false;
||||||| merged common ancestors
NS_IMETHODIMP
CutCommand::IsCommandEnabled(const char* aCommandName,
                             nsISupports* aCommandRefCon,
                             bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  *aIsEnabled = false;
=======
StaticRefPtr<RedoCommand> RedoCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  if (!textEditor->IsSelectionEditable()) {
    return NS_OK;
  }
  return editor->CanCut(aIsEnabled);
}

NS_IMETHODIMP
CutCommand::DoCommand(const char* aCommandName, nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  if (!textEditor->IsSelectionEditable()) {
    return NS_OK;
  }
  return editor->CanCut(aIsEnabled);
}

NS_IMETHODIMP
CutCommand::DoCommand(const char* aCommandName,
                      nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
=======
bool RedoCommand::IsCommandEnabled(Command aCommand,
                                   TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  return aTextEditor->IsSelectionEditable() && aTextEditor->CanRedo();
}

<<<<<<< HEAD
NS_IMETHODIMP
CutCommand::DoCommandParams(const char* aCommandName, nsICommandParams* aParams,
                            nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
||||||| merged common ancestors
NS_IMETHODIMP
CutCommand::DoCommandParams(const char* aCommandName,
                            nsICommandParams* aParams,
                            nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
=======
nsresult RedoCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                nsIPrincipal* aPrincipal) const {
  nsresult rv = aTextEditor.RedoAsAction(1, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "RedoAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
CutCommand::GetCommandStateParams(const char* aCommandName,
                                  nsICommandParams* aParams,
                                  nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
CutCommand::GetCommandStateParams(const char* aCommandName,
                                  nsICommandParams* aParams,
                                  nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
nsresult RedoCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::CutCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
CutOrDeleteCommand::IsCommandEnabled(const char* aCommandName,
                                     nsISupports* aCommandRefCon,
                                     bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *aIsEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}
||||||| merged common ancestors
NS_IMETHODIMP
CutOrDeleteCommand::IsCommandEnabled(const char* aCommandName,
                                     nsISupports* aCommandRefCon,
                                     bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *aIsEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}
=======
StaticRefPtr<CutCommand> CutCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
CutOrDeleteCommand::DoCommand(const char* aCommandName,
                              nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
CutOrDeleteCommand::DoCommand(const char* aCommandName,
                              nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
=======
bool CutCommand::IsCommandEnabled(Command aCommand,
                                  TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  dom::Selection* selection = textEditor->GetSelection();
  if (selection && selection->IsCollapsed()) {
    nsresult rv = textEditor->DeleteSelectionAsAction(nsIEditor::eNext,
                                                      nsIEditor::eStrip);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }
  return textEditor->Cut();
||||||| merged common ancestors
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  dom::Selection* selection = textEditor->GetSelection();
  if (selection && selection->IsCollapsed()) {
    nsresult rv =
      textEditor->DeleteSelectionAsAction(nsIEditor::eNext,
                                          nsIEditor::eStrip);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }
  return textEditor->Cut();
=======
  return aTextEditor->IsSelectionEditable() && aTextEditor->CanCut();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
CutOrDeleteCommand::DoCommandParams(const char* aCommandName,
                                    nsICommandParams* aParams,
                                    nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
||||||| merged common ancestors
NS_IMETHODIMP
CutOrDeleteCommand::DoCommandParams(const char* aCommandName,
                                    nsICommandParams* aParams,
                                    nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
=======
nsresult CutCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                               nsIPrincipal* aPrincipal) const {
  nsresult rv = aTextEditor.CutAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "CutAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
CutOrDeleteCommand::GetCommandStateParams(const char* aCommandName,
                                          nsICommandParams* aParams,
                                          nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
CutOrDeleteCommand::GetCommandStateParams(const char* aCommandName,
                                          nsICommandParams* aParams,
                                          nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
nsresult CutCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::CutOrDeleteCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
CopyCommand::IsCommandEnabled(const char* aCommandName,
                              nsISupports* aCommandRefCon, bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  return textEditor->CanCopy(aIsEnabled);
}
||||||| merged common ancestors
NS_IMETHODIMP
CopyCommand::IsCommandEnabled(const char* aCommandName,
                              nsISupports* aCommandRefCon,
                              bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  return textEditor->CanCopy(aIsEnabled);
}
=======
StaticRefPtr<CutOrDeleteCommand> CutOrDeleteCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
CopyCommand::DoCommand(const char* aCommandName, nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
CopyCommand::DoCommand(const char* aCommandName,
                       nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
=======
bool CutOrDeleteCommand::IsCommandEnabled(Command aCommand,
                                          TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  return aTextEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
CopyCommand::DoCommandParams(const char* aCommandName,
                             nsICommandParams* aParams,
                             nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
||||||| merged common ancestors
NS_IMETHODIMP
CopyCommand::DoCommandParams(const char* aCommandName,
                             nsICommandParams* aParams,
                             nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
=======
nsresult CutOrDeleteCommand::DoCommand(Command aCommand,
                                       TextEditor& aTextEditor,
                                       nsIPrincipal* aPrincipal) const {
  dom::Selection* selection = aTextEditor.GetSelection();
  if (selection && selection->IsCollapsed()) {
    nsresult rv = aTextEditor.DeleteSelectionAsAction(
        nsIEditor::eNext, nsIEditor::eStrip, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "DeleteSelectionsAction() failed");
    return rv;
  }
  nsresult rv = aTextEditor.CutAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "CutAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
CopyCommand::GetCommandStateParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
CopyCommand::GetCommandStateParams(const char* aCommandName,
                                   nsICommandParams* aParams,
                                   nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
nsresult CutOrDeleteCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::CopyCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
CopyOrDeleteCommand::IsCommandEnabled(const char* aCommandName,
                                      nsISupports* aCommandRefCon,
                                      bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
||||||| merged common ancestors
NS_IMETHODIMP
CopyOrDeleteCommand::IsCommandEnabled(const char* aCommandName,
                                      nsISupports* aCommandRefCon,
                                      bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
=======
StaticRefPtr<CopyCommand> CopyCommand::sInstance;
>>>>>>> upstream-releases

bool CopyCommand::IsCommandEnabled(Command aCommand,
                                   TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }
  return aTextEditor->CanCopy();
}

<<<<<<< HEAD
NS_IMETHODIMP
CopyOrDeleteCommand::DoCommand(const char* aCommandName,
                               nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  dom::Selection* selection = textEditor->GetSelection();
  if (selection && selection->IsCollapsed()) {
    nsresult rv = textEditor->DeleteSelectionAsAction(nsIEditor::eNextWord,
                                                      nsIEditor::eStrip);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }
  return textEditor->Copy();
||||||| merged common ancestors
NS_IMETHODIMP
CopyOrDeleteCommand::DoCommand(const char* aCommandName,
                               nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  dom::Selection* selection = textEditor->GetSelection();
  if (selection && selection->IsCollapsed()) {
    nsresult rv =
      textEditor->DeleteSelectionAsAction(nsIEditor::eNextWord,
                                          nsIEditor::eStrip);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }
  return textEditor->Copy();
=======
nsresult CopyCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                nsIPrincipal* aPrincipal) const {
  // Shouldn't cause "beforeinput" event so that we don't need to specify
  // the given principal.
  return aTextEditor.Copy();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
CopyOrDeleteCommand::DoCommandParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
CopyOrDeleteCommand::GetCommandStateParams(const char* aCommandName,
                                           nsICommandParams* aParams,
                                           nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
CopyOrDeleteCommand::DoCommandParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
CopyOrDeleteCommand::GetCommandStateParams(const char* aCommandName,
                                           nsICommandParams* aParams,
                                           nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
nsresult CopyCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::CopyOrDeleteCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
CopyAndCollapseToEndCommand::IsCommandEnabled(const char* aCommandName,
                                              nsISupports* aCommandRefCon,
                                              bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
CopyAndCollapseToEndCommand::IsCommandEnabled(const char* aCommandName,
                                              nsISupports* aCommandRefCon,
                                              bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
=======
StaticRefPtr<CopyOrDeleteCommand> CopyOrDeleteCommand::sInstance;

bool CopyOrDeleteCommand::IsCommandEnabled(Command aCommand,
                                           TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  return aTextEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
CopyAndCollapseToEndCommand::DoCommand(const char* aCommandName,
                                       nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsresult rv = textEditor->Copy();
  if (NS_FAILED(rv)) {
||||||| merged common ancestors
NS_IMETHODIMP
CopyAndCollapseToEndCommand::DoCommand(const char* aCommandName,
                                       nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsresult rv = textEditor->Copy();
  if (NS_FAILED(rv)) {
=======
nsresult CopyOrDeleteCommand::DoCommand(Command aCommand,
                                        TextEditor& aTextEditor,
                                        nsIPrincipal* aPrincipal) const {
  dom::Selection* selection = aTextEditor.GetSelection();
  if (selection && selection->IsCollapsed()) {
    nsresult rv = aTextEditor.DeleteSelectionAsAction(
        nsIEditor::eNextWord, nsIEditor::eStrip, aPrincipal);
    NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "DeleteSelectionAsAction() failed");
>>>>>>> upstream-releases
    return rv;
  }
  // Shouldn't cause "beforeinput" event so that we don't need to specify
  // the given principal.
  nsresult rv = aTextEditor.Copy();
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "Copy() failed");
  return rv;
}

<<<<<<< HEAD
NS_IMETHODIMP
CopyAndCollapseToEndCommand::DoCommandParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
CopyAndCollapseToEndCommand::GetCommandStateParams(
    const char* aCommandName, nsICommandParams* aParams,
    nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
CopyAndCollapseToEndCommand::DoCommandParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
CopyAndCollapseToEndCommand::GetCommandStateParams(const char* aCommandName,
                                                   nsICommandParams* aParams,
                                                   nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
nsresult CopyOrDeleteCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::PasteCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
PasteCommand::IsCommandEnabled(const char* aCommandName,
                               nsISupports* aCommandRefCon, bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  *aIsEnabled = false;

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  if (!textEditor->IsSelectionEditable()) {
    return NS_OK;
  }
  return textEditor->CanPaste(nsIClipboard::kGlobalClipboard, aIsEnabled);
}
||||||| merged common ancestors
NS_IMETHODIMP
PasteCommand::IsCommandEnabled(const char* aCommandName,
                               nsISupports* aCommandRefCon,
                               bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  *aIsEnabled = false;

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  if (!textEditor->IsSelectionEditable()) {
    return NS_OK;
  }
  return textEditor->CanPaste(nsIClipboard::kGlobalClipboard, aIsEnabled);
}
=======
StaticRefPtr<PasteCommand> PasteCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
PasteCommand::DoCommand(const char* aCommandName, nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
PasteCommand::DoCommand(const char* aCommandName,
                        nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
bool PasteCommand::IsCommandEnabled(Command aCommand,
                                    TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  return aTextEditor->IsSelectionEditable() &&
         aTextEditor->CanPaste(nsIClipboard::kGlobalClipboard);
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteCommand::DoCommandParams(const char* aCommandName,
                              nsICommandParams* aParams,
                              nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
||||||| merged common ancestors
NS_IMETHODIMP
PasteCommand::DoCommandParams(const char* aCommandName,
                              nsICommandParams* aParams,
                              nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
=======
nsresult PasteCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                 nsIPrincipal* aPrincipal) const {
  nsresult rv = aTextEditor.PasteAsAction(nsIClipboard::kGlobalClipboard, true,
                                          aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "PasteAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteCommand::GetCommandStateParams(const char* aCommandName,
                                    nsICommandParams* aParams,
                                    nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
PasteCommand::GetCommandStateParams(const char* aCommandName,
                                    nsICommandParams* aParams,
                                    nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
nsresult PasteCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::PasteTransferableCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
PasteTransferableCommand::IsCommandEnabled(const char* aCommandName,
                                           nsISupports* aCommandRefCon,
                                           bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
||||||| merged common ancestors
NS_IMETHODIMP
PasteTransferableCommand::IsCommandEnabled(const char* aCommandName,
                                           nsISupports* aCommandRefCon,
                                           bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
=======
StaticRefPtr<PasteTransferableCommand> PasteTransferableCommand::sInstance;
>>>>>>> upstream-releases

bool PasteTransferableCommand::IsCommandEnabled(Command aCommand,
                                                TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }
  return aTextEditor->IsSelectionEditable() &&
         aTextEditor->CanPasteTransferable(nullptr);
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteTransferableCommand::DoCommand(const char* aCommandName,
                                    nsISupports* aCommandRefCon) {
||||||| merged common ancestors
NS_IMETHODIMP
PasteTransferableCommand::DoCommand(const char* aCommandName,
                                    nsISupports* aCommandRefCon)
{
=======
nsresult PasteTransferableCommand::DoCommand(Command aCommand,
                                             TextEditor& aTextEditor,
                                             nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  return NS_ERROR_FAILURE;
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteTransferableCommand::DoCommandParams(const char* aCommandName,
                                          nsICommandParams* aParams,
                                          nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsISupports> supports =
      aParams->AsCommandParams()->GetISupports("transferable");
  if (NS_WARN_IF(!supports)) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsITransferable> trans = do_QueryInterface(supports);
  if (NS_WARN_IF(!trans)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
PasteTransferableCommand::DoCommandParams(const char* aCommandName,
                                          nsICommandParams* aParams,
                                          nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsISupports> supports =
    aParams->AsCommandParams()->GetISupports("transferable");
  if (NS_WARN_IF(!supports)) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsITransferable> trans = do_QueryInterface(supports);
  if (NS_WARN_IF(!trans)) {
    return NS_ERROR_FAILURE;
=======
nsresult PasteTransferableCommand::DoCommandParam(
    Command aCommand, nsITransferable* aTransferableParam,
    TextEditor& aTextEditor, nsIPrincipal* aPrincipal) const {
  if (NS_WARN_IF(!aTransferableParam)) {
    return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsresult rv = textEditor->PasteTransferable(trans);
  if (rv == NS_ERROR_EDITOR_DESTROYED) {
    // Return NS_OK when editor is destroyed since it's expected by the
    // web app.
    return NS_OK;
  }
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
||||||| merged common ancestors

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  return textEditor->PasteTransferable(trans);
=======
  nsresult rv =
      aTextEditor.PasteTransferableAsAction(aTransferableParam, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "PasteTransferableAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteTransferableCommand::GetCommandStateParams(const char* aCommandName,
                                                nsICommandParams* aParams,
                                                nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
PasteTransferableCommand::GetCommandStateParams(const char* aCommandName,
                                                nsICommandParams* aParams,
                                                nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
nsresult PasteTransferableCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  if (NS_WARN_IF(!aTextEditor)) {
    return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  nsCOMPtr<nsISupports> supports =
      aParams->AsCommandParams()->GetISupports("transferable");
||||||| merged common ancestors
  nsCOMPtr<nsISupports> supports =
    aParams->AsCommandParams()->GetISupports("transferable");
=======
  nsCOMPtr<nsISupports> supports = aParams.GetISupports("transferable");
>>>>>>> upstream-releases
  if (NS_WARN_IF(!supports)) {
    return NS_ERROR_FAILURE;
  }

  nsCOMPtr<nsITransferable> trans;
  trans = do_QueryInterface(supports);
  if (NS_WARN_IF(!trans)) {
    return NS_ERROR_FAILURE;
  }

<<<<<<< HEAD
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);

  return aParams->AsCommandParams()->SetBool(
      STATE_ENABLED, textEditor->CanPasteTransferable(trans));
||||||| merged common ancestors
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);

  return aParams->AsCommandParams()->SetBool(
           STATE_ENABLED, textEditor->CanPasteTransferable(trans));
=======
  return aParams.SetBool(STATE_ENABLED,
                         aTextEditor->CanPasteTransferable(trans));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::SwitchTextDirectionCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
SwitchTextDirectionCommand::IsCommandEnabled(const char* aCommandName,
                                             nsISupports* aCommandRefCon,
                                             bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *aIsEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}
||||||| merged common ancestors
NS_IMETHODIMP
SwitchTextDirectionCommand::IsCommandEnabled(const char* aCommandName,
                                             nsISupports* aCommandRefCon,
                                             bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *aIsEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}
=======
StaticRefPtr<SwitchTextDirectionCommand> SwitchTextDirectionCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
SwitchTextDirectionCommand::DoCommand(const char* aCommandName,
                                      nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
SwitchTextDirectionCommand::DoCommand(const char* aCommandName,
                                      nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
bool SwitchTextDirectionCommand::IsCommandEnabled(
    Command aCommand, TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  return aTextEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
SwitchTextDirectionCommand::DoCommandParams(const char* aCommandName,
                                            nsICommandParams* aParams,
                                            nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
||||||| merged common ancestors
NS_IMETHODIMP
SwitchTextDirectionCommand::DoCommandParams(const char* aCommandName,
                                            nsICommandParams* aParams,
                                            nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
=======
nsresult SwitchTextDirectionCommand::DoCommand(Command aCommand,
                                               TextEditor& aTextEditor,
                                               nsIPrincipal* aPrincipal) const {
  nsresult rv = aTextEditor.ToggleTextDirectionAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                       "ToggleTextDirectionAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
SwitchTextDirectionCommand::GetCommandStateParams(const char* aCommandName,
                                                  nsICommandParams* aParams,
                                                  nsISupports* aCommandRefCon) {
  bool canSwitchTextDirection = true;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canSwitchTextDirection);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED,
                                             canSwitchTextDirection);
||||||| merged common ancestors
NS_IMETHODIMP
SwitchTextDirectionCommand::GetCommandStateParams(const char* aCommandName,
                                                  nsICommandParams* aParams,
                                                  nsISupports* aCommandRefCon)
{
  bool canSwitchTextDirection = true;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canSwitchTextDirection);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED,
                                             canSwitchTextDirection);
=======
nsresult SwitchTextDirectionCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::DeleteCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
DeleteCommand::IsCommandEnabled(const char* aCommandName,
                                nsISupports* aCommandRefCon, bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  *aIsEnabled = false;
||||||| merged common ancestors
NS_IMETHODIMP
DeleteCommand::IsCommandEnabled(const char* aCommandName,
                                nsISupports* aCommandRefCon,
                                bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  *aIsEnabled = false;
=======
StaticRefPtr<DeleteCommand> DeleteCommand::sInstance;
>>>>>>> upstream-releases

bool DeleteCommand::IsCommandEnabled(Command aCommand,
                                     TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
  }
  // We can generally delete whenever the selection is editable.  However,
  // cmd_delete doesn't make sense if the selection is collapsed because it's
  // directionless, which is the same condition under which we can't cut.
  bool isEnabled = aTextEditor->IsSelectionEditable();

  if (aCommand == Command::Delete && isEnabled) {
    return aTextEditor->CanDelete();
  }
  return isEnabled;
}

<<<<<<< HEAD
NS_IMETHODIMP
DeleteCommand::DoCommand(const char* aCommandName,
                         nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }

||||||| merged common ancestors
NS_IMETHODIMP
DeleteCommand::DoCommand(const char* aCommandName,
                         nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }

=======
nsresult DeleteCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                  nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  nsIEditor::EDirection deleteDir = nsIEditor::eNone;
<<<<<<< HEAD

  if (!nsCRT::strcmp("cmd_delete", aCommandName)) {
    // Really this should probably be eNone, but it only makes a difference if
    // the selection is collapsed, and then this command is disabled.  So let's
    // keep it as it always was to avoid breaking things.
    deleteDir = nsIEditor::ePrevious;
  } else if (!nsCRT::strcmp("cmd_deleteCharForward", aCommandName)) {
    deleteDir = nsIEditor::eNext;
  } else if (!nsCRT::strcmp("cmd_deleteCharBackward", aCommandName)) {
    deleteDir = nsIEditor::ePrevious;
  } else if (!nsCRT::strcmp("cmd_deleteWordBackward", aCommandName)) {
    deleteDir = nsIEditor::ePreviousWord;
  } else if (!nsCRT::strcmp("cmd_deleteWordForward", aCommandName)) {
    deleteDir = nsIEditor::eNextWord;
  } else if (!nsCRT::strcmp("cmd_deleteToBeginningOfLine", aCommandName)) {
    deleteDir = nsIEditor::eToBeginningOfLine;
  } else if (!nsCRT::strcmp("cmd_deleteToEndOfLine", aCommandName)) {
    deleteDir = nsIEditor::eToEndOfLine;
  } else {
    MOZ_CRASH("Unrecognized nsDeleteCommand");
  }

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsresult rv =
      textEditor->DeleteSelectionAsAction(deleteDir, nsIEditor::eStrip);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
}

NS_IMETHODIMP
DeleteCommand::DoCommandParams(const char* aCommandName,
                               nsICommandParams* aParams,
                               nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
DeleteCommand::GetCommandStateParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors

  if (!nsCRT::strcmp("cmd_delete", aCommandName)) {
    // Really this should probably be eNone, but it only makes a difference if
    // the selection is collapsed, and then this command is disabled.  So let's
    // keep it as it always was to avoid breaking things.
    deleteDir = nsIEditor::ePrevious;
  } else if (!nsCRT::strcmp("cmd_deleteCharForward", aCommandName)) {
    deleteDir = nsIEditor::eNext;
  } else if (!nsCRT::strcmp("cmd_deleteCharBackward", aCommandName)) {
    deleteDir = nsIEditor::ePrevious;
  } else if (!nsCRT::strcmp("cmd_deleteWordBackward", aCommandName)) {
    deleteDir = nsIEditor::ePreviousWord;
  } else if (!nsCRT::strcmp("cmd_deleteWordForward", aCommandName)) {
    deleteDir = nsIEditor::eNextWord;
  } else if (!nsCRT::strcmp("cmd_deleteToBeginningOfLine", aCommandName)) {
    deleteDir = nsIEditor::eToBeginningOfLine;
  } else if (!nsCRT::strcmp("cmd_deleteToEndOfLine", aCommandName)) {
    deleteDir = nsIEditor::eToEndOfLine;
  } else {
    MOZ_CRASH("Unrecognized nsDeleteCommand");
  }

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsresult rv =
    textEditor->DeleteSelectionAsAction(deleteDir, nsIEditor::eStrip);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
}

NS_IMETHODIMP
DeleteCommand::DoCommandParams(const char* aCommandName,
                               nsICommandParams* aParams,
                               nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
DeleteCommand::GetCommandStateParams(const char* aCommandName,
                                     nsICommandParams* aParams,
                                     nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
  switch (aCommand) {
    case Command::Delete:
      // Really this should probably be eNone, but it only makes a difference
      // if the selection is collapsed, and then this command is disabled.  So
      // let's keep it as it always was to avoid breaking things.
      deleteDir = nsIEditor::ePrevious;
      break;
    case Command::DeleteCharForward:
      deleteDir = nsIEditor::eNext;
      break;
    case Command::DeleteCharBackward:
      deleteDir = nsIEditor::ePrevious;
      break;
    case Command::DeleteWordBackward:
      deleteDir = nsIEditor::ePreviousWord;
      break;
    case Command::DeleteWordForward:
      deleteDir = nsIEditor::eNextWord;
      break;
    case Command::DeleteToBeginningOfLine:
      deleteDir = nsIEditor::eToBeginningOfLine;
      break;
    case Command::DeleteToEndOfLine:
      deleteDir = nsIEditor::eToEndOfLine;
      break;
    default:
      MOZ_CRASH("Unrecognized nsDeleteCommand");
  }
  nsresult rv = aTextEditor.DeleteSelectionAsAction(
      deleteDir, nsIEditor::eStrip, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "DeleteSelectionAsAction() failed");
  return rv;
}

nsresult DeleteCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::SelectAllCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
SelectAllCommand::IsCommandEnabled(const char* aCommandName,
                                   nsISupports* aCommandRefCon,
                                   bool* aIsEnabled) {
  NS_ENSURE_ARG_POINTER(aIsEnabled);
||||||| merged common ancestors
NS_IMETHODIMP
SelectAllCommand::IsCommandEnabled(const char* aCommandName,
                                   nsISupports* aCommandRefCon,
                                   bool* aIsEnabled)
{
  NS_ENSURE_ARG_POINTER(aIsEnabled);
=======
StaticRefPtr<SelectAllCommand> SelectAllCommand::sInstance;
>>>>>>> upstream-releases

bool SelectAllCommand::IsCommandEnabled(Command aCommand,
                                        TextEditor* aTextEditor) const {
  // You can always select all, unless the selection is editable,
  // and the editable region is empty!
  if (!aTextEditor) {
    return true;
  }

  // You can select all if there is an editor which is non-empty
  bool isEmpty = false;
<<<<<<< HEAD
  rv = textEditor->IsEmpty(&isEmpty);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  *aIsEnabled = !isEmpty;
  return NS_OK;
}

NS_IMETHODIMP
SelectAllCommand::DoCommand(const char* aCommandName,
                            nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
  rv = textEditor->IsEmpty(&isEmpty);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  *aIsEnabled = !isEmpty;
  return NS_OK;
}

NS_IMETHODIMP
SelectAllCommand::DoCommand(const char* aCommandName,
                            nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
=======
  if (NS_WARN_IF(NS_FAILED(aTextEditor->IsEmpty(&isEmpty)))) {
    return false;
>>>>>>> upstream-releases
  }
  return !isEmpty;
}

<<<<<<< HEAD
NS_IMETHODIMP
SelectAllCommand::DoCommandParams(const char* aCommandName,
                                  nsICommandParams* aParams,
                                  nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
||||||| merged common ancestors
NS_IMETHODIMP
SelectAllCommand::DoCommandParams(const char* aCommandName,
                                  nsICommandParams* aParams,
                                  nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
=======
nsresult SelectAllCommand::DoCommand(Command aCommand, TextEditor& aTextEditor,
                                     nsIPrincipal* aPrincipal) const {
  // Shouldn't cause "beforeinput" event so that we don't need to specify
  // aPrincipal.
  return aTextEditor.SelectAll();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
SelectAllCommand::GetCommandStateParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
SelectAllCommand::GetCommandStateParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
nsresult SelectAllCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::SelectionMoveCommands
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
SelectionMoveCommands::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* aCommandRefCon,
                                        bool* aIsEnabled) {
  NS_ENSURE_ARG_POINTER(aIsEnabled);
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
SelectionMoveCommands::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* aCommandRefCon,
                                        bool* aIsEnabled)
{
  NS_ENSURE_ARG_POINTER(aIsEnabled);
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    *aIsEnabled = false;
    return NS_OK;
=======
StaticRefPtr<SelectionMoveCommands> SelectionMoveCommands::sInstance;

bool SelectionMoveCommands::IsCommandEnabled(Command aCommand,
                                             TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  return aTextEditor->IsSelectionEditable();
}

static const struct ScrollCommand {
<<<<<<< HEAD
  const char* reverseScroll;
  const char* forwardScroll;
||||||| merged common ancestors
  const char *reverseScroll;
  const char *forwardScroll;
=======
  Command mReverseScroll;
  Command mForwardScroll;
>>>>>>> upstream-releases
  nsresult (NS_STDCALL nsISelectionController::*scroll)(bool);
<<<<<<< HEAD
} scrollCommands[] = {{"cmd_scrollTop", "cmd_scrollBottom",
                       &nsISelectionController::CompleteScroll},
                      {"cmd_scrollPageUp", "cmd_scrollPageDown",
                       &nsISelectionController::ScrollPage},
                      {"cmd_scrollLineUp", "cmd_scrollLineDown",
                       &nsISelectionController::ScrollLine}};
||||||| merged common ancestors
} scrollCommands[] = {
  { "cmd_scrollTop", "cmd_scrollBottom",
    &nsISelectionController::CompleteScroll },
  { "cmd_scrollPageUp", "cmd_scrollPageDown",
    &nsISelectionController::ScrollPage },
  { "cmd_scrollLineUp", "cmd_scrollLineDown",
    &nsISelectionController::ScrollLine }
};
=======
} scrollCommands[] = {{Command::ScrollTop, Command::ScrollBottom,
                       &nsISelectionController::CompleteScroll},
                      {Command::ScrollPageUp, Command::ScrollPageDown,
                       &nsISelectionController::ScrollPage},
                      {Command::ScrollLineUp, Command::ScrollLineDown,
                       &nsISelectionController::ScrollLine}};
>>>>>>> upstream-releases

static const struct MoveCommand {
<<<<<<< HEAD
  const char* reverseMove;
  const char* forwardMove;
  const char* reverseSelect;
  const char* forwardSelect;
||||||| merged common ancestors
  const char *reverseMove;
  const char *forwardMove;
  const char *reverseSelect;
  const char *forwardSelect;
=======
  Command mReverseMove;
  Command mForwardMove;
  Command mReverseSelect;
  Command mForwardSelect;
>>>>>>> upstream-releases
  nsresult (NS_STDCALL nsISelectionController::*move)(bool, bool);
} moveCommands[] = {
<<<<<<< HEAD
    {"cmd_charPrevious", "cmd_charNext", "cmd_selectCharPrevious",
     "cmd_selectCharNext", &nsISelectionController::CharacterMove},
    {"cmd_linePrevious", "cmd_lineNext", "cmd_selectLinePrevious",
     "cmd_selectLineNext", &nsISelectionController::LineMove},
    {"cmd_wordPrevious", "cmd_wordNext", "cmd_selectWordPrevious",
     "cmd_selectWordNext", &nsISelectionController::WordMove},
    {"cmd_beginLine", "cmd_endLine", "cmd_selectBeginLine", "cmd_selectEndLine",
     &nsISelectionController::IntraLineMove},
    {"cmd_movePageUp", "cmd_movePageDown", "cmd_selectPageUp",
     "cmd_selectPageDown", &nsISelectionController::PageMove},
    {"cmd_moveTop", "cmd_moveBottom", "cmd_selectTop", "cmd_selectBottom",
     &nsISelectionController::CompleteMove}};
||||||| merged common ancestors
  { "cmd_charPrevious", "cmd_charNext",
    "cmd_selectCharPrevious", "cmd_selectCharNext",
    &nsISelectionController::CharacterMove },
  { "cmd_linePrevious", "cmd_lineNext",
    "cmd_selectLinePrevious", "cmd_selectLineNext",
    &nsISelectionController::LineMove },
  { "cmd_wordPrevious", "cmd_wordNext",
    "cmd_selectWordPrevious", "cmd_selectWordNext",
    &nsISelectionController::WordMove },
  { "cmd_beginLine", "cmd_endLine",
    "cmd_selectBeginLine", "cmd_selectEndLine",
    &nsISelectionController::IntraLineMove },
  { "cmd_movePageUp", "cmd_movePageDown",
    "cmd_selectPageUp", "cmd_selectPageDown",
    &nsISelectionController::PageMove },
  { "cmd_moveTop", "cmd_moveBottom",
    "cmd_selectTop", "cmd_selectBottom",
    &nsISelectionController::CompleteMove }
};
=======
    {Command::CharPrevious, Command::CharNext, Command::SelectCharPrevious,
     Command::SelectCharNext, &nsISelectionController::CharacterMove},
    {Command::LinePrevious, Command::LineNext, Command::SelectLinePrevious,
     Command::SelectLineNext, &nsISelectionController::LineMove},
    {Command::WordPrevious, Command::WordNext, Command::SelectWordPrevious,
     Command::SelectWordNext, &nsISelectionController::WordMove},
    {Command::BeginLine, Command::EndLine, Command::SelectBeginLine,
     Command::SelectEndLine, &nsISelectionController::IntraLineMove},
    {Command::MovePageUp, Command::MovePageDown, Command::SelectPageUp,
     Command::SelectPageDown, &nsISelectionController::PageMove},
    {Command::MoveTop, Command::MoveBottom, Command::SelectTop,
     Command::SelectBottom, &nsISelectionController::CompleteMove}};
>>>>>>> upstream-releases

static const struct PhysicalCommand {
<<<<<<< HEAD
  const char* move;
  const char* select;
||||||| merged common ancestors
  const char *move;
  const char *select;
=======
  Command mMove;
  Command mSelect;
>>>>>>> upstream-releases
  int16_t direction;
  int16_t amount;
} physicalCommands[] = {
<<<<<<< HEAD
    {"cmd_moveLeft", "cmd_selectLeft", nsISelectionController::MOVE_LEFT, 0},
    {"cmd_moveRight", "cmd_selectRight", nsISelectionController::MOVE_RIGHT, 0},
    {"cmd_moveUp", "cmd_selectUp", nsISelectionController::MOVE_UP, 0},
    {"cmd_moveDown", "cmd_selectDown", nsISelectionController::MOVE_DOWN, 0},
    {"cmd_moveLeft2", "cmd_selectLeft2", nsISelectionController::MOVE_LEFT, 1},
    {"cmd_moveRight2", "cmd_selectRight2", nsISelectionController::MOVE_RIGHT,
     1},
    {"cmd_moveUp2", "cmd_selectUp2", nsISelectionController::MOVE_UP, 1},
    {"cmd_moveDown2", "cmd_selectDown2", nsISelectionController::MOVE_DOWN, 1}};

NS_IMETHODIMP
SelectionMoveCommands::DoCommand(const char* aCommandName,
                                 nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsCOMPtr<nsIDocument> doc = textEditor->GetDocument();
  if (doc) {
||||||| merged common ancestors
  { "cmd_moveLeft", "cmd_selectLeft",
    nsISelectionController::MOVE_LEFT, 0 },
  { "cmd_moveRight", "cmd_selectRight",
    nsISelectionController::MOVE_RIGHT, 0 },
  { "cmd_moveUp", "cmd_selectUp",
    nsISelectionController::MOVE_UP, 0 },
  { "cmd_moveDown", "cmd_selectDown",
    nsISelectionController::MOVE_DOWN, 0 },
  { "cmd_moveLeft2", "cmd_selectLeft2",
    nsISelectionController::MOVE_LEFT, 1 },
  { "cmd_moveRight2", "cmd_selectRight2",
    nsISelectionController::MOVE_RIGHT, 1 },
  { "cmd_moveUp2", "cmd_selectUp2",
    nsISelectionController::MOVE_UP, 1 },
  { "cmd_moveDown2", "cmd_selectDown2",
    nsISelectionController::MOVE_DOWN, 1 }
};

NS_IMETHODIMP
SelectionMoveCommands::DoCommand(const char* aCommandName,
                                 nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsCOMPtr<nsIDocument> doc = textEditor->GetDocument();
  if (doc) {
=======
    {Command::MoveLeft, Command::SelectLeft, nsISelectionController::MOVE_LEFT,
     0},
    {Command::MoveRight, Command::SelectRight,
     nsISelectionController::MOVE_RIGHT, 0},
    {Command::MoveUp, Command::SelectUp, nsISelectionController::MOVE_UP, 0},
    {Command::MoveDown, Command::SelectDown, nsISelectionController::MOVE_DOWN,
     0},
    {Command::MoveLeft2, Command::SelectLeft2,
     nsISelectionController::MOVE_LEFT, 1},
    {Command::MoveRight2, Command::SelectRight2,
     nsISelectionController::MOVE_RIGHT, 1},
    {Command::MoveUp2, Command::SelectUp2, nsISelectionController::MOVE_UP, 1},
    {Command::MoveDown2, Command::SelectDown2,
     nsISelectionController::MOVE_DOWN, 1}};

nsresult SelectionMoveCommands::DoCommand(Command aCommand,
                                          TextEditor& aTextEditor,
                                          nsIPrincipal* aPrincipal) const {
  RefPtr<Document> document = aTextEditor.GetDocument();
  if (document) {
>>>>>>> upstream-releases
    // Most of the commands below (possibly all of them) need layout to
    // be up to date.
    document->FlushPendingNotifications(FlushType::Layout);
  }

  nsCOMPtr<nsISelectionController> selectionController =
<<<<<<< HEAD
      textEditor->GetSelectionController();
||||||| merged common ancestors
    textEditor->GetSelectionController();
=======
      aTextEditor.GetSelectionController();
>>>>>>> upstream-releases
  if (NS_WARN_IF(!selectionController)) {
    return NS_ERROR_FAILURE;
  }

  // scroll commands
<<<<<<< HEAD
  for (size_t i = 0; i < mozilla::ArrayLength(scrollCommands); i++) {
    const ScrollCommand& cmd = scrollCommands[i];
    if (!nsCRT::strcmp(aCommandName, cmd.reverseScroll)) {
||||||| merged common ancestors
  for (size_t i = 0; i < mozilla::ArrayLength(scrollCommands); i++) {
    const ScrollCommand &cmd = scrollCommands[i];
    if (!nsCRT::strcmp(aCommandName, cmd.reverseScroll)) {
=======
  for (size_t i = 0; i < ArrayLength(scrollCommands); i++) {
    const ScrollCommand& cmd = scrollCommands[i];
    if (aCommand == cmd.mReverseScroll) {
>>>>>>> upstream-releases
      return (selectionController->*(cmd.scroll))(false);
    }
    if (aCommand == cmd.mForwardScroll) {
      return (selectionController->*(cmd.scroll))(true);
    }
  }

  // caret movement/selection commands
<<<<<<< HEAD
  for (size_t i = 0; i < mozilla::ArrayLength(moveCommands); i++) {
    const MoveCommand& cmd = moveCommands[i];
    if (!nsCRT::strcmp(aCommandName, cmd.reverseMove)) {
||||||| merged common ancestors
  for (size_t i = 0; i < mozilla::ArrayLength(moveCommands); i++) {
    const MoveCommand &cmd = moveCommands[i];
    if (!nsCRT::strcmp(aCommandName, cmd.reverseMove)) {
=======
  for (size_t i = 0; i < ArrayLength(moveCommands); i++) {
    const MoveCommand& cmd = moveCommands[i];
    if (aCommand == cmd.mReverseMove) {
>>>>>>> upstream-releases
      return (selectionController->*(cmd.move))(false, false);
    }
    if (aCommand == cmd.mForwardMove) {
      return (selectionController->*(cmd.move))(true, false);
    }
    if (aCommand == cmd.mReverseSelect) {
      return (selectionController->*(cmd.move))(false, true);
    }
    if (aCommand == cmd.mForwardSelect) {
      return (selectionController->*(cmd.move))(true, true);
    }
  }

  // physical-direction movement/selection
<<<<<<< HEAD
  for (size_t i = 0; i < mozilla::ArrayLength(physicalCommands); i++) {
    const PhysicalCommand& cmd = physicalCommands[i];
    if (!nsCRT::strcmp(aCommandName, cmd.move)) {
||||||| merged common ancestors
  for (size_t i = 0; i < mozilla::ArrayLength(physicalCommands); i++) {
    const PhysicalCommand &cmd = physicalCommands[i];
    if (!nsCRT::strcmp(aCommandName, cmd.move)) {
=======
  for (size_t i = 0; i < ArrayLength(physicalCommands); i++) {
    const PhysicalCommand& cmd = physicalCommands[i];
    if (aCommand == cmd.mMove) {
>>>>>>> upstream-releases
      return selectionController->PhysicalMove(cmd.direction, cmd.amount,
                                               false);
<<<<<<< HEAD
    } else if (!nsCRT::strcmp(aCommandName, cmd.select)) {
      return selectionController->PhysicalMove(cmd.direction, cmd.amount, true);
||||||| merged common ancestors
    } else if (!nsCRT::strcmp(aCommandName, cmd.select)) {
      return selectionController->PhysicalMove(cmd.direction, cmd.amount,
                                               true);
=======
    }
    if (aCommand == cmd.mSelect) {
      return selectionController->PhysicalMove(cmd.direction, cmd.amount, true);
>>>>>>> upstream-releases
    }
  }

  return NS_ERROR_FAILURE;
}

<<<<<<< HEAD
NS_IMETHODIMP
SelectionMoveCommands::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
SelectionMoveCommands::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* aCommandRefCon) {
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
||||||| merged common ancestors
NS_IMETHODIMP
SelectionMoveCommands::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
SelectionMoveCommands::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* aCommandRefCon)
{
  bool canUndo;
  IsCommandEnabled(aCommandName, aCommandRefCon, &canUndo);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, canUndo);
=======
nsresult SelectionMoveCommands::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::InsertPlaintextCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
InsertPlaintextCommand::IsCommandEnabled(const char* aCommandName,
                                         nsISupports* aCommandRefCon,
                                         bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    *aIsEnabled = false;
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
InsertPlaintextCommand::IsCommandEnabled(const char* aCommandName,
                                         nsISupports* aCommandRefCon,
                                         bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    *aIsEnabled = false;
    return NS_ERROR_FAILURE;
=======
StaticRefPtr<InsertPlaintextCommand> InsertPlaintextCommand::sInstance;

bool InsertPlaintextCommand::IsCommandEnabled(Command aCommand,
                                              TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
  return aTextEditor->IsSelectionEditable();
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertPlaintextCommand::DoCommand(const char* aCommandName,
                                  nsISupports* aCommandRefCon) {
  // No value is equivalent to empty string
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
||||||| merged common ancestors
NS_IMETHODIMP
InsertPlaintextCommand::DoCommand(const char* aCommandName,
                                  nsISupports* aCommandRefCon)
{
  // No value is equivalent to empty string
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
=======
nsresult InsertPlaintextCommand::DoCommand(Command aCommand,
                                           TextEditor& aTextEditor,
                                           nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  // XXX InsertTextAsAction() is not same as OnInputText().  However, other
  //     commands to insert line break or paragraph separator use OnInput*().
  //     According to the semantics of those methods, using *AsAction() is
  //     better, however, this may not cause two or more placeholder
  //     transactions to the top transaction since its name may not be
  //     nsGkAtoms::TypingTxnName.
  DebugOnly<nsresult> rv =
      aTextEditor.InsertTextAsAction(EmptyString(), aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "InsertTextAsAction() failed");
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertPlaintextCommand::DoCommandParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon) {
  if (NS_WARN_IF(!aParams)) {
||||||| merged common ancestors
NS_IMETHODIMP
InsertPlaintextCommand::DoCommandParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon)
{
  if (NS_WARN_IF(!aParams)) {
=======
nsresult InsertPlaintextCommand::DoCommandParam(
    Command aCommand, const nsAString& aStringParam, TextEditor& aTextEditor,
    nsIPrincipal* aPrincipal) const {
  if (NS_WARN_IF(aStringParam.IsVoid())) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  // XXX InsertTextAsAction() is not same as OnInputText().  However, other
  //     commands to insert line break or paragraph separator use OnInput*().
  //     According to the semantics of those methods, using *AsAction() is
  //     better, however, this may not cause two or more placeholder
  //     transactions to the top transaction since its name may not be
  //     nsGkAtoms::TypingTxnName.
  DebugOnly<nsresult> rv =
      aTextEditor.InsertTextAsAction(aStringParam, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "InsertTextAsAction() failed");
  return NS_OK;
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertPlaintextCommand::GetCommandStateParams(const char* aCommandName,
                                              nsICommandParams* aParams,
                                              nsISupports* aCommandRefCon) {
  if (NS_WARN_IF(!aParams)) {
    return NS_ERROR_INVALID_ARG;
  }

  bool aIsEnabled = false;
  IsCommandEnabled(aCommandName, aCommandRefCon, &aIsEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, aIsEnabled);
||||||| merged common ancestors
NS_IMETHODIMP
InsertPlaintextCommand::GetCommandStateParams(const char* aCommandName,
                                              nsICommandParams* aParams,
                                              nsISupports* aCommandRefCon)
{
  if (NS_WARN_IF(!aParams)) {
    return NS_ERROR_INVALID_ARG;
  }

  bool aIsEnabled = false;
  IsCommandEnabled(aCommandName, aCommandRefCon, &aIsEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, aIsEnabled);
=======
nsresult InsertPlaintextCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::InsertParagraphCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
InsertParagraphCommand::IsCommandEnabled(const char* aCommandName,
                                         nsISupports* aCommandRefCon,
                                         bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    *aIsEnabled = false;
    return NS_ERROR_FAILURE;
  }

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *aIsEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}
||||||| merged common ancestors
NS_IMETHODIMP
InsertParagraphCommand::IsCommandEnabled(const char* aCommandName,
                                         nsISupports* aCommandRefCon,
                                         bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    *aIsEnabled = false;
    return NS_ERROR_FAILURE;
  }

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *aIsEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}
=======
StaticRefPtr<InsertParagraphCommand> InsertParagraphCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
InsertParagraphCommand::DoCommand(const char* aCommandName,
                                  nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
InsertParagraphCommand::DoCommand(const char* aCommandName,
                                  nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
bool InsertParagraphCommand::IsCommandEnabled(Command aCommand,
                                              TextEditor* aTextEditor) const {
  if (!aTextEditor || !aTextEditor->AsHTMLEditor()) {
    return false;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD

  HTMLEditor* htmlEditor = editor->AsHTMLEditor();
  if (!htmlEditor) {
    return NS_OK;  // Do nothing for now.
  }
  return htmlEditor->InsertParagraphSeparatorAsAction();
||||||| merged common ancestors

  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  // XXX OnInputParagraphSeparator() is a handler of user input.  So, this
  //     call may not be expected.
  return textEditor->OnInputParagraphSeparator();
=======
  return aTextEditor->IsSelectionEditable();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertParagraphCommand::DoCommandParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
InsertParagraphCommand::GetCommandStateParams(const char* aCommandName,
                                              nsICommandParams* aParams,
                                              nsISupports* aCommandRefCon) {
  if (NS_WARN_IF(!aParams)) {
    return NS_ERROR_INVALID_ARG;
||||||| merged common ancestors
NS_IMETHODIMP
InsertParagraphCommand::DoCommandParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
InsertParagraphCommand::GetCommandStateParams(const char* aCommandName,
                                              nsICommandParams* aParams,
                                              nsISupports* aCommandRefCon)
{
  if (NS_WARN_IF(!aParams)) {
    return NS_ERROR_INVALID_ARG;
=======
nsresult InsertParagraphCommand::DoCommand(Command aCommand,
                                           TextEditor& aTextEditor,
                                           nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (!htmlEditor) {
    return NS_OK;  // Do nothing for now.
>>>>>>> upstream-releases
  }
  nsresult rv = htmlEditor->InsertParagraphSeparatorAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                       "InsertParagraphSeparatorAsAction() failed");
  return rv;
}

nsresult InsertParagraphCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
}

/******************************************************************************
 * mozilla::InsertLineBreakCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
InsertLineBreakCommand::IsCommandEnabled(const char* aCommandName,
                                         nsISupports* aCommandRefCon,
                                         bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    *aIsEnabled = false;
    return NS_ERROR_FAILURE;
  }
||||||| merged common ancestors
NS_IMETHODIMP
InsertLineBreakCommand::IsCommandEnabled(const char* aCommandName,
                                         nsISupports* aCommandRefCon,
                                         bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    *aIsEnabled = false;
    return NS_ERROR_FAILURE;
  }
=======
StaticRefPtr<InsertLineBreakCommand> InsertLineBreakCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *aIsEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}

NS_IMETHODIMP
InsertLineBreakCommand::DoCommand(const char* aCommandName,
                                  nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *aIsEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}

NS_IMETHODIMP
InsertLineBreakCommand::DoCommand(const char* aCommandName,
                                  nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
bool InsertLineBreakCommand::IsCommandEnabled(Command aCommand,
                                              TextEditor* aTextEditor) const {
  if (!aTextEditor || !aTextEditor->AsHTMLEditor()) {
    return false;
>>>>>>> upstream-releases
  }
  return aTextEditor->IsSelectionEditable();
}

nsresult InsertLineBreakCommand::DoCommand(Command aCommand,
                                           TextEditor& aTextEditor,
                                           nsIPrincipal* aPrincipal) const {
  HTMLEditor* htmlEditor = aTextEditor.AsHTMLEditor();
  if (!htmlEditor) {
    return NS_ERROR_FAILURE;
  }
<<<<<<< HEAD
  return htmlEditor->InsertLineBreakAsAction();
||||||| merged common ancestors
  // XXX OnInputLineBreak() is a handler of user input.  So, this call may not
  //     be expected.
  return htmlEditor->OnInputLineBreak();
=======
  nsresult rv = MOZ_KnownLive(htmlEditor)->InsertLineBreakAsAction(aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "InsertLineBreakAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
InsertLineBreakCommand::DoCommandParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon) {
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
InsertLineBreakCommand::GetCommandStateParams(const char* aCommandName,
                                              nsICommandParams* aParams,
                                              nsISupports* aCommandRefCon) {
  if (NS_WARN_IF(!aParams)) {
    return NS_ERROR_INVALID_ARG;
  }

  bool aIsEnabled = false;
  IsCommandEnabled(aCommandName, aCommandRefCon, &aIsEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, aIsEnabled);
||||||| merged common ancestors
NS_IMETHODIMP
InsertLineBreakCommand::DoCommandParams(const char* aCommandName,
                                        nsICommandParams* aParams,
                                        nsISupports* aCommandRefCon)
{
  return DoCommand(aCommandName, aCommandRefCon);
}

NS_IMETHODIMP
InsertLineBreakCommand::GetCommandStateParams(const char* aCommandName,
                                              nsICommandParams* aParams,
                                              nsISupports* aCommandRefCon)
{
  if (NS_WARN_IF(!aParams)) {
    return NS_ERROR_INVALID_ARG;
  }

  bool aIsEnabled = false;
  IsCommandEnabled(aCommandName, aCommandRefCon, &aIsEnabled);
  return aParams->AsCommandParams()->SetBool(STATE_ENABLED, aIsEnabled);
=======
nsresult InsertLineBreakCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  return aParams.SetBool(STATE_ENABLED,
                         IsCommandEnabled(aCommand, aTextEditor));
>>>>>>> upstream-releases
}

/******************************************************************************
 * mozilla::PasteQuotationCommand
 ******************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
PasteQuotationCommand::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* aCommandRefCon,
                                        bool* aIsEnabled) {
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  *aIsEnabled = false;

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  if (textEditor->IsSingleLineEditor()) {
    return NS_OK;
  }
  return textEditor->CanPaste(nsIClipboard::kGlobalClipboard, aIsEnabled);
}
||||||| merged common ancestors
NS_IMETHODIMP
PasteQuotationCommand::IsCommandEnabled(const char* aCommandName,
                                        nsISupports* aCommandRefCon,
                                        bool* aIsEnabled)
{
  if (NS_WARN_IF(!aIsEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  *aIsEnabled = false;

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  if (textEditor->IsSingleLineEditor()) {
    return NS_OK;
  }
  return textEditor->CanPaste(nsIClipboard::kGlobalClipboard, aIsEnabled);
}
=======
StaticRefPtr<PasteQuotationCommand> PasteQuotationCommand::sInstance;
>>>>>>> upstream-releases

<<<<<<< HEAD
NS_IMETHODIMP
PasteQuotationCommand::DoCommand(const char* aCommandName,
                                 nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
||||||| merged common ancestors
NS_IMETHODIMP
PasteQuotationCommand::DoCommand(const char* aCommandName,
                                 nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_FAILURE;
=======
bool PasteQuotationCommand::IsCommandEnabled(Command aCommand,
                                             TextEditor* aTextEditor) const {
  if (!aTextEditor) {
    return false;
>>>>>>> upstream-releases
  }
<<<<<<< HEAD
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsresult rv = textEditor->PasteAsQuotationAsAction(
      nsIClipboard::kGlobalClipboard, true);
  if (rv == NS_ERROR_EDITOR_DESTROYED) {
    // Return NS_OK when editor is destroyed since it's expected by the
    // web app.
    return NS_OK;
  }
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
||||||| merged common ancestors
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  return textEditor->PasteAsQuotationAsAction(nsIClipboard::kGlobalClipboard,
                                              true);
=======
  return !aTextEditor->IsSingleLineEditor() &&
         aTextEditor->CanPaste(nsIClipboard::kGlobalClipboard);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteQuotationCommand::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  nsresult rv = textEditor->PasteAsQuotationAsAction(
      nsIClipboard::kGlobalClipboard, true);
  if (rv == NS_ERROR_EDITOR_DESTROYED) {
    // Return NS_OK when editor is destroyed since it's expected by the
    // web app.
    return NS_OK;
  }
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
||||||| merged common ancestors
NS_IMETHODIMP
PasteQuotationCommand::DoCommandParams(const char* aCommandName,
                                       nsICommandParams* aParams,
                                       nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
    return NS_ERROR_FAILURE;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  return textEditor->PasteAsQuotationAsAction(nsIClipboard::kGlobalClipboard,
                                              true);
=======
nsresult PasteQuotationCommand::DoCommand(Command aCommand,
                                          TextEditor& aTextEditor,
                                          nsIPrincipal* aPrincipal) const {
  nsresult rv = aTextEditor.PasteAsQuotationAsAction(
      nsIClipboard::kGlobalClipboard, true, aPrincipal);
  NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "PasteAsQuotationAsAction() failed");
  return rv;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
NS_IMETHODIMP
PasteQuotationCommand::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* aCommandRefCon) {
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
||||||| merged common ancestors
NS_IMETHODIMP
PasteQuotationCommand::GetCommandStateParams(const char* aCommandName,
                                             nsICommandParams* aParams,
                                             nsISupports* aCommandRefCon)
{
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(aCommandRefCon);
  if (!editor) {
=======
nsresult PasteQuotationCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  if (!aTextEditor) {
>>>>>>> upstream-releases
    return NS_OK;
  }
  aParams.SetBool(STATE_ENABLED,
                  aTextEditor->CanPaste(nsIClipboard::kGlobalClipboard));
  return NS_OK;
}

}  // namespace mozilla
