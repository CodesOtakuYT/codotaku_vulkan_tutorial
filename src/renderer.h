#pragma once

void graphicsPipelineCreate(State *state) {
    uint32_t shaderVertexCode[] = {0x07230203,0x00010000,0x000d000a,0x00000036,
                                   0x00000000,0x00020011,0x00000001,0x0006000b,
                                   0x00000001,0x4c534c47,0x6474732e,0x3035342e,
                                   0x00000000,0x0003000e,0x00000000,0x00000001,
                                   0x0008000f,0x00000000,0x00000004,0x6e69616d,
                                   0x00000000,0x00000022,0x00000026,0x00000031,
                                   0x00030003,0x00000002,0x000001cc,0x000a0004,
                                   0x475f4c47,0x4c474f4f,0x70635f45,0x74735f70,
                                   0x5f656c79,0x656e696c,0x7269645f,0x69746365,
                                   0x00006576,0x00080004,0x475f4c47,0x4c474f4f,
                                   0x6e695f45,0x64756c63,0x69645f65,0x74636572,
                                   0x00657669,0x00040005,0x00000004,0x6e69616d,
                                   0x00000000,0x00050005,0x0000000c,0x69736f70,
                                   0x6e6f6974,0x00000073,0x00040005,0x00000017,
                                   0x6f6c6f63,0x00007372,0x00060005,0x00000020,
                                   0x505f6c67,0x65567265,0x78657472,0x00000000,
                                   0x00060006,0x00000020,0x00000000,0x505f6c67,
                                   0x7469736f,0x006e6f69,0x00070006,0x00000020,
                                   0x00000001,0x505f6c67,0x746e696f,0x657a6953,
                                   0x00000000,0x00070006,0x00000020,0x00000002,
                                   0x435f6c67,0x4470696c,0x61747369,0x0065636e,
                                   0x00070006,0x00000020,0x00000003,0x435f6c67,
                                   0x446c6c75,0x61747369,0x0065636e,0x00030005,
                                   0x00000022,0x00000000,0x00060005,0x00000026,
                                   0x565f6c67,0x65747265,0x646e4978,0x00007865,
                                   0x00050005,0x00000031,0x67617266,0x6f6c6f43,
                                   0x00000072,0x00050048,0x00000020,0x00000000,
                                   0x0000000b,0x00000000,0x00050048,0x00000020,
                                   0x00000001,0x0000000b,0x00000001,0x00050048,
                                   0x00000020,0x00000002,0x0000000b,0x00000003,
                                   0x00050048,0x00000020,0x00000003,0x0000000b,
                                   0x00000004,0x00030047,0x00000020,0x00000002,
                                   0x00040047,0x00000026,0x0000000b,0x0000002a,
                                   0x00040047,0x00000031,0x0000001e,0x00000000,
                                   0x00020013,0x00000002,0x00030021,0x00000003,
                                   0x00000002,0x00030016,0x00000006,0x00000020,
                                   0x00040017,0x00000007,0x00000006,0x00000002,
                                   0x00040015,0x00000008,0x00000020,0x00000000,
                                   0x0004002b,0x00000008,0x00000009,0x00000003,
                                   0x0004001c,0x0000000a,0x00000007,0x00000009,
                                   0x00040020,0x0000000b,0x00000006,0x0000000a,
                                   0x0004003b,0x0000000b,0x0000000c,0x00000006,
                                   0x0004002b,0x00000006,0x0000000d,0xbf000000,
                                   0x0004002b,0x00000006,0x0000000e,0x3f000000,
                                   0x0005002c,0x00000007,0x0000000f,0x0000000d,
                                   0x0000000e,0x0005002c,0x00000007,0x00000010,
                                   0x0000000e,0x0000000e,0x0004002b,0x00000006,
                                   0x00000011,0x00000000,0x0005002c,0x00000007,
                                   0x00000012,0x00000011,0x0000000d,0x0006002c,
                                   0x0000000a,0x00000013,0x0000000f,0x00000010,
                                   0x00000012,0x00040017,0x00000014,0x00000006,
                                   0x00000003,0x0004001c,0x00000015,0x00000014,
                                   0x00000009,0x00040020,0x00000016,0x00000006,
                                   0x00000015,0x0004003b,0x00000016,0x00000017,
                                   0x00000006,0x0004002b,0x00000006,0x00000018,
                                   0x3f800000,0x0006002c,0x00000014,0x00000019,
                                   0x00000018,0x00000011,0x00000011,0x0006002c,
                                   0x00000014,0x0000001a,0x00000011,0x00000018,
                                   0x00000011,0x0006002c,0x00000014,0x0000001b,
                                   0x00000011,0x00000011,0x00000018,0x0006002c,
                                   0x00000015,0x0000001c,0x00000019,0x0000001a,
                                   0x0000001b,0x00040017,0x0000001d,0x00000006,
                                   0x00000004,0x0004002b,0x00000008,0x0000001e,
                                   0x00000001,0x0004001c,0x0000001f,0x00000006,
                                   0x0000001e,0x0006001e,0x00000020,0x0000001d,
                                   0x00000006,0x0000001f,0x0000001f,0x00040020,
                                   0x00000021,0x00000003,0x00000020,0x0004003b,
                                   0x00000021,0x00000022,0x00000003,0x00040015,
                                   0x00000023,0x00000020,0x00000001,0x0004002b,
                                   0x00000023,0x00000024,0x00000000,0x00040020,
                                   0x00000025,0x00000001,0x00000023,0x0004003b,
                                   0x00000025,0x00000026,0x00000001,0x00040020,
                                   0x00000028,0x00000006,0x00000007,0x00040020,
                                   0x0000002e,0x00000003,0x0000001d,0x00040020,
                                   0x00000030,0x00000003,0x00000014,0x0004003b,
                                   0x00000030,0x00000031,0x00000003,0x00040020,
                                   0x00000033,0x00000006,0x00000014,0x00050036,
                                   0x00000002,0x00000004,0x00000000,0x00000003,
                                   0x000200f8,0x00000005,0x0003003e,0x0000000c,
                                   0x00000013,0x0003003e,0x00000017,0x0000001c,
                                   0x0004003d,0x00000023,0x00000027,0x00000026,
                                   0x00050041,0x00000028,0x00000029,0x0000000c,
                                   0x00000027,0x0004003d,0x00000007,0x0000002a,
                                   0x00000029,0x00050051,0x00000006,0x0000002b,
                                   0x0000002a,0x00000000,0x00050051,0x00000006,
                                   0x0000002c,0x0000002a,0x00000001,0x00070050,
                                   0x0000001d,0x0000002d,0x0000002b,0x0000002c,
                                   0x00000011,0x00000018,0x00050041,0x0000002e,
                                   0x0000002f,0x00000022,0x00000024,0x0003003e,
                                   0x0000002f,0x0000002d,0x0004003d,0x00000023,
                                   0x00000032,0x00000026,0x00050041,0x00000033,
                                   0x00000034,0x00000017,0x00000032,0x0004003d,
                                   0x00000014,0x00000035,0x00000034,0x0003003e,
                                   0x00000031,0x00000035,0x000100fd,0x00010038};

    uint32_t shaderFragmentCode[] = {0x07230203,0x00010000,0x000d000a,0x00000013,
                                     0x00000000,0x00020011,0x00000001,0x0006000b,
                                     0x00000001,0x4c534c47,0x6474732e,0x3035342e,
                                     0x00000000,0x0003000e,0x00000000,0x00000001,
                                     0x0007000f,0x00000004,0x00000004,0x6e69616d,
                                     0x00000000,0x00000009,0x0000000c,0x00030010,
                                     0x00000004,0x00000007,0x00030003,0x00000002,
                                     0x000001cc,0x000a0004,0x475f4c47,0x4c474f4f,
                                     0x70635f45,0x74735f70,0x5f656c79,0x656e696c,
                                     0x7269645f,0x69746365,0x00006576,0x00080004,
                                     0x475f4c47,0x4c474f4f,0x6e695f45,0x64756c63,
                                     0x69645f65,0x74636572,0x00657669,0x00040005,
                                     0x00000004,0x6e69616d,0x00000000,0x00050005,
                                     0x00000009,0x4374756f,0x726f6c6f,0x00000000,
                                     0x00050005,0x0000000c,0x67617266,0x6f6c6f43,
                                     0x00000072,0x00040047,0x00000009,0x0000001e,
                                     0x00000000,0x00040047,0x0000000c,0x0000001e,
                                     0x00000000,0x00020013,0x00000002,0x00030021,
                                     0x00000003,0x00000002,0x00030016,0x00000006,
                                     0x00000020,0x00040017,0x00000007,0x00000006,
                                     0x00000004,0x00040020,0x00000008,0x00000003,
                                     0x00000007,0x0004003b,0x00000008,0x00000009,
                                     0x00000003,0x00040017,0x0000000a,0x00000006,
                                     0x00000003,0x00040020,0x0000000b,0x00000001,
                                     0x0000000a,0x0004003b,0x0000000b,0x0000000c,
                                     0x00000001,0x0004002b,0x00000006,0x0000000e,
                                     0x3f800000,0x00050036,0x00000002,0x00000004,
                                     0x00000000,0x00000003,0x000200f8,0x00000005,
                                     0x0004003d,0x0000000a,0x0000000d,0x0000000c,
                                     0x00050051,0x00000006,0x0000000f,0x0000000d,
                                     0x00000000,0x00050051,0x00000006,0x00000010,
                                     0x0000000d,0x00000001,0x00050051,0x00000006,
                                     0x00000011,0x0000000d,0x00000002,0x00070050,
                                     0x00000007,0x00000012,0x0000000f,0x00000010,
                                     0x00000011,0x0000000e,0x0003003e,0x00000009,
                                     0x00000012,0x000100fd,0x00010038};

    const char *shaderEntryFunctionName = "main";

    VkShaderModule vertexShaderModule, fragmentShaderModule;

    EXPECT(vkCreateShaderModule(state->context.device, &(VkShaderModuleCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pCode = shaderVertexCode,
        .codeSize = sizeof(shaderVertexCode),
    }, state->config.allocator, &vertexShaderModule), "Couldn't create vertex shader module")

    EXPECT(vkCreateShaderModule(state->context.device, &(VkShaderModuleCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .pCode = shaderFragmentCode,
        .codeSize = sizeof(shaderFragmentCode),
    }, state->config.allocator, &fragmentShaderModule), "Couldn't create fragment shader module")

    VkPipelineShaderStageCreateInfo shaderStages[] = {
        (VkPipelineShaderStageCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .module = vertexShaderModule,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .pName = shaderEntryFunctionName,
        },
        (VkPipelineShaderStageCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .module = fragmentShaderModule,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .pName = shaderEntryFunctionName,
        },
    };

    VkExtent2D imageExtent = state->window.swapchain.imageExtent;

    VkViewport viewports[] = {
        {
            .width = (float) imageExtent.width,
            .height = (float) imageExtent.height,
            .maxDepth = 1.0f,
        }
    };

    VkRect2D scissors[] = {
        {
            .extent = imageExtent,
        }
    };

    VkPipelineColorBlendAttachmentState colorBlendAttachmentStates[] = {
        {
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
        }
    };

    EXPECT(vkCreatePipelineLayout(state->context.device, &(VkPipelineLayoutCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    }, state->config.allocator, &state->renderer.pipelineLayout), "Couldn't create pipeline layout")

    EXPECT(vkCreateGraphicsPipelines(state->context.device, NULL, 1, &(VkGraphicsPipelineCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .layout = state->renderer.pipelineLayout,
        .pVertexInputState = &(VkPipelineVertexInputStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        },
        .pInputAssemblyState = &(VkPipelineInputAssemblyStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        },
        .pStages = (const VkPipelineShaderStageCreateInfo *) &shaderStages,
        .stageCount = sizeof(shaderStages)/sizeof(*shaderStages),
        .pViewportState = &(VkPipelineViewportStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
            .viewportCount = sizeof(viewports)/sizeof(*viewports),
            .pViewports = viewports,
            .scissorCount = sizeof(scissors)/sizeof(*scissors),
            .pScissors = scissors,
        },
        .pRasterizationState = &(VkPipelineRasterizationStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .lineWidth = 1.0,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
            .cullMode = VK_CULL_MODE_BACK_BIT,
        },
        .pMultisampleState = &(VkPipelineMultisampleStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        },
        .pColorBlendState = &(VkPipelineColorBlendStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .attachmentCount = sizeof(colorBlendAttachmentStates)/sizeof(*colorBlendAttachmentStates),
            .pAttachments = colorBlendAttachmentStates,
        },
        .renderPass = state->renderer.renderpass,
    }, state->config.allocator, &state->renderer.graphicsPipeline), "Couldn't create graphics pipeline")

    vkDestroyPipelineLayout(state->context.device, state->renderer.pipelineLayout, state->config.allocator);
    vkDestroyShaderModule(state->context.device, vertexShaderModule, state->config.allocator);
    vkDestroyShaderModule(state->context.device, fragmentShaderModule, state->config.allocator);
}

