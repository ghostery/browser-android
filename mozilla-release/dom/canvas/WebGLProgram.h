/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef WEBGL_PROGRAM_H_
#define WEBGL_PROGRAM_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "mozilla/LinkedList.h"
#include "mozilla/RefPtr.h"
#include "mozilla/WeakPtr.h"
#include "nsString.h"
#include "nsWrapperCache.h"

#include "CacheInvalidator.h"
#include "WebGLContext.h"
#include "WebGLObjectModel.h"

namespace mozilla {
class ErrorResult;
class WebGLActiveInfo;
class WebGLProgram;
class WebGLShader;
class WebGLUniformLocation;

namespace dom {
template <typename>
struct Nullable;
class OwningUnsignedLongOrUint32ArrayOrBoolean;
template <typename>
class Sequence;
}  // namespace dom

namespace webgl {

enum class TextureBaseType : uint8_t;

struct AttribInfo final {
  const RefPtr<WebGLActiveInfo> mActiveInfo;
  const GLint mLoc;  // -1 for active built-ins
};

struct UniformInfo final {
  typedef decltype(WebGLContext::mBound2DTextures) TexListT;

  const RefPtr<WebGLActiveInfo> mActiveInfo;
  const TexListT* const mSamplerTexList;
  const webgl::TextureBaseType mTexBaseType;
  const bool mIsShadowSampler;

  std::vector<uint32_t> mSamplerValues;

 protected:
  static const TexListT* GetTexList(WebGLActiveInfo* activeInfo);

 public:
  explicit UniformInfo(WebGLActiveInfo* activeInfo);
};

<<<<<<< HEAD
struct UniformBlockInfo final {
  const nsCString mUserName;
  const nsCString mMappedName;
  const uint32_t mDataSize;

  const IndexedBufferBinding* mBinding;

  UniformBlockInfo(WebGLContext* webgl, const nsACString& userName,
                   const nsACString& mappedName, uint32_t dataSize)
      : mUserName(userName),
        mMappedName(mappedName),
        mDataSize(dataSize),
        mBinding(&webgl->mIndexedUniformBufferBindings[0]) {}
};
||||||| merged common ancestors
struct UniformBlockInfo final
{
    const nsCString mUserName;
    const nsCString mMappedName;
    const uint32_t mDataSize;

    const IndexedBufferBinding* mBinding;

    UniformBlockInfo(WebGLContext* webgl, const nsACString& userName,
                     const nsACString& mappedName, uint32_t dataSize)
        : mUserName(userName)
        , mMappedName(mappedName)
        , mDataSize(dataSize)
        , mBinding(&webgl->mIndexedUniformBufferBindings[0])
    { }
};
=======
struct UniformBlockInfo final {
  const nsCString mUserName;
  const nsCString mMappedName;
  const uint32_t mDataSize;
>>>>>>> upstream-releases

<<<<<<< HEAD
struct CachedDrawFetchLimits final {
  uint64_t maxVerts;
  uint64_t maxInstances;
||||||| merged common ancestors
struct CachedDrawFetchLimits final {
    uint64_t maxVerts;
    uint64_t maxInstances;
=======
  const IndexedBufferBinding* mBinding;

