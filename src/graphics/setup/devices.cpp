#include "devices.h"

#include <iostream>
#include <debug_break/debug_break.h>

PhysicalDevice::PhysicalDevice(Instance& instance) : m_device(VK_NULL_HANDLE) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance.get(), &deviceCount, nullptr);
    if (deviceCount == 0) {
        std::cerr << "[PhysicalDevice] Cannot find any graphic card" << std::endl;
        debug_break();
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance.get(), &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            m_device = device;
            break;
        }
    }

    if (m_device == VK_NULL_HANDLE) {
        std::cerr << "[PhysicalDevice] Cannot find suitable graphic card" << std::endl;
        debug_break();
    }
}

PhysicalDevice::~PhysicalDevice() {
}

bool PhysicalDevice::isDeviceSuitable(const VkPhysicalDevice& device) {
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    return true;
}

LogicalDevice::LogicalDevice()
{
}

LogicalDevice::~LogicalDevice()
{
}
