#include "vertex-buffer.h"

#include "graphics/setup/devices.h"

VertexInputDescription::VertexInputDescription()
{
	m_description = vk::VertexInputBindingDescription(0, sizeof(Vertex), vk::VertexInputRate::eVertex);
	m_inputDescription = {
		vk::VertexInputAttributeDescription(0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, Vertex::position)),
		vk::VertexInputAttributeDescription(1, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, Vertex::color))
	};
}

VertexInputDescription::~VertexInputDescription()
{
}

vk::VertexInputBindingDescription VertexInputDescription::getBindingDescription()
{
	return m_description;
}

std::array<vk::VertexInputAttributeDescription, 2> VertexInputDescription::getAttributeDescriptions()
{
	return m_inputDescription;
}

VertexBuffer::VertexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& device) 
	: m_physicalDevice(physicalDevice), m_device(device)
{
	m_vertices[0] = { glm::vec2(0, -0.5), glm::vec3(1, 0, 0) };
	m_vertices[1] = { glm::vec2(0.5, 0.5), glm::vec3(0, 1, 0) };
	m_vertices[2] = { glm::vec2(-0.5, 0.5), glm::vec3(0, 0, 1) };

	vk::BufferCreateInfo info = vk::BufferCreateInfo(
		vk::BufferCreateFlags(), 
		sizeof(m_vertices.at(0)) * m_vertices.size(),
		vk::BufferUsageFlagBits::eVertexBuffer,
		vk::SharingMode::eExclusive
	);

	m_buffer = m_device.get().createBufferUnique(info);

	// Memory handling (may use the AMD openGPU lib to handle this in the future)
	{
		vk::MemoryRequirements memoryRequirements = m_device.get().getBufferMemoryRequirements(m_buffer.get());

		unsigned int memoryTypeIndex = PhysicalDevice::findMemoryType(
			m_physicalDevice.get().getMemoryProperties(),
			memoryRequirements.memoryTypeBits,
			vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
		);

		m_deviceMemory = m_device.get().allocateMemoryUnique(
			vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex)
		);

		// Copy the data to the device memory
		unsigned int* pData = static_cast<unsigned int*>(m_device.get().mapMemory(m_deviceMemory.get(), 0, memoryRequirements.size));
		memcpy(pData, m_vertices.data(), sizeof(m_vertices)); // FIXME TODO size might be wrong
		m_device.get().unmapMemory(m_deviceMemory.get());

		m_device.get().bindBufferMemory(m_buffer.get(), m_deviceMemory.get(), 0);
	}
	
}

VertexBuffer::~VertexBuffer()
{
	// TODO destroy buffer and free memory ? (maybe done automatically ?)
}

vk::UniqueBuffer& VertexBuffer::getBuffer()
{
	return m_buffer;
}
