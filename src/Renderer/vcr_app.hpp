#ifndef VCR_APP_HPP
#define VCR_APP_HPP

#include "vcr_device.hpp"
#include "vcr_obj.hpp"
#include "vcr_renderer.hpp"
#include "vcr_window.hpp"
#include "vcr_descriptor.hpp"

#include <memory>
#include <vector>

static constexpr int WIDTH = 800;
static constexpr int HEIGHT = 600;

namespace vcr {
class App {
private:
    Window window{WIDTH, HEIGHT, "Vulkan App"};
    Device device{window};
    Renderer renderer{window, device};

    std::unique_ptr<DescriptorPool> globalPool{};
    std::vector<Object> objects;
    
public:
    App();
    ~App();
    App(const App &) = delete;
    App& operator=(const App &) = delete;
    void run();
private:
    void loadObjects();
};
}

#endif // VCR_APP_HPP