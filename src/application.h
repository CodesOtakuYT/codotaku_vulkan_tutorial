#pragma once

void init(State *state) {
    errorHandlingSetup();
    infoLog();

    glfwInit();

    contextCreate(state);
    windowCreate(state);
    rendererCreate(state);
}

void loop(State *state) {
    while(!windowShouldClose(state)) {
        double frameTimeStart = glfwGetTime();
        windowPollEvents(state);
        swapchainImageAcquire(state);
        commandBufferRecord(state);
        commandBufferSubmit(state);
        swapchainImagePresent(state);
        double frameTimeEnd = glfwGetTime();
        double frameTime = frameTimeEnd-frameTimeStart;
        double frameFrequency = 1/frameTime;
        static char fpsString[8];
        snprintf(fpsString, 8, "FPS: %f\n", frameFrequency);
        glfwSetWindowTitle(state->window.handle, fpsString);
    }
}

void cleanup(State *state) {
    rendererDestroy(state);
    windowDestroy(state);
    contextDestroy(state);
}