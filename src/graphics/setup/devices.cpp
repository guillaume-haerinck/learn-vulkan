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
    std::vector<vk::ExtensionProperties> availableExtensions = device.enumerateDeviceExtensionProperties();
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

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo(
            vk::DeviceQueueCreateFlags(),
            queueFamily,
            1,
            &queuePriority
        );
        queueCreateInfos.push_back(queueCreateInfo);
    }
    
    vk::PhysicalDeviceFeatures deviceFeatures = {};

    // TODO might need to handle layer count etc now at 0
    vk::DeviceCreateInfo createInfo(
        vk::DeviceCreateFlags(),
        static_cast<uint32_t>(queueCreateInfos.size()),
        queueCreateInfos.data(),
        0,
        nullptr,
        static_cast<uint32_t>(physicalDevice.getDeviceExtensions().size()),
        physicalDevice.getDeviceExtensions().data(),
        &deviceFeatures
    );

    m_device = physicalDevice.get().createDevice(createInfo);

    m_graphicsQueue = m_device.getQueue(indices.graphicsFamily.value(), 0);
    m_presentQueue = m_device.getQueue(indices.presentFamily.value(), 0);
}

LogicalDevice::~LogicalDevice() {
    m_device.destroy();
}
