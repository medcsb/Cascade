#include "vcr_app.hpp"
#include "gravity_system_test.hpp"
#include "simple_render_system.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <glm/gtc/constants.hpp>

#include <array>
#include <stdexcept>

const std::string vertShaderPath = "shaders/shader.vert.spv";
const std::string fragShaderPath = "shaders/shader.frag.spv";

namespace vcr {

std::vector<Object2D> createPhysicsObjects(Device& device) {
    // create some models
    std::shared_ptr<Model> squareModel = createSquareModel(
        device,
        {.5f, .0f}); // offset model by .5 so rotation occurs at edge rather than center of square
    std::shared_ptr<Model> circleModel = createCircleModel(device, 64);

    // create physics objects
    std::vector<Object2D> physicsObjects{};
    auto red = Object2D::createObject2D();
    red.transform2d.scale = glm::vec2{.05f};
    red.transform2d.translation = {.5f, .5f};
    red.color = {1.f, 0.f, 0.f};
    red.rigidBody2d.velocity = {-.5f, .0f};
    red.model = circleModel;
    physicsObjects.push_back(std::move(red));
    auto blue = Object2D::createObject2D();
    blue.transform2d.scale = glm::vec2{.05f};
    blue.transform2d.translation = {-.45f, -.25f};
    blue.color = {0.f, 0.f, 1.f};
    blue.rigidBody2d.velocity = {.5f, .0f};
    blue.model = circleModel;
    physicsObjects.push_back(std::move(blue));

    return physicsObjects;
}

std::vector<Object2D> createVectorField(Device& device) {
    std::shared_ptr<Model> squareModel = createSquareModel(
        device,
        {.5f, .0f}); // offset model by .5 so rotation occurs at edge rather than center of square
    // create vector field
    std::vector<Object2D> vectorField{};
    int gridCount = 40;
    for (int i = 0; i < gridCount; i++) {
        for (int j = 0; j < gridCount; j++) {
            auto vf = Object2D::createObject2D();
            vf.transform2d.scale = glm::vec2(0.005f);
            vf.transform2d.translation = {-1.0f + (i + 0.5f) * 2.0f / gridCount,
                                          -1.0f + (j + 0.5f) * 2.0f / gridCount};
            vf.color = glm::vec3(1.0f);
            vf.model = squareModel;
            vectorField.push_back(std::move(vf));
        }
    }
    return vectorField;
}

App::App() {
    loadObject2Ds();
}
App::~App() {
}

void App::run() {

    std::vector<Object2D> physicsObjects = createPhysicsObjects(device);
    std::vector<Object2D> vectorField = createVectorField(device);

    GravityPhysicsSystem gravitySystem{0.81f};
    Vec2FieldSystem vecFieldSystem{};

    SimpleRenderSystem simpleRenderSystem{device, renderer.getSwapChainRenderPass()};

    while (!window.shouldClose()) {
        glfwPollEvents();
        if (auto commandBuffer = renderer.beginFrame()) {
            // simple test of gravity system
            gravitySystem.update(physicsObjects, 1.f / 60.f, 5);
            vecFieldSystem.update(gravitySystem, physicsObjects, vectorField);

            renderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderObject2Ds(commandBuffer, physicsObjects);
            simpleRenderSystem.renderObject2Ds(commandBuffer, vectorField);
            renderer.endSwapChainRenderPass(commandBuffer);
            renderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void App::loadObject2Ds() {
    std::vector<Model::Vertex> vertices{{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                                        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
    auto model = std::make_shared<Model>(device, vertices);

    auto triangle = Object2D::createObject2D();
    triangle.model = model;
    triangle.color = {.1f, .8f, .1f};
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale = {2.f, .5f};
    triangle.transform2d.rotation = .25f * glm::two_pi<float>();

    objects2D.push_back(std::move(triangle));
}

} // namespace vcr