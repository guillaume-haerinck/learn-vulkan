#include "devices.h"

#include <iostream>
#include <set>
#include <debug_break/debug_break.h>

#include "graphics/output/swap-chain.h"

//////////////////////////////////////////////////////
////////////////// PHYSICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

PhysicalDevice::PhysicalDevice(Instance& instance, Surface& surface) : m_device(VK_NULL_HANDLE) {
    m_deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    
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

    findQueueFamilies(device, surface);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = SwapChain::querySwapChainSupport(device, surface.get());
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader &&
           m_queueFamilyIndices.isComplete() &&
           extensionsSupported &&
           swapChainAdequate;
}

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice& device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void PhysicalDevice::findQueueFamilies(VkPhysicalDevice& device, Surface& surface) {
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

    m_queueFamilyIndices = indices;
}

//////////////////////////////////////////////////////
/////////////////// LOGICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

LogicalDevice::LogicalDevice(PhysicalDevice& physicalDevice) {
    QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo createInfo = {};
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice.getDeviceExtensions().size());
        createInfo.ppEnabledExtensionNames = physicalDevice.getDeviceExtensions().data();
        // enabledLayerCount and ppEnabledLayerNames fields are ignored by up-to-date implementations
    }

    if (vkCreateDevice(physicalDevice.get(), &createInfo, nullptr, &m_device) != VK_SUCCESS) {
        std::cerr << "[LogicalDevice] Failed to create" << std::endl;
        debug_break();
    }

    vkGetDeviceQueue(m_device, indices.graphicsFamily.value(), 0, &m_graphicsQueue);
    vkGetDeviceQueue(m_device, indices.presentFamily.value(), 0, &m_presentQueue);
}

LogicalDevice::~LogicalDevice() {
    vkDestroyDevice(m_device, nullptr);
}
