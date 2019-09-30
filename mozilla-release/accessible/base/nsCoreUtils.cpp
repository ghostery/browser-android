/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsCoreUtils.h"

#include "nsIAccessibleTypes.h"

#include "nsIBaseWindow.h"
#include "nsIDocShellTreeOwner.h"
#include "mozilla/dom/Document.h"
#include "nsRange.h"
#include "nsXULElement.h"
#include "nsIDocShell.h"
#include "nsIObserverService.h"
#include "nsPresContext.h"
#include "nsIScrollableFrame.h"
#include "nsISelectionController.h"
#include "nsISimpleEnumerator.h"
#include "mozilla/dom/TouchEvent.h"
#include "mozilla/ErrorResult.h"
#include "mozilla/EventListenerManager.h"
#include "mozilla/EventStateManager.h"
#include "mozilla/MouseEvents.h"
#include "mozilla/PresShell.h"
#include "mozilla/TouchEvents.h"
#include "nsView.h"
#include "nsGkAtoms.h"

#include "nsComponentManagerUtils.h"

#include "XULTreeElement.h"
#include "nsTreeColumns.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/HTMLLabelElement.h"
#include "mozilla/dom/MouseEventBinding.h"
#include "mozilla/dom/Selection.h"

using namespace mozilla;

////////////////////////////////////////////////////////////////////////////////
// nsCoreUtils
////////////////////////////////////////////////////////////////////////////////

