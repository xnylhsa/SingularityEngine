#include "Precompiled.h"
#include "Vulkan/Memory/VulkanMemoryObject.h"
#include "Vulkan/Util/VulkanFunctions.h"
#include "Vulkan/Device/VulkanDevice.h"
using namespace SingularityEngine::SERenderer;

VulkanMemoryObject::VulkanMemoryObject(std::weak_ptr<VulkanDevice> device, VkDeviceMemory deviceMemory) : mMemoryObject(deviceMemory), mDevice(device)
{
	
}

bool VulkanMemoryObject::destroy()
{
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	if (mMemoryObject != VK_NULL_HANDLE)
	{
		vkFreeMemory(*vulkanDevice, mMemoryObject, nullptr);
		mMemoryObject = VK_NULL_HANDLE;
	}
	return true;
}

bool VulkanMemoryObject::MapUpdateAndUnmapHostVisibleMemory(VkDeviceSize offset, VkDeviceSize dataSize, void* data, bool unmap, void** pointer)
{

	VkResult result;
	void* localPointer = nullptr;
	auto vulkanDevice = mDevice.lock();
	ASSERT(vulkanDevice, "[SERenderer::VulkanImage] device lost.");
	result = vkMapMemory(*vulkanDevice, mMemoryObject, offset, dataSize, 0, &localPointer);
	if (VK_SUCCESS != result) {
		LOG("[Graphics::Memory] Could not map memory from device object!");
		return false;
	}
	std::memcpy(localPointer, data, dataSize);

	std::vector<VkMappedMemoryRange> memoryRanges = {
		{
			VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
			nullptr,
			mMemoryObject,
			offset,
			dataSize
		}
	};
	result = vkFlushMappedMemoryRanges(*vulkanDevice, static_cast<uint32_t>(memoryRanges.size()), &memoryRanges[0]);
	if (VK_SUCCESS != result)
	{
		LOG("[Graphics::Memory] Failed to flush and update memory!");
		return false;
	}

	if (unmap)
	{
		vkUnmapMemory(*vulkanDevice, mMemoryObject);
	}
	else if (pointer)
	{
		*pointer = localPointer;
	}
	return true;
}
