#pragma once

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct DokGlobalTable {
#pragma region    PFN_{Global} {Global};
#if defined(VK_VERSION_1_0) 
    PFN_vkCreateInstance vkCreateInstance;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
    PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
#endif // defined(VK_VERSION_1_0) 
#if defined(VK_VERSION_1_1) 
    PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
#endif // defined(VK_VERSION_1_1) 
#pragma endregion
} DokGlobalTable;

typedef struct DokInstanceTable {
    VkInstance instance;
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
#pragma region    PFN_{Instance} {Instance};
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
#if defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1) 
    PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR;
#endif // defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1) 
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
#if defined(VK_KHR_video_queue) 
    PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR;
#endif // defined(VK_KHR_video_queue) 
#if defined(VK_GGP_stream_descriptor_surface) 
    PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP;
#endif // defined(VK_GGP_stream_descriptor_surface) 
#if defined(VK_NN_vi_surface) 
    PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#endif // defined(VK_NN_vi_surface) 
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
#if defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
    PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
    PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
#endif // defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
#if defined(VK_EXT_descriptor_heap) 
    PFN_vkGetPhysicalDeviceDescriptorSizeEXT vkGetPhysicalDeviceDescriptorSizeEXT;
#endif // defined(VK_EXT_descriptor_heap) 
#if defined(VK_EXT_sample_locations) 
    PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
#endif // defined(VK_EXT_sample_locations) 
#if defined(VK_FUCHSIA_imagepipe_surface) 
    PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA;
#endif // defined(VK_FUCHSIA_imagepipe_surface) 
#if defined(VK_EXT_metal_surface) 
    PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT;
#endif // defined(VK_EXT_metal_surface) 
#if defined(VK_KHR_fragment_shading_rate) 
    PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR;
#endif // defined(VK_KHR_fragment_shading_rate) 
#if defined(VK_NV_cooperative_matrix) 
    PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
#endif // defined(VK_NV_cooperative_matrix) 
#if defined(VK_NV_coverage_reduction_mode) 
    PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
#endif // defined(VK_NV_coverage_reduction_mode) 
#if defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
    PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
#endif // defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
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
#if defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
    PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM vkGetPhysicalDeviceExternalTensorPropertiesARM;
#endif // defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
#if defined(VK_NV_optical_flow) 
    PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV vkGetPhysicalDeviceOpticalFlowImageFormatsNV;
#endif // defined(VK_NV_optical_flow) 
#if defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
    PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV vkGetPhysicalDeviceCooperativeVectorPropertiesNV;
#endif // defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
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
#if defined(VK_EXT_cooperative_matrix_maintenance1) 
    PFN_vkGetPhysicalDeviceCooperativeMatrixProperties2EXT vkGetPhysicalDeviceCooperativeMatrixProperties2EXT;
#endif // defined(VK_EXT_cooperative_matrix_maintenance1) 
#if defined(VK_SEC_ubm_surface) 
    PFN_vkCreateUbmSurfaceSEC vkCreateUbmSurfaceSEC;
    PFN_vkGetPhysicalDeviceUbmPresentationSupportSEC vkGetPhysicalDeviceUbmPresentationSupportSEC;
#endif // defined(VK_SEC_ubm_surface) 
#pragma endregion
} DokInstanceTable;

typedef struct DokDeviceTable {
    VkDevice device;
#pragma region    PFN_{Device} {Device};
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
#if defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1) 
    PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
    PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
    PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif // defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1) 
#if defined(VK_KHR_display_swapchain) 
    PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif // defined(VK_KHR_display_swapchain) 
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
#if defined(VK_NVX_binary_import) && VK_NVX_BINARY_IMPORT_SPEC_VERSION >= 2 
    PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX;
    PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX;
    PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX;
    PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX;
    PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX;
#endif // defined(VK_NVX_binary_import) && VK_NVX_BINARY_IMPORT_SPEC_VERSION >= 2 
#if defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 4 
    PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX;
    PFN_vkGetImageViewHandle64NVX vkGetImageViewHandle64NVX;
    PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX;
    PFN_vkGetDeviceCombinedImageSamplerIndexNVX vkGetDeviceCombinedImageSamplerIndexNVX;
#endif // defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 4 
#if defined(VK_AMD_shader_info) 
    PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif // defined(VK_AMD_shader_info) 
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
#if defined(VK_EXT_conditional_rendering) && VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION >= 2 
    PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
    PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif // defined(VK_EXT_conditional_rendering) && VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION >= 2 
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
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 
    PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
    PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT;
    PFN_vkCmdSetDiscardRectangleModeEXT vkCmdSetDiscardRectangleModeEXT;
#endif // defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 
#if defined(VK_EXT_hdr_metadata) && VK_EXT_HDR_METADATA_SPEC_VERSION >= 3 
    PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif // defined(VK_EXT_hdr_metadata) && VK_EXT_HDR_METADATA_SPEC_VERSION >= 3 
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
#if defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
    PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
    PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
    PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
    PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
    PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
    PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
    PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
    PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
#endif // defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
#if defined(VK_ANDROID_external_memory_android_hardware_buffer) && VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION >= 5 
    PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
    PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif // defined(VK_ANDROID_external_memory_android_hardware_buffer) && VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION >= 5 
#if defined(VK_AMD_gpa_interface) 
    PFN_vkCreateGpaSessionAMD vkCreateGpaSessionAMD;
    PFN_vkDestroyGpaSessionAMD vkDestroyGpaSessionAMD;
    PFN_vkSetGpaDeviceClockModeAMD vkSetGpaDeviceClockModeAMD;
    PFN_vkGetGpaDeviceClockInfoAMD vkGetGpaDeviceClockInfoAMD;
    PFN_vkCmdBeginGpaSessionAMD vkCmdBeginGpaSessionAMD;
    PFN_vkCmdEndGpaSessionAMD vkCmdEndGpaSessionAMD;
    PFN_vkCmdBeginGpaSampleAMD vkCmdBeginGpaSampleAMD;
    PFN_vkCmdEndGpaSampleAMD vkCmdEndGpaSampleAMD;
    PFN_vkGetGpaSessionStatusAMD vkGetGpaSessionStatusAMD;
    PFN_vkGetGpaSessionResultsAMD vkGetGpaSessionResultsAMD;
    PFN_vkResetGpaSessionAMD vkResetGpaSessionAMD;
    PFN_vkCmdCopyGpaSessionResultsAMD vkCmdCopyGpaSessionResultsAMD;
#endif // defined(VK_AMD_gpa_interface) 
#if defined(VK_AMDX_shader_enqueue) && VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION >= 2 
    PFN_vkCreateExecutionGraphPipelinesAMDX vkCreateExecutionGraphPipelinesAMDX;
    PFN_vkGetExecutionGraphPipelineScratchSizeAMDX vkGetExecutionGraphPipelineScratchSizeAMDX;
    PFN_vkGetExecutionGraphPipelineNodeIndexAMDX vkGetExecutionGraphPipelineNodeIndexAMDX;
    PFN_vkCmdInitializeGraphScratchMemoryAMDX vkCmdInitializeGraphScratchMemoryAMDX;
    PFN_vkCmdDispatchGraphAMDX vkCmdDispatchGraphAMDX;
    PFN_vkCmdDispatchGraphIndirectAMDX vkCmdDispatchGraphIndirectAMDX;
    PFN_vkCmdDispatchGraphIndirectCountAMDX vkCmdDispatchGraphIndirectCountAMDX;
#endif // defined(VK_AMDX_shader_enqueue) && VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION >= 2 
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
#if defined(VK_EXT_image_drm_format_modifier) && VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION >= 2 
    PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT;
#endif // defined(VK_EXT_image_drm_format_modifier) && VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION >= 2 
#if defined(VK_EXT_validation_cache) 
    PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
    PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
    PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
    PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
#endif // defined(VK_EXT_validation_cache) 
#if defined(VK_NV_shading_rate_image) && VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION >= 3 
    PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV;
    PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV;
    PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV;
#endif // defined(VK_NV_shading_rate_image) && VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION >= 3 
#if defined(VK_EXT_external_memory_host) 
    PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif // defined(VK_EXT_external_memory_host) 
#if defined(VK_AMD_buffer_marker) 
    PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif // defined(VK_AMD_buffer_marker) 
#if defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
    PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD;
#endif // defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
#if defined(VK_NV_mesh_shader) 
    PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV;
    PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV;
#endif // defined(VK_NV_mesh_shader) 
#if defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) 
    PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV;
#endif // defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) 
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 
    PFN_vkCmdSetExclusiveScissorEnableNV vkCmdSetExclusiveScissorEnableNV;
    PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV;
#endif // defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 
#if defined(VK_NV_device_diagnostic_checkpoints) && VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION >= 2 
    PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
    PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif // defined(VK_NV_device_diagnostic_checkpoints) && VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION >= 2 
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
    PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV;
#endif // defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
#if defined(VK_EXT_present_timing) && VK_EXT_PRESENT_TIMING_SPEC_VERSION >= 3 
    PFN_vkSetSwapchainPresentTimingQueueSizeEXT vkSetSwapchainPresentTimingQueueSizeEXT;
    PFN_vkGetSwapchainTimingPropertiesEXT vkGetSwapchainTimingPropertiesEXT;
    PFN_vkGetSwapchainTimeDomainPropertiesEXT vkGetSwapchainTimeDomainPropertiesEXT;
    PFN_vkGetPastPresentationTimingEXT vkGetPastPresentationTimingEXT;
#endif // defined(VK_EXT_present_timing) && VK_EXT_PRESENT_TIMING_SPEC_VERSION >= 3 
#if defined(VK_INTEL_performance_query) && VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION >= 2 
    PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL;
    PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL;
    PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL;
    PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL;
    PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL;
    PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL;
    PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL;
    PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL;
    PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL;
#endif // defined(VK_INTEL_performance_query) && VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION >= 2 
#if defined(VK_AMD_display_native_hdr) 
    PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD;
#endif // defined(VK_AMD_display_native_hdr) 
#if defined(VK_KHR_fragment_shading_rate) 
    PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR;
#endif // defined(VK_KHR_fragment_shading_rate) 
#if defined(VK_KHR_present_wait) 
    PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#endif // defined(VK_KHR_present_wait) 
#if defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
    PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
    PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif // defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) 
    PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
#endif // defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) 
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
#if defined(VK_NV_device_generated_commands) && VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION >= 3 
    PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV;
    PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV;
    PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV;
    PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV;
    PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV;
    PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV;
#endif // defined(VK_NV_device_generated_commands) && VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION >= 3 
#if defined(VK_EXT_depth_bias_control) 
    PFN_vkCmdSetDepthBias2EXT vkCmdSetDepthBias2EXT;
#endif // defined(VK_EXT_depth_bias_control) 
#if defined(VK_KHR_video_encode_queue) 
    PFN_vkGetEncodedVideoSessionParametersKHR vkGetEncodedVideoSessionParametersKHR;
    PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR;
#endif // defined(VK_KHR_video_encode_queue) 
#if defined(VK_QCOM_queue_perf_hint) 
    PFN_vkQueueSetPerfHintQCOM vkQueueSetPerfHintQCOM;
#endif // defined(VK_QCOM_queue_perf_hint) 
#if defined(VK_NV_cuda_kernel_launch) && VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION >= 2 
    PFN_vkCreateCudaModuleNV vkCreateCudaModuleNV;
    PFN_vkGetCudaModuleCacheNV vkGetCudaModuleCacheNV;
    PFN_vkCreateCudaFunctionNV vkCreateCudaFunctionNV;
    PFN_vkDestroyCudaModuleNV vkDestroyCudaModuleNV;
    PFN_vkDestroyCudaFunctionNV vkDestroyCudaFunctionNV;
    PFN_vkCmdCudaLaunchKernelNV vkCmdCudaLaunchKernelNV;
#endif // defined(VK_NV_cuda_kernel_launch) && VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION >= 2 
#if defined(VK_QCOM_tile_shading) && VK_QCOM_TILE_SHADING_SPEC_VERSION >= 2 
    PFN_vkCmdDispatchTileQCOM vkCmdDispatchTileQCOM;
    PFN_vkCmdBeginPerTileExecutionQCOM vkCmdBeginPerTileExecutionQCOM;
    PFN_vkCmdEndPerTileExecutionQCOM vkCmdEndPerTileExecutionQCOM;
#endif // defined(VK_QCOM_tile_shading) && VK_QCOM_TILE_SHADING_SPEC_VERSION >= 2 
#if defined(VK_EXT_metal_objects) && VK_EXT_METAL_OBJECTS_SPEC_VERSION >= 2 
    PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT;
#endif // defined(VK_EXT_metal_objects) && VK_EXT_METAL_OBJECTS_SPEC_VERSION >= 2 
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
#if (defined(VK_EXT_vertex_input_dynamic_state) && VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
    PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT;
#endif // (defined(VK_EXT_vertex_input_dynamic_state) && VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
#if defined(VK_FUCHSIA_external_memory) 
    PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA;
    PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA;
#endif // defined(VK_FUCHSIA_external_memory) 
#if defined(VK_FUCHSIA_external_semaphore) 
    PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA;
    PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA;
#endif // defined(VK_FUCHSIA_external_semaphore) 
#if defined(VK_FUCHSIA_buffer_collection) && VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION >= 2 
    PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA;
    PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA;
    PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA;
    PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA;
    PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA;
#endif // defined(VK_FUCHSIA_buffer_collection) && VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION >= 2 
#if defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 3 
    PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
    PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI;
#endif // defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 3 
#if defined(VK_HUAWEI_invocation_mask) 
    PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI;
#endif // defined(VK_HUAWEI_invocation_mask) 
#if defined(VK_NV_external_memory_rdma) 
    PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV;
#endif // defined(VK_NV_external_memory_rdma) 
#if defined(VK_EXT_pipeline_properties) 
    PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT;
#endif // defined(VK_EXT_pipeline_properties) 
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
#if defined(VK_HUAWEI_cluster_culling_shader) && VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION >= 3 
    PFN_vkCmdDrawClusterHUAWEI vkCmdDrawClusterHUAWEI;
    PFN_vkCmdDrawClusterIndirectHUAWEI vkCmdDrawClusterIndirectHUAWEI;
#endif // defined(VK_HUAWEI_cluster_culling_shader) && VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION >= 3 
#if defined(VK_EXT_pageable_device_local_memory) 
    PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT;
#endif // defined(VK_EXT_pageable_device_local_memory) 
#if defined(VK_ARM_scheduling_controls) && VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION >= 2 
    PFN_vkCmdSetDispatchParametersARM vkCmdSetDispatchParametersARM;
#endif // defined(VK_ARM_scheduling_controls) && VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION >= 2 
#if defined(VK_VALVE_descriptor_set_host_mapping) 
    PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE;
    PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE;
#endif // defined(VK_VALVE_descriptor_set_host_mapping) 
#if defined(VK_NV_device_generated_commands_compute) && VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION >= 2 
    PFN_vkGetPipelineIndirectMemoryRequirementsNV vkGetPipelineIndirectMemoryRequirementsNV;
    PFN_vkCmdUpdatePipelineIndirectBufferNV vkCmdUpdatePipelineIndirectBufferNV;
    PFN_vkGetPipelineIndirectDeviceAddressNV vkGetPipelineIndirectDeviceAddressNV;
#endif // defined(VK_NV_device_generated_commands_compute) && VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION >= 2 
#if defined(VK_OHOS_external_memory) 
    PFN_vkGetNativeBufferPropertiesOHOS vkGetNativeBufferPropertiesOHOS;
    PFN_vkGetMemoryNativeBufferOHOS vkGetMemoryNativeBufferOHOS;
#endif // defined(VK_OHOS_external_memory) 
#if (defined(VK_EXT_extended_dynamic_state3) && VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
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
#endif // (defined(VK_EXT_extended_dynamic_state3) && VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
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
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) || (defined(VK_EXT_shader_object) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) 
    PFN_vkCmdSetLineRasterizationModeEXT vkCmdSetLineRasterizationModeEXT;
    PFN_vkCmdSetLineStippleEnableEXT vkCmdSetLineStippleEnableEXT;
#endif // (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) || (defined(VK_EXT_shader_object) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) 
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
#if defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
    PFN_vkCreateTensorARM vkCreateTensorARM;
    PFN_vkDestroyTensorARM vkDestroyTensorARM;
    PFN_vkCreateTensorViewARM vkCreateTensorViewARM;
    PFN_vkDestroyTensorViewARM vkDestroyTensorViewARM;
    PFN_vkGetTensorMemoryRequirementsARM vkGetTensorMemoryRequirementsARM;
    PFN_vkBindTensorMemoryARM vkBindTensorMemoryARM;
    PFN_vkGetDeviceTensorMemoryRequirementsARM vkGetDeviceTensorMemoryRequirementsARM;
    PFN_vkCmdCopyTensorARM vkCmdCopyTensorARM;
#endif // defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
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
    PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT;
    PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT;
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
#if defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
    PFN_vkConvertCooperativeVectorMatrixNV vkConvertCooperativeVectorMatrixNV;
    PFN_vkCmdConvertCooperativeVectorMatrixNV vkCmdConvertCooperativeVectorMatrixNV;
#endif // defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
#if defined(VK_NV_low_latency2) && VK_NV_LOW_LATENCY_2_SPEC_VERSION >= 3 
    PFN_vkSetLatencySleepModeNV vkSetLatencySleepModeNV;
    PFN_vkLatencySleepNV vkLatencySleepNV;
    PFN_vkSetLatencyMarkerNV vkSetLatencyMarkerNV;
    PFN_vkGetLatencyTimingsNV vkGetLatencyTimingsNV;
    PFN_vkQueueNotifyOutOfBandNV vkQueueNotifyOutOfBandNV;
#endif // defined(VK_NV_low_latency2) && VK_NV_LOW_LATENCY_2_SPEC_VERSION >= 3 
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
#if defined(VK_KHR_calibrated_timestamps) 
    PFN_vkGetCalibratedTimestampsKHR vkGetCalibratedTimestampsKHR;
#endif // defined(VK_KHR_calibrated_timestamps) 
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
#if defined(VK_NV_cluster_acceleration_structure) && VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION >= 4 
    PFN_vkGetClusterAccelerationStructureBuildSizesNV vkGetClusterAccelerationStructureBuildSizesNV;
    PFN_vkCmdBuildClusterAccelerationStructureIndirectNV vkCmdBuildClusterAccelerationStructureIndirectNV;
#endif // defined(VK_NV_cluster_acceleration_structure) && VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION >= 4 
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
#pragma endregion
} DokDeviceTable;

void dokLoadGlobalTable(PFN_vkGetInstanceProcAddr instanceProcAddr, DokGlobalTable* pOut);

void dokLoadInstanceTable(PFN_vkGetInstanceProcAddr instanceProcAddr, VkInstance instance, DokInstanceTable* pOut);

