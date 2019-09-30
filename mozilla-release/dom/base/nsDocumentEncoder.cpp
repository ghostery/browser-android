/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Object that can be used to serialize selections, ranges, or nodes
 * to strings in a gazillion different ways.
 */

#include "nsIDocumentEncoder.h"

#include <utility>

#include "nscore.h"
#include "nsIFactory.h"
#include "nsISupports.h"
#include "mozilla/dom/Document.h"
#include "nsCOMPtr.h"
#include "nsIContentSerializer.h"
#include "mozilla/Encoding.h"
#include "nsIOutputStream.h"
#include "nsRange.h"
#include "nsGkAtoms.h"
#include "nsHTMLDocument.h"
#include "nsIContent.h"
#include "nsIScriptContext.h"
#include "nsIScriptGlobalObject.h"
#include "nsIScriptSecurityManager.h"
#include "mozilla/dom/Selection.h"
#include "nsITransferable.h"  // for kUnicodeMime
#include "nsContentUtils.h"
#include "nsElementTable.h"
#include "nsNodeUtils.h"
#include "nsUnicharUtils.h"
#include "nsReadableUtils.h"
#include "nsTArray.h"
#include "nsIFrame.h"
#include "nsStringBuffer.h"
#include "mozilla/dom/Comment.h"
#include "mozilla/dom/DocumentType.h"
#include "mozilla/dom/Element.h"
#include "mozilla/dom/ProcessingInstruction.h"
#include "mozilla/dom/ShadowRoot.h"
#include "mozilla/dom/Text.h"
#include "nsLayoutUtils.h"
#include "mozilla/Maybe.h"
#include "mozilla/ScopeExit.h"

using namespace mozilla;
using namespace mozilla::dom;

<<<<<<< HEAD
enum nsRangeIterationDirection { kDirectionOut = -1, kDirectionIn = 1 };
||||||| merged common ancestors
enum nsRangeIterationDirection {
  kDirectionOut = -1,
  kDirectionIn = 1
};
=======
enum nsRangeIterationDirection { kDirectionOut = -1, kDirectionIn = 1 };

class TextStreamer {
 public:
  /**
   * @param aStream Will be kept alive by the TextStreamer.
   * @param aUnicodeEncoder Needs to be non-nullptr.
   */
  TextStreamer(nsIOutputStream& aStream, UniquePtr<Encoder> aUnicodeEncoder,
               bool aIsPlainText);

  /**
   * @param aString Will be truncated if aString is written to stream.
   */
  nsresult FlushIfStringLongEnough(nsAString& aString);

  /**
   * @param aString Will be truncated.
   */
  nsresult ForceFlush(nsAString& aString);

 private:
  const static uint32_t kMaxLengthBeforeFlush = 1024;

  const static uint32_t kEncoderBufferSizeInBytes = 4096;

  nsresult EncodeAndWrite(const nsAString& aString);

  nsresult EncodeAndWriteAndTruncate(nsAString& aString);

  const nsCOMPtr<nsIOutputStream> mStream;
  const UniquePtr<Encoder> mUnicodeEncoder;
  const bool mIsPlainText;
};

TextStreamer::TextStreamer(nsIOutputStream& aStream,
                           UniquePtr<Encoder> aUnicodeEncoder,
                           bool aIsPlainText)
    : mStream{&aStream},
      mUnicodeEncoder(std::move(aUnicodeEncoder)),
      mIsPlainText(aIsPlainText) {
  MOZ_ASSERT(mUnicodeEncoder);
}

nsresult TextStreamer::FlushIfStringLongEnough(nsAString& aString) {
  nsresult rv = NS_OK;

  if (aString.Length() > kMaxLengthBeforeFlush) {
    rv = EncodeAndWriteAndTruncate(aString);
  }

  return rv;
}

nsresult TextStreamer::ForceFlush(nsAString& aString) {
  return EncodeAndWriteAndTruncate(aString);
}

nsresult TextStreamer::EncodeAndWrite(const nsAString& aString) {
  if (aString.IsEmpty()) {
    return NS_OK;
  }

  uint8_t buffer[kEncoderBufferSizeInBytes];
  auto src = MakeSpan(aString);
  auto bufferSpan = MakeSpan(buffer);
  // Reserve space for terminator
  auto dst = bufferSpan.To(bufferSpan.Length() - 1);
  for (;;) {
    uint32_t result;
    size_t read;
    size_t written;
    bool hadErrors;
    if (mIsPlainText) {
      Tie(result, read, written) =
          mUnicodeEncoder->EncodeFromUTF16WithoutReplacement(src, dst, false);
      if (result != kInputEmpty && result != kOutputFull) {
        // There's always room for one byte in the case of
        // an unmappable character, because otherwise
        // we'd have gotten `kOutputFull`.
        dst[written++] = '?';
      }
    } else {
      Tie(result, read, written, hadErrors) =
          mUnicodeEncoder->EncodeFromUTF16(src, dst, false);
    }
    Unused << hadErrors;
    src = src.From(read);
    // Sadly, we still have test cases that implement nsIOutputStream in JS, so
    // the buffer needs to be zero-terminated for XPConnect to do its thing.
    // See bug 170416.
    bufferSpan[written] = 0;
    uint32_t streamWritten;
    nsresult rv = mStream->Write(reinterpret_cast<char*>(dst.Elements()),
                                 written, &streamWritten);
    if (NS_FAILED(rv)) {
      return rv;
    }
    if (result == kInputEmpty) {
      return NS_OK;
    }
  }
}

nsresult TextStreamer::EncodeAndWriteAndTruncate(nsAString& aString) {
  const nsresult rv = EncodeAndWrite(aString);
  aString.Truncate();
  return rv;
}

/**
 * The scope may be limited to either a selection, range, or node.
 */
class EncodingScope {
 public:
  /**
   * @return true, iff the scope is limited to a selection, range or node.
   */
  bool IsLimited() const;

  RefPtr<Selection> mSelection;
  RefPtr<nsRange> mRange;
  nsCOMPtr<nsINode> mNode;
  bool mNodeIsContainer = false;
};

bool EncodingScope::IsLimited() const { return mSelection || mRange || mNode; }

struct RangeBoundaryPathsAndOffsets {
  using ContainerPath = AutoTArray<nsIContent*, 8>;
  using ContainerOffsets = AutoTArray<int32_t, 8>;

  // The first node is the range's boundary node, the following ones the
  // ancestors.
  ContainerPath mStartContainerPath;
  // The first offset represents where at the boundary node the range starts.
  // Each other offset is the index of the child relative to its parent.
  ContainerOffsets mStartContainerOffsets;

  // The first node is the range's boundary node, the following one the
  // ancestors.
  ContainerPath mEndContainerPath;
  // The first offset represents where at the boundary node the range ends.
  // Each other offset is the index of the child relative to its parent.
  ContainerOffsets mEndContainerOffsets;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
class nsDocumentEncoder : public nsIDocumentEncoder {
 public:
||||||| merged common ancestors
class nsDocumentEncoder : public nsIDocumentEncoder
{
public:
=======
struct ContextInfoDepth {
  uint32_t mStart = 0;
  uint32_t mEnd = 0;
};

class nsDocumentEncoder : public nsIDocumentEncoder {
 public:
>>>>>>> upstream-releases
  nsDocumentEncoder();

  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_CLASS(nsDocumentEncoder)
  NS_DECL_NSIDOCUMENTENCODER

 protected:
  virtual ~nsDocumentEncoder();

  void Initialize(bool aClearCachedSerializer = true);

  /**
   * @param aMaxLength As described at
   * `nsIDocumentEncodder.encodeToStringWithMaxLength`.
   */
  nsresult SerializeDependingOnScope(nsAString& aOutput, uint32_t aMaxLength);

  nsresult SerializeSelection(nsAString& aOutput);

  nsresult SerializeNode(nsAString& aOutput);

  /**
   * @param aMaxLength As described at
   * `nsIDocumentEncodder.encodeToStringWithMaxLength`.
   */
  nsresult SerializeWholeDocument(nsAString& aOutput, uint32_t aMaxLength);

  nsresult SerializeNodeStart(nsINode& aOriginalNode, int32_t aStartOffset,
                              int32_t aEndOffset, nsAString& aStr,
<<<<<<< HEAD
                              nsINode* aOriginalNode = nullptr);
  nsresult SerializeToStringRecursive(nsINode* aNode, nsAString& aStr,
||||||| merged common ancestors
                              nsINode* aOriginalNode = nullptr);
  nsresult SerializeToStringRecursive(nsINode* aNode,
                                      nsAString& aStr,
=======
                              nsINode* aFixupNode = nullptr);
  nsresult SerializeToStringRecursive(nsINode* aNode, nsAString& aStr,
>>>>>>> upstream-releases
                                      bool aDontSerializeRoot,
                                      uint32_t aMaxLength = 0);
  nsresult SerializeNodeEnd(nsINode& aNode, nsAString& aStr);
  // This serializes the content of aNode.
  nsresult SerializeToStringIterative(nsINode* aNode, nsAString& aStr);
  nsresult SerializeRangeToString(nsRange* aRange, nsAString& aOutputString);
  nsresult SerializeRangeNodes(nsRange* aRange, nsINode* aNode,
                               nsAString& aString, int32_t aDepth);
  nsresult SerializeRangeContextStart(const nsTArray<nsINode*>& aAncestorArray,
                                      nsAString& aString);
  nsresult SerializeRangeContextEnd(nsAString& aString);

  virtual int32_t GetImmediateContextCount(
      const nsTArray<nsINode*>& aAncestorArray) {
    return -1;
  }

<<<<<<< HEAD
  nsresult FlushText(nsAString& aString, bool aForce);

