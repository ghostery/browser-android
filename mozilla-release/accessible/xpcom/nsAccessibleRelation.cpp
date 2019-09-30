/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsAccessibleRelation.h"

#include "Relation.h"
#include "Accessible.h"
#include "xpcAccessibleDocument.h"

#include "nsArrayUtils.h"
#include "nsComponentManagerUtils.h"

using namespace mozilla::a11y;

<<<<<<< HEAD
nsAccessibleRelation::nsAccessibleRelation(uint32_t aType, Relation *aRel)
    : mType(aType) {
||||||| merged common ancestors
nsAccessibleRelation::nsAccessibleRelation(uint32_t aType,
                                           Relation* aRel) :
  mType(aType)
{
=======
nsAccessibleRelation::nsAccessibleRelation(uint32_t aType, Relation* aRel)
    : mType(aType) {
>>>>>>> upstream-releases
  mTargets = do_CreateInstance(NS_ARRAY_CONTRACTID);
  Accessible *targetAcc = nullptr;
  while ((targetAcc = aRel->Next()))
    mTargets->AppendElement(static_cast<nsIAccessible *>(ToXPC(targetAcc)));
}

<<<<<<< HEAD
nsAccessibleRelation::nsAccessibleRelation(
    uint32_t aType, const nsTArray<ProxyAccessible *> *aTargets)
    : mType(aType) {
||||||| merged common ancestors
nsAccessibleRelation::nsAccessibleRelation(uint32_t aType,
                                           const nsTArray<ProxyAccessible*>* aTargets) :
  mType(aType)
{
=======
nsAccessibleRelation::nsAccessibleRelation(
    uint32_t aType, const nsTArray<ProxyAccessible*>* aTargets)
    : mType(aType) {
>>>>>>> upstream-releases
  mTargets = do_CreateInstance(NS_ARRAY_CONTRACTID);
  for (uint32_t idx = 0; idx < aTargets->Length(); ++idx) {
    mTargets->AppendElement(
<<<<<<< HEAD
        static_cast<nsIAccessible *>(ToXPC(aTargets->ElementAt(idx))));
||||||| merged common ancestors
      static_cast<nsIAccessible*>(ToXPC(aTargets->ElementAt(idx))));
=======
        static_cast<nsIAccessible*>(ToXPC(aTargets->ElementAt(idx))));
>>>>>>> upstream-releases
  }
}

nsAccessibleRelation::~nsAccessibleRelation() {}

// nsISupports
NS_IMPL_ISUPPORTS(nsAccessibleRelation, nsIAccessibleRelation)

// nsIAccessibleRelation
NS_IMETHODIMP
<<<<<<< HEAD
nsAccessibleRelation::GetRelationType(uint32_t *aType) {
||||||| merged common ancestors
nsAccessibleRelation::GetRelationType(uint32_t *aType)
{
=======
nsAccessibleRelation::GetRelationType(uint32_t* aType) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aType);
  *aType = mType;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsAccessibleRelation::GetTargetsCount(uint32_t *aCount) {
||||||| merged common ancestors
nsAccessibleRelation::GetTargetsCount(uint32_t *aCount)
{
=======
nsAccessibleRelation::GetTargetsCount(uint32_t* aCount) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aCount);
  *aCount = 0;
  return mTargets->GetLength(aCount);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsAccessibleRelation::GetTarget(uint32_t aIndex, nsIAccessible **aTarget) {
||||||| merged common ancestors
nsAccessibleRelation::GetTarget(uint32_t aIndex, nsIAccessible **aTarget)
{
=======
nsAccessibleRelation::GetTarget(uint32_t aIndex, nsIAccessible** aTarget) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aTarget);
  nsresult rv = NS_OK;
  nsCOMPtr<nsIAccessible> target = do_QueryElementAt(mTargets, aIndex, &rv);
  target.forget(aTarget);
  return rv;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsAccessibleRelation::GetTargets(nsIArray **aTargets) {
||||||| merged common ancestors
nsAccessibleRelation::GetTargets(nsIArray **aTargets)
{
=======
nsAccessibleRelation::GetTargets(nsIArray** aTargets) {
>>>>>>> upstream-releases
  NS_ENSURE_ARG_POINTER(aTargets);
  NS_ADDREF(*aTargets = mTargets);
  return NS_OK;
}
