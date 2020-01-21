#include "surface.h"

#include <iostream>
#include <debug_break/debug_break.h>

Surface::Surface(Instance& instance, GLFWwindow* window) : m_instance(instance) {
    if (glfwCreateWindowSurface(instance.get(), window, nullptr, &m_surface) != VK_SUCCESS) {
        std::cerr << "[Surface] Failed to create" << std::endl;
        debug_break();
    }
}

Surface::~Surface() {
    vkDestroySurfaceKHR(m_instance.get(), m_surface, nullptr);
}
