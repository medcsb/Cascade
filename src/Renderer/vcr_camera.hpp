#ifndef VCR_CAMERA_HPP
#define VCR_CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vcr {

class Camera {

private:
    glm::mat4 projectionMatrix{1.0f};
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 inverseViewMatrix{1.0f};

public:
    Camera();
    ~Camera();

    void setPerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
    void setViewDirection(const glm::vec3& position, const glm::vec3& direction, const glm::vec3& up);

    const glm::mat4& getProjectionMatrix() const {return projectionMatrix;}
    const glm::mat4& getViewMatrix() const {return viewMatrix;}
    const glm::mat4& getInverseViewMatrix() const {return inverseViewMatrix;}

};
}

#endif // VCR_CAMERA_HPP