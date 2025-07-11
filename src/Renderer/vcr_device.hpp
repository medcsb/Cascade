#ifndef VCR_DEVICE_HPP
#define VCR_DEVICE_HPP

#include "vcr_window.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <vector>

// Validation Layers
const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
// Swap chain extensions
const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

namespace vcr {

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
              void* pUserData);

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator,
                                      VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator);

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() const {return graphicsFamily.has_value() && presentFamily.has_value();}
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Device {

private:
    Window& window;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSurfaceKHR surface;
    VkPresentModeKHR presentMode;
    VkCommandPool commandPool;

public:
    Device(Window& window);
    ~Device();

    void init();
    VkDevice getDevice() const {return device;}
    VkPhysicalDevice getPhysicalDevice() const {return physicalDevice;}
    VkSurfaceKHR getSurface() const {return surface;}
    GLFWwindow* getWindow() const {return window.getWindow();}
    VkCommandPool getCommandPool() const {return commandPool;}
    VkQueue getGraphicsQueue() const {return graphicsQueue;}
    VkQueue getPresentQueue() const {return presentQueue;}
    
    static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice, 
                                                VkSurfaceKHR surface);
private:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();

    void removeUnsuitableDevices(std::vector<VkPhysicalDevice>& devices);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    VkPhysicalDevice pickBestPhysicalDevice(const std::vector<VkPhysicalDevice>& devices);
    uint32_t getDeviceScore(VkPhysicalDevice device);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();

    void log() {logChosenPhysicalDevice();}
    void logChosenPhysicalDevice();
    void logExtensionList();
    void logValidationLayers();
};
} // namespace vcr

#endif // VCR_DEVICE_HPP