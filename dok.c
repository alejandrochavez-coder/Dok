#include "dok.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

struct DokInstance_T {
    PFN_vkGetInstanceProcAddr instanceProcAddr;
#if defined(_WIN32)
    HMODULE module;
#else
    void* module;
#endif
};

DokResult dokCreateInstance(DokInstance* pOut) {
    struct DokInstance_T* ptrInstance = malloc(sizeof(struct DokInstance_T));
    if (!ptrInstance) {
        return DOK_OUT_OF_MEMORY;
    }
#ifdef _WIN32
    ptrInstance->module = LoadLibrary("vulkan-1.dll");
    if (!ptrInstance->module) {
        return DOK_VULKAN_UNSUPPORTED;
    }
    ptrInstance->instanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(ptrInstance->module, "vkGetInstanceProcAddr");
#endif
    *pOut = ptrInstance;
    return DOK_SUCCESS;
}

void dokDestroyInstance(DokInstance instance) {
#ifdef _WIN32
    FreeLibrary(instance->module);
#endif
    free(instance);
}

void dokLoadGlobalTable(DokInstance instance, DokGlobalTable* pOut) {
    PFN_vkGetInstanceProcAddr instanceProcAddr = instance->instanceProcAddr;

#if defined(VK_VERSION_1_0) 
    pOut->vkCreateInstance = instanceProcAddr(NULL, "vkCreateInstance");
    pOut->vkGetInstanceProcAddr = instanceProcAddr(NULL, "vkGetInstanceProcAddr");
    pOut->vkEnumerateInstanceExtensionProperties = instanceProcAddr(NULL, "vkEnumerateInstanceExtensionProperties");
    pOut->vkEnumerateInstanceLayerProperties = instanceProcAddr(NULL, "vkEnumerateInstanceLayerProperties");
#endif //VK_VERSION_1_0
#if defined(VK_VERSION_1_1) 
    pOut->vkEnumerateInstanceVersion = instanceProcAddr(NULL, "vkEnumerateInstanceVersion");
#endif //VK_VERSION_1_1
}

void dokLoadInstanceTable(VkInstance instance, DokGlobalTable* pFunctions, DokInstanceTable* pOut) {
    PFN_vkGetInstanceProcAddr instanceProcAddr = pFunctions->instanceProcAddr;

#if defined(VK_VERSION_1_0) 
    pOut->vkDestroyInstance = instanceProcAddr(instance, "vkDestroyInstance");
    pOut->vkEnumeratePhysicalDevices = instanceProcAddr(instance, "vkEnumeratePhysicalDevices");
    pOut->vkGetPhysicalDeviceFeatures = instanceProcAddr(instance, "vkGetPhysicalDeviceFeatures");
    pOut->vkGetPhysicalDeviceFormatProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties");
    pOut->vkGetPhysicalDeviceImageFormatProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties");
    pOut->vkGetPhysicalDeviceProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceProperties");
    pOut->vkGetPhysicalDeviceQueueFamilyProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties");
    pOut->vkGetPhysicalDeviceMemoryProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties");
    pOut->vkGetDeviceProcAddr = instanceProcAddr(instance, "vkGetDeviceProcAddr");
    pOut->vkCreateDevice = instanceProcAddr(instance, "vkCreateDevice");
    pOut->vkEnumerateDeviceExtensionProperties = instanceProcAddr(instance, "vkEnumerateDeviceExtensionProperties");
    pOut->vkEnumerateDeviceLayerProperties = instanceProcAddr(instance, "vkEnumerateDeviceLayerProperties");
    pOut->vkGetPhysicalDeviceSparseImageFormatProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties");
#endif //VK_VERSION_1_0
#if defined(VK_VERSION_1_1) 
    pOut->vkEnumeratePhysicalDeviceGroups = instanceProcAddr(instance, "vkEnumeratePhysicalDeviceGroups");
    pOut->vkGetPhysicalDeviceFeatures2 = instanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2");
    pOut->vkGetPhysicalDeviceProperties2 = instanceProcAddr(instance, "vkGetPhysicalDeviceProperties2");
    pOut->vkGetPhysicalDeviceFormatProperties2 = instanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2");
    pOut->vkGetPhysicalDeviceImageFormatProperties2 = instanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties2");
    pOut->vkGetPhysicalDeviceQueueFamilyProperties2 = instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties2");
    pOut->vkGetPhysicalDeviceMemoryProperties2 = instanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties2");
    pOut->vkGetPhysicalDeviceSparseImageFormatProperties2 = instanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties2");
    pOut->vkGetPhysicalDeviceExternalBufferProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceExternalBufferProperties");
    pOut->vkGetPhysicalDeviceExternalFenceProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceExternalFenceProperties");
    pOut->vkGetPhysicalDeviceExternalSemaphoreProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceExternalSemaphoreProperties");
#endif //VK_VERSION_1_1
#if defined(VK_VERSION_1_3) 
    pOut->vkGetPhysicalDeviceToolProperties = instanceProcAddr(instance, "vkGetPhysicalDeviceToolProperties");
#endif //VK_VERSION_1_3
#if defined(VK_KHR_surface) 
    pOut->vkDestroySurfaceKHR = instanceProcAddr(instance, "vkDestroySurfaceKHR");
    pOut->vkGetPhysicalDeviceSurfaceSupportKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR");
    pOut->vkGetPhysicalDeviceSurfaceCapabilitiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
    pOut->vkGetPhysicalDeviceSurfaceFormatsKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR");
    pOut->vkGetPhysicalDeviceSurfacePresentModesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR");
#endif //VK_KHR_surface
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) 
    pOut->vkGetPhysicalDevicePresentRectanglesKHR = instanceProcAddr(instance, "vkGetPhysicalDevicePresentRectanglesKHR");
#endif //(VK_KHR_swapchain+VK_VERSION_1_1),(VK_KHR_device_group+VK_KHR_surface)
#if defined(VK_KHR_display) 
    pOut->vkGetPhysicalDeviceDisplayPropertiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPropertiesKHR");
    pOut->vkGetPhysicalDeviceDisplayPlanePropertiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
    pOut->vkGetDisplayPlaneSupportedDisplaysKHR = instanceProcAddr(instance, "vkGetDisplayPlaneSupportedDisplaysKHR");
    pOut->vkGetDisplayModePropertiesKHR = instanceProcAddr(instance, "vkGetDisplayModePropertiesKHR");
    pOut->vkCreateDisplayModeKHR = instanceProcAddr(instance, "vkCreateDisplayModeKHR");
    pOut->vkGetDisplayPlaneCapabilitiesKHR = instanceProcAddr(instance, "vkGetDisplayPlaneCapabilitiesKHR");
    pOut->vkCreateDisplayPlaneSurfaceKHR = instanceProcAddr(instance, "vkCreateDisplayPlaneSurfaceKHR");
#endif //VK_KHR_display
#if defined(VK_KHR_xlib_surface) 
    pOut->vkCreateXlibSurfaceKHR = instanceProcAddr(instance, "vkCreateXlibSurfaceKHR");
    pOut->vkGetPhysicalDeviceXlibPresentationSupportKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif //VK_KHR_xlib_surface
#if defined(VK_KHR_xcb_surface) 
    pOut->vkCreateXcbSurfaceKHR = instanceProcAddr(instance, "vkCreateXcbSurfaceKHR");
    pOut->vkGetPhysicalDeviceXcbPresentationSupportKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
#endif //VK_KHR_xcb_surface
#if defined(VK_KHR_wayland_surface) 
    pOut->vkCreateWaylandSurfaceKHR = instanceProcAddr(instance, "vkCreateWaylandSurfaceKHR");
    pOut->vkGetPhysicalDeviceWaylandPresentationSupportKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
#endif //VK_KHR_wayland_surface
#if defined(VK_KHR_android_surface) 
    pOut->vkCreateAndroidSurfaceKHR = instanceProcAddr(instance, "vkCreateAndroidSurfaceKHR");
#endif //VK_KHR_android_surface
#if defined(VK_KHR_win32_surface) 
    pOut->vkCreateWin32SurfaceKHR = instanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
    pOut->vkGetPhysicalDeviceWin32PresentationSupportKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif //VK_KHR_win32_surface
#if defined(VK_EXT_debug_report) && VK_EXT_DEBUG_REPORT_SPEC_VERSION >= 10 
    pOut->vkCreateDebugReportCallbackEXT = instanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    pOut->vkDestroyDebugReportCallbackEXT = instanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    pOut->vkDebugReportMessageEXT = instanceProcAddr(instance, "vkDebugReportMessageEXT");
#endif //VK_EXT_debug_report+VK_EXT_DEBUG_REPORT_SPEC_VERSION >= 10
#if defined(VK_KHR_video_queue) 
    pOut->vkGetPhysicalDeviceVideoCapabilitiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceVideoCapabilitiesKHR");
    pOut->vkGetPhysicalDeviceVideoFormatPropertiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceVideoFormatPropertiesKHR");
#endif //VK_KHR_video_queue
#if defined(VK_GGP_stream_descriptor_surface) 
    pOut->vkCreateStreamDescriptorSurfaceGGP = instanceProcAddr(instance, "vkCreateStreamDescriptorSurfaceGGP");
