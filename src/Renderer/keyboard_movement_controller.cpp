#include "keyboard_movement_controller.hpp"

#include <limits>

namespace vcr {

KeyboardMovementController::KeyboardMovementController(Window& window, Camera& camera) :
    window(window),
    camera(camera) {}
KeyboardMovementController::~KeyboardMovementController() {}

void KeyboardMovementController::processInput(float dt) {
    bool fpsKeyCurrentlyPressed = glfwGetKey(window.getWindow(), keyMapping.toggleFPSMode) == GLFW_PRESS;
    if (fpsKeyCurrentlyPressed && !fpsKeyPressed) {
        fpsMode = !fpsMode;
        if (fpsMode) {
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            firstMouse = true;
        } else {
            glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    fpsKeyPressed = fpsKeyCurrentlyPressed;

    glm::vec3 rotate{0.0};
    if (fpsMode) {
        double mouseX, mouseY;
        glfwGetCursorPos(window.getWindow(), &mouseX, &mouseY);

        if (firstMouse) {
            lastMouseX = mouseX;
            lastMouseY = mouseY;
            firstMouse = false;
        }

        double delataX = mouseX - lastMouseX;
        double delataY = mouseY - lastMouseY;

        lastMouseX = mouseX;
        lastMouseY = mouseY;

        rotate.x += static_cast<float>(delataY) * mouseSensitivity;
        rotate.y += static_cast<float>(delataX) * mouseSensitivity;
    }

    // apply rotation
    if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon()) {
        glm::vec3 position = glm::vec3(camera.getInverseViewMatrix()[3]);
        glm::vec3 forward = glm::normalize(-glm::vec3(camera.getInverseViewMatrix()[2]));
        glm::vec3 right = glm::normalize(glm::vec3(camera.getInverseViewMatrix()[0]));
        glm::vec3 up = glm::normalize(glm::cross(right, forward));

        // apply pitvh and yaw
        glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), -rotate.y, up);
        glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), -rotate.x, right);
        
        glm::vec3 newDirection = glm::normalize(glm::vec3(yawRotation * pitchRotation * glm::vec4(forward, 0.0f)));
        camera.setViewDirection(position, newDirection, up);
    }

    glm::vec3 forwardDirection = glm::normalize(-glm::vec3(camera.getInverseViewMatrix()[2]));
    glm::vec3 rightDirection = glm::normalize(glm::vec3(camera.getInverseViewMatrix()[0]));
    glm::vec3 upDirection = glm::normalize(glm::cross(rightDirection, forwardDirection));

    glm::vec3 moveDirection{0.0f};
    if (glfwGetKey(window.getWindow(), keyMapping.moveForward) == GLFW_PRESS) moveDirection += forwardDirection;
    if (glfwGetKey(window.getWindow(), keyMapping.moveBackward) == GLFW_PRESS) moveDirection -= forwardDirection;
    if (glfwGetKey(window.getWindow(), keyMapping.moveLeft) == GLFW_PRESS) moveDirection -= rightDirection;
    if (glfwGetKey(window.getWindow(), keyMapping.moveRight) == GLFW_PRESS) moveDirection += rightDirection;
    if (glfwGetKey(window.getWindow(), keyMapping.moveUp) == GLFW_PRESS) moveDirection += upDirection;
    if (glfwGetKey(window.getWindow(), keyMapping.moveDown) == GLFW_PRESS) moveDirection -= upDirection;

    // apply movement
    if (glm::dot(moveDirection, moveDirection) > std::numeric_limits<float>::epsilon()) {
        moveDirection = glm::normalize(moveDirection) * moveSpeed * dt;
        glm::vec3 position = glm::vec3(camera.getInverseViewMatrix()[3]);
        position += moveDirection;
        camera.setViewDirection(position, forwardDirection, upDirection);
    }
}
}