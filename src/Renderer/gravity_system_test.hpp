#ifndef GRAVITY_SYSTEM_TEST_HPP
#define GRAVITY_SYSTEM_TEST_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "vcr_2d_obj.hpp"
#include <vector>

namespace vcr {

std::unique_ptr<Model> createSquareModel(Device& device, glm::vec2 offset) {
    std::vector<Model::Vertex> vertices = {
        {{-0.5f, -0.5f}},
        {{0.5f, 0.5f}},
        {{-0.5f, 0.5f}},
        {{-0.5f, -0.5f}},
        {{0.5f, -0.5f}},
        {{0.5f, 0.5f}}, //
    };
    for (auto& v : vertices) {
        v.position += offset;
    }
    return std::make_unique<Model>(device, vertices);
}

std::unique_ptr<Model> createCircleModel(Device& device, unsigned int numSides) {
    std::vector<Model::Vertex> uniqueVertices{};
    for (int i = 0; i < numSides; i++) {
        float angle = i * glm::two_pi<float>() / numSides;
        uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
    }
    uniqueVertices.push_back({}); // adds center vertex at 0, 0

    std::vector<Model::Vertex> vertices{};
    for (int i = 0; i < numSides; i++) {
        vertices.push_back(uniqueVertices[i]);
        vertices.push_back(uniqueVertices[(i + 1) % numSides]);
        vertices.push_back(uniqueVertices[numSides]);
    }
    return std::make_unique<Model>(device, vertices);
}

class GravityPhysicsSystem {
public:
    const float strengthGravity;
    GravityPhysicsSystem(float strength) : strengthGravity(strength) {}

    void update(std::vector<Object2D>& objs, float dt, unsigned int substeps = 1) {
        const float stepDelta = dt / substeps;
        for (size_t i = 0; i < substeps; ++i) {
            stepSimulation(objs, stepDelta);
        }
    }

    glm::vec2 computeForce(const Object2D& fromObj, const Object2D& toObj) const {
        auto offset = fromObj.transform2d.translation - toObj.transform2d.translation;
        float distanceSquared = glm::dot(offset, offset);
        if (distanceSquared < 0.00000001f) {
            return glm::vec2(0.0f);
        }
        float force =
            strengthGravity * toObj.rigidBody2d.mass * fromObj.rigidBody2d.mass / distanceSquared;
        return force * offset / glm::sqrt(distanceSquared);
    }

private:
    void stepSimulation(std::vector<Object2D>& physicsObjs, float dt) {
        for (auto iterA = physicsObjs.begin(); iterA != physicsObjs.end(); ++iterA) {
            auto& objA = *iterA;
            for (auto iterB = iterA; iterB != physicsObjs.end(); ++iterB) {
                if (iterA == iterB) continue; // Skip self
                auto& objB = *iterB;

                auto force = computeForce(objA, objB);
                objA.rigidBody2d.velocity += dt * -force / objA.rigidBody2d.mass;
                objB.rigidBody2d.velocity += dt * force / objB.rigidBody2d.mass;
            }
        }

        for (auto& obj : physicsObjs) {
            obj.transform2d.translation += dt * obj.rigidBody2d.velocity;
        }
    }
};

class Vec2FieldSystem {
public:
    void update(const GravityPhysicsSystem& physicsSystem,
                std::vector<Object2D>& physicsObjs,
                std::vector<Object2D>& vectorField) {
        // For each field line we caluclate the net graviation force for that point in space
        for (auto& vf : vectorField) {
            glm::vec2 direction{};
            for (auto& obj : physicsObjs) {
                direction += physicsSystem.computeForce(obj, vf);
            }

            // This scales the length of the field line based on the log of the length
            // values were chosen just through trial and error based on what i liked the look
            // of and then the field line is rotated to point in the direction of the field
            vf.transform2d.scale.x =
                0.005f + 0.045f * glm::clamp(glm::log(glm::length(direction) + 1) / 3.f, 0.f, 1.f);
            vf.transform2d.rotation = atan2(direction.y, direction.x);
        }
    }
};

} // namespace vcr

#endif // GRAVITY_SYSTEM_TEST_HPP