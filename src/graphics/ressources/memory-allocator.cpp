#include "memory-allocator.h"

#include "graphics/ressources/buffers.h"
#include "graphics/ressources/images.h"

MemoryAllocator::MemoryAllocator(PhysicalDevice& physicalDevice, LogicalDevice& device)
	: m_physicalDevice(physicalDevice), m_device(device)
{
	
}

MemoryAllocator::~MemoryAllocator()
{
}

vk::UniqueDeviceMemory MemoryAllocator::allocateAndBindBuffer(IBuffer& buffer, unsigned int index, bool copyData)
{
	// TODO allocate big chunk and split it for the buffer instead
	// Memory handling (may use the AMD openGPU lib to handle this in the future)
	// https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
	vk::MemoryRequirements memoryRequirements = m_device.get().getBufferMemoryRequirements(buffer.getBuffer(index));

	unsigned int memoryTypeIndex = PhysicalDevice::findMemoryType(
		m_physicalDevice.get().getMemoryProperties(),
		memoryRequirements.memoryTypeBits,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
	);

	vk::UniqueDeviceMemory deviceMemory = m_device.get().allocateMemoryUnique(
		vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex)
	);

	if (copyData)
	{
		unsigned int* pData = static_cast<unsigned int*>(m_device.get().mapMemory(deviceMemory.get(), 0, memoryRequirements.size));
		memcpy(pData, buffer.getData(), buffer.getByteSize());
		m_device.get().unmapMemory(deviceMemory.get());
	}

	m_device.get().bindBufferMemory(buffer.getBuffer(index), deviceMemory.get(), 0);
	return std::move(deviceMemory); // transfert unique ptr ownership
}

vk::UniqueDeviceMemory MemoryAllocator::allocateAndBindImage(ImageView& image)
{
	vk::MemoryRequirements memoryRequirements = m_device.get().getImageMemoryRequirements(image.get());

	unsigned int memoryTypeIndex = PhysicalDevice::findMemoryType(
		m_physicalDevice.get().getMemoryProperties(),
		memoryRequirements.memoryTypeBits,
		vk::MemoryPropertyFlagBits::eDeviceLocal
	); // FIMXE flags might be wrong, not using a staging buffer

	vk::UniqueDeviceMemory deviceMemory = m_device.get().allocateMemoryUnique(
		vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex)
	);
	m_device.get().bindImageMemory(image.get(), deviceMemory.get(), 0);

	return std::move(deviceMemory); // transfert unique ptr ownership
}
