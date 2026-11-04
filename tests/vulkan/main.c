#define DOK_VULKAN_TABLES_IMPLEMENTATION
#include <dok/dok_vulkan_tables.h>
#include <dok/dok_vector.h>
#include <dok/dok_core.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

alignas(uint32_t) const uint8_t spirvShader[] = {
    #embed "demo.spv"
};

typedef struct TestPhysicalDevice_T* TestPhysicalDevice;
typedef struct TestInstance_T* TestInstance;
typedef struct TestDevice_T* TestDevice;
typedef struct TestContext_T* TestContext;

struct TestInstance_T {
    DokInstanceTable table;

    VkInstance handle;

    DokVkLoader loader;
    DokVector physicalDevices;
};

struct TestPhysicalDevice_T {
    VkPhysicalDevice handle;

    TestInstance instance;
    uint32_t queueFamilyIndex;
};

struct TestDevice_T {
    DokDeviceTable table;

    VkDevice handle;

    TestPhysicalDevice physicalDevice;
    DokWindow window;

    DokVector renderCompleteSemaphores;
    DokVector swapchainImagesViews;
    DokVector swapchainImages;
    VkSwapchainKHR swapchain;

    VkFormat imageFormat;
    VkSurfaceKHR surface;
    VkQueue queue;
};

struct TestContext_T {
    TestDevice device;
    
    DokVector imageAcquiredSemaphores;
    DokVector commandBuffers;
    DokVector fences;

    VkDeviceMemory deviceMemory;
    VkCommandPool commandPool;
    VkPipeline pipeline;
    uint32_t frameIndex;
    uint32_t framesInFlight;
    VkBuffer vertexBuffer;
};

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef struct Vector3 {
    float x;
    float y;
    float z;
} Vector3;

typedef struct Vertex {
    Vector2 position;
    Vector3 color;
} Vertex;

Vertex vertices[] = {
    (Vertex){
        .position = (Vector2){
            .x = 0.0f,
            .y = -0.1f
        },
        .color = (Vector3){
            .x = 1.0f,
            .y = 0.5f,
            .z = 0.0f
        }
    },
    (Vertex){
        .position = (Vector2){
            .x = 0.5f,
            .y = 0.5f
        },
        .color = (Vector3){
            .x = 0.0f,
            .y = 1.0f,
            .z = 0.0f
        }
    },
    (Vertex){
        .position = (Vector2){
            .x = -0.5f,
            .y = 0.5f
        },
        .color = (Vector3){
            .x = 0.0f,
            .y = 0.0f,
            .z = 1.0f
        }
    }
};

