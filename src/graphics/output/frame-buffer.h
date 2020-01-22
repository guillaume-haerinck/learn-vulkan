#pragma once

#include <vulkan/vulkan.h>

#include "graphics/setup/devices.h"
#include "graphics/output/swap-chain.h"
#include "graphics/render/pipeline.h"

class FrameBuffer {
public:
    FrameBuffer(LogicalDevice& device, SwapChain& swapChain, Pipeline& pipeline);
    ~FrameBuffer();

private:
    LogicalDevice& m_device;
    std::vector<VkFramebuffer> swapChainFramebuffers;
};
