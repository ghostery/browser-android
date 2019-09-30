/* -*- Mode: C++; tab-width: 20; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GFX_WINDOWS_PLATFORM_H
#define GFX_WINDOWS_PLATFORM_H

/**
 * XXX to get CAIRO_HAS_DWRITE_FONT
 * and cairo_win32_scaled_font_select_font
 */
#include "cairo-win32.h"

#include "gfxCrashReporterUtils.h"
#include "gfxFontUtils.h"
#include "gfxWindowsSurface.h"
#include "gfxFont.h"
#include "gfxDWriteFonts.h"
#include "gfxPlatform.h"
#include "gfxTelemetry.h"
#include "gfxTypes.h"
#include "mozilla/Attributes.h"
#include "mozilla/Atomics.h"
#include "nsTArray.h"
#include "nsDataHashtable.h"

#include "mozilla/Mutex.h"
#include "mozilla/RefPtr.h"

#include <windows.h>
#include <objbase.h>

#include <dxgi.h>

// This header is available in the June 2010 SDK and in the Win8 SDK
#include <d3dcommon.h>
// Win 8.0 SDK types we'll need when building using older sdks.
<<<<<<< HEAD
#if !defined(D3D_FEATURE_LEVEL_11_1)  // defined in the 8.0 SDK only
#define D3D_FEATURE_LEVEL_11_1 static_cast<D3D_FEATURE_LEVEL>(0xb100)
#define D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION 2048
#define D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION 4096
||||||| merged common ancestors
#if !defined(D3D_FEATURE_LEVEL_11_1) // defined in the 8.0 SDK only
#define D3D_FEATURE_LEVEL_11_1 static_cast<D3D_FEATURE_LEVEL>(0xb100)
#define D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION 2048
#define D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION 4096
=======
#if !defined(D3D_FEATURE_LEVEL_11_1)  // defined in the 8.0 SDK only
#  define D3D_FEATURE_LEVEL_11_1 static_cast<D3D_FEATURE_LEVEL>(0xb100)
#  define D3D_FL9_1_REQ_TEXTURE2D_U_OR_V_DIMENSION 2048
#  define D3D_FL9_3_REQ_TEXTURE2D_U_OR_V_DIMENSION 4096
>>>>>>> upstream-releases
#endif

namespace mozilla {
namespace gfx {
class DrawTarget;
class FeatureState;
class DeviceManagerDx;
}  // namespace gfx
namespace layers {
class ReadbackManagerD3D11;
}
}  // namespace mozilla
struct IDirect3DDevice9;
struct ID3D11Device;
struct IDXGIAdapter1;

/**
 * Utility to get a Windows HDC from a Moz2D DrawTarget.  If the DrawTarget is
 * not backed by a HDC this will get the HDC for the screen device context
 * instead.
 */
class MOZ_STACK_CLASS DCForMetrics final {
 public:
  explicit DCForMetrics();

  ~DCForMetrics() { ReleaseDC(nullptr, mDC); }

  operator HDC() { return mDC; }

 private:
  HDC mDC;
};

// ClearType parameters set by running ClearType tuner
struct ClearTypeParameterInfo {
  ClearTypeParameterInfo()
      : gamma(-1),
        pixelStructure(-1),
        clearTypeLevel(-1),
        enhancedContrast(-1) {}

  nsString displayName;  // typically just 'DISPLAY1'
  int32_t gamma;
  int32_t pixelStructure;
  int32_t clearTypeLevel;
  int32_t enhancedContrast;
};

class gfxWindowsPlatform : public gfxPlatform {
  friend class mozilla::gfx::DeviceManagerDx;

<<<<<<< HEAD
 public:
  enum TextRenderingMode {
    TEXT_RENDERING_NO_CLEARTYPE,
    TEXT_RENDERING_NORMAL,
    TEXT_RENDERING_GDI_CLASSIC,
    TEXT_RENDERING_COUNT
  };

