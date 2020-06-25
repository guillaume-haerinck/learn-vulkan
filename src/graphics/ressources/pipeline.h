#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

#include "graphics/setup/devices.h"
#include "graphics/output/swap-chain.h"
#include "graphics/ressources/buffers.h"
#include "graphics/ressources/descriptors.h"

class Pipeline {
public:
    Pipeline(LogicalDevice& device, SwapChain& swapChain, DescriptorPool& descriptorPool, MemoryAllocator& memoryAllocator);
    ~Pipeline();

    std::vector<vk::Framebuffer>& getFrameBuffers() { return m_swapChainFramebuffers; }
    vk::RenderPass& getRenderPass() { return m_renderPass; }
    vk::Pipeline& get() { return m_graphicsPipeline; }
    vk::PipelineLayout& getLayout() { return m_pipelineLayout.getLayout(); }
    vk::DescriptorSet& getDescriptorSet(unsigned int index) { return m_descriptorSets.get(index); }

    void updateUniformBuffer(unsigned int imageIndex);

private:
    std::vector<char> readBinaryFile(const std::string& filename);
    vk::ShaderModule createShaderModule(const std::vector<char>& code, LogicalDevice& device);

private:
    LogicalDevice& m_device;
    PipelineLayout m_pipelineLayout;
    UniformBuffer m_uniformBuffer;
    DescriptorSets m_descriptorSets;
    vk::RenderPass m_renderPass;
    vk::Pipeline m_graphicsPipeline;
    std::vector<vk::Framebuffer> m_swapChainFramebuffers;
    VertexInputDescription m_vbDescription;
};
