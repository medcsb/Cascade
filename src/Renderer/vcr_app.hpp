#ifndef VCR_APP_HPP
#define VCR_APP_HPP

#include "vcr_window.hpp"
#include "vcr_pipeline.hpp"
#include "vcr_device.hpp"
#include "vcr_swap_chain.hpp"

#include <memory>
#include <vector>

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;

namespace vcr {
class App {
private:
    Window window{WIDTH, HEIGHT, "Vulkan App"};
    Device device{window};
    SwapChain swapChain{device, window.getExtent()};
    std::unique_ptr<Pipeline> pipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
public:
    App();
    ~App();
    App(const App &) = delete;
    void operator=(const App &) = delete;
    void run();
private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();
};
}

#endif // VCR_APP_HPP