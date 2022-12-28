#pragma once

void instanceCreate(State *state) {
    uint32_t requiredExtensionsCount;
    const char **requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

    EXPECT(vkCreateInstance(&(VkInstanceCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &(VkApplicationInfo) {
                    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pApplicationName = state->config.applicationName,
                    .pEngineName = state->config.engineName,
                    .apiVersion = state->config.apiVersion,
            },
            .enabledExtensionCount = requiredExtensionsCount,
            .ppEnabledExtensionNames = requiredExtensions,
    }, state->config.allocator, &state->context.instance), "Couldn't create instance")
}

void physicalDeviceSelect(State *state) {
    uint32_t count;
    EXPECT(vkEnumeratePhysicalDevices(state->context.instance, &count, NULL), "Couldn't enumerate physical devices count")
    EXPECT(count == 0, "Couldn't find a vulkan supported physical device")
    VkResult result = vkEnumeratePhysicalDevices(state->context.instance, &(uint32_t){1}, &state->context.physicalDevice);
    if(result != VK_INCOMPLETE) EXPECT(result, "Couldn't enumerate physical devices")
}

void queueFamilySelect(State *state) {
    state->context.queueFamily = UINT32_MAX;

    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(state->context.physicalDevice, &count, NULL);

    VkQueueFamilyProperties *queueFamilies = malloc(count * sizeof(VkQueueFamilyProperties));
    EXPECT(queueFamilies == NULL, "Couldn't allocate memory")
    vkGetPhysicalDeviceQueueFamilyProperties(state->context.physicalDevice, &count, queueFamilies);

    for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < count; ++queueFamilyIndex) {
        VkQueueFamilyProperties properties = queueFamilies[queueFamilyIndex];

        if((properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
           && glfwGetPhysicalDevicePresentationSupport(state->context.instance, state->context.physicalDevice, queueFamilyIndex)) {
            state->context.queueFamily = queueFamilyIndex;
            break;
        }
    }

    EXPECT(state->context.queueFamily == UINT32_MAX, "Couldn't find a suitable queue family")
    free(queueFamilies);
}

void deviceCreate(State *state) {
    EXPECT(vkCreateDevice(state->context.physicalDevice, &(VkDeviceCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pQueueCreateInfos = &(VkDeviceQueueCreateInfo) {
                    .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                    .queueFamilyIndex = state->context.queueFamily,
                    .queueCount = 1,
                    .pQueuePriorities = &(float){1.0},
            },
            .queueCreateInfoCount = 1,
            .enabledExtensionCount = 1,
            .ppEnabledExtensionNames = &(const char *) {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
    }, state->config.allocator, &state->context.device), "Couldn't create device and queues")
}

void queueGet(State *state) {
    vkGetDeviceQueue(state->context.device, state->context.queueFamily, 0, &state->context.queue);
}

void contextCreate(State *state) {
    instanceCreate(state);
    physicalDeviceSelect(state);
    queueFamilySelect(state);
    deviceCreate(state);
    queueGet(state);
}

void contextDestroy(State *state) {
    vkDestroyDevice(state->context.device, state->config.allocator);
    vkDestroyInstance(state->context.instance, state->config.allocator);
}