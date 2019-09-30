/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "InputAPZContext.h"

namespace mozilla {
namespace layers {

ScrollableLayerGuid InputAPZContext::sGuid;
uint64_t InputAPZContext::sBlockId = 0;
nsEventStatus InputAPZContext::sApzResponse = nsEventStatus_eIgnore;
bool InputAPZContext::sPendingLayerization = false;
bool InputAPZContext::sRoutedToChildProcess = false;

<<<<<<< HEAD
/*static*/ ScrollableLayerGuid InputAPZContext::GetTargetLayerGuid() {
  return sGuid;
}
||||||| merged common ancestors
/*static*/ ScrollableLayerGuid
InputAPZContext::GetTargetLayerGuid()
{
  return sGuid;
}
=======
/*static*/
ScrollableLayerGuid InputAPZContext::GetTargetLayerGuid() { return sGuid; }
>>>>>>> upstream-releases

<<<<<<< HEAD
/*static*/ uint64_t InputAPZContext::GetInputBlockId() { return sBlockId; }
||||||| merged common ancestors
/*static*/ uint64_t
InputAPZContext::GetInputBlockId()
{
  return sBlockId;
}
=======
/*static*/
uint64_t InputAPZContext::GetInputBlockId() { return sBlockId; }
>>>>>>> upstream-releases

<<<<<<< HEAD
/*static*/ nsEventStatus InputAPZContext::GetApzResponse() {
  return sApzResponse;
}
||||||| merged common ancestors
/*static*/ nsEventStatus
InputAPZContext::GetApzResponse()
{
  return sApzResponse;
}
=======
/*static*/
nsEventStatus InputAPZContext::GetApzResponse() { return sApzResponse; }
>>>>>>> upstream-releases

<<<<<<< HEAD
/*static*/ bool InputAPZContext::HavePendingLayerization() {
  return sPendingLayerization;
}
||||||| merged common ancestors
/*static*/ bool
InputAPZContext::HavePendingLayerization()
{
  return sPendingLayerization;
}
=======
/*static*/
bool InputAPZContext::HavePendingLayerization() { return sPendingLayerization; }
>>>>>>> upstream-releases

<<<<<<< HEAD
/*static*/ bool InputAPZContext::WasRoutedToChildProcess() {
||||||| merged common ancestors
/*static*/ bool
InputAPZContext::WasRoutedToChildProcess()
{
=======
/*static*/
bool InputAPZContext::WasRoutedToChildProcess() {
>>>>>>> upstream-releases
  return sRoutedToChildProcess;
}

InputAPZContext::InputAPZContext(const ScrollableLayerGuid& aGuid,
                                 const uint64_t& aBlockId,
                                 const nsEventStatus& aApzResponse,
                                 bool aPendingLayerization)
    : mOldGuid(sGuid),
      mOldBlockId(sBlockId),
      mOldApzResponse(sApzResponse),
      mOldPendingLayerization(sPendingLayerization),
      mOldRoutedToChildProcess(sRoutedToChildProcess) {
  sGuid = aGuid;
  sBlockId = aBlockId;
  sApzResponse = aApzResponse;
  sPendingLayerization = aPendingLayerization;
  sRoutedToChildProcess = false;
}

InputAPZContext::~InputAPZContext() {
  sGuid = mOldGuid;
  sBlockId = mOldBlockId;
  sApzResponse = mOldApzResponse;
  sPendingLayerization = mOldPendingLayerization;
  sRoutedToChildProcess = mOldRoutedToChildProcess;
}

<<<<<<< HEAD
/*static*/ void InputAPZContext::SetRoutedToChildProcess() {
||||||| merged common ancestors
/*static*/ void
InputAPZContext::SetRoutedToChildProcess()
{
=======
/*static*/
void InputAPZContext::SetRoutedToChildProcess() {
>>>>>>> upstream-releases
  sRoutedToChildProcess = true;
}

}  // namespace layers
}  // namespace mozilla
