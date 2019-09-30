/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrOnFlushResourceProvider.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrDrawingManager.h"
#include "GrProxyProvider.h"
<<<<<<< HEAD
#include "GrRenderTargetContext.h"
||||||| merged common ancestors
=======
#include "GrRecordingContext.h"
#include "GrRecordingContextPriv.h"
#include "GrRenderTargetContext.h"
>>>>>>> upstream-releases
#include "GrSurfaceProxy.h"

sk_sp<GrRenderTargetContext> GrOnFlushResourceProvider::makeRenderTargetContext(
<<<<<<< HEAD
        const GrSurfaceDesc& desc,
        GrSurfaceOrigin origin,
        sk_sp<SkColorSpace> colorSpace,
        const SkSurfaceProps* props) {
    GrSurfaceDesc tmpDesc = desc;
    tmpDesc.fFlags |= kRenderTarget_GrSurfaceFlag;

    auto proxyProvider = fDrawingMgr->getContext()->contextPriv().proxyProvider();
    auto resourceProvider = fDrawingMgr->getContext()->contextPriv().resourceProvider();

    // Because this is being allocated at the start of a flush we must ensure the proxy
    // will, when instantiated, have no pending IO.
    // TODO: fold the kNoPendingIO_Flag into GrSurfaceFlags?
    sk_sp<GrSurfaceProxy> proxy =
            proxyProvider->createProxy(tmpDesc, origin, SkBackingFit::kExact, SkBudgeted::kYes,
                                       GrInternalSurfaceFlags::kNoPendingIO);
    if (!proxy || !proxy->asRenderTargetProxy()) {
||||||| merged common ancestors
                                                        const GrSurfaceDesc& desc,
                                                        sk_sp<SkColorSpace> colorSpace,
                                                        const SkSurfaceProps* props) {
    GrSurfaceDesc tmpDesc = desc;
    tmpDesc.fFlags |= kRenderTarget_GrSurfaceFlag;

    auto proxyProvider = fDrawingMgr->getContext()->contextPriv().proxyProvider();
    auto resourceProvider = fDrawingMgr->getContext()->contextPriv().resourceProvider();

    // Because this is being allocated at the start of a flush we must ensure the proxy
    // will, when instantiated, have no pending IO.
    // TODO: fold the kNoPendingIO_Flag into GrSurfaceFlags?
    sk_sp<GrSurfaceProxy> proxy = proxyProvider->createProxy(tmpDesc, SkBackingFit::kExact,
                                                             SkBudgeted::kYes,
                                                             GrResourceProvider::kNoPendingIO_Flag);
    if (!proxy->asRenderTargetProxy()) {
=======
                                                        sk_sp<GrSurfaceProxy> proxy,
                                                        sk_sp<SkColorSpace> colorSpace,
                                                        const SkSurfaceProps* props) {
    // Since this is at flush time and these won't be allocated for us by the GrResourceAllocator
    // we have to manually ensure it is allocated here. The proxy had best have been created
    // with the kNoPendingIO flag!
    if (!this->instatiateProxy(proxy.get())) {
>>>>>>> upstream-releases
        return nullptr;
    }

    sk_sp<GrRenderTargetContext> renderTargetContext(
        fDrawingMgr->makeRenderTargetContext(std::move(proxy),
                                             std::move(colorSpace),
                                             props, false));

    if (!renderTargetContext) {
        return nullptr;
    }

    renderTargetContext->discard();

    return renderTargetContext;
}

<<<<<<< HEAD
sk_sp<GrRenderTargetContext> GrOnFlushResourceProvider::makeRenderTargetContext(
                                                        sk_sp<GrSurfaceProxy> proxy,
                                                        sk_sp<SkColorSpace> colorSpace,
                                                        const SkSurfaceProps* props) {
    // Since this is at flush time and these won't be allocated for us by the GrResourceAllocator
    // we have to manually ensure it is allocated here. The proxy had best have been created
    // with the kNoPendingIO flag!
    if (!this->instatiateProxy(proxy.get())) {
        return nullptr;
    }

    sk_sp<GrRenderTargetContext> renderTargetContext(
        fDrawingMgr->makeRenderTargetContext(std::move(proxy),
                                             std::move(colorSpace),
                                             props, false));

    if (!renderTargetContext) {
        return nullptr;
    }

    renderTargetContext->discard();
||||||| merged common ancestors
sk_sp<GrRenderTargetContext> GrOnFlushResourceProvider::makeRenderTargetContext(
                                                        sk_sp<GrSurfaceProxy> proxy,
                                                        sk_sp<SkColorSpace> colorSpace,
                                                        const SkSurfaceProps* props) {
    sk_sp<GrRenderTargetContext> renderTargetContext(
        fDrawingMgr->makeRenderTargetContext(std::move(proxy),
                                             std::move(colorSpace),
                                             props, false));

    if (!renderTargetContext) {
        return nullptr;
    }

    auto resourceProvider = fDrawingMgr->getContext()->contextPriv().resourceProvider();

    // Since this is at flush time and these won't be allocated for us by the GrResourceAllocator
    // we have to manually ensure it is allocated here. The proxy had best have been created
    // with the kNoPendingIO flag!
    if (!renderTargetContext->asSurfaceProxy()->instantiate(resourceProvider)) {
        return nullptr;
    }

    renderTargetContext->discard();
=======
bool GrOnFlushResourceProvider::assignUniqueKeyToProxy(const GrUniqueKey& key,
                                                       GrTextureProxy* proxy) {
    auto proxyProvider = fDrawingMgr->getContext()->priv().proxyProvider();
    return proxyProvider->assignUniqueKeyToProxy(key, proxy);
}

void GrOnFlushResourceProvider::removeUniqueKeyFromProxy(GrTextureProxy* proxy) {
    auto proxyProvider = fDrawingMgr->getContext()->priv().proxyProvider();
    proxyProvider->removeUniqueKeyFromProxy(proxy);
}

void GrOnFlushResourceProvider::processInvalidUniqueKey(const GrUniqueKey& key) {
    auto proxyProvider = fDrawingMgr->getContext()->priv().proxyProvider();
    proxyProvider->processInvalidUniqueKey(key, nullptr,
                                           GrProxyProvider::InvalidateGPUResource::kYes);
}
>>>>>>> upstream-releases

sk_sp<GrTextureProxy> GrOnFlushResourceProvider::findOrCreateProxyByUniqueKey(
        const GrUniqueKey& key, GrSurfaceOrigin origin) {
    auto proxyProvider = fDrawingMgr->getContext()->priv().proxyProvider();
    return proxyProvider->findOrCreateProxyByUniqueKey(key, origin);
}

bool GrOnFlushResourceProvider::assignUniqueKeyToProxy(const GrUniqueKey& key,
                                                       GrTextureProxy* proxy) {
    auto proxyProvider = fDrawingMgr->getContext()->contextPriv().proxyProvider();
    return proxyProvider->assignUniqueKeyToProxy(key, proxy);
}

void GrOnFlushResourceProvider::removeUniqueKeyFromProxy(const GrUniqueKey& key,
                                                         GrTextureProxy* proxy) {
    auto proxyProvider = fDrawingMgr->getContext()->contextPriv().proxyProvider();
    proxyProvider->removeUniqueKeyFromProxy(key, proxy);
}

sk_sp<GrTextureProxy> GrOnFlushResourceProvider::findOrCreateProxyByUniqueKey(
        const GrUniqueKey& key, GrSurfaceOrigin origin) {
    auto proxyProvider = fDrawingMgr->getContext()->contextPriv().proxyProvider();
    return proxyProvider->findOrCreateProxyByUniqueKey(key, origin);
}

bool GrOnFlushResourceProvider::instatiateProxy(GrSurfaceProxy* proxy) {
    // TODO: this class should probably just get a GrDirectContext
    auto direct = fDrawingMgr->getContext()->priv().asDirectContext();
    if (!direct) {
        return false;
    }

    auto resourceProvider = direct->priv().resourceProvider();

    if (GrSurfaceProxy::LazyState::kNot != proxy->lazyInstantiationState()) {
<<<<<<< HEAD
        // DDL TODO: Decide if we ever plan to have these proxies use the GrUninstantiateTracker
        // to support unistantiating them at the end of a flush.
||||||| merged common ancestors
=======
        // DDL TODO: Decide if we ever plan to have these proxies use the GrDeinstantiateTracker
        // to support unistantiating them at the end of a flush.
>>>>>>> upstream-releases
        return proxy->priv().doLazyInstantiation(resourceProvider);
    }

    return proxy->instantiate(resourceProvider);
}

<<<<<<< HEAD
sk_sp<GrBuffer> GrOnFlushResourceProvider::makeBuffer(GrBufferType intendedType, size_t size,
                                                      const void* data) {
    auto resourceProvider = fDrawingMgr->getContext()->contextPriv().resourceProvider();
    return sk_sp<GrBuffer>(resourceProvider->createBuffer(size, intendedType,
                                                          kDynamic_GrAccessPattern,
                                                          GrResourceProvider::Flags::kNoPendingIO,
                                                          data));
||||||| merged common ancestors
sk_sp<GrBuffer> GrOnFlushResourceProvider::makeBuffer(GrBufferType intendedType, size_t size,
                                                      const void* data) {
    auto resourceProvider = fDrawingMgr->getContext()->contextPriv().resourceProvider();
    return sk_sp<GrBuffer>(resourceProvider->createBuffer(size, intendedType,
                                                          kDynamic_GrAccessPattern,
                                                          GrResourceProvider::kNoPendingIO_Flag,
                                                          data));
=======
sk_sp<GrGpuBuffer> GrOnFlushResourceProvider::makeBuffer(GrGpuBufferType intendedType, size_t size,
                                                         const void* data) {
    // TODO: this class should probably just get a GrDirectContext
    auto direct = fDrawingMgr->getContext()->priv().asDirectContext();
    if (!direct) {
        return nullptr;
    }

    auto resourceProvider = direct->priv().resourceProvider();

    return sk_sp<GrGpuBuffer>(
            resourceProvider->createBuffer(size, intendedType, kDynamic_GrAccessPattern, data));
>>>>>>> upstream-releases
}

sk_sp<const GrGpuBuffer> GrOnFlushResourceProvider::findOrMakeStaticBuffer(
        GrGpuBufferType intendedType, size_t size, const void* data, const GrUniqueKey& key) {
    // TODO: class should probably just get a GrDirectContext
    auto direct = fDrawingMgr->getContext()->priv().asDirectContext();
    if (!direct) {
        return nullptr;
    }

    auto resourceProvider = direct->priv().resourceProvider();

    sk_sp<const GrGpuBuffer> buffer =
            resourceProvider->findOrMakeStaticBuffer(intendedType, size, data, key);
    // Static buffers should never have pending IO.
    SkASSERT(!buffer || !buffer->resourcePriv().hasPendingIO_debugOnly());
    return buffer;
}

<<<<<<< HEAD
uint32_t GrOnFlushResourceProvider::contextUniqueID() const {
    return fDrawingMgr->getContext()->uniqueID();
}

||||||| merged common ancestors
=======
uint32_t GrOnFlushResourceProvider::contextID() const {
    return fDrawingMgr->getContext()->priv().contextID();
}

>>>>>>> upstream-releases
const GrCaps* GrOnFlushResourceProvider::caps() const {
<<<<<<< HEAD
    return fDrawingMgr->getContext()->contextPriv().caps();
||||||| merged common ancestors
    return fDrawingMgr->getContext()->caps();
=======
    return fDrawingMgr->getContext()->priv().caps();
>>>>>>> upstream-releases
}
