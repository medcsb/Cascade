#include "vcr_pipeline.hpp"

namespace vcr {

Pipeline::Pipeline(Device &device) : device(device) {}

Pipeline::~Pipeline() {
    vkDestroyPipeline(device.getDevice(), graphicsPipeline, nullptr);
    if (pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device.getDevice(), pipelineLayout, nullptr);
    }
    vkDestroyShaderModule(device.getDevice(), vertShaderModule, nullptr);
    vkDestroyShaderModule(device.getDevice(), fragShaderModule, nullptr);
}

void Pipeline::createGraphicsPipeline(VkRenderPass& renderPass,
                                      const std::string& vertShaderPath,
                                      const std::string& fragShaderPath) {
    PipelineConfig config{};
    Pipeline::defaultPipelineConfig(config);

    auto shaderModules = createShaderModules(vertShaderPath, fragShaderPath);
    vertShaderModule = shaderModules[0];
    fragShaderModule = shaderModules[1];

    auto shaderStages = createShaderStages(vertShaderModule, fragShaderModule);

    pipelineLayout = createPipelineLayout(device);

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &config.vertexInputState;
    pipelineInfo.pInputAssemblyState = &config.inputAssemblyState;
    pipelineInfo.pViewportState = &config.viewportState;
    pipelineInfo.pRasterizationState = &config.rasterizationState;
    pipelineInfo.pMultisampleState = &config.multisampleState;
    pipelineInfo.pColorBlendState = &config.colorBlendState;
    pipelineInfo.pDynamicState = &config.dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(device.getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

std::array<VkShaderModule, 2> Pipeline::createShaderModules(
    const std::string &vertexShaderPath,
    const std::string &fragmentShaderPath) {
    auto vertShaderCode = readFile(vertexShaderPath);
    auto fragShaderCode = readFile(fragmentShaderPath);

    vertShaderModule = createShaderModule(vertShaderCode);
    fragShaderModule = createShaderModule(fragShaderCode);

    return {vertShaderModule, fragShaderModule};
}

VkShaderModule Pipeline::createShaderModule(const std::vector<char> &code) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}

std::array<VkPipelineShaderStageCreateInfo, 2> Pipeline::createShaderStages(VkShaderModule& vertShaderModule,
                                                                           VkShaderModule& fragShaderModule) {
    std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {};

    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = vertShaderModule;
    shaderStages[0].pName = "main";

    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = fragShaderModule;
    shaderStages[1].pName = "main";

    return shaderStages;
}

void Pipeline::createDynamicState(PipelineConfig &configInfo) {
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(configInfo.dynamicStates.size());
    dynamicState.pDynamicStates = configInfo.dynamicStates.data();
    configInfo.dynamicState = dynamicState;
}

void Pipeline::createVertexInputState(PipelineConfig &configInfo) {
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0; // No vertex bindings
    vertexInputInfo.vertexAttributeDescriptionCount = 0; // No vertex attributes
    configInfo.vertexInputState = vertexInputInfo;
}

void Pipeline::createInputAssemblyState(PipelineConfig &configInfo) {
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    configInfo.inputAssemblyState = inputAssembly;
}

void Pipeline::createViewportState(PipelineConfig &configInfo) {
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    configInfo.viewportState = viewportState;
}

void Pipeline::createRasterizationState(PipelineConfig &configInfo) {
    VkPipelineRasterizationStateCreateInfo rasterizationState{};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.lineWidth = 1.0f;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizationState.depthBiasEnable = VK_FALSE;
    configInfo.rasterizationState = rasterizationState;
}

void Pipeline::createMultisampleState(PipelineConfig &configInfo) {
    VkPipelineMultisampleStateCreateInfo multisampleState{};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.sampleShadingEnable = VK_FALSE;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    configInfo.multisampleState = multisampleState;
}

void Pipeline::createColorBlendAttachment(PipelineConfig &configInfo) {
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    configInfo.colorBlendAttachment = colorBlendAttachment;
}

void Pipeline::createColorBlendState(PipelineConfig &configInfo) {
    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &configInfo.colorBlendAttachment;
    colorBlendState.blendConstants[0] = 0.0f;
    colorBlendState.blendConstants[1] = 0.0f;
    colorBlendState.blendConstants[2] = 0.0f;
    colorBlendState.blendConstants[3] = 0.0f;
    configInfo.colorBlendState = colorBlendState;
}

VkPipelineLayout Pipeline::createPipelineLayout(Device& device) {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    VkPipelineLayout pipelineLayout;
    if (vkCreatePipelineLayout(device.getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }
    return pipelineLayout;
}

PipelineConfig Pipeline::defaultPipelineConfig(PipelineConfig &configInfo) {
    configInfo.dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    createDynamicState(configInfo);
    createVertexInputState(configInfo);
    createInputAssemblyState(configInfo);
    createViewportState(configInfo);
    createRasterizationState(configInfo);
    createMultisampleState(configInfo);
    createColorBlendAttachment(configInfo);
    createColorBlendState(configInfo);
    
    return configInfo;
}

}