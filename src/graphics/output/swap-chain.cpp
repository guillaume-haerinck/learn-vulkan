#include "swap-chain.h"

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <debug_break/debug_break.h>

SwapChain::SwapChain(PhysicalDevice& physicalDevice, LogicalDevice& device, Surface& surface) : m_device(device) {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice.get(), surface.get());
    vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    QueueFamilyIndices indices = physicalDevice.getQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    vk::SwapchainCreateInfoKHR createInfo(
        vk::SwapchainCreateFlagsKHR(),
        surface.get(),
        imageCount,
        surfaceFormat.format,
        surfaceFormat.colorSpace,
        extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive,
        0,
        nullptr,
        swapChainSupport.capabilities.currentTransform,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        presentMode,
        true,
        nullptr
    );

    if (indices.graphicsFamily != indices.presentFamily) {
        // If the graphics and present queues are from different queue families, we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode as VK_SHARING_MODE_CONCURRENT
        createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    m_swapChain = device.get().createSwapchainKHR(createInfo);
    m_swapChainImages = device.get().getSwapchainImagesKHR(m_swapChain);
    m_swapChainExtent = extent;
    m_swapChainImageFormat = surfaceFormat.format;

    createImageViews();
}

SwapChain::~SwapChain() {
    m_device.get().destroySwapchainKHR(m_swapChain);
    for (auto imageView : m_swapChainImageViews) {
        m_device.get().destroyImageView(imageView);
    }
}

SwapChainSupportDetails SwapChain::querySwapChainSupport(vk::PhysicalDevice& physicalDevice, vk::SurfaceKHR& surface) {
    SwapChainSupportDetails details;
    details.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
    details.formats = physicalDevice.getSurfaceFormatsKHR(surface);
    details.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
    return details;
}

vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

vk::PresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        }
    }
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    } else {
        // TODO get size from elsewhere
        const int WIDTH = 800;
        const int HEIGHT = 600;

        vk::Extent2D actualExtent = { WIDTH, HEIGHT };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        
        return actualExtent;
    }
}

void SwapChain::createImageViews() {
    m_swapChainImageViews.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); i++) {
        vk::ImageViewCreateInfo createInfo(
            vk::ImageViewCreateFlags(),
            m_swapChainImages[i],
            vk::ImageViewType::e2D,
            m_swapChainImageFormat,
            vk::ComponentMapping(vk::ComponentSwizzle::eIdentity),
            vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)
        );
        m_swapChainImageViews[i] = m_device.get().createImageView(createInfo);
    }
}
