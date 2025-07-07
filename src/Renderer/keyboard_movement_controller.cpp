#include "keyboard_movement_controller.hpp"

// std
#include <limits>

namespace vcr {

void KeyboardMovementController::moveInPlaneXZ(GLFWwindow* window, float dt, Object& object) {
    // Toggle FPS mode
    bool fpsKeyCurrentlyPressed = glfwGetKey(window, keys.toggleFPS) == GLFW_PRESS;
    if (fpsKeyCurrentlyPressed && !fpsKeyPressed) {
        fpsMode = !fpsMode;
        if (fpsMode) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    fpsKeyPressed = fpsKeyCurrentlyPressed;

    // Handle rotation based on mode
    glm::vec3 rotate{0};
    
    if (fpsMode) {
        // Mouse look for FPS mode
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        if (firstMouse) {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstMouse = false;
        }
        
        double deltaX = mouseX - lastMouseX;
        double deltaY = lastMouseY - mouseY; // Reversed since y-coordinates go from bottom to top
        
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        
        rotate.y += static_cast<float>(deltaX) * mouseSensitivity;
        rotate.x += static_cast<float>(deltaY) * mouseSensitivity;
    } else {
        // Arrow key look for normal mode
        if (glfwGetKey(window, keys.lookRight) == GLFW_PRESS) rotate.y += 1.f;
        if (glfwGetKey(window, keys.lookLeft) == GLFW_PRESS) rotate.y -= 1.f;
        if (glfwGetKey(window, keys.lookUp) == GLFW_PRESS) rotate.x += 1.f;
        if (glfwGetKey(window, keys.lookDown) == GLFW_PRESS) rotate.x -= 1.f;
        
        if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
            rotate = lookSpeed * dt * glm::normalize(rotate);
        }
    }

    // Apply rotation
    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        object.transform.rotation += rotate;
    }

    // limit pitch values between about +/- 85ish degrees
    object.transform.rotation.x = glm::clamp(object.transform.rotation.x, -1.5f, 1.5f);
    object.transform.rotation.y = glm::mod(object.transform.rotation.y, glm::two_pi<float>());

    float yaw = object.transform.rotation.y;
    const glm::vec3 forwardDir{sin(yaw), 0.f, cos(yaw)};
    const glm::vec3 rightDir{forwardDir.z, 0.f, -forwardDir.x};
    const glm::vec3 upDir{0.f, -1.f, 0.f};

    glm::vec3 moveDir{0.f};
    if (glfwGetKey(window, keys.moveForward) == GLFW_PRESS) moveDir += forwardDir;
    if (glfwGetKey(window, keys.moveBackward) == GLFW_PRESS) moveDir -= forwardDir;
    if (glfwGetKey(window, keys.moveRight) == GLFW_PRESS) moveDir += rightDir;
    if (glfwGetKey(window, keys.moveLeft) == GLFW_PRESS) moveDir -= rightDir;
    if (glfwGetKey(window, keys.moveUp) == GLFW_PRESS) moveDir += upDir;
    if (glfwGetKey(window, keys.moveDown) == GLFW_PRESS) moveDir -= upDir;

    if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon()) {
        object.transform.translation += moveSpeed * dt * glm::normalize(moveDir);
    }
}
} // namespace vcr