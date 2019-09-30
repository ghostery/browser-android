/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrVkBackendContext_DEFINED
#define GrVkBackendContext_DEFINED

#include "GrVkTypes.h"
#include "SkRefCnt.h"
#include "vk/GrVkMemoryAllocator.h"

class GrVkExtensions;

enum GrVkExtensionFlags {
    kEXT_debug_report_GrVkExtensionFlag    = 0x0001,
    kNV_glsl_shader_GrVkExtensionFlag      = 0x0002,
    kKHR_surface_GrVkExtensionFlag         = 0x0004,
    kKHR_swapchain_GrVkExtensionFlag       = 0x0008,
    kKHR_win32_surface_GrVkExtensionFlag   = 0x0010,
    kKHR_android_surface_GrVkExtensionFlag = 0x0020,
    kKHR_xcb_surface_GrVkExtensionFlag     = 0x0040,
};

enum GrVkFeatureFlags {
    kGeometryShader_GrVkFeatureFlag    = 0x0001,
    kDualSrcBlend_GrVkFeatureFlag      = 0x0002,
    kSampleRateShading_GrVkFeatureFlag = 0x0004,
};

// It is not guarenteed VkPhysicalDeviceProperties2 will be in the client's header so we forward
// declare it here to be safe.
struct VkPhysicalDeviceFeatures2;

// The BackendContext contains all of the base Vulkan objects needed by the GrVkGpu. The assumption
// is that the client will set these up and pass them to the GrVkGpu constructor. The VkDevice
// created must support at least one graphics queue, which is passed in as well.
// The QueueFamilyIndex must match the family of the given queue. It is needed for CommandPool
// creation, and any GrBackendObjects handed to us (e.g., for wrapped textures) needs to be created
// in or transitioned to that family. The refs held by members of this struct must be released
// (either by deleting the struct or manually releasing the refs) before the underlying vulkan
// device and instance are destroyed.
struct SK_API GrVkBackendContext {
    VkInstance                 fInstance;
    VkPhysicalDevice           fPhysicalDevice;
    VkDevice                   fDevice;
    VkQueue                    fQueue;
    uint32_t                   fGraphicsQueueIndex;
<<<<<<< HEAD
    uint32_t                   fMinAPIVersion; // Deprecated. Set fInstanceVersion instead.
    uint32_t                   fInstanceVersion = 0;
    uint32_t                   fExtensions = 0; // Deprecated. Use fVkExtensions instead.
    const GrVkExtensions*      fVkExtensions = nullptr;
    uint32_t                   fFeatures; // Deprecated. Use either fDeviceFeatures[2] instead.
    // The client can create their VkDevice with either a VkPhysicalDeviceFeatures or
    // VkPhysicalDeviceFeatures2 struct, thus we have to support taking both. The
    // VkPhysicalDeviceFeatures2 struct is needed so we know if the client enabled any extension
    // specific features. If fDeviceFeatures2 is not null then we ignore fDeviceFeatures. If both
    // fDeviceFeatures and fDeviceFeatures2 are null we will assume no features are enabled.
    VkPhysicalDeviceFeatures*  fDeviceFeatures = nullptr;
    VkPhysicalDeviceFeatures2* fDeviceFeatures2 = nullptr;
    sk_sp<GrVkMemoryAllocator> fMemoryAllocator;
    GrVkGetProc                fGetProc = nullptr;
    // This is deprecated and should be set to false. The client is responsible for managing the
    // lifetime of the VkInstance and VkDevice objects.
    bool                       fOwnsInstanceAndDevice = false;
||||||| merged common ancestors
    uint32_t                   fMinAPIVersion;
    uint32_t                   fExtensions;
    uint32_t                   fFeatures;
    sk_sp<const GrVkInterface> fInterface;
    /**
     * Controls whether this object destroys the instance and device upon destruction. The default
     * is temporarily 'true' to avoid breaking existing clients but will be changed to 'false'.
     */
    bool                       fOwnsInstanceAndDevice = true;

    using CanPresentFn = std::function<bool(VkInstance, VkPhysicalDevice,
                                            uint32_t queueFamilyIndex)>;

    /**
     * Helper function to create the Vulkan objects needed for a Vulkan-backed GrContext.
     * Note that the version that uses the unified "GetProc" instead of separate "GetInstanceProc"
     * and "GetDeviceProc" functions will be removed.
     *
     * If presentQueueIndex is non-NULL, will try to set up presentQueue as part of device
     * creation using the platform-specific canPresent() function.
     *
     * This will set fOwnsInstanceAndDevice to 'true'. If it is subsequently set to 'false' then
     * the client owns the lifetime of the created VkDevice and VkInstance.
     */
    static const GrVkBackendContext* Create(uint32_t* presentQueueIndex = nullptr,
                                            CanPresentFn = CanPresentFn(),
                                            GrVkInterface::GetProc getProc = nullptr);

    static const GrVkBackendContext* Create(const GrVkInterface::GetInstanceProc& getInstanceProc,
                                            const GrVkInterface::GetDeviceProc& getDeviceProc,
                                            uint32_t* presentQueueIndex = nullptr,
                                            CanPresentFn canPresent = CanPresentFn()) {
        if (!getInstanceProc || !getDeviceProc) {
            return nullptr;
        }
        auto getProc = [&getInstanceProc, &getDeviceProc](const char* proc_name,
                                                          VkInstance instance, VkDevice device) {
            if (device != VK_NULL_HANDLE) {
                return getDeviceProc(device, proc_name);
            }
            return getInstanceProc(instance, proc_name);
        };
        return Create(presentQueueIndex, canPresent, getProc);
    }

    ~GrVkBackendContext() override;
=======
    uint32_t                   fMinAPIVersion; // Deprecated. Set fInstanceVersion instead.
    uint32_t                   fInstanceVersion = 0; // Deprecated. Set fMaxApiVersion instead.
    // The max api version set here should match the value set in VkApplicationInfo::apiVersion when
    // then VkInstance was created.
    uint32_t                   fMaxAPIVersion = 0;
    uint32_t                   fExtensions = 0; // Deprecated. Use fVkExtensions instead.
    const GrVkExtensions*      fVkExtensions = nullptr;
    uint32_t                   fFeatures; // Deprecated. Use either fDeviceFeatures[2] instead.
    // The client can create their VkDevice with either a VkPhysicalDeviceFeatures or
    // VkPhysicalDeviceFeatures2 struct, thus we have to support taking both. The
    // VkPhysicalDeviceFeatures2 struct is needed so we know if the client enabled any extension
    // specific features. If fDeviceFeatures2 is not null then we ignore fDeviceFeatures. If both
    // fDeviceFeatures and fDeviceFeatures2 are null we will assume no features are enabled.
    VkPhysicalDeviceFeatures*  fDeviceFeatures = nullptr;
    VkPhysicalDeviceFeatures2* fDeviceFeatures2 = nullptr;
    sk_sp<GrVkMemoryAllocator> fMemoryAllocator;
    GrVkGetProc                fGetProc = nullptr;
    // This is deprecated and should be set to false. The client is responsible for managing the
    // lifetime of the VkInstance and VkDevice objects.
    bool                       fOwnsInstanceAndDevice = false;
>>>>>>> upstream-releases
};

#endif
