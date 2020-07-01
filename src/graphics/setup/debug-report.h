#pragma once

#include <vulkan/vulkan.hpp>
#include <glm/glm.hpp>

#include "instance.h"
#include "devices.h"

// TODO handle name, tag and label with macros to prevent footprint on non-debug builds

class DebugReport {
public:
    DebugReport(Instance& instance, LogicalDevice& device);
    ~DebugReport();

    void setObjectName(vk::ObjectType type, uint64_t objectHandle, const char* name);
    void setTag(vk::ObjectType type, uint64_t objectHandle, uint64_t id, size_t tagSize, const void* pTag);

    void startLabel(const vk::Queue& queue, const char* name, const glm::vec4& color);
    void insertSubLabel(const vk::Queue& queue, const char* name, const glm::vec4& color);
    void endLabel(const vk::Queue& queue);

    void startLabel(const vk::CommandBuffer& command, const char* name, const glm::vec4& color);
    void insertSubLabel(const vk::CommandBuffer& command, const char* name, const glm::vec4& color);
    void endLabel(const vk::CommandBuffer& command);

    // Helper functions

    static std::string vkResultToString(VkResult err);

    template<typename T>
    static uint64_t getVulkanHandle(T const& cppHandle) {
        return uint64_t(static_cast<T::CType>(cppHandle));
    };

    // Callbacks

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

private:
    Instance& m_instance;
    LogicalDevice& m_device;

    vk::DispatchLoaderDynamic m_dldi;
    vk::DebugUtilsMessengerEXT m_validationCallback;
};