<<<<<<< HEAD
bool nsCoreUtils::IsLabelWithControl(nsIContent *aContent) {
  dom::HTMLLabelElement *label = dom::HTMLLabelElement::FromNode(aContent);
  if (label && label->GetControl()) return true;
||||||| merged common ancestors
bool
nsCoreUtils::IsLabelWithControl(nsIContent* aContent)
{
  dom::HTMLLabelElement* label = dom::HTMLLabelElement::FromNode(aContent);
  if (label && label->GetControl())
    return true;
=======
bool nsCoreUtils::IsLabelWithControl(nsIContent* aContent) {
  dom::HTMLLabelElement* label = dom::HTMLLabelElement::FromNode(aContent);
  if (label && label->GetControl()) return true;
>>>>>>> upstream-releases

  return false;
}

<<<<<<< HEAD
bool nsCoreUtils::HasClickListener(nsIContent *aContent) {
||||||| merged common ancestors
bool
nsCoreUtils::HasClickListener(nsIContent *aContent)
{
=======
bool nsCoreUtils::HasClickListener(nsIContent* aContent) {
>>>>>>> upstream-releases
  NS_ENSURE_TRUE(aContent, false);
<<<<<<< HEAD
  EventListenerManager *listenerManager =
      aContent->GetExistingListenerManager();
||||||| merged common ancestors
  EventListenerManager* listenerManager =
    aContent->GetExistingListenerManager();
=======
  EventListenerManager* listenerManager =
      aContent->GetExistingListenerManager();
>>>>>>> upstream-releases

  return listenerManager &&
<<<<<<< HEAD
         (listenerManager->HasListenersFor(nsGkAtoms::onclick) ||
          listenerManager->HasListenersFor(nsGkAtoms::onmousedown) ||
          listenerManager->HasListenersFor(nsGkAtoms::onmouseup));
}

void nsCoreUtils::DispatchClickEvent(nsITreeBoxObject *aTreeBoxObj,
                                     int32_t aRowIndex, nsTreeColumn *aColumn,
                                     const nsAString &aPseudoElt) {
  RefPtr<dom::Element> tcElm;
  aTreeBoxObj->GetTreeBody(getter_AddRefs(tcElm));
  if (!tcElm) return;
||||||| merged common ancestors
    (listenerManager->HasListenersFor(nsGkAtoms::onclick) ||
     listenerManager->HasListenersFor(nsGkAtoms::onmousedown) ||
     listenerManager->HasListenersFor(nsGkAtoms::onmouseup));
}

void
nsCoreUtils::DispatchClickEvent(nsITreeBoxObject *aTreeBoxObj,
                                int32_t aRowIndex, nsTreeColumn *aColumn,
                                const nsAString& aPseudoElt)
{
  RefPtr<dom::Element> tcElm;
  aTreeBoxObj->GetTreeBody(getter_AddRefs(tcElm));
  if (!tcElm)
    return;
=======
         (listenerManager->HasListenersFor(nsGkAtoms::onclick) ||
          listenerManager->HasListenersFor(nsGkAtoms::onmousedown) ||
          listenerManager->HasListenersFor(nsGkAtoms::onmouseup));
}
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsIDocument *document = tcElm->GetUncomposedDoc();
  if (!document) return;
||||||| merged common ancestors
  nsIDocument *document = tcElm->GetUncomposedDoc();
  if (!document)
    return;
=======
void nsCoreUtils::DispatchClickEvent(XULTreeElement* aTree, int32_t aRowIndex,
                                     nsTreeColumn* aColumn,
                                     const nsAString& aPseudoElt) {
  RefPtr<dom::Element> tcElm = aTree->GetTreeBody();
  if (!tcElm) return;

  Document* document = tcElm->GetUncomposedDoc();
  if (!document) return;
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIPresShell> presShell = document->GetShell();
  if (!presShell) return;
||||||| merged common ancestors
  nsCOMPtr<nsIPresShell> presShell = document->GetShell();
  if (!presShell)
    return;
=======
  RefPtr<PresShell> presShell = document->GetPresShell();
  if (!presShell) {
    return;
  }
>>>>>>> upstream-releases

  // Ensure row is visible.
  aTree->EnsureRowIsVisible(aRowIndex);

  // Calculate x and y coordinates.
<<<<<<< HEAD
  int32_t x = 0, y = 0, width = 0, height = 0;
  nsresult rv = aTreeBoxObj->GetCoordsForCellItem(
      aRowIndex, aColumn, aPseudoElt, &x, &y, &width, &height);
  if (NS_FAILED(rv)) return;
||||||| merged common ancestors
  int32_t x = 0, y = 0, width = 0, height = 0;
  nsresult rv = aTreeBoxObj->GetCoordsForCellItem(aRowIndex, aColumn,
                                                  aPseudoElt,
                                                  &x, &y, &width, &height);
  if (NS_FAILED(rv))
    return;
=======
  nsresult rv;
  nsIntRect rect =
      aTree->GetCoordsForCellItem(aRowIndex, aColumn, aPseudoElt, rv);
  if (NS_FAILED(rv)) {
    return;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  nsCOMPtr<nsIBoxObject> tcBoxObj =
      nsXULElement::FromNode(tcElm)->GetBoxObject(IgnoreErrors());

  int32_t tcX = 0;
  tcBoxObj->GetX(&tcX);

  int32_t tcY = 0;
  tcBoxObj->GetY(&tcY);
||||||| merged common ancestors
  nsCOMPtr<nsIBoxObject> tcBoxObj =
    nsXULElement::FromNode(tcElm)->GetBoxObject(IgnoreErrors());

  int32_t tcX = 0;
  tcBoxObj->GetX(&tcX);

  int32_t tcY = 0;
  tcBoxObj->GetY(&tcY);
=======
  RefPtr<DOMRect> treeBodyRect = tcElm->GetBoundingClientRect();
  int32_t tcX = (int32_t)treeBodyRect->X();
  int32_t tcY = (int32_t)treeBodyRect->Y();
>>>>>>> upstream-releases

  // Dispatch mouse events.
  AutoWeakFrame tcFrame = tcElm->GetPrimaryFrame();
  nsIFrame *rootFrame = presShell->GetRootFrame();

  nsPoint offset;
<<<<<<< HEAD
  nsIWidget *rootWidget = rootFrame->GetView()->GetNearestWidget(&offset);
||||||| merged common ancestors
  nsIWidget *rootWidget =
    rootFrame->GetView()->GetNearestWidget(&offset);
=======
  nsCOMPtr<nsIWidget> rootWidget =
      rootFrame->GetView()->GetNearestWidget(&offset);
>>>>>>> upstream-releases

  RefPtr<nsPresContext> presContext = presShell->GetPresContext();

<<<<<<< HEAD
  int32_t cnvdX = presContext->CSSPixelsToDevPixels(tcX + x + 1) +
                  presContext->AppUnitsToDevPixels(offset.x);
  int32_t cnvdY = presContext->CSSPixelsToDevPixels(tcY + y + 1) +
                  presContext->AppUnitsToDevPixels(offset.y);
||||||| merged common ancestors
  int32_t cnvdX = presContext->CSSPixelsToDevPixels(tcX + x + 1) +
    presContext->AppUnitsToDevPixels(offset.x);
  int32_t cnvdY = presContext->CSSPixelsToDevPixels(tcY + y + 1) +
    presContext->AppUnitsToDevPixels(offset.y);
=======
  int32_t cnvdX = presContext->CSSPixelsToDevPixels(tcX + int32_t(rect.x) + 1) +
                  presContext->AppUnitsToDevPixels(offset.x);
  int32_t cnvdY = presContext->CSSPixelsToDevPixels(tcY + int32_t(rect.y) + 1) +
                  presContext->AppUnitsToDevPixels(offset.y);
>>>>>>> upstream-releases

  // XUL is just desktop, so there is no real reason for senfing touch events.
  DispatchMouseEvent(eMouseDown, cnvdX, cnvdY, tcElm, tcFrame, presShell,
                     rootWidget);

  DispatchMouseEvent(eMouseUp, cnvdX, cnvdY, tcElm, tcFrame, presShell,
                     rootWidget);
}

<<<<<<< HEAD
void nsCoreUtils::DispatchMouseEvent(EventMessage aMessage, int32_t aX,
                                     int32_t aY, nsIContent *aContent,
                                     nsIFrame *aFrame, nsIPresShell *aPresShell,
                                     nsIWidget *aRootWidget) {
  WidgetMouseEvent event(true, aMessage, aRootWidget, WidgetMouseEvent::eReal,
                         WidgetMouseEvent::eNormal);
||||||| merged common ancestors
void
nsCoreUtils::DispatchMouseEvent(EventMessage aMessage, int32_t aX, int32_t aY,
                                nsIContent *aContent, nsIFrame *aFrame,
                                nsIPresShell *aPresShell, nsIWidget *aRootWidget)
{
  WidgetMouseEvent event(true, aMessage, aRootWidget,
                         WidgetMouseEvent::eReal, WidgetMouseEvent::eNormal);
=======
void nsCoreUtils::DispatchMouseEvent(EventMessage aMessage, int32_t aX,
                                     int32_t aY, nsIContent* aContent,
                                     nsIFrame* aFrame, PresShell* aPresShell,
                                     nsIWidget* aRootWidget) {
  WidgetMouseEvent event(true, aMessage, aRootWidget, WidgetMouseEvent::eReal,
                         WidgetMouseEvent::eNormal);
>>>>>>> upstream-releases

  event.mRefPoint = LayoutDeviceIntPoint(aX, aY);

  event.mClickCount = 1;
  event.mButton = MouseButton::eLeft;
  event.mTime = PR_IntervalNow();
  event.mInputSource = dom::MouseEvent_Binding::MOZ_SOURCE_UNKNOWN;

  nsEventStatus status = nsEventStatus_eIgnore;
  aPresShell->HandleEventWithTarget(&event, aFrame, aContent, &status);
}

<<<<<<< HEAD
void nsCoreUtils::DispatchTouchEvent(EventMessage aMessage, int32_t aX,
                                     int32_t aY, nsIContent *aContent,
                                     nsIFrame *aFrame, nsIPresShell *aPresShell,
                                     nsIWidget *aRootWidget) {
  nsIDocShell *docShell = nullptr;
||||||| merged common ancestors
void
nsCoreUtils::DispatchTouchEvent(EventMessage aMessage, int32_t aX, int32_t aY,
                                nsIContent* aContent, nsIFrame* aFrame,
                                nsIPresShell* aPresShell, nsIWidget* aRootWidget)
{
  nsIDocShell* docShell = nullptr;
=======
void nsCoreUtils::DispatchTouchEvent(EventMessage aMessage, int32_t aX,
                                     int32_t aY, nsIContent* aContent,
                                     nsIFrame* aFrame, PresShell* aPresShell,
                                     nsIWidget* aRootWidget) {
  nsIDocShell* docShell = nullptr;
>>>>>>> upstream-releases
  if (aPresShell->GetDocument()) {
    docShell = aPresShell->GetDocument()->GetDocShell();
  }
  if (!dom::TouchEvent::PrefEnabled(docShell)) {
    return;
  }

  WidgetTouchEvent event(true, aMessage, aRootWidget);

  event.mTime = PR_IntervalNow();

  // XXX: Touch has an identifier of -1 to hint that it is synthesized.
  RefPtr<dom::Touch> t = new dom::Touch(-1, LayoutDeviceIntPoint(aX, aY),
                                        LayoutDeviceIntPoint(1, 1), 0.0f, 1.0f);
  t->SetTouchTarget(aContent);
  event.mTouches.AppendElement(t);
  nsEventStatus status = nsEventStatus_eIgnore;
  aPresShell->HandleEventWithTarget(&event, aFrame, aContent, &status);
}

<<<<<<< HEAD
uint32_t nsCoreUtils::GetAccessKeyFor(nsIContent *aContent) {
||||||| merged common ancestors
uint32_t
nsCoreUtils::GetAccessKeyFor(nsIContent* aContent)
{
=======
uint32_t nsCoreUtils::GetAccessKeyFor(nsIContent* aContent) {
>>>>>>> upstream-releases
  // Accesskeys are registered by @accesskey attribute only. At first check
  // whether it is presented on the given element to avoid the slow
  // EventStateManager::GetRegisteredAccessKey() method.
  if (!aContent->IsElement() ||
      !aContent->AsElement()->HasAttr(kNameSpaceID_None, nsGkAtoms::accesskey))
    return 0;

<<<<<<< HEAD
  nsPresContext *presContext = aContent->OwnerDoc()->GetPresContext();
  if (!presContext) return 0;
||||||| merged common ancestors
  nsPresContext* presContext = aContent->OwnerDoc()->GetPresContext();
  if (!presContext)
    return 0;
=======
  nsPresContext* presContext = aContent->OwnerDoc()->GetPresContext();
  if (!presContext) return 0;
>>>>>>> upstream-releases

<<<<<<< HEAD
  EventStateManager *esm = presContext->EventStateManager();
  if (!esm) return 0;
||||||| merged common ancestors
  EventStateManager *esm = presContext->EventStateManager();
  if (!esm)
    return 0;
=======
  EventStateManager* esm = presContext->EventStateManager();
  if (!esm) return 0;
>>>>>>> upstream-releases

  return esm->GetRegisteredAccessKey(aContent->AsElement());
}

<<<<<<< HEAD
nsIContent *nsCoreUtils::GetDOMElementFor(nsIContent *aContent) {
  if (aContent->IsElement()) return aContent;
||||||| merged common ancestors
nsIContent *
nsCoreUtils::GetDOMElementFor(nsIContent *aContent)
{
  if (aContent->IsElement())
    return aContent;
=======
nsIContent* nsCoreUtils::GetDOMElementFor(nsIContent* aContent) {
  if (aContent->IsElement()) return aContent;
>>>>>>> upstream-releases

  if (aContent->IsText()) return aContent->GetFlattenedTreeParent();

  return nullptr;
}

<<<<<<< HEAD
nsINode *nsCoreUtils::GetDOMNodeFromDOMPoint(nsINode *aNode, uint32_t aOffset) {
||||||| merged common ancestors
nsINode *
nsCoreUtils::GetDOMNodeFromDOMPoint(nsINode *aNode, uint32_t aOffset)
{
=======
nsINode* nsCoreUtils::GetDOMNodeFromDOMPoint(nsINode* aNode, uint32_t aOffset) {
>>>>>>> upstream-releases
  if (aNode && aNode->IsElement()) {
    uint32_t childCount = aNode->GetChildCount();
    NS_ASSERTION(aOffset <= childCount, "Wrong offset of the DOM point!");

    // The offset can be after last child of container node that means DOM point
    // is placed immediately after the last child. In this case use the DOM node
    // from the given DOM point is used as result node.
    if (aOffset != childCount) return aNode->GetChildAt_Deprecated(aOffset);
  }

  return aNode;
}

<<<<<<< HEAD
bool nsCoreUtils::IsAncestorOf(nsINode *aPossibleAncestorNode,
                               nsINode *aPossibleDescendantNode,
                               nsINode *aRootNode) {
||||||| merged common ancestors
bool
nsCoreUtils::IsAncestorOf(nsINode *aPossibleAncestorNode,
                          nsINode *aPossibleDescendantNode,
                          nsINode *aRootNode)
{
=======
bool nsCoreUtils::IsAncestorOf(nsINode* aPossibleAncestorNode,
                               nsINode* aPossibleDescendantNode,
                               nsINode* aRootNode) {
>>>>>>> upstream-releases
  NS_ENSURE_TRUE(aPossibleAncestorNode && aPossibleDescendantNode, false);

  nsINode* parentNode = aPossibleDescendantNode;
  while ((parentNode = parentNode->GetParentNode()) &&
         parentNode != aRootNode) {
    if (parentNode == aPossibleAncestorNode) return true;
  }

  return false;
}

<<<<<<< HEAD
nsresult nsCoreUtils::ScrollSubstringTo(nsIFrame *aFrame, nsRange *aRange,
                                        uint32_t aScrollType) {
  nsIPresShell::ScrollAxis vertical, horizontal;
||||||| merged common ancestors
nsresult
nsCoreUtils::ScrollSubstringTo(nsIFrame* aFrame, nsRange* aRange,
                               uint32_t aScrollType)
{
  nsIPresShell::ScrollAxis vertical, horizontal;
=======
nsresult nsCoreUtils::ScrollSubstringTo(nsIFrame* aFrame, nsRange* aRange,
                                        uint32_t aScrollType) {
  ScrollAxis vertical, horizontal;
>>>>>>> upstream-releases
  ConvertScrollTypeToPercents(aScrollType, &vertical, &horizontal);

  return ScrollSubstringTo(aFrame, aRange, vertical, horizontal);
}

<<<<<<< HEAD
nsresult nsCoreUtils::ScrollSubstringTo(nsIFrame *aFrame, nsRange *aRange,
                                        nsIPresShell::ScrollAxis aVertical,
                                        nsIPresShell::ScrollAxis aHorizontal) {
||||||| merged common ancestors
nsresult
nsCoreUtils::ScrollSubstringTo(nsIFrame* aFrame, nsRange* aRange,
                               nsIPresShell::ScrollAxis aVertical,
                               nsIPresShell::ScrollAxis aHorizontal)
{
=======
nsresult nsCoreUtils::ScrollSubstringTo(nsIFrame* aFrame, nsRange* aRange,
                                        ScrollAxis aVertical,
                                        ScrollAxis aHorizontal) {
>>>>>>> upstream-releases
  if (!aFrame || !aRange) {
    return NS_ERROR_FAILURE;
  }

  nsPresContext* presContext = aFrame->PresContext();

  nsCOMPtr<nsISelectionController> selCon;
  aFrame->GetSelectionController(presContext, getter_AddRefs(selCon));
  NS_ENSURE_TRUE(selCon, NS_ERROR_FAILURE);

  RefPtr<dom::Selection> selection =
      selCon->GetSelection(nsISelectionController::SELECTION_ACCESSIBILITY);

  selection->RemoveAllRanges(IgnoreErrors());
  selection->AddRangeAndSelectFramesAndNotifyListeners(*aRange, IgnoreErrors());

  selection->ScrollIntoView(nsISelectionController::SELECTION_ANCHOR_REGION,
                            aVertical, aHorizontal,
                            Selection::SCROLL_SYNCHRONOUS);

  selection->CollapseToStart(IgnoreErrors());

  return NS_OK;
}

<<<<<<< HEAD
void nsCoreUtils::ScrollFrameToPoint(nsIFrame *aScrollableFrame,
                                     nsIFrame *aFrame,
                                     const nsIntPoint &aPoint) {
  nsIScrollableFrame *scrollableFrame = do_QueryFrame(aScrollableFrame);
  if (!scrollableFrame) return;
||||||| merged common ancestors
void
nsCoreUtils::ScrollFrameToPoint(nsIFrame *aScrollableFrame,
                                nsIFrame *aFrame,
                                const nsIntPoint& aPoint)
{
  nsIScrollableFrame* scrollableFrame = do_QueryFrame(aScrollableFrame);
  if (!scrollableFrame)
    return;
=======
void nsCoreUtils::ScrollFrameToPoint(nsIFrame* aScrollableFrame,
                                     nsIFrame* aFrame,
                                     const nsIntPoint& aPoint) {
  nsIScrollableFrame* scrollableFrame = do_QueryFrame(aScrollableFrame);
  if (!scrollableFrame) return;
>>>>>>> upstream-releases

  nsPoint point =
      ToAppUnits(aPoint, aFrame->PresContext()->AppUnitsPerDevPixel());
  nsRect frameRect = aFrame->GetScreenRectInAppUnits();
  nsPoint deltaPoint = point - frameRect.TopLeft();

  nsPoint scrollPoint = scrollableFrame->GetScrollPosition();
  scrollPoint -= deltaPoint;

  scrollableFrame->ScrollTo(scrollPoint, ScrollMode::Instant);
}

<<<<<<< HEAD
void nsCoreUtils::ConvertScrollTypeToPercents(
    uint32_t aScrollType, nsIPresShell::ScrollAxis *aVertical,
    nsIPresShell::ScrollAxis *aHorizontal) {
  int16_t whereY, whereX;
  nsIPresShell::WhenToScroll whenY, whenX;
  switch (aScrollType) {
||||||| merged common ancestors
void
nsCoreUtils::ConvertScrollTypeToPercents(uint32_t aScrollType,
                                         nsIPresShell::ScrollAxis *aVertical,
                                         nsIPresShell::ScrollAxis *aHorizontal)
{
  int16_t whereY, whereX;
  nsIPresShell::WhenToScroll whenY, whenX;
  switch (aScrollType)
  {
=======
void nsCoreUtils::ConvertScrollTypeToPercents(uint32_t aScrollType,
                                              ScrollAxis* aVertical,
                                              ScrollAxis* aHorizontal) {
  WhereToScroll whereY, whereX;
  WhenToScroll whenY, whenX;
  switch (aScrollType) {
>>>>>>> upstream-releases
    case nsIAccessibleScrollType::SCROLL_TYPE_TOP_LEFT:
<<<<<<< HEAD
      whereY = nsIPresShell::SCROLL_TOP;
      whenY = nsIPresShell::SCROLL_ALWAYS;
      whereX = nsIPresShell::SCROLL_LEFT;
      whenX = nsIPresShell::SCROLL_ALWAYS;
||||||| merged common ancestors
      whereY = nsIPresShell::SCROLL_TOP;
      whenY  = nsIPresShell::SCROLL_ALWAYS;
      whereX = nsIPresShell::SCROLL_LEFT;
      whenX  = nsIPresShell::SCROLL_ALWAYS;
=======
      whereY = kScrollToTop;
      whenY = WhenToScroll::Always;
      whereX = kScrollToLeft;
      whenX = WhenToScroll::Always;
>>>>>>> upstream-releases
      break;
    case nsIAccessibleScrollType::SCROLL_TYPE_BOTTOM_RIGHT:
<<<<<<< HEAD
      whereY = nsIPresShell::SCROLL_BOTTOM;
      whenY = nsIPresShell::SCROLL_ALWAYS;
      whereX = nsIPresShell::SCROLL_RIGHT;
      whenX = nsIPresShell::SCROLL_ALWAYS;
||||||| merged common ancestors
      whereY = nsIPresShell::SCROLL_BOTTOM;
      whenY  = nsIPresShell::SCROLL_ALWAYS;
      whereX = nsIPresShell::SCROLL_RIGHT;
      whenX  = nsIPresShell::SCROLL_ALWAYS;
=======
      whereY = kScrollToBottom;
      whenY = WhenToScroll::Always;
      whereX = kScrollToRight;
      whenX = WhenToScroll::Always;
>>>>>>> upstream-releases
      break;
    case nsIAccessibleScrollType::SCROLL_TYPE_TOP_EDGE:
<<<<<<< HEAD
      whereY = nsIPresShell::SCROLL_TOP;
      whenY = nsIPresShell::SCROLL_ALWAYS;
      whereX = nsIPresShell::SCROLL_MINIMUM;
      whenX = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
||||||| merged common ancestors
      whereY = nsIPresShell::SCROLL_TOP;
      whenY  = nsIPresShell::SCROLL_ALWAYS;
      whereX = nsIPresShell::SCROLL_MINIMUM;
      whenX  = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
=======
      whereY = kScrollToTop;
      whenY = WhenToScroll::Always;
      whereX = kScrollMinimum;
      whenX = WhenToScroll::IfNotFullyVisible;
>>>>>>> upstream-releases
      break;
    case nsIAccessibleScrollType::SCROLL_TYPE_BOTTOM_EDGE:
<<<<<<< HEAD
      whereY = nsIPresShell::SCROLL_BOTTOM;
      whenY = nsIPresShell::SCROLL_ALWAYS;
      whereX = nsIPresShell::SCROLL_MINIMUM;
      whenX = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
||||||| merged common ancestors
      whereY = nsIPresShell::SCROLL_BOTTOM;
      whenY  = nsIPresShell::SCROLL_ALWAYS;
      whereX = nsIPresShell::SCROLL_MINIMUM;
      whenX  = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
=======
      whereY = kScrollToBottom;
      whenY = WhenToScroll::Always;
      whereX = kScrollMinimum;
      whenX = WhenToScroll::IfNotFullyVisible;
>>>>>>> upstream-releases
      break;
    case nsIAccessibleScrollType::SCROLL_TYPE_LEFT_EDGE:
<<<<<<< HEAD
      whereY = nsIPresShell::SCROLL_MINIMUM;
      whenY = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
      whereX = nsIPresShell::SCROLL_LEFT;
      whenX = nsIPresShell::SCROLL_ALWAYS;
||||||| merged common ancestors
      whereY = nsIPresShell::SCROLL_MINIMUM;
      whenY  = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
      whereX = nsIPresShell::SCROLL_LEFT;
      whenX  = nsIPresShell::SCROLL_ALWAYS;
=======
      whereY = kScrollMinimum;
      whenY = WhenToScroll::IfNotFullyVisible;
      whereX = kScrollToLeft;
      whenX = WhenToScroll::Always;
>>>>>>> upstream-releases
      break;
    case nsIAccessibleScrollType::SCROLL_TYPE_RIGHT_EDGE:
<<<<<<< HEAD
      whereY = nsIPresShell::SCROLL_MINIMUM;
      whenY = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
      whereX = nsIPresShell::SCROLL_RIGHT;
      whenX = nsIPresShell::SCROLL_ALWAYS;
||||||| merged common ancestors
      whereY = nsIPresShell::SCROLL_MINIMUM;
      whenY  = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
      whereX = nsIPresShell::SCROLL_RIGHT;
      whenX  = nsIPresShell::SCROLL_ALWAYS;
=======
      whereY = kScrollMinimum;
      whenY = WhenToScroll::IfNotFullyVisible;
      whereX = kScrollToRight;
      whenX = WhenToScroll::Always;
>>>>>>> upstream-releases
      break;
    default:
<<<<<<< HEAD
      whereY = nsIPresShell::SCROLL_MINIMUM;
      whenY = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
      whereX = nsIPresShell::SCROLL_MINIMUM;
      whenX = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
||||||| merged common ancestors
      whereY = nsIPresShell::SCROLL_MINIMUM;
      whenY  = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
      whereX = nsIPresShell::SCROLL_MINIMUM;
      whenX  = nsIPresShell::SCROLL_IF_NOT_FULLY_VISIBLE;
=======
      whereY = kScrollMinimum;
      whenY = WhenToScroll::IfNotFullyVisible;
      whereX = kScrollMinimum;
      whenX = WhenToScroll::IfNotFullyVisible;
>>>>>>> upstream-releases
  }
  *aVertical = ScrollAxis(whereY, whenY);
  *aHorizontal = ScrollAxis(whereX, whenX);
}

<<<<<<< HEAD
nsIntPoint nsCoreUtils::GetScreenCoordsForWindow(nsINode *aNode) {
||||||| merged common ancestors
nsIntPoint
nsCoreUtils::GetScreenCoordsForWindow(nsINode *aNode)
{
=======
nsIntPoint nsCoreUtils::GetScreenCoordsForWindow(nsINode* aNode) {
>>>>>>> upstream-releases
  nsIntPoint coords(0, 0);
  nsCOMPtr<nsIDocShellTreeItem> treeItem(GetDocShellFor(aNode));
  if (!treeItem) return coords;

  nsCOMPtr<nsIDocShellTreeOwner> treeOwner;
  treeItem->GetTreeOwner(getter_AddRefs(treeOwner));
  if (!treeOwner) return coords;

  nsCOMPtr<nsIBaseWindow> baseWindow = do_QueryInterface(treeOwner);
  if (baseWindow)
    baseWindow->GetPosition(&coords.x, &coords.y);  // in device pixels

  return coords;
}

<<<<<<< HEAD
already_AddRefed<nsIDocShell> nsCoreUtils::GetDocShellFor(nsINode *aNode) {
  if (!aNode) return nullptr;
||||||| merged common ancestors
already_AddRefed<nsIDocShell>
nsCoreUtils::GetDocShellFor(nsINode *aNode)
{
  if (!aNode)
    return nullptr;
=======
already_AddRefed<nsIDocShell> nsCoreUtils::GetDocShellFor(nsINode* aNode) {
  if (!aNode) return nullptr;
>>>>>>> upstream-releases

  nsCOMPtr<nsIDocShell> docShell = aNode->OwnerDoc()->GetDocShell();
  return docShell.forget();
}

<<<<<<< HEAD
bool nsCoreUtils::IsRootDocument(nsIDocument *aDocument) {
||||||| merged common ancestors
bool
nsCoreUtils::IsRootDocument(nsIDocument *aDocument)
{
=======
bool nsCoreUtils::IsRootDocument(Document* aDocument) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIDocShellTreeItem> docShellTreeItem = aDocument->GetDocShell();
  NS_ASSERTION(docShellTreeItem, "No document shell for document!");

  nsCOMPtr<nsIDocShellTreeItem> parentTreeItem;
  docShellTreeItem->GetParent(getter_AddRefs(parentTreeItem));

  return !parentTreeItem;
}

<<<<<<< HEAD
bool nsCoreUtils::IsContentDocument(nsIDocument *aDocument) {
||||||| merged common ancestors
bool
nsCoreUtils::IsContentDocument(nsIDocument *aDocument)
{
=======
bool nsCoreUtils::IsContentDocument(Document* aDocument) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIDocShellTreeItem> docShellTreeItem = aDocument->GetDocShell();
  NS_ASSERTION(docShellTreeItem, "No document shell tree item for document!");

  return (docShellTreeItem->ItemType() == nsIDocShellTreeItem::typeContent);
}

<<<<<<< HEAD
bool nsCoreUtils::IsTabDocument(nsIDocument *aDocumentNode) {
||||||| merged common ancestors
bool
nsCoreUtils::IsTabDocument(nsIDocument* aDocumentNode)
{
=======
bool nsCoreUtils::IsTabDocument(Document* aDocumentNode) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIDocShellTreeItem> treeItem(aDocumentNode->GetDocShell());

  nsCOMPtr<nsIDocShellTreeItem> parentTreeItem;
  treeItem->GetParent(getter_AddRefs(parentTreeItem));

  // Tab document running in own process doesn't have parent.
  if (XRE_IsContentProcess()) return !parentTreeItem;

  // Parent of docshell for tab document running in chrome process is root.
  nsCOMPtr<nsIDocShellTreeItem> rootTreeItem;
  treeItem->GetRootTreeItem(getter_AddRefs(rootTreeItem));

  return parentTreeItem == rootTreeItem;
}

<<<<<<< HEAD
bool nsCoreUtils::IsErrorPage(nsIDocument *aDocument) {
  nsIURI *uri = aDocument->GetDocumentURI();
||||||| merged common ancestors
bool
nsCoreUtils::IsErrorPage(nsIDocument *aDocument)
{
  nsIURI *uri = aDocument->GetDocumentURI();
=======
bool nsCoreUtils::IsErrorPage(Document* aDocument) {
  nsIURI* uri = aDocument->GetDocumentURI();
>>>>>>> upstream-releases
  bool isAboutScheme = false;
  uri->SchemeIs("about", &isAboutScheme);
  if (!isAboutScheme) return false;

  nsAutoCString path;
  uri->GetPathQueryRef(path);

  NS_NAMED_LITERAL_CSTRING(neterror, "neterror");
  NS_NAMED_LITERAL_CSTRING(certerror, "certerror");

  return StringBeginsWith(path, neterror) || StringBeginsWith(path, certerror);
}

<<<<<<< HEAD
bool nsCoreUtils::GetID(nsIContent *aContent, nsAString &aID) {
||||||| merged common ancestors
bool
nsCoreUtils::GetID(nsIContent *aContent, nsAString& aID)
{
=======
bool nsCoreUtils::GetID(nsIContent* aContent, nsAString& aID) {
>>>>>>> upstream-releases
  return aContent->IsElement() &&
         aContent->AsElement()->GetAttr(kNameSpaceID_None, nsGkAtoms::id, aID);
}

<<<<<<< HEAD
bool nsCoreUtils::GetUIntAttr(nsIContent *aContent, nsAtom *aAttr,
                              int32_t *aUInt) {
||||||| merged common ancestors
bool
nsCoreUtils::GetUIntAttr(nsIContent *aContent, nsAtom *aAttr, int32_t *aUInt)
{
=======
bool nsCoreUtils::GetUIntAttr(nsIContent* aContent, nsAtom* aAttr,
                              int32_t* aUInt) {
>>>>>>> upstream-releases
  nsAutoString value;
  if (!aContent->IsElement()) {
    return false;
  }
  aContent->AsElement()->GetAttr(kNameSpaceID_None, aAttr, value);
  if (!value.IsEmpty()) {
    nsresult error = NS_OK;
    int32_t integer = value.ToInteger(&error);
    if (NS_SUCCEEDED(error) && integer > 0) {
      *aUInt = integer;
      return true;
    }
  }

  return false;
}

<<<<<<< HEAD
void nsCoreUtils::GetLanguageFor(nsIContent *aContent, nsIContent *aRootContent,
                                 nsAString &aLanguage) {
||||||| merged common ancestors
void
nsCoreUtils::GetLanguageFor(nsIContent *aContent, nsIContent *aRootContent,
                            nsAString& aLanguage)
{
=======
void nsCoreUtils::GetLanguageFor(nsIContent* aContent, nsIContent* aRootContent,
                                 nsAString& aLanguage) {
>>>>>>> upstream-releases
  aLanguage.Truncate();

  nsIContent* walkUp = aContent;
  while (walkUp && walkUp != aRootContent &&
         (!walkUp->IsElement() ||
          !walkUp->AsElement()->GetAttr(kNameSpaceID_None, nsGkAtoms::lang,
                                        aLanguage)))
    walkUp = walkUp->GetParent();
}

<<<<<<< HEAD
already_AddRefed<nsIBoxObject> nsCoreUtils::GetTreeBodyBoxObject(
    nsITreeBoxObject *aTreeBoxObj) {
  RefPtr<dom::Element> tcElm;
  aTreeBoxObj->GetTreeBody(getter_AddRefs(tcElm));
  RefPtr<nsXULElement> tcXULElm = nsXULElement::FromNodeOrNull(tcElm);
  if (!tcXULElm) return nullptr;

  return tcXULElm->GetBoxObject(IgnoreErrors());
}

already_AddRefed<nsITreeBoxObject> nsCoreUtils::GetTreeBoxObject(
    nsIContent *aContent) {
||||||| merged common ancestors
already_AddRefed<nsIBoxObject>
nsCoreUtils::GetTreeBodyBoxObject(nsITreeBoxObject *aTreeBoxObj)
{
  RefPtr<dom::Element> tcElm;
  aTreeBoxObj->GetTreeBody(getter_AddRefs(tcElm));
  RefPtr<nsXULElement> tcXULElm = nsXULElement::FromNodeOrNull(tcElm);
  if (!tcXULElm)
    return nullptr;

  return tcXULElm->GetBoxObject(IgnoreErrors());
}

already_AddRefed<nsITreeBoxObject>
nsCoreUtils::GetTreeBoxObject(nsIContent *aContent)
{
=======
XULTreeElement* nsCoreUtils::GetTree(nsIContent* aContent) {
>>>>>>> upstream-releases
  // Find DOMNode's parents recursively until reach the <tree> tag
  nsIContent *currentContent = aContent;
  while (currentContent) {
<<<<<<< HEAD
    if (currentContent->NodeInfo()->Equals(nsGkAtoms::tree, kNameSpaceID_XUL)) {
      // We will get the nsITreeBoxObject from the tree node
      RefPtr<nsXULElement> xulElement = nsXULElement::FromNode(currentContent);
      nsCOMPtr<nsIBoxObject> box = xulElement->GetBoxObject(IgnoreErrors());
      nsCOMPtr<nsITreeBoxObject> treeBox(do_QueryInterface(box));
      if (treeBox) return treeBox.forget();
||||||| merged common ancestors
    if (currentContent->NodeInfo()->Equals(nsGkAtoms::tree,
                                           kNameSpaceID_XUL)) {
      // We will get the nsITreeBoxObject from the tree node
      RefPtr<nsXULElement> xulElement =
        nsXULElement::FromNode(currentContent);
      nsCOMPtr<nsIBoxObject> box = xulElement->GetBoxObject(IgnoreErrors());
      nsCOMPtr<nsITreeBoxObject> treeBox(do_QueryInterface(box));
      if (treeBox)
        return treeBox.forget();
=======
    if (currentContent->NodeInfo()->Equals(nsGkAtoms::tree, kNameSpaceID_XUL)) {
      return XULTreeElement::FromNode(currentContent);
>>>>>>> upstream-releases
    }
    currentContent = currentContent->GetFlattenedTreeParent();
  }

  return nullptr;
}

<<<<<<< HEAD
already_AddRefed<nsTreeColumn> nsCoreUtils::GetFirstSensibleColumn(
    nsITreeBoxObject *aTree) {
  RefPtr<nsTreeColumns> cols;
  aTree->GetColumns(getter_AddRefs(cols));
  if (!cols) return nullptr;
||||||| merged common ancestors
already_AddRefed<nsTreeColumn>
nsCoreUtils::GetFirstSensibleColumn(nsITreeBoxObject *aTree)
{
  RefPtr<nsTreeColumns> cols;
  aTree->GetColumns(getter_AddRefs(cols));
  if (!cols)
    return nullptr;
=======
already_AddRefed<nsTreeColumn> nsCoreUtils::GetFirstSensibleColumn(
    XULTreeElement* aTree) {
  RefPtr<nsTreeColumns> cols = aTree->GetColumns();
  if (!cols) return nullptr;
>>>>>>> upstream-releases

  RefPtr<nsTreeColumn> column = cols->GetFirstColumn();
  if (column && IsColumnHidden(column)) return GetNextSensibleColumn(column);

  return column.forget();
}

<<<<<<< HEAD
uint32_t nsCoreUtils::GetSensibleColumnCount(nsITreeBoxObject *aTree) {
||||||| merged common ancestors
uint32_t
nsCoreUtils::GetSensibleColumnCount(nsITreeBoxObject *aTree)
{
=======
uint32_t nsCoreUtils::GetSensibleColumnCount(XULTreeElement* aTree) {
>>>>>>> upstream-releases
  uint32_t count = 0;

<<<<<<< HEAD
  RefPtr<nsTreeColumns> cols;
  aTree->GetColumns(getter_AddRefs(cols));
  if (!cols) return count;
||||||| merged common ancestors
  RefPtr<nsTreeColumns> cols;
  aTree->GetColumns(getter_AddRefs(cols));
  if (!cols)
    return count;
=======
  RefPtr<nsTreeColumns> cols = aTree->GetColumns();
  if (!cols) return count;
>>>>>>> upstream-releases

  nsTreeColumn *column = cols->GetFirstColumn();

  while (column) {
    if (!IsColumnHidden(column)) count++;

    column = column->GetNext();
  }

  return count;
}

<<<<<<< HEAD
already_AddRefed<nsTreeColumn> nsCoreUtils::GetSensibleColumnAt(
    nsITreeBoxObject *aTree, uint32_t aIndex) {
||||||| merged common ancestors
already_AddRefed<nsTreeColumn>
nsCoreUtils::GetSensibleColumnAt(nsITreeBoxObject *aTree, uint32_t aIndex)
{
=======
already_AddRefed<nsTreeColumn> nsCoreUtils::GetSensibleColumnAt(
    XULTreeElement* aTree, uint32_t aIndex) {
>>>>>>> upstream-releases
  uint32_t idx = aIndex;

  nsCOMPtr<nsTreeColumn> column = GetFirstSensibleColumn(aTree);
  while (column) {
    if (idx == 0) return column.forget();

    idx--;
    column = GetNextSensibleColumn(column);
  }

  return nullptr;
}

<<<<<<< HEAD
already_AddRefed<nsTreeColumn> nsCoreUtils::GetNextSensibleColumn(
    nsTreeColumn *aColumn) {
||||||| merged common ancestors
already_AddRefed<nsTreeColumn>
nsCoreUtils::GetNextSensibleColumn(nsTreeColumn* aColumn)
{
=======
already_AddRefed<nsTreeColumn> nsCoreUtils::GetNextSensibleColumn(
    nsTreeColumn* aColumn) {
>>>>>>> upstream-releases
  RefPtr<nsTreeColumn> nextColumn = aColumn->GetNext();

  while (nextColumn && IsColumnHidden(nextColumn)) {
    nextColumn = nextColumn->GetNext();
  }

  return nextColumn.forget();
}

<<<<<<< HEAD
already_AddRefed<nsTreeColumn> nsCoreUtils::GetPreviousSensibleColumn(
    nsTreeColumn *aColumn) {
||||||| merged common ancestors
already_AddRefed<nsTreeColumn>
nsCoreUtils::GetPreviousSensibleColumn(nsTreeColumn* aColumn)
{
=======
already_AddRefed<nsTreeColumn> nsCoreUtils::GetPreviousSensibleColumn(
    nsTreeColumn* aColumn) {
>>>>>>> upstream-releases
  RefPtr<nsTreeColumn> prevColumn = aColumn->GetPrevious();

  while (prevColumn && IsColumnHidden(prevColumn)) {
    prevColumn = prevColumn->GetPrevious();
  }

  return prevColumn.forget();
}

<<<<<<< HEAD
bool nsCoreUtils::IsColumnHidden(nsTreeColumn *aColumn) {
  Element *element = aColumn->Element();
||||||| merged common ancestors
bool
nsCoreUtils::IsColumnHidden(nsTreeColumn* aColumn)
{
  Element* element = aColumn->Element();
=======
bool nsCoreUtils::IsColumnHidden(nsTreeColumn* aColumn) {
  Element* element = aColumn->Element();
>>>>>>> upstream-releases
  return element->AttrValueIs(kNameSpaceID_None, nsGkAtoms::hidden,
                              nsGkAtoms::_true, eCaseMatters);
}

<<<<<<< HEAD
void nsCoreUtils::ScrollTo(nsIPresShell *aPresShell, nsIContent *aContent,
                           uint32_t aScrollType) {
  nsIPresShell::ScrollAxis vertical, horizontal;
||||||| merged common ancestors
void
nsCoreUtils::ScrollTo(nsIPresShell* aPresShell, nsIContent* aContent,
                      uint32_t aScrollType)
{
  nsIPresShell::ScrollAxis vertical, horizontal;
=======
void nsCoreUtils::ScrollTo(PresShell* aPresShell, nsIContent* aContent,
                           uint32_t aScrollType) {
  ScrollAxis vertical, horizontal;
>>>>>>> upstream-releases
  ConvertScrollTypeToPercents(aScrollType, &vertical, &horizontal);
  aPresShell->ScrollContentIntoView(aContent, vertical, horizontal,
                                    ScrollFlags::ScrollOverflowHidden);
}

<<<<<<< HEAD
bool nsCoreUtils::IsWhitespaceString(const nsAString &aString) {
||||||| merged common ancestors
bool
nsCoreUtils::IsWhitespaceString(const nsAString& aString)
{
=======
bool nsCoreUtils::IsWhitespaceString(const nsAString& aString) {
>>>>>>> upstream-releases
  nsAString::const_char_iterator iterBegin, iterEnd;

  aString.BeginReading(iterBegin);
  aString.EndReading(iterEnd);

  while (iterBegin != iterEnd && IsWhitespace(*iterBegin)) ++iterBegin;

  return iterBegin == iterEnd;
}

bool nsCoreUtils::AccEventObserversExist() {
  nsCOMPtr<nsIObserverService> obsService = services::GetObserverService();
  NS_ENSURE_TRUE(obsService, false);

  nsCOMPtr<nsISimpleEnumerator> observers;
  obsService->EnumerateObservers(NS_ACCESSIBLE_EVENT_TOPIC,
                                 getter_AddRefs(observers));
  NS_ENSURE_TRUE(observers, false);

  bool hasObservers = false;
  observers->HasMoreElements(&hasObservers);

  return hasObservers;
}

void nsCoreUtils::DispatchAccEvent(RefPtr<nsIAccessibleEvent> event) {
  nsCOMPtr<nsIObserverService> obsService = services::GetObserverService();
  NS_ENSURE_TRUE_VOID(obsService);

  obsService->NotifyObservers(event, NS_ACCESSIBLE_EVENT_TOPIC, nullptr);
}
