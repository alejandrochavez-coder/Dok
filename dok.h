#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DokResult {
    DOK_SUCCESS = 0,
    DOK_OUT_OF_MEMORY = 1,
    DOK_VULKAN_UNSUPPORTED = 2
} DokResult;

typedef struct DokInstance_T* DokInstance;
typedef struct DokGlobalFunctions DokGlobalFunctions;
typedef struct DokInstanceFunctions DokInstanceFunctions;
typedef struct DokDeviceFunctions DokDeviceFunctions;

DokResult dokCreateInstance(DokInstance* pInstance);

void dokDestroyInstance(DokInstance instance);

void dokInitializeGlobalFunctions(DokInstance instance, DokGlobalFunctions* pGlobalFunctions);

void dokInitializeInstanceFunctions(VkInstance instance, DokGlobalFunctions* pGlobalFunctions, DokInstanceFunctions* pInstanceFunctions);

void dokInitializeDeviceFunctions(VkDevice device, DokInstanceFunctions* pInstanceFunctions, DokDeviceFunctions* pDeviceFunctions);

#ifdef __cplusplus
}
#endif

struct DokGlobalFunctions {
//DOK_START_GLOBAL
#if defined(VK_VERSION_1_0)
    PFN_vkCreateInstance vkCreateInstance;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
#endif // defined(VK_VERSION_1_0)
#if defined(VK_VERSION_1_1)
    PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
#endif // defined(VK_VERSION_1_1)
//DOK_END_GLOBAL
};

struct DokInstanceFunctions {
//DOK_START_INSTANCE
#if defined(VK_VERSION_1_0)
    PFN_vkDestroyInstance vkDestroyInstance;
    PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
    PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
    PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
    PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
    PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
    PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
    PFN_vkCreateDevice vkCreateDevice;
    PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
    PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
#endif // defined(VK_VERSION_1_0)
#if defined(VK_VERSION_1_1)
    PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups;
    PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
    PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
    PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2;
    PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
    PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2;
    PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties;
    PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties;
    PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties;
#endif // defined(VK_VERSION_1_1)
#if defined(VK_VERSION_1_3)
    PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties;
#endif // defined(VK_VERSION_1_3)
#if defined(VK_KHR_surface)
    PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
#endif // defined(VK_KHR_surface)
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface))
    PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR;
#endif // (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface))
#if defined(VK_KHR_display)
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
    PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
    PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
    PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
    PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
#endif // defined(VK_KHR_display)
#if defined(VK_KHR_xlib_surface)
    PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif // defined(VK_KHR_xlib_surface)
#if defined(VK_KHR_xcb_surface)
    PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif // defined(VK_KHR_xcb_surface)
#if defined(VK_KHR_wayland_surface)
    PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif // defined(VK_KHR_wayland_surface)
#if defined(VK_KHR_android_surface)
    PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif // defined(VK_KHR_android_surface)
#if defined(VK_KHR_win32_surface)
    PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif // defined(VK_KHR_win32_surface)
#if defined(VK_EXT_debug_report) && (VK_EXT_DEBUG_REPORT_SPEC_VERSION>=10)
    PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
    PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
#endif // defined(VK_EXT_debug_report) && (VK_EXT_DEBUG_REPORT_SPEC_VERSION>=10)
#if defined(VK_KHR_video_queue)
    PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR;
#endif // defined(VK_KHR_video_queue)
#if defined(VK_GGP_stream_descriptor_surface)
    PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP;
#endif // defined(VK_GGP_stream_descriptor_surface)
#if defined(VK_NV_external_memory_capabilities)
    PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
#endif // defined(VK_NV_external_memory_capabilities)
#if defined(VK_KHR_get_physical_device_properties2)
    PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR;
    PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR;
    PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR;
    PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR;
    PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
#endif // defined(VK_KHR_get_physical_device_properties2)
#if defined(VK_NN_vi_surface)
    PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#endif // defined(VK_NN_vi_surface)
#if defined(VK_KHR_device_group_creation)
    PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR;
#endif // defined(VK_KHR_device_group_creation)
#if defined(VK_KHR_external_memory_capabilities)
    PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR;
#endif // defined(VK_KHR_external_memory_capabilities)
#if defined(VK_KHR_external_semaphore_capabilities)
    PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
#endif // defined(VK_KHR_external_semaphore_capabilities)
#if defined(VK_EXT_direct_mode_display)
    PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT;
#endif // defined(VK_EXT_direct_mode_display)
#if defined(VK_EXT_acquire_xlib_display)
    PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT;
    PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT;
#endif // defined(VK_EXT_acquire_xlib_display)
#if defined(VK_EXT_display_surface_counter)
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#endif // defined(VK_EXT_display_surface_counter)
#if defined(VK_KHR_external_fence_capabilities)
    PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR;
#endif // defined(VK_KHR_external_fence_capabilities)
#if defined(VK_KHR_performance_query)
    PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
    PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
#endif // defined(VK_KHR_performance_query)
#if defined(VK_KHR_get_surface_capabilities2)
    PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
    PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif // defined(VK_KHR_get_surface_capabilities2)
#if defined(VK_KHR_get_display_properties2)
    PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR;
    PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
    PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR;
    PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR;
#endif // defined(VK_KHR_get_display_properties2)
#if defined(VK_MVK_ios_surface) && (VK_MVK_IOS_SURFACE_SPEC_VERSION>=3)
    PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK;
#endif // defined(VK_MVK_ios_surface) && (VK_MVK_IOS_SURFACE_SPEC_VERSION>=3)
#if defined(VK_MVK_macos_surface) && (VK_MVK_MACOS_SURFACE_SPEC_VERSION>=3)
    PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#endif // defined(VK_MVK_macos_surface) && (VK_MVK_MACOS_SURFACE_SPEC_VERSION>=3)
#if defined(VK_EXT_debug_utils) && (VK_EXT_DEBUG_UTILS_SPEC_VERSION>=2)
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
    PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
#endif // defined(VK_EXT_debug_utils) && (VK_EXT_DEBUG_UTILS_SPEC_VERSION>=2)
#if defined(VK_EXT_descriptor_heap)
    PFN_vkGetPhysicalDeviceDescriptorSizeEXT vkGetPhysicalDeviceDescriptorSizeEXT;
#endif // defined(VK_EXT_descriptor_heap)
#if defined(VK_EXT_sample_locations)
    PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
#endif // defined(VK_EXT_sample_locations)
#if defined(VK_EXT_calibrated_timestamps) && (VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION>=2)
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
#endif // defined(VK_EXT_calibrated_timestamps) && (VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION>=2)
#if defined(VK_FUCHSIA_imagepipe_surface)
    PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA;
#endif // defined(VK_FUCHSIA_imagepipe_surface)
#if defined(VK_EXT_metal_surface)
    PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT;
#endif // defined(VK_EXT_metal_surface)
#if defined(VK_KHR_fragment_shading_rate)
    PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR;
#endif // defined(VK_KHR_fragment_shading_rate)
#if defined(VK_EXT_tooling_info)
    PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT;
#endif // defined(VK_EXT_tooling_info)
#if defined(VK_NV_cooperative_matrix)
    PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
#endif // defined(VK_NV_cooperative_matrix)
#if defined(VK_NV_coverage_reduction_mode)
    PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
#endif // defined(VK_NV_coverage_reduction_mode)
#if defined(VK_EXT_full_screen_exclusive) && (VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION>=4)
    PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
#endif // defined(VK_EXT_full_screen_exclusive) && (VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION>=4)
#if defined(VK_EXT_headless_surface)
    PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT;
#endif // defined(VK_EXT_headless_surface)
#if defined(VK_EXT_acquire_drm_display)
    PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT;
    PFN_vkGetDrmDisplayEXT vkGetDrmDisplayEXT;
#endif // defined(VK_EXT_acquire_drm_display)
#if defined(VK_KHR_video_encode_queue)
    PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR;
#endif // defined(VK_KHR_video_encode_queue)
#if defined(VK_NV_acquire_winrt_display)
    PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV;
    PFN_vkGetWinrtDisplayNV vkGetWinrtDisplayNV;
#endif // defined(VK_NV_acquire_winrt_display)
#if defined(VK_EXT_directfb_surface)
    PFN_vkCreateDirectFBSurfaceEXT vkCreateDirectFBSurfaceEXT;
    PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT vkGetPhysicalDeviceDirectFBPresentationSupportEXT;
#endif // defined(VK_EXT_directfb_surface)
#if defined(VK_QNX_screen_surface)
    PFN_vkCreateScreenSurfaceQNX vkCreateScreenSurfaceQNX;
    PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX vkGetPhysicalDeviceScreenPresentationSupportQNX;
