#include "semaphore.h"

#include <iostream>
#include <debug_break/debug_break.h>

Semaphore::Semaphore(LogicalDevice& device) : m_device(device) {
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
        vkCreateSemaphore(device.get(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS) {

        std::cerr << "[Semaphore] Failed to create" << std::endl;
        debug_break();
    }
}

Semaphore::~Semaphore() {
    vkDestroySemaphore(m_device.get(), m_renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(m_device.get(), m_imageAvailableSemaphore, nullptr);
}
