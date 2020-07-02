#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

#include "graphics/setup/instance.h"

class Surface {
public:
    Surface(Instance& instance, GLFWwindow* window);
    ~Surface();

    vk::SurfaceKHR& get() { return m_surface; }

private:
    vk::SurfaceKHR m_surface;
    Instance& m_instance;
};
