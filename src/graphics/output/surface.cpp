#include "surface.h"

#include <iostream>
#include <debug_break/debug_break.h>

Surface::Surface(Instance& instance, GLFWwindow* window) : m_instance(instance) {
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(instance.get(), window, nullptr, &surface) != VK_SUCCESS) {
        std::cerr << "[Surface] Failed to create" << std::endl;
        debug_break();
    }
    m_surface = surface;
}

Surface::~Surface() {
    m_instance.get().destroySurfaceKHR(m_surface);
}
