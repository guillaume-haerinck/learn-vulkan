#include "app.h"

#include <iostream>
#include <debug_break/debug_break.h>

App::App() {
    initWindow();
    m_vkInstance = new Instance;
    m_surface = new Surface(*m_vkInstance, m_window);
    m_physicalDevice = new PhysicalDevice(*m_vkInstance, *m_surface);
    m_logicalDevice = new LogicalDevice(*m_physicalDevice);
    m_swapChain = new SwapChain(*m_physicalDevice, *m_logicalDevice, *m_surface);
    m_pipeline = new Pipeline(*m_logicalDevice, *m_swapChain);
    m_commandPool = new CommandPool(*m_physicalDevice, *m_logicalDevice);
    m_commandBuffer = new CommandBuffer(*m_logicalDevice, *m_commandPool, *m_pipeline, *m_swapChain);
    m_semaphore = new Semaphore(*m_logicalDevice);
}

App::~App() {
    delete m_semaphore;
    delete m_commandBuffer;
    delete m_commandPool;
    delete m_pipeline;
    delete m_swapChain;
    delete m_logicalDevice;
    delete m_physicalDevice;
    delete m_surface;
    delete m_vkInstance;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void App::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr); // TODO set size elsewhere
}

void App::update() {
    while(!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(m_logicalDevice->get());
}

void App::drawFrame() {
    uint32_t imageIndex;
    vkAcquireNextImageKHR(m_logicalDevice->get(), m_swapChain->get(), UINT64_MAX, m_semaphore->getImageAvailable(), VK_NULL_HANDLE, &imageIndex);

    VkSemaphore signalSemaphores[] = { m_semaphore->getRenderFinished() };

    VkSubmitInfo submitInfo = {};
    {
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        VkSemaphore waitSemaphores[] = { m_semaphore->getImageAvailable() };
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffer->get()[imageIndex];
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
    }

    if (vkQueueSubmit(m_logicalDevice->getGraphicQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        std::cerr << "[App] Failed to submit draw command buffer" << std::endl;
        debug_break();
    }

    VkPresentInfoKHR presentInfo = {};
    {
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_swapChain->get() };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
    }

    vkQueuePresentKHR(m_logicalDevice->getPresentQueue(), &presentInfo);

    vkQueueWaitIdle(m_logicalDevice->getPresentQueue());
}
