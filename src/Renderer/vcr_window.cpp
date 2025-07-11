#include "vcr_window.hpp"

namespace vcr {

Window::Window(uint32_t width, uint32_t height, const std::string& name)
    : width(width), height(height), windowName(name) {}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::init() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto vcrWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    vcrWindow->framebufferResized = true;
}

} // namespace vcr