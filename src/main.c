#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#define EXPECT(ERROR, FORMAT, ...) {                                                                                                    \
    int errCode;                                                                                                                        \
    if((errCode = ERROR)) {                                                                                                             \
        fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n", __FILE_NAME__, __FUNCTION__, __LINE__, errCode, ##__VA_ARGS__); \
        raise(SIGABRT);                                                                                                                 \
    }                                                                                                                                   \
}

void glfwErrorCallback(int errorCode, const char *description) {
    EXPECT(errorCode, "GLFW: %s", description)
}

void exitCallback() {
    glfwTerminate();
}

typedef struct {
    // Configurable
    const char *applicationName;
    const char *engineName;
    const char *windowTitle;

    int windowWidth, windowHeight;
    bool windowFullscreen;

    VkAllocationCallbacks *allocator;

    // GLFW
    GLFWmonitor *windowMonitor;
    GLFWwindow *window;

    // Vulkan
    uint32_t apiVersion;
    uint32_t queueFamily;
    uint32_t swapchainImageCount;

    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR surface;
    VkDevice device;
    VkQueue queue;
    
    VkSwapchainKHR swapchain;
    VkImage *swapchainImages;
    VkImageView *swapchainImageViews;
} State;

void setupErrorHandling() {
    glfwSetErrorCallback(glfwErrorCallback);
    atexit(exitCallback);
}

void createWindow(State *state) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if(state->windowFullscreen) {
        state->windowMonitor = glfwGetPrimaryMonitor();

        const GLFWvidmode *mode = glfwGetVideoMode(state->windowMonitor);
        state->windowWidth = mode->width;
        state->windowHeight = mode->height;
    }

    state->window = glfwCreateWindow(state->windowWidth, state->windowHeight, state->windowTitle, state->windowMonitor, NULL);
}

void createInstance(State *state) {
    uint32_t requiredExtensionsCount;
    const char **requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

    EXPECT(vkCreateInstance(&(VkInstanceCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pApplicationInfo = &(VkApplicationInfo) {
                    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pApplicationName = state->applicationName,
                    .pEngineName = state->engineName,
                    .apiVersion = state->apiVersion,
            },
            .enabledExtensionCount = requiredExtensionsCount,
            .ppEnabledExtensionNames = requiredExtensions,
    }, state->allocator, &state->instance), "Couldn't create instance")
}