void dokLoadDeviceTable(PFN_vkGetDeviceProcAddr deviceProcAddr, VkDevice device, DokDeviceTable* pOut);

#ifdef DOK_VULKAN_TABLES_IMPLEMENTATION

void dokLoadGlobalTable(PFN_vkGetInstanceProcAddr instanceProcAddr, DokGlobalTable* pOut) {
#pragma region    pOut->{Global} = (PFN_{Global})instanceProcAddr(NULL, "{Global}");
#if defined(VK_VERSION_1_0) 
    pOut->vkCreateInstance = (PFN_vkCreateInstance)instanceProcAddr(NULL, "vkCreateInstance");
    pOut->vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)instanceProcAddr(NULL, "vkGetInstanceProcAddr");
    pOut->vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)instanceProcAddr(NULL, "vkEnumerateInstanceExtensionProperties");
    pOut->vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)instanceProcAddr(NULL, "vkEnumerateInstanceLayerProperties");
#endif // defined(VK_VERSION_1_0) 
#if defined(VK_VERSION_1_1) 
    pOut->vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)instanceProcAddr(NULL, "vkEnumerateInstanceVersion");
#endif // defined(VK_VERSION_1_1) 
#pragma endregion
}

void dokLoadInstanceTable(PFN_vkGetInstanceProcAddr instanceProcAddr, VkInstance instance, DokInstanceTable* pOut) {
    pOut->instance = instance;
    pOut->vkGetInstanceProcAddr = instanceProcAddr;
#pragma region    pOut->{Instance} = (PFN_{Instance})instanceProcAddr(instance, "{Instance}");
#if defined(VK_VERSION_1_0) 
    pOut->vkDestroyInstance = (PFN_vkDestroyInstance)instanceProcAddr(instance, "vkDestroyInstance");
    pOut->vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)instanceProcAddr(instance, "vkEnumeratePhysicalDevices");
    pOut->vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)instanceProcAddr(instance, "vkGetPhysicalDeviceFeatures");
    pOut->vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties");
    pOut->vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties");
    pOut->vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
    pOut->vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    pOut->vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties");
    pOut->vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)instanceProcAddr(instance, "vkGetDeviceProcAddr");
    pOut->vkCreateDevice = (PFN_vkCreateDevice)instanceProcAddr(instance, "vkCreateDevice");
    pOut->vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)instanceProcAddr(instance, "vkEnumerateDeviceExtensionProperties");
    pOut->vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)instanceProcAddr(instance, "vkEnumerateDeviceLayerProperties");
    pOut->vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties");
#endif // defined(VK_VERSION_1_0) 
#if defined(VK_VERSION_1_1) 
    pOut->vkEnumeratePhysicalDeviceGroups = (PFN_vkEnumeratePhysicalDeviceGroups)instanceProcAddr(instance, "vkEnumeratePhysicalDeviceGroups");
    pOut->vkGetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2)instanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2");
    pOut->vkGetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2)instanceProcAddr(instance, "vkGetPhysicalDeviceProperties2");
    pOut->vkGetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2)instanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2");
    pOut->vkGetPhysicalDeviceImageFormatProperties2 = (PFN_vkGetPhysicalDeviceImageFormatProperties2)instanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties2");
    pOut->vkGetPhysicalDeviceQueueFamilyProperties2 = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2)instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties2");
    pOut->vkGetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2)instanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties2");
    pOut->vkGetPhysicalDeviceSparseImageFormatProperties2 = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)instanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties2");
    pOut->vkGetPhysicalDeviceExternalBufferProperties = (PFN_vkGetPhysicalDeviceExternalBufferProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceExternalBufferProperties");
    pOut->vkGetPhysicalDeviceExternalFenceProperties = (PFN_vkGetPhysicalDeviceExternalFenceProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceExternalFenceProperties");
    pOut->vkGetPhysicalDeviceExternalSemaphoreProperties = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceExternalSemaphoreProperties");
#endif // defined(VK_VERSION_1_1) 
#if defined(VK_VERSION_1_3) 
    pOut->vkGetPhysicalDeviceToolProperties = (PFN_vkGetPhysicalDeviceToolProperties)instanceProcAddr(instance, "vkGetPhysicalDeviceToolProperties");
#endif // defined(VK_VERSION_1_3) 
#if defined(VK_KHR_surface) 
    pOut->vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)instanceProcAddr(instance, "vkDestroySurfaceKHR");
    pOut->vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
    pOut->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    pOut->vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    pOut->vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
#endif // defined(VK_KHR_surface) 
#if defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1) 
    pOut->vkGetPhysicalDevicePresentRectanglesKHR = (PFN_vkGetPhysicalDevicePresentRectanglesKHR)instanceProcAddr(instance, "vkGetPhysicalDevicePresentRectanglesKHR");
#endif // defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1) 
#if defined(VK_KHR_display) 
    pOut->vkGetPhysicalDeviceDisplayPropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPropertiesKHR");
    pOut->vkGetPhysicalDeviceDisplayPlanePropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
    pOut->vkGetDisplayPlaneSupportedDisplaysKHR = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR)instanceProcAddr(instance, "vkGetDisplayPlaneSupportedDisplaysKHR");
    pOut->vkGetDisplayModePropertiesKHR = (PFN_vkGetDisplayModePropertiesKHR)instanceProcAddr(instance, "vkGetDisplayModePropertiesKHR");
    pOut->vkCreateDisplayModeKHR = (PFN_vkCreateDisplayModeKHR)instanceProcAddr(instance, "vkCreateDisplayModeKHR");
    pOut->vkGetDisplayPlaneCapabilitiesKHR = (PFN_vkGetDisplayPlaneCapabilitiesKHR)instanceProcAddr(instance, "vkGetDisplayPlaneCapabilitiesKHR");
    pOut->vkCreateDisplayPlaneSurfaceKHR = (PFN_vkCreateDisplayPlaneSurfaceKHR)instanceProcAddr(instance, "vkCreateDisplayPlaneSurfaceKHR");
#endif // defined(VK_KHR_display) 
#if defined(VK_KHR_xlib_surface) 
    pOut->vkCreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)instanceProcAddr(instance, "vkCreateXlibSurfaceKHR");
    pOut->vkGetPhysicalDeviceXlibPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif // defined(VK_KHR_xlib_surface) 
#if defined(VK_KHR_xcb_surface) 
    pOut->vkCreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)instanceProcAddr(instance, "vkCreateXcbSurfaceKHR");
    pOut->vkGetPhysicalDeviceXcbPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
#endif // defined(VK_KHR_xcb_surface) 
#if defined(VK_KHR_wayland_surface) 
    pOut->vkCreateWaylandSurfaceKHR = (PFN_vkCreateWaylandSurfaceKHR)instanceProcAddr(instance, "vkCreateWaylandSurfaceKHR");
    pOut->vkGetPhysicalDeviceWaylandPresentationSupportKHR = (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
#endif // defined(VK_KHR_wayland_surface) 
#if defined(VK_KHR_android_surface) 
    pOut->vkCreateAndroidSurfaceKHR = (PFN_vkCreateAndroidSurfaceKHR)instanceProcAddr(instance, "vkCreateAndroidSurfaceKHR");
#endif // defined(VK_KHR_android_surface) 
#if defined(VK_KHR_win32_surface) 
    pOut->vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)instanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
    pOut->vkGetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif // defined(VK_KHR_win32_surface) 
#if defined(VK_KHR_video_queue) 
    pOut->vkGetPhysicalDeviceVideoCapabilitiesKHR = (PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceVideoCapabilitiesKHR");
    pOut->vkGetPhysicalDeviceVideoFormatPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceVideoFormatPropertiesKHR");
#endif // defined(VK_KHR_video_queue) 
#if defined(VK_GGP_stream_descriptor_surface) 
    pOut->vkCreateStreamDescriptorSurfaceGGP = (PFN_vkCreateStreamDescriptorSurfaceGGP)instanceProcAddr(instance, "vkCreateStreamDescriptorSurfaceGGP");
#endif // defined(VK_GGP_stream_descriptor_surface) 
#if defined(VK_NN_vi_surface) 
    pOut->vkCreateViSurfaceNN = (PFN_vkCreateViSurfaceNN)instanceProcAddr(instance, "vkCreateViSurfaceNN");
#endif // defined(VK_NN_vi_surface) 
#if defined(VK_EXT_direct_mode_display) 
    pOut->vkReleaseDisplayEXT = (PFN_vkReleaseDisplayEXT)instanceProcAddr(instance, "vkReleaseDisplayEXT");
#endif // defined(VK_EXT_direct_mode_display) 
#if defined(VK_EXT_acquire_xlib_display) 
    pOut->vkAcquireXlibDisplayEXT = (PFN_vkAcquireXlibDisplayEXT)instanceProcAddr(instance, "vkAcquireXlibDisplayEXT");
    pOut->vkGetRandROutputDisplayEXT = (PFN_vkGetRandROutputDisplayEXT)instanceProcAddr(instance, "vkGetRandROutputDisplayEXT");
#endif // defined(VK_EXT_acquire_xlib_display) 
#if defined(VK_EXT_display_surface_counter) 
    pOut->vkGetPhysicalDeviceSurfaceCapabilities2EXT = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT)instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
#endif // defined(VK_EXT_display_surface_counter) 
#if defined(VK_KHR_performance_query) 
    pOut->vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR)instanceProcAddr(instance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR");
    pOut->vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = (PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR");
#endif // defined(VK_KHR_performance_query) 
#if defined(VK_KHR_get_surface_capabilities2) 
    pOut->vkGetPhysicalDeviceSurfaceCapabilities2KHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
    pOut->vkGetPhysicalDeviceSurfaceFormats2KHR = (PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormats2KHR");
#endif // defined(VK_KHR_get_surface_capabilities2) 
#if defined(VK_KHR_get_display_properties2) 
    pOut->vkGetPhysicalDeviceDisplayProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayProperties2KHR)instanceProcAddr(instance, "vkGetPhysicalDeviceDisplayProperties2KHR");
    pOut->vkGetPhysicalDeviceDisplayPlaneProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)instanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
    pOut->vkGetDisplayModeProperties2KHR = (PFN_vkGetDisplayModeProperties2KHR)instanceProcAddr(instance, "vkGetDisplayModeProperties2KHR");
    pOut->vkGetDisplayPlaneCapabilities2KHR = (PFN_vkGetDisplayPlaneCapabilities2KHR)instanceProcAddr(instance, "vkGetDisplayPlaneCapabilities2KHR");
#endif // defined(VK_KHR_get_display_properties2) 
#if defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
    pOut->vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)instanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    pOut->vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)instanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    pOut->vkSubmitDebugUtilsMessageEXT = (PFN_vkSubmitDebugUtilsMessageEXT)instanceProcAddr(instance, "vkSubmitDebugUtilsMessageEXT");
#endif // defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
#if defined(VK_EXT_descriptor_heap) 
    pOut->vkGetPhysicalDeviceDescriptorSizeEXT = (PFN_vkGetPhysicalDeviceDescriptorSizeEXT)instanceProcAddr(instance, "vkGetPhysicalDeviceDescriptorSizeEXT");
#endif // defined(VK_EXT_descriptor_heap) 
#if defined(VK_EXT_sample_locations) 
    pOut->vkGetPhysicalDeviceMultisamplePropertiesEXT = (PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT)instanceProcAddr(instance, "vkGetPhysicalDeviceMultisamplePropertiesEXT");
#endif // defined(VK_EXT_sample_locations) 
#if defined(VK_FUCHSIA_imagepipe_surface) 
    pOut->vkCreateImagePipeSurfaceFUCHSIA = (PFN_vkCreateImagePipeSurfaceFUCHSIA)instanceProcAddr(instance, "vkCreateImagePipeSurfaceFUCHSIA");
#endif // defined(VK_FUCHSIA_imagepipe_surface) 
#if defined(VK_EXT_metal_surface) 
    pOut->vkCreateMetalSurfaceEXT = (PFN_vkCreateMetalSurfaceEXT)instanceProcAddr(instance, "vkCreateMetalSurfaceEXT");
#endif // defined(VK_EXT_metal_surface) 
#if defined(VK_KHR_fragment_shading_rate) 
    pOut->vkGetPhysicalDeviceFragmentShadingRatesKHR = (PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceFragmentShadingRatesKHR");
#endif // defined(VK_KHR_fragment_shading_rate) 
#if defined(VK_NV_cooperative_matrix) 
    pOut->vkGetPhysicalDeviceCooperativeMatrixPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV");
#endif // defined(VK_NV_cooperative_matrix) 
#if defined(VK_NV_coverage_reduction_mode) 
    pOut->vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = (PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV)instanceProcAddr(instance, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV");
#endif // defined(VK_NV_coverage_reduction_mode) 
#if defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
    pOut->vkGetPhysicalDeviceSurfacePresentModes2EXT = (PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT)instanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModes2EXT");
#endif // defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
#if defined(VK_EXT_headless_surface) 
    pOut->vkCreateHeadlessSurfaceEXT = (PFN_vkCreateHeadlessSurfaceEXT)instanceProcAddr(instance, "vkCreateHeadlessSurfaceEXT");
#endif // defined(VK_EXT_headless_surface) 
#if defined(VK_EXT_acquire_drm_display) 
    pOut->vkAcquireDrmDisplayEXT = (PFN_vkAcquireDrmDisplayEXT)instanceProcAddr(instance, "vkAcquireDrmDisplayEXT");
    pOut->vkGetDrmDisplayEXT = (PFN_vkGetDrmDisplayEXT)instanceProcAddr(instance, "vkGetDrmDisplayEXT");
#endif // defined(VK_EXT_acquire_drm_display) 
#if defined(VK_KHR_video_encode_queue) 
    pOut->vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR");
#endif // defined(VK_KHR_video_encode_queue) 
#if defined(VK_NV_acquire_winrt_display) 
    pOut->vkAcquireWinrtDisplayNV = (PFN_vkAcquireWinrtDisplayNV)instanceProcAddr(instance, "vkAcquireWinrtDisplayNV");
    pOut->vkGetWinrtDisplayNV = (PFN_vkGetWinrtDisplayNV)instanceProcAddr(instance, "vkGetWinrtDisplayNV");
#endif // defined(VK_NV_acquire_winrt_display) 
#if defined(VK_EXT_directfb_surface) 
    pOut->vkCreateDirectFBSurfaceEXT = (PFN_vkCreateDirectFBSurfaceEXT)instanceProcAddr(instance, "vkCreateDirectFBSurfaceEXT");
    pOut->vkGetPhysicalDeviceDirectFBPresentationSupportEXT = (PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT)instanceProcAddr(instance, "vkGetPhysicalDeviceDirectFBPresentationSupportEXT");
#endif // defined(VK_EXT_directfb_surface) 
#if defined(VK_QNX_screen_surface) 
    pOut->vkCreateScreenSurfaceQNX = (PFN_vkCreateScreenSurfaceQNX)instanceProcAddr(instance, "vkCreateScreenSurfaceQNX");
    pOut->vkGetPhysicalDeviceScreenPresentationSupportQNX = (PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX)instanceProcAddr(instance, "vkGetPhysicalDeviceScreenPresentationSupportQNX");
#endif // defined(VK_QNX_screen_surface) 
#if defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
    pOut->vkGetPhysicalDeviceExternalTensorPropertiesARM = (PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM)instanceProcAddr(instance, "vkGetPhysicalDeviceExternalTensorPropertiesARM");
#endif // defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
#if defined(VK_NV_optical_flow) 
    pOut->vkGetPhysicalDeviceOpticalFlowImageFormatsNV = (PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV)instanceProcAddr(instance, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV");
#endif // defined(VK_NV_optical_flow) 
#if defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
    pOut->vkGetPhysicalDeviceCooperativeVectorPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV)instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeVectorPropertiesNV");
#endif // defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
#if defined(VK_KHR_cooperative_matrix) 
    pOut->vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR");
#endif // defined(VK_KHR_cooperative_matrix) 
#if defined(VK_ARM_data_graph) 
    pOut->vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM)instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM");
    pOut->vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM)instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM");
#endif // defined(VK_ARM_data_graph) 
#if (defined(VK_ARM_data_graph_instruction_set_tosa)) || (defined(VK_ARM_data_graph_optical_flow)) 
    pOut->vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM)instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM");
#endif // (defined(VK_ARM_data_graph_instruction_set_tosa)) || (defined(VK_ARM_data_graph_optical_flow)) 
#if defined(VK_KHR_calibrated_timestamps) 
    pOut->vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR)instanceProcAddr(instance, "vkGetPhysicalDeviceCalibrateableTimeDomainsKHR");
#endif // defined(VK_KHR_calibrated_timestamps) 
#if defined(VK_OHOS_surface) 
    pOut->vkCreateSurfaceOHOS = (PFN_vkCreateSurfaceOHOS)instanceProcAddr(instance, "vkCreateSurfaceOHOS");
#endif // defined(VK_OHOS_surface) 
#if defined(VK_NV_cooperative_matrix2) 
    pOut->vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV)instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV");
#endif // defined(VK_NV_cooperative_matrix2) 
#if defined(VK_ARM_performance_counters_by_region) 
    pOut->vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM)instanceProcAddr(instance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM");
#endif // defined(VK_ARM_performance_counters_by_region) 
#if defined(VK_ARM_shader_instrumentation) 
    pOut->vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM = (PFN_vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM)instanceProcAddr(instance, "vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM");
#endif // defined(VK_ARM_shader_instrumentation) 
#if defined(VK_ARM_data_graph_optical_flow) 
    pOut->vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM)instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM");
#endif // defined(VK_ARM_data_graph_optical_flow) 
#if defined(VK_EXT_cooperative_matrix_maintenance1) 
    pOut->vkGetPhysicalDeviceCooperativeMatrixProperties2EXT = (PFN_vkGetPhysicalDeviceCooperativeMatrixProperties2EXT)instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixProperties2EXT");
#endif // defined(VK_EXT_cooperative_matrix_maintenance1) 
#if defined(VK_SEC_ubm_surface) 
    pOut->vkCreateUbmSurfaceSEC = (PFN_vkCreateUbmSurfaceSEC)instanceProcAddr(instance, "vkCreateUbmSurfaceSEC");
    pOut->vkGetPhysicalDeviceUbmPresentationSupportSEC = (PFN_vkGetPhysicalDeviceUbmPresentationSupportSEC)instanceProcAddr(instance, "vkGetPhysicalDeviceUbmPresentationSupportSEC");
#endif // defined(VK_SEC_ubm_surface) 
#pragma endregion
}

