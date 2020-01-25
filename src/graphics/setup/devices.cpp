#include "devices.h"

#include <iostream>
#include <set>
#include <debug_break/debug_break.h>

//#include "graphics/output/swap-chain.h"

//////////////////////////////////////////////////////
////////////////// PHYSICAL DEVICE ///////////////////
//////////////////////////////////////////////////////

PhysicalDevice::PhysicalDevice(Instance& instance, Surface& surface) {
    m_deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    std::vector<vk::PhysicalDevice> devices = instance.get().enumeratePhysicalDevices();
    if (devices.size() == 0) {
        std::cerr << "[PhysicalDevice] Cannot find any graphic card" << std::endl;
        debug_break();
    }

    for (auto& device : devices) {
        if (isDeviceSuitable(device, surface.get())) {
            m_device = device;
            break;
        }
    }
}

PhysicalDevice::~PhysicalDevice() {
}

bool PhysicalDevice::isDeviceSuitable(vk::PhysicalDevice& device, vk::SurfaceKHR& surface) {
    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
    vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();

    findQueueFamilies(device, surface);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = true; // Temp, put to false
    if (extensionsSupported) {
        /*
        SwapChainSupportDetails swapChainSupport = SwapChain::querySwapChainSupport(device, surface.get());
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        */
    }

    return deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu &&
           deviceFeatures.geometryShader &&
           m_queueFamilyIndices.isComplete() &&
           extensionsSupported &&
           swapChainAdequate;
}

bool PhysicalDevice::checkDeviceExtensionSupport(vk::PhysicalDevice& device) {
    auto availableExtensions = device.enumerateDeviceExtensionProperties();
    std::set<std::string> requiredExtensions(m_deviceExtensions.begin(), m_deviceExtensions.end());
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

void PhysicalDevice::findQueueFamilies(vk::PhysicalDevice& device, vk::SurfaceKHR& surface) {
    QueueFamilyIndices indices;
    std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            indices.graphicsFamily = i;
        }

        if (device.getSurfaceSupportKHR(i, surface)) {
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
