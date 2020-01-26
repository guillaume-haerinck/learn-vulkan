#include "surface.h"

#include <iostream>
#include <debug_break/debug_break.h>

Surface::Surface(Instance& instance, GLFWwindow* window) : m_instance(instance) {
    auto psurf = VkSurfaceKHR(m_surface);
    if (glfwCreateWindowSurface(VkInstance(instance.get()), window, nullptr, &psurf) != VK_SUCCESS) {
        std::cerr << "[Surface] Failed to create" << std::endl;
        debug_break();
    }
    m_surface = vk::SurfaceKHR(psurf);
}

Surface::~Surface() {
    m_instance.get().destroySurfaceKHR(m_surface);
}