#endif //VK_GGP_stream_descriptor_surface
#if defined(VK_NV_external_memory_capabilities) 
    pOut->vkGetPhysicalDeviceExternalImageFormatPropertiesNV = instanceProcAddr(instance, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV");
#endif //VK_NV_external_memory_capabilities
#if defined(VK_KHR_get_physical_device_properties2) 
    pOut->vkGetPhysicalDeviceFeatures2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2KHR");
    pOut->vkGetPhysicalDeviceProperties2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceProperties2KHR");
    pOut->vkGetPhysicalDeviceFormatProperties2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties2KHR");
    pOut->vkGetPhysicalDeviceImageFormatProperties2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceImageFormatProperties2KHR");
    pOut->vkGetPhysicalDeviceQueueFamilyProperties2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties2KHR");
    pOut->vkGetPhysicalDeviceMemoryProperties2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties2KHR");
    pOut->vkGetPhysicalDeviceSparseImageFormatProperties2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR");
#endif //VK_KHR_get_physical_device_properties2
#if defined(VK_NN_vi_surface) 
    pOut->vkCreateViSurfaceNN = instanceProcAddr(instance, "vkCreateViSurfaceNN");
#endif //VK_NN_vi_surface
#if defined(VK_KHR_device_group_creation) 
    pOut->vkEnumeratePhysicalDeviceGroupsKHR = instanceProcAddr(instance, "vkEnumeratePhysicalDeviceGroupsKHR");
#endif //VK_KHR_device_group_creation
#if defined(VK_KHR_external_memory_capabilities) 
    pOut->vkGetPhysicalDeviceExternalBufferPropertiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceExternalBufferPropertiesKHR");
#endif //VK_KHR_external_memory_capabilities
#if defined(VK_KHR_external_semaphore_capabilities) 
    pOut->vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR");
#endif //VK_KHR_external_semaphore_capabilities
#if defined(VK_EXT_direct_mode_display) 
    pOut->vkReleaseDisplayEXT = instanceProcAddr(instance, "vkReleaseDisplayEXT");
#endif //VK_EXT_direct_mode_display
#if defined(VK_EXT_acquire_xlib_display) 
    pOut->vkAcquireXlibDisplayEXT = instanceProcAddr(instance, "vkAcquireXlibDisplayEXT");
    pOut->vkGetRandROutputDisplayEXT = instanceProcAddr(instance, "vkGetRandROutputDisplayEXT");
#endif //VK_EXT_acquire_xlib_display
#if defined(VK_EXT_display_surface_counter) 
    pOut->vkGetPhysicalDeviceSurfaceCapabilities2EXT = instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
#endif //VK_EXT_display_surface_counter
#if defined(VK_KHR_external_fence_capabilities) 
    pOut->vkGetPhysicalDeviceExternalFencePropertiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceExternalFencePropertiesKHR");
#endif //VK_KHR_external_fence_capabilities
#if defined(VK_KHR_performance_query) 
    pOut->vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = instanceProcAddr(instance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR");
    pOut->vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR");
#endif //VK_KHR_performance_query
#if defined(VK_KHR_get_surface_capabilities2) 
    pOut->vkGetPhysicalDeviceSurfaceCapabilities2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
    pOut->vkGetPhysicalDeviceSurfaceFormats2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormats2KHR");
#endif //VK_KHR_get_surface_capabilities2
#if defined(VK_KHR_get_display_properties2) 
    pOut->vkGetPhysicalDeviceDisplayProperties2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceDisplayProperties2KHR");
    pOut->vkGetPhysicalDeviceDisplayPlaneProperties2KHR = instanceProcAddr(instance, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
    pOut->vkGetDisplayModeProperties2KHR = instanceProcAddr(instance, "vkGetDisplayModeProperties2KHR");
    pOut->vkGetDisplayPlaneCapabilities2KHR = instanceProcAddr(instance, "vkGetDisplayPlaneCapabilities2KHR");
#endif //VK_KHR_get_display_properties2
#if defined(VK_MVK_ios_surface) && VK_MVK_IOS_SURFACE_SPEC_VERSION >= 3 
    pOut->vkCreateIOSSurfaceMVK = instanceProcAddr(instance, "vkCreateIOSSurfaceMVK");
#endif //VK_MVK_ios_surface+VK_MVK_IOS_SURFACE_SPEC_VERSION >= 3
#if defined(VK_MVK_macos_surface) && VK_MVK_MACOS_SURFACE_SPEC_VERSION >= 3 
    pOut->vkCreateMacOSSurfaceMVK = instanceProcAddr(instance, "vkCreateMacOSSurfaceMVK");
#endif //VK_MVK_macos_surface+VK_MVK_MACOS_SURFACE_SPEC_VERSION >= 3
#if defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
    pOut->vkCreateDebugUtilsMessengerEXT = instanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    pOut->vkDestroyDebugUtilsMessengerEXT = instanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    pOut->vkSubmitDebugUtilsMessageEXT = instanceProcAddr(instance, "vkSubmitDebugUtilsMessageEXT");
#endif //VK_EXT_debug_utils+VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2
#if defined(VK_EXT_descriptor_heap) 
    pOut->vkGetPhysicalDeviceDescriptorSizeEXT = instanceProcAddr(instance, "vkGetPhysicalDeviceDescriptorSizeEXT");
#endif //VK_EXT_descriptor_heap
#if defined(VK_EXT_sample_locations) 
    pOut->vkGetPhysicalDeviceMultisamplePropertiesEXT = instanceProcAddr(instance, "vkGetPhysicalDeviceMultisamplePropertiesEXT");
#endif //VK_EXT_sample_locations
#if defined(VK_EXT_calibrated_timestamps) && VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION >= 2 
    pOut->vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = instanceProcAddr(instance, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT");
#endif //VK_EXT_calibrated_timestamps+VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION >= 2
#if defined(VK_FUCHSIA_imagepipe_surface) 
    pOut->vkCreateImagePipeSurfaceFUCHSIA = instanceProcAddr(instance, "vkCreateImagePipeSurfaceFUCHSIA");
#endif //VK_FUCHSIA_imagepipe_surface
#if defined(VK_EXT_metal_surface) 
    pOut->vkCreateMetalSurfaceEXT = instanceProcAddr(instance, "vkCreateMetalSurfaceEXT");
#endif //VK_EXT_metal_surface
#if defined(VK_KHR_fragment_shading_rate) 
    pOut->vkGetPhysicalDeviceFragmentShadingRatesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceFragmentShadingRatesKHR");
#endif //VK_KHR_fragment_shading_rate
#if defined(VK_EXT_tooling_info) 
    pOut->vkGetPhysicalDeviceToolPropertiesEXT = instanceProcAddr(instance, "vkGetPhysicalDeviceToolPropertiesEXT");
#endif //VK_EXT_tooling_info
#if defined(VK_NV_cooperative_matrix) 
    pOut->vkGetPhysicalDeviceCooperativeMatrixPropertiesNV = instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV");
#endif //VK_NV_cooperative_matrix
#if defined(VK_NV_coverage_reduction_mode) 
    pOut->vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = instanceProcAddr(instance, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV");
#endif //VK_NV_coverage_reduction_mode
#if defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
    pOut->vkGetPhysicalDeviceSurfacePresentModes2EXT = instanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModes2EXT");
#endif //VK_EXT_full_screen_exclusive+VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4
#if defined(VK_EXT_headless_surface) 
    pOut->vkCreateHeadlessSurfaceEXT = instanceProcAddr(instance, "vkCreateHeadlessSurfaceEXT");
#endif //VK_EXT_headless_surface
#if defined(VK_EXT_acquire_drm_display) 
    pOut->vkAcquireDrmDisplayEXT = instanceProcAddr(instance, "vkAcquireDrmDisplayEXT");
    pOut->vkGetDrmDisplayEXT = instanceProcAddr(instance, "vkGetDrmDisplayEXT");
#endif //VK_EXT_acquire_drm_display
#if defined(VK_KHR_video_encode_queue) 
    pOut->vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR");
#endif //VK_KHR_video_encode_queue
#if defined(VK_NV_acquire_winrt_display) 
    pOut->vkAcquireWinrtDisplayNV = instanceProcAddr(instance, "vkAcquireWinrtDisplayNV");
    pOut->vkGetWinrtDisplayNV = instanceProcAddr(instance, "vkGetWinrtDisplayNV");
#endif //VK_NV_acquire_winrt_display
#if defined(VK_EXT_directfb_surface) 
    pOut->vkCreateDirectFBSurfaceEXT = instanceProcAddr(instance, "vkCreateDirectFBSurfaceEXT");
    pOut->vkGetPhysicalDeviceDirectFBPresentationSupportEXT = instanceProcAddr(instance, "vkGetPhysicalDeviceDirectFBPresentationSupportEXT");
#endif //VK_EXT_directfb_surface
#if defined(VK_QNX_screen_surface) 
    pOut->vkCreateScreenSurfaceQNX = instanceProcAddr(instance, "vkCreateScreenSurfaceQNX");
    pOut->vkGetPhysicalDeviceScreenPresentationSupportQNX = instanceProcAddr(instance, "vkGetPhysicalDeviceScreenPresentationSupportQNX");
#endif //VK_QNX_screen_surface
#if defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
    pOut->vkGetPhysicalDeviceExternalTensorPropertiesARM = instanceProcAddr(instance, "vkGetPhysicalDeviceExternalTensorPropertiesARM");
#endif //VK_ARM_tensors+VK_ARM_TENSORS_SPEC_VERSION >= 2
#if defined(VK_NV_optical_flow) 
    pOut->vkGetPhysicalDeviceOpticalFlowImageFormatsNV = instanceProcAddr(instance, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV");
#endif //VK_NV_optical_flow
#if defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
    pOut->vkGetPhysicalDeviceCooperativeVectorPropertiesNV = instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeVectorPropertiesNV");
#endif //VK_NV_cooperative_vector+VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4
#if defined(VK_KHR_cooperative_matrix) 
    pOut->vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR");
#endif //VK_KHR_cooperative_matrix
#if defined(VK_ARM_data_graph) 
    pOut->vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM = instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM");
    pOut->vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM = instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM");
#endif //VK_ARM_data_graph
#if (defined(VK_ARM_data_graph_instruction_set_tosa)) || (defined(VK_ARM_data_graph_optical_flow)) 
    pOut->vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM = instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyDataGraphEngineOperationPropertiesARM");
#endif //(VK_ARM_data_graph_instruction_set_tosa),(VK_ARM_data_graph_optical_flow)
#if defined(VK_KHR_calibrated_timestamps) 
    pOut->vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = instanceProcAddr(instance, "vkGetPhysicalDeviceCalibrateableTimeDomainsKHR");
#endif //VK_KHR_calibrated_timestamps
#if defined(VK_OHOS_surface) 
    pOut->vkCreateSurfaceOHOS = instanceProcAddr(instance, "vkCreateSurfaceOHOS");
#endif //VK_OHOS_surface
#if defined(VK_NV_cooperative_matrix2) 
    pOut->vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV = instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV");
#endif //VK_NV_cooperative_matrix2
#if defined(VK_ARM_performance_counters_by_region) 
    pOut->vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM = instanceProcAddr(instance, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM");
#endif //VK_ARM_performance_counters_by_region
#if defined(VK_ARM_shader_instrumentation) 
    pOut->vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM = instanceProcAddr(instance, "vkEnumeratePhysicalDeviceShaderInstrumentationMetricsARM");
#endif //VK_ARM_shader_instrumentation
#if defined(VK_ARM_data_graph_optical_flow) 
    pOut->vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM = instanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyDataGraphOpticalFlowImageFormatsARM");
#endif //VK_ARM_data_graph_optical_flow
#if defined(VK_EXT_cooperative_matrix_maintenance1) 
    pOut->vkGetPhysicalDeviceCooperativeMatrixProperties2EXT = instanceProcAddr(instance, "vkGetPhysicalDeviceCooperativeMatrixProperties2EXT");
#endif //VK_EXT_cooperative_matrix_maintenance1
#if defined(VK_SEC_ubm_surface) 
    pOut->vkCreateUbmSurfaceSEC = instanceProcAddr(instance, "vkCreateUbmSurfaceSEC");
    pOut->vkGetPhysicalDeviceUbmPresentationSupportSEC = instanceProcAddr(instance, "vkGetPhysicalDeviceUbmPresentationSupportSEC");
#endif //VK_SEC_ubm_surface

}

void dokLoadDeviceTable(VkDevice device, DokInstanceTable* pFunctions, DokDeviceTable* pOut) {
    PFN_vkGetDeviceProcAddr deviceProcAddr = pFunctions->vkGetDeviceProcAddr;

#if defined(VK_VERSION_1_0) 
    pOut->vkDestroyDevice = deviceProcAddr(device, "vkDestroyDevice");
    pOut->vkGetDeviceQueue = deviceProcAddr(device, "vkGetDeviceQueue");
    pOut->vkQueueSubmit = deviceProcAddr(device, "vkQueueSubmit");
    pOut->vkQueueWaitIdle = deviceProcAddr(device, "vkQueueWaitIdle");
    pOut->vkDeviceWaitIdle = deviceProcAddr(device, "vkDeviceWaitIdle");
    pOut->vkAllocateMemory = deviceProcAddr(device, "vkAllocateMemory");
    pOut->vkFreeMemory = deviceProcAddr(device, "vkFreeMemory");
    pOut->vkMapMemory = deviceProcAddr(device, "vkMapMemory");
    pOut->vkUnmapMemory = deviceProcAddr(device, "vkUnmapMemory");
    pOut->vkFlushMappedMemoryRanges = deviceProcAddr(device, "vkFlushMappedMemoryRanges");
    pOut->vkInvalidateMappedMemoryRanges = deviceProcAddr(device, "vkInvalidateMappedMemoryRanges");
    pOut->vkGetDeviceMemoryCommitment = deviceProcAddr(device, "vkGetDeviceMemoryCommitment");
    pOut->vkBindBufferMemory = deviceProcAddr(device, "vkBindBufferMemory");
    pOut->vkBindImageMemory = deviceProcAddr(device, "vkBindImageMemory");
    pOut->vkGetBufferMemoryRequirements = deviceProcAddr(device, "vkGetBufferMemoryRequirements");
    pOut->vkGetImageMemoryRequirements = deviceProcAddr(device, "vkGetImageMemoryRequirements");
    pOut->vkGetImageSparseMemoryRequirements = deviceProcAddr(device, "vkGetImageSparseMemoryRequirements");
    pOut->vkQueueBindSparse = deviceProcAddr(device, "vkQueueBindSparse");
    pOut->vkCreateFence = deviceProcAddr(device, "vkCreateFence");
    pOut->vkDestroyFence = deviceProcAddr(device, "vkDestroyFence");
    pOut->vkResetFences = deviceProcAddr(device, "vkResetFences");
    pOut->vkGetFenceStatus = deviceProcAddr(device, "vkGetFenceStatus");
    pOut->vkWaitForFences = deviceProcAddr(device, "vkWaitForFences");
    pOut->vkCreateSemaphore = deviceProcAddr(device, "vkCreateSemaphore");
    pOut->vkDestroySemaphore = deviceProcAddr(device, "vkDestroySemaphore");
    pOut->vkCreateQueryPool = deviceProcAddr(device, "vkCreateQueryPool");
    pOut->vkDestroyQueryPool = deviceProcAddr(device, "vkDestroyQueryPool");
    pOut->vkGetQueryPoolResults = deviceProcAddr(device, "vkGetQueryPoolResults");
    pOut->vkCreateBuffer = deviceProcAddr(device, "vkCreateBuffer");
    pOut->vkDestroyBuffer = deviceProcAddr(device, "vkDestroyBuffer");
    pOut->vkCreateImage = deviceProcAddr(device, "vkCreateImage");
    pOut->vkDestroyImage = deviceProcAddr(device, "vkDestroyImage");
    pOut->vkGetImageSubresourceLayout = deviceProcAddr(device, "vkGetImageSubresourceLayout");
    pOut->vkCreateImageView = deviceProcAddr(device, "vkCreateImageView");
    pOut->vkDestroyImageView = deviceProcAddr(device, "vkDestroyImageView");
    pOut->vkCreateCommandPool = deviceProcAddr(device, "vkCreateCommandPool");
    pOut->vkDestroyCommandPool = deviceProcAddr(device, "vkDestroyCommandPool");
    pOut->vkResetCommandPool = deviceProcAddr(device, "vkResetCommandPool");
    pOut->vkAllocateCommandBuffers = deviceProcAddr(device, "vkAllocateCommandBuffers");
    pOut->vkFreeCommandBuffers = deviceProcAddr(device, "vkFreeCommandBuffers");
    pOut->vkBeginCommandBuffer = deviceProcAddr(device, "vkBeginCommandBuffer");
    pOut->vkEndCommandBuffer = deviceProcAddr(device, "vkEndCommandBuffer");
    pOut->vkResetCommandBuffer = deviceProcAddr(device, "vkResetCommandBuffer");
    pOut->vkCmdCopyBuffer = deviceProcAddr(device, "vkCmdCopyBuffer");
    pOut->vkCmdCopyImage = deviceProcAddr(device, "vkCmdCopyImage");
    pOut->vkCmdCopyBufferToImage = deviceProcAddr(device, "vkCmdCopyBufferToImage");
    pOut->vkCmdCopyImageToBuffer = deviceProcAddr(device, "vkCmdCopyImageToBuffer");
    pOut->vkCmdUpdateBuffer = deviceProcAddr(device, "vkCmdUpdateBuffer");
    pOut->vkCmdFillBuffer = deviceProcAddr(device, "vkCmdFillBuffer");
    pOut->vkCmdPipelineBarrier = deviceProcAddr(device, "vkCmdPipelineBarrier");
    pOut->vkCmdBeginQuery = deviceProcAddr(device, "vkCmdBeginQuery");
    pOut->vkCmdEndQuery = deviceProcAddr(device, "vkCmdEndQuery");
    pOut->vkCmdResetQueryPool = deviceProcAddr(device, "vkCmdResetQueryPool");
    pOut->vkCmdWriteTimestamp = deviceProcAddr(device, "vkCmdWriteTimestamp");
    pOut->vkCmdCopyQueryPoolResults = deviceProcAddr(device, "vkCmdCopyQueryPoolResults");
    pOut->vkCmdExecuteCommands = deviceProcAddr(device, "vkCmdExecuteCommands");
    pOut->vkCreateEvent = deviceProcAddr(device, "vkCreateEvent");
    pOut->vkDestroyEvent = deviceProcAddr(device, "vkDestroyEvent");
    pOut->vkGetEventStatus = deviceProcAddr(device, "vkGetEventStatus");
    pOut->vkSetEvent = deviceProcAddr(device, "vkSetEvent");
    pOut->vkResetEvent = deviceProcAddr(device, "vkResetEvent");
    pOut->vkCreateBufferView = deviceProcAddr(device, "vkCreateBufferView");
    pOut->vkDestroyBufferView = deviceProcAddr(device, "vkDestroyBufferView");
    pOut->vkCreateShaderModule = deviceProcAddr(device, "vkCreateShaderModule");
    pOut->vkDestroyShaderModule = deviceProcAddr(device, "vkDestroyShaderModule");
    pOut->vkCreatePipelineCache = deviceProcAddr(device, "vkCreatePipelineCache");
    pOut->vkDestroyPipelineCache = deviceProcAddr(device, "vkDestroyPipelineCache");
    pOut->vkGetPipelineCacheData = deviceProcAddr(device, "vkGetPipelineCacheData");
    pOut->vkMergePipelineCaches = deviceProcAddr(device, "vkMergePipelineCaches");
    pOut->vkCreateComputePipelines = deviceProcAddr(device, "vkCreateComputePipelines");
    pOut->vkDestroyPipeline = deviceProcAddr(device, "vkDestroyPipeline");
    pOut->vkCreatePipelineLayout = deviceProcAddr(device, "vkCreatePipelineLayout");
    pOut->vkDestroyPipelineLayout = deviceProcAddr(device, "vkDestroyPipelineLayout");
    pOut->vkCreateSampler = deviceProcAddr(device, "vkCreateSampler");
    pOut->vkDestroySampler = deviceProcAddr(device, "vkDestroySampler");
    pOut->vkCreateDescriptorSetLayout = deviceProcAddr(device, "vkCreateDescriptorSetLayout");
    pOut->vkDestroyDescriptorSetLayout = deviceProcAddr(device, "vkDestroyDescriptorSetLayout");
    pOut->vkCreateDescriptorPool = deviceProcAddr(device, "vkCreateDescriptorPool");
    pOut->vkDestroyDescriptorPool = deviceProcAddr(device, "vkDestroyDescriptorPool");
    pOut->vkResetDescriptorPool = deviceProcAddr(device, "vkResetDescriptorPool");
    pOut->vkAllocateDescriptorSets = deviceProcAddr(device, "vkAllocateDescriptorSets");
    pOut->vkFreeDescriptorSets = deviceProcAddr(device, "vkFreeDescriptorSets");
    pOut->vkUpdateDescriptorSets = deviceProcAddr(device, "vkUpdateDescriptorSets");
    pOut->vkCmdBindPipeline = deviceProcAddr(device, "vkCmdBindPipeline");
    pOut->vkCmdBindDescriptorSets = deviceProcAddr(device, "vkCmdBindDescriptorSets");
    pOut->vkCmdClearColorImage = deviceProcAddr(device, "vkCmdClearColorImage");
    pOut->vkCmdDispatch = deviceProcAddr(device, "vkCmdDispatch");
    pOut->vkCmdDispatchIndirect = deviceProcAddr(device, "vkCmdDispatchIndirect");
    pOut->vkCmdSetEvent = deviceProcAddr(device, "vkCmdSetEvent");
    pOut->vkCmdResetEvent = deviceProcAddr(device, "vkCmdResetEvent");
    pOut->vkCmdWaitEvents = deviceProcAddr(device, "vkCmdWaitEvents");
    pOut->vkCmdPushConstants = deviceProcAddr(device, "vkCmdPushConstants");
    pOut->vkCreateGraphicsPipelines = deviceProcAddr(device, "vkCreateGraphicsPipelines");
    pOut->vkCreateFramebuffer = deviceProcAddr(device, "vkCreateFramebuffer");
    pOut->vkDestroyFramebuffer = deviceProcAddr(device, "vkDestroyFramebuffer");
    pOut->vkCreateRenderPass = deviceProcAddr(device, "vkCreateRenderPass");
    pOut->vkDestroyRenderPass = deviceProcAddr(device, "vkDestroyRenderPass");
    pOut->vkGetRenderAreaGranularity = deviceProcAddr(device, "vkGetRenderAreaGranularity");
    pOut->vkCmdSetViewport = deviceProcAddr(device, "vkCmdSetViewport");
    pOut->vkCmdSetScissor = deviceProcAddr(device, "vkCmdSetScissor");
    pOut->vkCmdSetLineWidth = deviceProcAddr(device, "vkCmdSetLineWidth");
    pOut->vkCmdSetDepthBias = deviceProcAddr(device, "vkCmdSetDepthBias");
    pOut->vkCmdSetBlendConstants = deviceProcAddr(device, "vkCmdSetBlendConstants");
    pOut->vkCmdSetDepthBounds = deviceProcAddr(device, "vkCmdSetDepthBounds");
    pOut->vkCmdSetStencilCompareMask = deviceProcAddr(device, "vkCmdSetStencilCompareMask");
    pOut->vkCmdSetStencilWriteMask = deviceProcAddr(device, "vkCmdSetStencilWriteMask");
    pOut->vkCmdSetStencilReference = deviceProcAddr(device, "vkCmdSetStencilReference");
    pOut->vkCmdBindIndexBuffer = deviceProcAddr(device, "vkCmdBindIndexBuffer");
    pOut->vkCmdBindVertexBuffers = deviceProcAddr(device, "vkCmdBindVertexBuffers");
    pOut->vkCmdDraw = deviceProcAddr(device, "vkCmdDraw");
    pOut->vkCmdDrawIndexed = deviceProcAddr(device, "vkCmdDrawIndexed");
    pOut->vkCmdDrawIndirect = deviceProcAddr(device, "vkCmdDrawIndirect");
    pOut->vkCmdDrawIndexedIndirect = deviceProcAddr(device, "vkCmdDrawIndexedIndirect");
    pOut->vkCmdBlitImage = deviceProcAddr(device, "vkCmdBlitImage");
    pOut->vkCmdClearDepthStencilImage = deviceProcAddr(device, "vkCmdClearDepthStencilImage");
    pOut->vkCmdClearAttachments = deviceProcAddr(device, "vkCmdClearAttachments");
    pOut->vkCmdResolveImage = deviceProcAddr(device, "vkCmdResolveImage");
    pOut->vkCmdBeginRenderPass = deviceProcAddr(device, "vkCmdBeginRenderPass");
    pOut->vkCmdNextSubpass = deviceProcAddr(device, "vkCmdNextSubpass");
    pOut->vkCmdEndRenderPass = deviceProcAddr(device, "vkCmdEndRenderPass");
#endif //VK_VERSION_1_0
#if defined(VK_VERSION_1_1) 
    pOut->vkBindBufferMemory2 = deviceProcAddr(device, "vkBindBufferMemory2");
    pOut->vkBindImageMemory2 = deviceProcAddr(device, "vkBindImageMemory2");
    pOut->vkGetDeviceGroupPeerMemoryFeatures = deviceProcAddr(device, "vkGetDeviceGroupPeerMemoryFeatures");
    pOut->vkCmdSetDeviceMask = deviceProcAddr(device, "vkCmdSetDeviceMask");
    pOut->vkGetImageMemoryRequirements2 = deviceProcAddr(device, "vkGetImageMemoryRequirements2");
    pOut->vkGetBufferMemoryRequirements2 = deviceProcAddr(device, "vkGetBufferMemoryRequirements2");
    pOut->vkGetImageSparseMemoryRequirements2 = deviceProcAddr(device, "vkGetImageSparseMemoryRequirements2");
    pOut->vkTrimCommandPool = deviceProcAddr(device, "vkTrimCommandPool");
    pOut->vkGetDeviceQueue2 = deviceProcAddr(device, "vkGetDeviceQueue2");
    pOut->vkCmdDispatchBase = deviceProcAddr(device, "vkCmdDispatchBase");
    pOut->vkCreateDescriptorUpdateTemplate = deviceProcAddr(device, "vkCreateDescriptorUpdateTemplate");
    pOut->vkDestroyDescriptorUpdateTemplate = deviceProcAddr(device, "vkDestroyDescriptorUpdateTemplate");
    pOut->vkUpdateDescriptorSetWithTemplate = deviceProcAddr(device, "vkUpdateDescriptorSetWithTemplate");
    pOut->vkGetDescriptorSetLayoutSupport = deviceProcAddr(device, "vkGetDescriptorSetLayoutSupport");
    pOut->vkCreateSamplerYcbcrConversion = deviceProcAddr(device, "vkCreateSamplerYcbcrConversion");
    pOut->vkDestroySamplerYcbcrConversion = deviceProcAddr(device, "vkDestroySamplerYcbcrConversion");
#endif //VK_VERSION_1_1
#if defined(VK_VERSION_1_2) 
    pOut->vkResetQueryPool = deviceProcAddr(device, "vkResetQueryPool");
    pOut->vkGetSemaphoreCounterValue = deviceProcAddr(device, "vkGetSemaphoreCounterValue");
    pOut->vkWaitSemaphores = deviceProcAddr(device, "vkWaitSemaphores");
    pOut->vkSignalSemaphore = deviceProcAddr(device, "vkSignalSemaphore");
    pOut->vkGetBufferDeviceAddress = deviceProcAddr(device, "vkGetBufferDeviceAddress");
    pOut->vkGetBufferOpaqueCaptureAddress = deviceProcAddr(device, "vkGetBufferOpaqueCaptureAddress");
    pOut->vkGetDeviceMemoryOpaqueCaptureAddress = deviceProcAddr(device, "vkGetDeviceMemoryOpaqueCaptureAddress");
    pOut->vkCmdDrawIndirectCount = deviceProcAddr(device, "vkCmdDrawIndirectCount");
    pOut->vkCmdDrawIndexedIndirectCount = deviceProcAddr(device, "vkCmdDrawIndexedIndirectCount");
    pOut->vkCreateRenderPass2 = deviceProcAddr(device, "vkCreateRenderPass2");
    pOut->vkCmdBeginRenderPass2 = deviceProcAddr(device, "vkCmdBeginRenderPass2");
    pOut->vkCmdNextSubpass2 = deviceProcAddr(device, "vkCmdNextSubpass2");
    pOut->vkCmdEndRenderPass2 = deviceProcAddr(device, "vkCmdEndRenderPass2");
#endif //VK_VERSION_1_2
#if defined(VK_VERSION_1_3) 
    pOut->vkCreatePrivateDataSlot = deviceProcAddr(device, "vkCreatePrivateDataSlot");
    pOut->vkDestroyPrivateDataSlot = deviceProcAddr(device, "vkDestroyPrivateDataSlot");
    pOut->vkSetPrivateData = deviceProcAddr(device, "vkSetPrivateData");
    pOut->vkGetPrivateData = deviceProcAddr(device, "vkGetPrivateData");
    pOut->vkCmdPipelineBarrier2 = deviceProcAddr(device, "vkCmdPipelineBarrier2");
    pOut->vkCmdWriteTimestamp2 = deviceProcAddr(device, "vkCmdWriteTimestamp2");
    pOut->vkQueueSubmit2 = deviceProcAddr(device, "vkQueueSubmit2");
    pOut->vkCmdCopyBuffer2 = deviceProcAddr(device, "vkCmdCopyBuffer2");
    pOut->vkCmdCopyImage2 = deviceProcAddr(device, "vkCmdCopyImage2");
    pOut->vkCmdCopyBufferToImage2 = deviceProcAddr(device, "vkCmdCopyBufferToImage2");
    pOut->vkCmdCopyImageToBuffer2 = deviceProcAddr(device, "vkCmdCopyImageToBuffer2");
    pOut->vkGetDeviceBufferMemoryRequirements = deviceProcAddr(device, "vkGetDeviceBufferMemoryRequirements");
    pOut->vkGetDeviceImageMemoryRequirements = deviceProcAddr(device, "vkGetDeviceImageMemoryRequirements");
    pOut->vkGetDeviceImageSparseMemoryRequirements = deviceProcAddr(device, "vkGetDeviceImageSparseMemoryRequirements");
    pOut->vkCmdSetEvent2 = deviceProcAddr(device, "vkCmdSetEvent2");
    pOut->vkCmdResetEvent2 = deviceProcAddr(device, "vkCmdResetEvent2");
    pOut->vkCmdWaitEvents2 = deviceProcAddr(device, "vkCmdWaitEvents2");
    pOut->vkCmdBlitImage2 = deviceProcAddr(device, "vkCmdBlitImage2");
    pOut->vkCmdResolveImage2 = deviceProcAddr(device, "vkCmdResolveImage2");
    pOut->vkCmdBeginRendering = deviceProcAddr(device, "vkCmdBeginRendering");
    pOut->vkCmdEndRendering = deviceProcAddr(device, "vkCmdEndRendering");
    pOut->vkCmdSetCullMode = deviceProcAddr(device, "vkCmdSetCullMode");
    pOut->vkCmdSetFrontFace = deviceProcAddr(device, "vkCmdSetFrontFace");
    pOut->vkCmdSetPrimitiveTopology = deviceProcAddr(device, "vkCmdSetPrimitiveTopology");
    pOut->vkCmdSetViewportWithCount = deviceProcAddr(device, "vkCmdSetViewportWithCount");
    pOut->vkCmdSetScissorWithCount = deviceProcAddr(device, "vkCmdSetScissorWithCount");
    pOut->vkCmdBindVertexBuffers2 = deviceProcAddr(device, "vkCmdBindVertexBuffers2");
    pOut->vkCmdSetDepthTestEnable = deviceProcAddr(device, "vkCmdSetDepthTestEnable");
    pOut->vkCmdSetDepthWriteEnable = deviceProcAddr(device, "vkCmdSetDepthWriteEnable");
    pOut->vkCmdSetDepthCompareOp = deviceProcAddr(device, "vkCmdSetDepthCompareOp");
    pOut->vkCmdSetDepthBoundsTestEnable = deviceProcAddr(device, "vkCmdSetDepthBoundsTestEnable");
    pOut->vkCmdSetStencilTestEnable = deviceProcAddr(device, "vkCmdSetStencilTestEnable");
    pOut->vkCmdSetStencilOp = deviceProcAddr(device, "vkCmdSetStencilOp");
    pOut->vkCmdSetRasterizerDiscardEnable = deviceProcAddr(device, "vkCmdSetRasterizerDiscardEnable");
    pOut->vkCmdSetDepthBiasEnable = deviceProcAddr(device, "vkCmdSetDepthBiasEnable");
    pOut->vkCmdSetPrimitiveRestartEnable = deviceProcAddr(device, "vkCmdSetPrimitiveRestartEnable");
#endif //VK_VERSION_1_3
#if defined(VK_VERSION_1_4) 
    pOut->vkMapMemory2 = deviceProcAddr(device, "vkMapMemory2");
    pOut->vkUnmapMemory2 = deviceProcAddr(device, "vkUnmapMemory2");
    pOut->vkGetDeviceImageSubresourceLayout = deviceProcAddr(device, "vkGetDeviceImageSubresourceLayout");
    pOut->vkGetImageSubresourceLayout2 = deviceProcAddr(device, "vkGetImageSubresourceLayout2");
    pOut->vkCopyMemoryToImage = deviceProcAddr(device, "vkCopyMemoryToImage");
    pOut->vkCopyImageToMemory = deviceProcAddr(device, "vkCopyImageToMemory");
    pOut->vkCopyImageToImage = deviceProcAddr(device, "vkCopyImageToImage");
    pOut->vkTransitionImageLayout = deviceProcAddr(device, "vkTransitionImageLayout");
    pOut->vkCmdPushDescriptorSet = deviceProcAddr(device, "vkCmdPushDescriptorSet");
    pOut->vkCmdPushDescriptorSetWithTemplate = deviceProcAddr(device, "vkCmdPushDescriptorSetWithTemplate");
    pOut->vkCmdBindDescriptorSets2 = deviceProcAddr(device, "vkCmdBindDescriptorSets2");
    pOut->vkCmdPushConstants2 = deviceProcAddr(device, "vkCmdPushConstants2");
    pOut->vkCmdPushDescriptorSet2 = deviceProcAddr(device, "vkCmdPushDescriptorSet2");
    pOut->vkCmdPushDescriptorSetWithTemplate2 = deviceProcAddr(device, "vkCmdPushDescriptorSetWithTemplate2");
    pOut->vkCmdSetLineStipple = deviceProcAddr(device, "vkCmdSetLineStipple");
    pOut->vkCmdBindIndexBuffer2 = deviceProcAddr(device, "vkCmdBindIndexBuffer2");
    pOut->vkGetRenderingAreaGranularity = deviceProcAddr(device, "vkGetRenderingAreaGranularity");
    pOut->vkCmdSetRenderingAttachmentLocations = deviceProcAddr(device, "vkCmdSetRenderingAttachmentLocations");
    pOut->vkCmdSetRenderingInputAttachmentIndices = deviceProcAddr(device, "vkCmdSetRenderingInputAttachmentIndices");
#endif //VK_VERSION_1_4
#if defined(VK_KHR_swapchain) 
    pOut->vkCreateSwapchainKHR = deviceProcAddr(device, "vkCreateSwapchainKHR");
    pOut->vkDestroySwapchainKHR = deviceProcAddr(device, "vkDestroySwapchainKHR");
    pOut->vkGetSwapchainImagesKHR = deviceProcAddr(device, "vkGetSwapchainImagesKHR");
    pOut->vkAcquireNextImageKHR = deviceProcAddr(device, "vkAcquireNextImageKHR");
    pOut->vkQueuePresentKHR = deviceProcAddr(device, "vkQueuePresentKHR");
#endif //VK_KHR_swapchain
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) 
    pOut->vkGetDeviceGroupPresentCapabilitiesKHR = deviceProcAddr(device, "vkGetDeviceGroupPresentCapabilitiesKHR");
    pOut->vkGetDeviceGroupSurfacePresentModesKHR = deviceProcAddr(device, "vkGetDeviceGroupSurfacePresentModesKHR");
#endif //(VK_KHR_swapchain+VK_VERSION_1_1),(VK_KHR_device_group+VK_KHR_surface)
#if (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) || (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) 
    pOut->vkAcquireNextImage2KHR = deviceProcAddr(device, "vkAcquireNextImage2KHR");
#endif //(VK_KHR_swapchain+VK_VERSION_1_1),(VK_KHR_device_group+VK_KHR_swapchain)
#if defined(VK_KHR_display_swapchain) 
    pOut->vkCreateSharedSwapchainsKHR = deviceProcAddr(device, "vkCreateSharedSwapchainsKHR");
#endif //VK_KHR_display_swapchain
#if defined(VK_EXT_debug_marker) && VK_EXT_DEBUG_MARKER_SPEC_VERSION >= 4 
    pOut->vkDebugMarkerSetObjectTagEXT = deviceProcAddr(device, "vkDebugMarkerSetObjectTagEXT");
    pOut->vkDebugMarkerSetObjectNameEXT = deviceProcAddr(device, "vkDebugMarkerSetObjectNameEXT");
    pOut->vkCmdDebugMarkerBeginEXT = deviceProcAddr(device, "vkCmdDebugMarkerBeginEXT");
    pOut->vkCmdDebugMarkerEndEXT = deviceProcAddr(device, "vkCmdDebugMarkerEndEXT");
    pOut->vkCmdDebugMarkerInsertEXT = deviceProcAddr(device, "vkCmdDebugMarkerInsertEXT");
#endif //VK_EXT_debug_marker+VK_EXT_DEBUG_MARKER_SPEC_VERSION >= 4
#if defined(VK_KHR_video_queue) 
    pOut->vkCreateVideoSessionKHR = deviceProcAddr(device, "vkCreateVideoSessionKHR");
    pOut->vkDestroyVideoSessionKHR = deviceProcAddr(device, "vkDestroyVideoSessionKHR");
    pOut->vkGetVideoSessionMemoryRequirementsKHR = deviceProcAddr(device, "vkGetVideoSessionMemoryRequirementsKHR");
    pOut->vkBindVideoSessionMemoryKHR = deviceProcAddr(device, "vkBindVideoSessionMemoryKHR");
    pOut->vkCreateVideoSessionParametersKHR = deviceProcAddr(device, "vkCreateVideoSessionParametersKHR");
    pOut->vkUpdateVideoSessionParametersKHR = deviceProcAddr(device, "vkUpdateVideoSessionParametersKHR");
    pOut->vkDestroyVideoSessionParametersKHR = deviceProcAddr(device, "vkDestroyVideoSessionParametersKHR");
    pOut->vkCmdBeginVideoCodingKHR = deviceProcAddr(device, "vkCmdBeginVideoCodingKHR");
    pOut->vkCmdEndVideoCodingKHR = deviceProcAddr(device, "vkCmdEndVideoCodingKHR");
    pOut->vkCmdControlVideoCodingKHR = deviceProcAddr(device, "vkCmdControlVideoCodingKHR");
#endif //VK_KHR_video_queue
#if defined(VK_KHR_video_decode_queue) 
    pOut->vkCmdDecodeVideoKHR = deviceProcAddr(device, "vkCmdDecodeVideoKHR");
#endif //VK_KHR_video_decode_queue
#if defined(VK_EXT_transform_feedback) 
    pOut->vkCmdBindTransformFeedbackBuffersEXT = deviceProcAddr(device, "vkCmdBindTransformFeedbackBuffersEXT");
    pOut->vkCmdBeginTransformFeedbackEXT = deviceProcAddr(device, "vkCmdBeginTransformFeedbackEXT");
    pOut->vkCmdEndTransformFeedbackEXT = deviceProcAddr(device, "vkCmdEndTransformFeedbackEXT");
    pOut->vkCmdBeginQueryIndexedEXT = deviceProcAddr(device, "vkCmdBeginQueryIndexedEXT");
    pOut->vkCmdEndQueryIndexedEXT = deviceProcAddr(device, "vkCmdEndQueryIndexedEXT");
    pOut->vkCmdDrawIndirectByteCountEXT = deviceProcAddr(device, "vkCmdDrawIndirectByteCountEXT");
#endif //VK_EXT_transform_feedback
#if defined(VK_NVX_binary_import) && VK_NVX_BINARY_IMPORT_SPEC_VERSION >= 2 
    pOut->vkCreateCuModuleNVX = deviceProcAddr(device, "vkCreateCuModuleNVX");
    pOut->vkCreateCuFunctionNVX = deviceProcAddr(device, "vkCreateCuFunctionNVX");
    pOut->vkDestroyCuModuleNVX = deviceProcAddr(device, "vkDestroyCuModuleNVX");
    pOut->vkDestroyCuFunctionNVX = deviceProcAddr(device, "vkDestroyCuFunctionNVX");
    pOut->vkCmdCuLaunchKernelNVX = deviceProcAddr(device, "vkCmdCuLaunchKernelNVX");
#endif //VK_NVX_binary_import+VK_NVX_BINARY_IMPORT_SPEC_VERSION >= 2
#if defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 4 
    pOut->vkGetImageViewHandleNVX = deviceProcAddr(device, "vkGetImageViewHandleNVX");
    pOut->vkGetImageViewHandle64NVX = deviceProcAddr(device, "vkGetImageViewHandle64NVX");
    pOut->vkGetImageViewAddressNVX = deviceProcAddr(device, "vkGetImageViewAddressNVX");
    pOut->vkGetDeviceCombinedImageSamplerIndexNVX = deviceProcAddr(device, "vkGetDeviceCombinedImageSamplerIndexNVX");
#endif //VK_NVX_image_view_handle+VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 4
#if defined(VK_AMD_draw_indirect_count) && VK_AMD_DRAW_INDIRECT_COUNT_SPEC_VERSION >= 2 
    pOut->vkCmdDrawIndirectCountAMD = deviceProcAddr(device, "vkCmdDrawIndirectCountAMD");
    pOut->vkCmdDrawIndexedIndirectCountAMD = deviceProcAddr(device, "vkCmdDrawIndexedIndirectCountAMD");
#endif //VK_AMD_draw_indirect_count+VK_AMD_DRAW_INDIRECT_COUNT_SPEC_VERSION >= 2
#if defined(VK_AMD_shader_info) 
    pOut->vkGetShaderInfoAMD = deviceProcAddr(device, "vkGetShaderInfoAMD");
#endif //VK_AMD_shader_info
#if defined(VK_KHR_dynamic_rendering) 
    pOut->vkCmdBeginRenderingKHR = deviceProcAddr(device, "vkCmdBeginRenderingKHR");
    pOut->vkCmdEndRenderingKHR = deviceProcAddr(device, "vkCmdEndRenderingKHR");
#endif //VK_KHR_dynamic_rendering
#if defined(VK_NV_external_memory_win32) 
    pOut->vkGetMemoryWin32HandleNV = deviceProcAddr(device, "vkGetMemoryWin32HandleNV");
#endif //VK_NV_external_memory_win32
#if defined(VK_KHR_device_group) 
    pOut->vkGetDeviceGroupPeerMemoryFeaturesKHR = deviceProcAddr(device, "vkGetDeviceGroupPeerMemoryFeaturesKHR");
    pOut->vkCmdSetDeviceMaskKHR = deviceProcAddr(device, "vkCmdSetDeviceMaskKHR");
    pOut->vkCmdDispatchBaseKHR = deviceProcAddr(device, "vkCmdDispatchBaseKHR");
#endif //VK_KHR_device_group
#if defined(VK_KHR_maintenance1) 
    pOut->vkTrimCommandPoolKHR = deviceProcAddr(device, "vkTrimCommandPoolKHR");
#endif //VK_KHR_maintenance1
#if defined(VK_KHR_external_memory_win32) 
    pOut->vkGetMemoryWin32HandleKHR = deviceProcAddr(device, "vkGetMemoryWin32HandleKHR");
    pOut->vkGetMemoryWin32HandlePropertiesKHR = deviceProcAddr(device, "vkGetMemoryWin32HandlePropertiesKHR");
#endif //VK_KHR_external_memory_win32
#if defined(VK_KHR_external_memory_fd) 
    pOut->vkGetMemoryFdKHR = deviceProcAddr(device, "vkGetMemoryFdKHR");
    pOut->vkGetMemoryFdPropertiesKHR = deviceProcAddr(device, "vkGetMemoryFdPropertiesKHR");
#endif //VK_KHR_external_memory_fd
#if defined(VK_KHR_external_semaphore_win32) 
    pOut->vkImportSemaphoreWin32HandleKHR = deviceProcAddr(device, "vkImportSemaphoreWin32HandleKHR");
    pOut->vkGetSemaphoreWin32HandleKHR = deviceProcAddr(device, "vkGetSemaphoreWin32HandleKHR");
#endif //VK_KHR_external_semaphore_win32
#if defined(VK_KHR_external_semaphore_fd) 
    pOut->vkImportSemaphoreFdKHR = deviceProcAddr(device, "vkImportSemaphoreFdKHR");
    pOut->vkGetSemaphoreFdKHR = deviceProcAddr(device, "vkGetSemaphoreFdKHR");
#endif //VK_KHR_external_semaphore_fd
#if defined(VK_KHR_push_descriptor) 
    pOut->vkCmdPushDescriptorSetKHR = deviceProcAddr(device, "vkCmdPushDescriptorSetKHR");
#endif //VK_KHR_push_descriptor
#if (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template))) || (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) 
    pOut->vkCmdPushDescriptorSetWithTemplateKHR = deviceProcAddr(device, "vkCmdPushDescriptorSetWithTemplateKHR");
#endif //(VK_KHR_push_descriptor+(VK_VERSION_1_1,VK_KHR_descriptor_update_template)),(VK_KHR_descriptor_update_template+VK_KHR_push_descriptor)
#if defined(VK_EXT_conditional_rendering) && VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION >= 2 
    pOut->vkCmdBeginConditionalRenderingEXT = deviceProcAddr(device, "vkCmdBeginConditionalRenderingEXT");
    pOut->vkCmdEndConditionalRenderingEXT = deviceProcAddr(device, "vkCmdEndConditionalRenderingEXT");
#endif //VK_EXT_conditional_rendering+VK_EXT_CONDITIONAL_RENDERING_SPEC_VERSION >= 2
#if defined(VK_KHR_descriptor_update_template) 
    pOut->vkCreateDescriptorUpdateTemplateKHR = deviceProcAddr(device, "vkCreateDescriptorUpdateTemplateKHR");
    pOut->vkDestroyDescriptorUpdateTemplateKHR = deviceProcAddr(device, "vkDestroyDescriptorUpdateTemplateKHR");
    pOut->vkUpdateDescriptorSetWithTemplateKHR = deviceProcAddr(device, "vkUpdateDescriptorSetWithTemplateKHR");
#endif //VK_KHR_descriptor_update_template
#if defined(VK_NV_clip_space_w_scaling) 
    pOut->vkCmdSetViewportWScalingNV = deviceProcAddr(device, "vkCmdSetViewportWScalingNV");
#endif //VK_NV_clip_space_w_scaling
#if defined(VK_EXT_display_control) 
    pOut->vkDisplayPowerControlEXT = deviceProcAddr(device, "vkDisplayPowerControlEXT");
    pOut->vkRegisterDeviceEventEXT = deviceProcAddr(device, "vkRegisterDeviceEventEXT");
    pOut->vkRegisterDisplayEventEXT = deviceProcAddr(device, "vkRegisterDisplayEventEXT");
    pOut->vkGetSwapchainCounterEXT = deviceProcAddr(device, "vkGetSwapchainCounterEXT");
#endif //VK_EXT_display_control
#if defined(VK_GOOGLE_display_timing) 
    pOut->vkGetRefreshCycleDurationGOOGLE = deviceProcAddr(device, "vkGetRefreshCycleDurationGOOGLE");
    pOut->vkGetPastPresentationTimingGOOGLE = deviceProcAddr(device, "vkGetPastPresentationTimingGOOGLE");
#endif //VK_GOOGLE_display_timing
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 
    pOut->vkCmdSetDiscardRectangleEXT = deviceProcAddr(device, "vkCmdSetDiscardRectangleEXT");
    pOut->vkCmdSetDiscardRectangleEnableEXT = deviceProcAddr(device, "vkCmdSetDiscardRectangleEnableEXT");
    pOut->vkCmdSetDiscardRectangleModeEXT = deviceProcAddr(device, "vkCmdSetDiscardRectangleModeEXT");
#endif //VK_EXT_discard_rectangles+VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
#if defined(VK_EXT_hdr_metadata) && VK_EXT_HDR_METADATA_SPEC_VERSION >= 3 
    pOut->vkSetHdrMetadataEXT = deviceProcAddr(device, "vkSetHdrMetadataEXT");
#endif //VK_EXT_hdr_metadata+VK_EXT_HDR_METADATA_SPEC_VERSION >= 3
#if defined(VK_KHR_create_renderpass2) 
    pOut->vkCreateRenderPass2KHR = deviceProcAddr(device, "vkCreateRenderPass2KHR");
    pOut->vkCmdBeginRenderPass2KHR = deviceProcAddr(device, "vkCmdBeginRenderPass2KHR");
    pOut->vkCmdNextSubpass2KHR = deviceProcAddr(device, "vkCmdNextSubpass2KHR");
    pOut->vkCmdEndRenderPass2KHR = deviceProcAddr(device, "vkCmdEndRenderPass2KHR");
#endif //VK_KHR_create_renderpass2
#if defined(VK_KHR_shared_presentable_image) 
    pOut->vkGetSwapchainStatusKHR = deviceProcAddr(device, "vkGetSwapchainStatusKHR");
#endif //VK_KHR_shared_presentable_image
#if defined(VK_KHR_external_fence_win32) 
    pOut->vkImportFenceWin32HandleKHR = deviceProcAddr(device, "vkImportFenceWin32HandleKHR");
    pOut->vkGetFenceWin32HandleKHR = deviceProcAddr(device, "vkGetFenceWin32HandleKHR");
#endif //VK_KHR_external_fence_win32
#if defined(VK_KHR_external_fence_fd) 
    pOut->vkImportFenceFdKHR = deviceProcAddr(device, "vkImportFenceFdKHR");
    pOut->vkGetFenceFdKHR = deviceProcAddr(device, "vkGetFenceFdKHR");
#endif //VK_KHR_external_fence_fd
#if defined(VK_KHR_performance_query) 
    pOut->vkAcquireProfilingLockKHR = deviceProcAddr(device, "vkAcquireProfilingLockKHR");
    pOut->vkReleaseProfilingLockKHR = deviceProcAddr(device, "vkReleaseProfilingLockKHR");
#endif //VK_KHR_performance_query
#if defined(VK_EXT_debug_utils) && VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2 
    pOut->vkSetDebugUtilsObjectNameEXT = deviceProcAddr(device, "vkSetDebugUtilsObjectNameEXT");
    pOut->vkSetDebugUtilsObjectTagEXT = deviceProcAddr(device, "vkSetDebugUtilsObjectTagEXT");
    pOut->vkQueueBeginDebugUtilsLabelEXT = deviceProcAddr(device, "vkQueueBeginDebugUtilsLabelEXT");
    pOut->vkQueueEndDebugUtilsLabelEXT = deviceProcAddr(device, "vkQueueEndDebugUtilsLabelEXT");
    pOut->vkQueueInsertDebugUtilsLabelEXT = deviceProcAddr(device, "vkQueueInsertDebugUtilsLabelEXT");
    pOut->vkCmdBeginDebugUtilsLabelEXT = deviceProcAddr(device, "vkCmdBeginDebugUtilsLabelEXT");
    pOut->vkCmdEndDebugUtilsLabelEXT = deviceProcAddr(device, "vkCmdEndDebugUtilsLabelEXT");
    pOut->vkCmdInsertDebugUtilsLabelEXT = deviceProcAddr(device, "vkCmdInsertDebugUtilsLabelEXT");
#endif //VK_EXT_debug_utils+VK_EXT_DEBUG_UTILS_SPEC_VERSION >= 2
#if defined(VK_ANDROID_external_memory_android_hardware_buffer) && VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION >= 5 
    pOut->vkGetAndroidHardwareBufferPropertiesANDROID = deviceProcAddr(device, "vkGetAndroidHardwareBufferPropertiesANDROID");
    pOut->vkGetMemoryAndroidHardwareBufferANDROID = deviceProcAddr(device, "vkGetMemoryAndroidHardwareBufferANDROID");
#endif //VK_ANDROID_external_memory_android_hardware_buffer+VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_SPEC_VERSION >= 5
#if defined(VK_AMD_gpa_interface) 
    pOut->vkCreateGpaSessionAMD = deviceProcAddr(device, "vkCreateGpaSessionAMD");
    pOut->vkDestroyGpaSessionAMD = deviceProcAddr(device, "vkDestroyGpaSessionAMD");
    pOut->vkSetGpaDeviceClockModeAMD = deviceProcAddr(device, "vkSetGpaDeviceClockModeAMD");
    pOut->vkGetGpaDeviceClockInfoAMD = deviceProcAddr(device, "vkGetGpaDeviceClockInfoAMD");
    pOut->vkCmdBeginGpaSessionAMD = deviceProcAddr(device, "vkCmdBeginGpaSessionAMD");
    pOut->vkCmdEndGpaSessionAMD = deviceProcAddr(device, "vkCmdEndGpaSessionAMD");
    pOut->vkCmdBeginGpaSampleAMD = deviceProcAddr(device, "vkCmdBeginGpaSampleAMD");
    pOut->vkCmdEndGpaSampleAMD = deviceProcAddr(device, "vkCmdEndGpaSampleAMD");
    pOut->vkGetGpaSessionStatusAMD = deviceProcAddr(device, "vkGetGpaSessionStatusAMD");
    pOut->vkGetGpaSessionResultsAMD = deviceProcAddr(device, "vkGetGpaSessionResultsAMD");
    pOut->vkResetGpaSessionAMD = deviceProcAddr(device, "vkResetGpaSessionAMD");
    pOut->vkCmdCopyGpaSessionResultsAMD = deviceProcAddr(device, "vkCmdCopyGpaSessionResultsAMD");
#endif //VK_AMD_gpa_interface
#if defined(VK_AMDX_shader_enqueue) && VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION >= 2 
    pOut->vkCreateExecutionGraphPipelinesAMDX = deviceProcAddr(device, "vkCreateExecutionGraphPipelinesAMDX");
    pOut->vkGetExecutionGraphPipelineScratchSizeAMDX = deviceProcAddr(device, "vkGetExecutionGraphPipelineScratchSizeAMDX");
    pOut->vkGetExecutionGraphPipelineNodeIndexAMDX = deviceProcAddr(device, "vkGetExecutionGraphPipelineNodeIndexAMDX");
    pOut->vkCmdInitializeGraphScratchMemoryAMDX = deviceProcAddr(device, "vkCmdInitializeGraphScratchMemoryAMDX");
    pOut->vkCmdDispatchGraphAMDX = deviceProcAddr(device, "vkCmdDispatchGraphAMDX");
    pOut->vkCmdDispatchGraphIndirectAMDX = deviceProcAddr(device, "vkCmdDispatchGraphIndirectAMDX");
    pOut->vkCmdDispatchGraphIndirectCountAMDX = deviceProcAddr(device, "vkCmdDispatchGraphIndirectCountAMDX");
#endif //VK_AMDX_shader_enqueue+VK_AMDX_SHADER_ENQUEUE_SPEC_VERSION >= 2
#if defined(VK_EXT_descriptor_heap) 
    pOut->vkWriteSamplerDescriptorsEXT = deviceProcAddr(device, "vkWriteSamplerDescriptorsEXT");
    pOut->vkWriteResourceDescriptorsEXT = deviceProcAddr(device, "vkWriteResourceDescriptorsEXT");
    pOut->vkCmdBindSamplerHeapEXT = deviceProcAddr(device, "vkCmdBindSamplerHeapEXT");
    pOut->vkCmdBindResourceHeapEXT = deviceProcAddr(device, "vkCmdBindResourceHeapEXT");
    pOut->vkCmdPushDataEXT = deviceProcAddr(device, "vkCmdPushDataEXT");
    pOut->vkGetImageOpaqueCaptureDataEXT = deviceProcAddr(device, "vkGetImageOpaqueCaptureDataEXT");
#endif //VK_EXT_descriptor_heap
#if defined(VK_EXT_descriptor_heap) && defined(VK_EXT_custom_border_color) 
    pOut->vkRegisterCustomBorderColorEXT = deviceProcAddr(device, "vkRegisterCustomBorderColorEXT");
    pOut->vkUnregisterCustomBorderColorEXT = deviceProcAddr(device, "vkUnregisterCustomBorderColorEXT");
#endif //VK_EXT_descriptor_heap+VK_EXT_custom_border_color
#if defined(VK_EXT_descriptor_heap) && defined(VK_ARM_tensors) 
    pOut->vkGetTensorOpaqueCaptureDataARM = deviceProcAddr(device, "vkGetTensorOpaqueCaptureDataARM");
#endif //VK_EXT_descriptor_heap+VK_ARM_tensors
#if defined(VK_EXT_sample_locations) 
    pOut->vkCmdSetSampleLocationsEXT = deviceProcAddr(device, "vkCmdSetSampleLocationsEXT");
#endif //VK_EXT_sample_locations
#if defined(VK_KHR_get_memory_requirements2) 
    pOut->vkGetImageMemoryRequirements2KHR = deviceProcAddr(device, "vkGetImageMemoryRequirements2KHR");
    pOut->vkGetBufferMemoryRequirements2KHR = deviceProcAddr(device, "vkGetBufferMemoryRequirements2KHR");
    pOut->vkGetImageSparseMemoryRequirements2KHR = deviceProcAddr(device, "vkGetImageSparseMemoryRequirements2KHR");
#endif //VK_KHR_get_memory_requirements2
#if defined(VK_KHR_acceleration_structure) 
    pOut->vkCreateAccelerationStructureKHR = deviceProcAddr(device, "vkCreateAccelerationStructureKHR");
    pOut->vkDestroyAccelerationStructureKHR = deviceProcAddr(device, "vkDestroyAccelerationStructureKHR");
    pOut->vkCmdBuildAccelerationStructuresKHR = deviceProcAddr(device, "vkCmdBuildAccelerationStructuresKHR");
    pOut->vkCmdBuildAccelerationStructuresIndirectKHR = deviceProcAddr(device, "vkCmdBuildAccelerationStructuresIndirectKHR");
    pOut->vkBuildAccelerationStructuresKHR = deviceProcAddr(device, "vkBuildAccelerationStructuresKHR");
    pOut->vkCopyAccelerationStructureKHR = deviceProcAddr(device, "vkCopyAccelerationStructureKHR");
    pOut->vkCopyAccelerationStructureToMemoryKHR = deviceProcAddr(device, "vkCopyAccelerationStructureToMemoryKHR");
    pOut->vkCopyMemoryToAccelerationStructureKHR = deviceProcAddr(device, "vkCopyMemoryToAccelerationStructureKHR");
    pOut->vkWriteAccelerationStructuresPropertiesKHR = deviceProcAddr(device, "vkWriteAccelerationStructuresPropertiesKHR");
    pOut->vkCmdCopyAccelerationStructureKHR = deviceProcAddr(device, "vkCmdCopyAccelerationStructureKHR");
    pOut->vkCmdCopyAccelerationStructureToMemoryKHR = deviceProcAddr(device, "vkCmdCopyAccelerationStructureToMemoryKHR");
    pOut->vkCmdCopyMemoryToAccelerationStructureKHR = deviceProcAddr(device, "vkCmdCopyMemoryToAccelerationStructureKHR");
    pOut->vkGetAccelerationStructureDeviceAddressKHR = deviceProcAddr(device, "vkGetAccelerationStructureDeviceAddressKHR");
    pOut->vkCmdWriteAccelerationStructuresPropertiesKHR = deviceProcAddr(device, "vkCmdWriteAccelerationStructuresPropertiesKHR");
    pOut->vkGetDeviceAccelerationStructureCompatibilityKHR = deviceProcAddr(device, "vkGetDeviceAccelerationStructureCompatibilityKHR");
    pOut->vkGetAccelerationStructureBuildSizesKHR = deviceProcAddr(device, "vkGetAccelerationStructureBuildSizesKHR");
#endif //VK_KHR_acceleration_structure
#if defined(VK_KHR_ray_tracing_pipeline) 
    pOut->vkCmdTraceRaysKHR = deviceProcAddr(device, "vkCmdTraceRaysKHR");
    pOut->vkCreateRayTracingPipelinesKHR = deviceProcAddr(device, "vkCreateRayTracingPipelinesKHR");
    pOut->vkGetRayTracingShaderGroupHandlesKHR = deviceProcAddr(device, "vkGetRayTracingShaderGroupHandlesKHR");
    pOut->vkGetRayTracingCaptureReplayShaderGroupHandlesKHR = deviceProcAddr(device, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR");
    pOut->vkCmdTraceRaysIndirectKHR = deviceProcAddr(device, "vkCmdTraceRaysIndirectKHR");
    pOut->vkGetRayTracingShaderGroupStackSizeKHR = deviceProcAddr(device, "vkGetRayTracingShaderGroupStackSizeKHR");
    pOut->vkCmdSetRayTracingPipelineStackSizeKHR = deviceProcAddr(device, "vkCmdSetRayTracingPipelineStackSizeKHR");
#endif //VK_KHR_ray_tracing_pipeline
#if defined(VK_KHR_sampler_ycbcr_conversion) 
    pOut->vkCreateSamplerYcbcrConversionKHR = deviceProcAddr(device, "vkCreateSamplerYcbcrConversionKHR");
    pOut->vkDestroySamplerYcbcrConversionKHR = deviceProcAddr(device, "vkDestroySamplerYcbcrConversionKHR");
#endif //VK_KHR_sampler_ycbcr_conversion
#if defined(VK_KHR_bind_memory2) 
    pOut->vkBindBufferMemory2KHR = deviceProcAddr(device, "vkBindBufferMemory2KHR");
    pOut->vkBindImageMemory2KHR = deviceProcAddr(device, "vkBindImageMemory2KHR");
#endif //VK_KHR_bind_memory2
#if defined(VK_EXT_image_drm_format_modifier) && VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION >= 2 
    pOut->vkGetImageDrmFormatModifierPropertiesEXT = deviceProcAddr(device, "vkGetImageDrmFormatModifierPropertiesEXT");
#endif //VK_EXT_image_drm_format_modifier+VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_SPEC_VERSION >= 2
#if defined(VK_EXT_validation_cache) 
    pOut->vkCreateValidationCacheEXT = deviceProcAddr(device, "vkCreateValidationCacheEXT");
    pOut->vkDestroyValidationCacheEXT = deviceProcAddr(device, "vkDestroyValidationCacheEXT");
    pOut->vkMergeValidationCachesEXT = deviceProcAddr(device, "vkMergeValidationCachesEXT");
    pOut->vkGetValidationCacheDataEXT = deviceProcAddr(device, "vkGetValidationCacheDataEXT");
#endif //VK_EXT_validation_cache
#if defined(VK_NV_shading_rate_image) && VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION >= 3 
    pOut->vkCmdBindShadingRateImageNV = deviceProcAddr(device, "vkCmdBindShadingRateImageNV");
    pOut->vkCmdSetViewportShadingRatePaletteNV = deviceProcAddr(device, "vkCmdSetViewportShadingRatePaletteNV");
    pOut->vkCmdSetCoarseSampleOrderNV = deviceProcAddr(device, "vkCmdSetCoarseSampleOrderNV");
#endif //VK_NV_shading_rate_image+VK_NV_SHADING_RATE_IMAGE_SPEC_VERSION >= 3
#if defined(VK_NV_ray_tracing) && VK_NV_RAY_TRACING_SPEC_VERSION >= 3 
    pOut->vkCreateAccelerationStructureNV = deviceProcAddr(device, "vkCreateAccelerationStructureNV");
    pOut->vkDestroyAccelerationStructureNV = deviceProcAddr(device, "vkDestroyAccelerationStructureNV");
    pOut->vkGetAccelerationStructureMemoryRequirementsNV = deviceProcAddr(device, "vkGetAccelerationStructureMemoryRequirementsNV");
    pOut->vkBindAccelerationStructureMemoryNV = deviceProcAddr(device, "vkBindAccelerationStructureMemoryNV");
    pOut->vkCmdBuildAccelerationStructureNV = deviceProcAddr(device, "vkCmdBuildAccelerationStructureNV");
    pOut->vkCmdCopyAccelerationStructureNV = deviceProcAddr(device, "vkCmdCopyAccelerationStructureNV");
    pOut->vkCmdTraceRaysNV = deviceProcAddr(device, "vkCmdTraceRaysNV");
    pOut->vkCreateRayTracingPipelinesNV = deviceProcAddr(device, "vkCreateRayTracingPipelinesNV");
    pOut->vkGetRayTracingShaderGroupHandlesNV = deviceProcAddr(device, "vkGetRayTracingShaderGroupHandlesNV");
    pOut->vkGetAccelerationStructureHandleNV = deviceProcAddr(device, "vkGetAccelerationStructureHandleNV");
    pOut->vkCmdWriteAccelerationStructuresPropertiesNV = deviceProcAddr(device, "vkCmdWriteAccelerationStructuresPropertiesNV");
    pOut->vkCompileDeferredNV = deviceProcAddr(device, "vkCompileDeferredNV");
#endif //VK_NV_ray_tracing+VK_NV_RAY_TRACING_SPEC_VERSION >= 3
#if defined(VK_KHR_maintenance3) 
    pOut->vkGetDescriptorSetLayoutSupportKHR = deviceProcAddr(device, "vkGetDescriptorSetLayoutSupportKHR");
#endif //VK_KHR_maintenance3
#if defined(VK_KHR_draw_indirect_count) 
    pOut->vkCmdDrawIndirectCountKHR = deviceProcAddr(device, "vkCmdDrawIndirectCountKHR");
    pOut->vkCmdDrawIndexedIndirectCountKHR = deviceProcAddr(device, "vkCmdDrawIndexedIndirectCountKHR");
#endif //VK_KHR_draw_indirect_count
#if defined(VK_EXT_external_memory_host) 
    pOut->vkGetMemoryHostPointerPropertiesEXT = deviceProcAddr(device, "vkGetMemoryHostPointerPropertiesEXT");
#endif //VK_EXT_external_memory_host
#if defined(VK_AMD_buffer_marker) 
    pOut->vkCmdWriteBufferMarkerAMD = deviceProcAddr(device, "vkCmdWriteBufferMarkerAMD");
#endif //VK_AMD_buffer_marker
#if defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
    pOut->vkCmdWriteBufferMarker2AMD = deviceProcAddr(device, "vkCmdWriteBufferMarker2AMD");
#endif //VK_AMD_buffer_marker+(VK_VERSION_1_3,VK_KHR_synchronization2)
#if defined(VK_EXT_calibrated_timestamps) && VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION >= 2 
    pOut->vkGetCalibratedTimestampsEXT = deviceProcAddr(device, "vkGetCalibratedTimestampsEXT");
#endif //VK_EXT_calibrated_timestamps+VK_EXT_CALIBRATED_TIMESTAMPS_SPEC_VERSION >= 2
#if defined(VK_NV_mesh_shader) 
    pOut->vkCmdDrawMeshTasksNV = deviceProcAddr(device, "vkCmdDrawMeshTasksNV");
    pOut->vkCmdDrawMeshTasksIndirectNV = deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectNV");
#endif //VK_NV_mesh_shader
#if defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) 
    pOut->vkCmdDrawMeshTasksIndirectCountNV = deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectCountNV");
#endif //VK_NV_mesh_shader+(VK_VERSION_1_2,VK_KHR_draw_indirect_count,VK_AMD_draw_indirect_count)
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 
    pOut->vkCmdSetExclusiveScissorEnableNV = deviceProcAddr(device, "vkCmdSetExclusiveScissorEnableNV");
    pOut->vkCmdSetExclusiveScissorNV = deviceProcAddr(device, "vkCmdSetExclusiveScissorNV");
#endif //VK_NV_scissor_exclusive+VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
#if defined(VK_NV_device_diagnostic_checkpoints) && VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION >= 2 
    pOut->vkCmdSetCheckpointNV = deviceProcAddr(device, "vkCmdSetCheckpointNV");
    pOut->vkGetQueueCheckpointDataNV = deviceProcAddr(device, "vkGetQueueCheckpointDataNV");
#endif //VK_NV_device_diagnostic_checkpoints+VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_SPEC_VERSION >= 2
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) 
    pOut->vkGetQueueCheckpointData2NV = deviceProcAddr(device, "vkGetQueueCheckpointData2NV");
#endif //VK_NV_device_diagnostic_checkpoints+(VK_VERSION_1_3,VK_KHR_synchronization2)
#if defined(VK_KHR_timeline_semaphore) 
    pOut->vkGetSemaphoreCounterValueKHR = deviceProcAddr(device, "vkGetSemaphoreCounterValueKHR");
    pOut->vkWaitSemaphoresKHR = deviceProcAddr(device, "vkWaitSemaphoresKHR");
    pOut->vkSignalSemaphoreKHR = deviceProcAddr(device, "vkSignalSemaphoreKHR");
#endif //VK_KHR_timeline_semaphore
#if defined(VK_EXT_present_timing) && VK_EXT_PRESENT_TIMING_SPEC_VERSION >= 3 
    pOut->vkSetSwapchainPresentTimingQueueSizeEXT = deviceProcAddr(device, "vkSetSwapchainPresentTimingQueueSizeEXT");
    pOut->vkGetSwapchainTimingPropertiesEXT = deviceProcAddr(device, "vkGetSwapchainTimingPropertiesEXT");
    pOut->vkGetSwapchainTimeDomainPropertiesEXT = deviceProcAddr(device, "vkGetSwapchainTimeDomainPropertiesEXT");
    pOut->vkGetPastPresentationTimingEXT = deviceProcAddr(device, "vkGetPastPresentationTimingEXT");
#endif //VK_EXT_present_timing+VK_EXT_PRESENT_TIMING_SPEC_VERSION >= 3
#if defined(VK_INTEL_performance_query) && VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION >= 2 
    pOut->vkInitializePerformanceApiINTEL = deviceProcAddr(device, "vkInitializePerformanceApiINTEL");
    pOut->vkUninitializePerformanceApiINTEL = deviceProcAddr(device, "vkUninitializePerformanceApiINTEL");
    pOut->vkCmdSetPerformanceMarkerINTEL = deviceProcAddr(device, "vkCmdSetPerformanceMarkerINTEL");
    pOut->vkCmdSetPerformanceStreamMarkerINTEL = deviceProcAddr(device, "vkCmdSetPerformanceStreamMarkerINTEL");
    pOut->vkCmdSetPerformanceOverrideINTEL = deviceProcAddr(device, "vkCmdSetPerformanceOverrideINTEL");
    pOut->vkAcquirePerformanceConfigurationINTEL = deviceProcAddr(device, "vkAcquirePerformanceConfigurationINTEL");
    pOut->vkReleasePerformanceConfigurationINTEL = deviceProcAddr(device, "vkReleasePerformanceConfigurationINTEL");
    pOut->vkQueueSetPerformanceConfigurationINTEL = deviceProcAddr(device, "vkQueueSetPerformanceConfigurationINTEL");
    pOut->vkGetPerformanceParameterINTEL = deviceProcAddr(device, "vkGetPerformanceParameterINTEL");
#endif //VK_INTEL_performance_query+VK_INTEL_PERFORMANCE_QUERY_SPEC_VERSION >= 2
#if defined(VK_AMD_display_native_hdr) 
    pOut->vkSetLocalDimmingAMD = deviceProcAddr(device, "vkSetLocalDimmingAMD");
#endif //VK_AMD_display_native_hdr
#if defined(VK_KHR_fragment_shading_rate) 
    pOut->vkCmdSetFragmentShadingRateKHR = deviceProcAddr(device, "vkCmdSetFragmentShadingRateKHR");
#endif //VK_KHR_fragment_shading_rate
#if defined(VK_KHR_dynamic_rendering_local_read) 
    pOut->vkCmdSetRenderingAttachmentLocationsKHR = deviceProcAddr(device, "vkCmdSetRenderingAttachmentLocationsKHR");
    pOut->vkCmdSetRenderingInputAttachmentIndicesKHR = deviceProcAddr(device, "vkCmdSetRenderingInputAttachmentIndicesKHR");
#endif //VK_KHR_dynamic_rendering_local_read
#if defined(VK_EXT_buffer_device_address) && VK_EXT_BUFFER_DEVICE_ADDRESS_SPEC_VERSION >= 2 
    pOut->vkGetBufferDeviceAddressEXT = deviceProcAddr(device, "vkGetBufferDeviceAddressEXT");
#endif //VK_EXT_buffer_device_address+VK_EXT_BUFFER_DEVICE_ADDRESS_SPEC_VERSION >= 2
#if defined(VK_KHR_present_wait) 
    pOut->vkWaitForPresentKHR = deviceProcAddr(device, "vkWaitForPresentKHR");
#endif //VK_KHR_present_wait
#if defined(VK_EXT_full_screen_exclusive) && VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4 
    pOut->vkAcquireFullScreenExclusiveModeEXT = deviceProcAddr(device, "vkAcquireFullScreenExclusiveModeEXT");
    pOut->vkReleaseFullScreenExclusiveModeEXT = deviceProcAddr(device, "vkReleaseFullScreenExclusiveModeEXT");
#endif //VK_EXT_full_screen_exclusive+VK_EXT_FULL_SCREEN_EXCLUSIVE_SPEC_VERSION >= 4
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) 
    pOut->vkGetDeviceGroupSurfacePresentModes2EXT = deviceProcAddr(device, "vkGetDeviceGroupSurfacePresentModes2EXT");
#endif //VK_EXT_full_screen_exclusive+(VK_KHR_device_group,VK_VERSION_1_1)
#if defined(VK_KHR_buffer_device_address) 
    pOut->vkGetBufferDeviceAddressKHR = deviceProcAddr(device, "vkGetBufferDeviceAddressKHR");
    pOut->vkGetBufferOpaqueCaptureAddressKHR = deviceProcAddr(device, "vkGetBufferOpaqueCaptureAddressKHR");
    pOut->vkGetDeviceMemoryOpaqueCaptureAddressKHR = deviceProcAddr(device, "vkGetDeviceMemoryOpaqueCaptureAddressKHR");
#endif //VK_KHR_buffer_device_address
#if defined(VK_EXT_line_rasterization) 
    pOut->vkCmdSetLineStippleEXT = deviceProcAddr(device, "vkCmdSetLineStippleEXT");
#endif //VK_EXT_line_rasterization
#if defined(VK_EXT_host_query_reset) 
    pOut->vkResetQueryPoolEXT = deviceProcAddr(device, "vkResetQueryPoolEXT");
#endif //VK_EXT_host_query_reset
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) 
    pOut->vkCmdSetCullModeEXT = deviceProcAddr(device, "vkCmdSetCullModeEXT");
    pOut->vkCmdSetFrontFaceEXT = deviceProcAddr(device, "vkCmdSetFrontFaceEXT");
    pOut->vkCmdSetPrimitiveTopologyEXT = deviceProcAddr(device, "vkCmdSetPrimitiveTopologyEXT");
    pOut->vkCmdSetViewportWithCountEXT = deviceProcAddr(device, "vkCmdSetViewportWithCountEXT");
    pOut->vkCmdSetScissorWithCountEXT = deviceProcAddr(device, "vkCmdSetScissorWithCountEXT");
    pOut->vkCmdBindVertexBuffers2EXT = deviceProcAddr(device, "vkCmdBindVertexBuffers2EXT");
    pOut->vkCmdSetDepthTestEnableEXT = deviceProcAddr(device, "vkCmdSetDepthTestEnableEXT");
    pOut->vkCmdSetDepthWriteEnableEXT = deviceProcAddr(device, "vkCmdSetDepthWriteEnableEXT");
    pOut->vkCmdSetDepthCompareOpEXT = deviceProcAddr(device, "vkCmdSetDepthCompareOpEXT");
    pOut->vkCmdSetDepthBoundsTestEnableEXT = deviceProcAddr(device, "vkCmdSetDepthBoundsTestEnableEXT");
    pOut->vkCmdSetStencilTestEnableEXT = deviceProcAddr(device, "vkCmdSetStencilTestEnableEXT");
    pOut->vkCmdSetStencilOpEXT = deviceProcAddr(device, "vkCmdSetStencilOpEXT");
#endif //(VK_EXT_extended_dynamic_state),(VK_EXT_shader_object)
#if defined(VK_KHR_deferred_host_operations) 
    pOut->vkCreateDeferredOperationKHR = deviceProcAddr(device, "vkCreateDeferredOperationKHR");
    pOut->vkDestroyDeferredOperationKHR = deviceProcAddr(device, "vkDestroyDeferredOperationKHR");
    pOut->vkGetDeferredOperationMaxConcurrencyKHR = deviceProcAddr(device, "vkGetDeferredOperationMaxConcurrencyKHR");
    pOut->vkGetDeferredOperationResultKHR = deviceProcAddr(device, "vkGetDeferredOperationResultKHR");
    pOut->vkDeferredOperationJoinKHR = deviceProcAddr(device, "vkDeferredOperationJoinKHR");
#endif //VK_KHR_deferred_host_operations
#if defined(VK_KHR_pipeline_executable_properties) 
    pOut->vkGetPipelineExecutablePropertiesKHR = deviceProcAddr(device, "vkGetPipelineExecutablePropertiesKHR");
    pOut->vkGetPipelineExecutableStatisticsKHR = deviceProcAddr(device, "vkGetPipelineExecutableStatisticsKHR");
    pOut->vkGetPipelineExecutableInternalRepresentationsKHR = deviceProcAddr(device, "vkGetPipelineExecutableInternalRepresentationsKHR");
#endif //VK_KHR_pipeline_executable_properties
#if defined(VK_EXT_host_image_copy) 
    pOut->vkCopyMemoryToImageEXT = deviceProcAddr(device, "vkCopyMemoryToImageEXT");
    pOut->vkCopyImageToMemoryEXT = deviceProcAddr(device, "vkCopyImageToMemoryEXT");
    pOut->vkCopyImageToImageEXT = deviceProcAddr(device, "vkCopyImageToImageEXT");
    pOut->vkTransitionImageLayoutEXT = deviceProcAddr(device, "vkTransitionImageLayoutEXT");
#endif //VK_EXT_host_image_copy
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) 
    pOut->vkGetImageSubresourceLayout2EXT = deviceProcAddr(device, "vkGetImageSubresourceLayout2EXT");
#endif //(VK_EXT_host_image_copy),(VK_EXT_image_compression_control)
#if defined(VK_KHR_map_memory2) 
    pOut->vkMapMemory2KHR = deviceProcAddr(device, "vkMapMemory2KHR");
    pOut->vkUnmapMemory2KHR = deviceProcAddr(device, "vkUnmapMemory2KHR");
#endif //VK_KHR_map_memory2
#if defined(VK_EXT_swapchain_maintenance1) 
    pOut->vkReleaseSwapchainImagesEXT = deviceProcAddr(device, "vkReleaseSwapchainImagesEXT");
#endif //VK_EXT_swapchain_maintenance1
#if defined(VK_NV_device_generated_commands) && VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION >= 3 
    pOut->vkGetGeneratedCommandsMemoryRequirementsNV = deviceProcAddr(device, "vkGetGeneratedCommandsMemoryRequirementsNV");
    pOut->vkCmdPreprocessGeneratedCommandsNV = deviceProcAddr(device, "vkCmdPreprocessGeneratedCommandsNV");
    pOut->vkCmdExecuteGeneratedCommandsNV = deviceProcAddr(device, "vkCmdExecuteGeneratedCommandsNV");
    pOut->vkCmdBindPipelineShaderGroupNV = deviceProcAddr(device, "vkCmdBindPipelineShaderGroupNV");
    pOut->vkCreateIndirectCommandsLayoutNV = deviceProcAddr(device, "vkCreateIndirectCommandsLayoutNV");
    pOut->vkDestroyIndirectCommandsLayoutNV = deviceProcAddr(device, "vkDestroyIndirectCommandsLayoutNV");
#endif //VK_NV_device_generated_commands+VK_NV_DEVICE_GENERATED_COMMANDS_SPEC_VERSION >= 3
#if defined(VK_EXT_depth_bias_control) 
    pOut->vkCmdSetDepthBias2EXT = deviceProcAddr(device, "vkCmdSetDepthBias2EXT");
#endif //VK_EXT_depth_bias_control
#if defined(VK_EXT_private_data) 
    pOut->vkCreatePrivateDataSlotEXT = deviceProcAddr(device, "vkCreatePrivateDataSlotEXT");
    pOut->vkDestroyPrivateDataSlotEXT = deviceProcAddr(device, "vkDestroyPrivateDataSlotEXT");
    pOut->vkSetPrivateDataEXT = deviceProcAddr(device, "vkSetPrivateDataEXT");
    pOut->vkGetPrivateDataEXT = deviceProcAddr(device, "vkGetPrivateDataEXT");
#endif //VK_EXT_private_data
#if defined(VK_KHR_video_encode_queue) 
    pOut->vkGetEncodedVideoSessionParametersKHR = deviceProcAddr(device, "vkGetEncodedVideoSessionParametersKHR");
    pOut->vkCmdEncodeVideoKHR = deviceProcAddr(device, "vkCmdEncodeVideoKHR");
#endif //VK_KHR_video_encode_queue
#if defined(VK_QCOM_queue_perf_hint) 
    pOut->vkQueueSetPerfHintQCOM = deviceProcAddr(device, "vkQueueSetPerfHintQCOM");
#endif //VK_QCOM_queue_perf_hint
#if defined(VK_NV_cuda_kernel_launch) && VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION >= 2 
    pOut->vkCreateCudaModuleNV = deviceProcAddr(device, "vkCreateCudaModuleNV");
    pOut->vkGetCudaModuleCacheNV = deviceProcAddr(device, "vkGetCudaModuleCacheNV");
    pOut->vkCreateCudaFunctionNV = deviceProcAddr(device, "vkCreateCudaFunctionNV");
    pOut->vkDestroyCudaModuleNV = deviceProcAddr(device, "vkDestroyCudaModuleNV");
    pOut->vkDestroyCudaFunctionNV = deviceProcAddr(device, "vkDestroyCudaFunctionNV");
    pOut->vkCmdCudaLaunchKernelNV = deviceProcAddr(device, "vkCmdCudaLaunchKernelNV");
#endif //VK_NV_cuda_kernel_launch+VK_NV_CUDA_KERNEL_LAUNCH_SPEC_VERSION >= 2
#if defined(VK_QCOM_tile_shading) && VK_QCOM_TILE_SHADING_SPEC_VERSION >= 2 
    pOut->vkCmdDispatchTileQCOM = deviceProcAddr(device, "vkCmdDispatchTileQCOM");
    pOut->vkCmdBeginPerTileExecutionQCOM = deviceProcAddr(device, "vkCmdBeginPerTileExecutionQCOM");
    pOut->vkCmdEndPerTileExecutionQCOM = deviceProcAddr(device, "vkCmdEndPerTileExecutionQCOM");
#endif //VK_QCOM_tile_shading+VK_QCOM_TILE_SHADING_SPEC_VERSION >= 2
#if defined(VK_NV_low_latency) && VK_NV_LOW_LATENCY_SPEC_VERSION >= 2 
    pOut->vkSetLatencySleepModeLegacyNV = deviceProcAddr(device, "vkSetLatencySleepModeLegacyNV");
    pOut->vkLatencySleepLegacyNV = deviceProcAddr(device, "vkLatencySleepLegacyNV");
    pOut->vkSetLatencyMarkerLegacyNV = deviceProcAddr(device, "vkSetLatencyMarkerLegacyNV");
    pOut->vkGetLatencyTimingsLegacyNV = deviceProcAddr(device, "vkGetLatencyTimingsLegacyNV");
    pOut->vkQueueNotifyOutOfBandLegacyNV = deviceProcAddr(device, "vkQueueNotifyOutOfBandLegacyNV");
    pOut->vkGetSleepStatusLegacyNV = deviceProcAddr(device, "vkGetSleepStatusLegacyNV");
    pOut->vkShutdownLatencyDeviceLegacyNV = deviceProcAddr(device, "vkShutdownLatencyDeviceLegacyNV");
#endif //VK_NV_low_latency+VK_NV_LOW_LATENCY_SPEC_VERSION >= 2
#if defined(VK_EXT_metal_objects) && VK_EXT_METAL_OBJECTS_SPEC_VERSION >= 2 
    pOut->vkExportMetalObjectsEXT = deviceProcAddr(device, "vkExportMetalObjectsEXT");
#endif //VK_EXT_metal_objects+VK_EXT_METAL_OBJECTS_SPEC_VERSION >= 2
#if defined(VK_KHR_synchronization2) 
    pOut->vkCmdSetEvent2KHR = deviceProcAddr(device, "vkCmdSetEvent2KHR");
    pOut->vkCmdResetEvent2KHR = deviceProcAddr(device, "vkCmdResetEvent2KHR");
    pOut->vkCmdWaitEvents2KHR = deviceProcAddr(device, "vkCmdWaitEvents2KHR");
    pOut->vkCmdPipelineBarrier2KHR = deviceProcAddr(device, "vkCmdPipelineBarrier2KHR");
    pOut->vkCmdWriteTimestamp2KHR = deviceProcAddr(device, "vkCmdWriteTimestamp2KHR");
    pOut->vkQueueSubmit2KHR = deviceProcAddr(device, "vkQueueSubmit2KHR");
#endif //VK_KHR_synchronization2
#if defined(VK_EXT_descriptor_buffer) 
    pOut->vkGetDescriptorSetLayoutSizeEXT = deviceProcAddr(device, "vkGetDescriptorSetLayoutSizeEXT");
    pOut->vkGetDescriptorSetLayoutBindingOffsetEXT = deviceProcAddr(device, "vkGetDescriptorSetLayoutBindingOffsetEXT");
    pOut->vkGetDescriptorEXT = deviceProcAddr(device, "vkGetDescriptorEXT");
    pOut->vkCmdBindDescriptorBuffersEXT = deviceProcAddr(device, "vkCmdBindDescriptorBuffersEXT");
    pOut->vkCmdSetDescriptorBufferOffsetsEXT = deviceProcAddr(device, "vkCmdSetDescriptorBufferOffsetsEXT");
    pOut->vkCmdBindDescriptorBufferEmbeddedSamplersEXT = deviceProcAddr(device, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT");
    pOut->vkGetBufferOpaqueCaptureDescriptorDataEXT = deviceProcAddr(device, "vkGetBufferOpaqueCaptureDescriptorDataEXT");
    pOut->vkGetImageOpaqueCaptureDescriptorDataEXT = deviceProcAddr(device, "vkGetImageOpaqueCaptureDescriptorDataEXT");
    pOut->vkGetImageViewOpaqueCaptureDescriptorDataEXT = deviceProcAddr(device, "vkGetImageViewOpaqueCaptureDescriptorDataEXT");
    pOut->vkGetSamplerOpaqueCaptureDescriptorDataEXT = deviceProcAddr(device, "vkGetSamplerOpaqueCaptureDescriptorDataEXT");
#endif //VK_EXT_descriptor_buffer
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) 
    pOut->vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT = deviceProcAddr(device, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT");
#endif //VK_EXT_descriptor_buffer+(VK_KHR_acceleration_structure,VK_NV_ray_tracing)
#if defined(VK_KHR_device_address_commands) 
    pOut->vkCmdBindIndexBuffer3KHR = deviceProcAddr(device, "vkCmdBindIndexBuffer3KHR");
    pOut->vkCmdBindVertexBuffers3KHR = deviceProcAddr(device, "vkCmdBindVertexBuffers3KHR");
    pOut->vkCmdDrawIndirect2KHR = deviceProcAddr(device, "vkCmdDrawIndirect2KHR");
    pOut->vkCmdDrawIndexedIndirect2KHR = deviceProcAddr(device, "vkCmdDrawIndexedIndirect2KHR");
    pOut->vkCmdDispatchIndirect2KHR = deviceProcAddr(device, "vkCmdDispatchIndirect2KHR");
    pOut->vkCmdCopyMemoryKHR = deviceProcAddr(device, "vkCmdCopyMemoryKHR");
    pOut->vkCmdCopyMemoryToImageKHR = deviceProcAddr(device, "vkCmdCopyMemoryToImageKHR");
    pOut->vkCmdCopyImageToMemoryKHR = deviceProcAddr(device, "vkCmdCopyImageToMemoryKHR");
    pOut->vkCmdUpdateMemoryKHR = deviceProcAddr(device, "vkCmdUpdateMemoryKHR");
    pOut->vkCmdFillMemoryKHR = deviceProcAddr(device, "vkCmdFillMemoryKHR");
    pOut->vkCmdCopyQueryPoolResultsToMemoryKHR = deviceProcAddr(device, "vkCmdCopyQueryPoolResultsToMemoryKHR");
#endif //VK_KHR_device_address_commands
#if defined(VK_KHR_device_address_commands) && (defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2)) 
    pOut->vkCmdDrawIndirectCount2KHR = deviceProcAddr(device, "vkCmdDrawIndirectCount2KHR");
    pOut->vkCmdDrawIndexedIndirectCount2KHR = deviceProcAddr(device, "vkCmdDrawIndexedIndirectCount2KHR");
#endif //VK_KHR_device_address_commands+(VK_KHR_draw_indirect_count,VK_VERSION_1_2)
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_conditional_rendering) 
    pOut->vkCmdBeginConditionalRendering2EXT = deviceProcAddr(device, "vkCmdBeginConditionalRendering2EXT");
#endif //VK_KHR_device_address_commands+VK_EXT_conditional_rendering
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_transform_feedback) 
    pOut->vkCmdBindTransformFeedbackBuffers2EXT = deviceProcAddr(device, "vkCmdBindTransformFeedbackBuffers2EXT");
    pOut->vkCmdBeginTransformFeedback2EXT = deviceProcAddr(device, "vkCmdBeginTransformFeedback2EXT");
    pOut->vkCmdEndTransformFeedback2EXT = deviceProcAddr(device, "vkCmdEndTransformFeedback2EXT");
    pOut->vkCmdDrawIndirectByteCount2EXT = deviceProcAddr(device, "vkCmdDrawIndirectByteCount2EXT");
#endif //VK_KHR_device_address_commands+VK_EXT_transform_feedback
#if defined(VK_KHR_device_address_commands) && defined(VK_EXT_mesh_shader) 
    pOut->vkCmdDrawMeshTasksIndirect2EXT = deviceProcAddr(device, "vkCmdDrawMeshTasksIndirect2EXT");
#endif //VK_KHR_device_address_commands+VK_EXT_mesh_shader
#if defined(VK_KHR_device_address_commands) && ((defined(VK_KHR_draw_indirect_count) || defined(VK_VERSION_1_2)) && defined(VK_EXT_mesh_shader)) 
    pOut->vkCmdDrawMeshTasksIndirectCount2EXT = deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectCount2EXT");
#endif //VK_KHR_device_address_commands+((VK_KHR_draw_indirect_count,VK_VERSION_1_2)+VK_EXT_mesh_shader)
#if defined(VK_KHR_device_address_commands) && defined(VK_AMD_buffer_marker) 
    pOut->vkCmdWriteMarkerToMemoryAMD = deviceProcAddr(device, "vkCmdWriteMarkerToMemoryAMD");
#endif //VK_KHR_device_address_commands+VK_AMD_buffer_marker
#if defined(VK_KHR_device_address_commands) && defined(VK_KHR_acceleration_structure) 
    pOut->vkCreateAccelerationStructure2KHR = deviceProcAddr(device, "vkCreateAccelerationStructure2KHR");
#endif //VK_KHR_device_address_commands+VK_KHR_acceleration_structure
#if defined(VK_NV_fragment_shading_rate_enums) 
    pOut->vkCmdSetFragmentShadingRateEnumNV = deviceProcAddr(device, "vkCmdSetFragmentShadingRateEnumNV");
#endif //VK_NV_fragment_shading_rate_enums
#if defined(VK_EXT_mesh_shader) 
    pOut->vkCmdDrawMeshTasksEXT = deviceProcAddr(device, "vkCmdDrawMeshTasksEXT");
    pOut->vkCmdDrawMeshTasksIndirectEXT = deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectEXT");
#endif //VK_EXT_mesh_shader
#if defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) 
    pOut->vkCmdDrawMeshTasksIndirectCountEXT = deviceProcAddr(device, "vkCmdDrawMeshTasksIndirectCountEXT");
#endif //VK_EXT_mesh_shader+(VK_VERSION_1_2,VK_KHR_draw_indirect_count,VK_AMD_draw_indirect_count)
#if defined(VK_KHR_copy_commands2) 
    pOut->vkCmdCopyBuffer2KHR = deviceProcAddr(device, "vkCmdCopyBuffer2KHR");
    pOut->vkCmdCopyImage2KHR = deviceProcAddr(device, "vkCmdCopyImage2KHR");
    pOut->vkCmdCopyBufferToImage2KHR = deviceProcAddr(device, "vkCmdCopyBufferToImage2KHR");
    pOut->vkCmdCopyImageToBuffer2KHR = deviceProcAddr(device, "vkCmdCopyImageToBuffer2KHR");
    pOut->vkCmdBlitImage2KHR = deviceProcAddr(device, "vkCmdBlitImage2KHR");
    pOut->vkCmdResolveImage2KHR = deviceProcAddr(device, "vkCmdResolveImage2KHR");
#endif //VK_KHR_copy_commands2
#if defined(VK_EXT_device_fault) && VK_EXT_DEVICE_FAULT_SPEC_VERSION >= 2 
    pOut->vkGetDeviceFaultInfoEXT = deviceProcAddr(device, "vkGetDeviceFaultInfoEXT");
#endif //VK_EXT_device_fault+VK_EXT_DEVICE_FAULT_SPEC_VERSION >= 2
#if (defined(VK_EXT_vertex_input_dynamic_state) && VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
    pOut->vkCmdSetVertexInputEXT = deviceProcAddr(device, "vkCmdSetVertexInputEXT");
#endif //(VK_EXT_vertex_input_dynamic_state+VK_EXT_VERTEX_INPUT_DYNAMIC_STATE_SPEC_VERSION >= 2),(VK_EXT_shader_object)
#if defined(VK_FUCHSIA_external_memory) 
    pOut->vkGetMemoryZirconHandleFUCHSIA = deviceProcAddr(device, "vkGetMemoryZirconHandleFUCHSIA");
    pOut->vkGetMemoryZirconHandlePropertiesFUCHSIA = deviceProcAddr(device, "vkGetMemoryZirconHandlePropertiesFUCHSIA");
#endif //VK_FUCHSIA_external_memory
#if defined(VK_FUCHSIA_external_semaphore) 
    pOut->vkImportSemaphoreZirconHandleFUCHSIA = deviceProcAddr(device, "vkImportSemaphoreZirconHandleFUCHSIA");
    pOut->vkGetSemaphoreZirconHandleFUCHSIA = deviceProcAddr(device, "vkGetSemaphoreZirconHandleFUCHSIA");
#endif //VK_FUCHSIA_external_semaphore
#if defined(VK_FUCHSIA_buffer_collection) && VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION >= 2 
    pOut->vkCreateBufferCollectionFUCHSIA = deviceProcAddr(device, "vkCreateBufferCollectionFUCHSIA");
    pOut->vkSetBufferCollectionImageConstraintsFUCHSIA = deviceProcAddr(device, "vkSetBufferCollectionImageConstraintsFUCHSIA");
    pOut->vkSetBufferCollectionBufferConstraintsFUCHSIA = deviceProcAddr(device, "vkSetBufferCollectionBufferConstraintsFUCHSIA");
    pOut->vkDestroyBufferCollectionFUCHSIA = deviceProcAddr(device, "vkDestroyBufferCollectionFUCHSIA");
    pOut->vkGetBufferCollectionPropertiesFUCHSIA = deviceProcAddr(device, "vkGetBufferCollectionPropertiesFUCHSIA");
#endif //VK_FUCHSIA_buffer_collection+VK_FUCHSIA_BUFFER_COLLECTION_SPEC_VERSION >= 2
#if defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 3 
    pOut->vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = deviceProcAddr(device, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI");
    pOut->vkCmdSubpassShadingHUAWEI = deviceProcAddr(device, "vkCmdSubpassShadingHUAWEI");
#endif //VK_HUAWEI_subpass_shading+VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 3
#if defined(VK_HUAWEI_invocation_mask) 
    pOut->vkCmdBindInvocationMaskHUAWEI = deviceProcAddr(device, "vkCmdBindInvocationMaskHUAWEI");
#endif //VK_HUAWEI_invocation_mask
#if defined(VK_NV_external_memory_rdma) 
    pOut->vkGetMemoryRemoteAddressNV = deviceProcAddr(device, "vkGetMemoryRemoteAddressNV");
#endif //VK_NV_external_memory_rdma
#if defined(VK_EXT_pipeline_properties) 
    pOut->vkGetPipelinePropertiesEXT = deviceProcAddr(device, "vkGetPipelinePropertiesEXT");
#endif //VK_EXT_pipeline_properties
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) 
    pOut->vkCmdSetPatchControlPointsEXT = deviceProcAddr(device, "vkCmdSetPatchControlPointsEXT");
    pOut->vkCmdSetRasterizerDiscardEnableEXT = deviceProcAddr(device, "vkCmdSetRasterizerDiscardEnableEXT");
    pOut->vkCmdSetDepthBiasEnableEXT = deviceProcAddr(device, "vkCmdSetDepthBiasEnableEXT");
    pOut->vkCmdSetLogicOpEXT = deviceProcAddr(device, "vkCmdSetLogicOpEXT");
    pOut->vkCmdSetPrimitiveRestartEnableEXT = deviceProcAddr(device, "vkCmdSetPrimitiveRestartEnableEXT");
#endif //(VK_EXT_extended_dynamic_state2),(VK_EXT_shader_object)
#if defined(VK_EXT_color_write_enable) 
    pOut->vkCmdSetColorWriteEnableEXT = deviceProcAddr(device, "vkCmdSetColorWriteEnableEXT");
#endif //VK_EXT_color_write_enable
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) 
    pOut->vkCmdTraceRaysIndirect2KHR = deviceProcAddr(device, "vkCmdTraceRaysIndirect2KHR");
#endif //VK_KHR_ray_tracing_maintenance1+VK_KHR_ray_tracing_pipeline
#if defined(VK_EXT_multi_draw) 
    pOut->vkCmdDrawMultiEXT = deviceProcAddr(device, "vkCmdDrawMultiEXT");
    pOut->vkCmdDrawMultiIndexedEXT = deviceProcAddr(device, "vkCmdDrawMultiIndexedEXT");
#endif //VK_EXT_multi_draw
#if defined(VK_EXT_opacity_micromap) && VK_EXT_OPACITY_MICROMAP_SPEC_VERSION >= 2 
    pOut->vkCreateMicromapEXT = deviceProcAddr(device, "vkCreateMicromapEXT");
    pOut->vkDestroyMicromapEXT = deviceProcAddr(device, "vkDestroyMicromapEXT");
    pOut->vkCmdBuildMicromapsEXT = deviceProcAddr(device, "vkCmdBuildMicromapsEXT");
    pOut->vkBuildMicromapsEXT = deviceProcAddr(device, "vkBuildMicromapsEXT");
    pOut->vkCopyMicromapEXT = deviceProcAddr(device, "vkCopyMicromapEXT");
    pOut->vkCopyMicromapToMemoryEXT = deviceProcAddr(device, "vkCopyMicromapToMemoryEXT");
    pOut->vkCopyMemoryToMicromapEXT = deviceProcAddr(device, "vkCopyMemoryToMicromapEXT");
    pOut->vkWriteMicromapsPropertiesEXT = deviceProcAddr(device, "vkWriteMicromapsPropertiesEXT");
    pOut->vkCmdCopyMicromapEXT = deviceProcAddr(device, "vkCmdCopyMicromapEXT");
    pOut->vkCmdCopyMicromapToMemoryEXT = deviceProcAddr(device, "vkCmdCopyMicromapToMemoryEXT");
    pOut->vkCmdCopyMemoryToMicromapEXT = deviceProcAddr(device, "vkCmdCopyMemoryToMicromapEXT");
    pOut->vkCmdWriteMicromapsPropertiesEXT = deviceProcAddr(device, "vkCmdWriteMicromapsPropertiesEXT");
    pOut->vkGetDeviceMicromapCompatibilityEXT = deviceProcAddr(device, "vkGetDeviceMicromapCompatibilityEXT");
    pOut->vkGetMicromapBuildSizesEXT = deviceProcAddr(device, "vkGetMicromapBuildSizesEXT");
#endif //VK_EXT_opacity_micromap+VK_EXT_OPACITY_MICROMAP_SPEC_VERSION >= 2
#if defined(VK_HUAWEI_cluster_culling_shader) && VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION >= 3 
    pOut->vkCmdDrawClusterHUAWEI = deviceProcAddr(device, "vkCmdDrawClusterHUAWEI");
    pOut->vkCmdDrawClusterIndirectHUAWEI = deviceProcAddr(device, "vkCmdDrawClusterIndirectHUAWEI");
#endif //VK_HUAWEI_cluster_culling_shader+VK_HUAWEI_CLUSTER_CULLING_SHADER_SPEC_VERSION >= 3
#if defined(VK_EXT_pageable_device_local_memory) 
    pOut->vkSetDeviceMemoryPriorityEXT = deviceProcAddr(device, "vkSetDeviceMemoryPriorityEXT");
#endif //VK_EXT_pageable_device_local_memory
#if defined(VK_KHR_maintenance4) 
    pOut->vkGetDeviceBufferMemoryRequirementsKHR = deviceProcAddr(device, "vkGetDeviceBufferMemoryRequirementsKHR");
    pOut->vkGetDeviceImageMemoryRequirementsKHR = deviceProcAddr(device, "vkGetDeviceImageMemoryRequirementsKHR");
    pOut->vkGetDeviceImageSparseMemoryRequirementsKHR = deviceProcAddr(device, "vkGetDeviceImageSparseMemoryRequirementsKHR");
#endif //VK_KHR_maintenance4
#if defined(VK_ARM_scheduling_controls) && VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION >= 2 
    pOut->vkCmdSetDispatchParametersARM = deviceProcAddr(device, "vkCmdSetDispatchParametersARM");
#endif //VK_ARM_scheduling_controls+VK_ARM_SCHEDULING_CONTROLS_SPEC_VERSION >= 2
#if defined(VK_VALVE_descriptor_set_host_mapping) 
    pOut->vkGetDescriptorSetLayoutHostMappingInfoVALVE = deviceProcAddr(device, "vkGetDescriptorSetLayoutHostMappingInfoVALVE");
    pOut->vkGetDescriptorSetHostMappingVALVE = deviceProcAddr(device, "vkGetDescriptorSetHostMappingVALVE");
#endif //VK_VALVE_descriptor_set_host_mapping
#if defined(VK_NV_copy_memory_indirect) 
    pOut->vkCmdCopyMemoryIndirectNV = deviceProcAddr(device, "vkCmdCopyMemoryIndirectNV");
    pOut->vkCmdCopyMemoryToImageIndirectNV = deviceProcAddr(device, "vkCmdCopyMemoryToImageIndirectNV");
#endif //VK_NV_copy_memory_indirect
#if defined(VK_NV_memory_decompression) 
    pOut->vkCmdDecompressMemoryNV = deviceProcAddr(device, "vkCmdDecompressMemoryNV");
    pOut->vkCmdDecompressMemoryIndirectCountNV = deviceProcAddr(device, "vkCmdDecompressMemoryIndirectCountNV");
#endif //VK_NV_memory_decompression
#if defined(VK_NV_device_generated_commands_compute) && VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION >= 2 
    pOut->vkGetPipelineIndirectMemoryRequirementsNV = deviceProcAddr(device, "vkGetPipelineIndirectMemoryRequirementsNV");
    pOut->vkCmdUpdatePipelineIndirectBufferNV = deviceProcAddr(device, "vkCmdUpdatePipelineIndirectBufferNV");
    pOut->vkGetPipelineIndirectDeviceAddressNV = deviceProcAddr(device, "vkGetPipelineIndirectDeviceAddressNV");
#endif //VK_NV_device_generated_commands_compute+VK_NV_DEVICE_GENERATED_COMMANDS_COMPUTE_SPEC_VERSION >= 2
#if defined(VK_OHOS_external_memory) 
    pOut->vkGetNativeBufferPropertiesOHOS = deviceProcAddr(device, "vkGetNativeBufferPropertiesOHOS");
    pOut->vkGetMemoryNativeBufferOHOS = deviceProcAddr(device, "vkGetMemoryNativeBufferOHOS");
#endif //VK_OHOS_external_memory
#if (defined(VK_EXT_extended_dynamic_state3) && VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION >= 2) || (defined(VK_EXT_shader_object)) 
    pOut->vkCmdSetDepthClampEnableEXT = deviceProcAddr(device, "vkCmdSetDepthClampEnableEXT");
    pOut->vkCmdSetPolygonModeEXT = deviceProcAddr(device, "vkCmdSetPolygonModeEXT");
    pOut->vkCmdSetRasterizationSamplesEXT = deviceProcAddr(device, "vkCmdSetRasterizationSamplesEXT");
    pOut->vkCmdSetSampleMaskEXT = deviceProcAddr(device, "vkCmdSetSampleMaskEXT");
    pOut->vkCmdSetAlphaToCoverageEnableEXT = deviceProcAddr(device, "vkCmdSetAlphaToCoverageEnableEXT");
    pOut->vkCmdSetAlphaToOneEnableEXT = deviceProcAddr(device, "vkCmdSetAlphaToOneEnableEXT");
    pOut->vkCmdSetLogicOpEnableEXT = deviceProcAddr(device, "vkCmdSetLogicOpEnableEXT");
    pOut->vkCmdSetColorBlendEnableEXT = deviceProcAddr(device, "vkCmdSetColorBlendEnableEXT");
    pOut->vkCmdSetColorBlendEquationEXT = deviceProcAddr(device, "vkCmdSetColorBlendEquationEXT");
    pOut->vkCmdSetColorWriteMaskEXT = deviceProcAddr(device, "vkCmdSetColorWriteMaskEXT");
#endif //(VK_EXT_extended_dynamic_state3+VK_EXT_EXTENDED_DYNAMIC_STATE_3_SPEC_VERSION >= 2),(VK_EXT_shader_object)
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) 
    pOut->vkCmdSetTessellationDomainOriginEXT = deviceProcAddr(device, "vkCmdSetTessellationDomainOriginEXT");