void dokLoadDeviceTable(PFN_vkGetDeviceProcAddr deviceProcAddr, VkDevice device, DokDeviceTable* pOut) {
    pOut->device = device;
#pragma region    pOut->{Device} = (PFN_{Device})deviceProcAddr(device, "{Device}");
#if defined(VK_VERSION_1_0) 
    pOut->vkDestroyDevice = (PFN_vkDestroyDevice)deviceProcAddr(device, "vkDestroyDevice");
    pOut->vkGetDeviceQueue = (PFN_vkGetDeviceQueue)deviceProcAddr(device, "vkGetDeviceQueue");
    pOut->vkQueueSubmit = (PFN_vkQueueSubmit)deviceProcAddr(device, "vkQueueSubmit");
    pOut->vkQueueWaitIdle = (PFN_vkQueueWaitIdle)deviceProcAddr(device, "vkQueueWaitIdle");
    pOut->vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)deviceProcAddr(device, "vkDeviceWaitIdle");
    pOut->vkAllocateMemory = (PFN_vkAllocateMemory)deviceProcAddr(device, "vkAllocateMemory");
    pOut->vkFreeMemory = (PFN_vkFreeMemory)deviceProcAddr(device, "vkFreeMemory");
    pOut->vkMapMemory = (PFN_vkMapMemory)deviceProcAddr(device, "vkMapMemory");
    pOut->vkUnmapMemory = (PFN_vkUnmapMemory)deviceProcAddr(device, "vkUnmapMemory");
    pOut->vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)deviceProcAddr(device, "vkFlushMappedMemoryRanges");
    pOut->vkInvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)deviceProcAddr(device, "vkInvalidateMappedMemoryRanges");
    pOut->vkGetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment)deviceProcAddr(device, "vkGetDeviceMemoryCommitment");
    pOut->vkBindBufferMemory = (PFN_vkBindBufferMemory)deviceProcAddr(device, "vkBindBufferMemory");
    pOut->vkBindImageMemory = (PFN_vkBindImageMemory)deviceProcAddr(device, "vkBindImageMemory");
    pOut->vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)deviceProcAddr(device, "vkGetBufferMemoryRequirements");
    pOut->vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)deviceProcAddr(device, "vkGetImageMemoryRequirements");
    pOut->vkGetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements)deviceProcAddr(device, "vkGetImageSparseMemoryRequirements");
    pOut->vkQueueBindSparse = (PFN_vkQueueBindSparse)deviceProcAddr(device, "vkQueueBindSparse");
    pOut->vkCreateFence = (PFN_vkCreateFence)deviceProcAddr(device, "vkCreateFence");
    pOut->vkDestroyFence = (PFN_vkDestroyFence)deviceProcAddr(device, "vkDestroyFence");
    pOut->vkResetFences = (PFN_vkResetFences)deviceProcAddr(device, "vkResetFences");
    pOut->vkGetFenceStatus = (PFN_vkGetFenceStatus)deviceProcAddr(device, "vkGetFenceStatus");
    pOut->vkWaitForFences = (PFN_vkWaitForFences)deviceProcAddr(device, "vkWaitForFences");
    pOut->vkCreateSemaphore = (PFN_vkCreateSemaphore)deviceProcAddr(device, "vkCreateSemaphore");
    pOut->vkDestroySemaphore = (PFN_vkDestroySemaphore)deviceProcAddr(device, "vkDestroySemaphore");
    pOut->vkCreateQueryPool = (PFN_vkCreateQueryPool)deviceProcAddr(device, "vkCreateQueryPool");
    pOut->vkDestroyQueryPool = (PFN_vkDestroyQueryPool)deviceProcAddr(device, "vkDestroyQueryPool");
    pOut->vkGetQueryPoolResults = (PFN_vkGetQueryPoolResults)deviceProcAddr(device, "vkGetQueryPoolResults");
    pOut->vkCreateBuffer = (PFN_vkCreateBuffer)deviceProcAddr(device, "vkCreateBuffer");
    pOut->vkDestroyBuffer = (PFN_vkDestroyBuffer)deviceProcAddr(device, "vkDestroyBuffer");
    pOut->vkCreateImage = (PFN_vkCreateImage)deviceProcAddr(device, "vkCreateImage");
    pOut->vkDestroyImage = (PFN_vkDestroyImage)deviceProcAddr(device, "vkDestroyImage");
    pOut->vkGetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout)deviceProcAddr(device, "vkGetImageSubresourceLayout");
    pOut->vkCreateImageView = (PFN_vkCreateImageView)deviceProcAddr(device, "vkCreateImageView");
    pOut->vkDestroyImageView = (PFN_vkDestroyImageView)deviceProcAddr(device, "vkDestroyImageView");
    pOut->vkCreateCommandPool = (PFN_vkCreateCommandPool)deviceProcAddr(device, "vkCreateCommandPool");
    pOut->vkDestroyCommandPool = (PFN_vkDestroyCommandPool)deviceProcAddr(device, "vkDestroyCommandPool");
    pOut->vkResetCommandPool = (PFN_vkResetCommandPool)deviceProcAddr(device, "vkResetCommandPool");
    pOut->vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)deviceProcAddr(device, "vkAllocateCommandBuffers");
    pOut->vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers)deviceProcAddr(device, "vkFreeCommandBuffers");
    pOut->vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)deviceProcAddr(device, "vkBeginCommandBuffer");
    pOut->vkEndCommandBuffer = (PFN_vkEndCommandBuffer)deviceProcAddr(device, "vkEndCommandBuffer");
    pOut->vkResetCommandBuffer = (PFN_vkResetCommandBuffer)deviceProcAddr(device, "vkResetCommandBuffer");
    pOut->vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer)deviceProcAddr(device, "vkCmdCopyBuffer");
    pOut->vkCmdCopyImage = (PFN_vkCmdCopyImage)deviceProcAddr(device, "vkCmdCopyImage");
    pOut->vkCmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)deviceProcAddr(device, "vkCmdCopyBufferToImage");
    pOut->vkCmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer)deviceProcAddr(device, "vkCmdCopyImageToBuffer");
    pOut->vkCmdUpdateBuffer = (PFN_vkCmdUpdateBuffer)deviceProcAddr(device, "vkCmdUpdateBuffer");
    pOut->vkCmdFillBuffer = (PFN_vkCmdFillBuffer)deviceProcAddr(device, "vkCmdFillBuffer");
    pOut->vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)deviceProcAddr(device, "vkCmdPipelineBarrier");
    pOut->vkCmdBeginQuery = (PFN_vkCmdBeginQuery)deviceProcAddr(device, "vkCmdBeginQuery");
    pOut->vkCmdEndQuery = (PFN_vkCmdEndQuery)deviceProcAddr(device, "vkCmdEndQuery");
    pOut->vkCmdResetQueryPool = (PFN_vkCmdResetQueryPool)deviceProcAddr(device, "vkCmdResetQueryPool");
    pOut->vkCmdWriteTimestamp = (PFN_vkCmdWriteTimestamp)deviceProcAddr(device, "vkCmdWriteTimestamp");
    pOut->vkCmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults)deviceProcAddr(device, "vkCmdCopyQueryPoolResults");
    pOut->vkCmdExecuteCommands = (PFN_vkCmdExecuteCommands)deviceProcAddr(device, "vkCmdExecuteCommands");
    pOut->vkCreateEvent = (PFN_vkCreateEvent)deviceProcAddr(device, "vkCreateEvent");
    pOut->vkDestroyEvent = (PFN_vkDestroyEvent)deviceProcAddr(device, "vkDestroyEvent");
    pOut->vkGetEventStatus = (PFN_vkGetEventStatus)deviceProcAddr(device, "vkGetEventStatus");
    pOut->vkSetEvent = (PFN_vkSetEvent)deviceProcAddr(device, "vkSetEvent");
    pOut->vkResetEvent = (PFN_vkResetEvent)deviceProcAddr(device, "vkResetEvent");
    pOut->vkCreateBufferView = (PFN_vkCreateBufferView)deviceProcAddr(device, "vkCreateBufferView");
    pOut->vkDestroyBufferView = (PFN_vkDestroyBufferView)deviceProcAddr(device, "vkDestroyBufferView");
    pOut->vkCreateShaderModule = (PFN_vkCreateShaderModule)deviceProcAddr(device, "vkCreateShaderModule");
    pOut->vkDestroyShaderModule = (PFN_vkDestroyShaderModule)deviceProcAddr(device, "vkDestroyShaderModule");
    pOut->vkCreatePipelineCache = (PFN_vkCreatePipelineCache)deviceProcAddr(device, "vkCreatePipelineCache");
    pOut->vkDestroyPipelineCache = (PFN_vkDestroyPipelineCache)deviceProcAddr(device, "vkDestroyPipelineCache");
    pOut->vkGetPipelineCacheData = (PFN_vkGetPipelineCacheData)deviceProcAddr(device, "vkGetPipelineCacheData");
    pOut->vkMergePipelineCaches = (PFN_vkMergePipelineCaches)deviceProcAddr(device, "vkMergePipelineCaches");
    pOut->vkCreateComputePipelines = (PFN_vkCreateComputePipelines)deviceProcAddr(device, "vkCreateComputePipelines");
    pOut->vkDestroyPipeline = (PFN_vkDestroyPipeline)deviceProcAddr(device, "vkDestroyPipeline");
    pOut->vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)deviceProcAddr(device, "vkCreatePipelineLayout");
    pOut->vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)deviceProcAddr(device, "vkDestroyPipelineLayout");
    pOut->vkCreateSampler = (PFN_vkCreateSampler)deviceProcAddr(device, "vkCreateSampler");
    pOut->vkDestroySampler = (PFN_vkDestroySampler)deviceProcAddr(device, "vkDestroySampler");
    pOut->vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)deviceProcAddr(device, "vkCreateDescriptorSetLayout");
    pOut->vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)deviceProcAddr(device, "vkDestroyDescriptorSetLayout");
    pOut->vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool)deviceProcAddr(device, "vkCreateDescriptorPool");
    pOut->vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)deviceProcAddr(device, "vkDestroyDescriptorPool");
    pOut->vkResetDescriptorPool = (PFN_vkResetDescriptorPool)deviceProcAddr(device, "vkResetDescriptorPool");
    pOut->vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)deviceProcAddr(device, "vkAllocateDescriptorSets");
    pOut->vkFreeDescriptorSets = (PFN_vkFreeDescriptorSets)deviceProcAddr(device, "vkFreeDescriptorSets");
    pOut->vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)deviceProcAddr(device, "vkUpdateDescriptorSets");
    pOut->vkCmdBindPipeline = (PFN_vkCmdBindPipeline)deviceProcAddr(device, "vkCmdBindPipeline");
    pOut->vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)deviceProcAddr(device, "vkCmdBindDescriptorSets");
    pOut->vkCmdClearColorImage = (PFN_vkCmdClearColorImage)deviceProcAddr(device, "vkCmdClearColorImage");
    pOut->vkCmdDispatch = (PFN_vkCmdDispatch)deviceProcAddr(device, "vkCmdDispatch");
    pOut->vkCmdDispatchIndirect = (PFN_vkCmdDispatchIndirect)deviceProcAddr(device, "vkCmdDispatchIndirect");
    pOut->vkCmdSetEvent = (PFN_vkCmdSetEvent)deviceProcAddr(device, "vkCmdSetEvent");
    pOut->vkCmdResetEvent = (PFN_vkCmdResetEvent)deviceProcAddr(device, "vkCmdResetEvent");
    pOut->vkCmdWaitEvents = (PFN_vkCmdWaitEvents)deviceProcAddr(device, "vkCmdWaitEvents");
    pOut->vkCmdPushConstants = (PFN_vkCmdPushConstants)deviceProcAddr(device, "vkCmdPushConstants");
    pOut->vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)deviceProcAddr(device, "vkCreateGraphicsPipelines");
    pOut->vkCreateFramebuffer = (PFN_vkCreateFramebuffer)deviceProcAddr(device, "vkCreateFramebuffer");
    pOut->vkDestroyFramebuffer = (PFN_vkDestroyFramebuffer)deviceProcAddr(device, "vkDestroyFramebuffer");
    pOut->vkCreateRenderPass = (PFN_vkCreateRenderPass)deviceProcAddr(device, "vkCreateRenderPass");
    pOut->vkDestroyRenderPass = (PFN_vkDestroyRenderPass)deviceProcAddr(device, "vkDestroyRenderPass");
    pOut->vkGetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity)deviceProcAddr(device, "vkGetRenderAreaGranularity");
    pOut->vkCmdSetViewport = (PFN_vkCmdSetViewport)deviceProcAddr(device, "vkCmdSetViewport");
    pOut->vkCmdSetScissor = (PFN_vkCmdSetScissor)deviceProcAddr(device, "vkCmdSetScissor");
    pOut->vkCmdSetLineWidth = (PFN_vkCmdSetLineWidth)deviceProcAddr(device, "vkCmdSetLineWidth");
    pOut->vkCmdSetDepthBias = (PFN_vkCmdSetDepthBias)deviceProcAddr(device, "vkCmdSetDepthBias");
    pOut->vkCmdSetBlendConstants = (PFN_vkCmdSetBlendConstants)deviceProcAddr(device, "vkCmdSetBlendConstants");
    pOut->vkCmdSetDepthBounds = (PFN_vkCmdSetDepthBounds)deviceProcAddr(device, "vkCmdSetDepthBounds");
    pOut->vkCmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask)deviceProcAddr(device, "vkCmdSetStencilCompareMask");
    pOut->vkCmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask)deviceProcAddr(device, "vkCmdSetStencilWriteMask");
    pOut->vkCmdSetStencilReference = (PFN_vkCmdSetStencilReference)deviceProcAddr(device, "vkCmdSetStencilReference");
    pOut->vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)deviceProcAddr(device, "vkCmdBindIndexBuffer");
    pOut->vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)deviceProcAddr(device, "vkCmdBindVertexBuffers");
    pOut->vkCmdDraw = (PFN_vkCmdDraw)deviceProcAddr(device, "vkCmdDraw");
    pOut->vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)deviceProcAddr(device, "vkCmdDrawIndexed");
    pOut->vkCmdDrawIndirect = (PFN_vkCmdDrawIndirect)deviceProcAddr(device, "vkCmdDrawIndirect");
    pOut->vkCmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect)deviceProcAddr(device, "vkCmdDrawIndexedIndirect");
    pOut->vkCmdBlitImage = (PFN_vkCmdBlitImage)deviceProcAddr(device, "vkCmdBlitImage");
    pOut->vkCmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage)deviceProcAddr(device, "vkCmdClearDepthStencilImage");
    pOut->vkCmdClearAttachments = (PFN_vkCmdClearAttachments)deviceProcAddr(device, "vkCmdClearAttachments");
    pOut->vkCmdResolveImage = (PFN_vkCmdResolveImage)deviceProcAddr(device, "vkCmdResolveImage");
    pOut->vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)deviceProcAddr(device, "vkCmdBeginRenderPass");
    pOut->vkCmdNextSubpass = (PFN_vkCmdNextSubpass)deviceProcAddr(device, "vkCmdNextSubpass");
    pOut->vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass)deviceProcAddr(device, "vkCmdEndRenderPass");
#endif // defined(VK_VERSION_1_0) 
#if defined(VK_VERSION_1_1) 
    pOut->vkBindBufferMemory2 = (PFN_vkBindBufferMemory2)deviceProcAddr(device, "vkBindBufferMemory2");
    pOut->vkBindImageMemory2 = (PFN_vkBindImageMemory2)deviceProcAddr(device, "vkBindImageMemory2");
    pOut->vkGetDeviceGroupPeerMemoryFeatures = (PFN_vkGetDeviceGroupPeerMemoryFeatures)deviceProcAddr(device, "vkGetDeviceGroupPeerMemoryFeatures");
    pOut->vkCmdSetDeviceMask = (PFN_vkCmdSetDeviceMask)deviceProcAddr(device, "vkCmdSetDeviceMask");
    pOut->vkGetImageMemoryRequirements2 = (PFN_vkGetImageMemoryRequirements2)deviceProcAddr(device, "vkGetImageMemoryRequirements2");
    pOut->vkGetBufferMemoryRequirements2 = (PFN_vkGetBufferMemoryRequirements2)deviceProcAddr(device, "vkGetBufferMemoryRequirements2");
    pOut->vkGetImageSparseMemoryRequirements2 = (PFN_vkGetImageSparseMemoryRequirements2)deviceProcAddr(device, "vkGetImageSparseMemoryRequirements2");
    pOut->vkTrimCommandPool = (PFN_vkTrimCommandPool)deviceProcAddr(device, "vkTrimCommandPool");
    pOut->vkGetDeviceQueue2 = (PFN_vkGetDeviceQueue2)deviceProcAddr(device, "vkGetDeviceQueue2");
    pOut->vkCmdDispatchBase = (PFN_vkCmdDispatchBase)deviceProcAddr(device, "vkCmdDispatchBase");
    pOut->vkCreateDescriptorUpdateTemplate = (PFN_vkCreateDescriptorUpdateTemplate)deviceProcAddr(device, "vkCreateDescriptorUpdateTemplate");
    pOut->vkDestroyDescriptorUpdateTemplate = (PFN_vkDestroyDescriptorUpdateTemplate)deviceProcAddr(device, "vkDestroyDescriptorUpdateTemplate");
    pOut->vkUpdateDescriptorSetWithTemplate = (PFN_vkUpdateDescriptorSetWithTemplate)deviceProcAddr(device, "vkUpdateDescriptorSetWithTemplate");
    pOut->vkGetDescriptorSetLayoutSupport = (PFN_vkGetDescriptorSetLayoutSupport)deviceProcAddr(device, "vkGetDescriptorSetLayoutSupport");
    pOut->vkCreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion)deviceProcAddr(device, "vkCreateSamplerYcbcrConversion");
    pOut->vkDestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion)deviceProcAddr(device, "vkDestroySamplerYcbcrConversion");
#endif // defined(VK_VERSION_1_1) 
#if defined(VK_VERSION_1_2) 
    pOut->vkResetQueryPool = (PFN_vkResetQueryPool)deviceProcAddr(device, "vkResetQueryPool");
    pOut->vkGetSemaphoreCounterValue = (PFN_vkGetSemaphoreCounterValue)deviceProcAddr(device, "vkGetSemaphoreCounterValue");
    pOut->vkWaitSemaphores = (PFN_vkWaitSemaphores)deviceProcAddr(device, "vkWaitSemaphores");
    pOut->vkSignalSemaphore = (PFN_vkSignalSemaphore)deviceProcAddr(device, "vkSignalSemaphore");
    pOut->vkGetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddress)deviceProcAddr(device, "vkGetBufferDeviceAddress");
    pOut->vkGetBufferOpaqueCaptureAddress = (PFN_vkGetBufferOpaqueCaptureAddress)deviceProcAddr(device, "vkGetBufferOpaqueCaptureAddress");
    pOut->vkGetDeviceMemoryOpaqueCaptureAddress = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress)deviceProcAddr(device, "vkGetDeviceMemoryOpaqueCaptureAddress");
    pOut->vkCmdDrawIndirectCount = (PFN_vkCmdDrawIndirectCount)deviceProcAddr(device, "vkCmdDrawIndirectCount");
    pOut->vkCmdDrawIndexedIndirectCount = (PFN_vkCmdDrawIndexedIndirectCount)deviceProcAddr(device, "vkCmdDrawIndexedIndirectCount");
    pOut->vkCreateRenderPass2 = (PFN_vkCreateRenderPass2)deviceProcAddr(device, "vkCreateRenderPass2");
    pOut->vkCmdBeginRenderPass2 = (PFN_vkCmdBeginRenderPass2)deviceProcAddr(device, "vkCmdBeginRenderPass2");
    pOut->vkCmdNextSubpass2 = (PFN_vkCmdNextSubpass2)deviceProcAddr(device, "vkCmdNextSubpass2");
    pOut->vkCmdEndRenderPass2 = (PFN_vkCmdEndRenderPass2)deviceProcAddr(device, "vkCmdEndRenderPass2");
