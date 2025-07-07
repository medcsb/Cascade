#ifndef VCR_CAMERA_HPP
#define VCR_CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace vcr {

class Camera {
private:
    glm::mat4 projectionMatrix{1.0f};
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 inverseViewMatrix{1.0f};

public:
    void setOrthographicProjection(float left,
                                   float right,
                                   float top,
                                   float bottom,
                                   float near,
                                   float far);
    void setPerspectiveProjection(float fovY, float aspect, float near, float far);

    void setViewDirection(const glm::vec3& position,
                          const glm::vec3& direction,
                          const glm::vec3& up = glm::vec3(0.0f, -1.0f, 0.0f));
    void setViewTarget(const glm::vec3& position,
                       const glm::vec3& target,
                       const glm::vec3& up = glm::vec3(0.0f, -1.0f, 0.0f));
    void setViewYXZ(const glm::vec3& position, const glm::vec3& rotation);

    const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
    const glm::mat4& getViewMatrix() const { return viewMatrix; }
    const glm::mat4& getInverseViewMatrix() const { return inverseViewMatrix; }
};
} // namespace vcr

#endif // VCR_CAMERA_HPP