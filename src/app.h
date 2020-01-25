#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "graphics/setup/instance.h"
#include "graphics/setup/devices.h"
#include "graphics/output/surface.h"
/*
#include "graphics/render/pipeline.h"
#include "graphics/render/commands.h"
#include "graphics/render/semaphore.h"
#include "graphics/output/swap-chain.h"
*/

class App {
public:
    App();
    ~App();

    void update();

private:
    void initWindow();
    void drawFrame();

private:
    GLFWwindow* m_window;
    Instance* m_vkInstance;
    Surface* m_surface;
    PhysicalDevice* m_physicalDevice;
    LogicalDevice* m_logicalDevice;
    /*
    SwapChain* m_swapChain;
    Pipeline* m_pipeline;
    CommandPool* m_commandPool;
    CommandBuffer* m_commandBuffer;
    Semaphore* m_semaphore;
    */
};

