#pragma once

#include <vulkan/vulkan.hpp>
#include <imgui.h>

#include "pipeline.h"
#include "graphics/setup/devices.h"
#include "graphics/render/swap-chain.h"
#include "graphics/ressources/buffers.h"

//////////////////////////////////////////////////////
//////////////////// COMMAND POOL ////////////////////
//////////////////////////////////////////////////////

class CommandPool {
public:
    CommandPool(PhysicalDevice& physicalDevice, LogicalDevice& device);
    ~CommandPool();

    vk::CommandPool& get() { return m_commandPool; }

private:
    LogicalDevice& m_device;
    vk::CommandPool m_commandPool;
};


//////////////////////////////////////////////////////
/////////////////// COMMAND BUFFER ///////////////////
//////////////////////////////////////////////////////

// TODO command buffers might not have to be a separate class, might need a renderer class which create commands based on an array of objects etc.

class CommandBuffer {
public:
    CommandBuffer(LogicalDevice& device, CommandPool& commandPool, Pipeline& pipeline, SwapChain& swapChain, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, ImDrawData* draw_data);
    ~CommandBuffer();

    std::vector<vk::CommandBuffer>& get() { return m_commandBuffers; }

private:
    std::vector<vk::CommandBuffer> m_commandBuffers;
};

