#ifndef VCR_MODEL_HPP
#define VCR_MODEL_HPP

#include "vcr_device.hpp"

#include "vk_buffer_utils.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include <vector>
#include <array>

namespace vcr {

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
};

class Model {
private:
    std::vector<Vertex> vertexData;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    VkMemoryRequirements memRequirements;

    Device &device;

public:
    Model(Device &device);
    ~Model();

    void createVertexBuffer();
    void createIndexBuffer();

    VkBuffer getVertexBuffer() const {return vertexBuffer;}
    VkBuffer getIndexBuffer() const {return indexBuffer;}
    std::vector<Vertex> getVertexData() const {return vertexData;}
    std::vector<uint32_t> getIndices() const {return indices;}

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();

private:


};
}

#endif // VCR_MODEL_HPP