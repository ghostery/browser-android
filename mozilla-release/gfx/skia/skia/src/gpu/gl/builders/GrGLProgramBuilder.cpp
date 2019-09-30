/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrGLProgramBuilder.h"

#include "GrAutoLocaleSetter.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrCoordTransform.h"
#include "GrGLProgramBuilder.h"
#include "GrProgramDesc.h"
#include "GrShaderCaps.h"
#include "GrSwizzle.h"
#include "SkAutoMalloc.h"
#include "SkATrace.h"
#include "SkTraceEvent.h"
#include "gl/GrGLGpu.h"
#include "gl/GrGLProgram.h"
#include "gl/builders/GrGLShaderStringBuilder.h"
#include "glsl/GrGLSLFragmentProcessor.h"
#include "glsl/GrGLSLGeometryProcessor.h"
#include "glsl/GrGLSLProgramDataManager.h"
#include "glsl/GrGLSLXferProcessor.h"

#define GL_CALL(X) GR_GL_CALL(this->gpu()->glInterface(), X)
#define GL_CALL_RET(R, X) GR_GL_CALL_RET(this->gpu()->glInterface(), R, X)

<<<<<<< HEAD
GrGLProgram* GrGLProgramBuilder::CreateProgram(const GrPrimitiveProcessor& primProc,
                                               const GrPipeline& pipeline,
||||||| merged common ancestors
GrGLProgram* GrGLProgramBuilder::CreateProgram(const GrPipeline& pipeline,
                                               const GrPrimitiveProcessor& primProc,
=======
GrGLProgram* GrGLProgramBuilder::CreateProgram(GrRenderTarget* renderTarget, GrSurfaceOrigin origin,
                                               const GrPrimitiveProcessor& primProc,
                                               const GrTextureProxy* const primProcProxies[],
                                               const GrPipeline& pipeline,
>>>>>>> upstream-releases
                                               GrProgramDesc* desc,
                                               GrGLGpu* gpu) {
    SkASSERT(!pipeline.isBad());

    ATRACE_ANDROID_FRAMEWORK("Shader Compile");
    GrAutoLocaleSetter als("C");

    // create a builder.  This will be handed off to effects so they can use it to add
    // uniforms, varyings, textures, etc
    GrGLProgramBuilder builder(gpu, renderTarget, origin,
                               pipeline, primProc, primProcProxies, desc);

<<<<<<< HEAD
    auto persistentCache = gpu->getContext()->contextPriv().getPersistentCache();
    if (persistentCache && gpu->glCaps().programBinarySupport()) {
||||||| merged common ancestors
    if (gpu->getContext()->getPersistentCache() && gpu->glCaps().programBinarySupport()) {
=======
    auto persistentCache = gpu->getContext()->priv().getPersistentCache();
    if (persistentCache) {
>>>>>>> upstream-releases
        sk_sp<SkData> key = SkData::MakeWithoutCopy(desc->asKey(), desc->keyLength());
        builder.fCached = persistentCache->load(*key);
        // the eventual end goal is to completely skip emitAndInstallProcs on a cache hit, but it's
        // doing necessary setup in addition to generating the SkSL code. Currently we are only able
        // to skip the SkSL->GLSL step on a cache hit.
    }
    if (!builder.emitAndInstallProcs()) {
        return nullptr;
    }
    return builder.finalize();
}

/////////////////////////////////////////////////////////////////////////////

GrGLProgramBuilder::GrGLProgramBuilder(GrGLGpu* gpu,
                                       GrRenderTarget* renderTarget,
                                       GrSurfaceOrigin origin,
                                       const GrPipeline& pipeline,
                                       const GrPrimitiveProcessor& primProc,
                                       const GrTextureProxy* const primProcProxies[],
                                       GrProgramDesc* desc)
<<<<<<< HEAD
        : INHERITED(primProc, pipeline, desc)
        , fGpu(gpu)
        , fVaryingHandler(this)
        , fUniformHandler(this)
        , fVertexAttributeCnt(0)
        , fInstanceAttributeCnt(0)
        , fVertexStride(0)
        , fInstanceStride(0) {}
||||||| merged common ancestors
    : INHERITED(pipeline, primProc, desc)
    , fGpu(gpu)
    , fVaryingHandler(this)
    , fUniformHandler(this) {
}
=======
        : INHERITED(renderTarget, origin, primProc, primProcProxies, pipeline, desc)
        , fGpu(gpu)
        , fVaryingHandler(this)
        , fUniformHandler(this)
        , fVertexAttributeCnt(0)
        , fInstanceAttributeCnt(0)
        , fVertexStride(0)
        , fInstanceStride(0) {}
>>>>>>> upstream-releases

const GrCaps* GrGLProgramBuilder::caps() const {
    return fGpu->caps();
}

bool GrGLProgramBuilder::compileAndAttachShaders(const char* glsl,
                                                 int length,
                                                 GrGLuint programId,
                                                 GrGLenum type,
                                                 SkTDArray<GrGLuint>* shaderIds,
                                                 const SkSL::Program::Settings& settings,
                                                 const SkSL::Program::Inputs& inputs) {
    GrGLGpu* gpu = this->gpu();
    GrGLuint shaderId = GrGLCompileAndAttachShader(gpu->glContext(),
                                                   programId,
                                                   type,
                                                   glsl,
                                                   length,
                                                   gpu->stats(),
                                                   settings);
    if (!shaderId) {
        return false;
    }

    *shaderIds->append() = shaderId;
    if (inputs.fFlipY) {
        GrProgramDesc* d = this->desc();
        d->setSurfaceOriginKey(GrGLSLFragmentShaderBuilder::KeyForSurfaceOrigin(this->origin()));
    }

    return true;
}

bool GrGLProgramBuilder::compileAndAttachShaders(GrGLSLShaderBuilder& shader,
                                                 GrGLuint programId,
                                                 GrGLenum type,
                                                 SkTDArray<GrGLuint>* shaderIds,
                                                 const SkSL::Program::Settings& settings,
                                                 SkSL::Program::Inputs* outInputs) {
    SkSL::String glsl;
    std::unique_ptr<SkSL::Program> program = GrSkSLtoGLSL(gpu()->glContext(), type,
                                                 shader.fCompilerStrings.begin(),
                                                 shader.fCompilerStringLengths.begin(),
                                                 shader.fCompilerStrings.count(),
                                                 settings,
                                                 &glsl);
    *outInputs = program->fInputs;
    return this->compileAndAttachShaders(glsl.c_str(),
                                         glsl.size(),
                                         programId,
                                         type,
                                         shaderIds,
                                         settings,
                                         *outInputs);
}

<<<<<<< HEAD
void GrGLProgramBuilder::computeCountsAndStrides(GrGLuint programID,
                                                 const GrPrimitiveProcessor& primProc,
                                                 bool bindAttribLocations) {
    fVertexAttributeCnt = primProc.numVertexAttributes();
    fInstanceAttributeCnt = primProc.numInstanceAttributes();
    fAttributes.reset(
            new GrGLProgram::Attribute[fVertexAttributeCnt + fInstanceAttributeCnt]);
    auto addAttr = [&](int i, const auto& a, size_t* stride) {
        fAttributes[i].fCPUType = a.cpuType();
        fAttributes[i].fGPUType = a.gpuType();
        fAttributes[i].fOffset = *stride;
        *stride += a.sizeAlign4();
        fAttributes[i].fLocation = i;
        if (bindAttribLocations) {
            GL_CALL(BindAttribLocation(programID, i, a.name()));
        }
    };
    fVertexStride = 0;
    int i = 0;
    for (; i < fVertexAttributeCnt; i++) {
        addAttr(i, primProc.vertexAttribute(i), &fVertexStride);
        SkASSERT(fAttributes[i].fOffset == primProc.debugOnly_vertexAttributeOffset(i));
    }
    SkASSERT(fVertexStride == primProc.debugOnly_vertexStride());
    fInstanceStride = 0;
    for (int j = 0; j < fInstanceAttributeCnt; j++, ++i) {
        addAttr(i, primProc.instanceAttribute(j), &fInstanceStride);
        SkASSERT(fAttributes[i].fOffset == primProc.debugOnly_instanceAttributeOffset(j));
    }
    SkASSERT(fInstanceStride == primProc.debugOnly_instanceStride());
}

void GrGLProgramBuilder::addInputVars(const SkSL::Program::Inputs& inputs) {
    if (inputs.fRTWidth) {
        this->addRTWidthUniform(SKSL_RTWIDTH_NAME);
    }
    if (inputs.fRTHeight) {
        this->addRTHeightUniform(SKSL_RTHEIGHT_NAME);
    }
}

||||||| merged common ancestors
=======
void GrGLProgramBuilder::computeCountsAndStrides(GrGLuint programID,
                                                 const GrPrimitiveProcessor& primProc,
                                                 bool bindAttribLocations) {
    fVertexAttributeCnt = primProc.numVertexAttributes();
    fInstanceAttributeCnt = primProc.numInstanceAttributes();
    fAttributes.reset(
            new GrGLProgram::Attribute[fVertexAttributeCnt + fInstanceAttributeCnt]);
    auto addAttr = [&](int i, const auto& a, size_t* stride) {
        fAttributes[i].fCPUType = a.cpuType();
        fAttributes[i].fGPUType = a.gpuType();
        fAttributes[i].fOffset = *stride;
        *stride += a.sizeAlign4();
        fAttributes[i].fLocation = i;
        if (bindAttribLocations) {
            GL_CALL(BindAttribLocation(programID, i, a.name()));
        }
    };
    fVertexStride = 0;
    int i = 0;
    for (const auto& attr : primProc.vertexAttributes()) {
        addAttr(i++, attr, &fVertexStride);
    }
    SkASSERT(fVertexStride == primProc.vertexStride());
    fInstanceStride = 0;
    for (const auto& attr : primProc.instanceAttributes()) {
        addAttr(i++, attr, &fInstanceStride);
    }
    SkASSERT(fInstanceStride == primProc.instanceStride());
}

void GrGLProgramBuilder::addInputVars(const SkSL::Program::Inputs& inputs) {
    if (inputs.fRTWidth) {
        this->addRTWidthUniform(SKSL_RTWIDTH_NAME);
    }
    if (inputs.fRTHeight) {
        this->addRTHeightUniform(SKSL_RTHEIGHT_NAME);
    }
}

void GrGLProgramBuilder::storeShaderInCache(const SkSL::Program::Inputs& inputs, GrGLuint programID,
                                            const SkSL::String& glsl) {
    if (!this->gpu()->getContext()->priv().getPersistentCache()) {
        return;
    }
    sk_sp<SkData> key = SkData::MakeWithoutCopy(desc()->asKey(), desc()->keyLength());
    if (fGpu->glCaps().programBinarySupport()) {
        // binary cache
        GrGLsizei length = 0;
        GL_CALL(GetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length));
        if (length > 0) {
            GrGLenum binaryFormat;
            std::unique_ptr<char[]> binary(new char[length]);
            GL_CALL(GetProgramBinary(programID, length, &length, &binaryFormat, binary.get()));
            size_t dataLength = sizeof(inputs) + sizeof(binaryFormat) + length;
            std::unique_ptr<uint8_t[]> data(new uint8_t[dataLength]);
            size_t offset = 0;
            memcpy(data.get() + offset, &inputs, sizeof(inputs));
            offset += sizeof(inputs);
            memcpy(data.get() + offset, &binaryFormat, sizeof(binaryFormat));
            offset += sizeof(binaryFormat);
            memcpy(data.get() + offset, binary.get(), length);
            this->gpu()->getContext()->priv().getPersistentCache()->store(
                                            *key, *SkData::MakeWithoutCopy(data.get(), dataLength));
        }
    } else {
        // source cache
        size_t dataLength = sizeof(inputs) + glsl.length();
        std::unique_ptr<uint8_t[]> data(new uint8_t[dataLength]);
        size_t offset = 0;
        memcpy(data.get() + offset, &inputs, sizeof(inputs));
        offset += sizeof(inputs);
        memcpy(data.get() + offset, glsl.data(), glsl.length());
        this->gpu()->getContext()->priv().getPersistentCache()->store(
                                            *key, *SkData::MakeWithoutCopy(data.get(), dataLength));
    }
}

>>>>>>> upstream-releases
GrGLProgram* GrGLProgramBuilder::finalize() {
    TRACE_EVENT0("skia", TRACE_FUNC);

    // verify we can get a program id
    GrGLuint programID;
    GL_CALL_RET(programID, CreateProgram());
    if (0 == programID) {
        return nullptr;
    }

    if (this->gpu()->glCaps().programBinarySupport() &&
<<<<<<< HEAD
        this->gpu()->getContext()->contextPriv().getPersistentCache()) {
||||||| merged common ancestors
        this->gpu()->getContext()->getPersistentCache()) {
=======
        this->gpu()->getContext()->priv().getPersistentCache()) {
>>>>>>> upstream-releases
        GL_CALL(ProgramParameteri(programID, GR_GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GR_GL_TRUE));
    }

    this->finalizeShaders();

    // compile shaders and bind attributes / uniforms
    const GrPrimitiveProcessor& primProc = this->primitiveProcessor();
    SkSL::Program::Settings settings;
    settings.fCaps = this->gpu()->glCaps().shaderCaps();
<<<<<<< HEAD
    settings.fFlipY = this->pipeline().proxy()->origin() != kTopLeft_GrSurfaceOrigin;
    settings.fSharpenTextures = this->gpu()->getContext()->contextPriv().sharpenMipmappedTextures();
    settings.fFragColorIsInOut = this->fragColorIsInOut();

||||||| merged common ancestors
    settings.fFlipY = this->pipeline().proxy()->origin() != kTopLeft_GrSurfaceOrigin;
    settings.fSharpenTextures = this->gpu()->getContext()->contextPriv().sharpenMipmappedTextures();
=======
    settings.fFlipY = this->origin() != kTopLeft_GrSurfaceOrigin;
    settings.fSharpenTextures =
                    this->gpu()->getContext()->priv().options().fSharpenMipmappedTextures;
    settings.fFragColorIsInOut = this->fragColorIsInOut();

>>>>>>> upstream-releases
    SkSL::Program::Inputs inputs;
    SkTDArray<GrGLuint> shadersToDelete;
<<<<<<< HEAD
    bool cached = fGpu->glCaps().programBinarySupport() && nullptr != fCached.get();
||||||| merged common ancestors
    bool cached = nullptr != fCached.get();
=======
    // Calling GetProgramiv is expensive in Chromium. Assume success in release builds.
    bool checkLinked = kChromium_GrGLDriver != fGpu->ctxInfo().driver();
#ifdef SK_DEBUG
    checkLinked = true;
#endif
    bool cached = fCached.get() != nullptr;
    SkSL::String glsl;
>>>>>>> upstream-releases
    if (cached) {
        const uint8_t* bytes = fCached->bytes();
        size_t offset = 0;
        memcpy(&inputs, bytes + offset, sizeof(inputs));
        offset += sizeof(inputs);
<<<<<<< HEAD
        int binaryFormat;
        memcpy(&binaryFormat, bytes + offset, sizeof(binaryFormat));
        offset += sizeof(binaryFormat);
        GrGLClearErr(this->gpu()->glInterface());
        GR_GL_CALL_NOERRCHECK(this->gpu()->glInterface(),
                              ProgramBinary(programID, binaryFormat, (void*) (bytes + offset),
                                            fCached->size() - offset));
        if (GR_GL_GET_ERROR(this->gpu()->glInterface()) == GR_GL_NO_ERROR) {
            cached = this->checkLinkStatus(programID);
            if (cached) {
                this->addInputVars(inputs);
                this->computeCountsAndStrides(programID, primProc, false);
            }
        } else {
            cached = false;
        }
    }
    if (!cached) {
        // cache miss, compile shaders
        if (fFS.fForceHighPrecision) {
            settings.fForceHighPrecision = true;
||||||| merged common ancestors
        if (inputs.fRTHeight) {
            this->addRTHeightUniform(SKSL_RTHEIGHT_NAME);
        }
        int binaryFormat;
        memcpy(&binaryFormat, bytes + offset, sizeof(binaryFormat));
        offset += sizeof(binaryFormat);
        GL_CALL(ProgramBinary(programID, binaryFormat, (void*) (bytes + offset),
                              fCached->size() - offset));
    } else {
        // cache miss, compile shaders
        if (fFS.fForceHighPrecision) {
            settings.fForceHighPrecision = true;
=======
        if (fGpu->glCaps().programBinarySupport()) {
            // binary cache hit, just hand the binary to GL
            int binaryFormat;
            memcpy(&binaryFormat, bytes + offset, sizeof(binaryFormat));
            offset += sizeof(binaryFormat);
            GrGLClearErr(this->gpu()->glInterface());
            GR_GL_CALL_NOERRCHECK(this->gpu()->glInterface(),
                                  ProgramBinary(programID, binaryFormat, (void*) (bytes + offset),
                                                fCached->size() - offset));
            if (GR_GL_GET_ERROR(this->gpu()->glInterface()) == GR_GL_NO_ERROR) {
                if (checkLinked) {
                    cached = this->checkLinkStatus(programID);
                }
                if (cached) {
                    this->addInputVars(inputs);
                    this->computeCountsAndStrides(programID, primProc, false);
                }
            } else {
                cached = false;
            }
        } else {
            // source cache hit, we don't need to compile the SkSL->GLSL
            glsl = SkSL::String(((const char*) bytes) + offset, fCached->size() - offset);
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        SkSL::String glsl;
        std::unique_ptr<SkSL::Program> fs = GrSkSLtoGLSL(gpu()->glContext(),
                                                         GR_GL_FRAGMENT_SHADER,
                                                         fFS.fCompilerStrings.begin(),
                                                         fFS.fCompilerStringLengths.begin(),
                                                         fFS.fCompilerStrings.count(),
                                                         settings,
                                                         &glsl);
        if (!fs) {
            this->cleanupProgram(programID, shadersToDelete);
            return nullptr;
||||||| merged common ancestors
        SkSL::String glsl;
        std::unique_ptr<SkSL::Program> fs = GrSkSLtoGLSL(gpu()->glContext(),
                                                         GR_GL_FRAGMENT_SHADER,
                                                         fFS.fCompilerStrings.begin(),
                                                         fFS.fCompilerStringLengths.begin(),
                                                         fFS.fCompilerStrings.count(),
                                                         settings,
                                                         &glsl);
        inputs = fs->fInputs;
        if (inputs.fRTHeight) {
            this->addRTHeightUniform(SKSL_RTHEIGHT_NAME);
=======
    }
    if (!cached || !fGpu->glCaps().programBinarySupport()) {
        // either a cache miss, or we can't store binaries in the cache
        if (glsl == "" || true) {
            // don't have cached GLSL, need to compile SkSL->GLSL
            if (fFS.fForceHighPrecision) {
                settings.fForceHighPrecision = true;
            }
            std::unique_ptr<SkSL::Program> fs = GrSkSLtoGLSL(gpu()->glContext(),
                                                             GR_GL_FRAGMENT_SHADER,
                                                             fFS.fCompilerStrings.begin(),
                                                             fFS.fCompilerStringLengths.begin(),
                                                             fFS.fCompilerStrings.count(),
                                                             settings,
                                                             &glsl);
            if (!fs) {
                this->cleanupProgram(programID, shadersToDelete);
                return nullptr;
            }
            inputs = fs->fInputs;
        } else {
            // we've pulled GLSL and inputs from the cache, but still need to do some setup
            this->addInputVars(inputs);
            this->computeCountsAndStrides(programID, primProc, false);
>>>>>>> upstream-releases
        }
<<<<<<< HEAD
        inputs = fs->fInputs;
        this->addInputVars(inputs);
||||||| merged common ancestors
=======
        this->addInputVars(inputs);
>>>>>>> upstream-releases
        if (!this->compileAndAttachShaders(glsl.c_str(), glsl.size(), programID,
                                           GR_GL_FRAGMENT_SHADER, &shadersToDelete, settings,
                                           inputs)) {
            this->cleanupProgram(programID, shadersToDelete);
            return nullptr;
        }

        std::unique_ptr<SkSL::Program> vs = GrSkSLtoGLSL(gpu()->glContext(),
                                                         GR_GL_VERTEX_SHADER,
                                                         fVS.fCompilerStrings.begin(),
                                                         fVS.fCompilerStringLengths.begin(),
                                                         fVS.fCompilerStrings.count(),
                                                         settings,
                                                         &glsl);
        if (!vs || !this->compileAndAttachShaders(glsl.c_str(), glsl.size(), programID,
                                                  GR_GL_VERTEX_SHADER, &shadersToDelete, settings,
                                                  inputs)) {
            this->cleanupProgram(programID, shadersToDelete);
            return nullptr;
        }

        // NVPR actually requires a vertex shader to compile
        bool useNvpr = primProc.isPathRendering();
        if (!useNvpr) {
            this->computeCountsAndStrides(programID, primProc, true);
        }

        if (primProc.willUseGeoShader()) {
            std::unique_ptr<SkSL::Program> gs;
            gs = GrSkSLtoGLSL(gpu()->glContext(),
                              GR_GL_GEOMETRY_SHADER,
                              fGS.fCompilerStrings.begin(),
                              fGS.fCompilerStringLengths.begin(),
                              fGS.fCompilerStrings.count(),
                              settings,
                              &glsl);
            if (!gs || !this->compileAndAttachShaders(glsl.c_str(), glsl.size(), programID,
                                                      GR_GL_GEOMETRY_SHADER, &shadersToDelete,
                                                      settings, inputs)) {
                this->cleanupProgram(programID, shadersToDelete);
                return nullptr;
            }
        }
        this->bindProgramResourceLocations(programID);

        GL_CALL(LinkProgram(programID));
        if (checkLinked) {
            if (!this->checkLinkStatus(programID)) {
                GL_CALL(DeleteProgram(programID));
                SkDebugf("VS:\n");
                GrGLPrintShader(fGpu->glContext(),
                                GR_GL_VERTEX_SHADER,
                                fVS.fCompilerStrings.begin(),
                                fVS.fCompilerStringLengths.begin(),
                                fVS.fCompilerStrings.count(),
                                settings);
                if (primProc.willUseGeoShader()) {
                    SkDebugf("\nGS:\n");
                    GrGLPrintShader(fGpu->glContext(),
                                    GR_GL_GEOMETRY_SHADER,
                                    fGS.fCompilerStrings.begin(),
                                    fGS.fCompilerStringLengths.begin(),
                                    fGS.fCompilerStrings.count(), settings);
                }
                SkDebugf("\nFS:\n");
                GrGLPrintShader(fGpu->glContext(),
                                GR_GL_FRAGMENT_SHADER,
                                fFS.fCompilerStrings.begin(),
                                fFS.fCompilerStringLengths.begin(),
                                fFS.fCompilerStrings.count(),
                                settings);
                return nullptr;
            }
<<<<<<< HEAD
            SkDebugf("\nFS:\n");
            GrGLPrintShader(fGpu->glContext(), GR_GL_FRAGMENT_SHADER, fFS.fCompilerStrings.begin(),
                            fFS.fCompilerStringLengths.begin(), fFS.fCompilerStrings.count(),
                            settings);
            return nullptr;
||||||| merged common ancestors
            SkDebugf("\nFS:\n");
            GrGLPrintShader(fGpu->glContext(), GR_GL_FRAGMENT_SHADER, fFS.fCompilerStrings.begin(),
                            fFS.fCompilerStringLengths.begin(), fFS.fCompilerStrings.count(),
                            settings);
            SkDEBUGFAIL("");
            return nullptr;
=======
>>>>>>> upstream-releases
        }
    }
    this->resolveProgramResourceLocations(programID);

    this->cleanupShaders(shadersToDelete);
<<<<<<< HEAD
    if (!cached && this->gpu()->getContext()->contextPriv().getPersistentCache() &&
        fGpu->glCaps().programBinarySupport()) {
        GrGLsizei length = 0;
        GL_CALL(GetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length));
        if (length > 0) {
            // store shader in cache
            sk_sp<SkData> key = SkData::MakeWithoutCopy(desc()->asKey(), desc()->keyLength());
            GrGLenum binaryFormat;
            std::unique_ptr<char[]> binary(new char[length]);
            GL_CALL(GetProgramBinary(programID, length, &length, &binaryFormat, binary.get()));
            size_t dataLength = sizeof(inputs) + sizeof(binaryFormat) + length;
            std::unique_ptr<uint8_t[]> data(new uint8_t[dataLength]);
            size_t offset = 0;
            memcpy(data.get() + offset, &inputs, sizeof(inputs));
            offset += sizeof(inputs);
            memcpy(data.get() + offset, &binaryFormat, sizeof(binaryFormat));
            offset += sizeof(binaryFormat);
            memcpy(data.get() + offset, binary.get(), length);
            this->gpu()->getContext()->contextPriv().getPersistentCache()->store(
                                            *key, *SkData::MakeWithoutCopy(data.get(), dataLength));
        }
||||||| merged common ancestors
    if (!cached && this->gpu()->getContext()->getPersistentCache() &&
        fGpu->glCaps().programBinarySupport()) {
        GrGLsizei length = 0;
        GL_CALL(GetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length));
        if (length > 0) {
            // store shader in cache
            sk_sp<SkData> key = SkData::MakeWithoutCopy(desc()->asKey(), desc()->keyLength());
            GrGLenum binaryFormat;
            std::unique_ptr<char[]> binary(new char[length]);
            GL_CALL(GetProgramBinary(programID, length, &length, &binaryFormat, binary.get()));
            size_t dataLength = sizeof(inputs) + sizeof(binaryFormat) + length;
            std::unique_ptr<uint8_t[]> data(new uint8_t[dataLength]);
            size_t offset = 0;
            memcpy(data.get() + offset, &inputs, sizeof(inputs));
            offset += sizeof(inputs);
            memcpy(data.get() + offset, &binaryFormat, sizeof(binaryFormat));
            offset += sizeof(binaryFormat);
            memcpy(data.get() + offset, binary.get(), length);
            this->gpu()->getContext()->getPersistentCache()->store(*key,
                                                  *SkData::MakeWithoutCopy(data.get(), dataLength));
        }
=======
    if (!cached) {
        this->storeShaderInCache(inputs, programID, glsl);
>>>>>>> upstream-releases
    }
    return this->createProgram(programID);
}

void GrGLProgramBuilder::bindProgramResourceLocations(GrGLuint programID) {
    fUniformHandler.bindUniformLocations(programID, fGpu->glCaps());

    const GrGLCaps& caps = this->gpu()->glCaps();
    if (fFS.hasCustomColorOutput() && caps.bindFragDataLocationSupport()) {
        GL_CALL(BindFragDataLocation(programID, 0,
                                     GrGLSLFragmentShaderBuilder::DeclaredColorOutputName()));
    }
    if (fFS.hasSecondaryOutput() && caps.shaderCaps()->mustDeclareFragmentShaderOutput()) {
        GL_CALL(BindFragDataLocationIndexed(programID, 0, 1,
                                  GrGLSLFragmentShaderBuilder::DeclaredSecondaryColorOutputName()));
    }

    // handle NVPR separable varyings
    if (!fGpu->glCaps().shaderCaps()->pathRenderingSupport() ||
        !fGpu->glPathRendering()->shouldBindFragmentInputs()) {
        return;
    }
    int count = fVaryingHandler.fPathProcVaryingInfos.count();
    for (int i = 0; i < count; ++i) {
        GL_CALL(BindFragmentInputLocation(programID, i,
                                       fVaryingHandler.fPathProcVaryingInfos[i].fVariable.c_str()));
        fVaryingHandler.fPathProcVaryingInfos[i].fLocation = i;
    }
}

bool GrGLProgramBuilder::checkLinkStatus(GrGLuint programID) {
    GrGLint linked = GR_GL_INIT_ZERO;
    GL_CALL(GetProgramiv(programID, GR_GL_LINK_STATUS, &linked));
    if (!linked) {
        SkDebugf("Program linking failed.\n");
        GrGLint infoLen = GR_GL_INIT_ZERO;
        GL_CALL(GetProgramiv(programID, GR_GL_INFO_LOG_LENGTH, &infoLen));
        SkAutoMalloc log(sizeof(char)*(infoLen+1));  // outside if for debugger
        if (infoLen > 0) {
            // retrieve length even though we don't need it to workaround
            // bug in chrome cmd buffer param validation.
            GrGLsizei length = GR_GL_INIT_ZERO;
            GL_CALL(GetProgramInfoLog(programID,
                                      infoLen+1,
                                      &length,
                                      (char*)log.get()));
            SkDebugf("%s", (char*)log.get());
        }
    }
    return SkToBool(linked);
}

void GrGLProgramBuilder::resolveProgramResourceLocations(GrGLuint programID) {
    fUniformHandler.getUniformLocations(programID, fGpu->glCaps());

    // handle NVPR separable varyings
    if (!fGpu->glCaps().shaderCaps()->pathRenderingSupport() ||
        fGpu->glPathRendering()->shouldBindFragmentInputs()) {
        return;
    }
    int count = fVaryingHandler.fPathProcVaryingInfos.count();
    for (int i = 0; i < count; ++i) {
        GrGLint location;
        GL_CALL_RET(location, GetProgramResourceLocation(
                                       programID,
                                       GR_GL_FRAGMENT_INPUT,
                                       fVaryingHandler.fPathProcVaryingInfos[i].fVariable.c_str()));
        fVaryingHandler.fPathProcVaryingInfos[i].fLocation = location;
    }
}

void GrGLProgramBuilder::cleanupProgram(GrGLuint programID, const SkTDArray<GrGLuint>& shaderIDs) {
    GL_CALL(DeleteProgram(programID));
    this->cleanupShaders(shaderIDs);
}
void GrGLProgramBuilder::cleanupShaders(const SkTDArray<GrGLuint>& shaderIDs) {
    for (int i = 0; i < shaderIDs.count(); ++i) {
        GL_CALL(DeleteShader(shaderIDs[i]));
    }
}

GrGLProgram* GrGLProgramBuilder::createProgram(GrGLuint programID) {
    return new GrGLProgram(fGpu,
                           fUniformHandles,
                           programID,
                           fUniformHandler.fUniforms,
                           fUniformHandler.fSamplers,
                           fVaryingHandler.fPathProcVaryingInfos,
                           std::move(fGeometryProcessor),
                           std::move(fXferProcessor),
                           std::move(fFragmentProcessors),
                           fFragmentProcessorCnt,
                           std::move(fAttributes),
                           fVertexAttributeCnt,
                           fInstanceAttributeCnt,
                           fVertexStride,
                           fInstanceStride);
}
