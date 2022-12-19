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

    VkInstance instance;

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

void init(State *state) {
    setupErrorHandling();
    logInfo();
    createWindow(state);
    createInstance(state);
}

void loop(State *state) {
    while(!glfwWindowShouldClose(state->window)) {
        glfwPollEvents();
    }
}

void cleanup(State *state) {
    glfwDestroyWindow(state->window);
    vkDestroyInstance(state->instance, state->allocator);
    state->window = NULL;
}

int main() {
    State state = {
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