#include "buffer.h"

#include "graphics/setup/devices.h"

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

VertexBuffer::VertexBuffer(LogicalDevice& device, PhysicalDevice& physicalDevice, MemoryAllocator& memoryAllocator)
    : IBuffer(device, memoryAllocator)
{
    m_vertices[0] = { glm::vec2(-0.5, -0.5), glm::vec3(1, 1, 1) };
    m_vertices[1] = { glm::vec2(0.5, -0.5), glm::vec3(1, 0, 0) };
    m_vertices[2] = { glm::vec2(0.5, 0.5), glm::vec3(0, 1, 0) };
    m_vertices[3] = { glm::vec2(-0.5, 0.5), glm::vec3(0, 0, 1) };

    vk::BufferCreateInfo info = vk::BufferCreateInfo(
        vk::BufferCreateFlags(),
        sizeof(m_vertices.at(0)) * m_vertices.size(),
        vk::BufferUsageFlagBits::eVertexBuffer,
        vk::SharingMode::eExclusive
    );

    m_buffer = m_device.get().createBufferUnique(info);

    allocateAndBindBuffer();
}

IndexBuffer::IndexBuffer(LogicalDevice& device, MemoryAllocator& memoryAllocator)
    : IBuffer(device, memoryAllocator)
{
    m_indices = {
        0, 1, 2,
        2, 3, 0
    };

    vk::BufferCreateInfo info = vk::BufferCreateInfo(
        vk::BufferCreateFlags(),
        sizeof(m_indices.at(0)) * m_indices.size(),
        vk::BufferUsageFlagBits::eIndexBuffer,
        vk::SharingMode::eExclusive
    );

    m_buffer = m_device.get().createBufferUnique(info);

    allocateAndBindBuffer();
}