void testUpdateSwapchain(TestDevice device, VkPresentModeKHR presentMode) {
    TestPhysicalDevice physicalDevice = device->physicalDevice;
    TestInstance instance = physicalDevice->instance;

    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    instance->table.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->handle, device->surface, &surfaceCapabilities);

    VkExtent2D swapchainExtent = surfaceCapabilities.currentExtent;
    if (swapchainExtent.width == 0xFFFFFFFF) {
        dokGetWindowExtent(device->window, &swapchainExtent);
    }

    VkSwapchainCreateInfoKHR swapchainInfo = { 0 };
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = device->surface;
    swapchainInfo.minImageCount = surfaceCapabilities.minImageCount;
    swapchainInfo.imageFormat = device->imageFormat;
    swapchainInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchainInfo.imageExtent = swapchainExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = presentMode;
    swapchainInfo.clipped = VK_TRUE;
    swapchainInfo.oldSwapchain = device->swapchain;

    device->table.vkCreateSwapchainKHR(device->handle, &swapchainInfo, NULL, &device->swapchain);

    for (uint32_t i = 0; i < device->swapchainImagesViews.length; i++) {
        device->table.vkDestroyImageView(device->handle, dokVectorCast(VkImageView, device->swapchainImagesViews.data)[i], NULL);
    }

    uint32_t imageCount;
    device->table.vkGetSwapchainImagesKHR(device->handle, device->swapchain, &imageCount, NULL);
    dokVectorResize(VkImage, imageCount, device->swapchainImages);
    device->table.vkGetSwapchainImagesKHR(device->handle, device->swapchain, &imageCount, dokVectorCast(VkImage, device->swapchainImages.data));
    dokVectorResize(VkImageView, imageCount, device->swapchainImagesViews);

    for (uint32_t i = 0; i < device->renderCompleteSemaphores.length; i++) {
        device->table.vkDestroySemaphore(device->handle, dokVectorCast(VkSemaphore, device->renderCompleteSemaphores.data)[i], NULL);
    }
    dokVectorResize(VkSemaphore, imageCount, device->renderCompleteSemaphores);

    VkImageSubresourceRange subresourceRange = { 0 };
    subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.levelCount = 1;
    subresourceRange.layerCount = 1;

    VkSemaphoreCreateInfo semaphoreInfo = { 0 };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    for (uint32_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo viewInfo = { 0 };
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = dokVectorCast(VkImage, device->swapchainImages.data)[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = device->imageFormat;
        viewInfo.subresourceRange = subresourceRange;

        device->table.vkCreateImageView(device->handle, &viewInfo, NULL, &dokVectorCast(VkImageView, device->swapchainImagesViews.data)[i]);
        device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &dokVectorCast(VkSemaphore, device->renderCompleteSemaphores.data)[i]);
    }

    if (swapchainInfo.oldSwapchain != VK_NULL_HANDLE) {
        device->table.vkDestroySwapchainKHR(device->handle, swapchainInfo.oldSwapchain, NULL);
    }

}

void onWindowResize(DokWindow window, uint32_t width, uint32_t height, void* pUserData) {
    TestDevice device = *((TestDevice*)pUserData);

    device->table.vkDeviceWaitIdle(device->handle);
    testUpdateSwapchain(device, VK_PRESENT_MODE_FIFO_KHR);
}

void observerDestroy(DokWindow window, void* pUserData) {
    *((bool*)pUserData) = false;
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

    globalTable.vkCreateInstance(&instanceInfo, NULL, &pInstance->handle);
    dokLoadInstanceTable(globalTable.vkGetInstanceProcAddr, pInstance->handle, &pInstance->table);

    dokCreateVector(TestPhysicalDevice, &pInstance->physicalDevices);
    *pOut = pInstance;
}

VkBool32 testGetDeviceQueueFamily(
    TestInstance        instance,
    VkPhysicalDevice    physicalDevice,
    uint32_t*           pOut) {

    uint32_t queueFamilyCount = 0;
    instance->table.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*) malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    instance->table.vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    for (size_t i = 0; i < queueFamilyCount; i++) {
        if (dokGetPhysicalDevicePresentationSupport(instance->table.vkGetInstanceProcAddr, instance->handle, physicalDevice, i) == VK_FALSE) {
            continue;
        }

        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            *pOut = i;
            return VK_TRUE;
        }
    }

    return VK_FALSE;
}

