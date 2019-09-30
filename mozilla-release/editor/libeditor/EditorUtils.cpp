/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/EditorUtils.h"

#include "mozilla/ContentIterator.h"
#include "mozilla/EditorDOMPoint.h"
#include "mozilla/OwningNonNull.h"
#include "mozilla/dom/Selection.h"
#include "nsComponentManagerUtils.h"
#include "nsError.h"
#include "nsIContent.h"
#include "nsIDocShell.h"
#include "mozilla/dom/Document.h"
#include "nsIInterfaceRequestorUtils.h"
#include "nsINode.h"

class nsISupports;
class nsRange;

namespace mozilla {

using namespace dom;

/******************************************************************************
 * some helper classes for iterating the dom tree
 *****************************************************************************/

<<<<<<< HEAD
DOMIterator::DOMIterator(
    nsINode& aNode MOZ_GUARD_OBJECT_NOTIFIER_PARAM_IN_IMPL) {
||||||| merged common ancestors
DOMIterator::DOMIterator(nsINode& aNode MOZ_GUARD_OBJECT_NOTIFIER_PARAM_IN_IMPL)
{
=======
DOMIterator::DOMIterator(nsINode& aNode MOZ_GUARD_OBJECT_NOTIFIER_PARAM_IN_IMPL)
    : mIter(&mPostOrderIter) {
>>>>>>> upstream-releases
  MOZ_GUARD_OBJECT_NOTIFIER_INIT;
  DebugOnly<nsresult> rv = mIter->Init(&aNode);
  MOZ_ASSERT(NS_SUCCEEDED(rv));
}

<<<<<<< HEAD
nsresult DOMIterator::Init(nsRange& aRange) {
  mIter = NS_NewContentIterator();
  return mIter->Init(&aRange);
}
||||||| merged common ancestors
nsresult
DOMIterator::Init(nsRange& aRange)
{
  mIter = NS_NewContentIterator();
  return mIter->Init(&aRange);
}
=======
nsresult DOMIterator::Init(nsRange& aRange) { return mIter->Init(&aRange); }
>>>>>>> upstream-releases

<<<<<<< HEAD
DOMIterator::DOMIterator(MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_IN_IMPL) {
||||||| merged common ancestors
DOMIterator::DOMIterator(MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_IN_IMPL)
{
=======
DOMIterator::DOMIterator(MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_IN_IMPL)
    : mIter(&mPostOrderIter) {
>>>>>>> upstream-releases
  MOZ_GUARD_OBJECT_NOTIFIER_INIT;
}

<<<<<<< HEAD
DOMIterator::~DOMIterator() {}

void DOMIterator::AppendList(
    const BoolDomIterFunctor& functor,
    nsTArray<OwningNonNull<nsINode>>& arrayOfNodes) const {
||||||| merged common ancestors
DOMIterator::~DOMIterator()
{
}

void
DOMIterator::AppendList(const BoolDomIterFunctor& functor,
                        nsTArray<OwningNonNull<nsINode>>& arrayOfNodes) const
{
=======
void DOMIterator::AppendList(
    const BoolDomIterFunctor& functor,
    nsTArray<OwningNonNull<nsINode>>& arrayOfNodes) const {
>>>>>>> upstream-releases
  // Iterate through dom and build list
  for (; !mIter->IsDone(); mIter->Next()) {
    nsCOMPtr<nsINode> node = mIter->GetCurrentNode();

    if (functor(node)) {
      arrayOfNodes.AppendElement(*node);
    }
  }
}

DOMSubtreeIterator::DOMSubtreeIterator(
<<<<<<< HEAD
    MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_IN_IMPL)
    : DOMIterator(MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_TO_PARENT) {}
||||||| merged common ancestors
                      MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_IN_IMPL)
  : DOMIterator(MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_TO_PARENT)
{
}
=======
    MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_IN_IMPL)
    : DOMIterator(MOZ_GUARD_OBJECT_NOTIFIER_ONLY_PARAM_TO_PARENT) {
  mIter = &mSubtreeIter;
}
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult DOMSubtreeIterator::Init(nsRange& aRange) {
  mIter = NS_NewContentSubtreeIterator();
||||||| merged common ancestors
nsresult
DOMSubtreeIterator::Init(nsRange& aRange)
{
  mIter = NS_NewContentSubtreeIterator();
=======
nsresult DOMSubtreeIterator::Init(nsRange& aRange) {
>>>>>>> upstream-releases
  return mIter->Init(&aRange);
}

<<<<<<< HEAD
DOMSubtreeIterator::~DOMSubtreeIterator() {}

||||||| merged common ancestors
DOMSubtreeIterator::~DOMSubtreeIterator()
{
}

=======
>>>>>>> upstream-releases
/******************************************************************************
 * some general purpose editor utils
 *****************************************************************************/

bool EditorUtils::IsDescendantOf(const nsINode& aNode, const nsINode& aParent,
                                 EditorRawDOMPoint* aOutPoint /* = nullptr */) {
  if (aOutPoint) {
    aOutPoint->Clear();
  }

  if (&aNode == &aParent) {
    return false;
  }

  for (const nsINode* node = &aNode; node; node = node->GetParentNode()) {
    if (node->GetParentNode() == &aParent) {
      if (aOutPoint) {
        MOZ_ASSERT(node->IsContent());
        aOutPoint->Set(node->AsContent());
      }
      return true;
    }
  }

  return false;
}

bool EditorUtils::IsDescendantOf(const nsINode& aNode, const nsINode& aParent,
                                 EditorDOMPoint* aOutPoint) {
  MOZ_ASSERT(aOutPoint);
  aOutPoint->Clear();
  if (&aNode == &aParent) {
    return false;
  }

  for (const nsINode* node = &aNode; node; node = node->GetParentNode()) {
    if (node->GetParentNode() == &aParent) {
      MOZ_ASSERT(node->IsContent());
      aOutPoint->Set(node->AsContent());
      return true;
    }
  }

  return false;
}

}  // namespace mozilla
