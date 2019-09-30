/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsString.h"
#include "nsIComponentManager.h"
#include "nsIWeakReferenceUtils.h"
#include "nsBaseCommandController.h"
#include "nsControllerCommandTable.h"

NS_IMPL_ADDREF(nsBaseCommandController)
NS_IMPL_RELEASE(nsBaseCommandController)

NS_INTERFACE_MAP_BEGIN(nsBaseCommandController)
  NS_INTERFACE_MAP_ENTRY(nsIController)
  NS_INTERFACE_MAP_ENTRY(nsICommandController)
  NS_INTERFACE_MAP_ENTRY(nsIControllerContext)
  NS_INTERFACE_MAP_ENTRY(nsIInterfaceRequestor)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIControllerContext)
NS_INTERFACE_MAP_END

<<<<<<< HEAD
nsBaseCommandController::nsBaseCommandController()
    : mCommandContextRawPtr(nullptr) {}
||||||| merged common ancestors
nsBaseCommandController::nsBaseCommandController()
  : mCommandContextRawPtr(nullptr)
{
}
=======
nsBaseCommandController::nsBaseCommandController(
    nsControllerCommandTable* aControllerCommandTable)
    : mCommandContextRawPtr(nullptr), mCommandTable(aControllerCommandTable) {}
>>>>>>> upstream-releases

nsBaseCommandController::~nsBaseCommandController() {}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseCommandController::Init(nsIControllerCommandTable* aCommandTable) {
  if (aCommandTable) {
    mCommandTable = aCommandTable;
  } else {
    mCommandTable = new nsControllerCommandTable();
  }

  return NS_OK;
}

