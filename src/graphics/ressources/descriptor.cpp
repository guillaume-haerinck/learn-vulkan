#include "descriptor.h"

#include "buffer.h"

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

DescriptorSetLayout::DescriptorSetLayout()
{
	vk::DescriptorSetLayoutBinding uboLayoutBinding(
		0, // binding
		vk::DescriptorType::eUniformBuffer,
		1, // descriptorCount
		vk::ShaderStageFlagBits::eVertex,
		nullptr // pImmutableSamplers
	);
}

DescriptorSetLayout::~DescriptorSetLayout()
{
}

DescriptorSet::DescriptorSet()
{
}

DescriptorSet::~DescriptorSet()
{
}

PipelineLayout::PipelineLayout()
{
}

PipelineLayout::~PipelineLayout()
{
}

DescriptorPool::DescriptorPool()
{
}

DescriptorPool::~DescriptorPool()
{
}
