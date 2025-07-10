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
    Window window{width, height, name};
    Device device{window};
    SwapChain swapChain{device};
    Pipeline pipeline{device};

    VkRenderPass renderPass;

public:
    Renderer();
    ~Renderer();

    void init();
    void run();
private:
    void mainLoop();
    void createRenderPass();
};

}

#endif // VCR_RENDERER_HPP