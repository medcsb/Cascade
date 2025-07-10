#ifndef VCR_PIEPLINE_HPP
#define VCR_PIEPLINE_HPP

#include "vcr_device.hpp"
#include "vcr_swapchain.hpp"
#include "file_utils.hpp"

#include <iostream>
#include <fstream>
#include <array>

namespace vcr {

class Pipeline {

struct PipelineConfig {
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkPipelineDynamicStateCreateInfo dynamicState;
    VkPipelineVertexInputStateCreateInfo vertexInputState;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
    VkPipelineViewportStateCreateInfo viewportState;
    VkPipelineRasterizationStateCreateInfo rasterizationState;
    VkPipelineMultisampleStateCreateInfo multisampleState;
    VkPipelineColorBlendStateCreateInfo colorBlendState;
    VkPipelineLayout pipelineLayout;
};

private:
    Device &device;
    VkExtent2D extent;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
public:
    Pipeline(Device &device);
    ~Pipeline();

    void setExtent(const VkExtent2D &extent) {this->extent = extent;}

    void createGraphicsPipeline();
    void createShaderModules(const std::string &vertexShaderPath,
                             const std::string &fragmentShaderPath);
private:
    VkShaderModule createShaderModule(const std::vector<char> &code);
    static std::array<VkPipelineShaderStageCreateInfo, 2> createShaderStages(VkShaderModule vertShaderModule,
                                                                           VkShaderModule fragShaderModule);
    static VkPipelineDynamicStateCreateInfo createDynamicState();
    static VkPipelineVertexInputStateCreateInfo createVertexInputState();
    static VkPipelineInputAssemblyStateCreateInfo createInputAssemblyState();
    static VkPipelineViewportStateCreateInfo createViewportState(VkExtent2D extent);
    static VkPipelineRasterizationStateCreateInfo createRasterizationState();
    static VkPipelineMultisampleStateCreateInfo createMultisampleState();
    static VkPipelineColorBlendStateCreateInfo createColorBlendState();
    static VkPipelineLayout createPipelineLayout(Device& device);
};
}

#endif // VCR_PIEPLINE_HPP