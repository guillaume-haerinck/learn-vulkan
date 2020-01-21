#include "pipeline.h"

#include <fstream>
#include <iostream>
#include <debug_break/debug_break.h>

//////////////////////////////////////////////////////
////////////////// CONSTANT BUFFER ///////////////////
//////////////////////////////////////////////////////

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
}

//////////////////////////////////////////////////////
////////////////////// PIPELINE //////////////////////
//////////////////////////////////////////////////////

Pipeline::Pipeline(LogicalDevice& device) {
    // TODO create shaders and compile them
    auto vertShaderCode = readBinaryFile("shaders/vert.spv");
    auto fragShaderCode = readBinaryFile("shaders/frag.spv");
    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, device);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, device);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    {
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";
    }

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    {
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";
    }

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    // TODO https://vulkan-tutorial.com/en/Drawing_a_triangle/Graphics_pipeline_basics/Fixed_functions

    vkDestroyShaderModule(device.get(), fragShaderModule, nullptr);
    vkDestroyShaderModule(device.get(), vertShaderModule, nullptr);
}

Pipeline::~Pipeline() {
    
}

std::vector<char> Pipeline::readBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "[Pipeline] Failed to open SPIR-V file : " << filename << std::endl;
        debug_break();
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderModule Pipeline::createShaderModule(const std::vector<char>& code, LogicalDevice& device) {
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device.get(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        std::cerr << "[Pipeline] Failed to create shader module" << std::endl;
        debug_break();
    }
    return shaderModule;
}
