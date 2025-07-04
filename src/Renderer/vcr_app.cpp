#include "vcr_app.hpp"


#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <array>
#include <stdexcept>

const std::string vertShaderPath = "shaders/shader.vert.spv";
const std::string fragShaderPath = "shaders/shader.frag.spv";

void sirpinski(
    std::vector<vcr::Model::Vertex> &vertices,
    uint32_t depth,
    glm::vec2 top,
    glm::vec2 left,
    glm::vec2 right) {
    if (depth == 0) {
        vertices.push_back({top});
        vertices.push_back({left});
        vertices.push_back({right});
        return;
    }
    glm::vec2 midLeft = (top + left) / 2.0f;
    glm::vec2 midRight = (top + right) / 2.0f;
    glm::vec2 midBottom = (left + right) / 2.0f;
    sirpinski(vertices, depth - 1, top, midLeft, midRight);
    sirpinski(vertices, depth - 1, midLeft, left, midBottom);
    sirpinski(vertices, depth - 1, midRight, midBottom, right);
}

namespace vcr {

struct SimplePushConstantData {
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

App::App() {
    loadModels();
    createPipelineLayout();
    recreateSwapChain();
    createCommandBuffers();
}

App::~App() {
  vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void App::run() {
  while (!window.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }
  vkDeviceWaitIdle(device.device());
}

void App::drawFrame() {
  uint32_t imageIndex;
  VkResult result = swapChain->acquireNextImage(&imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }
  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  recordCommandBuffer(imageIndex);
  result = swapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized()) {
    window.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }
  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

void App::createPipelineLayout() {
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;            
    pipelineLayoutInfo.pSetLayouts = nullptr;         
    pipelineLayoutInfo.pushConstantRangeCount = 1;    
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; 
    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void App::createPipeline() {
    PipelineConfigInfo pipelineConfig{};
    Pipeline::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = swapChain->getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    pipeline = std::make_unique<Pipeline>(vertShaderPath, fragShaderPath,
                                        device, pipelineConfig);
}

void App::createCommandBuffers() {
  commandBuffers.resize(swapChain->imageCount());
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = device.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(device.device(), &allocInfo,
                               commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void App::freeCommandBuffers() {
    vkFreeCommandBuffers(device.device(),
                        device.getCommandPool(),
                        static_cast<uint32_t>(commandBuffers.size()),
                        commandBuffers.data());
    commandBuffers.clear();
}

void App::recreateSwapChain() {
    auto extent = window.getExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = window.getExtent();
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(device.device());
    if (swapChain == nullptr) {
        swapChain = std::make_unique<SwapChain>(device, extent);
    } else {
        swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));
        if (swapChain->imageCount() != commandBuffers.size()) {
            freeCommandBuffers();
            createCommandBuffers();
        }
    }
    createPipeline();
}

void App::recordCommandBuffer(int imgIndex) {
    static int frame = 0;
    frame = (frame + 1) % 1000;

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (vkBeginCommandBuffer(commandBuffers[imgIndex], &beginInfo) != VK_SUCCESS) {
      throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = swapChain->getRenderPass();
    renderPassInfo.framebuffer = swapChain->getFrameBuffer(imgIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imgIndex],
                        &renderPassInfo,
                        VK_SUBPASS_CONTENTS_INLINE);
    
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imgIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imgIndex], 0, 1, &scissor);

    pipeline->bind(commandBuffers[imgIndex]);
    model->bind(commandBuffers[imgIndex]);

    for (int j=0; j<4; j++) {
        SimplePushConstantData push{};
        push.offset = {-0.5f + frame * 0.001f, -0.5f + j * 0.25f};
        push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

        vkCmdPushConstants(commandBuffers[imgIndex],
                           pipelineLayout,
                           VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                           0,
                           sizeof(SimplePushConstantData),
                           &push);
        model->draw(commandBuffers[imgIndex]);
    }
    vkCmdEndRenderPass(commandBuffers[imgIndex]);
    if (vkEndCommandBuffer(commandBuffers[imgIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void App::loadModels() {
    std::vector<Model::Vertex> vertices = {
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    //std::vector<Model::Vertex> vertices;
    //sirpinski(vertices, 5, {-0.7f, 0.7f}, {0.7f, 0.7f}, {0.0f, -0.7f});

    model = std::make_unique<Model>(device, vertices);
}
} // namespace vcr