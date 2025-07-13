#ifndef VCR_MODEL_HPP
#define VCR_MODEL_HPP

#include "vcr_device.hpp"

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
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkMemoryRequirements memRequirements;

    Device &device;

public:
    Model(Device &device);
    ~Model();

    void createVertexBuffer();

    VkBuffer getVertexBuffer() const {return vertexBuffer;}
    std::vector<Vertex> getVertexData() const {return vertexData;}

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();

private:

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

};
}

#endif // VCR_MODEL_HPP