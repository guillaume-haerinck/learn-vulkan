#pragma once

#include <optional>
#include <vulkan/vulkan.hpp>

#include "instance.h"
#include "graphics/output/surface.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

//////////////////////////////////////////////////////
////////////////// PHYSICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

class PhysicalDevice {
public:
    PhysicalDevice(Instance& instance, Surface& surface);
    ~PhysicalDevice();

    vk::PhysicalDevice& get() { return m_device; }
    QueueFamilyIndices& getQueueFamilyIndices() { return m_queueFamilyIndices; }
    const std::vector<const char*>& getDeviceExtensions() const { return m_deviceExtensions; }

    static unsigned int findMemoryType(vk::PhysicalDeviceMemoryProperties const& memoryProperties, uint32_t typeBits, vk::MemoryPropertyFlags requirementsMask);

private:
    bool isDeviceSuitable(vk::PhysicalDevice& device, vk::SurfaceKHR& surface);
    bool checkDeviceExtensionSupport(vk::PhysicalDevice& device);
    void findQueueFamilies(vk::PhysicalDevice& device, vk::SurfaceKHR& surface);

private:
    vk::PhysicalDevice m_device;
    QueueFamilyIndices m_queueFamilyIndices;
    std::vector<const char*> m_deviceExtensions;
};

//////////////////////////////////////////////////////
/////////////////// LOGICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

class LogicalDevice {
public:
    LogicalDevice(PhysicalDevice& physicalDevice);
    ~LogicalDevice();

    vk::Device& get() { return m_device; }
    vk::Queue& getGraphicQueue() { return m_graphicsQueue; }
    vk::Queue& getPresentQueue() { return m_presentQueue; }
    unsigned int getGraphicFamilyIndex() { return m_graphicFamilyIndex; }

private:
    vk::Device m_device;
    vk::Queue m_graphicsQueue;
    vk::Queue m_presentQueue;
    unsigned int m_graphicFamilyIndex;
};