#endif // defined(VK_QNX_screen_surface)
#if defined(VK_ARM_tensors) && (VK_ARM_TENSORS_SPEC_VERSION>=2)
    PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM vkGetPhysicalDeviceExternalTensorPropertiesARM;
#endif // defined(VK_ARM_tensors) && (VK_ARM_TENSORS_SPEC_VERSION>=2)
#if defined(VK_NV_optical_flow)
    PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV vkGetPhysicalDeviceOpticalFlowImageFormatsNV;
#endif // defined(VK_NV_optical_flow)
#if defined(VK_NV_cooperative_vector) && (VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION>=4)
    PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV vkGetPhysicalDeviceCooperativeVectorPropertiesNV;
#endif // defined(VK_NV_cooperative_vector) && (VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION>=4)
#if defined(VK_KHR_cooperative_matrix)
    PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR;
#endif // defined(VK_KHR_cooperative_matrix)
#if defined(VK_ARM_data_graph)
    PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM;
    PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM;
#endif // defined(VK_ARM_data_graph)
#if (defined(VK_ARM_data_graph_instruction_set_tosa)) || (defined(VK_ARM_data_graph_optical_flow))
    PFN_vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM;
#endif // (defined(VK_ARM_data_graph_instruction_set_tosa)) || (defined(VK_ARM_data_graph_optical_flow))
#if defined(VK_KHR_calibrated_timestamps)
    PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR vkGetPhysicalDeviceCalibrateableTimeDomainsKHR;
#endif // defined(VK_KHR_calibrated_timestamps)
#if defined(VK_OHOS_surface)
    PFN_vkCreateSurfaceOHOS vkCreateSurfaceOHOS;
#endif // defined(VK_OHOS_surface)
#if defined(VK_NV_cooperative_matrix2)
    PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV;
#endif // defined(VK_NV_cooperative_matrix2)
#if defined(VK_ARM_performance_counters_by_region)
    PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM;
#endif // defined(VK_ARM_performance_counters_by_region)
#if defined(VK_ARM_shader_instrumentation)
    PFN_vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM;
#endif // defined(VK_ARM_shader_instrumentation)
#if defined(VK_ARM_data_graph_optical_flow)
    PFN_vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM;
#endif // defined(VK_ARM_data_graph_optical_flow)
#if defined(VK_SEC_ubm_surface)
    PFN_vkCreateUbmSurfaceSEC vkCreateUbmSurfaceSEC;
    PFN_vkGetPhysicalDeviceUbmPresentationSupportSEC vkGetPhysicalDeviceUbmPresentationSupportSEC;
#endif // defined(VK_SEC_ubm_surface)
//DOK_END_INSTANCE
};

struct DokDeviceFunctions {
//DOK_START_DEVICE
#if defined(VK_VERSION_1_0)
    PFN_vkDestroyDevice vkDestroyDevice;
    PFN_vkGetDeviceQueue vkGetDeviceQueue;
    PFN_vkQueueSubmit vkQueueSubmit;
    PFN_vkQueueWaitIdle vkQueueWaitIdle;
    PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
    PFN_vkAllocateMemory vkAllocateMemory;
    PFN_vkFreeMemory vkFreeMemory;
    PFN_vkMapMemory vkMapMemory;
    PFN_vkUnmapMemory vkUnmapMemory;
    PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
    PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
    PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
    PFN_vkBindBufferMemory vkBindBufferMemory;
    PFN_vkBindImageMemory vkBindImageMemory;
    PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
    PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
    PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
    PFN_vkQueueBindSparse vkQueueBindSparse;
    PFN_vkCreateFence vkCreateFence;
    PFN_vkDestroyFence vkDestroyFence;
    PFN_vkResetFences vkResetFences;
    PFN_vkGetFenceStatus vkGetFenceStatus;
    PFN_vkWaitForFences vkWaitForFences;
    PFN_vkCreateSemaphore vkCreateSemaphore;
    PFN_vkDestroySemaphore vkDestroySemaphore;
    PFN_vkCreateQueryPool vkCreateQueryPool;
    PFN_vkDestroyQueryPool vkDestroyQueryPool;
    PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
    PFN_vkCreateBuffer vkCreateBuffer;
    PFN_vkDestroyBuffer vkDestroyBuffer;
    PFN_vkCreateImage vkCreateImage;
    PFN_vkDestroyImage vkDestroyImage;
    PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
    PFN_vkCreateImageView vkCreateImageView;
    PFN_vkDestroyImageView vkDestroyImageView;
    PFN_vkCreateCommandPool vkCreateCommandPool;
    PFN_vkDestroyCommandPool vkDestroyCommandPool;
    PFN_vkResetCommandPool vkResetCommandPool;
    PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
    PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
    PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
    PFN_vkEndCommandBuffer vkEndCommandBuffer;
    PFN_vkResetCommandBuffer vkResetCommandBuffer;
    PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
    PFN_vkCmdCopyImage vkCmdCopyImage;
    PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
    PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
    PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
    PFN_vkCmdFillBuffer vkCmdFillBuffer;
    PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
    PFN_vkCmdBeginQuery vkCmdBeginQuery;
    PFN_vkCmdEndQuery vkCmdEndQuery;
    PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
    PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
    PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
    PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
    PFN_vkCreateEvent vkCreateEvent;
    PFN_vkDestroyEvent vkDestroyEvent;
    PFN_vkGetEventStatus vkGetEventStatus;
    PFN_vkSetEvent vkSetEvent;
    PFN_vkResetEvent vkResetEvent;
    PFN_vkCreateBufferView vkCreateBufferView;
    PFN_vkDestroyBufferView vkDestroyBufferView;
    PFN_vkCreateShaderModule vkCreateShaderModule;
    PFN_vkDestroyShaderModule vkDestroyShaderModule;
    PFN_vkCreatePipelineCache vkCreatePipelineCache;
    PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
    PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
    PFN_vkMergePipelineCaches vkMergePipelineCaches;
    PFN_vkCreateComputePipelines vkCreateComputePipelines;
    PFN_vkDestroyPipeline vkDestroyPipeline;
    PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
    PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
    PFN_vkCreateSampler vkCreateSampler;
    PFN_vkDestroySampler vkDestroySampler;
    PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
    PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
    PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
    PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
    PFN_vkResetDescriptorPool vkResetDescriptorPool;
    PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
    PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
    PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
    PFN_vkCmdBindPipeline vkCmdBindPipeline;
    PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
    PFN_vkCmdClearColorImage vkCmdClearColorImage;
    PFN_vkCmdDispatch vkCmdDispatch;
    PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
    PFN_vkCmdSetEvent vkCmdSetEvent;
    PFN_vkCmdResetEvent vkCmdResetEvent;
    PFN_vkCmdWaitEvents vkCmdWaitEvents;
    PFN_vkCmdPushConstants vkCmdPushConstants;
    PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
    PFN_vkCreateFramebuffer vkCreateFramebuffer;
    PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
    PFN_vkCreateRenderPass vkCreateRenderPass;
    PFN_vkDestroyRenderPass vkDestroyRenderPass;
    PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
    PFN_vkCmdSetViewport vkCmdSetViewport;
    PFN_vkCmdSetScissor vkCmdSetScissor;
    PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
    PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
    PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
    PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
    PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
    PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
    PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
    PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
    PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
    PFN_vkCmdDraw vkCmdDraw;
    PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
    PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
    PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
    PFN_vkCmdBlitImage vkCmdBlitImage;
    PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
    PFN_vkCmdClearAttachments vkCmdClearAttachments;
    PFN_vkCmdResolveImage vkCmdResolveImage;
    PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
    PFN_vkCmdNextSubpass vkCmdNextSubpass;
    PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
#endif // defined(VK_VERSION_1_0)
#if defined(VK_VERSION_1_1)
    PFN_vkBindBufferMemory2 vkBindBufferMemory2;
    PFN_vkBindImageMemory2 vkBindImageMemory2;
    PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
    PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
    PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
    PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
    PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
    PFN_vkTrimCommandPool vkTrimCommandPool;
    PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
    PFN_vkCmdDispatchBase vkCmdDispatchBase;
    PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
    PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
    PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
    PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
    PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
    PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
#endif // defined(VK_VERSION_1_1)
#if defined(VK_VERSION_1_2)
    PFN_vkResetQueryPool vkResetQueryPool;
    PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
    PFN_vkWaitSemaphores vkWaitSemaphores;
    PFN_vkSignalSemaphore vkSignalSemaphore;
    PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
    PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress;
    PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress;
    PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount;
    PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount;
    PFN_vkCreateRenderPass2 vkCreateRenderPass2;
    PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2;
    PFN_vkCmdNextSubpass2 vkCmdNextSubpass2;
    PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2;
#endif // defined(VK_VERSION_1_2)
#if defined(VK_VERSION_1_3)
    PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot;
    PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot;
    PFN_vkSetPrivateData vkSetPrivateData;
    PFN_vkGetPrivateData vkGetPrivateData;
    PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
    PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2;
    PFN_vkQueueSubmit2 vkQueueSubmit2;
    PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2;
    PFN_vkCmdCopyImage2 vkCmdCopyImage2;
    PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2;
    PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2;
    PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
    PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
    PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements;
    PFN_vkCmdSetEvent2 vkCmdSetEvent2;
    PFN_vkCmdResetEvent2 vkCmdResetEvent2;
    PFN_vkCmdWaitEvents2 vkCmdWaitEvents2;
    PFN_vkCmdBlitImage2 vkCmdBlitImage2;
    PFN_vkCmdResolveImage2 vkCmdResolveImage2;
    PFN_vkCmdBeginRendering vkCmdBeginRendering;
    PFN_vkCmdEndRendering vkCmdEndRendering;
    PFN_vkCmdSetCullMode vkCmdSetCullMode;
    PFN_vkCmdSetFrontFace vkCmdSetFrontFace;
    PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
    PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount;
    PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount;
    PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
    PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable;
    PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable;
    PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp;
    PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable;
    PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable;
    PFN_vkCmdSetStencilOp vkCmdSetStencilOp;
    PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable;
    PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable;
    PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable;
#endif // defined(VK_VERSION_1_3)
#if defined(VK_VERSION_1_4)
    PFN_vkMapMemory2 vkMapMemory2;
    PFN_vkUnmapMemory2 vkUnmapMemory2;
    PFN_vkGetDeviceImageSubresourceLayout vkGetDeviceImageSubresourceLayout;
    PFN_vkGetImageSubresourceLayout2 vkGetImageSubresourceLayout2;
    PFN_vkCopyMemoryToImage vkCopyMemoryToImage;
    PFN_vkCopyImageToMemory vkCopyImageToMemory;
    PFN_vkCopyImageToImage vkCopyImageToImage;
    PFN_vkTransitionImageLayout vkTransitionImageLayout;
    PFN_vkCmdPushDescriptorSet vkCmdPushDescriptorSet;
    PFN_vkCmdPushDescriptorSetWithTemplate vkCmdPushDescriptorSetWithTemplate;
    PFN_vkCmdBindDescriptorSets2 vkCmdBindDescriptorSets2;
    PFN_vkCmdPushConstants2 vkCmdPushConstants2;
    PFN_vkCmdPushDescriptorSet2 vkCmdPushDescriptorSet2;
    PFN_vkCmdPushDescriptorSetWithTemplate2 vkCmdPushDescriptorSetWithTemplate2;
    PFN_vkCmdSetLineStipple vkCmdSetLineStipple;
    PFN_vkCmdBindIndexBuffer2 vkCmdBindIndexBuffer2;
    PFN_vkGetRenderingAreaGranularity vkGetRenderingAreaGranularity;
    PFN_vkCmdSetRenderingAttachmentLocations vkCmdSetRenderingAttachmentLocations;
    PFN_vkCmdSetRenderingInputAttachmentIndices vkCmdSetRenderingInputAttachmentIndices;
#endif // defined(VK_VERSION_1_4)
#if defined(VK_KHR_swapchain)
    PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
    PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif // defined(VK_KHR_swapchain)
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface))
    PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
    PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
