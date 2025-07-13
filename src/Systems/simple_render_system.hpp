#ifndef SIMPLE_RENDER_SYSTEM_HPP
#define SIMPLE_RENDER_SYSTEM_HPP

#include "vcr_renderer.hpp"

class SimpleRenderSystem {

private:
    vcr::Renderer &renderer;
    

public:
    SimpleRenderSystem(vcr::Renderer &renderer);
    ~SimpleRenderSystem();

    void drawFrame();
};

#endif // SIMPLE_RENDER_SYSTEM_HPP