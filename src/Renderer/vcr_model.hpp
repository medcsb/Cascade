#ifndef VCR_MODEL_HPP
#define VCR_MODEL_HPP

#include "vcr_device.hpp"

#include "vk_buffer_utils.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <stb_image.h>

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
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;

    Device &device;

public:
    Model(Device &device);
    ~Model();

    void createVertexBuffer();
    void createIndexBuffer();
    void createTextureImage(const std::string &filePath);

    VkBuffer getVertexBuffer() const {return vertexBuffer;}
    VkBuffer getIndexBuffer() const {return indexBuffer;}
    std::vector<Vertex> getVertexData() const {return vertexData;}
    std::vector<uint32_t> getIndices() const {return indices;}

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();

private:

    void createImage(uint32_t width, 
                     uint32_t height, 
                     VkFormat format, 
                     VkImageTiling tiling, 
                     VkImageUsageFlags usage, 
                     VkMemoryPropertyFlags properties, 
                     VkImage &image, 
                     VkDeviceMemory &imageMemory);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

};
}

#endif // VCR_MODEL_HPP