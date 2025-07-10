#ifndef VCR_APP_HPP
#define VCR_APP_HPP

#include "vcr_renderer.hpp"
#include "vcr_window.hpp"
#include "vcr_device.hpp"
#include "vcr_swapchain.hpp"

const uint32_t width = 800;
const uint32_t height = 600;
const std::string name = "test App";

namespace vcr {
class App {
private:
    Window window{width, height, name};
    Device device{window};
    SwapChain swapChain{window, device};
    Renderer renderer{window};
public:
    void init();
    void run();
private:
    void mainLoop();
};
}

#endif // VCR_APP_HPP