#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

#include "graphics/setup/devices.h"
#include "surface.h"

struct SwapChainSupportDetails {
    vk::SurfaceCapabilitiesKHR capabilities;
    std::vector<vk::SurfaceFormatKHR> formats;
    std::vector<vk::PresentModeKHR> presentModes;
};

// FIXME c++ api on this class stops the triangle from being rendered

class SwapChain {
public:
    SwapChain(PhysicalDevice& physicalDevice, LogicalDevice& device, Surface& surface);
    ~SwapChain();

    static SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface);
    
    vk::SwapchainKHR& get() { return m_swapChain; }
    vk::Extent2D& getExtent() { return m_swapChainExtent; }
    vk::Format& getImageFormat() { return m_swapChainImageFormat; }
    std::vector<vk::ImageView>& getImageViews() { return m_swapChainImageViews; }
    

private:
    vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
    vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
    vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);

    void createImageViews();

private:
    LogicalDevice& m_device;
    vk::SwapchainKHR m_swapChain;
    std::vector<vk::Image> m_swapChainImages;
    std::vector<vk::ImageView> m_swapChainImageViews;
    vk::Format m_swapChainImageFormat;
    vk::Extent2D m_swapChainExtent;
};