void graphicsPipelineDestroy(State *state) {
    vkDestroyPipeline(state->context.device, state->renderer.graphicsPipeline, state->config.allocator);
}

void renderpassCreate(State *state) {
    VkFormat image_format = state->window.swapchain.format;

    VkAttachmentReference color_attachment_references[] = {
        {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        }
    };

    VkSubpassDescription subpass_descriptions[] = {
        {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = color_attachment_references,
        }
    };

    VkAttachmentDescription attachment_descriptions[] = {
        {
            .format = image_format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        }
    };

    EXPECT(vkCreateRenderPass(state->context.device, &(VkRenderPassCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .subpassCount = sizeof(subpass_descriptions)/sizeof(*subpass_descriptions),
        .pSubpasses = (const VkSubpassDescription *) &subpass_descriptions,
        .attachmentCount = sizeof(attachment_descriptions)/sizeof(*attachment_descriptions),
        .pAttachments = attachment_descriptions,
        .pDependencies = &(VkSubpassDependency) {
            .srcSubpass = VK_SUBPASS_EXTERNAL,
            .dstSubpass = 0,

            .srcAccessMask = 0,
            .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,

            .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        }
    }, state->config.allocator, &state->renderer.renderpass), "Couldn't create renderpass")
}

void renderpassDestroy(State *state) {
    vkDestroyRenderPass(state->context.device, state->renderer.renderpass, state->config.allocator);
}

