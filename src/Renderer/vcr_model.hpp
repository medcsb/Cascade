#ifndef VCR_MODEL_HPP
#define VCR_MODEL_HPP

#include "vcr_device.hpp"

#include "vk_utils.hpp"

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <stb_image.h>

#include <vector>
#include <array>

namespace vcr {

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
};

class Model {
private:
    std::vector<Vertex> vertexData;
    std::vector<uint32_t> indices;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    Device &device;

public:
    Model(Device &device);
    ~Model();

    void createVertexBuffer();
    void createIndexBuffer();
    void createTextures(const std::string &filePath);
    
    VkBuffer getVertexBuffer() const {return vertexBuffer;}
    VkBuffer getIndexBuffer() const {return indexBuffer;}
    std::vector<Vertex> getVertexData() const {return vertexData;}
    std::vector<uint32_t> getIndices() const {return indices;}
    VkImage getTextureImage() const {return textureImage;}
    VkImageView getTextureImageView() const {return textureImageView;}
    VkSampler getTextureSampler() const {return textureSampler;}
    
    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
    
private:
    
    void createTextureImage(const std::string &filePath);
    void createTextureImageView();
    void createTextureSampler();

};
}

#endif // VCR_MODEL_HPP