void testEnumerateDevices(TestInstance instance, uint32_t* pOutDeviceCount, TestPhysicalDevice* pOutDevices) {
    uint32_t physicalDeviceCount;
    instance->table.vkEnumeratePhysicalDevices(instance->handle, &physicalDeviceCount, NULL);

    if (instance->physicalDevices.length == physicalDeviceCount) {
        goto end;
    }

    if (!dokVectorEmpty(instance->physicalDevices)) {

        for (uint32_t i = 0; i < physicalDeviceCount; i++) {
            free(dokVectorCast(TestPhysicalDevice, instance->physicalDevices.data)[i]);
        }
        dokVectorClear(instance->physicalDevices);
    }

    VkPhysicalDevice* physicalDevices = (VkPhysicalDevice*) malloc(physicalDeviceCount * sizeof(VkPhysicalDevice));
    instance->table.vkEnumeratePhysicalDevices(instance->handle, &physicalDeviceCount, physicalDevices);

    for (uint32_t i = 0; i < physicalDeviceCount; i++) {
        VkPhysicalDevice physicalDevice = physicalDevices[i];
        uint32_t queueFamilyIndex;

        if (testGetDeviceQueueFamily(instance, physicalDevice, &queueFamilyIndex)) {

            TestPhysicalDevice device = (TestPhysicalDevice) malloc(sizeof(struct TestPhysicalDevice_T));
            device->queueFamilyIndex = queueFamilyIndex;
            device->handle = physicalDevice;
            device->instance = instance;

            dokVectorPush(TestPhysicalDevice, instance->physicalDevices, device);

        }
    }

    free(physicalDevices);

end:
    if (pOutDeviceCount) {
        *pOutDeviceCount = physicalDeviceCount;
    }

    if (pOutDevices) {
        for (uint32_t i = 0; i < physicalDeviceCount; i++) {
            pOutDevices[i] = dokVectorCast(TestPhysicalDevice, instance->physicalDevices.data)[i];
        }
    }

}

void testCreateDevice(TestPhysicalDevice physicalDevice, DokWindow window, VkFormat imageFormat, TestDevice* pOut) {
    TestDevice pDevice = (TestDevice) malloc(sizeof(struct TestDevice_T));
    dokCreateVector(VkSemaphore, &pDevice->renderCompleteSemaphores);
    dokCreateVector(VkImageView, &pDevice->swapchainImagesViews);
    dokCreateVector(VkImage, &pDevice->swapchainImages);
    pDevice->swapchain = VK_NULL_HANDLE;
    pDevice->imageFormat = imageFormat;
    pDevice->physicalDevice = physicalDevice;
    pDevice->window = window;

    TestInstance instance = physicalDevice->instance;
    dokCreateWindowSurface(instance->table.vkGetInstanceProcAddr, instance->handle, window, &pDevice->surface);

    float priority = 1.0;

    VkDeviceQueueCreateInfo queueInfo = { 0 };
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = physicalDevice->queueFamilyIndex;
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

    instance->table.vkCreateDevice(physicalDevice->handle, &deviceInfo, NULL, &pDevice->handle);
    dokLoadDeviceTable(instance->table.vkGetDeviceProcAddr, pDevice->handle, &pDevice->table);

    pDevice->table.vkGetDeviceQueue(pDevice->handle, physicalDevice->queueFamilyIndex, 0, &pDevice->queue);
    testUpdateSwapchain(pDevice, VK_PRESENT_MODE_FIFO_KHR);
    *pOut = pDevice;
}

void testCreateBuffer(
    TestDevice              device, 
    VkBufferUsageFlags      usage,
    VkDeviceSize            size,
    VkMemoryPropertyFlags   properties, 
    VkBuffer*               pOutBuffer, 
    VkDeviceMemory*         pOutMemory) {

    VkBufferCreateInfo bufferInfo = { 0 };
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkBuffer buffer;
    device->table.vkCreateBuffer(device->handle, &bufferInfo, NULL, &buffer);

    VkMemoryRequirements memoryRequirements;
    device->table.vkGetBufferMemoryRequirements(device->handle, buffer, &memoryRequirements);

    VkPhysicalDeviceMemoryProperties memoryProperties;
    device->physicalDevice->instance->table.vkGetPhysicalDeviceMemoryProperties(device->physicalDevice->handle, &memoryProperties);

    VkMemoryAllocateInfo memoryAllocateInfo = { 0 };
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.allocationSize = memoryRequirements.size;

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((memoryRequirements.memoryTypeBits & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties)) {
            memoryAllocateInfo.memoryTypeIndex = i;
            break;
        }
    }

    VkDeviceMemory memory;

    device->table.vkAllocateMemory(device->handle, &memoryAllocateInfo, NULL, &memory);
    device->table.vkBindBufferMemory(device->handle, buffer, memory, 0);

    *pOutBuffer = buffer;
    *pOutMemory = memory;
   
}

