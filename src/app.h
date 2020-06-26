#pragma once

#include <GLFW/glfw3.h>

#include "graphics/setup/instance.h"
#include "graphics/setup/devices.h"
#include "graphics/setup/debug-report.h"
#include "graphics/output/surface.h"
#include "graphics/output/swap-chain.h"
#include "graphics/ressources/pipeline.h"
#include "graphics/ressources/commands.h"
#include "graphics/ressources/buffers.h"
#include "graphics/ressources/memory-allocator.h"
#include "graphics/synchronisation/semaphore.h"
#include "controls/arcball-camera.h"
#include "controls/input-action.h"

class App {
public:
    App();
    ~App();

    void update();

private:
    void initWindow();
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
    CommandBuffer* m_commandBuffer;
    Semaphore* m_semaphore;
    VertexBuffer* m_vertexBuffer;
    IndexBuffer* m_indexBuffer;
    MemoryAllocator* m_memoryAllocator;
    ArcballCamera m_camera;
    static InputAction m_inputs;
};

