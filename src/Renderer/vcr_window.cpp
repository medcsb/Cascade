#include "vcr_window.hpp"

#include <stdexcept>

namespace vcr {

Window::Window(int width, int height, const std::string& name) :
    width(width), height(height), windowName(name) {
    initWindow();
}

Window::~Window() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}

void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto vcrWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    vcrWindow->framebufferResized = true;
    vcrWindow->width = width;
    vcrWindow->height = height;
}
}