#endif //(VK_EXT_extended_dynamic_state3+(VK_KHR_maintenance2,VK_VERSION_1_1)),(VK_EXT_shader_object)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) 
    pOut->vkCmdSetRasterizationStreamEXT = deviceProcAddr(device, "vkCmdSetRasterizationStreamEXT");
#endif //(VK_EXT_extended_dynamic_state3+VK_EXT_transform_feedback),(VK_EXT_shader_object+VK_EXT_transform_feedback)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) 
    pOut->vkCmdSetConservativeRasterizationModeEXT = deviceProcAddr(device, "vkCmdSetConservativeRasterizationModeEXT");
    pOut->vkCmdSetExtraPrimitiveOverestimationSizeEXT = deviceProcAddr(device, "vkCmdSetExtraPrimitiveOverestimationSizeEXT");
#endif //(VK_EXT_extended_dynamic_state3+VK_EXT_conservative_rasterization),(VK_EXT_shader_object+VK_EXT_conservative_rasterization)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) 
    pOut->vkCmdSetDepthClipEnableEXT = deviceProcAddr(device, "vkCmdSetDepthClipEnableEXT");
#endif //(VK_EXT_extended_dynamic_state3+VK_EXT_depth_clip_enable),(VK_EXT_shader_object+VK_EXT_depth_clip_enable)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) 
    pOut->vkCmdSetSampleLocationsEnableEXT = deviceProcAddr(device, "vkCmdSetSampleLocationsEnableEXT");
