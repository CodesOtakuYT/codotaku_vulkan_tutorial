#pragma once

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

typedef struct {
    const char *applicationName;
    const char *engineName;
    const char *windowTitle;

    int windowWidth, windowHeight;
    bool windowFullscreen;

    uint32_t apiVersion;
    VkAllocationCallbacks *allocator;
    VkComponentMapping swapchainComponentsMapping;
    uint32_t swapchainBuffering;
    VkClearValue backgroundColor;
} Config;

typedef struct {
    uint32_t queueFamily;

    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue queue;
} Context;

typedef struct {
    VkSwapchainKHR handle;

    uint32_t imageCount;
    VkImage *images;
    VkImageView *imageViews;

    VkFormat format;
    VkColorSpaceKHR colorSpace;
    VkExtent2D imageExtent;
    uint32_t imageAcquiredIndex;
} Swapchain;

typedef struct {
    GLFWwindow *handle;
    VkSurfaceKHR surface;
    Swapchain swapchain;
} Window;

typedef struct {
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderpass;
    VkFramebuffer* framebuffers;
    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
    VkSemaphore imageAcquiredSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
} Renderer;

typedef struct {
    Config config;
    Window window;
    Context context;
    Renderer renderer;
} State;

enum SwapchainBuffering {
    SWAPCHAIN_DOUBLE_BUFFERING = 2,
    SWAPCHAIN_TRIPLE_BUFFERING = 3,
};