  gfxWindowsPlatform();
  virtual ~gfxWindowsPlatform();
  static gfxWindowsPlatform* GetPlatform() {
    return (gfxWindowsPlatform*)gfxPlatform::GetPlatform();
  }
||||||| merged common ancestors
public:
    enum TextRenderingMode {
        TEXT_RENDERING_NO_CLEARTYPE,
        TEXT_RENDERING_NORMAL,
        TEXT_RENDERING_GDI_CLASSIC,
        TEXT_RENDERING_COUNT
    };
=======
 public:
  enum TextRenderingMode {
    TEXT_RENDERING_NO_CLEARTYPE,
    TEXT_RENDERING_NORMAL,
    TEXT_RENDERING_GDI_CLASSIC,
    TEXT_RENDERING_COUNT
  };

  gfxWindowsPlatform();
  virtual ~gfxWindowsPlatform();
  static gfxWindowsPlatform* GetPlatform() {
    return (gfxWindowsPlatform*)gfxPlatform::GetPlatform();
  }

  void EnsureDevicesInitialized() override;
  bool DevicesInitialized() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual gfxPlatformFontList* CreatePlatformFontList() override;
||||||| merged common ancestors
    gfxWindowsPlatform();
    virtual ~gfxWindowsPlatform();
    static gfxWindowsPlatform *GetPlatform() {
        return (gfxWindowsPlatform*) gfxPlatform::GetPlatform();
    }
=======
  gfxPlatformFontList* CreatePlatformFontList() override;
>>>>>>> upstream-releases

  virtual already_AddRefed<gfxASurface> CreateOffscreenSurface(
      const IntSize& aSize, gfxImageFormat aFormat) override;

  enum RenderMode {
    /* Use GDI and windows surfaces */
    RENDER_GDI = 0,

    /* Use 32bpp image surfaces and call StretchDIBits */
    RENDER_IMAGE_STRETCH32,

    /* Use 32bpp image surfaces, and do 32->24 conversion before calling
       StretchDIBits */
    RENDER_IMAGE_STRETCH24,

    /* Use Direct2D rendering */
    RENDER_DIRECT2D,

    /* max */
    RENDER_MODE_MAX
  };

  bool IsDirect2DBackend();

  /**
   * Updates render mode with relation to the current preferences and
   * available devices.
   */
  void UpdateRenderMode();

  /**
   * Verifies a D2D device is present and working, will attempt to create one
   * it is non-functional or non-existant.
   *
   * \param aAttemptForce Attempt to force D2D cairo device creation by using
   * cairo device creation routines.
   */
  void VerifyD2DDevice(bool aAttemptForce);

<<<<<<< HEAD
  virtual void GetCommonFallbackFonts(
      uint32_t aCh, uint32_t aNextCh, Script aRunScript,
      nsTArray<const char*>& aFontList) override;
||||||| merged common ancestors
    /**
     * Verifies a D2D device is present and working, will attempt to create one
     * it is non-functional or non-existant.
     *
     * \param aAttemptForce Attempt to force D2D cairo device creation by using
     * cairo device creation routines.
     */
    void VerifyD2DDevice(bool aAttemptForce);
=======
  void GetCommonFallbackFonts(uint32_t aCh, uint32_t aNextCh, Script aRunScript,
                              nsTArray<const char*>& aFontList) override;
>>>>>>> upstream-releases

  gfxFontGroup* CreateFontGroup(const mozilla::FontFamilyList& aFontFamilyList,
                                const gfxFontStyle* aStyle,
                                gfxTextPerfMetrics* aTextPerf,
                                gfxUserFontSet* aUserFontSet,
                                gfxFloat aDevToCssSize) override;

<<<<<<< HEAD
  virtual bool CanUseHardwareVideoDecoding() override;
||||||| merged common ancestors
    gfxFontGroup*
    CreateFontGroup(const mozilla::FontFamilyList& aFontFamilyList,
                    const gfxFontStyle *aStyle,
                    gfxTextPerfMetrics* aTextPerf,
                    gfxUserFontSet *aUserFontSet,
                    gfxFloat aDevToCssSize) override;
=======
  bool CanUseHardwareVideoDecoding() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void CompositorUpdated() override;
||||||| merged common ancestors
    virtual bool CanUseHardwareVideoDecoding() override;
=======
  void CompositorUpdated() override;
>>>>>>> upstream-releases

