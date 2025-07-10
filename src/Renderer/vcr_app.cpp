#include "vcr_app.hpp"

namespace vcr {

void App::init() {
    window.init();
    device.init();
    swapChain.createSwapChain();
    renderer.init();
}

void App::run() {
    mainLoop();
}

void App::mainLoop() {
    while (!window.windowShouldClose()) {
        window.pollEvents();
    } 
}

} // namespace vcr