void logInfo() {
    uint32_t instanceApiVersion;
    EXPECT(vkEnumerateInstanceVersion(&instanceApiVersion), "Couldn't enumerate instance version")
    uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
    uint32_t apiVersionMajor = VK_API_VERSION_MAJOR(instanceApiVersion);
    uint32_t apiVersionMinor = VK_API_VERSION_MINOR(instanceApiVersion);
    uint32_t apiVersionPatch = VK_API_VERSION_PATCH(instanceApiVersion);
    printf("Vulkan API %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor, apiVersionMinor, apiVersionPatch);
    printf("GLFW %s\n", glfwGetVersionString());
}

void selectPhysicalDevice(State *state) {
    uint32_t count;
    EXPECT(vkEnumeratePhysicalDevices(state->instance, &count, NULL), "Couldn't enumerate physical devices count")
    EXPECT(count == 0, "Couldn't find a vulkan supported physical device")
    EXPECT(vkEnumeratePhysicalDevices(state->instance, &(uint32_t){1}, &state->physicalDevice), "Couldn't enumerate physical devices count")
}

void createSurface(State *state) {
    EXPECT(glfwCreateWindowSurface(state->instance, state->window, state->allocator, &state->surface), "Couldn't create window surface")
}

void selectQueueFamily(State *state) {
    state->queueFamily = UINT32_MAX;

    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(state->physicalDevice, &count, NULL);

    VkQueueFamilyProperties *queueFamilies = malloc(count * sizeof(VkQueueFamilyProperties));
    EXPECT(queueFamilies == NULL, "Couldn't allocate memory")
    vkGetPhysicalDeviceQueueFamilyProperties(state->physicalDevice, &count, queueFamilies);

    for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < count; ++queueFamilyIndex) {
        VkQueueFamilyProperties properties = queueFamilies[queueFamilyIndex];

        if((properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        && glfwGetPhysicalDevicePresentationSupport(state->instance, state->physicalDevice, queueFamilyIndex)) {
            state->queueFamily = queueFamilyIndex;
            break;
        }
    }

    EXPECT(state->queueFamily == UINT32_MAX, "Couldn't find a suitable queue family")
    free(queueFamilies);
}

void createDevice(State *state) {
    EXPECT(vkCreateDevice(state->physicalDevice, &(VkDeviceCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = &(VkDeviceQueueCreateInfo) {
                .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
                .queueFamilyIndex = state->queueFamily,
                .queueCount = 1,
                .pQueuePriorities = &(float){1.0},
        },
        .queueCreateInfoCount = 1,
        .enabledExtensionCount = 1,
        .ppEnabledExtensionNames = &(const char *) {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
    }, state->allocator, &state->device), "Couldn't create device and queues")
}

void getQueue(State *state) {
    vkGetDeviceQueue(state->device, state->queueFamily, 0, &state->queue);
}

uint32_t clamp(uint32_t value, uint32_t min, uint32_t max) {
    if(value < min) {
        return min;
    } else if(value > max) {
        return max;
    }
    return value;
}

void createSwapchain(State *state) {
    VkSurfaceCapabilitiesKHR capabilities;
    EXPECT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(state->physicalDevice, state->surface, &capabilities), "Failed to get surface capabilities")

    uint32_t formatCount;
    EXPECT(vkGetPhysicalDeviceSurfaceFormatsKHR(state->physicalDevice, state->surface, &formatCount, NULL), "Couldn't get surface formats")
    VkSurfaceFormatKHR *formats = malloc(formatCount*sizeof(VkSurfaceFormatKHR));
    EXPECT(!formats, "Couldn't allocate formats memory")
    EXPECT(vkGetPhysicalDeviceSurfaceFormatsKHR(state->physicalDevice, state->surface, &formatCount, formats), "Couldn't get surface formats")

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

    VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
    uint32_t presentModeCount;
    EXPECT(vkGetPhysicalDeviceSurfacePresentModesKHR(state->physicalDevice, state->surface, &presentModeCount, NULL), "Couldn't get surface present modes count")
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

    EXPECT(vkCreateSwapchainKHR(state->device, &(VkSwapchainCreateInfoKHR) {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = state->surface,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices = &state->queueFamily,
        .clipped = true,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .preTransform = capabilities.currentTransform,
        .imageExtent = capabilities.currentExtent,
        .imageFormat = format.format,
        .imageColorSpace = format.colorSpace,
        .presentMode = presentMode,
        .minImageCount = clamp(3, capabilities.minImageCount, capabilities.maxImageCount ? capabilities.maxImageCount : UINT32_MAX),
    }, state->allocator, &state->swapchain), "Couldn't create swapchain")

    EXPECT(vkGetSwapchainImagesKHR(state->device, state->swapchain, &state->swapchainImageCount, NULL), "Couldn't get swapchain images count")
    state->swapchainImages = malloc(state->swapchainImageCount*sizeof(VkImage));
    EXPECT(!state->swapchainImages, "Couldn't allocate memory for swapchain images")
    EXPECT(vkGetSwapchainImagesKHR(state->device, state->swapchain, &state->swapchainImageCount, state->swapchainImages), "Couldn't get swapchain images")
    state->swapchainImageViews = malloc(state->swapchainImageCount*sizeof(VkImageView));
    EXPECT(!state->swapchainImageViews, "Couldn't allocate memory for swapchain image views")

    for (int i = 0; i < state->swapchainImageCount; ++i) {
        EXPECT(vkCreateImageView(state->device, &(VkImageViewCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .format = format.format,
            .image = state->swapchainImages[i],
            .components = (VkComponentMapping) {},
            .subresourceRange = (VkImageSubresourceRange) {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .layerCount = 1,
                .levelCount = 1,
            },
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
        }, state->allocator, &state->swapchainImageViews[i]), "Couldn't create image view %i", i)
    }
}

void init(State *state) {
    setupErrorHandling();
    logInfo();

    createWindow(state);
    createInstance(state);

    selectPhysicalDevice(state);
    createSurface(state);
    selectQueueFamily(state);
    createDevice(state);
    getQueue(state);

    createSwapchain(state);
}

void loop(State *state) {
    while(!glfwWindowShouldClose(state->window)) {
        glfwPollEvents();
    }
}

void cleanup(State *state) {
    for (int i = 0; i < state->swapchainImageCount; ++i) {
        vkDestroyImageView(state->device, state->swapchainImageViews[i], state->allocator);
    }
    free(state->swapchainImageViews);
    free(state->swapchainImages);
    vkDestroySwapchainKHR(state->device, state->swapchain, state->allocator);
    vkDestroyDevice(state->device, state->allocator);
    vkDestroySurfaceKHR(state->instance, state->surface, state->allocator);
    vkDestroyInstance(state->instance, state->allocator);
}

int main() {
    State state = {
        .applicationName = "CODOTAKU",
        .engineName = "CODOTAKU ENGINE",
        .windowTitle = "CODOTAKU",
        .windowWidth = 720,
        .windowHeight = 480,
        .windowFullscreen = false,
        .apiVersion = VK_API_VERSION_1_3,
    };

    init(&state);
    loop(&state);
    cleanup(&state);

    return EXIT_SUCCESS;
}