void framebuffersCreate(State *state) {
    uint32_t framebufferCount = state->window.swapchain.imageCount;
    state->renderer.framebuffers = malloc(framebufferCount * sizeof(VkFramebuffer));
    EXPECT(state->renderer.framebuffers == NULL, "Couldn't allocate memory for framebuffers array")
    VkExtent2D framebufferExtent = state->window.swapchain.imageExtent;

    for (int framebufferIndex = 0; framebufferIndex < framebufferCount; ++framebufferIndex) {
        EXPECT(vkCreateFramebuffer(state->context.device, &(VkFramebufferCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .layers = 1,
            .renderPass = state->renderer.renderpass,
            .width = framebufferExtent.width,
            .height = framebufferExtent.height,
            .attachmentCount = 1,
            .pAttachments = &state->window.swapchain.imageViews[framebufferIndex],
        }, state->config.allocator, &state->renderer.framebuffers[framebufferIndex]), "Couldn't create framebuffer %i", framebufferIndex)
    }
}

void framebuffersDestroy(State *state) {
    uint32_t framebuffer_count = state->window.swapchain.imageCount;

    for (int framebuffer_index = 0; framebuffer_index < framebuffer_count; ++framebuffer_index) {
        vkDestroyFramebuffer(state->context.device, state->renderer.framebuffers[framebuffer_index], state->config.allocator);
    }

    free(state->renderer.framebuffers);
}

