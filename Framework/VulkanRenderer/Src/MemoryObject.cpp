#include "Precompiled.h"
#include "MemoryObject.h"
#include "VulkanFunctions.h"

using namespace SingularityEngine::Vulkan;

bool MemoryObject::destroy(VkDevice& device)
{
	if (mMemoryObject != VK_NULL_HANDLE)
	{
		vkFreeMemory(device, mMemoryObject, nullptr);
		mMemoryObject = VK_NULL_HANDLE;
	}
	return true;
}

bool MemoryObject::createBufferMemoryObject(VkPhysicalDevice& physicalDevice, VkDevice& device, VkMemoryPropertyFlags memoryProperties, VkBuffer& buffer)
{
	VkPhysicalDeviceMemoryProperties physicalDeviceMemProps;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemProps);

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

	mMemoryObject = VK_NULL_HANDLE;
	for (uint32_t type = 0; type < physicalDeviceMemProps.memoryTypeCount; ++type)
	{
		if ((memoryRequirements.memoryTypeBits & (1 << type)) && (physicalDeviceMemProps.memoryTypes[type].propertyFlags & memoryProperties) == memoryProperties)
		{
			VkMemoryAllocateInfo bufferMemoryAllocateInfo = {
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				memoryRequirements.size,
				type
			};


			if (vkAllocateMemory(device, &bufferMemoryAllocateInfo, nullptr, &mMemoryObject) == VK_SUCCESS)
			{
				break;
			}
		}
	}

	if (mMemoryObject == VK_NULL_HANDLE)
	{
		LOG("[Graphics::Buffer] Could not allocate memory for buffer!");
		return false;
	}

	if (vkBindBufferMemory(device, buffer, mMemoryObject, 0) != VK_SUCCESS)
	{
		LOG("[Graphics::Buffer] Could not bind memory object to buffer!");
		return false;
	}
	return true;
}

bool MemoryObject::createImageMemoryObject(VkPhysicalDevice& physicalDevice, VkDevice& device, VkMemoryPropertyFlags memoryProperties, VkImage& image)
{
	VkPhysicalDeviceMemoryProperties physicalDeviceMemProps;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &physicalDeviceMemProps);

	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements(device, image, &memoryRequirements);

	mMemoryObject = VK_NULL_HANDLE;
	for (uint32_t type = 0; type < physicalDeviceMemProps.memoryTypeCount; ++type)
	{
		if ((memoryRequirements.memoryTypeBits & (1 << type)) && (physicalDeviceMemProps.memoryTypes[type].propertyFlags & memoryProperties) == memoryProperties)
		{
			VkMemoryAllocateInfo bufferMemoryAllocateInfo = {
				VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
				nullptr,
				memoryRequirements.size,
				type
			};


			if (vkAllocateMemory(device, &bufferMemoryAllocateInfo, nullptr, &mMemoryObject) == VK_SUCCESS)
			{
				break;
			}
		}
	}

	if (mMemoryObject == VK_NULL_HANDLE)
	{
		LOG("[Graphics::Buffer] Could not allocate memory for buffer!");
		return false;
	}

	if (vkBindImageMemory(device, image, mMemoryObject, 0) != VK_SUCCESS)
	{
		LOG("[Graphics::Buffer] Could not bind memory object to buffer!");
		return false;
	}

	return true;
}

bool MemoryObject::MapUpdateAndUnmapHostVisibleMemory(VkDevice device, VkDeviceSize offset, VkDeviceSize dataSize, void* data, bool unmap, void** pointer)
{
	VkResult result;
	void* localPointer = nullptr;
	result = vkMapMemory(device, mMemoryObject, offset, dataSize, 0, &localPointer);
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
	result = vkFlushMappedMemoryRanges(device, static_cast<uint32_t>(memoryRanges.size()), &memoryRanges[0]);
	if (VK_SUCCESS != result)
	{
		LOG("[Graphics::Memory] Failed to flush and update memory!");
		return false;
	}

	if (unmap)
	{
		vkUnmapMemory(device, mMemoryObject);
	}
	else if (pointer)
	{
		*pointer = localPointer;
	}
	return true;
}
