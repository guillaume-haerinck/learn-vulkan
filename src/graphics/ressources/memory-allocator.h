#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

#include "graphics/setup/devices.h"

// Forward declaration
class IBuffer;
class ImageView;
class TextureImageView;

// TODO allocate big chunk and split it for the buffer instead
// Memory handling (may use the AMD openGPU lib to handle this in the future)
// https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator

// TODO better understanding of staging buffers

// TODO check design, split only the allocation part might hurt code lisibility. Stuff like "createBuffer" with a few arguments might be a better approach

class MemoryAllocator {
public:
	MemoryAllocator(PhysicalDevice& physicalDevice, LogicalDevice& device);
	~MemoryAllocator();

	vk::UniqueDeviceMemory allocateAndBindBuffer(IBuffer& buffer, unsigned int index = 0, bool copyData = true);
	vk::UniqueDeviceMemory allocateAndBindImage(ImageView& image);

	vk::UniqueDeviceMemory allocateStagingBuffer(vk::Buffer& buffer, unsigned int byteWidth, unsigned char* data);

private:
	PhysicalDevice& m_physicalDevice;
	LogicalDevice& m_device;
};
