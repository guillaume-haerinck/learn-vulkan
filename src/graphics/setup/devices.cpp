#include "devices.h"

#include <iostream>
#include <debug_break/debug_break.h>

//////////////////////////////////////////////////////
////////////////// PHYSICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

PhysicalDevice::PhysicalDevice(Instance& instance, Surface& surface) : m_device(VK_NULL_HANDLE) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance.get(), &deviceCount, nullptr);
    if (deviceCount == 0) {
        std::cerr << "[PhysicalDevice] Cannot find any graphic card" << std::endl;
        debug_break();
    }
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance.get(), &deviceCount, devices.data());

    for (auto& device : devices) {
        if (isDeviceSuitable(device, surface)) {
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

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice& device, Surface& surface) {
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    QueueFamilyIndices indices = findQueueFamilies(device, surface);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader &&
           indices.isComplete();
}

QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice& device, Surface& surface) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.get(), &presentSupport);
        if (presentSupport) {
            indices.presentFamily = i;
        }

        if (indices.isComplete()) {
            break;
        }
        i++;
    }

    return indices;
}

//////////////////////////////////////////////////////
/////////////////// LOGICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

LogicalDevice::LogicalDevice(PhysicalDevice& physicalDevice) {
    QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    {
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;
    }
    
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        // enabledLayerCount and ppEnabledLayerNames fields are ignored by up-to-date implementations
    }

    if (vkCreateDevice(physicalDevice.get(), &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        std::cerr << "[LogicalDevice] Failed to create" << std::endl;
        debug_break();
    }

    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);

    // TODO create the presentation queue 
    // https://vulkan-tutorial.com/en/Drawing_a_triangle/Presentation/Window_surface
}

LogicalDevice::~LogicalDevice() {
    vkDestroyDevice(m_device, nullptr);
}
