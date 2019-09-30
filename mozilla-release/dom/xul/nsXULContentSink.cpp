/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * An implementation for a Gecko-style content sink that knows how
 * to build a content model (the "prototype" document) from XUL.
 *
 * For more information on XUL,
 * see http://developer.mozilla.org/en/docs/XUL
 */

#include "nsXULContentSink.h"

#include "jsfriendapi.h"

#include "nsCOMPtr.h"
#include "nsHTMLStyleSheet.h"
#include "nsIContentSink.h"
#include "mozilla/dom/Document.h"
#include "nsIDOMEventListener.h"
#include "nsIFormControl.h"
#include "mozilla/dom/NodeInfo.h"
#include "nsIScriptContext.h"
#include "nsIScriptGlobalObject.h"
#include "nsIServiceManager.h"
#include "nsIURL.h"
#include "nsNameSpaceManager.h"
#include "nsParserBase.h"
#include "nsViewManager.h"
#include "nsIScriptSecurityManager.h"
#include "nsLayoutCID.h"
#include "nsNetUtil.h"
#include "nsString.h"
#include "nsReadableUtils.h"
#include "nsXULElement.h"
#include "mozilla/Logging.h"
#include "nsCRT.h"

#include "nsXULPrototypeDocument.h"  // XXXbe temporary
#include "mozilla/css/Loader.h"

#include "nsUnicharUtils.h"
#include "nsGkAtoms.h"
#include "nsContentUtils.h"
#include "nsAttrName.h"
#include "nsXMLContentSink.h"
#include "nsIConsoleService.h"
#include "nsIScriptError.h"
#include "nsContentTypeParser.h"
#include "XULDocument.h"

<<<<<<< HEAD
static mozilla::LazyLogModule gContentSinkLog("nsXULContentSink");
;
||||||| merged common ancestors
static mozilla::LazyLogModule gContentSinkLog("nsXULContentSink");;
=======
static mozilla::LazyLogModule gContentSinkLog("nsXULContentSink");
>>>>>>> upstream-releases

//----------------------------------------------------------------------

XULContentSinkImpl::ContextStack::ContextStack() : mTop(nullptr), mDepth(0) {}

XULContentSinkImpl::ContextStack::~ContextStack() {
  while (mTop) {
    Entry* doomed = mTop;
    mTop = mTop->mNext;
    delete doomed;
  }
}

<<<<<<< HEAD
nsresult XULContentSinkImpl::ContextStack::Push(nsXULPrototypeNode* aNode,
                                                State aState) {
  Entry* entry = new Entry(aNode, aState, mTop);

  mTop = entry;

  ++mDepth;
  return NS_OK;
||||||| merged common ancestors
nsresult
XULContentSinkImpl::ContextStack::Push(nsXULPrototypeNode* aNode, State aState)
{
    Entry* entry = new Entry(aNode, aState, mTop);

    mTop = entry;

    ++mDepth;
    return NS_OK;
=======
void XULContentSinkImpl::ContextStack::Push(RefPtr<nsXULPrototypeNode>&& aNode,
                                            State aState) {
  mTop = new Entry(std::move(aNode), aState, mTop);
  ++mDepth;
>>>>>>> upstream-releases
}

nsresult XULContentSinkImpl::ContextStack::Pop(State* aState) {
  if (mDepth == 0) return NS_ERROR_UNEXPECTED;

  Entry* entry = mTop;
  mTop = mTop->mNext;
  --mDepth;

  *aState = entry->mState;
  delete entry;

  return NS_OK;
}

nsresult XULContentSinkImpl::ContextStack::GetTopNode(
    RefPtr<nsXULPrototypeNode>& aNode) {
  if (mDepth == 0) return NS_ERROR_UNEXPECTED;

  aNode = mTop->mNode;
  return NS_OK;
}

nsresult XULContentSinkImpl::ContextStack::GetTopChildren(
    nsPrototypeArray** aChildren) {
  if (mDepth == 0) return NS_ERROR_UNEXPECTED;

  *aChildren = &(mTop->mChildren);
  return NS_OK;
}

void XULContentSinkImpl::ContextStack::Clear() {
  Entry* cur = mTop;
  while (cur) {
    // Release the root element (and its descendants).
    Entry* next = cur->mNext;
    delete cur;
    cur = next;
  }

  mTop = nullptr;
  mDepth = 0;
}

void XULContentSinkImpl::ContextStack::Traverse(
    nsCycleCollectionTraversalCallback& aCb) {
  nsCycleCollectionTraversalCallback& cb = aCb;
  for (ContextStack::Entry* tmp = mTop; tmp; tmp = tmp->mNext) {
    NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mNode)
    NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mChildren)
  }
}

//----------------------------------------------------------------------

XULContentSinkImpl::XULContentSinkImpl()
    : mText(nullptr),
      mTextLength(0),
      mTextSize(0),
      mConstrainSize(true),
<<<<<<< HEAD
      mState(eInProlog) {}
||||||| merged common ancestors
      mState(eInProlog)
{
}

=======
      mState(eInProlog) {}

XULContentSinkImpl::~XULContentSinkImpl() {
  // The context stack _should_ be empty, unless something has gone wrong.
  NS_ASSERTION(mContextStack.Depth() == 0, "Context stack not empty?");
  mContextStack.Clear();
>>>>>>> upstream-releases

<<<<<<< HEAD
XULContentSinkImpl::~XULContentSinkImpl() {
  // The context stack _should_ be empty, unless something has gone wrong.
  NS_ASSERTION(mContextStack.Depth() == 0, "Context stack not empty?");
  mContextStack.Clear();

  free(mText);
||||||| merged common ancestors
XULContentSinkImpl::~XULContentSinkImpl()
{
    // The context stack _should_ be empty, unless something has gone wrong.
    NS_ASSERTION(mContextStack.Depth() == 0, "Context stack not empty?");
    mContextStack.Clear();

    free(mText);
=======
  free(mText);
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------
// nsISupports interface

NS_IMPL_CYCLE_COLLECTION_CLASS(XULContentSinkImpl)

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN(XULContentSinkImpl)
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mNodeInfoManager)
  tmp->mContextStack.Clear();
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mPrototype)
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mParser)
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN(XULContentSinkImpl)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mNodeInfoManager)
  tmp->mContextStack.Traverse(cb);
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mPrototype)
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mParser)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(XULContentSinkImpl)
  NS_INTERFACE_MAP_ENTRY_AMBIGUOUS(nsISupports, nsIXMLContentSink)
  NS_INTERFACE_MAP_ENTRY(nsIXMLContentSink)
  NS_INTERFACE_MAP_ENTRY(nsIExpatSink)
  NS_INTERFACE_MAP_ENTRY(nsIContentSink)
