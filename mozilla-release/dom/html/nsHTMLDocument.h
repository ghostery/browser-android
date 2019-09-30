/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef nsHTMLDocument_h___
#define nsHTMLDocument_h___

#include "mozilla/Attributes.h"
#include "nsContentList.h"
#include "mozilla/dom/Document.h"
#include "nsIHTMLCollection.h"
#include "nsIScriptElement.h"
#include "nsTArray.h"

#include "PLDHashTable.h"
#include "nsIHttpChannel.h"
#include "nsThreadUtils.h"
#include "mozilla/dom/HTMLSharedElement.h"
#include "mozilla/dom/BindingDeclarations.h"

class nsCommandManager;
class nsIURI;
class nsIDocShell;
class nsICachingChannel;
class nsILoadGroup;

namespace mozilla {
namespace dom {
class HTMLAllCollection;
<<<<<<< HEAD
}  // namespace dom
}  // namespace mozilla

class nsHTMLDocument : public nsDocument, public nsIHTMLDocument {
 public:
  using nsDocument::GetPlugins;
  using nsDocument::SetDocumentURI;
||||||| merged common ancestors
} // namespace dom
} // namespace mozilla

class nsHTMLDocument : public nsDocument,
                       public nsIHTMLDocument
{
public:
  using nsDocument::SetDocumentURI;
  using nsDocument::GetPlugins;
=======
template <typename T>
struct Nullable;
class WindowProxyHolder;
}  // namespace dom
}  // namespace mozilla

class nsHTMLDocument : public mozilla::dom::Document {
 protected:
  typedef mozilla::net::ReferrerPolicy ReferrerPolicy;
  typedef mozilla::dom::Document Document;
  typedef mozilla::Encoding Encoding;
  template <typename T>
  using NotNull = mozilla::NotNull<T>;

 public:
  using Document::GetPlugins;
  using Document::SetDocumentURI;
>>>>>>> upstream-releases

  nsHTMLDocument();
  virtual nsresult Init() override;

  NS_DECL_ISUPPORTS_INHERITED
  NS_DECL_CYCLE_COLLECTION_CLASS_INHERITED(nsHTMLDocument, Document)

  // Document
  virtual void Reset(nsIChannel* aChannel, nsILoadGroup* aLoadGroup) override;
  virtual void ResetToURI(nsIURI* aURI, nsILoadGroup* aLoadGroup,
                          nsIPrincipal* aPrincipal,
                          nsIPrincipal* aStoragePrincipal) override;

  virtual nsresult StartDocumentLoad(const char* aCommand, nsIChannel* aChannel,
                                     nsILoadGroup* aLoadGroup,
                                     nsISupports* aContainer,
                                     nsIStreamListener** aDocListener,
                                     bool aReset = true,
                                     nsIContentSink* aSink = nullptr) override;

<<<<<<< HEAD
  virtual bool IsWriting() override { return mWriteLevel != uint32_t(0); }
||||||| merged common ancestors
  virtual bool IsWriting() override
  {
    return mWriteLevel != uint32_t(0);
  }
=======
 protected:
  virtual bool UseWidthDeviceWidthFallbackViewport() const override;
>>>>>>> upstream-releases

 public:
  virtual Element* GetUnfocusedKeyEventTarget() override;

  nsContentList* GetExistingForms() const { return mForms; }

  mozilla::dom::HTMLAllCollection* All();

  // Returns whether an object was found for aName.
  bool ResolveName(JSContext* aCx, const nsAString& aName,
<<<<<<< HEAD
                   JS::MutableHandle<JS::Value> aRetval,
                   mozilla::ErrorResult& aError);

  virtual void AddedForm() override;
  virtual void RemovedForm() override;
  virtual int32_t GetNumFormsSynchronous() override;
  virtual void TearingDownEditor() override;
  virtual void SetIsXHTML(bool aXHTML) override {
    mType = (aXHTML ? eXHTML : eHTML);
  }
  virtual void SetDocWriteDisabled(bool aDisabled) override {
    mDisableDocWrite = aDisabled;
  }

