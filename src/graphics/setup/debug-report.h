#pragma once

#include <vulkan/vulkan.hpp>
#include "instance.h"

class DebugReport {
public:
    DebugReport(Instance& instance);
    ~DebugReport();

    /*
     * @brief New extension to handle error
     * @link https://www.lunarg.com/wp-content/uploads/2018/05/Vulkan-Debug-Utils_05_18_v1.pdf
     */
    static VKAPI_ATTR VkBool32 VKAPI_CALL validationCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    );

    static void checkVkResult(VkResult err);

    static std::string vkResultToString(VkResult err);

private:
    Instance& m_instance;

    VkDebugUtilsMessengerEXT m_validationCallback;
    PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;
};
