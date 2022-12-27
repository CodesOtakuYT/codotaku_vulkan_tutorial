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
        windowPollEvents(state);
    }
}

void cleanup(State *state) {
    rendererDestroy(state);
    windowDestroy(state);
    contextDestroy(state);
}