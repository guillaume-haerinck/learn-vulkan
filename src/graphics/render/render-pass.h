#pragma once

#include <vulkan/vulkan.hpp>

#include "graphics/ressources/images.h"

// TODO defered shading with only one render pass using subpasses

class RenderPass {
public:
	RenderPass();
	~RenderPass();

private:
	vk::RenderPass m_renderPass;
	// TODO subpasses
};

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();

private:
	std::vector<vk::Framebuffer> m_framebuffers; // One for each swapchain image, they share the same attachments and depth
	RenderPass m_renderPass;
	//std::vector<ImageView> m_attachments;
	//ImageView m_depth;
};
