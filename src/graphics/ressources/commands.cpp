#include "commands.h"

#include <iostream>
#include <debug_break/debug_break.h>

//////////////////////////////////////////////////////
//////////////////// COMMAND POOL ////////////////////
//////////////////////////////////////////////////////

CommandPool::CommandPool(PhysicalDevice& physicalDevice, LogicalDevice& device) : m_device(device) {
    QueueFamilyIndices queueFamilyIndices = physicalDevice.getQueueFamilyIndices();

    vk::CommandPoolCreateInfo poolInfo(vk::CommandPoolCreateFlags(), queueFamilyIndices.graphicsFamily.value());
    m_commandPool = m_device.get().createCommandPool(poolInfo);
}

CommandPool::~CommandPool() {
    m_device.get().destroyCommandPool(m_commandPool);
}

//////////////////////////////////////////////////////
/////////////////// COMMAND BUFFER ///////////////////
//////////////////////////////////////////////////////

CommandBuffer::CommandBuffer(LogicalDevice& device, CommandPool& commandPool, Pipeline& pipeline, SwapChain& swapChain, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer) {
    m_commandBuffers.resize(pipeline.getFrameBuffers().size());

    vk::CommandBufferAllocateInfo allocInfo(
        commandPool.get(), vk::CommandBufferLevel::ePrimary, (uint32_t) m_commandBuffers.size()
    );
    m_commandBuffers = device.get().allocateCommandBuffers(allocInfo);

    for (size_t i = 0; i < m_commandBuffers.size(); i++) {
        vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eRenderPassContinue, nullptr);
        m_commandBuffers[i].begin(beginInfo);

        vk::ClearValue clearValues[1];
        vk::ClearColorValue clearColor(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        clearValues[0].color = clearColor;
        vk::RenderPassBeginInfo renderPassInfo(
            pipeline.getRenderPass(),
            pipeline.getFrameBuffers()[i],
            vk::Rect2D({ 0, 0 }, swapChain.getExtent()),
            1, clearValues
        );

        m_commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
        m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
        m_commandBuffers[i].bindVertexBuffers(0, vertexBuffer.getBuffer(), { 0 });
        m_commandBuffers[i].bindIndexBuffer(indexBuffer.getBuffer(), 0, vk::IndexType::eUint32);
        m_commandBuffers[i].drawIndexed(indexBuffer.getDataElementCount(), 1, 0, 0, 0);
        m_commandBuffers[i].endRenderPass();
        m_commandBuffers[i].end();
    }
}

CommandBuffer::~CommandBuffer() {

}

