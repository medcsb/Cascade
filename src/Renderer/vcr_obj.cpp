#include "vcr_obj.hpp"

namespace vcr {

glm::mat4 TransformComponent::mat4() {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);
    transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, scale);
    return transform;
}

glm::mat3 TransformComponent::normalMatrix() {
    // Normal matrix is the inverse transpose of the upper-left 3x3 part of the model matrix
    glm::mat4 modelMatrix = mat4();
    return glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
}
} // namespace vcr