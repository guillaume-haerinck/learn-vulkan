#include "images.h"

ImageView::ImageView(LogicalDevice& device, MemoryAllocator& memoryAllocator)
{
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
		vk::Format::eR8G8B8A8Srgb,
		vk::ComponentMapping(),
		vk::ImageSubresourceRange(
			vk::ImageAspectFlagBits::eDepth
		)
	);
	m_imageView = device.get().createImageViewUnique(imageViewInfo);
}

ImageView::~ImageView()
{
}

Sampler::Sampler()
{
}

Sampler::~Sampler()
{
}
