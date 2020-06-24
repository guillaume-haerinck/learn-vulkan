#include "buffer.h"

#include "graphics/setup/devices.h"

VertexBuffer::VertexBuffer(LogicalDevice& device, PhysicalDevice& physicalDevice, MemoryAllocator& memoryAllocator)
    : IBuffer(device, memoryAllocator)
{
    m_vertices[0] = { glm::vec2(-0.5, -0.5), glm::vec3(1, 1, 1) };
    m_vertices[1] = { glm::vec2(0.5, -0.5), glm::vec3(1, 0, 0) };
    m_vertices[2] = { glm::vec2(0.5, 0.5), glm::vec3(0, 1, 0) };
    m_vertices[3] = { glm::vec2(-0.5, 0.5), glm::vec3(0, 0, 1) };

    vk::BufferCreateInfo info(
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

    vk::BufferCreateInfo info(
        vk::BufferCreateFlags(),
        sizeof(m_indices.at(0)) * m_indices.size(),
        vk::BufferUsageFlagBits::eIndexBuffer,
        vk::SharingMode::eExclusive
    );

    m_buffer = m_device.get().createBufferUnique(info);

    allocateAndBindBuffer();
}

UniformBuffer::UniformBuffer()
{
}

UniformBuffer::~UniformBuffer()
{
}