#endif // (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface))
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain))
    PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif // (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain))
#if defined(VK_KHR_display_swapchain)
    PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif // defined(VK_KHR_display_swapchain)
#if defined(VK_EXT_debug_marker) && (VK_EXT_DEBUG_MARKER_SPEC_VERSION>=4)
    PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
    PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
    PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
    PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
    PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
#endif // defined(VK_EXT_debug_marker) && (VK_EXT_DEBUG_MARKER_SPEC_VERSION>=4)
#if defined(VK_KHR_video_queue)
    PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR;
    PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR;
    PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR;
    PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR;
    PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR;
    PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR;
    PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR;
    PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR;
    PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR;
    PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR;
#endif // defined(VK_KHR_video_queue)
#if defined(VK_KHR_video_decode_queue)
    PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR;
#endif // defined(VK_KHR_video_decode_queue)
#if defined(VK_EXT_transform_feedback)
    PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT;
    PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT;
    PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT;
    PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT;
    PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT;
    PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT;
#endif // defined(VK_EXT_transform_feedback)
#if defined(VK_NVX_binary_import) && (VK_NVX_BINARY_IMPORT_SPEC_VERSION>=2)
    PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX;
    PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX;
    PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX;
    PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX;
    PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX;
#endif // defined(VK_NVX_binary_import) && (VK_NVX_BINARY_IMPORT_SPEC_VERSION>=2)
#if defined(VK_NVX_image_view_handle) && (VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION>=4)
    PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX;
    PFN_vkGetImageViewHandle64NVX vkGetImageViewHandle64NVX;
    PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX;
    PFN_vkGetDeviceCombinedImageSamplerIndexNVX vkGetDeviceCombinedImageSamplerIndexNVX;
#endif // defined(VK_NVX_image_view_handle) && (VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION>=4)
#if defined(VK_AMD_draw_indirect_count) && (VK_AMD_DRAW_INDIRECT_COUNT_SPEC_VERSION>=2)
    PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
    PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
#endif // defined(VK_AMD_draw_indirect_count) && (VK_AMD_DRAW_INDIRECT_COUNT_SPEC_VERSION>=2)
#if defined(VK_AMD_shader_info)
    PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif // defined(VK_AMD_shader_info)
#if defined(VK_KHR_dynamic_rendering)
    PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
    PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;
#endif // defined(VK_KHR_dynamic_rendering)
#if defined(VK_NV_external_memory_win32)
    PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif // defined(VK_NV_external_memory_win32)
#if defined(VK_KHR_device_group)
    PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
    PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
    PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
#endif // defined(VK_KHR_device_group)
#if defined(VK_KHR_maintenance1)
    PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#endif // defined(VK_KHR_maintenance1)
#if defined(VK_KHR_external_memory_win32)
    PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
    PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#endif // defined(VK_KHR_external_memory_win32)
#if defined(VK_KHR_external_memory_fd)
    PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
    PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#endif // defined(VK_KHR_external_memory_fd)
#if defined(VK_KHR_external_semaphore_win32)
    PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
    PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
#endif // defined(VK_KHR_external_semaphore_win32)
#if defined(VK_KHR_external_semaphore_fd)
    PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
    PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
#endif // defined(VK_KHR_external_semaphore_fd)
#if defined(VK_KHR_push_descriptor)
    PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#endif // defined(VK_KHR_push_descriptor)
#if (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template))) || (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor))
    PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#endif // (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template))) || (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor))
#if defined(VK_EXT_conditional_rendering) && (VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION>=2)
    PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
    PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif // defined(VK_EXT_conditional_rendering) && (VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION>=2)
#if defined(VK_KHR_descriptor_update_template)
    PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
    PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
    PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#endif // defined(VK_KHR_descriptor_update_template)
#if defined(VK_NV_clip_space_w_scaling)
    PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#endif // defined(VK_NV_clip_space_w_scaling)
#if defined(VK_EXT_display_control)
    PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
    PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
    PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
    PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
#endif // defined(VK_EXT_display_control)
#if defined(VK_GOOGLE_display_timing)
    PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
    PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
#endif // defined(VK_GOOGLE_display_timing)
#if defined(VK_EXT_discard_rectangles) && (VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION>=2)
    PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
    PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT;
    PFN_vkCmdSetDiscardRectangleModeEXT vkCmdSetDiscardRectangleModeEXT;
#endif // defined(VK_EXT_discard_rectangles) && (VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION>=2)
#if defined(VK_EXT_hdr_metadata) && (VK_EXT_HDR_METADATA_SPEC_VERSION>=3)
    PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif // defined(VK_EXT_hdr_metadata) && (VK_EXT_HDR_METADATA_SPEC_VERSION>=3)
#if defined(VK_KHR_create_renderpass2)
    PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
    PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
    PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
    PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
#endif // defined(VK_KHR_create_renderpass2)
#if defined(VK_KHR_shared_presentable_image)
    PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#endif // defined(VK_KHR_shared_presentable_image)
#if defined(VK_KHR_external_fence_win32)
    PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
    PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
#endif // defined(VK_KHR_external_fence_win32)
#if defined(VK_KHR_external_fence_fd)
    PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
    PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