  nsresult ChangeContentEditableCount(nsIContent* aElement,
                                      int32_t aChange) override;
  void DeferredContentEditableCountChange(nsIContent* aElement);

  virtual EditingState GetEditingState() override { return mEditingState; }

  virtual void DisableCookieAccess() override { mDisableCookieAccess = true; }
||||||| merged common ancestors
                   JS::MutableHandle<JS::Value> aRetval, mozilla::ErrorResult& aError);

  virtual void AddedForm() override;
  virtual void RemovedForm() override;
  virtual int32_t GetNumFormsSynchronous() override;
  virtual void TearingDownEditor() override;
  virtual void SetIsXHTML(bool aXHTML) override
  {
    mType = (aXHTML ? eXHTML : eHTML);
  }
  virtual void SetDocWriteDisabled(bool aDisabled) override
  {
    mDisableDocWrite = aDisabled;
  }

  nsresult ChangeContentEditableCount(nsIContent *aElement, int32_t aChange) override;
  void DeferredContentEditableCountChange(nsIContent *aElement);

  virtual EditingState GetEditingState() override
  {
    return mEditingState;
  }

  virtual void DisableCookieAccess() override
  {
    mDisableCookieAccess = true;
  }
=======
                   JS::MutableHandle<JS::Value> aRetval,
                   mozilla::ErrorResult& aError);
>>>>>>> upstream-releases

<<<<<<< HEAD
  class nsAutoEditingState {
   public:
    nsAutoEditingState(nsHTMLDocument* aDoc, EditingState aState)
        : mDoc(aDoc), mSavedState(aDoc->mEditingState) {
      aDoc->mEditingState = aState;
    }
    ~nsAutoEditingState() { mDoc->mEditingState = mSavedState; }

   private:
    nsHTMLDocument* mDoc;
    EditingState mSavedState;
  };
  friend class nsAutoEditingState;

  void EndUpdate() override;

  virtual void SetMayStartLayout(bool aMayStartLayout) override;

  virtual nsresult SetEditingState(EditingState aState) override;
||||||| merged common ancestors
  class nsAutoEditingState {
  public:
    nsAutoEditingState(nsHTMLDocument* aDoc, EditingState aState)
      : mDoc(aDoc), mSavedState(aDoc->mEditingState)
    {
      aDoc->mEditingState = aState;
    }
    ~nsAutoEditingState() {
      mDoc->mEditingState = mSavedState;
    }
  private:
    nsHTMLDocument* mDoc;
    EditingState    mSavedState;
  };
  friend class nsAutoEditingState;

  void EndUpdate() override;

  virtual void SetMayStartLayout(bool aMayStartLayout) override;

  virtual nsresult SetEditingState(EditingState aState) override;
=======
  /**
   * Called when form->BindToTree() is called so that document knows
   * immediately when a form is added
   */
  void AddedForm();
  /**
   * Called when form->SetDocument() is called so that document knows
   * immediately when a form is removed
   */
  void RemovedForm();
  /**
   * Called to get a better count of forms than document.forms can provide
   * without calling FlushPendingNotifications (bug 138892).
   */
  // XXXbz is this still needed now that we can flush just content,
  // not the rest?
  int32_t GetNumFormsSynchronous();
  void SetIsXHTML(bool aXHTML) { mType = (aXHTML ? eXHTML : eHTML); }
>>>>>>> upstream-releases

  virtual nsresult Clone(mozilla::dom::NodeInfo*,
                         nsINode** aResult) const override;

  using mozilla::dom::DocumentOrShadowRoot::GetElementById;

<<<<<<< HEAD
  virtual void DocAddSizeOfExcludingThis(
      nsWindowSizes& aWindowSizes) const override;
  // DocAddSizeOfIncludingThis is inherited from nsIDocument.
||||||| merged common ancestors
  virtual void DocAddSizeOfExcludingThis(nsWindowSizes& aWindowSizes) const override;
  // DocAddSizeOfIncludingThis is inherited from nsIDocument.
=======
  virtual void DocAddSizeOfExcludingThis(
      nsWindowSizes& aWindowSizes) const override;
  // DocAddSizeOfIncludingThis is inherited from Document.