NS_IMETHODIMP
nsBaseCommandController::SetCommandContext(nsISupports* aCommandContext) {
||||||| merged common ancestors
nsBaseCommandController::Init(nsIControllerCommandTable* aCommandTable)
{
  if (aCommandTable) {
    mCommandTable = aCommandTable;
  } else {
    mCommandTable = new nsControllerCommandTable();
  }

  return NS_OK;
}

NS_IMETHODIMP
nsBaseCommandController::SetCommandContext(nsISupports* aCommandContext)
{
=======
nsBaseCommandController::SetCommandContext(nsISupports* aCommandContext) {
>>>>>>> upstream-releases
  mCommandContextWeakPtr = nullptr;
  mCommandContextRawPtr = nullptr;

  if (aCommandContext) {
    nsCOMPtr<nsISupportsWeakReference> weak =
        do_QueryInterface(aCommandContext);
    if (weak) {
      nsresult rv =
          weak->GetWeakReference(getter_AddRefs(mCommandContextWeakPtr));
      NS_ENSURE_SUCCESS(rv, rv);
    } else {
      mCommandContextRawPtr = aCommandContext;
    }
  }

  return NS_OK;
}

NS_IMETHODIMP
nsBaseCommandController::GetInterface(const nsIID& aIID, void** aResult) {
  NS_ENSURE_ARG_POINTER(aResult);

  if (NS_SUCCEEDED(QueryInterface(aIID, aResult))) {
    return NS_OK;
  }

  if (aIID.Equals(NS_GET_IID(nsIControllerCommandTable))) {
    if (mCommandTable) {
      *aResult =
          do_AddRef(static_cast<nsIControllerCommandTable*>(mCommandTable))
              .take();
      return NS_OK;
    }
    return NS_ERROR_NOT_INITIALIZED;
  }

  return NS_NOINTERFACE;
}

/* =======================================================================
 * nsIController
 * ======================================================================= */

NS_IMETHODIMP
nsBaseCommandController::IsCommandEnabled(const char* aCommand, bool* aResult) {
  NS_ENSURE_ARG_POINTER(aCommand);
  NS_ENSURE_ARG_POINTER(aResult);
  NS_ENSURE_STATE(mCommandTable);

  nsISupports* context = mCommandContextRawPtr;
  nsCOMPtr<nsISupports> weak;
  if (!context) {
    weak = do_QueryReferent(mCommandContextWeakPtr);
    context = weak;
  }
  return mCommandTable->IsCommandEnabled(aCommand, context, aResult);
}

NS_IMETHODIMP
nsBaseCommandController::SupportsCommand(const char* aCommand, bool* aResult) {
  NS_ENSURE_ARG_POINTER(aCommand);
  NS_ENSURE_ARG_POINTER(aResult);
  NS_ENSURE_STATE(mCommandTable);

  nsISupports* context = mCommandContextRawPtr;
  nsCOMPtr<nsISupports> weak;
  if (!context) {
    weak = do_QueryReferent(mCommandContextWeakPtr);
    context = weak;
  }
  return mCommandTable->SupportsCommand(aCommand, context, aResult);
}

NS_IMETHODIMP
nsBaseCommandController::DoCommand(const char* aCommand) {
  NS_ENSURE_ARG_POINTER(aCommand);
  NS_ENSURE_STATE(mCommandTable);

  nsCOMPtr<nsISupports> context = mCommandContextRawPtr;
  if (!context) {
    context = do_QueryReferent(mCommandContextWeakPtr);
  }
  RefPtr<nsControllerCommandTable> commandTable(mCommandTable);
  return commandTable->DoCommand(aCommand, context);
}

NS_IMETHODIMP
nsBaseCommandController::DoCommandWithParams(const char* aCommand,
                                             nsICommandParams* aParams) {
  NS_ENSURE_ARG_POINTER(aCommand);
  NS_ENSURE_STATE(mCommandTable);

  nsCOMPtr<nsISupports> context = mCommandContextRawPtr;
  if (!context) {
    context = do_QueryReferent(mCommandContextWeakPtr);
  }
  RefPtr<nsControllerCommandTable> commandTable(mCommandTable);
  return commandTable->DoCommandParams(aCommand, aParams, context);
}

NS_IMETHODIMP
nsBaseCommandController::GetCommandStateWithParams(const char* aCommand,
                                                   nsICommandParams* aParams) {
  NS_ENSURE_ARG_POINTER(aCommand);
  NS_ENSURE_STATE(mCommandTable);

  nsISupports* context = mCommandContextRawPtr;
  nsCOMPtr<nsISupports> weak;
  if (!context) {
    weak = do_QueryReferent(mCommandContextWeakPtr);
    context = weak;
  }
  return mCommandTable->GetCommandState(aCommand, aParams, context);
}

NS_IMETHODIMP
nsBaseCommandController::OnEvent(const char* aEventName) {
  NS_ENSURE_ARG_POINTER(aEventName);
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsBaseCommandController::GetSupportedCommands(uint32_t* aCount,
                                              char*** aCommands) {
||||||| merged common ancestors
nsBaseCommandController::GetSupportedCommands(uint32_t* aCount,
                                              char*** aCommands)
{
=======
nsBaseCommandController::GetSupportedCommands(nsTArray<nsCString>& aCommands) {
>>>>>>> upstream-releases
  NS_ENSURE_STATE(mCommandTable);
  return mCommandTable->GetSupportedCommands(aCommands);
}

<<<<<<< HEAD
typedef already_AddRefed<nsIControllerCommandTable> (*CommandTableCreatorFn)();

static already_AddRefed<nsIController>
CreateControllerWithSingletonCommandTable(CommandTableCreatorFn aCreatorFn) {
  nsCOMPtr<nsIController> controller = new nsBaseCommandController();
||||||| merged common ancestors
typedef already_AddRefed<nsIControllerCommandTable> (*CommandTableCreatorFn)();

static already_AddRefed<nsIController>
CreateControllerWithSingletonCommandTable(CommandTableCreatorFn aCreatorFn)
{
  nsCOMPtr<nsIController> controller = new nsBaseCommandController();
=======
typedef already_AddRefed<nsControllerCommandTable> (*CommandTableCreatorFn)();
>>>>>>> upstream-releases

static already_AddRefed<nsBaseCommandController>
CreateControllerWithSingletonCommandTable(CommandTableCreatorFn aCreatorFn) {
  RefPtr<nsControllerCommandTable> commandTable = aCreatorFn();
  if (!commandTable) {
    return nullptr;
  }

  // this is a singleton; make it immutable
  commandTable->MakeImmutable();

<<<<<<< HEAD
  nsresult rv;
  nsCOMPtr<nsIControllerContext> controllerContext =
      do_QueryInterface(controller, &rv);
  if (NS_FAILED(rv)) return nullptr;

  rv = controllerContext->Init(commandTable);
  if (NS_FAILED(rv)) return nullptr;

  return controller.forget();
||||||| merged common ancestors
  nsresult rv;
  nsCOMPtr<nsIControllerContext> controllerContext = do_QueryInterface(controller, &rv);
  if (NS_FAILED(rv)) return nullptr;

  rv = controllerContext->Init(commandTable);
  if (NS_FAILED(rv)) return nullptr;

  return controller.forget();
=======
  RefPtr<nsBaseCommandController> commandController =
      new nsBaseCommandController(commandTable);
  return commandController.forget();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
already_AddRefed<nsIController>
nsBaseCommandController::CreateWindowController() {
||||||| merged common ancestors
already_AddRefed<nsIController>
nsBaseCommandController::CreateWindowController()
{
=======
already_AddRefed<nsBaseCommandController>
nsBaseCommandController::CreateWindowController() {
>>>>>>> upstream-releases
  return CreateControllerWithSingletonCommandTable(
      nsControllerCommandTable::CreateWindowCommandTable);
}

<<<<<<< HEAD
already_AddRefed<nsIController>
nsBaseCommandController::CreateEditorController() {
||||||| merged common ancestors
already_AddRefed<nsIController>
nsBaseCommandController::CreateEditorController()
{
=======
already_AddRefed<nsBaseCommandController>
nsBaseCommandController::CreateEditorController() {
>>>>>>> upstream-releases
  return CreateControllerWithSingletonCommandTable(
      nsControllerCommandTable::CreateEditorCommandTable);
}

<<<<<<< HEAD
already_AddRefed<nsIController>
nsBaseCommandController::CreateEditingController() {
||||||| merged common ancestors
already_AddRefed<nsIController>
nsBaseCommandController::CreateEditingController()
{
=======
already_AddRefed<nsBaseCommandController>
nsBaseCommandController::CreateEditingController() {
>>>>>>> upstream-releases
  return CreateControllerWithSingletonCommandTable(
      nsControllerCommandTable::CreateEditingCommandTable);
}

<<<<<<< HEAD
already_AddRefed<nsIController>
nsBaseCommandController::CreateHTMLEditorController() {
||||||| merged common ancestors
already_AddRefed<nsIController>
nsBaseCommandController::CreateHTMLEditorController()
{
=======
already_AddRefed<nsBaseCommandController>
nsBaseCommandController::CreateHTMLEditorController() {
>>>>>>> upstream-releases
  return CreateControllerWithSingletonCommandTable(
      nsControllerCommandTable::CreateHTMLEditorCommandTable);
}

<<<<<<< HEAD
already_AddRefed<nsIController>
nsBaseCommandController::CreateHTMLEditorDocStateController() {
||||||| merged common ancestors
already_AddRefed<nsIController>
nsBaseCommandController::CreateHTMLEditorDocStateController()
{
=======
already_AddRefed<nsBaseCommandController>
nsBaseCommandController::CreateHTMLEditorDocStateController() {
>>>>>>> upstream-releases
  return CreateControllerWithSingletonCommandTable(
      nsControllerCommandTable::CreateHTMLEditorDocStateCommandTable);
}