#endif // defined(VK_KHR_external_fence_fd)
#if defined(VK_KHR_performance_query)
    PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR;
    PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR;
#endif // defined(VK_KHR_performance_query)
#if defined(VK_EXT_debug_utils) && (VK_EXT_DEBUG_UTILS_SPEC_VERSION>=2)
    PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
    PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
    PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
    PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
    PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
    PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
    PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
    PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
#endif // defined(VK_EXT_debug_utils) && (VK_EXT_DEBUG_UTILS_SPEC_VERSION>=2)
#if defined(VK_ANDROID_external_memory_android_hardware_buffer) && (VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION>=5)
    PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
    PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif // defined(VK_ANDROID_external_memory_android_hardware_buffer) && (VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION>=5)
#if defined(VK_AMDX_shader_enqueue) && (VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION>=2)
    PFN_vkCreateExecutionGraphPipelinesAMDX vkCreateExecutionGraphPipelinesAMDX;
    PFN_vkGetExecutionGraphPipelineScratchSizeAMDX vkGetExecutionGraphPipelineScratchSizeAMDX;
    PFN_vkGetExecutionGraphPipelineNodeIndexAMDX vkGetExecutionGraphPipelineNodeIndexAMDX;
    PFN_vkCmdInitializeGraphScratchMemoryAMDX vkCmdInitializeGraphScratchMemoryAMDX;
    PFN_vkCmdDispatchGraphAMDX vkCmdDispatchGraphAMDX;
    PFN_vkCmdDispatchGraphIndirectAMDX vkCmdDispatchGraphIndirectAMDX;
    PFN_vkCmdDispatchGraphIndirectCountAMDX vkCmdDispatchGraphIndirectCountAMDX;
#endif // defined(VK_AMDX_shader_enqueue) && (VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION>=2)
#if defined(VK_EXT_descriptor_heap)
    PFN_vkWriteSamplerDescriptorsEXT vkWriteSamplerDescriptorsEXT;
    PFN_vkWriteResourceDescriptorsEXT vkWriteResourceDescriptorsEXT;
    PFN_vkCmdBindSamplerHeapEXT vkCmdBindSamplerHeapEXT;
    PFN_vkCmdBindResourceHeapEXT vkCmdBindResourceHeapEXT;
    PFN_vkCmdPushDataEXT vkCmdPushDataEXT;
    PFN_vkGetImageOpaqueCaptureDataEXT vkGetImageOpaqueCaptureDataEXT;
#endif // defined(VK_EXT_descriptor_heap)
#if defined(VK_EXT_descriptor_heap) && defined(VK_EXT_custom_border_color)
    PFN_vkRegisterCustomBorderColorEXT vkRegisterCustomBorderColorEXT;
    PFN_vkUnregisterCustomBorderColorEXT vkUnregisterCustomBorderColorEXT;
#endif // defined(VK_EXT_descriptor_heap) && defined(VK_EXT_custom_border_color)
#if defined(VK_EXT_descriptor_heap) && defined(VK_ARM_tensors)
    PFN_vkGetTensorOpaqueCaptureDataARM vkGetTensorOpaqueCaptureDataARM;
#endif // defined(VK_EXT_descriptor_heap) && defined(VK_ARM_tensors)
#if defined(VK_EXT_sample_locations)
    PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
#endif // defined(VK_EXT_sample_locations)
#if defined(VK_KHR_get_memory_requirements2)
    PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
    PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
    PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#endif // defined(VK_KHR_get_memory_requirements2)
#if defined(VK_KHR_acceleration_structure)
    PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
    PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
    PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR;
    PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
    PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR;
    PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR;
    PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR;
    PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR;
    PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
    PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR;
    PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR;
    PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
    PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR;
    PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR;
    PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
#endif // defined(VK_KHR_acceleration_structure)
#if defined(VK_KHR_ray_tracing_pipeline)
    PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
    PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
    PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
    PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
    PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR;
    PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR;
    PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR;
#endif // defined(VK_KHR_ray_tracing_pipeline)
#if defined(VK_KHR_sampler_ycbcr_conversion)
    PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
    PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#endif // defined(VK_KHR_sampler_ycbcr_conversion)
#if defined(VK_KHR_bind_memory2)
    PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
    PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#endif // defined(VK_KHR_bind_memory2)
#if defined(VK_EXT_image_drm_format_modifier) && (VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION>=2)
    PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT;
#endif // defined(VK_EXT_image_drm_format_modifier) && (VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION>=2)
#if defined(VK_EXT_validation_cache)
    PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
    PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
    PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
    PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
#endif // defined(VK_EXT_validation_cache)
#if defined(VK_NV_shading_rate_image) && (VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION>=3)
    PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV;
    PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV;
    PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV;
#endif // defined(VK_NV_shading_rate_image) && (VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION>=3)
#if defined(VK_NV_ray_tracing) && (VK_NV_RAY_TRACING_SPEC_VERSION>=3)
    PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV;
    PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV;
    PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV;
    PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV;
    PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV;
    PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV;
    PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV;
    PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV;
    PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV;
    PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV;
    PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV;
    PFN_vkCompileDeferredNV vkCompileDeferredNV;
#endif // defined(VK_NV_ray_tracing) && (VK_NV_RAY_TRACING_SPEC_VERSION>=3)
#if defined(VK_KHR_maintenance3)
    PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#endif // defined(VK_KHR_maintenance3)
#if defined(VK_KHR_draw_indirect_count)
    PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
    PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
#endif // defined(VK_KHR_draw_indirect_count)
#if defined(VK_EXT_external_memory_host)
    PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif // defined(VK_EXT_external_memory_host)
#if defined(VK_AMD_buffer_marker)
    PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif // defined(VK_AMD_buffer_marker)
#if defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
    PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD;
#endif // defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
#if defined(VK_EXT_calibrated_timestamps) && (VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION>=2)
    PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT;
#endif // defined(VK_EXT_calibrated_timestamps) && (VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION>=2)
#if defined(VK_NV_mesh_shader)
    PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV;
    PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV;
#endif // defined(VK_NV_mesh_shader)
#if defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
    PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV;
#endif // defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
#if defined(VK_NV_scissor_exclusive) && (VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION>=2)
    PFN_vkCmdSetExclusiveScissorEnableNV vkCmdSetExclusiveScissorEnableNV;
    PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV;
#endif // defined(VK_NV_scissor_exclusive) && (VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION>=2)
#if defined(VK_NV_device_diagnostic_checkpoints) && (VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION>=2)
    PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
    PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif // defined(VK_NV_device_diagnostic_checkpoints) && (VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION>=2)
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
    PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV;
#endif // defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
#if defined(VK_KHR_timeline_semaphore)
    PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR;
    PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR;
    PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR;
#endif // defined(VK_KHR_timeline_semaphore)
#if defined(VK_EXT_present_timing) && (VK_EXT_PRESENT_TIMING_SPEC_VERSION>=3)
    PFN_vkSetSwapchainPresentTimingQueueSizeEXT vkSetSwapchainPresentTimingQueueSizeEXT;
    PFN_vkGetSwapchainTimingPropertiesEXT vkGetSwapchainTimingPropertiesEXT;
    PFN_vkGetSwapchainTimeDomainPropertiesEXT vkGetSwapchainTimeDomainPropertiesEXT;
    PFN_vkGetPastPresentationTimingEXT vkGetPastPresentationTimingEXT;
#endif // defined(VK_EXT_present_timing) && (VK_EXT_PRESENT_TIMING_SPEC_VERSION>=3)
#if defined(VK_INTEL_performance_query) && (VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION>=2)
    PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL;
    PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL;
    PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL;
    PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL;
    PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL;
    PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL;
    PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL;
    PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL;
    PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL;
#endif // defined(VK_INTEL_performance_query) && (VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION>=2)
#if defined(VK_AMD_display_native_hdr)
    PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD;
#endif // defined(VK_AMD_display_native_hdr)
#if defined(VK_KHR_fragment_shading_rate)
    PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR;
#endif // defined(VK_KHR_fragment_shading_rate)
#if defined(VK_KHR_dynamic_rendering_local_read)
    PFN_vkCmdSetRenderingAttachmentLocationsKHR vkCmdSetRenderingAttachmentLocationsKHR;
    PFN_vkCmdSetRenderingInputAttachmentIndicesKHR vkCmdSetRenderingInputAttachmentIndicesKHR;
#endif // defined(VK_KHR_dynamic_rendering_local_read)
#if defined(VK_EXT_buffer_device_address) && (VK_EXT_BUFFER_DEVICE_ADDRESS_SPEC_VERSION>=2)
    PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT;
