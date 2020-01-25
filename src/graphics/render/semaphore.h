#pragma once

#include <vulkan/vulkan.h>

#include "graphics/setup/devices.h"

/*

class Semaphore {
public:
    Semaphore(LogicalDevice& device);
    ~Semaphore();

    VkSemaphore& getImageAvailable() { return m_imageAvailableSemaphore; }
    VkSemaphore& getRenderFinished() { return m_renderFinishedSemaphore; }

private:
    LogicalDevice& m_device;
    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
};

*/
