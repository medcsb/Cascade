#ifndef FILE_UTILS_HPP
#define FILE_UTILS_HPP


#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>


namespace vcr {


static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

}

#endif // FILE_UTILS_HPP