  bool DidRenderingDeviceReset(
      DeviceResetReason* aResetReason = nullptr) override;
  void SchedulePaintIfDeviceReset() override;
  void CheckForContentOnlyDeviceReset();

<<<<<<< HEAD
  bool AllowOpenGLCanvas() override;
||||||| merged common ancestors
    bool DidRenderingDeviceReset(DeviceResetReason* aResetReason = nullptr) override;
    void SchedulePaintIfDeviceReset() override;
    void CheckForContentOnlyDeviceReset();
=======
  mozilla::gfx::BackendType GetContentBackendFor(
      mozilla::layers::LayersBackend aLayers) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::gfx::BackendType GetContentBackendFor(
      mozilla::layers::LayersBackend aLayers) override;
||||||| merged common ancestors
    bool AllowOpenGLCanvas() override;
=======
  mozilla::gfx::BackendType GetPreferredCanvasBackend() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::gfx::BackendType GetPreferredCanvasBackend() override;
||||||| merged common ancestors
    mozilla::gfx::BackendType GetContentBackendFor(mozilla::layers::LayersBackend aLayers) override;
=======
  static void GetDLLVersion(char16ptr_t aDLLPath, nsAString& aVersion);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void GetDLLVersion(char16ptr_t aDLLPath, nsAString& aVersion);
||||||| merged common ancestors
    mozilla::gfx::BackendType GetPreferredCanvasBackend() override;
=======
  // returns ClearType tuning information for each display
  static void GetCleartypeParams(nsTArray<ClearTypeParameterInfo>& aParams);
>>>>>>> upstream-releases

<<<<<<< HEAD
  // returns ClearType tuning information for each display
  static void GetCleartypeParams(nsTArray<ClearTypeParameterInfo>& aParams);
||||||| merged common ancestors
    static void GetDLLVersion(char16ptr_t aDLLPath, nsAString& aVersion);
=======
  void FontsPrefsChanged(const char* aPref) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual void FontsPrefsChanged(const char* aPref) override;
||||||| merged common ancestors
    // returns ClearType tuning information for each display
    static void GetCleartypeParams(nsTArray<ClearTypeParameterInfo>& aParams);
=======
  void SetupClearTypeParams();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void SetupClearTypeParams();
||||||| merged common ancestors
    virtual void FontsPrefsChanged(const char *aPref) override;
=======
  static inline bool DWriteEnabled() {
    return !!mozilla::gfx::Factory::GetDWriteFactory();
  }
  inline DWRITE_MEASURING_MODE DWriteMeasuringMode() { return mMeasuringMode; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  inline bool DWriteEnabled() const {
    return !!mozilla::gfx::Factory::GetDWriteFactory();
  }
  inline DWRITE_MEASURING_MODE DWriteMeasuringMode() { return mMeasuringMode; }
||||||| merged common ancestors
    void SetupClearTypeParams();
=======
  IDWriteRenderingParams* GetRenderingParams(TextRenderingMode aRenderMode) {
    return mRenderingParams[aRenderMode];
  }
>>>>>>> upstream-releases

<<<<<<< HEAD
  IDWriteRenderingParams* GetRenderingParams(TextRenderingMode aRenderMode) {
    return mRenderingParams[aRenderMode];
  }
||||||| merged common ancestors
    inline bool DWriteEnabled() const { return !!mozilla::gfx::Factory::GetDWriteFactory(); }
    inline DWRITE_MEASURING_MODE DWriteMeasuringMode() { return mMeasuringMode; }
=======
 public:
  bool DwmCompositionEnabled();
>>>>>>> upstream-releases

<<<<<<< HEAD
 public:
  bool DwmCompositionEnabled();
||||||| merged common ancestors
    IDWriteRenderingParams *GetRenderingParams(TextRenderingMode aRenderMode)
    { return mRenderingParams[aRenderMode]; }
=======
  mozilla::layers::ReadbackManagerD3D11* GetReadbackManager();
>>>>>>> upstream-releases

<<<<<<< HEAD
  mozilla::layers::ReadbackManagerD3D11* GetReadbackManager();
||||||| merged common ancestors
public:
    bool DwmCompositionEnabled();
=======
  static bool IsOptimus();
>>>>>>> upstream-releases

<<<<<<< HEAD
  static bool IsOptimus();
||||||| merged common ancestors
    mozilla::layers::ReadbackManagerD3D11* GetReadbackManager();
=======
  bool SupportsApzWheelInput() const override { return true; }
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool SupportsApzWheelInput() const override { return true; }
||||||| merged common ancestors
    static bool IsOptimus();
=======
  // Recreate devices as needed for a device reset. Returns true if a device
  // reset occurred.
  bool HandleDeviceReset();
  void UpdateBackendPrefs();
>>>>>>> upstream-releases

<<<<<<< HEAD
  // Recreate devices as needed for a device reset. Returns true if a device
  // reset occurred.
  bool HandleDeviceReset();
  void UpdateBackendPrefs();
||||||| merged common ancestors
    bool SupportsApzWheelInput() const override {
      return true;
    }
=======
  already_AddRefed<mozilla::gfx::VsyncSource> CreateHardwareVsyncSource()
      override;
  static mozilla::Atomic<size_t> sD3D11SharedTextures;
  static mozilla::Atomic<size_t> sD3D9SharedTextures;
>>>>>>> upstream-releases

<<<<<<< HEAD
  virtual already_AddRefed<mozilla::gfx::VsyncSource>
  CreateHardwareVsyncSource() override;
  static mozilla::Atomic<size_t> sD3D11SharedTextures;
  static mozilla::Atomic<size_t> sD3D9SharedTextures;
||||||| merged common ancestors
    // Recreate devices as needed for a device reset. Returns true if a device
    // reset occurred.
    bool HandleDeviceReset();
    void UpdateBackendPrefs();
=======
  bool SupportsPluginDirectBitmapDrawing() override { return true; }
  bool SupportsPluginDirectDXGIDrawing();
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool SupportsPluginDirectBitmapDrawing() override { return true; }
  bool SupportsPluginDirectDXGIDrawing();
||||||| merged common ancestors
    virtual already_AddRefed<mozilla::gfx::VsyncSource> CreateHardwareVsyncSource() override;
    static mozilla::Atomic<size_t> sD3D11SharedTextures;
    static mozilla::Atomic<size_t> sD3D9SharedTextures;
=======
  static void RecordContentDeviceFailure(
      mozilla::gfx::TelemetryDeviceCode aDevice);
>>>>>>> upstream-releases

<<<<<<< HEAD
  static void RecordContentDeviceFailure(
      mozilla::gfx::TelemetryDeviceCode aDevice);
||||||| merged common ancestors
    bool SupportsPluginDirectBitmapDrawing() override {
      return true;
    }
    bool SupportsPluginDirectDXGIDrawing();
=======
 protected:
  bool AccelerateLayersByDefault() override { return true; }
  void GetAcceleratedCompositorBackends(
      nsTArray<mozilla::layers::LayersBackend>& aBackends) override;
  void GetPlatformCMSOutputProfile(void*& mem, size_t& size) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  bool AccelerateLayersByDefault() override { return true; }
  void GetAcceleratedCompositorBackends(
      nsTArray<mozilla::layers::LayersBackend>& aBackends) override;
  virtual void GetPlatformCMSOutputProfile(void*& mem, size_t& size) override;
||||||| merged common ancestors
    static void RecordContentDeviceFailure(mozilla::gfx::TelemetryDeviceCode aDevice);
=======
  void ImportGPUDeviceData(const mozilla::gfx::GPUDeviceData& aData) override;
  void ImportContentDeviceData(
      const mozilla::gfx::ContentDeviceData& aData) override;
  void BuildContentDeviceData(mozilla::gfx::ContentDeviceData* aOut) override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void ImportGPUDeviceData(const mozilla::gfx::GPUDeviceData& aData) override;
  void ImportContentDeviceData(
      const mozilla::gfx::ContentDeviceData& aData) override;
  void BuildContentDeviceData(mozilla::gfx::ContentDeviceData* aOut) override;
||||||| merged common ancestors
protected:
    bool AccelerateLayersByDefault() override {
      return true;
    }
    void GetAcceleratedCompositorBackends(nsTArray<mozilla::layers::LayersBackend>& aBackends) override;
    virtual void GetPlatformCMSOutputProfile(void* &mem, size_t &size) override;
=======
  BackendPrefsData GetBackendPrefs() const override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  BackendPrefsData GetBackendPrefs() const override;
||||||| merged common ancestors
    void ImportGPUDeviceData(const mozilla::gfx::GPUDeviceData& aData) override;
    void ImportContentDeviceData(const mozilla::gfx::ContentDeviceData& aData) override;
    void BuildContentDeviceData(mozilla::gfx::ContentDeviceData* aOut) override;
=======
  bool CheckVariationFontSupport() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  bool CheckVariationFontSupport() override;
||||||| merged common ancestors
    BackendPrefsData GetBackendPrefs() const override;
=======
 protected:
  RenderMode mRenderMode;
>>>>>>> upstream-releases

<<<<<<< HEAD
 protected:
  RenderMode mRenderMode;
||||||| merged common ancestors
    bool CheckVariationFontSupport() override;
=======
 private:
  bool HasBattery() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
 private:
  bool HasBattery() override;
||||||| merged common ancestors
protected:
    RenderMode mRenderMode;
=======
  void Init();
  void InitAcceleration() override;
  void InitWebRenderConfig() override;
>>>>>>> upstream-releases

<<<<<<< HEAD
  void Init();
  void InitAcceleration() override;
  void InitWebRenderConfig() override;
||||||| merged common ancestors
private:
    void Init();
    void InitAcceleration() override;
    void InitWebRenderConfig() override;
=======
  void InitializeDevices();
  void InitializeD3D11();
  void InitializeD2D();
  bool InitDWriteSupport();
  bool InitGPUProcessSupport();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void InitializeDevices();
  void InitializeD3D11();
  void InitializeD2D();
  bool InitDWriteSupport();
  bool InitGPUProcessSupport();
||||||| merged common ancestors
    void InitializeDevices();
    void InitializeD3D11();
    void InitializeD2D();
    bool InitDWriteSupport();
    bool InitGPUProcessSupport();
=======
  void DisableD2D(mozilla::gfx::FeatureStatus aStatus, const char* aMessage,
                  const nsACString& aFailureId);
>>>>>>> upstream-releases

<<<<<<< HEAD
  void DisableD2D(mozilla::gfx::FeatureStatus aStatus, const char* aMessage,
                  const nsACString& aFailureId);
||||||| merged common ancestors
    void DisableD2D(mozilla::gfx::FeatureStatus aStatus, const char* aMessage,
                    const nsACString& aFailureId);
=======
  void InitializeConfig();
  void InitializeD3D9Config();
  void InitializeD3D11Config();
  void InitializeD2DConfig();
  void InitializeDirectDrawConfig();
  void InitializeAdvancedLayersConfig();
>>>>>>> upstream-releases

<<<<<<< HEAD
  void InitializeConfig();
  void InitializeD3D9Config();
  void InitializeD3D11Config();
  void InitializeD2DConfig();
  void InitializeDirectDrawConfig();
  void InitializeAdvancedLayersConfig();
||||||| merged common ancestors
    void InitializeConfig();
    void InitializeD3D9Config();
    void InitializeD3D11Config();
    void InitializeD2DConfig();
    void InitializeDirectDrawConfig();
    void InitializeAdvancedLayersConfig();
=======
  void RecordStartupTelemetry();
>>>>>>> upstream-releases

  RefPtr<IDWriteRenderingParams> mRenderingParams[TEXT_RENDERING_COUNT];
  DWRITE_MEASURING_MODE mMeasuringMode;

<<<<<<< HEAD
  RefPtr<mozilla::layers::ReadbackManagerD3D11> mD3D11ReadbackManager;
||||||| merged common ancestors
    RefPtr<mozilla::layers::ReadbackManagerD3D11> mD3D11ReadbackManager;
=======
  RefPtr<mozilla::layers::ReadbackManagerD3D11> mD3D11ReadbackManager;
  bool mInitializedDevices = false;
>>>>>>> upstream-releases
};

#endif /* GFX_WINDOWS_PLATFORM_H */
