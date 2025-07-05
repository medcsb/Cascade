#ifndef SIMPLE_RENDER_SYSTEM_HPP
#define SIMPLE_RENDER_SYSTEM_HPP

#include "vcr_device.hpp"
#include "vcr_2d_obj.hpp"
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

    void renderObject2Ds(VkCommandBuffer commandBuffer, std::vector<Object2D>& objects2D);

private:
    void createPipelineLayout();
    void createPipeline(VkRenderPass renderPass);

    Device& device;

    std::unique_ptr<Pipeline> pipeline;
    VkPipelineLayout pipelineLayout;
};
} // namespace vcr

#endif // SIMPLE_RENDER_SYSTEM_HPP