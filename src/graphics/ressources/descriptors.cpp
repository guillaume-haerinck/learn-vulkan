#include "descriptors.h"

#include "buffers.h"

VertexInputDescription::VertexInputDescription() {
	m_description = vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
	m_inputDescription = {
		vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, Vertex::position)),
		vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, Vertex::normal)),
		vk::VertexInputAttributeDescription(2, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, Vertex::uv))
	};
}

VertexInputDescription::~VertexInputDescription() {}

vk::VertexInputBindingDescription VertexInputDescription::getBindingDescription() {
	return m_description;
}

std::array<vk::VertexInputAttributeDescription, 3> VertexInputDescription::getAttributeDescriptions() {
	return m_inputDescription;
}

DescriptorSetLayout::DescriptorSetLayout(LogicalDevice& device) {
	vk::DescriptorSetLayoutBinding uboLayoutBinding(
		0, // binding
		vk::DescriptorType::eUniformBuffer,
		1, // descriptorCount
		vk::ShaderStageFlagBits::eVertex,
		nullptr // pImmutableSamplers
	);

	vk::DescriptorSetLayoutBinding samplerLayoutBinding(
		1, // binding
		vk::DescriptorType::eCombinedImageSampler,
		1, // descriptorCount
		vk::ShaderStageFlagBits::eFragment,
		nullptr // pImmutableSamplers
	);

	vk::DescriptorSetLayoutBinding layouts[2] = {
		uboLayoutBinding, samplerLayoutBinding
	};

	vk::DescriptorSetLayoutCreateInfo layoutInfo(
		vk::DescriptorSetLayoutCreateFlags(),
		2, layouts
	);

	m_layout = device.get().createDescriptorSetLayoutUnique(layoutInfo);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
}

DescriptorPool::DescriptorPool(LogicalDevice& device)
	: m_device(device)
{
	std::vector<vk::DescriptorPoolSize> poolSizes = {
		{ vk::DescriptorType::eSampler, 1000 },
		{ vk::DescriptorType::eCombinedImageSampler, 1000 },
		{ vk::DescriptorType::eSampledImage, 1000 },
		{ vk::DescriptorType::eStorageImage, 1000 },
		{ vk::DescriptorType::eUniformTexelBuffer, 1000 },
		{ vk::DescriptorType::eStorageTexelBuffer, 1000 },
		{ vk::DescriptorType::eUniformBuffer, 1000 },
		{ vk::DescriptorType::eStorageBuffer, 1000 },
		{ vk::DescriptorType::eUniformBufferDynamic, 1000 },
		{ vk::DescriptorType::eStorageBufferDynamic, 1000 },
		{ vk::DescriptorType::eInputAttachment, 1000 }
	};

	vk::DescriptorPoolCreateInfo info(
		vk::DescriptorPoolCreateFlags(),
		1000 * poolSizes.size(), // maxSets
		poolSizes.size(), // poolSizeCount
		poolSizes.data()
	);

	m_descriptorPool = device.get().createDescriptorPoolUnique(info);
}

DescriptorPool::~DescriptorPool()
{
}


DescriptorSets::DescriptorSets(LogicalDevice& device, 
	DescriptorPool& descriptorPool, 
	DescriptorSetLayout& descriptorSetLayout, 
	unsigned int swapChainImagesCount, 
	UniformBuffer& uniformBuffer, 
	Sampler& sampler,
	TextureImageView& texture)
	: m_device(device)
{
	std::vector<vk::DescriptorSetLayout> layouts(swapChainImagesCount, descriptorSetLayout.get());

	vk::DescriptorSetAllocateInfo info(
		descriptorPool.get(),
		layouts.size(),
		layouts.data()
	);

	m_descriptorSets = device.get().allocateDescriptorSets(info);

	// Populate descriptors
	for (int i = 0; i < layouts.size(); i++)
	{
		vk::DescriptorBufferInfo bufferInfo(
			uniformBuffer.getBuffer(i),
			0, // offset
			uniformBuffer.getByteSize(i)
		);

		vk::DescriptorImageInfo imageInfo(
			sampler.get(),
			texture.get(),
			vk::ImageLayout::eShaderReadOnlyOptimal
		);

		// TODO add imageInfo to writeInfo

		vk::WriteDescriptorSet writeInfo(
			m_descriptorSets.at(i),
			0, // dstBinding
			0, // dstArrayElement
			1, // descriptorCount
			vk::DescriptorType::eUniformBuffer,
			nullptr, // pImageInfo
			&bufferInfo,
			nullptr // pTexelBufferView
		);

		m_device.get().updateDescriptorSets(1, &writeInfo, 0, nullptr);
	}
}

DescriptorSets::~DescriptorSets()
{
}

PipelineLayout::PipelineLayout(LogicalDevice& device)
	: m_device(device), m_descriptorSetLayout(device)
{
	vk::PipelineLayoutCreateInfo pipelineLayoutInfo(
		vk::PipelineLayoutCreateFlags(), 
		1, // setLayoutCount
		&m_descriptorSetLayout.get(),
		0, // pushConstantRangeCount
		nullptr
	);
	m_pipelineLayout = device.get().createPipelineLayout(pipelineLayoutInfo);
}

PipelineLayout::~PipelineLayout()
{
	m_device.get().destroyPipelineLayout(m_pipelineLayout);
}
