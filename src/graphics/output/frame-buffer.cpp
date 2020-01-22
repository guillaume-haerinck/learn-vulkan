#include "frame-buffer.h"

#include <iostream>
#include <debug_break/debug_break.h>

FrameBuffer::FrameBuffer(LogicalDevice& device, SwapChain& swapChain, Pipeline& pipeline) : m_device(device) {
    swapChainFramebuffers.resize(swapChain.getImageViews().size());
    for (size_t i = 0; i < swapChain.getImageViews().size(); i++) {
        VkImageView attachments[] = {
            swapChain.getImageViews()[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = pipeline.getRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain.getExtent().width;
        framebufferInfo.height = swapChain.getExtent().height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device.get(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            std::cerr << "[SwapChain] Failed to create framebuffer" << std::endl;
            debug_break();
        }
    }
}

FrameBuffer::~FrameBuffer() {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(m_device.get(), framebuffer, nullptr);
    }
}
