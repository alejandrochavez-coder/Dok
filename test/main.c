#define DOK_VULKAN_TABLES_IMPLEMENTATION
#include <dok/dok_vulkan_tables.h>
#include <dok/dok_vulkan_loader.h>
#include <dok/dok_window.h>
#include <stdlib.h>
#include <stdio.h>

alignas(uint32_t) const uint8_t spirvShader[] = {
    #embed "demo.spv"
};

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData
) {
    printf("[VULKAN VALIDATION] %s\n", callbackData->pMessage);

    return VK_FALSE;
}

VkResult dokCreateVkInstance(DokGlobalTable* globalTable, VkInstance* pOut) {
    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Dok";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    const char* instanceExtensions[] = { VK_KHR_SURFACE_EXTENSION_NAME, dokGetPlatformSurfaceExtension(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };

    VkDebugUtilsMessengerCreateInfoEXT debugInfo = { 0 };
    debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugInfo.messageSeverity = 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    debugInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    debugInfo.pfnUserCallback = debugCallback;

    VkInstanceCreateInfo instanceInfo = { 0 };
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = &debugInfo;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.ppEnabledLayerNames = validationLayers;
    instanceInfo.enabledLayerCount = sizeof(validationLayers) / sizeof(validationLayers[0]);
    instanceInfo.ppEnabledExtensionNames = instanceExtensions;
    instanceInfo.enabledExtensionCount = sizeof(instanceExtensions) / sizeof(instanceExtensions[0]);

    VkInstance instance;
    return globalTable->vkCreateInstance(&instanceInfo, NULL, pOut);
}

VkBool32 dokGetDeviceQueueFamily(
    DokInstanceTable*   instanceTable,
    VkPhysicalDevice    physicalDevice,
    uint32_t*           pOut) {

    uint32_t queueFamilyCount = 0;
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*) malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++) {
        if (dokGetPhysicalDevicePresentationSupport(instanceTable, physicalDevice, i) == VK_FALSE) {
            continue;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            *pOut = i;
            return VK_TRUE;
        }
    }

    return VK_FALSE;
}

VkBool32 dokChoosePhysicalDevice(DokInstanceTable* instanceTable, VkPhysicalDevice* pOutPhysicalDevice, uint32_t* pOutQueueFamilyIndex) {
    uint32_t physicalDeviceCount;
    instanceTable->vkEnumeratePhysicalDevices(instanceTable->instance, &physicalDeviceCount, NULL);

    VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*) malloc(physicalDeviceCount * sizeof(VkPhysicalDevice));
    instanceTable->vkEnumeratePhysicalDevices(instanceTable->instance, &physicalDeviceCount, physicalDevices);

    for (uint32_t i = 0; i < physicalDeviceCount; i++) {
        VkPhysicalDevice physicalDevice = physicalDevices[i];
        if (dokGetDeviceQueueFamily(instanceTable, physicalDevice, pOutQueueFamilyIndex)) {
            *pOutPhysicalDevice = physicalDevice;
            free(physicalDevices);
            return VK_TRUE;
        }
    }

    free(physicalDevices);
    return VK_FALSE;
}

VkResult dokCreateVkDevice(DokInstanceTable* instanceTable, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkDevice* pOut) {
    float priority = 1.0;

    VkDeviceQueueCreateInfo queueInfo = { 0 };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;

    const char* deviceExtensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkPhysicalDeviceVulkan12Features enabledVk12Features = { 0 };
    enabledVk12Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    enabledVk12Features.descriptorIndexing = VK_TRUE;
    enabledVk12Features.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
    enabledVk12Features.descriptorBindingVariableDescriptorCount = VK_TRUE;
    enabledVk12Features.runtimeDescriptorArray = VK_TRUE;
    enabledVk12Features.bufferDeviceAddress = VK_TRUE;

    VkPhysicalDeviceVulkan13Features enabledVk13Features = { 0 };
    enabledVk13Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    enabledVk13Features.pNext = &enabledVk12Features;
    enabledVk13Features.synchronization2 = VK_TRUE;
    enabledVk13Features.dynamicRendering = VK_TRUE;

    VkPhysicalDeviceFeatures enabledVk10Features = { 0 };
    enabledVk10Features.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo deviceInfo = { 0 };
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.pNext = &enabledVk13Features;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount = 1;
    deviceInfo.ppEnabledExtensionNames = deviceExtensions;
    deviceInfo.pEnabledFeatures = &enabledVk10Features;

    return instanceTable->vkCreateDevice(physicalDevice, &deviceInfo, NULL, pOut);
}

