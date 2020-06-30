#pragma once

#include <vulkan/vulkan.hpp>
#include "instance.h"

class DebugReport {
public:
    DebugReport(Instance& instance);
    ~DebugReport();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugReportFlagsEXT       flags,
        VkDebugReportObjectTypeEXT  objectType,
        uint64_t                    object,
        size_t                      location,
        int32_t                     messageCode,
        const char*                 pLayerPrefix,
        const char*                 pMessage,
        void*                       pUserData
    );

    static VKAPI_ATTR VkBool32 VKAPI_CALL validationCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData
    );

    static void checkVkResult(VkResult err);

    static std::string vkResultToString(VkResult err);

private:
    Instance& m_instance;

    VkDebugReportCallbackEXT m_debugCallback;
    PFN_vkCreateDebugReportCallbackEXT pfnVkCreateDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT pfnVkDestroyDebugReportCallbackEXT;

    // Not used as default already output to cout
    VkDebugUtilsMessengerEXT m_validationCallback;
    PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerEXT;
};
