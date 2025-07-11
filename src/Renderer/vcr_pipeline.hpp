#ifndef VCR_PIEPLINE_HPP
#define VCR_PIEPLINE_HPP

#include "vcr_device.hpp"
#include "vcr_swapchain.hpp"
#include "file_utils.hpp"

#include <iostream>
#include <fstream>
#include <array>

namespace vcr {

struct PipelineConfig {
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkPipelineDynamicStateCreateInfo dynamicState;
    VkPipelineVertexInputStateCreateInfo vertexInputState;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineViewportStateCreateInfo viewportState;
    VkPipelineRasterizationStateCreateInfo rasterizationState;
    VkPipelineMultisampleStateCreateInfo multisampleState;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendState;
    VkPipelineLayout pipelineLayout;
    VkRenderPass renderPass;
};

class Pipeline {
private:
    Device &device;
    VkExtent2D extent;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline;
public:
    Pipeline(Device &device);
    ~Pipeline();

    VkPipeline getGraphicsPipeline() const {return graphicsPipeline;}

    void setExtent(const VkExtent2D &extent) {this->extent = extent;}

    void createGraphicsPipeline(VkRenderPass renderPass);
    std::array<VkShaderModule, 2> createShaderModules(
        const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath);

    static PipelineConfig defaultPipelineConfig(Device &device,
                                                VkExtent2D& extent,
                                                VkShaderModule& vertShaderModule,
                                                VkShaderModule& fragShaderModule,
                                                VkPipelineLayout& pipelineLayout,
                                                VkRenderPass& renderPass);
    static VkViewport createViewport(const VkExtent2D &extent);
    static VkRect2D createScissor(const VkExtent2D &extent);
private:
    VkShaderModule createShaderModule(const std::vector<char> &code);
    static std::array<VkPipelineShaderStageCreateInfo, 2> createShaderStages(VkShaderModule vertShaderModule,
                                                                           VkShaderModule fragShaderModule);
    static VkPipelineDynamicStateCreateInfo createDynamicState(std::vector<VkDynamicState>& dynamicStates);
    static VkPipelineVertexInputStateCreateInfo createVertexInputState();
    static VkPipelineInputAssemblyStateCreateInfo createInputAssemblyState();
    static VkPipelineViewportStateCreateInfo createViewportState(VkViewport& viewport, VkRect2D& scissor);
    static VkPipelineRasterizationStateCreateInfo createRasterizationState();
    static VkPipelineMultisampleStateCreateInfo createMultisampleState();
    static VkPipelineColorBlendAttachmentState createColorBlendAttachment();
    static VkPipelineColorBlendStateCreateInfo createColorBlendState(
    const VkPipelineColorBlendAttachmentState& colorBlendAttachment);
    static VkPipelineLayout createPipelineLayout(Device& device);
};
}

#endif // VCR_PIEPLINE_HPP