>>>>>>> upstream-releases

  virtual bool WillIgnoreCharsetOverride() override;

  // WebIDL API
<<<<<<< HEAD
  virtual JSObject* WrapNode(JSContext* aCx,
                             JS::Handle<JSObject*> aGivenProto) override;
  void GetDomain(nsAString& aDomain);
  void SetDomain(const nsAString& aDomain, mozilla::ErrorResult& rv);
||||||| merged common ancestors
  virtual JSObject* WrapNode(JSContext* aCx, JS::Handle<JSObject*> aGivenProto)
    override;
  void GetDomain(nsAString& aDomain);
  void SetDomain(const nsAString& aDomain, mozilla::ErrorResult& rv);
=======
  virtual JSObject* WrapNode(JSContext* aCx,
                             JS::Handle<JSObject*> aGivenProto) override;
>>>>>>> upstream-releases
  bool IsRegistrableDomainSuffixOfOrEqualTo(const nsAString& aHostSuffixString,
                                            const nsACString& aOrigHost);
  void NamedGetter(JSContext* cx, const nsAString& aName, bool& aFound,
                   JS::MutableHandle<JSObject*> aRetval,
                   mozilla::ErrorResult& rv) {
    JS::Rooted<JS::Value> v(cx);
    if ((aFound = ResolveName(cx, aName, &v, rv))) {
      aRetval.set(v.toObjectOrNull());
    }
  }
  void GetSupportedNames(nsTArray<nsString>& aNames);
