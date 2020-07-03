#include "pipeline.h"

#include <fstream>
#include <iostream>
#include <debug_break/debug_break.h>

Pipeline::Pipeline(LogicalDevice& device, 
    SwapChain& swapChain, 
    DescriptorPool& descriptorPool, 
    MemoryAllocator& memoryAllocator,
    Sampler& sampler,
    TextureImageView& texture
)
    : m_device(device), m_pipelineLayout(device), 
      m_uniformBuffer(device, memoryAllocator, swapChain.getImageViews().size()),
      m_descriptorSets(device, descriptorPool, 
          m_pipelineLayout.getDescriptorSetLayout(), 
          swapChain.getImageViews().size(), 
          m_uniformBuffer, sampler, texture
      ),
      m_framebuffer(device, swapChain, memoryAllocator)
{
    // Shader stages
    vk::PipelineShaderStageCreateInfo shaderStages[2];
    {
        std::vector<char> vertShaderCode = readBinaryFile("res/shaders/test_vs.spv");
        vk::PipelineShaderStageCreateInfo vertShaderStageInfo(
            vk::PipelineShaderStageCreateFlags(),
            vk::ShaderStageFlagBits::eVertex,
            createShaderModule(vertShaderCode, device),
            "main"
        );
        shaderStages[0] = vertShaderStageInfo;

        std::vector<char> fragShaderCode = readBinaryFile("res/shaders/test_ps.spv");
        vk::PipelineShaderStageCreateInfo fragShaderStageInfo(
            vk::PipelineShaderStageCreateFlags(),
            vk::ShaderStageFlagBits::eFragment,
            createShaderModule(fragShaderCode, device),
            "main"
        );
        shaderStages[1] = fragShaderStageInfo;
    }

    // Vertex input
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
        vk::PipelineVertexInputStateCreateFlags(), 
        1, // bindingDescriptionCount
        &m_vbDescription.getBindingDescription(),
        m_vbDescription.getAttributeDescriptions().size(), 
        m_vbDescription.getAttributeDescriptions().data()
    );

    // Input assembly
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly(vk::PipelineInputAssemblyStateCreateFlags(), vk::PrimitiveTopology::eTriangleList, false);

    // Viewport
    auto viewport = vk::Viewport(0.0f, 0.0f, (float) swapChain.getExtent().width, (float) swapChain.getExtent().height, 0.0f, 1.0f);
    auto scisor = vk::Rect2D(vk::Offset2D(0.0f, 0.0f), swapChain.getExtent());
    vk::PipelineViewportStateCreateInfo viewportState(vk::PipelineViewportStateCreateFlags(), 1, &viewport, 1, &scisor);

    // Rasterizer
    vk::PipelineRasterizationStateCreateInfo rasterizer(
        vk::PipelineRasterizationStateCreateFlags(),
        false,  // depthClamp
        false,  // rasterizerDiscard
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack,
        vk::FrontFace::eCounterClockwise,
        false,  // depthBias
        0.0f,   // depthBiasConstantFactor
        0.0f,   // depthBiasClamp
        0.0f,   // depthBiasSlopeFactor
        1.0f    // lineWidth
    );

    // Multisampling
    vk::PipelineMultisampleStateCreateInfo multisampling(
        vk::PipelineMultisampleStateCreateFlags(), vk::SampleCountFlagBits::e1, false, 1.0f, nullptr, false, false
    );

    // Color blending
    auto attachement = vk::PipelineColorBlendAttachmentState(
        false,  // blend
        vk::BlendFactor::eOne, // srcColorBlendFactor
        vk::BlendFactor::eZero, // dstColorBlendFactor
        vk::BlendOp::eAdd, // colorBlendOp
        vk::BlendFactor::eOne, // srcAlphaBlendFactor
        vk::BlendFactor::eZero, // dstAlphaBlendFactor
        vk::BlendOp::eAdd, // alphaBlendOp
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA // colorWriteMask
    );
    vk::PipelineColorBlendStateCreateInfo colorBlending(
        vk::PipelineColorBlendStateCreateFlags(),
        false,  // logicOp
        vk::LogicOp::eCopy,
        1,      // attachementCount
        &attachement,
        { 0.0f, 0.0f, 0.0f, 0.0f } // BlendConstants
    );

    // Z-Test
    vk::PipelineDepthStencilStateCreateInfo depthStencil(
        vk::PipelineDepthStencilStateCreateFlags(),
        true, // depthTest
        true, // depthWrite
        vk::CompareOp::eLess,
        false, // depthBound
        false, // stencilTest
        vk::StencilOpState(), // front
        vk::StencilOpState(), // back
        0, // minDepthBound
        1 // maxDepthBound
    );
    
    // Create pipeline
    vk::GraphicsPipelineCreateInfo pipelineInfo(
        vk::PipelineCreateFlags(),
        2, shaderStages,
        &vertexInputInfo,
        &inputAssembly,
        nullptr, // pTessellationState
        &viewportState,
        &rasterizer,
        &multisampling,
        &depthStencil,
        &colorBlending,
        nullptr, // pDynamicState
        m_pipelineLayout.get(),
        m_framebuffer.getRenderPass(),
        0, // subpass
        nullptr, // basePipelineHandle
        -1 // basePipelineIndex
    );
    m_graphicsPipeline = m_device.get().createGraphicsPipeline(nullptr, pipelineInfo).value;
    
    // Cleanup
    m_device.get().destroyShaderModule(shaderStages[0].module);
    m_device.get().destroyShaderModule(shaderStages[1].module);
}

Pipeline::~Pipeline() {
    m_device.get().destroyPipeline(m_graphicsPipeline);
}

void Pipeline::updateUniformBuffer(unsigned int imageIndex, const glm::mat4x4& viewProj)
{
    m_uniformBuffer.updateBuffer(imageIndex, viewProj);
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

vk::ShaderModule Pipeline::createShaderModule(const std::vector<char>& code, LogicalDevice& device) {
    vk::ShaderModuleCreateInfo createInfo(
        vk::ShaderModuleCreateFlags(),
        code.size(),
        reinterpret_cast<const uint32_t*>(code.data())
    );

    return device.get().createShaderModule(createInfo);
}
