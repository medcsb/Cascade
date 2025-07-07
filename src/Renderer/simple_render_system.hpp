#ifndef SIMPLE_RENDER_SYSTEM_HPP
#define SIMPLE_RENDER_SYSTEM_HPP

#include "vcr_camera.hpp"
#include "vcr_device.hpp"
#include "vcr_frame_info.hpp"
#include "vcr_obj.hpp"
#include "vcr_pipeline.hpp"

// std
#include <memory>
#include <vector>

namespace vcr {
class SimpleRenderSystem {
public:
    SimpleRenderSystem(Device& device, VkRenderPass renderPass);
    ~SimpleRenderSystem();

    SimpleRenderSystem(const SimpleRenderSystem&) = delete;
    SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

    void renderObjects(FrameInfo& frameInfo, std::vector<Object>& objects, float dt);

private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    Device& device;

    std::unique_ptr<Pipeline> pipeline;
    VkPipelineLayout pipelineLayout;
};
} // namespace vcr

#endif // SIMPLE_RENDER_SYSTEM_HPP