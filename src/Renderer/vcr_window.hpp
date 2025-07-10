#ifndef VCR_WINDOW_HPP
#define VCR_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vcr {

class Window {
private:
    uint32_t width;
    uint32_t height;
    std::string windowName;

    GLFWwindow* window;

public:
    Window(uint32_t width, uint32_t height, const std::string& name);
    ~Window();
    void init();
    GLFWwindow* getWindow() {return window;}
    bool windowShouldClose() {return glfwWindowShouldClose(window);}
    void pollEvents() {glfwPollEvents();}
};
}

#endif // VCR_WINDOW_HPP