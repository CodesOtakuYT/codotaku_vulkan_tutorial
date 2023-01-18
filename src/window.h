void swapchainImageAcquire(State *state) {
    EXPECT(vkAcquireNextImageKHR(state->context.device, state->window.swapchain.handle, UINT64_MAX, state->renderer.imageAcquiredSemaphore, NULL, &state->window.swapchain.imageAcquiredIndex), "Couldn't acquire next image")
}

void swapchainImagePresent(State *state) {
    EXPECT(vkQueuePresentKHR(state->context.queue, &(VkPresentInfoKHR) {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .swapchainCount = 1,
        .pSwapchains = &state->window.swapchain.handle,
        .pImageIndices = &state->window.swapchain.imageAcquiredIndex,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &state->renderer.renderFinishedSemaphore,
    }), "Couldn't present swapchain image %i", state->window.swapchain.imageAcquiredIndex)
    EXPECT(vkWaitForFences(state->context.device, 1, &state->renderer.inFlightFence, VK_FALSE, UINT64_MAX), "Couldn't wait for fence")
    EXPECT(vkResetFences(state->context.device, 1, &state->renderer.inFlightFence), "Couldn't reset the fence")
}

void windowPollEvents(const State *state) {
    glfwPollEvents();
}

bool windowShouldClose(const State *state) {
    return glfwWindowShouldClose(state->window.handle);
}

void surfaceCreate(State *state) {
    EXPECT(glfwCreateWindowSurface(state->context.instance, state->window.handle, state->config.allocator, &state->window.surface), "Couldn't create window surface")
}

void surfaceDestroy(State *state) {
    vkDestroySurfaceKHR(state->context.instance, state->window.surface, state->config.allocator);
}

VkSurfaceCapabilitiesKHR surfaceCapabilitiesGet(const State *state) {
    VkSurfaceCapabilitiesKHR capabilities;
    EXPECT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(state->context.physicalDevice, state->window.surface, &capabilities), "Failed to get surface capabilities")
    return capabilities;
}

VkSurfaceFormatKHR surfaceFormatsSelect(const State *state) {
    uint32_t formatCount;
    EXPECT(vkGetPhysicalDeviceSurfaceFormatsKHR(state->context.physicalDevice, state->window.surface, &formatCount, NULL), "Couldn't get surface formats")
    VkSurfaceFormatKHR *formats = malloc(formatCount*sizeof(VkSurfaceFormatKHR));
    EXPECT(!formats, "Couldn't allocate formats memory")
    EXPECT(vkGetPhysicalDeviceSurfaceFormatsKHR(state->context.physicalDevice, state->window.surface, &formatCount, formats), "Couldn't get surface formats")

    uint32_t formatIndex = 0;

    for (int i = 0; i < formatCount; ++i) {
        VkSurfaceFormatKHR format = formats[i];
        if(format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR && format.format == VK_FORMAT_B8G8R8A8_SRGB) {
            formatIndex = i;
            break;
        }
    }

    VkSurfaceFormatKHR format = formats[formatIndex];
    free(formats);
    return format;
}

VkPresentModeKHR surfacePresentModesSelect(const State *state) {
    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    uint32_t presentModeCount;
    EXPECT(vkGetPhysicalDeviceSurfacePresentModesKHR(state->context.physicalDevice, state->window.surface, &presentModeCount, NULL), "Couldn't get surface present modes count")
    VkPresentModeKHR *presentModes = malloc(presentModeCount*sizeof(VkPresentModeKHR));
    EXPECT(!presentModes, "Couldn't allocate present modes memory")

    uint32_t presentModeIndex = UINT32_MAX;

    for (int i = 0; i < presentModeCount; ++i) {
        VkPresentModeKHR mode = presentModes[i];
        if(mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            presentModeIndex = i;
            break;
        }
    }

    if(presentModeIndex != UINT32_MAX) {
        presentMode = presentModes[presentModeIndex];
    }

    free(presentModes);
    return presentMode;
}

