#include "debug-report.h"

#include <iostream>
#include <sstream>
#include <rang.hpp>
#include <debug_break/debug_break.h>

DebugReport::DebugReport(Instance& instance) : m_instance(instance) {
#ifndef NDEBUG
    pfnVkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.get().getProcAddr("vkCreateDebugUtilsMessengerEXT"));
    if (!pfnVkCreateDebugUtilsMessengerEXT)
        std::cerr << "GetInstanceProcAddr: Unable to find pfnVkCreateDebugUtilsMessengerEXT function." << std::endl;

    pfnVkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.get().getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
    if (!pfnVkDestroyDebugUtilsMessengerEXT)
        std::cerr << "GetInstanceProcAddr: Unable to find pfnVkDestroyDebugUtilsMessengerEXT function." << std::endl;

    // Use of the c API because Vulkan Hpp need dynamic function loading for extensions
    VkDebugUtilsMessengerCreateInfoEXT info = {};
    info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info.pNext = nullptr;
    info.flags = 0; // reserved for future use
    info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    info.pfnUserCallback = &DebugReport::validationCallback;
    info.pUserData = nullptr;

    if (pfnVkCreateDebugUtilsMessengerEXT(VkInstance(instance.get()), &info, nullptr, &m_validationCallback) != VK_SUCCESS) {
        std::cerr << "[DebugReport] Cannot create debug report callback" << std::endl;
        debug_break();
    }
#endif
}

DebugReport::~DebugReport() {
#ifndef NDEBUG
    pfnVkDestroyDebugUtilsMessengerEXT(VkInstance(m_instance.get()), m_validationCallback, nullptr);
#endif
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugReport::validationCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData)
{
    std::cerr << rang::style::bold;

    switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT: std::cerr << rang::fg::red; break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT: std::cerr << rang::fg::yellow; break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: std::cerr << rang::fg::green; break;
    default: break;
    }

    std::cerr << "[" << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>(messageSeverity)) << " ";
    std::cerr << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(messageTypes)) << "]\n";
    std::cerr << rang::fg::reset << rang::style::reset;
    std::cerr << pCallbackData->pMessage << "\n";
    std::cerr << rang::fg::gray << "messageIDName   = " << rang::fg::reset;
    std::cerr << pCallbackData->pMessageIdName << "\n";
    std::cerr << rang::fg::gray << "messageIDNumber = " << rang::fg::reset;
    std::cerr << pCallbackData->messageIdNumber << "\n";

    if (0 < pCallbackData->queueLabelCount) {
        std::cerr << rang::fg::gray << "Queue Labels:\n";
        for (uint8_t i = 0; i < pCallbackData->queueLabelCount; i++)
            std::cerr << rang::fg::gray << "\t" << "labelName = " << rang::fg::reset << pCallbackData->pQueueLabels[i].pLabelName << "\n";
    }

    if (0 < pCallbackData->cmdBufLabelCount) {
        std::cerr << rang::fg::gray << "CommandBuffer Labels:\n";
        for (uint8_t i = 0; i < pCallbackData->cmdBufLabelCount; i++)
            std::cerr << rang::fg::gray << "\t" << "labelName = " << rang::fg::reset << pCallbackData->pCmdBufLabels[i].pLabelName << "\n";
    }

    if (0 < pCallbackData->objectCount) {
        std::cerr << rang::fg::gray << "Objects:\n";

        for (unsigned int i = 0; i < pCallbackData->objectCount; i++) {
            std::cerr << rang::fg::gray << " " << i << ": \n";
            std::cerr << rang::fg::gray << "  " << "objectType   = " << rang::fg::reset << vk::to_string(static_cast<vk::ObjectType>(pCallbackData->pObjects[i].objectType)) << "\n";
            std::cerr << rang::fg::gray << "  " << "objectHandle = " << rang::fg::reset << pCallbackData->pObjects[i].objectHandle << "\n";

            if (pCallbackData->pObjects[i].pObjectName)
                std::cerr << rang::fg::gray << "  " << "objectName   = " << rang::fg::reset << pCallbackData->pObjects[i].pObjectName << "\n";
        }
    }

    return VK_FALSE;
}

void DebugReport::checkVkResult(VkResult err) {
    if (err != VK_SUCCESS)
        std::cerr << rang::fg::red << "[ImGui] " << rang::fg::reset << vkResultToString(err) << std::endl;
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
