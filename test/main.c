#define DOK_VULKAN_TABLES_IMPLEMENTATION
#include <dok/dok_vulkan_tables.h>
#include <dok/dok_vulkan_loader.h>
#include <dok/dok_window.h>
#include <stdlib.h>
#include <stdio.h>

alignas(uint32_t) const uint8_t spirvShader[] = {
    #embed "demo.spv"
};

VkBool32 dokGetDeviceQueueFamily(
    DokInstanceTable*   instanceTable,
    VkPhysicalDevice    physicalDevice,
    uint32_t            requirements,
    uint32_t*           pOut) {

    uint32_t queueFamilyCount = 0;
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*) malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    instanceTable->vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++) {
        if (dokGetPhysicalDevicePresentationSupport(instanceTable, physicalDevice, i) == VK_FALSE) {
            continue;
        }

        if (queueFamilies[i].queueFlags & requirements) {
            *pOut = i;
            return VK_TRUE;
        }
    }

    return VK_FALSE;
}

void transitionImageLayout(
    DokDeviceTable* deviceTable,
    VkCommandBuffer commandBuffer,
    VkImage image
) {
    VkImageSubresourceRange subresourceRange = { 0 };
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    VkImageMemoryBarrier2 barrier = { 0 };
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.srcStageMask = VK_PIPELINE_STAGE_2_NONE;
    barrier.srcAccessMask = VK_ACCESS_2_NONE;
    barrier.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
    barrier.dstAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
    barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.image = image;
    barrier.subresourceRange = subresourceRange;

    VkDependencyInfo dependencyInfo = { 0 };
    dependencyInfo.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependencyInfo.imageMemoryBarrierCount = 1;
    dependencyInfo.pImageMemoryBarriers = &barrier;

    deviceTable->vkCmdPipelineBarrier2(commandBuffer, &dependencyInfo);
}

void recordCommandBuffer(
    DokDeviceTable* deviceTable,
    VkCommandBuffer commandBuffer,
    VkImage image,
    VkImageView imageView,
    uint32_t windowWidth,
    uint32_t windowHeight
) {
    VkCommandBufferBeginInfo beginInfo = { 0 };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    deviceTable->vkBeginCommandBuffer(commandBuffer, &beginInfo);
    transitionImageLayout(deviceTable, commandBuffer, image);

    VkClearColorValue clearColor = { 0 };
    clearColor.float32[3] = 1;

    VkClearValue clearValue = { 0 };
    clearValue.color = clearColor;

    VkRenderingAttachmentInfo attachmentInfo = { 0 };
    attachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    attachmentInfo.imageView = imageView;
    attachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentInfo.clearValue = clearValue;

    VkExtent2D renderExtent = { 0 };
    renderExtent.width = windowWidth;
    renderExtent.height = windowHeight;

    VkRect2D renderArea = { 0 };
    renderArea.extent = renderExtent;

    VkRenderingInfo renderingInfo = { 0 };
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea = renderArea;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &attachmentInfo;

    deviceTable->vkCmdBeginRendering(commandBuffer, &renderingInfo);

    deviceTable->vkCmdEndRendering(commandBuffer);
    transitionImageLayout(deviceTable, commandBuffer, image);

    deviceTable->vkEndCommandBuffer(commandBuffer);
}

