/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FilteredContentIterator.h"

#include "mozilla/ContentIterator.h"
#include "mozilla/Move.h"
#include "mozilla/mozalloc.h"
#include "mozilla/dom/AbstractRange.h"

#include "nsComponentManagerUtils.h"
#include "nsComposeTxtSrvFilter.h"
#include "nsContentUtils.h"
#include "nsDebug.h"
#include "nsError.h"
#include "nsAtom.h"
#include "nsIContent.h"
#include "nsINode.h"
#include "nsISupportsBase.h"
#include "nsISupportsUtils.h"
#include "nsRange.h"

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
using namespace mozilla;

//------------------------------------------------------------
nsFilteredContentIterator::nsFilteredContentIterator(
    UniquePtr<nsComposeTxtSrvFilter> aFilter)
    : mIterator(NS_NewContentIterator()),
      mPreIterator(NS_NewPreContentIterator()),
      mFilter(std::move(aFilter)),
      mDidSkip(false),
      mIsOutOfRange(false),
      mDirection(eDirNotSet) {}
||||||| merged common ancestors
using namespace mozilla;

//------------------------------------------------------------
nsFilteredContentIterator::nsFilteredContentIterator(UniquePtr<nsComposeTxtSrvFilter> aFilter) :
  mIterator(NS_NewContentIterator()),
  mPreIterator(NS_NewPreContentIterator()),
  mFilter(std::move(aFilter)),
  mDidSkip(false),
  mIsOutOfRange(false),
  mDirection(eDirNotSet)
{
}
=======
namespace mozilla {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
//------------------------------------------------------------
nsFilteredContentIterator::~nsFilteredContentIterator() {}
||||||| merged common ancestors
//------------------------------------------------------------
nsFilteredContentIterator::~nsFilteredContentIterator()
{
}
=======
using namespace dom;
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp

FilteredContentIterator::FilteredContentIterator(
    UniquePtr<nsComposeTxtSrvFilter> aFilter)
    : mCurrentIterator(nullptr),
      mFilter(std::move(aFilter)),
      mDidSkip(false),
      mIsOutOfRange(false),
      mDirection(eDirNotSet) {}

FilteredContentIterator::~FilteredContentIterator() {}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
NS_IMPL_CYCLE_COLLECTION(nsFilteredContentIterator, mCurrentIterator, mIterator,
                         mPreIterator, mRange)
||||||| merged common ancestors
NS_IMPL_CYCLE_COLLECTION(nsFilteredContentIterator,
                         mCurrentIterator,
                         mIterator,
                         mPreIterator,
                         mRange)
=======
NS_IMPL_CYCLE_COLLECTION(FilteredContentIterator, mPostIterator, mPreIterator,
                         mRange)
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
//------------------------------------------------------------
nsresult nsFilteredContentIterator::Init(nsINode* aRoot) {
||||||| merged common ancestors
//------------------------------------------------------------
nsresult
nsFilteredContentIterator::Init(nsINode* aRoot)
{
=======
NS_IMPL_CYCLE_COLLECTION_ROOT_NATIVE(FilteredContentIterator, AddRef)
NS_IMPL_CYCLE_COLLECTION_UNROOT_NATIVE(FilteredContentIterator, Release)

nsresult FilteredContentIterator::Init(nsINode* aRoot) {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  NS_ENSURE_ARG_POINTER(aRoot);
<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
  NS_ENSURE_TRUE(mPreIterator, NS_ERROR_FAILURE);
  NS_ENSURE_TRUE(mIterator, NS_ERROR_FAILURE);
  mIsOutOfRange = false;
  mDirection = eForward;
  mCurrentIterator = mPreIterator;
||||||| merged common ancestors
  NS_ENSURE_TRUE(mPreIterator, NS_ERROR_FAILURE);
  NS_ENSURE_TRUE(mIterator, NS_ERROR_FAILURE);
  mIsOutOfRange    = false;
  mDirection       = eForward;
  mCurrentIterator = mPreIterator;
=======
  mIsOutOfRange = false;
  mDirection = eForward;
  mCurrentIterator = &mPreIterator;
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp

  mRange = new nsRange(aRoot);
  mRange->SelectNode(*aRoot, IgnoreErrors());

  nsresult rv = mPreIterator.Init(mRange);
  NS_ENSURE_SUCCESS(rv, rv);
  return mPostIterator.Init(mRange);
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
//------------------------------------------------------------
nsresult nsFilteredContentIterator::Init(nsRange* aRange) {
  if (NS_WARN_IF(!aRange)) {
||||||| merged common ancestors
//------------------------------------------------------------
nsresult
nsFilteredContentIterator::Init(nsRange* aRange)
{
  if (NS_WARN_IF(!aRange)) {
=======
nsresult FilteredContentIterator::Init(const AbstractRange* aAbstractRange) {
  if (NS_WARN_IF(!aAbstractRange)) {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
    return NS_ERROR_INVALID_ARG;
  }

  if (NS_WARN_IF(!aAbstractRange->IsPositioned())) {
    return NS_ERROR_INVALID_ARG;
  }

  mRange = nsRange::Create(aAbstractRange, IgnoreErrors());
  if (NS_WARN_IF(!mRange)) {
    return NS_ERROR_FAILURE;
  }
  return InitWithRange();
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
//------------------------------------------------------------
nsresult nsFilteredContentIterator::Init(nsINode* aStartContainer,
                                         uint32_t aStartOffset,
                                         nsINode* aEndContainer,
                                         uint32_t aEndOffset) {
||||||| merged common ancestors
//------------------------------------------------------------
nsresult
nsFilteredContentIterator::Init(nsINode* aStartContainer, uint32_t aStartOffset,
                                nsINode* aEndContainer, uint32_t aEndOffset)
{
=======
nsresult FilteredContentIterator::Init(nsINode* aStartContainer,
                                       uint32_t aStartOffset,
                                       nsINode* aEndContainer,
                                       uint32_t aEndOffset) {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  return Init(RawRangeBoundary(aStartContainer, aStartOffset),
              RawRangeBoundary(aEndContainer, aEndOffset));
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
nsresult nsFilteredContentIterator::Init(const RawRangeBoundary& aStart,
                                         const RawRangeBoundary& aEnd) {
  RefPtr<nsRange> range;
  nsresult rv = nsRange::CreateRange(aStart, aEnd, getter_AddRefs(range));
  if (NS_WARN_IF(NS_FAILED(rv)) || NS_WARN_IF(!range) ||
      NS_WARN_IF(!range->IsPositioned())) {
||||||| merged common ancestors
nsresult
nsFilteredContentIterator::Init(const RawRangeBoundary& aStart,
                                const RawRangeBoundary& aEnd)
{
  RefPtr<nsRange> range;
  nsresult rv = nsRange::CreateRange(aStart, aEnd, getter_AddRefs(range));
  if (NS_WARN_IF(NS_FAILED(rv)) || NS_WARN_IF(!range) ||
      NS_WARN_IF(!range->IsPositioned())) {
=======
nsresult FilteredContentIterator::Init(const RawRangeBoundary& aStartBoundary,
                                       const RawRangeBoundary& aEndBoundary) {
  RefPtr<nsRange> range =
      nsRange::Create(aStartBoundary, aEndBoundary, IgnoreErrors());
  if (NS_WARN_IF(!range) || NS_WARN_IF(!range->IsPositioned())) {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
    return NS_ERROR_INVALID_ARG;
  }

  MOZ_ASSERT(range->StartRef() == aStartBoundary);
  MOZ_ASSERT(range->EndRef() == aEndBoundary);

  mRange = std::move(range);

  return InitWithRange();
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
nsresult nsFilteredContentIterator::InitWithRange() {
||||||| merged common ancestors
nsresult
nsFilteredContentIterator::InitWithRange()
{
=======
nsresult FilteredContentIterator::InitWithRange() {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  MOZ_ASSERT(mRange);
  MOZ_ASSERT(mRange->IsPositioned());

  mIsOutOfRange = false;
  mDirection = eForward;
  mCurrentIterator = &mPreIterator;

  nsresult rv = mPreIterator.Init(mRange);
  if (NS_WARN_IF(NS_FAILED(rv))) {
    return rv;
  }
  return mPostIterator.Init(mRange);
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
//------------------------------------------------------------
nsresult nsFilteredContentIterator::SwitchDirections(bool aChangeToForward) {
  nsINode* node = mCurrentIterator->GetCurrentNode();
||||||| merged common ancestors
//------------------------------------------------------------
nsresult
nsFilteredContentIterator::SwitchDirections(bool aChangeToForward)
{
  nsINode *node = mCurrentIterator->GetCurrentNode();
=======
nsresult FilteredContentIterator::SwitchDirections(bool aChangeToForward) {
  nsINode* node = mCurrentIterator->GetCurrentNode();
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp

  if (aChangeToForward) {
<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
    mCurrentIterator = mPreIterator;
    mDirection = eForward;
||||||| merged common ancestors
    mCurrentIterator = mPreIterator;
    mDirection       = eForward;
=======
    mCurrentIterator = &mPreIterator;
    mDirection = eForward;
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  } else {
<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
    mCurrentIterator = mIterator;
    mDirection = eBackward;
||||||| merged common ancestors
    mCurrentIterator = mIterator;
    mDirection       = eBackward;
=======
    mCurrentIterator = &mPostIterator;
    mDirection = eBackward;
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  }

  if (node) {
    nsresult rv = mCurrentIterator->PositionAt(node);
    if (NS_FAILED(rv)) {
      mIsOutOfRange = true;
      return rv;
    }
  }
  return NS_OK;
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
//------------------------------------------------------------
void nsFilteredContentIterator::First() {
||||||| merged common ancestors
//------------------------------------------------------------
void
nsFilteredContentIterator::First()
{
=======
void FilteredContentIterator::First() {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  if (!mCurrentIterator) {
    NS_ERROR("Missing iterator!");

    return;
  }

  // If we are switching directions then
  // we need to switch how we process the nodes
  if (mDirection != eForward) {
<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
    mCurrentIterator = mPreIterator;
    mDirection = eForward;
    mIsOutOfRange = false;
||||||| merged common ancestors
    mCurrentIterator = mPreIterator;
    mDirection       = eForward;
    mIsOutOfRange    = false;
=======
    mCurrentIterator = &mPreIterator;
    mDirection = eForward;
    mIsOutOfRange = false;
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  }

  mCurrentIterator->First();

  if (mCurrentIterator->IsDone()) {
    return;
  }

  nsINode* currentNode = mCurrentIterator->GetCurrentNode();

  bool didCross;
  CheckAdvNode(currentNode, didCross, eForward);
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
//------------------------------------------------------------
void nsFilteredContentIterator::Last() {
||||||| merged common ancestors
//------------------------------------------------------------
void
nsFilteredContentIterator::Last()
{
=======
void FilteredContentIterator::Last() {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  if (!mCurrentIterator) {
    NS_ERROR("Missing iterator!");

    return;
  }

  // If we are switching directions then
  // we need to switch how we process the nodes
  if (mDirection != eBackward) {
<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
    mCurrentIterator = mIterator;
    mDirection = eBackward;
    mIsOutOfRange = false;
||||||| merged common ancestors
    mCurrentIterator = mIterator;
    mDirection       = eBackward;
    mIsOutOfRange    = false;
=======
    mCurrentIterator = &mPostIterator;
    mDirection = eBackward;
    mIsOutOfRange = false;
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  }

  mCurrentIterator->Last();

  if (mCurrentIterator->IsDone()) {
    return;
  }

  nsINode* currentNode = mCurrentIterator->GetCurrentNode();

  bool didCross;
  CheckAdvNode(currentNode, didCross, eBackward);
}

///////////////////////////////////////////////////////////////////////////
// ContentToParentOffset: returns the content node's parent and offset.
//
static void ContentToParentOffset(nsIContent* aContent, nsIContent** aParent,
                                  int32_t* aOffset) {
  if (!aParent || !aOffset) return;

  *aParent = nullptr;
  *aOffset = 0;

  if (!aContent) return;

  nsCOMPtr<nsIContent> parent = aContent->GetParent();
  if (!parent) return;

  *aOffset = parent->ComputeIndexOf(aContent);
  parent.forget(aParent);
}

///////////////////////////////////////////////////////////////////////////
// ContentIsInTraversalRange: returns true if content is visited during
// the traversal of the range in the specified mode.
//
static bool ContentIsInTraversalRange(nsIContent* aContent, bool aIsPreMode,
                                      nsINode* aStartContainer,
                                      int32_t aStartOffset,
                                      nsINode* aEndContainer,
                                      int32_t aEndOffset) {
  NS_ENSURE_TRUE(aStartContainer && aEndContainer && aContent, false);

  nsCOMPtr<nsIContent> parentNode;
  int32_t indx = 0;

  ContentToParentOffset(aContent, getter_AddRefs(parentNode), &indx);

  NS_ENSURE_TRUE(parentNode, false);

  if (!aIsPreMode) ++indx;

  int32_t startRes = nsContentUtils::ComparePoints(
      aStartContainer, aStartOffset, parentNode, indx);
  int32_t endRes = nsContentUtils::ComparePoints(aEndContainer, aEndOffset,
                                                 parentNode, indx);
  return (startRes <= 0) && (endRes >= 0);
}

static bool ContentIsInTraversalRange(nsRange* aRange, nsIContent* aNextContent,
                                      bool aIsPreMode) {
  // XXXbz we have a caller below (in AdvanceNode) who passes null for
  // aNextContent!
  NS_ENSURE_TRUE(aNextContent && aRange, false);

  return ContentIsInTraversalRange(
      aNextContent, aIsPreMode, aRange->GetStartContainer(),
      static_cast<int32_t>(aRange->StartOffset()), aRange->GetEndContainer(),
      static_cast<int32_t>(aRange->EndOffset()));
}

// Helper function to advance to the next or previous node
<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
nsresult nsFilteredContentIterator::AdvanceNode(nsINode* aNode,
                                                nsINode*& aNewNode,
                                                eDirectionType aDir) {
||||||| merged common ancestors
nsresult
nsFilteredContentIterator::AdvanceNode(nsINode* aNode, nsINode*& aNewNode, eDirectionType aDir)
{
=======
nsresult FilteredContentIterator::AdvanceNode(nsINode* aNode,
                                              nsINode*& aNewNode,
                                              eDirectionType aDir) {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  nsCOMPtr<nsIContent> nextNode;
  if (aDir == eForward) {
    nextNode = aNode->GetNextSibling();
  } else {
    nextNode = aNode->GetPreviousSibling();
  }

  if (nextNode) {
    // If we got here, that means we found the nxt/prv node
    // make sure it is in our DOMRange
    bool intersects =
        ContentIsInTraversalRange(mRange, nextNode, aDir == eForward);
    if (intersects) {
      aNewNode = nextNode;
      NS_ADDREF(aNewNode);
      return NS_OK;
    }
  } else {
    // The next node was null so we need to walk up the parent(s)
    nsCOMPtr<nsINode> parent = aNode->GetParentNode();
    NS_ASSERTION(parent, "parent can't be nullptr");

    // Make sure the parent is in the DOMRange before going further
    // XXXbz why are we passing nextNode, not the parent???  If this gets fixed,
    // then ContentIsInTraversalRange can stop null-checking its second arg.
    bool intersects =
        ContentIsInTraversalRange(mRange, nextNode, aDir == eForward);
    if (intersects) {
      // Now find the nxt/prv node after/before this node
      nsresult rv = AdvanceNode(parent, aNewNode, aDir);
      if (NS_SUCCEEDED(rv) && aNewNode) {
        return NS_OK;
      }
    }
  }

  // if we get here it pretty much means
  // we went out of the DOM Range
  mIsOutOfRange = true;

  return NS_ERROR_FAILURE;
}

// Helper function to see if the next/prev node should be skipped
<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
void nsFilteredContentIterator::CheckAdvNode(nsINode* aNode, bool& aDidSkip,
                                             eDirectionType aDir) {
  aDidSkip = false;
||||||| merged common ancestors
void
nsFilteredContentIterator::CheckAdvNode(nsINode* aNode, bool& aDidSkip, eDirectionType aDir)
{
  aDidSkip      = false;
=======
void FilteredContentIterator::CheckAdvNode(nsINode* aNode, bool& aDidSkip,
                                           eDirectionType aDir) {
  aDidSkip = false;
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  mIsOutOfRange = false;

  if (aNode && mFilter) {
    nsCOMPtr<nsINode> currentNode = aNode;
    while (1) {
      if (mFilter->Skip(aNode)) {
        aDidSkip = true;
        // Get the next/prev node and then
        // see if we should skip that
        nsCOMPtr<nsINode> advNode;
        nsresult rv = AdvanceNode(aNode, *getter_AddRefs(advNode), aDir);
        if (NS_SUCCEEDED(rv) && advNode) {
          aNode = advNode;
        } else {
          return;  // fell out of range
        }
      } else {
        if (aNode != currentNode) {
          nsCOMPtr<nsIContent> content(do_QueryInterface(aNode));
          mCurrentIterator->PositionAt(content);
        }
        return;  // found something
      }
    }
  }
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
void nsFilteredContentIterator::Next() {
||||||| merged common ancestors
void
nsFilteredContentIterator::Next()
{
=======
void FilteredContentIterator::Next() {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  if (mIsOutOfRange || !mCurrentIterator) {
    NS_ASSERTION(mCurrentIterator, "Missing iterator!");

    return;
  }

  // If we are switching directions then
  // we need to switch how we process the nodes
  if (mDirection != eForward) {
    nsresult rv = SwitchDirections(true);
    if (NS_FAILED(rv)) {
      return;
    }
  }

  mCurrentIterator->Next();

  if (mCurrentIterator->IsDone()) {
    return;
  }

  // If we can't get the current node then
  // don't check to see if we can skip it
  nsINode* currentNode = mCurrentIterator->GetCurrentNode();

  CheckAdvNode(currentNode, mDidSkip, eForward);
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
void nsFilteredContentIterator::Prev() {
||||||| merged common ancestors
void
nsFilteredContentIterator::Prev()
{
=======
void FilteredContentIterator::Prev() {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  if (mIsOutOfRange || !mCurrentIterator) {
    NS_ASSERTION(mCurrentIterator, "Missing iterator!");

    return;
  }

  // If we are switching directions then
  // we need to switch how we process the nodes
  if (mDirection != eBackward) {
    nsresult rv = SwitchDirections(false);
    if (NS_FAILED(rv)) {
      return;
    }
  }

  mCurrentIterator->Prev();

  if (mCurrentIterator->IsDone()) {
    return;
  }

  // If we can't get the current node then
  // don't check to see if we can skip it
  nsINode* currentNode = mCurrentIterator->GetCurrentNode();

  CheckAdvNode(currentNode, mDidSkip, eBackward);
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
nsINode* nsFilteredContentIterator::GetCurrentNode() {
||||||| merged common ancestors
nsINode *
nsFilteredContentIterator::GetCurrentNode()
{
=======
nsINode* FilteredContentIterator::GetCurrentNode() {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  if (mIsOutOfRange || !mCurrentIterator) {
    return nullptr;
  }

  return mCurrentIterator->GetCurrentNode();
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
bool nsFilteredContentIterator::IsDone() {
||||||| merged common ancestors
bool
nsFilteredContentIterator::IsDone()
{
=======
bool FilteredContentIterator::IsDone() {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  if (mIsOutOfRange || !mCurrentIterator) {
    return true;
  }

  return mCurrentIterator->IsDone();
}

<<<<<<< HEAD:mozilla-release/editor/spellchecker/nsFilteredContentIterator.cpp
nsresult nsFilteredContentIterator::PositionAt(nsINode* aCurNode) {
||||||| merged common ancestors
nsresult
nsFilteredContentIterator::PositionAt(nsINode* aCurNode)
{
=======
nsresult FilteredContentIterator::PositionAt(nsINode* aCurNode) {
>>>>>>> upstream-releases:mozilla-release/editor/spellchecker/FilteredContentIterator.cpp
  NS_ENSURE_TRUE(mCurrentIterator, NS_ERROR_FAILURE);
  mIsOutOfRange = false;
  return mCurrentIterator->PositionAt(aCurNode);
}

}  // namespace mozilla