#endif //(VK_EXT_extended_dynamic_state3+VK_EXT_sample_locations),(VK_EXT_shader_object+VK_EXT_sample_locations)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) 
    pOut->vkCmdSetColorBlendAdvancedEXT = deviceProcAddr(device, "vkCmdSetColorBlendAdvancedEXT");
#endif //(VK_EXT_extended_dynamic_state3+VK_EXT_blend_operation_advanced),(VK_EXT_shader_object+VK_EXT_blend_operation_advanced)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) 
    pOut->vkCmdSetProvokingVertexModeEXT = deviceProcAddr(device, "vkCmdSetProvokingVertexModeEXT");
#endif //(VK_EXT_extended_dynamic_state3+VK_EXT_provoking_vertex),(VK_EXT_shader_object+VK_EXT_provoking_vertex)
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) || (defined(VK_EXT_shader_object) && (defined(VK_VERSION_1_4) || defined(VK_KHR_line_rasterization) || defined(VK_EXT_line_rasterization))) 
    pOut->vkCmdSetLineRasterizationModeEXT = deviceProcAddr(device, "vkCmdSetLineRasterizationModeEXT");
    pOut->vkCmdSetLineStippleEnableEXT = deviceProcAddr(device, "vkCmdSetLineStippleEnableEXT");
