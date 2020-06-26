#include "app.h"

#include <iostream>
#include <debug_break/debug_break.h>
#include <vulkan/vulkan.hpp>

InputAction App::m_inputs;

App::App() {
    initWindow();

    glfwSetMouseButtonCallback(m_window, processMouseInputs);
    glfwSetCursorPosCallback(m_window, processMousePos);

    try {
        m_vkInstance = new Instance;
        m_debugReport = new DebugReport(*m_vkInstance);
        m_surface = new Surface(*m_vkInstance, m_window);
        m_physicalDevice = new PhysicalDevice(*m_vkInstance, *m_surface);
        m_logicalDevice = new LogicalDevice(*m_physicalDevice);
        m_memoryAllocator = new MemoryAllocator(*m_physicalDevice, *m_logicalDevice);
        m_swapChain = new SwapChain(*m_physicalDevice, *m_logicalDevice, *m_surface);
        m_vertexBuffer = new VertexBuffer(*m_logicalDevice, *m_memoryAllocator);
        m_indexBuffer = new IndexBuffer(*m_logicalDevice, *m_memoryAllocator);
        m_descriptorPool = new DescriptorPool(*m_logicalDevice, m_swapChain->getImageViews().size());
        m_pipeline = new Pipeline(*m_logicalDevice, *m_swapChain, *m_descriptorPool, *m_memoryAllocator);
        m_commandPool = new CommandPool(*m_physicalDevice, *m_logicalDevice);
        m_commandBuffer = new CommandBuffer(*m_logicalDevice, *m_commandPool, *m_pipeline, *m_swapChain, *m_vertexBuffer, *m_indexBuffer);
        m_semaphore = new Semaphore(*m_logicalDevice);

    } catch (vk::SystemError e) {
        std::cerr << "vk::SystemError: " << e.what() << std::endl;
        debug_break();
    } catch (const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
        debug_break();
    } catch (...) {
        std::cerr << "unknown error\n";
        debug_break();
    }
}

App::~App() {
    delete m_semaphore;
    delete m_commandBuffer;
    delete m_commandPool;
    delete m_pipeline;
    delete m_descriptorPool;
    delete m_swapChain;
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete m_memoryAllocator;
    delete m_logicalDevice;
    delete m_physicalDevice;
    delete m_surface;
    delete m_debugReport;
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
    try {
        while(!glfwWindowShouldClose(m_window)) {
            glfwPollEvents();
            m_camera.update(m_inputs);
            drawFrame();
        }
        m_logicalDevice->get().waitIdle();
    } catch (vk::SystemError e) {
        std::cerr << "vk::SystemError: " << e.what() << std::endl;
        debug_break();
    } catch (const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
        debug_break();
    } catch (...) {
        std::cerr << "unknown error\n";
        debug_break();
    }
}

void App::drawFrame() {
    uint32_t imageIndex;
    m_logicalDevice->get().acquireNextImageKHR(m_swapChain->get(), UINT64_MAX, m_semaphore->getImageAvailable(), nullptr, &imageIndex);

    vk::Semaphore signalSemaphores[] = { m_semaphore->getRenderFinished() };
    vk::Semaphore waitSemaphores[] = { m_semaphore->getImageAvailable() };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

    m_pipeline->updateUniformBuffer(imageIndex, m_camera.getViewProj());

    vk::SubmitInfo submitInfo(
        1, waitSemaphores, waitStages,
        1, &m_commandBuffer->get()[imageIndex],
        1, signalSemaphores
    );
    m_logicalDevice->getGraphicQueue().submit(submitInfo, nullptr);

    vk::SwapchainKHR swapChains[] = { m_swapChain->get() };
    vk::PresentInfoKHR presentInfo(
        1, signalSemaphores,
        1, swapChains,
        &imageIndex, nullptr
    );
    m_logicalDevice->getPresentQueue().presentKHR(presentInfo);

    m_logicalDevice->getPresentQueue().waitIdle();
}

void App::processMouseInputs(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        m_inputs.camOrbit = true;
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        m_inputs.camOrbit = false;
}

void App::processMousePos(GLFWwindow* window, double xpos, double ypos)
{
    m_inputs.mousePosDelta.x = m_inputs.mousePos.x - xpos;
    m_inputs.mousePosDelta.y = m_inputs.mousePos.y - ypos;
    m_inputs.mousePos = glm::vec2(xpos, ypos);
}
