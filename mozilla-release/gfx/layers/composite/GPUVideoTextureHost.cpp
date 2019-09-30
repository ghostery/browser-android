/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "GPUVideoTextureHost.h"
#include "mozilla/RemoteDecoderManagerParent.h"
#include "ImageContainer.h"
#include "mozilla/layers/VideoBridgeParent.h"

namespace mozilla {
namespace layers {

<<<<<<< HEAD
GPUVideoTextureHost::GPUVideoTextureHost(TextureFlags aFlags,
                                         TextureHost* aWrappedTextureHost)
    : TextureHost(aFlags), mWrappedTextureHost(aWrappedTextureHost) {
||||||| merged common ancestors
GPUVideoTextureHost::GPUVideoTextureHost(TextureFlags aFlags,
                                         TextureHost* aWrappedTextureHost)
  : TextureHost(aFlags)
  , mWrappedTextureHost(aWrappedTextureHost)
{
=======
GPUVideoTextureHost::GPUVideoTextureHost(
    TextureFlags aFlags, const SurfaceDescriptorGPUVideo& aDescriptor)
    : TextureHost(aFlags), mDescriptor(aDescriptor) {
>>>>>>> upstream-releases
  MOZ_COUNT_CTOR(GPUVideoTextureHost);
}

GPUVideoTextureHost::~GPUVideoTextureHost() {
  MOZ_COUNT_DTOR(GPUVideoTextureHost);
}

<<<<<<< HEAD
GPUVideoTextureHost* GPUVideoTextureHost::CreateFromDescriptor(
    TextureFlags aFlags, const SurfaceDescriptorGPUVideo& aDescriptor) {
  TextureHost* wrappedTextureHost =
      VideoBridgeParent::GetSingleton()->LookupTexture(aDescriptor.handle());
  if (!wrappedTextureHost) {
    return nullptr;
||||||| merged common ancestors
GPUVideoTextureHost*
GPUVideoTextureHost::CreateFromDescriptor(
  TextureFlags aFlags,
  const SurfaceDescriptorGPUVideo& aDescriptor)
{
  TextureHost* wrappedTextureHost =
    VideoBridgeParent::GetSingleton()->LookupTexture(aDescriptor.handle());
  if (!wrappedTextureHost) {
    return nullptr;
=======
GPUVideoTextureHost* GPUVideoTextureHost::CreateFromDescriptor(
    TextureFlags aFlags, const SurfaceDescriptorGPUVideo& aDescriptor) {
  return new GPUVideoTextureHost(aFlags, aDescriptor);
}

TextureHost* GPUVideoTextureHost::EnsureWrappedTextureHost() {
  if (mWrappedTextureHost) {
    return mWrappedTextureHost;
>>>>>>> upstream-releases
  }

  // In the future when the RDD process has a PVideoBridge connection,
  // then there might be two VideoBridgeParents (one within the GPU process,
  // one from RDD). We'll need to flag which one to use to lookup our
  // descriptor, or just try both.
  mWrappedTextureHost =
      VideoBridgeParent::GetSingleton()->LookupTexture(mDescriptor.handle());
  return mWrappedTextureHost;
}

<<<<<<< HEAD
bool GPUVideoTextureHost::Lock() {
  if (!mWrappedTextureHost) {
||||||| merged common ancestors
bool
GPUVideoTextureHost::Lock()
{
  if (!mWrappedTextureHost) {
=======
bool GPUVideoTextureHost::Lock() {
  if (!EnsureWrappedTextureHost()) {
>>>>>>> upstream-releases
    return false;
  }
  return EnsureWrappedTextureHost()->Lock();
}

<<<<<<< HEAD
void GPUVideoTextureHost::Unlock() {
  if (!mWrappedTextureHost) {
||||||| merged common ancestors
void
GPUVideoTextureHost::Unlock()
{
  if (!mWrappedTextureHost) {
=======
void GPUVideoTextureHost::Unlock() {
  if (!EnsureWrappedTextureHost()) {
>>>>>>> upstream-releases
    return;
  }
  EnsureWrappedTextureHost()->Unlock();
}

<<<<<<< HEAD
bool GPUVideoTextureHost::BindTextureSource(
    CompositableTextureSourceRef& aTexture) {
  if (!mWrappedTextureHost) {
||||||| merged common ancestors
bool
GPUVideoTextureHost::BindTextureSource(CompositableTextureSourceRef& aTexture)
{
  if (!mWrappedTextureHost) {
=======
bool GPUVideoTextureHost::BindTextureSource(
    CompositableTextureSourceRef& aTexture) {
  if (!EnsureWrappedTextureHost()) {
>>>>>>> upstream-releases
    return false;
  }
  return EnsureWrappedTextureHost()->BindTextureSource(aTexture);
}

<<<<<<< HEAD
bool GPUVideoTextureHost::AcquireTextureSource(
    CompositableTextureSourceRef& aTexture) {
  if (!mWrappedTextureHost) {
||||||| merged common ancestors
bool
GPUVideoTextureHost::AcquireTextureSource(CompositableTextureSourceRef& aTexture)
{
  if (!mWrappedTextureHost) {
=======
bool GPUVideoTextureHost::AcquireTextureSource(
    CompositableTextureSourceRef& aTexture) {
  if (!EnsureWrappedTextureHost()) {
>>>>>>> upstream-releases
    return false;
  }
  return EnsureWrappedTextureHost()->AcquireTextureSource(aTexture);
}

<<<<<<< HEAD
void GPUVideoTextureHost::SetTextureSourceProvider(
    TextureSourceProvider* aProvider) {
  if (mWrappedTextureHost) {
    mWrappedTextureHost->SetTextureSourceProvider(aProvider);
||||||| merged common ancestors
void
GPUVideoTextureHost::SetTextureSourceProvider(TextureSourceProvider* aProvider)
{
  if (mWrappedTextureHost) {
    mWrappedTextureHost->SetTextureSourceProvider(aProvider);
=======
void GPUVideoTextureHost::SetTextureSourceProvider(
    TextureSourceProvider* aProvider) {
  if (EnsureWrappedTextureHost()) {
    EnsureWrappedTextureHost()->SetTextureSourceProvider(aProvider);
>>>>>>> upstream-releases
  }
}

<<<<<<< HEAD
YUVColorSpace GPUVideoTextureHost::GetYUVColorSpace() const {
||||||| merged common ancestors
YUVColorSpace
GPUVideoTextureHost::GetYUVColorSpace() const
{
=======
gfx::YUVColorSpace GPUVideoTextureHost::GetYUVColorSpace() const {
>>>>>>> upstream-releases
  if (mWrappedTextureHost) {
    return mWrappedTextureHost->GetYUVColorSpace();
  }
  return gfx::YUVColorSpace::UNKNOWN;
}

gfx::IntSize GPUVideoTextureHost::GetSize() const {
  if (!mWrappedTextureHost) {
    return gfx::IntSize();
  }
  return mWrappedTextureHost->GetSize();
}

gfx::SurfaceFormat GPUVideoTextureHost::GetFormat() const {
  if (!mWrappedTextureHost) {
    return gfx::SurfaceFormat::UNKNOWN;
  }
  return mWrappedTextureHost->GetFormat();
}

bool GPUVideoTextureHost::HasIntermediateBuffer() const {
  MOZ_ASSERT(mWrappedTextureHost);
  if (!mWrappedTextureHost) {
    return false;
  }

  return mWrappedTextureHost->HasIntermediateBuffer();
}

<<<<<<< HEAD
void GPUVideoTextureHost::CreateRenderTexture(
    const wr::ExternalImageId& aExternalImageId) {
  MOZ_ASSERT(mWrappedTextureHost);
||||||| merged common ancestors
void
GPUVideoTextureHost::CreateRenderTexture(const wr::ExternalImageId& aExternalImageId)
{
  MOZ_ASSERT(mWrappedTextureHost);
=======
void GPUVideoTextureHost::CreateRenderTexture(
    const wr::ExternalImageId& aExternalImageId) {
  MOZ_ASSERT(EnsureWrappedTextureHost());
  if (!EnsureWrappedTextureHost()) {
    return;
  }
>>>>>>> upstream-releases

  EnsureWrappedTextureHost()->CreateRenderTexture(aExternalImageId);
}

<<<<<<< HEAD
uint32_t GPUVideoTextureHost::NumSubTextures() const {
  MOZ_ASSERT(mWrappedTextureHost);
  return mWrappedTextureHost->NumSubTextures();
||||||| merged common ancestors
uint32_t
GPUVideoTextureHost::NumSubTextures() const
{
  MOZ_ASSERT(mWrappedTextureHost);
  return mWrappedTextureHost->NumSubTextures();
=======
uint32_t GPUVideoTextureHost::NumSubTextures() {
  MOZ_ASSERT(EnsureWrappedTextureHost());
  if (!EnsureWrappedTextureHost()) {
    return 0;
  }
  return EnsureWrappedTextureHost()->NumSubTextures();
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GPUVideoTextureHost::PushResourceUpdates(
    wr::TransactionBuilder& aResources, ResourceUpdateOp aOp,
    const Range<wr::ImageKey>& aImageKeys, const wr::ExternalImageId& aExtID) {
  MOZ_ASSERT(mWrappedTextureHost);
  mWrappedTextureHost->PushResourceUpdates(aResources, aOp, aImageKeys, aExtID);
||||||| merged common ancestors
void
GPUVideoTextureHost::PushResourceUpdates(wr::TransactionBuilder& aResources,
                                         ResourceUpdateOp aOp,
                                         const Range<wr::ImageKey>& aImageKeys,
                                         const wr::ExternalImageId& aExtID)
{
  MOZ_ASSERT(mWrappedTextureHost);
  mWrappedTextureHost->PushResourceUpdates(aResources, aOp, aImageKeys, aExtID);
=======
void GPUVideoTextureHost::PushResourceUpdates(
    wr::TransactionBuilder& aResources, ResourceUpdateOp aOp,
    const Range<wr::ImageKey>& aImageKeys, const wr::ExternalImageId& aExtID) {
  MOZ_ASSERT(EnsureWrappedTextureHost());
  if (!EnsureWrappedTextureHost()) {
    return;
  }
  EnsureWrappedTextureHost()->PushResourceUpdates(aResources, aOp, aImageKeys,
                                                  aExtID);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
void GPUVideoTextureHost::PushDisplayItems(
    wr::DisplayListBuilder& aBuilder, const wr::LayoutRect& aBounds,
    const wr::LayoutRect& aClip, wr::ImageRendering aFilter,
    const Range<wr::ImageKey>& aImageKeys) {
  MOZ_ASSERT(mWrappedTextureHost);
||||||| merged common ancestors
void
GPUVideoTextureHost::PushDisplayItems(wr::DisplayListBuilder& aBuilder,
                                      const wr::LayoutRect& aBounds,
                                      const wr::LayoutRect& aClip,
                                      wr::ImageRendering aFilter,
                                      const Range<wr::ImageKey>& aImageKeys)
{
  MOZ_ASSERT(mWrappedTextureHost);
=======
void GPUVideoTextureHost::PushDisplayItems(
    wr::DisplayListBuilder& aBuilder, const wr::LayoutRect& aBounds,
    const wr::LayoutRect& aClip, wr::ImageRendering aFilter,
    const Range<wr::ImageKey>& aImageKeys) {
  MOZ_ASSERT(EnsureWrappedTextureHost());
>>>>>>> upstream-releases
  MOZ_ASSERT(aImageKeys.length() > 0);
  if (!EnsureWrappedTextureHost()) {
    return;
  }

<<<<<<< HEAD
  mWrappedTextureHost->PushDisplayItems(aBuilder, aBounds, aClip, aFilter,
                                        aImageKeys);
||||||| merged common ancestors
  mWrappedTextureHost->PushDisplayItems(aBuilder,
                                         aBounds,
                                         aClip,
                                         aFilter,
                                         aImageKeys);
=======
  EnsureWrappedTextureHost()->PushDisplayItems(aBuilder, aBounds, aClip,
                                               aFilter, aImageKeys);
>>>>>>> upstream-releases
}

<<<<<<< HEAD
bool GPUVideoTextureHost::SupportsWrNativeTexture() {
  MOZ_ASSERT(mWrappedTextureHost);
  return mWrappedTextureHost->SupportsWrNativeTexture();
||||||| merged common ancestors
bool
GPUVideoTextureHost::SupportsWrNativeTexture()
{
  MOZ_ASSERT(mWrappedTextureHost);
  return mWrappedTextureHost->SupportsWrNativeTexture();
=======
bool GPUVideoTextureHost::SupportsWrNativeTexture() {
  MOZ_ASSERT(EnsureWrappedTextureHost());
  if (!EnsureWrappedTextureHost()) {
    return false;
  }
  return EnsureWrappedTextureHost()->SupportsWrNativeTexture();
>>>>>>> upstream-releases
}

}  // namespace layers
}  // namespace mozilla