#endif //(VK_EXT_extended_dynamic_state3+(VK_VERSION_1_4,VK_KHR_line_rasterization,VK_EXT_line_rasterization)),(VK_EXT_shader_object+(VK_VERSION_1_4,VK_KHR_line_rasterization,VK_EXT_line_rasterization))
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) 
    pOut->vkCmdSetDepthClipNegativeOneToOneEXT = deviceProcAddr(device, "vkCmdSetDepthClipNegativeOneToOneEXT");
#endif //(VK_EXT_extended_dynamic_state3+VK_EXT_depth_clip_control),(VK_EXT_shader_object+VK_EXT_depth_clip_control)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) 
    pOut->vkCmdSetViewportWScalingEnableNV = deviceProcAddr(device, "vkCmdSetViewportWScalingEnableNV");
#endif //(VK_EXT_extended_dynamic_state3+VK_NV_clip_space_w_scaling),(VK_EXT_shader_object+VK_NV_clip_space_w_scaling)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) 
    pOut->vkCmdSetViewportSwizzleNV = deviceProcAddr(device, "vkCmdSetViewportSwizzleNV");
#endif //(VK_EXT_extended_dynamic_state3+VK_NV_viewport_swizzle),(VK_EXT_shader_object+VK_NV_viewport_swizzle)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) 
    pOut->vkCmdSetCoverageToColorEnableNV = deviceProcAddr(device, "vkCmdSetCoverageToColorEnableNV");
    pOut->vkCmdSetCoverageToColorLocationNV = deviceProcAddr(device, "vkCmdSetCoverageToColorLocationNV");
