#ifndef VCR_WINDOW_HPP
#define VCR_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace vcr
{

class Window
{
private:
    int width;
    int height;
    bool framebufferResized = false;

    GLFWwindow* window;
    std::string windowName;

public:
    Window(int width, int height, const std::string& name);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const { return glfwWindowShouldClose(window); }
    VkExtent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
    bool wasWindowResized() { return framebufferResized; }
    GLFWwindow* getGLFWWindow() const { return window; }
    void resetWindowResizedFlag() { framebufferResized = false; }

    void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

private:
    void initWindow();
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};
} // namespace vcr

#endif // VCR_WINDOW_HPP