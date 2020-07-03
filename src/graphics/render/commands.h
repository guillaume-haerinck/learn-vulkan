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
// TODO put static utility function ?

class CommandBufferFactory {
public:
    CommandBufferFactory(LogicalDevice& device, CommandPool& commandPool);
    ~CommandBufferFactory();


    std::vector<vk::CommandBuffer> mainLoop(Pipeline& pipeline, SwapChain& swapChain, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, ImDrawData* draw_data);
    vk::UniqueCommandBuffer createAndBeginSingleTimeBuffer();

private:
    LogicalDevice& m_device; 
    CommandPool& m_commandPool;
};
