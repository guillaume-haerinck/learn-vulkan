#include "app.h"

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
}

App::~App() {
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
    }
}
