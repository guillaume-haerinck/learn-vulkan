#include "render-pass.h"

RenderPass::RenderPass(LogicalDevice& device, SwapChain& swapChain)
    : m_device(device)
{
    // Color buffer
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

    // Depth buffer
    vk::AttachmentDescription depthAttachment(
        vk::AttachmentDescriptionFlags(),
        vk::Format::eD32Sfloat,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare, // Stencil load
        vk::AttachmentStoreOp::eDontCare, // Stencil store
        vk::ImageLayout::eUndefined, // Initial layout
        vk::ImageLayout::eDepthStencilAttachmentOptimal // End layout
    );
    vk::AttachmentReference depthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    // Handle subpass
    vk::SubpassDescription subpass(
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        0, nullptr, // Input attachments
        1, &colorAttachmentRef, // Color attachments
        nullptr, // Resolve attachment
        &depthAttachmentRef, // DepthStencil Attachment
        0, nullptr // Preserve attachments
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
    std::array<vk::AttachmentDescription, 2> attachments = {
        colorAttachment, depthAttachment
    };
    vk::RenderPassCreateInfo renderPassInfo(
        vk::RenderPassCreateFlags(),
        2, attachments.data(),
        1, &subpass,
        1, &dependency
    );
    m_renderPass = m_device.get().createRenderPass(renderPassInfo);
}

RenderPass::~RenderPass()
{
    m_device.get().destroyRenderPass(m_renderPass);
}

FrameBuffer::FrameBuffer(LogicalDevice& device, SwapChain& swapChain, MemoryAllocator& memoryAllocator)
    : m_device(device), m_renderPass(device, swapChain), m_depth(device, memoryAllocator)
{
    m_swapChainFramebuffers.resize(swapChain.getImageViews().size());
    for (size_t i = 0; i < swapChain.getImageViews().size(); i++) {
        vk::ImageView attachments[] = {
            swapChain.getImageViews()[i],
            m_depth.get()
        };

        vk::FramebufferCreateInfo framebufferInfo(
            vk::FramebufferCreateFlags(),
            m_renderPass.get(),
            2, attachments,
            swapChain.getExtent().width,
            swapChain.getExtent().height,
            1 // layers
        );
        m_swapChainFramebuffers[i] = m_device.get().createFramebuffer(framebufferInfo);
    }
}

FrameBuffer::~FrameBuffer()
{
    for (auto framebuffer : m_swapChainFramebuffers) {
        m_device.get().destroyFramebuffer(framebuffer);
    }
}
