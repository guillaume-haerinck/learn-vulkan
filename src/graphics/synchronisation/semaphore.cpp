#include "semaphore.h"

#include <iostream>
#include <debug_break/debug_break.h>

Semaphore::Semaphore(LogicalDevice& device) : m_device(device) {
    m_imageAvailableSemaphore = m_device.get().createSemaphore(vk::SemaphoreCreateInfo());
    m_renderFinishedSemaphore = m_device.get().createSemaphore(vk::SemaphoreCreateInfo());
}

Semaphore::~Semaphore() {
    m_device.get().destroySemaphore(m_renderFinishedSemaphore);
    m_device.get().destroySemaphore(m_imageAvailableSemaphore);
}
