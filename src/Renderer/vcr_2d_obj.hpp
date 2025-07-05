#ifndef VCR_2D_OBJ_HPP
#define VCR_2D_OBJ_HPP

#include "vcr_model.hpp"

#include <memory>

namespace vcr {

struct RigidBody2dComponent {
    glm::vec2 velocity;
    float mass{1.0f};
};

struct Transform2dComponent {
    glm::vec2 translation{};
    glm::vec2 scale{1.0f, 1.0f};
    float rotation;

    glm::mat2 mat2() {
        const float sin = glm::sin(rotation);
        const float cos = glm::cos(rotation);
        glm::mat2 rotMat = {{cos, sin}, {-sin, cos}};
        glm::mat2 scaleMat = {{scale.x, 0.0f}, {0.0f, scale.y}};
        // return rotMat * scaleMat;
        return rotMat * scaleMat;
    }
};

class Object2D {
public:
    using id_t = unsigned int;

    Object2D(const Object2D&) = delete;
    Object2D& operator=(const Object2D&) = delete;
    Object2D(Object2D&&) = default;
    Object2D& operator=(Object2D&&) = default;

    static Object2D createObject2D() {
        static id_t currentId = 0;
        return Object2D(currentId++);
    }

    id_t getId() const { return id; }

    std::shared_ptr<Model> model{};
    glm::vec3 color{};
    Transform2dComponent transform2d{};
    RigidBody2dComponent rigidBody2d{};

private:
    Object2D(id_t objId) : id(objId) {}

    id_t id;
};
} // namespace vcr

#endif // VCR_2D_OBJ_HPP