#endif //(VK_EXT_extended_dynamic_state3+VK_NV_fragment_coverage_to_color),(VK_EXT_shader_object+VK_NV_fragment_coverage_to_color)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) 
    pOut->vkCmdSetCoverageModulationModeNV = deviceProcAddr(device, "vkCmdSetCoverageModulationModeNV");
    pOut->vkCmdSetCoverageModulationTableEnableNV = deviceProcAddr(device, "vkCmdSetCoverageModulationTableEnableNV");
    pOut->vkCmdSetCoverageModulationTableNV = deviceProcAddr(device, "vkCmdSetCoverageModulationTableNV");
#endif //(VK_EXT_extended_dynamic_state3+VK_NV_framebuffer_mixed_samples),(VK_EXT_shader_object+VK_NV_framebuffer_mixed_samples)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) 
    pOut->vkCmdSetShadingRateImageEnableNV = deviceProcAddr(device, "vkCmdSetShadingRateImageEnableNV");
#endif //(VK_EXT_extended_dynamic_state3+VK_NV_shading_rate_image),(VK_EXT_shader_object+VK_NV_shading_rate_image)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) 
    pOut->vkCmdSetRepresentativeFragmentTestEnableNV = deviceProcAddr(device, "vkCmdSetRepresentativeFragmentTestEnableNV");
