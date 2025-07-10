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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}
} // namespace vcr