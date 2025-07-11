#ifndef VCR_SWAPCHAIN_HPP
#define VCR_SWAPCHAIN_HPP

#include "vcr_device.hpp"

namespace vcr {

class SwapChain {
private:
    VkSwapchainKHR swapChain;
    VkFormat swapChainImageFormat;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkImage> swapChainImages;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;

    uint32_t imageCount = 0;

    Window& window;
    Device &device;

public:
    SwapChain(Device &device, Window &window);
    ~SwapChain();

    void init();
    void createFramebuffers(VkRenderPass &renderPass);
    void recreateSwapChain(VkRenderPass &renderPass);

    VkExtent2D getExtent() const {return extent;}
    VkFormat getImageFormat() const {return swapChainImageFormat;}
    uint32_t getImageCount() const {return imageCount;}
    std::vector<VkImageView> getImageViews() const {return swapChainImageViews;}
    VkSwapchainKHR getSwapChain() const {return swapChain;}
    std::vector<VkFramebuffer> getFramebuffers() const {return swapChainFramebuffers;}
    bool isFramebufferResized() const {return window.isFramebufferResized();}

    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
private:

    void createSwapChain();
    void createImageViews();

    void cleanupSwapChain();

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