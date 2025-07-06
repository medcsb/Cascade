#ifndef VCR_OBJ_HPP
#define VCR_OBJ_HPP

#include "vcr_model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

namespace vcr {

struct TransformComponent {
    glm::vec3 translation{};
    glm::vec3 scale{1.0f, 1.0f, 1.0f};
    glm::vec3 rotation{};

    // Matrix corresponds to translate * Rx * Ry * Rz * scale transformation
    // Rotation convention uses tait-bryan angles with axis order of X, Y, Z
    glm::mat4 mat4();
    glm::mat3 normalMatrix();
};

class Object {
public:
    using id_t = unsigned int;

    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) = default;
    Object& operator=(Object&&) = default;

    static Object createObject() {
        static id_t currentId = 0;
        return Object(currentId++);
    }

    id_t getId() const { return id; }

    std::shared_ptr<Model> model{};
    glm::vec3 color{};
    TransformComponent transform{};

private:
    Object(id_t objId) : id(objId) {}

    id_t id;
};
} // namespace vcr

#endif // VCR_OBJ_HPP