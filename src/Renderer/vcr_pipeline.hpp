#ifndef VCR_PIEPLINE_HPP
#define VCR_PIEPLINE_HPP

#include "vcr_device.hpp"
#include "vcr_swapchain.hpp"
#include "vcr_model.hpp"
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
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkPipelineDynamicStateCreateInfo dynamicState;
    VkPipelineVertexInputStateCreateInfo vertexInputState;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
    VkPipelineViewportStateCreateInfo viewportState;
    VkPipelineRasterizationStateCreateInfo rasterizationState;
    VkPipelineMultisampleStateCreateInfo multisampleState;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendState;
    VkPipelineDepthStencilStateCreateInfo depthStencilState;
    VkVertexInputBindingDescription vertexBindingDescription;
    std::array<VkVertexInputAttributeDescription, 3> vertexAttributeDescriptions;

    void setSamples(VkSampleCountFlagBits samples) {msaaSamples = samples;}
};

class Pipeline {
private:
    Device &device;
    Model& model;
    VkExtent2D extent;

    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;

    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
public:
    Pipeline(Device &device, Model& model);
    ~Pipeline();

    VkPipeline getGraphicsPipeline() const {return graphicsPipeline;}
    VkPipelineLayout getPipelineLayout() const {return pipelineLayout;}

    void setExtent(const VkExtent2D &extent) {this->extent = extent;}

    void createGraphicsPipeline(VkRenderPass& renderPass,
                                VkDescriptorSetLayout& descriptorSetLayout,
                                const std::string& vertShaderPath,
                                const std::string& fragShaderPath);
    std::array<VkShaderModule, 2> createShaderModules(
        const std::string &vertexShaderPath,
        const std::string &fragmentShaderPath);

    static PipelineConfig defaultPipelineConfig(PipelineConfig &configInfo);
private:
    VkShaderModule createShaderModule(const std::vector<char> &code);
    static std::array<VkPipelineShaderStageCreateInfo, 2> createShaderStages(VkShaderModule& vertShaderModule,
                                                                           VkShaderModule& fragShaderModule);
    static void createDynamicState(PipelineConfig &configInfo);
    static void createVertexInputState(PipelineConfig &configInfo);
    static void createInputAssemblyState(PipelineConfig &configInfo);
    static void createViewportState(PipelineConfig &configInfo);
    static void createRasterizationState(PipelineConfig &configInfo);
    static void createMultisampleState(PipelineConfig &configInfo);
    // TODO : Move ColorBlendAttachment and ColorBlendState to one function
    static void createColorBlendAttachment(PipelineConfig &configInfo);
    static void createColorBlendState(PipelineConfig &configInfo);
    static void createDepthStencilState(PipelineConfig &configInfo);
    static VkPipelineLayout createPipelineLayout(Device& device, VkDescriptorSetLayout& descriptorSetLayout);
};
}

#endif // VCR_PIEPLINE_HPP