#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "graphics/setup/devices.h"
#include "surface.h"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class SwapChain {
public:
    SwapChain(PhysicalDevice& physicalDevice, LogicalDevice& device, Surface& surface);
    ~SwapChain();

    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& physicalDevice, VkSurfaceKHR& surface);
    
    VkSwapchainKHR& get() { return m_swapChain; }
    VkExtent2D& getExtent() { return m_swapChainExtent; }
    VkFormat& getImageFormat() { return m_swapChainImageFormat; }
    std::vector<VkImageView>& getImageViews() { return m_swapChainImageViews; }
    

private:
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    void createImageViews();

private:
    LogicalDevice& m_device;
    VkSwapchainKHR m_swapChain;
    std::vector<VkImage> m_swapChainImages;
    std::vector<VkImageView> m_swapChainImageViews;
    VkFormat m_swapChainImageFormat;
    VkExtent2D m_swapChainExtent;
};
