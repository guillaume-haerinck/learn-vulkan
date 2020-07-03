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

vk::UniqueDeviceMemory MemoryAllocator::allocateAndBindBuffer(IBuffer& buffer, unsigned int index, bool copyData) {
	vk::MemoryRequirements memoryRequirements = m_device.get().getBufferMemoryRequirements(buffer.getBuffer(index));

	unsigned int memoryTypeIndex = PhysicalDevice::findMemoryType(
		m_physicalDevice.get().getMemoryProperties(),
		memoryRequirements.memoryTypeBits,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
	);

	vk::UniqueDeviceMemory deviceMemory = m_device.get().allocateMemoryUnique(
		vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex)
	);

	if (copyData) {
		unsigned int* pData = static_cast<unsigned int*>(m_device.get().mapMemory(deviceMemory.get(), 0, memoryRequirements.size));
		memcpy(pData, buffer.getData(), buffer.getByteSize());
		m_device.get().unmapMemory(deviceMemory.get());
	}

	m_device.get().bindBufferMemory(buffer.getBuffer(index), deviceMemory.get(), 0);
	return std::move(deviceMemory); // transfert unique ptr ownership
}

vk::UniqueDeviceMemory MemoryAllocator::allocateAndBindImage(ImageView& image)
{
	vk::MemoryRequirements memoryRequirements = m_device.get().getImageMemoryRequirements(image.getImage());

	unsigned int memoryTypeIndex = PhysicalDevice::findMemoryType(
		m_physicalDevice.get().getMemoryProperties(),
		memoryRequirements.memoryTypeBits,
		vk::MemoryPropertyFlagBits::eDeviceLocal
	);

	vk::UniqueDeviceMemory deviceMemory = m_device.get().allocateMemoryUnique(
		vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex)
	);

	m_device.get().bindImageMemory(image.getImage(), deviceMemory.get(), 0);
	return std::move(deviceMemory); // transfert unique ptr ownership
}

vk::UniqueDeviceMemory MemoryAllocator::allocateAndBindStagingBuffer(vk::Buffer& buffer, unsigned int byteWidth, unsigned char* data) {
	vk::MemoryRequirements memoryRequirements = m_device.get().getBufferMemoryRequirements(buffer);

	unsigned int memoryTypeIndex = PhysicalDevice::findMemoryType(
		m_physicalDevice.get().getMemoryProperties(),
		memoryRequirements.memoryTypeBits,
		vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
	);

	vk::UniqueDeviceMemory deviceMemory = m_device.get().allocateMemoryUnique(
		vk::MemoryAllocateInfo(memoryRequirements.size, memoryTypeIndex)
	);

	unsigned int* pData = static_cast<unsigned int*>(m_device.get().mapMemory(deviceMemory.get(), 0, memoryRequirements.size));
	memcpy(pData, data, byteWidth);
	m_device.get().unmapMemory(deviceMemory.get());

	m_device.get().bindBufferMemory(buffer, deviceMemory.get(), 0);
	return std::move(deviceMemory); // transfert unique ptr ownership
}
