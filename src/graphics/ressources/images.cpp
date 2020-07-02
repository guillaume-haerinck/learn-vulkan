#include "images.h"

#include "stb_image.h"
#include <iostream>

// TODO use KTX image format

DepthImageView::DepthImageView(LogicalDevice& device, MemoryAllocator& memoryAllocator) {
	vk::ImageCreateInfo imageInfo(
		vk::ImageCreateFlags(),
		vk::ImageType::e2D,
		vk::Format::eD32Sfloat,
		vk::Extent3D(800, 600, 1),
		1, // mipLevels
		1, // arrayLayers
		vk::SampleCountFlagBits::e1,
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eDepthStencilAttachment,
		vk::SharingMode::eExclusive,
		0, // queueFamilyIndexCount
		0, // pQueueFamilyIndices, ignored when not concurent
		vk::ImageLayout::eUndefined
	);
	m_image = device.get().createImageUnique(imageInfo);
	m_memory = memoryAllocator.allocateAndBindImage(*this);

	vk::ImageViewCreateInfo imageViewInfo(
		vk::ImageViewCreateFlags(),
		m_image.get(),
		vk::ImageViewType::e2D,
		vk::Format::eD32Sfloat,
		vk::ComponentMapping(),
		vk::ImageSubresourceRange(
			vk::ImageAspectFlagBits::eDepth,
			0, // baseMipLevel
			1, // levelCount
			0, // baseArrayLayer
			1  // layerCount
		)
	);
	m_imageView = device.get().createImageViewUnique(imageViewInfo);
}

DepthImageView::~DepthImageView()
{
}

TextureImageView::TextureImageView(LogicalDevice& device, MemoryAllocator& memoryAllocator, const char* filePath) {
	// Load image
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(filePath, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	if (pixels == nullptr) {
		std::cerr << "Texture not found : " << filePath << std::endl;
		assert(false && "Texture not found");
	}
	unsigned int byteSize = texWidth * texHeight * 4;

	// Move data to staging buffer
	vk::BufferCreateInfo info(
		vk::BufferCreateFlags(),
		byteSize,
		vk::BufferUsageFlagBits::eTransferSrc,
		vk::SharingMode::eExclusive
	);
	auto stagingBuffer = device.get().createBufferUnique(info);
	auto stagingBufferMemory = memoryAllocator.allocateStagingBuffer(*stagingBuffer, byteSize, pixels);
	stbi_image_free(pixels);

	vk::ImageCreateInfo imageInfo(
		vk::ImageCreateFlags(),
		vk::ImageType::e2D,
		vk::Format::eR8G8B8A8Srgb,
		vk::Extent3D(texWidth, texHeight, 1),
		1, // mipLevels
		1, // arrayLayers
		vk::SampleCountFlagBits::e1,
		vk::ImageTiling::eOptimal,
		vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
		vk::SharingMode::eExclusive,
		0, // queueFamilyIndexCount
		0, // pQueueFamilyIndices, ignored when not concurent
		vk::ImageLayout::eUndefined
	);
	m_image = device.get().createImageUnique(imageInfo);
	m_memory = memoryAllocator.allocateAndBindImage(*this);

	vk::ImageViewCreateInfo imageViewInfo(
		vk::ImageViewCreateFlags(),
		m_image.get(),
		vk::ImageViewType::e2D,
		vk::Format::eR8G8B8A8Srgb,
		vk::ComponentMapping(),
		vk::ImageSubresourceRange(
			vk::ImageAspectFlagBits::eColor,
			0, // baseMipLevel
			1, // levelCount
			0, // baseArrayLayer
			1  // layerCount
		)
	);
	m_imageView = device.get().createImageViewUnique(imageViewInfo);

	

	// TODO transfert data from staging to end buffer with a command buffer
	// https://vulkan-tutorial.com/Texture_mapping/Images
}

TextureImageView::~TextureImageView()
{
}

Sampler::Sampler()
{
}

Sampler::~Sampler()
{
}
