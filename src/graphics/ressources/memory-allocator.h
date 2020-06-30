#pragma once

#include <vulkan/vulkan.hpp>
#include <vector>

#include "graphics/setup/devices.h"

// Forward declaration
class IBuffer;
class ImageView;

class MemoryAllocator
{
public:
	MemoryAllocator(PhysicalDevice& physicalDevice, LogicalDevice& device);
	~MemoryAllocator();

	vk::UniqueDeviceMemory allocateAndBindBuffer(IBuffer& buffer, unsigned int index = 0, bool copyData = true);
	vk::UniqueDeviceMemory allocateAndBindImage(ImageView& image);

private:
	PhysicalDevice& m_physicalDevice;
	LogicalDevice& m_device;
};
