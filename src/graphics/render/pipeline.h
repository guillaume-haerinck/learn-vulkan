#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

#include "graphics/setup/devices.h"
#include "graphics/output/swap-chain.h"

//////////////////////////////////////////////////////
////////////////// CONSTANT BUFFER ///////////////////
//////////////////////////////////////////////////////

class ConstantBuffer {
public:
    ConstantBuffer();
    ~ConstantBuffer();

private:
};

//////////////////////////////////////////////////////
////////////////////// PIPELINE //////////////////////
//////////////////////////////////////////////////////

class Pipeline {
public:
    Pipeline(LogicalDevice& device, SwapChain& swapChain);
    ~Pipeline();

private:
    std::vector<char> readBinaryFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code, LogicalDevice& device);

private:
    LogicalDevice& m_device;
    VkPipelineLayout m_pipelineLayout;
    VkRenderPass m_renderPass; // Temp, move to frame-buffer ?
    VkPipeline m_graphicsPipeline;
};
