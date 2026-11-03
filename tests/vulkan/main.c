#define DOK_VULKAN_TABLES_IMPLEMENTATION
#include <dok/dok_vulkan_tables.h>
#include <dok/dok_core.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

alignas(uint32_t) const uint8_t spirvShader[] = {
    #embed "demo.spv"
};

typedef struct TestInstance_T* TestInstance;
typedef struct TestWindow_T* TestWindow;

typedef struct TestDevice {
    VkPhysicalDevice physicalDevice;
    uint32_t queueFamilyIndex;
    VkDevice device;
    VkQueue queue;
    DokDeviceTable table;
} TestDevice;

struct TestInstance_T {
    DokInstanceTable table;
    DokVkLoader loader;
    VkInstance instance;
};

struct TestWindow_T {
    VkSurfaceKHR surface;
    DokWindow window;
};


void observerDestroy(DokWindow window, void* pUserData) {
    *((bool*)pUserData) = false;
}

VkBool32 testGetDeviceQueueFamily(
    DokInstanceTable*   instanceTable,
    VkPhysicalDevice    physicalDevice,
    uint32_t*           pOut) {

    uint32_t queueFamilyCount = 0;
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*) malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++) {
        if (dokGetPhysicalDevicePresentationSupport(instanceTable->vkGetInstanceProcAddr, instanceTable->instance, physicalDevice, i) == VK_FALSE) {
            continue;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            *pOut = i;
            return VK_TRUE;
        }
    }

    return VK_FALSE;
}

VkBool32 testChoosePhysicalDevice(DokInstanceTable* instanceTable, VkPhysicalDevice* pOutPhysicalDevice, uint32_t* pOutQueueFamilyIndex) {
    uint32_t physicalDeviceCount;
    instanceTable->vkEnumeratePhysicalDevices(instanceTable->instance, &physicalDeviceCount, NULL);

    VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*) malloc(physicalDeviceCount * sizeof(VkPhysicalDevice));
    instanceTable->vkEnumeratePhysicalDevices(instanceTable->instance, &physicalDeviceCount, physicalDevices);

    for (uint32_t i = 0; i < physicalDeviceCount; i++) {
        VkPhysicalDevice physicalDevice = physicalDevices[i];
        if (testGetDeviceQueueFamily(instanceTable, physicalDevice, pOutQueueFamilyIndex)) {
            *pOutPhysicalDevice = physicalDevice;
            free(physicalDevices);
            return VK_TRUE;
        }
    }

    free(physicalDevices);
    return VK_FALSE;
}