int main() {
    DokVkLoader loader;
    dokLoadVulkan(&loader);

    PFN_vkGetInstanceProcAddr instanceProcAddr = (PFN_vkGetInstanceProcAddr) dokGetFunction(loader, "vkGetInstanceProcAddr");

    DokGlobalTable globalTable = { 0 };
    dokLoadGlobalTable(instanceProcAddr, &globalTable);

    VkInstance instance;
    dokCreateVkInstance(&globalTable, &instance);

    DokInstanceTable instanceTable = { 0 };
    dokLoadInstanceTable(instanceProcAddr, instance, &instanceTable);

    VkPhysicalDevice physicalDevice;
    uint32_t queueFamilyIndex;
    dokChoosePhysicalDevice(&instanceTable, &physicalDevice, &queueFamilyIndex);

    VkDevice device;
    dokCreateVkDevice(&instanceTable, physicalDevice, queueFamilyIndex, &device);
    
    DokDeviceTable deviceTable = { 0 };
    dokLoadDeviceTable(instanceTable.vkGetDeviceProcAddr, device, &deviceTable);

    VkQueue queue;
    deviceTable.vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

    DokWindow window;
    dokCreateWindow("PEPE", 930, 200, &window);

    VkSurfaceKHR surface;
    dokCreateWindowSurface(&instanceTable, window, &surface);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    instanceTable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

    VkExtent2D swapchainExtent = surfaceCapabilities.currentExtent;
    if (swapchainExtent.width == 0xFFFFFFFF) {
        uint32_t windowWidth;
        uint32_t windowHeight;
        dokGetWindowSize(window, &windowWidth, &windowHeight);

        swapchainExtent.width = windowWidth;
        swapchainExtent.height = windowHeight;
    }

    VkFormat imageFormat = VK_FORMAT_B8G8R8A8_SRGB;

    VkSwapchainCreateInfoKHR swapchainInfo = { 0 };
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = surface;
    swapchainInfo.minImageCount = surfaceCapabilities.minImageCount,
    swapchainInfo.imageFormat = imageFormat;
    swapchainInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchainInfo.imageExtent = swapchainExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;;

    VkSwapchainKHR swapchain;
    deviceTable.vkCreateSwapchainKHR(device, &swapchainInfo, NULL, &swapchain);

    uint32_t imageCount;
    deviceTable.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL);

    VkImage* images = (VkImage*) malloc(imageCount * sizeof(VkImage));
    deviceTable.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images);

    VkImageView* imageViews = (VkImageView*) malloc(imageCount * sizeof(VkImageView));
        
    VkImageSubresourceRange subresourceRange = { 0 };
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    for (uint32_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo viewInfo = { 0 };
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
        viewInfo.subresourceRange = subresourceRange;

        deviceTable.vkCreateImageView(device, &viewInfo, NULL, &imageViews[i]);
    }

    VkShaderModuleCreateInfo shaderInfo = { 0 };
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = sizeof(spirvShader);
    shaderInfo.pCode = (uint32_t*)spirvShader;

    VkShaderModule shaderModule;
    deviceTable.vkCreateShaderModule(device, &shaderInfo, NULL, &shaderModule);

    VkPipelineShaderStageCreateInfo vertexInfo = { 0 };
    vertexInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexInfo.module = shaderModule;
    vertexInfo.pName = "vertMain";

    VkPipelineShaderStageCreateInfo fragInfo = { 0 };
    fragInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragInfo.module = shaderModule;
    fragInfo.pName = "fragMain";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexInfo, fragInfo };

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamicState = { 0 };
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
    dynamicState.pDynamicStates = dynamicStates;

    VkPipelineVertexInputStateCreateInfo vertexInputState = { 0 };
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = { 0 };
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo viewportState = { 0 };
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizationState = { 0 };
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleState = { 0 };
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState blendAttachment = { 0 };
    blendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo colorBlendState = { 0 };
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &blendAttachment;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = { 0 };
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    
    VkPipelineLayout pipelineLayout;
    deviceTable.vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout);

    VkPipelineRenderingCreateInfo renderingInfo = { 0 };
    renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachmentFormats = &imageFormat;

    VkSemaphoreCreateInfo semaphoreInfo = { 0 };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = { 0 };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkFence fence;
    VkSemaphore presentCompleteSemaphore;
    VkSemaphore renderFinishedSemaphore;

    deviceTable.vkCreateSemaphore(device, &semaphoreInfo, NULL, &presentCompleteSemaphore);
    deviceTable.vkCreateSemaphore(device, &semaphoreInfo, NULL, &renderFinishedSemaphore);
    deviceTable.vkCreateFence(device, &fenceInfo, NULL, &fence);

    VkCommandPoolCreateInfo commandPoolInfo = { 0 };
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = queueFamilyIndex;

    VkCommandPool commandPool;
    deviceTable.vkCreateCommandPool(device, &commandPoolInfo, NULL, &commandPool);

    VkCommandBuffer commandBuffer;

    VkCommandBufferAllocateInfo allocateInfo = { 0 };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = commandPool;
    allocateInfo.commandBufferCount = 1;

    deviceTable.vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer);

    VkGraphicsPipelineCreateInfo pipelineInfo = { 0 };
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = &renderingInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizationState;
    pipelineInfo.pMultisampleState = &multisampleState;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;

    VkPipeline pipeline;
    deviceTable.vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline);

    while (dokWindowExists(window)) {
        dokPollWindow(window);

        deviceTable.vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
        deviceTable.vkResetFences(device, 1, &fence);

        uint32_t imageIndex;
        deviceTable.vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, presentCompleteSemaphore, VK_NULL_HANDLE, &imageIndex);

        VkCommandBufferBeginInfo beginInfo = { 0 };
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        deviceTable.vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkImageSubresourceRange subresourceRange = { 0 };
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        subresourceRange.levelCount = 1;
        subresourceRange.layerCount = 1;

        VkImageMemoryBarrier2 barrier = { 0 };
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        barrier.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        barrier.srcAccessMask = VK_ACCESS_2_NONE;
        barrier.dstStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        barrier.image = images[imageIndex];
        barrier.subresourceRange = subresourceRange;

        VkDependencyInfo dependencyInfo = { 0 };
        dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        dependencyInfo.imageMemoryBarrierCount = 1;
        dependencyInfo.pImageMemoryBarriers = &barrier;

        deviceTable.vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);

        VkClearColorValue clearColor = { 0 };
        clearColor.float32[2] = 0.2f;
        clearColor.float32[3] = 1.0f;

        VkClearValue clearValue = { 0 };
        clearValue.color = clearColor;

        VkRenderingAttachmentInfo attachmentInfo = { 0 };
        attachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        attachmentInfo.imageView = imageViews[imageIndex];
        attachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentInfo.clearValue = clearValue;

        VkRect2D renderArea = { 0 };
        renderArea.extent = swapchainExtent;

        VkRenderingInfo renderingInfo = { 0 };
        renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        renderingInfo.renderArea = renderArea;
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &attachmentInfo;

        deviceTable.vkCmdBeginRendering(commandBuffer, &renderingInfo);

        VkViewport viewport = { 0 };
        viewport.width = swapchainExtent.width;
        viewport.height = swapchainExtent.height;
        viewport.maxDepth = 1.0f;

        VkRect2D scissors = { 0 };
        scissors.extent = swapchainExtent;

        deviceTable.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        deviceTable.vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        deviceTable.vkCmdSetScissor(commandBuffer, 0, 1, &scissors);

        deviceTable.vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        deviceTable.vkCmdEndRendering(commandBuffer);

        VkImageMemoryBarrier2 barrierPresent = { 0 };
        barrierPresent.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        barrierPresent.srcStageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;
        barrierPresent.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrierPresent.dstStageMask = VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT;
        barrierPresent.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        barrierPresent.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        barrierPresent.image = images[imageIndex];
        barrierPresent.subresourceRange = subresourceRange;

        VkDependencyInfo barrierPresentDependencyInfo = { 0 };
        barrierPresentDependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
        barrierPresentDependencyInfo.imageMemoryBarrierCount = 1;
        barrierPresentDependencyInfo.pImageMemoryBarriers = &barrierPresent;

        deviceTable.vkCmdPipelineBarrier2(commandBuffer, &barrierPresentDependencyInfo);

        deviceTable.vkEndCommandBuffer(commandBuffer);

        deviceTable.vkQueueWaitIdle(queue);

        VkSemaphoreSubmitInfo waitSemaphoreInfo = { 0 };
        waitSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        waitSemaphoreInfo.semaphore = presentCompleteSemaphore;
        waitSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkCommandBufferSubmitInfo commandBufferSubmitInfo = { 0 };
        commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
        commandBufferSubmitInfo.commandBuffer = commandBuffer;

        VkSemaphoreSubmitInfo signalSemaphoreInfo = { 0 };
        signalSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
        signalSemaphoreInfo.semaphore = renderFinishedSemaphore;
        signalSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

        VkSubmitInfo2 submitInfo = { 0 };
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
        submitInfo.waitSemaphoreInfoCount = 1;
        submitInfo.pWaitSemaphoreInfos = &waitSemaphoreInfo;
        submitInfo.commandBufferInfoCount = 1;
        submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;
        submitInfo.signalSemaphoreInfoCount = 1;
        submitInfo.pSignalSemaphoreInfos = &signalSemaphoreInfo;

        deviceTable.vkQueueSubmit2(queue, 1, &submitInfo, fence);

        VkPresentInfoKHR presentInfo = { 0 };
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &imageIndex;

        deviceTable.vkQueuePresentKHR(queue, &presentInfo);
        // break;
    }

    dokUnloadVulkan(loader);
    dokFreeWindow(window);
    return 0;
}