#endif // defined(VK_VERSION_1_2) 
#if defined(VK_VERSION_1_3) 
    pOut->vkCreatePrivateDataSlot = (PFN_vkCreatePrivateDataSlot)deviceProcAddr(device, "vkCreatePrivateDataSlot");
    pOut->vkDestroyPrivateDataSlot = (PFN_vkDestroyPrivateDataSlot)deviceProcAddr(device, "vkDestroyPrivateDataSlot");
    pOut->vkSetPrivateData = (PFN_vkSetPrivateData)deviceProcAddr(device, "vkSetPrivateData");
    pOut->vkGetPrivateData = (PFN_vkGetPrivateData)deviceProcAddr(device, "vkGetPrivateData");
    pOut->vkCmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2)deviceProcAddr(device, "vkCmdPipelineBarrier2");
    pOut->vkCmdWriteTimestamp2 = (PFN_vkCmdWriteTimestamp2)deviceProcAddr(device, "vkCmdWriteTimestamp2");
    pOut->vkQueueSubmit2 = (PFN_vkQueueSubmit2)deviceProcAddr(device, "vkQueueSubmit2");
    pOut->vkCmdCopyBuffer2 = (PFN_vkCmdCopyBuffer2)deviceProcAddr(device, "vkCmdCopyBuffer2");
    pOut->vkCmdCopyImage2 = (PFN_vkCmdCopyImage2)deviceProcAddr(device, "vkCmdCopyImage2");
    pOut->vkCmdCopyBufferToImage2 = (PFN_vkCmdCopyBufferToImage2)deviceProcAddr(device, "vkCmdCopyBufferToImage2");
    pOut->vkCmdCopyImageToBuffer2 = (PFN_vkCmdCopyImageToBuffer2)deviceProcAddr(device, "vkCmdCopyImageToBuffer2");
    pOut->vkGetDeviceBufferMemoryRequirements = (PFN_vkGetDeviceBufferMemoryRequirements)deviceProcAddr(device, "vkGetDeviceBufferMemoryRequirements");
    pOut->vkGetDeviceImageMemoryRequirements = (PFN_vkGetDeviceImageMemoryRequirements)deviceProcAddr(device, "vkGetDeviceImageMemoryRequirements");
    pOut->vkGetDeviceImageSparseMemoryRequirements = (PFN_vkGetDeviceImageSparseMemoryRequirements)deviceProcAddr(device, "vkGetDeviceImageSparseMemoryRequirements");
    pOut->vkCmdSetEvent2 = (PFN_vkCmdSetEvent2)deviceProcAddr(device, "vkCmdSetEvent2");
    pOut->vkCmdResetEvent2 = (PFN_vkCmdResetEvent2)deviceProcAddr(device, "vkCmdResetEvent2");
    pOut->vkCmdWaitEvents2 = (PFN_vkCmdWaitEvents2)deviceProcAddr(device, "vkCmdWaitEvents2");
    pOut->vkCmdBlitImage2 = (PFN_vkCmdBlitImage2)deviceProcAddr(device, "vkCmdBlitImage2");
    pOut->vkCmdResolveImage2 = (PFN_vkCmdResolveImage2)deviceProcAddr(device, "vkCmdResolveImage2");
    pOut->vkCmdBeginRendering = (PFN_vkCmdBeginRendering)deviceProcAddr(device, "vkCmdBeginRendering");
    pOut->vkCmdEndRendering = (PFN_vkCmdEndRendering)deviceProcAddr(device, "vkCmdEndRendering");
    pOut->vkCmdSetCullMode = (PFN_vkCmdSetCullMode)deviceProcAddr(device, "vkCmdSetCullMode");
    pOut->vkCmdSetFrontFace = (PFN_vkCmdSetFrontFace)deviceProcAddr(device, "vkCmdSetFrontFace");
    pOut->vkCmdSetPrimitiveTopology = (PFN_vkCmdSetPrimitiveTopology)deviceProcAddr(device, "vkCmdSetPrimitiveTopology");
    pOut->vkCmdSetViewportWithCount = (PFN_vkCmdSetViewportWithCount)deviceProcAddr(device, "vkCmdSetViewportWithCount");
    pOut->vkCmdSetScissorWithCount = (PFN_vkCmdSetScissorWithCount)deviceProcAddr(device, "vkCmdSetScissorWithCount");
    pOut->vkCmdBindVertexBuffers2 = (PFN_vkCmdBindVertexBuffers2)deviceProcAddr(device, "vkCmdBindVertexBuffers2");
    pOut->vkCmdSetDepthTestEnable = (PFN_vkCmdSetDepthTestEnable)deviceProcAddr(device, "vkCmdSetDepthTestEnable");
    pOut->vkCmdSetDepthWriteEnable = (PFN_vkCmdSetDepthWriteEnable)deviceProcAddr(device, "vkCmdSetDepthWriteEnable");
    pOut->vkCmdSetDepthCompareOp = (PFN_vkCmdSetDepthCompareOp)deviceProcAddr(device, "vkCmdSetDepthCompareOp");
    pOut->vkCmdSetDepthBoundsTestEnable = (PFN_vkCmdSetDepthBoundsTestEnable)deviceProcAddr(device, "vkCmdSetDepthBoundsTestEnable");
    pOut->vkCmdSetStencilTestEnable = (PFN_vkCmdSetStencilTestEnable)deviceProcAddr(device, "vkCmdSetStencilTestEnable");
    pOut->vkCmdSetStencilOp = (PFN_vkCmdSetStencilOp)deviceProcAddr(device, "vkCmdSetStencilOp");
    pOut->vkCmdSetRasterizerDiscardEnable = (PFN_vkCmdSetRasterizerDiscardEnable)deviceProcAddr(device, "vkCmdSetRasterizerDiscardEnable");
    pOut->vkCmdSetDepthBiasEnable = (PFN_vkCmdSetDepthBiasEnable)deviceProcAddr(device, "vkCmdSetDepthBiasEnable");
    pOut->vkCmdSetPrimitiveRestartEnable = (PFN_vkCmdSetPrimitiveRestartEnable)deviceProcAddr(device, "vkCmdSetPrimitiveRestartEnable");
#endif // defined(VK_VERSION_1_3) 
#if defined(VK_VERSION_1_4) 
    pOut->vkMapMemory2 = (PFN_vkMapMemory2)deviceProcAddr(device, "vkMapMemory2");
    pOut->vkUnmapMemory2 = (PFN_vkUnmapMemory2)deviceProcAddr(device, "vkUnmapMemory2");
    pOut->vkGetDeviceImageSubresourceLayout = (PFN_vkGetDeviceImageSubresourceLayout)deviceProcAddr(device, "vkGetDeviceImageSubresourceLayout");
    pOut->vkGetImageSubresourceLayout2 = (PFN_vkGetImageSubresourceLayout2)deviceProcAddr(device, "vkGetImageSubresourceLayout2");
    pOut->vkCopyMemoryToImage = (PFN_vkCopyMemoryToImage)deviceProcAddr(device, "vkCopyMemoryToImage");
    pOut->vkCopyImageToMemory = (PFN_vkCopyImageToMemory)deviceProcAddr(device, "vkCopyImageToMemory");
    pOut->vkCopyImageToImage = (PFN_vkCopyImageToImage)deviceProcAddr(device, "vkCopyImageToImage");
    pOut->vkTransitionImageLayout = (PFN_vkTransitionImageLayout)deviceProcAddr(device, "vkTransitionImageLayout");
    pOut->vkCmdPushDescriptorSet = (PFN_vkCmdPushDescriptorSet)deviceProcAddr(device, "vkCmdPushDescriptorSet");
    pOut->vkCmdPushDescriptorSetWithTemplate = (PFN_vkCmdPushDescriptorSetWithTemplate)deviceProcAddr(device, "vkCmdPushDescriptorSetWithTemplate");
    pOut->vkCmdBindDescriptorSets2 = (PFN_vkCmdBindDescriptorSets2)deviceProcAddr(device, "vkCmdBindDescriptorSets2");
    pOut->vkCmdPushConstants2 = (PFN_vkCmdPushConstants2)deviceProcAddr(device, "vkCmdPushConstants2");
    pOut->vkCmdPushDescriptorSet2 = (PFN_vkCmdPushDescriptorSet2)deviceProcAddr(device, "vkCmdPushDescriptorSet2");
    pOut->vkCmdPushDescriptorSetWithTemplate2 = (PFN_vkCmdPushDescriptorSetWithTemplate2)deviceProcAddr(device, "vkCmdPushDescriptorSetWithTemplate2");
    pOut->vkCmdSetLineStipple = (PFN_vkCmdSetLineStipple)deviceProcAddr(device, "vkCmdSetLineStipple");
    pOut->vkCmdBindIndexBuffer2 = (PFN_vkCmdBindIndexBuffer2)deviceProcAddr(device, "vkCmdBindIndexBuffer2");
    pOut->vkGetRenderingAreaGranularity = (PFN_vkGetRenderingAreaGranularity)deviceProcAddr(device, "vkGetRenderingAreaGranularity");
    pOut->vkCmdSetRenderingAttachmentLocations = (PFN_vkCmdSetRenderingAttachmentLocations)deviceProcAddr(device, "vkCmdSetRenderingAttachmentLocations");
    pOut->vkCmdSetRenderingInputAttachmentIndices = (PFN_vkCmdSetRenderingInputAttachmentIndices)deviceProcAddr(device, "vkCmdSetRenderingInputAttachmentIndices");
#endif // defined(VK_VERSION_1_4) 
#if defined(VK_KHR_swapchain) 
    pOut->vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)deviceProcAddr(device, "vkCreateSwapchainKHR");
    pOut->vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)deviceProcAddr(device, "vkDestroySwapchainKHR");
    pOut->vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)deviceProcAddr(device, "vkGetSwapchainImagesKHR");
    pOut->vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)deviceProcAddr(device, "vkAcquireNextImageKHR");
    pOut->vkQueuePresentKHR = (PFN_vkQueuePresentKHR)deviceProcAddr(device, "vkQueuePresentKHR");
#endif // defined(VK_KHR_swapchain) 
#if defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1) 
    pOut->vkGetDeviceGroupPresentCapabilitiesKHR = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)deviceProcAddr(device, "vkGetDeviceGroupPresentCapabilitiesKHR");
    pOut->vkGetDeviceGroupSurfacePresentModesKHR = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)deviceProcAddr(device, "vkGetDeviceGroupSurfacePresentModesKHR");
    pOut->vkAcquireNextImage2KHR = (PFN_vkAcquireNextImage2KHR)deviceProcAddr(device, "vkAcquireNextImage2KHR");
#endif // defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1) 
#if defined(VK_KHR_display_swapchain) 
    pOut->vkCreateSharedSwapchainsKHR = (PFN_vkCreateSharedSwapchainsKHR)deviceProcAddr(device, "vkCreateSharedSwapchainsKHR");
#endif // defined(VK_KHR_display_swapchain) 
#if defined(VK_KHR_video_queue) 
    pOut->vkCreateVideoSessionKHR = (PFN_vkCreateVideoSessionKHR)deviceProcAddr(device, "vkCreateVideoSessionKHR");
    pOut->vkDestroyVideoSessionKHR = (PFN_vkDestroyVideoSessionKHR)deviceProcAddr(device, "vkDestroyVideoSessionKHR");
    pOut->vkGetVideoSessionMemoryRequirementsKHR = (PFN_vkGetVideoSessionMemoryRequirementsKHR)deviceProcAddr(device, "vkGetVideoSessionMemoryRequirementsKHR");
    pOut->vkBindVideoSessionMemoryKHR = (PFN_vkBindVideoSessionMemoryKHR)deviceProcAddr(device, "vkBindVideoSessionMemoryKHR");
    pOut->vkCreateVideoSessionParametersKHR = (PFN_vkCreateVideoSessionParametersKHR)deviceProcAddr(device, "vkCreateVideoSessionParametersKHR");
    pOut->vkUpdateVideoSessionParametersKHR = (PFN_vkUpdateVideoSessionParametersKHR)deviceProcAddr(device, "vkUpdateVideoSessionParametersKHR");
    pOut->vkDestroyVideoSessionParametersKHR = (PFN_vkDestroyVideoSessionParametersKHR)deviceProcAddr(device, "vkDestroyVideoSessionParametersKHR");
    pOut->vkCmdBeginVideoCodingKHR = (PFN_vkCmdBeginVideoCodingKHR)deviceProcAddr(device, "vkCmdBeginVideoCodingKHR");
    pOut->vkCmdEndVideoCodingKHR = (PFN_vkCmdEndVideoCodingKHR)deviceProcAddr(device, "vkCmdEndVideoCodingKHR");
    pOut->vkCmdControlVideoCodingKHR = (PFN_vkCmdControlVideoCodingKHR)deviceProcAddr(device, "vkCmdControlVideoCodingKHR");
#endif // defined(VK_KHR_video_queue) 
#if defined(VK_KHR_video_decode_queue) 
    pOut->vkCmdDecodeVideoKHR = (PFN_vkCmdDecodeVideoKHR)deviceProcAddr(device, "vkCmdDecodeVideoKHR");
#endif // defined(VK_KHR_video_decode_queue) 
#if defined(VK_EXT_transform_feedback) 
    pOut->vkCmdBindTransformFeedbackBuffersEXT = (PFN_vkCmdBindTransformFeedbackBuffersEXT)deviceProcAddr(device, "vkCmdBindTransformFeedbackBuffersEXT");
    pOut->vkCmdBeginTransformFeedbackEXT = (PFN_vkCmdBeginTransformFeedbackEXT)deviceProcAddr(device, "vkCmdBeginTransformFeedbackEXT");
    pOut->vkCmdEndTransformFeedbackEXT = (PFN_vkCmdEndTransformFeedbackEXT)deviceProcAddr(device, "vkCmdEndTransformFeedbackEXT");
    pOut->vkCmdBeginQueryIndexedEXT = (PFN_vkCmdBeginQueryIndexedEXT)deviceProcAddr(device, "vkCmdBeginQueryIndexedEXT");
    pOut->vkCmdEndQueryIndexedEXT = (PFN_vkCmdEndQueryIndexedEXT)deviceProcAddr(device, "vkCmdEndQueryIndexedEXT");
    pOut->vkCmdDrawIndirectByteCountEXT = (PFN_vkCmdDrawIndirectByteCountEXT)deviceProcAddr(device, "vkCmdDrawIndirectByteCountEXT");
#endif // defined(VK_EXT_transform_feedback) 
#if defined(VK_NVX_binary_import) && VK_NVX_BINARY_IMPORT_SPEC_VERSION >= 2 
    pOut->vkCreateCuModuleNVX = (PFN_vkCreateCuModuleNVX)deviceProcAddr(device, "vkCreateCuModuleNVX");
    pOut->vkCreateCuFunctionNVX = (PFN_vkCreateCuFunctionNVX)deviceProcAddr(device, "vkCreateCuFunctionNVX");
    pOut->vkDestroyCuModuleNVX = (PFN_vkDestroyCuModuleNVX)deviceProcAddr(device, "vkDestroyCuModuleNVX");
    pOut->vkDestroyCuFunctionNVX = (PFN_vkDestroyCuFunctionNVX)deviceProcAddr(device, "vkDestroyCuFunctionNVX");
    pOut->vkCmdCuLaunchKernelNVX = (PFN_vkCmdCuLaunchKernelNVX)deviceProcAddr(device, "vkCmdCuLaunchKernelNVX");
#endif // defined(VK_NVX_binary_import) && VK_NVX_BINARY_IMPORT_SPEC_VERSION >= 2 
#if defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 4 
    pOut->vkGetImageViewHandleNVX = (PFN_vkGetImageViewHandleNVX)deviceProcAddr(device, "vkGetImageViewHandleNVX");
    pOut->vkGetImageViewHandle64NVX = (PFN_vkGetImageViewHandle64NVX)deviceProcAddr(device, "vkGetImageViewHandle64NVX");
    pOut->vkGetImageViewAddressNVX = (PFN_vkGetImageViewAddressNVX)deviceProcAddr(device, "vkGetImageViewAddressNVX");
    pOut->vkGetDeviceCombinedImageSamplerIndexNVX = (PFN_vkGetDeviceCombinedImageSamplerIndexNVX)deviceProcAddr(device, "vkGetDeviceCombinedImageSamplerIndexNVX");
#endif // defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 4 
#if defined(VK_AMD_shader_info) 
    pOut->vkGetShaderInfoAMD = (PFN_vkGetShaderInfoAMD)deviceProcAddr(device, "vkGetShaderInfoAMD");
#endif // defined(VK_AMD_shader_info) 
#if defined(VK_KHR_external_memory_win32) 
    pOut->vkGetMemoryWin32HandleKHR = (PFN_vkGetMemoryWin32HandleKHR)deviceProcAddr(device, "vkGetMemoryWin32HandleKHR");
    pOut->vkGetMemoryWin32HandlePropertiesKHR = (PFN_vkGetMemoryWin32HandlePropertiesKHR)deviceProcAddr(device, "vkGetMemoryWin32HandlePropertiesKHR");
#endif // defined(VK_KHR_external_memory_win32) 
#if defined(VK_KHR_external_memory_fd) 
    pOut->vkGetMemoryFdKHR = (PFN_vkGetMemoryFdKHR)deviceProcAddr(device, "vkGetMemoryFdKHR");
    pOut->vkGetMemoryFdPropertiesKHR = (PFN_vkGetMemoryFdPropertiesKHR)deviceProcAddr(device, "vkGetMemoryFdPropertiesKHR");
#endif // defined(VK_KHR_external_memory_fd) 
#if defined(VK_KHR_external_semaphore_win32) 
    pOut->vkImportSemaphoreWin32HandleKHR = (PFN_vkImportSemaphoreWin32HandleKHR)deviceProcAddr(device, "vkImportSemaphoreWin32HandleKHR");
    pOut->vkGetSemaphoreWin32HandleKHR = (PFN_vkGetSemaphoreWin32HandleKHR)deviceProcAddr(device, "vkGetSemaphoreWin32HandleKHR");
