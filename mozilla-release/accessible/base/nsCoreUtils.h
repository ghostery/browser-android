/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsCoreUtils_h_
#define nsCoreUtils_h_

#include "mozilla/EventForwards.h"
#include "mozilla/dom/Element.h"
#include "nsIAccessibleEvent.h"
#include "nsIContent.h"
<<<<<<< HEAD
#include "nsIDocument.h"  // for GetShell()
#include "nsIPresShell.h"
||||||| merged common ancestors
#include "nsIDocument.h" // for GetShell()
#include "nsIPresShell.h"
=======
#include "mozilla/dom/Document.h"  // for GetPresShell()
>>>>>>> upstream-releases

#include "nsPoint.h"
#include "nsTArray.h"

class nsRange;
class nsTreeColumn;
class nsIFrame;
class nsIDocShell;
class nsIWidget;

namespace mozilla {
class PresShell;
namespace dom {
class XULTreeElement;
}
}  // namespace mozilla

/**
 * Core utils.
 */
<<<<<<< HEAD
class nsCoreUtils {
 public:
||||||| merged common ancestors
class nsCoreUtils
{
public:
=======
class nsCoreUtils {
 public:
  typedef mozilla::PresShell PresShell;
  typedef mozilla::dom::Document Document;

>>>>>>> upstream-releases
  /**
   * Return true if the given node is a label of a control.
   */
  static bool IsLabelWithControl(nsIContent* aContent);

  /**
   * Return true if the given node has registered click, mousedown or mouseup
   * event listeners.
   */
  static bool HasClickListener(nsIContent* aContent);

  /**
   * Dispatch click event to XUL tree cell.
   *
   * @param  aTree        [in] tree
   * @param  aRowIndex    [in] row index
   * @param  aColumn      [in] column object
   * @param  aPseudoElm   [in] pseudo element inside the cell, see
   *                       XULTreeElement for available values
   */
<<<<<<< HEAD
  static void DispatchClickEvent(nsITreeBoxObject *aTreeBoxObj,
                                 int32_t aRowIndex, nsTreeColumn *aColumn,
                                 const nsAString &aPseudoElt = EmptyString());
||||||| merged common ancestors
  static void DispatchClickEvent(nsITreeBoxObject *aTreeBoxObj,
                                 int32_t aRowIndex, nsTreeColumn *aColumn,
                                 const nsAString& aPseudoElt = EmptyString());
=======
  MOZ_CAN_RUN_SCRIPT
  static void DispatchClickEvent(mozilla::dom::XULTreeElement* aTree,
                                 int32_t aRowIndex, nsTreeColumn* aColumn,
                                 const nsAString& aPseudoElt = EmptyString());
>>>>>>> upstream-releases

  /**
   * Send mouse event to the given element.
   *
   * @param aMessage     [in] an event message (see EventForwards.h)
   * @param aX           [in] x coordinate in dev pixels
   * @param aY           [in] y coordinate in dev pixels
   * @param aContent     [in] the element
   * @param aFrame       [in] frame of the element
   * @param aPresShell   [in] the presshell for the element
   * @param aRootWidget  [in] the root widget of the element
   */
<<<<<<< HEAD
  static void DispatchMouseEvent(mozilla::EventMessage aMessage, int32_t aX,
                                 int32_t aY, nsIContent *aContent,
                                 nsIFrame *aFrame, nsIPresShell *aPresShell,
                                 nsIWidget *aRootWidget);
||||||| merged common ancestors
  static void DispatchMouseEvent(mozilla::EventMessage aMessage,
                                 int32_t aX, int32_t aY,
                                 nsIContent *aContent, nsIFrame *aFrame,
                                 nsIPresShell *aPresShell, nsIWidget *aRootWidget);
=======
  MOZ_CAN_RUN_SCRIPT
  static void DispatchMouseEvent(mozilla::EventMessage aMessage, int32_t aX,
                                 int32_t aY, nsIContent* aContent,
                                 nsIFrame* aFrame, PresShell* aPresShell,
                                 nsIWidget* aRootWidget);
>>>>>>> upstream-releases