<<<<<<< HEAD
  already_AddRefed<nsIDocument> Open(
      JSContext* cx, const mozilla::dom::Optional<nsAString>& /* unused */,
      const nsAString& aReplace, mozilla::ErrorResult& aError);
  already_AddRefed<nsPIDOMWindowOuter> Open(
      JSContext* cx, const nsAString& aURL, const nsAString& aName,
      const nsAString& aFeatures, bool aReplace, mozilla::ErrorResult& rv);
  void Close(mozilla::ErrorResult& rv);
  void Write(JSContext* cx, const mozilla::dom::Sequence<nsString>& aText,
             mozilla::ErrorResult& rv);
  void Writeln(JSContext* cx, const mozilla::dom::Sequence<nsString>& aText,
               mozilla::ErrorResult& rv);
  void GetDesignMode(nsAString& aDesignMode);
  void SetDesignMode(const nsAString& aDesignMode,
                     nsIPrincipal& aSubjectPrincipal, mozilla::ErrorResult& rv);
  void SetDesignMode(const nsAString& aDesignMode,
                     const mozilla::Maybe<nsIPrincipal*>& aSubjectPrincipal,
                     mozilla::ErrorResult& rv);
  bool ExecCommand(const nsAString& aCommandID, bool aDoShowUI,
                   const nsAString& aValue, nsIPrincipal& aSubjectPrincipal,
                   mozilla::ErrorResult& rv);
  bool QueryCommandEnabled(const nsAString& aCommandID,
                           nsIPrincipal& aSubjectPrincipal,
                           mozilla::ErrorResult& rv);
  bool QueryCommandIndeterm(const nsAString& aCommandID,
                            mozilla::ErrorResult& rv);
  bool QueryCommandState(const nsAString& aCommandID, mozilla::ErrorResult& rv);
  bool QueryCommandSupported(const nsAString& aCommandID,
                             mozilla::dom::CallerType aCallerType);
  void QueryCommandValue(const nsAString& aCommandID, nsAString& aValue,
                         mozilla::ErrorResult& rv);
  void GetFgColor(nsAString& aFgColor);
  void SetFgColor(const nsAString& aFgColor);
  void GetLinkColor(nsAString& aLinkColor);
  void SetLinkColor(const nsAString& aLinkColor);
  void GetVlinkColor(nsAString& aAvlinkColor);
  void SetVlinkColor(const nsAString& aVlinkColor);
  void GetAlinkColor(nsAString& aAlinkColor);
  void SetAlinkColor(const nsAString& aAlinkColor);
  void GetBgColor(nsAString& aBgColor);
  void SetBgColor(const nsAString& aBgColor);
  void Clear() const {
||||||| merged common ancestors
  already_AddRefed<nsIDocument> Open(JSContext* cx,
                                     const mozilla::dom::Optional<nsAString>& /* unused */,
                                     const nsAString& aReplace,
                                     mozilla::ErrorResult& aError);
  already_AddRefed<nsPIDOMWindowOuter>
  Open(JSContext* cx,
       const nsAString& aURL,
       const nsAString& aName,
       const nsAString& aFeatures,
       bool aReplace,
       mozilla::ErrorResult& rv);
  void Close(mozilla::ErrorResult& rv);
  void Write(JSContext* cx, const mozilla::dom::Sequence<nsString>& aText,
             mozilla::ErrorResult& rv);
  void Writeln(JSContext* cx, const mozilla::dom::Sequence<nsString>& aText,
               mozilla::ErrorResult& rv);
  void GetDesignMode(nsAString& aDesignMode);
  void SetDesignMode(const nsAString& aDesignMode,
                     nsIPrincipal& aSubjectPrincipal,
                     mozilla::ErrorResult& rv);
  void SetDesignMode(const nsAString& aDesignMode,
                     const mozilla::Maybe<nsIPrincipal*>& aSubjectPrincipal,
                     mozilla::ErrorResult& rv);
  bool ExecCommand(const nsAString& aCommandID, bool aDoShowUI,
                   const nsAString& aValue,
                   nsIPrincipal& aSubjectPrincipal,
                   mozilla::ErrorResult& rv);
  bool QueryCommandEnabled(const nsAString& aCommandID,
                           nsIPrincipal& aSubjectPrincipal,
                           mozilla::ErrorResult& rv);
  bool QueryCommandIndeterm(const nsAString& aCommandID,
                            mozilla::ErrorResult& rv);
  bool QueryCommandState(const nsAString& aCommandID, mozilla::ErrorResult& rv);
  bool QueryCommandSupported(const nsAString& aCommandID,
                             mozilla::dom::CallerType aCallerType);
  void QueryCommandValue(const nsAString& aCommandID, nsAString& aValue,
                         mozilla::ErrorResult& rv);
  void GetFgColor(nsAString& aFgColor);
  void SetFgColor(const nsAString& aFgColor);
  void GetLinkColor(nsAString& aLinkColor);
  void SetLinkColor(const nsAString& aLinkColor);
  void GetVlinkColor(nsAString& aAvlinkColor);
  void SetVlinkColor(const nsAString& aVlinkColor);
  void GetAlinkColor(nsAString& aAlinkColor);
  void SetAlinkColor(const nsAString& aAlinkColor);
  void GetBgColor(nsAString& aBgColor);
  void SetBgColor(const nsAString& aBgColor);
  void Clear() const
  {
=======
  void Clear() const {
>>>>>>> upstream-releases
    // Deprecated
  }
  void CaptureEvents();
  void ReleaseEvents();
  // We're picking up GetLocation from Document
<<<<<<< HEAD
  already_AddRefed<mozilla::dom::Location> GetLocation() const {
    return nsIDocument::GetLocation();
||||||| merged common ancestors
  already_AddRefed<mozilla::dom::Location> GetLocation() const
  {
    return nsIDocument::GetLocation();
=======
  already_AddRefed<mozilla::dom::Location> GetLocation() const {
    return Document::GetLocation();
>>>>>>> upstream-releases
  }

  static bool MatchFormControls(Element* aElement, int32_t aNamespaceID,
                                nsAtom* aAtom, void* aData);

  void GetFormsAndFormControls(nsContentList** aFormList,
                               nsContentList** aFormControlList);

  void UserInteractionForTesting();

 protected:
  ~nsHTMLDocument();

  nsresult GetBodySize(int32_t* aWidth, int32_t* aHeight);

  nsIContent* MatchId(nsIContent* aContent, const nsAString& aId);

<<<<<<< HEAD
  static void DocumentWriteTerminationFunc(nsISupports* aRef);

  already_AddRefed<nsIURI> GetDomainURI();
  already_AddRefed<nsIURI> CreateInheritingURIForHost(
      const nsACString& aHostString);
  already_AddRefed<nsIURI> RegistrableDomainSuffixOfInternal(
      const nsAString& aHostSuffixString, nsIURI* aOrigHost);

  void WriteCommon(JSContext* cx, const nsAString& aText,
                   bool aNewlineTerminate, mozilla::ErrorResult& aRv);
  // A version of WriteCommon used by WebIDL bindings
  void WriteCommon(JSContext* cx, const mozilla::dom::Sequence<nsString>& aText,
                   bool aNewlineTerminate, mozilla::ErrorResult& rv);

  nsresult CreateAndAddWyciwygChannel(void);
  nsresult RemoveWyciwygChannel(void);

  // This should *ONLY* be used in GetCookie/SetCookie.
  already_AddRefed<nsIChannel> CreateDummyChannelForCookies(
      nsIURI* aCodebaseURI);

  /**
   * Like IsEditingOn(), but will flush as needed first.
   */
  bool IsEditingOnAfterFlush();

  void* GenerateParserKey(void);
||||||| merged common ancestors
  static void DocumentWriteTerminationFunc(nsISupports *aRef);

  already_AddRefed<nsIURI> GetDomainURI();
  already_AddRefed<nsIURI> CreateInheritingURIForHost(const nsACString& aHostString);
  already_AddRefed<nsIURI> RegistrableDomainSuffixOfInternal(const nsAString& aHostSuffixString,
                                                             nsIURI* aOrigHost);


  void WriteCommon(JSContext *cx, const nsAString& aText,
                   bool aNewlineTerminate, mozilla::ErrorResult& aRv);
  // A version of WriteCommon used by WebIDL bindings
  void WriteCommon(JSContext *cx,
                   const mozilla::dom::Sequence<nsString>& aText,
                   bool aNewlineTerminate,
                   mozilla::ErrorResult& rv);

  nsresult CreateAndAddWyciwygChannel(void);
  nsresult RemoveWyciwygChannel(void);

  // This should *ONLY* be used in GetCookie/SetCookie.
  already_AddRefed<nsIChannel> CreateDummyChannelForCookies(nsIURI* aCodebaseURI);

  /**
   * Like IsEditingOn(), but will flush as needed first.
   */
  bool IsEditingOnAfterFlush();

  void *GenerateParserKey(void);
=======
  static void DocumentWriteTerminationFunc(nsISupports* aRef);
>>>>>>> upstream-releases

  // A helper class to keep nsContentList objects alive for a short period of
  // time. Note, when the final Release is called on an nsContentList object, it
  // removes itself from MutationObserver list.
  class ContentListHolder : public mozilla::Runnable {
   public:
    ContentListHolder(nsHTMLDocument* aDocument, nsContentList* aFormList,
                      nsContentList* aFormControlList)
        : mozilla::Runnable("ContentListHolder"),
          mDocument(aDocument),
          mFormList(aFormList),
          mFormControlList(aFormControlList) {}

    ~ContentListHolder() {
      MOZ_ASSERT(!mDocument->mContentListHolder ||
                 mDocument->mContentListHolder == this);
      mDocument->mContentListHolder = nullptr;
    }

    RefPtr<nsHTMLDocument> mDocument;
    RefPtr<nsContentList> mFormList;
    RefPtr<nsContentList> mFormControlList;
  };

  friend class ContentListHolder;
  ContentListHolder* mContentListHolder;

  RefPtr<mozilla::dom::HTMLAllCollection> mAll;

  /** # of forms in the document, synchronously set */
  int32_t mNumForms;

  static void TryHintCharset(nsIContentViewer* aContentViewer,
                             int32_t& aCharsetSource,
                             NotNull<const Encoding*>& aEncoding);
  void TryUserForcedCharset(nsIContentViewer* aCv, nsIDocShell* aDocShell,
                            int32_t& aCharsetSource,
                            NotNull<const Encoding*>& aEncoding);
  static void TryCacheCharset(nsICachingChannel* aCachingChannel,
                              int32_t& aCharsetSource,
                              NotNull<const Encoding*>& aEncoding);
  void TryParentCharset(nsIDocShell* aDocShell, int32_t& charsetSource,
                        NotNull<const Encoding*>& aEncoding);
  void TryTLD(int32_t& aCharsetSource, NotNull<const Encoding*>& aCharset);
<<<<<<< HEAD
  void TryFallback(int32_t& aCharsetSource,
                   NotNull<const Encoding*>& aEncoding);

  // Override so we can munge the charset on our wyciwyg channel as needed.
  virtual void SetDocumentCharacterSet(
      NotNull<const Encoding*> aEncoding) override;

  // Tracks if we are currently processing any document.write calls (either
  // implicit or explicit). Note that if a write call writes out something which
  // would block the parser, then mWriteLevel will be incorrect until the parser
  // finishes processing that script.
  uint32_t mWriteLevel;
||||||| merged common ancestors
  void TryFallback(int32_t& aCharsetSource,
                   NotNull<const Encoding*>& aEncoding);

  // Override so we can munge the charset on our wyciwyg channel as needed.
  virtual void
    SetDocumentCharacterSet(NotNull<const Encoding*> aEncoding) override;

  // Tracks if we are currently processing any document.write calls (either
  // implicit or explicit). Note that if a write call writes out something which
  // would block the parser, then mWriteLevel will be incorrect until the parser
  // finishes processing that script.
  uint32_t mWriteLevel;
=======
  static void TryFallback(int32_t& aCharsetSource,
                          NotNull<const Encoding*>& aEncoding);
>>>>>>> upstream-releases

  // Load flags of the document's channel
  uint32_t mLoadFlags;

  bool mWarnedWidthHeight;

<<<<<<< HEAD
  nsCOMPtr<nsIWyciwygChannel> mWyciwygChannel;

  /* Midas implementation */
  nsresult GetMidasCommandManager(nsICommandManager** aCommandManager);

  nsCOMPtr<nsICommandManager> mMidasCommandManager;

  nsresult TurnEditingOff();
  nsresult EditingStateChanged();
  void MaybeEditingStateChanged();

  uint32_t mContentEditableCount;
  EditingState mEditingState;

  // When false, the .cookies property is completely disabled
  bool mDisableCookieAccess;

||||||| merged common ancestors
  nsCOMPtr<nsIWyciwygChannel> mWyciwygChannel;

  /* Midas implementation */
  nsresult   GetMidasCommandManager(nsICommandManager** aCommandManager);

  nsCOMPtr<nsICommandManager> mMidasCommandManager;

  nsresult TurnEditingOff();
  nsresult EditingStateChanged();
  void MaybeEditingStateChanged();

  uint32_t mContentEditableCount;
  EditingState mEditingState;

  // When false, the .cookies property is completely disabled
  bool mDisableCookieAccess;

=======
>>>>>>> upstream-releases
  /**
   * Set to true once we know that we are loading plain text content.
   */
  bool mIsPlainText;
};

<<<<<<< HEAD
inline nsHTMLDocument* nsIDocument::AsHTMLDocument() {
||||||| merged common ancestors
inline nsHTMLDocument*
nsIDocument::AsHTMLDocument()
{
=======
namespace mozilla {
namespace dom {

inline nsHTMLDocument* Document::AsHTMLDocument() {
>>>>>>> upstream-releases
  MOZ_ASSERT(IsHTMLOrXHTML());
  return static_cast<nsHTMLDocument*>(this);
}

<<<<<<< HEAD
#define NS_HTML_DOCUMENT_INTERFACE_TABLE_BEGIN(_class) \
  NS_DOCUMENT_INTERFACE_TABLE_BEGIN(_class)            \
  NS_INTERFACE_TABLE_ENTRY(_class, nsIHTMLDocument)
||||||| merged common ancestors
#define NS_HTML_DOCUMENT_INTERFACE_TABLE_BEGIN(_class)                        \
    NS_DOCUMENT_INTERFACE_TABLE_BEGIN(_class)                                 \
    NS_INTERFACE_TABLE_ENTRY(_class, nsIHTMLDocument)
=======
}  // namespace dom
}  // namespace mozilla
>>>>>>> upstream-releases

#endif /* nsHTMLDocument_h___ */
