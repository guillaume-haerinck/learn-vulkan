#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "graphics/instance.h"

class App {
public:
    App();
    ~App();

    void update();

private:
    void initWindow();

private:
    GLFWwindow* m_window;
    Instance* m_vkInstance;
};