#endif // defined(VK_KHR_external_semaphore_win32) 
#if defined(VK_KHR_external_semaphore_fd) 
    pOut->vkImportSemaphoreFdKHR = (PFN_vkImportSemaphoreFdKHR)deviceProcAddr(device, "vkImportSemaphoreFdKHR");
    pOut->vkGetSemaphoreFdKHR = (PFN_vkGetSemaphoreFdKHR)deviceProcAddr(device, "vkGetSemaphoreFdKHR");
#endif // defined(VK_KHR_external_semaphore_fd) 
#if defined(VK_EXT_conditional_rendering) && VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION >= 2 
    pOut->vkCmdBeginConditionalRenderingEXT = (PFN_vkCmdBeginConditionalRenderingEXT)deviceProcAddr(device, "vkCmdBeginConditionalRenderingEXT");
    pOut->vkCmdEndConditionalRenderingEXT = (PFN_vkCmdEndConditionalRenderingEXT)deviceProcAddr(device, "vkCmdEndConditionalRenderingEXT");
#endif // defined(VK_EXT_conditional_rendering) && VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION >= 2 
#if defined(VK_NV_clip_space_w_scaling) 
    pOut->vkCmdSetViewportWScalingNV = (PFN_vkCmdSetViewportWScalingNV)deviceProcAddr(device, "vkCmdSetViewportWScalingNV");
#endif // defined(VK_NV_clip_space_w_scaling) 
#if defined(VK_EXT_display_control) 
    pOut->vkDisplayPowerControlEXT = (PFN_vkDisplayPowerControlEXT)deviceProcAddr(device, "vkDisplayPowerControlEXT");
    pOut->vkRegisterDeviceEventEXT = (PFN_vkRegisterDeviceEventEXT)deviceProcAddr(device, "vkRegisterDeviceEventEXT");
    pOut->vkRegisterDisplayEventEXT = (PFN_vkRegisterDisplayEventEXT)deviceProcAddr(device, "vkRegisterDisplayEventEXT");
    pOut->vkGetSwapchainCounterEXT = (PFN_vkGetSwapchainCounterEXT)deviceProcAddr(device, "vkGetSwapchainCounterEXT");
#endif // defined(VK_EXT_display_control) 
#if defined(VK_GOOGLE_display_timing) 
    pOut->vkGetRefreshCycleDurationGOOGLE = (PFN_vkGetRefreshCycleDurationGOOGLE)deviceProcAddr(device, "vkGetRefreshCycleDurationGOOGLE");
    pOut->vkGetPastPresentationTimingGOOGLE = (PFN_vkGetPastPresentationTimingGOOGLE)deviceProcAddr(device, "vkGetPastPresentationTimingGOOGLE");
#endif // defined(VK_GOOGLE_display_timing) 
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 
    pOut->vkCmdSetDiscardRectangleEXT = (PFN_vkCmdSetDiscardRectangleEXT)deviceProcAddr(device, "vkCmdSetDiscardRectangleEXT");
    pOut->vkCmdSetDiscardRectangleEnableEXT = (PFN_vkCmdSetDiscardRectangleEnableEXT)deviceProcAddr(device, "vkCmdSetDiscardRectangleEnableEXT");
    pOut->vkCmdSetDiscardRectangleModeEXT = (PFN_vkCmdSetDiscardRectangleModeEXT)deviceProcAddr(device, "vkCmdSetDiscardRectangleModeEXT");
#endif // defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 
#if defined(VK_EXT_hdr_metadata) && VK_EXT_HDR_METADATA_SPEC_VERSION >= 3 
    pOut->vkSetHdrMetadataEXT = (PFN_vkSetHdrMetadataEXT)deviceProcAddr(device, "vkSetHdrMetadataEXT");
#endif // defined(VK_EXT_hdr_metadata) && VK_EXT_HDR_METADATA_SPEC_VERSION >= 3 
#if defined(VK_KHR_shared_presentable_image) 
    pOut->vkGetSwapchainStatusKHR = (PFN_vkGetSwapchainStatusKHR)deviceProcAddr(device, "vkGetSwapchainStatusKHR");
#endif // defined(VK_KHR_shared_presentable_image) 
#if defined(VK_KHR_external_fence_win32) 
    pOut->vkImportFenceWin32HandleKHR = (PFN_vkImportFenceWin32HandleKHR)deviceProcAddr(device, "vkImportFenceWin32HandleKHR");
    pOut->vkGetFenceWin32HandleKHR = (PFN_vkGetFenceWin32HandleKHR)deviceProcAddr(device, "vkGetFenceWin32HandleKHR");
#endif // defined(VK_KHR_external_fence_win32) 
#if defined(VK_KHR_external_fence_fd) 
    pOut->vkImportFenceFdKHR = (PFN_vkImportFenceFdKHR)deviceProcAddr(device, "vkImportFenceFdKHR");
    pOut->vkGetFenceFdKHR = (PFN_vkGetFenceFdKHR)deviceProcAddr(device, "vkGetFenceFdKHR");
#endif // defined(VK_KHR_external_fence_fd) 
#if defined(VK_KHR_performance_query) 
    pOut->vkAcquireProfilingLockKHR = (PFN_vkAcquireProfilingLockKHR)deviceProcAddr(device, "vkAcquireProfilingLockKHR");
    pOut->vkReleaseProfilingLockKHR = (PFN_vkReleaseProfilingLockKHR)deviceProcAddr(device, "vkReleaseProfilingLockKHR");
#endif // defined(VK_KHR_performance_query) 
#if defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
    pOut->vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)deviceProcAddr(device, "vkSetDebugUtilsObjectNameEXT");
    pOut->vkSetDebugUtilsObjectTagEXT = (PFN_vkSetDebugUtilsObjectTagEXT)deviceProcAddr(device, "vkSetDebugUtilsObjectTagEXT");
    pOut->vkQueueBeginDebugUtilsLabelEXT = (PFN_vkQueueBeginDebugUtilsLabelEXT)deviceProcAddr(device, "vkQueueBeginDebugUtilsLabelEXT");
    pOut->vkQueueEndDebugUtilsLabelEXT = (PFN_vkQueueEndDebugUtilsLabelEXT)deviceProcAddr(device, "vkQueueEndDebugUtilsLabelEXT");
    pOut->vkQueueInsertDebugUtilsLabelEXT = (PFN_vkQueueInsertDebugUtilsLabelEXT)deviceProcAddr(device, "vkQueueInsertDebugUtilsLabelEXT");
    pOut->vkCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)deviceProcAddr(device, "vkCmdBeginDebugUtilsLabelEXT");
    pOut->vkCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)deviceProcAddr(device, "vkCmdEndDebugUtilsLabelEXT");
    pOut->vkCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)deviceProcAddr(device, "vkCmdInsertDebugUtilsLabelEXT");
#endif // defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
#if defined(VK_ANDROID_external_memory_android_hardware_buffer) && VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION >= 5 
    pOut->vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID)deviceProcAddr(device, "vkGetAndroidHardwareBufferPropertiesANDROID");
    pOut->vkGetMemoryAndroidHardwareBufferANDROID = (PFN_vkGetMemoryAndroidHardwareBufferANDROID)deviceProcAddr(device, "vkGetMemoryAndroidHardwareBufferANDROID");
#endif // defined(VK_ANDROID_external_memory_android_hardware_buffer) && VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION >= 5 
#if defined(VK_AMD_gpa_interface) 
    pOut->vkCreateGpaSessionAMD = (PFN_vkCreateGpaSessionAMD)deviceProcAddr(device, "vkCreateGpaSessionAMD");
    pOut->vkDestroyGpaSessionAMD = (PFN_vkDestroyGpaSessionAMD)deviceProcAddr(device, "vkDestroyGpaSessionAMD");
    pOut->vkSetGpaDeviceClockModeAMD = (PFN_vkSetGpaDeviceClockModeAMD)deviceProcAddr(device, "vkSetGpaDeviceClockModeAMD");
    pOut->vkGetGpaDeviceClockInfoAMD = (PFN_vkGetGpaDeviceClockInfoAMD)deviceProcAddr(device, "vkGetGpaDeviceClockInfoAMD");
    pOut->vkCmdBeginGpaSessionAMD = (PFN_vkCmdBeginGpaSessionAMD)deviceProcAddr(device, "vkCmdBeginGpaSessionAMD");
    pOut->vkCmdEndGpaSessionAMD = (PFN_vkCmdEndGpaSessionAMD)deviceProcAddr(device, "vkCmdEndGpaSessionAMD");
    pOut->vkCmdBeginGpaSampleAMD = (PFN_vkCmdBeginGpaSampleAMD)deviceProcAddr(device, "vkCmdBeginGpaSampleAMD");
    pOut->vkCmdEndGpaSampleAMD = (PFN_vkCmdEndGpaSampleAMD)deviceProcAddr(device, "vkCmdEndGpaSampleAMD");
    pOut->vkGetGpaSessionStatusAMD = (PFN_vkGetGpaSessionStatusAMD)deviceProcAddr(device, "vkGetGpaSessionStatusAMD");
    pOut->vkGetGpaSessionResultsAMD = (PFN_vkGetGpaSessionResultsAMD)deviceProcAddr(device, "vkGetGpaSessionResultsAMD");
    pOut->vkResetGpaSessionAMD = (PFN_vkResetGpaSessionAMD)deviceProcAddr(device, "vkResetGpaSessionAMD");
    pOut->vkCmdCopyGpaSessionResultsAMD = (PFN_vkCmdCopyGpaSessionResultsAMD)deviceProcAddr(device, "vkCmdCopyGpaSessionResultsAMD");
#endif // defined(VK_AMD_gpa_interface) 
#if defined(VK_AMDX_shader_enqueue) && VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION >= 2 
    pOut->vkCreateExecutionGraphPipelinesAMDX = (PFN_vkCreateExecutionGraphPipelinesAMDX)deviceProcAddr(device, "vkCreateExecutionGraphPipelinesAMDX");
    pOut->vkGetExecutionGraphPipelineScratchSizeAMDX = (PFN_vkGetExecutionGraphPipelineScratchSizeAMDX)deviceProcAddr(device, "vkGetExecutionGraphPipelineScratchSizeAMDX");
    pOut->vkGetExecutionGraphPipelineNodeIndexAMDX = (PFN_vkGetExecutionGraphPipelineNodeIndexAMDX)deviceProcAddr(device, "vkGetExecutionGraphPipelineNodeIndexAMDX");
    pOut->vkCmdInitializeGraphScratchMemoryAMDX = (PFN_vkCmdInitializeGraphScratchMemoryAMDX)deviceProcAddr(device, "vkCmdInitializeGraphScratchMemoryAMDX");
    pOut->vkCmdDispatchGraphAMDX = (PFN_vkCmdDispatchGraphAMDX)deviceProcAddr(device, "vkCmdDispatchGraphAMDX");
    pOut->vkCmdDispatchGraphIndirectAMDX = (PFN_vkCmdDispatchGraphIndirectAMDX)deviceProcAddr(device, "vkCmdDispatchGraphIndirectAMDX");
    pOut->vkCmdDispatchGraphIndirectCountAMDX = (PFN_vkCmdDispatchGraphIndirectCountAMDX)deviceProcAddr(device, "vkCmdDispatchGraphIndirectCountAMDX");
#endif // defined(VK_AMDX_shader_enqueue) && VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION >= 2 
#if defined(VK_EXT_descriptor_heap) 
    pOut->vkWriteSamplerDescriptorsEXT = (PFN_vkWriteSamplerDescriptorsEXT)deviceProcAddr(device, "vkWriteSamplerDescriptorsEXT");
    pOut->vkWriteResourceDescriptorsEXT = (PFN_vkWriteResourceDescriptorsEXT)deviceProcAddr(device, "vkWriteResourceDescriptorsEXT");
    pOut->vkCmdBindSamplerHeapEXT = (PFN_vkCmdBindSamplerHeapEXT)deviceProcAddr(device, "vkCmdBindSamplerHeapEXT");
    pOut->vkCmdBindResourceHeapEXT = (PFN_vkCmdBindResourceHeapEXT)deviceProcAddr(device, "vkCmdBindResourceHeapEXT");
    pOut->vkCmdPushDataEXT = (PFN_vkCmdPushDataEXT)deviceProcAddr(device, "vkCmdPushDataEXT");
    pOut->vkGetImageOpaqueCaptureDataEXT = (PFN_vkGetImageOpaqueCaptureDataEXT)deviceProcAddr(device, "vkGetImageOpaqueCaptureDataEXT");
#endif // defined(VK_EXT_descriptor_heap) 
#if defined(VK_EXT_descriptor_heap) && defined(VK_EXT_custom_border_color) 
    pOut->vkRegisterCustomBorderColorEXT = (PFN_vkRegisterCustomBorderColorEXT)deviceProcAddr(device, "vkRegisterCustomBorderColorEXT");
    pOut->vkUnregisterCustomBorderColorEXT = (PFN_vkUnregisterCustomBorderColorEXT)deviceProcAddr(device, "vkUnregisterCustomBorderColorEXT");
#endif // defined(VK_EXT_descriptor_heap) && defined(VK_EXT_custom_border_color) 
#if defined(VK_EXT_descriptor_heap) && defined(VK_ARM_tensors) 
    pOut->vkGetTensorOpaqueCaptureDataARM = (PFN_vkGetTensorOpaqueCaptureDataARM)deviceProcAddr(device, "vkGetTensorOpaqueCaptureDataARM");
#endif // defined(VK_EXT_descriptor_heap) && defined(VK_ARM_tensors) 
#if defined(VK_EXT_sample_locations) 
    pOut->vkCmdSetSampleLocationsEXT = (PFN_vkCmdSetSampleLocationsEXT)deviceProcAddr(device, "vkCmdSetSampleLocationsEXT");
#endif // defined(VK_EXT_sample_locations) 
#if defined(VK_KHR_acceleration_structure) 
    pOut->vkCreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR)deviceProcAddr(device, "vkCreateAccelerationStructureKHR");
    pOut->vkDestroyAccelerationStructureKHR = (PFN_vkDestroyAccelerationStructureKHR)deviceProcAddr(device, "vkDestroyAccelerationStructureKHR");
    pOut->vkCmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR)deviceProcAddr(device, "vkCmdBuildAccelerationStructuresKHR");
    pOut->vkCmdBuildAccelerationStructuresIndirectKHR = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR)deviceProcAddr(device, "vkCmdBuildAccelerationStructuresIndirectKHR");
    pOut->vkBuildAccelerationStructuresKHR = (PFN_vkBuildAccelerationStructuresKHR)deviceProcAddr(device, "vkBuildAccelerationStructuresKHR");
    pOut->vkCopyAccelerationStructureKHR = (PFN_vkCopyAccelerationStructureKHR)deviceProcAddr(device, "vkCopyAccelerationStructureKHR");
    pOut->vkCopyAccelerationStructureToMemoryKHR = (PFN_vkCopyAccelerationStructureToMemoryKHR)deviceProcAddr(device, "vkCopyAccelerationStructureToMemoryKHR");
    pOut->vkCopyMemoryToAccelerationStructureKHR = (PFN_vkCopyMemoryToAccelerationStructureKHR)deviceProcAddr(device, "vkCopyMemoryToAccelerationStructureKHR");
    pOut->vkWriteAccelerationStructuresPropertiesKHR = (PFN_vkWriteAccelerationStructuresPropertiesKHR)deviceProcAddr(device, "vkWriteAccelerationStructuresPropertiesKHR");
    pOut->vkCmdCopyAccelerationStructureKHR = (PFN_vkCmdCopyAccelerationStructureKHR)deviceProcAddr(device, "vkCmdCopyAccelerationStructureKHR");
    pOut->vkCmdCopyAccelerationStructureToMemoryKHR = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR)deviceProcAddr(device, "vkCmdCopyAccelerationStructureToMemoryKHR");
    pOut->vkCmdCopyMemoryToAccelerationStructureKHR = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR)deviceProcAddr(device, "vkCmdCopyMemoryToAccelerationStructureKHR");
    pOut->vkGetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR)deviceProcAddr(device, "vkGetAccelerationStructureDeviceAddressKHR");
    pOut->vkCmdWriteAccelerationStructuresPropertiesKHR = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR)deviceProcAddr(device, "vkCmdWriteAccelerationStructuresPropertiesKHR");
    pOut->vkGetDeviceAccelerationStructureCompatibilityKHR = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR)deviceProcAddr(device, "vkGetDeviceAccelerationStructureCompatibilityKHR");
    pOut->vkGetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR)deviceProcAddr(device, "vkGetAccelerationStructureBuildSizesKHR");
#endif // defined(VK_KHR_acceleration_structure) 
#if defined(VK_KHR_ray_tracing_pipeline) 
    pOut->vkCmdTraceRaysKHR = (PFN_vkCmdTraceRaysKHR)deviceProcAddr(device, "vkCmdTraceRaysKHR");
    pOut->vkCreateRayTracingPipelinesKHR = (PFN_vkCreateRayTracingPipelinesKHR)deviceProcAddr(device, "vkCreateRayTracingPipelinesKHR");
    pOut->vkGetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR)deviceProcAddr(device, "vkGetRayTracingShaderGroupHandlesKHR");
    pOut->vkGetRayTracingCaptureReplayShaderGroupHandlesKHR = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR)deviceProcAddr(device, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR");
    pOut->vkCmdTraceRaysIndirectKHR = (PFN_vkCmdTraceRaysIndirectKHR)deviceProcAddr(device, "vkCmdTraceRaysIndirectKHR");
    pOut->vkGetRayTracingShaderGroupStackSizeKHR = (PFN_vkGetRayTracingShaderGroupStackSizeKHR)deviceProcAddr(device, "vkGetRayTracingShaderGroupStackSizeKHR");
    pOut->vkCmdSetRayTracingPipelineStackSizeKHR = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR)deviceProcAddr(device, "vkCmdSetRayTracingPipelineStackSizeKHR");
#endif // defined(VK_KHR_ray_tracing_pipeline) 
#if defined(VK_EXT_image_drm_format_modifier) && VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION >= 2 
    pOut->vkGetImageDrmFormatModifierPropertiesEXT = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)deviceProcAddr(device, "vkGetImageDrmFormatModifierPropertiesEXT");
