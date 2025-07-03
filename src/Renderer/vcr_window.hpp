#ifndef VCR_WINDOW_HPP
#define VCR_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vcr {

class Window {
private:
    GLFWwindow* window;
    const int width;
    const int height;

    std::string windowName;

public:
    Window(int width, int height, const std::string& name);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const { return glfwWindowShouldClose(window); }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
    VkExtent2D getExtent() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

private:
    void initWindow();
};
}

#endif // VCR_WINDOW_HPP