#pragma once

#include <vulkan/vulkan.h>

#include "graphics/setup/devices.h"

class Semaphore {
public:
    Semaphore(LogicalDevice& device);
    ~Semaphore();

    vk::Semaphore& getImageAvailable() { return m_imageAvailableSemaphore; }
    vk::Semaphore& getRenderFinished() { return m_renderFinishedSemaphore; }

private:
    LogicalDevice& m_device;
    vk::Semaphore m_imageAvailableSemaphore;
    vk::Semaphore m_renderFinishedSemaphore;
};