  /**
   * Send a touch event with a single touch point to the given element.
   *
   * @param aMessage     [in] an event message (see EventForwards.h)
   * @param aX           [in] x coordinate in dev pixels
   * @param aY           [in] y coordinate in dev pixels
   * @param aContent     [in] the element
   * @param aFrame       [in] frame of the element
   * @param aPresShell   [in] the presshell for the element
   * @param aRootWidget  [in] the root widget of the element
   */
<<<<<<< HEAD
  static void DispatchTouchEvent(mozilla::EventMessage aMessage, int32_t aX,
                                 int32_t aY, nsIContent *aContent,
                                 nsIFrame *aFrame, nsIPresShell *aPresShell,
                                 nsIWidget *aRootWidget);
||||||| merged common ancestors
  static void DispatchTouchEvent(mozilla::EventMessage aMessage,
                                 int32_t aX, int32_t aY,
                                 nsIContent* aContent, nsIFrame* aFrame,
                                 nsIPresShell* aPresShell, nsIWidget* aRootWidget);
=======
  MOZ_CAN_RUN_SCRIPT
  static void DispatchTouchEvent(mozilla::EventMessage aMessage, int32_t aX,
                                 int32_t aY, nsIContent* aContent,
                                 nsIFrame* aFrame, PresShell* aPresShell,
                                 nsIWidget* aRootWidget);
>>>>>>> upstream-releases

  /**
   * Return an accesskey registered on the given element by
   * EventStateManager or 0 if there is no registered accesskey.
   *
   * @param aContent - the given element.
   */
  static uint32_t GetAccessKeyFor(nsIContent* aContent);

  /**
   * Return DOM element related with the given node, i.e.
   * a) itself if it is DOM element
   * b) parent element if it is text node
   * c) otherwise nullptr
   *
   * @param aNode  [in] the given DOM node
   */
<<<<<<< HEAD
  static nsIContent *GetDOMElementFor(nsIContent *aContent);
||||||| merged common ancestors
  static nsIContent* GetDOMElementFor(nsIContent *aContent);
=======
  static nsIContent* GetDOMElementFor(nsIContent* aContent);
>>>>>>> upstream-releases

  /**
   * Return DOM node for the given DOM point.
   */
  static nsINode* GetDOMNodeFromDOMPoint(nsINode* aNode, uint32_t aOffset);

  /**
   * Is the first passed in node an ancestor of the second?
   * Note: A node is not considered to be the ancestor of itself.
   *
   * @param  aPossibleAncestorNode   [in] node to test for ancestor-ness of
   *                                   aPossibleDescendantNode
   * @param  aPossibleDescendantNode [in] node to test for descendant-ness of
   *                                   aPossibleAncestorNode
   * @param  aRootNode               [in, optional] the root node that search
   *                                   search should be performed within
   * @return true                     if aPossibleAncestorNode is an ancestor of
   *                                   aPossibleDescendantNode
   */
<<<<<<< HEAD
  static bool IsAncestorOf(nsINode *aPossibleAncestorNode,
                           nsINode *aPossibleDescendantNode,
                           nsINode *aRootNode = nullptr);
||||||| merged common ancestors
   static bool IsAncestorOf(nsINode *aPossibleAncestorNode,
                              nsINode *aPossibleDescendantNode,
                              nsINode *aRootNode = nullptr);
=======
  static bool IsAncestorOf(nsINode* aPossibleAncestorNode,
                           nsINode* aPossibleDescendantNode,
                           nsINode* aRootNode = nullptr);
>>>>>>> upstream-releases

  /**
   * Helper method to scroll range into view, used for implementation of
   * nsIAccessibleText::scrollSubstringTo().
   *
   * @param aFrame        the frame for accessible the range belongs to.
   * @param aRange    the range to scroll to
   * @param aScrollType   the place a range should be scrolled to
   */
  static nsresult ScrollSubstringTo(nsIFrame *aFrame, nsRange *aRange,
                                    uint32_t aScrollType);

