#include "commands.h"

#include <iostream>
#include <debug_break/debug_break.h>

//////////////////////////////////////////////////////
//////////////////// COMMAND POOL ////////////////////
//////////////////////////////////////////////////////

CommandPool::CommandPool(PhysicalDevice& physicalDevice, LogicalDevice& device) : m_device(device) {
    QueueFamilyIndices queueFamilyIndices = physicalDevice.getQueueFamilyIndices();

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(device.get(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
        std::cerr << "[CommandPool] Failed to create" << std::endl;
        debug_break();
    }
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool(m_device.get(), m_commandPool, nullptr);
}


//////////////////////////////////////////////////////
/////////////////// COMMAND BUFFER ///////////////////
//////////////////////////////////////////////////////

CommandBuffer::CommandBuffer(LogicalDevice& device, CommandPool& commandPool, Pipeline& pipeline) {
    m_commandBuffers.resize(pipeline.getFrameBuffers().size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.get();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) m_commandBuffers.size();

    if (vkAllocateCommandBuffers(device.get(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
        std::cerr << "[CommandBuffer] Failed to create" << std::endl;
        debug_break();
    }

    for (size_t i = 0; i < m_commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            std::cerr << "[CommandBuffer] Failed to begin recording" << std::endl;
            debug_break();
        }
    }
}

CommandBuffer::~CommandBuffer() {

}
