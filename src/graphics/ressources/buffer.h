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
    
    vk::UniqueBuffer& getBuffer() { return m_buffer; }
    void allocateAndBindBuffer() { m_memoryAllocator.allocateAndBindBuffer(*this); }

    virtual void* getData() = 0;
    virtual size_t getByteSize() = 0;
    virtual size_t getDataElementCount() = 0;

protected:
    LogicalDevice& m_device;
    MemoryAllocator& m_memoryAllocator;
    vk::UniqueBuffer m_buffer;
};

/**
 * @brief Geometry data
 */
class VertexBuffer : public IBuffer {
public:
    VertexBuffer(LogicalDevice& device, PhysicalDevice& physicalDevice, MemoryAllocator& memoryAllocator);
    ~VertexBuffer() {}

    
    size_t getDataElementCount() override { return m_vertices.size(); }
    void* getData() override { return m_vertices.data(); }
    size_t getByteSize() override { return sizeof(m_vertices); }

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

    size_t getDataElementCount() override { return m_indices.size(); }
    void* getData() override { return m_indices.data(); }
    size_t getByteSize() override { return sizeof(m_indices); }

private:
    std::array<unsigned int, 6> m_indices;
};

/**
 * @brief Uniform buffer updated once each frame
 */
struct PerFrameUB
{
    glm::mat4 world;
    glm::mat4 viewProj;
};

class UniformBuffer {
public:
    UniformBuffer();
    ~UniformBuffer();

private:

};