#endif // defined(VK_EXT_buffer_device_address) && (VK_EXT_BUFFER_DEVICE_ADDRESS_SPEC_VERSION>=2)
#if defined(VK_KHR_present_wait)
    PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#endif // defined(VK_KHR_present_wait)
#if defined(VK_EXT_full_screen_exclusive) && (VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION>=4)
    PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
    PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif // defined(VK_EXT_full_screen_exclusive) && (VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION>=4)
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1))
    PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
#endif // defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1))
#if defined(VK_KHR_buffer_device_address)
    PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
    PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR;
    PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR;
#endif // defined(VK_KHR_buffer_device_address)
#if defined(VK_EXT_line_rasterization)
    PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT;
#endif // defined(VK_EXT_line_rasterization)
#if defined(VK_EXT_host_query_reset)
    PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT;
#endif // defined(VK_EXT_host_query_reset)
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
    PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT;
    PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT;
    PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT;
    PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT;
    PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT;
    PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT;
    PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT;
    PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT;
    PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT;
    PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT;
    PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT;
    PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT;
#endif // (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
#if defined(VK_KHR_deferred_host_operations)
    PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR;
    PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR;
    PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR;
    PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR;
    PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR;
#endif // defined(VK_KHR_deferred_host_operations)
#if defined(VK_KHR_pipeline_executable_properties)
    PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR;
    PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR;
    PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR;
#endif // defined(VK_KHR_pipeline_executable_properties)
#if defined(VK_EXT_host_image_copy)
    PFN_vkCopyMemoryToImageEXT vkCopyMemoryToImageEXT;
    PFN_vkCopyImageToMemoryEXT vkCopyImageToMemoryEXT;
    PFN_vkCopyImageToImageEXT vkCopyImageToImageEXT;
    PFN_vkTransitionImageLayoutEXT vkTransitionImageLayoutEXT;
#endif // defined(VK_EXT_host_image_copy)
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
    PFN_vkGetImageSubresourceLayout2EXT vkGetImageSubresourceLayout2EXT;
#endif // (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
#if defined(VK_KHR_map_memory2)
    PFN_vkMapMemory2KHR vkMapMemory2KHR;
    PFN_vkUnmapMemory2KHR vkUnmapMemory2KHR;
#endif // defined(VK_KHR_map_memory2)
#if defined(VK_EXT_swapchain_maintenance1)
    PFN_vkReleaseSwapchainImagesEXT vkReleaseSwapchainImagesEXT;
#endif // defined(VK_EXT_swapchain_maintenance1)
#if defined(VK_NV_device_generated_commands) && (VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION>=3)
    PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV;
    PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV;
    PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV;
    PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV;
    PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV;
    PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV;
#endif // defined(VK_NV_device_generated_commands) && (VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION>=3)
#if defined(VK_EXT_depth_bias_control)
    PFN_vkCmdSetDepthBias2EXT vkCmdSetDepthBias2EXT;
#endif // defined(VK_EXT_depth_bias_control)
#if defined(VK_EXT_private_data)
    PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT;
    PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT;
    PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT;
    PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT;
#endif // defined(VK_EXT_private_data)
#if defined(VK_KHR_video_encode_queue)
    PFN_vkGetEncodedVideoSessionParametersKHR vkGetEncodedVideoSessionParametersKHR;
    PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR;
#endif // defined(VK_KHR_video_encode_queue)
#if defined(VK_QCOM_queue_perf_hint)
    PFN_vkQueueSetPerfHintQCOM vkQueueSetPerfHintQCOM;
#endif // defined(VK_QCOM_queue_perf_hint)
#if defined(VK_NV_cuda_kernel_launch) && (VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION>=2)
    PFN_vkCreateCudaModuleNV vkCreateCudaModuleNV;
    PFN_vkGetCudaModuleCacheNV vkGetCudaModuleCacheNV;
    PFN_vkCreateCudaFunctionNV vkCreateCudaFunctionNV;
    PFN_vkDestroyCudaModuleNV vkDestroyCudaModuleNV;
    PFN_vkDestroyCudaFunctionNV vkDestroyCudaFunctionNV;
    PFN_vkCmdCudaLaunchKernelNV vkCmdCudaLaunchKernelNV;
#endif // defined(VK_NV_cuda_kernel_launch) && (VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION>=2)
#if defined(VK_QCOM_tile_shading) && (VK_QCOM_TILE_SHADING_SPEC_VERSION>=2)
    PFN_vkCmdDispatchTileQCOM vkCmdDispatchTileQCOM;
    PFN_vkCmdBeginPerTileExecutionQCOM vkCmdBeginPerTileExecutionQCOM;
    PFN_vkCmdEndPerTileExecutionQCOM vkCmdEndPerTileExecutionQCOM;
#endif // defined(VK_QCOM_tile_shading) && (VK_QCOM_TILE_SHADING_SPEC_VERSION>=2)
#if defined(VK_EXT_metal_objects) && (VK_EXT_METAL_OBJECTS_SPEC_VERSION>=2)
    PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT;
#endif // defined(VK_EXT_metal_objects) && (VK_EXT_METAL_OBJECTS_SPEC_VERSION>=2)
#if defined(VK_KHR_synchronization2)
    PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
    PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
    PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
    PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
    PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
    PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
#endif // defined(VK_KHR_synchronization2)
#if defined(VK_EXT_descriptor_buffer)
    PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT;
    PFN_vkGetDescriptorSetLayoutBindingOffsetEXT vkGetDescriptorSetLayoutBindingOffsetEXT;
    PFN_vkGetDescriptorEXT vkGetDescriptorEXT;
    PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT;
    PFN_vkCmdSetDescriptorBufferOffsetsEXT vkCmdSetDescriptorBufferOffsetsEXT;
    PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT vkCmdBindDescriptorBufferEmbeddedSamplersEXT;
    PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT vkGetBufferOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetImageOpaqueCaptureDescriptorDataEXT vkGetImageOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT vkGetImageViewOpaqueCaptureDescriptorDataEXT;
    PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT vkGetSamplerOpaqueCaptureDescriptorDataEXT;
#endif // defined(VK_EXT_descriptor_buffer)
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
    PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT;
#endif // defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
#if defined(VK_KHR_device_address_commands)
    PFN_vkCmdBindIndexBuffer3KHR vkCmdBindIndexBuffer3KHR;
    PFN_vkCmdBindVertexBuffers3KHR vkCmdBindVertexBuffers3KHR;
    PFN_vkCmdDrawIndirect2KHR vkCmdDrawIndirect2KHR;
    PFN_vkCmdDrawIndexedIndirect2KHR vkCmdDrawIndexedIndirect2KHR;
    PFN_vkCmdDispatchIndirect2KHR vkCmdDispatchIndirect2KHR;
    PFN_vkCmdCopyMemoryKHR vkCmdCopyMemoryKHR;
    PFN_vkCmdCopyMemoryToImageKHR vkCmdCopyMemoryToImageKHR;
    PFN_vkCmdCopyImageToMemoryKHR vkCmdCopyImageToMemoryKHR;
    PFN_vkCmdUpdateMemoryKHR vkCmdUpdateMemoryKHR;
    PFN_vkCmdFillMemoryKHR vkCmdFillMemoryKHR;
    PFN_vkCmdCopyQueryPoolResultsToMemoryKHR vkCmdCopyQueryPoolResultsToMemoryKHR;
#endif // defined(VK_KHR_device_address_commands)
#if defined(VK_KHR_device_address_commands) && (defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2))
    PFN_vkCmdDrawIndirectCount2KHR vkCmdDrawIndirectCount2KHR;
    PFN_vkCmdDrawIndexedIndirectCount2KHR vkCmdDrawIndexedIndirectCount2KHR;
#endif // defined(VK_KHR_device_address_commands) && (defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2))
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_conditional_rendering)
    PFN_vkCmdBeginConditionalRendering2EXT vkCmdBeginConditionalRendering2EXT;
#endif // defined(VK_KHR_device_address_commands) && defined(VK_EXT_conditional_rendering)
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_transform_feedback)
    PFN_vkCmdBindTransformFeedbackBuffers2EXT vkCmdBindTransformFeedbackBuffers2EXT;
    PFN_vkCmdBeginTransformFeedback2EXT vkCmdBeginTransformFeedback2EXT;
    PFN_vkCmdEndTransformFeedback2EXT vkCmdEndTransformFeedback2EXT;
    PFN_vkCmdDrawIndirectByteCount2EXT vkCmdDrawIndirectByteCount2EXT;
#endif // defined(VK_KHR_device_address_commands) && defined(VK_EXT_transform_feedback)
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_mesh_shader)
    PFN_vkCmdDrawMeshTasksIndirect2EXT vkCmdDrawMeshTasksIndirect2EXT;
