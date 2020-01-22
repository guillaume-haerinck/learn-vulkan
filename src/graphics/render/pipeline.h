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

    std::vector<VkFramebuffer>& getFrameBuffers() { return m_swapChainFramebuffers; }
    VkRenderPass& getRenderPass() { return m_renderPass; }
    VkPipeline& get() { return m_graphicsPipeline; }

private:
    std::vector<char> readBinaryFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code, LogicalDevice& device);

private:
    LogicalDevice& m_device;
    VkPipelineLayout m_pipelineLayout;
    VkRenderPass m_renderPass;
    VkPipeline m_graphicsPipeline;
    std::vector<VkFramebuffer> m_swapChainFramebuffers;
};