void commandPoolCreate(State *state) {
    EXPECT(vkCreateCommandPool(state->context.device, &(VkCommandPoolCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .queueFamilyIndex = state->context.queueFamily,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    }, state->config.allocator, &state->renderer.commandPool), "Couldn't create command pool")
}

void commandPoolDestroy(State *state) {
    vkDestroyCommandPool(state->context.device, state->renderer.commandPool, state->config.allocator);
}

void commandBufferAllocate(State *state) {
    EXPECT(vkAllocateCommandBuffers(state->context.device, &(VkCommandBufferAllocateInfo) {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = state->renderer.commandPool,
        .commandBufferCount = 1,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    }, &state->renderer.commandBuffer), "Couldn't allocate command buffer")
}

void syncObjectsCreate(State *state) {
    EXPECT(vkCreateSemaphore(state->context.device, &(VkSemaphoreCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    }, state->config.allocator, &state->renderer.imageAcquiredSemaphore), "Couldn't create image acquired semaphore")

    EXPECT(vkCreateSemaphore(state->context.device, &(VkSemaphoreCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    }, state->config.allocator, &state->renderer.renderFinishedSemaphore), "Couldn't create render finished semaphore")

    EXPECT(vkCreateFence(state->context.device, &(VkFenceCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
    }, state->config.allocator, &state->renderer.inFlightFence), "Couldn't create in-flight fence")
}

