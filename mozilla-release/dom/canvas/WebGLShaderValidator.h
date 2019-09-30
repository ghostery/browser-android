/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef WEBGL_SHADER_VALIDATOR_H_
#define WEBGL_SHADER_VALIDATOR_H_

#include <string>

#include "GLDefs.h"
#include "GLSLANG/ShaderLang.h"
#include "nsString.h"

namespace mozilla {
namespace webgl {

<<<<<<< HEAD
class ShaderValidator final {
  const ShHandle mHandle;
  const ShCompileOptions mCompileOptions;
  const int mMaxVaryingVectors;
  bool mHasRun;

 public:
  static ShaderValidator* Create(GLenum shaderType, ShShaderSpec spec,
                                 ShShaderOutput outputLanguage,
                                 const ShBuiltInResources& resources,
                                 ShCompileOptions compileOptions);

 private:
  ShaderValidator(ShHandle handle, ShCompileOptions compileOptions,
                  int maxVaryingVectors)
      : mHandle(handle),
        mCompileOptions(compileOptions),
        mMaxVaryingVectors(maxVaryingVectors),
        mHasRun(false) {}

 public:
  ~ShaderValidator();

  bool ValidateAndTranslate(const char* source);
  void GetInfoLog(nsACString* out) const;
  void GetOutput(nsACString* out) const;
  bool CanLinkTo(const ShaderValidator* prev, nsCString* const out_log) const;
  size_t CalcNumSamplerUniforms() const;
  size_t NumAttributes() const;

  bool FindAttribUserNameByMappedName(
      const std::string& mappedName,
      const std::string** const out_userName) const;

  bool FindAttribMappedNameByUserName(
      const std::string& userName,
      const std::string** const out_mappedName) const;

  bool FindVaryingMappedNameByUserName(
      const std::string& userName,
      const std::string** const out_mappedName) const;

  bool FindVaryingByMappedName(const std::string& mappedName,
                               std::string* const out_userName,
                               bool* const out_isArray) const;
  bool FindUniformByMappedName(const std::string& mappedName,
                               std::string* const out_userName,
                               bool* const out_isArray) const;
  bool UnmapUniformBlockName(const nsACString& baseMappedName,
                             nsCString* const out_baseUserName) const;

  void EnumerateFragOutputs(
      std::map<nsCString, const nsCString>& out_FragOutputs) const;

  bool ValidateTransformFeedback(
      const std::vector<nsString>& userNames, uint32_t maxComponents,
      nsCString* const out_errorText,
      std::vector<std::string>* const out_mappedNames);
||||||| merged common ancestors
class ShaderValidator final
{
    const ShHandle mHandle;
    const ShCompileOptions mCompileOptions;
    const int mMaxVaryingVectors;
    bool mHasRun;

public:
    static ShaderValidator* Create(GLenum shaderType, ShShaderSpec spec,
                                   ShShaderOutput outputLanguage,
                                   const ShBuiltInResources& resources,
                                   ShCompileOptions compileOptions);

private:
    ShaderValidator(ShHandle handle, ShCompileOptions compileOptions,
                    int maxVaryingVectors)
        : mHandle(handle)
        , mCompileOptions(compileOptions)
        , mMaxVaryingVectors(maxVaryingVectors)
        , mHasRun(false)
    { }

public:
    ~ShaderValidator();

    bool ValidateAndTranslate(const char* source);
    void GetInfoLog(nsACString* out) const;
    void GetOutput(nsACString* out) const;
    bool CanLinkTo(const ShaderValidator* prev, nsCString* const out_log) const;
    size_t CalcNumSamplerUniforms() const;
    size_t NumAttributes() const;

    bool FindAttribUserNameByMappedName(const std::string& mappedName,
                                        const std::string** const out_userName) const;

    bool FindAttribMappedNameByUserName(const std::string& userName,
                                        const std::string** const out_mappedName) const;

    bool FindVaryingMappedNameByUserName(const std::string& userName,
                                         const std::string** const out_mappedName) const;

    bool FindVaryingByMappedName(const std::string& mappedName,
                                 std::string* const out_userName,
                                 bool* const out_isArray) const;
    bool FindUniformByMappedName(const std::string& mappedName,
                                 std::string* const out_userName,
                                 bool* const out_isArray) const;
    bool UnmapUniformBlockName(const nsACString& baseMappedName,
                               nsCString* const out_baseUserName) const;

    void EnumerateFragOutputs(std::map<nsCString, const nsCString> &out_FragOutputs) const;

    bool ValidateTransformFeedback(const std::vector<nsString>& userNames,
                                   uint32_t maxComponents, nsCString* const out_errorText,
                                   std::vector<std::string>* const out_mappedNames);
=======
class ShaderValidatorResults final {
 public:
  std::string mInfoLog;
  bool mValid = false;

  std::string mObjectCode;
  int mShaderVersion = 0;
  int mVertexShaderNumViews = 0;

  std::vector<sh::Attribute> mAttributes;
  std::vector<sh::InterfaceBlock> mInterfaceBlocks;
  std::vector<sh::OutputVariable> mOutputVariables;
  std::vector<sh::Uniform> mUniforms;
  std::vector<sh::Varying> mVaryings;

  int mMaxVaryingVectors = 0;

  bool CanLinkTo(const ShaderValidatorResults& vert,
                 nsCString* const out_log) const;
  bool FindUniformByMappedName(const std::string& mappedName,
                               std::string* const out_userName,
                               bool* const out_isArray) const;
  size_t SizeOfIncludingThis(MallocSizeOf) const;
};

class ShaderValidator final {
 public:
  const ShHandle mHandle;

 private:
  const ShCompileOptions mCompileOptions;
  const int mMaxVaryingVectors;

 public:
  static std::unique_ptr<ShaderValidator> Create(
      GLenum shaderType, ShShaderSpec spec, ShShaderOutput outputLanguage,
      const ShBuiltInResources& resources, ShCompileOptions compileOptions);

 private:
  ShaderValidator(ShHandle handle, ShCompileOptions compileOptions,
                  int maxVaryingVectors)
      : mHandle(handle),
        mCompileOptions(compileOptions),
        mMaxVaryingVectors(maxVaryingVectors) {}

 public:
  ~ShaderValidator();

  std::unique_ptr<const ShaderValidatorResults> ValidateAndTranslate(
      const char*) const;
>>>>>>> upstream-releases
};

}  // namespace webgl
}  // namespace mozilla

#endif  // WEBGL_SHADER_VALIDATOR_H_