void swapchainImagesGet(State *state) {
    EXPECT(vkGetSwapchainImagesKHR(state->context.device, state->window.swapchain.handle, &state->window.swapchain.imageCount, NULL), "Couldn't get swapchain images count")
    state->window.swapchain.images = malloc(state->window.swapchain.imageCount*sizeof(VkImage));
    EXPECT(!state->window.swapchain.images, "Couldn't allocate memory for swapchain images")
    EXPECT(vkGetSwapchainImagesKHR(state->context.device, state->window.swapchain.handle, &state->window.swapchain.imageCount, state->window.swapchain.images), "Couldn't get swapchain images")
}

void swapchainImageViewsCreate(State *state) {
    state->window.swapchain.imageViews = malloc(state->window.swapchain.imageCount*sizeof(VkImageView));
    EXPECT(!state->window.swapchain.imageViews, "Couldn't allocate memory for swapchain image views")

    for (int i = 0; i < state->window.swapchain.imageCount; ++i) {
        EXPECT(vkCreateImageView(state->context.device, &(VkImageViewCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .format = state->window.swapchain.format,
                .image = state->window.swapchain.images[i],
                .components = state->config.swapchainComponentsMapping,
                .subresourceRange = (VkImageSubresourceRange) {
                        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                        .layerCount = 1,
                        .levelCount = 1,
                },
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
        }, state->config.allocator, &state->window.swapchain.imageViews[i]), "Couldn't create image view %i", i)
    }
}

void swapchainCreate(State *state) {
    VkSurfaceCapabilitiesKHR capabilities = surfaceCapabilitiesGet(state);
    VkSurfaceFormatKHR surfaceFormat = surfaceFormatsSelect(state);
    VkPresentModeKHR presentMode = surfacePresentModesSelect(state);

    state->window.swapchain.format = surfaceFormat.format;
    state->window.swapchain.colorSpace = surfaceFormat.colorSpace;
    state->window.swapchain.imageExtent = capabilities.currentExtent;

    EXPECT(vkCreateSwapchainKHR(state->context.device, &(VkSwapchainCreateInfoKHR) {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = state->window.surface,
            .queueFamilyIndexCount = 1,
            .pQueueFamilyIndices = &state->context.queueFamily,
            .clipped = true,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform = capabilities.currentTransform,
            .imageExtent = state->window.swapchain.imageExtent,
            .imageFormat = state->window.swapchain.format,
            .imageColorSpace = state->window.swapchain.colorSpace,
            .presentMode = presentMode,
            .minImageCount = clamp(state->config.swapchainBuffering, capabilities.minImageCount, capabilities.maxImageCount ? capabilities.maxImageCount : UINT32_MAX),
    }, state->config.allocator, &state->window.swapchain.handle), "Couldn't create swapchain")

    swapchainImagesGet(state);
    swapchainImageViewsCreate(state);
}

void swapchainDestroy(State *state) {
    for (int i = 0; i < state->window.swapchain.imageCount; ++i) {
        vkDestroyImageView(state->context.device, state->window.swapchain.imageViews[i], state->config.allocator);
    }
    free(state->window.swapchain.imageViews);
    free(state->window.swapchain.images);

    vkDestroySwapchainKHR(state->context.device, state->window.swapchain.handle, state->config.allocator);
}

void windowCreate(State *state) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    int width = state->config.windowWidth;
    int height = state->config.windowHeight;

    GLFWmonitor *monitor = NULL;

    if(state->config.windowFullscreen) {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        width = mode->width;
        height = mode->height;
    }

    state->window.handle = glfwCreateWindow(width, height, state->config.windowTitle, monitor, NULL);

    surfaceCreate(state);
    swapchainCreate(state);
}

void windowDestroy(State *state) {
    swapchainDestroy(state);
    surfaceDestroy(state);
    glfwDestroyWindow(state->window.handle);
}