NS_INTERFACE_MAP_END

NS_IMPL_CYCLE_COLLECTING_ADDREF(XULContentSinkImpl)
NS_IMPL_CYCLE_COLLECTING_RELEASE(XULContentSinkImpl)

//----------------------------------------------------------------------
// nsIContentSink interface

NS_IMETHODIMP
XULContentSinkImpl::WillBuildModel(nsDTDMode aDTDMode) {
#if FIXME
  if (!mParentContentSink) {
    // If we're _not_ an overlay, then notify the document that
    // the load is beginning.
    mDocument->BeginLoad();
  }
#endif

  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
XULContentSinkImpl::DidBuildModel(bool aTerminated) {
  nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument);
  if (doc) {
    doc->EndLoad();
    mDocument = nullptr;
  }
||||||| merged common ancestors
XULContentSinkImpl::DidBuildModel(bool aTerminated)
{
    nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument);
    if (doc) {
        doc->EndLoad();
        mDocument = nullptr;
    }
=======
XULContentSinkImpl::DidBuildModel(bool aTerminated) {
  nsCOMPtr<Document> doc = do_QueryReferent(mDocument);
  if (doc) {
    mPrototype->NotifyLoadDone();
    mDocument = nullptr;
  }
>>>>>>> upstream-releases

  // Drop our reference to the parser to get rid of a circular
  // reference.
  mParser = nullptr;
  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::WillInterrupt(void) {
  // XXX Notify the docshell, if necessary
  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::WillResume(void) {
  // XXX Notify the docshell, if necessary
  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::SetParser(nsParserBase* aParser) {
  mParser = aParser;
  return NS_OK;
}

<<<<<<< HEAD
void XULContentSinkImpl::SetDocumentCharset(
    NotNull<const Encoding*> aEncoding) {
  nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument);
  if (doc) {
    doc->SetDocumentCharacterSet(aEncoding);
  }
||||||| merged common ancestors
void
XULContentSinkImpl::SetDocumentCharset(NotNull<const Encoding*> aEncoding)
{
    nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument);
    if (doc) {
        doc->SetDocumentCharacterSet(aEncoding);
    }
=======
void XULContentSinkImpl::SetDocumentCharset(
    NotNull<const Encoding*> aEncoding) {
  nsCOMPtr<Document> doc = do_QueryReferent(mDocument);
  if (doc) {
    doc->SetDocumentCharacterSet(aEncoding);
  }
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsISupports* XULContentSinkImpl::GetTarget() {
  nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument);
  return doc;
||||||| merged common ancestors
nsISupports *
XULContentSinkImpl::GetTarget()
{
    nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument);
    return doc;
=======
nsISupports* XULContentSinkImpl::GetTarget() {
  nsCOMPtr<Document> doc = do_QueryReferent(mDocument);
  return ToSupports(doc);
>>>>>>> upstream-releases
}

//----------------------------------------------------------------------

<<<<<<< HEAD
nsresult XULContentSinkImpl::Init(nsIDocument* aDocument,
                                  nsXULPrototypeDocument* aPrototype) {
  MOZ_ASSERT(aDocument != nullptr, "null ptr");
  if (!aDocument) return NS_ERROR_NULL_POINTER;
||||||| merged common ancestors
nsresult
XULContentSinkImpl::Init(nsIDocument* aDocument,
                         nsXULPrototypeDocument* aPrototype)
{
    MOZ_ASSERT(aDocument != nullptr, "null ptr");
    if (! aDocument)
        return NS_ERROR_NULL_POINTER;
=======
nsresult XULContentSinkImpl::Init(Document* aDocument,
                                  nsXULPrototypeDocument* aPrototype) {
  MOZ_ASSERT(aDocument != nullptr, "null ptr");
  if (!aDocument) return NS_ERROR_NULL_POINTER;
>>>>>>> upstream-releases

  mDocument = do_GetWeakReference(aDocument);
  mPrototype = aPrototype;

  mDocumentURL = mPrototype->GetURI();
  mNodeInfoManager = aPrototype->GetNodeInfoManager();
  if (!mNodeInfoManager) return NS_ERROR_UNEXPECTED;

  mState = eInProlog;
  return NS_OK;
}

//----------------------------------------------------------------------
//
// Text buffering
//

bool XULContentSinkImpl::IsDataInBuffer(char16_t* buffer, int32_t length) {
  for (int32_t i = 0; i < length; ++i) {
    if (buffer[i] == ' ' || buffer[i] == '\t' || buffer[i] == '\n' ||
        buffer[i] == '\r')
      continue;

    return true;
  }
  return false;
}

nsresult XULContentSinkImpl::FlushText(bool aCreateTextNode) {
  nsresult rv;

  do {
    // Don't do anything if there's no text to create a node from, or
    // if they've told us not to create a text node
    if (!mTextLength) break;

    if (!aCreateTextNode) break;

    RefPtr<nsXULPrototypeNode> node;
    rv = mContextStack.GetTopNode(node);
    if (NS_FAILED(rv)) return rv;

    bool stripWhitespace = false;
    if (node->mType == nsXULPrototypeNode::eType_Element) {
      mozilla::dom::NodeInfo* nodeInfo =
          static_cast<nsXULPrototypeElement*>(node.get())->mNodeInfo;

      if (nodeInfo->NamespaceEquals(kNameSpaceID_XUL))
        stripWhitespace = !nodeInfo->Equals(nsGkAtoms::label) &&
                          !nodeInfo->Equals(nsGkAtoms::description);
    }

    // Don't bother if there's nothing but whitespace.
    if (stripWhitespace && !IsDataInBuffer(mText, mTextLength)) break;

    // Don't bother if we're not in XUL document body
    if (mState != eInDocumentElement || mContextStack.Depth() == 0) break;

<<<<<<< HEAD
    nsXULPrototypeText* text = new nsXULPrototypeText();
    text->mValue.Assign(mText, mTextLength);
    if (stripWhitespace) text->mValue.Trim(" \t\n\r");
||||||| merged common ancestors
        nsXULPrototypeText* text = new nsXULPrototypeText();
        text->mValue.Assign(mText, mTextLength);
        if (stripWhitespace)
            text->mValue.Trim(" \t\n\r");
=======
    RefPtr<nsXULPrototypeText> text = new nsXULPrototypeText();
    text->mValue.Assign(mText, mTextLength);
    if (stripWhitespace) text->mValue.Trim(" \t\n\r");
>>>>>>> upstream-releases

    // hook it up
    nsPrototypeArray* children = nullptr;
    rv = mContextStack.GetTopChildren(&children);
    if (NS_FAILED(rv)) return rv;

<<<<<<< HEAD
    // transfer ownership of 'text' to the children array
    children->AppendElement(text);
  } while (0);
||||||| merged common ancestors
        // transfer ownership of 'text' to the children array
        children->AppendElement(text);
    } while (0);
=======
    children->AppendElement(text.forget());
  } while (0);
>>>>>>> upstream-releases

  // Reset our text buffer
  mTextLength = 0;
  return NS_OK;
}

//----------------------------------------------------------------------

<<<<<<< HEAD
nsresult XULContentSinkImpl::NormalizeAttributeString(
    const char16_t* aExpatName, nsAttrName& aName) {
  int32_t nameSpaceID;
  RefPtr<nsAtom> prefix, localName;
  nsContentUtils::SplitExpatName(aExpatName, getter_AddRefs(prefix),
                                 getter_AddRefs(localName), &nameSpaceID);

  if (nameSpaceID == kNameSpaceID_None) {
    aName.SetTo(localName);

    return NS_OK;
  }
||||||| merged common ancestors
nsresult
XULContentSinkImpl::NormalizeAttributeString(const char16_t *aExpatName,
                                             nsAttrName &aName)
{
    int32_t nameSpaceID;
    RefPtr<nsAtom> prefix, localName;
    nsContentUtils::SplitExpatName(aExpatName, getter_AddRefs(prefix),
                                   getter_AddRefs(localName), &nameSpaceID);

    if (nameSpaceID == kNameSpaceID_None) {
        aName.SetTo(localName);

        return NS_OK;
    }
=======
nsresult XULContentSinkImpl::NormalizeAttributeString(
    const char16_t* aExpatName, nsAttrName& aName) {
  int32_t nameSpaceID;
  RefPtr<nsAtom> prefix, localName;
  nsContentUtils::SplitExpatName(aExpatName, getter_AddRefs(prefix),
                                 getter_AddRefs(localName), &nameSpaceID);
>>>>>>> upstream-releases

<<<<<<< HEAD
  RefPtr<mozilla::dom::NodeInfo> ni;
  ni = mNodeInfoManager->GetNodeInfo(localName, prefix, nameSpaceID,
                                     nsINode::ATTRIBUTE_NODE);
  aName.SetTo(ni);
||||||| merged common ancestors
    RefPtr<mozilla::dom::NodeInfo> ni;
    ni = mNodeInfoManager->GetNodeInfo(localName, prefix,
                                       nameSpaceID,
                                       nsINode::ATTRIBUTE_NODE);
    aName.SetTo(ni);
=======
  if (nameSpaceID == kNameSpaceID_None) {
    aName.SetTo(localName);
>>>>>>> upstream-releases

<<<<<<< HEAD
  return NS_OK;
}
||||||| merged common ancestors
    return NS_OK;
}
=======
    return NS_OK;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult XULContentSinkImpl::CreateElement(mozilla::dom::NodeInfo* aNodeInfo,
                                           nsXULPrototypeElement** aResult) {
  nsXULPrototypeElement* element = new nsXULPrototypeElement();
  element->mNodeInfo = aNodeInfo;
||||||| merged common ancestors
nsresult
XULContentSinkImpl::CreateElement(mozilla::dom::NodeInfo *aNodeInfo,
                                  nsXULPrototypeElement** aResult)
{
    nsXULPrototypeElement* element = new nsXULPrototypeElement();
    element->mNodeInfo    = aNodeInfo;
=======
  RefPtr<mozilla::dom::NodeInfo> ni;
  ni = mNodeInfoManager->GetNodeInfo(localName, prefix, nameSpaceID,
                                     nsINode::ATTRIBUTE_NODE);
  aName.SetTo(ni);
>>>>>>> upstream-releases

<<<<<<< HEAD
  *aResult = element;
  return NS_OK;
||||||| merged common ancestors
    *aResult = element;
    return NS_OK;
=======
  return NS_OK;
>>>>>>> upstream-releases
}

/**** BEGIN NEW APIs ****/

NS_IMETHODIMP
XULContentSinkImpl::HandleStartElement(const char16_t* aName,
                                       const char16_t** aAtts,
                                       uint32_t aAttsCount,
                                       uint32_t aLineNumber,
                                       uint32_t aColumnNumber) {
  // XXX Hopefully the parser will flag this before we get here. If
  // we're in the epilog, there should be no new elements
  MOZ_ASSERT(mState != eInEpilog, "tag in XUL doc epilog");
  MOZ_ASSERT(aAttsCount % 2 == 0, "incorrect aAttsCount");

  // Adjust aAttsCount so it's the actual number of attributes
  aAttsCount /= 2;

  if (mState == eInEpilog) return NS_ERROR_UNEXPECTED;

  if (mState != eInScript) {
    FlushText();
  }

  int32_t nameSpaceID;
  RefPtr<nsAtom> prefix, localName;
  nsContentUtils::SplitExpatName(aName, getter_AddRefs(prefix),
                                 getter_AddRefs(localName), &nameSpaceID);

  RefPtr<mozilla::dom::NodeInfo> nodeInfo;
  nodeInfo = mNodeInfoManager->GetNodeInfo(localName, prefix, nameSpaceID,
                                           nsINode::ELEMENT_NODE);

  nsresult rv = NS_OK;
  switch (mState) {
    case eInProlog:
      // We're the root document element
      rv = OpenRoot(aAtts, aAttsCount, nodeInfo);
      break;

    case eInDocumentElement:
      rv = OpenTag(aAtts, aAttsCount, aLineNumber, nodeInfo);
      break;

    case eInEpilog:
    case eInScript:
      MOZ_LOG(
          gContentSinkLog, LogLevel::Warning,
          ("xul: warning: unexpected tags in epilog at line %d", aLineNumber));
      rv = NS_ERROR_UNEXPECTED;  // XXX
      break;
  }

  return rv;
}

NS_IMETHODIMP
XULContentSinkImpl::HandleEndElement(const char16_t* aName) {
  // Never EVER return anything but NS_OK or
  // NS_ERROR_HTMLPARSER_BLOCK from this method. Doing so will blow
  // the parser's little mind all over the planet.
  nsresult rv;

  RefPtr<nsXULPrototypeNode> node;
  rv = mContextStack.GetTopNode(node);

  if (NS_FAILED(rv)) {
    return NS_OK;
  }

  switch (node->mType) {
    case nsXULPrototypeNode::eType_Element: {
      // Flush any text _now_, so that we'll get text nodes created
      // before popping the stack.
      FlushText();

      // Pop the context stack and do prototype hookup.
      nsPrototypeArray* children = nullptr;
      rv = mContextStack.GetTopChildren(&children);
      if (NS_FAILED(rv)) return rv;

      nsXULPrototypeElement* element =
          static_cast<nsXULPrototypeElement*>(node.get());

      int32_t count = children->Length();
      if (count) {
        element->mChildren.SetCapacity(count);

        for (int32_t i = 0; i < count; ++i)
          element->mChildren.AppendElement(children->ElementAt(i));
      }
    } break;

    case nsXULPrototypeNode::eType_Script: {
      nsXULPrototypeScript* script =
          static_cast<nsXULPrototypeScript*>(node.get());

<<<<<<< HEAD
      // If given a src= attribute, we must ignore script tag content.
      if (!script->mSrcURI && !script->HasScriptObject()) {
        nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument);
||||||| merged common ancestors
        // If given a src= attribute, we must ignore script tag content.
        if (!script->mSrcURI && !script->HasScriptObject()) {
            nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument);
=======
      // If given a src= attribute, we must ignore script tag content.
      if (!script->mSrcURI && !script->HasScriptObject()) {
        nsCOMPtr<Document> doc = do_QueryReferent(mDocument);
>>>>>>> upstream-releases

        script->mOutOfLine = false;
        if (doc) {
          script->Compile(mText, mTextLength, JS::SourceOwnership::Borrowed,
                          mDocumentURL, script->mLineNo, doc);
        }
      }

      FlushText(false);
    } break;

    default:
      NS_ERROR("didn't expect that");
      break;
  }

  rv = mContextStack.Pop(&mState);
  NS_ASSERTION(NS_SUCCEEDED(rv), "context stack corrupted");
  if (NS_FAILED(rv)) return rv;

  if (mContextStack.Depth() == 0) {
    // The root element should -always- be an element, because
    // it'll have been created via XULContentSinkImpl::OpenRoot().
    NS_ASSERTION(node->mType == nsXULPrototypeNode::eType_Element,
                 "root is not an element");
    if (node->mType != nsXULPrototypeNode::eType_Element)
      return NS_ERROR_UNEXPECTED;

    // Now that we're done parsing, set the prototype document's
    // root element. This transfers ownership of the prototype
    // element tree to the prototype document.
    nsXULPrototypeElement* element =
        static_cast<nsXULPrototypeElement*>(node.get());

    mPrototype->SetRootElement(element);
    mState = eInEpilog;
  }

  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::HandleComment(const char16_t* aName) {
  FlushText();
  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::HandleCDataSection(const char16_t* aData,
                                       uint32_t aLength) {
  FlushText();
  return AddText(aData, aLength);
}

NS_IMETHODIMP
XULContentSinkImpl::HandleDoctypeDecl(const nsAString& aSubset,
                                      const nsAString& aName,
                                      const nsAString& aSystemId,
                                      const nsAString& aPublicId,
                                      nsISupports* aCatalogData) {
  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::HandleCharacterData(const char16_t* aData,
                                        uint32_t aLength) {
  if (aData && mState != eInProlog && mState != eInEpilog) {
    return AddText(aData, aLength);
  }
  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::HandleProcessingInstruction(const char16_t* aTarget,
                                                const char16_t* aData) {
  FlushText();

  const nsDependentString target(aTarget);
  const nsDependentString data(aData);

  // Note: the created nsXULPrototypePI has mRefCnt == 1
  RefPtr<nsXULPrototypePI> pi = new nsXULPrototypePI();
  pi->mTarget = target;
  pi->mData = data;

  if (mState == eInProlog) {
    // Note: passing in already addrefed pi
    return mPrototype->AddProcessingInstruction(pi);
  }

  nsresult rv;
  nsPrototypeArray* children = nullptr;
  rv = mContextStack.GetTopChildren(&children);
  if (NS_FAILED(rv)) {
    return rv;
  }

  if (!children->AppendElement(pi)) {
    return NS_ERROR_OUT_OF_MEMORY;
  }

  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::HandleXMLDeclaration(const char16_t* aVersion,
                                         const char16_t* aEncoding,
                                         int32_t aStandalone) {
  return NS_OK;
}

NS_IMETHODIMP
XULContentSinkImpl::ReportError(const char16_t* aErrorText,
                                const char16_t* aSourceText,
                                nsIScriptError* aError, bool* _retval) {
  MOZ_ASSERT(aError && aSourceText && aErrorText, "Check arguments!!!");

  // The expat driver should report the error.
  *_retval = true;

  nsresult rv = NS_OK;

  // make sure to empty the context stack so that
  // <parsererror> could become the root element.
  mContextStack.Clear();

  mState = eInProlog;

  // Clear any buffered-up text we have.  It's enough to set the length to 0.
  // The buffer itself is allocated when we're created and deleted in our
  // destructor, so don't mess with it.
  mTextLength = 0;

<<<<<<< HEAD
  // return leaving the document empty if we're asked to not add a <parsererror>
  // root node
  nsCOMPtr<nsIDocument> idoc = do_QueryReferent(mDocument);
||||||| merged common ancestors
  // return leaving the document empty if we're asked to not add a <parsererror> root node
  nsCOMPtr<nsIDocument> idoc = do_QueryReferent(mDocument);
=======
  // return leaving the document empty if we're asked to not add a <parsererror>
  // root node
  nsCOMPtr<Document> idoc = do_QueryReferent(mDocument);
>>>>>>> upstream-releases
  if (idoc && idoc->SuppressParserErrorElement()) {
    return NS_OK;
  };

  const char16_t* noAtts[] = {0, 0};

  NS_NAMED_LITERAL_STRING(
      errorNs, "http://www.mozilla.org/newlayout/xml/parsererror.xml");

  nsAutoString parsererror(errorNs);
  parsererror.Append((char16_t)0xFFFF);
  parsererror.AppendLiteral("parsererror");

  rv = HandleStartElement(parsererror.get(), noAtts, 0, 0, 0);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = HandleCharacterData(aErrorText, NS_strlen(aErrorText));
  NS_ENSURE_SUCCESS(rv, rv);

  nsAutoString sourcetext(errorNs);
  sourcetext.Append((char16_t)0xFFFF);
  sourcetext.AppendLiteral("sourcetext");

  rv = HandleStartElement(sourcetext.get(), noAtts, 0, 0, 0);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = HandleCharacterData(aSourceText, NS_strlen(aSourceText));
  NS_ENSURE_SUCCESS(rv, rv);

  rv = HandleEndElement(sourcetext.get());
  NS_ENSURE_SUCCESS(rv, rv);

  rv = HandleEndElement(parsererror.get());
  NS_ENSURE_SUCCESS(rv, rv);

  return rv;
}

<<<<<<< HEAD
nsresult XULContentSinkImpl::OpenRoot(const char16_t** aAttributes,
                                      const uint32_t aAttrLen,
                                      mozilla::dom::NodeInfo* aNodeInfo) {
  NS_ASSERTION(mState == eInProlog, "how'd we get here?");
  if (mState != eInProlog) return NS_ERROR_UNEXPECTED;

  nsresult rv;
||||||| merged common ancestors
nsresult
XULContentSinkImpl::OpenRoot(const char16_t** aAttributes,
                             const uint32_t aAttrLen,
                             mozilla::dom::NodeInfo *aNodeInfo)
{
    NS_ASSERTION(mState == eInProlog, "how'd we get here?");
    if (mState != eInProlog)
        return NS_ERROR_UNEXPECTED;

    nsresult rv;

    if (aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XHTML) ||
        aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XUL)) {
        MOZ_LOG(gContentSinkLog, LogLevel::Error,
               ("xul: script tag not allowed as root content element"));
=======
nsresult XULContentSinkImpl::OpenRoot(const char16_t** aAttributes,
                                      const uint32_t aAttrLen,
                                      mozilla::dom::NodeInfo* aNodeInfo) {
  NS_ASSERTION(mState == eInProlog, "how'd we get here?");
  if (mState != eInProlog) return NS_ERROR_UNEXPECTED;

  if (aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XHTML) ||
      aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XUL)) {
    MOZ_LOG(gContentSinkLog, LogLevel::Error,
            ("xul: script tag not allowed as root content element"));
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XHTML) ||
      aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XUL)) {
    MOZ_LOG(gContentSinkLog, LogLevel::Error,
            ("xul: script tag not allowed as root content element"));
||||||| merged common ancestors
        return NS_ERROR_UNEXPECTED;
    }
=======
    return NS_ERROR_UNEXPECTED;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
    return NS_ERROR_UNEXPECTED;
  }
||||||| merged common ancestors
    // Create the element
    nsXULPrototypeElement* element;
    rv = CreateElement(aNodeInfo, &element);
=======
  // Create the element
  RefPtr<nsXULPrototypeElement> element = new nsXULPrototypeElement(aNodeInfo);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Create the element
  nsXULPrototypeElement* element;
  rv = CreateElement(aNodeInfo, &element);
||||||| merged common ancestors
    if (NS_FAILED(rv)) {
        if (MOZ_LOG_TEST(gContentSinkLog, LogLevel::Error)) {
            nsAutoString anodeC;
            aNodeInfo->GetName(anodeC);
            MOZ_LOG(gContentSinkLog, LogLevel::Error,
                   ("xul: unable to create element '%s' at line %d",
                    NS_ConvertUTF16toUTF8(anodeC).get(),
                    -1)); // XXX pass in line number
        }
=======
  // Add the attributes
  nsresult rv = AddAttributes(aAttributes, aAttrLen, element);
  if (NS_FAILED(rv)) return rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (NS_FAILED(rv)) {
    if (MOZ_LOG_TEST(gContentSinkLog, LogLevel::Error)) {
      nsAutoString anodeC;
      aNodeInfo->GetName(anodeC);
      MOZ_LOG(gContentSinkLog, LogLevel::Error,
              ("xul: unable to create element '%s' at line %d",
               NS_ConvertUTF16toUTF8(anodeC).get(),
               -1));  // XXX pass in line number
    }
||||||| merged common ancestors
        return rv;
    }
=======
  // Push the element onto the context stack, so that child
  // containers will hook up to us as their parent.
  mContextStack.Push(std::move(element), mState);
>>>>>>> upstream-releases

<<<<<<< HEAD
    return rv;
  }

  // Push the element onto the context stack, so that child
  // containers will hook up to us as their parent.
  rv = mContextStack.Push(element, mState);
  if (NS_FAILED(rv)) {
    element->Release();
    return rv;
  }

  // Add the attributes
  rv = AddAttributes(aAttributes, aAttrLen, element);
  if (NS_FAILED(rv)) return rv;

  mState = eInDocumentElement;
  return NS_OK;
||||||| merged common ancestors
    // Push the element onto the context stack, so that child
    // containers will hook up to us as their parent.
    rv = mContextStack.Push(element, mState);
    if (NS_FAILED(rv)) {
        element->Release();
        return rv;
    }

    // Add the attributes
    rv = AddAttributes(aAttributes, aAttrLen, element);
    if (NS_FAILED(rv)) return rv;

    mState = eInDocumentElement;
    return NS_OK;
=======
  mState = eInDocumentElement;
  return NS_OK;
>>>>>>> upstream-releases
}

<<<<<<< HEAD
nsresult XULContentSinkImpl::OpenTag(const char16_t** aAttributes,
                                     const uint32_t aAttrLen,
                                     const uint32_t aLineNumber,
                                     mozilla::dom::NodeInfo* aNodeInfo) {
  nsresult rv;

  // Create the element
  nsXULPrototypeElement* element;
  rv = CreateElement(aNodeInfo, &element);

  if (NS_FAILED(rv)) {
    if (MOZ_LOG_TEST(gContentSinkLog, LogLevel::Error)) {
      nsAutoString anodeC;
      aNodeInfo->GetName(anodeC);
      MOZ_LOG(gContentSinkLog, LogLevel::Error,
              ("xul: unable to create element '%s' at line %d",
               NS_ConvertUTF16toUTF8(anodeC).get(), aLineNumber));
    }
||||||| merged common ancestors
nsresult
XULContentSinkImpl::OpenTag(const char16_t** aAttributes,
                            const uint32_t aAttrLen,
                            const uint32_t aLineNumber,
                            mozilla::dom::NodeInfo *aNodeInfo)
{
    nsresult rv;

    // Create the element
    nsXULPrototypeElement* element;
    rv = CreateElement(aNodeInfo, &element);

    if (NS_FAILED(rv)) {
        if (MOZ_LOG_TEST(gContentSinkLog, LogLevel::Error)) {
            nsAutoString anodeC;
            aNodeInfo->GetName(anodeC);
            MOZ_LOG(gContentSinkLog, LogLevel::Error,
                   ("xul: unable to create element '%s' at line %d",
                    NS_ConvertUTF16toUTF8(anodeC).get(),
                    aLineNumber));
        }

        return rv;
    }
=======
nsresult XULContentSinkImpl::OpenTag(const char16_t** aAttributes,
                                     const uint32_t aAttrLen,
                                     const uint32_t aLineNumber,
                                     mozilla::dom::NodeInfo* aNodeInfo) {
  // Create the element
  RefPtr<nsXULPrototypeElement> element = new nsXULPrototypeElement(aNodeInfo);
>>>>>>> upstream-releases

<<<<<<< HEAD
    return rv;
  }
||||||| merged common ancestors
    // Link this element to its parent.
    nsPrototypeArray* children = nullptr;
    rv = mContextStack.GetTopChildren(&children);
    if (NS_FAILED(rv)) {
        delete element;
        return rv;
    }
=======
  // Link this element to its parent.
  nsPrototypeArray* children = nullptr;
  nsresult rv = mContextStack.GetTopChildren(&children);
  if (NS_FAILED(rv)) {
    return rv;
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Link this element to its parent.
  nsPrototypeArray* children = nullptr;
  rv = mContextStack.GetTopChildren(&children);
  if (NS_FAILED(rv)) {
    delete element;
    return rv;
  }
||||||| merged common ancestors
    // Add the attributes
    rv = AddAttributes(aAttributes, aAttrLen, element);
    if (NS_FAILED(rv)) return rv;
=======
  // Add the attributes
  rv = AddAttributes(aAttributes, aAttrLen, element);
  if (NS_FAILED(rv)) return rv;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Add the attributes
  rv = AddAttributes(aAttributes, aAttrLen, element);
  if (NS_FAILED(rv)) return rv;
||||||| merged common ancestors
    children->AppendElement(element);
=======
  children->AppendElement(element);
>>>>>>> upstream-releases

<<<<<<< HEAD
  children->AppendElement(element);
||||||| merged common ancestors
    if (aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XHTML) ||
        aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XUL)) {
        // Do scripty things now
        rv = OpenScript(aAttributes, aLineNumber);
        NS_ENSURE_SUCCESS(rv, rv);
=======
  if (aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XHTML) ||
      aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XUL)) {
    // Do scripty things now
    rv = OpenScript(aAttributes, aLineNumber);
    NS_ENSURE_SUCCESS(rv, rv);
>>>>>>> upstream-releases

<<<<<<< HEAD
  if (aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XHTML) ||
      aNodeInfo->Equals(nsGkAtoms::script, kNameSpaceID_XUL)) {
    // Do scripty things now
    rv = OpenScript(aAttributes, aLineNumber);
    NS_ENSURE_SUCCESS(rv, rv);

    NS_ASSERTION(mState == eInScript || mState == eInDocumentElement,
                 "Unexpected state");
    if (mState == eInScript) {
      // OpenScript has pushed the nsPrototypeScriptElement onto the
      // stack, so we're done.
      return NS_OK;
||||||| merged common ancestors
        NS_ASSERTION(mState == eInScript || mState == eInDocumentElement,
                     "Unexpected state");
        if (mState == eInScript) {
            // OpenScript has pushed the nsPrototypeScriptElement onto the
            // stack, so we're done.
            return NS_OK;
        }
=======
    NS_ASSERTION(mState == eInScript || mState == eInDocumentElement,
                 "Unexpected state");
    if (mState == eInScript) {
      // OpenScript has pushed the nsPrototypeScriptElement onto the
      // stack, so we're done.
      return NS_OK;
>>>>>>> upstream-releases
    }
  }

<<<<<<< HEAD
  // Push the element onto the context stack, so that child
  // containers will hook up to us as their parent.
  rv = mContextStack.Push(element, mState);
  if (NS_FAILED(rv)) return rv;
||||||| merged common ancestors
    // Push the element onto the context stack, so that child
    // containers will hook up to us as their parent.
    rv = mContextStack.Push(element, mState);
    if (NS_FAILED(rv)) return rv;
=======
  // Push the element onto the context stack, so that child
  // containers will hook up to us as their parent.
  mContextStack.Push(std::move(element), mState);
>>>>>>> upstream-releases

  mState = eInDocumentElement;
  return NS_OK;
}

nsresult XULContentSinkImpl::OpenScript(const char16_t** aAttributes,
                                        const uint32_t aLineNumber) {
  bool isJavaScript = true;
  nsresult rv;

  // Look for SRC attribute and look for a LANGUAGE attribute
  nsAutoString src;
  while (*aAttributes) {
<<<<<<< HEAD
    const nsDependentString key(aAttributes[0]);
    if (key.EqualsLiteral("src")) {
      src.Assign(aAttributes[1]);
    } else if (key.EqualsLiteral("type")) {
      nsDependentString str(aAttributes[1]);
      nsContentTypeParser parser(str);
      nsAutoString mimeType;
      rv = parser.GetType(mimeType);
      if (NS_FAILED(rv)) {
        if (rv == NS_ERROR_INVALID_ARG) {
          // Fail immediately rather than checking if later things
          // are okay.
          return NS_OK;
        }
        // We do want the warning here
        NS_ENSURE_SUCCESS(rv, rv);
      }

      if (nsContentUtils::IsJavascriptMIMEType(mimeType)) {
        isJavaScript = true;

        // Get the version string, and ensure that JavaScript supports it.
        nsAutoString versionName;
        rv = parser.GetParameter("version", versionName);

        if (NS_SUCCEEDED(rv)) {
          nsContentUtils::ReportToConsoleNonLocalized(
              NS_LITERAL_STRING(
                  "Versioned JavaScripts are no longer supported. "
                  "Please remove the version parameter."),
              nsIScriptError::errorFlag, NS_LITERAL_CSTRING("XUL Document"),
              nullptr, mDocumentURL, EmptyString(), aLineNumber);
          isJavaScript = false;
        } else if (rv != NS_ERROR_INVALID_ARG) {
          return rv;
        }
      } else {
        isJavaScript = false;
      }
    } else if (key.EqualsLiteral("language")) {
      // Language is deprecated, and the impl in ScriptLoader ignores the
      // various version strings anyway.  So we make no attempt to support
      // languages other than JS for language=
      nsAutoString lang(aAttributes[1]);
      if (nsContentUtils::IsJavaScriptLanguage(lang)) {
        isJavaScript = true;
||||||| merged common ancestors
      const nsDependentString key(aAttributes[0]);
      if (key.EqualsLiteral("src")) {
          src.Assign(aAttributes[1]);
      } else if (key.EqualsLiteral("type")) {
          nsDependentString str(aAttributes[1]);
          nsContentTypeParser parser(str);
          nsAutoString mimeType;
          rv = parser.GetType(mimeType);
          if (NS_FAILED(rv)) {
              if (rv == NS_ERROR_INVALID_ARG) {
                  // Fail immediately rather than checking if later things
                  // are okay.
                  return NS_OK;
              }
              // We do want the warning here
              NS_ENSURE_SUCCESS(rv, rv);
          }

          if (nsContentUtils::IsJavascriptMIMEType(mimeType)) {
              isJavaScript = true;

              // Get the version string, and ensure that JavaScript supports it.
              nsAutoString versionName;
              rv = parser.GetParameter("version", versionName);

              if (NS_SUCCEEDED(rv)) {
                  nsContentUtils::ReportToConsoleNonLocalized(
                      NS_LITERAL_STRING("Versioned JavaScripts are no longer supported. "
                                        "Please remove the version parameter."),
                      nsIScriptError::errorFlag,
                      NS_LITERAL_CSTRING("XUL Document"),
                      nullptr, mDocumentURL, EmptyString(), aLineNumber);
                  isJavaScript = false;
              } else if (rv != NS_ERROR_INVALID_ARG) {
                  return rv;
              }
          } else {
              isJavaScript = false;
          }
      } else if (key.EqualsLiteral("language")) {
          // Language is deprecated, and the impl in ScriptLoader ignores the
          // various version strings anyway.  So we make no attempt to support
          // languages other than JS for language=
          nsAutoString lang(aAttributes[1]);
          if (nsContentUtils::IsJavaScriptLanguage(lang)) {
              isJavaScript = true;
          }
=======
    const nsDependentString key(aAttributes[0]);
    if (key.EqualsLiteral("src")) {
      src.Assign(aAttributes[1]);
    } else if (key.EqualsLiteral("type")) {
      nsDependentString str(aAttributes[1]);
      nsContentTypeParser parser(str);
      nsAutoString mimeType;
      rv = parser.GetType(mimeType);
      if (NS_FAILED(rv)) {
        if (rv == NS_ERROR_INVALID_ARG) {
          // Fail immediately rather than checking if later things
          // are okay.
          return NS_OK;
        }
        // We do want the warning here
        NS_ENSURE_SUCCESS(rv, rv);
      }

      if (nsContentUtils::IsJavascriptMIMEType(mimeType)) {
        isJavaScript = true;

        // Get the version string, and ensure that JavaScript supports it.
        nsAutoString versionName;
        rv = parser.GetParameter("version", versionName);

        if (NS_SUCCEEDED(rv)) {
          nsContentUtils::ReportToConsoleNonLocalized(
              NS_LITERAL_STRING(
                  "Versioned JavaScripts are no longer supported. "
                  "Please remove the version parameter."),
              nsIScriptError::errorFlag, NS_LITERAL_CSTRING("XUL Document"),
              nullptr, mDocumentURL, EmptyString(), aLineNumber);
          isJavaScript = false;
        } else if (rv != NS_ERROR_INVALID_ARG) {
          return rv;
        }
      } else {
        isJavaScript = false;
>>>>>>> upstream-releases
      }
<<<<<<< HEAD
    }
    aAttributes += 2;
||||||| merged common ancestors
      aAttributes += 2;
=======
    } else if (key.EqualsLiteral("language")) {
      // Language is deprecated, and the impl in ScriptLoader ignores the
      // various version strings anyway.  So we make no attempt to support
      // languages other than JS for language=
      nsAutoString lang(aAttributes[1]);
      if (nsContentUtils::IsJavaScriptLanguage(lang)) {
        isJavaScript = true;
      }
    }
    aAttributes += 2;
>>>>>>> upstream-releases
  }

  // Don't process scripts that aren't JavaScript.
  if (!isJavaScript) {
    return NS_OK;
  }

  nsCOMPtr<Document> doc(do_QueryReferent(mDocument));
  nsCOMPtr<nsIScriptGlobalObject> globalObject;
  if (doc) globalObject = do_QueryInterface(doc->GetWindow());
  RefPtr<nsXULPrototypeScript> script = new nsXULPrototypeScript(aLineNumber);

  // If there is a SRC attribute...
  if (!src.IsEmpty()) {
    // Use the SRC attribute value to load the URL
    rv = NS_NewURI(getter_AddRefs(script->mSrcURI), src, nullptr, mDocumentURL);

    // Check if this document is allowed to load a script from this source
    // NOTE: if we ever allow scripts added via the DOM to run, we need to
    // add a CheckLoadURI call for that as well.
    if (NS_SUCCEEDED(rv)) {
      if (!mSecMan)
        mSecMan = do_GetService(NS_SCRIPTSECURITYMANAGER_CONTRACTID, &rv);
      if (NS_SUCCEEDED(rv)) {
<<<<<<< HEAD
        nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument, &rv);
||||||| merged common ancestors
          if (!mSecMan)
              mSecMan = do_GetService(NS_SCRIPTSECURITYMANAGER_CONTRACTID, &rv);
          if (NS_SUCCEEDED(rv)) {
              nsCOMPtr<nsIDocument> doc = do_QueryReferent(mDocument, &rv);

              if (NS_SUCCEEDED(rv)) {
                  rv = mSecMan->
                      CheckLoadURIWithPrincipal(doc->NodePrincipal(),
                                                script->mSrcURI,
                                                nsIScriptSecurityManager::ALLOW_CHROME);
              }
          }
      }
=======
        nsCOMPtr<Document> doc = do_QueryReferent(mDocument, &rv);
>>>>>>> upstream-releases

        if (NS_SUCCEEDED(rv)) {
          rv = mSecMan->CheckLoadURIWithPrincipal(
              doc->NodePrincipal(), script->mSrcURI,
              nsIScriptSecurityManager::ALLOW_CHROME);
        }
      }
<<<<<<< HEAD
    }
||||||| merged common ancestors
=======
    }

    if (NS_FAILED(rv)) {
      return rv;
    }
>>>>>>> upstream-releases

<<<<<<< HEAD
    if (NS_FAILED(rv)) {
      return rv;
    }

    // Attempt to deserialize an out-of-line script from the FastLoad
    // file right away.  Otherwise we'll end up reloading the script and
    // corrupting the FastLoad file trying to serialize it, in the case
    // where it's already there.
    script->DeserializeOutOfLine(nullptr, mPrototype);
||||||| merged common ancestors
      // Attempt to deserialize an out-of-line script from the FastLoad
      // file right away.  Otherwise we'll end up reloading the script and
      // corrupting the FastLoad file trying to serialize it, in the case
      // where it's already there.
      script->DeserializeOutOfLine(nullptr, mPrototype);
=======
    // Attempt to deserialize an out-of-line script from the FastLoad
    // file right away.  Otherwise we'll end up reloading the script and
    // corrupting the FastLoad file trying to serialize it, in the case
    // where it's already there.
    script->DeserializeOutOfLine(nullptr, mPrototype);
>>>>>>> upstream-releases
  }

  nsPrototypeArray* children = nullptr;
  rv = mContextStack.GetTopChildren(&children);
  if (NS_FAILED(rv)) {
    return rv;
  }

  children->AppendElement(script);

  mConstrainSize = false;

  mContextStack.Push(script, mState);
  mState = eInScript;

  return NS_OK;
}

nsresult XULContentSinkImpl::AddAttributes(const char16_t** aAttributes,
                                           const uint32_t aAttrLen,
                                           nsXULPrototypeElement* aElement) {
  // Add tag attributes to the element
  nsresult rv;

  // Create storage for the attributes
  nsXULPrototypeAttribute* attrs = nullptr;
  if (aAttrLen > 0) {
    attrs = new nsXULPrototypeAttribute[aAttrLen];
  }

  aElement->mAttributes = attrs;
  aElement->mNumAttributes = aAttrLen;

  // Copy the attributes into the prototype
  uint32_t i;
  for (i = 0; i < aAttrLen; ++i) {
    rv = NormalizeAttributeString(aAttributes[i * 2], attrs[i].mName);
    NS_ENSURE_SUCCESS(rv, rv);

    rv = aElement->SetAttrAt(i, nsDependentString(aAttributes[i * 2 + 1]),
                             mDocumentURL);
    NS_ENSURE_SUCCESS(rv, rv);

    if (MOZ_LOG_TEST(gContentSinkLog, LogLevel::Debug)) {
      nsAutoString extraWhiteSpace;
      int32_t cnt = mContextStack.Depth();
      while (--cnt >= 0) extraWhiteSpace.AppendLiteral("  ");
      nsAutoString qnameC, valueC;
      qnameC.Assign(aAttributes[0]);
      valueC.Assign(aAttributes[1]);
      MOZ_LOG(gContentSinkLog, LogLevel::Debug,
              ("xul: %.5d. %s    %s=%s",
               -1,  // XXX pass in line number
               NS_ConvertUTF16toUTF8(extraWhiteSpace).get(),
               NS_ConvertUTF16toUTF8(qnameC).get(),
               NS_ConvertUTF16toUTF8(valueC).get()));
    }
  }

  return NS_OK;
}

nsresult XULContentSinkImpl::AddText(const char16_t* aText, int32_t aLength) {
  // Create buffer when we first need it
  if (0 == mTextSize) {
    mText = (char16_t*)malloc(sizeof(char16_t) * 4096);
    if (nullptr == mText) {
      return NS_ERROR_OUT_OF_MEMORY;
    }
    mTextSize = 4096;
  }

  // Copy data from string into our buffer; flush buffer when it fills up
  int32_t offset = 0;
  while (0 != aLength) {
    int32_t amount = mTextSize - mTextLength;
    if (amount > aLength) {
      amount = aLength;
    }
    if (0 == amount) {
      if (mConstrainSize) {
        nsresult rv = FlushText();
        if (NS_OK != rv) {
          return rv;
        }
      } else {
        CheckedInt32 size = mTextSize;
        size += aLength;
        if (!size.isValid()) {
          return NS_ERROR_OUT_OF_MEMORY;
        }
        mTextSize = size.value();

        mText = (char16_t*)realloc(mText, sizeof(char16_t) * mTextSize);
        if (nullptr == mText) {
          return NS_ERROR_OUT_OF_MEMORY;
        }
      }
    }
    memcpy(&mText[mTextLength], aText + offset, sizeof(char16_t) * amount);

    mTextLength += amount;
    offset += amount;
    aLength -= amount;
  }

  return NS_OK;
}
