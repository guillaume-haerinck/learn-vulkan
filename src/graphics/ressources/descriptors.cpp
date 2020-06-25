#include "descriptors.h"

#include "buffers.h"

VertexInputDescription::VertexInputDescription()
{
	m_description = vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
	m_inputDescription = {
		vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, Vertex::position)),
		vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, Vertex::color))
	};
}

VertexInputDescription::~VertexInputDescription() {}

vk::VertexInputBindingDescription VertexInputDescription::getBindingDescription()
{
	return m_description;
}

std::array<vk::VertexInputAttributeDescription, 2> VertexInputDescription::getAttributeDescriptions()
{
	return m_inputDescription;
}

DescriptorSetLayout::DescriptorSetLayout(LogicalDevice& device)
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding(
		0, // binding
		vk::DescriptorType::eUniformBuffer,
		1, // descriptorCount
		vk::ShaderStageFlagBits::eVertex,
		nullptr // pImmutableSamplers
	);

	vk::DescriptorSetLayoutCreateInfo layoutInfo(
		vk::DescriptorSetLayoutCreateFlags(),
		1,
		&uboLayoutBinding
	);

	m_layout = device.get().createDescriptorSetLayoutUnique(layoutInfo);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
}

DescriptorPool::DescriptorPool(LogicalDevice& device, unsigned int swapChainImagesCount)
	: m_device(device)
{
	vk::DescriptorPoolSize poolSize(vk::DescriptorType::eUniformBuffer);

	vk::DescriptorPoolCreateInfo info(
		vk::DescriptorPoolCreateFlags(),
		swapChainImagesCount,
		1,
		&poolSize
	);

	m_descriptorPool = device.get().createDescriptorPoolUnique(info);
}

DescriptorPool::~DescriptorPool()
{
	m_device.get().destroyDescriptorPool(m_descriptorPool.get());
}


DescriptorSet::DescriptorSet(LogicalDevice& device, DescriptorPool& descriptorPool, DescriptorSetLayout& descriptorSetLayout, unsigned int swapChainImagesCount, UniformBuffer& uniformBuffer)
	: m_device(device)
{
	std::vector<vk::DescriptorSetLayout> layouts(swapChainImagesCount, descriptorSetLayout.get());

	vk::DescriptorSetAllocateInfo info(
		descriptorPool.get(),
		layouts.size(),
		layouts.data()
	);

	m_descriptorSets = device.get().allocateDescriptorSetsUnique(info);

	// Populate descriptors
	for (int i = 0; i < layouts.size(); i++)
	{
		vk::DescriptorBufferInfo bufferInfo(
			uniformBuffer.getBuffer(i),
			0, // offset
			uniformBuffer.getByteSize(i)
		);

		// TODO next
	}
}

DescriptorSet::~DescriptorSet()
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
