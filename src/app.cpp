#include "app.h"

#include <iostream>
#include <debug_break/debug_break.h>
#include <vulkan/vulkan.hpp>
#include <instrumentor/intrumentor.h>

InputAction App::m_inputs;

App::App() {
    PROFILE_BEGIN_SESSION("Learn Vulkan", "learn-vulkan-profiling.json");
    PROFILE_SCOPE("Init application");

    initWindow();

    glfwSetMouseButtonCallback(m_window, processMouseInputs);
    glfwSetCursorPosCallback(m_window, processMousePos);
    glfwSetScrollCallback(m_window, processMouseScroll);

    Model model = m_loader.loadModelFromFile("res/Vulkan-Samples-Assets/scenes/teapot.gltf");

    try {
        m_vkInstance = new Instance;
        m_surface = new Surface(*m_vkInstance, m_window);
        m_physicalDevice = new PhysicalDevice(*m_vkInstance, *m_surface);
        m_logicalDevice = new LogicalDevice(*m_physicalDevice);
        m_debugReport = new DebugReport(*m_vkInstance, *m_logicalDevice);
        m_memoryAllocator = new MemoryAllocator(*m_physicalDevice, *m_logicalDevice);
        m_swapChain = new SwapChain(*m_physicalDevice, *m_logicalDevice, *m_surface);
        m_vertexBuffer = new VertexBuffer(*m_logicalDevice, *m_memoryAllocator, model.vertices);
        m_indexBuffer = new IndexBuffer(*m_logicalDevice, *m_memoryAllocator, model);
        m_descriptorPool = new DescriptorPool(*m_logicalDevice);
        m_texture = new TextureImageView(*m_logicalDevice, *m_memoryAllocator, "res/temp.jpg");
        m_pipeline = new Pipeline(*m_logicalDevice, *m_swapChain, *m_descriptorPool, *m_memoryAllocator);
        m_commandPool = new CommandPool(*m_physicalDevice, *m_logicalDevice);
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

    initImgui();
}

App::~App() {
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete m_semaphore;
    delete m_commandPool;
    delete m_pipeline;
    delete m_descriptorPool;
    delete m_swapChain;
    delete m_vertexBuffer;
    delete m_indexBuffer;
    delete m_texture;
    delete m_memoryAllocator;
    delete m_logicalDevice;
    delete m_physicalDevice;
    delete m_surface;
    delete m_debugReport;
    delete m_vkInstance;
    glfwDestroyWindow(m_window);
    glfwTerminate();
    PROFILE_END_SESSION();
}

void App::initWindow() {
    PROFILE_SCOPE("Init window");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr); // TODO set size elsewhere
}

void App::initImgui()
{
    PROFILE_SCOPE("Init imgui");
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForVulkan(m_window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = m_vkInstance->get();
    init_info.PhysicalDevice = m_physicalDevice->get();
    init_info.Device = m_logicalDevice->get();
    init_info.QueueFamily = m_logicalDevice->getGraphicFamilyIndex();
    init_info.Queue = m_logicalDevice->getGraphicQueue();
    init_info.DescriptorPool = m_descriptorPool->get(); // FIXME
    init_info.MinImageCount = 2;
    init_info.ImageCount = m_swapChain->getImageViews().size();
    init_info.PipelineCache = nullptr;
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = &DebugReport::checkVkResult;
    
    ImGui_ImplVulkan_Init(&init_info, m_pipeline->getRenderPass());

    // Upload Fonts
    {
        vk::CommandBufferAllocateInfo allocInfo(
            m_commandPool->get(), 
            vk::CommandBufferLevel::ePrimary, 
            1
        );
        auto commandBuffer = m_logicalDevice->get().allocateCommandBuffers(allocInfo).at(0);
        
        vk::CommandBufferBeginInfo beginInfo(
            vk::CommandBufferUsageFlagBits::eOneTimeSubmit, 
            nullptr
        );
        commandBuffer.begin(beginInfo);
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        commandBuffer.end();

        vk::SubmitInfo submitInfo(
            0, nullptr, // waitSemaphores
            nullptr, // waitDstStageMask
            1, &commandBuffer, // commandBuffers
            0, nullptr // signalSemaphores
        );
        m_logicalDevice->getPresentQueue().submit(submitInfo, nullptr);

        m_logicalDevice->getPresentQueue().waitIdle();
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
}

void App::update() {
    try {
        while(!glfwWindowShouldClose(m_window)) {
            PROFILE_SCOPE("App Main loop");

            glfwPollEvents();
            if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
                m_camera.update(m_inputs);

            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Temp
            ImGui::Begin("Hello, world!");
                ImGui::Text("This is some useful text.");
            ImGui::End();
            ImGui::ShowDemoWindow();

            ImGui::EndFrame();
            ImGui::Render();
            drawFrame();
            resetInputs();
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
    PROFILE_SCOPE("App draw frame");
    uint32_t imageIndex;
    m_logicalDevice->get().acquireNextImageKHR(m_swapChain->get(), UINT64_MAX, m_semaphore->getImageAvailable(), nullptr, &imageIndex);

    vk::Semaphore signalSemaphores[] = { m_semaphore->getRenderFinished() };
    vk::Semaphore waitSemaphores[] = { m_semaphore->getImageAvailable() };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

    m_pipeline->updateUniformBuffer(imageIndex, m_camera.getViewProj());

    // TODO not that great to recreate command buffers each frame, only do it if there are changes
    CommandBuffer commandBuffer(*m_logicalDevice, *m_commandPool, *m_pipeline, *m_swapChain, *m_vertexBuffer, *m_indexBuffer, ImGui::GetDrawData());
    m_debugReport->startLabel(commandBuffer.get().at(imageIndex), "Draw ImGui and Teapot", glm::vec4(1, 1, 0, 1));
    m_debugReport->setObjectName(
        vk::ObjectType::eCommandBuffer, 
        DebugReport::getVulkanHandle(commandBuffer.get().at(imageIndex)), 
        "Main Command Buffer"
    );

    vk::SubmitInfo submitInfo(
        1, waitSemaphores, waitStages,
        1, &commandBuffer.get()[imageIndex],
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
    m_debugReport->endLabel(commandBuffer.get().at(imageIndex));

    m_logicalDevice->getPresentQueue().waitIdle();
}

void App::processMouseInputs(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        m_inputs.camOrbit = true;
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        m_inputs.camOrbit = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        m_inputs.camPan = true;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        m_inputs.camPan = false;

    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
        m_inputs.camPan = true;
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
        m_inputs.camPan = false;
}

void App::processMousePos(GLFWwindow* window, double xpos, double ypos)
{
    m_inputs.mousePosDelta.x = m_inputs.mousePos.x - xpos;
    m_inputs.mousePosDelta.y = m_inputs.mousePos.y - ypos;
    m_inputs.mousePos = glm::vec2(xpos, ypos);
}

void App::processMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    m_inputs.mouseWheelDelta = yoffset;
    m_inputs.camDolly = true;
}

void App::resetInputs()
{
    m_inputs.camDolly = false;
    m_inputs.mousePosDelta = glm::vec2(0);
    m_inputs.mouseWheelDelta = 0;
}
