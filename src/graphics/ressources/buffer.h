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

class VertexInputDescription {
public:
    VertexInputDescription();
    ~VertexInputDescription();

    vk::VertexInputBindingDescription getBindingDescription();
    std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions();

private:
    vk::VertexInputBindingDescription m_description;
    std::array<vk::VertexInputAttributeDescription, 2> m_inputDescription;
};

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

// TODO use a staging buffer in vertex buffer

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