void copyBuffer(
    TestContext context,
    VkBuffer srcBuffer, 
    VkBuffer dstBuffer, 
    VkDeviceSize size) {

    VkCommandBufferAllocateInfo allocateInfo = { 0 };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = context->commandPool;
    allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandBufferCount = 1;

    TestDevice device = context->device;

    VkCommandBuffer copyCommandBuffer;
    device->table.vkAllocateCommandBuffers(device->handle, &allocateInfo, &copyCommandBuffer);

    VkCommandBufferBeginInfo beginInfo = { 0 };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VkBufferCopy bufferCopy = { 0 };
    bufferCopy.size = size;

    device->table.vkBeginCommandBuffer(copyCommandBuffer, &beginInfo);
    device->table.vkCmdCopyBuffer(copyCommandBuffer, srcBuffer, dstBuffer, 1, &bufferCopy);

    device->table.vkEndCommandBuffer(copyCommandBuffer);
}

void testCreateContext(TestDevice device, uint32_t framesInFlight, TestContext* pOut) {
    TestContext pContext = (TestContext) malloc(sizeof(struct TestContext_T));
    dokCreateVectorWithSize(VkSemaphore, &pContext->imageAcquiredSemaphores, framesInFlight);
    dokCreateVectorWithSize(VkCommandBuffer, &pContext->commandBuffers, framesInFlight);
    dokCreateVectorWithSize(VkFence, &pContext->fences, framesInFlight);
    pContext->device = device;
    pContext->frameIndex = 0;
    pContext->framesInFlight = framesInFlight;

    VkSemaphoreCreateInfo semaphoreInfo = { 0 };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = { 0 };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < framesInFlight; i++) {
        device->table.vkCreateSemaphore(device->handle, &semaphoreInfo, NULL, &dokVectorCast(VkSemaphore, pContext->imageAcquiredSemaphores.data)[i]);
        device->table.vkCreateFence(device->handle, &fenceInfo, NULL, &dokVectorCast(VkFence, pContext->fences.data)[i]);
    }

    VkCommandPoolCreateInfo commandPoolInfo = { 0 };
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolInfo.queueFamilyIndex = device->physicalDevice->queueFamilyIndex;

    device->table.vkCreateCommandPool(device->handle, &commandPoolInfo, NULL, &pContext->commandPool);

    VkCommandBufferAllocateInfo allocateInfo = { 0 };
    allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.commandPool = pContext->commandPool;
    allocateInfo.commandBufferCount = framesInFlight;

    device->table.vkAllocateCommandBuffers(device->handle, &allocateInfo, dokVectorCast(VkCommandBuffer, pContext->commandBuffers.data));

    VkPipelineRenderingCreateInfo renderingInfo = { 0 };
    renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachmentFormats = &device->imageFormat;

    VkShaderModuleCreateInfo shaderInfo = { 0 };
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = sizeof(spirvShader);
    shaderInfo.pCode = (uint32_t*)spirvShader;

    VkShaderModule shaderModule;
    device->table.vkCreateShaderModule(device->handle, &shaderInfo, NULL, &shaderModule);

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

    VkVertexInputBindingDescription bindingDescriptor = { 0 };
    bindingDescriptor.stride = sizeof(Vertex);
    bindingDescriptor.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    VkVertexInputAttributeDescription firstAttribute = { 0 };
    firstAttribute.format = VK_FORMAT_R32G32_SFLOAT;
    firstAttribute.offset = offsetof(Vertex, position);

    VkVertexInputAttributeDescription secondAttribute = { 0 };
    secondAttribute.location = 1;
    secondAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
    secondAttribute.offset = offsetof(Vertex, color);

    VkVertexInputAttributeDescription attributeDescriptors[] = { firstAttribute, secondAttribute };

    VkPipelineVertexInputStateCreateInfo vertexInputState = { 0 };
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.pVertexBindingDescriptions = &bindingDescriptor;
    vertexInputState.vertexAttributeDescriptionCount = sizeof(attributeDescriptors) / sizeof(attributeDescriptors[0]);
    vertexInputState.pVertexAttributeDescriptions = attributeDescriptors;

    VkDeviceSize bufferSize = sizeof(vertices);

    VkDeviceMemory stagingBufferMemory;
    VkBuffer stagingBuffer;

    testCreateBuffer(
        device,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        bufferSize,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &stagingBuffer,
        &stagingBufferMemory
    );

    void* data;
    device->table.vkMapMemory(device->handle, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices, bufferSize);
    device->table.vkUnmapMemory(device->handle, stagingBufferMemory);

    testCreateBuffer(
        device, 
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
        bufferSize,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &pContext->vertexBuffer,
        &pContext->deviceMemory
    );

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
    device->table.vkCreatePipelineLayout(device->handle, &pipelineLayoutInfo, NULL, &pipelineLayout);

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

    device->table.vkCreateGraphicsPipelines(device->handle, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &pContext->pipeline);
    *pOut = pContext;
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

