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
	auto stagingBufferMemory = memoryAllocator.allocateAndBindStagingBuffer(*stagingBuffer, byteSize, pixels);
	stbi_image_free(pixels);

	// Create image
	{
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
	}

	// Transfert data from staging to end buffer with a command buffer
	{
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

		// Commands
		auto commandBuffer = commandBufferFactory.createAndBeginSingleTimeBuffer();
		setImageLayout(commandBuffer.get(), vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);
		commandBuffer->copyBufferToImage(stagingBuffer.get(), m_image.get(), vk::ImageLayout::eTransferDstOptimal, region);
		setImageLayout(commandBuffer.get(), vk::Format::eR8G8B8A8Srgb, vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);
		commandBuffer->end();

		// Send commands
		vk::SubmitInfo submitInfo(
			0, nullptr, // waitSemaphores
			nullptr, // waitDstStageMask
			1, &commandBuffer.get(), // commandBuffers
			0, nullptr // signalSemaphores
		);

		device.getGraphicQueue().submit(submitInfo, nullptr);
		device.getGraphicQueue().waitIdle();
	}
}

TextureImageView::~TextureImageView()
{
}

Sampler::Sampler(LogicalDevice& device) {
	vk::SamplerCreateInfo info(
		vk::SamplerCreateFlags(),
		vk::Filter::eLinear, // magFilter
		vk::Filter::eLinear, // minFilter
		vk::SamplerMipmapMode::eLinear,
		vk::SamplerAddressMode::eRepeat,
		vk::SamplerAddressMode::eRepeat,
		vk::SamplerAddressMode::eRepeat,
		0.f, // mipLodBias
		true, // anisotropy
		16.f, // maxAnisotropy
		false, // compareEnable
		vk::CompareOp::eAlways,
		0.f, // minLod
		0.f, // maxLod
		vk::BorderColor::eIntOpaqueBlack,
		false // unnormalizedCoordinates
	);
	m_sampler = device.get().createSamplerUnique(info);
}

Sampler::~Sampler()
{
}

void ImageView::setImageLayout(vk::CommandBuffer const& commandBuffer, vk::Format format, vk::ImageLayout oldImageLayout, vk::ImageLayout newImageLayout) {
	vk::AccessFlags sourceAccessMask;
	switch (oldImageLayout) {
	case vk::ImageLayout::eTransferDstOptimal: sourceAccessMask = vk::AccessFlagBits::eTransferWrite; break;
	case vk::ImageLayout::ePreinitialized: sourceAccessMask = vk::AccessFlagBits::eHostWrite; break;
	case vk::ImageLayout::eGeneral:  // sourceAccessMask is empty
	case vk::ImageLayout::eUndefined: break;
	default: assert(false); break;
	}

	vk::PipelineStageFlags sourceStage;
	switch (oldImageLayout) {
	case vk::ImageLayout::eGeneral:
	case vk::ImageLayout::ePreinitialized: sourceStage = vk::PipelineStageFlagBits::eHost; break;
	case vk::ImageLayout::eTransferDstOptimal: sourceStage = vk::PipelineStageFlagBits::eTransfer; break;
	case vk::ImageLayout::eUndefined: sourceStage = vk::PipelineStageFlagBits::eTopOfPipe; break;
	default: assert(false); break;
	}

	vk::AccessFlags destinationAccessMask;
	switch (newImageLayout) {
	case vk::ImageLayout::eColorAttachmentOptimal:
		destinationAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
		break;
	case vk::ImageLayout::eDepthStencilAttachmentOptimal:
		destinationAccessMask =
			vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;
		break;
	case vk::ImageLayout::eGeneral:  // empty destinationAccessMask
	case vk::ImageLayout::ePresentSrcKHR: break;
	case vk::ImageLayout::eShaderReadOnlyOptimal: destinationAccessMask = vk::AccessFlagBits::eShaderRead; break;
	case vk::ImageLayout::eTransferSrcOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferRead; break;
	case vk::ImageLayout::eTransferDstOptimal: destinationAccessMask = vk::AccessFlagBits::eTransferWrite; break;
	default: assert(false); break;
	}

	vk::PipelineStageFlags destinationStage;
	switch (newImageLayout) {
	case vk::ImageLayout::eColorAttachmentOptimal:
		destinationStage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
		break;
	case vk::ImageLayout::eDepthStencilAttachmentOptimal:
		destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
		break;
	case vk::ImageLayout::eGeneral: destinationStage = vk::PipelineStageFlagBits::eHost; break;
	case vk::ImageLayout::ePresentSrcKHR: destinationStage = vk::PipelineStageFlagBits::eBottomOfPipe; break;
	case vk::ImageLayout::eShaderReadOnlyOptimal:
		destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
		break;
	case vk::ImageLayout::eTransferDstOptimal:
	case vk::ImageLayout::eTransferSrcOptimal: destinationStage = vk::PipelineStageFlagBits::eTransfer; break;
	default: assert(false); break;
	}

	vk::ImageAspectFlags aspectMask;
	if (newImageLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		aspectMask = vk::ImageAspectFlagBits::eDepth;
		if (format == vk::Format::eD32SfloatS8Uint || format == vk::Format::eD24UnormS8Uint)
			aspectMask |= vk::ImageAspectFlagBits::eStencil;
	} else {
		aspectMask = vk::ImageAspectFlagBits::eColor;
	}

	vk::ImageSubresourceRange imageSubresourceRange(aspectMask, 0, 1, 0, 1);
	vk::ImageMemoryBarrier    imageMemoryBarrier(
		sourceAccessMask,
		destinationAccessMask,
		oldImageLayout,
		newImageLayout,
		VK_QUEUE_FAMILY_IGNORED,
		VK_QUEUE_FAMILY_IGNORED,
		m_image.get(),
		imageSubresourceRange
	);
	return commandBuffer.pipelineBarrier(sourceStage, destinationStage, {}, nullptr, nullptr, imageMemoryBarrier);
}
