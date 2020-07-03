#include "images.h"

#include <stb_image.h>
#include <iostream>

#include "graphics/render/commands.h"

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

TextureImageView::TextureImageView(LogicalDevice& device, MemoryAllocator& memoryAllocator, CommandBufferFactory& commandBufferFactory, const char* filePath) {
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

	// Transfert data from staging to end buffer with a command buffer
	// https://vulkan-tutorial.com/Texture_mapping/Images
	// TODO use command buffer factory
	/*
	{
		vk::CommandBufferAllocateInfo allocInfo(
			commandPool.get(),
			vk::CommandBufferLevel::ePrimary,
			1
		);
		auto commandBuffer = device.get().allocateCommandBuffers(allocInfo).at(0);

		vk::CommandBufferBeginInfo beginInfo(
			vk::CommandBufferUsageFlagBits::eOneTimeSubmit,
			nullptr
		);

		vk::BufferImageCopy region(
			0, // bufferOffset
			0, // bufferRowLength
			0,  // bufferImageHeight
			vk::ImageSubresourceLayers(
				vk::ImageAspectFlagBits::eColor,
				0, // mipLevel
				0, // baseArrayLayer
				1  // layerCount
			),
			vk::Offset3D(0, 0, 0),
			vk::Extent3D(texWidth, texHeight, 1)
		);

		vk::ImageMemoryBarrier barrier(
			vk::AccessFlags(), // srcAccessMask FIXME Might be wrong
			vk::AccessFlagBits::eTransferWrite, // dstAccessMask
			vk::ImageLayout::eUndefined, // oldLayout
			vk::ImageLayout::eTransferDstOptimal, // newLayout
			VK_QUEUE_FAMILY_IGNORED, // srcQueueFamilyIndex
			VK_QUEUE_FAMILY_IGNORED, // dstQueueFamilyIndex
			m_image.get(),
			vk::ImageSubresourceRange(
				vk::ImageAspectFlagBits::eColor,
				0, // mipLevel
				1, // levelCount
				0, // baseArrayLayer
				1  // layerCount
			)
		);

		commandBuffer.begin(beginInfo);
		


		commandBuffer.end();

		vk::SubmitInfo submitInfo(
			0, nullptr, // waitSemaphores
			nullptr, // waitDstStageMask
			1, &commandBuffer, // commandBuffers
			0, nullptr // signalSemaphores
		);
		device.getPresentQueue().submit(submitInfo, nullptr);

		device.getPresentQueue().waitIdle();
	}
	*/
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
