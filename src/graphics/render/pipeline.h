#pragma once

#include <vulkan/vulkan.hpp>
#include <string>
#include <vector>

#include "swap-chain.h"
#include "render-pass.h"
#include "graphics/setup/devices.h"
#include "graphics/ressources/buffers.h"
#include "graphics/ressources/descriptors.h"
#include "graphics/ressources/images.h"

class Pipeline {
public:
    Pipeline(LogicalDevice& device, SwapChain& swapChain, DescriptorPool& descriptorPool, MemoryAllocator& memoryAllocator);
    ~Pipeline();

    std::vector<vk::Framebuffer>& getFrameBuffers() { return m_framebuffer.get(); }
    vk::RenderPass& getRenderPass() { return m_framebuffer.getRenderPass(); }
    vk::Pipeline& get() { return m_graphicsPipeline; }
    vk::PipelineLayout& getLayout() { return m_pipelineLayout.get(); }
    vk::DescriptorSet& getDescriptorSet(unsigned int index) { return m_descriptorSets.get(index); }

    void updateUniformBuffer(unsigned int imageIndex, const glm::mat4x4& viewProj);

private:
    std::vector<char> readBinaryFile(const std::string& filename);
    vk::ShaderModule createShaderModule(const std::vector<char>& code, LogicalDevice& device);

private:
    LogicalDevice& m_device;

    FrameBuffer m_framebuffer;
    UniformBuffer m_uniformBuffer;
    PipelineLayout m_pipelineLayout;
    DescriptorSets m_descriptorSets;
    VertexInputDescription m_vbDescription;

    vk::Pipeline m_graphicsPipeline;
};
