#include "vcr_model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <cassert>
#include <cstring>
#include <iostream>

namespace vcr {

Model::Model(Device& device, const Model::Builder& builder) : device{device} {
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}

Model::~Model() {
    vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
    vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
    if (!hasIndexBuffer) return;
    vkDestroyBuffer(device.device(), indexBuffer, nullptr);
    vkFreeMemory(device.device(), indexBufferMemory, nullptr);
}

std::unique_ptr<Model> Model::createModelFromFile(Device& device, const std::string& filePath) {
    Builder builder{};
    builder.loadModel(filePath);
    std::cout << "Vertex Count : " << builder.vertices.size() << "\n";
    return std::make_unique<Model>(device, builder);
}

void Model::createVertexBuffers(const std::vector<Vertex>& vertices) {
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Model must have at least 3 vertices");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    device.createBuffer(bufferSize,
                        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        stagingBuffer,
                        stagingBufferMemory);

    void* data;
    vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.device(), stagingBufferMemory);

    device.createBuffer(bufferSize,
                        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        vertexBuffer,
                        vertexBufferMemory);

    device.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
    // destroy the staging buffer as its only use is we copy data from host to device (staging
    // buffer) then from staging buffer to more optimal memory DEVICE_LOCAL_BIT
    vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
    vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}

void Model::createIndexBuffers(const std::vector<uint32_t>& indices) {
    indexCount = static_cast<uint32_t>(indices.size());
    hasIndexBuffer = indexCount > 0;
    if (!hasIndexBuffer) return;
    VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    device.createBuffer(bufferSize,
                        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                        stagingBuffer,
                        stagingBufferMemory);

    void* data;
    vkMapMemory(device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(device.device(), stagingBufferMemory);

    device.createBuffer(bufferSize,
                        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                        indexBuffer,
                        indexBufferMemory);

    device.copyBuffer(stagingBuffer, indexBuffer, bufferSize);
    vkDestroyBuffer(device.device(), stagingBuffer, nullptr);
    vkFreeMemory(device.device(), stagingBufferMemory, nullptr);
}

void Model::draw(VkCommandBuffer commandBuffer) {
    if (hasIndexBuffer) {
        vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
    } else {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }
}

void Model::bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    if (hasIndexBuffer) {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);
    return attributeDescriptions;
}

void Model::Builder::loadModel(const std::string& filePath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
        throw std::runtime_error("TinyObj failed to load objects ==> " + warn + err);
    }
    vertices.clear();
    indices.clear();

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            if (index.vertex_index >= 0) {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                auto colorIndex = 3 * index.vertex_index + 2;
                if (colorIndex < attrib.colors.size()) {
                    vertex.color = {
                        attrib.colors[colorIndex - 2],
                        attrib.colors[colorIndex - 1],
                        attrib.colors[colorIndex - 0],
                    };
                } else {
                    vertex.color = {1.f, 1.f, 1.f}; // set default color
                }
            }

            if (index.normal_index >= 0) {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
            }

            if (index.texcoord_index >= 0) {
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }
            vertices.push_back(vertex);
        }
    }
}

} // namespace vcr