#endif // defined(VK_EXT_image_drm_format_modifier) && VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION >= 2 
#if defined(VK_EXT_validation_cache) 
    pOut->vkCreateValidationCacheEXT = (PFN_vkCreateValidationCacheEXT)deviceProcAddr(device, "vkCreateValidationCacheEXT");
    pOut->vkDestroyValidationCacheEXT = (PFN_vkDestroyValidationCacheEXT)deviceProcAddr(device, "vkDestroyValidationCacheEXT");
    pOut->vkMergeValidationCachesEXT = (PFN_vkMergeValidationCachesEXT)deviceProcAddr(device, "vkMergeValidationCachesEXT");
    pOut->vkGetValidationCacheDataEXT = (PFN_vkGetValidationCacheDataEXT)deviceProcAddr(device, "vkGetValidationCacheDataEXT");
#endif // defined(VK_EXT_validation_cache) 
#if defined(VK_NV_shading_rate_image) && VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION >= 3 
    pOut->vkCmdBindShadingRateImageNV = (PFN_vkCmdBindShadingRateImageNV)deviceProcAddr(device, "vkCmdBindShadingRateImageNV");
    pOut->vkCmdSetViewportShadingRatePaletteNV = (PFN_vkCmdSetViewportShadingRatePaletteNV)deviceProcAddr(device, "vkCmdSetViewportShadingRatePaletteNV");
    pOut->vkCmdSetCoarseSampleOrderNV = (PFN_vkCmdSetCoarseSampleOrderNV)deviceProcAddr(device, "vkCmdSetCoarseSampleOrderNV");
#endif // defined(VK_NV_shading_rate_image) && VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION >= 3 
#if defined(VK_EXT_external_memory_host) 
    pOut->vkGetMemoryHostPointerPropertiesEXT = (PFN_vkGetMemoryHostPointerPropertiesEXT)deviceProcAddr(device, "vkGetMemoryHostPointerPropertiesEXT");
#endif // defined(VK_EXT_external_memory_host) 
#if defined(VK_AMD_buffer_marker) 
    pOut->vkCmdWriteBufferMarkerAMD = (PFN_vkCmdWriteBufferMarkerAMD)deviceProcAddr(device, "vkCmdWriteBufferMarkerAMD");
#endif // defined(VK_AMD_buffer_marker) 
#if defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
    pOut->vkCmdWriteBufferMarker2AMD = (PFN_vkCmdWriteBufferMarker2AMD)deviceProcAddr(device, "vkCmdWriteBufferMarker2AMD");
#endif // defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
#if defined(VK_NV_mesh_shader) 
    pOut->vkCmdDrawMeshTasksNV = (PFN_vkCmdDrawMeshTasksNV)deviceProcAddr(device, "vkCmdDrawMeshTasksNV");
    pOut->vkCmdDrawMeshTasksIndirectNV = (PFN_vkCmdDrawMeshTasksIndirectNV)deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectNV");
#endif // defined(VK_NV_mesh_shader) 
#if defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) 
    pOut->vkCmdDrawMeshTasksIndirectCountNV = (PFN_vkCmdDrawMeshTasksIndirectCountNV)deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectCountNV");
#endif // defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) 
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 
    pOut->vkCmdSetExclusiveScissorEnableNV = (PFN_vkCmdSetExclusiveScissorEnableNV)deviceProcAddr(device, "vkCmdSetExclusiveScissorEnableNV");
    pOut->vkCmdSetExclusiveScissorNV = (PFN_vkCmdSetExclusiveScissorNV)deviceProcAddr(device, "vkCmdSetExclusiveScissorNV");
#endif // defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 
#if defined(VK_NV_device_diagnostic_checkpoints) && VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION >= 2 
    pOut->vkCmdSetCheckpointNV = (PFN_vkCmdSetCheckpointNV)deviceProcAddr(device, "vkCmdSetCheckpointNV");
    pOut->vkGetQueueCheckpointDataNV = (PFN_vkGetQueueCheckpointDataNV)deviceProcAddr(device, "vkGetQueueCheckpointDataNV");
#endif // defined(VK_NV_device_diagnostic_checkpoints) && VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION >= 2 
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
    pOut->vkGetQueueCheckpointData2NV = (PFN_vkGetQueueCheckpointData2NV)deviceProcAddr(device, "vkGetQueueCheckpointData2NV");
#endif // defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
#if defined(VK_EXT_present_timing) && VK_EXT_PRESENT_TIMING_SPEC_VERSION >= 3 
    pOut->vkSetSwapchainPresentTimingQueueSizeEXT = (PFN_vkSetSwapchainPresentTimingQueueSizeEXT)deviceProcAddr(device, "vkSetSwapchainPresentTimingQueueSizeEXT");
    pOut->vkGetSwapchainTimingPropertiesEXT = (PFN_vkGetSwapchainTimingPropertiesEXT)deviceProcAddr(device, "vkGetSwapchainTimingPropertiesEXT");
    pOut->vkGetSwapchainTimeDomainPropertiesEXT = (PFN_vkGetSwapchainTimeDomainPropertiesEXT)deviceProcAddr(device, "vkGetSwapchainTimeDomainPropertiesEXT");
    pOut->vkGetPastPresentationTimingEXT = (PFN_vkGetPastPresentationTimingEXT)deviceProcAddr(device, "vkGetPastPresentationTimingEXT");
#endif // defined(VK_EXT_present_timing) && VK_EXT_PRESENT_TIMING_SPEC_VERSION >= 3 
#if defined(VK_INTEL_performance_query) && VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION >= 2 
    pOut->vkInitializePerformanceApiINTEL = (PFN_vkInitializePerformanceApiINTEL)deviceProcAddr(device, "vkInitializePerformanceApiINTEL");
    pOut->vkUninitializePerformanceApiINTEL = (PFN_vkUninitializePerformanceApiINTEL)deviceProcAddr(device, "vkUninitializePerformanceApiINTEL");
    pOut->vkCmdSetPerformanceMarkerINTEL = (PFN_vkCmdSetPerformanceMarkerINTEL)deviceProcAddr(device, "vkCmdSetPerformanceMarkerINTEL");
    pOut->vkCmdSetPerformanceStreamMarkerINTEL = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)deviceProcAddr(device, "vkCmdSetPerformanceStreamMarkerINTEL");
    pOut->vkCmdSetPerformanceOverrideINTEL = (PFN_vkCmdSetPerformanceOverrideINTEL)deviceProcAddr(device, "vkCmdSetPerformanceOverrideINTEL");
    pOut->vkAcquirePerformanceConfigurationINTEL = (PFN_vkAcquirePerformanceConfigurationINTEL)deviceProcAddr(device, "vkAcquirePerformanceConfigurationINTEL");
    pOut->vkReleasePerformanceConfigurationINTEL = (PFN_vkReleasePerformanceConfigurationINTEL)deviceProcAddr(device, "vkReleasePerformanceConfigurationINTEL");
    pOut->vkQueueSetPerformanceConfigurationINTEL = (PFN_vkQueueSetPerformanceConfigurationINTEL)deviceProcAddr(device, "vkQueueSetPerformanceConfigurationINTEL");
    pOut->vkGetPerformanceParameterINTEL = (PFN_vkGetPerformanceParameterINTEL)deviceProcAddr(device, "vkGetPerformanceParameterINTEL");
#endif // defined(VK_INTEL_performance_query) && VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION >= 2 
#if defined(VK_AMD_display_native_hdr) 
    pOut->vkSetLocalDimmingAMD = (PFN_vkSetLocalDimmingAMD)deviceProcAddr(device, "vkSetLocalDimmingAMD");
#endif // defined(VK_AMD_display_native_hdr) 
#if defined(VK_KHR_fragment_shading_rate) 
    pOut->vkCmdSetFragmentShadingRateKHR = (PFN_vkCmdSetFragmentShadingRateKHR)deviceProcAddr(device, "vkCmdSetFragmentShadingRateKHR");
#endif // defined(VK_KHR_fragment_shading_rate) 
#if defined(VK_KHR_present_wait) 
    pOut->vkWaitForPresentKHR = (PFN_vkWaitForPresentKHR)deviceProcAddr(device, "vkWaitForPresentKHR");
#endif // defined(VK_KHR_present_wait) 
#if defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
    pOut->vkAcquireFullScreenExclusiveModeEXT = (PFN_vkAcquireFullScreenExclusiveModeEXT)deviceProcAddr(device, "vkAcquireFullScreenExclusiveModeEXT");
    pOut->vkReleaseFullScreenExclusiveModeEXT = (PFN_vkReleaseFullScreenExclusiveModeEXT)deviceProcAddr(device, "vkReleaseFullScreenExclusiveModeEXT");
#endif // defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) 
    pOut->vkGetDeviceGroupSurfacePresentModes2EXT = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT)deviceProcAddr(device, "vkGetDeviceGroupSurfacePresentModes2EXT");
#endif // defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) 
#if defined(VK_KHR_deferred_host_operations) 
    pOut->vkCreateDeferredOperationKHR = (PFN_vkCreateDeferredOperationKHR)deviceProcAddr(device, "vkCreateDeferredOperationKHR");
    pOut->vkDestroyDeferredOperationKHR = (PFN_vkDestroyDeferredOperationKHR)deviceProcAddr(device, "vkDestroyDeferredOperationKHR");
    pOut->vkGetDeferredOperationMaxConcurrencyKHR = (PFN_vkGetDeferredOperationMaxConcurrencyKHR)deviceProcAddr(device, "vkGetDeferredOperationMaxConcurrencyKHR");
    pOut->vkGetDeferredOperationResultKHR = (PFN_vkGetDeferredOperationResultKHR)deviceProcAddr(device, "vkGetDeferredOperationResultKHR");
    pOut->vkDeferredOperationJoinKHR = (PFN_vkDeferredOperationJoinKHR)deviceProcAddr(device, "vkDeferredOperationJoinKHR");
#endif // defined(VK_KHR_deferred_host_operations) 
#if defined(VK_KHR_pipeline_executable_properties) 
    pOut->vkGetPipelineExecutablePropertiesKHR = (PFN_vkGetPipelineExecutablePropertiesKHR)deviceProcAddr(device, "vkGetPipelineExecutablePropertiesKHR");
    pOut->vkGetPipelineExecutableStatisticsKHR = (PFN_vkGetPipelineExecutableStatisticsKHR)deviceProcAddr(device, "vkGetPipelineExecutableStatisticsKHR");
    pOut->vkGetPipelineExecutableInternalRepresentationsKHR = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)deviceProcAddr(device, "vkGetPipelineExecutableInternalRepresentationsKHR");
#endif // defined(VK_KHR_pipeline_executable_properties) 
#if defined(VK_NV_device_generated_commands) && VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION >= 3 
    pOut->vkGetGeneratedCommandsMemoryRequirementsNV = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV)deviceProcAddr(device, "vkGetGeneratedCommandsMemoryRequirementsNV");
    pOut->vkCmdPreprocessGeneratedCommandsNV = (PFN_vkCmdPreprocessGeneratedCommandsNV)deviceProcAddr(device, "vkCmdPreprocessGeneratedCommandsNV");
    pOut->vkCmdExecuteGeneratedCommandsNV = (PFN_vkCmdExecuteGeneratedCommandsNV)deviceProcAddr(device, "vkCmdExecuteGeneratedCommandsNV");
    pOut->vkCmdBindPipelineShaderGroupNV = (PFN_vkCmdBindPipelineShaderGroupNV)deviceProcAddr(device, "vkCmdBindPipelineShaderGroupNV");
    pOut->vkCreateIndirectCommandsLayoutNV = (PFN_vkCreateIndirectCommandsLayoutNV)deviceProcAddr(device, "vkCreateIndirectCommandsLayoutNV");
    pOut->vkDestroyIndirectCommandsLayoutNV = (PFN_vkDestroyIndirectCommandsLayoutNV)deviceProcAddr(device, "vkDestroyIndirectCommandsLayoutNV");
#endif // defined(VK_NV_device_generated_commands) && VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION >= 3 
#if defined(VK_EXT_depth_bias_control) 
    pOut->vkCmdSetDepthBias2EXT = (PFN_vkCmdSetDepthBias2EXT)deviceProcAddr(device, "vkCmdSetDepthBias2EXT");
#endif // defined(VK_EXT_depth_bias_control) 
#if defined(VK_KHR_video_encode_queue) 
    pOut->vkGetEncodedVideoSessionParametersKHR = (PFN_vkGetEncodedVideoSessionParametersKHR)deviceProcAddr(device, "vkGetEncodedVideoSessionParametersKHR");
    pOut->vkCmdEncodeVideoKHR = (PFN_vkCmdEncodeVideoKHR)deviceProcAddr(device, "vkCmdEncodeVideoKHR");
#endif // defined(VK_KHR_video_encode_queue) 
#if defined(VK_QCOM_queue_perf_hint) 
    pOut->vkQueueSetPerfHintQCOM = (PFN_vkQueueSetPerfHintQCOM)deviceProcAddr(device, "vkQueueSetPerfHintQCOM");
#endif // defined(VK_QCOM_queue_perf_hint) 
#if defined(VK_NV_cuda_kernel_launch) && VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION >= 2 
    pOut->vkCreateCudaModuleNV = (PFN_vkCreateCudaModuleNV)deviceProcAddr(device, "vkCreateCudaModuleNV");
    pOut->vkGetCudaModuleCacheNV = (PFN_vkGetCudaModuleCacheNV)deviceProcAddr(device, "vkGetCudaModuleCacheNV");
    pOut->vkCreateCudaFunctionNV = (PFN_vkCreateCudaFunctionNV)deviceProcAddr(device, "vkCreateCudaFunctionNV");
    pOut->vkDestroyCudaModuleNV = (PFN_vkDestroyCudaModuleNV)deviceProcAddr(device, "vkDestroyCudaModuleNV");
    pOut->vkDestroyCudaFunctionNV = (PFN_vkDestroyCudaFunctionNV)deviceProcAddr(device, "vkDestroyCudaFunctionNV");
    pOut->vkCmdCudaLaunchKernelNV = (PFN_vkCmdCudaLaunchKernelNV)deviceProcAddr(device, "vkCmdCudaLaunchKernelNV");
#endif // defined(VK_NV_cuda_kernel_launch) && VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION >= 2 
#if defined(VK_QCOM_tile_shading) && VK_QCOM_TILE_SHADING_SPEC_VERSION >= 2 
    pOut->vkCmdDispatchTileQCOM = (PFN_vkCmdDispatchTileQCOM)deviceProcAddr(device, "vkCmdDispatchTileQCOM");
    pOut->vkCmdBeginPerTileExecutionQCOM = (PFN_vkCmdBeginPerTileExecutionQCOM)deviceProcAddr(device, "vkCmdBeginPerTileExecutionQCOM");
    pOut->vkCmdEndPerTileExecutionQCOM = (PFN_vkCmdEndPerTileExecutionQCOM)deviceProcAddr(device, "vkCmdEndPerTileExecutionQCOM");
#endif // defined(VK_QCOM_tile_shading) && VK_QCOM_TILE_SHADING_SPEC_VERSION >= 2 
#if defined(VK_EXT_metal_objects) && VK_EXT_METAL_OBJECTS_SPEC_VERSION >= 2 
    pOut->vkExportMetalObjectsEXT = (PFN_vkExportMetalObjectsEXT)deviceProcAddr(device, "vkExportMetalObjectsEXT");
#endif // defined(VK_EXT_metal_objects) && VK_EXT_METAL_OBJECTS_SPEC_VERSION >= 2 
#if defined(VK_KHR_device_address_commands) 
    pOut->vkCmdBindIndexBuffer3KHR = (PFN_vkCmdBindIndexBuffer3KHR)deviceProcAddr(device, "vkCmdBindIndexBuffer3KHR");
    pOut->vkCmdBindVertexBuffers3KHR = (PFN_vkCmdBindVertexBuffers3KHR)deviceProcAddr(device, "vkCmdBindVertexBuffers3KHR");
    pOut->vkCmdDrawIndirect2KHR = (PFN_vkCmdDrawIndirect2KHR)deviceProcAddr(device, "vkCmdDrawIndirect2KHR");
    pOut->vkCmdDrawIndexedIndirect2KHR = (PFN_vkCmdDrawIndexedIndirect2KHR)deviceProcAddr(device, "vkCmdDrawIndexedIndirect2KHR");
    pOut->vkCmdDispatchIndirect2KHR = (PFN_vkCmdDispatchIndirect2KHR)deviceProcAddr(device, "vkCmdDispatchIndirect2KHR");
    pOut->vkCmdCopyMemoryKHR = (PFN_vkCmdCopyMemoryKHR)deviceProcAddr(device, "vkCmdCopyMemoryKHR");
    pOut->vkCmdCopyMemoryToImageKHR = (PFN_vkCmdCopyMemoryToImageKHR)deviceProcAddr(device, "vkCmdCopyMemoryToImageKHR");
    pOut->vkCmdCopyImageToMemoryKHR = (PFN_vkCmdCopyImageToMemoryKHR)deviceProcAddr(device, "vkCmdCopyImageToMemoryKHR");
    pOut->vkCmdUpdateMemoryKHR = (PFN_vkCmdUpdateMemoryKHR)deviceProcAddr(device, "vkCmdUpdateMemoryKHR");
    pOut->vkCmdFillMemoryKHR = (PFN_vkCmdFillMemoryKHR)deviceProcAddr(device, "vkCmdFillMemoryKHR");
    pOut->vkCmdCopyQueryPoolResultsToMemoryKHR = (PFN_vkCmdCopyQueryPoolResultsToMemoryKHR)deviceProcAddr(device, "vkCmdCopyQueryPoolResultsToMemoryKHR");
#endif // defined(VK_KHR_device_address_commands) 
#if defined(VK_KHR_device_address_commands) && (defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2)) 
    pOut->vkCmdDrawIndirectCount2KHR = (PFN_vkCmdDrawIndirectCount2KHR)deviceProcAddr(device, "vkCmdDrawIndirectCount2KHR");
    pOut->vkCmdDrawIndexedIndirectCount2KHR = (PFN_vkCmdDrawIndexedIndirectCount2KHR)deviceProcAddr(device, "vkCmdDrawIndexedIndirectCount2KHR");
#endif // defined(VK_KHR_device_address_commands) && (defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2)) 
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_conditional_rendering) 
    pOut->vkCmdBeginConditionalRendering2EXT = (PFN_vkCmdBeginConditionalRendering2EXT)deviceProcAddr(device, "vkCmdBeginConditionalRendering2EXT");
#endif // defined(VK_KHR_device_address_commands) && defined(VK_EXT_conditional_rendering) 
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_transform_feedback) 
    pOut->vkCmdBindTransformFeedbackBuffers2EXT = (PFN_vkCmdBindTransformFeedbackBuffers2EXT)deviceProcAddr(device, "vkCmdBindTransformFeedbackBuffers2EXT");
    pOut->vkCmdBeginTransformFeedback2EXT = (PFN_vkCmdBeginTransformFeedback2EXT)deviceProcAddr(device, "vkCmdBeginTransformFeedback2EXT");
    pOut->vkCmdEndTransformFeedback2EXT = (PFN_vkCmdEndTransformFeedback2EXT)deviceProcAddr(device, "vkCmdEndTransformFeedback2EXT");
    pOut->vkCmdDrawIndirectByteCount2EXT = (PFN_vkCmdDrawIndirectByteCount2EXT)deviceProcAddr(device, "vkCmdDrawIndirectByteCount2EXT");
