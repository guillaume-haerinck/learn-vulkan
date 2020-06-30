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

void DebugReport::checkVkResult(VkResult err)
{
    if (err != VK_SUCCESS)
        std::cerr << "[ImGui] " << vkResultToString(err) << std::endl;
}

std::string DebugReport::vkResultToString(VkResult err)
{
    switch (err) {
#define STR(r) case VK_ ##r: return #r
        STR(NOT_READY);
        STR(TIMEOUT);
        STR(EVENT_SET);
        STR(EVENT_RESET);
        STR(INCOMPLETE);
        STR(ERROR_OUT_OF_HOST_MEMORY);
        STR(ERROR_OUT_OF_DEVICE_MEMORY);
        STR(ERROR_INITIALIZATION_FAILED);
        STR(ERROR_DEVICE_LOST);
        STR(ERROR_MEMORY_MAP_FAILED);
        STR(ERROR_LAYER_NOT_PRESENT);
        STR(ERROR_EXTENSION_NOT_PRESENT);
        STR(ERROR_FEATURE_NOT_PRESENT);
        STR(ERROR_INCOMPATIBLE_DRIVER);
        STR(ERROR_TOO_MANY_OBJECTS);
        STR(ERROR_FORMAT_NOT_SUPPORTED);
        STR(ERROR_SURFACE_LOST_KHR);
        STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
        STR(SUBOPTIMAL_KHR);
        STR(ERROR_OUT_OF_DATE_KHR);
        STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
        STR(ERROR_VALIDATION_FAILED_EXT);
        STR(ERROR_INVALID_SHADER_NV);
        STR(ERROR_OUT_OF_POOL_MEMORY);
#undef STR
    default:
        return "UNKNOWN_ERROR";
    }
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
