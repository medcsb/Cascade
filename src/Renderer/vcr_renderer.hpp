#ifndef VCR_RENDERER_HPP
#define VCR_RENDERER_HPP

#include "vcr_window.hpp"
#include "vcr_device.hpp"
#include "vcr_swapchain.hpp"
#include "vcr_pipeline.hpp"

const uint32_t width = 800;
const uint32_t height = 600;
const std::string name = "test App";

namespace vcr {

class Renderer {
private:
    const int MAX_FRAMES_IN_FLIGHT = 2;

    VkRenderPass renderPass;
    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentFrame = 0;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    Window window{width, height, name};
    Device device{window};
    SwapChain swapChain{device};
    Pipeline pipeline{device};
public:
    Renderer();
    ~Renderer();

    void init();
    void run();
private:
    void mainLoop();
    void drawFrame();

    void createRenderPass();
    void createFramebuffers();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
};

}

#endif // VCR_RENDERER_HPP