void testContextProcess(TestContext context) {
    TestDevice device = context->device;
    uint32_t frameIndex = context->frameIndex;

    VkFence fence = dokVectorCast(VkFence, context->fences.data)[frameIndex];
    device->table.vkWaitForFences(device->handle, 1, &fence, VK_TRUE, UINT64_MAX);
    device->table.vkResetFences(device->handle, 1, &fence);

    VkSemaphore imageAcquireSemaphore = dokVectorCast(VkSemaphore, context->imageAcquiredSemaphores.data)[frameIndex];

    uint32_t imageIndex;
    device->table.vkAcquireNextImageKHR(device->handle, device->swapchain, UINT64_MAX, imageAcquireSemaphore, VK_NULL_HANDLE, &imageIndex);

    VkCommandBuffer commandBuffer = dokVectorCast(VkCommandBuffer, context->commandBuffers.data)[frameIndex];
    device->table.vkResetCommandBuffer(commandBuffer, 0);

    VkCommandBufferBeginInfo beginInfo = { 0 };
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    device->table.vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkImage image = dokVectorCast(VkImage, device->swapchainImages.data)[imageIndex];

    testTransitionImageLayout(
        &device->table, commandBuffer, image,
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

    VkImageView imageView = dokVectorCast(VkImageView, device->swapchainImagesViews.data)[imageIndex];

    VkRenderingAttachmentInfo attachmentInfo = { 0 };
    attachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    attachmentInfo.imageView = imageView;
    attachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    attachmentInfo.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentInfo.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentInfo.clearValue = clearValue;

    VkExtent2D windowExtent;
    dokGetWindowExtent(device->window, &windowExtent);

    VkRect2D renderArea = { 0 };
    renderArea.extent = windowExtent;

    VkRenderingInfo renderingInfo = { 0 };
    renderingInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    renderingInfo.renderArea = renderArea;
    renderingInfo.layerCount = 1;
    renderingInfo.colorAttachmentCount = 1;
    renderingInfo.pColorAttachments = &attachmentInfo;

    device->table.vkCmdBeginRendering(commandBuffer, &renderingInfo);

    VkViewport viewport = { 0 };
    viewport.width = windowExtent.width;
    viewport.height = windowExtent.height;
    viewport.maxDepth = 1.0f;

    VkRect2D scissors = { 0 };
    scissors.extent = windowExtent;

    device->table.vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, context->pipeline);
    
    VkDeviceSize offset = { 0 };
    device->table.vkCmdBindVertexBuffers(commandBuffer, 0, 1, &context->vertexBuffer    , &offset);
    
    device->table.vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    device->table.vkCmdSetScissor(commandBuffer, 0, 1, &scissors);

    device->table.vkCmdDraw(commandBuffer, sizeof(vertices) / sizeof(vertices[0]), 1, 0, 0);
    device->table.vkCmdEndRendering(commandBuffer);

    testTransitionImageLayout(
        &device->table, commandBuffer, image,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        VK_ACCESS_2_NONE,
        VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_2_BOTTOM_OF_PIPE_BIT
    );

    device->table.vkEndCommandBuffer(commandBuffer);
    device->table.vkQueueWaitIdle(device->queue);

    VkSemaphoreSubmitInfo waitSemaphoreInfo = { 0 };
    waitSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    waitSemaphoreInfo.semaphore = imageAcquireSemaphore;
    waitSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkCommandBufferSubmitInfo commandBufferSubmitInfo = { 0 };
    commandBufferSubmitInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO;
    commandBufferSubmitInfo.commandBuffer = commandBuffer;

    VkSemaphore renderCompleteSemaphore = dokVectorCast(VkSemaphore, device->renderCompleteSemaphores.data)[imageIndex];

    VkSemaphoreSubmitInfo signalSemaphoreInfo = { 0 };
    signalSemaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO;
    signalSemaphoreInfo.semaphore = renderCompleteSemaphore;
    signalSemaphoreInfo.stageMask = VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo2 submitInfo = { 0 };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO_2;
    submitInfo.waitSemaphoreInfoCount = 1;
    submitInfo.pWaitSemaphoreInfos = &waitSemaphoreInfo;
    submitInfo.commandBufferInfoCount = 1;
    submitInfo.pCommandBufferInfos = &commandBufferSubmitInfo;
    submitInfo.signalSemaphoreInfoCount = 1;
    submitInfo.pSignalSemaphoreInfos = &signalSemaphoreInfo;

    device->table.vkQueueSubmit2(device->queue, 1, &submitInfo, fence);

    VkPresentInfoKHR presentInfo = { 0 };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderCompleteSemaphore;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &device->swapchain;
    presentInfo.pImageIndices = &imageIndex;

    device->table.vkQueuePresentKHR(device->queue, &presentInfo);

    context->frameIndex = (frameIndex + 1) % context->framesInFlight;
}


