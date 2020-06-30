#pragma once

#include <vulkan/vulkan.hpp>

#include "graphics/setup/devices.h"
#include "graphics/ressources/memory-allocator.h"

class ImageView {
public:
	ImageView(LogicalDevice& device, MemoryAllocator& memoryAllocator);
	~ImageView();

	vk::Image& get() { return m_image.get(); }
	vk::ImageView& getImageView() { return m_imageView.get(); }

private:
	vk::UniqueImage m_image;
	vk::UniqueImageView m_imageView;
	vk::UniqueDeviceMemory m_memory;
};

class Sampler {
public:
	Sampler();
	~Sampler();

private:

};