#endif //(VK_EXT_extended_dynamic_state3+VK_NV_representative_fragment_test),(VK_EXT_shader_object+VK_NV_representative_fragment_test)
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) 
    pOut->vkCmdSetCoverageReductionModeNV = deviceProcAddr(device, "vkCmdSetCoverageReductionModeNV");
#endif //(VK_EXT_extended_dynamic_state3+VK_NV_coverage_reduction_mode),(VK_EXT_shader_object+VK_NV_coverage_reduction_mode)
#if defined(VK_ARM_tensors) && VK_ARM_TENSORS_SPEC_VERSION >= 2 
    pOut->vkCreateTensorARM = deviceProcAddr(device, "vkCreateTensorARM");
    pOut->vkDestroyTensorARM = deviceProcAddr(device, "vkDestroyTensorARM");
    pOut->vkCreateTensorViewARM = deviceProcAddr(device, "vkCreateTensorViewARM");
    pOut->vkDestroyTensorViewARM = deviceProcAddr(device, "vkDestroyTensorViewARM");
    pOut->vkGetTensorMemoryRequirementsARM = deviceProcAddr(device, "vkGetTensorMemoryRequirementsARM");
    pOut->vkBindTensorMemoryARM = deviceProcAddr(device, "vkBindTensorMemoryARM");
    pOut->vkGetDeviceTensorMemoryRequirementsARM = deviceProcAddr(device, "vkGetDeviceTensorMemoryRequirementsARM");
    pOut->vkCmdCopyTensorARM = deviceProcAddr(device, "vkCmdCopyTensorARM");
