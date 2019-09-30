/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

<<<<<<< HEAD
#include "mozilla/HTMLEditor.h"          // for HTMLEditor
#include "mozilla/HTMLEditorCommands.h"  // for SetDocumentOptionsCommand, etc
#include "mozilla/TextEditor.h"          // for TextEditor
#include "nsCommandParams.h"             // for nsCommandParams
#include "nsCOMPtr.h"                    // for nsCOMPtr, do_QueryInterface, etc
#include "nsCRT.h"                       // for nsCRT
#include "nsDebug.h"                     // for NS_ENSURE_ARG_POINTER, etc
#include "nsError.h"                     // for NS_ERROR_INVALID_ARG, etc
#include "nsIDocShell.h"                 // for nsIDocShell
#include "nsIDocument.h"                 // for nsIDocument
#include "nsIEditingSession.h"           // for nsIEditingSession, etc
#include "nsIEditor.h"                   // for nsIEditor
#include "nsIPlaintextEditor.h"          // for nsIPlaintextEditor, etc
#include "nsIPresShell.h"                // for nsIPresShell
#include "nsISelectionController.h"      // for nsISelectionController
#include "nsISupportsImpl.h"             // for nsPresContext::Release
#include "nsISupportsUtils.h"            // for NS_IF_ADDREF
#include "nsIURI.h"                      // for nsIURI
#include "nsPresContext.h"               // for nsPresContext
#include "nscore.h"                      // for NS_IMETHODIMP, nsresult, etc

class nsISupports;

// defines
#define STATE_ENABLED "state_enabled"
||||||| merged common ancestors

#include "mozilla/HTMLEditor.h"         // for HTMLEditor
#include "mozilla/HTMLEditorCommands.h" // for SetDocumentOptionsCommand, etc
#include "mozilla/TextEditor.h"         // for TextEditor
#include "nsCommandParams.h"            // for nsCommandParams
#include "nsCOMPtr.h"                   // for nsCOMPtr, do_QueryInterface, etc
#include "nsCRT.h"                      // for nsCRT
#include "nsDebug.h"                    // for NS_ENSURE_ARG_POINTER, etc
#include "nsError.h"                    // for NS_ERROR_INVALID_ARG, etc
#include "nsIDocShell.h"                // for nsIDocShell
#include "nsIDocument.h"                // for nsIDocument
#include "nsIEditingSession.h"          // for nsIEditingSession, etc
#include "nsIEditor.h"                  // for nsIEditor
#include "nsIPlaintextEditor.h"         // for nsIPlaintextEditor, etc
#include "nsIPresShell.h"               // for nsIPresShell
#include "nsISelectionController.h"     // for nsISelectionController
#include "nsISupportsImpl.h"            // for nsPresContext::Release
#include "nsISupportsUtils.h"           // for NS_IF_ADDREF
#include "nsIURI.h"                     // for nsIURI
#include "nsPresContext.h"              // for nsPresContext
#include "nscore.h"                     // for NS_IMETHODIMP, nsresult, etc

class nsISupports;

//defines
#define STATE_ENABLED  "state_enabled"
=======
#include "mozilla/EditorCommands.h"

#include "mozilla/HTMLEditor.h"      // for HTMLEditor
#include "mozilla/TextEditor.h"      // for TextEditor
#include "mozilla/dom/Document.h"    // for Document
#include "nsCommandParams.h"         // for nsCommandParams
#include "nsIDocShell.h"             // for nsIDocShell
#include "nsIEditingSession.h"       // for nsIEditingSession, etc
#include "nsIPrincipal.h"            // for nsIPrincipal
#include "nsISelectionController.h"  // for nsISelectionController
#include "nsISupportsImpl.h"         // for nsPresContext::Release
#include "nsISupportsUtils.h"        // for NS_IF_ADDREF
#include "nsIURI.h"                  // for nsIURI
#include "nsPresContext.h"           // for nsPresContext

// defines
#define STATE_ENABLED "state_enabled"
>>>>>>> upstream-releases
#define STATE_ALL "state_all"
#define STATE_ATTRIBUTE "state_attribute"
#define STATE_DATA "state_data"

