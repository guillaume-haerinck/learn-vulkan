#include "buffers.h"

#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

#include "graphics/setup/devices.h"

VertexBuffer::VertexBuffer(LogicalDevice& device, MemoryAllocator& memoryAllocator)
    : IBuffer(device, memoryAllocator)
{
    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3
    m_vertices = {
        // Front v0,v1,v2,v3
        Vertex(glm::vec3(1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(0, 0, 1), glm::vec2(1, 1)),
        // Right v0,v3,v4,v5
        Vertex(glm::vec3(1, 1, 1), glm::vec3(1, 0, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(1, 0, 0), glm::vec2(1, 1)),
        // Top v0,v5,v6,v1
        Vertex(glm::vec3(1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(0, 1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(0, 1, 0), glm::vec2(1, 1)),
        // Left v1,v6,v7,v2
        Vertex(glm::vec3(-1, 1, 1), glm::vec3(-1, 0, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(-1, 0, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(-1, 0, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(-1, 0, 0), glm::vec2(1, 1)),
        // Bottom v7,v4,v3,v2
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(1, 0)),
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0, -1, 0), glm::vec2(0, 0)),
        Vertex(glm::vec3(1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(0, 1)),
        Vertex(glm::vec3(-1, -1, 1), glm::vec3(0, -1, 0), glm::vec2(1, 1)),
        // Back v4,v7,v6,v5
        Vertex(glm::vec3(1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 0)),
        Vertex(glm::vec3(-1, -1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 0)),
        Vertex(glm::vec3(-1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(0, 1)),
        Vertex(glm::vec3(1, 1, -1), glm::vec3(0, 0, -1), glm::vec2(1, 1))
    };

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
        0, 1, 2,   2, 3, 0,       // front
        4, 5, 6,   6, 7, 4,       // right
        8, 9, 10,  10,11, 8,      // top
        12,13,14,  14,15,12,      // left
        16,17,18,  18,19,16,      // bottom
        20,21,22,  22,23,20		  // back
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
    m_ubo.world = glm::translate(glm::mat4(1), glm::vec3(0.5, 0.5, 0));
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

void UniformBuffer::updateBuffer(unsigned int currentImage, const glm::mat4x4& viewProj)
{
    // TODO use push constants instead to update buffer

    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    m_ubo.world = glm::rotate(glm::mat4(1), time * glm::radians(90.0f), glm::vec3(0, 0, 1));
    m_ubo.viewProj = viewProj;
    
    // Copy data
    vk::MemoryRequirements memoryRequirements = m_device.get().getBufferMemoryRequirements(m_buffers.at(currentImage).get());
    unsigned int* pData = static_cast<unsigned int*>(m_device.get().mapMemory(m_bufferMemories.at(currentImage).get(), 0, memoryRequirements.size));
    memcpy(pData, &m_ubo, sizeof(m_ubo));
    m_device.get().unmapMemory(m_bufferMemories.at(currentImage).get());
}
