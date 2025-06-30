#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <stdexcept>
#include <vector>

void initGLFW() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    if (!glfwVulkanSupported()) {
        throw std::runtime_error("GLFW: Vulkan not supported");
    }
}

GLFWwindow* createWindow() {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // No OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan Test", nullptr, nullptr);
    if (!window) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    return window;
}

VkInstance createVulkanInstance() {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "CascadeTest";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "CascadeEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Get required extensions from GLFW
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;

    VkInstance instance;
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }
    return instance;
}

int main() {
    try {
        // Test glm
        glm::vec3 testVec(1.0f, 2.0f, 3.0f);
        std::cout << "GLM vector: (" << testVec.x << ", " << testVec.y << ", " << testVec.z << ")\n";

        // Init GLFW + Vulkan
        initGLFW();
        GLFWwindow* window = createWindow();
        VkInstance instance = createVulkanInstance();

        std::cout << "Vulkan instance created successfully.\n";

        // Simple loop
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }

        // Cleanup
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();

        std::cout << "Clean exit.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}