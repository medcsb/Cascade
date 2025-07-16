#ifndef KEYBOARD_MOVEMENT_CONTROLLER_HPP
#define KEYBOARD_MOVEMENT_CONTROLLER_HPP

#include "vcr_window.hpp"
#include "vcr_camera.hpp"

namespace vcr {

struct KeyMapping {
    int moveForward = GLFW_KEY_W;
    int moveBackward = GLFW_KEY_S;
    int moveLeft = GLFW_KEY_A;
    int moveRight = GLFW_KEY_D;
    int moveUp = GLFW_KEY_SPACE;
    int moveDown = GLFW_KEY_LEFT_ALT;
    int toggleFPSMode = GLFW_KEY_F;
    int rotateCamLeft = GLFW_KEY_Q;
    int rotateCamRight = GLFW_KEY_E;
    int leftArrow = GLFW_KEY_LEFT;
    int rightArrow = GLFW_KEY_RIGHT;
    int upArrow = GLFW_KEY_UP;
    int downArrow = GLFW_KEY_DOWN;
};

class KeyboardMovementController {
private:
    Window& window;
    Camera& camera;

    bool fpsMode = false;
    bool fpsKeyPressed = false;
    bool firstMouse = true;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;
    
    float moveSpeed{3.0f};
    float lookSpeed{0.5f};
    float mouseSensitivity{0.0009f};
    float rotationSpeed{0.01f};

    KeyMapping keyMapping{};

public:
    KeyboardMovementController(Window& window, Camera& camera);
    ~KeyboardMovementController();

    void processInput(float dt);
};
}

#endif // KEYBOARD_MOVEMENT_CONTROLLER_HPP