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

Pipeline::Pipeline(LogicalDevice& device, SwapChain& swapChain) : m_device(device) {
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
        vk::FrontFace::eClockwise,
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

    // Constant buffers ? TODO temp move to ConstantBuffer ?
    vk::PipelineLayoutCreateInfo pipelineLayoutInfo(vk::PipelineLayoutCreateFlags(), 0, nullptr, 0, nullptr);
    m_pipelineLayout = device.get().createPipelineLayout(pipelineLayoutInfo);
    
    // Render pass, TODO move to a separate class
    {
        vk::AttachmentDescription colorAttachment(
            vk::AttachmentDescriptionFlags(),
            swapChain.getImageFormat(),
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        );

        vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);
        vk::SubpassDescription subpass(
            vk::SubpassDescriptionFlags(),
            vk::PipelineBindPoint::eGraphics,
            0, nullptr,
            1, &colorAttachmentRef
        );

        vk::SubpassDependency dependency(
            VK_SUBPASS_EXTERNAL, // srcSubpass
            0, // dstSubpass
            vk::PipelineStageFlagBits::eColorAttachmentOutput, // srcStage
            vk::PipelineStageFlagBits::eColorAttachmentOutput, // dstStage
            vk::AccessFlagBits::eColorAttachmentRead, // srcAccess
            vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite // dstAccess
        );
        
        // Create render pass
        vk::RenderPassCreateInfo renderPassInfo(
            vk::RenderPassCreateFlags(),
            1, &colorAttachment,
            1, &subpass,
            1, &dependency
        );
        m_renderPass = m_device.get().createRenderPass(renderPassInfo);
    }
    
    // Create pipeline
    vk::GraphicsPipelineCreateInfo pipelineInfo(
        vk::PipelineCreateFlags(),
        2, shaderStages,
        &vertexInputInfo,
        &inputAssembly,
        nullptr,
        &viewportState,
        &rasterizer,
        &multisampling,
        nullptr,
        &colorBlending,
        nullptr,
        m_pipelineLayout,
        m_renderPass,
        0,
        nullptr,
        -1
    );
    m_graphicsPipeline = m_device.get().createGraphicsPipeline(nullptr, pipelineInfo);

    // Framebuffers
    {
        m_swapChainFramebuffers.resize(swapChain.getImageViews().size());
        for (size_t i = 0; i < swapChain.getImageViews().size(); i++) {
            vk::ImageView attachments[] = {
                swapChain.getImageViews()[i]
            };

            vk::FramebufferCreateInfo framebufferInfo(
                vk::FramebufferCreateFlags(),
                m_renderPass,
                1, attachments,
                swapChain.getExtent().width,
                swapChain.getExtent().height,
                1 // layers
            );
            m_swapChainFramebuffers[i] = m_device.get().createFramebuffer(framebufferInfo);
        }
    }
    
    // Cleanup
    m_device.get().destroyShaderModule(shaderStages[0].module);
    m_device.get().destroyShaderModule(shaderStages[1].module);
}

Pipeline::~Pipeline() {
    for (auto framebuffer : m_swapChainFramebuffers) {
        m_device.get().destroyFramebuffer(framebuffer);
    }
    m_device.get().destroyPipeline(m_graphicsPipeline);
    m_device.get().destroyPipelineLayout(m_pipelineLayout);
    m_device.get().destroyRenderPass(m_renderPass);
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
