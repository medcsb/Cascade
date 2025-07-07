#include "vcr_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "simple_render_system.hpp"
#include "vcr_buffer.hpp"
#include "vcr_camera.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <numeric>
#include <stdexcept>

const std::string vertShaderPath = "shaders/shader.vert.spv";
const std::string fragShaderPath = "shaders/shader.frag.spv";

namespace vcr {

struct GlobalUbo {
    glm::mat4 projectionView{1.0f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3{1.0f, -3.0f, -1.0f});
};

App::App() {
    globalPool =
        DescriptorPool::Builder(device)
            .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
    loadObjects();
}
App::~App() {
    globalPool = nullptr;
}

void App::run() {
    std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
    for (size_t i = 0; i < uboBuffers.size(); i++) {
        uboBuffers[i] = std::make_unique<Buffer>(device,
                                                 sizeof(GlobalUbo),
                                                 1,
                                                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        uboBuffers[i]->map();
    }

    auto globalSetLayout =
        DescriptorSetLayout::Builder(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

    std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
        auto bufferInfo = uboBuffers[i]->descriptorInfo();
        DescriptorWriter(*globalSetLayout, *globalPool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);
    }

    SimpleRenderSystem simpleRenderSystem{
        device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
    Camera camera{};

    Object viewerObject = Object::createObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();

    while (!window.shouldClose()) {
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        float frameTime =
            std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime)
                .count();
        currentTime = newTime;

        cameraController.moveInPlaneXZ(window.getGLFWWindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = renderer.getAspectRatio();
        camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);
        // camera.setOrthographicProjection(-aspect, aspect, -1.0f, 1.0f, -1.0f, 10.0f);
        if (auto commandBuffer = renderer.beginFrame()) {
            int frameIndex = renderer.getFrameIndex();
            FrameInfo frameInfo{
                frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};
            // update
            GlobalUbo ubo{};
            ubo.projectionView = camera.getProjectionMatrix() * camera.getViewMatrix();
            uboBuffers[frameIndex]->writeToBuffer(&ubo);
            uboBuffers[frameIndex]->flush();
            // render
            renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderObjects(frameInfo, objects, frameTime);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadObjects() {
    std::shared_ptr<Model> model =
        Model::createModelFromFile(device, "assets/models/smooth_vase.obj");
    Object obj3D = Object::createObject();
    obj3D.model = model;
    obj3D.transform.translation = glm::vec3(0.0f, 0.0f, 2.5f);
    obj3D.transform.scale = glm::vec3(1.0f, -1.0f, 1.0f); // Flip the model vertically
    objects.push_back(std::move(obj3D));
}

} // namespace vcr