  UniformBlockInfo(WebGLContext* webgl, const nsACString& userName,
                   const nsACString& mappedName, uint32_t dataSize)
      : mUserName(userName),
        mMappedName(mappedName),
        mDataSize(dataSize),
        mBinding(&webgl->mIndexedUniformBufferBindings[0]) {}
>>>>>>> upstream-releases
};

<<<<<<< HEAD
struct LinkedProgramInfo final : public RefCounted<LinkedProgramInfo>,
                                 public SupportsWeakPtr<LinkedProgramInfo>,
                                 public CacheInvalidator {
  friend class mozilla::WebGLProgram;
||||||| merged common ancestors
struct LinkedProgramInfo final
    : public RefCounted<LinkedProgramInfo>
    , public SupportsWeakPtr<LinkedProgramInfo>
    , public CacheInvalidator
{
    friend class mozilla::WebGLProgram;
=======
struct FragOutputInfo final {
  const uint8_t loc;
  const nsCString userName;
  const nsCString mappedName;
  const TextureBaseType baseType;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  MOZ_DECLARE_REFCOUNTED_TYPENAME(LinkedProgramInfo)
  MOZ_DECLARE_WEAKREFERENCE_TYPENAME(LinkedProgramInfo)
||||||| merged common ancestors
    MOZ_DECLARE_REFCOUNTED_TYPENAME(LinkedProgramInfo)
    MOZ_DECLARE_WEAKREFERENCE_TYPENAME(LinkedProgramInfo)
=======
struct CachedDrawFetchLimits final {
  uint64_t maxVerts = 0;
  uint64_t maxInstances = 0;
  std::vector<BufferAndIndex> usedBuffers;
};
>>>>>>> upstream-releases

<<<<<<< HEAD
  //////
||||||| merged common ancestors
    //////
=======
struct LinkedProgramInfo final : public RefCounted<LinkedProgramInfo>,
                                 public SupportsWeakPtr<LinkedProgramInfo>,
                                 public CacheInvalidator {
  friend class mozilla::WebGLProgram;
>>>>>>> upstream-releases

<<<<<<< HEAD
  WebGLProgram* const prog;
  const GLenum transformFeedbackBufferMode;
||||||| merged common ancestors
    WebGLProgram* const prog;
    const GLenum transformFeedbackBufferMode;
=======
  MOZ_DECLARE_REFCOUNTED_TYPENAME(LinkedProgramInfo)
  MOZ_DECLARE_WEAKREFERENCE_TYPENAME(LinkedProgramInfo)
>>>>>>> upstream-releases

<<<<<<< HEAD
  std::vector<AttribInfo> attribs;
  std::vector<UniformInfo*> uniforms;            // Owns its contents.
  std::vector<UniformBlockInfo*> uniformBlocks;  // Owns its contents.
  std::vector<RefPtr<WebGLActiveInfo>> transformFeedbackVaryings;
||||||| merged common ancestors
    std::vector<AttribInfo> attribs;
    std::vector<UniformInfo*> uniforms; // Owns its contents.
    std::vector<UniformBlockInfo*> uniformBlocks; // Owns its contents.
    std::vector<RefPtr<WebGLActiveInfo>> transformFeedbackVaryings;
=======
  //////
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Needed for draw call validation.
  std::vector<UniformInfo*> uniformSamplers;
||||||| merged common ancestors
    // Needed for draw call validation.
    std::vector<UniformInfo*> uniformSamplers;
=======
  WebGLProgram* const prog;
  const GLenum transformFeedbackBufferMode;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mutable std::vector<size_t> componentsPerTFVert;
||||||| merged common ancestors
    mutable std::vector<size_t> componentsPerTFVert;
=======
  std::vector<AttribInfo> attribs;
  std::vector<UniformInfo*> uniforms;            // Owns its contents.
  std::vector<UniformBlockInfo*> uniformBlocks;  // Owns its contents.
  std::vector<RefPtr<WebGLActiveInfo>> transformFeedbackVaryings;
  std::unordered_map<uint8_t, const FragOutputInfo> fragOutputs;
  uint8_t zLayerCount = 1;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool attrib0Active;
||||||| merged common ancestors
    bool attrib0Active;
=======
  // Needed for draw call validation.
  std::vector<UniformInfo*> uniformSamplers;
>>>>>>> upstream-releases

<<<<<<< HEAD
  //////
||||||| merged common ancestors
    //////
=======
  mutable std::vector<size_t> componentsPerTFVert;
>>>>>>> upstream-releases

<<<<<<< HEAD
  // The maps for the frag data names to the translated names.
  std::map<nsCString, const nsCString> fragDataMap;
||||||| merged common ancestors
    // The maps for the frag data names to the translated names.
    std::map<nsCString, const nsCString> fragDataMap;
=======
  bool attrib0Active;
>>>>>>> upstream-releases

  //////

  mutable CacheWeakMap<const WebGLVertexArray*, CachedDrawFetchLimits>
      mDrawFetchCache;

  const CachedDrawFetchLimits* GetDrawFetchLimits() const;

  //////

  explicit LinkedProgramInfo(WebGLProgram* prog);
  ~LinkedProgramInfo();

<<<<<<< HEAD
  bool FindAttrib(const nsCString& userName,
                  const AttribInfo** const out_info) const;
  bool FindUniform(const nsCString& userName, nsCString* const out_mappedName,
                   size_t* const out_arrayIndex,
                   UniformInfo** const out_info) const;
  bool MapFragDataName(const nsCString& userName,
                       nsCString* const out_mappedName) const;
||||||| merged common ancestors
    bool FindAttrib(const nsCString& userName, const AttribInfo** const out_info) const;
    bool FindUniform(const nsCString& userName, nsCString* const out_mappedName,
                     size_t* const out_arrayIndex, UniformInfo** const out_info) const;
    bool MapFragDataName(const nsCString& userName,
                         nsCString* const out_mappedName) const;
=======
  bool FindAttrib(const nsCString& userName,
                  const AttribInfo** const out_info) const;
  bool FindUniform(const nsCString& userName, nsCString* const out_mappedName,
                   size_t* const out_arrayIndex,
                   UniformInfo** const out_info) const;
>>>>>>> upstream-releases
};

<<<<<<< HEAD
}  // namespace webgl

class WebGLProgram final : public nsWrapperCache,
                           public WebGLRefCountedObject<WebGLProgram>,
                           public LinkedListElement<WebGLProgram> {
  friend class WebGLTransformFeedback;
  friend struct webgl::LinkedProgramInfo;

 public:
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(WebGLProgram)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(WebGLProgram)

  explicit WebGLProgram(WebGLContext* webgl);

  void Delete();

  // GL funcs
  void AttachShader(WebGLShader* shader);
  void BindAttribLocation(GLuint index, const nsAString& name);
  void DetachShader(const WebGLShader* shader);
  already_AddRefed<WebGLActiveInfo> GetActiveAttrib(GLuint index) const;
  already_AddRefed<WebGLActiveInfo> GetActiveUniform(GLuint index) const;
  void GetAttachedShaders(nsTArray<RefPtr<WebGLShader>>* const out) const;
  GLint GetAttribLocation(const nsAString& name) const;
  GLint GetFragDataLocation(const nsAString& name) const;
  void GetProgramInfoLog(nsAString* const out) const;
  JS::Value GetProgramParameter(GLenum pname) const;
  GLuint GetUniformBlockIndex(const nsAString& name) const;
  void GetActiveUniformBlockName(GLuint uniformBlockIndex,
                                 nsAString& name) const;
  JS::Value GetActiveUniformBlockParam(GLuint uniformBlockIndex,
                                       GLenum pname) const;
  JS::Value GetActiveUniformBlockActiveUniforms(
      JSContext* cx, GLuint uniformBlockIndex,
      ErrorResult* const out_error) const;
  already_AddRefed<WebGLUniformLocation> GetUniformLocation(
      const nsAString& name) const;
  void GetUniformIndices(const dom::Sequence<nsString>& uniformNames,
                         dom::Nullable<nsTArray<GLuint>>& retval) const;
  void UniformBlockBinding(GLuint uniformBlockIndex,
                           GLuint uniformBlockBinding) const;

  void LinkProgram();
  bool UseProgram() const;
  void ValidateProgram() const;

  ////////////////

  bool FindAttribUserNameByMappedName(const nsACString& mappedName,
                                      nsCString* const out_userName) const;
  bool FindVaryingByMappedName(const nsACString& mappedName,
                               nsCString* const out_userName,
                               bool* const out_isArray) const;
  bool FindUniformByMappedName(const nsACString& mappedName,
                               nsCString* const out_userName,
                               bool* const out_isArray) const;
  bool UnmapUniformBlockName(const nsCString& mappedName,
                             nsCString* const out_userName) const;

  void TransformFeedbackVaryings(const dom::Sequence<nsString>& varyings,
                                 GLenum bufferMode);
  already_AddRefed<WebGLActiveInfo> GetTransformFeedbackVarying(
      GLuint index) const;

  void EnumerateFragOutputs(
      std::map<nsCString, const nsCString>& out_FragOutputs) const;

  bool IsLinked() const { return mMostRecentLinkInfo; }

  const webgl::LinkedProgramInfo* LinkInfo() const {
    return mMostRecentLinkInfo.get();
  }

  WebGLContext* GetParentObject() const { return mContext; }

  virtual JSObject* WrapObject(JSContext* js,
                               JS::Handle<JSObject*> givenProto) override;

 private:
  ~WebGLProgram();

  void LinkAndUpdate();
  bool ValidateForLink();
  bool ValidateAfterTentativeLink(nsCString* const out_linkLog) const;

 public:
  const GLuint mGLName;

 private:
  WebGLRefPtr<WebGLShader> mVertShader;
  WebGLRefPtr<WebGLShader> mFragShader;
  size_t mNumActiveTFOs;

  std::map<nsCString, GLuint> mNextLink_BoundAttribLocs;

  std::vector<nsString> mNextLink_TransformFeedbackVaryings;
  GLenum mNextLink_TransformFeedbackBufferMode;

  nsCString mLinkLog;
  RefPtr<const webgl::LinkedProgramInfo> mMostRecentLinkInfo;
||||||| merged common ancestors
} // namespace webgl

class WebGLProgram final
    : public nsWrapperCache
    , public WebGLRefCountedObject<WebGLProgram>
    , public LinkedListElement<WebGLProgram>
{
    friend class WebGLTransformFeedback;
    friend struct webgl::LinkedProgramInfo;

public:
    NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(WebGLProgram)
    NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(WebGLProgram)

    explicit WebGLProgram(WebGLContext* webgl);

    void Delete();

    // GL funcs
    void AttachShader(WebGLShader* shader);
    void BindAttribLocation(GLuint index, const nsAString& name);
    void DetachShader(const WebGLShader* shader);
    already_AddRefed<WebGLActiveInfo> GetActiveAttrib(GLuint index) const;
    already_AddRefed<WebGLActiveInfo> GetActiveUniform(GLuint index) const;
    void GetAttachedShaders(nsTArray<RefPtr<WebGLShader>>* const out) const;
    GLint GetAttribLocation(const nsAString& name) const;
    GLint GetFragDataLocation(const nsAString& name) const;
    void GetProgramInfoLog(nsAString* const out) const;
    JS::Value GetProgramParameter(GLenum pname) const;
    GLuint GetUniformBlockIndex(const nsAString& name) const;
    void GetActiveUniformBlockName(GLuint uniformBlockIndex, nsAString& name) const;
    JS::Value GetActiveUniformBlockParam(GLuint uniformBlockIndex, GLenum pname) const;
    JS::Value GetActiveUniformBlockActiveUniforms(JSContext* cx, GLuint uniformBlockIndex,
                                                  ErrorResult* const out_error) const;
    already_AddRefed<WebGLUniformLocation> GetUniformLocation(const nsAString& name) const;
    void GetUniformIndices(const dom::Sequence<nsString>& uniformNames,
                           dom::Nullable< nsTArray<GLuint> >& retval) const;
    void UniformBlockBinding(GLuint uniformBlockIndex, GLuint uniformBlockBinding) const;

    void LinkProgram();
    bool UseProgram() const;
    void ValidateProgram() const;

    ////////////////

    bool FindAttribUserNameByMappedName(const nsACString& mappedName,
                                        nsCString* const out_userName) const;
    bool FindVaryingByMappedName(const nsACString& mappedName,
                                 nsCString* const out_userName,
                                 bool* const out_isArray) const;
    bool FindUniformByMappedName(const nsACString& mappedName,
                                 nsCString* const out_userName,
                                 bool* const out_isArray) const;
    bool UnmapUniformBlockName(const nsCString& mappedName,
                               nsCString* const out_userName) const;

    void TransformFeedbackVaryings(const dom::Sequence<nsString>& varyings,
                                   GLenum bufferMode);
    already_AddRefed<WebGLActiveInfo> GetTransformFeedbackVarying(GLuint index) const;

    void EnumerateFragOutputs(std::map<nsCString, const nsCString> &out_FragOutputs) const;

    bool IsLinked() const { return mMostRecentLinkInfo; }

    const webgl::LinkedProgramInfo* LinkInfo() const {
        return mMostRecentLinkInfo.get();
    }

    WebGLContext* GetParentObject() const {
        return mContext;
    }

    virtual JSObject* WrapObject(JSContext* js, JS::Handle<JSObject*> givenProto) override;

private:
    ~WebGLProgram();

    void LinkAndUpdate();
    bool ValidateForLink();
    bool ValidateAfterTentativeLink(nsCString* const out_linkLog) const;

public:
    const GLuint mGLName;

private:
    WebGLRefPtr<WebGLShader> mVertShader;
    WebGLRefPtr<WebGLShader> mFragShader;
    size_t mNumActiveTFOs;

    std::map<nsCString, GLuint> mNextLink_BoundAttribLocs;

    std::vector<nsString> mNextLink_TransformFeedbackVaryings;
    GLenum mNextLink_TransformFeedbackBufferMode;

    nsCString mLinkLog;
    RefPtr<const webgl::LinkedProgramInfo> mMostRecentLinkInfo;
=======
}  // namespace webgl

class WebGLProgram final : public nsWrapperCache,
                           public WebGLRefCountedObject<WebGLProgram>,
                           public LinkedListElement<WebGLProgram> {
  friend class WebGLTransformFeedback;
  friend struct webgl::LinkedProgramInfo;

 public:
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(WebGLProgram)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(WebGLProgram)

  explicit WebGLProgram(WebGLContext* webgl);

  void Delete();

  // GL funcs
  void AttachShader(WebGLShader* shader);
  void BindAttribLocation(GLuint index, const nsAString& name);
  void DetachShader(const WebGLShader* shader);
  already_AddRefed<WebGLActiveInfo> GetActiveAttrib(GLuint index) const;
  already_AddRefed<WebGLActiveInfo> GetActiveUniform(GLuint index) const;
  void GetAttachedShaders(nsTArray<RefPtr<WebGLShader>>* const out) const;
  GLint GetAttribLocation(const nsAString& name) const;
  GLint GetFragDataLocation(const nsAString& name) const;
  void GetProgramInfoLog(nsAString* const out) const;
  JS::Value GetProgramParameter(GLenum pname) const;
  GLuint GetUniformBlockIndex(const nsAString& name) const;
  void GetActiveUniformBlockName(GLuint uniformBlockIndex,
                                 nsAString& name) const;
  JS::Value GetActiveUniformBlockParam(GLuint uniformBlockIndex,
                                       GLenum pname) const;
  JS::Value GetActiveUniformBlockActiveUniforms(
      JSContext* cx, GLuint uniformBlockIndex,
      ErrorResult* const out_error) const;
  already_AddRefed<WebGLUniformLocation> GetUniformLocation(
      const nsAString& name) const;
  void GetUniformIndices(const dom::Sequence<nsString>& uniformNames,
                         dom::Nullable<nsTArray<GLuint>>& retval) const;
  void UniformBlockBinding(GLuint uniformBlockIndex,
                           GLuint uniformBlockBinding) const;

  void LinkProgram();
  bool UseProgram() const;
  void ValidateProgram() const;

  ////////////////

  bool FindAttribUserNameByMappedName(const nsACString& mappedName,
                                      nsCString* const out_userName) const;
  bool FindVaryingByMappedName(const nsACString& mappedName,
                               nsCString* const out_userName,
                               bool* const out_isArray) const;
  bool FindUniformByMappedName(const nsACString& mappedName,
                               nsCString* const out_userName,
                               bool* const out_isArray) const;
  bool UnmapUniformBlockName(const nsCString& mappedName,
                             nsCString* const out_userName) const;

  void TransformFeedbackVaryings(const dom::Sequence<nsString>& varyings,
                                 GLenum bufferMode);
  already_AddRefed<WebGLActiveInfo> GetTransformFeedbackVarying(
      GLuint index) const;

  void EnumerateFragOutputs(
      std::map<nsCString, const nsCString>& out_FragOutputs) const;

  bool IsLinked() const { return mMostRecentLinkInfo; }

  const webgl::LinkedProgramInfo* LinkInfo() const {
    return mMostRecentLinkInfo.get();
  }

  const auto& VertShader() const { return mVertShader; }
  const auto& FragShader() const { return mFragShader; }

  WebGLContext* GetParentObject() const { return mContext; }

  virtual JSObject* WrapObject(JSContext* js,
                               JS::Handle<JSObject*> givenProto) override;

 private:
  ~WebGLProgram();

  void LinkAndUpdate();
  bool ValidateForLink();
  bool ValidateAfterTentativeLink(nsCString* const out_linkLog) const;

 public:
  const GLuint mGLName;

 private:
  WebGLRefPtr<WebGLShader> mVertShader;
  WebGLRefPtr<WebGLShader> mFragShader;
  size_t mNumActiveTFOs;

  std::map<std::string, GLuint> mNextLink_BoundAttribLocs;

  std::vector<nsString> mNextLink_TransformFeedbackVaryings;
  GLenum mNextLink_TransformFeedbackBufferMode;

  nsCString mLinkLog;
  RefPtr<const webgl::LinkedProgramInfo> mMostRecentLinkInfo;
>>>>>>> upstream-releases
};

}  // namespace mozilla

#endif  // WEBGL_PROGRAM_H_