namespace mozilla {

<<<<<<< HEAD
NS_IMETHODIMP
SetDocumentOptionsCommand::IsCommandEnabled(const char* aCommandName,
                                            nsISupports* refCon,
                                            bool* outCmdEnabled) {
  if (NS_WARN_IF(!outCmdEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *outCmdEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}

NS_IMETHODIMP
SetDocumentOptionsCommand::DoCommand(const char* aCommandName,
                                     nsISupports* refCon) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
SetDocumentOptionsCommand::DoCommandParams(const char* aCommandName,
                                           nsICommandParams* aParams,
                                           nsISupports* refCon) {
  if (NS_WARN_IF(!aParams)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_INVALID_ARG;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);

  RefPtr<nsPresContext> presContext = textEditor->GetPresContext();
  if (NS_WARN_IF(!presContext)) {
    return NS_ERROR_FAILURE;
  }

  nsCommandParams* params = aParams->AsCommandParams();

  IgnoredErrorResult error;
  int32_t animationMode = params->GetInt("imageAnimation", error);
  if (!error.Failed()) {
    // for possible values of animation mode, see:
    // http://lxr.mozilla.org/seamonkey/source/image/public/imgIContainer.idl
    presContext->SetImageAnimationMode(animationMode);
  } else {
    error.SuppressException();
  }

  bool allowPlugins = params->GetBool("plugins", error);
  if (error.Failed()) {
    return NS_OK;
  }

  nsCOMPtr<nsIDocShell> docShell(presContext->GetDocShell());
  if (NS_WARN_IF(!docShell)) {
    return NS_ERROR_FAILURE;
  }

  nsresult rv = docShell->SetAllowPlugins(allowPlugins);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
}

NS_IMETHODIMP
SetDocumentOptionsCommand::GetCommandStateParams(const char* aCommandName,
                                                 nsICommandParams* aParams,
                                                 nsISupports* refCon) {
  if (NS_WARN_IF(!aParams) || NS_WARN_IF(!refCon)) {
    return NS_ERROR_INVALID_ARG;
  }

  // The base editor owns most state info
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_INVALID_ARG;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);

  nsCommandParams* params = aParams->AsCommandParams();

  // Always get the enabled state
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  nsresult rv = params->SetBool(STATE_ENABLED, outCmdEnabled);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  // get pres context
  RefPtr<nsPresContext> presContext = textEditor->GetPresContext();
  if (NS_WARN_IF(!presContext)) {
    return NS_ERROR_FAILURE;
  }

  IgnoredErrorResult error;
  Unused << params->GetInt("imageAnimation", error);
  if (!error.Failed()) {
    // for possible values of animation mode, see
    // http://lxr.mozilla.org/seamonkey/source/image/public/imgIContainer.idl
    rv = params->SetInt("imageAnimation", presContext->ImageAnimationMode());
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
  } else {
    error.SuppressException();
  }

  bool allowPlugins = params->GetBool("plugins", error);
  if (error.Failed()) {
    return NS_OK;
  }

  nsCOMPtr<nsIDocShell> docShell(presContext->GetDocShell());
  if (NS_WARN_IF(!docShell)) {
    return NS_ERROR_FAILURE;
  }

  allowPlugins = docShell->PluginsAllowedInCurrentDoc();
  rv = params->SetBool("plugins", allowPlugins);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
}

/**
||||||| merged common ancestors
NS_IMETHODIMP
SetDocumentOptionsCommand::IsCommandEnabled(const char* aCommandName,
                                            nsISupports* refCon,
                                            bool* outCmdEnabled)
{
  if (NS_WARN_IF(!outCmdEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (!editor) {
    *outCmdEnabled = false;
    return NS_OK;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);
  *outCmdEnabled = textEditor->IsSelectionEditable();
  return NS_OK;
}

NS_IMETHODIMP
SetDocumentOptionsCommand::DoCommand(const char* aCommandName,
                                     nsISupports* refCon)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
SetDocumentOptionsCommand::DoCommandParams(const char* aCommandName,
                                           nsICommandParams* aParams,
                                           nsISupports* refCon)
{
  if (NS_WARN_IF(!aParams)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_INVALID_ARG;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);

  RefPtr<nsPresContext> presContext = textEditor->GetPresContext();
  if (NS_WARN_IF(!presContext)) {
    return NS_ERROR_FAILURE;
  }

  nsCommandParams* params = aParams->AsCommandParams();

  IgnoredErrorResult error;
  int32_t animationMode = params->GetInt("imageAnimation", error);
  if (!error.Failed()) {
    // for possible values of animation mode, see:
    // http://lxr.mozilla.org/seamonkey/source/image/public/imgIContainer.idl
    presContext->SetImageAnimationMode(animationMode);
  } else {
    error.SuppressException();
  }

  bool allowPlugins = params->GetBool("plugins", error);
  if (error.Failed()) {
    return NS_OK;
  }

  nsCOMPtr<nsIDocShell> docShell(presContext->GetDocShell());
  if (NS_WARN_IF(!docShell)) {
    return NS_ERROR_FAILURE;
  }

  nsresult rv = docShell->SetAllowPlugins(allowPlugins);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
}

NS_IMETHODIMP
SetDocumentOptionsCommand::GetCommandStateParams(const char* aCommandName,
                                                 nsICommandParams* aParams,
                                                 nsISupports* refCon)
{
  if (NS_WARN_IF(!aParams) || NS_WARN_IF(!refCon)) {
    return NS_ERROR_INVALID_ARG;
  }

  // The base editor owns most state info
  nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
  if (NS_WARN_IF(!editor)) {
    return NS_ERROR_INVALID_ARG;
  }
  TextEditor* textEditor = editor->AsTextEditor();
  MOZ_ASSERT(textEditor);

  nsCommandParams* params = aParams->AsCommandParams();

  // Always get the enabled state
  bool outCmdEnabled = false;
  IsCommandEnabled(aCommandName, refCon, &outCmdEnabled);
  nsresult rv = params->SetBool(STATE_ENABLED, outCmdEnabled);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  // get pres context
  RefPtr<nsPresContext> presContext = textEditor->GetPresContext();
  if (NS_WARN_IF(!presContext)) {
    return NS_ERROR_FAILURE;
  }

  IgnoredErrorResult error;
  Unused << params->GetInt("imageAnimation", error);
  if (!error.Failed()) {
    // for possible values of animation mode, see
    // http://lxr.mozilla.org/seamonkey/source/image/public/imgIContainer.idl
    rv = params->SetInt("imageAnimation", presContext->ImageAnimationMode());
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
  } else {
    error.SuppressException();
  }

  bool allowPlugins = params->GetBool("plugins", error);
  if (error.Failed()) {
    return NS_OK;
  }

  nsCOMPtr<nsIDocShell> docShell(presContext->GetDocShell());
  if (NS_WARN_IF(!docShell)) {
    return NS_ERROR_FAILURE;
  }

  allowPlugins = docShell->PluginsAllowedInCurrentDoc();
  rv = params->SetBool("plugins", allowPlugins);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return NS_OK;
}


/**
=======
/*****************************************************************************
 * mozilla::SetDocumentStateCommand
 *
>>>>>>> upstream-releases
 *  Commands for document state that may be changed via doCommandParams
 *  As of 11/11/02, this is just "cmd_setDocumentModified"
 *  Note that you can use the same command class, SetDocumentStateCommand,
 *    for more than one of this type of command
 *    We check the input command param for different behavior
 *****************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
SetDocumentStateCommand::IsCommandEnabled(const char* aCommandName,
                                          nsISupports* refCon,
                                          bool* outCmdEnabled) {
  if (NS_WARN_IF(!outCmdEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
||||||| merged common ancestors
NS_IMETHODIMP
SetDocumentStateCommand::IsCommandEnabled(const char* aCommandName,
                                          nsISupports* refCon,
                                          bool* outCmdEnabled)
{
  if (NS_WARN_IF(!outCmdEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
=======
StaticRefPtr<SetDocumentStateCommand> SetDocumentStateCommand::sInstance;
>>>>>>> upstream-releases

bool SetDocumentStateCommand::IsCommandEnabled(Command aCommand,
                                               TextEditor* aTextEditor) const {
  // These commands are always enabled if given editor is an HTMLEditor.
  return aTextEditor && aTextEditor->AsHTMLEditor();
}

<<<<<<< HEAD
NS_IMETHODIMP
SetDocumentStateCommand::DoCommand(const char* aCommandName,
                                   nsISupports* refCon) {
||||||| merged common ancestors
NS_IMETHODIMP
SetDocumentStateCommand::DoCommand(const char* aCommandName,
                                   nsISupports* refCon)
{
=======
nsresult SetDocumentStateCommand::DoCommand(Command aCommand,
                                            TextEditor& aTextEditor,
                                            nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
NS_IMETHODIMP
SetDocumentStateCommand::DoCommandParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon) {
  if (NS_WARN_IF(!aParams)) {
||||||| merged common ancestors
NS_IMETHODIMP
SetDocumentStateCommand::DoCommandParams(const char* aCommandName,
                                         nsICommandParams* aParams,
                                         nsISupports* refCon)
{
  if (NS_WARN_IF(!aParams)) {
=======
nsresult SetDocumentStateCommand::DoCommandParam(
    Command aCommand, const Maybe<bool>& aBoolParam, TextEditor& aTextEditor,
    nsIPrincipal* aPrincipal) const {
  if (NS_WARN_IF(aBoolParam.isNothing())) {
>>>>>>> upstream-releases
    return NS_ERROR_INVALID_ARG;
  }

  if (NS_WARN_IF(!aTextEditor.AsHTMLEditor())) {
    return NS_ERROR_FAILURE;
  }

  switch (aCommand) {
    case Command::SetDocumentModified: {
      if (aBoolParam.value()) {
        nsresult rv = aTextEditor.IncrementModificationCount(1);
        NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                             "IncrementModificationCount() failed");
        return rv;
      }
      nsresult rv = aTextEditor.ResetModificationCount();
      NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "ResetModificationCount() failed");
      return rv;
    }
<<<<<<< HEAD
    return NS_OK;
  }

  if (!nsCRT::strcmp(aCommandName, "cmd_setDocumentReadOnly")) {
    ErrorResult error;
    bool isReadOnly = params->GetBool(STATE_ATTRIBUTE, error);
    if (NS_WARN_IF(error.Failed())) {
      return error.StealNSResult();
    }
    if (isReadOnly) {
      nsresult rv =
          textEditor->AddFlags(nsIPlaintextEditor::eEditorReadonlyMask);
      if (NS_WARN_IF(NS_FAILED(rv))) {
||||||| merged common ancestors
    return NS_OK;
  }

  if (!nsCRT::strcmp(aCommandName, "cmd_setDocumentReadOnly")) {
    ErrorResult error;
    bool isReadOnly = params->GetBool(STATE_ATTRIBUTE, error);
    if (NS_WARN_IF(error.Failed())) {
      return error.StealNSResult();
    }
    if (isReadOnly) {
      nsresult rv =
        textEditor->AddFlags(nsIPlaintextEditor::eEditorReadonlyMask);
      if (NS_WARN_IF(NS_FAILED(rv))) {
=======
    case Command::SetDocumentReadOnly: {
      ErrorResult error;
      if (aBoolParam.value()) {
        nsresult rv =
            aTextEditor.AddFlags(nsIPlaintextEditor::eEditorReadonlyMask);
        NS_WARNING_ASSERTION(
            NS_SUCCEEDED(rv),
            "AddFlags(nsIPlaintextEditor::eEditorReadonlyMask) failed");
>>>>>>> upstream-releases
        return rv;
      }
<<<<<<< HEAD
      return NS_OK;
    }
    nsresult rv =
        textEditor->RemoveFlags(nsIPlaintextEditor::eEditorReadonlyMask);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  if (!nsCRT::strcmp(aCommandName, "cmd_setDocumentUseCSS")) {
    HTMLEditor* htmlEditor = textEditor->AsHTMLEditor();
    if (NS_WARN_IF(!htmlEditor)) {
      return NS_ERROR_INVALID_ARG;
    }
    ErrorResult error;
    bool desireCSS = params->GetBool(STATE_ATTRIBUTE, error);
    if (NS_WARN_IF(error.Failed())) {
      return error.StealNSResult();
    }
    nsresult rv = htmlEditor->SetIsCSSEnabled(desireCSS);
    if (NS_WARN_IF(NS_FAILED(rv))) {
||||||| merged common ancestors
      return NS_OK;
    }
    nsresult rv =
      textEditor->RemoveFlags(nsIPlaintextEditor::eEditorReadonlyMask);
    if (NS_WARN_IF(NS_FAILED(rv))) {
      return rv;
    }
    return NS_OK;
  }

  if (!nsCRT::strcmp(aCommandName, "cmd_setDocumentUseCSS")) {
    HTMLEditor* htmlEditor = textEditor->AsHTMLEditor();
    if (NS_WARN_IF(!htmlEditor)) {
      return NS_ERROR_INVALID_ARG;
    }
    ErrorResult error;
    bool desireCSS = params->GetBool(STATE_ATTRIBUTE, error);
    if (NS_WARN_IF(error.Failed())) {
      return error.StealNSResult();
    }
    nsresult rv = htmlEditor->SetIsCSSEnabled(desireCSS);
    if (NS_WARN_IF(NS_FAILED(rv))) {
=======
      nsresult rv =
          aTextEditor.RemoveFlags(nsIPlaintextEditor::eEditorReadonlyMask);
      NS_WARNING_ASSERTION(
          NS_SUCCEEDED(rv),
          "RemoveFlags(nsIPlaintextEditor::eEditorReadonlyMask) failed");
>>>>>>> upstream-releases
      return rv;
    }
<<<<<<< HEAD
    return NS_OK;
  }

  if (!nsCRT::strcmp(aCommandName, "cmd_insertBrOnReturn")) {
    HTMLEditor* htmlEditor = textEditor->AsHTMLEditor();
    if (NS_WARN_IF(!htmlEditor)) {
      return NS_ERROR_INVALID_ARG;
    }
    ErrorResult error;
    bool insertBrOnReturn = params->GetBool(STATE_ATTRIBUTE, error);
    if (NS_WARN_IF(error.Failed())) {
      return error.StealNSResult();
    }
    nsresult rv =
        htmlEditor->SetReturnInParagraphCreatesNewParagraph(!insertBrOnReturn);
    if (NS_WARN_IF(NS_FAILED(rv))) {
||||||| merged common ancestors
    return NS_OK;
  }

  if (!nsCRT::strcmp(aCommandName, "cmd_insertBrOnReturn")) {
    HTMLEditor* htmlEditor = textEditor->AsHTMLEditor();
    if (NS_WARN_IF(!htmlEditor)) {
      return NS_ERROR_INVALID_ARG;
    }
    ErrorResult error;
    bool insertBrOnReturn = params->GetBool(STATE_ATTRIBUTE, error);
    if (NS_WARN_IF(error.Failed())) {
      return error.StealNSResult();
    }
    nsresult rv =
      htmlEditor->SetReturnInParagraphCreatesNewParagraph(!insertBrOnReturn);
    if (NS_WARN_IF(NS_FAILED(rv))) {
=======
    case Command::SetDocumentUseCSS: {
      nsresult rv =
          aTextEditor.AsHTMLEditor()->SetIsCSSEnabled(aBoolParam.value());
      NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "SetIsCSSEnabled() failed");
>>>>>>> upstream-releases
      return rv;
    }
    case Command::SetDocumentInsertBROnEnterKeyPress: {
      nsresult rv =
          aTextEditor.AsHTMLEditor()->SetReturnInParagraphCreatesNewParagraph(
              !aBoolParam.value());
      NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                           "SetReturnInParagraphCreatesNewParagraph() failed");
      return rv;
    }
    case Command::ToggleObjectResizers: {
      MOZ_KnownLive(aTextEditor.AsHTMLEditor())
          ->EnableObjectResizer(aBoolParam.value());
      return NS_OK;
    }
    case Command::ToggleInlineTableEditor: {
      MOZ_KnownLive(aTextEditor.AsHTMLEditor())
          ->EnableInlineTableEditor(aBoolParam.value());
      return NS_OK;
    }
    case Command::ToggleAbsolutePositionEditor: {
      MOZ_KnownLive(aTextEditor.AsHTMLEditor())
          ->EnableAbsolutePositionEditor(aBoolParam.value());
      return NS_OK;
    }
    default:
      return NS_ERROR_NOT_IMPLEMENTED;
  }
}

nsresult SetDocumentStateCommand::DoCommandParam(
    Command aCommand, const nsACString& aCStringParam, TextEditor& aTextEditor,
    nsIPrincipal* aPrincipal) const {
  if (NS_WARN_IF(aCStringParam.IsVoid())) {
    return NS_ERROR_INVALID_ARG;
  }

  if (NS_WARN_IF(!aTextEditor.AsHTMLEditor())) {
    return NS_ERROR_FAILURE;
  }

  switch (aCommand) {
    case Command::SetDocumentDefaultParagraphSeparator: {
      if (aCStringParam.LowerCaseEqualsLiteral("div")) {
        aTextEditor.AsHTMLEditor()->SetDefaultParagraphSeparator(
            ParagraphSeparator::div);
        return NS_OK;
      }
      if (aCStringParam.LowerCaseEqualsLiteral("p")) {
        aTextEditor.AsHTMLEditor()->SetDefaultParagraphSeparator(
            ParagraphSeparator::p);
        return NS_OK;
      }
      if (aCStringParam.LowerCaseEqualsLiteral("br")) {
        // Mozilla extension for backwards compatibility
        aTextEditor.AsHTMLEditor()->SetDefaultParagraphSeparator(
            ParagraphSeparator::br);
        return NS_OK;
      }

      // This should not be reachable from nsHTMLDocument::ExecCommand
      // XXX Shouldn't return error in this case because Chrome does not throw
      //     exception in this case.
      NS_WARNING("Invalid default paragraph separator");
      return NS_ERROR_UNEXPECTED;
    }
    default:
      return NS_ERROR_NOT_IMPLEMENTED;
  }
}

<<<<<<< HEAD
NS_IMETHODIMP
SetDocumentStateCommand::GetCommandStateParams(const char* aCommandName,
                                               nsICommandParams* aParams,
                                               nsISupports* refCon) {
  if (NS_WARN_IF(!aParams) || NS_WARN_IF(!refCon)) {
    return NS_ERROR_INVALID_ARG;
  }

  // If the result is set to STATE_ALL as bool value, queryCommandState()
  // returns the bool value.
||||||| merged common ancestors
NS_IMETHODIMP
SetDocumentStateCommand::GetCommandStateParams(const char* aCommandName,
                                               nsICommandParams* aParams,
                                               nsISupports* refCon)
{
  if (NS_WARN_IF(!aParams) || NS_WARN_IF(!refCon)) {
    return NS_ERROR_INVALID_ARG;
  }

  // If the result is set to STATE_ALL as bool value, queryCommandState()
  // returns the bool value.
=======
nsresult SetDocumentStateCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
>>>>>>> upstream-releases
  // If the result is set to STATE_ATTRIBUTE as CString value,
  // queryCommandValue() returns the string value.
  // Otherwise, ignored.

  // The base editor owns most state info
  if (NS_WARN_IF(!aTextEditor)) {
    return NS_ERROR_INVALID_ARG;
  }

  if (NS_WARN_IF(!aTextEditor->AsHTMLEditor())) {
    return NS_ERROR_FAILURE;
  }

  // Always get the enabled state
  nsresult rv =
      aParams.SetBool(STATE_ENABLED, IsCommandEnabled(aCommand, aTextEditor));
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }

  switch (aCommand) {
    case Command::SetDocumentModified: {
      bool modified;
      rv = aTextEditor->GetDocumentModified(&modified);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
      // XXX Nobody refers this result due to wrong type.
      rv = aParams.SetBool(STATE_ATTRIBUTE, modified);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
      return NS_OK;
    }
    case Command::SetDocumentReadOnly: {
      // XXX Nobody refers this result due to wrong type.
      rv = aParams.SetBool(STATE_ATTRIBUTE, aTextEditor->IsReadonly());
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
      return NS_OK;
    }
<<<<<<< HEAD

    switch (htmlEditor->GetDefaultParagraphSeparator()) {
      case ParagraphSeparator::div: {
        DebugOnly<nsresult> rv =
            params->SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("div"));
        NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                             "Failed to set command params to return \"div\"");
        return NS_OK;
||||||| merged common ancestors

    switch (htmlEditor->GetDefaultParagraphSeparator()) {
      case ParagraphSeparator::div: {
        DebugOnly<nsresult> rv =
          params->SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("div"));
        NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
          "Failed to set command params to return \"div\"");
        return NS_OK;
=======
    case Command::SetDocumentUseCSS: {
      HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
      if (NS_WARN_IF(!htmlEditor)) {
        return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      case ParagraphSeparator::p: {
        DebugOnly<nsresult> rv =
            params->SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("p"));
        NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                             "Failed to set command params to return \"p\"");
        return NS_OK;
||||||| merged common ancestors
      case ParagraphSeparator::p: {
        DebugOnly<nsresult> rv =
          params->SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("p"));
        NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
          "Failed to set command params to return \"p\"");
        return NS_OK;
=======
      rv = aParams.SetBool(STATE_ALL, htmlEditor->IsCSSEnabled());
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
      case ParagraphSeparator::br: {
        DebugOnly<nsresult> rv =
            params->SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("br"));
        NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                             "Failed to set command params to return \"br\"");
        return NS_OK;
||||||| merged common ancestors
      case ParagraphSeparator::br: {
        DebugOnly<nsresult> rv =
          params->SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("br"));
        NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
          "Failed to set command params to return \"br\"");
        return NS_OK;
=======
      return NS_OK;
    }
    case Command::SetDocumentInsertBROnEnterKeyPress: {
      HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
      if (NS_WARN_IF(!htmlEditor)) {
        return NS_ERROR_INVALID_ARG;
      }
      bool createPOnReturn;
      htmlEditor->GetReturnInParagraphCreatesNewParagraph(&createPOnReturn);
      // XXX Nobody refers this result due to wrong type.
      rv = aParams.SetBool(STATE_ATTRIBUTE, !createPOnReturn);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
>>>>>>> upstream-releases
      }
      return NS_OK;
    }
    case Command::SetDocumentDefaultParagraphSeparator: {
      HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
      if (NS_WARN_IF(!htmlEditor)) {
        return NS_ERROR_INVALID_ARG;
      }

      switch (htmlEditor->GetDefaultParagraphSeparator()) {
        case ParagraphSeparator::div: {
          DebugOnly<nsresult> rv =
              aParams.SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("div"));
          NS_WARNING_ASSERTION(
              NS_SUCCEEDED(rv),
              "Failed to set command params to return \"div\"");
          return NS_OK;
        }
        case ParagraphSeparator::p: {
          DebugOnly<nsresult> rv =
              aParams.SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("p"));
          NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                               "Failed to set command params to return \"p\"");
          return NS_OK;
        }
        case ParagraphSeparator::br: {
          DebugOnly<nsresult> rv =
              aParams.SetCString(STATE_ATTRIBUTE, NS_LITERAL_CSTRING("br"));
          NS_WARNING_ASSERTION(NS_SUCCEEDED(rv),
                               "Failed to set command params to return \"br\"");
          return NS_OK;
        }
        default:
          MOZ_ASSERT_UNREACHABLE("Invalid paragraph separator value");
          return NS_ERROR_UNEXPECTED;
      }
    }
    case Command::ToggleObjectResizers: {
      HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
      if (NS_WARN_IF(!htmlEditor)) {
        return NS_ERROR_INVALID_ARG;
      }
      // We returned the result as STATE_ATTRIBUTE with bool value 60 or
      // earlier. So, the result was ignored by both
      // nsHTMLDocument::QueryCommandValue() and
      // nsHTMLDocument::QueryCommandState().
      rv = aParams.SetBool(STATE_ALL, htmlEditor->IsObjectResizerEnabled());
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
      return NS_OK;
    }
    case Command::ToggleInlineTableEditor: {
      HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
      if (NS_WARN_IF(!htmlEditor)) {
        return NS_ERROR_INVALID_ARG;
      }
      // We returned the result as STATE_ATTRIBUTE with bool value 60 or
      // earlier. So, the result was ignored by both
      // nsHTMLDocument::QueryCommandValue() and
      // nsHTMLDocument::QueryCommandState().
      rv = aParams.SetBool(STATE_ALL, htmlEditor->IsInlineTableEditorEnabled());
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
      return NS_OK;
    }
    case Command::ToggleAbsolutePositionEditor: {
      HTMLEditor* htmlEditor = aTextEditor->AsHTMLEditor();
      if (NS_WARN_IF(!htmlEditor)) {
        return NS_ERROR_INVALID_ARG;
      }
      return aParams.SetBool(STATE_ALL,
                             htmlEditor->IsAbsolutePositionEditorEnabled());
    }
    default:
      return NS_ERROR_NOT_IMPLEMENTED;
  }
}

/*****************************************************************************
 * mozilla::DocumentStateCommand
 *
 * Commands just for state notification
 *  As of 11/21/02, possible commands are:
 *    "obs_documentCreated"
 *    "obs_documentWillBeDestroyed"
 *    "obs_documentLocationChanged"
 *  Note that you can use the same command class, DocumentStateCommand
 *    for these or future observer commands.
 *    We check the input command param for different behavior
 *
 *  How to use:
 *  1. Get the nsCommandManager for the current editor
 *  2. Implement an nsIObserve object, e.g:
 *
 *    void Observe(
<<<<<<< HEAD
 *        in nsISupports aSubject, // The nsICommandManager calling this
 *                                 // Observer
||||||| merged common ancestors
 *        in nsISupports aSubject, // The nsICommandManager calling this Observer
=======
 *        in nsISupports aSubject, // The nsCommandManager calling this
 *                                 // Observer
>>>>>>> upstream-releases
 *        in string      aTopic,   // command name, e.g.:"obs_documentCreated"
 *                                 //    or "obs_documentWillBeDestroyed"
          in wstring     aData );  // ignored (set to "command_status_changed")
 *
 *  3. Add the observer by:
 *       commandManager.addObserver(observeobject, obs_documentCreated);
 *  4. In the appropriate location in editorSession, editor, or commands code,
 *     trigger the notification of this observer by something like:
 *
<<<<<<< HEAD
 *  nsCOMPtr<nsICommandManager> commandManager = mDocShell->GetCommandManager();
 *  nsCOMPtr<nsPICommandUpdater> commandUpdater =
 *    do_QueryInterface(commandManager);
 *  NS_ENSURE_TRUE(commandUpdater, NS_ERROR_FAILURE);
 *    commandUpdater->CommandStatusChanged(obs_documentCreated);
||||||| merged common ancestors
 *  nsCOMPtr<nsICommandManager> commandManager = mDocShell->GetCommandManager();
 *  nsCOMPtr<nsPICommandUpdater> commandUpdater = do_QueryInterface(commandManager);
 *  NS_ENSURE_TRUE(commandUpdater, NS_ERROR_FAILURE);
 *    commandUpdater->CommandStatusChanged(obs_documentCreated);
=======
 *  RefPtr<nsCommandManager> commandManager = mDocShell->GetCommandManager();
 *  commandManager->CommandStatusChanged(obs_documentCreated);
>>>>>>> upstream-releases
 *
 *  5. Use GetCommandStateParams() to obtain state information
 *     e.g., any creation state codes when creating an editor are
 *     supplied for "obs_documentCreated" command in the
 *     "state_data" param's value
 *****************************************************************************/

<<<<<<< HEAD
NS_IMETHODIMP
DocumentStateCommand::IsCommandEnabled(const char* aCommandName,
                                       nsISupports* refCon,
                                       bool* outCmdEnabled) {
  if (NS_WARN_IF(!outCmdEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
||||||| merged common ancestors
NS_IMETHODIMP
DocumentStateCommand::IsCommandEnabled(const char* aCommandName,
                                       nsISupports* refCon,
                                       bool* outCmdEnabled)
{
  if (NS_WARN_IF(!outCmdEnabled)) {
    return NS_ERROR_INVALID_ARG;
  }
=======
StaticRefPtr<DocumentStateCommand> DocumentStateCommand::sInstance;
>>>>>>> upstream-releases

bool DocumentStateCommand::IsCommandEnabled(Command aCommand,
                                            TextEditor* aTextEditor) const {
  // Always return false to discourage callers from using DoCommand()
  return false;
}

<<<<<<< HEAD
NS_IMETHODIMP
DocumentStateCommand::DoCommand(const char* aCommandName, nsISupports* refCon) {
||||||| merged common ancestors
NS_IMETHODIMP
DocumentStateCommand::DoCommand(const char* aCommandName,
                                nsISupports* refCon)
{
=======
nsresult DocumentStateCommand::DoCommand(Command aCommand,
                                         TextEditor& aTextEditor,
                                         nsIPrincipal* aPrincipal) const {
>>>>>>> upstream-releases
  return NS_ERROR_NOT_IMPLEMENTED;
}

<<<<<<< HEAD
NS_IMETHODIMP
DocumentStateCommand::DoCommandParams(const char* aCommandName,
                                      nsICommandParams* aParams,
                                      nsISupports* refCon) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
DocumentStateCommand::GetCommandStateParams(const char* aCommandName,
                                            nsICommandParams* aParams,
                                            nsISupports* refCon) {
  if (NS_WARN_IF(!aParams) || NS_WARN_IF(!aCommandName)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsCommandParams* params = aParams->AsCommandParams();

  if (!nsCRT::strcmp(aCommandName, "obs_documentCreated")) {
    uint32_t editorStatus = nsIEditingSession::eEditorErrorUnknown;

    nsCOMPtr<nsIEditingSession> editingSession = do_QueryInterface(refCon);
    if (editingSession) {
      // refCon is initially set to nsIEditingSession until editor
      //  is successfully created and source doc is loaded
      // Embedder gets error status if this fails
      // If called before startup is finished,
      //    status = eEditorCreationInProgress
      nsresult rv = editingSession->GetEditorStatus(&editorStatus);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
    } else {
      // If refCon is an editor, then everything started up OK!
      nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
      if (editor) {
||||||| merged common ancestors
NS_IMETHODIMP
DocumentStateCommand::DoCommandParams(const char* aCommandName,
                                      nsICommandParams* aParams,
                                      nsISupports* refCon)
{
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
DocumentStateCommand::GetCommandStateParams(const char* aCommandName,
                                            nsICommandParams* aParams,
                                            nsISupports* refCon)
{
  if (NS_WARN_IF(!aParams) || NS_WARN_IF(!aCommandName)) {
    return NS_ERROR_INVALID_ARG;
  }

  nsCommandParams* params = aParams->AsCommandParams();

  if (!nsCRT::strcmp(aCommandName, "obs_documentCreated")) {
    uint32_t editorStatus = nsIEditingSession::eEditorErrorUnknown;

    nsCOMPtr<nsIEditingSession> editingSession = do_QueryInterface(refCon);
    if (editingSession) {
      // refCon is initially set to nsIEditingSession until editor
      //  is successfully created and source doc is loaded
      // Embedder gets error status if this fails
      // If called before startup is finished,
      //    status = eEditorCreationInProgress
      nsresult rv = editingSession->GetEditorStatus(&editorStatus);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
    } else {
      // If refCon is an editor, then everything started up OK!
      nsCOMPtr<nsIEditor> editor = do_QueryInterface(refCon);
      if (editor) {
=======
nsresult DocumentStateCommand::GetCommandStateParams(
    Command aCommand, nsCommandParams& aParams, TextEditor* aTextEditor,
    nsIEditingSession* aEditingSession) const {
  switch (aCommand) {
    case Command::EditorObserverDocumentCreated: {
      uint32_t editorStatus = nsIEditingSession::eEditorErrorUnknown;
      if (aEditingSession) {
        // Current context is initially set to nsIEditingSession until editor is
        // successfully created and source doc is loaded.  Embedder gets error
        // status if this fails.  If called before startup is finished,
        // status will be eEditorCreationInProgress.
        nsresult rv = aEditingSession->GetEditorStatus(&editorStatus);
        if (NS_WARN_IF(NS_FAILED(rv))) {
          return rv;
        }
      } else if (aTextEditor) {
        // If current context is an editor, then everything started up OK!
>>>>>>> upstream-releases
        editorStatus = nsIEditingSession::eEditorOK;
      }

      // Note that if refCon is not-null, but is neither
      // an nsIEditingSession or nsIEditor, we return "eEditorErrorUnknown"
      DebugOnly<nsresult> rv = aParams.SetInt(STATE_DATA, editorStatus);
      NS_WARNING_ASSERTION(NS_SUCCEEDED(rv), "Failed to set editor status");
      return NS_OK;
    }
    case Command::EditorObserverDocumentLocationChanged: {
      if (!aTextEditor) {
        return NS_OK;
      }
      Document* document = aTextEditor->GetDocument();
      if (NS_WARN_IF(!document)) {
        return NS_ERROR_FAILURE;
      }
      nsIURI* uri = document->GetDocumentURI();
      if (NS_WARN_IF(!uri)) {
        return NS_ERROR_FAILURE;
      }
      nsresult rv = aParams.SetISupports(STATE_DATA, uri);
      if (NS_WARN_IF(NS_FAILED(rv))) {
        return rv;
      }
      return NS_OK;
    }
    default:
      return NS_ERROR_NOT_IMPLEMENTED;
  }
}

}  // namespace mozilla
