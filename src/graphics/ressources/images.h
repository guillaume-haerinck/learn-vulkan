#pragma once

#include <vulkan/vulkan.hpp>

#include "graphics/setup/devices.h"
#include "graphics/ressources/memory-allocator.h"

class CommandBufferFactory;

// TODO might use the setImage function from https://github.com/KhronosGroup/Vulkan-Hpp/blob/20ee34ae970e6a89a93d385d186bd780b92651a0/samples/utils/utils.hpp

/**
 * @brief Abstract class for images
 */
class ImageView {
public:
	ImageView() {};
	virtual ~ImageView() {}

	vk::Image& getImage() { return m_image.get(); }
	vk::ImageView& get() { return m_imageView.get(); }

protected:
	void setImageLayout(vk::CommandBuffer const& commandBuffer,
		vk::Format                      format,
		vk::ImageLayout                 oldImageLayout,
		vk::ImageLayout                 newImageLayout);
	
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
	TextureImageView(LogicalDevice& device, MemoryAllocator& memoryAllocator, CommandBufferFactory& commandBufferFactory, const char* filePath);
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
