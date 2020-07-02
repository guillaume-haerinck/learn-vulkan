#pragma once

#include <vulkan/vulkan.hpp>

#include "graphics/ressources/images.h"
#include "graphics/render/swap-chain.h"

// TODO defered shading with only one render pass using subpasses

class RenderPass {
public:
	RenderPass(LogicalDevice& device, SwapChain& swapChain);
	~RenderPass();

	vk::RenderPass& get() { return m_renderPass; }

private:
	LogicalDevice& m_device;
	vk::RenderPass m_renderPass;
};

class FrameBuffer {
public:
	FrameBuffer(LogicalDevice& device, SwapChain& swapChain, MemoryAllocator& memoryAllocator);
	~FrameBuffer();

	vk::RenderPass& getRenderPass() { return m_renderPass.get(); }
	std::vector<vk::Framebuffer>& get() { return m_swapChainFramebuffers; }

private:
	LogicalDevice& m_device;
	std::vector<vk::Framebuffer> m_swapChainFramebuffers; // One for each swapchain image, they share the same attachments and depth
	RenderPass m_renderPass;
	std::vector<ImageView> m_attachments;
	DepthImageView m_depth;
};