#endif // defined(VK_KHR_device_address_commands) && defined(VK_EXT_mesh_shader)
#if defined(VK_KHR_device_address_commands) && ((defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2)) && defined(VK_EXT_mesh_shader))
    PFN_vkCmdDrawMeshTasksIndirectCount2EXT vkCmdDrawMeshTasksIndirectCount2EXT;
#endif // defined(VK_KHR_device_address_commands) && ((defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2)) && defined(VK_EXT_mesh_shader))
#if defined(VK_KHR_device_address_commands) && defined(VK_AMD_buffer_marker)
    PFN_vkCmdWriteMarkerToMemoryAMD vkCmdWriteMarkerToMemoryAMD;
#endif // defined(VK_KHR_device_address_commands) && defined(VK_AMD_buffer_marker)
#if defined(VK_KHR_device_address_commands) && defined(VK_KHR_acceleration_structure)
    PFN_vkCreateAccelerationStructure2KHR vkCreateAccelerationStructure2KHR;
#endif // defined(VK_KHR_device_address_commands) && defined(VK_KHR_acceleration_structure)
#if defined(VK_NV_fragment_shading_rate_enums)
    PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV;
#endif // defined(VK_NV_fragment_shading_rate_enums)
#if defined(VK_EXT_mesh_shader)
    PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT;
    PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT;
#endif // defined(VK_EXT_mesh_shader)
#if defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
    PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT;
#endif // defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
#if defined(VK_KHR_copy_commands2)
    PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR;
    PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR;
    PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR;
    PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR;
    PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR;
    PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR;
#endif // defined(VK_KHR_copy_commands2)
#if defined(VK_EXT_device_fault) && (VK_EXT_DEVICE_FAULT_SPEC_VERSION>=2)
    PFN_vkGetDeviceFaultInfoEXT vkGetDeviceFaultInfoEXT;
#endif // defined(VK_EXT_device_fault) && (VK_EXT_DEVICE_FAULT_SPEC_VERSION>=2)
#if (defined(VK_EXT_vertex_input_dynamic_state) && (VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION>=2)) || (defined(VK_EXT_shader_object))
    PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT;
#endif // (defined(VK_EXT_vertex_input_dynamic_state) && (VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION>=2)) || (defined(VK_EXT_shader_object))
#if defined(VK_FUCHSIA_external_memory)
    PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA;
    PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA;
#endif // defined(VK_FUCHSIA_external_memory)
#if defined(VK_FUCHSIA_external_semaphore)
    PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA;
    PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA;
#endif // defined(VK_FUCHSIA_external_semaphore)
#if defined(VK_FUCHSIA_buffer_collection) && (VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION>=2)
    PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA;
    PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA;
    PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA;
    PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA;
    PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA;
#endif // defined(VK_FUCHSIA_buffer_collection) && (VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION>=2)
#if defined(VK_HUAWEI_subpass_shading) && (VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION>=3)
    PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
    PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI;
#endif // defined(VK_HUAWEI_subpass_shading) && (VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION>=3)
#if defined(VK_HUAWEI_invocation_mask)
    PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI;
#endif // defined(VK_HUAWEI_invocation_mask)
#if defined(VK_NV_external_memory_rdma)
    PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV;
#endif // defined(VK_NV_external_memory_rdma)
#if defined(VK_EXT_pipeline_properties)
    PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT;
#endif // defined(VK_EXT_pipeline_properties)
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
    PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT;
    PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT;
    PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT;
    PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT;
    PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT;
#endif // (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
#if defined(VK_EXT_color_write_enable)
    PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT;
#endif // defined(VK_EXT_color_write_enable)
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
    PFN_vkCmdTraceRaysIndirect2KHR vkCmdTraceRaysIndirect2KHR;
#endif // defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
#if defined(VK_EXT_multi_draw)
    PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT;
    PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT;
#endif // defined(VK_EXT_multi_draw)
#if defined(VK_EXT_opacity_micromap) && (VK_EXT_OPACITY_MICROMAP_SPEC_VERSION>=2)
    PFN_vkCreateMicromapEXT vkCreateMicromapEXT;
    PFN_vkDestroyMicromapEXT vkDestroyMicromapEXT;
    PFN_vkCmdBuildMicromapsEXT vkCmdBuildMicromapsEXT;
    PFN_vkBuildMicromapsEXT vkBuildMicromapsEXT;
    PFN_vkCopyMicromapEXT vkCopyMicromapEXT;
    PFN_vkCopyMicromapToMemoryEXT vkCopyMicromapToMemoryEXT;
    PFN_vkCopyMemoryToMicromapEXT vkCopyMemoryToMicromapEXT;
    PFN_vkWriteMicromapsPropertiesEXT vkWriteMicromapsPropertiesEXT;
    PFN_vkCmdCopyMicromapEXT vkCmdCopyMicromapEXT;
    PFN_vkCmdCopyMicromapToMemoryEXT vkCmdCopyMicromapToMemoryEXT;
    PFN_vkCmdCopyMemoryToMicromapEXT vkCmdCopyMemoryToMicromapEXT;
    PFN_vkCmdWriteMicromapsPropertiesEXT vkCmdWriteMicromapsPropertiesEXT;
    PFN_vkGetDeviceMicromapCompatibilityEXT vkGetDeviceMicromapCompatibilityEXT;
    PFN_vkGetMicromapBuildSizesEXT vkGetMicromapBuildSizesEXT;
#endif // defined(VK_EXT_opacity_micromap) && (VK_EXT_OPACITY_MICROMAP_SPEC_VERSION>=2)
#if defined(VK_HUAWEI_cluster_culling_shader) && (VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION>=3)
    PFN_vkCmdDrawClusterHUAWEI vkCmdDrawClusterHUAWEI;
    PFN_vkCmdDrawClusterIndirectHUAWEI vkCmdDrawClusterIndirectHUAWEI;
#endif // defined(VK_HUAWEI_cluster_culling_shader) && (VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION>=3)
#if defined(VK_EXT_pageable_device_local_memory)
    PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT;
#endif // defined(VK_EXT_pageable_device_local_memory)
#if defined(VK_KHR_maintenance4)
    PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR;
    PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR;
    PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR;
#endif // defined(VK_KHR_maintenance4)
#if defined(VK_ARM_scheduling_controls) && (VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION>=2)
    PFN_vkCmdSetDispatchParametersARM vkCmdSetDispatchParametersARM;
#endif // defined(VK_ARM_scheduling_controls) && (VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION>=2)
#if defined(VK_VALVE_descriptor_set_host_mapping)
    PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE;
    PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE;
#endif // defined(VK_VALVE_descriptor_set_host_mapping)
#if defined(VK_NV_copy_memory_indirect)
    PFN_vkCmdCopyMemoryIndirectNV vkCmdCopyMemoryIndirectNV;
    PFN_vkCmdCopyMemoryToImageIndirectNV vkCmdCopyMemoryToImageIndirectNV;
#endif // defined(VK_NV_copy_memory_indirect)
#if defined(VK_NV_memory_decompression)
    PFN_vkCmdDecompressMemoryNV vkCmdDecompressMemoryNV;
    PFN_vkCmdDecompressMemoryIndirectCountNV vkCmdDecompressMemoryIndirectCountNV;
#endif // defined(VK_NV_memory_decompression)
#if defined(VK_NV_device_generated_commands_compute) && (VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION>=2)
    PFN_vkGetPipelineIndirectMemoryRequirementsNV vkGetPipelineIndirectMemoryRequirementsNV;
    PFN_vkCmdUpdatePipelineIndirectBufferNV vkCmdUpdatePipelineIndirectBufferNV;
    PFN_vkGetPipelineIndirectDeviceAddressNV vkGetPipelineIndirectDeviceAddressNV;
#endif // defined(VK_NV_device_generated_commands_compute) && (VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION>=2)
#if defined(VK_OHOS_external_memory)
    PFN_vkGetNativeBufferPropertiesOHOS vkGetNativeBufferPropertiesOHOS;
    PFN_vkGetMemoryNativeBufferOHOS vkGetMemoryNativeBufferOHOS;