VkResult testCreateVkDevice(DokInstanceTable* instanceTable, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkDevice* pOut) {
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

VkResult testCreateVkPipeline(DokDeviceTable* deviceTable, VkPipeline* pOut) {
    VkFormat imageFormat = VK_FORMAT_B8G8R8A8_SRGB;

    VkPipelineRenderingCreateInfo renderingInfo = { 0 };
    renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachmentFormats = &imageFormat;

    VkShaderModuleCreateInfo shaderInfo = { 0 };
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = sizeof(spirvShader);
    shaderInfo.pCode = (uint32_t*)spirvShader;

    VkShaderModule shaderModule;
    deviceTable->vkCreateShaderModule(deviceTable->device, &shaderInfo, NULL, &shaderModule);

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
    deviceTable->vkCreatePipelineLayout(deviceTable->device, &pipelineLayoutInfo, NULL, &pipelineLayout);

    VkGraphicsPipelineCreateInfo pipelineInfo = { 0 };
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = &renderingInfo;
    pipelineInfo.stageCount = sizeof(shaderStages) / sizeof(shaderStages[0]);
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizationState;
    pipelineInfo.pMultisampleState = &multisampleState;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;

    return deviceTable->vkCreateGraphicsPipelines(deviceTable->device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, pOut);
}

void testTransitionImageLayout(
    DokDeviceTable* deviceTable, VkCommandBuffer commandBuffer, VkImage image,
    VkImageLayout oldLayout,
    VkImageLayout newLayout,
    VkAccessFlags2 srcAccessMask,
    VkAccessFlags2 dstAccessMask,
    VkPipelineStageFlags2 srcStageMask,
    VkPipelineStageFlags2 dstStageMask) {

    VkImageSubresourceRange subresourceRange = { 0 };
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier2 barrier = { 0 };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.srcStageMask = srcStageMask;
    barrier.srcAccessMask = srcAccessMask;
    barrier.dstStageMask = dstStageMask;
    barrier.dstAccessMask = dstAccessMask;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.image = image;
    barrier.subresourceRange = subresourceRange;

    VkDependencyInfo dependencyInfo = { 0 };
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.imageMemoryBarrierCount = 1;
    dependencyInfo.pImageMemoryBarriers = &barrier;

    deviceTable->vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
}

void testCreateInstance(TestInstance* pOut) {
    TestInstance pInstance = (TestInstance) malloc(sizeof(struct TestInstance_T));
    dokLoadVulkan(&pInstance->loader);

    PFN_vkGetInstanceProcAddr instanceProcAddr = (PFN_vkGetInstanceProcAddr) 
        dokGetFunction(pInstance->loader, "vkGetInstanceProcAddr");

    DokGlobalTable globalTable;
    dokLoadGlobalTable(instanceProcAddr, &globalTable);

    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Dok";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    const char* instanceExtensions[] = { 
        VK_KHR_SURFACE_EXTENSION_NAME, 
        dokGetPlatformSurfaceExtension() 
    };

    VkInstanceCreateInfo instanceInfo = { 0 };
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.ppEnabledExtensionNames = instanceExtensions;
    instanceInfo.enabledExtensionCount = sizeof(instanceExtensions) / sizeof(instanceExtensions[0]);

    globalTable.vkCreateInstance(&instanceInfo, NULL, &pInstance->instance);
    dokLoadInstanceTable(globalTable.vkGetInstanceProcAddr, pInstance->instance, &pInstance->table);

    *pOut = pInstance;
}

void testCreateDevice(TestInstance testInstance, TestDevice* pOut) {
    testChoosePhysicalDevice(&testInstance->table, &pOut->physicalDevice, &pOut->queueFamilyIndex);
    testCreateVkDevice(&testInstance->table, pOut->physicalDevice, pOut->queueFamilyIndex, &pOut->device);
    dokLoadDeviceTable(testInstance->table.vkGetDeviceProcAddr, pOut->device, &pOut->table);
    pOut->table.vkGetDeviceQueue(pOut->device, pOut->queueFamilyIndex, 0, &pOut->queue);
}

void testCreateWindow(TestInstance testInstance, TestWindow* pOut) {
    TestWindow pWindow = (TestWindow) malloc(sizeof(struct TestWindow_T));
    DokWindowCreateInfo windowInfo = { 0 };
    windowInfo.pTitle = "PEPE";
    windowInfo.height = 420;
    windowInfo.width = 720;

    dokCreateWindow(&windowInfo, &pWindow->window);
    dokCreateWindowSurface(testInstance->table.vkGetInstanceProcAddr, testInstance->instance, pWindow->window, &pWindow->surface);
    *pOut = pWindow;
}

