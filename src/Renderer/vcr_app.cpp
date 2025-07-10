#include "vcr_app.hpp"

namespace vcr {

void App::run() {
    renderer.init();
    renderer.run();
}

} // namespace vcr