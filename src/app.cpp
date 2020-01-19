#include "app.h"

App::App() {
    initWindow();
    m_vkInstance = new Instance;
}

App::~App() {
    delete m_vkInstance;
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void App::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    m_window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
}

void App::update() {
    while(!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}
