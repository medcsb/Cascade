#ifndef VCR_SWAPCHAIN_HPP
#define VCR_SWAPCHAIN_HPP

#include "vcr_device.hpp"

namespace vcr {

class SwapChain {
private:
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;

    uint32_t imageCount = 0;

    std::vector<VkImage> swapChainImages;

    Device &device;
    Window &window;

public:
    SwapChain(Window &window, Device &device);
    ~SwapChain();

    void createSwapChain();
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
private:

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void log() {
        logSwapPresentMode();
    }
    void logSwapPresentMode();
    std::string presentModeToString(VkPresentModeKHR mode) {
        switch (mode) {
            case VK_PRESENT_MODE_IMMEDIATE_KHR: return "Immediate";
            case VK_PRESENT_MODE_MAILBOX_KHR: return "Mailbox";
            case VK_PRESENT_MODE_FIFO_KHR: return "FIFO";
            case VK_PRESENT_MODE_FIFO_RELAXED_KHR: return "FIFO Relaxed";
            default: return "Unknown";
        }
    }
};
}

#endif // VCR_SWAPCHAIN_HPP