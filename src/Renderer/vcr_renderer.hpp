#ifndef VCR_RENDERER_HPP
#define VCR_RENDERER_HPP

#include "vcr_window.hpp"

namespace vcr {

class Renderer {

private:
    Window& window;

public:
    Renderer(Window& window);
    ~Renderer();

    void init();
private:
};

}

#endif // VCR_RENDERER_HPP