#endif // defined(VK_KHR_device_address_commands) && defined(VK_EXT_transform_feedback) 
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_mesh_shader) 
    pOut->vkCmdDrawMeshTasksIndirect2EXT = (PFN_vkCmdDrawMeshTasksIndirect2EXT)deviceProcAddr(device, "vkCmdDrawMeshTasksIndirect2EXT");
#endif // defined(VK_KHR_device_address_commands) && defined(VK_EXT_mesh_shader) 
#if defined(VK_KHR_device_address_commands) && ((defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2)) && defined(VK_EXT_mesh_shader)) 
    pOut->vkCmdDrawMeshTasksIndirectCount2EXT = (PFN_vkCmdDrawMeshTasksIndirectCount2EXT)deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectCount2EXT");
#endif // defined(VK_KHR_device_address_commands) && ((defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2)) && defined(VK_EXT_mesh_shader)) 
#if defined(VK_KHR_device_address_commands) && defined(VK_AMD_buffer_marker) 
    pOut->vkCmdWriteMarkerToMemoryAMD = (PFN_vkCmdWriteMarkerToMemoryAMD)deviceProcAddr(device, "vkCmdWriteMarkerToMemoryAMD");
#endif // defined(VK_KHR_device_address_commands) && defined(VK_AMD_buffer_marker) 
#if defined(VK_KHR_device_address_commands) && defined(VK_KHR_acceleration_structure) 
    pOut->vkCreateAccelerationStructure2KHR = (PFN_vkCreateAccelerationStructure2KHR)deviceProcAddr(device, "vkCreateAccelerationStructure2KHR");
#endif // defined(VK_KHR_device_address_commands) && defined(VK_KHR_acceleration_structure) 
#if defined(VK_NV_fragment_shading_rate_enums) 
    pOut->vkCmdSetFragmentShadingRateEnumNV = (PFN_vkCmdSetFragmentShadingRateEnumNV)deviceProcAddr(device, "vkCmdSetFragmentShadingRateEnumNV");
#endif // defined(VK_NV_fragment_shading_rate_enums) 
#if defined(VK_EXT_mesh_shader) 
    pOut->vkCmdDrawMeshTasksEXT = (PFN_vkCmdDrawMeshTasksEXT)deviceProcAddr(device, "vkCmdDrawMeshTasksEXT");
    pOut->vkCmdDrawMeshTasksIndirectEXT = (PFN_vkCmdDrawMeshTasksIndirectEXT)deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectEXT");
#endif // defined(VK_EXT_mesh_shader) 
#if defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) 
    pOut->vkCmdDrawMeshTasksIndirectCountEXT = (PFN_vkCmdDrawMeshTasksIndirectCountEXT)deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectCountEXT");
#endif // defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) 
#if (defined(VK_EXT_vertex_input_dynamic_state) && VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
    pOut->vkCmdSetVertexInputEXT = (PFN_vkCmdSetVertexInputEXT)deviceProcAddr(device, "vkCmdSetVertexInputEXT");
#endif // (defined(VK_EXT_vertex_input_dynamic_state) && VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
#if defined(VK_FUCHSIA_external_memory) 
    pOut->vkGetMemoryZirconHandleFUCHSIA = (PFN_vkGetMemoryZirconHandleFUCHSIA)deviceProcAddr(device, "vkGetMemoryZirconHandleFUCHSIA");
    pOut->vkGetMemoryZirconHandlePropertiesFUCHSIA = (PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA)deviceProcAddr(device, "vkGetMemoryZirconHandlePropertiesFUCHSIA");
#endif // defined(VK_FUCHSIA_external_memory) 
#if defined(VK_FUCHSIA_external_semaphore) 
    pOut->vkImportSemaphoreZirconHandleFUCHSIA = (PFN_vkImportSemaphoreZirconHandleFUCHSIA)deviceProcAddr(device, "vkImportSemaphoreZirconHandleFUCHSIA");
    pOut->vkGetSemaphoreZirconHandleFUCHSIA = (PFN_vkGetSemaphoreZirconHandleFUCHSIA)deviceProcAddr(device, "vkGetSemaphoreZirconHandleFUCHSIA");
#endif // defined(VK_FUCHSIA_external_semaphore) 
#if defined(VK_FUCHSIA_buffer_collection) && VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION >= 2 
    pOut->vkCreateBufferCollectionFUCHSIA = (PFN_vkCreateBufferCollectionFUCHSIA)deviceProcAddr(device, "vkCreateBufferCollectionFUCHSIA");
    pOut->vkSetBufferCollectionImageConstraintsFUCHSIA = (PFN_vkSetBufferCollectionImageConstraintsFUCHSIA)deviceProcAddr(device, "vkSetBufferCollectionImageConstraintsFUCHSIA");
    pOut->vkSetBufferCollectionBufferConstraintsFUCHSIA = (PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA)deviceProcAddr(device, "vkSetBufferCollectionBufferConstraintsFUCHSIA");
    pOut->vkDestroyBufferCollectionFUCHSIA = (PFN_vkDestroyBufferCollectionFUCHSIA)deviceProcAddr(device, "vkDestroyBufferCollectionFUCHSIA");
    pOut->vkGetBufferCollectionPropertiesFUCHSIA = (PFN_vkGetBufferCollectionPropertiesFUCHSIA)deviceProcAddr(device, "vkGetBufferCollectionPropertiesFUCHSIA");
#endif // defined(VK_FUCHSIA_buffer_collection) && VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION >= 2 
#if defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 3 
    pOut->vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI)deviceProcAddr(device, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI");
    pOut->vkCmdSubpassShadingHUAWEI = (PFN_vkCmdSubpassShadingHUAWEI)deviceProcAddr(device, "vkCmdSubpassShadingHUAWEI");
#endif // defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 3 
#if defined(VK_HUAWEI_invocation_mask) 
    pOut->vkCmdBindInvocationMaskHUAWEI = (PFN_vkCmdBindInvocationMaskHUAWEI)deviceProcAddr(device, "vkCmdBindInvocationMaskHUAWEI");
#endif // defined(VK_HUAWEI_invocation_mask) 
#if defined(VK_NV_external_memory_rdma) 
    pOut->vkGetMemoryRemoteAddressNV = (PFN_vkGetMemoryRemoteAddressNV)deviceProcAddr(device, "vkGetMemoryRemoteAddressNV");
#endif // defined(VK_NV_external_memory_rdma) 
#if defined(VK_EXT_pipeline_properties) 
    pOut->vkGetPipelinePropertiesEXT = (PFN_vkGetPipelinePropertiesEXT)deviceProcAddr(device, "vkGetPipelinePropertiesEXT");
#endif // defined(VK_EXT_pipeline_properties) 
#if defined(VK_EXT_color_write_enable) 
    pOut->vkCmdSetColorWriteEnableEXT = (PFN_vkCmdSetColorWriteEnableEXT)deviceProcAddr(device, "vkCmdSetColorWriteEnableEXT");
#endif // defined(VK_EXT_color_write_enable) 
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) 
    pOut->vkCmdTraceRaysIndirect2KHR = (PFN_vkCmdTraceRaysIndirect2KHR)deviceProcAddr(device, "vkCmdTraceRaysIndirect2KHR");
#endif // defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) 
#if defined(VK_EXT_multi_draw) 
    pOut->vkCmdDrawMultiEXT = (PFN_vkCmdDrawMultiEXT)deviceProcAddr(device, "vkCmdDrawMultiEXT");
    pOut->vkCmdDrawMultiIndexedEXT = (PFN_vkCmdDrawMultiIndexedEXT)deviceProcAddr(device, "vkCmdDrawMultiIndexedEXT");
#endif // defined(VK_EXT_multi_draw) 
#if defined(VK_HUAWEI_cluster_culling_shader) && VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION >= 3 
    pOut->vkCmdDrawClusterHUAWEI = (PFN_vkCmdDrawClusterHUAWEI)deviceProcAddr(device, "vkCmdDrawClusterHUAWEI");
    pOut->vkCmdDrawClusterIndirectHUAWEI = (PFN_vkCmdDrawClusterIndirectHUAWEI)deviceProcAddr(device, "vkCmdDrawClusterIndirectHUAWEI");
#endif // defined(VK_HUAWEI_cluster_culling_shader) && VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION >= 3 
#if defined(VK_EXT_pageable_device_local_memory) 
    pOut->vkSetDeviceMemoryPriorityEXT = (PFN_vkSetDeviceMemoryPriorityEXT)deviceProcAddr(device, "vkSetDeviceMemoryPriorityEXT");
#endif // defined(VK_EXT_pageable_device_local_memory) 
#if defined(VK_ARM_scheduling_controls) && VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION >= 2 
    pOut->vkCmdSetDispatchParametersARM = (PFN_vkCmdSetDispatchParametersARM)deviceProcAddr(device, "vkCmdSetDispatchParametersARM");
#endif // defined(VK_ARM_scheduling_controls) && VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION >= 2 
#if defined(VK_VALVE_descriptor_set_host_mapping) 
    pOut->vkGetDescriptorSetLayoutHostMappingInfoVALVE = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE)deviceProcAddr(device, "vkGetDescriptorSetLayoutHostMappingInfoVALVE");
    pOut->vkGetDescriptorSetHostMappingVALVE = (PFN_vkGetDescriptorSetHostMappingVALVE)deviceProcAddr(device, "vkGetDescriptorSetHostMappingVALVE");
#endif // defined(VK_VALVE_descriptor_set_host_mapping) 
#if defined(VK_NV_device_generated_commands_compute) && VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION >= 2 
    pOut->vkGetPipelineIndirectMemoryRequirementsNV = (PFN_vkGetPipelineIndirectMemoryRequirementsNV)deviceProcAddr(device, "vkGetPipelineIndirectMemoryRequirementsNV");
    pOut->vkCmdUpdatePipelineIndirectBufferNV = (PFN_vkCmdUpdatePipelineIndirectBufferNV)deviceProcAddr(device, "vkCmdUpdatePipelineIndirectBufferNV");
    pOut->vkGetPipelineIndirectDeviceAddressNV = (PFN_vkGetPipelineIndirectDeviceAddressNV)deviceProcAddr(device, "vkGetPipelineIndirectDeviceAddressNV");
#endif // defined(VK_NV_device_generated_commands_compute) && VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION >= 2 
#if defined(VK_OHOS_external_memory) 
    pOut->vkGetNativeBufferPropertiesOHOS = (PFN_vkGetNativeBufferPropertiesOHOS)deviceProcAddr(device, "vkGetNativeBufferPropertiesOHOS");
    pOut->vkGetMemoryNativeBufferOHOS = (PFN_vkGetMemoryNativeBufferOHOS)deviceProcAddr(device, "vkGetMemoryNativeBufferOHOS");
#endif // defined(VK_OHOS_external_memory) 
#if (defined(VK_EXT_extended_dynamic_state3) && VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
    pOut->vkCmdSetDepthClampEnableEXT = (PFN_vkCmdSetDepthClampEnableEXT)deviceProcAddr(device, "vkCmdSetDepthClampEnableEXT");
    pOut->vkCmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)deviceProcAddr(device, "vkCmdSetPolygonModeEXT");
    pOut->vkCmdSetRasterizationSamplesEXT = (PFN_vkCmdSetRasterizationSamplesEXT)deviceProcAddr(device, "vkCmdSetRasterizationSamplesEXT");
    pOut->vkCmdSetSampleMaskEXT = (PFN_vkCmdSetSampleMaskEXT)deviceProcAddr(device, "vkCmdSetSampleMaskEXT");
    pOut->vkCmdSetAlphaToCoverageEnableEXT = (PFN_vkCmdSetAlphaToCoverageEnableEXT)deviceProcAddr(device, "vkCmdSetAlphaToCoverageEnableEXT");
    pOut->vkCmdSetAlphaToOneEnableEXT = (PFN_vkCmdSetAlphaToOneEnableEXT)deviceProcAddr(device, "vkCmdSetAlphaToOneEnableEXT");
    pOut->vkCmdSetLogicOpEnableEXT = (PFN_vkCmdSetLogicOpEnableEXT)deviceProcAddr(device, "vkCmdSetLogicOpEnableEXT");
    pOut->vkCmdSetColorBlendEnableEXT = (PFN_vkCmdSetColorBlendEnableEXT)deviceProcAddr(device, "vkCmdSetColorBlendEnableEXT");
    pOut->vkCmdSetColorBlendEquationEXT = (PFN_vkCmdSetColorBlendEquationEXT)deviceProcAddr(device, "vkCmdSetColorBlendEquationEXT");
    pOut->vkCmdSetColorWriteMaskEXT = (PFN_vkCmdSetColorWriteMaskEXT)deviceProcAddr(device, "vkCmdSetColorWriteMaskEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) 
    pOut->vkCmdSetTessellationDomainOriginEXT = (PFN_vkCmdSetTessellationDomainOriginEXT)deviceProcAddr(device, "vkCmdSetTessellationDomainOriginEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) 
    pOut->vkCmdSetRasterizationStreamEXT = (PFN_vkCmdSetRasterizationStreamEXT)deviceProcAddr(device, "vkCmdSetRasterizationStreamEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) 
    pOut->vkCmdSetConservativeRasterizationModeEXT = (PFN_vkCmdSetConservativeRasterizationModeEXT)deviceProcAddr(device, "vkCmdSetConservativeRasterizationModeEXT");
    pOut->vkCmdSetExtraPrimitiveOverestimationSizeEXT = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT)deviceProcAddr(device, "vkCmdSetExtraPrimitiveOverestimationSizeEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) 
    pOut->vkCmdSetDepthClipEnableEXT = (PFN_vkCmdSetDepthClipEnableEXT)deviceProcAddr(device, "vkCmdSetDepthClipEnableEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) 
    pOut->vkCmdSetSampleLocationsEnableEXT = (PFN_vkCmdSetSampleLocationsEnableEXT)deviceProcAddr(device, "vkCmdSetSampleLocationsEnableEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) 
    pOut->vkCmdSetColorBlendAdvancedEXT = (PFN_vkCmdSetColorBlendAdvancedEXT)deviceProcAddr(device, "vkCmdSetColorBlendAdvancedEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) 
    pOut->vkCmdSetProvokingVertexModeEXT = (PFN_vkCmdSetProvokingVertexModeEXT)deviceProcAddr(device, "vkCmdSetProvokingVertexModeEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) 
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) || (defined(VK_EXT_shader_object) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) 
    pOut->vkCmdSetLineRasterizationModeEXT = (PFN_vkCmdSetLineRasterizationModeEXT)deviceProcAddr(device, "vkCmdSetLineRasterizationModeEXT");
    pOut->vkCmdSetLineStippleEnableEXT = (PFN_vkCmdSetLineStippleEnableEXT)deviceProcAddr(device, "vkCmdSetLineStippleEnableEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) || (defined(VK_EXT_shader_object) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) 
    pOut->vkCmdSetDepthClipNegativeOneToOneEXT = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT)deviceProcAddr(device, "vkCmdSetDepthClipNegativeOneToOneEXT");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) 
    pOut->vkCmdSetViewportWScalingEnableNV = (PFN_vkCmdSetViewportWScalingEnableNV)deviceProcAddr(device, "vkCmdSetViewportWScalingEnableNV");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) 
    pOut->vkCmdSetViewportSwizzleNV = (PFN_vkCmdSetViewportSwizzleNV)deviceProcAddr(device, "vkCmdSetViewportSwizzleNV");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) 
    pOut->vkCmdSetCoverageToColorEnableNV = (PFN_vkCmdSetCoverageToColorEnableNV)deviceProcAddr(device, "vkCmdSetCoverageToColorEnableNV");
    pOut->vkCmdSetCoverageToColorLocationNV = (PFN_vkCmdSetCoverageToColorLocationNV)deviceProcAddr(device, "vkCmdSetCoverageToColorLocationNV");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) 
    pOut->vkCmdSetCoverageModulationModeNV = (PFN_vkCmdSetCoverageModulationModeNV)deviceProcAddr(device, "vkCmdSetCoverageModulationModeNV");
    pOut->vkCmdSetCoverageModulationTableEnableNV = (PFN_vkCmdSetCoverageModulationTableEnableNV)deviceProcAddr(device, "vkCmdSetCoverageModulationTableEnableNV");
    pOut->vkCmdSetCoverageModulationTableNV = (PFN_vkCmdSetCoverageModulationTableNV)deviceProcAddr(device, "vkCmdSetCoverageModulationTableNV");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) 
    pOut->vkCmdSetShadingRateImageEnableNV = (PFN_vkCmdSetShadingRateImageEnableNV)deviceProcAddr(device, "vkCmdSetShadingRateImageEnableNV");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) 
    pOut->vkCmdSetRepresentativeFragmentTestEnableNV = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV)deviceProcAddr(device, "vkCmdSetRepresentativeFragmentTestEnableNV");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) 
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) 
    pOut->vkCmdSetCoverageReductionModeNV = (PFN_vkCmdSetCoverageReductionModeNV)deviceProcAddr(device, "vkCmdSetCoverageReductionModeNV");
#endif // (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) 
#if defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
    pOut->vkCreateTensorARM = (PFN_vkCreateTensorARM)deviceProcAddr(device, "vkCreateTensorARM");
    pOut->vkDestroyTensorARM = (PFN_vkDestroyTensorARM)deviceProcAddr(device, "vkDestroyTensorARM");
    pOut->vkCreateTensorViewARM = (PFN_vkCreateTensorViewARM)deviceProcAddr(device, "vkCreateTensorViewARM");
    pOut->vkDestroyTensorViewARM = (PFN_vkDestroyTensorViewARM)deviceProcAddr(device, "vkDestroyTensorViewARM");
    pOut->vkGetTensorMemoryRequirementsARM = (PFN_vkGetTensorMemoryRequirementsARM)deviceProcAddr(device, "vkGetTensorMemoryRequirementsARM");
    pOut->vkBindTensorMemoryARM = (PFN_vkBindTensorMemoryARM)deviceProcAddr(device, "vkBindTensorMemoryARM");
    pOut->vkGetDeviceTensorMemoryRequirementsARM = (PFN_vkGetDeviceTensorMemoryRequirementsARM)deviceProcAddr(device, "vkGetDeviceTensorMemoryRequirementsARM");
    pOut->vkCmdCopyTensorARM = (PFN_vkCmdCopyTensorARM)deviceProcAddr(device, "vkCmdCopyTensorARM");
