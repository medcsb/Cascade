#ifndef VCR_APP_HPP
#define VCR_APP_HPP

#include "vcr_renderer.hpp"
#include "vcr_window.hpp"
#include "vcr_device.hpp"
#include "vcr_swapchain.hpp"

namespace vcr {
class App {
private:
    Renderer renderer;
public:
    void run();
private:
};
}

#endif // VCR_APP_HPP