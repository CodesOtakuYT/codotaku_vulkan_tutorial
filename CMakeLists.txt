cmake_minimum_required(VERSION 3.23)
project(codotaku_vulkan_tutorial C)

set(CMAKE_C_STANDARD 17)

add_executable(codotaku_vulkan_tutorial src/main.c src/utils.h src/toolkit.h src/headers.h src/context.h src/renderer.h src/window.h src/application.h)
target_link_libraries(codotaku_vulkan_tutorial glfw vulkan)