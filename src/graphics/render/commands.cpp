#include "commands.h"

#include <imgui_impl_vulkan.h>
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

CommandBufferFactory::CommandBufferFactory(LogicalDevice& device, CommandPool& commandPool)
    : m_device(device), m_commandPool(commandPool)
{}

CommandBufferFactory::~CommandBufferFactory() {

}

std::vector<vk::CommandBuffer> CommandBufferFactory::mainLoop(Pipeline& pipeline, SwapChain& swapChain, VertexBuffer& vertexBuffer, IndexBuffer& indexBuffer, ImDrawData* draw_data)
{
    std::vector<vk::CommandBuffer> commandBuffers;
    commandBuffers.resize(pipeline.getFrameBuffers().size());

    vk::CommandBufferAllocateInfo allocInfo(
        m_commandPool.get(), vk::CommandBufferLevel::ePrimary, (uint32_t)commandBuffers.size()
    );
    commandBuffers = m_device.get().allocateCommandBuffers(allocInfo);

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eRenderPassContinue, nullptr);
        commandBuffers[i].begin(beginInfo);

        vk::ClearValue clearValues[2];
        vk::ClearColorValue clearColor(std::array<float, 4>{0.0f, 0.0f, 0.0f, 1.0f});
        clearValues[0].color = clearColor;
        clearValues[1].depthStencil = { 1, 0 };
        vk::RenderPassBeginInfo renderPassInfo(
            pipeline.getRenderPass(),
            pipeline.getFrameBuffers()[i],
            vk::Rect2D({ 0, 0 }, swapChain.getExtent()),
            2, clearValues
        );

        commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
        commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline.get());
        commandBuffers[i].bindVertexBuffers(0, vertexBuffer.getBuffer(), { 0 });
        commandBuffers[i].bindIndexBuffer(indexBuffer.getBuffer(), 0, vk::IndexType::eUint32);
        commandBuffers[i].bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline.getLayout(), 0, 1, &pipeline.getDescriptorSet(i), 0, nullptr);
        commandBuffers[i].drawIndexed(indexBuffer.getDataElementCount(), 1, 0, 0, 0);
        ImGui_ImplVulkan_RenderDrawData(draw_data, commandBuffers[i]);
        commandBuffers[i].endRenderPass();
        commandBuffers[i].end();
    }

    return commandBuffers;
}