#endif // defined(VK_OHOS_external_memory)
#if (defined(VK_EXT_extended_dynamic_state3) && (VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION>=2)) || (defined(VK_EXT_shader_object))
    PFN_vkCmdSetDepthClampEnableEXT vkCmdSetDepthClampEnableEXT;
    PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT;
    PFN_vkCmdSetRasterizationSamplesEXT vkCmdSetRasterizationSamplesEXT;
    PFN_vkCmdSetSampleMaskEXT vkCmdSetSampleMaskEXT;
    PFN_vkCmdSetAlphaToCoverageEnableEXT vkCmdSetAlphaToCoverageEnableEXT;
    PFN_vkCmdSetAlphaToOneEnableEXT vkCmdSetAlphaToOneEnableEXT;
    PFN_vkCmdSetLogicOpEnableEXT vkCmdSetLogicOpEnableEXT;
    PFN_vkCmdSetColorBlendEnableEXT vkCmdSetColorBlendEnableEXT;
    PFN_vkCmdSetColorBlendEquationEXT vkCmdSetColorBlendEquationEXT;
    PFN_vkCmdSetColorWriteMaskEXT vkCmdSetColorWriteMaskEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && (VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION>=2)) || (defined(VK_EXT_shader_object))
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
    PFN_vkCmdSetTessellationDomainOriginEXT vkCmdSetTessellationDomainOriginEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
    PFN_vkCmdSetRasterizationStreamEXT vkCmdSetRasterizationStreamEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
    PFN_vkCmdSetConservativeRasterizationModeEXT vkCmdSetConservativeRasterizationModeEXT;
    PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT vkCmdSetExtraPrimitiveOverestimationSizeEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
    PFN_vkCmdSetDepthClipEnableEXT vkCmdSetDepthClipEnableEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
    PFN_vkCmdSetSampleLocationsEnableEXT vkCmdSetSampleLocationsEnableEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
    PFN_vkCmdSetColorBlendAdvancedEXT vkCmdSetColorBlendAdvancedEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
    PFN_vkCmdSetProvokingVertexModeEXT vkCmdSetProvokingVertexModeEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
    PFN_vkCmdSetLineRasterizationModeEXT vkCmdSetLineRasterizationModeEXT;
    PFN_vkCmdSetLineStippleEnableEXT vkCmdSetLineStippleEnableEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
    PFN_vkCmdSetDepthClipNegativeOneToOneEXT vkCmdSetDepthClipNegativeOneToOneEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
    PFN_vkCmdSetViewportWScalingEnableNV vkCmdSetViewportWScalingEnableNV;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
    PFN_vkCmdSetViewportSwizzleNV vkCmdSetViewportSwizzleNV;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
    PFN_vkCmdSetCoverageToColorEnableNV vkCmdSetCoverageToColorEnableNV;
    PFN_vkCmdSetCoverageToColorLocationNV vkCmdSetCoverageToColorLocationNV;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
    PFN_vkCmdSetCoverageModulationModeNV vkCmdSetCoverageModulationModeNV;
    PFN_vkCmdSetCoverageModulationTableEnableNV vkCmdSetCoverageModulationTableEnableNV;
    PFN_vkCmdSetCoverageModulationTableNV vkCmdSetCoverageModulationTableNV;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
    PFN_vkCmdSetShadingRateImageEnableNV vkCmdSetShadingRateImageEnableNV;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
    PFN_vkCmdSetRepresentativeFragmentTestEnableNV vkCmdSetRepresentativeFragmentTestEnableNV;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
    PFN_vkCmdSetCoverageReductionModeNV vkCmdSetCoverageReductionModeNV;
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
#if defined(VK_ARM_tensors) && (VK_ARM_TENSORS_SPEC_VERSION>=2)
    PFN_vkCreateTensorARM vkCreateTensorARM;
    PFN_vkDestroyTensorARM vkDestroyTensorARM;
    PFN_vkCreateTensorViewARM vkCreateTensorViewARM;
    PFN_vkDestroyTensorViewARM vkDestroyTensorViewARM;
    PFN_vkGetTensorMemoryRequirementsARM vkGetTensorMemoryRequirementsARM;
    PFN_vkBindTensorMemoryARM vkBindTensorMemoryARM;
    PFN_vkGetDeviceTensorMemoryRequirementsARM vkGetDeviceTensorMemoryRequirementsARM;
    PFN_vkCmdCopyTensorARM vkCmdCopyTensorARM;
#endif // defined(VK_ARM_tensors) && (VK_ARM_TENSORS_SPEC_VERSION>=2)
#if defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer)
    PFN_vkGetTensorOpaqueCaptureDescriptorDataARM vkGetTensorOpaqueCaptureDescriptorDataARM;
    PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM vkGetTensorViewOpaqueCaptureDescriptorDataARM;
#endif // defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer)
#if defined(VK_EXT_shader_module_identifier)
    PFN_vkGetShaderModuleIdentifierEXT vkGetShaderModuleIdentifierEXT;
    PFN_vkGetShaderModuleCreateInfoIdentifierEXT vkGetShaderModuleCreateInfoIdentifierEXT;
#endif // defined(VK_EXT_shader_module_identifier)
#if defined(VK_NV_optical_flow)
    PFN_vkCreateOpticalFlowSessionNV vkCreateOpticalFlowSessionNV;
    PFN_vkDestroyOpticalFlowSessionNV vkDestroyOpticalFlowSessionNV;
    PFN_vkBindOpticalFlowSessionImageNV vkBindOpticalFlowSessionImageNV;
    PFN_vkCmdOpticalFlowExecuteNV vkCmdOpticalFlowExecuteNV;
#endif // defined(VK_NV_optical_flow)
#if defined(VK_KHR_maintenance5)
    PFN_vkCmdBindIndexBuffer2KHR vkCmdBindIndexBuffer2KHR;
    PFN_vkGetRenderingAreaGranularityKHR vkGetRenderingAreaGranularityKHR;
    PFN_vkGetDeviceImageSubresourceLayoutKHR vkGetDeviceImageSubresourceLayoutKHR;
    PFN_vkGetImageSubresourceLayout2KHR vkGetImageSubresourceLayout2KHR;
#endif // defined(VK_KHR_maintenance5)
#if defined(VK_AMD_anti_lag)
    PFN_vkAntiLagUpdateAMD vkAntiLagUpdateAMD;
#endif // defined(VK_AMD_anti_lag)
#if defined(VK_KHR_present_wait2)
    PFN_vkWaitForPresent2KHR vkWaitForPresent2KHR;
#endif // defined(VK_KHR_present_wait2)
#if defined(VK_EXT_shader_object)
    PFN_vkCreateShadersEXT vkCreateShadersEXT;
    PFN_vkDestroyShaderEXT vkDestroyShaderEXT;
    PFN_vkGetShaderBinaryDataEXT vkGetShaderBinaryDataEXT;
    PFN_vkCmdBindShadersEXT vkCmdBindShadersEXT;
#endif // defined(VK_EXT_shader_object)
#if (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_depth_clamp_control))
    PFN_vkCmdSetDepthClampRangeEXT vkCmdSetDepthClampRangeEXT;
#endif // (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_depth_clamp_control))
#if defined(VK_KHR_pipeline_binary)
    PFN_vkCreatePipelineBinariesKHR vkCreatePipelineBinariesKHR;
    PFN_vkDestroyPipelineBinaryKHR vkDestroyPipelineBinaryKHR;
    PFN_vkGetPipelineKeyKHR vkGetPipelineKeyKHR;
    PFN_vkGetPipelineBinaryDataKHR vkGetPipelineBinaryDataKHR;
    PFN_vkReleaseCapturedPipelineDataKHR vkReleaseCapturedPipelineDataKHR;
#endif // defined(VK_KHR_pipeline_binary)
#if defined(VK_QCOM_tile_properties)
    PFN_vkGetFramebufferTilePropertiesQCOM vkGetFramebufferTilePropertiesQCOM;
    PFN_vkGetDynamicRenderingTilePropertiesQCOM vkGetDynamicRenderingTilePropertiesQCOM;
#endif // defined(VK_QCOM_tile_properties)
#if defined(VK_KHR_swapchain_maintenance1)
    PFN_vkReleaseSwapchainImagesKHR vkReleaseSwapchainImagesKHR;
#endif // defined(VK_KHR_swapchain_maintenance1)
#if defined(VK_NV_cooperative_vector) && (VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION>=4)
    PFN_vkConvertCooperativeVectorMatrixNV vkConvertCooperativeVectorMatrixNV;
    PFN_vkCmdConvertCooperativeVectorMatrixNV vkCmdConvertCooperativeVectorMatrixNV;
#endif // defined(VK_NV_cooperative_vector) && (VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION>=4)
#if defined(VK_NV_low_latency2) && (VK_NV_LOW_LATENCY_2_SPEC_VERSION>=2)
    PFN_vkSetLatencySleepModeNV vkSetLatencySleepModeNV;
    PFN_vkLatencySleepNV vkLatencySleepNV;
    PFN_vkSetLatencyMarkerNV vkSetLatencyMarkerNV;
    PFN_vkGetLatencyTimingsNV vkGetLatencyTimingsNV;
    PFN_vkQueueNotifyOutOfBandNV vkQueueNotifyOutOfBandNV;
