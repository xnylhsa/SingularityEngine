#pragma once
#include "Common.h"

namespace SingularityEngine::Vulkan
{
	class MemoryObject
	{
	public:
		bool destroy(VkDevice& device);
		bool createImageMemoryObject(VkPhysicalDevice& physicalDevice, VkDevice& device, VkMemoryPropertyFlags memoryProperties, VkImage& image);
		bool createBufferMemoryObject(VkPhysicalDevice& physicalDevice, VkDevice& device, VkMemoryPropertyFlags memoryProperties, VkBuffer& vkbuffer);
		bool MapUpdateAndUnmapHostVisibleMemory(VkDevice device, VkDeviceSize offset, VkDeviceSize dataSize, void* data, bool unmap = true, void** pointer = nullptr);
		bool isValid() { return mMemoryObject != VK_NULL_HANDLE; }
	private:
		VkDeviceMemory mMemoryObject;
	};
}