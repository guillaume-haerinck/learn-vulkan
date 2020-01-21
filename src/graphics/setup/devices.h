#pragma once

#include <optional>
#include <vulkan/vulkan.h>

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

    VkPhysicalDevice& get() { return m_device; }
    QueueFamilyIndices& getQueueFamilyIndices() { return m_queueFamilyIndices; }

private:
    bool isDeviceSuitable(VkPhysicalDevice& device, Surface& surface);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device, Surface& surface);

private:
    VkPhysicalDevice m_device;
    QueueFamilyIndices m_queueFamilyIndices;
};

//////////////////////////////////////////////////////
/////////////////// LOGICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

class LogicalDevice {
public:
    LogicalDevice(PhysicalDevice& physicalDevice);
    ~LogicalDevice();

private:
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
};
