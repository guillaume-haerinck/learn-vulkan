#pragma once

#include <vulkan/vulkan.h>

#include "graphics/setup/devices.h"
#include "pipeline.h"

//////////////////////////////////////////////////////
//////////////////// COMMAND POOL ////////////////////
//////////////////////////////////////////////////////

class CommandPool {
public:
    CommandPool(PhysicalDevice& physicalDevice, LogicalDevice& device);
    ~CommandPool();

    VkCommandPool& get() { return m_commandPool; }

private:
    LogicalDevice& m_device;
    VkCommandPool m_commandPool;
};


//////////////////////////////////////////////////////
/////////////////// COMMAND BUFFER ///////////////////
//////////////////////////////////////////////////////

class CommandBuffer {
public:
    CommandBuffer(LogicalDevice& device, CommandPool& commandPool, Pipeline& pipeline);
    ~CommandBuffer();

private:
    std::vector<VkCommandBuffer> m_commandBuffers;
};
