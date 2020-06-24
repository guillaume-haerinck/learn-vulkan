#include "memory-allocator.h"

#include "graphics/ressources/buffer.h"

MemoryAllocator::MemoryAllocator(PhysicalDevice& physicalDevice, LogicalDevice& device)
	: m_physicalDevice(physicalDevice), m_device(device)
{
	
}

MemoryAllocator::~MemoryAllocator()
{
}

void MemoryAllocator::allocateAndBindBuffer(IBuffer& buffer)
{
	// TODO allocate big chunk and split it for the buffer instead
	// Memory handling (may use the AMD openGPU lib to handle this in the future)
	// https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
	vk::MemoryRequirements memoryRequirements = m_device.get().getBufferMemoryRequirements(buffer.getBuffer().get());

	unsigned int memoryTypeIndex = PhysicalDevice::findMemoryType(
		m_physicalDevice.get().getMemoryProperties(),
		memoryRequirements.memoryTypeBits,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
	);

	m_deviceMemory = m_device.get().allocateMemoryUnique(
		vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex)
	);

	// Copy the data to the device memory
	unsigned int* pData = static_cast<unsigned int*>(m_device.get().mapMemory(m_deviceMemory.get(), 0, memoryRequirements.size));
	memcpy(pData, buffer.getData(), buffer.getByteSize());
	m_device.get().unmapMemory(m_deviceMemory.get());

	m_device.get().bindBufferMemory(buffer.getBuffer().get(), m_deviceMemory.get(), 0);
}