  bool IsVisibleNode(nsINode* aNode) {
    MOZ_ASSERT(aNode, "null node");

||||||| merged common ancestors
  nsresult FlushText(nsAString& aString, bool aForce);

  bool IsVisibleNode(nsINode* aNode)
  {
    MOZ_ASSERT(aNode, "null node");

=======
  bool IsInvisibleNodeAndShouldBeSkipped(nsINode& aNode) const {
>>>>>>> upstream-releases
    if (mFlags & SkipInvisibleContent) {
      // Treat the visibility of the ShadowRoot as if it were
      // the host content.
      //
      // FIXME(emilio): I suspect instead of this a bunch of the GetParent()
      // calls here should be doing GetFlattenedTreeParent, then this condition
      // should be unreachable...
      nsINode* node{&aNode};
      if (ShadowRoot* shadowRoot = ShadowRoot::FromNode(node)) {
        node = shadowRoot->GetHost();
      }

      if (node->IsContent()) {
        nsIFrame* frame = node->AsContent()->GetPrimaryFrame();
        if (!frame) {
          if (node->IsElement() && node->AsElement()->IsDisplayContents()) {
            return false;
          }
          if (node->IsText()) {
            // We have already checked that our parent is visible.
            //
            // FIXME(emilio): Text not assigned to a <slot> in Shadow DOM should
            // probably return false...
            return false;
          }
          if (node->IsHTMLElement(nsGkAtoms::rp)) {
            // Ruby parentheses are part of ruby structure, hence
            // shouldn't be stripped out even if it is not displayed.
            return false;
          }
          return true;
        }
        bool isVisible = frame->StyleVisibility()->IsVisible();
<<<<<<< HEAD
        if (!isVisible && aNode->IsText()) return false;
||||||| merged common ancestors
        if (!isVisible && aNode->IsText())
          return false;
=======
        if (!isVisible && node->IsText()) {
          return true;
        }
>>>>>>> upstream-releases
      }
    }
    return false;
  }

  virtual bool IncludeInContext(nsINode* aNode);

  void ReleaseDocumentReferenceAndInitialize(bool aClearCachedSerializer);

  class MOZ_STACK_CLASS AutoReleaseDocumentIfNeeded final {
   public:
    explicit AutoReleaseDocumentIfNeeded(nsDocumentEncoder* aEncoder)
        : mEncoder(aEncoder) {}

    ~AutoReleaseDocumentIfNeeded() {
      if (mEncoder->mFlags & RequiresReinitAfterOutput) {
        const bool clearCachedSerializer = false;
        mEncoder->ReleaseDocumentReferenceAndInitialize(clearCachedSerializer);
      }
    }

   private:
    nsDocumentEncoder* mEncoder;
  };

<<<<<<< HEAD
  nsCOMPtr<nsIDocument> mDocument;
  RefPtr<Selection> mSelection;
  RefPtr<nsRange> mRange;
  nsCOMPtr<nsINode> mNode;
  nsCOMPtr<nsIOutputStream> mStream;
||||||| merged common ancestors
  nsCOMPtr<nsIDocument>          mDocument;
  RefPtr<Selection>              mSelection;
  RefPtr<nsRange>              mRange;
  nsCOMPtr<nsINode>              mNode;
  nsCOMPtr<nsIOutputStream>      mStream;
=======
  nsCOMPtr<Document> mDocument;
  EncodingScope mEncodingScope;
>>>>>>> upstream-releases
  nsCOMPtr<nsIContentSerializer> mSerializer;
<<<<<<< HEAD
  UniquePtr<Encoder> mUnicodeEncoder;
  nsCOMPtr<nsINode> mCommonParent;
||||||| merged common ancestors
  UniquePtr<Encoder> mUnicodeEncoder;
  nsCOMPtr<nsINode>              mCommonParent;
=======
  Maybe<TextStreamer> mTextStreamer;
  nsCOMPtr<nsINode> mCommonAncestorOfRange;
>>>>>>> upstream-releases
  nsCOMPtr<nsIDocumentEncoderNodeFixup> mNodeFixup;

  nsString mMimeType;
  const Encoding* mEncoding;
<<<<<<< HEAD
  uint32_t mFlags;
  uint32_t mWrapColumn;
  uint32_t mStartDepth;
  uint32_t mEndDepth;
  int32_t mStartRootIndex;
  int32_t mEndRootIndex;
  AutoTArray<nsINode*, 8> mCommonAncestors;
  AutoTArray<nsIContent*, 8> mStartNodes;
  AutoTArray<int32_t, 8> mStartOffsets;
  AutoTArray<nsIContent*, 8> mEndNodes;
  AutoTArray<int32_t, 8> mEndOffsets;
||||||| merged common ancestors
  uint32_t          mFlags;
  uint32_t          mWrapColumn;
  uint32_t          mStartDepth;
  uint32_t          mEndDepth;
  int32_t           mStartRootIndex;
  int32_t           mEndRootIndex;
  AutoTArray<nsINode*, 8>    mCommonAncestors;
  AutoTArray<nsIContent*, 8> mStartNodes;
  AutoTArray<int32_t, 8>     mStartOffsets;
  AutoTArray<nsIContent*, 8> mEndNodes;
  AutoTArray<int32_t, 8>     mEndOffsets;
=======
  uint32_t mFlags;
  uint32_t mWrapColumn;
  ContextInfoDepth mContextInfoDepth;
  int32_t mStartRootIndex;
  int32_t mEndRootIndex;
  AutoTArray<nsINode*, 8> mCommonAncestors;
  RangeBoundaryPathsAndOffsets mRangeBoundaryPathsAndOffsets;
>>>>>>> upstream-releases
  AutoTArray<AutoTArray<nsINode*, 8>, 8> mRangeContexts;
  // Whether the serializer cares about being notified to scan elements to
  // keep track of whether they are preformatted.  This stores the out
  // argument of nsIContentSerializer::Init().
  bool mNeedsPreformatScanning;
  bool mHaltRangeHint;
  // Used when context has already been serialized for
  // table cell selections (where parent is <tr>)
<<<<<<< HEAD
  bool mDisableContextSerialize;
  bool mIsCopying;  // Set to true only while copying
  bool mNodeIsContainer;
  bool mIsPlainText;
  nsStringBuffer* mCachedBuffer;
||||||| merged common ancestors
  bool              mDisableContextSerialize;
  bool              mIsCopying;  // Set to true only while copying
  bool              mNodeIsContainer;
  bool mIsPlainText;
  nsStringBuffer*   mCachedBuffer;
=======
  bool mDisableContextSerialize;
  bool mIsCopying;  // Set to true only while copying
  nsStringBuffer* mCachedBuffer;
>>>>>>> upstream-releases
};

NS_IMPL_CYCLE_COLLECTING_ADDREF(nsDocumentEncoder)
NS_IMPL_CYCLE_COLLECTING_RELEASE_WITH_LAST_RELEASE(
    nsDocumentEncoder, ReleaseDocumentReferenceAndInitialize(true))

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION(nsDocumentEncoder)
  NS_INTERFACE_MAP_ENTRY(nsIDocumentEncoder)
  NS_INTERFACE_MAP_ENTRY(nsISupports)
NS_INTERFACE_MAP_END

<<<<<<< HEAD
NS_IMPL_CYCLE_COLLECTION(nsDocumentEncoder, mDocument, mSelection, mRange,
                         mNode, mSerializer, mCommonParent)
||||||| merged common ancestors
NS_IMPL_CYCLE_COLLECTION(nsDocumentEncoder,
                         mDocument, mSelection, mRange, mNode, mSerializer,
                         mCommonParent)
=======
NS_IMPL_CYCLE_COLLECTION(nsDocumentEncoder, mDocument,
                         mEncodingScope.mSelection, mEncodingScope.mRange,
                         mEncodingScope.mNode, mSerializer,
                         mCommonAncestorOfRange)
>>>>>>> upstream-releases

nsDocumentEncoder::nsDocumentEncoder()
    : mEncoding(nullptr), mIsCopying(false), mCachedBuffer(nullptr) {
  Initialize();
  mMimeType.AssignLiteral("text/plain");
}

void nsDocumentEncoder::Initialize(bool aClearCachedSerializer) {
  mFlags = 0;
  mWrapColumn = 72;
  mContextInfoDepth = {};
  mStartRootIndex = 0;
  mEndRootIndex = 0;
  mNeedsPreformatScanning = false;
  mHaltRangeHint = false;
  mDisableContextSerialize = false;
  mEncodingScope = {};
  mCommonAncestorOfRange = nullptr;
  mNodeFixup = nullptr;
  mRangeBoundaryPathsAndOffsets = {};
  if (aClearCachedSerializer) {
    mSerializer = nullptr;
  }
}

<<<<<<< HEAD
nsDocumentEncoder::~nsDocumentEncoder() {
||||||| merged common ancestors
nsDocumentEncoder::~nsDocumentEncoder()
{
=======
static bool ParentIsTR(nsIContent* aContent) {
  mozilla::dom::Element* parent = aContent->GetParentElement();
  if (!parent) {
    return false;
  }
  return parent->IsHTMLElement(nsGkAtoms::tr);
}

nsresult nsDocumentEncoder::SerializeDependingOnScope(nsAString& aOutput,
                                                      uint32_t aMaxLength) {
  nsresult rv = NS_OK;
  if (mEncodingScope.mSelection) {
    rv = SerializeSelection(aOutput);
  } else if (nsRange* range = mEncodingScope.mRange) {
    rv = SerializeRangeToString(range, aOutput);
  } else if (mEncodingScope.mNode) {
    rv = SerializeNode(aOutput);
  } else {
    rv = SerializeWholeDocument(aOutput, aMaxLength);
  }

  mEncodingScope = {};

  return rv;
}

nsresult nsDocumentEncoder::SerializeSelection(nsAString& aOutput) {
  NS_ENSURE_TRUE(mEncodingScope.mSelection, NS_ERROR_FAILURE);

  nsresult rv = NS_OK;
  Selection* selection = mEncodingScope.mSelection;
  uint32_t count = selection->RangeCount();

  nsCOMPtr<nsINode> node;
  nsCOMPtr<nsINode> prevNode;
  uint32_t firstRangeStartDepth = 0;
  for (uint32_t i = 0; i < count; ++i) {
    RefPtr<nsRange> range = selection->GetRangeAt(i);

    // Bug 236546: newlines not added when copying table cells into clipboard
    // Each selected cell shows up as a range containing a row with a single
    // cell get the row, compare it to previous row and emit </tr><tr> as
    // needed Bug 137450: Problem copying/pasting a table from a web page to
    // Excel. Each separate block of <tr></tr> produced above will be wrapped
    // by the immediate context. This assumes that you can't select cells that
    // are multiple selections from two tables simultaneously.
    node = range->GetStartContainer();
    NS_ENSURE_TRUE(node, NS_ERROR_FAILURE);
    if (node != prevNode) {
      if (prevNode) {
        rv = SerializeNodeEnd(*prevNode, aOutput);
        NS_ENSURE_SUCCESS(rv, rv);
      }
      nsCOMPtr<nsIContent> content = do_QueryInterface(node);
      if (content && content->IsHTMLElement(nsGkAtoms::tr) &&
          !ParentIsTR(content)) {
        if (!prevNode) {
          // Went from a non-<tr> to a <tr>
          mCommonAncestors.Clear();
          nsContentUtils::GetAncestors(node->GetParentNode(), mCommonAncestors);
          rv = SerializeRangeContextStart(mCommonAncestors, aOutput);
          NS_ENSURE_SUCCESS(rv, rv);
          // Don't let SerializeRangeToString serialize the context again
          mDisableContextSerialize = true;
        }

        rv = SerializeNodeStart(*node, 0, -1, aOutput);
        NS_ENSURE_SUCCESS(rv, rv);
        prevNode = node;
      } else if (prevNode) {
        // Went from a <tr> to a non-<tr>
        mDisableContextSerialize = false;
        rv = SerializeRangeContextEnd(aOutput);
        NS_ENSURE_SUCCESS(rv, rv);
        prevNode = nullptr;
      }
    }

    rv = SerializeRangeToString(range, aOutput);
    NS_ENSURE_SUCCESS(rv, rv);
    if (i == 0) {
      firstRangeStartDepth = mContextInfoDepth.mStart;
    }
  }
  mContextInfoDepth.mStart = firstRangeStartDepth;

  if (prevNode) {
    rv = SerializeNodeEnd(*prevNode, aOutput);
    NS_ENSURE_SUCCESS(rv, rv);
    mDisableContextSerialize = false;
    rv = SerializeRangeContextEnd(aOutput);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  // Just to be safe
  mDisableContextSerialize = false;

  return rv;
}

nsresult nsDocumentEncoder::SerializeNode(nsAString& aOutput) {
  NS_ENSURE_TRUE(mEncodingScope.mNode, NS_ERROR_FAILURE);

  nsresult rv = NS_OK;
  nsINode* node = mEncodingScope.mNode;
  const bool nodeIsContainer = mEncodingScope.mNodeIsContainer;
  if (!mNodeFixup && !(mFlags & SkipInvisibleContent) && !mTextStreamer &&
      nodeIsContainer) {
    rv = SerializeToStringIterative(node, aOutput);
  } else {
    rv = SerializeToStringRecursive(node, aOutput, nodeIsContainer);
  }

  return rv;
}

nsresult nsDocumentEncoder::SerializeWholeDocument(nsAString& aOutput,
                                                   uint32_t aMaxLength) {
  NS_ENSURE_FALSE(mEncodingScope.mSelection, NS_ERROR_FAILURE);
  NS_ENSURE_FALSE(mEncodingScope.mRange, NS_ERROR_FAILURE);
  NS_ENSURE_FALSE(mEncodingScope.mNode, NS_ERROR_FAILURE);

  nsresult rv = mSerializer->AppendDocumentStart(mDocument, aOutput);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = SerializeToStringRecursive(mDocument, aOutput, false, aMaxLength);
  return rv;
}

nsDocumentEncoder::~nsDocumentEncoder() {
>>>>>>> upstream-releases
  if (mCachedBuffer) {
    mCachedBuffer->Release();
  }
}

NS_IMETHODIMP
<<<<<<< HEAD
nsDocumentEncoder::Init(nsIDocument* aDocument, const nsAString& aMimeType,
                        uint32_t aFlags) {
||||||| merged common ancestors
nsDocumentEncoder::Init(nsIDocument* aDocument,
                        const nsAString& aMimeType,
                        uint32_t aFlags)
{
=======
nsDocumentEncoder::Init(Document* aDocument, const nsAString& aMimeType,
                        uint32_t aFlags) {
>>>>>>> upstream-releases
  return NativeInit(aDocument, aMimeType, aFlags);
}

NS_IMETHODIMP
<<<<<<< HEAD
nsDocumentEncoder::NativeInit(nsIDocument* aDocument,
                              const nsAString& aMimeType, uint32_t aFlags) {
  if (!aDocument) return NS_ERROR_INVALID_ARG;
||||||| merged common ancestors
nsDocumentEncoder::NativeInit(nsIDocument* aDocument,
                              const nsAString& aMimeType,
                              uint32_t aFlags)
{
  if (!aDocument)
    return NS_ERROR_INVALID_ARG;
=======
nsDocumentEncoder::NativeInit(Document* aDocument, const nsAString& aMimeType,
                              uint32_t aFlags) {
  if (!aDocument) return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases

  Initialize(!mMimeType.Equals(aMimeType));

  mDocument = aDocument;

  mMimeType = aMimeType;

  mFlags = aFlags;
  mIsCopying = false;

  return NS_OK;
}

NS_IMETHODIMP
nsDocumentEncoder::SetWrapColumn(uint32_t aWC) {
  mWrapColumn = aWC;
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsDocumentEncoder::SetSelection(Selection* aSelection) {
  mSelection = aSelection;
||||||| merged common ancestors
nsDocumentEncoder::SetSelection(Selection* aSelection)
{
  mSelection = aSelection;
=======
nsDocumentEncoder::SetSelection(Selection* aSelection) {
  mEncodingScope.mSelection = aSelection;
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsDocumentEncoder::SetRange(nsRange* aRange) {
  mRange = aRange;
||||||| merged common ancestors
nsDocumentEncoder::SetRange(nsRange* aRange)
{
  mRange = aRange;
=======
nsDocumentEncoder::SetRange(nsRange* aRange) {
  mEncodingScope.mRange = aRange;
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsDocumentEncoder::SetNode(nsINode* aNode) {
  mNodeIsContainer = false;
  mNode = aNode;
||||||| merged common ancestors
nsDocumentEncoder::SetNode(nsINode* aNode)
{
  mNodeIsContainer = false;
  mNode = aNode;
=======
nsDocumentEncoder::SetNode(nsINode* aNode) {
  mEncodingScope.mNodeIsContainer = false;
  mEncodingScope.mNode = aNode;
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
<<<<<<< HEAD
nsDocumentEncoder::SetContainerNode(nsINode* aContainer) {
  mNodeIsContainer = true;
  mNode = aContainer;
||||||| merged common ancestors
nsDocumentEncoder::SetContainerNode(nsINode* aContainer)
{
  mNodeIsContainer = true;
  mNode = aContainer;
=======
nsDocumentEncoder::SetContainerNode(nsINode* aContainer) {
  mEncodingScope.mNodeIsContainer = true;
  mEncodingScope.mNode = aContainer;
>>>>>>> upstream-releases
  return NS_OK;
}

NS_IMETHODIMP
nsDocumentEncoder::SetCharset(const nsACString& aCharset) {
  const Encoding* encoding = Encoding::ForLabel(aCharset);
  if (!encoding) {
    return NS_ERROR_UCONV_NOCONV;
  }
  mEncoding = encoding->OutputEncoding();
  return NS_OK;
}

NS_IMETHODIMP
nsDocumentEncoder::GetMimeType(nsAString& aMimeType) {
  aMimeType = mMimeType;
  return NS_OK;
}

bool nsDocumentEncoder::IncludeInContext(nsINode* aNode) { return false; }

<<<<<<< HEAD
nsresult nsDocumentEncoder::SerializeNodeStart(nsINode* aNode,
                                               int32_t aStartOffset,
                                               int32_t aEndOffset,
                                               nsAString& aStr,
                                               nsINode* aOriginalNode) {
  if (mNeedsPreformatScanning && aNode->IsElement()) {
    mSerializer->ScanElementForPreformat(aNode->AsElement());
||||||| merged common ancestors
bool
nsDocumentEncoder::IncludeInContext(nsINode *aNode)
{
  return false;
}

nsresult
nsDocumentEncoder::SerializeNodeStart(nsINode* aNode,
                                      int32_t aStartOffset,
                                      int32_t aEndOffset,
                                      nsAString& aStr,
                                      nsINode* aOriginalNode)
{
  if (mNeedsPreformatScanning && aNode->IsElement()) {
    mSerializer->ScanElementForPreformat(aNode->AsElement());
=======
class FixupNodeDeterminer {
 public:
  FixupNodeDeterminer(nsIDocumentEncoderNodeFixup* aNodeFixup,
                      nsINode* aFixupNode, nsINode& aOriginalNode)
      : mIsSerializationOfFixupChildrenNeeded{false},
        mNodeFixup(aNodeFixup),
        mOriginalNode(aOriginalNode) {
    if (mNodeFixup) {
      if (aFixupNode) {
        mFixupNode = aFixupNode;
      } else {
        mNodeFixup->FixupNode(&mOriginalNode,
                              &mIsSerializationOfFixupChildrenNeeded,
                              getter_AddRefs(mFixupNode));
      }
    }
  }

  bool IsSerializationOfFixupChildrenNeeded() const {
    return mIsSerializationOfFixupChildrenNeeded;
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  if (!IsVisibleNode(aNode)) return NS_OK;
||||||| merged common ancestors
  if (!IsVisibleNode(aNode))
    return NS_OK;
=======
  /**
   * @return The fixup node, if available, otherwise the original node. The
   * former is kept alive by this object.
   */
  nsINode& GetFixupNodeFallBackToOriginalNode() const {
    return mFixupNode ? *mFixupNode : mOriginalNode;
  }
>>>>>>> upstream-releases

 private:
  bool mIsSerializationOfFixupChildrenNeeded;
  nsIDocumentEncoderNodeFixup* mNodeFixup;
  nsCOMPtr<nsINode> mFixupNode;
  nsINode& mOriginalNode;
};

nsresult nsDocumentEncoder::SerializeNodeStart(nsINode& aOriginalNode,
                                               int32_t aStartOffset,
                                               int32_t aEndOffset,
                                               nsAString& aStr,
                                               nsINode* aFixupNode) {
  if (mNeedsPreformatScanning) {
    if (aOriginalNode.IsElement()) {
      mSerializer->ScanElementForPreformat(aOriginalNode.AsElement());
    } else if (aOriginalNode.IsText()) {
      const nsCOMPtr<nsINode> parent = aOriginalNode.GetParent();
      if (parent && parent->IsElement()) {
        mSerializer->ScanElementForPreformat(parent->AsElement());
      }
    }
  }

<<<<<<< HEAD
  // Either there was no fixed-up node,
  // or the caller did fixup themselves and aNode is already fixed
  if (!node) node = aNode;
||||||| merged common ancestors
  // Either there was no fixed-up node,
  // or the caller did fixup themselves and aNode is already fixed
  if (!node)
    node = aNode;
=======
  if (IsInvisibleNodeAndShouldBeSkipped(aOriginalNode)) {
    return NS_OK;
  }

  FixupNodeDeterminer fixupNodeDeterminer{mNodeFixup, aFixupNode,
                                          aOriginalNode};
  nsINode* node = &fixupNodeDeterminer.GetFixupNodeFallBackToOriginalNode();

  nsresult rv = NS_OK;
>>>>>>> upstream-releases

  if (node->IsElement()) {
    if ((mFlags & (nsIDocumentEncoder::OutputPreformatted |
                   nsIDocumentEncoder::OutputDropInvisibleBreak)) &&
        nsLayoutUtils::IsInvisibleBreak(node)) {
      return rv;
    }
    Element* originalElement = aOriginalNode.AsElement();
    rv = mSerializer->AppendElementStart(node->AsElement(), originalElement,
                                         aStr);
    return rv;
  }

  switch (node->NodeType()) {
<<<<<<< HEAD
    case nsINode::TEXT_NODE: {
      mSerializer->AppendText(static_cast<nsIContent*>(node), aStartOffset,
                              aEndOffset, aStr);
||||||| merged common ancestors
    case nsINode::TEXT_NODE:
    {
      mSerializer->AppendText(static_cast<nsIContent*>(node),
                              aStartOffset, aEndOffset, aStr);
=======
    case nsINode::TEXT_NODE: {
      rv = mSerializer->AppendText(static_cast<nsIContent*>(node), aStartOffset,
                                   aEndOffset, aStr);
>>>>>>> upstream-releases
      break;
    }
<<<<<<< HEAD
    case nsINode::CDATA_SECTION_NODE: {
      mSerializer->AppendCDATASection(static_cast<nsIContent*>(node),
                                      aStartOffset, aEndOffset, aStr);
||||||| merged common ancestors
    case nsINode::CDATA_SECTION_NODE:
    {
      mSerializer->AppendCDATASection(static_cast<nsIContent*>(node),
                                      aStartOffset, aEndOffset, aStr);
=======
    case nsINode::CDATA_SECTION_NODE: {
      rv = mSerializer->AppendCDATASection(static_cast<nsIContent*>(node),
                                           aStartOffset, aEndOffset, aStr);
>>>>>>> upstream-releases
      break;
    }
<<<<<<< HEAD
    case nsINode::PROCESSING_INSTRUCTION_NODE: {
      mSerializer->AppendProcessingInstruction(
          static_cast<ProcessingInstruction*>(node), aStartOffset, aEndOffset,
          aStr);
||||||| merged common ancestors
    case nsINode::PROCESSING_INSTRUCTION_NODE:
    {
      mSerializer->AppendProcessingInstruction(static_cast<ProcessingInstruction*>(node),
                                               aStartOffset, aEndOffset, aStr);
=======
    case nsINode::PROCESSING_INSTRUCTION_NODE: {
      rv = mSerializer->AppendProcessingInstruction(
          static_cast<ProcessingInstruction*>(node), aStartOffset, aEndOffset,
          aStr);
>>>>>>> upstream-releases
      break;
    }
<<<<<<< HEAD
    case nsINode::COMMENT_NODE: {
      mSerializer->AppendComment(static_cast<Comment*>(node), aStartOffset,
                                 aEndOffset, aStr);
||||||| merged common ancestors
    case nsINode::COMMENT_NODE:
    {
      mSerializer->AppendComment(static_cast<Comment*>(node),
                                 aStartOffset, aEndOffset, aStr);
=======
    case nsINode::COMMENT_NODE: {
      rv = mSerializer->AppendComment(static_cast<Comment*>(node), aStartOffset,
                                      aEndOffset, aStr);
>>>>>>> upstream-releases
      break;
    }
<<<<<<< HEAD
    case nsINode::DOCUMENT_TYPE_NODE: {
      mSerializer->AppendDoctype(static_cast<DocumentType*>(node), aStr);
||||||| merged common ancestors
    case nsINode::DOCUMENT_TYPE_NODE:
    {
      mSerializer->AppendDoctype(static_cast<DocumentType*>(node), aStr);
=======
    case nsINode::DOCUMENT_TYPE_NODE: {
      rv = mSerializer->AppendDoctype(static_cast<DocumentType*>(node), aStr);
>>>>>>> upstream-releases
      break;
    }
  }

  return rv;
}

<<<<<<< HEAD
nsresult nsDocumentEncoder::SerializeNodeEnd(nsINode* aNode, nsAString& aStr) {
  if (mNeedsPreformatScanning && aNode->IsElement()) {
    mSerializer->ForgetElementForPreformat(aNode->AsElement());
||||||| merged common ancestors
nsresult
nsDocumentEncoder::SerializeNodeEnd(nsINode* aNode,
                                    nsAString& aStr)
{
  if (mNeedsPreformatScanning && aNode->IsElement()) {
    mSerializer->ForgetElementForPreformat(aNode->AsElement());
=======
nsresult nsDocumentEncoder::SerializeNodeEnd(nsINode& aNode, nsAString& aStr) {
  if (mNeedsPreformatScanning) {
    if (aNode.IsElement()) {
      mSerializer->ForgetElementForPreformat(aNode.AsElement());
    } else if (aNode.IsText()) {
      const nsCOMPtr<nsINode> parent = aNode.GetParent();
      if (parent && parent->IsElement()) {
        mSerializer->ForgetElementForPreformat(parent->AsElement());
      }
    }
>>>>>>> upstream-releases
  }

<<<<<<< HEAD
  if (!IsVisibleNode(aNode)) return NS_OK;
||||||| merged common ancestors
  if (!IsVisibleNode(aNode))
    return NS_OK;
=======
  if (IsInvisibleNodeAndShouldBeSkipped(aNode)) {
    return NS_OK;
  }
>>>>>>> upstream-releases

  nsresult rv = NS_OK;

  if (aNode.IsElement()) {
    rv = mSerializer->AppendElementEnd(aNode.AsElement(), aStr);
  }

  return rv;
}

nsresult nsDocumentEncoder::SerializeToStringRecursive(nsINode* aNode,
                                                       nsAString& aStr,
                                                       bool aDontSerializeRoot,
                                                       uint32_t aMaxLength) {
  if (aMaxLength > 0 && aStr.Length() >= aMaxLength) {
    return NS_OK;
  }

<<<<<<< HEAD
  if (!IsVisibleNode(aNode)) return NS_OK;

  nsresult rv = NS_OK;
  bool serializeClonedChildren = false;
  nsINode* maybeFixedNode = nullptr;
||||||| merged common ancestors
  if (!IsVisibleNode(aNode))
    return NS_OK;

  nsresult rv = NS_OK;
  bool serializeClonedChildren = false;
  nsINode* maybeFixedNode = nullptr;
=======
  NS_ENSURE_TRUE(aNode, NS_ERROR_NULL_POINTER);
>>>>>>> upstream-releases

  if (IsInvisibleNodeAndShouldBeSkipped(*aNode)) {
    return NS_OK;
  }

<<<<<<< HEAD
  if (!maybeFixedNode) maybeFixedNode = aNode;
||||||| merged common ancestors
  if (!maybeFixedNode)
    maybeFixedNode = aNode;
=======
  FixupNodeDeterminer fixupNodeDeterminer{mNodeFixup, nullptr, *aNode};
  nsINode* maybeFixedNode =
      &fixupNodeDeterminer.GetFixupNodeFallBackToOriginalNode();
>>>>>>> upstream-releases

  if ((mFlags & SkipInvisibleContent) &&
      !(mFlags & OutputNonTextContentAsPlaceholder)) {
    if (aNode->IsContent()) {
      if (nsIFrame* frame = aNode->AsContent()->GetPrimaryFrame()) {
        if (!frame->IsSelectable(nullptr)) {
          aDontSerializeRoot = true;
        }
      }
    }
  }

  nsresult rv = NS_OK;

  if (!aDontSerializeRoot) {
    int32_t endOffset = -1;
    if (aMaxLength > 0) {
      MOZ_ASSERT(aMaxLength >= aStr.Length());
      endOffset = aMaxLength - aStr.Length();
    }
    rv = SerializeNodeStart(*aNode, 0, endOffset, aStr, maybeFixedNode);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  nsINode* node = fixupNodeDeterminer.IsSerializationOfFixupChildrenNeeded()
                      ? maybeFixedNode
                      : aNode;

  for (nsINode* child = nsNodeUtils::GetFirstChildOfTemplateOrNode(node); child;
       child = child->GetNextSibling()) {
    rv = SerializeToStringRecursive(child, aStr, false, aMaxLength);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  if (!aDontSerializeRoot) {
    rv = SerializeNodeEnd(*maybeFixedNode, aStr);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  if (mTextStreamer) {
    rv = mTextStreamer->FlushIfStringLongEnough(aStr);
  }

  return rv;
}

nsresult nsDocumentEncoder::SerializeToStringIterative(nsINode* aNode,
                                                       nsAString& aStr) {
  nsresult rv;

  nsINode* node = nsNodeUtils::GetFirstChildOfTemplateOrNode(aNode);
  while (node) {
    nsINode* current = node;
    rv = SerializeNodeStart(*current, 0, -1, aStr, current);
    NS_ENSURE_SUCCESS(rv, rv);
    node = nsNodeUtils::GetFirstChildOfTemplateOrNode(current);
    while (!node && current && current != aNode) {
      rv = SerializeNodeEnd(*current, aStr);
      NS_ENSURE_SUCCESS(rv, rv);
      // Check if we have siblings.
      node = current->GetNextSibling();
      if (!node) {
        // Perhaps parent node has siblings.
        current = current->GetParentNode();

        // Handle template element. If the parent is a template's content,
        // then adjust the parent to be the template element.
        if (current && current != aNode && current->IsDocumentFragment()) {
          nsIContent* host = current->AsDocumentFragment()->GetHost();
          if (host && host->IsHTMLElement(nsGkAtoms::_template)) {
            current = host;
          }
        }
      }
    }
  }

  return NS_OK;
}

<<<<<<< HEAD
static nsresult ConvertAndWrite(const nsAString& aString,
                                nsIOutputStream* aStream, Encoder* aEncoder,
                                bool aIsPlainText) {
  NS_ENSURE_ARG_POINTER(aStream);
  NS_ENSURE_ARG_POINTER(aEncoder);

  if (!aString.Length()) {
    return NS_OK;
  }

  uint8_t buffer[4096];
  auto src = MakeSpan(aString);
  auto bufferSpan = MakeSpan(buffer);
  // Reserve space for terminator
  auto dst = bufferSpan.To(bufferSpan.Length() - 1);
  for (;;) {
    uint32_t result;
    size_t read;
    size_t written;
    bool hadErrors;
    if (aIsPlainText) {
      Tie(result, read, written) =
          aEncoder->EncodeFromUTF16WithoutReplacement(src, dst, false);
      if (result != kInputEmpty && result != kOutputFull) {
        // There's always room for one byte in the case of
        // an unmappable character, because otherwise
        // we'd have gotten `kOutputFull`.
        dst[written++] = '?';
      }
    } else {
      Tie(result, read, written, hadErrors) =
          aEncoder->EncodeFromUTF16(src, dst, false);
    }
    Unused << hadErrors;
    src = src.From(read);
    // Sadly, we still have test cases that implement nsIOutputStream in JS, so
    // the buffer needs to be zero-terminated for XPConnect to do its thing.
    // See bug 170416.
    bufferSpan[written] = 0;
    uint32_t streamWritten;
    nsresult rv = aStream->Write(reinterpret_cast<char*>(dst.Elements()),
                                 written, &streamWritten);
    if (NS_FAILED(rv)) {
      return rv;
    }
    if (result == kInputEmpty) {
      return NS_OK;
    }
  }
}
||||||| merged common ancestors
static nsresult
ConvertAndWrite(const nsAString& aString,
                nsIOutputStream* aStream,
                Encoder* aEncoder,
                bool aIsPlainText)
{
  NS_ENSURE_ARG_POINTER(aStream);
  NS_ENSURE_ARG_POINTER(aEncoder);

  if (!aString.Length()) {
    return NS_OK;
  }

  uint8_t buffer[4096];
  auto src = MakeSpan(aString);
  auto bufferSpan = MakeSpan(buffer);
  // Reserve space for terminator
  auto dst = bufferSpan.To(bufferSpan.Length() - 1);
  for (;;) {
    uint32_t result;
    size_t read;
    size_t written;
    bool hadErrors;
    if (aIsPlainText) {
      Tie(result, read, written) =
        aEncoder->EncodeFromUTF16WithoutReplacement(src, dst, false);
      if (result != kInputEmpty && result != kOutputFull) {
        // There's always room for one byte in the case of
        // an unmappable character, because otherwise
        // we'd have gotten `kOutputFull`.
        dst[written++] = '?';
      }
    } else {
      Tie(result, read, written, hadErrors) =
        aEncoder->EncodeFromUTF16(src, dst, false);
    }
    Unused << hadErrors;
    src = src.From(read);
    // Sadly, we still have test cases that implement nsIOutputStream in JS, so
    // the buffer needs to be zero-terminated for XPConnect to do its thing.
    // See bug 170416.
    bufferSpan[written] = 0;
    uint32_t streamWritten;
    nsresult rv = aStream->Write(
      reinterpret_cast<char*>(dst.Elements()), written, &streamWritten);
    if (NS_FAILED(rv)) {
      return rv;
    }
    if (result == kInputEmpty) {
      return NS_OK;
    }
  }
}
=======
static bool IsTextNode(nsINode* aNode) { return aNode && aNode->IsText(); }
>>>>>>> upstream-releases

<<<<<<< HEAD
nsresult nsDocumentEncoder::FlushText(nsAString& aString, bool aForce) {
  if (!mStream) return NS_OK;

  nsresult rv = NS_OK;

  if (aString.Length() > 1024 || aForce) {
    rv = ConvertAndWrite(aString, mStream, mUnicodeEncoder.get(), mIsPlainText);

    aString.Truncate();
  }

  return rv;
}

static bool IsTextNode(nsINode* aNode) { return aNode && aNode->IsText(); }

nsresult nsDocumentEncoder::SerializeRangeNodes(nsRange* aRange, nsINode* aNode,
                                                nsAString& aString,
                                                int32_t aDepth) {
||||||| merged common ancestors
nsresult
nsDocumentEncoder::FlushText(nsAString& aString, bool aForce)
{
  if (!mStream)
    return NS_OK;

  nsresult rv = NS_OK;

  if (aString.Length() > 1024 || aForce) {
    rv = ConvertAndWrite(aString, mStream, mUnicodeEncoder.get(), mIsPlainText);

    aString.Truncate();
  }

  return rv;
}

static bool IsTextNode(nsINode *aNode)
{
  return aNode && aNode->IsText();
}

nsresult
nsDocumentEncoder::SerializeRangeNodes(nsRange* aRange,
                                       nsINode* aNode,
                                       nsAString& aString,
                                       int32_t aDepth)
{
=======
nsresult nsDocumentEncoder::SerializeRangeNodes(nsRange* const aRange,
                                                nsINode* const aNode,
                                                nsAString& aString,
                                                const int32_t aDepth) {
>>>>>>> upstream-releases
  nsCOMPtr<nsIContent> content = do_QueryInterface(aNode);
  NS_ENSURE_TRUE(content, NS_ERROR_FAILURE);

<<<<<<< HEAD
  if (!IsVisibleNode(aNode)) return NS_OK;
||||||| merged common ancestors
  if (!IsVisibleNode(aNode))
    return NS_OK;
=======
  if (IsInvisibleNodeAndShouldBeSkipped(*aNode)) {
    return NS_OK;
  }
>>>>>>> upstream-releases

  nsresult rv = NS_OK;

  // get start and end nodes for this recursion level
  nsCOMPtr<nsIContent> startNode, endNode;
  {
    auto& startContainerPath =
        mRangeBoundaryPathsAndOffsets.mStartContainerPath;
    auto& endContainerPath = mRangeBoundaryPathsAndOffsets.mEndContainerPath;
    int32_t start = mStartRootIndex - aDepth;
    if (start >= 0 && (uint32_t)start <= startContainerPath.Length()) {
      startNode = startContainerPath[start];
    }

    int32_t end = mEndRootIndex - aDepth;
    if (end >= 0 && (uint32_t)end <= endContainerPath.Length()) {
      endNode = endContainerPath[end];
    }
  }

  if (startNode != content && endNode != content) {
    // node is completely contained in range.  Serialize the whole subtree
    // rooted by this node.
    rv = SerializeToStringRecursive(aNode, aString, false);
    NS_ENSURE_SUCCESS(rv, rv);
  } else {
    // due to implementation it is impossible for text node to be both start and
    // end of range.  We would have handled that case without getting here.
    // XXXsmaug What does this all mean?
    if (IsTextNode(aNode)) {
      if (startNode == content) {
        int32_t startOffset = aRange->StartOffset();
        rv = SerializeNodeStart(*aNode, startOffset, -1, aString);
        NS_ENSURE_SUCCESS(rv, rv);
      } else {
        int32_t endOffset = aRange->EndOffset();
        rv = SerializeNodeStart(*aNode, 0, endOffset, aString);
        NS_ENSURE_SUCCESS(rv, rv);
      }
<<<<<<< HEAD
    } else {
      if (aNode != mCommonParent) {
        if (IncludeInContext(aNode)) {
          // halt the incrementing of mStartDepth/mEndDepth.  This is
||||||| merged common ancestors
    }
    else
    {
      if (aNode != mCommonParent)
      {
        if (IncludeInContext(aNode))
        {
          // halt the incrementing of mStartDepth/mEndDepth.  This is
=======
      rv = SerializeNodeEnd(*aNode, aString);
      NS_ENSURE_SUCCESS(rv, rv);
    } else {
      if (aNode != mCommonAncestorOfRange) {
        if (IncludeInContext(aNode)) {
          // halt the incrementing of mContextInfoDepth.  This is
>>>>>>> upstream-releases
          // so paste client will include this node in paste.
          mHaltRangeHint = true;
        }
        if ((startNode == content) && !mHaltRangeHint) {
          ++mContextInfoDepth.mStart;
        }
        if ((endNode == content) && !mHaltRangeHint) {
          ++mContextInfoDepth.mEnd;
        }

        // serialize the start of this node
        rv = SerializeNodeStart(*aNode, 0, -1, aString);
        NS_ENSURE_SUCCESS(rv, rv);
      }

      const auto& startContainerOffsets =
          mRangeBoundaryPathsAndOffsets.mStartContainerOffsets;
      const auto& endContainerOffsets =
          mRangeBoundaryPathsAndOffsets.mEndContainerOffsets;
      // do some calculations that will tell us which children of this
      // node are in the range.
      int32_t startOffset = 0, endOffset = -1;
      if (startNode == content && mStartRootIndex >= aDepth) {
        startOffset = startContainerOffsets[mStartRootIndex - aDepth];
      }
      if (endNode == content && mEndRootIndex >= aDepth) {
        endOffset = endContainerOffsets[mEndRootIndex - aDepth];
      }
      // generated content will cause offset values of -1 to be returned.
      uint32_t childCount = content->GetChildCount();

      if (startOffset == -1) startOffset = 0;
      if (endOffset == -1)
        endOffset = childCount;
      else {
        // if we are at the "tip" of the selection, endOffset is fine.
        // otherwise, we need to add one.  This is because of the semantics
        // of the offset list created by GetAncestorsAndOffsets().  The
        // intermediate points on the list use the endOffset of the
        // location of the ancestor, rather than just past it.  So we need
        // to add one here in order to include it in the children we serialize.
        if (aNode != aRange->GetEndContainer()) {
          endOffset++;
        }
      }

      if (endOffset) {
        // serialize the children of this node that are in the range
        nsIContent* childAsNode = content->GetFirstChild();
        int32_t j = 0;

        for (; j < startOffset && childAsNode; ++j) {
          childAsNode = childAsNode->GetNextSibling();
        }

        NS_ENSURE_TRUE(!!childAsNode, NS_ERROR_FAILURE);
        MOZ_ASSERT(j == startOffset);

        for (; childAsNode && j < endOffset; ++j) {
          if ((j == startOffset) || (j == endOffset - 1)) {
            rv = SerializeRangeNodes(aRange, childAsNode, aString, aDepth + 1);
          } else {
            rv = SerializeToStringRecursive(childAsNode, aString, false);
          }

          NS_ENSURE_SUCCESS(rv, rv);

          childAsNode = childAsNode->GetNextSibling();
        }
      }

      // serialize the end of this node
<<<<<<< HEAD
      if (aNode != mCommonParent) {
        rv = SerializeNodeEnd(aNode, aString);
||||||| merged common ancestors
      if (aNode != mCommonParent)
      {
        rv = SerializeNodeEnd(aNode, aString);
=======
      if (aNode != mCommonAncestorOfRange) {
        rv = SerializeNodeEnd(*aNode, aString);
>>>>>>> upstream-releases
        NS_ENSURE_SUCCESS(rv, rv);
      }
    }
  }
  return NS_OK;
}

nsresult nsDocumentEncoder::SerializeRangeContextStart(
    const nsTArray<nsINode*>& aAncestorArray, nsAString& aString) {
  if (mDisableContextSerialize) {
    return NS_OK;
  }

  AutoTArray<nsINode*, 8>* serializedContext = mRangeContexts.AppendElement();

  int32_t i = aAncestorArray.Length(), j;
  nsresult rv = NS_OK;

  // currently only for table-related elements; see Bug 137450
  j = GetImmediateContextCount(aAncestorArray);

  while (i > 0) {
    nsINode* node = aAncestorArray.ElementAt(--i);

    if (!node) break;

    // Either a general inclusion or as immediate context
    if (IncludeInContext(node) || i < j) {
      rv = SerializeNodeStart(*node, 0, -1, aString);
      serializedContext->AppendElement(node);
      if (NS_FAILED(rv)) break;
    }
  }

  return rv;
}

nsresult nsDocumentEncoder::SerializeRangeContextEnd(nsAString& aString) {
  if (mDisableContextSerialize) {
    return NS_OK;
  }

  MOZ_RELEASE_ASSERT(!mRangeContexts.IsEmpty(),
                     "Tried to end context without starting one.");
  AutoTArray<nsINode*, 8>& serializedContext = mRangeContexts.LastElement();

  nsresult rv = NS_OK;
  for (nsINode* node : Reversed(serializedContext)) {
    rv = SerializeNodeEnd(*node, aString);

    if (NS_FAILED(rv)) break;
  }

  mRangeContexts.RemoveLastElement();
  return rv;
}

nsresult nsDocumentEncoder::SerializeRangeToString(nsRange* aRange,
                                                   nsAString& aOutputString) {
  if (!aRange || aRange->Collapsed()) return NS_OK;

  mCommonAncestorOfRange = aRange->GetCommonAncestor();

<<<<<<< HEAD
  if (!mCommonParent) return NS_OK;
||||||| merged common ancestors
  if (!mCommonParent)
    return NS_OK;
=======
  if (!mCommonAncestorOfRange) {
    return NS_OK;
  }
>>>>>>> upstream-releases

  nsINode* startContainer = aRange->GetStartContainer();
  NS_ENSURE_TRUE(startContainer, NS_ERROR_FAILURE);
  int32_t startOffset = aRange->StartOffset();

  nsINode* endContainer = aRange->GetEndContainer();
  NS_ENSURE_TRUE(endContainer, NS_ERROR_FAILURE);
  int32_t endOffset = aRange->EndOffset();

  mContextInfoDepth = {};
  mCommonAncestors.Clear();

<<<<<<< HEAD
  nsContentUtils::GetAncestors(mCommonParent, mCommonAncestors);
  nsContentUtils::GetAncestorsAndOffsets(startContainer, startOffset,
                                         &mStartNodes, &mStartOffsets);
  nsContentUtils::GetAncestorsAndOffsets(endContainer, endOffset, &mEndNodes,
                                         &mEndOffsets);

  nsCOMPtr<nsIContent> commonContent = do_QueryInterface(mCommonParent);
  mStartRootIndex = mStartNodes.IndexOf(commonContent);
  mEndRootIndex = mEndNodes.IndexOf(commonContent);
||||||| merged common ancestors
  nsContentUtils::GetAncestors(mCommonParent, mCommonAncestors);
  nsContentUtils::GetAncestorsAndOffsets(startContainer, startOffset,
                                         &mStartNodes, &mStartOffsets);
  nsContentUtils::GetAncestorsAndOffsets(endContainer, endOffset,
                                         &mEndNodes, &mEndOffsets);

  nsCOMPtr<nsIContent> commonContent = do_QueryInterface(mCommonParent);
  mStartRootIndex = mStartNodes.IndexOf(commonContent);
  mEndRootIndex = mEndNodes.IndexOf(commonContent);
=======
  mRangeBoundaryPathsAndOffsets = {};
  auto& startContainerPath = mRangeBoundaryPathsAndOffsets.mStartContainerPath;
  auto& startContainerOffsets =
      mRangeBoundaryPathsAndOffsets.mStartContainerOffsets;
  auto& endContainerPath = mRangeBoundaryPathsAndOffsets.mEndContainerPath;
  auto& endContainerOffsets =
      mRangeBoundaryPathsAndOffsets.mEndContainerOffsets;

  nsContentUtils::GetAncestors(mCommonAncestorOfRange, mCommonAncestors);
  nsContentUtils::GetAncestorsAndOffsets(
      startContainer, startOffset, &startContainerPath, &startContainerOffsets);
  nsContentUtils::GetAncestorsAndOffsets(
      endContainer, endOffset, &endContainerPath, &endContainerOffsets);

  nsCOMPtr<nsIContent> commonContent =
      do_QueryInterface(mCommonAncestorOfRange);
  mStartRootIndex = startContainerPath.IndexOf(commonContent);
  mEndRootIndex = endContainerPath.IndexOf(commonContent);
>>>>>>> upstream-releases

  nsresult rv = NS_OK;

  rv = SerializeRangeContextStart(mCommonAncestors, aOutputString);
  NS_ENSURE_SUCCESS(rv, rv);

  if (startContainer == endContainer && IsTextNode(startContainer)) {
    if (mFlags & SkipInvisibleContent) {
      // Check that the parent is visible if we don't a frame.
      // IsInvisibleNodeAndShouldBeSkipped() will do it when there's a frame.
      nsCOMPtr<nsIContent> content = do_QueryInterface(startContainer);
      if (content && !content->GetPrimaryFrame()) {
        nsIContent* parent = content->GetParent();
<<<<<<< HEAD
        if (!parent || !IsVisibleNode(parent)) return NS_OK;
||||||| merged common ancestors
        if (!parent || !IsVisibleNode(parent))
          return NS_OK;
=======
        if (!parent || IsInvisibleNodeAndShouldBeSkipped(*parent)) {
          return NS_OK;
        }
>>>>>>> upstream-releases
      }
    }
    rv = SerializeNodeStart(*startContainer, startOffset, endOffset,
                            aOutputString);
    NS_ENSURE_SUCCESS(rv, rv);
    rv = SerializeNodeEnd(*startContainer, aOutputString);
    NS_ENSURE_SUCCESS(rv, rv);
  } else {
    rv = SerializeRangeNodes(aRange, mCommonAncestorOfRange, aOutputString, 0);
    NS_ENSURE_SUCCESS(rv, rv);
  }
  rv = SerializeRangeContextEnd(aOutputString);
  NS_ENSURE_SUCCESS(rv, rv);

  return rv;
}

<<<<<<< HEAD
void nsDocumentEncoder::Clear() {
||||||| merged common ancestors
void
nsDocumentEncoder::Clear()
{
=======
void nsDocumentEncoder::ReleaseDocumentReferenceAndInitialize(
    bool aClearCachedSerializer) {
>>>>>>> upstream-releases
  mDocument = nullptr;

  Initialize(aClearCachedSerializer);
}

NS_IMETHODIMP
nsDocumentEncoder::EncodeToString(nsAString& aOutputString) {
  return EncodeToStringWithMaxLength(0, aOutputString);
}

NS_IMETHODIMP
nsDocumentEncoder::EncodeToStringWithMaxLength(uint32_t aMaxLength,
                                               nsAString& aOutputString) {
  MOZ_ASSERT(mRangeContexts.IsEmpty(), "Re-entrant call to nsDocumentEncoder.");
  auto rangeContextGuard = MakeScopeExit([&] { mRangeContexts.Clear(); });

  if (!mDocument) return NS_ERROR_NOT_INITIALIZED;

  AutoReleaseDocumentIfNeeded autoReleaseDocument(this);

  aOutputString.Truncate();

  nsString output;
  static const size_t kStringBufferSizeInBytes = 2048;
  if (!mCachedBuffer) {
    mCachedBuffer = nsStringBuffer::Alloc(kStringBufferSizeInBytes).take();
    if (NS_WARN_IF(!mCachedBuffer)) {
      return NS_ERROR_OUT_OF_MEMORY;
    }
  }
<<<<<<< HEAD
  NS_ASSERTION(
      !mCachedBuffer->IsReadonly(),
      "DocumentEncoder shouldn't keep reference to non-readonly buffer!");
||||||| merged common ancestors
  NS_ASSERTION(!mCachedBuffer->IsReadonly(),
               "DocumentEncoder shouldn't keep reference to non-readonly buffer!");
=======
  NS_ASSERTION(
      !mCachedBuffer->IsReadonly(),
      "nsIDocumentEncoder shouldn't keep reference to non-readonly buffer!");
>>>>>>> upstream-releases
  static_cast<char16_t*>(mCachedBuffer->Data())[0] = char16_t(0);
  mCachedBuffer->ToString(0, output, true);
  // output owns the buffer now!
  mCachedBuffer = nullptr;

  if (!mSerializer) {
    nsAutoCString progId(NS_CONTENTSERIALIZER_CONTRACTID_PREFIX);
    AppendUTF16toUTF8(mMimeType, progId);

    mSerializer = do_CreateInstance(progId.get());
    NS_ENSURE_TRUE(mSerializer, NS_ERROR_NOT_IMPLEMENTED);
  }

  nsresult rv = NS_OK;

<<<<<<< HEAD
  bool rewriteEncodingDeclaration =
      !(mSelection || mRange || mNode) &&
      !(mFlags & OutputDontRewriteEncodingDeclaration);
  mSerializer->Init(mFlags, mWrapColumn, mEncoding, mIsCopying,
                    rewriteEncodingDeclaration, &mNeedsPreformatScanning);

  if (mSelection) {
    uint32_t count = mSelection->RangeCount();

    nsCOMPtr<nsINode> node, prevNode;
    uint32_t firstRangeStartDepth = 0;
    for (uint32_t i = 0; i < count; ++i) {
      RefPtr<nsRange> range = mSelection->GetRangeAt(i);

      // Bug 236546: newlines not added when copying table cells into clipboard
      // Each selected cell shows up as a range containing a row with a single
      // cell get the row, compare it to previous row and emit </tr><tr> as
      // needed Bug 137450: Problem copying/pasting a table from a web page to
      // Excel. Each separate block of <tr></tr> produced above will be wrapped
      // by the immediate context. This assumes that you can't select cells that
      // are multiple selections from two tables simultaneously.
      node = range->GetStartContainer();
      NS_ENSURE_TRUE(node, NS_ERROR_FAILURE);
      if (node != prevNode) {
        if (prevNode) {
          rv = SerializeNodeEnd(prevNode, output);
          NS_ENSURE_SUCCESS(rv, rv);
        }
        nsCOMPtr<nsIContent> content = do_QueryInterface(node);
        if (content && content->IsHTMLElement(nsGkAtoms::tr) &&
            !ParentIsTR(content)) {
          if (!prevNode) {
            // Went from a non-<tr> to a <tr>
            mCommonAncestors.Clear();
            nsContentUtils::GetAncestors(node->GetParentNode(),
                                         mCommonAncestors);
            rv = SerializeRangeContextStart(mCommonAncestors, output);
            NS_ENSURE_SUCCESS(rv, rv);
            // Don't let SerializeRangeToString serialize the context again
            mDisableContextSerialize = true;
          }

          rv = SerializeNodeStart(node, 0, -1, output);
          NS_ENSURE_SUCCESS(rv, rv);
          prevNode = node;
        } else if (prevNode) {
          // Went from a <tr> to a non-<tr>
          mDisableContextSerialize = false;
          rv = SerializeRangeContextEnd(output);
          NS_ENSURE_SUCCESS(rv, rv);
          prevNode = nullptr;
        }
      }

      rv = SerializeRangeToString(range, output);
      NS_ENSURE_SUCCESS(rv, rv);
      if (i == 0) {
        firstRangeStartDepth = mStartDepth;
      }
    }
    mStartDepth = firstRangeStartDepth;

    if (prevNode) {
      rv = SerializeNodeEnd(prevNode, output);
      NS_ENSURE_SUCCESS(rv, rv);
      mDisableContextSerialize = false;
      rv = SerializeRangeContextEnd(output);
      NS_ENSURE_SUCCESS(rv, rv);
    }

    // Just to be safe
    mDisableContextSerialize = false;

    mSelection = nullptr;
  } else if (mRange) {
    rv = SerializeRangeToString(mRange, output);

    mRange = nullptr;
  } else if (mNode) {
    if (!mNodeFixup && !(mFlags & SkipInvisibleContent) && !mStream &&
        mNodeIsContainer) {
      rv = SerializeToStringIterative(mNode, output);
    } else {
      rv = SerializeToStringRecursive(mNode, output, mNodeIsContainer);
    }
    mNode = nullptr;
  } else {
    rv = mSerializer->AppendDocumentStart(mDocument, output);

    if (NS_SUCCEEDED(rv)) {
      rv = SerializeToStringRecursive(mDocument, output, false, aMaxLength);
    }
  }
||||||| merged common ancestors
  bool rewriteEncodingDeclaration = !(mSelection || mRange || mNode) && !(mFlags & OutputDontRewriteEncodingDeclaration);
  mSerializer->Init(
    mFlags, mWrapColumn, mEncoding, mIsCopying, rewriteEncodingDeclaration, &mNeedsPreformatScanning);

  if (mSelection) {
    uint32_t count = mSelection->RangeCount();

    nsCOMPtr<nsINode> node, prevNode;
    uint32_t firstRangeStartDepth = 0;
    for (uint32_t i = 0; i < count; ++i) {
      RefPtr<nsRange> range = mSelection->GetRangeAt(i);

      // Bug 236546: newlines not added when copying table cells into clipboard
      // Each selected cell shows up as a range containing a row with a single cell
      // get the row, compare it to previous row and emit </tr><tr> as needed
      // Bug 137450: Problem copying/pasting a table from a web page to Excel.
      // Each separate block of <tr></tr> produced above will be wrapped by the
      // immediate context. This assumes that you can't select cells that are
      // multiple selections from two tables simultaneously.
      node = range->GetStartContainer();
      NS_ENSURE_TRUE(node, NS_ERROR_FAILURE);
      if (node != prevNode) {
        if (prevNode) {
          rv = SerializeNodeEnd(prevNode, output);
          NS_ENSURE_SUCCESS(rv, rv);
        }
        nsCOMPtr<nsIContent> content = do_QueryInterface(node);
        if (content && content->IsHTMLElement(nsGkAtoms::tr) && !ParentIsTR(content)) {
          if (!prevNode) {
            // Went from a non-<tr> to a <tr>
            mCommonAncestors.Clear();
            nsContentUtils::GetAncestors(node->GetParentNode(),
                                         mCommonAncestors);
            rv = SerializeRangeContextStart(mCommonAncestors, output);
            NS_ENSURE_SUCCESS(rv, rv);
            // Don't let SerializeRangeToString serialize the context again
            mDisableContextSerialize = true;
          }

          rv = SerializeNodeStart(node, 0, -1, output);
          NS_ENSURE_SUCCESS(rv, rv);
          prevNode = node;
        } else if (prevNode) {
          // Went from a <tr> to a non-<tr>
          mDisableContextSerialize = false;
          rv = SerializeRangeContextEnd(output);
          NS_ENSURE_SUCCESS(rv, rv);
          prevNode = nullptr;
        }
      }

      rv = SerializeRangeToString(range, output);
      NS_ENSURE_SUCCESS(rv, rv);
      if (i == 0) {
        firstRangeStartDepth = mStartDepth;
      }
    }
    mStartDepth = firstRangeStartDepth;

    if (prevNode) {
      rv = SerializeNodeEnd(prevNode, output);
      NS_ENSURE_SUCCESS(rv, rv);
      mDisableContextSerialize = false;
      rv = SerializeRangeContextEnd(output);
      NS_ENSURE_SUCCESS(rv, rv);
    }

    // Just to be safe
    mDisableContextSerialize = false;

    mSelection = nullptr;
  } else if (mRange) {
      rv = SerializeRangeToString(mRange, output);

      mRange = nullptr;
  } else if (mNode) {
    if (!mNodeFixup && !(mFlags & SkipInvisibleContent) && !mStream &&
        mNodeIsContainer) {
      rv = SerializeToStringIterative(mNode, output);
    } else {
      rv = SerializeToStringRecursive(mNode, output, mNodeIsContainer);
    }
    mNode = nullptr;
  } else {
    rv = mSerializer->AppendDocumentStart(mDocument, output);

    if (NS_SUCCEEDED(rv)) {
      rv = SerializeToStringRecursive(mDocument, output, false, aMaxLength);
    }
  }
=======
  bool rewriteEncodingDeclaration =
      !mEncodingScope.IsLimited() &&
      !(mFlags & OutputDontRewriteEncodingDeclaration);
  mSerializer->Init(mFlags, mWrapColumn, mEncoding, mIsCopying,
                    rewriteEncodingDeclaration, &mNeedsPreformatScanning);
>>>>>>> upstream-releases

  rv = SerializeDependingOnScope(output, aMaxLength);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = mSerializer->Flush(output);

  mCachedBuffer = nsStringBuffer::FromString(output);
  // We have to be careful how we set aOutputString, because we don't
  // want it to end up sharing mCachedBuffer if we plan to reuse it.
  bool setOutput = false;
  // Try to cache the buffer.
  if (mCachedBuffer) {
    if ((mCachedBuffer->StorageSize() == kStringBufferSizeInBytes) &&
        !mCachedBuffer->IsReadonly()) {
      mCachedBuffer->AddRef();
    } else {
      if (NS_SUCCEEDED(rv)) {
        mCachedBuffer->ToString(output.Length(), aOutputString);
        setOutput = true;
      }
      mCachedBuffer = nullptr;
    }
  }

  if (!setOutput && NS_SUCCEEDED(rv)) {
    aOutputString.Append(output.get(), output.Length());
  }

  return rv;
}

NS_IMETHODIMP
nsDocumentEncoder::EncodeToStream(nsIOutputStream* aStream) {
  MOZ_ASSERT(mRangeContexts.IsEmpty(), "Re-entrant call to nsDocumentEncoder.");
<<<<<<< HEAD
  auto rangeContextGuard = MakeScopeExit([&] { mRangeContexts.Clear(); });
||||||| merged common ancestors
  auto rangeContextGuard = MakeScopeExit([&] {
    mRangeContexts.Clear();
  });
=======
  auto rangeContextGuard = MakeScopeExit([&] { mRangeContexts.Clear(); });
  NS_ENSURE_ARG_POINTER(aStream);

>>>>>>> upstream-releases
  nsresult rv = NS_OK;

  if (!mDocument) return NS_ERROR_NOT_INITIALIZED;

  if (!mEncoding) {
    return NS_ERROR_UCONV_NOCONV;
  }

  const bool isPlainText = mMimeType.LowerCaseEqualsLiteral(kTextMime);
  mTextStreamer.emplace(*aStream, mEncoding->NewEncoder(), isPlainText);
  nsAutoString buf;

  rv = EncodeToString(buf);

  // Force a flush of the last chunk of data.
  rv = mTextStreamer->ForceFlush(buf);
  NS_ENSURE_SUCCESS(rv, rv);

  mTextStreamer.reset();

  return rv;
}

NS_IMETHODIMP
nsDocumentEncoder::EncodeToStringWithContext(nsAString& aContextString,
                                             nsAString& aInfoString,
                                             nsAString& aEncodedString) {
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
nsDocumentEncoder::SetNodeFixup(nsIDocumentEncoderNodeFixup* aFixup) {
  mNodeFixup = aFixup;
  return NS_OK;
}

bool do_getDocumentTypeSupportedForEncoding(const char* aContentType) {
  if (!nsCRT::strcmp(aContentType, "text/xml") ||
      !nsCRT::strcmp(aContentType, "application/xml") ||
      !nsCRT::strcmp(aContentType, "application/xhtml+xml") ||
      !nsCRT::strcmp(aContentType, "image/svg+xml") ||
      !nsCRT::strcmp(aContentType, "text/html") ||
      !nsCRT::strcmp(aContentType, "text/plain")) {
    return true;
  }
  return false;
}

already_AddRefed<nsIDocumentEncoder> do_createDocumentEncoder(
    const char* aContentType) {
  if (do_getDocumentTypeSupportedForEncoding(aContentType)) {
    return do_AddRef(new nsDocumentEncoder);
  }
  return nullptr;
}

class nsHTMLCopyEncoder : public nsDocumentEncoder {
 public:
  nsHTMLCopyEncoder();
  virtual ~nsHTMLCopyEncoder();

<<<<<<< HEAD
  NS_IMETHOD Init(nsIDocument* aDocument, const nsAString& aMimeType,
                  uint32_t aFlags) override;
||||||| merged common ancestors
  NS_IMETHOD Init(nsIDocument* aDocument, const nsAString& aMimeType, uint32_t aFlags) override;
=======
  NS_IMETHOD Init(Document* aDocument, const nsAString& aMimeType,
                  uint32_t aFlags) override;
>>>>>>> upstream-releases

  // overridden methods from nsDocumentEncoder
  MOZ_CAN_RUN_SCRIPT_BOUNDARY
  NS_IMETHOD SetSelection(Selection* aSelection) override;
  NS_IMETHOD EncodeToStringWithContext(nsAString& aContextString,
                                       nsAString& aInfoString,
                                       nsAString& aEncodedString) override;
  NS_IMETHOD EncodeToString(nsAString& aOutputString) override;

 protected:
  enum Endpoint { kStart, kEnd };

  nsresult PromoteRange(nsRange* inRange);
  nsresult PromoteAncestorChain(nsCOMPtr<nsINode>* ioNode,
                                int32_t* ioStartOffset, int32_t* ioEndOffset);
  nsresult GetPromotedPoint(Endpoint aWhere, nsINode* aNode, int32_t aOffset,
<<<<<<< HEAD
                            nsCOMPtr<nsINode>* outNode, int32_t* outOffset,
                            nsINode* aCommon);
  nsCOMPtr<nsINode> GetChildAt(nsINode* aParent, int32_t aOffset);
  bool IsMozBR(Element* aNode);
  nsresult GetNodeLocation(nsINode* inChild, nsCOMPtr<nsINode>* outParent,
                           int32_t* outOffset);
||||||| merged common ancestors
                            nsCOMPtr<nsINode>* outNode, int32_t* outOffset, nsINode* aCommon);
  nsCOMPtr<nsINode> GetChildAt(nsINode *aParent, int32_t aOffset);
  bool IsMozBR(Element* aNode);
  nsresult GetNodeLocation(nsINode *inChild, nsCOMPtr<nsINode> *outParent, int32_t *outOffset);
=======
                            nsCOMPtr<nsINode>* outNode, int32_t* outOffset,
                            nsINode* aCommon);
  static nsCOMPtr<nsINode> GetChildAt(nsINode* aParent, int32_t aOffset);
  static bool IsMozBR(Element* aNode);
  static nsresult GetNodeLocation(nsINode* inChild,
                                  nsCOMPtr<nsINode>* outParent,
                                  int32_t* outOffset);
>>>>>>> upstream-releases
  bool IsRoot(nsINode* aNode);
<<<<<<< HEAD
  bool IsFirstNode(nsINode* aNode);
  bool IsLastNode(nsINode* aNode);
  virtual bool IncludeInContext(nsINode* aNode) override;
  virtual int32_t GetImmediateContextCount(
      const nsTArray<nsINode*>& aAncestorArray) override;
||||||| merged common ancestors
  bool IsFirstNode(nsINode *aNode);
  bool IsLastNode(nsINode *aNode);
  virtual bool IncludeInContext(nsINode *aNode) override;
  virtual int32_t
  GetImmediateContextCount(const nsTArray<nsINode*>& aAncestorArray) override;
=======
  static bool IsFirstNode(nsINode* aNode);
  static bool IsLastNode(nsINode* aNode);
  virtual bool IncludeInContext(nsINode* aNode) override;
  virtual int32_t GetImmediateContextCount(
      const nsTArray<nsINode*>& aAncestorArray) override;
>>>>>>> upstream-releases

  bool mIsTextWidget;
};

nsHTMLCopyEncoder::nsHTMLCopyEncoder() { mIsTextWidget = false; }

nsHTMLCopyEncoder::~nsHTMLCopyEncoder() {}

NS_IMETHODIMP
<<<<<<< HEAD
nsHTMLCopyEncoder::Init(nsIDocument* aDocument, const nsAString& aMimeType,
                        uint32_t aFlags) {
  if (!aDocument) return NS_ERROR_INVALID_ARG;
||||||| merged common ancestors
nsHTMLCopyEncoder::Init(nsIDocument* aDocument,
                        const nsAString& aMimeType,
                        uint32_t aFlags)
{
  if (!aDocument)
    return NS_ERROR_INVALID_ARG;
=======
nsHTMLCopyEncoder::Init(Document* aDocument, const nsAString& aMimeType,
                        uint32_t aFlags) {
  if (!aDocument) return NS_ERROR_INVALID_ARG;
>>>>>>> upstream-releases

  mIsTextWidget = false;
  Initialize();

  mIsCopying = true;
  mDocument = aDocument;

  // Hack, hack! Traditionally, the caller passes text/unicode, which is
  // treated as "guess text/html or text/plain" in this context. (It has a
  // different meaning in other contexts. Sigh.) From now on, "text/plain"
  // means forcing text/plain instead of guessing.
  if (aMimeType.EqualsLiteral("text/plain")) {
    mMimeType.AssignLiteral("text/plain");
  } else {
    mMimeType.AssignLiteral("text/html");
  }

  // Make all links absolute when copying
  // (see related bugs #57296, #41924, #58646, #32768)
  mFlags = aFlags | OutputAbsoluteLinks;

  if (!mDocument->IsScriptEnabled()) mFlags |= OutputNoScriptContent;

  return NS_OK;
}

NS_IMETHODIMP
nsHTMLCopyEncoder::SetSelection(Selection* aSelection) {
  // check for text widgets: we need to recognize these so that
  // we don't tweak the selection to be outside of the magic
  // div that ender-lite text widgets are embedded in.

  if (!aSelection) return NS_ERROR_NULL_POINTER;

  uint32_t rangeCount = aSelection->RangeCount();

  // if selection is uninitialized return
  if (!rangeCount) {
    return NS_ERROR_FAILURE;
  }

  // we'll just use the common parent of the first range.  Implicit assumption
  // here that multi-range selections are table cell selections, in which case
  // the common parent is somewhere in the table and we don't really care where.
  //
  // FIXME(emilio, bug 1455894): This assumption is already wrong, and will
  // probably be more wrong in a Shadow DOM world...
  //
  // We should be able to write this as "Find the common ancestor of the
  // selection, then go through the flattened tree and serialize the selected
  // nodes", effectively serializing the composed tree.
  RefPtr<nsRange> range = aSelection->GetRangeAt(0);
  nsINode* commonParent = range->GetCommonAncestor();

  for (nsCOMPtr<nsIContent> selContent(do_QueryInterface(commonParent));
       selContent; selContent = selContent->GetParent()) {
    // checking for selection inside a plaintext form widget
    if (selContent->IsAnyOfHTMLElements(nsGkAtoms::input,
                                        nsGkAtoms::textarea)) {
      mIsTextWidget = true;
      break;
    }
<<<<<<< HEAD
#if defined(MOZ_THUNDERBIRD) || defined(MOZ_SUITE)
    else if (selContent->IsHTMLElement(nsGkAtoms::body)) {
      // Currently, setting mIsTextWidget to 'true' will result in the selection
      // being encoded/copied as pre-formatted plain text.
      // This is fine for copying pre-formatted plain text with Firefox, it is
      // already not correct for copying pre-formatted "rich" text (bold,
      // colour) with Firefox. As long as the serialisers aren't fixed, copying
      // pre-formatted text in Firefox is broken. If we set mIsTextWidget,
      // pre-formatted plain text is copied, but pre-formatted "rich" text loses
      // the "rich" formatting. If we don't set mIsTextWidget, "rich" text
      // attributes aren't lost, but white-space is lost.
      // So far the story for Firefox.
      //
      // Thunderbird has two *conflicting* requirements.
      // Case 1:
      // When selecting and copying text, even pre-formatted text, as a quote
      // to be placed into a reply, we *always* expect HTML to be copied.
      // Case 2:
      // When copying text in a so-called "plain text" message, that is
      // one where the body carries style "white-space:pre-wrap", the text
      // should be copied as pre-formatted plain text.
      //
      // Therefore the following code checks for "pre-wrap" on the body.
      // This is a terrible hack.
      //
      // The proper fix would be this:
      // For case 1:
      // Communicate the fact that HTML is required to EncodeToString(),
      // bug 1141786.
      // For case 2:
      // Wait for Firefox to get fixed to detect pre-formatting correctly,
      // bug 1174452.
      nsAutoString styleVal;
      if (selContent->IsElement() &&
          selContent->AsElement()->GetAttr(kNameSpaceID_None, nsGkAtoms::style,
                                           styleVal) &&
          styleVal.Find(NS_LITERAL_STRING("pre-wrap")) != kNotFound) {
        mIsTextWidget = true;
        break;
      }
    }
#endif
||||||| merged common ancestors
#if defined(MOZ_THUNDERBIRD) || defined(MOZ_SUITE)
    else if (selContent->IsHTMLElement(nsGkAtoms::body)) {
      // Currently, setting mIsTextWidget to 'true' will result in the selection
      // being encoded/copied as pre-formatted plain text.
      // This is fine for copying pre-formatted plain text with Firefox, it is
      // already not correct for copying pre-formatted "rich" text (bold, colour)
      // with Firefox. As long as the serialisers aren't fixed, copying
      // pre-formatted text in Firefox is broken. If we set mIsTextWidget,
      // pre-formatted plain text is copied, but pre-formatted "rich" text loses
      // the "rich" formatting. If we don't set mIsTextWidget, "rich" text
      // attributes aren't lost, but white-space is lost.
      // So far the story for Firefox.
      //
      // Thunderbird has two *conflicting* requirements.
      // Case 1:
      // When selecting and copying text, even pre-formatted text, as a quote
      // to be placed into a reply, we *always* expect HTML to be copied.
      // Case 2:
      // When copying text in a so-called "plain text" message, that is
      // one where the body carries style "white-space:pre-wrap", the text should
      // be copied as pre-formatted plain text.
      //
      // Therefore the following code checks for "pre-wrap" on the body.
      // This is a terrible hack.
      //
      // The proper fix would be this:
      // For case 1:
      // Communicate the fact that HTML is required to EncodeToString(),
      // bug 1141786.
      // For case 2:
      // Wait for Firefox to get fixed to detect pre-formatting correctly,
      // bug 1174452.
      nsAutoString styleVal;
      if (selContent->IsElement() &&
          selContent->AsElement()->GetAttr(kNameSpaceID_None, nsGkAtoms::style, styleVal) &&
          styleVal.Find(NS_LITERAL_STRING("pre-wrap")) != kNotFound) {
        mIsTextWidget = true;
        break;
      }
    }
#endif
=======
>>>>>>> upstream-releases
  }

  // normalize selection if we are not in a widget
<<<<<<< HEAD
  if (mIsTextWidget) {
    mSelection = aSelection;
||||||| merged common ancestors
  if (mIsTextWidget)
  {
    mSelection = aSelection;
=======
  if (mIsTextWidget) {
    mEncodingScope.mSelection = aSelection;
>>>>>>> upstream-releases
    mMimeType.AssignLiteral("text/plain");
    return NS_OK;
  }

  // XXX We should try to get rid of the Selection object here.
  // XXX bug 1245883

  // also consider ourselves in a text widget if we can't find an html document
  if (!(mDocument && mDocument->IsHTMLDocument())) {
    mIsTextWidget = true;
    mEncodingScope.mSelection = aSelection;
    // mMimeType is set to text/plain when encoding starts.
    return NS_OK;
  }

  // there's no Clone() for selection! fix...
<<<<<<< HEAD
  // nsresult rv = aSelection->Clone(getter_AddRefs(mSelection);
  // NS_ENSURE_SUCCESS(rv, rv);
  mSelection = new Selection();
||||||| merged common ancestors
  //nsresult rv = aSelection->Clone(getter_AddRefs(mSelection);
  //NS_ENSURE_SUCCESS(rv, rv);
  mSelection = new Selection();
=======
  // nsresult rv = aSelection->Clone(getter_AddRefs(mSelection);
  // NS_ENSURE_SUCCESS(rv, rv);
  mEncodingScope.mSelection = new Selection();
>>>>>>> upstream-releases

  // loop thru the ranges in the selection
  for (uint32_t rangeIdx = 0; rangeIdx < rangeCount; ++rangeIdx) {
    range = aSelection->GetRangeAt(rangeIdx);
    NS_ENSURE_TRUE(range, NS_ERROR_FAILURE);
    RefPtr<nsRange> myRange = range->CloneRange();
    MOZ_ASSERT(myRange);

    // adjust range to include any ancestors who's children are entirely
    // selected
    nsresult rv = PromoteRange(myRange);
    NS_ENSURE_SUCCESS(rv, rv);

    ErrorResult result;
    RefPtr<Selection> selection(mEncodingScope.mSelection);
    RefPtr<Document> document(mDocument);
    selection->AddRangeAndSelectFramesAndNotifyListeners(*myRange, document,
                                                         result);
    rv = result.StealNSResult();
    NS_ENSURE_SUCCESS(rv, rv);
  }

  return NS_OK;
}

NS_IMETHODIMP
nsHTMLCopyEncoder::EncodeToString(nsAString& aOutputString) {
  if (mIsTextWidget) {
    mMimeType.AssignLiteral("text/plain");
  }
  return nsDocumentEncoder::EncodeToString(aOutputString);
}

NS_IMETHODIMP
nsHTMLCopyEncoder::EncodeToStringWithContext(nsAString& aContextString,
                                             nsAString& aInfoString,
                                             nsAString& aEncodedString) {
  nsresult rv = EncodeToString(aEncodedString);
  NS_ENSURE_SUCCESS(rv, rv);

  // do not encode any context info or range hints if we are in a text widget.
  if (mIsTextWidget) return NS_OK;

  // now encode common ancestors into aContextString.  Note that the common
  // ancestors will be for the last range in the selection in the case of
  // multirange selections. encoding ancestors every range in a multirange
  // selection in a way that could be understood by the paste code would be a
  // lot more work to do.  As a practical matter, selections are single range,
  // and the ones that aren't are table cell selections where all the cells are
  // in the same table.

  // leaf of ancestors might be text node.  If so discard it.
  int32_t count = mCommonAncestors.Length();
  int32_t i;
  nsCOMPtr<nsINode> node;
  if (count > 0) node = mCommonAncestors.ElementAt(0);

  if (node && IsTextNode(node)) {
    mCommonAncestors.RemoveElementAt(0);
    if (mContextInfoDepth.mStart) {
      --mContextInfoDepth.mStart;
    }
    if (mContextInfoDepth.mEnd) {
      --mContextInfoDepth.mEnd;
    }
    count--;
  }

  i = count;
  while (i > 0) {
    node = mCommonAncestors.ElementAt(--i);
    rv = SerializeNodeStart(*node, 0, -1, aContextString);
    NS_ENSURE_SUCCESS(rv, rv);
  }
  // i = 0; guaranteed by above
  while (i < count) {
    node = mCommonAncestors.ElementAt(i++);
    rv = SerializeNodeEnd(*node, aContextString);
    NS_ENSURE_SUCCESS(rv, rv);
  }

  // encode range info : the start and end depth of the selection, where the
  // depth is distance down in the parent hierarchy.  Later we will need to add
  // leading/trailing whitespace info to this.
  nsAutoString infoString;
  infoString.AppendInt(mContextInfoDepth.mStart);
  infoString.Append(char16_t(','));
  infoString.AppendInt(mContextInfoDepth.mEnd);
  aInfoString = infoString;

  return rv;
}

bool nsHTMLCopyEncoder::IncludeInContext(nsINode* aNode) {
  nsCOMPtr<nsIContent> content(do_QueryInterface(aNode));

  if (!content) return false;

  return content->IsAnyOfHTMLElements(
      nsGkAtoms::b, nsGkAtoms::i, nsGkAtoms::u, nsGkAtoms::a, nsGkAtoms::tt,
      nsGkAtoms::s, nsGkAtoms::big, nsGkAtoms::small, nsGkAtoms::strike,
      nsGkAtoms::em, nsGkAtoms::strong, nsGkAtoms::dfn, nsGkAtoms::code,
      nsGkAtoms::cite, nsGkAtoms::var, nsGkAtoms::abbr, nsGkAtoms::font,
      nsGkAtoms::script, nsGkAtoms::span, nsGkAtoms::pre, nsGkAtoms::h1,
      nsGkAtoms::h2, nsGkAtoms::h3, nsGkAtoms::h4, nsGkAtoms::h5,
      nsGkAtoms::h6);
}

nsresult nsHTMLCopyEncoder::PromoteRange(nsRange* inRange) {
  if (!inRange->IsPositioned()) {
    return NS_ERROR_UNEXPECTED;
  }
  nsCOMPtr<nsINode> startNode = inRange->GetStartContainer();
  uint32_t startOffset = inRange->StartOffset();
  nsCOMPtr<nsINode> endNode = inRange->GetEndContainer();
  uint32_t endOffset = inRange->EndOffset();
  nsCOMPtr<nsINode> common = inRange->GetCommonAncestor();

  nsCOMPtr<nsINode> opStartNode;
  nsCOMPtr<nsINode> opEndNode;
  int32_t opStartOffset, opEndOffset;

  // examine range endpoints.
  nsresult rv =
      GetPromotedPoint(kStart, startNode, static_cast<int32_t>(startOffset),
                       address_of(opStartNode), &opStartOffset, common);
  NS_ENSURE_SUCCESS(rv, rv);
  rv = GetPromotedPoint(kEnd, endNode, static_cast<int32_t>(endOffset),
                        address_of(opEndNode), &opEndOffset, common);
  NS_ENSURE_SUCCESS(rv, rv);

  // if both range endpoints are at the common ancestor, check for possible
  // inclusion of ancestors
  if (opStartNode == common && opEndNode == common) {
    rv = PromoteAncestorChain(address_of(opStartNode), &opStartOffset,
                              &opEndOffset);
    NS_ENSURE_SUCCESS(rv, rv);
    opEndNode = opStartNode;
  }

  // set the range to the new values
  ErrorResult err;
  inRange->SetStart(*opStartNode, static_cast<uint32_t>(opStartOffset), err);
  if (NS_WARN_IF(err.Failed())) {
    return err.StealNSResult();
  }
  inRange->SetEnd(*opEndNode, static_cast<uint32_t>(opEndOffset), err);
  if (NS_WARN_IF(err.Failed())) {
    return err.StealNSResult();
  }
  return NS_OK;
}

// PromoteAncestorChain will promote a range represented by
// [{*ioNode,*ioStartOffset} , {*ioNode,*ioEndOffset}] The promotion is
// different from that found in getPromotedPoint: it will only promote one
// endpoint if it can promote the other.  Thus, instead of having a
// startnode/endNode, there is just the one ioNode.
nsresult nsHTMLCopyEncoder::PromoteAncestorChain(nsCOMPtr<nsINode>* ioNode,
                                                 int32_t* ioStartOffset,
                                                 int32_t* ioEndOffset) {
  if (!ioNode || !ioStartOffset || !ioEndOffset) return NS_ERROR_NULL_POINTER;

  nsresult rv = NS_OK;
  bool done = false;

  nsCOMPtr<nsINode> frontNode, endNode, parent;
  int32_t frontOffset, endOffset;

  // save the editable state of the ioNode, so we don't promote an ancestor if
  // it has different editable state
  nsCOMPtr<nsINode> node = *ioNode;
  bool isEditable = node->IsEditable();

  // loop for as long as we can promote both endpoints
  while (!done) {
    node = *ioNode;
    parent = node->GetParentNode();
    if (!parent) {
      done = true;
    } else {
      // passing parent as last param to GetPromotedPoint() allows it to promote
      // only one level up the hierarchy.
      rv = GetPromotedPoint(kStart, *ioNode, *ioStartOffset,
                            address_of(frontNode), &frontOffset, parent);
      NS_ENSURE_SUCCESS(rv, rv);
      // then we make the same attempt with the endpoint
      rv = GetPromotedPoint(kEnd, *ioNode, *ioEndOffset, address_of(endNode),
                            &endOffset, parent);
      NS_ENSURE_SUCCESS(rv, rv);

      // if both endpoints were promoted one level and isEditable is the same as
      // the original node, keep looping - otherwise we are done.
      if ((frontNode != parent) || (endNode != parent) ||
          (frontNode->IsEditable() != isEditable))
        done = true;
      else {
        *ioNode = frontNode;
        *ioStartOffset = frontOffset;
        *ioEndOffset = endOffset;
      }
    }
  }
  return rv;
}

nsresult nsHTMLCopyEncoder::GetPromotedPoint(Endpoint aWhere, nsINode* aNode,
                                             int32_t aOffset,
                                             nsCOMPtr<nsINode>* outNode,
                                             int32_t* outOffset,
                                             nsINode* common) {
  nsresult rv = NS_OK;
  nsCOMPtr<nsINode> node = aNode;
  nsCOMPtr<nsINode> parent = aNode;
  int32_t offset = aOffset;
  bool bResetPromotion = false;

  // default values
  *outNode = node;
  *outOffset = offset;

  if (common == node) return NS_OK;

  if (aWhere == kStart) {
    // some special casing for text nodes
    if (auto nodeAsText = aNode->GetAsText()) {
      // if not at beginning of text node, we are done
      if (offset > 0) {
        // unless everything before us in just whitespace.  NOTE: we need a more
        // general solution that truly detects all cases of non-significant
        // whitesace with no false alarms.
        nsAutoString text;
        nodeAsText->SubstringData(0, offset, text, IgnoreErrors());
        text.CompressWhitespace();
        if (!text.IsEmpty()) return NS_OK;
        bResetPromotion = true;
      }
      // else
      rv = GetNodeLocation(aNode, address_of(parent), &offset);
      NS_ENSURE_SUCCESS(rv, rv);
    } else {
      node = GetChildAt(parent, offset);
    }
    if (!node) node = parent;

    // finding the real start for this point.  look up the tree for as long as
    // we are the first node in the container, and as long as we haven't hit the
    // body node.
    if (!IsRoot(node) && (parent != common)) {
      rv = GetNodeLocation(node, address_of(parent), &offset);
      NS_ENSURE_SUCCESS(rv, rv);
      if (offset == -1) return NS_OK;  // we hit generated content; STOP
      while ((IsFirstNode(node)) && (!IsRoot(parent)) && (parent != common)) {
        if (bResetPromotion) {
          nsCOMPtr<nsIContent> content = do_QueryInterface(parent);
          if (content && content->IsHTMLElement()) {
            if (nsHTMLElement::IsBlock(
                    nsHTMLTags::AtomTagToId(content->NodeInfo()->NameAtom()))) {
              bResetPromotion = false;
            }
          }
        }

        node = parent;
        rv = GetNodeLocation(node, address_of(parent), &offset);
        NS_ENSURE_SUCCESS(rv, rv);
        if (offset == -1)  // we hit generated content; STOP
        {
          // back up a bit
          parent = node;
          offset = 0;
          break;
        }
      }
      if (bResetPromotion) {
        *outNode = aNode;
        *outOffset = aOffset;
      } else {
        *outNode = parent;
        *outOffset = offset;
      }
      return rv;
    }
  }

  if (aWhere == kEnd) {
    // some special casing for text nodes
    if (auto nodeAsText = aNode->GetAsText()) {
      // if not at end of text node, we are done
      uint32_t len = aNode->Length();
      if (offset < (int32_t)len) {
        // unless everything after us in just whitespace.  NOTE: we need a more
        // general solution that truly detects all cases of non-significant
        // whitespace with no false alarms.
        nsAutoString text;
        nodeAsText->SubstringData(offset, len - offset, text, IgnoreErrors());
        text.CompressWhitespace();
        if (!text.IsEmpty()) return NS_OK;
        bResetPromotion = true;
      }
      rv = GetNodeLocation(aNode, address_of(parent), &offset);
      NS_ENSURE_SUCCESS(rv, rv);
    } else {
      if (offset) offset--;  // we want node _before_ offset
      node = GetChildAt(parent, offset);
    }
    if (!node) node = parent;

    // finding the real end for this point.  look up the tree for as long as we
    // are the last node in the container, and as long as we haven't hit the
    // body node.
    if (!IsRoot(node) && (parent != common)) {
      rv = GetNodeLocation(node, address_of(parent), &offset);
      NS_ENSURE_SUCCESS(rv, rv);
      if (offset == -1) return NS_OK;  // we hit generated content; STOP
      while ((IsLastNode(node)) && (!IsRoot(parent)) && (parent != common)) {
        if (bResetPromotion) {
          nsCOMPtr<nsIContent> content = do_QueryInterface(parent);
          if (content && content->IsHTMLElement()) {
            if (nsHTMLElement::IsBlock(
                    nsHTMLTags::AtomTagToId(content->NodeInfo()->NameAtom()))) {
              bResetPromotion = false;
            }
          }
        }

        node = parent;
        rv = GetNodeLocation(node, address_of(parent), &offset);
        NS_ENSURE_SUCCESS(rv, rv);
        if (offset == -1)  // we hit generated content; STOP
        {
          // back up a bit
          parent = node;
          offset = 0;
          break;
        }
      }
      if (bResetPromotion) {
        *outNode = aNode;
        *outOffset = aOffset;
      } else {
        *outNode = parent;
        offset++;  // add one since this in an endpoint - want to be AFTER node.
        *outOffset = offset;
      }
      return rv;
    }
  }

  return rv;
}

nsCOMPtr<nsINode> nsHTMLCopyEncoder::GetChildAt(nsINode* aParent,
                                                int32_t aOffset) {
  nsCOMPtr<nsINode> resultNode;

  if (!aParent) return resultNode;

  nsCOMPtr<nsIContent> content = do_QueryInterface(aParent);
  MOZ_ASSERT(content, "null content in nsHTMLCopyEncoder::GetChildAt");

  resultNode = content->GetChildAt_Deprecated(aOffset);

  return resultNode;
}

bool nsHTMLCopyEncoder::IsMozBR(Element* aElement) {
  return aElement->IsHTMLElement(nsGkAtoms::br) &&
         aElement->AttrValueIs(kNameSpaceID_None, nsGkAtoms::type,
                               NS_LITERAL_STRING("_moz"), eIgnoreCase);
}

nsresult nsHTMLCopyEncoder::GetNodeLocation(nsINode* inChild,
                                            nsCOMPtr<nsINode>* outParent,
                                            int32_t* outOffset) {
  NS_ASSERTION((inChild && outParent && outOffset), "bad args");
  if (inChild && outParent && outOffset) {
    nsCOMPtr<nsIContent> child = do_QueryInterface(inChild);
    if (!child) {
      return NS_ERROR_NULL_POINTER;
    }

    nsIContent* parent = child->GetParent();
    if (!parent) {
      return NS_ERROR_NULL_POINTER;
    }

    *outParent = parent;
    *outOffset = parent->ComputeIndexOf(child);
    return NS_OK;
  }
  return NS_ERROR_NULL_POINTER;
}

bool nsHTMLCopyEncoder::IsRoot(nsINode* aNode) {
  nsCOMPtr<nsIContent> content = do_QueryInterface(aNode);
  if (!content) {
    return false;
  }

  if (mIsTextWidget) {
    return content->IsHTMLElement(nsGkAtoms::div);
  }

  return content->IsAnyOfHTMLElements(nsGkAtoms::body, nsGkAtoms::td,
                                      nsGkAtoms::th);
}

bool nsHTMLCopyEncoder::IsFirstNode(nsINode* aNode) {
  // need to check if any nodes before us are really visible.
  // Mike wrote something for me along these lines in nsSelectionController,
  // but I don't think it's ready for use yet - revisit.
  // HACK: for now, simply consider all whitespace text nodes to be
  // invisible formatting nodes.
  for (nsIContent* sibling = aNode->GetPreviousSibling(); sibling;
       sibling = sibling->GetPreviousSibling()) {
    if (!sibling->TextIsOnlyWhitespace()) {
      return false;
    }
  }

  return true;
}

bool nsHTMLCopyEncoder::IsLastNode(nsINode* aNode) {
  // need to check if any nodes after us are really visible.
  // Mike wrote something for me along these lines in nsSelectionController,
  // but I don't think it's ready for use yet - revisit.
  // HACK: for now, simply consider all whitespace text nodes to be
  // invisible formatting nodes.
  for (nsIContent* sibling = aNode->GetNextSibling(); sibling;
       sibling = sibling->GetNextSibling()) {
    if (sibling->IsElement() && IsMozBR(sibling->AsElement())) {
      // we ignore trailing moz BRs.
      continue;
    }
    if (!sibling->TextIsOnlyWhitespace()) {
      return false;
    }
  }

  return true;
}

already_AddRefed<nsIDocumentEncoder> do_createHTMLCopyEncoder() {
  return do_AddRef(new nsHTMLCopyEncoder);
}

int32_t nsHTMLCopyEncoder::GetImmediateContextCount(
    const nsTArray<nsINode*>& aAncestorArray) {
  int32_t i = aAncestorArray.Length(), j = 0;
  while (j < i) {
    nsINode* node = aAncestorArray.ElementAt(j);
    if (!node) {
      break;
    }
    nsCOMPtr<nsIContent> content(do_QueryInterface(node));
    if (!content || !content->IsAnyOfHTMLElements(
                        nsGkAtoms::tr, nsGkAtoms::thead, nsGkAtoms::tbody,
                        nsGkAtoms::tfoot, nsGkAtoms::table)) {
      break;
    }
    ++j;
  }
  return j;
}
