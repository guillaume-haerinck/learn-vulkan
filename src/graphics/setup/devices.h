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

    VkDevice& get() { return m_device; }
    VkQueue& getGraphicQueue() { return m_graphicsQueue; }
    VkQueue& getPresentQueue() { return m_presentQueue; }

private:
    VkDevice m_device;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
};
