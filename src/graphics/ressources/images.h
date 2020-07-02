#pragma once

#include <vulkan/vulkan.hpp>

#include "graphics/setup/devices.h"
#include "graphics/ressources/memory-allocator.h"

/**
 * @brief Abstract class for images
 */
class ImageView {
public:
	ImageView() {}
	virtual ~ImageView() {}

	vk::Image& getImage() { return m_image.get(); }
	vk::ImageView& get() { return m_imageView.get(); }
	
protected:
	vk::UniqueImage m_image;
	vk::UniqueImageView m_imageView;
	vk::UniqueDeviceMemory m_memory;
};

/**
 * @brief Depth buffer
 */
class DepthImageView : public ImageView {
public:
	DepthImageView(LogicalDevice& device, MemoryAllocator& memoryAllocator);
	~DepthImageView();
};

/**
 * @brief Texture color image from file
 */
class TextureImageView : public ImageView {
public:
	TextureImageView(LogicalDevice& device, MemoryAllocator& memoryAllocator, const char* filePath);
	~TextureImageView();
};

/**
 * @brief Texture sampler
 */
class Sampler {
public:
	Sampler();
	~Sampler();

private:

};
