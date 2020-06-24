#pragma once

#include <vulkan/vulkan.hpp>

#include "graphics/setup/devices.h"

// Forward declaration
class IBuffer;

class MemoryAllocator
{
public:
	MemoryAllocator(PhysicalDevice& physicalDevice, LogicalDevice& device);
	~MemoryAllocator();

	void allocateAndBindBuffer(IBuffer& buffer);

private:
	PhysicalDevice& m_physicalDevice;
	LogicalDevice& m_device;
	vk::UniqueDeviceMemory m_deviceMemory;
};
