#ifndef KEYBOARD_MOVEMENT_CONTROLLER_HPP
#define KEYBOARD_MOVEMENT_CONTROLLER_HPP

#include "vcr_window.hpp"
#include "vcr_camera.hpp"

namespace vcr {

const float NORMAL_MOVE_SPEED = 3.0f;
const float NORMAL_LOOK_SPEED = 0.5f;
const float NORMAL_MOUSE_SENSITIVITY = 0.0009f;
const float NORMAL_ROTATION_SPEED = 0.01f;

const float SLOW_MOVE_SPEED = 0.5f;
const float SLOW_LOOK_SPEED = 0.2f;
const float SLOW_MOUSE_SENSITIVITY = 0.0006f;
const float SLOW_ROTATION_SPEED = 0.0025f;

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
    int shift = GLFW_KEY_LEFT_SHIFT;
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
    
    float moveSpeed{NORMAL_MOVE_SPEED};
    float lookSpeed{NORMAL_LOOK_SPEED};
    float mouseSensitivity{NORMAL_MOUSE_SENSITIVITY};
    float rotationSpeed{NORMAL_ROTATION_SPEED};

    KeyMapping keyMapping{};

public:
    KeyboardMovementController(Window& window, Camera& camera);
    ~KeyboardMovementController();

    void processInput(float dt);
};
}

#endif // KEYBOARD_MOVEMENT_CONTROLLER_HPP