int main() {
    TestInstance instance;
    testCreateInstance(&instance);

    uint32_t physicalDeviceCount;
    testEnumerateDevices(instance, &physicalDeviceCount, NULL);

    TestPhysicalDevice* physicalDevices = (TestPhysicalDevice*) malloc(physicalDeviceCount * sizeof(TestPhysicalDevice));
    testEnumerateDevices(instance, &physicalDeviceCount, physicalDevices);

    TestPhysicalDevice physicalDevice = physicalDevices[0];
    free(physicalDevices);

    DokWindowCreateInfo windowInfo = { 0 };
    windowInfo.pTitle = "PEPE";
    windowInfo.height = 420;
    windowInfo.width = 720;

    DokWindow window;
    dokCreateWindow(&windowInfo, &window);

    TestDevice device;
    testCreateDevice(physicalDevice, window, VK_FORMAT_B8G8R8A8_SRGB, &device);

    TestContext context;
    testCreateContext(device, 1, &context);

    bool windowExists = true;

    DokWindowDestroyObserverCreateInfo destroyObserverInfo = { 0 };
    destroyObserverInfo.pUserData = &windowExists;
    destroyObserverInfo.pfnCallback = observerDestroy;

    DokWindowDestroyObserver destroyObserver;
    dokCreateDestroyObserver(window, &destroyObserverInfo, &destroyObserver);

    DokWindowResizeObserverCreateInfo resizeObserverInfo = { 0 };
    resizeObserverInfo.pfnCallback = onWindowResize;
    resizeObserverInfo.pUserData = &device;

    DokWindowResizeObserver resizeObserver;
    // dokCreateResizeObserver(window, &resizeObserverInfo, &resizeObserver);

    while (windowExists) {
        testContextProcess(context);
        dokPollWindow(window);

        device->table.vkDeviceWaitIdle(device->handle);
        testUpdateSwapchain(device, VK_PRESENT_MODE_FIFO_KHR);
    }

    return 0;
}