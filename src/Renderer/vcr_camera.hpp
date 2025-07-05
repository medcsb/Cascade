#ifndef VCR_CAMERA_HPP
#define VCR_CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace vcr {

class Camera {
private:
    glm::mat4 projectionMatrix{1.0f};
    
public:
    void setOrthographicProjection(float left,
                                   float right,
                                   float top,
                                   float bottom,
                                   float near,
                                   float far);
    void setPerspectiveProjection(float fovY, float aspect, float near, float far);
    const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
};
} // namespace vcr

#endif // VCR_CAMERA_HPP