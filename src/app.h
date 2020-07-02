#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "graphics/setup/instance.h"
#include "graphics/setup/devices.h"
#include "graphics/setup/debug-report.h"
#include "graphics/setup/surface.h"
#include "graphics/render/swap-chain.h"
#include "graphics/render/pipeline.h"
#include "graphics/render/commands.h"
#include "graphics/ressources/buffers.h"
#include "graphics/ressources/memory-allocator.h"
#include "graphics/synchronisation/semaphore.h"
#include "controls/arcball-camera.h"
#include "controls/input-action.h"
#include "loaders/gltf-loader.h"

// TODO create and visualize gameobjects into a tree, a scene view

class App {
public:
    App();
    ~App();

    void update();

private:
    void initWindow();
    void initImgui();
    void drawFrame();

    static void processMouseInputs(GLFWwindow* window, int button, int action, int mods);
    static void processMousePos(GLFWwindow* window, double xpos, double ypos);
    static void processMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
    void resetInputs();

private:
    GLFWwindow* m_window;
    Instance* m_vkInstance;
    DebugReport* m_debugReport;
    Surface* m_surface;
    PhysicalDevice* m_physicalDevice;
    LogicalDevice* m_logicalDevice;
    SwapChain* m_swapChain;
    DescriptorPool* m_descriptorPool;
    Pipeline* m_pipeline;
    CommandPool* m_commandPool;
    Semaphore* m_semaphore;
    VertexBuffer* m_vertexBuffer;
    IndexBuffer* m_indexBuffer;
    MemoryAllocator* m_memoryAllocator;
    ArcballCamera m_camera;
    GLTFLoader m_loader;
    static InputAction m_inputs;
};

