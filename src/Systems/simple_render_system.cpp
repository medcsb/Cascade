#include "simple_render_system.hpp"



SimpleRenderSystem::SimpleRenderSystem(vcr::Renderer &renderer) : renderer(renderer) {
    renderer.init();
}

SimpleRenderSystem::~SimpleRenderSystem() {}

void SimpleRenderSystem::drawFrame() {
    
}