int main() {
    TestInstance testInstance;
    testCreateInstance(&testInstance);

    DokInstanceTable instanceTable = testInstance->table;
    // VkInstance instance = testInstance->instance;

    TestDevice device;
    testCreateDevice(testInstance, &device);

    TestWindow testWindow;
    testCreateWindow(testInstance, &testWindow);

    VkSurfaceKHR surface = testWindow->surface;
    DokWindow window = testWindow->window;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    instanceTable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.physicalDevice, surface, &surfaceCapabilities);

    VkExtent2D swapchainExtent = surfaceCapabilities.currentExtent;
    if (swapchainExtent.width == 0xFFFFFFFF) {
        dokGetWindowExtent(window, &swapchainExtent);
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
    device.table.vkCreateSwapchainKHR(device.device, &swapchainInfo, NULL, &swapchain);

    uint32_t imageCount;
    device.table.vkGetSwapchainImagesKHR(device.device, swapchain, &imageCount, NULL);

    VkImage* images = (VkImage*) malloc(imageCount * sizeof(VkImage));
    device.table.vkGetSwapchainImagesKHR(device.device, swapchain, &imageCount, images);

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
        viewInfo.format = imageFormat;
        viewInfo.subresourceRange = subresourceRange;

        device.table.vkCreateImageView(device.device, &viewInfo, NULL, &imageViews[i]);
    }

    VkPipeline pipeline;
    testCreateVkPipeline(&device.table, &pipeline);

    VkCommandPoolCreateInfo commandPoolInfo = { 0 };
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = device.queueFamilyIndex;

    VkCommandPool commandPool;
    device.table.vkCreateCommandPool(device.device, &commandPoolInfo, NULL, &commandPool);

    VkCommandBufferAllocateInfo allocateInfo = { 0 };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = commandPool;
    allocateInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    device.table.vkAllocateCommandBuffers(device.device, &allocateInfo, &commandBuffer);

    VkSemaphoreCreateInfo semaphoreInfo = { 0 };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = { 0 };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    VkSemaphore presentCompleteSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence fence;

    device.table.vkCreateSemaphore(device.device, &semaphoreInfo, NULL, &presentCompleteSemaphore);
    device.table.vkCreateSemaphore(device.device, &semaphoreInfo, NULL, &renderFinishedSemaphore);
    device.table.vkCreateFence(device.device, &fenceInfo, NULL, &fence);

    bool windowExists = true;

    DokWindowDestroyObserverCreateInfo destroyObserverInfo = { 0 };
    destroyObserverInfo.pUserData = &windowExists;
    destroyObserverInfo.pfnCallback = observerDestroy;

    DokWindowDestroyObserver destroyObserver;
    dokCreateDestroyObserver(window, &destroyObserverInfo, &destroyObserver);

    while (windowExists) {
        dokPollWindow(window);

        device.table.vkWaitForFences(device.device, 1, &fence, VK_TRUE, UINT64_MAX);
        device.table.vkResetFences(device.device, 1, &fence);

        uint32_t imageIndex;
        device.table.vkAcquireNextImageKHR(device.device, swapchain, UINT64_MAX, presentCompleteSemaphore, VK_NULL_HANDLE, &imageIndex);

        VkCommandBufferBeginInfo beginInfo = { 0 };
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        device.table.vkBeginCommandBuffer(commandBuffer, &beginInfo);

        testTransitionImageLayout(
            &device.table, commandBuffer, images[imageIndex],
            VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_ACCESS_2_NONE,
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT
        );

        VkClearColorValue clearColor = { .float32 = {0.0f, 0.0f, 0.0f, 1.0f} };
        VkClearValue clearValue = { 0 };
        clearValue.color = clearColor;

        VkRenderingAttachmentInfo attachmentInfo = { 0 };
        attachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        attachmentInfo.imageView = imageViews[imageIndex];
        attachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentInfo.clearValue = clearValue;

        VkExtent2D windowExtent;
        dokGetWindowExtent(window, &windowExtent);

        VkRect2D renderArea = { 0 };
        renderArea.extent = windowExtent;

        VkRenderingInfo renderingInfo = { 0 };
        renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
        renderingInfo.renderArea = renderArea;
        renderingInfo.layerCount = 1;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachments = &attachmentInfo;

        device.table.vkCmdBeginRendering(commandBuffer, &renderingInfo);

        VkViewport viewport = { 0 };
        viewport.width = windowExtent.width;
        viewport.height = windowExtent.height;
        viewport.maxDepth = 1.0f;

        VkRect2D scissors = { 0 };
        scissors.extent = windowExtent;

        device.table.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
        device.table.vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        device.table.vkCmdSetScissor(commandBuffer, 0, 1, &scissors);

        device.table.vkCmdDraw(commandBuffer, 3, 1, 0, 0);
        device.table.vkCmdEndRendering(commandBuffer);

        testTransitionImageLayout(
            &device.table, commandBuffer, images[imageIndex],
            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            VK_ACCESS_2_NONE,
            VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
            VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT
        );

        device.table.vkEndCommandBuffer(commandBuffer);
        device.table.vkQueueWaitIdle(device.queue);

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

        device.table.vkQueueSubmit2(device.queue, 1, &submitInfo, fence);

        VkPresentInfoKHR presentInfo = { 0 };
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &renderFinishedSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &imageIndex;

        device.table.vkQueuePresentKHR(device.queue, &presentInfo);
    }

    // dokUnloadVulkan(loader);
    dokFreeWindow(window);
    return 0;
}