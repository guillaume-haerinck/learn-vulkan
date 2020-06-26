#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <array>

#include "graphics/setup/devices.h"
#include "graphics/ressources/memory-allocator.h"

// temp
struct Vertex
{
    glm::vec2 position;
    glm::vec3 color;
};

// TODO use a staging buffer
// TODO use the same buffer for both vertex, index and uniform (use offset to do so)

/**
 * @brief Abstract class for every buffer (VBO, IBO, UBO)
 */
class IBuffer
{
public:
    IBuffer(LogicalDevice& device, MemoryAllocator& memoryAllocator) 
        : m_device(device), m_memoryAllocator(memoryAllocator)
    {}

    virtual ~IBuffer() {}
    
    vk::Buffer& getBuffer(unsigned int index = 0) { return m_buffers.at(index).get(); }

    virtual void* getData(unsigned int index = 0) = 0;
    virtual size_t getByteSize(unsigned int index = 0) = 0;
    virtual size_t getDataElementCount(unsigned int index = 0) = 0;

protected:
    LogicalDevice& m_device;
    MemoryAllocator& m_memoryAllocator;
    std::vector<vk::UniqueBuffer> m_buffers;
    std::vector<vk::UniqueDeviceMemory> m_bufferMemories;
};

/**
 * @brief Geometry data
 */
class VertexBuffer : public IBuffer {
public:
    VertexBuffer(LogicalDevice& device, MemoryAllocator& memoryAllocator);
    ~VertexBuffer() {}

    
    size_t getDataElementCount(unsigned int index = 0) override { return m_vertices.size(); }
    void* getData(unsigned int index = 0) override { return m_vertices.data(); }
    size_t getByteSize(unsigned int index = 0) override { return sizeof(m_vertices); }

private:
    std::array<Vertex, 4> m_vertices;
};

/**
 * @brief Geometry data pointers
 */
class IndexBuffer : public IBuffer {
public:
    IndexBuffer(LogicalDevice& device, MemoryAllocator& memoryAllocator);
    ~IndexBuffer() {}

    size_t getDataElementCount(unsigned int index = 0) override { return m_indices.size(); }
    void* getData(unsigned int index = 0) override { return m_indices.data(); }
    size_t getByteSize(unsigned int index = 0) override { return sizeof(m_indices); }

private:
    std::array<unsigned int, 6> m_indices; // TODO use and test with std::vector (size bytes calculation problems)
};

/**
 * @brief Uniform buffer updated once each frame
 */
struct PerFrameUB
{
    glm::mat4 viewProj;
    glm::mat4 world;
};

class UniformBuffer : public IBuffer {
public:
    UniformBuffer(LogicalDevice& device, MemoryAllocator& memoryAllocator, unsigned int swapChainImagesCount = 3);
    ~UniformBuffer();

    size_t getDataElementCount(unsigned int index = 0) override { return 3; }
    void* getData(unsigned int index = 0) override { return &m_ubo; }
    size_t getByteSize(unsigned int index = 0) override { return sizeof(m_ubo); }

    void updateBuffer(unsigned int currentImage, const glm::mat4x4& viewProj);

private:
    PerFrameUB m_ubo;
};