#endif // defined(VK_NV_low_latency2) && (VK_NV_LOW_LATENCY_2_SPEC_VERSION>=2)
#if defined(VK_ARM_data_graph)
    PFN_vkCreateDataGraphPipelinesARM vkCreateDataGraphPipelinesARM;
    PFN_vkCreateDataGraphPipelineSessionARM vkCreateDataGraphPipelineSessionARM;
    PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM vkGetDataGraphPipelineSessionBindPointRequirementsARM;
    PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM vkGetDataGraphPipelineSessionMemoryRequirementsARM;
    PFN_vkBindDataGraphPipelineSessionMemoryARM vkBindDataGraphPipelineSessionMemoryARM;
    PFN_vkDestroyDataGraphPipelineSessionARM vkDestroyDataGraphPipelineSessionARM;
    PFN_vkCmdDispatchDataGraphARM vkCmdDispatchDataGraphARM;
    PFN_vkGetDataGraphPipelineAvailablePropertiesARM vkGetDataGraphPipelineAvailablePropertiesARM;
    PFN_vkGetDataGraphPipelinePropertiesARM vkGetDataGraphPipelinePropertiesARM;
#endif // defined(VK_ARM_data_graph)
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
    PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT vkCmdSetAttachmentFeedbackLoopEnableEXT;
#endif // defined(VK_EXT_attachment_feedback_loop_dynamic_state)
#if defined(VK_QNX_external_memory_screen_buffer)
    PFN_vkGetScreenBufferPropertiesQNX vkGetScreenBufferPropertiesQNX;
#endif // defined(VK_QNX_external_memory_screen_buffer)
#if defined(VK_KHR_line_rasterization)
    PFN_vkCmdSetLineStippleKHR vkCmdSetLineStippleKHR;
#endif // defined(VK_KHR_line_rasterization)
#if defined(VK_KHR_calibrated_timestamps)
    PFN_vkGetCalibratedTimestampsKHR vkGetCalibratedTimestampsKHR;
#endif // defined(VK_KHR_calibrated_timestamps)
#if defined(VK_KHR_maintenance6)
    PFN_vkCmdBindDescriptorSets2KHR vkCmdBindDescriptorSets2KHR;
    PFN_vkCmdPushConstants2KHR vkCmdPushConstants2KHR;
#endif // defined(VK_KHR_maintenance6)
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
    PFN_vkCmdPushDescriptorSet2KHR vkCmdPushDescriptorSet2KHR;
    PFN_vkCmdPushDescriptorSetWithTemplate2KHR vkCmdPushDescriptorSetWithTemplate2KHR;
#endif // defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
    PFN_vkCmdSetDescriptorBufferOffsets2EXT vkCmdSetDescriptorBufferOffsets2EXT;
    PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT vkCmdBindDescriptorBufferEmbeddedSamplers2EXT;
#endif // defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
#if defined(VK_QCOM_tile_memory_heap)
    PFN_vkCmdBindTileMemoryQCOM vkCmdBindTileMemoryQCOM;
#endif // defined(VK_QCOM_tile_memory_heap)
#if defined(VK_KHR_copy_memory_indirect)
    PFN_vkCmdCopyMemoryIndirectKHR vkCmdCopyMemoryIndirectKHR;
    PFN_vkCmdCopyMemoryToImageIndirectKHR vkCmdCopyMemoryToImageIndirectKHR;
#endif // defined(VK_KHR_copy_memory_indirect)
#if defined(VK_EXT_memory_decompression)
    PFN_vkCmdDecompressMemoryEXT vkCmdDecompressMemoryEXT;
    PFN_vkCmdDecompressMemoryIndirectCountEXT vkCmdDecompressMemoryIndirectCountEXT;
#endif // defined(VK_EXT_memory_decompression)
#if defined(VK_NV_external_compute_queue)
    PFN_vkCreateExternalComputeQueueNV vkCreateExternalComputeQueueNV;
    PFN_vkDestroyExternalComputeQueueNV vkDestroyExternalComputeQueueNV;
    PFN_vkGetExternalComputeQueueDataNV vkGetExternalComputeQueueDataNV;
#endif // defined(VK_NV_external_compute_queue)
#if defined(VK_NV_cluster_acceleration_structure) && (VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION>=4)
    PFN_vkGetClusterAccelerationStructureBuildSizesNV vkGetClusterAccelerationStructureBuildSizesNV;
    PFN_vkCmdBuildClusterAccelerationStructureIndirectNV vkCmdBuildClusterAccelerationStructureIndirectNV;
#endif // defined(VK_NV_cluster_acceleration_structure) && (VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION>=4)
#if defined(VK_NV_partitioned_acceleration_structure)
    PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV vkGetPartitionedAccelerationStructuresBuildSizesNV;
    PFN_vkCmdBuildPartitionedAccelerationStructuresNV vkCmdBuildPartitionedAccelerationStructuresNV;
#endif // defined(VK_NV_partitioned_acceleration_structure)
#if defined(VK_EXT_device_generated_commands)
    PFN_vkGetGeneratedCommandsMemoryRequirementsEXT vkGetGeneratedCommandsMemoryRequirementsEXT;
    PFN_vkCmdPreprocessGeneratedCommandsEXT vkCmdPreprocessGeneratedCommandsEXT;
    PFN_vkCmdExecuteGeneratedCommandsEXT vkCmdExecuteGeneratedCommandsEXT;
    PFN_vkCreateIndirectCommandsLayoutEXT vkCreateIndirectCommandsLayoutEXT;
    PFN_vkDestroyIndirectCommandsLayoutEXT vkDestroyIndirectCommandsLayoutEXT;
    PFN_vkCreateIndirectExecutionSetEXT vkCreateIndirectExecutionSetEXT;
    PFN_vkDestroyIndirectExecutionSetEXT vkDestroyIndirectExecutionSetEXT;
    PFN_vkUpdateIndirectExecutionSetPipelineEXT vkUpdateIndirectExecutionSetPipelineEXT;
    PFN_vkUpdateIndirectExecutionSetShaderEXT vkUpdateIndirectExecutionSetShaderEXT;
#endif // defined(VK_EXT_device_generated_commands)
#if defined(VK_KHR_device_fault)
    PFN_vkGetDeviceFaultReportsKHR vkGetDeviceFaultReportsKHR;
    PFN_vkGetDeviceFaultDebugInfoKHR vkGetDeviceFaultDebugInfoKHR;
#endif // defined(VK_KHR_device_fault)
#if defined(VK_EXT_external_memory_metal)
    PFN_vkGetMemoryMetalHandleEXT vkGetMemoryMetalHandleEXT;
    PFN_vkGetMemoryMetalHandlePropertiesEXT vkGetMemoryMetalHandlePropertiesEXT;
#endif // defined(VK_EXT_external_memory_metal)
#if defined(VK_ARM_shader_instrumentation)
    PFN_vkCreateShaderInstrumentationARM vkCreateShaderInstrumentationARM;
    PFN_vkDestroyShaderInstrumentationARM vkDestroyShaderInstrumentationARM;
    PFN_vkCmdBeginShaderInstrumentationARM vkCmdBeginShaderInstrumentationARM;
    PFN_vkCmdEndShaderInstrumentationARM vkCmdEndShaderInstrumentationARM;
    PFN_vkGetShaderInstrumentationValuesARM vkGetShaderInstrumentationValuesARM;
    PFN_vkClearShaderInstrumentationMetricsARM vkClearShaderInstrumentationMetricsARM;
#endif // defined(VK_ARM_shader_instrumentation)
#if defined(VK_EXT_fragment_density_map_offset)
    PFN_vkCmdEndRendering2EXT vkCmdEndRendering2EXT;
#endif // defined(VK_EXT_fragment_density_map_offset)
#if defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))
    PFN_vkCmdBeginCustomResolveEXT vkCmdBeginCustomResolveEXT;
#endif // defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))
#if defined(VK_KHR_maintenance10)
    PFN_vkCmdEndRendering2KHR vkCmdEndRendering2KHR;
#endif // defined(VK_KHR_maintenance10)
#if defined(VK_NV_compute_occupancy_priority)
    PFN_vkCmdSetComputeOccupancyPriorityNV vkCmdSetComputeOccupancyPriorityNV;
#endif // defined(VK_NV_compute_occupancy_priority)
#if defined(VK_EXT_primitive_restart_index)
    PFN_vkCmdSetPrimitiveRestartIndexEXT vkCmdSetPrimitiveRestartIndexEXT;
#endif // defined(VK_EXT_primitive_restart_index)
//DOK_END_DEVICE
};