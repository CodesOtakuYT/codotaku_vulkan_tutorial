#pragma once

#define EXPECT(ERROR, FORMAT, ...) {                                                                                                        \
    int macroErrorCode = ERROR;                                                                                                             \
    if(macroErrorCode) {                                                                                                                    \
        fprintf(stderr, "%s -> %s -> %i -> Error(%i):\n\t" FORMAT "\n", __FILE__, __func__, __LINE__, macroErrorCode, ##__VA_ARGS__);  \
        raise(SIGABRT);                                                                                                                     \
    }                                                                                                                                       \
}

void infoLog() {
    uint32_t instanceApiVersion;
    EXPECT(vkEnumerateInstanceVersion(&instanceApiVersion), "Couldn't enumerate instance version")
    uint32_t apiVersionVariant = VK_API_VERSION_VARIANT(instanceApiVersion);
    uint32_t apiVersionMajor = VK_API_VERSION_MAJOR(instanceApiVersion);
    uint32_t apiVersionMinor = VK_API_VERSION_MINOR(instanceApiVersion);
    uint32_t apiVersionPatch = VK_API_VERSION_PATCH(instanceApiVersion);
    printf("Vulkan API %i.%i.%i.%i\n", apiVersionVariant, apiVersionMajor, apiVersionMinor, apiVersionPatch);
    printf("GLFW %s\n", glfwGetVersionString());
}

void exitCallback() {
    glfwTerminate();
}

void glfwErrorCallback(int errorCode, const char *description) {
    EXPECT(errorCode, "GLFW: %s", description)
}

void errorHandlingSetup() {
    glfwSetErrorCallback(glfwErrorCallback);
    atexit(exitCallback);
}

uint32_t clamp(uint32_t value, uint32_t min, uint32_t max) {
    if(value < min) {
        return min;
    } else if(value > max) {
        return max;
    }
    return value;
}
