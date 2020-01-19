#pragma once

#include <vulkan/vulkan.h>
#include "instance.h"

class PhysicalDevice {
public:
    PhysicalDevice(Instance& instance);
    ~PhysicalDevice();

private:
    bool isDeviceSuitable(const VkPhysicalDevice& device);

private:
    VkPhysicalDevice m_device;
};

class LogicalDevice {
public:
    LogicalDevice();
    ~LogicalDevice();

private:
};
