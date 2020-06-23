#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>
#include <array>

#include "graphics/setup/devices.h"

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

class VertexBuffer {
public:
    VertexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& device);
    ~VertexBuffer();

    vk::UniqueBuffer& getBuffer();

private:
    LogicalDevice& m_device;
    PhysicalDevice& m_physicalDevice;
    vk::UniqueBuffer m_buffer;
    vk::UniqueDeviceMemory m_deviceMemory;
    std::array<Vertex, 3> m_vertices;
};
