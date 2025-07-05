#ifndef VCR_RENDERER_HPP
#define VCR_RENDERER_HPP

#include "vcr_device.hpp"
#include "vcr_swap_chain.hpp"
#include "vcr_window.hpp"

// std
#include <cassert>
#include <memory>
#include <vector>

namespace vcr {
class Renderer {
private:

    Window& window;
    Device& device;
    std::unique_ptr<SwapChain> swapChain;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentImageIndex;
    int currentFrameIndex;
    bool isFrameStarted;

public:
    Renderer(Window& window, Device& device);
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); }
    bool isFrameInProgress() const { return isFrameStarted; }

    VkCommandBuffer getCurrentCommandBuffer() const {
        assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
        return commandBuffers[currentFrameIndex];
    }

    int getFrameIndex() const {
        assert(isFrameStarted && "Cannot get frame index when frame not in progress");
        return currentFrameIndex;
    }

    VkCommandBuffer beginFrame();
    void endFrame();
    void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
    void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
    void createCommandBuffers();
    void freeCommandBuffers();
    void recreateSwapChain();
};
} // namespace vcr

#endif // VCR_RENDERER_HPP