int main() {
    DokVkLoader loader;
    dokLoadVulkan(&loader);

    PFN_vkGetInstanceProcAddr instanceProcAddr = (PFN_vkGetInstanceProcAddr) dokGetFunction(loader, "vkGetInstanceProcAddr");

    DokGlobalTable globalTable = { 0 };
    dokLoadGlobalTable(instanceProcAddr, &globalTable);

    VkApplicationInfo appInfo = { 0 };
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Dok";
    appInfo.apiVersion = VK_API_VERSION_1_3;

    const char* instanceExtensions[] = { VK_KHR_SURFACE_EXTENSION_NAME, dokGetPlatformSurfaceExtension() };

    VkInstanceCreateInfo instanceInfo = { 0 };
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = 2;
    instanceInfo.ppEnabledExtensionNames = instanceExtensions;

    VkInstance instance;
    globalTable.vkCreateInstance(&instanceInfo, NULL, &instance);

    DokInstanceTable instanceTable = { 0 };
    dokLoadInstanceTable(instanceProcAddr, instance, &instanceTable);

    DokWindow window;
    dokCreateWindow("PEPE", 930, 200, &window);

    VkSurfaceKHR surface;
    dokCreateWindowSurface(&instanceTable, window, &surface);

    uint32_t windowWidth;
    uint32_t windowHeight;
    dokGetWindowSize(window, &windowWidth, &windowHeight);

    uint32_t deviceCount = 0;
    instanceTable.vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*) malloc(deviceCount * sizeof(VkPhysicalDevice));
    instanceTable.vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices);

    VkPhysicalDevice physicalDevice = physicalDevices[0];
    free(physicalDevices);

    uint32_t queueFamily = 0;
    dokGetDeviceQueueFamily(&instanceTable, physicalDevice, VK_QUEUE_GRAPHICS_BIT, &queueFamily);

    float priority = 1.0;

    VkDeviceQueueCreateInfo queueInfo = { 0 };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = queueFamily;
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

    VkDevice device;
    instanceTable.vkCreateDevice(physicalDevice, &deviceInfo, NULL, &device);

    DokDeviceTable deviceTable = { 0 };
    dokLoadDeviceTable(instanceTable.vkGetDeviceProcAddr, device, &deviceTable);

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    instanceTable.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

    VkFormat imageFormat = VK_FORMAT_B8G8R8A8_SRGB;

    VkSwapchainCreateInfoKHR swapchainInfo = { 0 };
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = surface;
    swapchainInfo.minImageCount = surfaceCapabilities.minImageCount,
    swapchainInfo.imageFormat = imageFormat;
    swapchainInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchainInfo.imageExtent = surfaceCapabilities.currentExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

    VkSwapchainKHR swapchain;
    deviceTable.vkCreateSwapchainKHR(device, &swapchainInfo, NULL, &swapchain);

    uint32_t imageCount;
    deviceTable.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, NULL);

    VkImage* images = (VkImage*) malloc(imageCount * sizeof(VkImage));
    deviceTable.vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images);

    VkFormat formats[] = { VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    VkFormat depthFormat = VK_FORMAT_UNDEFINED;

    for (uint32_t i = 0; i < 2; i++) {
        VkFormatProperties2 formatProperties = { 0 };
        formatProperties.sType = VK_STRUCTURE_TYPE_FORMAT_PROPERTIES_2;
        instanceTable.vkGetPhysicalDeviceFormatProperties2(physicalDevice, formats[i], &formatProperties);

        if (formatProperties.formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            depthFormat = formats[i];
            break;
        }
    }

    VkShaderModuleCreateInfo shaderInfo = { 0 };
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = sizeof(spirvShader) / spirvShader[0] * sizeof(uint32_t);
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
    vertexInfo.pName = "fragMain";

    VkPipelineShaderStageCreateInfo stagesInfo[] = { vertexInfo, fragInfo };

    VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

    VkPipelineDynamicStateCreateInfo dynamicState = { 0 };
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = sizeof(dynamicStates) / sizeof(dynamicStates[0]);
    dynamicState.pDynamicStates = dynamicStates;

    VkPipelineVertexInputStateCreateInfo vertexInputState = { 0 };
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = { 0 };
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkViewport viewport = { 0 };
    viewport.height = (float) windowHeight;
    viewport.width = (float) windowWidth;
    viewport.maxDepth = 1.0f;

    VkExtent2D scissorExtent = { 0 };
    scissorExtent.height = windowHeight;
    scissorExtent.width = windowWidth;

    VkRect2D scissor = { 0 };
    scissor.extent = scissorExtent;

    VkPipelineViewportStateCreateInfo viewportState = { 0 };
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizationState = { 0 };
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo multisampleState = { 0 };
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState blendAttachment = { 0 };
    blendAttachment.colorWriteMask = 0xF;

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

    VkPipelineDepthStencilStateCreateInfo depthStencilState = { 0 };
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable = VK_TRUE;
    depthStencilState.depthWriteEnable = VK_TRUE;
    depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;

    VkGraphicsPipelineCreateInfo pipelineInfo = { 0 };
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = &renderingInfo;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stagesInfo;
    pipelineInfo.pVertexInputState = &vertexInputState;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizationState;
    pipelineInfo.pMultisampleState = &multisampleState;
    pipelineInfo.pColorBlendState = &colorBlendState;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.pDepthStencilState = &depthStencilState;
    pipelineInfo.layout = pipelineLayout;

    VkPipeline pipeline;
    deviceTable.vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pipeline);

    VkCommandPoolCreateInfo commandPoolInfo = { 0 };
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = queueFamily;

    VkCommandPool commandPool;
    deviceTable.vkCreateCommandPool(device, &commandPoolInfo, NULL, &commandPool);

    VkCommandBuffer commandBuffer;

    VkCommandBufferAllocateInfo allocateInfo = { 0 };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = commandPool;
    allocateInfo.commandBufferCount = 1;

    deviceTable.vkAllocateCommandBuffers(device, &allocateInfo, &commandBuffer);


    VkImageSubresourceRange subresourceRange = { 0 };
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    for (uint32_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo viewInfo = { 0 };
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = images[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = depthFormat;
        viewInfo.subresourceRange = subresourceRange;

        VkImageView imageView;
        deviceTable.vkCreateImageView(device, &viewInfo, NULL, &imageView);

        VkImageMemoryBarrier2 barrier = { 0 };
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
        barrier.srcStageMask = VK_PIPELINE_STAGE_2_NONE;
        barrier.srcAccessMask = VK_ACCESS_2_NONE;
        barrier.dstStageMask = VK_PIPELINE_STAGE_2_TRANSFER_BIT;
        barrier.dstAccessMask = VK_ACCESS_2_TRANSFER_WRITE_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    }



    // uint32_t windowWidth;
    // uint32_t windowHeight;

    // dokGetWindowSize(window, &windowWidth, &windowHeight);
    // VkExtent3D imageExtent = { 0 };
    // imageExtent.width = windowWidth;
    // imageExtent.height = windowHeight;
    // imageExtent.depth = 1;

    // VkImageCreateInfo imageInfo = { 0 };
    // imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    // imageInfo.imageType = VK_IMAGE_TYPE_2D;
    // imageInfo.format = depthFormat;
    // imageInfo.extent = imageExtent;
    // imageInfo.mipLevels = 1;
    // imageInfo.arrayLayers = 1;
    // imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    // imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    // imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    // imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    //

    dokUnloadVulkan(loader);
    dokFreeWindow(window);
    return 0;
}