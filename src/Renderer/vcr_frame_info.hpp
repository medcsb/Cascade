#ifndef VCR_FRAME_INFO_HPP
#define VCR_FRAME_INFO_HPP

#include "vcr_camera.hpp"

#include <vulkan/vulkan.h>

namespace vcr {
struct FrameInfo {
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera& camera;
    VkDescriptorSet globalDescriptorSet;
};
}

#endif // VCR_FRAME_INFO_HPP