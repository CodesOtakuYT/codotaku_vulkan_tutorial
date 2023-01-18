#include "toolkit.h"

int main() {
    State state = {
            .config = {
                    .applicationName = "CODOTAKU",
                    .engineName = "CODOTAKU ENGINE",
                    .windowTitle = "CODOTAKU",
                    .windowWidth = 720,
                    .windowHeight = 480,
                    .windowFullscreen = false,
                    .apiVersion = VK_API_VERSION_1_3,
                    .swapchainComponentsMapping = (VkComponentMapping) {},
                    .swapchainBuffering = SWAPCHAIN_TRIPLE_BUFFERING,
                    .backgroundColor = {{0.01f, 0.01f, 0.01f, 0.01f}}
            },
    };

    init(&state);
    loop(&state);
    cleanup(&state);

    return EXIT_SUCCESS;
}
