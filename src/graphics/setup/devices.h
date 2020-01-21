#pragma once

#include <optional>
#include <vulkan/vulkan.h>
#include "instance.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};

//////////////////////////////////////////////////////
////////////////// PHYSICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

class PhysicalDevice {
public:
    PhysicalDevice(Instance& instance);
    ~PhysicalDevice();

    VkPhysicalDevice& get() { return m_device; }
    QueueFamilyIndices getQueueFamilyIndices() { return findQueueFamilies(m_device); }

private:
    bool isDeviceSuitable(VkPhysicalDevice& device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& device);

private:
    VkPhysicalDevice m_device;
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
};
