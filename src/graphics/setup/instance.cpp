#include "instance.h"

#include <GLFW/glfw3.h>
#include <debug_break/debug_break.h>
#include <iostream>

Instance::Instance() {
    VkApplicationInfo appInfo = {};
    {
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_1;
    }
    
    VkInstanceCreateInfo createInfo = {};
    {
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;
    }

#ifndef NDEBUG
    const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    createInfo.ppEnabledLayerNames = validationLayers.data();
    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());

    if (!doesEveryValidationLayersExists(validationLayers, availableLayers)) {
        std::cerr << "[Instance] Some validation layer is missing" << std::endl;
        debug_break();
    }
#endif

    if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
        std::cerr << "[Instance] Cannot create instance !" << std::endl;
        debug_break();
    }
}

Instance::~Instance() {
    vkDestroyInstance(m_instance, nullptr);
}

bool Instance::doesEveryValidationLayersExists(const std::vector<const char*>& validationLayers, const std::vector<VkLayerProperties>& availableLayers) const {
    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            return false;
        }
    }

    return true;
}
