#ifndef VCR_PIPELINE_HPP
#define VCR_PIPELINE_HPP

#include "vcr_device.hpp"

#include <string>
#include <vector>

namespace vcr {

struct PipelineConfigInfo {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class Pipeline {
private:
    Device& device;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
public:
    Pipeline(const std::string& vertFilePath, const std::string& fragFilePath, Device& device, const PipelineConfigInfo& configInfo);
    ~Pipeline();
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
private:
    static std::vector<char> readFile(const std::string& filePath);
    void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo); 
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
};
}


#endif // VCR_PIPELINE_HPP