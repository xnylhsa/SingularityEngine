#include "Precompiled.h"
#include "VulkanMemoryObject.h"
#include "VulkanFunctions.h"
#include "VulkanContext.h"
using namespace SingularityEngine::Graphics;

VulkanMemoryObject::VulkanMemoryObject(VkDeviceMemory deviceMemory) : mMemoryObject(deviceMemory)
{

}

bool VulkanMemoryObject::destroy()
{
	if (mMemoryObject != VK_NULL_HANDLE)
	{
		std::shared_ptr<VulkanDevice> vulkanDevice = VulkanContext::GetVulkanDevice();
		vkFreeMemory(*vulkanDevice, mMemoryObject, nullptr);
		mMemoryObject = VK_NULL_HANDLE;
	}
	return true;
}

bool VulkanMemoryObject::MapUpdateAndUnmapHostVisibleMemory(VkDeviceSize offset, VkDeviceSize dataSize, void* data, bool unmap, void** pointer)
{
	std::shared_ptr<VulkanDevice> vulkanDevice = VulkanContext::GetVulkanDevice();

	VkResult result;
	void* localPointer = nullptr;
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
