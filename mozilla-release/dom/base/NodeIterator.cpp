/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Implementation of DOM Traversal's NodeIterator
 */

#include "mozilla/dom/NodeIterator.h"

#include "nsError.h"

#include "nsIContent.h"
#include "mozilla/dom/Document.h"
#include "nsContentUtils.h"
#include "nsCOMPtr.h"
#include "mozilla/dom/NodeFilterBinding.h"
#include "mozilla/dom/NodeIteratorBinding.h"

namespace mozilla {
namespace dom {

/*
 * NodePointer implementation
 */
<<<<<<< HEAD
NodeIterator::NodePointer::NodePointer(nsINode *aNode, bool aBeforeNode)
    : mNode(aNode), mBeforeNode(aBeforeNode) {}
||||||| merged common ancestors
NodeIterator::NodePointer::NodePointer(nsINode *aNode, bool aBeforeNode) :
    mNode(aNode),
    mBeforeNode(aBeforeNode)
{
}
=======
NodeIterator::NodePointer::NodePointer(nsINode* aNode, bool aBeforeNode)
    : mNode(aNode), mBeforeNode(aBeforeNode) {}
>>>>>>> upstream-releases

<<<<<<< HEAD
bool NodeIterator::NodePointer::MoveToNext(nsINode *aRoot) {
  if (!mNode) return false;
||||||| merged common ancestors
bool NodeIterator::NodePointer::MoveToNext(nsINode *aRoot)
{
    if (!mNode)
      return false;
=======
bool NodeIterator::NodePointer::MoveToNext(nsINode* aRoot) {
  if (!mNode) return false;
>>>>>>> upstream-releases

  if (mBeforeNode) {
    mBeforeNode = false;
    return true;
  }

<<<<<<< HEAD
  nsINode *child = mNode->GetFirstChild();
  if (child) {
    mNode = child;
    return true;
  }
||||||| merged common ancestors
    nsINode* child = mNode->GetFirstChild();
    if (child) {
        mNode = child;
        return true;
    }
=======
  nsINode* child = mNode->GetFirstChild();
  if (child) {
    mNode = child;
    return true;
  }
>>>>>>> upstream-releases

  return MoveForward(aRoot, mNode);
}

<<<<<<< HEAD
bool NodeIterator::NodePointer::MoveToPrevious(nsINode *aRoot) {
  if (!mNode) return false;
||||||| merged common ancestors
bool NodeIterator::NodePointer::MoveToPrevious(nsINode *aRoot)
{
    if (!mNode)
      return false;
=======
bool NodeIterator::NodePointer::MoveToPrevious(nsINode* aRoot) {
  if (!mNode) return false;
>>>>>>> upstream-releases

  if (!mBeforeNode) {
    mBeforeNode = true;
    return true;
  }

  if (mNode == aRoot) return false;

  MoveBackward(mNode->GetParentNode(), mNode->GetPreviousSibling());

  return true;
}

<<<<<<< HEAD
void NodeIterator::NodePointer::AdjustAfterRemoval(
    nsINode *aRoot, nsINode *aContainer, nsIContent *aChild,
    nsIContent *aPreviousSibling) {
  // If mNode is null or the root there is nothing to do.
  if (!mNode || mNode == aRoot) return;
||||||| merged common ancestors
void NodeIterator::NodePointer::AdjustAfterRemoval(nsINode *aRoot,
                                                   nsINode *aContainer,
                                                   nsIContent *aChild,
                                                   nsIContent *aPreviousSibling)
{
    // If mNode is null or the root there is nothing to do.
    if (!mNode || mNode == aRoot)
        return;
=======
void NodeIterator::NodePointer::AdjustAfterRemoval(
    nsINode* aRoot, nsINode* aContainer, nsIContent* aChild,
    nsIContent* aPreviousSibling) {
  // If mNode is null or the root there is nothing to do.
  if (!mNode || mNode == aRoot) return;
>>>>>>> upstream-releases

  // check if ancestor was removed
  if (!nsContentUtils::ContentIsDescendantOf(mNode, aChild)) return;

<<<<<<< HEAD
  if (mBeforeNode) {
    // Try the next sibling
    nsINode *nextSibling = aPreviousSibling ? aPreviousSibling->GetNextSibling()
                                            : aContainer->GetFirstChild();
||||||| merged common ancestors
    if (mBeforeNode) {
=======
  if (mBeforeNode) {
    // Try the next sibling
    nsINode* nextSibling = aPreviousSibling ? aPreviousSibling->GetNextSibling()
                                            : aContainer->GetFirstChild();
>>>>>>> upstream-releases

    if (nextSibling) {
      mNode = nextSibling;
      return;
    }

    // Next try siblings of ancestors
    if (MoveForward(aRoot, aContainer)) return;

    // No suitable node was found so try going backwards
    mBeforeNode = false;
  }

  MoveBackward(aContainer, aPreviousSibling);
}

<<<<<<< HEAD
bool NodeIterator::NodePointer::MoveForward(nsINode *aRoot, nsINode *aNode) {
  while (1) {
    if (aNode == aRoot) break;

    nsINode *sibling = aNode->GetNextSibling();
    if (sibling) {
      mNode = sibling;
      return true;
||||||| merged common ancestors
bool NodeIterator::NodePointer::MoveForward(nsINode *aRoot, nsINode *aNode)
{
    while (1) {
        if (aNode == aRoot)
            break;

        nsINode *sibling = aNode->GetNextSibling();
        if (sibling) {
            mNode = sibling;
            return true;
        }
        aNode = aNode->GetParentNode();
=======
bool NodeIterator::NodePointer::MoveForward(nsINode* aRoot, nsINode* aNode) {
  while (1) {
    if (aNode == aRoot) break;

    nsINode* sibling = aNode->GetNextSibling();
    if (sibling) {
      mNode = sibling;
      return true;
>>>>>>> upstream-releases
    }
    aNode = aNode->GetParentNode();
  }

  return false;
}

<<<<<<< HEAD
void NodeIterator::NodePointer::MoveBackward(nsINode *aParent, nsINode *aNode) {
  if (aNode) {
    do {
      mNode = aNode;
      aNode = aNode->GetLastChild();
    } while (aNode);
  } else {
    mNode = aParent;
  }
||||||| merged common ancestors
void NodeIterator::NodePointer::MoveBackward(nsINode *aParent, nsINode *aNode)
{
    if (aNode) {
        do {
            mNode = aNode;
            aNode = aNode->GetLastChild();
        } while (aNode);
    } else {
        mNode = aParent;
    }
=======
void NodeIterator::NodePointer::MoveBackward(nsINode* aParent, nsINode* aNode) {
  if (aNode) {
    do {
      mNode = aNode;
      aNode = aNode->GetLastChild();
    } while (aNode);
  } else {
    mNode = aParent;
  }
>>>>>>> upstream-releases
}

/*
 * Factories, constructors and destructors
 */

<<<<<<< HEAD
NodeIterator::NodeIterator(nsINode *aRoot, uint32_t aWhatToShow,
                           NodeFilter *aFilter)
    : nsTraversal(aRoot, aWhatToShow, aFilter), mPointer(mRoot, true) {
  aRoot->AddMutationObserver(this);
||||||| merged common ancestors
NodeIterator::NodeIterator(nsINode *aRoot,
                           uint32_t aWhatToShow,
                           NodeFilter* aFilter) :
    nsTraversal(aRoot, aWhatToShow, aFilter),
    mPointer(mRoot, true)
{
    aRoot->AddMutationObserver(this);
=======
NodeIterator::NodeIterator(nsINode* aRoot, uint32_t aWhatToShow,
                           NodeFilter* aFilter)
    : nsTraversal(aRoot, aWhatToShow, aFilter), mPointer(mRoot, true) {
  aRoot->AddMutationObserver(this);
>>>>>>> upstream-releases
}

NodeIterator::~NodeIterator() {
  /* destructor code */
  if (mRoot) mRoot->RemoveMutationObserver(this);
}

/*
 * nsISupports and cycle collection stuff
 */

NS_IMPL_CYCLE_COLLECTION_CLASS(NodeIterator)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN(NodeIterator)
  if (tmp->mRoot) tmp->mRoot->RemoveMutationObserver(tmp);
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mRoot)
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mFilter)
NS_IMPL_CYCLE_COLLECTION_UNLINK_END
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN(NodeIterator)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mRoot)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mFilter)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

// QueryInterface implementation for NodeIterator
NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(NodeIterator)
  NS_INTERFACE_MAP_ENTRY(nsIMutationObserver)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

NS_IMPL_CYCLE_COLLECTING_ADDREF(NodeIterator)
NS_IMPL_CYCLE_COLLECTING_RELEASE(NodeIterator)

<<<<<<< HEAD
already_AddRefed<nsINode> NodeIterator::NextOrPrevNode(
    NodePointer::MoveToMethodType aMove, ErrorResult &aResult) {
  if (mInAcceptNode) {
    aResult.Throw(NS_ERROR_DOM_INVALID_STATE_ERR);
    return nullptr;
  }

  mWorkingPointer = mPointer;

  struct AutoClear {
    NodePointer *mPtr;
    explicit AutoClear(NodePointer *ptr) : mPtr(ptr) {}
    ~AutoClear() { mPtr->Clear(); }
  } ac(&mWorkingPointer);

  while ((mWorkingPointer.*aMove)(mRoot)) {
    nsCOMPtr<nsINode> testNode = mWorkingPointer.mNode;
    int16_t filtered = TestNode(testNode, aResult);
    if (aResult.Failed()) {
      return nullptr;
||||||| merged common ancestors
already_AddRefed<nsINode>
NodeIterator::NextOrPrevNode(NodePointer::MoveToMethodType aMove,
                             ErrorResult& aResult)
{
    if (mInAcceptNode) {
        aResult.Throw(NS_ERROR_DOM_INVALID_STATE_ERR);
        return nullptr;
=======
already_AddRefed<nsINode> NodeIterator::NextOrPrevNode(
    NodePointer::MoveToMethodType aMove, ErrorResult& aResult) {
  if (mInAcceptNode) {
    aResult.Throw(NS_ERROR_DOM_INVALID_STATE_ERR);
    return nullptr;
  }

  mWorkingPointer = mPointer;

  struct AutoClear {
    NodePointer* mPtr;
    explicit AutoClear(NodePointer* ptr) : mPtr(ptr) {}
    ~AutoClear() { mPtr->Clear(); }
  } ac(&mWorkingPointer);

  while ((mWorkingPointer.*aMove)(mRoot)) {
    nsCOMPtr<nsINode> testNode = mWorkingPointer.mNode;
    int16_t filtered = TestNode(testNode, aResult);
    if (aResult.Failed()) {
      return nullptr;
>>>>>>> upstream-releases
    }

    if (filtered == NodeFilter_Binding::FILTER_ACCEPT) {
      mPointer = mWorkingPointer;
      return testNode.forget();
    }
  }

  return nullptr;
}

<<<<<<< HEAD
void NodeIterator::Detach() {
  if (mRoot) {
    mRoot->OwnerDoc()->WarnOnceAbout(nsIDocument::eNodeIteratorDetach);
  }
||||||| merged common ancestors
void
NodeIterator::Detach()
{
    if (mRoot) {
        mRoot->OwnerDoc()->WarnOnceAbout(nsIDocument::eNodeIteratorDetach);
    }
=======
void NodeIterator::Detach() {
  if (mRoot) {
    mRoot->OwnerDoc()->WarnOnceAbout(Document::eNodeIteratorDetach);
  }
>>>>>>> upstream-releases
}

/*
 * nsIMutationObserver interface
 */

<<<<<<< HEAD
void NodeIterator::ContentRemoved(nsIContent *aChild,
                                  nsIContent *aPreviousSibling) {
  nsINode *container = aChild->GetParentNode();
||||||| merged common ancestors
void NodeIterator::ContentRemoved(nsIContent* aChild,
                                  nsIContent* aPreviousSibling)
{
    nsINode* container = aChild->GetParentNode();
=======
void NodeIterator::ContentRemoved(nsIContent* aChild,
                                  nsIContent* aPreviousSibling) {
  nsINode* container = aChild->GetParentNode();
>>>>>>> upstream-releases

  mPointer.AdjustAfterRemoval(mRoot, container, aChild, aPreviousSibling);
  mWorkingPointer.AdjustAfterRemoval(mRoot, container, aChild,
                                     aPreviousSibling);
}

<<<<<<< HEAD
bool NodeIterator::WrapObject(JSContext *cx, JS::Handle<JSObject *> aGivenProto,
                              JS::MutableHandle<JSObject *> aReflector) {
  return NodeIterator_Binding::Wrap(cx, this, aGivenProto, aReflector);
||||||| merged common ancestors
bool
NodeIterator::WrapObject(JSContext *cx, JS::Handle<JSObject*> aGivenProto, JS::MutableHandle<JSObject*> aReflector)
{
    return NodeIterator_Binding::Wrap(cx, this, aGivenProto, aReflector);
=======
bool NodeIterator::WrapObject(JSContext* cx, JS::Handle<JSObject*> aGivenProto,
                              JS::MutableHandle<JSObject*> aReflector) {
  return NodeIterator_Binding::Wrap(cx, this, aGivenProto, aReflector);
>>>>>>> upstream-releases
}

}  // namespace dom
}  // namespace mozilla
