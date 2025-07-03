#include "vcr_app.hpp"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main() {
    vcr::App app{};
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}