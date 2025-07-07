#ifndef KEYBOARD_MOVEMENT_CONTROLLER_HPP
#define KEYBOARD_MOVEMENT_CONTROLLER_HPP

#include "vcr_obj.hpp"
#include "vcr_window.hpp"

namespace vcr {
class KeyboardMovementController {
public:
    struct KeyMappings {
        int moveLeft = GLFW_KEY_A;
        int moveRight = GLFW_KEY_D;
        int moveForward = GLFW_KEY_W;
        int moveBackward = GLFW_KEY_S;
        int moveUp = GLFW_KEY_SPACE;
        int moveDown = GLFW_KEY_LEFT_CONTROL;
        int lookLeft = GLFW_KEY_LEFT;
        int lookRight = GLFW_KEY_RIGHT;
        int lookUp = GLFW_KEY_UP;
        int lookDown = GLFW_KEY_DOWN;
        int toggleFPS = GLFW_KEY_F;
    };

    void moveInPlaneXZ(GLFWwindow* window, float dt, Object& object);

    KeyMappings keys{};
    float moveSpeed{3.f};
    float lookSpeed{2.5f};
    float mouseSensitivity{0.0009f}; // Mouse sensitivity for FPS mode
    
private:
    bool fpsMode = false;
    bool fpsKeyPressed = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
};
} // namespace vcr

#endif // KEYBOARD_MOVEMENT_CONTROLLER_HPP