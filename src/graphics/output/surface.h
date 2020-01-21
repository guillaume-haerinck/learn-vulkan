#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "graphics/setup/instance.h"

class Surface {
public:
    Surface(Instance& instance, GLFWwindow* window);
    ~Surface();

    VkSurfaceKHR& get() { return m_surface; }

private:
    VkSurfaceKHR m_surface;
    Instance& m_instance;
};