#endif // defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
#if defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer) 
    pOut->vkGetTensorOpaqueCaptureDescriptorDataARM = (PFN_vkGetTensorOpaqueCaptureDescriptorDataARM)deviceProcAddr(device, "vkGetTensorOpaqueCaptureDescriptorDataARM");
    pOut->vkGetTensorViewOpaqueCaptureDescriptorDataARM = (PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM)deviceProcAddr(device, "vkGetTensorViewOpaqueCaptureDescriptorDataARM");
#endif // defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer) 
#if defined(VK_EXT_shader_module_identifier) 
    pOut->vkGetShaderModuleIdentifierEXT = (PFN_vkGetShaderModuleIdentifierEXT)deviceProcAddr(device, "vkGetShaderModuleIdentifierEXT");
    pOut->vkGetShaderModuleCreateInfoIdentifierEXT = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT)deviceProcAddr(device, "vkGetShaderModuleCreateInfoIdentifierEXT");
#endif // defined(VK_EXT_shader_module_identifier) 
#if defined(VK_NV_optical_flow) 
    pOut->vkCreateOpticalFlowSessionNV = (PFN_vkCreateOpticalFlowSessionNV)deviceProcAddr(device, "vkCreateOpticalFlowSessionNV");
    pOut->vkDestroyOpticalFlowSessionNV = (PFN_vkDestroyOpticalFlowSessionNV)deviceProcAddr(device, "vkDestroyOpticalFlowSessionNV");
    pOut->vkBindOpticalFlowSessionImageNV = (PFN_vkBindOpticalFlowSessionImageNV)deviceProcAddr(device, "vkBindOpticalFlowSessionImageNV");
    pOut->vkCmdOpticalFlowExecuteNV = (PFN_vkCmdOpticalFlowExecuteNV)deviceProcAddr(device, "vkCmdOpticalFlowExecuteNV");
#endif // defined(VK_NV_optical_flow) 
#if defined(VK_AMD_anti_lag) 
    pOut->vkAntiLagUpdateAMD = (PFN_vkAntiLagUpdateAMD)deviceProcAddr(device, "vkAntiLagUpdateAMD");
#endif // defined(VK_AMD_anti_lag) 
#if defined(VK_KHR_present_wait2) 
    pOut->vkWaitForPresent2KHR = (PFN_vkWaitForPresent2KHR)deviceProcAddr(device, "vkWaitForPresent2KHR");
#endif // defined(VK_KHR_present_wait2) 
#if defined(VK_EXT_shader_object) 
    pOut->vkCreateShadersEXT = (PFN_vkCreateShadersEXT)deviceProcAddr(device, "vkCreateShadersEXT");
    pOut->vkDestroyShaderEXT = (PFN_vkDestroyShaderEXT)deviceProcAddr(device, "vkDestroyShaderEXT");
    pOut->vkGetShaderBinaryDataEXT = (PFN_vkGetShaderBinaryDataEXT)deviceProcAddr(device, "vkGetShaderBinaryDataEXT");
    pOut->vkCmdBindShadersEXT = (PFN_vkCmdBindShadersEXT)deviceProcAddr(device, "vkCmdBindShadersEXT");
    pOut->vkCmdSetPatchControlPointsEXT = (PFN_vkCmdSetPatchControlPointsEXT)deviceProcAddr(device, "vkCmdSetPatchControlPointsEXT");
    pOut->vkCmdSetLogicOpEXT = (PFN_vkCmdSetLogicOpEXT)deviceProcAddr(device, "vkCmdSetLogicOpEXT");
#endif // defined(VK_EXT_shader_object) 
#if (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_depth_clamp_control)) 
    pOut->vkCmdSetDepthClampRangeEXT = (PFN_vkCmdSetDepthClampRangeEXT)deviceProcAddr(device, "vkCmdSetDepthClampRangeEXT");
#endif // (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_depth_clamp_control)) 
#if defined(VK_KHR_pipeline_binary) 
    pOut->vkCreatePipelineBinariesKHR = (PFN_vkCreatePipelineBinariesKHR)deviceProcAddr(device, "vkCreatePipelineBinariesKHR");
    pOut->vkDestroyPipelineBinaryKHR = (PFN_vkDestroyPipelineBinaryKHR)deviceProcAddr(device, "vkDestroyPipelineBinaryKHR");
    pOut->vkGetPipelineKeyKHR = (PFN_vkGetPipelineKeyKHR)deviceProcAddr(device, "vkGetPipelineKeyKHR");
    pOut->vkGetPipelineBinaryDataKHR = (PFN_vkGetPipelineBinaryDataKHR)deviceProcAddr(device, "vkGetPipelineBinaryDataKHR");
    pOut->vkReleaseCapturedPipelineDataKHR = (PFN_vkReleaseCapturedPipelineDataKHR)deviceProcAddr(device, "vkReleaseCapturedPipelineDataKHR");
#endif // defined(VK_KHR_pipeline_binary) 
#if defined(VK_QCOM_tile_properties) 
    pOut->vkGetFramebufferTilePropertiesQCOM = (PFN_vkGetFramebufferTilePropertiesQCOM)deviceProcAddr(device, "vkGetFramebufferTilePropertiesQCOM");
    pOut->vkGetDynamicRenderingTilePropertiesQCOM = (PFN_vkGetDynamicRenderingTilePropertiesQCOM)deviceProcAddr(device, "vkGetDynamicRenderingTilePropertiesQCOM");
#endif // defined(VK_QCOM_tile_properties) 
#if defined(VK_KHR_swapchain_maintenance1) 
    pOut->vkReleaseSwapchainImagesKHR = (PFN_vkReleaseSwapchainImagesKHR)deviceProcAddr(device, "vkReleaseSwapchainImagesKHR");
#endif // defined(VK_KHR_swapchain_maintenance1) 
#if defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
    pOut->vkConvertCooperativeVectorMatrixNV = (PFN_vkConvertCooperativeVectorMatrixNV)deviceProcAddr(device, "vkConvertCooperativeVectorMatrixNV");
    pOut->vkCmdConvertCooperativeVectorMatrixNV = (PFN_vkCmdConvertCooperativeVectorMatrixNV)deviceProcAddr(device, "vkCmdConvertCooperativeVectorMatrixNV");
#endif // defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
#if defined(VK_NV_low_latency2) && VK_NV_LOW_LATENCY_2_SPEC_VERSION >= 3 
    pOut->vkSetLatencySleepModeNV = (PFN_vkSetLatencySleepModeNV)deviceProcAddr(device, "vkSetLatencySleepModeNV");
    pOut->vkLatencySleepNV = (PFN_vkLatencySleepNV)deviceProcAddr(device, "vkLatencySleepNV");
    pOut->vkSetLatencyMarkerNV = (PFN_vkSetLatencyMarkerNV)deviceProcAddr(device, "vkSetLatencyMarkerNV");
    pOut->vkGetLatencyTimingsNV = (PFN_vkGetLatencyTimingsNV)deviceProcAddr(device, "vkGetLatencyTimingsNV");
    pOut->vkQueueNotifyOutOfBandNV = (PFN_vkQueueNotifyOutOfBandNV)deviceProcAddr(device, "vkQueueNotifyOutOfBandNV");
#endif // defined(VK_NV_low_latency2) && VK_NV_LOW_LATENCY_2_SPEC_VERSION >= 3 
#if defined(VK_ARM_data_graph) 
    pOut->vkCreateDataGraphPipelinesARM = (PFN_vkCreateDataGraphPipelinesARM)deviceProcAddr(device, "vkCreateDataGraphPipelinesARM");
    pOut->vkCreateDataGraphPipelineSessionARM = (PFN_vkCreateDataGraphPipelineSessionARM)deviceProcAddr(device, "vkCreateDataGraphPipelineSessionARM");
    pOut->vkGetDataGraphPipelineSessionBindPointRequirementsARM = (PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM)deviceProcAddr(device, "vkGetDataGraphPipelineSessionBindPointRequirementsARM");
    pOut->vkGetDataGraphPipelineSessionMemoryRequirementsARM = (PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM)deviceProcAddr(device, "vkGetDataGraphPipelineSessionMemoryRequirementsARM");
    pOut->vkBindDataGraphPipelineSessionMemoryARM = (PFN_vkBindDataGraphPipelineSessionMemoryARM)deviceProcAddr(device, "vkBindDataGraphPipelineSessionMemoryARM");
    pOut->vkDestroyDataGraphPipelineSessionARM = (PFN_vkDestroyDataGraphPipelineSessionARM)deviceProcAddr(device, "vkDestroyDataGraphPipelineSessionARM");
    pOut->vkCmdDispatchDataGraphARM = (PFN_vkCmdDispatchDataGraphARM)deviceProcAddr(device, "vkCmdDispatchDataGraphARM");
    pOut->vkGetDataGraphPipelineAvailablePropertiesARM = (PFN_vkGetDataGraphPipelineAvailablePropertiesARM)deviceProcAddr(device, "vkGetDataGraphPipelineAvailablePropertiesARM");
    pOut->vkGetDataGraphPipelinePropertiesARM = (PFN_vkGetDataGraphPipelinePropertiesARM)deviceProcAddr(device, "vkGetDataGraphPipelinePropertiesARM");
#endif // defined(VK_ARM_data_graph) 
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state) 
    pOut->vkCmdSetAttachmentFeedbackLoopEnableEXT = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT)deviceProcAddr(device, "vkCmdSetAttachmentFeedbackLoopEnableEXT");
#endif // defined(VK_EXT_attachment_feedback_loop_dynamic_state) 
#if defined(VK_QNX_external_memory_screen_buffer) 
    pOut->vkGetScreenBufferPropertiesQNX = (PFN_vkGetScreenBufferPropertiesQNX)deviceProcAddr(device, "vkGetScreenBufferPropertiesQNX");
#endif // defined(VK_QNX_external_memory_screen_buffer) 
#if defined(VK_KHR_calibrated_timestamps) 
    pOut->vkGetCalibratedTimestampsKHR = (PFN_vkGetCalibratedTimestampsKHR)deviceProcAddr(device, "vkGetCalibratedTimestampsKHR");
#endif // defined(VK_KHR_calibrated_timestamps) 
#if defined(VK_QCOM_tile_memory_heap) 
    pOut->vkCmdBindTileMemoryQCOM = (PFN_vkCmdBindTileMemoryQCOM)deviceProcAddr(device, "vkCmdBindTileMemoryQCOM");
#endif // defined(VK_QCOM_tile_memory_heap) 
#if defined(VK_KHR_copy_memory_indirect) 
    pOut->vkCmdCopyMemoryIndirectKHR = (PFN_vkCmdCopyMemoryIndirectKHR)deviceProcAddr(device, "vkCmdCopyMemoryIndirectKHR");
    pOut->vkCmdCopyMemoryToImageIndirectKHR = (PFN_vkCmdCopyMemoryToImageIndirectKHR)deviceProcAddr(device, "vkCmdCopyMemoryToImageIndirectKHR");
#endif // defined(VK_KHR_copy_memory_indirect) 
#if defined(VK_EXT_memory_decompression) 
    pOut->vkCmdDecompressMemoryEXT = (PFN_vkCmdDecompressMemoryEXT)deviceProcAddr(device, "vkCmdDecompressMemoryEXT");
    pOut->vkCmdDecompressMemoryIndirectCountEXT = (PFN_vkCmdDecompressMemoryIndirectCountEXT)deviceProcAddr(device, "vkCmdDecompressMemoryIndirectCountEXT");
#endif // defined(VK_EXT_memory_decompression) 
#if defined(VK_NV_external_compute_queue) 
    pOut->vkCreateExternalComputeQueueNV = (PFN_vkCreateExternalComputeQueueNV)deviceProcAddr(device, "vkCreateExternalComputeQueueNV");
    pOut->vkDestroyExternalComputeQueueNV = (PFN_vkDestroyExternalComputeQueueNV)deviceProcAddr(device, "vkDestroyExternalComputeQueueNV");
    pOut->vkGetExternalComputeQueueDataNV = (PFN_vkGetExternalComputeQueueDataNV)deviceProcAddr(device, "vkGetExternalComputeQueueDataNV");
#endif // defined(VK_NV_external_compute_queue) 
#if defined(VK_NV_cluster_acceleration_structure) && VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION >= 4 
    pOut->vkGetClusterAccelerationStructureBuildSizesNV = (PFN_vkGetClusterAccelerationStructureBuildSizesNV)deviceProcAddr(device, "vkGetClusterAccelerationStructureBuildSizesNV");
    pOut->vkCmdBuildClusterAccelerationStructureIndirectNV = (PFN_vkCmdBuildClusterAccelerationStructureIndirectNV)deviceProcAddr(device, "vkCmdBuildClusterAccelerationStructureIndirectNV");
#endif // defined(VK_NV_cluster_acceleration_structure) && VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION >= 4 
#if defined(VK_NV_partitioned_acceleration_structure) 
    pOut->vkGetPartitionedAccelerationStructuresBuildSizesNV = (PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV)deviceProcAddr(device, "vkGetPartitionedAccelerationStructuresBuildSizesNV");
    pOut->vkCmdBuildPartitionedAccelerationStructuresNV = (PFN_vkCmdBuildPartitionedAccelerationStructuresNV)deviceProcAddr(device, "vkCmdBuildPartitionedAccelerationStructuresNV");
#endif // defined(VK_NV_partitioned_acceleration_structure) 
#if defined(VK_EXT_device_generated_commands) 
    pOut->vkGetGeneratedCommandsMemoryRequirementsEXT = (PFN_vkGetGeneratedCommandsMemoryRequirementsEXT)deviceProcAddr(device, "vkGetGeneratedCommandsMemoryRequirementsEXT");
    pOut->vkCmdPreprocessGeneratedCommandsEXT = (PFN_vkCmdPreprocessGeneratedCommandsEXT)deviceProcAddr(device, "vkCmdPreprocessGeneratedCommandsEXT");
    pOut->vkCmdExecuteGeneratedCommandsEXT = (PFN_vkCmdExecuteGeneratedCommandsEXT)deviceProcAddr(device, "vkCmdExecuteGeneratedCommandsEXT");
    pOut->vkCreateIndirectCommandsLayoutEXT = (PFN_vkCreateIndirectCommandsLayoutEXT)deviceProcAddr(device, "vkCreateIndirectCommandsLayoutEXT");
    pOut->vkDestroyIndirectCommandsLayoutEXT = (PFN_vkDestroyIndirectCommandsLayoutEXT)deviceProcAddr(device, "vkDestroyIndirectCommandsLayoutEXT");
    pOut->vkCreateIndirectExecutionSetEXT = (PFN_vkCreateIndirectExecutionSetEXT)deviceProcAddr(device, "vkCreateIndirectExecutionSetEXT");
    pOut->vkDestroyIndirectExecutionSetEXT = (PFN_vkDestroyIndirectExecutionSetEXT)deviceProcAddr(device, "vkDestroyIndirectExecutionSetEXT");
    pOut->vkUpdateIndirectExecutionSetPipelineEXT = (PFN_vkUpdateIndirectExecutionSetPipelineEXT)deviceProcAddr(device, "vkUpdateIndirectExecutionSetPipelineEXT");
    pOut->vkUpdateIndirectExecutionSetShaderEXT = (PFN_vkUpdateIndirectExecutionSetShaderEXT)deviceProcAddr(device, "vkUpdateIndirectExecutionSetShaderEXT");
#endif // defined(VK_EXT_device_generated_commands) 
#if defined(VK_KHR_device_fault) 
    pOut->vkGetDeviceFaultReportsKHR = (PFN_vkGetDeviceFaultReportsKHR)deviceProcAddr(device, "vkGetDeviceFaultReportsKHR");
    pOut->vkGetDeviceFaultDebugInfoKHR = (PFN_vkGetDeviceFaultDebugInfoKHR)deviceProcAddr(device, "vkGetDeviceFaultDebugInfoKHR");
#endif // defined(VK_KHR_device_fault) 
#if defined(VK_EXT_external_memory_metal) 
    pOut->vkGetMemoryMetalHandleEXT = (PFN_vkGetMemoryMetalHandleEXT)deviceProcAddr(device, "vkGetMemoryMetalHandleEXT");
    pOut->vkGetMemoryMetalHandlePropertiesEXT = (PFN_vkGetMemoryMetalHandlePropertiesEXT)deviceProcAddr(device, "vkGetMemoryMetalHandlePropertiesEXT");
#endif // defined(VK_EXT_external_memory_metal) 
#if defined(VK_ARM_shader_instrumentation) 
    pOut->vkCreateShaderInstrumentationARM = (PFN_vkCreateShaderInstrumentationARM)deviceProcAddr(device, "vkCreateShaderInstrumentationARM");
    pOut->vkDestroyShaderInstrumentationARM = (PFN_vkDestroyShaderInstrumentationARM)deviceProcAddr(device, "vkDestroyShaderInstrumentationARM");
    pOut->vkCmdBeginShaderInstrumentationARM = (PFN_vkCmdBeginShaderInstrumentationARM)deviceProcAddr(device, "vkCmdBeginShaderInstrumentationARM");
    pOut->vkCmdEndShaderInstrumentationARM = (PFN_vkCmdEndShaderInstrumentationARM)deviceProcAddr(device, "vkCmdEndShaderInstrumentationARM");
    pOut->vkGetShaderInstrumentationValuesARM = (PFN_vkGetShaderInstrumentationValuesARM)deviceProcAddr(device, "vkGetShaderInstrumentationValuesARM");
    pOut->vkClearShaderInstrumentationMetricsARM = (PFN_vkClearShaderInstrumentationMetricsARM)deviceProcAddr(device, "vkClearShaderInstrumentationMetricsARM");
#endif // defined(VK_ARM_shader_instrumentation) 
#if defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)) 
    pOut->vkCmdBeginCustomResolveEXT = (PFN_vkCmdBeginCustomResolveEXT)deviceProcAddr(device, "vkCmdBeginCustomResolveEXT");
#endif // defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)) 
#if defined(VK_KHR_maintenance10) 
    pOut->vkCmdEndRendering2KHR = (PFN_vkCmdEndRendering2KHR)deviceProcAddr(device, "vkCmdEndRendering2KHR");
#endif // defined(VK_KHR_maintenance10) 
#if defined(VK_NV_compute_occupancy_priority) 
    pOut->vkCmdSetComputeOccupancyPriorityNV = (PFN_vkCmdSetComputeOccupancyPriorityNV)deviceProcAddr(device, "vkCmdSetComputeOccupancyPriorityNV");
#endif // defined(VK_NV_compute_occupancy_priority) 
#if defined(VK_EXT_primitive_restart_index) 
    pOut->vkCmdSetPrimitiveRestartIndexEXT = (PFN_vkCmdSetPrimitiveRestartIndexEXT)deviceProcAddr(device, "vkCmdSetPrimitiveRestartIndexEXT");
#endif // defined(VK_EXT_primitive_restart_index) 
#pragma endregion
}

#endif

#ifdef __cplusplus
}
#endif