  /** Helper method to scroll range into view, used for implementation of
   * nsIAccessibleText::scrollSubstringTo[Point]().
   *
   * @param aFrame        the frame for accessible the range belongs to.
   * @param aRange    the range to scroll to
   * @param aVertical     how to align vertically, specified in percents, and
   * when.
   * @param aHorizontal     how to align horizontally, specified in percents,
   * and when.
   */
<<<<<<< HEAD
  static nsresult ScrollSubstringTo(nsIFrame *aFrame, nsRange *aRange,
                                    nsIPresShell::ScrollAxis aVertical,
                                    nsIPresShell::ScrollAxis aHorizontal);
||||||| merged common ancestors
  static nsresult ScrollSubstringTo(nsIFrame* aFrame, nsRange* aRange,
                                    nsIPresShell::ScrollAxis aVertical,
                                    nsIPresShell::ScrollAxis aHorizontal);
=======
  static nsresult ScrollSubstringTo(nsIFrame* aFrame, nsRange* aRange,
                                    mozilla::ScrollAxis aVertical,
                                    mozilla::ScrollAxis aHorizontal);
>>>>>>> upstream-releases

  /**
   * Scrolls the given frame to the point, used for implememntation of
   * nsIAccessible::scrollToPoint and nsIAccessibleText::scrollSubstringToPoint.
   *
   * @param aScrollableFrame  the scrollable frame
   * @param aFrame            the frame to scroll
   * @param aPoint            the point scroll to
   */
<<<<<<< HEAD
  static void ScrollFrameToPoint(nsIFrame *aScrollableFrame, nsIFrame *aFrame,
                                 const nsIntPoint &aPoint);
||||||| merged common ancestors
  static void ScrollFrameToPoint(nsIFrame *aScrollableFrame,
                                 nsIFrame *aFrame, const nsIntPoint& aPoint);
=======
  static void ScrollFrameToPoint(nsIFrame* aScrollableFrame, nsIFrame* aFrame,
                                 const nsIntPoint& aPoint);
>>>>>>> upstream-releases

  /**
   * Converts scroll type constant defined in nsIAccessibleScrollType to
   * vertical and horizontal parameters.
   */
<<<<<<< HEAD
  static void ConvertScrollTypeToPercents(
      uint32_t aScrollType, nsIPresShell::ScrollAxis *aVertical,
      nsIPresShell::ScrollAxis *aHorizontal);
||||||| merged common ancestors
  static void ConvertScrollTypeToPercents(uint32_t aScrollType,
                                          nsIPresShell::ScrollAxis *aVertical,
                                          nsIPresShell::ScrollAxis *aHorizontal);
=======
  static void ConvertScrollTypeToPercents(uint32_t aScrollType,
                                          mozilla::ScrollAxis* aVertical,
                                          mozilla::ScrollAxis* aHorizontal);
>>>>>>> upstream-releases

  /**
   * Returns coordinates in device pixels relative screen for the top level
   * window.
   *
   * @param aNode  the DOM node hosted in the window.
   */
  static nsIntPoint GetScreenCoordsForWindow(nsINode* aNode);

  /**
   * Return document shell for the given DOM node.
   */
  static already_AddRefed<nsIDocShell> GetDocShellFor(nsINode* aNode);

  /**
   * Return true if the given document is root document.
   */
  static bool IsRootDocument(Document* aDocument);

  /**
   * Return true if the given document is content document (not chrome).
   */
  static bool IsContentDocument(Document* aDocument);

  /**
   * Return true if the given document node is for tab document accessible.
   */
<<<<<<< HEAD
  static bool IsTabDocument(nsIDocument *aDocumentNode);
||||||| merged common ancestors
  static bool IsTabDocument(nsIDocument* aDocumentNode);
=======
  static bool IsTabDocument(Document* aDocumentNode);
>>>>>>> upstream-releases

