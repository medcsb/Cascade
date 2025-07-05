#ifndef VCR_MODEL_HPP
#define VCR_MODEL_HPP

#include "vcr_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace vcr {
class Model {
private:
    Device& device;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;

public:
    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    Model(Device& device, const std::vector<Vertex>& vertices);
    ~Model();
    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex>& vertices);
};
} // namespace vcr

#endif // VCR_MODEL_HPP