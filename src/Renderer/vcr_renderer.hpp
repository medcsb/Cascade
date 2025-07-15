#ifndef VCR_RENDERER_HPP
#define VCR_RENDERER_HPP

#include "vcr_window.hpp"
#include "vcr_device.hpp"
#include "vcr_swapchain.hpp"
#include "vcr_pipeline.hpp"
#include "vcr_camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <thread>

const uint32_t width = 800;
const uint32_t height = 600;
const std::string name = "test App";

namespace vcr {

struct UniformBufferObject {
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view;
    glm::mat4 proj;
};

class Renderer {
private:
    std::chrono::high_resolution_clock::time_point currentTime;
    UniformBufferObject ubo;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    VkDescriptorSetLayout descriptorSetLayout;
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkRenderPass renderPass;
    std::vector<VkFramebuffer> framebuffers;
    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentFrame = 0;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    Window window{width, height, name};
    Device device{window};
    SwapChain swapChain{device, window};
    Model model{device};
    Pipeline pipeline{device, model};
    Camera camera;
public:
    Renderer();
    ~Renderer();

    void init();
    void run();
private:
    void mainLoop();
    void drawFrame();

    void createRenderPass();
    void createCommandBuffers();
    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
    void createSyncObjects();
    void createDescriptorSetLayout();
    void createDescriptorPool();
    void createDescriptorSets();
    void createUniformBuffers();
    void updateUniformBuffer(uint32_t currentImage);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
};

}

#endif // VCR_RENDERER_HPP