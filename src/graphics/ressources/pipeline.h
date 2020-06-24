#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

#include "graphics/setup/devices.h"
#include "graphics/output/swap-chain.h"
#include "graphics/ressources/buffer.h"

class Pipeline {
public:
    Pipeline(LogicalDevice& device, SwapChain& swapChain);
    ~Pipeline();

    std::vector<vk::Framebuffer>& getFrameBuffers() { return m_swapChainFramebuffers; }
    vk::RenderPass& getRenderPass() { return m_renderPass; }
    vk::Pipeline& get() { return m_graphicsPipeline; }

private:
    std::vector<char> readBinaryFile(const std::string& filename);
    vk::ShaderModule createShaderModule(const std::vector<char>& code, LogicalDevice& device);

private:
    LogicalDevice& m_device;
    vk::PipelineLayout m_pipelineLayout;
    vk::RenderPass m_renderPass;
    vk::Pipeline m_graphicsPipeline;
    std::vector<vk::Framebuffer> m_swapChainFramebuffers;
    VertexInputDescription m_vbDescription;
};
