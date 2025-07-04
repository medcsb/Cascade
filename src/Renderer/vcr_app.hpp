#ifndef VCR_APP_HPP
#define VCR_APP_HPP

#include "vcr_window.hpp"
#include "vcr_pipeline.hpp"
#include "vcr_device.hpp"
#include "vcr_swap_chain.hpp"
#include "vcr_model.hpp"
#include "vcr_game_obj.hpp"

#include <memory>
#include <vector>

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;

namespace vcr {
class App {
private:
    Window window{WIDTH, HEIGHT, "Vulkan App"};
    Device device{window};
    std::unique_ptr<SwapChain> swapChain;
    std::unique_ptr<Pipeline> pipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<GameObject> gameObjects;
public:
    App();
    ~App();
    App(const App &) = delete;
    App& operator=(const App &) = delete;
    void run();
private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void freeCommandBuffers();
    void drawFrame();
    void loadGameObjects();
    void recreateSwapChain();
    void recordCommandBuffer(int imgIndex);
    void renderGameObjects(VkCommandBuffer commandBuffer);
};
}

#endif // VCR_APP_HPP