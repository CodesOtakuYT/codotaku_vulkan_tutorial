cmake_minimum_required(VERSION 3.23)
project(codotaku_vulkan_tutorial C)

set(CMAKE_C_STANDARD 17)

add_executable(codotaku_vulkan_tutorial src/main.c)
target_link_libraries(codotaku_vulkan_tutorial glfw vulkan)