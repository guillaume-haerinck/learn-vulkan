#include "buffers.h"

#include "graphics/setup/devices.h"

VertexBuffer::VertexBuffer(LogicalDevice& device, MemoryAllocator& memoryAllocator)
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

    m_buffers.push_back(m_device.get().createBufferUnique(info));
    m_bufferMemories.push_back(m_memoryAllocator.allocateAndBindBuffer(*this));
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

    m_buffers.push_back(m_device.get().createBufferUnique(info));
    m_bufferMemories.push_back(m_memoryAllocator.allocateAndBindBuffer(*this));
}

UniformBuffer::UniformBuffer(LogicalDevice& device, MemoryAllocator& memoryAllocator, unsigned int swapChainImagesCount) 
    : IBuffer(device, memoryAllocator)
{
    m_ubo.world = glm::mat4(1);
    m_ubo.viewProj = glm::mat4(1);

    vk::DeviceSize bufferSize = sizeof(PerFrameUB);

    vk::BufferCreateInfo info(
        vk::BufferCreateFlags(),
        bufferSize,
        vk::BufferUsageFlagBits::eUniformBuffer,
        vk::SharingMode::eExclusive
    );
    
    for (size_t i = 0; i < swapChainImagesCount; i++)
    {
        m_buffers.push_back(m_device.get().createBufferUnique(info));
        m_bufferMemories.push_back(m_memoryAllocator.allocateAndBindBuffer(*this, i));
    }
}

UniformBuffer::~UniformBuffer()
{
}
