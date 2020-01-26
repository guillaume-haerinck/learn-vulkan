#include "debug-report.h"

#include <iostream>
#include <debug_break/debug_break.h>

DebugReport::DebugReport(Instance& instance) : m_instance(instance) {
#ifndef NDEBUG
    pfnVkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(instance.get().getProcAddr("vkCreateDebugReportCallbackEXT"));
    if (!pfnVkCreateDebugReportCallbackEXT) {
        std::cerr << "GetInstanceProcAddr: Unable to find vkCreateDebugReportCallbackEXT function." << std::endl;
        debug_break();
    }

    pfnVkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(instance.get().getProcAddr("vkDestroyDebugReportCallbackEXT"));
    if (!pfnVkDestroyDebugReportCallbackEXT) {
        std::cerr << "GetInstanceProcAddr: Unable to find vkDestroyDebugReportCallbackEXT function." << std::endl;
        debug_break();
    }

    VkDebugReportCallbackCreateInfoEXT callbackCreateInfo;
    callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    callbackCreateInfo.pNext = nullptr;
    callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    callbackCreateInfo.pfnCallback = &DebugReport::debugCallback;
    callbackCreateInfo.pUserData = nullptr;

    if (pfnVkCreateDebugReportCallbackEXT(VkInstance(instance.get()), &callbackCreateInfo, nullptr, &m_debugCallback) != VK_SUCCESS) {
        std::cerr << "[DebugReport] Cannot create debug report callback" << std::endl;
        debug_break();
    }
#endif
}

DebugReport::~DebugReport() {
#ifndef NDEBUG
    pfnVkDestroyDebugReportCallbackEXT(VkInstance(m_instance.get()), m_debugCallback, nullptr);
#endif
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport::debugCallback(
    VkDebugReportFlagsEXT       flags,
    VkDebugReportObjectTypeEXT  objectType,
    uint64_t                    object,
    size_t                      location,
    int32_t                     messageCode,
    const char*                 pLayerPrefix,
    const char*                 pMessage,
    void*                       pUserData)
{
    std::cerr << "[Debug Report] " << pMessage << std::endl;
    return false;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL validationCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    std::cerr << "[Validation Layer] " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}