#endif //VK_ARM_tensors+VK_ARM_TENSORS_SPEC_VERSION >= 2
#if defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer) 
    pOut->vkGetTensorOpaqueCaptureDescriptorDataARM = deviceProcAddr(device, "vkGetTensorOpaqueCaptureDescriptorDataARM");
    pOut->vkGetTensorViewOpaqueCaptureDescriptorDataARM = deviceProcAddr(device, "vkGetTensorViewOpaqueCaptureDescriptorDataARM");
#endif //VK_ARM_tensors+VK_EXT_descriptor_buffer
#if defined(VK_EXT_shader_module_identifier) 
    pOut->vkGetShaderModuleIdentifierEXT = deviceProcAddr(device, "vkGetShaderModuleIdentifierEXT");
    pOut->vkGetShaderModuleCreateInfoIdentifierEXT = deviceProcAddr(device, "vkGetShaderModuleCreateInfoIdentifierEXT");
#endif //VK_EXT_shader_module_identifier
#if defined(VK_NV_optical_flow) 
    pOut->vkCreateOpticalFlowSessionNV = deviceProcAddr(device, "vkCreateOpticalFlowSessionNV");
    pOut->vkDestroyOpticalFlowSessionNV = deviceProcAddr(device, "vkDestroyOpticalFlowSessionNV");
    pOut->vkBindOpticalFlowSessionImageNV = deviceProcAddr(device, "vkBindOpticalFlowSessionImageNV");
    pOut->vkCmdOpticalFlowExecuteNV = deviceProcAddr(device, "vkCmdOpticalFlowExecuteNV");
#endif //VK_NV_optical_flow
#if defined(VK_KHR_maintenance5) 
    pOut->vkCmdBindIndexBuffer2KHR = deviceProcAddr(device, "vkCmdBindIndexBuffer2KHR");
    pOut->vkGetRenderingAreaGranularityKHR = deviceProcAddr(device, "vkGetRenderingAreaGranularityKHR");
    pOut->vkGetDeviceImageSubresourceLayoutKHR = deviceProcAddr(device, "vkGetDeviceImageSubresourceLayoutKHR");
    pOut->vkGetImageSubresourceLayout2KHR = deviceProcAddr(device, "vkGetImageSubresourceLayout2KHR");
#endif //VK_KHR_maintenance5
#if defined(VK_AMD_anti_lag) 
    pOut->vkAntiLagUpdateAMD = deviceProcAddr(device, "vkAntiLagUpdateAMD");
#endif //VK_AMD_anti_lag
#if defined(VK_KHR_present_wait2) 
    pOut->vkWaitForPresent2KHR = deviceProcAddr(device, "vkWaitForPresent2KHR");
#endif //VK_KHR_present_wait2
#if defined(VK_EXT_shader_object) 
    pOut->vkCreateShadersEXT = deviceProcAddr(device, "vkCreateShadersEXT");
    pOut->vkDestroyShaderEXT = deviceProcAddr(device, "vkDestroyShaderEXT");
    pOut->vkGetShaderBinaryDataEXT = deviceProcAddr(device, "vkGetShaderBinaryDataEXT");
    pOut->vkCmdBindShadersEXT = deviceProcAddr(device, "vkCmdBindShadersEXT");
#endif //VK_EXT_shader_object
#if (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_depth_clamp_control)) 
    pOut->vkCmdSetDepthClampRangeEXT = deviceProcAddr(device, "vkCmdSetDepthClampRangeEXT");
#endif //(VK_EXT_shader_object+VK_EXT_depth_clamp_control),(VK_EXT_depth_clamp_control)
#if defined(VK_KHR_pipeline_binary) 
    pOut->vkCreatePipelineBinariesKHR = deviceProcAddr(device, "vkCreatePipelineBinariesKHR");
    pOut->vkDestroyPipelineBinaryKHR = deviceProcAddr(device, "vkDestroyPipelineBinaryKHR");
    pOut->vkGetPipelineKeyKHR = deviceProcAddr(device, "vkGetPipelineKeyKHR");
    pOut->vkGetPipelineBinaryDataKHR = deviceProcAddr(device, "vkGetPipelineBinaryDataKHR");
    pOut->vkReleaseCapturedPipelineDataKHR = deviceProcAddr(device, "vkReleaseCapturedPipelineDataKHR");
#endif //VK_KHR_pipeline_binary
#if defined(VK_QCOM_tile_properties) 
    pOut->vkGetFramebufferTilePropertiesQCOM = deviceProcAddr(device, "vkGetFramebufferTilePropertiesQCOM");
    pOut->vkGetDynamicRenderingTilePropertiesQCOM = deviceProcAddr(device, "vkGetDynamicRenderingTilePropertiesQCOM");
#endif //VK_QCOM_tile_properties
#if defined(VK_KHR_swapchain_maintenance1) 
    pOut->vkReleaseSwapchainImagesKHR = deviceProcAddr(device, "vkReleaseSwapchainImagesKHR");
#endif //VK_KHR_swapchain_maintenance1
#if defined(VK_NV_cooperative_vector) && VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4 
    pOut->vkConvertCooperativeVectorMatrixNV = deviceProcAddr(device, "vkConvertCooperativeVectorMatrixNV");
    pOut->vkCmdConvertCooperativeVectorMatrixNV = deviceProcAddr(device, "vkCmdConvertCooperativeVectorMatrixNV");
#endif //VK_NV_cooperative_vector+VK_NV_COOPERATIVE_VECTOR_SPEC_VERSION >= 4
#if defined(VK_NV_low_latency2) && VK_NV_LOW_LATENCY_2_SPEC_VERSION >= 3 
    pOut->vkSetLatencySleepModeNV = deviceProcAddr(device, "vkSetLatencySleepModeNV");
    pOut->vkLatencySleepNV = deviceProcAddr(device, "vkLatencySleepNV");
    pOut->vkSetLatencyMarkerNV = deviceProcAddr(device, "vkSetLatencyMarkerNV");
    pOut->vkGetLatencyTimingsNV = deviceProcAddr(device, "vkGetLatencyTimingsNV");
    pOut->vkQueueNotifyOutOfBandNV = deviceProcAddr(device, "vkQueueNotifyOutOfBandNV");
#endif //VK_NV_low_latency2+VK_NV_LOW_LATENCY_2_SPEC_VERSION >= 3
#if defined(VK_ARM_data_graph) 
    pOut->vkCreateDataGraphPipelinesARM = deviceProcAddr(device, "vkCreateDataGraphPipelinesARM");
    pOut->vkCreateDataGraphPipelineSessionARM = deviceProcAddr(device, "vkCreateDataGraphPipelineSessionARM");
    pOut->vkGetDataGraphPipelineSessionBindPointRequirementsARM = deviceProcAddr(device, "vkGetDataGraphPipelineSessionBindPointRequirementsARM");
    pOut->vkGetDataGraphPipelineSessionMemoryRequirementsARM = deviceProcAddr(device, "vkGetDataGraphPipelineSessionMemoryRequirementsARM");
    pOut->vkBindDataGraphPipelineSessionMemoryARM = deviceProcAddr(device, "vkBindDataGraphPipelineSessionMemoryARM");
    pOut->vkDestroyDataGraphPipelineSessionARM = deviceProcAddr(device, "vkDestroyDataGraphPipelineSessionARM");
    pOut->vkCmdDispatchDataGraphARM = deviceProcAddr(device, "vkCmdDispatchDataGraphARM");
    pOut->vkGetDataGraphPipelineAvailablePropertiesARM = deviceProcAddr(device, "vkGetDataGraphPipelineAvailablePropertiesARM");
    pOut->vkGetDataGraphPipelinePropertiesARM = deviceProcAddr(device, "vkGetDataGraphPipelinePropertiesARM");
#endif //VK_ARM_data_graph
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state) 
    pOut->vkCmdSetAttachmentFeedbackLoopEnableEXT = deviceProcAddr(device, "vkCmdSetAttachmentFeedbackLoopEnableEXT");
#endif //VK_EXT_attachment_feedback_loop_dynamic_state
#if defined(VK_QNX_external_memory_screen_buffer) 
    pOut->vkGetScreenBufferPropertiesQNX = deviceProcAddr(device, "vkGetScreenBufferPropertiesQNX");
#endif //VK_QNX_external_memory_screen_buffer
#if defined(VK_KHR_line_rasterization) 
    pOut->vkCmdSetLineStippleKHR = deviceProcAddr(device, "vkCmdSetLineStippleKHR");
#endif //VK_KHR_line_rasterization
#if defined(VK_KHR_calibrated_timestamps) 
    pOut->vkGetCalibratedTimestampsKHR = deviceProcAddr(device, "vkGetCalibratedTimestampsKHR");
#endif //VK_KHR_calibrated_timestamps
#if defined(VK_KHR_maintenance6) 
    pOut->vkCmdBindDescriptorSets2KHR = deviceProcAddr(device, "vkCmdBindDescriptorSets2KHR");
    pOut->vkCmdPushConstants2KHR = deviceProcAddr(device, "vkCmdPushConstants2KHR");
#endif //VK_KHR_maintenance6
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) 
    pOut->vkCmdPushDescriptorSet2KHR = deviceProcAddr(device, "vkCmdPushDescriptorSet2KHR");
    pOut->vkCmdPushDescriptorSetWithTemplate2KHR = deviceProcAddr(device, "vkCmdPushDescriptorSetWithTemplate2KHR");
#endif //VK_KHR_maintenance6+VK_KHR_push_descriptor
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) 
    pOut->vkCmdSetDescriptorBufferOffsets2EXT = deviceProcAddr(device, "vkCmdSetDescriptorBufferOffsets2EXT");
    pOut->vkCmdBindDescriptorBufferEmbeddedSamplers2EXT = deviceProcAddr(device, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT");
#endif //VK_KHR_maintenance6+VK_EXT_descriptor_buffer
#if defined(VK_QCOM_tile_memory_heap) 
    pOut->vkCmdBindTileMemoryQCOM = deviceProcAddr(device, "vkCmdBindTileMemoryQCOM");
#endif //VK_QCOM_tile_memory_heap
#if defined(VK_KHR_copy_memory_indirect) 
    pOut->vkCmdCopyMemoryIndirectKHR = deviceProcAddr(device, "vkCmdCopyMemoryIndirectKHR");
    pOut->vkCmdCopyMemoryToImageIndirectKHR = deviceProcAddr(device, "vkCmdCopyMemoryToImageIndirectKHR");
#endif //VK_KHR_copy_memory_indirect
#if defined(VK_EXT_memory_decompression) 
    pOut->vkCmdDecompressMemoryEXT = deviceProcAddr(device, "vkCmdDecompressMemoryEXT");
    pOut->vkCmdDecompressMemoryIndirectCountEXT = deviceProcAddr(device, "vkCmdDecompressMemoryIndirectCountEXT");
#endif //VK_EXT_memory_decompression
#if defined(VK_NV_external_compute_queue) 
    pOut->vkCreateExternalComputeQueueNV = deviceProcAddr(device, "vkCreateExternalComputeQueueNV");
    pOut->vkDestroyExternalComputeQueueNV = deviceProcAddr(device, "vkDestroyExternalComputeQueueNV");
    pOut->vkGetExternalComputeQueueDataNV = deviceProcAddr(device, "vkGetExternalComputeQueueDataNV");
#endif //VK_NV_external_compute_queue
#if defined(VK_NV_cluster_acceleration_structure) && VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION >= 4 
    pOut->vkGetClusterAccelerationStructureBuildSizesNV = deviceProcAddr(device, "vkGetClusterAccelerationStructureBuildSizesNV");
    pOut->vkCmdBuildClusterAccelerationStructureIndirectNV = deviceProcAddr(device, "vkCmdBuildClusterAccelerationStructureIndirectNV");
#endif //VK_NV_cluster_acceleration_structure+VK_NV_CLUSTER_ACCELERATION_STRUCTURE_SPEC_VERSION >= 4
#if defined(VK_NV_partitioned_acceleration_structure) 
    pOut->vkGetPartitionedAccelerationStructuresBuildSizesNV = deviceProcAddr(device, "vkGetPartitionedAccelerationStructuresBuildSizesNV");
    pOut->vkCmdBuildPartitionedAccelerationStructuresNV = deviceProcAddr(device, "vkCmdBuildPartitionedAccelerationStructuresNV");
#endif //VK_NV_partitioned_acceleration_structure
#if defined(VK_EXT_device_generated_commands) 
    pOut->vkGetGeneratedCommandsMemoryRequirementsEXT = deviceProcAddr(device, "vkGetGeneratedCommandsMemoryRequirementsEXT");
    pOut->vkCmdPreprocessGeneratedCommandsEXT = deviceProcAddr(device, "vkCmdPreprocessGeneratedCommandsEXT");
    pOut->vkCmdExecuteGeneratedCommandsEXT = deviceProcAddr(device, "vkCmdExecuteGeneratedCommandsEXT");
    pOut->vkCreateIndirectCommandsLayoutEXT = deviceProcAddr(device, "vkCreateIndirectCommandsLayoutEXT");
    pOut->vkDestroyIndirectCommandsLayoutEXT = deviceProcAddr(device, "vkDestroyIndirectCommandsLayoutEXT");
    pOut->vkCreateIndirectExecutionSetEXT = deviceProcAddr(device, "vkCreateIndirectExecutionSetEXT");
    pOut->vkDestroyIndirectExecutionSetEXT = deviceProcAddr(device, "vkDestroyIndirectExecutionSetEXT");
    pOut->vkUpdateIndirectExecutionSetPipelineEXT = deviceProcAddr(device, "vkUpdateIndirectExecutionSetPipelineEXT");
    pOut->vkUpdateIndirectExecutionSetShaderEXT = deviceProcAddr(device, "vkUpdateIndirectExecutionSetShaderEXT");
#endif //VK_EXT_device_generated_commands
#if defined(VK_KHR_device_fault) 
    pOut->vkGetDeviceFaultReportsKHR = deviceProcAddr(device, "vkGetDeviceFaultReportsKHR");
    pOut->vkGetDeviceFaultDebugInfoKHR = deviceProcAddr(device, "vkGetDeviceFaultDebugInfoKHR");
#endif //VK_KHR_device_fault
#if defined(VK_EXT_external_memory_metal) 
    pOut->vkGetMemoryMetalHandleEXT = deviceProcAddr(device, "vkGetMemoryMetalHandleEXT");
    pOut->vkGetMemoryMetalHandlePropertiesEXT = deviceProcAddr(device, "vkGetMemoryMetalHandlePropertiesEXT");
#endif //VK_EXT_external_memory_metal
#if defined(VK_ARM_shader_instrumentation) 
    pOut->vkCreateShaderInstrumentationARM = deviceProcAddr(device, "vkCreateShaderInstrumentationARM");
    pOut->vkDestroyShaderInstrumentationARM = deviceProcAddr(device, "vkDestroyShaderInstrumentationARM");
    pOut->vkCmdBeginShaderInstrumentationARM = deviceProcAddr(device, "vkCmdBeginShaderInstrumentationARM");
    pOut->vkCmdEndShaderInstrumentationARM = deviceProcAddr(device, "vkCmdEndShaderInstrumentationARM");
    pOut->vkGetShaderInstrumentationValuesARM = deviceProcAddr(device, "vkGetShaderInstrumentationValuesARM");
    pOut->vkClearShaderInstrumentationMetricsARM = deviceProcAddr(device, "vkClearShaderInstrumentationMetricsARM");
#endif //VK_ARM_shader_instrumentation
#if defined(VK_EXT_fragment_density_map_offset) 
    pOut->vkCmdEndRendering2EXT = deviceProcAddr(device, "vkCmdEndRendering2EXT");
#endif //VK_EXT_fragment_density_map_offset
#if defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)) 
    pOut->vkCmdBeginCustomResolveEXT = deviceProcAddr(device, "vkCmdBeginCustomResolveEXT");
#endif //VK_EXT_custom_resolve+(VK_KHR_dynamic_rendering,VK_VERSION_1_3)
#if defined(VK_KHR_maintenance10) 
    pOut->vkCmdEndRendering2KHR = deviceProcAddr(device, "vkCmdEndRendering2KHR");
#endif //VK_KHR_maintenance10
#if defined(VK_NV_compute_occupancy_priority) 
    pOut->vkCmdSetComputeOccupancyPriorityNV = deviceProcAddr(device, "vkCmdSetComputeOccupancyPriorityNV");
#endif //VK_NV_compute_occupancy_priority
#if defined(VK_EXT_primitive_restart_index) 
    pOut->vkCmdSetPrimitiveRestartIndexEXT = deviceProcAddr(device, "vkCmdSetPrimitiveRestartIndexEXT");
#endif //VK_EXT_primitive_restart_index

}