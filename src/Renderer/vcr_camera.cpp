#include "vcr_camera.hpp"


namespace vcr {

Camera::Camera() {}
Camera::~Camera() {}

void Camera::setPerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane) {
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    projectionMatrix[1][1] *= -1; // Vulkan uses a different coordinate system
}

void Camera::setViewDirection(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up) {
    viewMatrix = glm::lookAt(position, position + direction, up);
    inverseViewMatrix = glm::inverse(viewMatrix);
}

}