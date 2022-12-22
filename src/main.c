#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#define PANIC(ERROR, FORMAT, ...) {                                                                                                     \
    if(ERROR) {                                                                                                                         \
        fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n", __FILE_NAME__, __FUNCTION__, __LINE__, ERROR, ##__VA_ARGS__);   \
        raise(SIGABRT);                                                                                                                 \
    }                                                                                                                                   \
}

void glfwErrorCallback(int errorCode, const char *description) {
    PANIC(errorCode, "GLFW: %s", description)
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
    bool windowResizable;
    bool windowFullscreen;

    VkAllocationCallbacks *allocator;

    // GLFW
    GLFWmonitor *windowMonitor;
    GLFWwindow *window;

    // Vulkan
    uint32_t apiVersion;
    uint32_t queueFamily;

    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkSurfaceKHR surface;
    VkDevice device;
    VkQueue queue;
} State;

void setupErrorHandling() {
    glfwSetErrorCallback(glfwErrorCallback);
    atexit(exitCallback);
}

void createWindow(State *state) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, state->windowResizable);

    if(state->windowFullscreen) {
        state->windowMonitor = glfwGetPrimaryMonitor();
        // Setting width and height to monitor's resolution
        GLFWvidmode *mode = glfwGetVideoMode(state->windowMonitor);
        state->windowWidth = mode->width;
        state->windowHeight = mode->height;
    }

    state->window = glfwCreateWindow(state->windowWidth, state->windowHeight, state->windowTitle, state->windowMonitor, NULL);
}

void createInstance(State *state) {
    uint32_t requiredExtensionsCount;
    const char **requiredExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionsCount);

    PANIC(vkCreateInstance(&(VkInstanceCreateInfo) {
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
    PANIC(vkEnumerateInstanceVersion(&instanceApiVersion), "Couldn't enumerate instance version")
    uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
    uint32_t apiVersionMajor = VK_API_VERSION_MAJOR(instanceApiVersion);
    uint32_t apiVersionMinor = VK_API_VERSION_MINOR(instanceApiVersion);
    uint32_t apiVersionPatch = VK_API_VERSION_PATCH(instanceApiVersion);
    printf("Vulkan API %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor, apiVersionMinor, apiVersionPatch);
    printf("GLFW %s\n", glfwGetVersionString());
}

void selectPhysicalDevice(State *state) {
    uint32_t count;
    PANIC(vkEnumeratePhysicalDevices(state->instance, &count, NULL), "Couldn't enumerate physical devices count")
    PANIC(count == 0, "Couldn't find a vulkan supported physical device")
    PANIC(vkEnumeratePhysicalDevices(state->instance, &(uint32_t){1}, &state->physicalDevice), "Couldn't enumerate physical devices count")
}

void createSurface(State *state) {
    PANIC(glfwCreateWindowSurface(state->instance, state->window, state->allocator, &state->surface), "Couldn't create window surface")
}

void selectQueueFamily(State *state) {
    state->queueFamily = UINT32_MAX;

    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(state->physicalDevice, &count, NULL);

    VkQueueFamilyProperties *queueFamilies = malloc(count * sizeof(VkQueueFamilyProperties));
    PANIC(queueFamilies == NULL, "Couldn't allocate memory")
    vkGetPhysicalDeviceQueueFamilyProperties(state->physicalDevice, &count, queueFamilies);

    for (uint32_t queueFamilyIndex = 0; queueFamilyIndex < count; ++queueFamilyIndex) {
        VkQueueFamilyProperties properties = queueFamilies[queueFamilyIndex];

        if((properties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        && glfwGetPhysicalDevicePresentationSupport(state->instance, state->physicalDevice, queueFamilyIndex)) {
            state->queueFamily = queueFamilyIndex;
            break;
        }
    }

    PANIC(state->queueFamily == UINT32_MAX, "Couldn't find a suitable queue family")
    free(queueFamilies);
}

void createDevice(State *state) {
    PANIC(vkCreateDevice(state->physicalDevice, &(VkDeviceCreateInfo) {
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
}

void loop(State *state) {
    while(!glfwWindowShouldClose(state->window)) {
        glfwPollEvents();
    }
}

void cleanup(State *state) {
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
        .windowResizable = false,
        .windowFullscreen = false,
        .apiVersion = VK_API_VERSION_1_3,
    };

    init(&state);
    loop(&state);
    cleanup(&state);

    return EXIT_SUCCESS;
}
