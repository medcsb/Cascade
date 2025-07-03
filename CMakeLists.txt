# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(Cascade LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# === Dependencies ===

# Vulkan-Headers
add_subdirectory(extern/Vulkan-Headers)

# Vulkan-Loader (links to Vulkan-Headers internally)
add_subdirectory(extern/Vulkan-Loader)

# GLFW (disable examples, tests, docs for faster build)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Disable GLFW examples")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Disable GLFW docs")
add_subdirectory(extern/glfw)

# GLM (header-only — no build)
add_subdirectory(extern/glm EXCLUDE_FROM_ALL)

# === Your app ===
file(GLOB RENDERER_SOURCES src/Renderer/*.cpp)
file(GLOB APP_SOURCES src/App/*.cpp)
add_executable(cascade_engine
    ${RENDERER_SOURCES}
    ${APP_SOURCES}
    # add your other source files here
)

# Include dirs (your includes + GLM if needed explicitly)
target_include_directories(cascade_engine PRIVATE
    extern/Vulkan-Headers/include
    extern/glm
    src/Renderer
    src/App
)

# Link libraries
target_link_libraries(cascade_engine PRIVATE
    vulkan
    glfw
)