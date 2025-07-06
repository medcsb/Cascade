#include "vcr_app.hpp"
#include "keyboard_movement_controller.hpp"
#include "simple_render_system.hpp"
#include "vcr_camera.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/constants.hpp>

#include <array>
#include <chrono>
#include <stdexcept>

const std::string vertShaderPath = "shaders/shader.vert.spv";
const std::string fragShaderPath = "shaders/shader.frag.spv";

namespace vcr {

App::App() {
    loadObjects();
}
App::~App() {
}

void App::run() {
    SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};
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
        //camera.setOrthographicProjection(-aspect, aspect, -1.0f, 1.0f, -1.0f, 10.0f);
        if (auto commandBuffer = renderer.beginFrame()) {
            renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderObjects(commandBuffer, objects, camera, frameTime);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadObjects() {
    std::shared_ptr<Model> model = Model::createModelFromFile(device, "assets/models/dragon.obj");
    Object obj3D = Object::createObject();
    obj3D.model = model;
    obj3D.transform.translation = glm::vec3(0.0f, 0.0f, 2.5f);
    obj3D.transform.scale = glm::vec3(1.0f, -1.0f, 1.0f); // Flip the model vertically
    objects.push_back(std::move(obj3D));
}

} // namespace vcr