void syncObjectsDestroy(State *state) {
    vkDestroyFence(state->context.device, state->renderer.inFlightFence, state->config.allocator);
    vkDestroySemaphore(state->context.device, state->renderer.renderFinishedSemaphore, state->config.allocator);
    vkDestroySemaphore(state->context.device, state->renderer.imageAcquiredSemaphore, state->config.allocator);
}

void commandBufferRecord(State *state) {
    VkCommandBuffer commandBuffer = state->renderer.commandBuffer;
    uint32_t imageAcquiredIndex = state->window.swapchain.imageAcquiredIndex;

    EXPECT(vkBeginCommandBuffer(commandBuffer, &(VkCommandBufferBeginInfo) {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    }), "Couldn't begin command buffer for frame")

    VkClearValue clearValues[] = {
            state->config.backgroundColor,
    };

    vkCmdBeginRenderPass(state->renderer.commandBuffer, &(VkRenderPassBeginInfo) {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = state->renderer.renderpass,
        .framebuffer = state->renderer.framebuffers[imageAcquiredIndex],
        .renderArea = (VkRect2D) {
            .extent = state->window.swapchain.imageExtent,
        },
        .clearValueCount = 1,
        .pClearValues = clearValues,
    }, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, state->renderer.graphicsPipeline);
    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    EXPECT(vkEndCommandBuffer(commandBuffer), "Couldn't end command buffer")
}

void commandBufferSubmit(State *state) {
    EXPECT(vkQueueSubmit(state->context.queue, 1, &(VkSubmitInfo) {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &state->renderer.commandBuffer,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &state->renderer.imageAcquiredSemaphore,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = &state->renderer.renderFinishedSemaphore,
        .pWaitDstStageMask = (VkPipelineStageFlags[]) {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        }
    }, state->renderer.inFlightFence), "Couldn't submit command buffer")
}

void rendererCreate(State *state) {
    renderpassCreate(state);
    graphicsPipelineCreate(state);
    framebuffersCreate(state);
    commandPoolCreate(state);
    commandBufferAllocate(state);
    syncObjectsCreate(state);
}

void rendererDestroy(State *state) {
    vkQueueWaitIdle(state->context.queue);

    syncObjectsDestroy(state);
    commandPoolDestroy(state);
    framebuffersDestroy(state);
    graphicsPipelineDestroy(state);
    renderpassDestroy(state);
}