  /**
   * Return true if the given document is an error page.
   */
  static bool IsErrorPage(Document* aDocument);

  /**
   * Return presShell for the document containing the given DOM node.
   */
<<<<<<< HEAD
  static nsIPresShell *GetPresShellFor(nsINode *aNode) {
    return aNode->OwnerDoc()->GetShell();
||||||| merged common ancestors
  static nsIPresShell *GetPresShellFor(nsINode *aNode)
  {
    return aNode->OwnerDoc()->GetShell();
=======
  static PresShell* GetPresShellFor(nsINode* aNode) {
    return aNode->OwnerDoc()->GetPresShell();
>>>>>>> upstream-releases
  }

  /**
   * Get the ID for an element, in some types of XML this may not be the ID
   * attribute
   * @param aContent  Node to get the ID for
   * @param aID       Where to put ID string
   * @return          true if there is an ID set for this node
   */
<<<<<<< HEAD
  static bool GetID(nsIContent *aContent, nsAString &aID);
||||||| merged common ancestors
  static bool GetID(nsIContent *aContent, nsAString& aID);
=======
  static bool GetID(nsIContent* aContent, nsAString& aID);
>>>>>>> upstream-releases

  /**
   * Convert attribute value of the given node to positive integer. If no
   * attribute or wrong value then false is returned.
   */
<<<<<<< HEAD
  static bool GetUIntAttr(nsIContent *aContent, nsAtom *aAttr, int32_t *aUInt);
||||||| merged common ancestors
  static bool GetUIntAttr(nsIContent *aContent, nsAtom *aAttr,
                          int32_t* aUInt);
=======
  static bool GetUIntAttr(nsIContent* aContent, nsAtom* aAttr, int32_t* aUInt);
>>>>>>> upstream-releases

  /**
   * Returns language for the given node.
   *
   * @param aContent     [in] the given node
   * @param aRootContent [in] container of the given node
   * @param aLanguage    [out] language
   */
<<<<<<< HEAD
  static void GetLanguageFor(nsIContent *aContent, nsIContent *aRootContent,
                             nsAString &aLanguage);
||||||| merged common ancestors
  static void GetLanguageFor(nsIContent *aContent, nsIContent *aRootContent,
                             nsAString& aLanguage);
=======
  static void GetLanguageFor(nsIContent* aContent, nsIContent* aRootContent,
                             nsAString& aLanguage);
>>>>>>> upstream-releases

  /**
<<<<<<< HEAD
   * Return box object for XUL treechildren element by tree box object.
   */
  static already_AddRefed<nsIBoxObject> GetTreeBodyBoxObject(
      nsITreeBoxObject *aTreeBoxObj);

  /**
   * Return tree box object from any levels DOMNode under the XUL tree.
||||||| merged common ancestors
   * Return box object for XUL treechildren element by tree box object.
   */
  static already_AddRefed<nsIBoxObject>
    GetTreeBodyBoxObject(nsITreeBoxObject *aTreeBoxObj);

  /**
   * Return tree box object from any levels DOMNode under the XUL tree.
=======
   * Return tree from any levels DOMNode under the XUL tree.
>>>>>>> upstream-releases
   */
<<<<<<< HEAD
  static already_AddRefed<nsITreeBoxObject> GetTreeBoxObject(
      nsIContent *aContent);
||||||| merged common ancestors
  static already_AddRefed<nsITreeBoxObject>
    GetTreeBoxObject(nsIContent* aContent);
=======
  static mozilla::dom::XULTreeElement* GetTree(nsIContent* aContent);
>>>>>>> upstream-releases

  /**
   * Return first sensible column for the given tree box object.
   */
<<<<<<< HEAD
  static already_AddRefed<nsTreeColumn> GetFirstSensibleColumn(
      nsITreeBoxObject *aTree);
||||||| merged common ancestors
  static already_AddRefed<nsTreeColumn>
    GetFirstSensibleColumn(nsITreeBoxObject *aTree);
=======
  static already_AddRefed<nsTreeColumn> GetFirstSensibleColumn(
      mozilla::dom::XULTreeElement* aTree);
>>>>>>> upstream-releases

  /**
   * Return sensible columns count for the given tree box object.
   */
  static uint32_t GetSensibleColumnCount(mozilla::dom::XULTreeElement* aTree);

  /**
   * Return sensible column at the given index for the given tree box object.
   */
<<<<<<< HEAD
  static already_AddRefed<nsTreeColumn> GetSensibleColumnAt(
      nsITreeBoxObject *aTree, uint32_t aIndex);
||||||| merged common ancestors
  static already_AddRefed<nsTreeColumn>
    GetSensibleColumnAt(nsITreeBoxObject *aTree, uint32_t aIndex);
=======
  static already_AddRefed<nsTreeColumn> GetSensibleColumnAt(
      mozilla::dom::XULTreeElement* aTree, uint32_t aIndex);
>>>>>>> upstream-releases

  /**
   * Return next sensible column for the given column.
   */
<<<<<<< HEAD
  static already_AddRefed<nsTreeColumn> GetNextSensibleColumn(
      nsTreeColumn *aColumn);
||||||| merged common ancestors
  static already_AddRefed<nsTreeColumn>
    GetNextSensibleColumn(nsTreeColumn *aColumn);
=======
  static already_AddRefed<nsTreeColumn> GetNextSensibleColumn(
      nsTreeColumn* aColumn);
>>>>>>> upstream-releases

  /**
   * Return previous sensible column for the given column.
   */
<<<<<<< HEAD
  static already_AddRefed<nsTreeColumn> GetPreviousSensibleColumn(
      nsTreeColumn *aColumn);
||||||| merged common ancestors
  static already_AddRefed<nsTreeColumn>
    GetPreviousSensibleColumn(nsTreeColumn *aColumn);
=======
  static already_AddRefed<nsTreeColumn> GetPreviousSensibleColumn(
      nsTreeColumn* aColumn);
>>>>>>> upstream-releases

  /**
   * Return true if the given column is hidden (i.e. not sensible).
   */
  static bool IsColumnHidden(nsTreeColumn* aColumn);

  /**
   * Scroll content into view.
   */
<<<<<<< HEAD
  static void ScrollTo(nsIPresShell *aPresShell, nsIContent *aContent,
||||||| merged common ancestors
  static void ScrollTo(nsIPresShell* aPresShell, nsIContent* aContent,
=======
  MOZ_CAN_RUN_SCRIPT
  static void ScrollTo(PresShell* aPresShell, nsIContent* aContent,
>>>>>>> upstream-releases
                       uint32_t aScrollType);

  /**
   * Return true if the given node is table header element.
   */
<<<<<<< HEAD
  static bool IsHTMLTableHeader(nsIContent *aContent) {
||||||| merged common ancestors
  static bool IsHTMLTableHeader(nsIContent *aContent)
  {
=======
  static bool IsHTMLTableHeader(nsIContent* aContent) {
>>>>>>> upstream-releases
    return aContent->NodeInfo()->Equals(nsGkAtoms::th) ||
           (aContent->IsElement() && aContent->AsElement()->HasAttr(
                                         kNameSpaceID_None, nsGkAtoms::scope));
  }

  /**
   * Returns true if the given string is empty or contains whitespace symbols
   * only. In contrast to nsWhitespaceTokenizer class it takes into account
   * non-breaking space (0xa0).
   */
  static bool IsWhitespaceString(const nsAString &aString);

  /**
   * Returns true if the given character is whitespace symbol.
   */
  static bool IsWhitespace(char16_t aChar) {
    return aChar == ' ' || aChar == '\n' || aChar == '\r' || aChar == '\t' ||
           aChar == 0xa0;
  }

  /*
   * Return true if there are any observers of accessible events.
   */
  static bool AccEventObserversExist();

  /**
   * Notify accessible event observers of an event.
   */
  static void DispatchAccEvent(RefPtr<nsIAccessibleEvent> aEvent);
};

#endif
